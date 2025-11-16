#include "CFrame.h"

CFrame::CFrame()
	: m_fCallLimit(0.f), m_fAccTimeDelta(0.f)
{
}

CFrame::~CFrame()
{
	Free();
}

_bool CFrame::IsPermit_Call(const _float fTimeDleta)
{
	m_fAccTimeDelta += fTimeDleta;

	if (m_fAccTimeDelta >= m_fCallLimit)
	{
		m_fAccTimeDelta = 0.f;

		return true;
	}

	return false;
}

HRESULT CFrame::Ready_Frame(const _float fCallLimit)
{
	m_fCallLimit = 1.f / fCallLimit;

	return S_OK;
}

CFrame* CFrame::Create(const _float fCallLimit)
{
	CFrame* pInstance = new CFrame();

	if (FAILED(pInstance->Ready_Frame(fCallLimit)))
	{
		Safe_Release(pInstance);
		MSG_BOX("frame create Failed");
		return nullptr;
	}

	return pInstance;
}

void CFrame::Free()
{
}
