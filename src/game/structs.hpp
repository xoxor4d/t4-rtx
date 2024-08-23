#pragma once

// https://github.com/JezuzLizard/T4SP-Server-Plugin/blob/main/src/game/xasset.hpp ( :* )

namespace game
{
	struct gentity_s;
	typedef float vec_t;
	typedef vec_t vec2_t[2];
	typedef vec_t vec3_t[3];
	typedef vec_t vec4_t[4];

	enum connstate_t : __int32
	{
		CA_DISCONNECTED = 0x0,
		CA_CINEMATIC = 0x1,
		CA_UICINEMATIC = 0x2,
		CA_LOGO = 0x3,
		CA_CONNECTING = 0x4,
		CA_CHALLENGING = 0x5,
		CA_CONNECTED = 0x6,
		CA_SENDINGSTATS = 0x7,
		CA_LOADING = 0x8,
		CA_PRIMED = 0x9,
		CA_ACTIVE = 0xA,
		CA_MAP_RESTART = 0xB,
	};

	enum KeyCatcherFlag_t : __int32
	{
		KEY_CATCHER_FLAG_CONSOLE = 0x1,
		KEY_CATCHER_FLAG_LOCATION_SELECTION = 0x8,
		KEY_CATCHER_FLAG_IN_POPUP = 0x10,
		KEY_CATCHER_FLAG_CHAT = 0x20,
	};

	struct clientUIActive_s
	{
		char active;
		char isRunning;
		char cgameInitialized;
		char cgameInitCalled;
		int mapPreloaded;
		KeyCatcherFlag_t keyCatchers;
		int displayHUDWithKeycatchUI;
		connstate_t connectionState;
		int nextScrollTime;
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

	struct orientation_t
	{
		float origin[3];
		float axis[3][3];
	};

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

	struct __declspec(align(8)) MaterialInfo
	{
		const char* name; //OFS: 0x0 SIZE: 0x4
		char gameFlags; //OFS: 0x4 SIZE: 0x1
		char sortKey; //OFS: 0x5 SIZE: 0x1
		char textureAtlasRowCount; //OFS: 0x6 SIZE: 0x1
		char textureAtlasColumnCount; //OFS: 0x7 SIZE: 0x1
		GfxDrawSurf drawSurf; //OFS: 0x8 SIZE: 0x8
		unsigned int surfaceTypeBits; //OFS: 0x10 SIZE: 0x4
		unsigned __int16 hashIndex; //OFS: 0x14 SIZE: 0x2
	};

	struct __declspec(align(1)) MaterialStreamRouting
	{
		char source; //OFS: 0x0 SIZE: 0x1
		char dest; //OFS: 0x1 SIZE: 0x1
	};

	struct MaterialVertexStreamRouting
	{
		MaterialStreamRouting data[16]; //OFS: 0x0 SIZE: 0x20
		IDirect3DVertexDeclaration9* decl[17]; //OFS: 0x20 SIZE: 0x44
	};

	struct MaterialVertexDeclaration
	{
		char streamCount; //OFS: 0x0 SIZE: 0x1
		bool hasOptionalSource; //OFS: 0x1 SIZE: 0x1
		bool isLoaded; //OFS: 0x2 SIZE: 0x1
		MaterialVertexStreamRouting routing; //OFS: 0x4 SIZE: 0x64
	};

	struct GfxVertexShaderLoadDef
	{
		unsigned int* program; //OFS: 0x0 SIZE: 0x4
		unsigned __int16 programSize; //OFS: 0x4 SIZE: 0x2
		unsigned __int16 loadForRenderer; //OFS: 0x6 SIZE: 0x2
	};

	struct MaterialVertexShaderProgram
	{
		IDirect3DVertexShader9* vs; //OFS: 0x0 SIZE: 0x4
		GfxVertexShaderLoadDef loadDef; //OFS: 0x4 SIZE: 0x8
	};

	struct MaterialVertexShader
	{
		const char* name; //OFS: 0x0 SIZE: 0x4
		MaterialVertexShaderProgram prog; //OFS: 0x4 SIZE: 0xC
	};

	struct GfxPixelShaderLoadDef
	{
		unsigned int* program; //OFS: 0x0 SIZE: 0x4
		unsigned __int16 programSize; //OFS: 0x4 SIZE: 0x2
		unsigned __int16 loadForRenderer; //OFS: 0x6 SIZE: 0x2
	};

	struct MaterialPixelShaderProgram
	{
		IDirect3DPixelShader9* ps; //OFS: 0x0 SIZE: 0x4
		GfxPixelShaderLoadDef loadDef; //OFS: 0x4 SIZE: 0x8
	};

	struct MaterialPixelShader
	{
		const char* name; //OFS: 0x0 SIZE: 0x4
		MaterialPixelShaderProgram prog; //OFS: 0x4 SIZE: 0xC
	};

	struct __declspec(align(2)) MaterialArgumentCodeConst
	{
		unsigned __int16 index; //OFS: 0x0 SIZE: 0x2
		char firstRow; //OFS: 0x2 SIZE: 0x1
		char rowCount; //OFS: 0x3 SIZE: 0x1
	};

	union MaterialArgumentDef
	{
		const float* literalConst; //OFS: 0x0 SIZE: 0x4
		MaterialArgumentCodeConst codeConst; //OFS: 0x1 SIZE: 0x4
		unsigned int codeSampler; //OFS: 0x2 SIZE: 0x4
		unsigned int nameHash; //OFS: 0x3 SIZE: 0x4
	};

	struct MaterialShaderArgument
	{
		unsigned __int16 type; //OFS: 0x0 SIZE: 0x2
		unsigned __int16 dest; //OFS: 0x2 SIZE: 0x2
		MaterialArgumentDef u; //OFS: 0x4 SIZE: 0x4
	};

	struct MaterialPass
	{
		MaterialVertexDeclaration* vertexDecl; //OFS: 0x0 SIZE: 0x4
		MaterialVertexShader* vertexShader; //OFS: 0x4 SIZE: 0x4
		MaterialPixelShader* pixelShader; //OFS: 0x8 SIZE: 0x4
		char perPrimArgCount; //OFS: 0xC SIZE: 0x1
		char perObjArgCount; //OFS: 0xD SIZE: 0x1
		char stableArgCount; //OFS: 0xE SIZE: 0x1
		char customSamplerFlags; //OFS: 0xF SIZE: 0x1
		MaterialShaderArgument* args; //OFS: 0x10 SIZE: 0x4
	};

	struct MaterialTechnique
	{
		const char* name; //OFS: 0x0 SIZE: 0x4
		unsigned __int16 flags; //OFS: 0x4 SIZE: 0x2
		unsigned __int16 passCount; //OFS: 0x6 SIZE: 0x2
		MaterialPass passArray[1]; //OFS: 0x8 SIZE: 0x14
	};

	struct MaterialTechniqueSet
	{
		const char* name; //OFS: 0x0 SIZE: 0x4
		char worldVertFormat; //OFS: 0x4 SIZE: 0x1
		bool hasBeenUploaded; //OFS: 0x5 SIZE: 0x1
		char unused[1]; //OFS: 0x6 SIZE: 0x1
		MaterialTechniqueSet* remappedTechniqueSet; //OFS: 0x8 SIZE: 0x4
		MaterialTechnique* techniques[59]; //OFS: 0xC SIZE: 0xEC
	};

	enum MapType : __int32
	{
		MAPTYPE_NONE = 0x0,
		MAPTYPE_INVALID1 = 0x1,
		MAPTYPE_INVALID2 = 0x2,
		MAPTYPE_2D = 0x3,
		MAPTYPE_3D = 0x4,
		MAPTYPE_CUBE = 0x5,
		MAPTYPE_COUNT = 0x6,
	};

	struct CardMemory
	{
		int platform[2]; //OFS: 0x0 SIZE: 0x8
	};

	/*struct GfxImage
	{
		MapType mapType;
		char par0[0x30];
		unsigned __int8 semantic;
		CardMemory cardMemory;
		unsigned __int16 width;
		unsigned __int16 height;
		unsigned __int16 depth;
		unsigned __int8 category;
		bool delayLoadPixels;
		unsigned __int8* pixels;
		unsigned int baseSize;
		unsigned __int16 streamSlot;
		unsigned __int16 streaming;
		char* name;
	};*/

	struct GfxImageLoadDef
	{
		char levelCount; //OFS: 0x0 SIZE: 0x1
		char flags; //OFS: 0x1 SIZE: 0x1
		__int16 dimensions[3]; //OFS: 0x2 SIZE: 0x6
		int format; //OFS: 0x8 SIZE: 0x4
		int resourceSize; //OFS: 0xC SIZE: 0x4
		char data[1]; //OFS: 0x10 SIZE: 0x1
	};

	union GfxTexture
	{
		IDirect3DBaseTexture9* basemap; //OFS: 0x0 SIZE: 0x4
		IDirect3DTexture9* map; //OFS: 0x1 SIZE: 0x4
		IDirect3DVolumeTexture9* volmap; //OFS: 0x2 SIZE: 0x4
		IDirect3DCubeTexture9* cubemap; //OFS: 0x3 SIZE: 0x4
		GfxImageLoadDef* loadDef; //OFS: 0x4 SIZE: 0x4
	};

	struct __declspec(align(1)) Picmip
	{
		char platform[2]; //OFS: 0x0 SIZE: 0x2
	};

	struct GfxImage
	{
		MapType mapType; //OFS: 0x0 SIZE: 0x4
		GfxTexture texture; //OFS: 0x4 SIZE: 0x4
		Picmip picmip; //OFS: 0x8 SIZE: 0x2
		bool noPicmip; //OFS: 0xA SIZE: 0x1
		char semantic; //OFS: 0xB SIZE: 0x1
		char track; //OFS: 0xC SIZE: 0x1
		CardMemory cardMemory; //OFS: 0x10 SIZE: 0x8
		unsigned __int16 width; //OFS: 0x18 SIZE: 0x2
		unsigned __int16 height; //OFS: 0x1A SIZE: 0x2
		unsigned __int16 depth; //OFS: 0x1C SIZE: 0x2
		char category; //OFS: 0x1E SIZE: 0x1
		bool delayLoadPixels; //OFS: 0x1F SIZE: 0x1
		const char* name; //OFS: 0x20 SIZE: 0x4
	};
	STATIC_ASSERT_SIZE(GfxImage, 0x24);
	STATIC_ASSERT_OFFSET(GfxImage, mapType, 0x0);
	STATIC_ASSERT_OFFSET(GfxImage, texture, 0x4);
	STATIC_ASSERT_OFFSET(GfxImage, picmip, 0x8);
	STATIC_ASSERT_OFFSET(GfxImage, noPicmip, 0xA);
	STATIC_ASSERT_OFFSET(GfxImage, semantic, 0xB);
	STATIC_ASSERT_OFFSET(GfxImage, track, 0xC);
	STATIC_ASSERT_OFFSET(GfxImage, cardMemory, 0x10);
	STATIC_ASSERT_OFFSET(GfxImage, width, 0x18);
	STATIC_ASSERT_OFFSET(GfxImage, height, 0x1A);
	STATIC_ASSERT_OFFSET(GfxImage, depth, 0x1C);
	STATIC_ASSERT_OFFSET(GfxImage, category, 0x1E);
	STATIC_ASSERT_OFFSET(GfxImage, delayLoadPixels, 0x1F);
	STATIC_ASSERT_OFFSET(GfxImage, name, 0x20);

	struct water_t
	{
		float floatTime;
		float* H0X;
		float* H0Y;
		float* wTerm;
		int M;
		int N;
		float Lx;
		float Lz;
		float gravity;
		float windvel;
		float winddir[2];
		float amplitude;
		float codeConstant[4];
		GfxImage* image;
	};

	union MaterialTextureDefInfo
	{
		GfxImage* image; //OFS: 0x0 SIZE: 0x4
		water_t* water; //OFS: 0x1 SIZE: 0x4
	};

	struct MaterialTextureDef
	{
		unsigned int nameHash; //OFS: 0x0 SIZE: 0x4
		char nameStart; //OFS: 0x4 SIZE: 0x1
		char nameEnd; //OFS: 0x5 SIZE: 0x1
		char samplerState; //OFS: 0x6 SIZE: 0x1
		char semantic; //OFS: 0x7 SIZE: 0x1
		char isMatureContent; //OFS: 0x8 SIZE: 0x1
		MaterialTextureDefInfo u; //OFS: 0xC SIZE: 0x4
	};

	struct MaterialConstantDef
	{
		unsigned int nameHash; //OFS: 0x0 SIZE: 0x4
		char name[12]; //OFS: 0x4 SIZE: 0xC
		float literal[4]; //OFS: 0x10 SIZE: 0x10
	};

	struct GfxStateBits
	{
		unsigned int loadBits[2]; //OFS: 0x0 SIZE: 0x8
	};

	struct Material
	{
		MaterialInfo info; //OFS: 0x0 SIZE: 0x18
		char stateBitsEntry[67]; //OFS: 0x18 SIZE: 0x43
		char textureCount; //OFS: 0x5B SIZE: 0x1
		char constantCount; //OFS: 0x5C SIZE: 0x1
		char stateBitsCount; //OFS: 0x5D SIZE: 0x1
		char stateFlags; //OFS: 0x5E SIZE: 0x1
		char cameraRegion; //OFS: 0x5F SIZE: 0x1
		MaterialTechniqueSet* techniqueSet; //OFS: 0x60 SIZE: 0x4
		MaterialTextureDef* textureTable; //OFS: 0x64 SIZE: 0x4
		MaterialConstantDef* constantTable; //OFS: 0x68 SIZE: 0x4
		GfxStateBits* stateBitsTable; //OFS: 0x6C SIZE: 0x4
	};

	struct cplane_s
	{
		float normal[3]; //OFS: 0x0 SIZE: 0xC
		float dist; //OFS: 0xC SIZE: 0x4
		char type; //OFS: 0x10 SIZE: 0x1
		char signbits; //OFS: 0x11 SIZE: 0x1
		char pad[2]; //OFS: 0x12 SIZE: 0x2
	};

	struct cStaticModelWritable
	{
		unsigned __int16 nextModelInWorldSector;
	};

	struct DObjAnimMat
	{
		float quat[4]; //OFS: 0x0 SIZE: 0x10
		float trans[3]; //OFS: 0x10 SIZE: 0xC
		float transWeight; //OFS: 0x1C SIZE: 0x4
	};

	struct XSurfaceVertexInfo
	{
		__int16 vertCount[4]; //OFS: 0x0 SIZE: 0x8
		unsigned __int16* vertsBlend; //OFS: 0x8 SIZE: 0x4
	};

	union GfxColor
	{
		unsigned int packed; //OFS: 0x0 SIZE: 0x4
		unsigned __int8 array[4]; //OFS: 0x1 SIZE: 0x4
	};

	union PackedTexCoords
	{
		unsigned int packed; //OFS: 0x0 SIZE: 0x4
	};

	union PackedUnitVec
	{
		unsigned int packed; //OFS: 0x0 SIZE: 0x4
		char array[4];
	};

	struct gfxVertexSteamsUnk
	{
		unsigned int stride;
		IDirect3DVertexBuffer9* vb;
		unsigned int offset;
	};


	struct GfxPackedVertex
	{
		float xyz[3]; //OFS: 0x0 SIZE: 0xC
		float binormalSign; //OFS: 0xC SIZE: 0x4
		GfxColor color; //OFS: 0x10 SIZE: 0x4
		PackedTexCoords texCoord; //OFS: 0x14 SIZE: 0x4
		PackedUnitVec normal; //OFS: 0x18 SIZE: 0x4
		PackedUnitVec tangent; //OFS: 0x1C SIZE: 0x4
	};

	struct __declspec(align(2)) XSurfaceCollisionAabb
	{
		unsigned __int16 mins[3]; //OFS: 0x0 SIZE: 0x6
		unsigned __int16 maxs[3]; //OFS: 0x6 SIZE: 0x6
	};

	struct __declspec(align(2)) XSurfaceCollisionNode
	{
		XSurfaceCollisionAabb aabb; //OFS: 0x0 SIZE: 0xC
		unsigned __int16 childBeginIndex; //OFS: 0xC SIZE: 0x2
		unsigned __int16 childCount; //OFS: 0xE SIZE: 0x2
	};

	struct __declspec(align(2)) XSurfaceCollisionLeaf
	{
		unsigned __int16 triangleBeginIndex; //OFS: 0x0 SIZE: 0x2
	};

	struct XSurfaceCollisionTree
	{
		float trans[3]; //OFS: 0x0 SIZE: 0xC
		float scale[3]; //OFS: 0xC SIZE: 0xC
		unsigned int nodeCount; //OFS: 0x18 SIZE: 0x4
		XSurfaceCollisionNode* nodes; //OFS: 0x1C SIZE: 0x4
		unsigned int leafCount; //OFS: 0x20 SIZE: 0x4
		XSurfaceCollisionLeaf* leafs; //OFS: 0x24 SIZE: 0x4
	};

	struct XRigidVertList
	{
		unsigned __int16 boneOffset; //OFS: 0x0 SIZE: 0x2
		unsigned __int16 vertCount; //OFS: 0x2 SIZE: 0x2
		unsigned __int16 triOffset; //OFS: 0x4 SIZE: 0x2
		unsigned __int16 triCount; //OFS: 0x6 SIZE: 0x2
		XSurfaceCollisionTree* collisionTree; //OFS: 0x8 SIZE: 0x4
	};

	struct XSurface
	{
		char tileMode; //OFS: 0x0 SIZE: 0x1
		bool deformed; //OFS: 0x1 SIZE: 0x1
		unsigned __int16 vertCount; //OFS: 0x2 SIZE: 0x2
		unsigned __int16 triCount; //OFS: 0x4 SIZE: 0x2
		char zoneHandle; //OFS: 0x6 SIZE: 0x1
		unsigned __int16 baseTriIndex; //OFS: 0x8 SIZE: 0x2
		unsigned __int16 baseVertIndex; //OFS: 0xA SIZE: 0x2
		unsigned __int16* triIndices; //OFS: 0xC SIZE: 0x4
		XSurfaceVertexInfo vertInfo; //OFS: 0x10 SIZE: 0xC
		GfxPackedVertex* verts0; //OFS: 0x1C SIZE: 0x4
		IDirect3DVertexBuffer9* vb0; //OFS: 0x20 SIZE: 0x4
		unsigned int vertListCount; //OFS: 0x24 SIZE: 0x4
		XRigidVertList* vertList; //OFS: 0x28 SIZE: 0x4
		IDirect3DIndexBuffer9* indexBuffer;
		int partBits[4];
	}; STATIC_ASSERT_SIZE(XSurface, 0x40);

	struct GfxStaticModelDrawStream
	{
		const unsigned int* primDrawSurfPos;
		GfxTexture* reflectionProbeTexture;
		unsigned int customSamplerFlags;
		XSurface* localSurf;
		unsigned int smodelCount;
		const unsigned __int16* smodelList;
		unsigned int reflectionProbeIndex;
	};

	struct XModelLodInfo
	{
		float dist; //OFS: 0x0 SIZE: 0x4
		unsigned __int16 numsurfs; //OFS: 0x4 SIZE: 0x2
		unsigned __int16 surfIndex; //OFS: 0x6 SIZE: 0x2
		int partBits[4]; //OFS: 0x8 SIZE: 0x10
		char lod; //OFS: 0x18 SIZE: 0x1
		char smcIndexPlusOne; //OFS: 0x19 SIZE: 0x1
		char smcAllocBits; //OFS: 0x1A SIZE: 0x1
		char unused; //OFS: 0x1B SIZE: 0x1
	};

	struct XModelCollTri_s
	{
		float plane[4]; //OFS: 0x0 SIZE: 0x10
		float svec[4]; //OFS: 0x10 SIZE: 0x10
		float tvec[4]; //OFS: 0x20 SIZE: 0x10
	};

	struct XModelCollSurf_s
	{
		XModelCollTri_s* collTris; //OFS: 0x0 SIZE: 0x4
		int numCollTris; //OFS: 0x4 SIZE: 0x4
		float mins[3]; //OFS: 0x8 SIZE: 0xC
		float maxs[3]; //OFS: 0x14 SIZE: 0xC
		int boneIdx; //OFS: 0x20 SIZE: 0x4
		int contents; //OFS: 0x24 SIZE: 0x4
		int surfFlags; //OFS: 0x28 SIZE: 0x4
	};

	struct XBoneInfo
	{
		float bounds[2][3]; //OFS: 0x0 SIZE: 0x18
		float offset[3]; //OFS: 0x18 SIZE: 0xC
		float radiusSquared; //OFS: 0x24 SIZE: 0x4
	};

	struct XModelHighMipBounds
	{
		float mins[3]; //OFS: 0x0 SIZE: 0xC
		float maxs[3]; //OFS 0xC SIZE: 0xC
	};

	struct XModelStreamInfo
	{
		XModelHighMipBounds* highMipBounds; //OFS 0x0 SIZE: 0x4
	};

