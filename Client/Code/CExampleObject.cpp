#include "pch.h"
#include "CExampleObject.h"
#include "CRenderer.h"
#include "CPrototypeManager.h"

CExampleObject::CExampleObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CRenderObject(pGraphicDev)
{
	PANEL_NAME(L"Example Object");
}

CExampleObject::CExampleObject(const CExampleObject& rhs)
	: CRenderObject(rhs)
{
	PANEL_NAME(L"Example Object");
}

CExampleObject::~CExampleObject()
{
}

HRESULT CExampleObject::Ready_GameObject()
{
	if (FAILED(Engine::CRenderObject::Ready_GameObject()))
		return E_FAIL;

	m_pTransformCom->Set_Pos({ -5.f, 0.f, 10.f });

	Add_EditorField();

	return S_OK;
}

_int CExampleObject::Update_GameObject(const _float fTimeDelta)
{
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	_vec3 vDir;
	m_pTransformCom->Get_Info(INFO_RIGHT, &vDir);
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 0.5f);

	Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CExampleObject::LateUpdate_GameObject(const _float fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CExampleObject::Render_GameObject()
{
	m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pBufferCom->Render_Buffer();
}

CExampleObject* CExampleObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CExampleObject* pInstance = new CExampleObject(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CExampleObject Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CExampleObject::Free()
{
	CGameObject::Free();
}

void CExampleObject::Add_EditorField()
{
	// Examples 
	CGameObject::Add_EditorField("bool test", DATATYPE::DATA_BOOL, &m_bBool);
	CGameObject::Add_EditorField("byte test", DATATYPE::DATA_BYTE, &m_bByte);
	CGameObject::Add_EditorField("ubyte test", DATATYPE::DATA_UBYTE, &m_ubByte);
	CGameObject::Add_EditorField("TCHAR test", DATATYPE::DATA_TCHAR, &m_szChar);
	CGameObject::Add_EditorField("short test", DATATYPE::DATA_SHORT, &m_sShort);
	CGameObject::Add_EditorField("ushort test", DATATYPE::DATA_USHORT, &m_usShort);
	CGameObject::Add_EditorField("int test", DATATYPE::DATA_INT, &m_iInt);
	CGameObject::Add_EditorField("uint test", DATATYPE::DATA_UINT, &m_uiInt);
	CGameObject::Add_EditorField("long test", DATATYPE::DATA_LONG, &m_lLong);
	CGameObject::Add_EditorField("ulong test", DATATYPE::DATA_ULONG, &m_ulLong);
	CGameObject::Add_EditorField("float test", DATATYPE::DATA_FLOAT, &m_fFloat);
	CGameObject::Add_EditorField("double test", DATATYPE::DATA_DOUBLE, &m_dDouble);
	CGameObject::Add_EditorField("vec2 test", DATATYPE::DATA_VEC2, &m_v2);
	CGameObject::Add_EditorField("vec3 test", DATATYPE::DATA_VEC3, &m_v3);
	CGameObject::Add_EditorField("vec4 test", DATATYPE::DATA_VEC4, &m_v4);
	CGameObject::Add_EditorField("mat test", DATATYPE::DATA_MAT, &m_mat);
	D3DXMatrixIdentity(&m_mat);
}
