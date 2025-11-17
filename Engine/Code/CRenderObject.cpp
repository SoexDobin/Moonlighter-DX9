#include "CRenderObject.h"
#include "CPrototypeManager.h"

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
	if (FAILED(Engine::CGameObject::Ready_GameObject()))
		return E_FAIL;


	return S_OK;
}

_int CRenderObject::Update_GameObject(const _float fTimeDelta)
{
	return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CRenderObject::LateUpdate_GameObject(const _float fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CRenderObject::Render_GameObject()
{

}

void CRenderObject::Free()
{
	Engine::CGameObject::Free();
}