	struct XModel
	{
		const char* name; //OFS: 0x0 SIZE: 0x4
		char numBones; //OFS: 0x4 SIZE: 0x1
		char numRootBones; //OFS: 0x5 SIZE: 0x1
		char numsurfs; //OFS: 0x6 SIZE: 0x1
		char lodRampType; //OFS: 0x7 SIZE: 0x1
		unsigned __int16* boneNames; //OFS: 0x8 SIZE: 0x4
		char* parentList; //OFS: 0xC SIZE: 0x4
		__int16* quats; //OFS: 0x10 SIZE: 0x4
		float* trans; //OFS: 0x14 SIZE: 0x4
		char* partClassification; //OFS: 0x18 SIZE: 0x4
		DObjAnimMat* baseMat; //OFS: 0x1C SIZE: 0x4
		XSurface* surfs; //OFS: 0x20 SIZE: 0x4
		Material** materialHandles; //OFS: 0x24 SIZE: 0x4
		XModelLodInfo lodInfo[4]; //OFS: 0x28 SIZE: 0x70
		XModelCollSurf_s* collSurfs; //OFS: 0x98 SIZE: 0x4
		int numCollSurfs; //OFS: 0x9C SIZE: 0x4
		int contents; //OFS: 0xA0 SIZE: 0x4
		XBoneInfo* boneInfo; //OFS: 0xA4 SIZE: 0x4
		float radius; //OFS: 0xA8 SIZE: 0x4
		float mins[3]; //OFS: 0xAC SIZE: 0xC
		float maxs[3]; //OFS: 0xB8 SIZE: 0xC
		__int16 numLods; //OFS: 0xC4 SIZE: 0x2
		__int16 collLod; //OFS: 0xC6 SIZE: 0x2
		XModelStreamInfo streamInfo; //OFS: 0xC8 SIZE: 0x4
		int memUsage; //OFS: 0xCC SIZE: 0x4
		char flags; //OFS: 0xD0 SIZE: 0x1
		bool bad; //OFS: 0xD1 SIZE: 0x1
		void* PhysPreset__physPreset; //OFS: 0xD4 SIZE: 0x4
		void* PhysGeomList__physGeoms; //OFS: 0xD8 SIZE: 0x4
		void* PhysGeomList__collmap; //OFS: 0xDC SIZE: 0x4
		void* PhysConstraints__physConstraints; //OFS: 0xE0 SIZE: 0x4
	};
	STATIC_ASSERT_SIZE(XModel, 0xE4);
	STATIC_ASSERT_OFFSET(XModel, surfs, 0x20);
	STATIC_ASSERT_OFFSET(XModel, lodInfo, 0x28);
	STATIC_ASSERT_OFFSET(XModel, numLods, 0xC4);
	STATIC_ASSERT_OFFSET(XModel, bad, 0xD1);

	struct GfxWindowTarget
	{
		HWND__* hwnd;
		IDirect3DSwapChain9* swapChain;
		int width;
		int height;
	};

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
		_D3DDISPLAYMODE displayModes[256];
		const char* resolutionNameTable[257];
		const char* refreshRateNameTable[257];
		char modeText[5120];
		IDirect3DQuery9* fencePool[8];
		unsigned int nextFence;
		int gpuSync;
		int multiSampleType;
		unsigned int multiSampleQuality;
		int sunSpriteSamples;
		IDirect3DSurface9* singleSampleDepthStencilSurface;
		int pad;
		bool deviceLost;
		bool inScene;
		int targetWindowIndex;
		int windowCount;
		GfxWindowTarget windows[1];
		// ....
	};
	STATIC_ASSERT_OFFSET(DxGlobals, windows, 0x2C78);

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

	enum GfxViewMode
	{
		VIEW_MODE_NONE = 0x0,
		VIEW_MODE_3D = 0x1,
		VIEW_MODE_2D = 0x2,
		VIEW_MODE_IDENTITY = 0x3,
	};

	struct GfxReadCmdBuf
	{
		const unsigned int* primDrawSurfPos;
	};

	struct GfxBspPreTessDrawSurf
	{
		unsigned __int16 baseSurfIndex;
		unsigned __int16 totalTriCount;
	};

	struct GfxViewInfo
	{
		GfxViewParms viewParms;
		GfxSceneDef sceneDef;
		// ....
	}; // size = 0x6D80

	struct FxCodeMeshData
	{
		unsigned int triCount;
		unsigned __int16* indices;
		unsigned __int16 argOffset;
		unsigned __int16 argCount;
		unsigned int pad;
	};

	struct GfxVertexBufferState
	{
		volatile int used;
		int total;
		IDirect3DVertexBuffer9* buffer;
		char* verts;
	};

	struct GfxMeshData
	{
		unsigned int indexCount;
		unsigned int totalIndexCount;
		unsigned __int16* indices;
		GfxVertexBufferState vb;
		unsigned int vertSize;
	};

	struct GfxDebugPoly
	{
		float color[4];
		int firstVert;
		int vertCount;
	};

	struct trDebugString_t
	{
		float xyz[3];
		float color[4];
		float scale;
		char text[96];
	};

	struct trDebugLine_t
	{
		float start[3];
		float end[3];
		float color[4];
		int depthTest;
	};

	struct GfxDebugPlume
	{
		float origin[3];
		float color[4];
		int score;
		int startTime;
		int duration;
	};

	struct DebugGlobals
	{
		float(*verts)[3];
		int vertCount;
		int vertLimit;
		GfxDebugPoly* polys;
		int polyCount;
		int polyLimit;
		trDebugString_t* strings;
		int stringCount;
		int stringLimit;
		trDebugString_t* externStrings;
		int externStringCount;
		int externMaxStringCount;
		trDebugLine_t* lines;
		int lineCount;
		int lineLimit;
		trDebugLine_t* externLines;
		int externLineCount;
		int externMaxLineCount;
		GfxDebugPlume* plumes;
		int plumeCount;
		int plumeLimit;
	};

	struct __declspec(align(4)) GfxBackEndData
	{
		char surfsBuffer[262144];
		FxCodeMeshData codeMeshes[2048];
		char pad0[569088];
		GfxMeshData codeMesh;
		char pad[466512];
		GfxMeshData markMesh;
		char pad1[58];
		unsigned int viewInfoIndex;
		unsigned int viewInfoCount;
		GfxViewInfo* viewInfo;
		char pad2[78];
		DebugGlobals debugGlobals;
		unsigned int drawType;
	};
	STATIC_ASSERT_OFFSET(GfxBackEndData, markMesh, 0x144D70);
	STATIC_ASSERT_OFFSET(GfxBackEndData, viewInfo, 0x144DD4);
	STATIC_ASSERT_OFFSET(GfxBackEndData, debugGlobals, 0x144E28);

	struct ModelLodFade
	{
		unsigned int value; //OFS: 0x0 SIZE: 0x4
	};

	struct GfxPackedPlacement
	{
		float origin[3]; //OFS: 0x0 SIZE: 0xC
		float axis[3][3]; //OFS: 0xC SIZE: 0x24
		float scale; //OFS: 0x30 SIZE: 0x4
	};

	struct GfxStaticModelDrawInst
	{
		float cullDist; //OFS: 0x0 SIZE: 0x4
		GfxPackedPlacement placement; //OFS: 0x4 SIZE: 0x34
		XModel* model; //OFS: 0x38 SIZE: 0x4
		ModelLodFade lodFade_idk; //OFS: 0x3C SIZE: 0x4
		int cachedLightSettingIndex_idk; //OFS: 0x40 SIZE: 0x4
		char field_44; //OFS: 0x44 SIZE: 0x1
		char field_45; //OFS: 0x45 SIZE: 0x1
		unsigned __int8 reflectionProbeIndex; //OFS: 0x46 SIZE: 0x1
		unsigned __int8 primaryLightIndex; //OFS: 0x47 SIZE: 0x1
		int flags; //OFS: 0x48 SIZE: 0x4
		unsigned int sModelCacheIndex_idk[4]; //OFS: 0x4C SIZE: 0x10
	};

	struct GfxStaticModelInst
	{
		float mins[3]; //OFS: 0x0 SIZE: 0xC
		float maxs[3]; //OFS: 0xC SIZE: 0xC
		GfxColor groundLighting; //OFS: 0x18 SIZE: 0x4
	};

	struct __declspec(align(4)) GfxModelSurfaceInfo
	{
		void* baseMat;
		char boneIndex;
		char boneCount;
		unsigned __int16 gfxEntIndex;
		unsigned __int16 lightingHandle;
		char dobjModelIndex;
	};

	union $178D1D161B34F636C03EBC0CA3007D75
	{
		GfxPackedVertex* skinnedVert;
		int oldSkinnedCachedOffset;
	};

	struct GfxModelSkinnedSurface
	{
		int skinnedCachedOffset;
		XSurface* xsurf;
		GfxModelSurfaceInfo info;
		$178D1D161B34F636C03EBC0CA3007D75 u;
	};

	struct GfxPlacement
	{
		float quat[4];
		float origin[3];
	};

	struct GfxScaledPlacement
	{
		GfxPlacement base;
		float scale;
	};

	struct GfxModelRigidSurface
	{
		GfxModelSkinnedSurface surf;
		GfxScaledPlacement placement;
	};

	struct GfxCmdBufSourceState
	{
		GfxCodeMatrices matrices;
		char pad0[2116];
		GfxBackEndData* data;
		char padx0[8];
		GfxViewParms viewParms;
		char pad00[364];
		float eyeOffset[4];
		unsigned int shadowableLightForShadowLookupMatrix;
		GfxScaledPlacement* objectPlacement;
		GfxViewParms* viewParms3D;
		bool depthHack;
		GfxScaledPlacement skinnedPlacement;
		int cameraView;
		GfxViewMode viewMode;
		int sceneDef;
	}; // size == 0x1390
	STATIC_ASSERT_OFFSET(GfxCmdBufSourceState, viewParms, 0x1050);
	STATIC_ASSERT_OFFSET(GfxCmdBufSourceState, viewParms3D, 0x1314);

	enum MaterialVertexDeclType
	{
		VERTDECL_GENERIC = 0x0,
		VERTDECL_PACKED = 0x1,
		VERTDECL_WORLD = 0x2,
		VERTDECL_WORLD_T1N0 = 0x3,
		VERTDECL_WORLD_T1N1 = 0x4,
		VERTDECL_WORLD_T2N0 = 0x5,
		VERTDECL_WORLD_T2N1 = 0x6,
		VERTDECL_WORLD_T2N2 = 0x7,
		VERTDECL_WORLD_T3N0 = 0x8,
		VERTDECL_WORLD_T3N1 = 0x9,
		VERTDECL_WORLD_T3N2 = 0xA,
		VERTDECL_WORLD_T4N0 = 0xB,
		VERTDECL_WORLD_T4N1 = 0xC,
		VERTDECL_WORLD_T4N2 = 0xD,
		VERTDECL_POS_TEX = 0xE,
		VERTDECL_STATICMODELCACHE = 0xF,
		VERTDECL_WATER = 0x10,
		VERTDECL_PARTICLECLOUD = 0x11,
		VERTDECL_COUNT = 0x12,
	};

	struct srfTriangles_t
	{
		int vertexLayerData; //OFS: 0x0 SIZE: 0x4
		int firstVertex; //OFS: 0x4 SIZE: 0x4
		unsigned __int16 vertexCount; //OFS: 0x8 SIZE: 0x2
		unsigned __int16 triCount; //OFS: 0xA SIZE: 0x2
		int baseIndex; //OFS: 0xC SIZE: 0x4
	};

	struct GfxSurface
	{
		srfTriangles_t tris; //OFS: 0x0 SIZE: 0x10
		int pad; //OFS: 0x10 SIZE: 0x4
		Material* material; //OFS: 0x14 SIZE: 0x4
		char lightmapIndex; //OFS: 0x18 SIZE: 0x1
		char reflectionProbeIndex; //OFS: 0x19 SIZE: 0x1
		char primaryLightIndex; //OFS: 0x1A SIZE: 0x1
		char flags; //OFS: 0x1B SIZE: 0x1
		float bounds[2][3]; //OFS: 0x1C SIZE: 0x18
	};

	struct GfxCullGroup
	{
		float mins[3]; //OFS: 0x0 SIZE: 0xC
		float maxs[3]; //OFS: 0xC SIZE: 0xC
		int surfaceCount; //OFS: 0x18 SIZE: 0x4
		int startSurfIndex; //OFS: 0x1C SIZE: 0x4
	};

	struct GfxWorldDpvsStatic
	{
		unsigned int smodelCount; //OFS: 0x0 SIZE: 0x4
		unsigned int staticSurfaceCount; //OFS: 0x4 SIZE: 0x4
		unsigned int litSurfsBegin; //OFS: 0x8 SIZE: 0x4
		unsigned int litSurfsEnd; //OFS: 0xC SIZE: 0x4
		unsigned int decalSurfsBegin; //OFS: 0x10 SIZE: 0x4
		unsigned int decalSurfsEnd; //OFS: 0x14 SIZE: 0x4
		unsigned int emissiveSurfsBegin; //OFS: 0x18 SIZE: 0x4
		unsigned int emissiveSurfsEnd; //OFS: 0x1C SIZE: 0x4
		unsigned int smodelVisDataCount; //OFS: 0x20 SIZE: 0x4
		unsigned int surfaceVisDataCount; //OFS: 0x24 SIZE: 0x4
		unsigned __int8* smodelVisData[3]; //OFS: 0x28 SIZE: 0xC
		unsigned __int8* surfaceVisData[3]; //OFS: 0x34 SIZE: 0xC
		unsigned int* lodData; //OFS: 0x40 SIZE: 0x4
		unsigned __int16* sortedSurfIndex; //OFS: 0x44 SIZE: 0x4
		GfxStaticModelInst* smodelInsts; //OFS: 0x48 SIZE: 0x4
		GfxSurface* surfaces; //OFS: 0x4C SIZE: 0x4
		GfxCullGroup* cullGroups; //OFS: 0x50 SIZE: 0x4
		GfxStaticModelDrawInst* smodelDrawInsts; //OFS: 0x54 SIZE: 0x4
		GfxDrawSurf* surfaceMaterials; //OFS: 0x58 SIZE: 0x4
		unsigned int* surfaceCastsSunShadow; //OFS: 0x5C SIZE: 0x4
		int usageCount; //OFS: 0x60 SIZE: 0x4
	};

	struct GfxWorldVertex
	{
		float xyz[3]; //OFS: 0x0 SIZE: 0xC
		float binormalSign; //OFS: 0xC SIZE: 0x4
		GfxColor color; //OFS: 0x10 SIZE: 0x4
		float texCoord[2]; //OFS: 0x14 SIZE: 0x8
		float lmapCoord[2]; //OFS: 0x1C SIZE: 0x8
		PackedUnitVec normal; //OFS: 0x24 SIZE: 0x4
		PackedUnitVec tangent; //OFS: 0x28 SIZE: 0x4
	};

	struct GfxVertexBuffer
	{
		IDirect3DVertexBuffer9* data; //OFS: 0x0 SIZE: 0x4
	};

	struct GfxWorldVertexData
	{
		GfxWorldVertex* vertices; //OFS: 0x0 SIZE: 0x4
		GfxVertexBuffer worldVb; //OFS: 0x4 SIZE: 0x4
	};

	struct GfxWorldVertexLayerData
	{
		unsigned __int8* data; //OFS: 0x0 SIZE: 0x4
		GfxVertexBuffer layerVb; //OFS: 0x4 SIZE: 0x4
	};

	struct SunLightParseParams
	{
		char name[64]; //OFS: 0x0 SIZE: 0x40
		float ambientScale; //OFS: 0x40 SIZE: 0x4
		float ambientColor[3]; //OFS: 0x44 SIZE: 0xC
		float diffuseFraction; //OFS: 0x50 SIZE: 0x4
		float sunLight; //OFS: 0x54 SIZE: 0x4
		float sunColor[3]; //OFS: 0x58 SIZE: 0xC
		float diffuseColor[3]; //OFS: 0x64 SIZE: 0xC
		bool diffuseColorHasBeenSet; //OFS: 0x70 SIZE: 0x1
		float angles[3]; //OFS: 0x74 SIZE: 0xC
		float treeScatterIntensity; //OFS: 0x80 SIZE: 0x4
		float treeScatterAmount; //OFS: 0x84 SIZE: 0x4
	};

	struct GfxLightImage
	{
		GfxImage* image; //OFS: 0x0 SIZE: 0x4
		unsigned __int8 samplerState; //OFS: 0x4 SIZE: 0x1
	};

	struct GfxLightDef
	{
		char* name; //OFS: 0x0 SIZE: 0x4
		GfxLightImage attenuation; //OFS: 0x4 SIZE: 0x8
		int lmapLookupStart; //OFS: 0xC SIZE: 0x4
	};

	struct GfxLight
	{
		unsigned __int8 type; //OFS: 0x0 SIZE: 0x1
		unsigned __int8 canUseShadowMap; //OFS: 0x1 SIZE: 0x1
		__int16 cullDist; //OFS: 0x2 SIZE: 0x2
		float color[3]; //OFS: 0x4 SIZE: 0xC
		float dir[3]; //OFS: 0x10 SIZE: 0xC
		float origin[3]; //OFS: 0x1C SIZE: 0xC
		float radius; //OFS: 0x28 SIZE: 0x4
		float cosHalfFovOuter; //OFS: 0x2C SIZE: 0x4
		float cosHalfFovInner; //OFS: 0x30 SIZE: 0x4
		int exponent; //OFS: 0x34 SIZE: 0x4
		unsigned int spotShadowIndex; //OFS: 0x38 SIZE: 0x4
		GfxLightDef* def; //OFS: 0x3C SIZE: 0x4
	};

	struct GfxReflectionProbe
	{
		float origin[3]; //OFS: 0x0 SIZE: 0xC
		GfxImage* reflectionImage; //OFS: 0xC SIZE: 0x4
	};

	struct GfxLightCorona
	{
		float origin[3]; //OFS: 0x0 SIZE: 0xC
		float radius; //OFS: 0xC SIZE: 0x4
		float color[3]; //OFS: 0x10 SIZE: 0xC
		float intensity; //OFS: 0x1C SIZE: 0x4
	};

	struct GfxWorldDpvsPlanes
	{
		int cellCount; //OFS: 0x0 SIZE: 0x4
		cplane_s* planes; //OFS: 0x4 SIZE: 0x4
		unsigned __int16* nodes; //OFS: 0x8 SIZE: 0x4
		unsigned int* sceneEntCellBits; //OFS: 0xC SIZE: 0x4
	};

	struct GfxAabbTree
	{
		float mins[3]; //OFS: 0x0 SIZE: 0xC
		float maxs[3]; //OFS: 0xC SIZE: 0xC
		unsigned __int16 childCount; //OFS: 0x18 SIZE: 0x2
		unsigned __int16 surfaceCount; //OFS: 0x1A SIZE: 0x2
		unsigned __int16 startSurfIndex; //OFS: 0x1C SIZE: 0x2
		unsigned __int16 smodelIndexCount; //OFS: 0x1E SIZE: 0x2
		unsigned __int16* smodelIndexes; //OFS: 0x20 SIZE: 0x4
		int childrenOffset; //OFS: 0x24 SIZE: 0x4
	};

	struct GfxCell;
	struct GfxPortal;

	struct DpvsPlane
	{
		float coeffs[4]; //OFS: 0x0 SIZE: 0x10
		unsigned __int8 side[3]; //OFS: 0x10 SIZE: 0x3
		unsigned __int8 pad; //OFS: 0x13 SIZE: 0x1
	};

	struct GfxPortalWritable
	{
		bool isQueued; //OFS: 0x0 SIZE: 0x1
		bool isAncestor; //OFS: 0x1 SIZE: 0x1
		unsigned __int8 recursionDepth; //OFS: 0x2 SIZE: 0x1
		unsigned __int8 hullPointCount; //OFS: 0x3 SIZE: 0x1
		float(*hullPoints)[2]; //OFS: 0x4 SIZE: 0x4
		GfxPortal* queuedParent; //OFS: 0x8 SIZE: 0x4
	};

	struct GfxPortal
	{
		GfxPortalWritable writable; //OFS: 0x0 SIZE: 0xC
		DpvsPlane plane; //OFS: 0xC SIZE: 0x14
		GfxCell* cell; //OFS: 0x20 SIZE: 0x4
		float(*vertices)[3]; //OFS: 0x24 SIZE: 0x4
		unsigned __int8 vertexCount; //OFS: 0x28 SIZE: 0x1
		float hullAxis[2][3]; //OFS: 0x2C SIZE: 0x18
	};

	struct GfxCell
	{
		float mins[3]; //OFS: 0x0 SIZE: 0xC
		float maxs[3]; //OFS: 0xC SIZE: 0xC
		int aabbTreeCount; //OFS: 0x18 SIZE: 0x4
		GfxAabbTree* aabbTree; //OFS: 0x1C SIZE: 0x4
		int portalCount; //OFS: 0x20 SIZE: 0x4
		GfxPortal* portals; //OFS: 0x24 SIZE: 0x4
		int cullGroupCount; //OFS: 0x28 SIZE: 0x4
		int* cullGroups; //OFS: 0x2C SIZE: 0x4
		unsigned __int8 reflectionProbeCount; //OFS: 0x30 SIZE: 0x1
		unsigned __int8* reflectionProbes; //OFS: 0x34 SIZE: 0x4
	};

	struct __declspec(align(2)) GfxLightGridEntry
	{
		unsigned __int16 colorsIndex; //OFS: 0x0 SIZE: 0x2
		unsigned __int8 primaryLightIndex; //OFS: 0x2 SIZE: 0x1
		unsigned __int8 needsTrace; //OFS: 0x3 SIZE: 0x1
	};

	struct __declspec(align(1)) GfxLightGridColors
	{
		unsigned __int8 rgb[56][3]; //OFS: 0x0 SIZE: 0xA8
	};

	struct GfxLightGrid
	{
		bool hasLightRegions; //OFS: 0x0 SIZE: 0x1
		unsigned int sunPrimaryLightIndex; //OFS: 0x4 SIZE: 0x4
		unsigned __int16 mins[3]; //OFS: 0x8 SIZE: 0x6
		unsigned __int16 maxs[3]; //OFS: 0xE SIZE: 0x6
		unsigned int rowAxis; //OFS: 0x14 SIZE: 0x4
		unsigned int colAxis; //OFS: 0x18 SIZE: 0x4
		unsigned __int16* rowDataStart; //OFS: 0x1C SIZE: 0x4
		unsigned int rawRowDataSize; //OFS: 0x20 SIZE: 0x4
		unsigned __int8* rawRowData; //OFS: 0x24 SIZE: 0x4
		unsigned int entryCount; //OFS: 0x28 SIZE: 0x4
		GfxLightGridEntry* entries; //OFS: 0x2C SIZE: 0x4
		unsigned int colorCount; //OFS: 0x30 SIZE: 0x4
		GfxLightGridColors* colors; //OFS: 0x34 SIZE: 0x4
	};

