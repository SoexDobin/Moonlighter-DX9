#include "CGameObject.h"
#include "CEditor.h"


CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDevice(pGraphicDev), m_bDisplayInEditor(false)
{
	m_pGraphicDevice->AddRef();
}

CGameObject::CGameObject(const CGameObject& rhs)
	: m_pGraphicDevice(rhs.m_pGraphicDevice), m_bDisplayInEditor(false)
{
	m_pGraphicDevice->AddRef();
}

CGameObject::~CGameObject()
{
}

CComponent* CGameObject::Get_Component(COMPONENTID eID, const wstring& wsComponentTag)
{
	CComponent* pComponent(nullptr);

	if (pComponent = Find_Component(eID, wsComponentTag))
		return pComponent;
	else 
		return nullptr;
}

CComponent* CGameObject::Find_Component(COMPONENTID eID, const wstring& wsComponentTag)
{
	auto iter = find_if(m_umComponent[eID].begin(), m_umComponent[eID].end()
		, [&wsComponentTag](pair<const wstring, CComponent*>& pair) -> _bool {
			if (pair.first == wsComponentTag)
				return true;

			return false;
		});

	if (iter == m_umComponent[eID].end()) return nullptr;
	
	return iter->second;
}

HRESULT CGameObject::Ready_GameObject()
{
	return S_OK;
}

_int CGameObject::Update_GameObject(const _float fTimeDelta)
{
	for_each(m_umComponent[ID_DYNAMIC].begin(), m_umComponent[ID_DYNAMIC].end()
		, [fTimeDelta](pair<const wstring, CComponent*>& pair) -> void {
			pair.second->Update_Component(fTimeDelta);
		});

	return 0;
}

void CGameObject::LateUpdate_GameObject(const _float fTimeDelta)
{
	for_each(m_umComponent[ID_DYNAMIC].begin(), m_umComponent[ID_DYNAMIC].end()
		, [fTimeDelta](pair<const wstring, CComponent*>& pair) -> void {
			pair.second->LateUpdate_Component();
		});
}

void CGameObject::Render_GameObject()
{
}

void CGameObject::Free()
{
	for (int i = 0; i < ID_END; ++i)
	{
		for_each(m_umComponent[i].begin(), m_umComponent[i].end(), CDeleteMap());
		m_umComponent[i].clear();
	}
		
	Safe_Release(m_pGraphicDevice);
}

void	CGameObject::Display_Editor()
{
	if (!m_bDisplayInEditor)
		return;

	ImGui::Begin(m_szBuffer);

#pragma region Component
	ImGui::Text("--- Component ---");

	_int dwIndex = 0;
	for (int i = ID_DYNAMIC; i < ID_END; ++i)
	{
		for (auto& component : m_umComponent[i])
		{
			ImGui::Checkbox(("##" + to_string((uintptr_t)component.second)).c_str(), &component.second->m_bDisplayInEditor);  ImGui::SameLine();
			ImGui::Text("%ls", component.second->m_szDisplayName);

			component.second->Display_Editor(m_szBuffer);
		}
	}
#pragma endregion

#pragma region Data
	ImGui::Text("--- Data ---");

	for (auto& field : m_EditorFieldList)
	{
		ImGui::PushItemWidth(120);

		CEditor::GetInstance()->Display_Editor(field);

		ImGui::PopItemWidth();
	}
#pragma endregion

	ImGui::End();
}
