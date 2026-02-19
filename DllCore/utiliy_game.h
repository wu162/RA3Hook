#pragma once

float __cdecl getRadomFloatValue(float minValue, float maxValue);
void* __fastcall newMemoryAllocation(UINT32 size);
void* __cdecl GameMemoryAllocation(UINT32 size);

void __cdecl GameMemoryFree(void* p);

void __fastcall InitializeUtiliyGameFunctionsOrigin(uintptr_t hmodEXE);
