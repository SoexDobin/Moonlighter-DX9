#pragma once
#include "CComponent.h"
class CEntityComponent : public CComponent
{
public :
    enum ENTITY_COM { STAT, DAMAGE, DETECTOR, C_END };
protected:
    explicit CEntityComponent(CGameObject* pOwner)
        : m_pOwner(pOwner), m_eComType(C_END) {}

    explicit CEntityComponent(const CEntityComponent& rhs)
        : CComponent(rhs), m_pOwner(rhs.m_pOwner), m_eComType(rhs.m_eComType) {}

    virtual ~CEntityComponent() {};

public :
    virtual ENTITY_COM    Get_EntityComType() { return m_eComType; }
    virtual	PROTOTYPE_COMPONENT	Get_ComponentType() { return PROTOTYPE_COMPONENT::COMPONENT_END; };

protected:
    CGameObject* m_pOwner;
    ENTITY_COM  m_eComType;
};

