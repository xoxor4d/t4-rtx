#include "std_include.hpp"
using namespace game::sp;

namespace components::sp
{
	struct unpacked_model_vert
	{
		game::vec3_t pos;
		game::vec3_t normal;
		game::vec2_t texcoord;
	};

	constexpr auto MODEL_VERTEX_STRIDE = 32u;
	constexpr auto MODEL_VERTEX_FORMAT = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);

	// #

	struct unpacked_world_vert
	{
		game::vec3_t pos;
		game::vec3_t normal;
		unsigned int color;
		game::vec2_t texcoord;
	};

	constexpr auto WORLD_VERTEX_STRIDE = 36u;
	constexpr auto WORLD_VERTEX_FORMAT = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	IDirect3DVertexBuffer9* gfx_world_vertexbuffer = nullptr;


	// *
	// static models (rigid)

	bool XSurfaceOptimize(game::XSurface* surf)
	{
		const auto dev = game::sp::dx->device;
		bool allocated_any = false;

		// setup vertexbuffer
		{
			//XSurfaceOptimizeRigid(model, surf);
			if (!surf->vb0)
			{
				void* vertex_buffer_data = nullptr;
				const auto vertex_bytes = surf->vertCount * MODEL_VERTEX_STRIDE;

				if (auto hr = dev->CreateVertexBuffer(vertex_bytes, 8, 0, D3DPOOL_DEFAULT, &surf->vb0, nullptr);
					hr >= 0)
				{
					if (hr = surf->vb0->Lock(0, 0, &vertex_buffer_data, 0);
						hr >= 0)
					{
						// we need to unpack normal and texcoords in 'GfxPackedVertex' to be able to use them for fixed-function rendering
						for (auto i = 0; i < surf->vertCount; i++)
						{
							// packed source vertex
							const auto src_vert = surf->verts0[i];

							// position of our unpacked vert within the vertex buffer
							const auto v_pos_in_buffer = i * MODEL_VERTEX_STRIDE;
							const auto v = reinterpret_cast<unpacked_model_vert*>(((DWORD)vertex_buffer_data + v_pos_in_buffer));

							// assign pos
							v->pos[0] = src_vert.xyz[0];
							v->pos[1] = src_vert.xyz[1];
							v->pos[2] = src_vert.xyz[2];

							// unpack normal and texcoords
							const auto scale = static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[3])) * (1.0f / 255.0f) + 0.7529412f;
							v->normal[0] = (static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[0])) * (1.0f / 127.0f) + -1.0f) * scale;
							v->normal[1] = (static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[1])) * (1.0f / 127.0f) + -1.0f) * scale;
							v->normal[2] = (static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[2])) * (1.0f / 127.0f) + -1.0f) * scale;

							game::sp::Vec2UnpackTexCoords(src_vert.texCoord.packed, v->texcoord);
						}

						surf->vb0->Unlock();
					}
					else
					{
						surf->vb0->Release();
						surf->vb0 = nullptr;
					}

					allocated_any = true;
				}
			}
		}

		return allocated_any;
	}

	__declspec(naked) void XSurfaceOptimizeRigid_stub()
	{
		const static uint32_t retn_addr = 0x72EF61;
		__asm
		{
			pushad;
			push	edi; // xsurf
			call	XSurfaceOptimize;
			add		esp, 4;
			popad;

			jmp		retn_addr;
		}
	}

	int XModelGetSurfaces(const game::XModel* model, game::XSurface** surfaces, const int submodel)
	{
		*surfaces = &model->surfs[model->lodInfo[submodel].surfIndex];
		return model->lodInfo[submodel].numsurfs;
	}

	void XModelOptimize(const game::XModel* model)
	{
		game::XSurface* surfaces;

		const auto lod_count = model->numLods;
		for (auto lod_index = 0; lod_index < lod_count; ++lod_index)
		{
			const auto surf_count = XModelGetSurfaces(model, &surfaces, lod_index);
			for (auto surf_index = 0; surf_index < surf_count; ++surf_index)
			{
				if (XSurfaceOptimize(&surfaces[surf_index]))
				{
#ifdef DEBUG
					game::sp::Com_PrintMessage(0, utils::va("Allocated buffers for smodel '%s'\n", model->name), 0);
#endif
				}
			}
		}
	}

	/**
	 * @brief builds a world matrix for the mesh
	 */
	void R_DrawStaticModelDrawSurfPlacement(game::GfxCmdBufSourceState* src, const game::GfxStaticModelDrawInst* inst)
	{
		const auto scale = inst->placement.scale;

		// inlined ikMatrixSet44
		src->matrices.matrix[0].m[0][0] = inst->placement.axis[0][0] * scale;
		src->matrices.matrix[0].m[0][1] = inst->placement.axis[0][1] * scale;
		src->matrices.matrix[0].m[0][2] = inst->placement.axis[0][2] * scale;
		src->matrices.matrix[0].m[0][3] = 0.0f;

		src->matrices.matrix[0].m[1][0] = inst->placement.axis[1][0] * scale;
		src->matrices.matrix[0].m[1][1] = inst->placement.axis[1][1] * scale;
		src->matrices.matrix[0].m[1][2] = inst->placement.axis[1][2] * scale;
		src->matrices.matrix[0].m[1][3] = 0.0f;

		src->matrices.matrix[0].m[2][0] = inst->placement.axis[2][0] * scale;
		src->matrices.matrix[0].m[2][1] = inst->placement.axis[2][1] * scale;
		src->matrices.matrix[0].m[2][2] = inst->placement.axis[2][2] * scale;
		src->matrices.matrix[0].m[2][3] = 0.0f;

		src->matrices.matrix[0].m[3][0] = inst->placement.origin[0];
		src->matrices.matrix[0].m[3][1] = inst->placement.origin[1];
		src->matrices.matrix[0].m[3][2] = inst->placement.origin[2];
		src->matrices.matrix[0].m[3][3] = 1.0f;
	}

	/**
	 * @brief completely rewritten R_DrawStaticModelDrawSurfNonOptimized to render static models using the fixed-function pipeline
	 */
	void R_DrawStaticModelDrawSurfNonOptimized(const game::GfxStaticModelDrawStream* drawstream, game::GfxCmdBufSourceState* src, game::GfxCmdBufState* cmd)
	{
		const auto smodel_count = drawstream->smodelCount;
		const auto smodel_list = (const game::GfxStaticModelDrawStream*) reinterpret_cast<const void*>(drawstream->smodelList);
		const auto draw_inst = game::sp::rgp->world->dpvs.smodelDrawInsts;
		const auto dev = game::sp::dx->device;

		// create buffers for all surfaces of the model (including LODs)
		// ^ already done on map load (when 'rtx_warm_smodels' is enabled) but this also covers dynamically spawned models
		for (auto index = 0u; index < smodel_count; index++)
		{
			const auto inst = &draw_inst[*((std::uint16_t*)&smodel_list->primDrawSurfPos + index)];
			XModelOptimize(inst->model);
		}

		// #
		// set streams

		// index and vertex buffers are within the fastfile that includes the model
		//const auto mem = &game::g_zones[static_cast<std::int8_t>(drawstream->localSurf->zoneHandle)].mem;

		// index buffer
		if (cmd->prim.indexBuffer != drawstream->localSurf->indexBuffer)
		{
			cmd->prim.indexBuffer = drawstream->localSurf->indexBuffer;
			dev->SetIndices(drawstream->localSurf->indexBuffer);
		}

		// custom vertexbuffer
		if (drawstream->localSurf->vb0)
		{
			cmd->prim.streams[0].vb = drawstream->localSurf->vb0;
			cmd->prim.streams[0].offset = 0;
			cmd->prim.streams[0].stride = MODEL_VERTEX_STRIDE;
			dev->SetStreamSource(0, drawstream->localSurf->vb0, 0, MODEL_VERTEX_STRIDE);
		}
		else // fallback to shader rendering if there is no custom vertexbuffer
		{
			__debugbreak();
		}

		IDirect3DVertexShader9* og_vertex_shader = nullptr;
		IDirect3DPixelShader9* og_pixel_shader = nullptr;

		{
			// vertex format
			dev->SetFVF(MODEL_VERTEX_FORMAT);

			// save shaders
			dev->GetVertexShader(&og_vertex_shader);
			dev->GetPixelShader(&og_pixel_shader);

			// def. needed or the game will render the mesh using shaders
			dev->SetVertexShader(nullptr);
			dev->SetPixelShader(nullptr);
		}

		// #
		// draw prim

		for (auto index = 0u; index < smodel_count; index++)
		{
			const auto inst = &draw_inst[*((std::uint16_t*)&smodel_list->primDrawSurfPos + index)];

			// transform model into the scene by updating the worldmatrix
			R_DrawStaticModelDrawSurfPlacement(src, inst);
			dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&src->matrices.matrix[0].m));

			// get indexbuffer offset
			const auto offset = 0;

			// draw the prim
			cmd->prim.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, drawstream->localSurf->vertCount, offset, drawstream->localSurf->triCount);
		}

		{
			// #
			// restore everything for following meshes rendered via shaders

			if (og_vertex_shader) dev->SetVertexShader(og_vertex_shader);
			if (og_pixel_shader) dev->SetPixelShader(og_pixel_shader);

			// restore world matrix
			game::sp::R_Set3D(0, src);
			dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&src->matrices.matrix[0].m));

			dev->SetFVF(NULL);
		}
	}

	// *
	// xmodels (rigid/skinned)

	void R_DrawXModelRigidModelSurf(game::GfxModelRigidSurface* model, [[maybe_unused]] game::GfxCmdBufSourceState* source, game::GfxCmdBufState* state)
	{
		const auto dev = game::sp::dx->device;
		const auto surf = model->surf.xsurf;

		if (surf->deformed)
		{
			__debugbreak();
		}

		// build custom vertexbuffer for dynamically spawned models
		if (!surf->vb0)
		{
			if (XSurfaceOptimize(surf) && state->material && state->material->info.name)
			{
#ifdef DEBUG
				game::sp::Com_PrintMessage(0, utils::va("Allocated buffers for smodel using material '%s'\n", state->material->info.name), 0);
#endif
			}
		}

		// #
		// set streams

		// index and vertex buffers are within the fastfile that includes the model
		//const auto mem = &game::g_zones[surf->zoneHandle].mem;

		state->prim.indexBuffer = surf->indexBuffer;
		state->prim.device->SetIndices(surf->indexBuffer);

		const float custom_scalar = 1.0f;

		// skysphere materials need to have sort = sky in assetmanager
		/*if (state->material && state->material->info.sortKey == 5)
		{
			custom_scalar = 1000.0f;
		}*/

		// #
		// build world matrix

		float model_axis[3][3] = {};
		utils::unit_quat_to_axis(model->placement.base.quat, model_axis);

		//const auto mtx = source->matrices.matrix[0].m;
		float mtx[4][4] = {};
		const auto scale = model->placement.scale;

		// inlined ikMatrixSet44
		mtx[0][0] = model_axis[0][0] * scale * custom_scalar;
		mtx[0][1] = model_axis[0][1] * scale * custom_scalar;
		mtx[0][2] = model_axis[0][2] * scale * custom_scalar;
		mtx[0][3] = 0.0f;

		mtx[1][0] = model_axis[1][0] * scale * custom_scalar;
		mtx[1][1] = model_axis[1][1] * scale * custom_scalar;
		mtx[1][2] = model_axis[1][2] * scale * custom_scalar;
		mtx[1][3] = 0.0f;

		mtx[2][0] = model_axis[2][0] * scale * custom_scalar;
		mtx[2][1] = model_axis[2][1] * scale * custom_scalar;
		mtx[2][2] = model_axis[2][2] * scale * custom_scalar;
		mtx[2][3] = 0.0f;

		mtx[3][0] = model->placement.base.origin[0];
		mtx[3][1] = model->placement.base.origin[1];
		mtx[3][2] = model->placement.base.origin[2];
		mtx[3][3] = 1.0f;

		IDirect3DVertexShader9* og_vertex_shader = nullptr;
		IDirect3DPixelShader9* og_pixel_shader = nullptr;

		if (surf->vb0)
		{
			// save shaders
			dev->GetVertexShader(&og_vertex_shader);
			dev->GetPixelShader(&og_pixel_shader);

			// def. needed or the game will render the mesh using shaders
			dev->SetVertexShader(nullptr);
			dev->SetPixelShader(nullptr);

			state->prim.streams[0].vb = surf->vb0;
			state->prim.streams[0].offset = 0;
			state->prim.streams[0].stride = MODEL_VERTEX_STRIDE;
			state->prim.device->SetStreamSource(0, surf->vb0, 0, MODEL_VERTEX_STRIDE);

			// vertex format
			dev->SetFVF(MODEL_VERTEX_FORMAT);

			// set world matrix
			dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&mtx));
		}
		
		// #
		// draw prim

		const auto offset = 0;
		state->prim.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, surf->vertCount, offset, surf->triCount);


		// #
		// restore everything for following meshes rendered via shaders

		{
			if (og_vertex_shader) dev->SetVertexShader(og_vertex_shader);
			if (og_pixel_shader) dev->SetPixelShader(og_pixel_shader);
			dev->SetFVF(NULL);
		}
	}

	__declspec(naked) void R_DrawXModelRigidModelSurf1_stub()
	{
		const static uint32_t retn_addr = 0x74CCB7;
		__asm
		{
			// state pushed
			// source pushed
			push	eax; // GfxModelRigidSurface
			call	R_DrawXModelRigidModelSurf;
			add		esp, 4;
			jmp		retn_addr;
		}
	}

	__declspec(naked) void R_DrawXModelRigidModelSurf2_stub()
	{
		const static uint32_t retn_addr = 0x74D069;
		__asm
		{
			// state pushed
			// source pushed
			push	ecx; // GfxModelRigidSurface
			call	R_DrawXModelRigidModelSurf;
			add		esp, 4;
			jmp		retn_addr;
		}
	}

	// ------------------------

	int R_SetIndexData(game::GfxCmdBufPrimState* state, const unsigned __int16* indices, int tri_count)
	{
		const auto index_count = 3 * tri_count;

		if (index_count + game::sp::gfx_buf->dynamicIndexBuffer->used > game::sp::gfx_buf->dynamicIndexBuffer->total)
		{
			game::sp::gfx_buf->dynamicIndexBuffer->used = 0;
		}

		if (!game::sp::gfx_buf->dynamicIndexBuffer->used)
		{
			game::sp::gfx_buf->dynamicIndexBuffer = game::sp::gfx_buf->dynamicIndexBufferPool;
		}

		void* buffer_data;
		if (const auto hr = game::sp::gfx_buf->dynamicIndexBuffer->buffer->Lock(2 * game::sp::gfx_buf->dynamicIndexBuffer->used, 6 * tri_count, &buffer_data, game::sp::gfx_buf->dynamicIndexBuffer->used != 0 ? 0x1000 : 0x2000);
			hr < 0)
		{
			//R_FatalLockError(hr);
			game::sp::Com_Error(0, "Fatal lock error :: R_SetIndexData");
		}

		memcpy(buffer_data, indices, 2 * index_count);
		game::sp::gfx_buf->dynamicIndexBuffer->buffer->Unlock();

		if (state->indexBuffer != game::sp::gfx_buf->dynamicIndexBuffer->buffer)
		{
			state->indexBuffer = game::sp::gfx_buf->dynamicIndexBuffer->buffer;
			state->device->SetIndices(game::sp::gfx_buf->dynamicIndexBuffer->buffer);
		}

		const auto base_index = game::sp::gfx_buf->dynamicIndexBuffer->used;
		game::sp::gfx_buf->dynamicIndexBuffer->used += index_count;

		return base_index;
	}

	void R_DrawXModelSkinnedUncached(game::GfxModelSkinnedSurface* skinned_surf, game::GfxCmdBufSourceState* src, game::GfxCmdBufState* state)
	{
		// fixed function rendering

		const auto surf = skinned_surf->xsurf;
		const auto start_index = R_SetIndexData(&state->prim, surf->triIndices, surf->triCount);

		if ((int)(MODEL_VERTEX_STRIDE * surf->vertCount + game::sp::gfx_buf->dynamicVertexBuffer->used) > game::sp::gfx_buf->dynamicVertexBuffer->total)
		{
			game::sp::gfx_buf->dynamicVertexBuffer->used = 0;
		}

		// R_SetVertexData
		void* buffer_data;
		if (const auto hr = game::sp::gfx_buf->dynamicVertexBuffer->buffer->Lock(game::sp::gfx_buf->dynamicVertexBuffer->used, MODEL_VERTEX_STRIDE * surf->vertCount, &buffer_data, game::sp::gfx_buf->dynamicVertexBuffer->used != 0 ? 0x1000 : 0x2000);
			hr < 0)
		{
			//R_FatalLockError(hr);
			game::sp::Com_Error(0, "Fatal lock error :: R_DrawXModelSkinnedUncached");
		}

		{
			for (auto i = 0u; i < surf->vertCount; i++)
			{
				// packed source vertex
				const auto src_vert = skinned_surf->u.skinnedVert[i];

				// position of our unpacked vert within the vertex buffer
				const auto v_pos_in_buffer = i * MODEL_VERTEX_STRIDE;
				const auto v = reinterpret_cast<unpacked_model_vert*>(((DWORD)buffer_data + v_pos_in_buffer));

				// vert pos
				v->pos[0] = src_vert.xyz[0];
				v->pos[1] = src_vert.xyz[1];
				v->pos[2] = src_vert.xyz[2];

				// unpack and assign vert normal

				// normal unpacking in a cod4 hlsl shader:
				// temp0	 = i.normal * float4(0.007874016, 0.007874016, 0.007874016, 0.003921569) + float4(-1, -1, -1, 0.7529412);
				// temp0.xyz = temp0.www * temp0;

				const auto scale = static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[3])) * (1.0f / 255.0f) + 0.7529412f;
				v->normal[0] = (static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[0])) * (1.0f / 127.0f) + -1.0f) * scale;
				v->normal[1] = (static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[1])) * (1.0f / 127.0f) + -1.0f) * scale;
				v->normal[2] = (static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[2])) * (1.0f / 127.0f) + -1.0f) * scale;

				// uv's
				game::sp::Vec2UnpackTexCoords(src_vert.texCoord.packed, v->texcoord);
			}

		}

		game::sp::gfx_buf->dynamicVertexBuffer->buffer->Unlock();
		const std::uint32_t vert_offset = game::sp::gfx_buf->dynamicVertexBuffer->used;
		game::sp::gfx_buf->dynamicVertexBuffer->used += (MODEL_VERTEX_STRIDE * surf->vertCount);


		// #
		// #

		if (state->prim.streams[0].vb != game::sp::gfx_buf->dynamicVertexBuffer->buffer || state->prim.streams[0].offset != vert_offset || state->prim.streams[0].stride != MODEL_VERTEX_STRIDE)
		{
			state->prim.streams[0].vb = game::sp::gfx_buf->dynamicVertexBuffer->buffer;
			state->prim.streams[0].offset = vert_offset;
			state->prim.streams[0].stride = MODEL_VERTEX_STRIDE;
			state->prim.device->SetStreamSource(0, game::sp::gfx_buf->dynamicVertexBuffer->buffer, vert_offset, MODEL_VERTEX_STRIDE);
		}

		IDirect3DVertexShader9* og_vertex_shader;
		IDirect3DPixelShader9* og_pixel_shader;

		{
			// save shaders
			state->prim.device->GetVertexShader(&og_vertex_shader);
			state->prim.device->GetPixelShader(&og_pixel_shader);

			// needed or game renders mesh with shaders
			state->prim.device->SetVertexShader(nullptr);
			state->prim.device->SetPixelShader(nullptr);

			// vertex format
			state->prim.device->SetFVF(MODEL_VERTEX_FORMAT);


			// #
			// build world matrix

			float model_axis[3][3] = {};
			utils::unit_quat_to_axis(src->skinnedPlacement.base.quat, model_axis);

			//const auto mtx = source->matrices.matrix[0].m;
			float mtx[4][4] = {};
			const auto scale = src->skinnedPlacement.scale;

			// inlined ikMatrixSet44
			mtx[0][0] = model_axis[0][0] * scale;
			mtx[0][1] = model_axis[0][1] * scale;
			mtx[0][2] = model_axis[0][2] * scale;
			mtx[0][3] = 0.0f;

			mtx[1][0] = model_axis[1][0] * scale;
			mtx[1][1] = model_axis[1][1] * scale;
			mtx[1][2] = model_axis[1][2] * scale;
			mtx[1][3] = 0.0f;

			mtx[2][0] = model_axis[2][0] * scale;
			mtx[2][1] = model_axis[2][1] * scale;
			mtx[2][2] = model_axis[2][2] * scale;
			mtx[2][3] = 0.0f;

			mtx[3][0] = src->skinnedPlacement.base.origin[0];
			mtx[3][1] = src->skinnedPlacement.base.origin[1];
			mtx[3][2] = src->skinnedPlacement.base.origin[2];
			mtx[3][3] = 1.0f;

			// set world matrix
			state->prim.device->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&mtx));
		}

		state->prim.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, surf->vertCount, start_index, surf->triCount);

		{
			state->prim.device->SetFVF(NULL);
			state->prim.device->SetVertexShader(og_vertex_shader);
			state->prim.device->SetPixelShader(og_pixel_shader);
		}
	}

	__declspec(naked) void R_DrawXModelSkinnedUncached_stub()
	{
		const static uint32_t retn_addr = 0x73F195;
		__asm
		{
			// GfxPackedVertex (skinnedVert) pushed (we ignore that because we push GfxModelSkinnedSurface which holds it)
			// state pushed
			// source pushed
			push	edi; // GfxModelSkinnedSurface
			call	R_DrawXModelSkinnedUncached;
			add		esp, 4;
			jmp		retn_addr;
		}
	}

	// *
	// world (bsp/terrain) drawing

	unsigned int R_ReadPrimDrawSurfInt(game::GfxReadCmdBuf* cmdBuf)
	{
		return *cmdBuf->primDrawSurfPos++;
	}

	const unsigned int* R_ReadPrimDrawSurfData(game::GfxReadCmdBuf* cmdBuf, unsigned int count)
	{
		const auto pos = cmdBuf->primDrawSurfPos;
		cmdBuf->primDrawSurfPos += count;

		return pos;
	}

	bool R_ReadBspPreTessDrawSurfs(game::GfxReadCmdBuf* cmdBuf, game::GfxBspPreTessDrawSurf** list, unsigned int* count, unsigned int* baseIndex)
	{
		*count = R_ReadPrimDrawSurfInt(cmdBuf);
		if (!*count)
		{
			return false;
		}

		*baseIndex = R_ReadPrimDrawSurfInt(cmdBuf);
		*list = (game::GfxBspPreTessDrawSurf*)R_ReadPrimDrawSurfData(cmdBuf, *count);

		return true;
	}

