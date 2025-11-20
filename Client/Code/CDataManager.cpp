#include "pch.h"
#include "CDataManager.h"

#include "CSoundManager.h"
#include "CPrototypeManager.h"
#include "CResourceManager.h"

IMPLEMENT_SINGLETON(CDataManager)

CDataManager::CDataManager()
{
}

CDataManager::~CDataManager()
{
	Free();
}

HRESULT CDataManager::Ready_Data(LPDIRECT3DDEVICE9 pGraphicDev)
{


	{
		if (FAILED(Ready_Sound()))
			return E_FAIL;

		if (FAILED(Ready_Resource(pGraphicDev)))
			return E_FAIL;

        if (FAILED(Ready_Player_Resource(pGraphicDev)))
            return E_FAIL;

		if (FAILED(Ready_Item_Resource(pGraphicDev)))
			return E_FAIL;

        if (FAILED(Ready_UI_Resource(pGraphicDev)))
            return E_FAIL;

		if (FAILED(Ready_Prototype(pGraphicDev)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CDataManager::Ready_Prototype(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (FAILED(Engine::CPrototypeManager::GetInstance()
		->Ready_Prototype(RECTCOLOR, CRectColor::Create(pGraphicDev))))
		return E_FAIL;

	if (FAILED(Engine::CPrototypeManager::GetInstance()
		->Ready_Prototype(RECTTEX, CRectTex::Create(pGraphicDev))))
		return E_FAIL;

	if (FAILED(Engine::CPrototypeManager::GetInstance()
		->Ready_Prototype(TRANSFORM, CTransform::Create(pGraphicDev))))
		return E_FAIL;

	if (FAILED(Engine::CPrototypeManager::GetInstance()
		->Ready_Prototype(TEXTURE, CTexture::Create(pGraphicDev, m_fDefault_AnimSpeed))))
		return E_FAIL;


	return S_OK;
}

HRESULT CDataManager::Ready_Resource(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CResourceManager& Res = *CResourceManager::GetInstance();

	if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Roll",
		n_wsResSpritePath + L"Player/Will_Roll_Shop_Down_%d.png", 8)))
		return E_FAIL;

	return S_OK;
}

#pragma region PlayerTexture

HRESULT CDataManager::Ready_Player_Resource(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CResourceManager& Res = *CResourceManager::GetInstance();

    // PlayerIdle

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Idle_Down",
        n_wsResSpritePath + L"Player/Idle/Down/PLAYER_IDLE_DOWN_%02d.png", 10)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Idle_Up",
        n_wsResSpritePath + L"Player/Idle/Up/PLAYER_IDLE_UP_%02d.png", 10)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Idle_Left",
        n_wsResSpritePath + L"Player/Idle/Left/PLAYER_IDLE_LEFT_%02d.png", 10)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Idle_Right",
        n_wsResSpritePath + L"Player/Idle/Right/PLAYER_IDLE_RIGHT_%02d.png", 10)))
        return E_FAIL;

    // PlayerWalk

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Walk_Down",
        n_wsResSpritePath + L"Player/Walk/Down/PLAYER_WALK_DOWN_%02d.png", 8)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Walk_Up",
        n_wsResSpritePath + L"Player/Walk/Up/PLAYER_WALK_UP_%02d.png", 8)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Walk_Left",
        n_wsResSpritePath + L"Player/Walk/Left/PLAYER_WALK_LEFT_%02d.png", 8)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Walk_Right",
        n_wsResSpritePath + L"Player/Walk/Right/PLAYER_WALK_RIGHT_%02d.png", 8)))
        return E_FAIL;

    // PlayerRoll

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Roll_Down",
        n_wsResSpritePath + L"Player/Roll/Down/PLAYER_ROLL_DOWN_%02d.png", 8)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Roll_Up",
        n_wsResSpritePath + L"Player/Roll/Up/PLAYER_ROLL_UP_%02d.png", 8)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Roll_Left",
        n_wsResSpritePath + L"Player/Roll/Left/PLAYER_ROLL_LEFT_%02d.png", 8)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Roll_Right",
        n_wsResSpritePath + L"Player/Roll/Right/PLAYER_ROLL_RIGHT_%02d.png", 8)))
        return E_FAIL;

    return S_OK;
}

#pragma endregion

HRESULT CDataManager::Ready_Item_Resource(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CResourceManager& Res = *CResourceManager::GetInstance();

	if (FAILED(Res.Add_Sprite(pGraphicDev, L"Item_Potion", 
		n_wsResSpritePath + L"Item/Potion/Item_Potion_Heal_%d.png", 4)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDataManager::Ready_UI_Resource(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CResourceManager& Res = *CResourceManager::GetInstance();


    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Inventory_Base",
        L"../Bin/Resource/Sprite/UI/Inven/Inventory_Base.png", 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"hp_barBack",
        L"../Bin/Resource/Sprite/UI/Static/hp_barBack.png", 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"icon_bag",
        L"../Bin/Resource/Sprite/UI/Static/icon_bag.png", 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"icon_coin",
        L"../Bin/Resource/Sprite/UI/Static/icon_coin.png", 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"icon_hp",
        L"../Bin/Resource/Sprite/UI/Static/icon_hp.png", 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"icon_Pouch",
        L"../Bin/Resource/Sprite/UI/Static/icon_Pouch.png", 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"interface_key",
        L"../Bin/Resource/Sprite/UI/Static/interface_key.png", 1)))
        return E_FAIL;



    return S_OK;
}

HRESULT CDataManager::Ready_Sound()
{
	Engine::CSoundManager::GetInstance()->Ready_SoundManager();

	return S_OK;
}

void CDataManager::Free()
{
	Engine::CResourceManager::DestroyInstance();
	Engine::CSoundManager::DestroyInstance();
}
