#include "CGameObject.h"
#include "CPrototypeManager.h"
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

// 기존에 사용하던 가지고 있는 컴포넌트의 wstring 테그를 통한 검색
CComponent* CGameObject::Get_Component(COMPONENTID eID, const wstring& wsComponentKey)
{
	CComponent* pComponent(nullptr);

	if (pComponent = Find_Component(eID, wsComponentKey))
		return pComponent;
	else
		return nullptr;
}
// 컴포넌트 타입을 검색해서 맨 먼저 발견된 첫번째 컴포넌트를 반환
CComponent* CGameObject::Get_Component(COMPONENTID eID, PROTOTYPE_COMPONENT ePrototype)
{
    auto iter = find_if(m_umComponent[eID].begin(), m_umComponent[eID].end()
        , [&ePrototype](const pair<const wstring, CComponent*>& pair) -> _bool {
            if (pair.second->Get_ComponentType() == ePrototype)
                return true;

            return false;
        });

    if (iter == m_umComponent[eID].end()) return nullptr;

    return iter->second;
}

CComponent* CGameObject::Find_Component(COMPONENTID eID, const wstring& wsComponentTag)
{
	auto iter = find_if(m_umComponent[eID].begin(), m_umComponent[eID].end()
		, [&wsComponentTag](const pair<const wstring, CComponent*>& pair) -> _bool {
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
		, [fTimeDelta](const pair<const wstring, CComponent*>& pair) -> void {
			pair.second->Update_Component(fTimeDelta);
		});

	return 0;
}

void CGameObject::LateUpdate_GameObject(const _float fTimeDelta)
{
	for_each(m_umComponent[ID_DYNAMIC].begin(), m_umComponent[ID_DYNAMIC].end()
		, [fTimeDelta](const pair<const wstring, CComponent*>& pair) -> void {
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

    bool bOpen = ImGui::Begin("Main Editor");
    if (bOpen)
    {
        ImGui::Columns(2, "MainEditorColumns", true);

        ImGui::BeginChild("LeftColumn", ImVec2(0, 0), true);
        {

        }
        ImGui::EndChild();

        ImGui::NextColumn();

        ImGui::BeginChild("Object Info", ImVec2(0, 0), true);
        {
            ImGui::Text("[ Component ]");

            _int dwIndex = 0;
            for (int i = ID_DYNAMIC; i < ID_END; ++i)
            {
                for (auto& component : m_umComponent[i])
                {
                    if (ImGui::CollapsingHeader(component.second->m_szDisplayName, component.second->m_bDisplayInEditor))
                    {
                        component.second->Display_Editor(m_szBuffer);
                    }
                }
            }
#pragma endregion

#pragma region Data
            ImGui::Text("[ Data ]");

            for (auto& field : m_EditorFieldList)
            {
                ImGui::PushItemWidth(120);

                CEditor::GetInstance()->Display_Editor(field);

                ImGui::PopItemWidth();
            }


        }
        ImGui::EndChild();

        ImGui::Columns(1);

        ImGui::End();
    }
}

void CGameObject::Set_EditorDisplayName(wstring wsName)
{
    wcscpy_s(m_szDisplayName, wsName.c_str());
    WideCharToMultiByte(CP_UTF8, 0, m_szDisplayName, -1, m_szBuffer, 256, nullptr, nullptr);
}
