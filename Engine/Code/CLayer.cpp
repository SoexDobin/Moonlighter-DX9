#include "CLayer.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{

}

CComponent* CLayer::Get_Component(COMPONENTID eID, const _tchar* pObjTag, const _tchar* pComponentTag)
{
	auto iter = find_if(m_umGameObject.begin(), m_umGameObject.end()
		, [&pObjTag](const pair<const _tchar* const, CGameObject*>& pair) -> _bool {
			if (0 == lstrcmpW(pair.first, pObjTag))
				return true;

			return false;
		});

	if (iter == m_umGameObject.end()) return nullptr;

	return iter->second->Get_Component(eID, pComponentTag);
}

HRESULT CLayer::Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject)
{
	if (nullptr == pGameObject) return E_FAIL;

	m_umGameObject.emplace(pair<const _tchar* const, CGameObject*>{ pObjTag, pGameObject });

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
		, [=, &iResult](pair<const _tchar* const, CGameObject*>& pair) -> _bool {
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
		, [fTimeDelta](pair<const _tchar* const, CGameObject*>& pair) -> void {
			pair.second->LateUpdate_GameObject(fTimeDelta);
		});
}

void CLayer::Render_Layer()
{
	for_each(m_umGameObject.begin(), m_umGameObject.end()
		, [](pair<const _tchar* const, CGameObject*>& pair) -> void {
			pair.second->Render_GameObject();
		});
}

CLayer* CLayer::Create()
{
	CLayer* pLayer = new CLayer;

	if (FAILED(pLayer->Ready_Layer()))
	{
		MSG_BOX("Layer Create Failed");
		Safe_Release(pLayer);
		return nullptr;
	}

	return pLayer;
}

void CLayer::Free()
{
	for_each(m_umGameObject.begin(), m_umGameObject.end(), CDeleteMap());
	m_umGameObject.clear();
}