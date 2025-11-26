#pragma once
#include "CScene.h"
class CUITestScene : public CScene
{
private:
    explicit CUITestScene(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CUITestScene() override;

public:
    HRESULT Ready_Scene() override;
    _int    Update_Scene(const _float fTimeDelta) override;
    void    LateUpdate_Scene(const _float fTimeDelta) override;
    void    Render_Scene() override;

private:
    HRESULT Ready_Camera_Layer(const wstring& wsLayerTag);
    HRESULT Ready_Environment_Layer(const wstring& wsLayerTag);
    HRESULT Ready_UIInven_Layer(const wstring& wsLayerTag);

public:
    void    Add_Slot();

public:
    static CUITestScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    void   UI_KeyInput(const _float& fTimeDelta);
    _bool  m_bCheck;

private:
    void Free() override;
};

