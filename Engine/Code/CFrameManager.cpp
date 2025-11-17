#include "CFrameManager.h"

IMPLEMENT_SINGLETON(CFrameManager)

CFrameManager::CFrameManager()
{
}

CFrameManager::~CFrameManager()
{
	Free();
}

_bool CFrameManager::IsPermit_Call(const _tchar* pFrameTag, const float fTimeDelta)
{
	CFrame* pFrame = Find_Frame(pFrameTag);
	if (nullptr == pFrame)
		return false;

	return pFrame->IsPermit_Call(fTimeDelta);
}

HRESULT CFrameManager::Ready_Frame(const _tchar* pFrameTag, const float fCallLimit)
{
	CFrame* pFrame = Find_Frame(pFrameTag);

	if (pFrame != nullptr)
		return E_FAIL;

	if (pFrame = CFrame::Create(fCallLimit))
	{
		m_umFrame.insert(pair<const _tchar*, CFrame*>{pFrameTag, pFrame});
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

CFrame* CFrameManager::Find_Frame(const _tchar* pFrameTag)
{
	auto iter = find_if(m_umFrame.begin(), m_umFrame.end()
		, [&](pair<const _tchar*, CFrame*> pair) -> _bool
		{
			if (!lstrcmp(pFrameTag, pair.first))
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
		, [](pair<const _tchar*, CFrame*> pair)
		{
			_ulong dwRefCnt = pair.second->Release();

			if (dwRefCnt == 0)
			{
				pair.second = nullptr;
			}
		});

	m_umFrame.clear();
}