	struct _GXTlutObj
	{
		unsigned int dummy[3]; //OFS: 0x0 SIZE: 0xC
	};

	struct nglPalette
	{
		_GXTlutObj TlutObj; //OFS: 0x0 SIZE: 0xC
		unsigned int PaletteFormat; //OFS: 0xC SIZE: 0x4
		unsigned int PaletteSize; //OFS: 0x10 SIZE: 0x4
		unsigned __int8* PaletteData; //OFS: 0x14 SIZE: 0x4
		bool InPlace; //OFS: 0x18 SIZE: 0x1
		bool Dirty; //OFS: 0x19 SIZE: 0x1
	};

	struct _GXTexObj
	{
		unsigned int dummy[8]; //OFS: 0x0 SIZE: 0x20
	};

	struct gpuTexture
	{
		unsigned __int16 Width; //OFS: 0x0 SIZE: 0x2
		unsigned __int16 Height; //OFS: 0x2 SIZE: 0x2
		unsigned __int8 TexelFormat; //OFS: 0x4 SIZE: 0x1
		unsigned __int8 Levels; //OFS: 0x5 SIZE: 0x1
		unsigned __int16 Dummy; //OFS: 0x6 SIZE: 0x2
		void* Data; //OFS: 0x8 SIZE: 0x4
		nglPalette* Palette; //OFS: 0xC SIZE: 0x4
		_GXTexObj texObj; //OFS: 0x10 SIZE: 0x20
	};

	struct GfxBrushModelWritable
	{
		float mins[3]; //OFS: 0x0 SIZE: 0xC
		float maxs[3]; //OFS: 0xC SIZE: 0xC
	};

	struct GfxBrushModel
	{
		GfxBrushModelWritable writable; //OFS: 0x0 SIZE: 0x18
		float bounds[2][3]; //OFS: 0x18 SIZE: 0x18
		unsigned int surfaceCount; //OFS: 0x30 SIZE: 0x4
		unsigned int startSurfIndex; //OFS: 0x34 SIZE: 0x4
	};

	struct MaterialMemory
	{
		Material* material; //OFS: 0x0 SIZE: 0x4
		int memory; //OFS: 0x4 SIZE: 0x4
	};

	struct sunflare_t
	{
		bool hasValidData; //OFS: 0x0 SIZE: 0x1
		Material* spriteMaterial; //OFS: 0x4 SIZE: 0x4
		Material* flareMaterial; //OFS: 0x8 SIZE: 0x4
		float spriteSize; //OFS: 0xC SIZE: 0x4
		float flareMinSize; //OFS: 0x10 SIZE: 0x4
		float flareMinDot; //OFS: 0x14 SIZE: 0x4
		float flareMaxSize; //OFS: 0x18 SIZE: 0x4
		float flareMaxDot; //OFS: 0x1C SIZE: 0x4
		float flareMaxAlpha; //OFS: 0x20 SIZE: 0x4
		int flareFadeInTime; //OFS: 0x24 SIZE: 0x4
		int flareFadeOutTime; //OFS: 0x28 SIZE: 0x4
		float blindMinDot; //OFS: 0x2C SIZE: 0x4
		float blindMaxDot; //OFS: 0x30 SIZE: 0x4
		float blindMaxDarken; //OFS: 0x34 SIZE: 0x4
		int blindFadeInTime; //OFS: 0x38 SIZE: 0x4
		int blindFadeOutTime; //OFS: 0x3C SIZE: 0x4
		float glareMinDot; //OFS: 0x40 SIZE: 0x4
		float glareMaxDot; //OFS: 0x44 SIZE: 0x4
		float glareMaxLighten; //OFS: 0x48 SIZE: 0x4
		int glareFadeInTime; //OFS: 0x4C SIZE: 0x4
		int glareFadeOutTime; //OFS: 0x50 SIZE: 0x4
		float sunFxPosition[3]; //OFS: 0x54 SIZE: 0xC
	};

	struct __declspec(align(2)) XModelDrawInfo
	{
		unsigned __int16 lod; //OFS: 0x0 SIZE: 0x2
		unsigned __int16 surfId; //OFS: 0x2 SIZE: 0x2
	};

	struct __declspec(align(2)) GfxSceneDynModel
	{
		XModelDrawInfo info; //OFS: 0x0 SIZE: 0x4
		unsigned __int16 dynEntId; //OFS: 0x4 SIZE: 0x2
	};

	struct __declspec(align(2)) BModelDrawInfo
	{
		unsigned __int16 surfId; //OFS: 0x0 SIZE: 0x2
	};

	struct __declspec(align(2)) GfxSceneDynBrush
	{
		BModelDrawInfo info; //OFS: 0x0 SIZE: 0x2
		unsigned __int16 dynEntId; //OFS: 0x2 SIZE: 0x2
	};

	struct GfxShadowGeometry
	{
		unsigned __int16 surfaceCount; //OFS: 0x0 SIZE: 0x2
		unsigned __int16 smodelCount; //OFS: 0x2 SIZE: 0x2
		unsigned __int16* sortedSurfIndex; //OFS: 0x4 SIZE: 0x4
		unsigned __int16* smodelIndex; //OFS: 0x8 SIZE: 0x4
	};

	struct GfxLightRegionAxis
	{
		float dir[3]; //OFS: 0x0 SIZE: 0xC
		float midPoint; //OFS: 0xC SIZE: 0x4
		float halfSize; //OFS: 0x10 SIZE: 0x4
	};

	struct GfxLightRegionHull
	{
		float kdopMidPoint[9]; //OFS: 0x0 SIZE: 0x24
		float kdopHalfSize[9]; //OFS: 0x24 SIZE: 0x24
		unsigned int axisCount; //OFS: 0x48 SIZE: 0x4
		GfxLightRegionAxis* axis; //OFS: 0x4C SIZE: 0x4
	};

	struct GfxLightRegion
	{
		unsigned int hullCount; //OFS: 0x0 SIZE: 0x4
		GfxLightRegionHull* hulls; //OFS: 0x4 SIZE: 0x4
	};

	struct GfxWorldDpvsDynamic
	{
		unsigned int dynEntClientWordCount[2]; //OFS: 0x0 SIZE: 0x8
		unsigned int dynEntClientCount[2]; //OFS: 0x8 SIZE: 0x8
		unsigned int* dynEntCellBits[2]; //OFS: 0x10 SIZE: 0x8
		unsigned __int8* dynEntVisData[2][3]; //OFS: 0x18 SIZE: 0x18
	};

	struct GfxWorldLodChain
	{
		float origin[3]; //OFS: 0x0 SIZE: 0xC
		float lastDist; //OFS: 0xC SIZE: 0x4
		unsigned int firstLodInfo; //OFS: 0x10 SIZE: 0x4
		unsigned __int16 lodInfoCount; //OFS: 0x14 SIZE: 0x2
	};

	struct GfxWorldLodInfo
	{
		float dist; //OFS: 0x0 SIZE: 0x4
		unsigned int firstSurf; //OFS: 0x4 SIZE: 0x4
		unsigned __int16 surfCount; //OFS: 0x8 SIZE: 0x2
	};

	struct GfxWaterBuffer
	{
		unsigned int bufferSize; //OFS: 0x0 SIZE: 0x4
		float(*buffer)[4]; //OFS: 0x4 SIZE: 0x4
	};

	struct GfxWorld
	{
		const char* name; //OFS: 0x0 SIZE: 0x4
		const char* baseName; //OFS: 0x4 SIZE: 0x4
		int planeCount; //OFS: 0x8 SIZE: 0x4
		int nodeCount; //OFS: 0xC SIZE: 0x4
		int indexCount; //OFS: 0x10 SIZE: 0x4
		unsigned __int16* indices; //OFS: 0x14 SIZE: 0x4
		int surfaceCount; //OFS: 0x18 SIZE: 0x4
		int streamInfo; //OFS: 0x1C SIZE: 0x4
		int skySurfCount; //OFS: 0x20 SIZE: 0x4
		int* skyStartSurfs; //OFS: 0x24 SIZE: 0x4
		GfxImage* skyImage; //OFS: 0x28 SIZE: 0x4
		unsigned __int8 skySamplerState; //OFS: 0x2C SIZE: 0x1
		char* skyBoxModel; //OFS: 0x30 SIZE: 0x4
		unsigned int vertexCount; //OFS: 0x34 SIZE: 0x4
		GfxWorldVertexData vd; //OFS: 0x38 SIZE: 0x8
		unsigned int vertexLayerDataSize; //OFS: 0x40 SIZE: 0x4
		GfxWorldVertexLayerData vld; //OFS: 0x44 SIZE: 0x8
		unsigned int vertexStream2DataSize; //OFS: 0x4C SIZE: 0x4
		SunLightParseParams sunParse; //OFS: 0x50 SIZE: 0x88
		GfxLight* sunLight; //OFS: 0xD8 SIZE: 0x4
		float sunColorFromBsp[3]; //OFS: 0xDC SIZE: 0xC
		unsigned int sunPrimaryLightIndex; //OFS: 0xE8 SIZE: 0x4
		unsigned int primaryLightCount; //OFS: 0xEC SIZE: 0x4
		int cullGroupCount; //OFS: 0xF0 SIZE: 0x4
		unsigned int reflectionProbeCount; //OFS: 0xF4 SIZE: 0x4
		GfxReflectionProbe* reflectionProbes; //OFS: 0xF8 SIZE: 0x4
		GfxTexture* reflectionProbeTextures; //OFS: 0xFC SIZE: 0x4
		unsigned int coronaCount; //OFS: 0x100 SIZE: 0x4
		GfxLightCorona* coronas; //OFS: 0x104 SIZE: 0x4
		GfxWorldDpvsPlanes dpvsPlanes; //OFS: 0x108 SIZE: 0x10
		int cellBitsCount; //OFS: 0x118 SIZE: 0x4
		GfxCell* cells; //OFS: 0x11C SIZE: 0x4
		int lightmapCount; //OFS: 0x120 SIZE: 0x4
		GfxImage** lightmaps; //OFS: 0x124 SIZE: 0x4
		GfxLightGrid lightGrid; //OFS: 0x128 SIZE: 0x38
		gpuTexture* lightmapPrimaryTextures; //OFS: 0x160 SIZE: 0x4
		gpuTexture* lightmapSecondaryTextures; //OFS: 0x164 SIZE: 0x4
		int modelCount; //OFS: 0x168 SIZE: 0x4
		GfxBrushModel* models; //OFS: 0x16C SIZE: 0x4
		float mins[3]; //OFS: 0x170 SIZE: 0xC
		float maxs[3]; //OFS: 0x17C SIZE: 0xC
		unsigned int checksum; //OFS: 0x188 SIZE: 0x4
		int materialMemoryCount; //OFS: 0x18C SIZE: 0x4
		MaterialMemory* materialMemory; //OFS: 0x190 SIZE: 0x4
		sunflare_t sun; //OFS: 0x194 SIZE: 0x60
		float outdoorLookupMatrix[4][4]; //OFS: 0x1F4 SIZE: 0x40
		GfxImage* outdoorImage; //OFS: 0x234 SIZE: 0x4
		unsigned int* cellCasterBits; //OFS: 0x238 SIZE: 0x4
		GfxSceneDynModel* sceneDynModel; //OFS: 0x23C SIZE: 0x4
		GfxSceneDynBrush* sceneDynBrush; //OFS: 0x240 SIZE: 0x4
		unsigned int* primaryLightEntityShadowVis; //OFS: 0x244 SIZE: 0x4
		unsigned int* primaryLightDynEntShadowVis[2]; //OFS: 0x248 SIZE: 0x8
		char* nonSunPrimaryLightForModelDynEnt; //OFS: 0x250 SIZE: 0x4
		GfxShadowGeometry* shadowGeom; //OFS: 0x254 SIZE: 0x4
		GfxLightRegion* lightRegion; //OFS: 0x258 SIZE: 0x4
		GfxWorldDpvsStatic dpvs; //OFS: 0x25C SIZE: 0x64
		GfxWorldDpvsDynamic dpvsDyn; //OFS: 0x2C0 SIZE: 0x30
		unsigned int worldLodChainCount; //OFS: 0x2F0 SIZE: 0x4
		GfxWorldLodChain* worldLodChains; //OFS: 0x2F4 SIZE: 0x4
		unsigned int worldLodInfoCount; //OFS: 0x2F8 SIZE: 0x4
		GfxWorldLodInfo* worldLodInfos; //OFS: 0x2FC SIZE: 0x4
		unsigned int worldLodSurfaceCount; //OFS: 0x300 SIZE: 0x4
		unsigned int* worldLodSurfaces; //OFS: 0x304 SIZE: 0x4
		float waterDirection; //OFS: 0x308 SIZE: 0x4
		GfxWaterBuffer waterBuffers[2]; //OFS: 0x30C SIZE: 0x10
		Material* waterMaterial; //OFS: 0x31C SIZE: 0x4
	};

	struct ComPrimaryLight
	{
		unsigned __int8 type;
		unsigned __int8 canUseShadowMap;
		unsigned __int8 exponent;
		char priority;
		__int16 cullDist;
		float color[3];
		float dir[3];
		float origin[3];
		float radius;
		float cosHalfFovOuter;
		float cosHalfFovInner;
		float cosHalfFovExpanded;
		float rotationLimit;
		float translationLimit;
		char* defName;
	};

	struct ComWorld
	{
		char* name;
		int isInUse;
		unsigned int primaryLightCount;
		ComPrimaryLight* primaryLights;
		// ...
	};

	struct SavedScreenParams
	{
		float s0; //OFS: 0x0 SIZE: 0x4
		float t0; //OFS: 0x4 SIZE: 0x4
		float _ds; //OFS: 0x8 SIZE: 0x4
		float dt; //OFS: 0xC SIZE: 0x4
		int isSet; //OFS: 0x10 SIZE: 0x4
	};

	struct r_global_permanent_t
	{
		Material* sortedMaterials[2048]; //OFS: 0x0 SIZE: 0x2000
		int needSortMaterials; //OFS: 0x2000 SIZE: 0x4
		int materialCount; //OFS: 0x2004 SIZE: 0x4
		int needMaterialPreload; //OFS: 0x2008 SIZE: 0x4
		GfxImage* whiteImage; //OFS: 0x200C SIZE: 0x4
		GfxImage* blackImage; //OFS: 0x2010 SIZE: 0x4
		GfxImage* blankImage; //OFS: 0x2014 SIZE: 0x4
		GfxImage* grayImage; //OFS: 0x2018 SIZE: 0x4
		GfxImage* identityNormalMapImage; //OFS: 0x201C SIZE: 0x4
		GfxImage* specularityImage; //OFS: 0x2020 SIZE: 0x4
		GfxImage* outdoorImage; //OFS: 0x2024 SIZE: 0x4
		GfxImage* pixelCostColorCode; //OFS: 0x2028 SIZE: 0x4
		GfxLightDef* dlightDef; //OFS: 0x202C SIZE: 0x4
		Material* defaultMaterial; //OFS: 0x2030 SIZE: 0x4
		Material* whiteMaterial; //OFS: 0x2034 SIZE: 0x4
		Material* additiveMaterial; //OFS: 0x2038 SIZE: 0x4
		Material* additiveMaterialNoDepth; //OFS: 0x203C SIZE: 0x4
		Material* pointMaterial; //OFS: 0x2040 SIZE: 0x4
		Material* lineMaterial; //OFS: 0x2044 SIZE: 0x4
		Material* lineMaterialNoDepth; //OFS: 0x2048 SIZE: 0x4
		Material* clearAlphaMaterial; //OFS: 0x204C SIZE: 0x4
		Material* clearAlphaStencilMaterial; //OFS: 0x2050 SIZE: 0x4
		Material* shadowClearMaterial; //OFS: 0x2054 SIZE: 0x4
		Material* shadowCookieOverlayMaterial; //OFS: 0x2058 SIZE: 0x4
		Material* shadowCookieBlurMaterial; //OFS: 0x205C SIZE: 0x4
		Material* shadowCasterMaterial; //OFS: 0x2060 SIZE: 0x4
		Material* shadowOverlayMaterial; //OFS: 0x2064 SIZE: 0x4
		Material* depthPrepassMaterial; //OFS: 0x2068 SIZE: 0x4
		Material* glareBlindMaterial; //OFS: 0x206C SIZE: 0x4
		Material* lightCoronaMaterial; //OFS: 0x2070 SIZE: 0x4
		Material* pixelCostAddDepthAlwaysMaterial; //OFS: 0x2074 SIZE: 0x4
		Material* pixelCostAddDepthDisableMaterial; //OFS: 0x2078 SIZE: 0x4
		Material* pixelCostAddDepthEqualMaterial; //OFS: 0x207C SIZE: 0x4
		Material* pixelCostAddDepthLessMaterial; //OFS: 0x2080 SIZE: 0x4
		Material* pixelCostAddDepthWriteMaterial; //OFS: 0x2084 SIZE: 0x4
		Material* pixelCostAddNoDepthWriteMaterial; //OFS: 0x2088 SIZE: 0x4
		Material* pixelCostColorCodeMaterial; //OFS: 0x208C SIZE: 0x4
		Material* stencilShadowMaterial; //OFS: 0x2090 SIZE: 0x4
		Material* stencilDisplayMaterial; //OFS: 0x2094 SIZE: 0x4
		Material* floatZDisplayMaterial; //OFS: 0x2098 SIZE: 0x4
		Material* colorChannelMixerMaterial; //OFS: 0x209C SIZE: 0x4
		Material* frameColorDebugMaterial; //OFS: 0x20A0 SIZE: 0x4
		Material* frameAlphaDebugMaterial; //OFS: 0x20A4 SIZE: 0x4
		GfxImage* rawImage; //OFS: 0x20A8 SIZE: 0x4
		GfxWorld* world; //OFS: 0x20AC SIZE: 0x4
		Material* feedbackReplaceMaterial; //OFS: 0x20B0 SIZE: 0x4
		Material* feedbackBlendMaterial; //OFS: 0x20B4 SIZE: 0x4
		Material* feedbackFilmBlendMaterial; //OFS: 0x20B8 SIZE: 0x4
		Material* cinematicMaterial; //OFS: 0x20BC SIZE: 0x4
		Material* flameThrowerFXMaterial; //OFS: 0x20C0 SIZE: 0x4
		Material* waterSheetingFXMaterial; //OFS: 0x20C4 SIZE: 0x4
		Material* waterDropletMaterial; //OFS: 0x20C8 SIZE: 0x4
		Material* reviveFXMaterial; //OFS: 0x20CC SIZE: 0x4
		Material* postFxColorMaterial; //OFS: 0x20D0 SIZE: 0x4
		Material* postFxMaterial; //OFS: 0x20D4 SIZE: 0x4
		Material* poisonFXMaterial; //OFS: 0x20D8 SIZE: 0x4
		Material* symmetricFilterMaterial[8]; //OFS: 0x20DC SIZE: 0x20
		Material* godRaysFilterMaterial; //OFS: 0x20FC SIZE: 0x4
		Material* shellShockBlurredMaterial; //OFS: 0x2100 SIZE: 0x4
		Material* shellShockFlashedMaterial; //OFS: 0x2104 SIZE: 0x4
		Material* glowConsistentSetupMaterial; //OFS: 0x2108 SIZE: 0x4
		Material* glowApplyBloomMaterial; //OFS: 0x210C SIZE: 0x4
		Material* ropeMaterial; //OFS: 0x2110 SIZE: 0x4
		int savedScreenTimes[4]; //OFS: 0x2114 SIZE: 0x10
		SavedScreenParams savedScreenParams[4]; //OFS: 0x2124 SIZE: 0x50
		GfxImage* terrain_scorch_images[31]; //OFS: 0x2174 SIZE: 0x7C
		GfxImage* splitscreen_sidebars; //OFS: 0x21F0 SIZE: 0x4
		char pad[140]; //OFS: 0x21F4 SIZE: 0x8C
	};

	struct GfxCmdBufPrimState
	{
		IDirect3DDevice9* device;
		IDirect3DIndexBuffer9* indexBuffer;
		MaterialVertexDeclType vertDeclType;
		int pad;
		gfxVertexSteamsUnk streams[3];
		IDirect3DVertexDeclaration9* vertexDecl;
	};
	STATIC_ASSERT_SIZE(GfxCmdBufPrimState, 0x38);


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

	struct GfxDrawPrimArgs
	{
		int vertexCount;
		int triCount;
		int baseIndex;
	};

	struct GfxCmdBufState
	{
		char refSamplerState[16];
		unsigned int samplerState[16];
		GfxTexture* samplerTexture[16];
		GfxCmdBufPrimState prim;
		Material* material;
		MaterialTechniqueType techType;
		MaterialTechnique* technique;
		MaterialPass* pass;
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

	struct GfxDynamicIndices
	{
		volatile int used;
		int total;
		unsigned __int16* indices;
	};

	struct GfxIndexBufferState
	{
		volatile int used;
		int total;
		IDirect3DIndexBuffer9* buffer;
		unsigned __int16* indices;
	};

	struct __declspec(align(4)) GfxBuffers
	{
		GfxDynamicIndices smodelCache;
		IDirect3DVertexBuffer9* smodelCacheVb;
		GfxIndexBufferState preTessIndexBufferPool[2];
		GfxIndexBufferState* preTessIndexBuffer;
		int preTessBufferFrame;
		GfxIndexBufferState dynamicIndexBufferPool[1];
		GfxIndexBufferState* dynamicIndexBuffer;
		GfxVertexBufferState skinnedCacheVbPool[2];
		char* skinnedCacheLockAddr;
		GfxVertexBufferState dynamicVertexBufferPool[1];
		GfxVertexBufferState* dynamicVertexBuffer;
	}; STATIC_ASSERT_OFFSET(GfxBuffers, dynamicVertexBuffer, 0x80);

	struct BModelSurface
	{
		GfxScaledPlacement* placement;
		GfxSurface* surf;
	};

	struct GfxCmdBufContext
	{
		GfxCmdBufSourceState* source;
		GfxCmdBufState* state;
	};

