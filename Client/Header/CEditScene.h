#pragma once
#include "CScene.h"
#include "CTerrainTex.h"
#include "CTerrainVillage.h"


class CEditScene : public CScene
{
private:
    explicit CEditScene(LPDIRECT3DDEVICE9 pGraphciDev);
    virtual ~CEditScene();

    _vec4 clear_color = { 0.f, 0.f, 0.f, 0.f };
    bool show_demo_window = true;
    bool show_another_window = true;

    ImGuiContext* g_MapEditor;
    CGameObject* pVillage;


    HRESULT Ready_Environment_Layer(const wstring pLayerTag);
    HRESULT Ready_GameLogic_Layer(const wstring pLayerTag);
    HRESULT Ready_UI_Layer(const wstring pLayerTag);
    HRESULT	Ready_Camera_Layer(const wstring wsLayerTag);

    HRESULT Add_TerrainVillage(const wstring temp);

    HRESULT Ready_Prototype();
    virtual void Free();

public:
    HRESULT Ready_Scene() override;
    _int Update_Scene(const _float fTimeDelta) override;
    void LateUpdate_Scene(const _float fTimeDelta) override;
    void Render_Scene() override;

    static CEditScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