#if 0
	int g_layerDataStride[]
	{
		0, 0, 0, 8, 12, 16, 20, 24, 24, 28, 32, 32, 36, 40, 0, 0, 16
	};

	void R_DrawPreTessTrisDecal(game::GfxCmdBufSourceState* src, game::GfxCmdBufPrimState* state, game::srfTriangles_t* tris, unsigned int baseIndex, unsigned int triCount)
	{
		auto v2 = tris[1].vertexLayerData;
		const auto layerDataStride = g_layerDataStride[state->vertDeclType];
		if (layerDataStride)
		{
			if (v2 < 0)
			{
				int x = 0; // meh
			}
			else
			{
				struct fmt
				{
					game::PackedUnitVec T;
					game::PackedUnitVec N;
				};

				// stride of 8
				// MaterialVertexDeclType: VERTDECL_WORLD_T1N0
				// second pair of texcoords and normals/blendweight

				//void* data = {};

				//game::sp::rgp->world->vld.layerVb.data->Lock(tris->vertexLayerData, layerDataStride, &data, 0);
				//auto xx = reinterpret_cast<fmt*>(data);
				//game::sp::rgp->world->vld.layerVb.data->Unlock();

				//const auto scale = static_cast<float>(static_cast<std::uint8_t>(xx->T.array[3])) * (1.0f / 255.0f) + 0.7529412f;
				//auto xx1 = (static_cast<float>(static_cast<std::uint8_t>(xx->T.array[0])) * (1.0f / 127.0f) + -1.0f) * scale;
				//auto xx2 = (static_cast<float>(static_cast<std::uint8_t>(xx->T.array[1])) * (1.0f / 127.0f) + -1.0f) * scale;
				//auto xx3 = (static_cast<float>(static_cast<std::uint8_t>(xx->T.array[2])) * (1.0f / 127.0f) + -1.0f) * scale;

				//// data, offset, stride
				////rgp.world->vld.layerVb.data, a2->vertexLayerData, layerDataStride
				//dev->SetStreamSource(1, game::sp::rgp->world->vld.layerVb.data, tris->vertexLayerData, layerDataStride);
				//state->device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, tris->vertexCount, baseIndex, triCount);

				//dev->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2);
				//state->device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, tris->vertexCount, baseIndex, triCount);
				//dev->SetFVF(WORLD_VERTEX_FORMAT);


				const auto dev = dx->device;

				src->matrices.matrix[0].m[3][0] = 0.0f;
				src->matrices.matrix[0].m[3][1] = 0.0f;
				src->matrices.matrix[0].m[3][2] = 0.0f;
				src->matrices.matrix[0].m[3][3] = 1.0f;
				dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&src->matrices.matrix[0].m));

				// texture alpha + vertex alpha (decal blending)
				dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
				dev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
				dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

				dev->SetStreamSource(0, gfx_world_vertexbuffer, WORLD_VERTEX_STRIDE* tris->firstVertex, WORLD_VERTEX_STRIDE);
				state->device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, tris->vertexCount, baseIndex, triCount);
			}
		}
	}