	struct GfxDrawSurfListInfo
	{
		GfxDrawSurf* drawSurfs;
		unsigned int drawSurfCount;
		MaterialTechniqueType baseTechType;
		GfxViewInfo* viewInfo;
		float viewOrigin[4];
		void* GfxLight__light;
		int cameraView;
	};

	struct GfxDrawSurfListArgs
	{
		GfxCmdBufContext context;
		unsigned int firstDrawSurfIndex;
		GfxDrawSurfListInfo* info;
	};


	struct MapEnts
	{
		char* name;
		char* entityString;
		int numEntityChars;
	};

	struct cStaticModel_s
	{
		cStaticModelWritable writable;
		XModel* xmodel;
		float origin[3];
		float invScaledAxis[3][3];
		float absmin[3];
		float absmax[3];
	};

	struct clipMap_t
	{
		char* name;
		int isInUse;
		int planeCount;
		void* cplane_s___planes;
		unsigned int numStaticModels;
		cStaticModel_s* staticModelList;
		unsigned int numMaterials;
		void* dmaterial_t___materials;
		unsigned int numBrushSides;
		void* cbrushside_t___brushsides;
		unsigned int numBrushEdges;
		unsigned __int8* brushEdges;
		unsigned int numNodes;
		void* cNode_t___nodes;
		unsigned int numLeafs;
		void* cLeaf_s___leafs;
		unsigned int leafbrushNodesCount;
		void* cLeafBrushNode_s___leafbrushNodes;
		unsigned int numLeafBrushes;
		unsigned __int16* leafbrushes;
		unsigned int numLeafSurfaces;
		unsigned int* leafsurfaces;
		unsigned int vertCount;
		float(*verts)[3];
		unsigned int numBrushVerts;
		float(*brushVerts)[3];
		unsigned int nuinds;
		unsigned __int16* uinds;
		int triCount;
		unsigned __int16* triIndices;
		unsigned __int8* triEdgeIsWalkable;
		int borderCount;
		void* CollisionBorder___borders;
		int partitionCount;
		void* CollisionPartition___partitions;
		int aabbTreeCount;
		void* CollisionAabbTree___aabbTrees;
		unsigned int numSubModels;
		void* cmodel_t___cmodels;
		unsigned __int16 numBrushes;
		void* cbrush_t___brushes;
		int numClusters;
		int clusterBytes;
		unsigned __int8* visibility;
		int vised;
		MapEnts* mapEnts;
		void* cbrush_t___box_brush;
		// .....
	}; STATIC_ASSERT_OFFSET(clipMap_t, mapEnts, 0xB4);

	struct __declspec(align(2)) XAnimParent
	{
		unsigned __int16 flags; //OFS: 0x0 SIZE: 0x2
		unsigned __int16 children; //OFS: 0x2 SIZE: 0x2
	};

	struct XAnimEntry
	{
		unsigned __int16 bCreated; //OFS: 0x0 SIZE: 0x2
		unsigned __int16 numAnims; //OFS: 0x2 SIZE: 0x2
		unsigned __int16 parent; //OFS: 0x4 SIZE: 0x2
		__int16 field_6; //OFS: 0x6 SIZE: 0x2
		XAnimParent animParent; //OFS: 0x8 SIZE: 0x4
	};

	struct XAnim_s
	{
		char* debugName; //OFS: 0x0 SIZE: 0x4
		unsigned int size; //OFS: 0x4 SIZE: 0x4
		char** debugAnimNames; //OFS: 0x8 SIZE: 0x4
		XAnimEntry entries[1]; //OFS: 0xC SIZE: 0xC
	};

	struct XAnimTree_s
	{
		XAnim_s* anims; //OFS: 0x0 SIZE: 0x4
		unsigned __int16 children; //OFS: 0x4 SIZE: 0x2
	};

	union XAnimIndices
	{
		char* _1; //OFS: 0x0 SIZE: 0x4
		unsigned __int16* _2; //OFS: 0x1 SIZE: 0x4
		void* data; //OFS: 0x2 SIZE: 0x4
	};

	struct XAnimNotifyInfo
	{
		unsigned __int16 name; //OFS: 0x0 SIZE: 0x2
		float time; //OFS: 0x4 SIZE: 0x4
	};

	union XAnimDynamicFrames
	{
		unsigned __int8(*_1)[3]; //OFS: 0x0 SIZE: 0x4
		unsigned __int16(*_2)[3]; //OFS: 0x1 SIZE: 0x4
	};

	union XAnimDynamicIndices
	{
		char _1[1]; //OFS: 0x0 SIZE: 0x1
		unsigned __int16 _2[1]; //OFS: 0x1 SIZE: 0x2
	};

	struct XAnimPartTransFrames
	{
		float mins[3]; //OFS: 0x0 SIZE: 0xC
		float size[3]; //OFS: 0xC SIZE: 0xC
		XAnimDynamicFrames frames; //OFS: 0x18 SIZE: 0x4
		XAnimDynamicIndices indices; //OFS: 0x1C SIZE: 0x2
	};

	union XAnimPartTransData
	{
		XAnimPartTransFrames frames; //OFS: 0x0 SIZE: 0x20
		float frame0[3]; //OFS: 0x1 SIZE: 0xC
	};

	struct XAnimPartTrans
	{
		unsigned __int16 size; //OFS: 0x0 SIZE: 0x2
		char smallTrans; //OFS: 0x2 SIZE: 0x1
		XAnimPartTransData u; //OFS: 0x4 SIZE: 0x20
	};

	struct XAnimDeltaPartQuatDataFrames
	{
		__int16(*frames)[2]; //OFS: 0x0 SIZE: 0x4
		XAnimDynamicIndices indices; //OFS: 0x4 SIZE: 0x2
	};

	union XAnimDeltaPartQuatData
	{
		XAnimDeltaPartQuatDataFrames frames; //OFS: 0x0 SIZE: 0x8
		__int16 frame0[2]; //OFS: 0x1 SIZE: 0x4
	};

	struct XAnimDeltaPartQuat
	{
		unsigned __int16 size; //OFS: 0x0 SIZE: 0x2
		XAnimDeltaPartQuatData u; //OFS: 0x4 SIZE: 0x8
	};

	struct XAnimDeltaPart
	{
		XAnimPartTrans* trans; //OFS: 0x0 SIZE: 0x4
		XAnimDeltaPartQuat* quat; //OFS: 0x4 SIZE: 0x4
	};

	struct XAnimParts
	{
		const char* name; //OFS: 0x0 SIZE: 0x4
		unsigned __int16 dataByteCount; //OFS: 0x4 SIZE: 0x2
		unsigned __int16 dataShortCount; //OFS: 0x6 SIZE: 0x2
		unsigned __int16 dataIntCount; //OFS: 0x8 SIZE: 0x2
		unsigned __int16 randomDataByteCount; //OFS: 0xA SIZE: 0x2
		unsigned __int16 randomDataIntCount; //OFS: 0xC SIZE: 0x2
		unsigned __int16 numframes; //OFS: 0xE SIZE: 0x2
		bool bLoop; //OFS: 0x10 SIZE: 0x1
		bool bDelta; //OFS: 0x11 SIZE: 0x1
		char boneCount[10]; //OFS: 0x12 SIZE: 0xA
		char notifyCount; //OFS: 0x1C SIZE: 0x1
		char assetType; //OFS: 0x1D SIZE: 0x1
		bool isDefault; //OFS: 0x1E SIZE: 0x1
		unsigned int randomDataShortCount; //OFS: 0x20 SIZE: 0x4
		unsigned int indexCount; //OFS: 0x24 SIZE: 0x4
		float framerate; //OFS: 0x28 SIZE: 0x4
		float frequency; //OFS: 0x2C SIZE: 0x4
		unsigned __int16* names; //OFS: 0x30 SIZE: 0x4
		char* dataByte; //OFS: 0x34 SIZE: 0x4
		__int16* dataShort; //OFS: 0x38 SIZE: 0x4
		int* dataInt; //OFS: 0x3C SIZE: 0x4
		__int16* randomDataShort; //OFS: 0x40 SIZE: 0x4
		char* randomDataByte; //OFS: 0x44 SIZE: 0x4
		int* randomDataInt; //OFS: 0x48 SIZE: 0x4
		XAnimIndices indices; //OFS: 0x4C SIZE: 0x4
		XAnimNotifyInfo* notify; //OFS: 0x50 SIZE: 0x4
		XAnimDeltaPart* deltaPart; //OFS: 0x54 SIZE: 0x4
	};

	union XAnimParent_u
	{
		XAnimParts* parts; //OFS: 0x0 SIZE: 0x4
		XAnimParent animParent; //OFS: 0x1 SIZE: 0x4
	};

	struct XAnimState
	{
		float currentAnimTime; //OFS: 0x0 SIZE: 0x4
		float oldTime; //OFS: 0x4 SIZE: 0x4
		__int16 cycleCount; //OFS: 0x8 SIZE: 0x2
		__int16 oldCycleCount; //OFS: 0xA SIZE: 0x2
		float goalTime; //OFS: 0xC SIZE: 0x4
		float goalWeight; //OFS: 0x10 SIZE: 0x4
		float weight; //OFS: 0x14 SIZE: 0x4
		float rate; //OFS: 0x18 SIZE: 0x4
		bool instantWeightChange; //OFS: 0x1C SIZE: 0x1
	};

	struct XAnimInfo
	{
		unsigned __int16 notifyChild; //OFS: 0x0 SIZE: 0x2
		__int16 notifyIndex; //OFS: 0x2 SIZE: 0x2
		unsigned __int16 notifyName; //OFS: 0x4 SIZE: 0x2
		unsigned __int16 notifyType; //OFS: 0x6 SIZE: 0x2
		unsigned __int16 prev; //OFS: 0x8 SIZE: 0x2
		unsigned __int16 next; //OFS: 0xA SIZE: 0x2
		unsigned __int16 children; //OFS: 0xC SIZE: 0x2
		unsigned __int16 parent; //OFS: 0xE SIZE: 0x2
		unsigned __int16 animIndex; //OFS: 0x10 SIZE: 0x2
		unsigned __int16 animToModel; //OFS: 0x12 SIZE: 0x2
		XAnimParent_u u; //OFS: 0x14 SIZE: 0x4
		XAnimState state; //OFS: 0x18 SIZE: 0x20
	};

	enum TraceHitType : __int32
	{
		TRACE_HITTYPE_NONE = 0x0,
		TRACE_HITTYPE_ENTITY = 0x1,
		TRACE_HITTYPE_DYNENT_MODEL = 0x2,
		TRACE_HITTYPE_DYNENT_BRUSH = 0x3,
	};

	struct trace_t
	{
		float normal[4]; //OFS: 0x0 SIZE: 0x10
		float fraction; //OFS: 0x10 SIZE: 0x4
		int surfaceFlags; //OFS: 0x14 SIZE: 0x4
		int contents; //OFS: 0x18 SIZE: 0x4
		char* material; //OFS: 0x1C SIZE: 0x4
		TraceHitType hitType; //OFS: 0x20 SIZE: 0x4
		unsigned __int16 hitId; //OFS: 0x24 SIZE: 0x2
		unsigned __int16 modelIndex; //OFS: 0x26 SIZE: 0x2
		unsigned __int16 partName; //OFS: 0x28 SIZE: 0x2
		unsigned __int16 boneIndex; //OFS: 0x2A SIZE: 0x2
		unsigned __int16 partGroup; //OFS: 0x2C SIZE: 0x2
		bool allsolid; //OFS: 0x2E SIZE: 0x1
		bool startsolid; //OFS: 0x2F SIZE: 0x1
		bool walkable; //OFS: 0x30 SIZE: 0x1
	};

	enum trType_t : __int32
	{
		TR_STATIONARY = 0x0,
		TR_INTERPOLATE = 0x1,
		TR_LINEAR = 0x2,
		TR_LINEAR_STOP = 0x3,
		TR_SINE = 0x4,
		TR_GRAVITY = 0x5,
		TR_ACCELERATE = 0x6,
		TR_DECELERATE = 0x7,
		TR_PHYSICS = 0x8,
		TR_XDOLL = 0x9,
		TR_FIRST_RAGDOLL = 0xA,
		TR_RAGDOLL = 0xA,
		TR_RAGDOLL_GRAVITY = 0xB,
		TR_RAGDOLL_INTERPOLATE = 0xC,
		TR_LAST_RAGDOLL = 0xC,
		TR_COUNT = 0xD,
	};

	struct trajectory_t
	{
		trType_t trType; //OFS: 0x0 SIZE: 0x4
		int trTime; //OFS: 0x4 SIZE: 0x4
		int trDuration; //OFS: 0x8 SIZE: 0x4
		float trBase[3]; //OFS: 0xC SIZE: 0xC
		float trDelta[3]; //OFS: 0x18 SIZE: 0xC
	};

	struct LerpEntityStateTurret
	{
		float gunAngles[3]; //OFS: 0x0 SIZE: 0xC
		unsigned __int8 overheating; //OFS: 0xC SIZE: 0x1
		float heatVal; //OFS: 0x10 SIZE: 0x4
	};

	struct LerpEntityStateLoopFx
	{
		float cullDist; //OFS: 0x0 SIZE: 0x4
		int period; //OFS: 0x4 SIZE: 0x4
	};

	union LerpEntityStateActor_unnamed_type_index
	{
		int actorNum; //OFS: 0x0 SIZE: 0x4
		int corpseNum; //OFS: 0x1 SIZE: 0x4
	};

	struct __declspec(align(2)) LerpEntityStateActor_unnamed_type_proneInfo
	{
		__int16 fBodyPitch; //OFS: 0x0 SIZE: 0x2
	};

	struct LerpEntityStateActor
	{
		LerpEntityStateActor_unnamed_type_index index; //OFS: 0x0 SIZE: 0x4
		int species; //OFS: 0x4 SIZE: 0x4
		LerpEntityStateActor_unnamed_type_proneInfo proneInfo; //OFS: 0x8 SIZE: 0x2
	};

	struct LerpEntityStatePrimaryLight
	{
		char colorAndExp[4]; //OFS: 0x0 SIZE: 0x4
		float intensity; //OFS: 0x4 SIZE: 0x4
		float radius; //OFS: 0x8 SIZE: 0x4
		float cosHalfFovOuter; //OFS: 0xC SIZE: 0x4
		float cosHalfFovInner; //OFS: 0x10 SIZE: 0x4
	};

	struct LerpEntityStatePlayer
	{
		float leanf; //OFS: 0x0 SIZE: 0x4
		int movementDir; //OFS: 0x4 SIZE: 0x4
		char vehicleType; //OFS: 0x8 SIZE: 0x1
		char vehicleAnimBoneIndex; //OFS: 0x9 SIZE: 0x1
		char vehicleSeat; //OFS: 0xA SIZE: 0x1
	};

	struct __declspec(align(2)) LerpEntityStateVehicleGunnerAngles
	{
		__int16 pitch; //OFS: 0x0 SIZE: 0x2
		__int16 yaw; //OFS: 0x2 SIZE: 0x2
	};

	union LerpEntityStateVehicleThrottle_u
	{
		__int16 throttle; //OFS: 0x0 SIZE: 0x2
		__int16 bodyPitch; //OFS: 0x1 SIZE: 0x2
	};

	struct LerpEntityStateVehicle
	{
		float steerYaw; //OFS: 0x0 SIZE: 0x4
		float bodyRoll; //OFS: 0x4 SIZE: 0x4
		float bodyPitch; //OFS: 0x8 SIZE: 0x4
		LerpEntityStateVehicleGunnerAngles gunnerAngles[4]; //OFS: 0xC SIZE: 0x10
		LerpEntityStateVehicleThrottle_u ___u3; //OFS: 0x1C SIZE: 0x2
		__int16 gunPitch; //OFS: 0x1E SIZE: 0x2
		__int16 gunYaw; //OFS: 0x20 SIZE: 0x2
		char drawOnCompass; //OFS: 0x22 SIZE: 0x1
		int teamAndOwnerIndex; //OFS: 0x24 SIZE: 0x4
	};

	struct LerpEntityStateMissile
	{
		int launchTime; //OFS: 0x0 SIZE: 0x4
		int parentClientNum; //OFS: 0x4 SIZE: 0x4
		int fuseTime; //OFS: 0x8 SIZE: 0x4
	};

	struct LerpEntityStateScriptMover
	{
		char attachTagIndex[4]; //OFS: 0x0 SIZE: 0x4
		int attachedTagIndex; //OFS: 0x4 SIZE: 0x4
		__int16 attachModelIndex[4]; //OFS: 0x8 SIZE: 0x8
		__int16 animScriptedAnim; //OFS: 0x10 SIZE: 0x2
		int animScriptedAnimTime; //OFS: 0x14 SIZE: 0x4
		__int16 attachedEntNum; //OFS: 0x18 SIZE: 0x2
		__int16 exploderIndex; //OFS: 0x1A SIZE: 0x2
	};

	struct LerpEntityStateSoundBlend
	{
		float lerp; //OFS: 0x0 SIZE: 0x4
		float volumeScale; //OFS: 0x4 SIZE: 0x4
	};

	struct LerpEntityStateAnonymous
	{
		int data[15]; //OFS: 0x0 SIZE: 0x3C
	};

	union LerpEntityStateTypeUnion
	{
		LerpEntityStateTurret turret; //OFS: 0x0 SIZE: 0x14
		LerpEntityStateLoopFx loopFx; //OFS: 0x1 SIZE: 0x8
		LerpEntityStateActor actor; //OFS: 0x2 SIZE: 0xC
		LerpEntityStatePrimaryLight primaryLight; //OFS: 0x3 SIZE: 0x14
		LerpEntityStatePlayer player; //OFS: 0x4 SIZE: 0xC
		LerpEntityStateVehicle vehicle; //OFS: 0x5 SIZE: 0x28
		LerpEntityStateMissile missile; //OFS: 0x6 SIZE: 0xC
		LerpEntityStateScriptMover scriptMover; //OFS: 0x7 SIZE: 0x1C
		LerpEntityStateSoundBlend soundBlend; //OFS: 0x8 SIZE: 0x8
		LerpEntityStateAnonymous anonymous; //OFS: 0x9 SIZE: 0x3C
	};

	struct LerpEntityState
	{
		int eFlags; //OFS: 0x0 SIZE: 0x4
		trajectory_t pos; //OFS: 0x4 SIZE: 0x24
		trajectory_t apos; //OFS: 0x28 SIZE: 0x24
		LerpEntityStateTypeUnion u; //OFS: 0x4C SIZE: 0x3C
		int usecount; //OFS: 0x88 SIZE: 0x4
	};

	struct __declspec(align(2)) LoopSound
	{
		unsigned __int16 soundAlias; //OFS: 0x0 SIZE: 0x2
		__int16 fadeTime; //OFS: 0x2 SIZE: 0x2
	};

