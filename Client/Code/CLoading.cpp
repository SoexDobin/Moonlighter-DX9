#include "pch.h"
#include "CLoading.h"
#include "CPrototypeManager.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev), m_bFinish(false)
{
	ZeroMemory(m_szLoading, sizeof(m_szLoading));
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading()
{
}

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL,	// 보안속성(핸들의 상속 여부, NULL인 경우 상속에서 제외)
										0,		// 디폴트 스택 사이즈(1바이트)
										Thread_Main, // 쓰레드 함수(__stdcall 호출 규약)
										this,	// 쓰레드 함수를 이용하여 가공할 데이터 주소
										0,		// 쓰레드 생성 및 실행을 조절하기 flag
										NULL);	// 쓰레드 id 반환

	m_eLoading = eID;

	return S_OK;
}

_uint CLoading::Loading_ForState()
{
	lstrcpy(m_szLoading, L"Buffer LOADING..........");

	lstrcpy(m_szLoading, L"Texture LOADING..........");

	lstrcpy(m_szLoading, L"Etc LOADING..........");

	m_bFinish = true;

	lstrcpy(m_szLoading, L"LOADING Complete!!!!");

	return 0;
}

unsigned int __stdcall CLoading::Thread_Main(void* pArg)
{
	CLoading* pLoading = reinterpret_cast<CLoading*>(pArg);

	_uint iFlag(0);

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForState();
		break;

	case LOADING_BOSS:
		break;

	}

	LeaveCriticalSection(pLoading->Get_Crt());

	// _endthreadex(0);

	return iFlag;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading* pLoading = new CLoading(pGraphicDev);

	if (FAILED(pLoading->Ready_Loading(eID)))
	{
		Safe_Release(pLoading);

		MSG_BOX("Loading Create Failed");
		return nullptr;
	}

	return pLoading;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}
