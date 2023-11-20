#pragma once

namespace game
{
	typedef float vec_t;
	typedef vec_t vec2_t[2];
	typedef vec_t vec3_t[3];
	typedef vec_t vec4_t[4];

	// obv. not complete
	struct __declspec(align(8)) DxGlobals
	{
		IDirect3D9* d3d9;
		IDirect3DDevice9* device;
		unsigned int adapterIndex;
		bool adapterNativeIsValid;
		int adapterNativeWidth;
		int adapterNativeHeight;
		int adapterFullscreenWidth;
		int adapterFullscreenHeight;
		int depthStencilFormat;
		unsigned int displayModeCount;
		// ....
	};

	struct GfxMatrix
	{
		float m[4][4];
	};

	struct GfxViewParms
	{
		GfxMatrix viewMatrix;
		GfxMatrix projectionMatrix;
		GfxMatrix viewProjectionMatrix;
		GfxMatrix inverseViewProjectionMatrix;
		float origin[4];
		float axis[3][3];
		float depthHackNearClip;
		float zNear;
		int pad;
	};
	STATIC_ASSERT_SIZE(GfxViewParms, 0x140);

	struct GfxSceneDef
	{
		int time;
		float floatTime;
		float viewOffset[3];
	};

	struct GfxCodeMatrices
	{
		GfxMatrix matrix[32];
	};

	//struct __declspec(align(16)) GfxCmdBufSourceState
	//{
	//	GfxCodeMatrices matrices;
	//	char pad0[2836];
	//	GfxViewParms* viewParms3D;
	//	/*GfxCodeMatrices matrices;
	//	GfxCmdBufInput input;
	//	GfxViewParms viewParms;
	//	GfxMatrix shadowLookupMatrix;
	//	unsigned __int16 constVersions[90];
	//	unsigned __int16 matrixVersions[8];
	//	float eyeOffset[4];
	//	unsigned int shadowableLightForShadowLookupMatrix;
	//	GfxScaledPlacement* objectPlacement;
	//	GfxViewParms* viewParms3D;
	//	unsigned int depthHackFlags;
	//	GfxScaledPlacement skinnedPlacement;
	//	int cameraView;
	//	GfxViewMode viewMode;
	//	GfxSceneDef sceneDef;
	//	GfxViewport sceneViewport;
	//	float materialTime;
	//	GfxViewportBehavior viewportBehavior;
	//	int renderTargetWidth;
	//	int renderTargetHeight;
	//	bool viewportIsDirty;
	//	unsigned int shadowableLightIndex;*/
	//};

	enum GfxViewMode
	{
		VIEW_MODE_NONE = 0x0,
		VIEW_MODE_3D = 0x1,
		VIEW_MODE_2D = 0x2,
		VIEW_MODE_IDENTITY = 0x3,
	};

	struct GfxCmdBufSourceState
	{
		GfxCodeMatrices matrices;
		char pad0[2128];
		GfxViewParms viewParms;
		char pad00[386];
		GfxViewParms* viewParms3D;
		char pad1[40];
		GfxViewMode viewMode;
		int sceneDef;
	}; // size == 0x1390
	STATIC_ASSERT_OFFSET(GfxCmdBufSourceState, viewParms3D, 0x1314);

	struct GfxViewInfo
	{
		GfxViewParms viewParms;
		GfxSceneDef sceneDef;
		/*GfxViewport sceneViewport;
		GfxViewport displayViewport;
		GfxViewport scissorViewport;
		ShadowType dynamicShadowType;
		char shadowCookieList[4624];
		char align_pad[8];
		int localClientNum;
		int isRenderingFullScreen;
		char needsFloatZ;
		GfxLight shadowableLights[255];
		int shadowableLightCount;
		PointLightPartition pointLightPartitions[4];
		GfxMeshData pointLightMeshData[4];
		int pointLightCount;
		int emissiveSpotLightIndex;
		GfxLight emissiveSpotLight;
		int emissiveSpotDrawSurfCount;
		float emissiveSpotDrawSurfs;
		float emissiveSpotLightCount;
		float blurRadius;
		float frustumPlanes[16];
		GfxDepthOfField dof;
		GfxFilm film;
		GfxGlow glow;
		void* cmds;
		char sunShadow[1240];
		int spotShadowCount;
		char gap5FBC[1942];
		GfxQuadMeshData* fullSceneViewMesh;
		GfxDrawSurfListInfo litInfo;
		GfxDrawSurfListInfo decalInfo;
		GfxDrawSurfListInfo emissiveInfo;
		GfxCmdBufInput input;*/
	}; // size = 0x6D80

