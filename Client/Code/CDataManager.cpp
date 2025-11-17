#include "pch.h"
#include "CDataManager.h"

#include "CSoundManager.h"

IMPLEMENT_SINGLETON(CDataManager)

CDataManager::CDataManager()
{
}

CDataManager::~CDataManager()
{
}

HRESULT CDataManager::Ready_Data()
{
	if (FAILED(Ready_Sound())) return E_FAIL;


	return S_OK;
}

HRESULT CDataManager::Ready_Resource()
{

	return S_OK;
}

HRESULT CDataManager::Ready_Sound()
{

	Engine::CSoundManager::GetInstance()->Ready_SoundManager();

	return S_OK;
}

void CDataManager::Free()
{
	Engine::CSoundManager::DestroyInstance();
}
