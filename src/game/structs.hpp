#pragma once

// https://github.com/JezuzLizard/T4SP-Server-Plugin/blob/main/src/game/xasset.hpp ( :* )

namespace game
{
	typedef float vec_t;
	typedef vec_t vec2_t[2];
	typedef vec_t vec3_t[3];
	typedef vec_t vec4_t[4];

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

	struct GfxBackEndData
	{
		char surfsBuffer[1330636]; // obv. not the correct size
		unsigned int viewInfoIndex;
		unsigned int viewInfoCount;
		GfxViewInfo* viewInfo;
	};
	STATIC_ASSERT_OFFSET(GfxBackEndData, viewInfo, 0x144DD4);

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
		char pad00[382];
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

	struct GfxVertexBufferState
	{
		volatile int used;
		int total;
		IDirect3DVertexBuffer9* buffer;
		char* verts;
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

	enum dvar_flags : unsigned __int16
	{
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

	union DvarValue
	{
		bool enabled;
		int integer;
		unsigned int unsignedInt;
		float value;
		float vector[4];
		const char* string;
		char color[4];
	};

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

	enum XAssetType
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

	union XAssetHeader
	{
		//XModelPieces *xmodelPieces;
		//PhysPreset *physPreset;
		//XAnimParts *parts;
		XModel *model;
		//Material *material;
		//MaterialPixelShader *pixelShader;
		//MaterialVertexShader *vertexShader;
		//MaterialTechniqueSet *techniqueSet;
		GfxImage *image;
		//snd_alias_list_t *sound;
		//SndCurve *sndCurve;
		//clipMap_t *clipMap;
		//ComWorld *comWorld;
		//GameWorldSp *gameWorldSp;
		//GameWorldMp *gameWorldMp;
		//MapEnts *mapEnts;
		//GfxWorld *gfxWorld;
		//GfxLightDef *lightDef;
		//Font_s *font;
		//MenuList *menuList;
		//menuDef_t *menu;
		//LocalizeEntry *localize;
		//WeaponDef *weapon;
		//SndDriverGlobals *sndDriverGlobals;
		//FxEffectDef *fx;
		//FxImpactTable *impactFx;
		//RawFile *rawfile;
		//StringTable *stringTable;
		void* data;
	};

	struct XAsset
	{
		XAssetType type;
		XAssetHeader header;
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

	}
}

