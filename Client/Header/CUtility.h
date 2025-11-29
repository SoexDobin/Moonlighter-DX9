#pragma once
#include "CBase.h"
#include "CTerrainVillage.h"
#include "CHouse.h"
#include "CTreeObject.h"
#include "CTerrainTex.h"
#include "CPrototypeManager.h"
#include "CManagement.h"
#include "CCollisionManager.h"
#include "CLayerHelper.h"

#include "CTerrainBoss.h"
#include "CBossWallFront.h"
#include "CBossWallFrontUpper.h"
#include "CBossWallSide.h"
#include "CBossWallSideUpper.h"
#include "CPumpkin.h"
#include "CVineOne.h"
#include "CVineTwo.h"
#include "CMapTrigger.h"


struct ObjectData
{
    string type;
    _vec3 pos;
    _vec3 scale;
};

namespace CUtility 
{
    static void SaveVillageMap(CTerrainVillage* pTerrain, const unordered_map<_uint16, CLayer*>& layers)
    {
        ofstream out("../Bin/Resource/MapData/mapVillage.dat", ios::binary);
        if (!out.is_open() || !pTerrain)
        {
            MSG_BOX("Utility Terrain Get Fail | Utility File Open Fail");
            return;
        }

        Engine::CTerrainTex* pTex = pTerrain->Get_TerrainBuffer();
        if (!pTex)
        {
            MSG_BOX("Utility Terrain Buffer Get Fail");
            return;
        }

        _ulong CntX = pTex->Get_VertexCountX();
        _ulong CntZ = pTex->Get_VertexCountZ();
        _ulong Interval = pTex->Get_VertexInterval();
        const wstring& heightMap = pTex->Get_HeightMap();

        out.write((char*)&CntX, sizeof(_ulong));
        out.write((char*)&CntZ, sizeof(_ulong));
        out.write((char*)&Interval, sizeof(_ulong));

        int len = (int)heightMap.length();
        out.write((char*)&len, sizeof(int));
        out.write((char*)heightMap.c_str(), sizeof(wchar_t) * len);

        vector<ObjectData> saveObjs;

        for (const auto& layerPair : layers)
        {
            const auto& objMap = layerPair.second->Get_Objects();
            for (const auto& objList : objMap)
            {
                for (CGameObject* pObj : objList.second)
                {
                    if (!pObj)
                    {
                        continue;
                    }

                    auto pTrans = static_cast<CRenderObject*>(pObj)->Get_Trans();
                    if (!pTrans)
                    {
                        continue;
                    }

                    ObjectData data;
                    const TCHAR* tcharStr = pObj->m_szDisplayName;
                    int size_needed = WideCharToMultiByte(CP_UTF8, 0, tcharStr, -1, NULL, 0, NULL, NULL);
                    string str(size_needed, 0);
                    WideCharToMultiByte(CP_UTF8, 0, tcharStr, -1, &str[0], size_needed, NULL, NULL);

                    string stringName = str;
                    data.type = stringName;
                    data.pos = pTrans->Get_Pos();
                    data.scale = pTrans->Get_Scale();

                    saveObjs.push_back(data);
                }
            }
        }

        int objCount = static_cast<int>(saveObjs.size());
        out.write((char*)&objCount, sizeof(int));

        for (const auto& obj : saveObjs)
        {
            int typeLen = static_cast<int>(obj.type.size());
            out.write((char*)&typeLen, sizeof(int));
            out.write(obj.type.c_str(), typeLen);

            out.write((char*)&obj.pos, sizeof(_vec3));
            out.write((char*)&obj.scale, sizeof(_vec3));
        }

        MSG_BOX("Save Success");
        out.close();
    }