#endif

	void R_DrawPreTessTris(game::GfxCmdBufSourceState* src, game::GfxCmdBufPrimState* state, game::srfTriangles_t* tris, unsigned int baseIndex, unsigned int triCount)
	{
		const auto dev = dx->device;

		src->matrices.matrix[0].m[3][0] = 0.0f;
		src->matrices.matrix[0].m[3][1] = 0.0f;
		src->matrices.matrix[0].m[3][2] = 0.0f;
		src->matrices.matrix[0].m[3][3] = 1.0f;
		dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&src->matrices.matrix[0].m));

		// texture alpha + vertex alpha (decal blending)
		dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		dev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

		dev->SetStreamSource(0, gfx_world_vertexbuffer, WORLD_VERTEX_STRIDE * tris->firstVertex, WORLD_VERTEX_STRIDE);
		state->device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, tris->vertexCount, baseIndex, triCount);

#if 0	// decal related tests
		auto v2 = tris[1].vertexLayerData;
		const auto layerDataStride = g_layerDataStride[state->vertDeclType];
		if (layerDataStride)
		{
			if (v2 < 0)
			{
				int x = 0; // meh
			}
			else
			{
				struct fmt
				{
					game::PackedUnitVec T;
					game::PackedUnitVec N;
				};

				// stride of 8
				// MaterialVertexDeclType: VERTDECL_WORLD_T1N0
				// second pair of texcoords and normals/blendweight

				//void* data = {};

				//game::sp::rgp->world->vld.layerVb.data->Lock(tris->vertexLayerData, layerDataStride, &data, 0);
				//auto xx = reinterpret_cast<fmt*>(data);
				//game::sp::rgp->world->vld.layerVb.data->Unlock();

				//const auto scale = static_cast<float>(static_cast<std::uint8_t>(xx->T.array[3])) * (1.0f / 255.0f) + 0.7529412f;
				//auto xx1 = (static_cast<float>(static_cast<std::uint8_t>(xx->T.array[0])) * (1.0f / 127.0f) + -1.0f) * scale;
				//auto xx2 = (static_cast<float>(static_cast<std::uint8_t>(xx->T.array[1])) * (1.0f / 127.0f) + -1.0f) * scale;
				//auto xx3 = (static_cast<float>(static_cast<std::uint8_t>(xx->T.array[2])) * (1.0f / 127.0f) + -1.0f) * scale;

				//// data, offset, stride
				////rgp.world->vld.layerVb.data, a2->vertexLayerData, layerDataStride
				//dev->SetStreamSource(1, game::sp::rgp->world->vld.layerVb.data, tris->vertexLayerData, layerDataStride);
				//state->device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, tris->vertexCount, baseIndex, triCount);

				//dev->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2);
				//state->device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, tris->vertexCount, baseIndex, triCount);
				//dev->SetFVF(WORLD_VERTEX_FORMAT);
			}
		}
