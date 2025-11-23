#pragma once
#include "CBase.h"
#include "CTerrainVillage.h"
#include "CTerrainTex.h"
#include "CPrototypeManager.h"
#include "CManagement.h"
#include "CCollisionManager.h"
#include <fstream>

namespace CUtility 
{
    static void SaveMap(CTerrainVillage* pTerrain)
    {
        if (!pTerrain) return;

        Engine::CTerrainTex* pTex = pTerrain->Get_TerrainBuffer();
        if (!pTex)
            return;

        ofstream out("../Bin/Resource/MapData/mapVillage.dat", ios::binary);
        if (!out.is_open()) return;

        _ulong CntX = pTex->Get_VertexCountX();
        _ulong CntZ = pTex->Get_VertexCountZ();
        _ulong Interval = pTex->Get_VertexInterval();

        out.write((char*)&CntX, sizeof(_ulong));
        out.write((char*)&CntZ, sizeof(_ulong));
        out.write((char*)&Interval, sizeof(_ulong));

        int len = (int)pTex->Get_HeightMap().length();
        out.write((char*)&len, sizeof(int));
        out.write((char*)pTex->Get_HeightMap().c_str(), sizeof(wchar_t) * len);

        out.close();
    }

    static void LoadMap(_ulong* pCntX, _ulong* pCntZ, _ulong* pInterval, wstring& heightMap)
    {
        if (!pCntX || !pCntZ || !pInterval) return;

        ifstream fin("../Bin/Resource/MapData/mapVillage.dat", ios::binary);
        if (!fin.is_open()) return;

        fin.read((char*)pCntX, sizeof(_ulong));
        fin.read((char*)pCntZ, sizeof(_ulong));
        fin.read((char*)pInterval, sizeof(_ulong));

        int len = 0;
        fin.read((char*)&len, sizeof(int));

        heightMap.resize(len);
        fin.read((char*)heightMap.data(), sizeof(wchar_t) * len);

        fin.close();
    }
};