	struct __declspec(align(2)) scr_const_t
	{
		unsigned __int16 _; //OFS: 0x0 SIZE: 0x2
		unsigned __int16 active2; //OFS: 0x2 SIZE: 0x2
		unsigned __int16 j_spine4; //OFS: 0x4 SIZE: 0x2
		unsigned __int16 j_helmet; //OFS: 0x6 SIZE: 0x2
		unsigned __int16 j_head; //OFS: 0x8 SIZE: 0x2
		unsigned __int16 all; //OFS: 0xA SIZE: 0x2
		unsigned __int16 allies; //OFS: 0xC SIZE: 0x2
		unsigned __int16 axis; //OFS: 0xE SIZE: 0x2
		unsigned __int16 bad_path; //OFS: 0x10 SIZE: 0x2
		unsigned __int16 begin_firing; //OFS: 0x12 SIZE: 0x2
		unsigned __int16 unknown_location; //OFS: 0x14 SIZE: 0x2
		unsigned __int16 cancel_location; //OFS: 0x16 SIZE: 0x2
		unsigned __int16 confirm_location; //OFS: 0x18 SIZE: 0x2
		unsigned __int16 regroup_location; //OFS: 0x1A SIZE: 0x2
		unsigned __int16 defend_location; //OFS: 0x1C SIZE: 0x2
		unsigned __int16 clear_squadcommand; //OFS: 0x1E SIZE: 0x2
		unsigned __int16 squadleader_changed; //OFS: 0x20 SIZE: 0x2
		unsigned __int16 squad_disbanded; //OFS: 0x22 SIZE: 0x2
		unsigned __int16 deployed_turret; //OFS: 0x24 SIZE: 0x2
		unsigned __int16 crouch; //OFS: 0x26 SIZE: 0x2
		unsigned __int16 current; //OFS: 0x28 SIZE: 0x2
		unsigned __int16 damage; //OFS: 0x2A SIZE: 0x2
		unsigned __int16 dead; //OFS: 0x2C SIZE: 0x2
		unsigned __int16 death; //OFS: 0x2E SIZE: 0x2
		unsigned __int16 disconnect; //OFS: 0x30 SIZE: 0x2
		unsigned __int16 death_or_disconnect; //OFS: 0x32 SIZE: 0x2
		unsigned __int16 detonate; //OFS: 0x34 SIZE: 0x2
		unsigned __int16 direct; //OFS: 0x36 SIZE: 0x2
		unsigned __int16 dlight; //OFS: 0x38 SIZE: 0x2
		unsigned __int16 done; //OFS: 0x3A SIZE: 0x2
		unsigned __int16 empty; //OFS: 0x3C SIZE: 0x2
		unsigned __int16 end_firing; //OFS: 0x3E SIZE: 0x2
		unsigned __int16 enter_vehicle; //OFS: 0x40 SIZE: 0x2
		unsigned __int16 entity; //OFS: 0x42 SIZE: 0x2
		unsigned __int16 exit_vehicle; //OFS: 0x44 SIZE: 0x2
		unsigned __int16 change_seat; //OFS: 0x46 SIZE: 0x2
		unsigned __int16 vehicle_death; //OFS: 0x48 SIZE: 0x2
		unsigned __int16 explode; //OFS: 0x4A SIZE: 0x2
		unsigned __int16 failed; //OFS: 0x4C SIZE: 0x2
		unsigned __int16 free; //OFS: 0x4E SIZE: 0x2
		unsigned __int16 fraction; //OFS: 0x50 SIZE: 0x2
		unsigned __int16 goal; //OFS: 0x52 SIZE: 0x2
		unsigned __int16 goal_changed; //OFS: 0x54 SIZE: 0x2
		unsigned __int16 goal_yaw; //OFS: 0x56 SIZE: 0x2
		unsigned __int16 grenade; //OFS: 0x58 SIZE: 0x2
		unsigned __int16 grenade_danger; //OFS: 0x5A SIZE: 0x2
		unsigned __int16 grenade_fire; //OFS: 0x5C SIZE: 0x2
		unsigned __int16 grenade_launcher_fire; //OFS: 0x5E SIZE: 0x2
		unsigned __int16 grenade_pullback; //OFS: 0x60 SIZE: 0x2
		unsigned __int16 info_notnull; //OFS: 0x62 SIZE: 0x2
		unsigned __int16 invisible; //OFS: 0x64 SIZE: 0x2
		unsigned __int16 key1; //OFS: 0x66 SIZE: 0x2
		unsigned __int16 key2; //OFS: 0x68 SIZE: 0x2
		unsigned __int16 killanimscript; //OFS: 0x6A SIZE: 0x2
		unsigned __int16 left; //OFS: 0x6C SIZE: 0x2
		unsigned __int16 left_tread; //OFS: 0x6E SIZE: 0x2
		unsigned __int16 light; //OFS: 0x70 SIZE: 0x2
		unsigned __int16 movedone; //OFS: 0x72 SIZE: 0x2
		unsigned __int16 noclass; //OFS: 0x74 SIZE: 0x2
		unsigned __int16 none; //OFS: 0x76 SIZE: 0x2
		unsigned __int16 normal; //OFS: 0x78 SIZE: 0x2
		unsigned __int16 player; //OFS: 0x7A SIZE: 0x2
		unsigned __int16 position; //OFS: 0x7C SIZE: 0x2
		unsigned __int16 projectile_impact; //OFS: 0x7E SIZE: 0x2
		unsigned __int16 prone; //OFS: 0x80 SIZE: 0x2
		unsigned __int16 right; //OFS: 0x82 SIZE: 0x2
		unsigned __int16 right_tread; //OFS: 0x84 SIZE: 0x2
		unsigned __int16 tank_armor; //OFS: 0x86 SIZE: 0x2
		unsigned __int16 reload; //OFS: 0x88 SIZE: 0x2
		unsigned __int16 reload_start; //OFS: 0x8A SIZE: 0x2
		unsigned __int16 rocket; //OFS: 0x8C SIZE: 0x2
		unsigned __int16 rotatedone; //OFS: 0x8E SIZE: 0x2
		unsigned __int16 script_brushmodel; //OFS: 0x90 SIZE: 0x2
		unsigned __int16 script_model; //OFS: 0x92 SIZE: 0x2
		unsigned __int16 script_origin; //OFS: 0x94 SIZE: 0x2
		unsigned __int16 snd_enveffectsprio_level; //OFS: 0x96 SIZE: 0x2
		unsigned __int16 snd_enveffectsprio_shellshock; //OFS: 0x98 SIZE: 0x2
		unsigned __int16 snd_busvolprio_holdbreath; //OFS: 0x9A SIZE: 0x2
		unsigned __int16 snd_busvolprio_pain; //OFS: 0x9C SIZE: 0x2
		unsigned __int16 snd_busvolprio_shellshock; //OFS: 0x9E SIZE: 0x2
		unsigned __int16 stand; //OFS: 0xA0 SIZE: 0x2
		unsigned __int16 suppression; //OFS: 0xA2 SIZE: 0x2
		unsigned __int16 suppression_end; //OFS: 0xA4 SIZE: 0x2
		unsigned __int16 surfacetype; //OFS: 0xA6 SIZE: 0x2
		unsigned __int16 tag_aim; //OFS: 0xA8 SIZE: 0x2
		unsigned __int16 tag_aim_animated; //OFS: 0xAA SIZE: 0x2
		unsigned __int16 tag_brass; //OFS: 0xAC SIZE: 0x2
		unsigned __int16 tag_butt; //OFS: 0xAE SIZE: 0x2
		unsigned __int16 tag_clip; //OFS: 0xB0 SIZE: 0x2
		unsigned __int16 tag_flash; //OFS: 0xB2 SIZE: 0x2
		unsigned __int16 tag_flash_11; //OFS: 0xB4 SIZE: 0x2
		unsigned __int16 tag_flash_2; //OFS: 0xB6 SIZE: 0x2
		unsigned __int16 tag_flash_22; //OFS: 0xB8 SIZE: 0x2
		unsigned __int16 tag_flash_3; //OFS: 0xBA SIZE: 0x2
		unsigned __int16 tag_fx; //OFS: 0xBC SIZE: 0x2
		unsigned __int16 tag_inhand2; //OFS: 0xBE SIZE: 0x2
		unsigned __int16 tag_knife_attach; //OFS: 0xC0 SIZE: 0x2
		unsigned __int16 tag_knife_fx; //OFS: 0xC2 SIZE: 0x2
		unsigned __int16 tag_bayonet; //OFS: 0xC4 SIZE: 0x2
		unsigned __int16 tag_laser; //OFS: 0xC6 SIZE: 0x2
		unsigned __int16 tag_origin; //OFS: 0xC8 SIZE: 0x2
		unsigned __int16 tag_weapon; //OFS: 0xCA SIZE: 0x2
		unsigned __int16 tag_player2; //OFS: 0xCC SIZE: 0x2
		unsigned __int16 tag_camera; //OFS: 0xCE SIZE: 0x2
		unsigned __int16 tag_weapon_right; //OFS: 0xD0 SIZE: 0x2
		unsigned __int16 tag_gasmask; //OFS: 0xD2 SIZE: 0x2
		unsigned __int16 tag_gasmask2; //OFS: 0xD4 SIZE: 0x2
		unsigned __int16 tag_sync; //OFS: 0xD6 SIZE: 0x2
		unsigned __int16 tag_wake; //OFS: 0xD8 SIZE: 0x2
		unsigned __int16 target_script_trigger; //OFS: 0xDA SIZE: 0x2
		unsigned __int16 tempEntity; //OFS: 0xDC SIZE: 0x2
		unsigned __int16 top; //OFS: 0xDE SIZE: 0x2
		unsigned __int16 touch; //OFS: 0xE0 SIZE: 0x2
		unsigned __int16 trigger; //OFS: 0xE2 SIZE: 0x2
		unsigned __int16 trigger_use; //OFS: 0xE4 SIZE: 0x2
		unsigned __int16 trigger_use_touch; //OFS: 0xE6 SIZE: 0x2
		unsigned __int16 trigger_damage; //OFS: 0xE8 SIZE: 0x2
		unsigned __int16 trigger_lookat; //OFS: 0xEA SIZE: 0x2
		unsigned __int16 trigger_radius; //OFS: 0xEC SIZE: 0x2
		unsigned __int16 truck_cam; //OFS: 0xEE SIZE: 0x2
		unsigned __int16 weapon_change_on_turret; //OFS: 0xF0 SIZE: 0x2
		unsigned __int16 weapon_change; //OFS: 0xF2 SIZE: 0x2
		unsigned __int16 weapon_change_complete; //OFS: 0xF4 SIZE: 0x2
		unsigned __int16 weapon_fired; //OFS: 0xF6 SIZE: 0x2
		unsigned __int16 weapon_pvp_attack; //OFS: 0xF8 SIZE: 0x2
		unsigned __int16 worldspawn; //OFS: 0xFA SIZE: 0x2
		unsigned __int16 flashbang; //OFS: 0xFC SIZE: 0x2
		unsigned __int16 flash; //OFS: 0xFE SIZE: 0x2
		unsigned __int16 smoke; //OFS: 0x100 SIZE: 0x2
		unsigned __int16 night_vision_on; //OFS: 0x102 SIZE: 0x2
		unsigned __int16 night_vision_off; //OFS: 0x104 SIZE: 0x2
		unsigned __int16 back_low; //OFS: 0x106 SIZE: 0x2
		unsigned __int16 back_mid; //OFS: 0x108 SIZE: 0x2
		unsigned __int16 back_up; //OFS: 0x10A SIZE: 0x2
		unsigned __int16 head; //OFS: 0x10C SIZE: 0x2
		unsigned __int16 j_mainroot; //OFS: 0x10E SIZE: 0x2
		unsigned __int16 neck; //OFS: 0x110 SIZE: 0x2
		unsigned __int16 pelvis; //OFS: 0x112 SIZE: 0x2
		unsigned __int16 j_head2; //OFS: 0x114 SIZE: 0x2
		unsigned __int16 MOD_UNKNOWN; //OFS: 0x116 SIZE: 0x2
		unsigned __int16 MOD_PISTOL_BULLET; //OFS: 0x118 SIZE: 0x2
		unsigned __int16 MOD_RIFLE_BULLET; //OFS: 0x11A SIZE: 0x2
		unsigned __int16 MOD_GRENADE; //OFS: 0x11C SIZE: 0x2
		unsigned __int16 MOD_GRENADE_SPLASH; //OFS: 0x11E SIZE: 0x2
		unsigned __int16 MOD_PROJECTILE; //OFS: 0x120 SIZE: 0x2
		unsigned __int16 MOD_PROJECTILE_SPLASH; //OFS: 0x122 SIZE: 0x2
		unsigned __int16 MOD_MELEE; //OFS: 0x124 SIZE: 0x2
		unsigned __int16 MOD_BAYONET; //OFS: 0x126 SIZE: 0x2
		unsigned __int16 MOD_HEAD_SHOT; //OFS: 0x128 SIZE: 0x2
		unsigned __int16 MOD_CRUSH; //OFS: 0x12A SIZE: 0x2
		unsigned __int16 MOD_TELEFRAG; //OFS: 0x12C SIZE: 0x2
		unsigned __int16 MOD_FALLING; //OFS: 0x12E SIZE: 0x2
		unsigned __int16 MOD_SUICIDE; //OFS: 0x130 SIZE: 0x2
		unsigned __int16 MOD_TRIGGER_HURT; //OFS: 0x132 SIZE: 0x2
		unsigned __int16 MOD_EXPLOSIVE; //OFS: 0x134 SIZE: 0x2
		unsigned __int16 MOD_IMPACT; //OFS: 0x136 SIZE: 0x2
		unsigned __int16 MOD_BURNED; //OFS: 0x138 SIZE: 0x2
		unsigned __int16 MOD_HIT_BY_OBJECT; //OFS: 0x13A SIZE: 0x2
		unsigned __int16 MOD_DROWN; //OFS: 0x13C SIZE: 0x2
		unsigned __int16 script_vehicle; //OFS: 0x13E SIZE: 0x2
		unsigned __int16 script_vehicle_collision; //OFS: 0x140 SIZE: 0x2
		unsigned __int16 script_vehicle_collmap; //OFS: 0x142 SIZE: 0x2
		unsigned __int16 script_vehicle_corpse; //OFS: 0x144 SIZE: 0x2
		unsigned __int16 turret_fire; //OFS: 0x146 SIZE: 0x2
		unsigned __int16 turret_on_target; //OFS: 0x148 SIZE: 0x2
		unsigned __int16 turret_not_on_target; //OFS: 0x14A SIZE: 0x2
		unsigned __int16 turret_on_vistarget; //OFS: 0x14C SIZE: 0x2
		unsigned __int16 turret_no_vis; //OFS: 0x14E SIZE: 0x2
		unsigned __int16 turret_rotate_stopped; //OFS: 0x150 SIZE: 0x2
		unsigned __int16 turret_deactivate; //OFS: 0x152 SIZE: 0x2
		unsigned __int16 turretstatechange; //OFS: 0x154 SIZE: 0x2
		unsigned __int16 turretownerchange; //OFS: 0x156 SIZE: 0x2
		unsigned __int16 reached_end_node; //OFS: 0x158 SIZE: 0x2
		unsigned __int16 reached_wait_node; //OFS: 0x15A SIZE: 0x2
		unsigned __int16 reached_wait_speed; //OFS: 0x15C SIZE: 0x2
		unsigned __int16 near_goal; //OFS: 0x15E SIZE: 0x2
		unsigned __int16 veh_collision; //OFS: 0x160 SIZE: 0x2
		unsigned __int16 veh_predictedcollision; //OFS: 0x162 SIZE: 0x2
		unsigned __int16 script_camera; //OFS: 0x164 SIZE: 0x2
		unsigned __int16 begin; //OFS: 0x166 SIZE: 0x2
		unsigned __int16 curve_nodehit; //OFS: 0x168 SIZE: 0x2
		unsigned __int16 curve_start; //OFS: 0x16A SIZE: 0x2
		unsigned __int16 curve_end; //OFS: 0x16C SIZE: 0x2
		unsigned __int16 tag_enter_driver; //OFS: 0x16E SIZE: 0x2
		unsigned __int16 tag_enter_gunner1; //OFS: 0x170 SIZE: 0x2
		unsigned __int16 tag_enter_gunner2; //OFS: 0x172 SIZE: 0x2
		unsigned __int16 tag_enter_gunner3; //OFS: 0x174 SIZE: 0x2
		unsigned __int16 tag_enter_gunner4; //OFS: 0x176 SIZE: 0x2
		unsigned __int16 tag_enter_passenger; //OFS: 0x178 SIZE: 0x2
		unsigned __int16 tag_enter_passenger2; //OFS: 0x17A SIZE: 0x2
		unsigned __int16 tag_enter_passenger3; //OFS: 0x17C SIZE: 0x2
		unsigned __int16 tag_enter_passenger4; //OFS: 0x17E SIZE: 0x2
		unsigned __int16 tag_player; //OFS: 0x180 SIZE: 0x2
		unsigned __int16 tag_passenger1; //OFS: 0x182 SIZE: 0x2
		unsigned __int16 tag_passenger2; //OFS: 0x184 SIZE: 0x2
		unsigned __int16 tag_passenger3; //OFS: 0x186 SIZE: 0x2
		unsigned __int16 tag_passenger4; //OFS: 0x188 SIZE: 0x2
		unsigned __int16 tag_gunner1; //OFS: 0x18A SIZE: 0x2
		unsigned __int16 tag_gunner2; //OFS: 0x18C SIZE: 0x2
		unsigned __int16 tag_gunner3; //OFS: 0x18E SIZE: 0x2
		unsigned __int16 tag_gunner4; //OFS: 0x190 SIZE: 0x2
		unsigned __int16 tag_gunner_barrel1; //OFS: 0x192 SIZE: 0x2
		unsigned __int16 tag_gunner_barrel2; //OFS: 0x194 SIZE: 0x2
		unsigned __int16 tag_gunner_barrel3; //OFS: 0x196 SIZE: 0x2
		unsigned __int16 tag_gunner_barrel4; //OFS: 0x198 SIZE: 0x2
		unsigned __int16 tag_gunner_turret1; //OFS: 0x19A SIZE: 0x2
		unsigned __int16 tag_gunner_turret2; //OFS: 0x19C SIZE: 0x2
		unsigned __int16 tag_gunner_turret3; //OFS: 0x19E SIZE: 0x2
		unsigned __int16 tag_gunner_turret4; //OFS: 0x1A0 SIZE: 0x2
		unsigned __int16 tag_flash_gunner1; //OFS: 0x1A2 SIZE: 0x2
		unsigned __int16 tag_flash_gunner2; //OFS: 0x1A4 SIZE: 0x2
		unsigned __int16 tag_flash_gunner3; //OFS: 0x1A6 SIZE: 0x2
		unsigned __int16 tag_flash_gunner4; //OFS: 0x1A8 SIZE: 0x2
		unsigned __int16 tag_flash_gunner1a; //OFS: 0x1AA SIZE: 0x2
		unsigned __int16 tag_flash_gunner2a; //OFS: 0x1AC SIZE: 0x2
		unsigned __int16 tag_flash_gunner3a; //OFS: 0x1AE SIZE: 0x2
		unsigned __int16 tag_flash_gunner4a; //OFS: 0x1B0 SIZE: 0x2
		unsigned __int16 tag_gunner_brass1; //OFS: 0x1B2 SIZE: 0x2
		unsigned __int16 tag_gunner_hands1; //OFS: 0x1B4 SIZE: 0x2
		unsigned __int16 tag_wheel_front_left; //OFS: 0x1B6 SIZE: 0x2
		unsigned __int16 tag_wheel_front_right; //OFS: 0x1B8 SIZE: 0x2
		unsigned __int16 tag_wheel_back_left; //OFS: 0x1BA SIZE: 0x2
		unsigned __int16 tag_wheel_back_right; //OFS: 0x1BC SIZE: 0x2
		unsigned __int16 tag_wheel_middle_left; //OFS: 0x1BE SIZE: 0x2
		unsigned __int16 tag_wheel_middle_right; //OFS: 0x1C0 SIZE: 0x2
		unsigned __int16 vampire_health_regen; //OFS: 0x1C2 SIZE: 0x2
		unsigned __int16 vampire_kill; //OFS: 0x1C4 SIZE: 0x2
		unsigned __int16 morphine_shot; //OFS: 0x1C6 SIZE: 0x2
		unsigned __int16 morphine_revive; //OFS: 0x1C8 SIZE: 0x2
		unsigned __int16 freelook; //OFS: 0x1CA SIZE: 0x2
		unsigned __int16 intermission; //OFS: 0x1CC SIZE: 0x2
		unsigned __int16 playing; //OFS: 0x1CE SIZE: 0x2
		unsigned __int16 spectator; //OFS: 0x1D0 SIZE: 0x2
		unsigned __int16 action_notify_attack; //OFS: 0x1D2 SIZE: 0x2
		unsigned __int16 action_notify_melee; //OFS: 0x1D4 SIZE: 0x2
		unsigned __int16 action_notify_use_reload; //OFS: 0x1D6 SIZE: 0x2
		unsigned __int16 always; //OFS: 0x1D8 SIZE: 0x2
		unsigned __int16 auto_ai; //OFS: 0x1DA SIZE: 0x2
		unsigned __int16 auto_nonai; //OFS: 0x1DC SIZE: 0x2
		unsigned __int16 back_left; //OFS: 0x1DE SIZE: 0x2
		unsigned __int16 back_right; //OFS: 0x1E0 SIZE: 0x2
		unsigned __int16 begin_custom_anim; //OFS: 0x1E2 SIZE: 0x2
		unsigned __int16 bullethit; //OFS: 0x1E4 SIZE: 0x2
		unsigned __int16 count; //OFS: 0x1E6 SIZE: 0x2
		unsigned __int16 corner_approach; //OFS: 0x1E8 SIZE: 0x2
		unsigned __int16 damage_notdone; //OFS: 0x1EA SIZE: 0x2
		unsigned __int16 deathplant; //OFS: 0x1EC SIZE: 0x2
		unsigned __int16 front_left; //OFS: 0x1EE SIZE: 0x2
		unsigned __int16 front_right; //OFS: 0x1F0 SIZE: 0x2
		unsigned __int16 tag_inhand; //OFS: 0x1F2 SIZE: 0x2
		unsigned __int16 high_priority; //OFS: 0x1F4 SIZE: 0x2
		unsigned __int16 info_player_deathmatch; //OFS: 0x1F6 SIZE: 0x2
		unsigned __int16 infinite_energy; //OFS: 0x1F8 SIZE: 0x2
		unsigned __int16 low_priority; //OFS: 0x1FA SIZE: 0x2
		unsigned __int16 manual; //OFS: 0x1FC SIZE: 0x2
		unsigned __int16 manual_ai; //OFS: 0x1FE SIZE: 0x2
		unsigned __int16 max_time; //OFS: 0x200 SIZE: 0x2
		unsigned __int16 menuresponse; //OFS: 0x202 SIZE: 0x2
		unsigned __int16 middle_left; //OFS: 0x204 SIZE: 0x2
		unsigned __int16 middle_right; //OFS: 0x206 SIZE: 0x2
		unsigned __int16 min_energy; //OFS: 0x208 SIZE: 0x2
		unsigned __int16 min_time; //OFS: 0x20A SIZE: 0x2
		unsigned __int16 neutral; //OFS: 0x20C SIZE: 0x2
		unsigned __int16 never; //OFS: 0x20E SIZE: 0x2
		unsigned __int16 pickup; //OFS: 0x210 SIZE: 0x2
		unsigned __int16 receiver; //OFS: 0x212 SIZE: 0x2
		unsigned __int16 sound_blend; //OFS: 0x214 SIZE: 0x2
		unsigned __int16 tag_wingtipl; //OFS: 0x216 SIZE: 0x2
		unsigned __int16 tag_wingtipr; //OFS: 0x218 SIZE: 0x2
		unsigned __int16 tag_wingmidl; //OFS: 0x21A SIZE: 0x2
		unsigned __int16 tag_wingmidr; //OFS: 0x21C SIZE: 0x2
		unsigned __int16 tag_prop; //OFS: 0x21E SIZE: 0x2
		unsigned __int16 tag_end; //OFS: 0x220 SIZE: 0x2
		unsigned __int16 tag_tailtop; //OFS: 0x222 SIZE: 0x2
		unsigned __int16 tag_tailbottom; //OFS: 0x224 SIZE: 0x2
		unsigned __int16 tag_detach; //OFS: 0x226 SIZE: 0x2
		unsigned __int16 tag_passenger; //OFS: 0x228 SIZE: 0x2
		unsigned __int16 tag_enter_back; //OFS: 0x22A SIZE: 0x2
		unsigned __int16 tag_detach2; //OFS: 0x22C SIZE: 0x2
		unsigned __int16 tag_popout; //OFS: 0x22E SIZE: 0x2
		unsigned __int16 tag_body; //OFS: 0x230 SIZE: 0x2
		unsigned __int16 tag_turret; //OFS: 0x232 SIZE: 0x2
		unsigned __int16 tag_turret_base; //OFS: 0x234 SIZE: 0x2
		unsigned __int16 tag_barrel; //OFS: 0x236 SIZE: 0x2
		unsigned __int16 tag_weapon_left; //OFS: 0x238 SIZE: 0x2
		unsigned __int16 human; //OFS: 0x23A SIZE: 0x2
		unsigned __int16 custom; //OFS: 0x23C SIZE: 0x2
		unsigned __int16 angle_deltas; //OFS: 0x23E SIZE: 0x2
		unsigned __int16 bulletwhizby; //OFS: 0x240 SIZE: 0x2
		unsigned __int16 dog; //OFS: 0x242 SIZE: 0x2
		unsigned __int16 enemy; //OFS: 0x244 SIZE: 0x2
		unsigned __int16 enemy_visible; //OFS: 0x246 SIZE: 0x2
		unsigned __int16 face_angle; //OFS: 0x248 SIZE: 0x2
		unsigned __int16 face_current; //OFS: 0x24A SIZE: 0x2
		unsigned __int16 face_default; //OFS: 0x24C SIZE: 0x2
		unsigned __int16 face_direction; //OFS: 0x24E SIZE: 0x2
		unsigned __int16 face_enemy; //OFS: 0x250 SIZE: 0x2
		unsigned __int16 face_enemy_or_motion; //OFS: 0x252 SIZE: 0x2
		unsigned __int16 face_goal; //OFS: 0x254 SIZE: 0x2
		unsigned __int16 face_motion; //OFS: 0x256 SIZE: 0x2
		unsigned __int16 face_point; //OFS: 0x258 SIZE: 0x2
		unsigned __int16 gravity; //OFS: 0x25A SIZE: 0x2
		unsigned __int16 groundEntChanged; //OFS: 0x25C SIZE: 0x2
		unsigned __int16 gunshot; //OFS: 0x25E SIZE: 0x2
		unsigned __int16 obstacle; //OFS: 0x260 SIZE: 0x2
		unsigned __int16 movemode; //OFS: 0x262 SIZE: 0x2
		unsigned __int16 node_out_of_range; //OFS: 0x264 SIZE: 0x2
		unsigned __int16 node_relinquished; //OFS: 0x266 SIZE: 0x2
		unsigned __int16 node_taken; //OFS: 0x268 SIZE: 0x2
		unsigned __int16 node_not_safe; //OFS: 0x26A SIZE: 0x2
		unsigned __int16 noclip; //OFS: 0x26C SIZE: 0x2
		unsigned __int16 nogravity; //OFS: 0x26E SIZE: 0x2
		unsigned __int16 nophysics; //OFS: 0x270 SIZE: 0x2
		unsigned __int16 pain; //OFS: 0x272 SIZE: 0x2
		unsigned __int16 run; //OFS: 0x274 SIZE: 0x2
		unsigned __int16 runto_arrived; //OFS: 0x276 SIZE: 0x2
		unsigned __int16 silenced_shot; //OFS: 0x278 SIZE: 0x2
		unsigned __int16 spawned; //OFS: 0x27A SIZE: 0x2
		unsigned __int16 start_move; //OFS: 0x27C SIZE: 0x2
		unsigned __int16 stop; //OFS: 0x27E SIZE: 0x2
		unsigned __int16 stop_soon; //OFS: 0x280 SIZE: 0x2
		unsigned __int16 tag_eye; //OFS: 0x282 SIZE: 0x2
		unsigned __int16 walk; //OFS: 0x284 SIZE: 0x2
		unsigned __int16 world; //OFS: 0x286 SIZE: 0x2
		unsigned __int16 zonly_physics; //OFS: 0x288 SIZE: 0x2
		unsigned __int16 j_ankle_le; //OFS: 0x28A SIZE: 0x2
		unsigned __int16 j_ankle_ri; //OFS: 0x28C SIZE: 0x2
		unsigned __int16 j_ball_le; //OFS: 0x28E SIZE: 0x2
		unsigned __int16 j_ball_ri; //OFS: 0x290 SIZE: 0x2
		unsigned __int16 j_palm_le; //OFS: 0x292 SIZE: 0x2
		unsigned __int16 j_palm_ri; //OFS: 0x294 SIZE: 0x2
		unsigned __int16 broken; //OFS: 0x296 SIZE: 0x2
		unsigned __int16 destructible; //OFS: 0x298 SIZE: 0x2
		unsigned __int16 snapacknowledged; //OFS: 0x29A SIZE: 0x2
		unsigned __int16 disconnected; //OFS: 0x29C SIZE: 0x2
		unsigned __int16 cinematic; //OFS: 0x29E SIZE: 0x2
		unsigned __int16 uicinematic; //OFS: 0x2A0 SIZE: 0x2
		unsigned __int16 logo; //OFS: 0x2A2 SIZE: 0x2
		unsigned __int16 connecting; //OFS: 0x2A4 SIZE: 0x2
		unsigned __int16 challenging; //OFS: 0x2A6 SIZE: 0x2
		unsigned __int16 connected; //OFS: 0x2A8 SIZE: 0x2
		unsigned __int16 sendingstats; //OFS: 0x2AA SIZE: 0x2
		unsigned __int16 loading; //OFS: 0x2AC SIZE: 0x2
		unsigned __int16 primed; //OFS: 0x2AE SIZE: 0x2
		unsigned __int16 active; //OFS: 0x2B0 SIZE: 0x2
		unsigned __int16 map_restart; //OFS: 0x2B2 SIZE: 0x2
		unsigned __int16 orientdone; //OFS: 0x2B4 SIZE: 0x2
	};

