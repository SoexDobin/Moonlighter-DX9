#include "CPrototypeManager.h"

IMPLEMENT_SINGLETON(CPrototypeManager)

CPrototypeManager::CPrototypeManager()
{
}

CPrototypeManager::~CPrototypeManager()
{
    Free();
}

HRESULT CPrototypeManager::Ready_Prototype(PROTOTYPE_COMPONENT pComponentTag, CComponent* pComponent)
{
    if (CComponent* pInstance = Find_Prototype(pComponentTag))
        return E_FAIL;

    if (pComponentTag != pComponent->Get_ComponentType())
        return E_FAIL;

    m_umPrototype.emplace(pair<PROTOTYPE_COMPONENT, CComponent*>{ pComponentTag ,pComponent });

    return S_OK;
}

CComponent* CPrototypeManager::Clone_Prototype(PROTOTYPE_COMPONENT pComponentTag)
{
    CComponent* pComponent = Find_Prototype(pComponentTag);

    if (nullptr == pComponent)
        return nullptr;

    if (pComponentTag != pComponent->Get_ComponentType())
        return nullptr;

    return pComponent->Clone();
}

CComponent* CPrototypeManager::Clone_Prototype(PROTOTYPE_COMPONENT pComponentTag, CGameObject* pOwner)
{
    if (pOwner == nullptr) return nullptr;
    if (CComponent* pCom = Clone_Prototype(pComponentTag))
    {
        pCom->Set_Owner(pOwner);
        return pCom;
    }

    return nullptr;
}

CComponent* CPrototypeManager::Find_Prototype(PROTOTYPE_COMPONENT pComponentTag)
{
    auto iter = find_if(m_umPrototype.begin(), m_umPrototype.end()
        , [&pComponentTag](pair<PROTOTYPE_COMPONENT const, CComponent*>& pair) -> _bool {
            if (pair.first == pComponentTag)
                return true;

            return false;
        });

    if (iter == m_umPrototype.end()) return nullptr;

    return iter->second;
}

void CPrototypeManager::Free()
{
    for_each(m_umPrototype.begin(), m_umPrototype.end(), CDeleteMap());
    m_umPrototype.clear();
}
