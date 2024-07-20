#pragma once

namespace components::sp
{
	class fixed_function : public component
	{
	public:
		fixed_function();
		~fixed_function() = default;
		const char* get_name() override { return "fixed_function_sp"; }

		static inline IDirect3DVertexBuffer9* dynamic_codemesh_vb = nullptr;
		static inline IDirect3DVertexBuffer9* dynamic_markmesh_world_vb = nullptr;
		static inline IDirect3DVertexBuffer9* dynamic_markmesh_model_vb = nullptr;

		static void unpack_normal(const game::PackedUnitVec* packed, game::vec3_t out);
		static void build_worldmatrix_for_object(float(*mtx)[4], const float* quat, const float* origin, const float scale);
		static void build_worldmatrix_for_object(float(*mtx)[4], float(*model_axis)[3], const float* origin, const float scale);
		static void copy_fx_buffer();
		static void copy_marks_buffer();
	};
}