	union entityState_index
	{
		__int16 brushmodel; //OFS: 0x0 SIZE: 0x2
		__int16 xmodel; //OFS: 0x1 SIZE: 0x2
		__int16 primaryLight; //OFS: 0x2 SIZE: 0x2
		unsigned __int16 bone; //OFS: 0x3 SIZE: 0x2
		int pad; //OFS: 0x4 SIZE: 0x4
	};

	union entityState_un1
	{
		char destructibleid; //OFS: 0x0 SIZE: 0x1
		char pad[4]; //OFS: 0x1 SIZE: 0x4
	};

	struct playerAnimState_t
	{
		int legsAnim; //OFS: 0x0 SIZE: 0x4
		int torsoAnim; //OFS: 0x4 SIZE: 0x4
		float fTorsoPitch; //OFS: 0x8 SIZE: 0x4
		float fWaistPitch; //OFS: 0xC SIZE: 0x4
	};

	union entityState_un2
	{
		playerAnimState_t anim; //OFS: 0x0 SIZE: 0x10
	};

	union entityState_un3
	{
		int item; //OFS: 0x0 SIZE: 0x4
		int hintString; //OFS: 0x1 SIZE: 0x4
		int vehicleXModel; //OFS: 0x2 SIZE: 0x4
		unsigned int secondBcAlias; //OFS: 0x3 SIZE: 0x4
		unsigned int soundTag; //OFS: 0x4 SIZE: 0x4
	};

	enum entityType_t : __int32
	{
		ET_GENERAL = 0x0,
		ET_PLAYER = 0x1,
		ET_PLAYER_CORPSE = 0x2,
		ET_ITEM = 0x3,
		ET_MISSILE = 0x4,
		ET_INVISIBLE = 0x5,
		ET_SCRIPTMOVER = 0x6,
		ET_SOUND_BLEND = 0x7,
		ET_FX = 0x8,
		ET_LOOP_FX = 0x9,
		ET_PRIMARY_LIGHT = 0xA,
		ET_MG42 = 0xB,
		ET_PLANE = 0xC,
		ET_VEHICLE = 0xD,
		ET_VEHICLE_COLLMAP = 0xE,
		ET_VEHICLE_CORPSE = 0xF,
		ET_ACTOR = 0x10,
		ET_ACTOR_SPAWNER = 0x11,
		ET_ACTOR_CORPSE = 0x12,
		ET_EVENTS = 0x13,
	};

	struct entityState_s
	{
		int number; //OFS: 0x0 SIZE: 0x4
		entityType_t eType; //OFS: 0x4 SIZE: 0x4
		LerpEntityState lerp; //OFS: 0x8 SIZE: 0x8C
		int time2; //OFS: 0x94 SIZE: 0x4
		int otherEntityNum; //OFS: 0x98 SIZE: 0x4
		int groundEntityNum; //OFS: 0x9C SIZE: 0x4
		LoopSound loopSound; //OFS: 0xA0 SIZE: 0x4
		int surfType; //OFS: 0xA4 SIZE: 0x4
		entityState_index index; //OFS: 0xA8 SIZE: 0x4
		int clientnum; //OFS: 0xAC SIZE: 0x4
		int iHeadIcon; //OFS: 0xB0 SIZE: 0x4
		int solid; //OFS: 0xB4 SIZE: 0x4
		int eventParm; //OFS: 0xB8 SIZE: 0x4
		int eventSequence; //OFS: 0xBC SIZE: 0x4
		int events[4]; //OFS: 0xC0 SIZE: 0x10
		int eventParms[4]; //OFS: 0xD0 SIZE: 0x10
		int weapon; //OFS: 0xE0 SIZE: 0x4
		int weaponModel; //OFS: 0xE4 SIZE: 0x4
		int targetname; //OFS: 0xE8 SIZE: 0x4
		entityState_un1 un1; //OFS: 0xEC SIZE: 0x4
		entityState_un2 un2; //OFS: 0xF0 SIZE: 0x10
		entityState_un3 un3; //OFS: 0x100 SIZE: 0x4
		int animtreeIndex; //OFS: 0x104 SIZE: 0x4
		int partBits[4]; //OFS: 0x108 SIZE: 0x10
	};

	struct __declspec(align(2)) EntHandle
	{
		unsigned __int16 number; //OFS: 0x0 SIZE: 0x2
		unsigned __int16 infoIndex; //OFS: 0x2 SIZE: 0x2
	};

	enum contents_e
	{
		CONTENTS_SOLID = 0x1,
		CONTENTS_FOLIAGE = 0x2,
		CONTENTS_NONCOLLIDING = 0x4,
		CONTENTS_GLASS = 0x10,
		CONTENTS_WATER = 0x20,
		CONTENTS_CANSHOOTCLIP = 0x40,
		CONTENTS_MISSILECLIP = 0x80,
		CONTENTS_ITEM = 0x100,
		CONTENTS_VEHICLECLIP = 0x200,
		CONTENTS_ITEMCLIP = 0x400,
		CONTENTS_SKY = 0x800,
		CONTENTS_AI_NOSIGHT = 0x1000,
		CONTENTS_CLIPSHOT = 0x2000,
		CONTENTS_CORPSE_CLIPSHOT = 0x4000,
		CONTENTS_ACTOR = 0x8000,
		CONTENTS_FAKE_ACTOR = 0x8000,
		CONTENTS_PLAYERCLIP = 0x10000,
		CONTENTS_MONSTERCLIP = 0x20000,
		CONTENTS_PLAYERVEHICLECLIP = 0x40000,
		CONTENTS_USE = 0x200000,
		CONTENTS_UTILITYCLIP = 0x400000,
		CONTENTS_VEHICLE = 0x800000,
		CONTENTS_MANTLE = 0x1000000,
		CONTENTS_PLAYER = 0x2000000,
		CONTENTS_CORPSE = 0x4000000,
		CONTENTS_DETAIL = 0x8000000,
		CONTENTS_STRUCTURAL = 0x10000000,
		CONTENTS_LOOKAT = 0x10000000,
		CONTENTS_TRIGGER = 0x40000000,
		CONTENTS_NODROP = 0x80000000,
	};

	struct entityShared_s
	{
		unsigned __int8 linked; //OFS: 0x0 SIZE: 0x1
		unsigned __int8 bmodel; //OFS: 0x1 SIZE: 0x1
		unsigned __int16 svFlags; //OFS: 0x2 SIZE: 0x2
		unsigned __int8 eventType; //OFS: 0x4 SIZE: 0x1
		unsigned __int8 inuse; //OFS: 0x5 SIZE: 0x1
		int clientMask[2]; //OFS: 0x8 SIZE: 0x8
		int broadcastTime; //OFS: 0x10 SIZE: 0x4
		float mins[3]; //OFS: 0x14 SIZE: 0xC
		float maxs[3]; //OFS: 0x20 SIZE: 0xC
		contents_e contents; //OFS: 0x2C SIZE: 0x4
		float absmin[3]; //OFS: 0x30 SIZE: 0xC
		float absmax[3]; //OFS: 0x3C SIZE: 0xC
		float currentOrigin[3]; //OFS: 0x48 SIZE: 0xC
		float currentAngles[3]; //OFS: 0x54 SIZE: 0xC
		EntHandle ownerNum; //OFS: 0x60 SIZE: 0x4
		int eventTime; //OFS: 0x64 SIZE: 0x4
	};

	struct flame_timed_damage_t
	{
		gentity_s* attacker; //OFS: 0x0 SIZE: 0x4
		int damage; //OFS: 0x4 SIZE: 0x4
		float damageDuration; //OFS: 0x8 SIZE: 0x4
		float damageInterval; //OFS: 0xC SIZE: 0x4
		int start_timestamp; //OFS: 0x10 SIZE: 0x4
		int end_timestamp; //OFS: 0x14 SIZE: 0x4
		int lastupdate_timestamp; //OFS: 0x18 SIZE: 0x4
	};

	enum team_t : __int32
	{
		TEAM_FREE = 0x0,
		TEAM_BAD = 0x0,
		TEAM_AXIS = 0x1,
		TEAM_ALLIES = 0x2,
		TEAM_NEUTRAL = 0x3,
		TEAM_DEAD = 0x4,
		TEAM_NUM_TEAMS = 0x5,
	};

	enum MissileStage : __int32
	{
		MISSILESTAGE_SOFTLAUNCH = 0x0,
		MISSILESTAGE_ASCENT = 0x1,
		MISSILESTAGE_DESCENT = 0x2,
	};

	enum MissileFlightMode : __int32
	{
		MISSILEFLIGHTMODE_TOP = 0x0,
		MISSILEFLIGHTMODE_DIRECT = 0x1,
	};

	struct missile_ent_t
	{
		float predictLandPos[3]; //OFS: 0x0 SIZE: 0xC
		int predictLandTime; //OFS: 0xC SIZE: 0x4
		int timestamp; //OFS: 0x10 SIZE: 0x4
		float time; //OFS: 0x14 SIZE: 0x4
		int timeOfBirth; //OFS: 0x18 SIZE: 0x4
		float travelDist; //OFS: 0x1C SIZE: 0x4
		float surfaceNormal[3]; //OFS: 0x20 SIZE: 0xC
		team_t team; //OFS: 0x2C SIZE: 0x4
		int thrownBack; //OFS: 0x30 SIZE: 0x4
		float curvature[3]; //OFS: 0x34 SIZE: 0xC
		float targetOffset[3]; //OFS: 0x40 SIZE: 0xC
		MissileStage stage; //OFS: 0x4C SIZE: 0x4
		MissileFlightMode flightMode; //OFS: 0x50 SIZE: 0x4
	};

	struct mover_ent_t
	{
		float decelTime; //OFS: 0x0 SIZE: 0x4
		float aDecelTime; //OFS: 0x4 SIZE: 0x4
		float speed; //OFS: 0x8 SIZE: 0x4
		float aSpeed; //OFS: 0xC SIZE: 0x4
		float midTime; //OFS: 0x10 SIZE: 0x4
		float aMidTime; //OFS: 0x14 SIZE: 0x4
		float pos1[3]; //OFS: 0x18 SIZE: 0xC
		float pos2[3]; //OFS: 0x24 SIZE: 0xC
		float pos3[3]; //OFS: 0x30 SIZE: 0xC
		float apos1[3]; //OFS: 0x3C SIZE: 0xC
		float apos2[3]; //OFS: 0x48 SIZE: 0xC
		float apos3[3]; //OFS: 0x54 SIZE: 0xC
	};

	struct trigger_ent_t
	{
		int threshold; //OFS: 0x0 SIZE: 0x4
		int accumulate; //OFS: 0x4 SIZE: 0x4
		int timestamp; //OFS: 0x8 SIZE: 0x4
		int singleUserEntIndex; //OFS: 0xC SIZE: 0x4
		bool requireLookAt; //OFS: 0x10 SIZE: 0x1
		int exposureIndex; //OFS: 0x14 SIZE: 0x4
		float exposureLerpToLighter; //OFS: 0x18 SIZE: 0x4
		float exposureLerpToDarker; //OFS: 0x1C SIZE: 0x4
		float exposureFeather[3]; //OFS: 0x20 SIZE: 0xC
	};
	union gentity_u
	{
		missile_ent_t missile; //OFS: 0x0 SIZE: 0x54
		mover_ent_t mover; //OFS: 0x1 SIZE: 0x60
		trigger_ent_t trigger; //OFS: 0x2 SIZE: 0x2C
	};

	struct snd_wait_t
	{
		unsigned __int16 notifyString; //OFS: 0x0 SIZE: 0x2
		unsigned __int16 index; //OFS: 0x2 SIZE: 0x2
		unsigned __int8 stoppable; //OFS: 0x4 SIZE: 0x1
		int basetime; //OFS: 0x8 SIZE: 0x4
		int duration; //OFS: 0xC SIZE: 0x4
	};

	struct tagInfo_s
	{
		gentity_s* parent; //OFS: 0x0 SIZE: 0x4
		gentity_s* next; //OFS: 0x4 SIZE: 0x4
		unsigned __int16 name; //OFS: 0x8 SIZE: 0x2
		int index; //OFS: 0xC SIZE: 0x4
		float axis[4][3]; //OFS: 0x10 SIZE: 0x30
		float parentInvAxis[4][3]; //OFS: 0x40 SIZE: 0x30
	};

	enum pmtype_t : __int32
	{
		PM_NORMAL = 0x0,
		PM_NORMAL_LINKED = 0x1,
		PM_NOCLIP = 0x2,
		PM_UFO = 0x3,
		PM_SPECTATOR = 0x4,
		PM_INTERMISSION = 0x5,
		PM_LASTSTAND = 0x6,
		PM_REVIVEE = 0x7,
		PM_LASTSTAND_TRANSITION = 0x8,
		PM_DEAD = 0x9,
		PM_DEAD_LINKED = 0xA,
	};

	enum pmflags_t
	{
		PMF_PRONE = 0x1,
		PMF_MANTLE = 0x4,
		PMF_LADDER = 0x8,
		PMF_BACKWARDS_RUN = 0x20,
		PMF_RESPAWNED = 0x400,
		PMF_JUMPING = 0x4000,
		PMF_SPRINTING = 0x8000,
		PMF_VEHICLE_ATTACHED = 0x100000,
	};

	enum OffhandSecondaryClass : __int32
	{
		PLAYER_OFFHAND_SECONDARY_SMOKE = 0x0,
		PLAYER_OFFHAND_SECONDARY_FLASH = 0x1,
		PLAYER_OFFHAND_SECONDARIES_TOTAL = 0x2,
	};

	enum weaponstate_t : __int32
	{
		WEAPON_READY = 0x0,
		WEAPON_RAISING = 0x1,
		WEAPON_RAISING_ALTSWITCH = 0x2,
		WEAPON_DROPPING = 0x3,
		WEAPON_DROPPING_QUICK = 0x4,
		WEAPON_FIRING = 0x5,
		WEAPON_RECHAMBERING = 0x6,
		WEAPON_RELOADING = 0x7,
		WEAPON_RELOADING_INTERUPT = 0x8,
		WEAPON_RELOAD_START = 0x9,
		WEAPON_RELOAD_START_INTERUPT = 0xA,
		WEAPON_RELOAD_END = 0xB,
		WEAPON_MELEE_CHARGE = 0xC,
		WEAPON_MELEE_INIT = 0xD,
		WEAPON_MELEE_FIRE = 0xE,
		WEAPON_MELEE_END = 0xF,
		WEAPON_OFFHAND_INIT = 0x10,
		WEAPON_OFFHAND_PREPARE = 0x11,
		WEAPON_OFFHAND_HOLD = 0x12,
		WEAPON_OFFHAND_START = 0x13,
		WEAPON_OFFHAND = 0x14,
		WEAPON_OFFHAND_END = 0x15,
		WEAPON_DETONATING = 0x16,
		WEAPON_SPRINT_RAISE = 0x17,
		WEAPON_SPRINT_LOOP = 0x18,
		WEAPON_SPRINT_DROP = 0x19,
		WEAPON_DEPLOYING = 0x1A,
		WEAPON_DEPLOYED = 0x1B,
		WEAPON_BREAKING_DOWN = 0x1C,
		WEAPON_SWIM_IN = 0x1D,
		WEAPON_SWIM_OUT = 0x1E,
		WEAPONSTATES_NUM = 0x1F,
	};

	enum ViewLockTypes : __int32
	{
		PLAYERVIEWLOCK_NONE = 0x0,
		PLAYERVIEWLOCK_FULL = 0x1,
		PLAYERVIEWLOCK_WEAPONJITTER = 0x2,
		PLAYERVIEWLOCKCOUNT = 0x3,
	};

	struct SprintState
	{
		int sprintButtonUpRequired;
		int sprintDelay;
		int lastSprintStart;
		int lastSprintEnd;
		int sprintStartMaxLength;
	};

	struct MantleState
	{
		float yaw;
		int timer;
		int transIndex;
		int flags;
	};

	enum objectiveState_t : __int32
	{
		OBJST_EMPTY = 0x0,
		OBJST_ACTIVE = 0x1,
		OBJST_INVISIBLE = 0x2,
		OBJST_DONE = 0x3,
		OBJST_CURRENT = 0x4,
		OBJST_FAILED = 0x5,
		OBJST_NUMSTATES = 0x6,
	};

	struct objective_t
	{
		objectiveState_t state;
		float origin[3];
		int entNum;
		int teamNum;
		int squadNum;
		int icon;
	};

	enum sessionState_t : __int32
	{
		SESS_STATE_PLAYING = 0x0,
		SESS_STATE_DEAD = 0x1,
		SESS_STATE_SPECTATOR = 0x2,
		SESS_STATE_INTERMISSION = 0x3,
	};