	struct GfxBackEndData
	{
		char pad0[1330636];
		unsigned int viewInfoIndex;
		unsigned int viewInfoCount;
		GfxViewInfo* viewInfo;
	};
	STATIC_ASSERT_OFFSET(GfxBackEndData, viewInfo, 0x144DD4);

	struct GfxDrawSurfFields
	{
		unsigned __int64 objectId : 16;
		unsigned __int64 reflectionProbeIndex : 8;
		unsigned __int64 customIndex : 5;
		unsigned __int64 materialSortedIndex : 11;
		unsigned __int64 prepass : 2;
		unsigned __int64 primaryLightIndex : 8;
		unsigned __int64 surfType : 4;
		unsigned __int64 primarySortKey : 6;
		unsigned __int64 unused : 4;
	};

	union GfxDrawSurf
	{
		GfxDrawSurfFields fields;
		unsigned __int64 packed;
	};

	enum MaterialTechniqueType
	{
		TECHNIQUE_DEPTH_PREPASS = 0x0,
		TECHNIQUE_BUILD_FLOAT_Z = 0x1,
		TECHNIQUE_BUILD_SHADOWMAP_DEPTH = 0x2,
		TECHNIQUE_BUILD_SHADOWMAP_COLOR = 0x3,
		TECHNIQUE_UNLIT = 0x4,
		TECHNIQUE_EMISSIVE = 0x5,
		TECHNIQUE_EMISSIVE_SHADOW = 0x6,
		TECHNIQUE_EMISSIVE_REFLECTED = 0x7,
		TECHNIQUE_LIT = 0x8,
		TECHNIQUE_LIT_FADE = 0x9,
		TECHNIQUE_LIT_SUN = 0xA,
		TECHNIQUE_LIT_SUN_FADE = 0xB,
		TECHNIQUE_LIT_SUN_SHADOW = 0xC,
		TECHNIQUE_LIT_SUN_SHADOW_FADE = 0xD,
		TECHNIQUE_LIT_SPOT = 0xE,
		TECHNIQUE_LIT_SPOT_FADE = 0xF,
		TECHNIQUE_LIT_SPOT_SHADOW = 0x10,
		TECHNIQUE_LIT_SPOT_SHADOW_FADE = 0x11,
		TECHNIQUE_LIT_OMNI = 0x12,
		TECHNIQUE_LIT_OMNI_FADE = 0x13,
		TECHNIQUE_LIT_OMNI_SHADOW = 0x14,
		TECHNIQUE_LIT_OMNI_SHADOW_FADE = 0x15,
		TECHNIQUE_LIT_CHARRED = 0x16,
		TECHNIQUE_LIT_FADE_CHARRED = 0x17,
		TECHNIQUE_LIT_SUN_CHARRED = 0x18,
		TECHNIQUE_LIT_SUN_FADE_CHARRED = 0x19,
		TECHNIQUE_LIT_SUN_SHADOW_CHARRED = 0x1A,
		TECHNIQUE_LIT_SUN_SHADOW_FADE_CHARRED = 0x1B,
		TECHNIQUE_LIT_SPOT_CHARRED = 0x1C,
		TECHNIQUE_LIT_SPOT_FADE_CHARRED = 0x1D,
		TECHNIQUE_LIT_SPOT_SHADOW_CHARRED = 0x1E,
		TECHNIQUE_LIT_SPOT_SHADOW_FADE_CHARRED = 0x1F,
		TECHNIQUE_LIT_OMNI_CHARRED = 0x20,
		TECHNIQUE_LIT_OMNI_FADE_CHARRED = 0x21,
		TECHNIQUE_LIT_OMNI_SHADOW_CHARRED = 0x22,
		TECHNIQUE_LIT_OMNI_SHADOW_FADE_CHARRED = 0x23,
		TECHNIQUE_LIT_INSTANCED = 0x24,
		TECHNIQUE_LIT_INSTANCED_SUN = 0x25,
		TECHNIQUE_LIT_INSTANCED_SUN_SHADOW = 0x26,
		TECHNIQUE_LIT_INSTANCED_SPOT = 0x27,
		TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW = 0x28,
		TECHNIQUE_LIT_INSTANCED_OMNI = 0x29,
		TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW = 0x2A,
		TECHNIQUE_LIGHT_SPOT = 0x2B,
		TECHNIQUE_LIGHT_OMNI = 0x2C,
		TECHNIQUE_LIGHT_SPOT_SHADOW = 0x2D,
		TECHNIQUE_LIGHT_SPOT_CHARRED = 0x2E,
		TECHNIQUE_LIGHT_OMNI_CHARRED = 0x2F,
		TECHNIQUE_LIGHT_SPOT_SHADOW_CHARRED = 0x30,
		TECHNIQUE_FAKELIGHT_NORMAL = 0x31,
		TECHNIQUE_FAKELIGHT_VIEW = 0x32,
		TECHNIQUE_SUNLIGHT_PREVIEW = 0x33,
		TECHNIQUE_CASE_TEXTURE = 0x34,
		TECHNIQUE_WIREFRAME_SOLID = 0x35,
		TECHNIQUE_WIREFRAME_SHADED = 0x36,
		TECHNIQUE_SHADOWCOOKIE_CASTER = 0x37,
		TECHNIQUE_SHADOWCOOKIE_RECEIVER = 0x38,
		TECHNIQUE_DEBUG_BUMPMAP = 0x39,
		TECHNIQUE_DEBUG_BUMPMAP_INSTANCED = 0x3A,
		TECHNIQUE_COUNT = 0x3B,
	};