    static void LoadVillageMap(
        LPDIRECT3DDEVICE9 pDevice,
        const unordered_map<_uint16, CLayer*>& layerMap)
    {
        ifstream fin("../Bin/Resource/MapData/mapVillage.dat", ios::binary);
        if (!fin.is_open())
        {
            MSG_BOX("mapVillage File Open Fail");
            return;
        }

        _ulong CntX, CntZ, Interval;
        int len = 0;

        fin.read((char*)&CntX, sizeof(_ulong));
        fin.read((char*)&CntZ, sizeof(_ulong));
        fin.read((char*)&Interval, sizeof(_ulong));
        fin.read((char*)&len, sizeof(int));

        wstring heightMap;
        heightMap.resize(len);
        fin.read((char*)heightMap.data(), sizeof(wchar_t) * len);

        auto iter = layerMap.find(CLayerHelper::GetInstance()->GetLayerIDByName(L"Environment_Layer"));
        CLayer* pEnvLayer = nullptr;
        if (iter != layerMap.end())
        {
            pEnvLayer = iter->second;
        }
        else
        {
            pEnvLayer = CLayer::Create();
        }

        int objCount = 0;
        fin.read((char*)&objCount, sizeof(int));
        for (int i = 0; i < objCount; ++i)
        {
            ObjectData data;
            int typeLen;
            fin.read((char*)&typeLen, sizeof(int));

            data.type.resize(typeLen);
            fin.read((char*)data.type.data(), typeLen);

            fin.read((char*)&data.pos, sizeof(_vec3));
            fin.read((char*)&data.scale, sizeof(_vec3));

            CGameObject* pObj = nullptr;

            if (!strcmp(data.type.c_str(), "House"))
            {
                pObj = CHouse::Create(pDevice);
            }
            else if (!strcmp(data.type.c_str(), "Tree"))
            {
                pObj = CTreeObject::Create(pDevice);
            }
            else if (!strcmp(data.type.c_str(), "Terrain"))
            {
                pObj = CTerrainVillage::Create(pDevice);
            }
            else
            {
                MSG_BOX("Object Not Found");
                continue;
            }

            if (!pObj)
            {
                MSG_BOX("Object Null");
                continue;
            }

            CTransform* pTrans = static_cast<CRenderObject*>(pObj)->Get_Trans();
            pTrans->Set_Pos(data.pos.x, data.pos.y, data.pos.z);
            pTrans->Set_Scale(data.scale.x, data.scale.y, data.scale.z);

            wstring wstringData = L"";
            wstringData.assign(data.type.begin(), data.type.end());

            pEnvLayer->Add_GameObject(wstringData, pObj);
        }
        //layerMap.emplace(pair<const wstring, CLayer*>{ L"Environment_Layer", pEnvLayer});
        fin.close();
    }

    // Boss Save
    static void SaveBossMap(CTerrainBoss* pTerrain, const unordered_map<_uint16, CLayer*>& layers)
    {
        ofstream out("../Bin/Resource/MapData/mapBoss2.dat", ios::binary);
        if (!out.is_open() || !pTerrain)
        {
            MSG_BOX("Utility Terrain Get Fail | Utility File Open Fail");
            return;
        }

        Engine::CTerrainBossTex* pTex = pTerrain->Get_TerrainBuffer();
        if (!pTex)
        {
            MSG_BOX("Utility Terrain Buffer Get Fail");
            return;
        }

        _ulong CntX = pTex->Get_VertexCountX();
        _ulong CntZ = pTex->Get_VertexCountZ();
        _ulong Interval = pTex->Get_VertexInterval();
        const wstring& heightMap = pTex->Get_HeightMap();

        out.write((char*)&CntX, sizeof(_ulong));
        out.write((char*)&CntZ, sizeof(_ulong));
        out.write((char*)&Interval, sizeof(_ulong));

        int len = (int)heightMap.length();
        out.write((char*)&len, sizeof(int));
        out.write((char*)heightMap.c_str(), sizeof(wchar_t) * len);

        vector<ObjectData> saveObjs;

        for (const auto& layerPair : layers)
        {
            const auto& objMap = layerPair.second->Get_Objects();
            for (const auto& objList : objMap)
            {
                for (CGameObject* pObj : objList.second)
                {
                    if (!pObj)
                    {
                        continue;
                    }

                    auto pTrans = static_cast<CRenderObject*>(pObj)->Get_Trans();
                    if (!pTrans)
                    {
                        continue;
                    }

                    ObjectData data;
                    const TCHAR* tcharStr = pObj->m_szDisplayName;
                    int size_needed = WideCharToMultiByte(CP_UTF8, 0, tcharStr, -1, NULL, 0, NULL, NULL);
                    string str(size_needed, 0);
                    WideCharToMultiByte(CP_UTF8, 0, tcharStr, -1, &str[0], size_needed, NULL, NULL);

                    string stringName = str;

                    if (strcmp(str.c_str(), "DynamicCam") == 0)
                    {
                        continue;
                    }

                    data.type = stringName;
                    data.pos = pTrans->Get_Pos();
                    data.scale = pTrans->Get_Scale();

                    saveObjs.push_back(data);
                }
            }
        }

        int objCount = static_cast<int>(saveObjs.size());
        out.write((char*)&objCount, sizeof(int));

        for (const auto& obj : saveObjs)
        {
            int typeLen = static_cast<int>(obj.type.size());
            out.write((char*)&typeLen, sizeof(int));
            out.write(obj.type.c_str(), typeLen);

            out.write((char*)&obj.pos, sizeof(_vec3));
            out.write((char*)&obj.scale, sizeof(_vec3));
        }

        MSG_BOX("Save Success");
        out.close();
    }

