#pragma once

#include "CRenderObject.h"

namespace Engine
{
    class CRectCollider;
}

class CMapTrigger : public CRenderObject
{
private:
    explicit CMapTrigger(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CMapTrigger(const CMapTrigger& rhs);
    virtual ~CMapTrigger() override;


    CRectCollider* m_pRectCollider;
    SCENETYPE eScene;

    void Free() override;

public:
    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float fTimeDelta) override;
    void LateUpdate_GameObject(const _float fTimeDelta) override;
    void Render_GameObject() override;

    const SCENETYPE GetTriggerScene() const { return eScene; }
    void SetTriggerScene(const SCENETYPE eScene) { this->eScene = eScene; }

    static CMapTrigger* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

