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
	HRESULT		Ready_Item_Resource(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT		Ready_Sound();

private:
	const wstring n_wsResSpritePath = L"../Bin/Resource/Sprite/";

	const _float m_fDefault_AnimSpeed = 10.f;

private:
	void		Free() override;
};

