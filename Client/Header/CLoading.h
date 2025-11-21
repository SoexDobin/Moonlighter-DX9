#pragma once
#include "CBase.h"
#include "Engine_Define.h"

class CLoading :  public CBase
{
public:
	enum LOADINGID	{LOADING_STAGE, LOADING_BOSS, LOADING_END };

private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading();

public:
	CRITICAL_SECTION*		Get_Crt() { return &m_Crt; }
	LOADINGID				Get_LoadingID() { return m_eLoading; }
	_bool					Get_Finish() { return m_bFinish; }
	const wstring&          Get_String() { return m_szLoading; }

public:
	HRESULT			Ready_Loading(LOADINGID eID);
	_uint			Loading_ForState();

public:
	static unsigned int __stdcall Thread_Main(void* pArg);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	HANDLE					m_hThread;

	CRITICAL_SECTION		m_Crt;
	LOADINGID				m_eLoading;
	_bool					m_bFinish;

	_tchar					m_szLoading[128];

public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);
private:
	virtual void	Free();

};

