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
}

