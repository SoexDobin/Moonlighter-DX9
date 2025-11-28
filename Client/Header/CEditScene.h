#pragma once
#include "CScene.h"
#include "CTerrainVillage.h"
#include "CTerrainDungeonNormal.h"
#include "CTerrainBoss.h"
#include "Client_Define.h"
#include "CLayerHelper.h"

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
    bool isReadyPick;

    ImGuiContext* g_MapEditor;
    CGameObject* pVillage;
    CGameObject* m_pSelectedObject;
    CGameObject* pDragObject;
    IDirect3DBaseTexture9* g_pPreviewTex;

    _vec3 vRot;


    HRESULT Ready_Environment_Layer(const wstring pLayerTag);
    HRESULT Ready_GameLogic_Layer(const wstring pLayerTag);
    HRESULT Ready_UI_Layer(const wstring pLayerTag);
    HRESULT	Ready_Camera_Layer(const wstring wsLayerTag);

    void InitPreviewTextures(const wstring wPreview);
    CGameObject* PickObject(const _vec3& vPickPos);
    _vec3 PickingArea();
    const char* toStringScene(SCENETYPE scene);

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
    HRESULT Add_VineOne(const wstring pLayerTag);
    HRESULT Add_VineTwo(const wstring pLayerTag);
    HRESULT Add_MapTrigger(const wstring pLayerTag);

    inline string WStringToUTF8(const std::wstring& wstr);

    template<typename TObject>
    HRESULT Add_ObjectToLayer(CEditScene* pScene, const wstring& pLayerTag, const wstring& objectName)
    {
        auto& layerMap = pScene->Get_Layers();

        auto iter = layerMap.find(Engine::CLayerHelper::GetInstance()->GetLayerIDByName(pLayerTag));
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
            if (FAILED(pScene->Get_Layer(L"GameLogic_Layer")->Add_GameObject(objectName, pGameObject)))
            {
                MSG_BOX("Add GameObject Fail");
                return E_FAIL;
            }
        }

        return S_OK;
    }

    template<typename T>
    void find_Terrain()
    {
        auto iter = Get_Layers().find(Engine::CLayerHelper::GetInstance()->GetLayerIDByName(L"Environment_NoCollision_Layer"));
        if (iter != Get_Layers().end())
        {
            CLayer* pLayer = iter->second;
            const auto& objMap = pLayer->Get_Objects();
            for (const auto& objList : objMap)
            {
                for (CGameObject* pObj : objList.second)
                {
                    if (wcscmp(pObj->m_szDisplayName, L"Terrain") == 0)
                    {
                        pVillage = static_cast<T*>(pObj);
                        break;
                    }
                }
            }
        }
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
