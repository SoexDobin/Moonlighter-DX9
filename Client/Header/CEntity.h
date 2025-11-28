#pragma once
#include "CRenderObject.h"

class CStatComponent;

class CEntity : public CRenderObject
{
private :
    explicit    CEntity(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit    CEntity(const CEntity& rhs);
    virtual     ~CEntity();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float fTimeDelta);
    virtual     void        Render_GameObject();

protected :
    list<CStatComponent> 


};

