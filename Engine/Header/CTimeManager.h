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
	_float		Get_TimeDelta(const wstring wsTimeTag);
	void		Set_TimeDelta(const wstring wsTimeTag);

public:
	HRESULT		Ready_Timer(const wstring wsTimerTag);
	CTimer*		Find_Timer(const wstring wsTimerTag);
	
private:
	unordered_map<wstring, CTimer*>	m_umTimer;

public:
	virtual void Free() override;
};

END