#endif
	}

#if 0
	game::MaterialTextureDef* R_SetPixelSamplerFromMaterial(game::GfxCmdBufSourceState* src, game::GfxCmdBufState* state, game::MaterialShaderArgument* arg, game::MaterialTextureDef* texDef)
	{
		const char* v3; // eax
		game::GfxImage* image; // [esp+8h] [ebp-Ch] BYREF
		game::Material* material; // [esp+Ch] [ebp-8h]

		material = state->material;
		while (texDef->nameHash != arg->u.codeSampler)
		{
			if (++texDef == &material->textureTable[(unsigned __int8)material->textureCount])
			{
				__debugbreak();
			}
		}

		if (texDef->semantic == 11)
		{
			//image = (game::GfxImage*)texDef->u.image[1].cardMemory.platform[0];
			//R_UploadWaterTexture(texDef->u.water, context->source->materialTime);
		}
		else
		{
			image = texDef->u.image;
		}

		if (texDef->semantic == 1)
		{
			// image @eax
			//R_SetSampler(image, src, state, arg->dest, samplerState);

			int samplerStateInt = texDef->samplerState;
			int dest = arg->dest;
			const static uint32_t R_SetSampler_addr = 0x72B4C0;
			__asm
			{
				pushad;
				push	samplerStateInt;
				push	dest;
				push	state;
				push	src;
				mov		eax, image;
				call	R_SetSampler_addr;
				add		esp, 16;
				popad;
			}
		}

		return texDef;
	}
