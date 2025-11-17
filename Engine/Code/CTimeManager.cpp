#include "CTimeManager.h"
#include "CTimer.h"

IMPLEMENT_SINGLETON(CTimeManager)

CTimeManager::CTimeManager()
{

}

CTimeManager::~CTimeManager()
{
	Free();
}

_float CTimeManager::Get_TimeDelta(const _tchar* pTimeTag)
{
	CTimer* pTimer = Find_Timer(pTimeTag);
	if (pTimer == nullptr)
		return 0.f;

	return pTimer->GetDeltaTime();
}

void CTimeManager::Set_TimeDelta(const _tchar* pTimeTag)
{
	CTimer* pTimer = Find_Timer(pTimeTag);
	if (pTimer == nullptr)
		return;

	pTimer->Update_Timer();
}

HRESULT CTimeManager::Ready_Timer(const _tchar* pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);

	if (pTimer != nullptr)
		return E_FAIL;

	if (pTimer = CTimer::Create())
	{
		m_umTimer.insert(pair<const _tchar*, CTimer*>{ pTimerTag, pTimer});
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

CTimer* CTimeManager::Find_Timer(const _tchar* pTimerTag)
{
	auto iter = find_if(m_umTimer.begin(), m_umTimer.end()
		, [&](pair<const _tchar*, CTimer*> pair) -> _bool {
			if (!lstrcmp(pTimerTag, pair.first))
				return true;
			
			return false;
		});

	if (iter != m_umTimer.end())
		return iter->second;
	else
		return nullptr;
}

void CTimeManager::Free()
{
	for_each(m_umTimer.begin(), m_umTimer.end(), 
		[](pair<const _tchar*, CTimer*> pair) 
		{
			_ulong dwRefCnt = pair.second->Release();

			if (dwRefCnt == 0)
				pair.second = nullptr;
		});
	m_umTimer.clear();
}
