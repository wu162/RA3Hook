#pragma once

namespace RA3::Core {
	void __fastcall C_ObjectCreationList_Hook();
	void __fastcall C_ObjectCreationList_Initialize(uintptr_t hmodEXE, int isNewSteam);

	void __fastcall C_OCL_FlingObjectCreateObjectASM();
}
