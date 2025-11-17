#include "CRenderObject.h"

CRenderObject::CRenderObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CRenderObject::CRenderObject(const CRenderObject& rhs)
	: CGameObject(rhs)
{
}

CRenderObject::~CRenderObject()
{
}

HRESULT CRenderObject::Ready_GameObject()
{

	return S_OK;
}

_int CRenderObject::Update_GameObject(const _float fTimeDelta)
{
	return 0;
}

void CRenderObject::LateUpdate_GameObject(const _float fTimeDelta)
{
}

void CRenderObject::Render_GameObject()
{
}

CRenderObject* CRenderObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{

	return nullptr;
}

void CRenderObject::Free()
{
	Engine::CGameObject::Free();
}