    static void LoadBossMap(
        LPDIRECT3DDEVICE9 pDevice,
        const unordered_map<_uint16, CLayer*>& layerMap)
    {
        ifstream fin("../Bin/Resource/MapData/mapBoss2.dat", ios::binary);
        if (!fin.is_open())
        {
            MSG_BOX("mapBoss File Open Fail");
            return;
        }

        _ulong CntX, CntZ, Interval;
        int len = 0;

        fin.read((char*)&CntX, sizeof(_ulong));
        fin.read((char*)&CntZ, sizeof(_ulong));
        fin.read((char*)&Interval, sizeof(_ulong));
        fin.read((char*)&len, sizeof(int));

        wstring heightMap;
        heightMap.resize(len);
        fin.read((char*)heightMap.data(), sizeof(wchar_t) * len);

        auto iter = layerMap.find(CLayerHelper::GetInstance()->GetLayerIDByName(L"Environment_Layer"));
        auto iter2 = layerMap.find(CLayerHelper::GetInstance()->GetLayerIDByName(L"Environment_NoCollision_Layer"));
        CLayer* pEnvLayer = nullptr;
        CLayer* pNonCollisionLayer = nullptr;
        if (iter != layerMap.end())
        {
            pEnvLayer = iter->second;
        }
        else
        {
            pEnvLayer = CLayer::Create();
        }

        if (iter2 != layerMap.end())
        {
            pNonCollisionLayer = iter2->second;
        }
        else
        {
            pNonCollisionLayer = CLayer::Create();
        }

        int objCount = 0;
        fin.read((char*)&objCount, sizeof(int));
        for (int i = 0; i < objCount; ++i)
        {
            ObjectData data;
            int typeLen;
            fin.read((char*)&typeLen, sizeof(int));

            data.type.resize(typeLen - 1);
            fin.read((char*)data.type.data(), typeLen);

            fin.read((char*)&data.pos, sizeof(_vec3));
            fin.read((char*)&data.scale, sizeof(_vec3));

            CGameObject* pObj = nullptr;

            if (data.type == "Terrain")
            {
                pObj = CTerrainBoss::Create(pDevice);
            }
            else if (data.type == "Boss_Wall_Front")
            {
                pObj = CBossWallFront::Create(pDevice);
            }
            else if (data.type == "Boss_Wall_Front_Up")
            {
                pObj = CBossWallFrontUpper::Create(pDevice);
            }
            else if (data.type == "Boss_Wall_Side")
            {
                pObj = CBossWallSide::Create(pDevice);
            }
            else if (data.type == "Boss_Wall_Side_Up")
            {
                pObj = CBossWallSideUpper::Create(pDevice);
            }
            else if (data.type == "Pumpkin")
            {
                pObj = CPumpkin::Create(pDevice);
            }
            else if (data.type == "VineOne")
            {
                pObj = CVineOne::Create(pDevice);
            }
            else if (data.type == "VineTwo")
            {
                pObj = CVineOne::Create(pDevice);
            }
            else if (data.type == "Map_Trigger")
            {
                pObj = CMapTrigger::Create(pDevice);
            }
            else
            {
                MSG_BOX("Object Not Found");
                continue;
            }

            if (!pObj)
            {
                MSG_BOX("Object Null");
                continue;
            }

            CTransform* pTrans = static_cast<CRenderObject*>(pObj)->Get_Trans();
            pTrans->Set_Pos(data.pos.x, data.pos.y, data.pos.z);
            pTrans->Set_Scale(data.scale.x, data.scale.y, data.scale.z);

            wstring wstringData = L"";
            wstringData.assign(data.type.begin(), data.type.end());

            if ((strcmp(data.type.c_str(), "Terrain")))
            {
                pEnvLayer->Add_GameObject(wstringData, pObj);
            }
            else
            {
                pNonCollisionLayer->Add_GameObject(wstringData, pObj);
            }
        }
        //layerMap.emplace(pair<const wstring, CLayer*>{ L"Environment_Layer", pEnvLayer});
        fin.close();
    }

