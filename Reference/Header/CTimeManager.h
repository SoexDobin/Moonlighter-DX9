#pragma once

#include "CBase.h"
#include "CTimer.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CTimeManager : public CBase
{
	DECLARE_SINGLETON(CTimeManager)
private:
	explicit CTimeManager();
	virtual ~CTimeManager() override;

public:
	_float		Get_TimeDelta(const _tchar* pTimeTag);
	void		Set_TimeDelta(const _tchar* pTimeTag);

public:
	HRESULT		Ready_Timer(const _tchar* pTimerTag);
	CTimer*		Find_Timer(const _tchar* pTimerTag);
	
private:
	unordered_map<const _tchar*, CTimer*>	m_umTimer;

public:
	virtual void Free() override;
};

END