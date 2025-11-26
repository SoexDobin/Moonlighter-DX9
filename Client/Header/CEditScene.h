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
    bool bDragging;
    ImGuiContext* g_MapEditor;
    CGameObject* pVillage;
    CGameObject* m_pSelectedObject;
    IDirect3DBaseTexture9* g_pPreviewTex;
    _vec3 vRot;


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
    HRESULT Add_BossWallSide_Down(const wstring pLayerTag);
    HRESULT Add_BossWallFront_Up(const wstring pLayerTag);
    HRESULT Add_BossWallSide_Up(const wstring pLayerTag);
    HRESULT Add_Pumpkin(const wstring pLayerTag);

    inline string WStringToUTF8(const std::wstring& wstr);
    template<typename TObject>
    HRESULT Add_ObjectToLayer(CEditScene* pScene, const wstring& pLayerTag, const wstring& objectName)
    {
        auto& layerMap = pScene->m_umLayer;

        auto iter = layerMap.find(pLayerTag);
        CGameObject* pGameObject = TObject::Create(pScene->m_pGraphicDevice);

        if (!pGameObject)
        {
            MSG_BOX("Create Object Fail");
            return E_FAIL;
        }

        if (iter != layerMap.end())
        {
            CLayer* pLayer = iter->second;
            if (FAILED(pLayer->Add_GameObject(objectName, pGameObject)))
            {
                MSG_BOX("Add GameObject Fail");
                return E_FAIL;
            }
        }
        else
        {
            CLayer* pGameLogicLayer = CLayer::Create();
            if (FAILED(pGameLogicLayer->Add_GameObject(objectName, pGameObject)))
            {
                MSG_BOX("Add GameObject Fail");
                return E_FAIL;
            }

            layerMap.emplace(pair<const wstring, CLayer*>{ pLayerTag, pGameLogicLayer });
        }

        return S_OK;
    }


    HRESULT Ready_Prototype();
    virtual void Free();

public:
    HRESULT Ready_Scene() override;
    _int Update_Scene(const _float fTimeDelta) override;
    void LateUpdate_Scene(const _float fTimeDelta) override;
    void Render_Scene() override;

    static CEditScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