	enum clientConnected_t : __int32
	{
		CON_DISCONNECTED = 0x0,
		CON_CONNECTING = 0x1,
		CON_CONNECTED = 0x2,
	};

	enum button_mask : __int32
	{
		KEY_FIRE = 0x1,
		KEY_SPRINT = 0x2,
		KEY_MELEE = 0x4,
		KEY_USE = 0x8,
		KEY_RELOAD = 0x10,
		KEY_USERELOAD = 0x20,
		KEY_LEANLEFT = 0x40,
		KEY_LEANRIGHT = 0x80,
		KEY_PRONE = 0x100,
		KEY_CROUCH = 0x200,
		KEY_GOSTAND = 0x400,
		KEY_ADSMODE = 0x800,
		KEY_TEMP = 0x1000,
		KEY_HOLDBREATH = 0x2000,
		KEY_FRAG = 0x4000,
		KEY_SMOKE = 0x8000,
		KEY_SELECTING_LOCATION = 0x10000,
		KEY_CANCEL_LOCATION = 0x20000,
		KEY_NIGHTVISION = 0x40000,
		KEY_ADS = 0x80000,
		KEY_REVERSE = 0x100000,
		KEY_HANDBRAKE = 0x200000,
		KEY_THROW = 0x400000,
		KEY_INMENU = 0x800000,
		KEY_UNK6 = 0x1000000,
		KEY_UNK7 = 0x2000000,
		KEY_UNK8 = 0x2000000,
		KEY_UNK9 = 0x4000000,
		KEY_UNK10 = 0x8000000,
	};

	struct __declspec(align(4)) usercmd_s
	{
		int serverTime;
		button_mask buttons;
		int angles[3];
		char weapon;
		char offHandIndex;
		char forward;
		char right;
		char upmove;
		char pitchmove;
		char yawmove;
		__int16 wiimoteGunPitch;
		__int16 wiimoteGunYaw;
		__int16 gunXOfs;
		__int16 gunYOfs;
		__int16 gunZOfs;
		int meleeChargeYaw;
		char meleeChargeDist;
		int rollmove;
		char selectedLocation[2];
		__int16 weapon_buddy;
	};

	enum VehicleAnimState : __int32
	{
		VEHICLEANIMSTATE_IDLE = 0x0,
		VEHICLEANIMSTATE_ENTRY = 0x1,
		VEHICLEANIMSTATE_CHANGEPOS = 0x2,
		VEHICLEANIMSTATE_EXIT = 0x3,
		VEHICLEANIMSTATECOUNT = 0x4,
	};

	struct __declspec(align(4)) clientState_s
	{
		int clientNum;
		team_t team;
		int modelindex;
		int attachModelIndex[6];
		int attachTagIndex[6];
		int lastDamageTime;
		int lastStandStartTime;
		int beingRevived;
		int score;
		int scoreMultiplier;
		char name[32];
		float maxSprintTimeMultiplier;
		int rank;
		int prestige;
		char clanAbbrev[8];
		int attachedEntNum;
		int attachedTagIndex;
		VehicleAnimState vehAnimState;
		int perks;
	};

	struct __declspec(align(4)) clientSession_s
	{
		sessionState_t sessionState;
		int forceSpectatorClient;
		int status_icon;
		int archiveTime;
		int score;
		int kills;
		int assists;
		int downs;
		int revives;
		int headshots;
		int rankxp;
		int something;
		__int16 scriptId;
		BYTE gap_32[2];
		clientConnected_t connected;
		usercmd_s cmd;
		usercmd_s oldcmd;
		int localClient;
		int predictItemPickup;
		char newnetname[32];
		int maxHealth;
		int enterTime;
		int teamState;
		int voteCount;
		int teamVoteCount;
		float moveSpeedScaleMultiplier;
		int viewmodelIndex;
		int noSpectate;
		int teamInfo;
		clientState_s cs;
		int psOffsetTime;
	};

	

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

	enum DB_FILE_EXISTS_PATH
	{
		DB_PATH_ZONE = 0,
		DB_PATH_MAIN = 1,
		DB_PATH_USERMAPS = 2
	};

	struct XZoneInfo
	{
		const char* name;
		int allocFlags;
		int freeFlags;
	};

	enum DemoType : __int32
	{
		DEMO_TYPE_NONE = 0x0,
		DEMO_TYPE_CLIENT = 0x1,
		DEMO_TYPE_SERVER = 0x2,
	};

	enum CubemapShot : __int32
	{
		CUBEMAPSHOT_NONE = 0x0,
		CUBEMAPSHOT_RIGHT = 0x1,
		CUBEMAPSHOT_LEFT = 0x2,
		CUBEMAPSHOT_BACK = 0x3,
		CUBEMAPSHOT_FRONT = 0x4,
		CUBEMAPSHOT_UP = 0x5,
		CUBEMAPSHOT_DOWN = 0x6,
		CUBEMAPSHOT_COUNT = 0x7,
	};

	enum he_type_t : __int32
	{
		HE_TYPE_FREE = 0x0,
		HE_TYPE_TEXT = 0x1,
		HE_TYPE_VALUE = 0x2,
		HE_TYPE_SCORE = 0x3,
		HE_TYPE_MATERIAL = 0x4,
		HE_TYPE_TIMER_DOWN = 0x5,
		HE_TYPE_TIMER_UP = 0x6,
		HE_TYPE_TENTHS_TIMER_DOWN = 0x7,
		HE_TYPE_TENTHS_TIMER_UP = 0x8,
		HE_TYPE_CLOCK_DOWN = 0x9,
		HE_TYPE_CLOCK_UP = 0xA,
		HE_TYPE_WAYPOINT = 0xB,
		HE_TYPE_COUNT = 0xC,
	};

	enum HudelemFontTypes
	{
		HE_FONT_DEFAULT = 0x0,
		HE_FONT_BIGFIXED = 0x1,
		HE_FONT_SMALLFIXED = 0x2,
		HE_FONT_OBJECTIVE = 0x3,
		HE_FONT_BIG = 0x4,
		HE_FONT_SMALL = 0x5,
		HE_FONT_COUNT = 0x6,
	};

	struct colorunpacked
	{
		char r;
		char g;
		char b;
		char a;
	};

	union __declspec(align(4)) hudelem_color_t
	{
		colorunpacked color;
		int rgba;
	};

	enum hudelem_flags_t : __int32
	{
		HE_FLAG_NONE = 0x0,
		HE_FLAG_FOREGROUND = 0x1,
		HE_FLAG_HIDEWHENDEAD = 0x2,
		HE_FLAG_HIDEWHENINMENU = 0x4,
		HE_FLAG_SHADOWED = 0x8,
		HE_FLAG_SHADOWEDMORE = 0x10,
		HE_FLAG_SET3DFONTUSEGLOWCOLOR = 0x20,
	};

	struct hudelem_s
	{
		he_type_t type;
		float x;
		float y;
		float z;
		int targetEntNum;
		float fontScale;
		float fromFontScale;
		int fontScaleStartTime;
		int fontScaleTime;
		HudelemFontTypes font;
		int alignOrg;
		int alignScreen;
		hudelem_color_t color;
		hudelem_color_t fromColor;
		int fadeStartTime;
		int fadeTime;
		int label;
		int width;
		int height;
		int materialIndex;
		int offscreenMaterialIdx;
		int fromWidth;
		int fromHeight;
		int scaleStartTime;
		int scaleTime;
		float fromX;
		float fromY;
		int fromAlignOrg;
		int fromAlignScreen;
		int moveStartTime;
		int moveTime;
		int time;
		int duration;
		float value;
		int text;
		float sort;
		hudelem_color_t glowColor;
		int fxBirthTime;
		int fxLetterTime;
		int fxDecayStartTime;
		int fxDecayDuration;
		int soundID;
		hudelem_flags_t flags;
	};

	struct playerState_s
	{
		int commandTime;
		pmtype_t pm_type;
		int bobCycle;
		pmflags_t pm_flags;
		int weapFlags;
		int otherFlags;
		int pm_time;
		LoopSound loopSound;
		float origin[3];
		float velocity[3];
		float oldVelocity[2];
		int weaponTime;
		int weaponDelay;
		int grenadeTimeLeft;
		int throwBackGrenadeOwner;
		int throwBackGrenadeTimeLeft;
		int weaponRestrictKickTime;
		bool mountAvailable;
		float mountPos[3];
		float mountDir;
		int foliageSoundTime;
		int gravity;
		float leanf;
		int speed;
		float delta_angles[3];
		int groundEntityNum;
		float vLadderVec[3];
		int jumpTime;
		float jumpOriginZ;
		int legsTimer;
		int legsAnim;
		int torsoTimer;
		int torsoAnim;
		int legsAnimDuration;
		int torsoAnimDuration;
		int damageTimer;
		int damageDuration;
		int flinchYawAnim;
		int corpseIndex;
		int movementDir;
		int eFlags;
		int eventSequence;
		int events[4];
		unsigned int eventParms[4];
		int oldEventSequence;
		int clientNum;
		int offHandIndex;
		OffhandSecondaryClass offhandSecondary;
		unsigned int weapon;
		weaponstate_t weaponstate;
		unsigned int weaponShotCount;
		float fWeaponPosFrac;
		int adsDelayTime;
		int spreadOverride;
		int spreadOverrideState;
		int viewmodelIndex;
		float viewangles[3];
		int viewHeightTarget;
		float viewHeightCurrent;
		int viewHeightLerpTime;
		int viewHeightLerpTarget;
		int viewHeightLerpDown;
		float viewAngleClampBase[2];
		float viewAngleClampRange[2];
		int damageEvent;
		int damageYaw;
		int damagePitch;
		int damageCount;
		int stats[6];
		int ammo[128];
		float heatpercent[128];
		bool overheating[128];
		int ammoclip[128];
		unsigned int weapons[4];
		unsigned int weaponold[4];
		unsigned int weaponrechamber[4];
		float proneDirection;
		float proneDirectionPitch;
		float proneTorsoPitch;
		ViewLockTypes viewlocked;
		int viewlocked_entNum;
		int vehiclePos;
		int vehicleType;
		int vehicleAnimBoneIndex;
		int linkFlags;
		float linkAngles[3];
		float groundTiltAngles[3];
		int cursorHint;
		int cursorHintString;
		int cursorHintEntIndex;
		int iCompassPlayerInfo;
		int radarEnabled;
		int locationSelectionInfo;
		SprintState sprintState;
		float fTorsoPitch;
		float fWaistPitch;
		float holdBreathScale;
		int holdBreathTimer;
		float moveSpeedScaleMultiplier;
		MantleState mantleState;
		int vehicleAnimStage;
		int vehicleEntryPoint;
		unsigned int scriptedAnim;
		int scriptedAnimTime;
		float meleeChargeYaw;
		int meleeChargeDist;
		int meleeChargeTime;
		int weapLockFlags;
		int weapLockedEntnum;
		unsigned int forcedViewAnimWeaponIdx;
		int forcedViewAnimWeaponState;
		unsigned int forcedViewAnimOriginalWeaponIdx;
		int collectibles;
		int actionSlotType[4];
		int actionSlotParams[4];
		int entityEventSequence;
		int weapAnim;
		float aimSpreadScale;
		int shellshockIndex;
		int shellshockTime;
		int shellshockDuration;
		float dofNearStart;
		float dofNearEnd;
		float dofFarStart;
		float dofFarEnd;
		float dofNearBlur;
		float dofFarBlur;
		float dofViewmodelStart;
		float dofViewmodelEnd;
		int waterlevel;
		int hudElemLastAssignedSoundID;
		int artilleryInboundIconLocation;
		objective_t objectives[16];
		char weaponmodels[128];
		int deltatime;
		hudelem_s hudelems[31];
		int perks;
	};

	struct __declspec(align(4)) gclient_s
	{
		playerState_s ps;
		clientSession_s sess;
		int spectatorClient;
		int noclip;
		int ufo;
		int bFrozen;
		int buttons;
		int oldbuttons;
		int latched_buttons;
		int buttonsSinceLastFrame;
		float fGunPitch;
		float fGunYaw;
		float fGunXOfs;
		float fGunYOfs;
		float fGunZOfs;
		int damage_blood;
		float damage_from[3];
		int damage_fromWorld;
		int respawnTime;
		int lastBadArcCreateTime;
		int outWaterTime;
		float currentAimSpreadScale;
		gentity_s* pHitHitEnt;
		EntHandle pLookatEnt;
		float prevLinkedInvQuat[4];
		bool prevLinkAnglesSet;
		bool link_doCollision;
		bool linkAnglesLocked;
		float linkAnglesFrac;
		float linkAnglesMinClamp[2];
		float linkAnglesMaxClamp[2];
		int inControlTime;
		int lastTouchTime;
		EntHandle useHoldEntity;
		int useHoldTime;
		int useButtonDone;
		int bDisableAutoPickup;
		int invulnerableExpireTime;
		bool invulnerableActivated;
		bool invulnerableEnabled;
		bool playerMoved;
		float playerLOSCheckPos[2];
		float playerLOSCheckDir[2];
		int playerLOSPosTime;
		int playerADSTargetTime;
		unsigned int lastWeapon;
		bool previouslyFiring;
		bool previouslyUsingNightVision;
		int groundTiltEntNum;
		int revive;
		int reviveTime;
		int lastStand;
		int lastStandTime;
		int switchSeatTime;
		int lastCmdTime;
		int inactivityTime;
		int inactivityWarning;
		int lastVoiceTime;
		int lastServerTime;
		int lastSpawnTime;
		int damageTime;
		float vGunSpeed[3];
		int dropWeaponTime;
		bool previouslyChangingWeapon;
	};

	struct __declspec(align(8)) gentity_s
	{
		entityState_s s; //OFS: 0x0 SIZE: 0x118
		entityShared_s r; //OFS: 0x118 SIZE: 0x68
		gclient_s* client; //OFS: 0x180 SIZE: 0x4
		void* actor_s__actor; //OFS: 0x184 SIZE: 0x4
		void* sentient_s__sentient; //OFS: 0x188 SIZE: 0x4
		void* scr_vehicle_s__scr_vehicle; //OFS: 0x18C SIZE: 0x4
		void* TurretInfo__pTurretInfo; //OFS: 0x190 SIZE: 0x4
		void* Destructible__destructible; //OFS: 0x194 SIZE: 0x4
		unsigned __int16 model; //OFS: 0x198 SIZE: 0x2
		unsigned __int8 physicsObject; //OFS: 0x19A SIZE: 0x1
		unsigned __int8 takedamage; //OFS: 0x19B SIZE: 0x1
		unsigned __int8 active; //OFS: 0x19C SIZE: 0x1
		unsigned __int8 nopickup; //OFS: 0x19D SIZE: 0x1
		unsigned __int8 handler; //OFS: 0x19E SIZE: 0x1
		unsigned __int16 classname; //OFS: 0x1A0 SIZE: 0x2
		unsigned __int16 script_linkName; //OFS: 0x1A2 SIZE: 0x2
		unsigned __int16 script_noteworthy; //OFS: 0x1A4 SIZE: 0x2
		unsigned __int16 target; //OFS: 0x1A6 SIZE: 0x2
		int targetname; //OFS: 0x1A8 SIZE: 0x4
		int spawnflags2; //OFS: 0x1AC SIZE: 0x4
		int spawnflags; //OFS: 0x1B0 SIZE: 0x4
		int flags; //OFS: 0x1B4 SIZE: 0x4
		int clipmask; //OFS: 0x1B8 SIZE: 0x4
		int processedFrame; //OFS: 0x1BC SIZE: 0x4
		EntHandle parent; //OFS: 0x1C0 SIZE: 0x4
		int nextthink; //OFS: 0x1C4 SIZE: 0x4
		int health; //OFS: 0x1C8 SIZE: 0x4
		int maxhealth; //OFS: 0x1CC SIZE: 0x4
		int nexteq; //OFS: 0x1D0 SIZE: 0x4
		int damage; //OFS: 0x1D4 SIZE: 0x4
		flame_timed_damage_t flame_timed_damage[4]; //OFS: 0x1D8 SIZE: 0x70
		int last_timed_radius_damage; //OFS: 0x248 SIZE: 0x4
		int count; //OFS: 0x24C SIZE: 0x4
		gentity_s* chain; //OFS: 0x250 SIZE: 0x4
		gentity_s* activator; //OFS: 0x254 SIZE: 0x4
		gentity_u u; //OFS: 0x258 SIZE: 0x60
		EntHandle missileTargetEnt; //OFS: 0x2B8 SIZE: 0x4
		__int16 lookAtText0; //OFS: 0x2BC SIZE: 0x2
		__int16 lookAtText1; //OFS: 0x2BE SIZE: 0x2
		snd_wait_t snd_wait; //OFS: 0x2C0 SIZE: 0x10
		tagInfo_s* tagInfo; //OFS: 0x2D0 SIZE: 0x4
		gentity_s* tagChildren; //OFS: 0x2D4 SIZE: 0x4
		void* animscripted_s__scripted; //OFS: 0x2D8 SIZE: 0x4
		__int16 attachTagNames[31]; //OFS: 0x2DC SIZE: 0x3E
		__int16 attachModelNames[31]; //OFS: 0x31A SIZE: 0x3E
		int disconnectedLinks; //OFS: 0x358 SIZE: 0x4
		int iDisconnectTime; //OFS: 0x35C SIZE: 0x4
		float angleLerpRate; //OFS: 0x360 SIZE: 0x4
		int physObjId; //OFS: 0x364 SIZE: 0x4
		XAnimTree_s* pAnimTree; //OFS: 0x368 SIZE: 0x4
		gentity_s* nextFree; //OFS: 0x36C SIZE: 0x4
		int scriptUse; //OFS: 0x370 SIZE: 0x4
		int birthTime; //OFS: 0x374 SIZE: 0x4
	};
	STATIC_ASSERT_SIZE(gentity_s, 0x378);

	struct animCmdState_s
	{
		int field_0;
		int field_4;
		int field_8;
		int field_C;
		int field_10;
		int field_14;
		int field_18;
		int field_1C;
		int field_20;
		int field_24;
		int field_28;
	};

	struct actorState_s
	{
		int actorIndex;
		int entityNum;
		team_t team;
		int modelindex;
		int attachModelIndex[6];
		int attachTagIndex[6];
		char name[32];
		int attachedEntNum;
		int attachedTagIndex;
		int animScriptedAnim;
		int hudwarningType;
		int lookAtEntNum;
		int lastLookAtEntNum;
	};

	struct __declspec(align(4)) snapshot_s
	{
		int snapFlags;
		int ping;
		int serverTime;
		playerState_s ps;
		int numEntities;
		int numClients;
		int numActors;
		int field_20C4;
		animCmdState_s parseAnimCmds[1117];
		int field_E0C4;
		char what;
		char whatthe[1024];
		BYTE gap_E4C9[3];
		entityState_s parseEntities[1024];
		clientState_s parseClients[4];
		actorState_s parseActors[32];
		int serverCommandSequence;
	};

	union cpose_u
	{
		char pad[136];
	};

	struct __declspec(align(4)) cpose_t
	{
		unsigned __int16 lightingHandle;
		unsigned __int8 eType;
		unsigned __int8 eTypeUnion;
		unsigned __int8 localClientNum;
		bool isRagdoll;
		int ragdollHandle;
		int physObjId;
		int physUserBody;
		unsigned __int8 destructiblePose;
		int startBurnTime;
		float wetness;
		int cullIn;
		float origin[3];
		float angles[3];
		float mins[3];
		cpose_u u;
	};

	struct __declspec(align(4)) cg_s
	{
		int clientNum;
		int localClientNum;
		DemoType demoType;
		CubemapShot cubemapShot;
		int cubemapSize;
		int renderScreen;
		int latestSnapshotNum;
		int snapServerTime;
		int loaded;
		snapshot_s* snap;
		snapshot_s* nextSnap;
		snapshot_s activeSnapshots[2];
		void* centity_s_currTarget;
		XModel* knifeModel;
		float frameInterpolation;
		int frametime;
		int time;
		int time_real;
		int oldTime;
		int physicsTime;
		char mapRestart;
		int renderingThirdPerson;
		void* script_camera;
		playerState_s predictedPlayerState;
		char viewModelPose_pad[0xACFB8];
		cpose_t viewModelPose;
	}; STATIC_ASSERT_OFFSET(cg_s, viewModelPose, 0x159CFC);

	struct __declspec(align(4)) clSnapshot_s
	{
		int valid;
		int snapFlags;
		int serverTime;
		int messageNum;
		int deltaNum;
		int ping;
		int whatistthis;
		playerState_s ps;
		int numEntities;
		int numClients;
		int numActors;
		int parseEntitiesNum;
		int parseClientsNum;
		int parseActorsNum;
		int parseAnimCmdsNum;
		int numAnimCmds;
		int serverCommandSequence;
	};

	struct __declspec(align(4)) clientActive_s
	{
		char usingAds;
		int timeoutcount;
		clSnapshot_s snap;
		char alwaysFalse;
		int serverTime;
		int oldServerTime;
		int oldFrameServerTime;
		int serverTimeDelta;
		int oldSnapServerTime;
		int extrapolatedSnapshot;
		int newSnapshots;
	};

	enum scriptInstance_t : int
	{
		SCRIPTINSTANCE_SERVER = 0x0,
		SCRIPTINSTANCE_CLIENT = 0x1,
		SCRIPT_INSTANCE_MAX = 0x2,
	};

