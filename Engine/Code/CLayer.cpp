#include "CLayer.h"

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

    pGameObject->Set_EditorDisplayName(wsObjTag);

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

            for (auto iterObj = pair.second.begin(); iterObj != pair.second.end(); )
            {
                iResult = (*iterObj)->Update_GameObject(fTimeDelta);

                if (iResult)
                {
                    CGameObject* pObj = *iterObj;
                    iterObj = pair.second.erase(iterObj);
                    Safe_Release(pObj);
                    continue;
                }
                ++iterObj;
            }

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
    pLayer->m_wsLayerTag = layerTag;

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
}

wstring CLayer::ws_selectedLayer = L"";

void CLayer::Display_Editor()
{



    bool bIsSelectedLayer = (ws_selectedLayer == m_wsLayerTag);

    for (auto& gameObjectList : m_umGameObject)
    {
        for (auto& gameObject : gameObjectList.second)
        {
            // 선택된 레이어가 아니면 모든 오브젝트 끄기
            if (!bIsSelectedLayer)
                gameObject->m_bDisplayInEditor = false;

            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            if (gameObject->m_bDisplayInEditor)
                flags |= ImGuiTreeNodeFlags_Selected;

            ImGui::TreeNodeEx((void*)(intptr_t)gameObject, flags, "%ls", gameObject->m_szDisplayName);

            if (ImGui::IsItemClicked())
            {
                // 현재 레이어 내부 오브젝트만 선택
                for (auto& listPair : m_umGameObject)
                    for (auto& obj : listPair.second)
                        obj->m_bDisplayInEditor = false;

                ws_selectedLayer = m_wsLayerTag;  
                gameObject->m_bDisplayInEditor = true;
            }

            if (gameObject->m_bDisplayInEditor)
            {
                ImGui::Separator();
                gameObject->Display_Editor();
            }
        }
    }
}
