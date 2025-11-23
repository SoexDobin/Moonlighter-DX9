#pragma once
#include "CScene.h"
#include "CTerrainTex.h"
#include "CTerrainVillage.h"


class CEditScene : public CScene
{
private:
    explicit CEditScene(LPDIRECT3DDEVICE9 pGraphciDev);
    virtual ~CEditScene();

    ImGuiContext* g_MapEditor;
    CGameObject* pVillage;
    CGameObject* m_pSelectedObject = nullptr;


    HRESULT Ready_Environment_Layer(const wstring pLayerTag);
    HRESULT Ready_GameLogic_Layer(const wstring pLayerTag);
    HRESULT Ready_UI_Layer(const wstring pLayerTag);
    HRESULT	Ready_Camera_Layer(const wstring wsLayerTag);

    HRESULT Add_TerrainVillage(const wstring pLayerTag);
    HRESULT Add_House(const wstring pLayerTag);
    HRESULT Add_Tree(const wstring pLayerTag);

    inline string WStringToUTF8(const std::wstring& wstr);

    HRESULT Ready_Prototype();
    virtual void Free();

public:
    HRESULT Ready_Scene() override;
    _int Update_Scene(const _float fTimeDelta) override;
    void LateUpdate_Scene(const _float fTimeDelta) override;
    void Render_Scene() override;

    static CEditScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
