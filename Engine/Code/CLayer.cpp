#include "CLayer.h"

CLayer::CLayer()
	: m_bDisplayInEditor(false)
{
}

CLayer::~CLayer()
{

}

CComponent* CLayer::Get_Component(COMPONENTID eID, const wstring wsObjTag, const wstring pComponentTag)
{
	auto iter = find_if(m_umGameObject.begin(), m_umGameObject.end()
		, [&wsObjTag](const pair<const wstring, CGameObject*>& pair) -> _bool {
			if (pair.first == wsObjTag)
				return true;

			return false;
		});

	if (iter == m_umGameObject.end()) return nullptr;

	return iter->second->Get_Component(eID, pComponentTag);
}

HRESULT CLayer::Add_GameObject(const wstring wsObjTag, CGameObject* pGameObject)
{
	if (nullptr == pGameObject) return E_FAIL;

	m_umGameObject.emplace(pair<wstring, CGameObject*>{ wsObjTag, pGameObject });

    return S_OK;
}

HRESULT CLayer::Ready_Layer()
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float fTimeDelta)
{
	_int iResult(0);
	auto iter = find_if(m_umGameObject.begin(), m_umGameObject.end()
		, [=, &iResult](pair<const wstring, CGameObject*>& pair) -> _bool {
			iResult = pair.second->Update_GameObject(fTimeDelta);

			if (iResult & 0x80000000) return true;
			return false;
		});

	if (iter != m_umGameObject.end()) return iResult;

    return iResult;
}

void CLayer::LateUpdate_Layer(const _float fTimeDelta)
{
	for_each(m_umGameObject.begin(), m_umGameObject.end()
		, [fTimeDelta](pair<const wstring, CGameObject*>& pair) -> void {
			pair.second->LateUpdate_GameObject(fTimeDelta);
		});
}

void CLayer::Render_Layer()
{
	for_each(m_umGameObject.begin(), m_umGameObject.end()
		, [](pair<const wstring, CGameObject*>& pair) -> void {
			pair.second->Render_GameObject();
		});
}

CLayer* CLayer::Create(wstring layerTag)
{
	CLayer* pLayer = new CLayer;

	if (FAILED(pLayer->Ready_Layer()))
	{
		MSG_BOX("Layer Create Failed");
		Safe_Release(pLayer);
		return nullptr;
	}

	pLayer->m_LayerTag = layerTag;
	return pLayer;
}

void CLayer::Free()
{
	for_each(m_umGameObject.begin(), m_umGameObject.end(), CDeleteMap());
	m_umGameObject.clear();
}

void CLayer::Display_Editor()
{
	if (!m_bDisplayInEditor)
		return;

	ImGui::Begin("Layer");

	for (auto& gameObject : m_umGameObject)
	{
		ImGui::Checkbox(("##" + to_string((uintptr_t)gameObject.second)).c_str(), &gameObject.second->m_bDisplayInEditor); ImGui::SameLine();
		ImGui::Text("%ls", gameObject.second->m_szDisplayName);

		gameObject.second->Display_Editor();
	}

	ImGui::End();
}