    template<typename T>
    static _vec3 Picking_Terrain(LPDIRECT3DDEVICE9 m_pGraphicDevice, HWND hWnd, T* pObj, _uint iVtX, _uint iVtZ, _uint iItv = 1)
    {
        if (pObj == nullptr)
        {
            return _vec3(0.f, 0.f, 0.f);
        }

        POINT ptMouse{};
        GetCursorPos(&ptMouse);
        ScreenToClient(hWnd, &ptMouse);

        _vec3 vMousePos;

        D3DVIEWPORT9 ViewPort;
        ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

        m_pGraphicDevice->GetViewport(&ViewPort);

        vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
        vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
        vMousePos.z = 0.f;

        D3DXMATRIX matProj;
        m_pGraphicDevice->GetTransform(D3DTS_PROJECTION, &matProj);
        D3DXMatrixInverse(&matProj, 0, &matProj);
        D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

        D3DXMATRIX matView;
        m_pGraphicDevice->GetTransform(D3DTS_VIEW, &matView);
        D3DXMatrixInverse(&matView, 0, &matView);

        _vec3 vRayPos{ 0.f, 0.f, 0.f };
        _vec3 vRayDir = vMousePos - vRayPos;

        D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
        D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

        D3DXMATRIX matWorld = (*pObj->Get_Trans()->Get_World());
        D3DXMatrixInverse(&matWorld, 0, &matWorld);

        D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
        D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

        _ulong dwVtxNumber[3]{};
        const _vec3* pTerrainVtxPos = pObj->Get_TerrainBuffer()->Get_VtxPos();
        _float fU = 0.f, fV = 0.f, fDist = 0.f;


        for (_ulong i = 0; i < iVtZ - 1; ++i)
        {
            for (_ulong j = 0; j < iVtX - 1; ++j)
            {
                _ulong dwIndex = i * iVtX + j;

                dwVtxNumber[0] = dwIndex + iVtX;
                dwVtxNumber[1] = dwIndex + iVtX + 1;
                dwVtxNumber[2] = dwIndex + 1;

                if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxNumber[1]],
                    &pTerrainVtxPos[dwVtxNumber[0]],
                    &pTerrainVtxPos[dwVtxNumber[2]],
                    &vRayPos, &vRayDir, &fU, &fV, &fDist))
                {
                    _vec3 vResult = _vec3(
                        pTerrainVtxPos[dwVtxNumber[1]].x + fU * (pTerrainVtxPos[dwVtxNumber[0]].x - pTerrainVtxPos[dwVtxNumber[1]].x),
                        0.f,
                        pTerrainVtxPos[dwVtxNumber[1]].z + fV * (pTerrainVtxPos[dwVtxNumber[2]].z - pTerrainVtxPos[dwVtxNumber[0]].z)
                    );
                    D3DXVec3TransformCoord(&vResult, &vResult, pObj->Get_Trans()->Get_World());
                    return vResult;
                }

                dwVtxNumber[0] = dwIndex + iVtX;
                dwVtxNumber[1] = dwIndex + 1;
                dwVtxNumber[2] = dwIndex;

                if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxNumber[2]],
                    &pTerrainVtxPos[dwVtxNumber[1]],
                    &pTerrainVtxPos[dwVtxNumber[0]],
                    &vRayPos, &vRayDir, &fU, &fV, &fDist))
                {
                    _vec3 vResult = _vec3(pTerrainVtxPos[dwVtxNumber[2]].x + fU * (pTerrainVtxPos[dwVtxNumber[1]].x - pTerrainVtxPos[dwVtxNumber[2]].x),
                        0.f,
                        pTerrainVtxPos[dwVtxNumber[2]].z + fV * (pTerrainVtxPos[dwVtxNumber[0]].z - pTerrainVtxPos[dwVtxNumber[2]].z));
                    D3DXVec3TransformCoord(&vResult, &vResult, pObj->Get_Trans()->Get_World());
                    return vResult;
                }
            }
        }
        return _vec3(0.f, 0.f, 0.f);
    }
};
