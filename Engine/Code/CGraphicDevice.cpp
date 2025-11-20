#include "CGraphicDevice.h"

IMPLEMENT_SINGLETON(CGraphicDevice)

CGraphicDevice::CGraphicDevice()
	: m_pSDK(nullptr), m_pGraphicDevice(nullptr)
{
}

CGraphicDevice::~CGraphicDevice()
{
	Free();
}

HRESULT CGraphicDevice::Ready_GraphicDevice(
	HWND hWnd, 
	WINMODE eMode, 
	const _uint& iSizeX, const _uint& iSizeY, 
	CGraphicDevice** ppGraphicDev)
{
	if (m_pSDK = Direct3DCreate9(D3D_SDK_VERSION))
	{
		D3DCAPS9	tCAPS;
		ZeroMemory(&tCAPS, sizeof(D3DCAPS9));

		if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &tCAPS)))
			return E_FAIL;
		
		_ulong dwFlag(0);
		if (tCAPS.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
			dwFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
		else 
			dwFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

		D3DPRESENT_PARAMETERS tD3DPP;
		ZeroMemory(&tD3DPP, sizeof(D3DPRESENT_PARAMETERS));

		{
			tD3DPP.BackBufferWidth = iSizeX;
			tD3DPP.BackBufferHeight = iSizeY;
			tD3DPP.BackBufferFormat = D3DFMT_A8R8G8B8;
			tD3DPP.BackBufferCount = 1;
			
			tD3DPP.MultiSampleType = D3DMULTISAMPLE_NONE;
			tD3DPP.MultiSampleQuality = 0;

			tD3DPP.hDeviceWindow = hWnd;
			tD3DPP.Windowed = eMode;
			tD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;

			tD3DPP.EnableAutoDepthStencil = TRUE;
			tD3DPP.AutoDepthStencilFormat = D3DFMT_D24S8;

			tD3DPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
			tD3DPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		}
		
		if (FAILED(m_pSDK->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			dwFlag,
			&tD3DPP,
			&m_pGraphicDevice)))
		{
			MSG_BOX("FAILED Create GraphicDevice At CGrapicDevice.cpp");
			return E_FAIL;
		}
			
		*ppGraphicDev = this;

		return S_OK;
	}
	else
	{
		MSG_BOX("MISS MATCH VERSION! At CGrapicDevice.cpp");
		return E_FAIL;
	}
}

void CGraphicDevice::Render_Begin(D3DXCOLOR Color)
{
	if (m_pGraphicDevice == nullptr) return;

	m_pGraphicDevice->Clear(0, 
							NULL, 
							D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,
							Color, 
							1.f, 
							0);

	m_pGraphicDevice->BeginScene();
}

void CGraphicDevice::Render_End()
{
	if (m_pGraphicDevice == nullptr) return;

	m_pGraphicDevice->EndScene();
	m_pGraphicDevice->Present(NULL, NULL, NULL, NULL);
}

void CGraphicDevice::Free()
{
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pSDK);
}