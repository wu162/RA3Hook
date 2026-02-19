#pragma once
#include <map>
#include <vector>

typedef struct EAStlVector_t
{
	void* begin;
	void* end;
	void* capacity;
}*pEAStlVector;

typedef struct C_FXParticleSystemTemplate_t {
	BYTE pad0[0x38];
	EAStlVector_t pFXsys;
	char pad44[0x30];
	void* Shader;
	UINT32 ShaderType;
}*pC_FXParticleSystemTemplate;
#if 1
static_assert(offsetof(C_FXParticleSystemTemplate_t, Shader) == 0x74);
static_assert(offsetof(C_FXParticleSystemTemplate_t, ShaderType) == 0x78);
#endif

// size is 0x148
typedef struct C_FXParticleSystem_t {
	void* vf_table; // 00C1BDA8
	char pad4[4];
	pC_FXParticleSystemTemplate pStaticData;
	char padC[0x34];
	char bitFlag40, bitFlag41;
}*pC_FXParticleSystem;
#if 1
static_assert(offsetof(C_FXParticleSystem_t, bitFlag40) == 0x40);
#endif

typedef struct C_FXParticleSystemManagerNode_t {
	C_FXParticleSystemManagerNode_t* next;
	C_FXParticleSystemManagerNode_t* previous;
	pC_FXParticleSystem pFXsys;
}*pC_FXParticleSystemManagerNode;

typedef struct C_FXParticleSystemManager_t {
	void* vf_table; // 00C0A3A8
	char pad4[0x30];
	int TotalParticleCount; // total number that have been activated. reset after exiting map
	C_FXParticleSystemManagerNode_t node;
	char pad44[0x3C];
	int CurrentParticleCount; // activated particle count
}*pC_FXParticleSystemManager;
#if 1
static_assert(offsetof(C_FXParticleSystemManager_t, TotalParticleCount) == 0x34);
static_assert(offsetof(C_FXParticleSystemManager_t, node) == 0x38);
static_assert(offsetof(C_FXParticleSystemManager_t, CurrentParticleCount) == 0x80);
#endif

namespace RA3::Core {
	enum class FXParticleSystem_ShaderType : UINT32 {
		INVALID_SHADER,
		ADDITIVE,
		ADDITIVE_ALPHA_TEST,
		ALPHA,
		ALPHA_TEST,
		MULTIPLY,
		ADDITIVE_NO_DEPTH_TEST,
		ALPHA_NO_DEPTH_TEST,
		W3D_ALPHA,
		all_count,
	};

	struct ExtraFXParticleSystemManager_List_t {
		void* Shader;
		pC_FXParticleSystemManagerNode lastNode[(UINT32)FXParticleSystem_ShaderType::all_count];
	};

	typedef struct ExtraFXParticleSystemManager_t {
		std::vector<ExtraFXParticleSystemManager_List_t> list;
		std::map<pC_FXParticleSystem, pC_FXParticleSystemManagerNode> FXsysMap;
	}*pExtraFXParticleSystemManager;

	void __fastcall C_FXParticleSystem_Hook();
	void __fastcall C_FXParticleSystem_Initialize(uintptr_t hmodEXE, int isNewSteam);

	void __fastcall C_FXParticleSystem_AddToManager(pC_FXParticleSystemManager pManager, int useless, pC_FXParticleSystem** ParticleSystemRef);
	void __fastcall C_FXParticleSystem_RemoveFromManager(pC_FXParticleSystemManager pManager, int useless, pC_FXParticleSystem** ParticleSystemRef);
	void __fastcall C_FXParticleSystem_ResetManager();

	void __fastcall EAStlVector_Pushback(pEAStlVector pIn, void* pData);

	pC_FXParticleSystemManagerNode __fastcall Extra_FXParticleSystem_GetManagerNode(pC_FXParticleSystemManagerNode pDefault, pC_FXParticleSystemTemplate pData,
																					pC_FXParticleSystemManagerNode pInput);
	void __fastcall Extra_FXParticleSystem_ClearManagerNode(pC_FXParticleSystemManagerNode pDefault, pC_FXParticleSystemManagerNode pInput);
	int __fastcall Extra_FXParticleSystem_GetListIndex(void* Shader, int NeedInsert);
}
