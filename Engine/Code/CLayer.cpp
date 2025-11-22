#include "CLayer.h"
#include "CCollisionManager.h"

CLayer::CLayer()
	: m_bDisplayInEditor(true)
{
}

CLayer::~CLayer()
{

}

CComponent* CLayer::Get_Component(COMPONENTID eID, const wstring& wsObjTag, const wstring& pComponentTag)
{
    CGameObject* pObj = nullptr;
	for_each(m_umGameObject.begin(), m_umGameObject.end()
		, [&wsObjTag, &pObj](pair<const wstring, list<CGameObject*>>& pair) -> void {
            if (pair.first == wsObjTag && !pair.second.empty())
            {
                pObj = pair.second.front();
                return;
            }
		});

    if (pObj != nullptr)
        return pObj->Get_Component(eID, pComponentTag);

    return nullptr;
}

HRESULT CLayer::Add_GameObject(const wstring& wsObjTag, CGameObject* pGameObject)
{
	if (nullptr == pGameObject) return E_FAIL;

    m_umGameObject[wsObjTag].push_back(pGameObject);	
    if (CComponent* pCol = pGameObject->Get_Component(ID_DYNAMIC, COLLIDER))
        CCollisionManager::GetInstance()->Add_Collider(m_wsLayerName, static_cast<CCollider*>(pCol));

    return S_OK;
}

HRESULT CLayer::Ready_Layer(const wstring& wsLayerName)
{
    m_wsLayerName = wsLayerName;
    
	return S_OK;
}

_int CLayer::Update_Layer(const _float fTimeDelta)
{
	_int iResult(0);
	auto iter = find_if(m_umGameObject.begin(), m_umGameObject.end()
		, [=, &iResult](pair<const std::wstring, list<CGameObject*>>& pair) -> _bool {

            for_each(pair.second.begin(), pair.second.end(),
                [=, &iResult](CGameObject* pObj) -> void {
                    iResult =pObj->Update_GameObject(fTimeDelta);
                });

			if (iResult & 0x80000000) return true;

			return false;
		});

	if (iter != m_umGameObject.end()) return iResult;

    return iResult;
}

void CLayer::LateUpdate_Layer(const _float fTimeDelta)
{
	for_each(m_umGameObject.begin(), m_umGameObject.end()
		, [fTimeDelta](pair<const std::wstring, list<CGameObject*>>& pair) -> void {

            for_each(pair.second.begin(), pair.second.end(),
                [fTimeDelta](CGameObject* pObj) -> void {
                    pObj->LateUpdate_GameObject(fTimeDelta);
                });

		});
}

void CLayer::Render_Layer()
{
	for_each(m_umGameObject.begin(), m_umGameObject.end()
		, [](pair<const std::wstring, list<CGameObject*>>& pair) -> void {

            for_each(pair.second.begin(), pair.second.end(),
                [](CGameObject* pObj) -> void {
                    pObj->Render_GameObject();
                });

		});
    CCollisionManager::GetInstance()->Update_Collision(m_wsLayerName);
}

CLayer* CLayer::Create(const wstring& layerTag)
{
	CLayer* pLayer = new CLayer;

	if (FAILED(pLayer->Ready_Layer(layerTag)))
	{
		MSG_BOX("Layer Create Failed");
		Safe_Release(pLayer);
		return nullptr;
	}
    
    int len = WideCharToMultiByte(CP_UTF8, 0, layerTag.c_str(), -1, nullptr, 0, nullptr, nullptr);
    WideCharToMultiByte(CP_UTF8, 0, layerTag.c_str(), -1, pLayer->m_LayerTag, len, nullptr, nullptr);

    return pLayer;
}

void CLayer::Free()
{
    for_each(m_umGameObject.begin(), m_umGameObject.end(),
        [](pair<const std::wstring, list<CGameObject*>>& pair) -> void {
            for (auto& pObj : pair.second)
                Safe_Release(pObj);
        });
	m_umGameObject.clear();

    CCollisionManager::GetInstance()->Release_Collider();
}

void CLayer::Display_Editor()
{
	if (!m_bDisplayInEditor)
		return;

    for (auto& gameObjectList : m_umGameObject)
        for (auto& gameObject : gameObjectList.second)
        {
            ImGui::Checkbox(("##" + to_string((uintptr_t)gameObject)).c_str(), &gameObject->m_bDisplayInEditor); ImGui::SameLine();
            ImGui::Text("%ls", gameObject->m_szDisplayName);

            gameObject->Display_Editor();
        }
}

