#pragma once

#include "CBase.h"
#include "Engine_Define.h"

class CDataManager : public CBase
{
	DECLARE_SINGLETON(CDataManager)
private:
	explicit CDataManager();
	virtual ~CDataManager() override;

public:
	// MainApp 시작시 호출 되는 함수
	HRESULT		Ready_Data(LPDIRECT3DDEVICE9 pGraphicDev);

private:

	// 각 씬에 맞는 함수로 제작하여 Ready_Data()에서 최종 호출
	// ex. Ready_Dungeon_Resource()
	HRESULT		Ready_Prototype(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT		Ready_Resource(LPDIRECT3DDEVICE9 pGraphicDev);
    HRESULT		Ready_Player_Resource(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT		Ready_Item_Resource(LPDIRECT3DDEVICE9 pGraphicDev);
    HRESULT     Ready_Boss_Resource(LPDIRECT3DDEVICE9 pGraphicDev);
    HRESULT     Ready_Monster_Resource(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT		Ready_UI_Resource(LPDIRECT3DDEVICE9 pGraphicDev);
    HRESULT     Ready_Map_Resource(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT		Ready_Sound();

private:
	const wstring& n_wsResSpritePath = L"../Bin/Resource/Sprite/";

	const _float n_fDefault_AnimSpeed = 10.f;


    const _ulong dwTerrainBossX = 61;
    const _ulong dwTerrainBossY = 37;
    const _ulong dwTerrainBossItv = 1;
private:
	void		Free() override;
};

