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

	CComponent* pCom = nullptr;
	if (pCom = CPrototypeManager::GetInstance()->Clone_Prototype(RECTTEX))
	{
		if (pCom->Get_ComponentType() == RECTTEX)
		{
			m_pBufferCom = static_cast<CRectTex*>(pCom);
			m_umComponent[ID_STATIC].emplace(pair<const wstring, CComponent*>
			{ n_wsBufferKey, pCom });
		}
		else
			return E_FAIL;
	}
	else
		return E_FAIL;
	
	if (pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TRANSFORM))
	{
		if (pCom->Get_ComponentType() == TRANSFORM)
		{
			m_pTransformCom = static_cast<CTransform*>(pCom);
			m_umComponent[ID_DYNAMIC].emplace(pair<const wstring, CComponent*>
			{ n_wsTransformKey, pCom });
		}
		else
			return E_FAIL;
	}
	else
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