#endif

	/**
	 * @brief completely rewritten R_DrawBspDrawSurfsPreTess to render bsp surfaces (world) using the fixed-function pipeline
	 */
	void R_DrawBspDrawSurfsPreTess(const unsigned int* primDrawSurfPos, game::GfxCmdBufSourceState* src, game::GfxCmdBufState* state)
	{
		const auto dev = dx->device;

		// #
		// setup fixed-function

		// vertex format
		dev->SetFVF(WORLD_VERTEX_FORMAT);

		// save shaders
		IDirect3DVertexShader9* og_vertex_shader;
		dev->GetVertexShader(&og_vertex_shader);

		IDirect3DPixelShader9* og_pixel_shader;
		dev->GetPixelShader(&og_pixel_shader);

		// def. needed or the game will render the mesh using shaders
		dev->SetVertexShader(nullptr);
		dev->SetPixelShader(nullptr);


		// #
		// draw prims

		unsigned int base_index, count;
		game::GfxBspPreTessDrawSurf* list;
		game::GfxReadCmdBuf cmd_buf = { primDrawSurfPos };

		while (R_ReadBspPreTessDrawSurfs(&cmd_buf, &list, &count, &base_index))
		{
			game::srfTriangles_t* prev_tris = nullptr;
			auto tri_count = 0u;
			auto base_vertex = -1;

			for (auto index = 0u; index < count; ++index)
			{
				const auto surf_index = static_cast<unsigned>(list[index].baseSurfIndex);
				if (surf_index >= static_cast<unsigned>(game::sp::rgp->world->surfaceCount))
				{
					__debugbreak();
					Com_Error(0, "R_DrawBspDrawSurfsPreTess :: surf_index >= static_cast<unsigned>(game::rgp->world->surfaceCount)");
				}

				const auto bsp_surf = &game::sp::rgp->world->dpvs.surfaces[surf_index];
				const auto tris = &bsp_surf->tris;

				if (base_vertex != bsp_surf->tris.firstVertex)
				{
					// never triggers?
					if (tri_count)
					{
						R_DrawPreTessTris(src, &state->prim, prev_tris, base_index, tri_count);
						base_index += 3 * tri_count;
						tri_count = 0;
					}

					prev_tris = tris;
					base_vertex = tris->firstVertex;
				}

				tri_count += list[index].totalTriCount;
			}

			R_DrawPreTessTris(src, &state->prim, prev_tris, base_index, tri_count);
		}

		// #
		// restore everything for following meshes rendered via shaders

		dev->SetVertexShader(og_vertex_shader);
		dev->SetPixelShader(og_pixel_shader);

		// restore world matrix
		game::sp::R_Set3D(0, src);
		dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&src->matrices.matrix[0].m));

		dev->SetFVF(NULL);
	}

	/**
	 * @brief completely rewritten R_TessBModel to render brushmodels using the fixed-function pipeline
	 * - most challenging issue yet
	 */
	std::uint32_t R_TessBModel(game::GfxDrawSurfListArgs* listArgs, [[maybe_unused]] void* x, [[maybe_unused]] void* y)
	{
		const auto source = listArgs->context.source;
		const auto prim = &listArgs->context.state->prim;

		// #
		// setup fixed-function

		// vertex format
		prim->device->SetFVF(WORLD_VERTEX_FORMAT);

		// save shaders
		IDirect3DVertexShader9* og_vertex_shader;
		prim->device->GetVertexShader(&og_vertex_shader);

		IDirect3DPixelShader9* og_pixel_shader;
		prim->device->GetPixelShader(&og_pixel_shader);

		// def. needed or the game will render the mesh using shaders
		prim->device->SetVertexShader(nullptr);
		prim->device->SetPixelShader(nullptr);

		// texture alpha + vertex alpha (decal blending)
		prim->device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		prim->device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		prim->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);


		// #
		// draw prims

		const auto draw_surf_list = &listArgs->info->drawSurfs[listArgs->firstDrawSurfIndex];
		const auto draw_surf_count = listArgs->info->drawSurfCount - listArgs->firstDrawSurfIndex;

		const auto draw_surf_sub_mask = 0xFFFFFFFFE0000000;

		game::GfxDrawSurf draw_surf = {};
		draw_surf.fields = draw_surf_list->fields;

		game::GfxDrawSurf draw_surf_key = {};
		draw_surf_key.packed = draw_surf.packed & draw_surf_sub_mask;

		const std::uint64_t first_surf = draw_surf_key.packed;
		auto draw_surf_index = 0u;

		while (true)
		{
			const auto bsurf = reinterpret_cast<game::BModelSurface*>(&source->data->surfsBuffer[4u * draw_surf.fields.objectId]);

			// #
			// transform

			float model_axis[3][3] = {};

			const auto placement = bsurf->placement;
			utils::unit_quat_to_axis(placement->base.quat, model_axis);
			const auto scale = placement->scale;

			//const auto mtx = source->matrices.matrix[0].m;
			float mtx[4][4] = {};

			// inlined ikMatrixSet44
			mtx[0][0] = model_axis[0][0] * scale;
			mtx[0][1] = model_axis[0][1] * scale;
			mtx[0][2] = model_axis[0][2] * scale;
			mtx[0][3] = 0.0f;

			mtx[1][0] = model_axis[1][0] * scale;
			mtx[1][1] = model_axis[1][1] * scale;
			mtx[1][2] = model_axis[1][2] * scale;
			mtx[1][3] = 0.0f;

			mtx[2][0] = model_axis[2][0] * scale;
			mtx[2][1] = model_axis[2][1] * scale;
			mtx[2][2] = model_axis[2][2] * scale;
			mtx[2][3] = 0.0f;

			mtx[3][0] = placement->base.origin[0];
			mtx[3][1] = placement->base.origin[1];
			mtx[3][2] = placement->base.origin[2];
			mtx[3][3] = 1.0f;

			prim->device->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&mtx));

			// #
			// ------

			const auto gfxsurf = bsurf->surf;
			const auto base_vertex = WORLD_VERTEX_STRIDE * gfxsurf->tris.firstVertex;

			if (prim->streams[0].vb != gfx_world_vertexbuffer || prim->streams[0].offset != base_vertex || prim->streams[0].stride != WORLD_VERTEX_STRIDE)
			{
				prim->streams[0].vb = gfx_world_vertexbuffer;
				prim->streams[0].offset = base_vertex;
				prim->streams[0].stride = WORLD_VERTEX_STRIDE;
				prim->device->SetStreamSource(0, gfx_world_vertexbuffer, base_vertex, WORLD_VERTEX_STRIDE);
			}

			const auto base_index = R_SetIndexData(prim, &game::sp::rgp->world->indices[gfxsurf->tris.baseIndex], gfxsurf->tris.triCount);
			prim->device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, gfxsurf->tris.vertexCount, base_index, gfxsurf->tris.triCount);

			++draw_surf_index;
			if (draw_surf_index == draw_surf_count)
			{
				break;
			}

			draw_surf.fields = draw_surf_list[draw_surf_index].fields;
			draw_surf_key.packed = draw_surf_list[draw_surf_index].packed & draw_surf_sub_mask;

			if (draw_surf_key.packed != first_surf)
			{
				break;
			}
		}

		prim->device->SetVertexShader(og_vertex_shader);
		prim->device->SetPixelShader(og_pixel_shader);
		prim->device->SetFVF(NULL);

		return draw_surf_index;
	}

