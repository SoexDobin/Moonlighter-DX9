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

		if (FAILED(Ready_Item_Resource(pGraphicDev)))
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

HRESULT CDataManager::Ready_Item_Resource(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CResourceManager& Res = *CResourceManager::GetInstance();

	if (FAILED(Res.Add_Sprite(pGraphicDev, L"Item_Potion", 
		n_wsResSpritePath + L"Item/Potion/Item_Potion_Heal_%d.png", 4)))
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
