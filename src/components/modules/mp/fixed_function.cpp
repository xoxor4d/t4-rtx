#include "std_include.hpp"

using namespace game::mp;
namespace components::mp
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
		const auto dev = game::mp::dx->device;
		bool allocated_any = false;

		// setup vertexbuffer
		{
			//XSurfaceOptimizeRigid(model, surf);
			if (surf && !surf->vb0 && surf->vertCount)
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

							game::mp::Vec2UnpackTexCoords(src_vert.texCoord.packed, v->texcoord);
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
		const static uint32_t retn_addr = 0x70CF5B;
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
					game::mp::Com_PrintMessage(0, utils::va("Allocated buffers for smodel '%s'\n", model->name), 0);
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
		const auto draw_inst = game::mp::rgp->world->dpvs.smodelDrawInsts;
		const auto dev = game::mp::dx->device;

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
			//R_DrawStaticModelDrawSurfPlacement(src, inst);

			float mtx[4][4] = {};
			const auto scale = inst->placement.scale;
			mtx[0][0] = inst->placement.axis[0][0] * scale;
			mtx[0][1] = inst->placement.axis[0][1] * scale;
			mtx[0][2] = inst->placement.axis[0][2] * scale;
			mtx[0][3] = 0.0f;

			mtx[1][0] = inst->placement.axis[1][0] * scale;
			mtx[1][1] = inst->placement.axis[1][1] * scale;
			mtx[1][2] = inst->placement.axis[1][2] * scale;
			mtx[1][3] = 0.0f;

			mtx[2][0] = inst->placement.axis[2][0] * scale;
			mtx[2][1] = inst->placement.axis[2][1] * scale;
			mtx[2][2] = inst->placement.axis[2][2] * scale;
			mtx[2][3] = 0.0f;

			mtx[3][0] = inst->placement.origin[0];
			mtx[3][1] = inst->placement.origin[1];
			mtx[3][2] = inst->placement.origin[2];
			mtx[3][3] = 1.0f;

			dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&mtx));

			// get indexbuffer offset
			const auto offset = 0;

			// draw the prim
			cmd->prim.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, drawstream->localSurf->vertCount, offset, drawstream->localSurf->triCount);
		}

		{
			// #
			// restore everything for following meshes rendered via shaders

			// do not restore shaders (fixes a few issues in der rise)
			//if (og_vertex_shader) dev->SetVertexShader(og_vertex_shader);
			//if (og_pixel_shader) dev->SetPixelShader(og_pixel_shader);

			// restore world matrix
			//game::mp::R_Set3D(0, src);
			dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&src->matrices.matrix[0].m));

			dev->SetFVF(NULL);
		}
	}

	// *
	// xmodels (rigid/skinned)

	void R_DrawXModelRigidModelSurf(game::GfxModelRigidSurface* model, [[maybe_unused]] game::GfxCmdBufSourceState* source, game::GfxCmdBufState* state)
	{
		const auto dev = game::mp::dx->device;
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
				game::mp::Com_PrintMessage(0, utils::va("Allocated buffers for smodel using material '%s'\n", state->material->info.name), 0);
#endif
			}
		}

		// #
		// set streams

		// index and vertex buffers are within the fastfile that includes the model
		//const auto mem = &game::g_zones[surf->zoneHandle].mem;

		state->prim.indexBuffer = surf->indexBuffer;
		state->prim.device->SetIndices(surf->indexBuffer);

		float custom_scalar = 1.0f;

		// skysphere materials need to have sort = sky in assetmanager
		if (state->material && state->material->info.sortKey == 5)
		{
			custom_scalar = 1000.0f;

			// disable fog for skysphere
			state->prim.device->SetRenderState(D3DRS_FOGENABLE, FALSE);
		}

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
			//if (og_vertex_shader) dev->SetVertexShader(og_vertex_shader);
			//if (og_pixel_shader) dev->SetPixelShader(og_pixel_shader);
			dev->SetFVF(NULL);
			dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&source->matrices.matrix[0].m));
		}
	}

	__declspec(naked) void R_DrawXModelRigidModelSurf1_stub()
	{
		const static uint32_t retn_addr = 0x724037;
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
		const static uint32_t retn_addr = 0x7243E9;
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

		if (index_count + game::mp::gfx_buf->dynamicIndexBuffer->used > game::mp::gfx_buf->dynamicIndexBuffer->total)
		{
			game::mp::gfx_buf->dynamicIndexBuffer->used = 0;
		}

		if (!game::mp::gfx_buf->dynamicIndexBuffer->used)
		{
			game::mp::gfx_buf->dynamicIndexBuffer = game::mp::gfx_buf->dynamicIndexBufferPool;
		}

		void* buffer_data;
		if (const auto hr = game::mp::gfx_buf->dynamicIndexBuffer->buffer->Lock(2 * game::mp::gfx_buf->dynamicIndexBuffer->used, 6 * tri_count, &buffer_data, game::mp::gfx_buf->dynamicIndexBuffer->used != 0 ? 0x1000 : 0x2000);
			hr < 0)
		{
			//R_FatalLockError(hr);
			game::mp::Com_Error(0, "Fatal lock error :: R_SetIndexData");
		}

		memcpy(buffer_data, indices, 2 * index_count);
		game::mp::gfx_buf->dynamicIndexBuffer->buffer->Unlock();

		if (state->indexBuffer != game::mp::gfx_buf->dynamicIndexBuffer->buffer)
		{
			state->indexBuffer = game::mp::gfx_buf->dynamicIndexBuffer->buffer;
			state->device->SetIndices(game::mp::gfx_buf->dynamicIndexBuffer->buffer);
		}

		const auto base_index = game::mp::gfx_buf->dynamicIndexBuffer->used;
		game::mp::gfx_buf->dynamicIndexBuffer->used += index_count;

		return base_index;
	}

	void R_DrawXModelSkinnedUncached(game::XSurface* surf, game::GfxPackedVertex* verts, game::GfxCmdBufSourceState* src, game::GfxCmdBufState* state)
	{
		// fixed function rendering

		//const auto surf = skinned_surf->xsurf;
		const auto start_index = R_SetIndexData(&state->prim, surf->triIndices, surf->triCount);

		if ((int)(MODEL_VERTEX_STRIDE * surf->vertCount + game::mp::gfx_buf->dynamicVertexBuffer->used) > game::mp::gfx_buf->dynamicVertexBuffer->total)
		{
			game::mp::gfx_buf->dynamicVertexBuffer->used = 0;
		}

		// R_SetVertexData
		void* buffer_data;
		if (const auto hr = game::mp::gfx_buf->dynamicVertexBuffer->buffer->Lock(game::mp::gfx_buf->dynamicVertexBuffer->used, MODEL_VERTEX_STRIDE * surf->vertCount, &buffer_data, game::mp::gfx_buf->dynamicVertexBuffer->used != 0 ? 0x1000 : 0x2000);
			hr < 0)
		{
			//R_FatalLockError(hr);
			game::mp::Com_Error(0, "Fatal lock error :: R_DrawXModelSkinnedUncached");
		}

		{
			for (auto i = 0u; i < surf->vertCount; i++)
			{
				// packed source vertex
				const auto src_vert = verts[i]; //skinned_surf->u.skinnedVert[i];

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
				game::mp::Vec2UnpackTexCoords(src_vert.texCoord.packed, v->texcoord);
			}

		}

		game::mp::gfx_buf->dynamicVertexBuffer->buffer->Unlock();
		const std::uint32_t vert_offset = game::mp::gfx_buf->dynamicVertexBuffer->used;
		game::mp::gfx_buf->dynamicVertexBuffer->used += (MODEL_VERTEX_STRIDE * surf->vertCount);


		// #
		// #

		if (state->prim.streams[0].vb != game::mp::gfx_buf->dynamicVertexBuffer->buffer || state->prim.streams[0].offset != vert_offset || state->prim.streams[0].stride != MODEL_VERTEX_STRIDE)
		{
			state->prim.streams[0].vb = game::mp::gfx_buf->dynamicVertexBuffer->buffer;
			state->prim.streams[0].offset = vert_offset;
			state->prim.streams[0].stride = MODEL_VERTEX_STRIDE;
			state->prim.device->SetStreamSource(0, game::mp::gfx_buf->dynamicVertexBuffer->buffer, vert_offset, MODEL_VERTEX_STRIDE);
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
			//state->prim.device->SetVertexShader(og_vertex_shader);
			//state->prim.device->SetPixelShader(og_pixel_shader);
			state->prim.device->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&src->matrices.matrix[0].m));
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
	}

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

		const auto saved_x = src->matrices.matrix[0].m[3][0];
		const auto saved_y = src->matrices.matrix[0].m[3][0];
		const auto saved_z = src->matrices.matrix[0].m[3][0];

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
				if (surf_index >= static_cast<unsigned>(game::mp::rgp->world->surfaceCount))
				{
					__debugbreak();
					Com_Error(0, "R_DrawBspDrawSurfsPreTess :: surf_index >= static_cast<unsigned>(game::rgp->world->surfaceCount)");
				}

				const auto bsp_surf = &game::mp::rgp->world->dpvs.surfaces[surf_index];
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

		//dev->SetVertexShader(og_vertex_shader);
		//dev->SetPixelShader(og_pixel_shader);
		dev->SetFVF(NULL);

		// restore world matrix
		//game::mp::R_Set3D(0, src);
		src->matrices.matrix[0].m[3][0] = saved_x;
		src->matrices.matrix[0].m[3][1] = saved_y;
		src->matrices.matrix[0].m[3][2] = saved_z;
		dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&src->matrices.matrix[0].m));
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

			const auto base_index = R_SetIndexData(prim, &game::mp::rgp->world->indices[gfxsurf->tris.baseIndex], gfxsurf->tris.triCount);
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

		//prim->device->SetVertexShader(og_vertex_shader);
		//prim->device->SetPixelShader(og_pixel_shader);
		prim->device->SetFVF(NULL);

		return draw_surf_index;
	}



	// *
	// effects

	IDirect3DVertexShader9* _og_codemesh_vertex_shader;
	IDirect3DPixelShader9* _og_codemesh_pixel_shader;
	//constexpr auto MAX_EFFECT_VERTS = 0x4000; // 0x1000
	constexpr auto MAX_EFFECT_VERTS_FOR_HOOK = 0x40; // vertcount for DrawIndexedPrimitive // og: 0x40

	void R_TessCodeMeshList_begin(game::GfxDrawSurfListArgs* listArgs)
	{
		const auto source = listArgs->context.source;
		const auto prim = &listArgs->context.state->prim;

		// #
		// setup fixed-function

		// vertex format
		prim->device->SetFVF(MODEL_VERTEX_FORMAT);

		// save shaders
		//prim->device->GetVertexShader(&_og_codemesh_vertex_shader);
		//prim->device->GetPixelShader(&_og_codemesh_pixel_shader);

		// def. needed or the game will render the mesh using shaders
		prim->device->SetVertexShader(nullptr);
		prim->device->SetPixelShader(nullptr);

		// texture alpha + vertex alpha (decal blending)
		//prim->device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		//prim->device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		//prim->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_BLENDFACTORALPHA);

		// #
		// vertices are already in 'world space' so origin is at 0 0 0

		// inlined ikMatrixSet44
		float mtx[4][4] = {};
		mtx[0][0] = 1.0f; mtx[0][1] = 0.0f; mtx[0][2] = 0.0f; mtx[0][3] = 0.0f;
		mtx[1][0] = 0.0f; mtx[1][1] = 1.0f; mtx[1][2] = 0.0f; mtx[1][3] = 0.0f;
		mtx[2][0] = 0.0f; mtx[2][1] = 0.0f; mtx[2][2] = 1.0f; mtx[2][3] = 0.0f;
		mtx[3][0] = 0.0f; mtx[3][1] = 0.0f; mtx[3][2] = 0.0f; mtx[3][3] = 1.0f;

		prim->device->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&mtx));

		// #
		// unpack codemesh vertex data and place new data into the dynamic vertex buffer

		void* og_buffer_data;
		if (const auto hr = source->data->codeMesh.vb.buffer->Lock(0, source->data->codeMesh.vb.used /*source->data->codeMesh.vertSize * MAX_EFFECT_VERTS*/, &og_buffer_data, D3DLOCK_READONLY);
			hr < 0)
		{
			//R_FatalLockError(hr);
			game::mp::Com_Error(0, "Fatal lock error - codeMesh :: R_TessCodeMeshList_begin");
		}

		if ((int)(source->data->codeMesh.vb.used /*MODEL_VERTEX_STRIDE * MAX_EFFECT_VERTS*/ + game::mp::gfx_buf->dynamicVertexBuffer->used) > game::mp::gfx_buf->dynamicVertexBuffer->total)
		{
			game::mp::gfx_buf->dynamicVertexBuffer->used = 0;
		}

		// R_SetVertexData
		void* buffer_data;
		if (const auto hr = game::mp::gfx_buf->dynamicVertexBuffer->buffer->Lock(
			game::mp::gfx_buf->dynamicVertexBuffer->used, source->data->codeMesh.vb.used /*MODEL_VERTEX_STRIDE * MAX_EFFECT_VERTS*/, &buffer_data,
				game::mp::gfx_buf->dynamicVertexBuffer->used != 0 ? 0x1000 : 0x2000); 
					hr < 0)
		{
			//R_FatalLockError(hr);
			game::mp::Com_Error(0, "Fatal lock error - dynamicVertexBuffer :: R_TessCodeMeshList_begin");
		}

		// #
		// unpack verts

		for (auto i = 0u; i * source->data->codeMesh.vertSize < (unsigned)source->data->codeMesh.vb.used && i < 0x4000; i++)
		{
			// position of vert within the vertex buffer
			const auto v_pos_in_buffer = i * source->data->codeMesh.vertSize; // size of GfxPackedVertex

			/*if (v_pos_in_buffer > source->data->codeMesh.vb.used)
			{
				break;
			}*/

			// interpret GfxPackedVertex as unpacked_model_vert
			const auto v = reinterpret_cast<unpacked_model_vert*>(((DWORD)buffer_data + v_pos_in_buffer));

			// interpret GfxPackedVertex as GfxPackedVertex 
			const auto src_vert = reinterpret_cast<game::GfxPackedVertex*>(((DWORD)og_buffer_data + v_pos_in_buffer));


			// vert pos
			v->pos[0] = src_vert->xyz[0];
			v->pos[1] = src_vert->xyz[1];
			v->pos[2] = src_vert->xyz[2];

			// normal unpacking in a cod4 hlsl shader:
			// temp0	 = i.normal * float4(0.007874016, 0.007874016, 0.007874016, 0.003921569) + float4(-1, -1, -1, 0.7529412);
			// temp0.xyz = temp0.www * temp0;

			const auto scale = static_cast<float>(static_cast<std::uint8_t>(src_vert->normal.array[3])) * (1.0f / 255.0f) + 0.7529412f;
			v->normal[0] = (static_cast<float>(static_cast<std::uint8_t>(src_vert->normal.array[0])) * (1.0f / 127.0f) + -1.0f) * scale;
			v->normal[1] = (static_cast<float>(static_cast<std::uint8_t>(src_vert->normal.array[1])) * (1.0f / 127.0f) + -1.0f) * scale;
			v->normal[2] = (static_cast<float>(static_cast<std::uint8_t>(src_vert->normal.array[2])) * (1.0f / 127.0f) + -1.0f) * scale;

			// uv's
			game::mp::Vec2UnpackTexCoords(src_vert->texCoord.packed, v->texcoord);
		}

		source->data->codeMesh.vb.buffer->Unlock();
		game::mp::gfx_buf->dynamicVertexBuffer->buffer->Unlock();

		const std::uint32_t vert_offset = game::mp::gfx_buf->dynamicVertexBuffer->used;
		game::mp::gfx_buf->dynamicVertexBuffer->used += source->data->codeMesh.vb.used /*(MODEL_VERTEX_STRIDE * MAX_EFFECT_VERTS)*/;

		// #
		// #

		if (prim->streams[0].vb != game::mp::gfx_buf->dynamicVertexBuffer->buffer || prim->streams[0].offset != vert_offset || prim->streams[0].stride != MODEL_VERTEX_STRIDE)
		{
			prim->streams[0].vb = game::mp::gfx_buf->dynamicVertexBuffer->buffer;
			prim->streams[0].offset = vert_offset;
			prim->streams[0].stride = MODEL_VERTEX_STRIDE;
			prim->device->SetStreamSource(0, game::mp::gfx_buf->dynamicVertexBuffer->buffer, vert_offset, MODEL_VERTEX_STRIDE);
		}

		// R_TessCodeMeshList :: game code will now render all codemesh drawsurfs - functions nop'd:
		// > R_UpdateVertexDecl - sets vertexshader
		// > R_SetStreamSource  - sets vertexbuffer (codeMesh.vb)
	}

	// reset ff at the end of R_TessCodeMeshList
	void R_TessCodeMeshList_end()
	{
		const auto dev = game::mp::dx->device;
		//dev->SetVertexShader(_og_codemesh_vertex_shader);
		//dev->SetPixelShader(_og_codemesh_pixel_shader);
		dev->SetFVF(NULL);
	}

	__declspec(naked) void R_TessCodeMeshList_begin_stub()
	{
		const static uint32_t retn_addr = 0x7133C3;
		__asm
		{
			pushad;
			push	eax; // listArgs
			call	R_TessCodeMeshList_begin;
			add		esp, 4;
			popad;

			// og code
			push    ebx;
			mov     ebx, [eax];
			push    esi;
			mov     esi, [eax + 0xC];
			jmp		retn_addr;
		}
	}

	__declspec(naked) void R_TessCodeMeshList_end_stub()
	{
		const static uint32_t retn_addr = 0x7136AC;
		__asm
		{
			pushad;
			call	R_TessCodeMeshList_end;
			popad;

			// og code
			pop     edi;
			pop     esi;
			pop     ebx;
			mov     esp, ebp;
			jmp		retn_addr;
		}
	}

	// *
	// build buffers

	void build_gfxworld_buffers()
	{
		DEBUG_PRINT("[T4RTX-DEBUG] # Function: build_gfxworld_buffers()\n");

		const auto dev = dx->device;
		void* vertex_buffer_data = nullptr;

		if (game::mp::rgp->world)
		{
			if (gfx_world_vertexbuffer)
			{
				gfx_world_vertexbuffer->Release();
				gfx_world_vertexbuffer = nullptr;

				__debugbreak();
				Com_Error(0, "build_gfxworld_buffers :: gfx_world_vertexbuffer != nullptr");
			}

			if (auto hr = dev->CreateVertexBuffer(WORLD_VERTEX_STRIDE * game::mp::rgp->world->vertexCount, D3DUSAGE_WRITEONLY, WORLD_VERTEX_FORMAT, D3DPOOL_DEFAULT, &gfx_world_vertexbuffer, nullptr);
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
					for (auto i = 0u; i < game::mp::rgp->world->vertexCount; i++)
					{
						// packed source vertex
						const auto src_vert = game::mp::rgp->world->vd.vertices[i];

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
		const static uint32_t stock_func = 0x457A10;
		const static uint32_t retn_addr = 0x4585EB;
		__asm
		{
			pushad;
			call	build_gfxworld_buffers;
			call	mp::main_module::on_map_load;
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
	}

	// called on renderer shutdown (R_Shutdown)
	__declspec(naked) void free_fixed_function_buffers_stub()
	{
		const static uint32_t stock_func = 0x6F6D40; // R_ResetModelLighting
		const static uint32_t retn_addr = 0x6AC1B0;
		__asm
		{
			pushad;
			call	free_fixed_function_buffers;
			call	main_module::on_map_shutdown;
			popad;

			call	stock_func;
			jmp		retn_addr;
		}
	}


	// *
	// *

	fixed_function::fixed_function()
	{
		// force fullbright
		utils::hook::nop(0x6FF3B9, 2); // no decals :[

		// fixed-function rendering of static models (R_TessStaticModelRigidDrawSurfList)
		utils::hook(0x721C52, R_DrawStaticModelDrawSurfNonOptimized, HOOK_CALL).install()->quick();

		// fixed-function rendering of rigid xmodels - call 1 (RB_TessXModelRigidDrawSurfList-> R_DrawXModelSurfCamera-> R_DrawXModelRigidModelSurf1
		utils::hook::nop(0x723FE1, 3); // dont overwrite eax with xsurf, wee want GfxModelRigidSurface
		utils::hook(0x724032, R_DrawXModelRigidModelSurf1_stub, HOOK_JUMP).install()->quick();

		// ^ call 2
		utils::hook(0x7243E4, R_DrawXModelRigidModelSurf2_stub, HOOK_JUMP).install()->quick();

		// fixed-function rendering of skinned (animated) models (R_TessXModelSkinnedDrawSurfList)
		utils::hook(0x71514F, R_DrawXModelSkinnedUncached, HOOK_CALL).install()->quick();

		// fixed-function rendering of world surfaces (R_TessTrianglesPreTessList)
		// :: R_SetStreamsForBspSurface -> R_ClearAllStreamSources -> Stream 1 (world->vld.layerVb) handles 'decals'
		// :: see technique 'lm_r0c0t1c1n1_nc_sm3.tech'
		utils::hook(0x717468, R_DrawBspDrawSurfsPreTess, HOOK_CALL).install()->quick(); // unlit

		// fixed-function rendering of brushmodels
		utils::hook(0x717480, R_TessBModel, HOOK_JUMP).install()->quick();

		// fixed-function rendering of effects - R_TessCodeMeshList (particle clounds are still shader based)
		if (!flags::has_flag("stock_effects"))
		{
			utils::hook::set<BYTE>(0x71352F + 5, MAX_EFFECT_VERTS_FOR_HOOK); // change max verts from 0x4000 to 0x1000 
			utils::hook::nop(0x7133EB, 5); // R_UpdateVertexDecl
			utils::hook::nop(0x713502, 5); // R_SetStreamSource
			utils::hook::nop(0x7133BC, 7); utils::hook(0x7133BC, R_TessCodeMeshList_begin_stub, HOOK_JUMP).install()->quick();
			utils::hook(0x7136A7, R_TessCodeMeshList_end_stub, HOOK_JUMP).install()->quick();
		}

		// ----

		// on map load :: build custom buffers for fixed-function rendering
		utils::hook(0x4585E6, init_fixed_function_buffers_stub, HOOK_JUMP).install()->quick(); // CG_Init :: CG_NorthDirectionChanged call
		utils::hook(0x70CF56, XSurfaceOptimizeRigid_stub, HOOK_JUMP).install()->quick(); // stock XSurfaceOptimize :: XSurfaceOptimizeRigid call

		// on renderer shutdown :: release custom buffers used by fixed-function rendering
		utils::hook(0x6AC1AB, free_fixed_function_buffers_stub, HOOK_JUMP).install()->quick(); // R_Shutdown :: R_ResetModelLighting call

#ifdef DEBUG
		command::add("rtx_rebuild_world", [this](command::params)
		{
			gfx_world_vertexbuffer->Release();
			gfx_world_vertexbuffer = nullptr;
			build_gfxworld_buffers();
		});
#endif
	}
}
