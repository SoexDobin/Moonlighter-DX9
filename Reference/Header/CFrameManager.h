#pragma once

#include "CBase.h"
#include "CFrame.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CFrameManager : public CBase
{
	DECLARE_SINGLETON(CFrameManager)

private:
	explicit CFrameManager();
	virtual ~CFrameManager() override;

public:
	_bool		IsPermit_Call(const wstring wsFrameTag, const _float fTimeDelta);
	HRESULT		Ready_Frame(const wstring wsFrameTag, const _float fCallLimit);

	_bool		IsTransit_NextFrame(_float fTimeDelta);
	void			Transit_NextFrame();

	void			Pause_Game();
	void			Restart_Game();

public :
	void			Modify_LastTimeDelta(_float* pTimeDelta);
	void			Set_MainFrame(const wstring wsFrameTag);
	void			Set_LastTimeDelta(const _float& fTimeDelta) { m_fLastTimeDelta = fTimeDelta; }
	_uint		Get_CurFPS() { return m_iLastFPS; }

private :
	CFrame*		Find_Frame(const wstring wsFrameTag);
	void				Compute_FPS();

private :
	CFrame*		m_mainFrame;
	wstring		m_wsMainFrameTag;

	_bool			m_bGamePaused;
	_bool			m_bStepFrame;
	_bool			m_bNextStep;
	_bool			m_bTimeDeltaModified;

	_float			m_fLastTimeDelta;

	_float			m_fComputeDelta;
	_uint			m_iComputeFPS, m_iLastFPS;

private:
	unordered_map<wstring, CFrame*>	m_umFrame;

private:
	void	Free() override;
};

END