#if 0
	void R_DrawBspDrawSurfsLitPreTess(const unsigned int* primDrawSurfPos, game::GfxCmdBufSourceState* src, game::GfxCmdBufState* state)
	{
		const auto dev = dx->device;

		// #
		// setup fixed-function

		// vertex format
		dev->SetFVF(WORLD_VERTEX_FORMAT);

		// save shaders
		IDirect3DVertexShader9* og_vertex_shader;
		dev->GetVertexShader(&og_vertex_shader);

		IDirect3DPixelShader9* og_pixel_shader;
		dev->GetPixelShader(&og_pixel_shader);

		// def. needed or the game will render the mesh using shaders
		dev->SetVertexShader(nullptr);
		dev->SetPixelShader(nullptr);


		// #
		// draw prims

		unsigned int base_index, count;
		game::GfxBspPreTessDrawSurf* list;
		game::GfxReadCmdBuf cmd_buf = { primDrawSurfPos };

		while (R_ReadBspPreTessDrawSurfs(&cmd_buf, &list, &count, &base_index))
		{
			for (auto index = 0u; index < count; ++index)
			{
				const auto surf_index = static_cast<unsigned>(list[index].baseSurfIndex);
				if (surf_index >= static_cast<unsigned>(game::sp::rgp->world->surfaceCount))
				{
					__debugbreak();
					Com_Error(0, "R_DrawBspDrawSurfsPreTess :: surf_index >= static_cast<unsigned>(game::rgp->world->surfaceCount)");
				}

				const auto bsp_surf = &game::sp::rgp->world->dpvs.surfaces[surf_index];
				const auto tris = &bsp_surf->tris;
				base_index += 3 * list[index].totalTriCount;
			}
		}

		// #
		// second loop with 'decals'

		// R_SetupPass here

		const auto pass = &state->technique->passArray[0];
		auto argCount = pass->stableArgCount;
		auto arg = &pass->args[(unsigned __int8)pass->perPrimArgCount + (unsigned __int8)pass->perObjArgCount];
		auto texDef = state->material->textureTable;

		pass->args[1];
		pass->args[2];
		pass->args[3];

		/*while (arg->type == 2)
		{
			texDef = R_SetPixelSamplerFromMaterial(src, state, arg++, texDef);
			if (!--argCount)
			{
				return;
			}
		}*/

		for (auto i = 0; i < argCount; i++)
		{
			if (arg->type == 2)
			{
				texDef = R_SetPixelSamplerFromMaterial(src, state, arg, texDef);
			}
			arg++;
		}

		cmd_buf.primDrawSurfPos = primDrawSurfPos;

		while (R_ReadBspPreTessDrawSurfs(&cmd_buf, &list, &count, &base_index))
		{
			for (auto index = 0u; index < count; ++index)
			{
				const auto surf_index = static_cast<unsigned>(list[index].baseSurfIndex);
				if (surf_index >= static_cast<unsigned>(game::sp::rgp->world->surfaceCount))
				{
					__debugbreak();
					Com_Error(0, "R_DrawBspDrawSurfsPreTess :: surf_index >= static_cast<unsigned>(game::rgp->world->surfaceCount)");
				}

				const auto bsp_surf = &game::sp::rgp->world->dpvs.surfaces[surf_index];
				const auto tris = &bsp_surf->tris;

				R_DrawPreTessTrisDecal(src, &state->prim, tris, base_index, list[index].totalTriCount);
				base_index += 3 * list[index].totalTriCount;
			}
		}


		// #
		// restore everything for following meshes rendered via shaders

		dev->SetVertexShader(og_vertex_shader);
		dev->SetPixelShader(og_pixel_shader);

		// restore world matrix
		game::sp::R_Set3D(0, src);
		dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&src->matrices.matrix[0].m));

		dev->SetFVF(NULL);
	}
