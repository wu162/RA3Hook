#include "../pch.h"

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "../Base/GlobalStructure.h"
#include "C_FXParticleSystem.h"

namespace RA3::Core {

	pExtraFXParticleSystemManager pExtraFXManager;

	uintptr_t _F_FXParticleSystem_AddToManager = 0x6D1BBC;
	uintptr_t _F_FXParticleSystem_RemoveFromManager = 0x6D1BC9;
	uintptr_t _F_FXParticleSystem_ResetManager = 0x6D1D23;

	uintptr_t _call_FXParticleSystem_FreeParticleSystemRef = 0x6C5BC0;
	uintptr_t _call_EAStlVector_pushback = 0x982FE0;

	void __fastcall C_FXParticleSystem_Hook()
	{
		hookGameCall((void*)_F_FXParticleSystem_AddToManager, (uintptr_t)C_FXParticleSystem_AddToManager);
		hookGameCall((void*)_F_FXParticleSystem_RemoveFromManager, (uintptr_t)C_FXParticleSystem_RemoveFromManager);
		hookGameBlock((void*)_F_FXParticleSystem_ResetManager, (uintptr_t)C_FXParticleSystem_ResetManager);

		pExtraFXManager = new ExtraFXParticleSystemManager_t;
		pExtraFXManager->list.reserve(13);
	}

	void __fastcall C_FXParticleSystem_Initialize(uintptr_t hmodEXE, int isNewSteam)
	{
		if (isNewSteam){
			_F_FXParticleSystem_AddToManager = 0x7100DC;
			_F_FXParticleSystem_RemoveFromManager = 0x7100E9;
			_F_FXParticleSystem_ResetManager = 0x710243;

			_call_FXParticleSystem_FreeParticleSystemRef = 0x704170;
			_call_EAStlVector_pushback = 0xA38A30;
		}
	}

	void __fastcall C_FXParticleSystem_AddToManager(pC_FXParticleSystemManager pManager, int useless, pC_FXParticleSystem** ParticleSystemRef)
	{
		auto ParticleNode = pManager->node.next; // ebp
		auto pParticleSys = **ParticleSystemRef; // esp+18
		auto pData = pParticleSys->pStaticData; // ebx

		// now it is at the start.
		auto pNode = (pC_FXParticleSystemManagerNode)GameMemoryAllocation(12);
		// if (pNode) is useless
		pExtraFXManager->FXsysMap[pParticleSys] = pNode;
		pNode->pFXsys = pParticleSys;

		pC_FXParticleSystemManagerNode NullNode = &pManager->node; // esi
		auto cur_node = ParticleNode;
		auto cur_shader = pData->Shader; // edi
		if (cur_shader) {
			if (cur_node != NullNode) {
				cur_node = Extra_FXParticleSystem_GetManagerNode(NullNode, pData, pNode);
				/*auto cur_shaderType = pData->ShaderType;
				while (1) {
					auto nextData = cur_node->pFXsys->pStaticData;

					if (nextData->Shader < cur_shader) break;
					if (nextData->ShaderType < cur_shaderType) break;

					cur_node = cur_node->next;
					if (cur_node == NullNode) break;
				}*/
			}
			// end
		} else {
			cur_node = NullNode;
		}

		pC_FXParticleSystemManagerNode nextNode = NullNode;
		if (cur_node != NullNode){
			if (cur_node != ParticleNode) {
				nextNode = cur_node->previous;
			} else {
				nextNode = ParticleNode;
			}
		}
		pNode->next = nextNode;
		pNode->previous = nextNode->previous;
		nextNode->previous->next = pNode;
		nextNode->previous = pNode;

		EAStlVector_Pushback(&pData->pFXsys, &pParticleSys);

		pParticleSys->bitFlag41 |= 8;
		pManager->CurrentParticleCount += 1;
	}