	struct MaterialInfo
	{
		const char* name;
		char gameFlags;
		char sortKey;
		char textureAtlasRowCount;
		char textureAtlasColumnCount;
	};

	struct MaterialTechnique
	{
		const char* name;
		unsigned __int16 flags;
		unsigned __int16 passCount;
		int passArray[1];
	};

	struct MaterialTechniqueSet
	{
		const char* name;
		char worldVertFormat;
		bool hasBeenUploaded;
		char unused[1];
		MaterialTechniqueSet* remappedTechniqueSet;
		MaterialTechnique* techniques[34];
	};

	struct Material
	{
		MaterialInfo info;
		char pad[88];
		MaterialTechniqueSet* techniqueSet;
	};

	struct r_global_permanent_t
	{
		Material* sortedMaterials[2048];
	};

	struct GfxCmdBufPrimState
	{
		IDirect3DDevice9* device;
	};

	enum GfxDepthRangeType
	{
		GFX_DEPTH_RANGE_SCENE = 0x0,
		GFX_DEPTH_RANGE_VIEWMODEL = 0x2,
		GFX_DEPTH_RANGE_FULL = 0xFFFFFFFF,
	};

	struct GfxViewport
	{
		int x;
		int y;
		int width;
		int height;
	};

	struct GfxCmdBufState
	{
		char refSamplerState[16];
		unsigned int samplerState[16];
		void* GfxTexture_samplerTexture[16];
		GfxCmdBufPrimState prim;
		char pad0[50];
		Material* material;
		MaterialTechniqueType techType;
		MaterialTechnique* technique;
		void* MaterialPass_pass;
		unsigned int passIndex;
		GfxDepthRangeType depthRangeType;
		float depthRangeNear;
		float depthRangeFar;
		char pad1[4116];
		void* pixelShader;
		void* vertexShader;
		GfxViewport viewport;
		int renderTargetId;
		Material* origMaterial;
		MaterialTechniqueType origTechType;
	};
	STATIC_ASSERT_OFFSET(GfxCmdBufState, material, 0xC8);
	STATIC_ASSERT_OFFSET(GfxCmdBufState, origMaterial, 0x1118);

	struct switch_material_t
	{
		bool switch_material;
		bool switch_technique;
		bool switch_technique_type;

		Material* current_material;
		MaterialTechnique* current_technique;

		Material* material;
		MaterialTechnique* technique;

		MaterialTechniqueType technique_type;
	};
}

