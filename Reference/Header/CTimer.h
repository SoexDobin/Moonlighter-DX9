#pragma once

#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CTimer : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer() override;

public:
	_float		GetDeltaTime() { return m_fDeltaTime; }

public:
	HRESULT		Ready_Timer();
	void		Update_Timer();

private:
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_LastTime;
	LARGE_INTEGER		m_CpuTick;

	_float				m_fDeltaTime;
	
public:
	static CTimer* Create();

private:
	void	Free()	override;
};

END

