#include "pch.h"
#include "CMainApp.h"

#include "CManagement.h"
#include "CDInputManager.h"
#include "CRenderer.h"
#include "CPrototypeManager.h"
#include "CDataManager.h"
#include "CEditor.h"
#include "CLightManager.h"

#include "CMainScene.h"
#include "CPlayerTestScene.h"

CMainApp::CMainApp()
	: m_pDeviceClass(nullptr), m_pGraphicDevice(nullptr)
	, m_pManageClass(nullptr)
{
	m_pManageClass = CManagement::GetInstance();
}

CMainApp::~CMainApp()
{

}

HRESULT CMainApp::Ready_MainApp()
{
	if (FAILED(Ready_DefaultSetting(&m_pGraphicDevice)))
		return E_FAIL;

	if (FAILED(Ready_Scene(m_pGraphicDevice)))
		return E_FAIL;

	return S_OK;
}

_int CMainApp::Update_MainApp(const _float fDeltaTime)
{
	Engine::CDInputManager::GetInstance()->Update_InputDev();

	m_pManageClass->Update_Scene(fDeltaTime);

	return 0;
}

void CMainApp::LateUpdate_MainApp(const _float fDeltaTime)
{
	m_pManageClass->LateUpdate_Scene(fDeltaTime);

}

void CMainApp::Render_MainApp()
{
	m_pDeviceClass->Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

	CEditor::GetInstance()->Render_Begin();
	CEditor::GetInstance()->Render_Editor();
	m_pManageClass->Render_Scene(m_pGraphicDevice);
	CEditor::GetInstance()->Render_End();
	m_pDeviceClass->Render_End();
}

HRESULT CMainApp::Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDevice)
{
	if (FAILED(CGraphicDevice::GetInstance()->Ready_GraphicDevice(
		g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass
	)))
	{
		return E_FAIL;
	}
	m_pDeviceClass->AddRef();

	(*ppGraphicDevice) = m_pDeviceClass->Get_GraphicDevice();
	(*ppGraphicDevice)->AddRef();

	(*ppGraphicDevice)->SetRenderState(D3DRS_LIGHTING, FALSE);
	(*ppGraphicDevice)->SetRenderState(D3DRS_ZENABLE, TRUE);
	(*ppGraphicDevice)->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//(*ppGraphicDevice)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//(*ppGraphicDevice)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	if (FAILED(CDInputManager::GetInstance()->Ready_InputDev(g_hInst, g_hWnd)))
		return E_FAIL;
	if (FAILED(CDataManager::GetInstance()->Ready_Data((*ppGraphicDevice))))
		return E_FAIL;
	if (FAILED(CEditor::GetInstance()->Ready_Editor(g_hWnd, m_pGraphicDevice)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	if (FAILED(Engine::CManagement::GetInstance()->Set_Scene(CMainScene::Create(pGraphicDevice))))
		return E_FAIL;
	
	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pApp = new CMainApp();
	if (FAILED(pApp->Ready_MainApp()))
	{
		MSG_BOX("CMainApp Create Failed");
		Safe_Release(pApp);
		return nullptr;
	}

	return pApp;
}

void CMainApp::Free()
{
	Safe_Release(m_pDeviceClass);
	Safe_Release(m_pGraphicDevice);

	CDataManager::DestroyInstance();

	Engine::CEditor::DestroyInstance();
	Engine::CLightManager::DestroyInstance();
	Engine::CDInputManager::DestroyInstance();
	Engine::CRenderer::DestroyInstance();
	Engine::CGraphicDevice::DestroyInstance();
	Engine::CManagement::DestroyInstance();
	Engine::CTimeManager::DestroyInstance();
	Engine::CFrameManager::DestroyInstance();
	Engine::CPrototypeManager::DestroyInstance();
}
