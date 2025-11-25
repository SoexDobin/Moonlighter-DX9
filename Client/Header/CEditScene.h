#pragma once
#include "CScene.h"
#include "CTerrainVillage.h"
#include "CTerrainDungeonNormal.h"
#include "CTerrainBoss.h"


class CEditScene : public CScene
{
private:

    enum CONFIRM {
        VILL_SAVE,
        VILL_LOAD,
        DUN_SAVE,
        DUN_LOAD,
        BOSS_SAVE,
        BOSS_LOAD,
        CONFIRM_END
    };

    explicit CEditScene(LPDIRECT3DDEVICE9 pGraphciDev);
    virtual ~CEditScene();

    bool confirm[CONFIRM_END];
    ImGuiContext* g_MapEditor;
    CGameObject* pVillage;
    CGameObject* m_pSelectedObject = nullptr;
    IDirect3DBaseTexture9* g_pPreviewTex;
    _vec3 vRot = { 0.f, 0.f, 0.f };


    HRESULT Ready_Environment_Layer(const wstring pLayerTag);
    HRESULT Ready_GameLogic_Layer(const wstring pLayerTag);
    HRESULT Ready_UI_Layer(const wstring pLayerTag);
    HRESULT	Ready_Camera_Layer(const wstring wsLayerTag);

    void InitPreviewTextures(const wstring wPreview);
    void find_VillageTerrain();

    HRESULT Add_TerrainVillage(const wstring pLayerTag);
    HRESULT Add_House(const wstring pLayerTag);
    HRESULT Add_Tree(const wstring pLayerTag);

    HRESULT Add_TerrainDungeon(const wstring pLayerTag);
    HRESULT Add_DungeonWall(const wstring pLayerTag);

    HRESULT Add_TerrainBoss(const wstring pLayerTag);
    HRESULT Add_BossWallFront_Down(const wstring pLayerTag);

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
