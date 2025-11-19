#include "CFrameManager.h"
#include "CTimeManager.h"

IMPLEMENT_SINGLETON(CFrameManager)

CFrameManager::CFrameManager()
	: m_mainFrame(nullptr), m_bGamePaused(false), m_bStepFrame(false), m_bNextStep(false), m_bTimeDeltaModified(false), 
	m_fLastTimeDelta(0.f), m_fComputeDelta(0.f), m_iComputeFPS(0), m_iLastFPS(0)
{
}

CFrameManager::~CFrameManager()
{
	Free();
}

_bool CFrameManager::IsPermit_Call(const wstring wsFrameTag, const _float fTimeDelta)
{
	CFrame* pFrame = Find_Frame(wsFrameTag);
	if (nullptr == pFrame)
		return false;

	return pFrame->IsPermit_Call(fTimeDelta);
}

HRESULT CFrameManager::Ready_Frame(const wstring wsFrameTag, const _float fCallLimit)
{
	CFrame* pFrame = Find_Frame(wsFrameTag);

	if (pFrame != nullptr)
		return E_FAIL;

	if (pFrame = CFrame::Create(fCallLimit))
	{
		m_umFrame.insert(pair<const wstring, CFrame*>{wsFrameTag, pFrame});
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

_bool CFrameManager::IsTransit_NextFrame(_float fTimeDelta)
{
	// Step mode active, and next frame is requested
	if (m_bStepFrame && m_bNextStep)
	{
		m_bNextStep = false;
		return true;
	}

	if (m_bGamePaused)
		return false;

	return IsPermit_Call(m_wsMainFrameTag, fTimeDelta);
}

void CFrameManager::Transit_NextFrame()
{
	m_bNextStep = true;
}

void CFrameManager::Compute_FPS()
{
	_float fDeltaTime_Immediate = Engine::CTimeManager::GetInstance()->Get_TimeDelta(L"DELTA_FPS60");

	m_fComputeDelta += fDeltaTime_Immediate;
	m_iComputeFPS++;

	if (m_fComputeDelta >= 1.f)
	{
		m_fComputeDelta = 0.f;
		m_iLastFPS = m_iComputeFPS;
		m_iComputeFPS = 0;
	}
}

void CFrameManager::Pause_Game()
{
	m_bGamePaused = true;
	m_bStepFrame = true;
}

void CFrameManager::Restart_Game()
{
	m_bGamePaused = false;
	m_bTimeDeltaModified = false;
	m_bStepFrame = false;
}

void CFrameManager::Set_MainFrame(const wstring wsFrameTag)
{
	m_wsMainFrameTag = wsFrameTag;
	m_mainFrame = Find_Frame(m_wsMainFrameTag);
}

void CFrameManager::Modify_LastTimeDelta(_float* pTimeDelta)
{
	if (m_bStepFrame)
	{
		*pTimeDelta = m_fLastTimeDelta;
		return;
	}

	if (!m_bTimeDeltaModified)
	{
		m_bTimeDeltaModified = true;

		*pTimeDelta = m_fLastTimeDelta;
	}

	Compute_FPS();
}

CFrame* CFrameManager::Find_Frame(const wstring wsFrameTag)
{
	auto iter = find_if(m_umFrame.begin(), m_umFrame.end()
		, [&](pair<const wstring, CFrame*> pair) -> _bool
		{
			if (wsFrameTag == pair.first)
				return true;
			
			return false;
		});

	if (iter == m_umFrame.end())
		return nullptr;

	return iter->second;
}

void CFrameManager::Free()
{
	for_each(m_umFrame.begin(), m_umFrame.end()
		, [](pair<const wstring, CFrame*> pair)
		{
			_ulong dwRefCnt = pair.second->Release();

			if (dwRefCnt == 0)
			{
				pair.second = nullptr;
			}
		});

	m_umFrame.clear();
}