	enum VariableType
	{
		VAR_UNDEFINED = 0x0,
		VAR_BEGIN_REF = 0x1,
		VAR_POINTER = 0x1,
		VAR_STRING = 0x2,
		VAR_ISTRING = 0x3,
		VAR_VECTOR = 0x4,
		VAR_END_REF = 0x5,
		VAR_FLOAT = 0x5,
		VAR_INTEGER = 0x6,
		VAR_CODEPOS = 0x7,
		VAR_PRECODEPOS = 0x8,
		VAR_FUNCTION = 0x9,
		VAR_BUILTIN_FUNCTION = 0xA,
		VAR_BUILTIN_METHOD = 0xB,
		VAR_STACK = 0xC,
		VAR_ANIMATION = 0xD,
		VAR_PRE_ANIMATION = 0xE,
		VAR_THREAD = 0xF,
		VAR_NOTIFY_THREAD = 0x10,
		VAR_TIME_THREAD = 0x11,
		VAR_CHILD_THREAD = 0x12,
		VAR_OBJECT = 0x13,
		VAR_DEAD_ENTITY = 0x14,
		VAR_ENTITY = 0x15,
		VAR_ARRAY = 0x16,
		VAR_DEAD_THREAD = 0x17,
		VAR_COUNT = 0x18,
		VAR_FREE = 0x18,
		VAR_THREAD_LIST = 0x19,
		VAR_ENDON_LIST = 0x1A,
		VAR_TOTAL_COUNT = 0x1B,
	};

	struct scr_entref_s
	{
		unsigned short entnum;
		unsigned short classnum;
	};

	typedef void(__cdecl* BuiltinFunction)();
	typedef void(__cdecl* BuiltinMethod)(scr_entref_s);

	enum dvar_flags : unsigned __int16
	{
		none = 0,
		archive = 1 << 0,			// 0x0001
		userinfo = 1 << 1,			// 0x0002
		serverinfo = 1 << 2,		// 0x0004
		systeminfo = 1 << 3,		// 0x0008
		init_flag = 1 << 4,				// 0x0010
		latched = 1 << 5,			// 0x0020
		readonly = 1 << 6,			// 0x0040
		cheat = 1 << 7,				// 0x0080
		developer = 1 << 8,			// 0x0100
		saved = 1 << 9,				// 0x0200
		no_restart = 1 << 10,		// 0x0400
		changeable_reset = 1 << 12,	// 0x1000
		external = 1 << 14,			// 0x4000
		autoexec = 1 << 15,			// 0x8000
	};

	enum dvar_type : __int8
	{
		boolean = 0x0,
		value = 0x1,
		vec2 = 0x2,
		vec3 = 0x3,
		vec4 = 0x4,
		integer = 0x5,
		enumeration = 0x6,
		string = 0x7,
		color = 0x8,
		rgb = 0x9,
	};

#pragma pack(push, 4)
	union DvarValue
	{
		bool enabled;
		int integer;
		unsigned int unsignedInt;
		float value;
		float vector[4];
		const char* string;
		char color[4];
	}; STATIC_ASSERT_SIZE(DvarValue, 0x10);
#pragma pack(pop)

	union DvarLimits
	{
		struct
		{
			int stringCount;
			const char** strings;
		} enumeration;
		struct
		{
			int min;
			int max;
		} integer;
		struct
		{
			float min;
			float max;
		} value;
		struct
		{
			float min;
			float max;
		} vector;
	};

	struct dvar_s
	{
		const char* name;
		const char* description;
		dvar_flags flags;
		dvar_type type;
		bool modified;
		char pad[4];
		DvarValue current;
		DvarValue latched;
		DvarValue reset;
		DvarValue saved;
		DvarLimits domain;
		dvar_s* hashNext;
	};
	STATIC_ASSERT_SIZE(dvar_s, 0x5C);

	struct cmd_function_s
	{
		cmd_function_s* next; //OFS: 0x0 SIZE: 0x4
		const char* name; //OFS: 0x4 SIZE: 0x4
		const char* autoCompleteDir; //OFS: 0x8 SIZE: 0x4
		const char* autoCompleteExt; //OFS: 0xC SIZE: 0x4
		void(__cdecl* function)(); //OFS: 0x10 SIZE: 0x4
	};

	struct CmdArgs
	{
		int nesting; //OFS: 0x0 SIZE: 0x4
		int localClientNum[8]; //OFS: 0x4 SIZE: 0x20
		int controllerIndex[8]; //OFS: 0x24 SIZE: 0x20
		int argc[8]; //OFS: 0x44 SIZE: 0x20
		char** argv[8]; //OFS: 0x64 SIZE: 0x20
	};

	struct weaponParms
	{
		float forward[3];
		float right[3];
		float up[3];
		float muzzleTrace[3];
		float gunForward[3];
		void* WeaponDef___weapDef;
	};

	struct DSkelPartBits
	{
		int anim[4];
		int control[4];
		int skel[4];
	};

	struct __declspec(align(4)) DSkel
	{
		DSkelPartBits partBits;
		int timeStamp;
		DObjAnimMat* mat;
	};

	struct DObj_s
	{
		XAnimTree_s* tree;
		unsigned __int16 duplicateParts;
		unsigned __int16 entnum;
		unsigned __int8 duplicatePartsSize;
		unsigned __int8 numModels;
		unsigned __int8 numBones;
		unsigned int ignoreCollision;
		int locked;
		DSkel skel;
		float radius;
		unsigned int hidePartBits[4];
		char localClientIndex;
		unsigned __int8 flags;
		unsigned __int16 ikStateIndex;
		XModel** models;
	};

	struct weaponInfo_s
	{
		DObj_s* viewModelDObj;
		XModel* handModel;
		XModel* gogglesModel;
		XModel* rocketModel;
		XModel* knifeModel;
		unsigned __int8 weapModelIdx;
		unsigned int partBits[4];
		int iPrevAnim;
		int hasAnimTree;
		XAnimTree_s* tree;
		int registered;
		void* gitem_s___item;
		char* translatedDisplayName;
		char* translatedModename;
		char* translatedAIOverlayDescription;
	};

	enum XAssetType : __int32
	{
		ASSET_TYPE_XMODELPIECES = 0x0,
		ASSET_TYPE_PHYSPRESET = 0x1,
		ASSET_TYPE_PHYSCONSTRAINTS = 0x2,
		ASSET_TYPE_DESTRUCTIBLEDEF = 0x3,
		ASSET_TYPE_XANIMPARTS = 0x4,
		ASSET_TYPE_XMODEL = 0x5,
		ASSET_TYPE_MATERIAL = 0x6,
		ASSET_TYPE_TECHNIQUE_SET = 0x7,
		ASSET_TYPE_IMAGE = 0x8,
		ASSET_TYPE_SOUND = 0x9,
		ASSET_TYPE_LOADED_SOUND = 0xA,
		ASSET_TYPE_CLIPMAP = 0xB,
		ASSET_TYPE_CLIPMAP_PVS = 0xC,
		ASSET_TYPE_COMWORLD = 0xD,
		ASSET_TYPE_GAMEWORLD_SP = 0xE,
		ASSET_TYPE_GAMEWORLD_MP = 0xF,
		ASSET_TYPE_MAP_ENTS = 0x10,
		ASSET_TYPE_GFXWORLD = 0x11,
		ASSET_TYPE_LIGHT_DEF = 0x12,
		ASSET_TYPE_UI_MAP = 0x13,
		ASSET_TYPE_FONT = 0x14,
		ASSET_TYPE_MENULIST = 0x15,
		ASSET_TYPE_MENU = 0x16,
		ASSET_TYPE_LOCALIZE_ENTRY = 0x17,
		ASSET_TYPE_WEAPON = 0x18,
		ASSET_TYPE_SNDDRIVER_GLOBALS = 0x19,
		ASSET_TYPE_FX = 0x1A,
		ASSET_TYPE_IMPACT_FX = 0x1B,
		ASSET_TYPE_AITYPE = 0x1C,
		ASSET_TYPE_MPTYPE = 0x1D,
		ASSET_TYPE_CHARACTER = 0x1E,
		ASSET_TYPE_XMODELALIAS = 0x1F,
		ASSET_TYPE_RAWFILE = 0x20,
		ASSET_TYPE_STRINGTABLE = 0x21,
		ASSET_TYPE_PACK_INDEX = 0x22,
		ASSET_TYPE_COUNT = 0x23,
		ASSET_TYPE_STRING = 0x23,
		ASSET_TYPE_ASSETLIST = 0x24,
	};

	struct Font_s
	{
		char* fontName;
		int pixelHeight;
		int glyphCount;
		Material* material;
		Material* glowMaterial;
		void* Glyph___glyphs;
	};

	struct FxEffectDef
	{
		const char* name;
		int flags;
		int totalSize;
		int msecLoopingLife;
		int elemDefCountLooping;
		int elemDefCountOneShot;
		int elemDefCountEmission;
		void* FxElemDef___elemDefs;
	};

	struct FxEffect
	{
		FxEffectDef* def;
		volatile int status;
		unsigned __int16 firstElemHandle[3];
		unsigned __int16 firstSortedElemHandle;
		unsigned __int16 firstTrailHandle;
		unsigned __int16 randomSeed;
		unsigned __int16 owner;
		unsigned __int16 packedLighting;
		// incomplete;
	};

	union XAssetHeader
	{
		void* data;
		XModel* model;
		Material* material;
		MaterialTechniqueSet* techniqueSet;
		GfxImage* image;
		clipMap_t* clipMap;
		MapEnts* mapEnts;
		GfxWorld* gfxWorld;
		FxEffectDef* fx;
	};

	struct XAsset
	{
		XAssetType type;
		XAssetHeader header;
	};

	struct __declspec(align(4)) XAssetEntry
	{
		XAsset asset;
		char zoneIndex;
		bool inuse;
		unsigned __int16 nextHash;
		unsigned __int16 nextOverride;
		unsigned __int16 usageFrame;
	};

	union __declspec(align(4)) XAssetEntryPoolEntry
	{
		XAssetEntry entry;
		XAssetEntryPoolEntry* next;
	};

	struct DpvsView
	{
		unsigned int renderFxFlagsCull;
		DpvsPlane frustumPlanes[14];
		int frustumPlaneCount;
	};

	struct DpvsGlob
	{
		DpvsPlane viewPlane;
		DpvsPlane fogPlane;
		DpvsPlane* nearPlane;
		DpvsPlane* farPlane;
		GfxMatrix* viewProjMtx;
		GfxMatrix* invViewProjMtx;
		int pad;
		float viewOrg[4];
		int viewOrgIsDir;
		int queuedCount;
		void* PortalHeapNode___portalQueue;
		void* GfxHullPointsPool___nextFreeHullPoints;
		float cullDist;
		DpvsPlane childPlanes[2048];
		DpvsView views[4][3];
		unsigned int cameraCellIndex;
		DpvsPlane* sideFrustumPlanes;
		unsigned int* entVisBits[4];
		unsigned int* cellBits;
		unsigned int cellVisibleBits[32];
	};
















	// ------------------------------------------------------------------------------------------------------------
	// ############################################################################################################
	// ------------------------------------------------------------------------------------------------------------

	namespace sp
	{

	}

















	// ------------------------------------------------------------------------------------------------------------
	// ############################################################################################################
	// ------------------------------------------------------------------------------------------------------------

	namespace mp
	{
		struct __declspec(align(4)) cg_s // TODO
		{
			int clientNum;
			int localClientNum;
			DemoType demoType;
			CubemapShot cubemapShot;
			int cubemapSize;
			int renderScreen;
			int latestSnapshotNum;
			int snapServerTime;
			int loaded;
			snapshot_s* snap;
			snapshot_s* nextSnap;
			snapshot_s activeSnapshots[2];
			void* centity_s_currTarget;
			XModel* knifeModel;
			float frameInterpolation;
			int frametime;
			int time;
			int time_real;
			int oldTime;
			int physicsTime;
			char mapRestart;
			int renderingThirdPerson;
			void* script_camera;
			playerState_s predictedPlayerState;
			// ......
		}; // 0x45C0

		struct predicted_playerstate_s
		{
			int pad[8];
			float origin[3];
			float velocity[3];
		};

		enum entityType_t : __int32
		{
			ET_GENERAL = 0x0,
			ET_PLAYER = 0x1,
			ET_PLAYER_CORPSE = 0x2,
			ET_ITEM = 0x3,
			ET_MISSILE = 0x4,
			ET_INVISIBLE = 0x5,
			ET_SCRIPTMOVER = 0x6,
			ET_SOUND_BLEND = 0x7,
			ET_FX = 0x8,
			ET_LOOP_FX = 0x9,
			ET_PRIMARY_LIGHT = 0xA,
			ET_MG42 = 0xB,
			ET_PLANE = 0xC,
			ET_VEHICLE = 0xD,
			ET_VEHICLE_COLLMAP = 0xE,
			ET_VEHICLE_CORPSE = 0xF,
			ET_ACTOR = 0x10,
			ET_ACTOR_SPAWNER = 0x11,
			ET_ACTOR_CORPSE = 0x12,
			ET_EVENTS = 0x13,
		};

		enum trType_t : __int32
		{
			TR_STATIONARY = 0x0,
			TR_INTERPOLATE = 0x1,
			TR_LINEAR = 0x2,
			TR_LINEAR_STOP = 0x3,
			TR_SINE = 0x4,
			TR_GRAVITY = 0x5,
			TR_ACCELERATE = 0x6,
			TR_DECELERATE = 0x7,
			TR_PHYSICS = 0x8,
			TR_XDOLL = 0x9,
			TR_FIRST_RAGDOLL = 0xA,
			TR_RAGDOLL = 0xA,
			TR_RAGDOLL_GRAVITY = 0xB,
			TR_RAGDOLL_INTERPOLATE = 0xC,
			TR_LAST_RAGDOLL = 0xC,
			TR_COUNT = 0xD,
		};

		struct trajectory_t
		{
			trType_t trType; //OFS: 0x0 SIZE: 0x4
			int trTime; //OFS: 0x4 SIZE: 0x4
			int trDuration; //OFS: 0x8 SIZE: 0x4
			float trBase[3]; //OFS: 0xC SIZE: 0xC
			float trDelta[3]; //OFS: 0x18 SIZE: 0xC
		};

		struct LerpEntityState
		{
			int eFlags; //OFS: 0x0 SIZE: 0x4
			trajectory_t pos; //OFS: 0x4 SIZE: 0x24
			trajectory_t apos; //OFS: 0x28 SIZE: 0x24
			char pad_u[0x3C]; //OFS: 0x4C SIZE: 0x3C
			int usecount; //OFS: 0x88 SIZE: 0x4
		};

		struct __declspec(align(2)) LoopSound
		{
			unsigned __int16 soundAlias; //OFS: 0x0 SIZE: 0x2
			__int16 fadeTime; //OFS: 0x2 SIZE: 0x2
		};

		union entityState_index
		{
			__int16 brushmodel; //OFS: 0x0 SIZE: 0x2
			__int16 xmodel; //OFS: 0x1 SIZE: 0x2
			__int16 primaryLight; //OFS: 0x2 SIZE: 0x2
			unsigned __int16 bone; //OFS: 0x3 SIZE: 0x2
			int pad; //OFS: 0x4 SIZE: 0x4
		};

		struct entityState_s
		{
			int number; //OFS: 0x0 SIZE: 0x4
			entityType_t eType; //OFS: 0x4 SIZE: 0x4
			LerpEntityState lerp; //OFS: 0x8 SIZE: 0x8C
			int time2; //OFS: 0x94 SIZE: 0x4
			int otherEntityNum; //OFS: 0x98 SIZE: 0x4
			int groundEntityNum; //OFS: 0x9C SIZE: 0x4
			LoopSound loopSound; //OFS: 0xA0 SIZE: 0x4
			int surfType; //OFS: 0xA4 SIZE: 0x4
			entityState_index index; //OFS: 0xA8 SIZE: 0x4
			int clientnum; //OFS: 0xAC SIZE: 0x4
			int iHeadIcon; //OFS: 0xB0 SIZE: 0x4
			int solid; //OFS: 0xB4 SIZE: 0x4
			int eventParm; //OFS: 0xB8 SIZE: 0x4
			int eventSequence; //OFS: 0xBC SIZE: 0x4
			int events[4]; //OFS: 0xC0 SIZE: 0x10
			int eventParms[4]; //OFS: 0xD0 SIZE: 0x10
			int weapon; //OFS: 0xE0 SIZE: 0x4
			int weaponModel; //OFS: 0xE4 SIZE: 0x4
			int targetname; //OFS: 0xE8 SIZE: 0x4
			char un1[0x4]; //OFS: 0xEC SIZE: 0x4
			char un2[0x10]; //OFS: 0xF0 SIZE: 0x10
			char un3[0x8]; //OFS: 0x100 SIZE: 0x4
			int animtreeIndex; //OFS: 0x104 SIZE: 0x4
			int partBits[4]; //OFS: 0x108 SIZE: 0x10
		};

		struct __declspec(align(2)) EntHandle
		{
			unsigned __int16 number; //OFS: 0x0 SIZE: 0x2
			unsigned __int16 infoIndex; //OFS: 0x2 SIZE: 0x2
		};

		enum contents_e
		{
			CONTENTS_SOLID = 0x1,
			CONTENTS_FOLIAGE = 0x2,
			CONTENTS_NONCOLLIDING = 0x4,
			CONTENTS_GLASS = 0x10,
			CONTENTS_WATER = 0x20,
			CONTENTS_CANSHOOTCLIP = 0x40,
			CONTENTS_MISSILECLIP = 0x80,
			CONTENTS_ITEM = 0x100,
			CONTENTS_VEHICLECLIP = 0x200,
			CONTENTS_ITEMCLIP = 0x400,
			CONTENTS_SKY = 0x800,
			CONTENTS_AI_NOSIGHT = 0x1000,
			CONTENTS_CLIPSHOT = 0x2000,
			CONTENTS_CORPSE_CLIPSHOT = 0x4000,
			CONTENTS_ACTOR = 0x8000,
			CONTENTS_FAKE_ACTOR = 0x8000,
			CONTENTS_PLAYERCLIP = 0x10000,
			CONTENTS_MONSTERCLIP = 0x20000,
			CONTENTS_PLAYERVEHICLECLIP = 0x40000,
			CONTENTS_USE = 0x200000,
			CONTENTS_UTILITYCLIP = 0x400000,
			CONTENTS_VEHICLE = 0x800000,
			CONTENTS_MANTLE = 0x1000000,
			CONTENTS_PLAYER = 0x2000000,
			CONTENTS_CORPSE = 0x4000000,
			CONTENTS_DETAIL = 0x8000000,
			CONTENTS_STRUCTURAL = 0x10000000,
			CONTENTS_LOOKAT = 0x10000000,
			CONTENTS_TRIGGER = 0x40000000,
			CONTENTS_NODROP = 0x80000000,
		};

		struct entityShared_s
		{
			unsigned __int8 linked; //OFS: 0x0 SIZE: 0x1
			unsigned __int8 bmodel; //OFS: 0x1 SIZE: 0x1
			unsigned __int16 svFlags; //OFS: 0x2 SIZE: 0x2
			unsigned __int8 eventType; //OFS: 0x4 SIZE: 0x1
			unsigned __int8 inuse; //OFS: 0x5 SIZE: 0x1
			int clientMask[2]; //OFS: 0x8 SIZE: 0x8
			int broadcastTime; //OFS: 0x10 SIZE: 0x4
			float mins[3]; //OFS: 0x14 SIZE: 0xC
			float maxs[3]; //OFS: 0x20 SIZE: 0xC
			contents_e contents; //OFS: 0x2C SIZE: 0x4
			float absmin[3]; //OFS: 0x30 SIZE: 0xC
			float absmax[3]; //OFS: 0x3C SIZE: 0xC
			float currentOrigin[3]; //OFS: 0x48 SIZE: 0xC
			float currentAngles[3]; //OFS: 0x54 SIZE: 0xC
			EntHandle ownerNum; //OFS: 0x60 SIZE: 0x4
			int eventTime; //OFS: 0x64 SIZE: 0x4
		};

		struct __declspec(align(8)) gentity_s
		{
			entityState_s s; //OFS: 0x0 SIZE: 0x118
			entityShared_s r; //OFS: 0x118 SIZE: 0x68
			void* gclient_s__client; //OFS: 0x180 SIZE: 0x4
			void* actor_s__actor; //OFS: 0x184 SIZE: 0x4
			void* sentient_s__sentient; //OFS: 0x188 SIZE: 0x4
			void* scr_vehicle_s__scr_vehicle; //OFS: 0x18C SIZE: 0x4
			void* TurretInfo__pTurretInfo; //OFS: 0x190 SIZE: 0x4
			void* Destructible__destructible; //OFS: 0x194 SIZE: 0x4
			unsigned __int16 model; //OFS: 0x198 SIZE: 0x2
			unsigned __int8 physicsObject; //OFS: 0x19A SIZE: 0x1
			unsigned __int8 takedamage; //OFS: 0x19B SIZE: 0x1
			unsigned __int8 active; //OFS: 0x19C SIZE: 0x1
			unsigned __int8 nopickup; //OFS: 0x19D SIZE: 0x1
			unsigned __int8 handler; //OFS: 0x19E SIZE: 0x1
			unsigned __int16 classname; //OFS: 0x1A0 SIZE: 0x2
			unsigned __int16 script_linkName; //OFS: 0x1A2 SIZE: 0x2
			unsigned __int16 script_noteworthy; //OFS: 0x1A4 SIZE: 0x2
			unsigned __int16 target; //OFS: 0x1A6 SIZE: 0x2
			int targetname; //OFS: 0x1A8 SIZE: 0x4
			int spawnflags2; //OFS: 0x1AC SIZE: 0x4
			int spawnflags; //OFS: 0x1B0 SIZE: 0x4
			int flags; //OFS: 0x1B4 SIZE: 0x4
			int clipmask; //OFS: 0x1B8 SIZE: 0x4
			int processedFrame; //OFS: 0x1BC SIZE: 0x4
			EntHandle parent; //OFS: 0x1C0 SIZE: 0x4
			int nextthink; //OFS: 0x1C4 SIZE: 0x4
			int health; //OFS: 0x1C8 SIZE: 0x4
			int maxhealth; //OFS: 0x1CC SIZE: 0x4
			int nexteq; //OFS: 0x1D0 SIZE: 0x4
			// ...
		};
		STATIC_ASSERT_OFFSET(gentity_s, r, 0x11C);
		STATIC_ASSERT_OFFSET(gentity_s, model, 0x19C);
	}
}