#endif

	// *
	// build buffers

	void build_gfxworld_buffers()
	{
		const auto dev = dx->device;
		void* vertex_buffer_data = nullptr;

		if (rgp->world)
		{
			if (gfx_world_vertexbuffer)
			{
				gfx_world_vertexbuffer->Release();
				gfx_world_vertexbuffer = nullptr;

				__debugbreak();
				Com_Error(0, "build_gfxworld_buffers :: gfx_world_vertexbuffer != nullptr");
			}

			if (auto hr = dev->CreateVertexBuffer(WORLD_VERTEX_STRIDE * rgp->world->vertexCount, D3DUSAGE_WRITEONLY, WORLD_VERTEX_FORMAT, D3DPOOL_DEFAULT, &gfx_world_vertexbuffer, nullptr);
				hr >= 0)
			{
				if (hr = gfx_world_vertexbuffer->Lock(0, 0, &vertex_buffer_data, 0);
					hr >= 0)
				{
					/*	struct GfxWorldVertex = 44 bytes
					{
						float xyz[3];
						float binormalSign;
						GfxColor color;
						float texCoord[2];
						float lmapCoord[2];
						PackedUnitVec normal;
						PackedUnitVec tangent;
					};*/

					// unpack normal so we can use fixed-function rendering with normals
					for (auto i = 0u; i < rgp->world->vertexCount; i++)
					{
						// packed source vertex
						const auto src_vert = rgp->world->vd.vertices[i];

						// position of our unpacked vert within the vertex buffer
						const auto v_pos_in_buffer = i * WORLD_VERTEX_STRIDE; // pos-xyz ; normal-xyz ; texcoords uv = 32 byte 
						const auto v = reinterpret_cast<unpacked_world_vert*>(((DWORD)vertex_buffer_data + v_pos_in_buffer));

						// vert pos
						v->pos[0] = src_vert.xyz[0];
						v->pos[1] = src_vert.xyz[1];
						v->pos[2] = src_vert.xyz[2];

						// unpack and assign vert normal

						// normal unpacking in a cod4 hlsl shader:
						// temp0	 = i.normal * float4(0.007874016, 0.007874016, 0.007874016, 0.003921569) + float4(-1, -1, -1, 0.7529412);
						// temp0.xyz = temp0.www * temp0;

						const auto scale = static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[3])) * (1.0f / 255.0f) + 0.7529412f;
						v->normal[0] = (static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[0])) * (1.0f / 127.0f) + -1.0f) * scale;
						v->normal[1] = (static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[1])) * (1.0f / 127.0f) + -1.0f) * scale;
						v->normal[2] = (static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[2])) * (1.0f / 127.0f) + -1.0f) * scale;

						// packed vertex color : rgb values are most likely used for 'decal' blending 
						// clear color so textures dont look likethey are covered in unicorn vomit
						auto col = src_vert.color;
						col.array[0] = 0xff;
						col.array[1] = 0xff;
						col.array[2] = 0xff;
						//col.array[3] = 0xff;

						// packed vertex color : used for alpha blending of decals
						v->color = col.packed;

						// uv's
						v->texcoord[0] = src_vert.texCoord[0];
						v->texcoord[1] = src_vert.texCoord[1];
					}

					gfx_world_vertexbuffer->Unlock();
				}
				else
				{
					gfx_world_vertexbuffer->Release();
					gfx_world_vertexbuffer = nullptr;
				}
			}
		}
	}

	// called on map load (cg_init)
	__declspec(naked) void init_fixed_function_buffers_stub()
	{
		const static uint32_t stock_func = 0x629EC0;
		const static uint32_t retn_addr = 0x664F9C;
		__asm
		{
			pushad;
			//call	build_static_model_buffers; // no need - we hook 'XSurfaceOptimize'
			call	build_gfxworld_buffers;
			popad;

			call	stock_func;
			jmp		retn_addr;
		}
	}

	// *
	// cleanup buffers

	void free_fixed_function_buffers()
	{
		// #
		// cleanup world buffer

		if (gfx_world_vertexbuffer)
		{
			gfx_world_vertexbuffer->Release();
			gfx_world_vertexbuffer = nullptr;
		}

		// cleanup model buffer
		// we dont need to cleanup in waw because the game already does that for us
	}

	// called on renderer shutdown (R_Shutdown)
	__declspec(naked) void free_fixed_function_buffers_stub()
	{
		const static uint32_t stock_func = 0x72D980; // R_ResetModelLighting
		const static uint32_t retn_addr = 0x6D6C50;
		__asm
		{
			pushad;
			call	free_fixed_function_buffers;
			popad;

			call	stock_func;
			jmp		retn_addr;
		}
	}

