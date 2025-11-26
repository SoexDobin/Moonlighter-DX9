#include "CPrototypeManager.h"

IMPLEMENT_SINGLETON(CPrototypeManager)

CPrototypeManager::CPrototypeManager()
{
}

CPrototypeManager::~CPrototypeManager()
{
    Free();
}

HRESULT CPrototypeManager::Ready_Prototype(PROTOTYPE_COMPONENT eComponentTag, CComponent* pComponent)
{
    if (CComponent* pInstance = Find_Prototype(eComponentTag))
    {
        MSG_BOX("This Component is Already Exist At CPrototypeManager.cpp");
        return E_FAIL;
    }
        
    if (eComponentTag != pComponent->Get_ComponentType())
        return E_FAIL;

    m_umPrototype.emplace(pair<PROTOTYPE_COMPONENT, CComponent*>{ eComponentTag ,pComponent });

    return S_OK;
}

HRESULT CPrototypeManager::Ready_Prototype(const wstring& wsCustomComponentTag, CComponent* pComponent)
{
    if (CComponent* pInstance = Find_CustomPrototype(wsCustomComponentTag))
    {
        MSG_BOX("This Component is Already Exist At CPrototypeManager.cpp");
        return E_FAIL;
    }
        
    m_umCustomPrototype.emplace(pair<wstring, CComponent*>{ wsCustomComponentTag, pComponent });

    return S_OK;
}

CComponent* CPrototypeManager::Clone_Prototype(PROTOTYPE_COMPONENT eComponentTag)
{
    CComponent* pComponent = Find_Prototype(eComponentTag);

    if (nullptr == pComponent)
        return nullptr;

    if (eComponentTag != pComponent->Get_ComponentType())
        return nullptr;

    return pComponent->Clone();
}

CComponent* CPrototypeManager::Clone_Prototype(PROTOTYPE_COMPONENT eComponentTag, CGameObject* pOwner)
{
    if (pOwner == nullptr) return nullptr;
    if (CComponent* pCom = Clone_Prototype(eComponentTag))
    {
        pCom->Set_Owner(pOwner);
        return pCom;
    }

    return nullptr;
}

CComponent* CPrototypeManager::Find_Prototype(PROTOTYPE_COMPONENT eComponentTag)
{
    auto iter = find_if(m_umPrototype.begin(), m_umPrototype.end()
        , [&eComponentTag](pair<PROTOTYPE_COMPONENT const, CComponent*>& pair) -> _bool {
            if (pair.first == eComponentTag)
                return true;

            return false;
        });

    if (iter == m_umPrototype.end()) return nullptr;

    return iter->second;
}

CComponent* CPrototypeManager::Find_CustomPrototype(const wstring& wsComponentTag)
{
    auto iter = find_if(m_umCustomPrototype.begin(), m_umCustomPrototype.end()
        , [&wsComponentTag](pair<wstring const, CComponent*>& pair) -> _bool {
            if (pair.first == wsComponentTag)
                return true;

            return false;
        });

    if (iter == m_umCustomPrototype.end()) return nullptr;

    return iter->second;
}

void CPrototypeManager::Free()
{
    for_each(m_umPrototype.begin(), m_umPrototype.end(), CDeleteMap());
    for_each(m_umCustomPrototype.begin(), m_umCustomPrototype.end(), CDeleteMap());
    m_umPrototype.clear();
    m_umCustomPrototype.clear();
}
