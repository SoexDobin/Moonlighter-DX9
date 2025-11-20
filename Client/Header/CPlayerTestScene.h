#pragma once
#include "CScene.h"

class CPlayerTestScene final : public CScene
{
private:
    explicit CPlayerTestScene(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CPlayerTestScene() override;

public:
    HRESULT Ready_Scene() override;
    _int    Update_Scene(const _float fTimeDelta) override;
    void    LateUpdate_Scene(const _float fTimeDelta) override;
    void    Render_Scene() override;

private:
    HRESULT Ready_Camera_Layer(const wstring& wsLayerTag);
    HRESULT Ready_GameLogic_Layer(const wstring& wsLayerTag);

public:
    static CPlayerTestScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    void Free() override;
};