#if 0
	__declspec(naked) void R_DrawBspDrawSurfsLitPreTess_stub()
	{
		const static uint32_t retn_addr = 0x7413F8;
		__asm
		{
			push	ecx;
			call	R_DrawBspDrawSurfsLitPreTess;
			add		esp, 4;
			jmp		retn_addr;
		}
	}
#endif

	

	fixed_function::fixed_function()
	{
		// force fullbright
		utils::hook::nop(0x724AC9, 2); // no decals :[

		// fixed-function rendering of static models (R_TessStaticModelRigidDrawSurfList)
		utils::hook(0x74E1F2, R_DrawStaticModelDrawSurfNonOptimized, HOOK_CALL).install()->quick();

		// fixed-function rendering of rigid xmodels - call 1 (RB_TessXModelRigidDrawSurfList-> R_DrawXModelSurfCamera-> R_DrawXModelRigidModelSurf1
		utils::hook::nop(0x74CC61, 3); // dont overwrite eax with xsurf, wee want GfxModelRigidSurface
		utils::hook(0x74CCB2, R_DrawXModelRigidModelSurf1_stub, HOOK_JUMP).install()->quick();

		// ^ call 2
		utils::hook(0x74D064, R_DrawXModelRigidModelSurf2_stub, HOOK_JUMP).install()->quick();

		// fixed-function rendering of skinned (animated) models (R_TessXModelSkinnedDrawSurfList)
		utils::hook(0x73F190, R_DrawXModelSkinnedUncached_stub, HOOK_JUMP).install()->quick();

		// #
		// fixed-function rendering of world surfaces (R_TessTrianglesPreTessList)
		// :: R_SetStreamsForBspSurface -> R_ClearAllStreamSources -> Stream 1 (world->vld.layerVb) handles 'decals'
		// :: see technique 'lm_r0c0t1c1n1_nc_sm3.tech'

		utils::hook(0x741408, R_DrawBspDrawSurfsPreTess, HOOK_CALL).install()->quick(); // unlit
		//utils::hook(0x7413F3, R_DrawBspDrawSurfsLitPreTess_stub, HOOK_JUMP).install()->quick(); // lit - decal tests

		// fixed-function rendering of brushmodels
		utils::hook(0x741420, R_TessBModel, HOOK_JUMP).install()->quick();

		// TODO :: effects

		// ----

		// on map load :: build custom buffers for fixed-function rendering
		utils::hook(0x664F97, init_fixed_function_buffers_stub, HOOK_JUMP).install()->quick(); // CG_Init :: CG_NorthDirectionChanged call
		utils::hook(0x72EF5C, XSurfaceOptimizeRigid_stub, HOOK_JUMP).install()->quick(); // stock XSurfaceOptimize :: XSurfaceOptimizeRigid call

		// on renderer shutdown :: release custom buffers used by fixed-function rendering
		utils::hook(0x6D6C4B, free_fixed_function_buffers_stub, HOOK_JUMP).install()->quick(); // R_Shutdown :: R_ResetModelLighting call

		command::add("rtx_rebuild_world", [this](command::params)
		{
			gfx_world_vertexbuffer->Release();
			gfx_world_vertexbuffer = nullptr;
			build_gfxworld_buffers();
		});
	}
}
