#pragma once

#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDevice : public CBase
{
	DECLARE_SINGLETON(CGraphicDevice)
private:
	explicit CGraphicDevice();
	virtual ~CGraphicDevice() override;

public:
	LPDIRECT3DDEVICE9	Get_GraphicDevice() const { return m_pGraphicDevice; };

public:
	HRESULT		Ready_GraphicDevice(
		HWND hWnd, 
		WINMODE eMode,
		const _uint& iSizeX,
		const _uint& iSizeY,
		CGraphicDevice** ppGraphicDev);

	void		Render_Begin(D3DXCOLOR Color);
	void		Render_End();

private:
	LPDIRECT3D9			m_pSDK;			
	LPDIRECT3DDEVICE9	m_pGraphicDevice;

private:
	virtual void Free() override;
};

END