	void __fastcall C_FXParticleSystem_RemoveFromManager(pC_FXParticleSystemManager pManager, int useless, pC_FXParticleSystem** ParticleSystemRef)
	{
		auto ParticleNode = pManager->node.next;
		auto pParticleSys = **ParticleSystemRef;
		pC_FXParticleSystemManagerNode NullNode = &pManager->node;

		if (ParticleNode == NullNode) return;

		pC_FXParticleSystemManagerNode cur_node;
		auto it = pExtraFXManager->FXsysMap.find(pParticleSys);
		if (it != pExtraFXManager->FXsysMap.end()) {
			cur_node = it->second;
			pExtraFXManager->FXsysMap.erase(it);

			if (cur_node == NullNode) return;
		} else {
			cur_node = ParticleNode;
			while (1) {
				if (cur_node->pFXsys == pParticleSys) break;

				cur_node = cur_node->next;
				if (cur_node == NullNode) return;
			}
		}

		Extra_FXParticleSystem_ClearManagerNode(NullNode, cur_node);

		auto nextNode = cur_node->next;
		auto previousNode = cur_node->previous;
		previousNode->next = nextNode;
		nextNode->previous = previousNode;

		GameMemoryFree(cur_node);

		pParticleSys->bitFlag41 &= 0xF7;

		typedef void* (__fastcall* callFunc)(pC_FXParticleSystem** ParticleSystemRef);
		auto fn = (callFunc)_call_FXParticleSystem_FreeParticleSystemRef;
		fn(ParticleSystemRef);

		pManager->CurrentParticleCount -= 1;
	}

	void __fastcall C_FXParticleSystem_ResetManager()
	{
		pExtraFXManager->list.clear();
		pExtraFXManager->FXsysMap.clear();
	}

	void __fastcall EAStlVector_Pushback(pEAStlVector pIn, void* pData)
	{
		auto v_end = pIn->end;
		if (v_end < pIn->capacity) {
			auto new_end = (uintptr_t*)v_end;
			auto p_data = (uintptr_t*)pData;
			*new_end = *p_data;

			new_end += 1;
			pIn->end = (void*)new_end;
		} else {
			typedef void* (__fastcall* callFunc)(pEAStlVector pIn, void* func, void* pEnd, void* pData);
			auto fn = (callFunc)_call_EAStlVector_pushback;
			fn(pIn, fn, v_end, pData);
		}
	}

	pC_FXParticleSystemManagerNode __fastcall Extra_FXParticleSystem_GetManagerNode(pC_FXParticleSystemManagerNode pDefault, pC_FXParticleSystemTemplate pData, pC_FXParticleSystemManagerNode pInput)
	{
		auto index = Extra_FXParticleSystem_GetListIndex(pData->Shader, 1);

		auto shaderType = pData->ShaderType;
		if (shaderType < (UINT32)FXParticleSystem_ShaderType::all_count) {
			auto pNode = pExtraFXManager->list[index].lastNode[shaderType];
			if (pNode) {
				return pNode;
			} else {
				pExtraFXManager->list[index].lastNode[shaderType] = pInput;
				return pDefault->next;
			}
		}

		return pDefault;
	}

	void __fastcall Extra_FXParticleSystem_ClearManagerNode(pC_FXParticleSystemManagerNode pDefault, pC_FXParticleSystemManagerNode pInput)
	{
		auto pParticleSys = pInput->pFXsys;
		auto pData = pParticleSys->pStaticData;

		auto shader = pData->Shader;
		if (!shader) return;

		auto index = Extra_FXParticleSystem_GetListIndex(pData->Shader, 0);
		if (index == -1) return;

		auto shaderType = pData->ShaderType;
		if (shaderType >= (UINT32)FXParticleSystem_ShaderType::all_count) return;

		auto pNode = pExtraFXManager->list[index].lastNode[shaderType];
		if (pNode != pInput) return;

		auto prevNode = pInput->previous;
		if (prevNode == pDefault) {
			pExtraFXManager->list[index].lastNode[shaderType] = nullptr;
			return;
		}

		auto prevData = prevNode->pFXsys->pStaticData;
		if (prevData->Shader != shader) {
			pExtraFXManager->list[index].lastNode[shaderType] = nullptr;
			return;
		}

		if (prevData->ShaderType != shaderType) {
			pExtraFXManager->list[index].lastNode[shaderType] = nullptr;
		} else {
			pExtraFXManager->list[index].lastNode[shaderType] = prevNode;
		}
	}

	int __fastcall Extra_FXParticleSystem_GetListIndex(void* Shader, int NeedInsert)
	{
		int v_size = pExtraFXManager->list.size();
		for (int i = 0; i < v_size; i++) {
			if (pExtraFXManager->list[i].Shader == Shader) {
				return i;
			}
		}

		if(!NeedInsert) return -1;

		ExtraFXParticleSystemManager_List_t out;
		out.Shader = Shader;
		ZeroMemory(out.lastNode, sizeof(out.lastNode));

		pExtraFXManager->list.push_back(out);
		return v_size;
	}

// end namespace RA3::Core
}
