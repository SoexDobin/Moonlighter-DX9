#include "pch.h"
#include "CTerrainVillage.h"
#include "CPrototypeManager.h"
#include "CRenderer.h"

CTerrainVillage::CTerrainVillage(LPDIRECT3DDEVICE9 pGraphicDev) : CRenderObject(pGraphicDev)
, m_pBufferCom(nullptr), m_pTransformCom(nullptr), m_pTextureCom(nullptr)
{
}

CTerrainVillage::CTerrainVillage(const CTerrainVillage& rhs) : CRenderObject(rhs)
, m_pBufferCom(nullptr), m_pTransformCom(nullptr), m_pTextureCom(nullptr)
{
}

CTerrainVillage::~CTerrainVillage()
{
}

HRESULT CTerrainVillage::Ready_GameObject()
{

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CTerrainVillage::Update_GameObject(const _float fTimeDelta)
{
	_int iExit = CRenderObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CTerrainVillage::LateUpdate_GameObject(const _float fTimeDelta)
{
	CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CTerrainVillage::Render_GameObject()
{
	m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

HRESULT CTerrainVillage::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE_TERRAIN));

	if (nullptr == pComponent)
		return E_FAIL;

	m_umComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	// transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>
		(Engine::CPrototypeManager::GetInstance()->Clone_Prototype(TRANSFORM));

	if (nullptr == pComponent)
		return E_FAIL;

	m_umComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	//// texture
	//pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>
	//	(Engine::CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE_TERRAIN));

	//if (nullptr == pComponent)
	//	return E_FAIL;

	m_umComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CTerrainVillage* CTerrainVillage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrainVillage* pTerrainVillege = new CTerrainVillage(pGraphicDev);

	if (FAILED(pTerrainVillege->Ready_GameObject()))
	{
		MSG_BOX("pTerrainVillege Create Failed");
		Safe_Release(pTerrainVillege);
		return nullptr;
	}

	return pTerrainVillege;
}

void CTerrainVillage::Free()
{
	CRenderObject::Free();
}