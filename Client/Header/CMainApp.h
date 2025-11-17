#pragma once

#include "CBase.h"

#pragma region EngineInclude
#include "CGraphicDevice.h"
#include "CTimeManager.h"
#include "CFrameManager.h"
namespace Engine {
	class CManagement;
}
#pragma endregion

class CMainApp : public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp() override;

public:
	HRESULT		Ready_MainApp();
	_int		Update_MainApp(const _float fDeltaTime);
	void		LateUpdate_MainApp(const _float fDeltaTime);
	void		Render_MainApp();

private:
	HRESULT		Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDevice);
	HRESULT		Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDevice);

private:
	Engine::CGraphicDevice*		m_pDeviceClass;
	Engine::CManagement*		m_pManageClass;
	LPDIRECT3DDEVICE9			m_pGraphicDevice;

public:
	static CMainApp* Create();

private:
	void Free() override;
};

