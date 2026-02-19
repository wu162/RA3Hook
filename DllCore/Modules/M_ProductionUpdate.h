#pragma once

/*
struct BuildList_Produced
{
	BYTE pad1[0x10];
	void* objectHash;
	BYTE pad2[0x40];
	BuildList_Produced* next;
};*/

// size is 0x70, init is 0070F2C0
typedef struct BuildList_Produced_t {
	void* vf_table;
	void* pProductionUpdate;
	char pad8[0x14];
	float BuildProgress, BuildSpeed;
	int ElapsedTime; // frame
	char pad28[0x8];
	int BuildCost; // maybe
}*pBuildList_Produced;
#if 1
static_assert(offsetof(BuildList_Produced_t, ElapsedTime) == 0x24);
static_assert(offsetof(BuildList_Produced_t, BuildCost) == 0x30);
#endif

// old size is 0x14C =========================================
typedef struct M_ProductionUpdate24_t {
	struct vft24_t {
		int(__fastcall* func00)(M_ProductionUpdate24_t* pIn, void* fn, void* pThingTemplate); // check it is buildable. 0 is ok.
		void* func04;
		void* func08;
		void* func0C;
		void* func10;
		int(__fastcall* func14)(M_ProductionUpdate24_t* pIn, void* fn, void* pBuildUnit, int hasShift); // remove from build list
		void* func18;
		void* func1C;
		void* func20;
		int(__fastcall* func24)(M_ProductionUpdate24_t* pIn, void* fn, void* pBuildUnit, int hasShift, uintptr_t addrCD7E84, int a4, int a5, int a6, int a7, int a8); // add to build list
		void* func28;
		void* func2C;
		void* func30;
		void* func34;
		void* func38;
		void* func3C;
		void* func40;
		void* func44;
		void* func48;
		void* func4C;
		void* func50;
		void* func54;
		void* func58;
		void* func5C;
		void* func60;
		void* func64;
		void* func68;
		void* func6C;
		void* func70;
		void* func74;
		void* func78;
		void* func7C;
		void* func80;
		bool(__fastcall* func84)(M_ProductionUpdate24_t* pIn); // CheckCanBuild
		void* func88;
		void* func8C;
		bool(__fastcall* func90)(M_ProductionUpdate24_t* pIn, void* fn, void* pThingTemplate); // check can build this unit. false is ok.
		// ac is get queue type
	};

	vft24_t* BuildList_vft;
	char pad28[0xC];
	void* FirstBuildUnit;
	void* LastBuildUnit;
	int RequestedBuildCount; // it is a cumulative count, which includes uncompleted items.
	int CurrentBuildCount;
	char pad44[0xF0];
	bool bDisableBuild;
	char pad135[0x17];
	// next is new
	void* LoopBuildUnit; // is pC_ThingTemplate?
	int LoopCoolDown;
}*pM_ProductionUpdate24;

typedef struct M_ProductionUpdate_t : baseCommonModule_t {
	M_ProductionUpdate24_t data;
}*pM_ProductionUpdate;
#if 1
	static_assert(offsetof(M_ProductionUpdate_t, data.BuildList_vft) == 0x24);
	static_assert(offsetof(M_ProductionUpdate_t, data.FirstBuildUnit) == 0x24+0x10);
	static_assert(offsetof(M_ProductionUpdate_t, data.bDisableBuild) == 0x134);
	static_assert(offsetof(M_ProductionUpdate_t, data.LoopBuildUnit) == 0x14C);
#endif
// ==================================================================================

namespace RA3::Module {
	void __fastcall M_ProductionUpdate_Hook();
	void __fastcall M_ProductionUpdate_Initialize(uintptr_t hmodEXE, int isNewSteam);

	void __fastcall M_ProductionUpdate_InitializeASM();
	void __fastcall M_ProductionUpdate_AddToBuildListASM();
	void __fastcall M_ProductionUpdate_AddToBuildListCPP(pC_GameObject pIn, void* pBuildUnit, int queueType);
	void __fastcall M_ProductionUpdate_RemoveFromBuildListASM();
	// vtf24+0x24
	void __fastcall M_ProductionUpdate_GetNewBuildCountASM();
	// vft24+0x84
	bool __fastcall M_ProductionUpdate24_CheckCanBuild(pM_ProductionUpdate24 pIn);

	//
	pM_ProductionUpdate24 __fastcall M_ProductionUpdate24_GetFromQueueType(void* pGO, int useless, int type);
}
