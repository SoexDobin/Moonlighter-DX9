#include "CTimer.h"
#include "CGraphicDevice.h"
#include "CLayer.h"
#include "CSphereCollider.h"

CTimer::CTimer()
	: m_fDeltaTime(0.f)
{
	ZeroMemory(&m_FrameTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_LastTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
}

CTimer::~CTimer()
{
	Free();
}

HRESULT CTimer::Ready_Timer()
{
	::QueryPerformanceCounter(&m_FrameTime);
	::QueryPerformanceCounter(&m_LastTime);
	::QueryPerformanceFrequency(&m_CpuTick);

	return S_OK;
}

void CTimer::Update_Timer()
{
	::QueryPerformanceCounter(&m_FrameTime);

	m_fDeltaTime = (m_FrameTime.QuadPart - m_LastTime.QuadPart) / _float(m_CpuTick.QuadPart);
	m_LastTime = m_FrameTime;
}

CTimer* CTimer::Create()
{
	CTimer* pInstance = new CTimer;

	if (FAILED(pInstance->Ready_Timer()))
	{
		Engine::Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTimer::Free()
{
}
