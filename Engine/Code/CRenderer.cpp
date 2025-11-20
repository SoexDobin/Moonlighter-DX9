#include "CRenderer.h"

IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer()
{
}

CRenderer::~CRenderer()
{
	Free();
}

void CRenderer::Add_RenderGroup(RENDERID eID, CGameObject* pGameObject)
{
	if (RENDER_END <= eID || eID < 0 || pGameObject == nullptr)
		return;

	m_RenderGroup[eID].push_back(pGameObject);
	pGameObject->AddRef();
}

void CRenderer::Render_GameObject(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Render_Priority(pGraphicDev);
	Render_NonAlpha(pGraphicDev);
	Render_Alpha(pGraphicDev);
	Render_UI(pGraphicDev);

	Clear_RenderGroup();
}

void CRenderer::Clear_RenderGroup()
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void CRenderer::Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for_each(m_RenderGroup[RENDER_PRIORITY].begin(), m_RenderGroup[RENDER_PRIORITY].end()
		, [](CGameObject* pGameObject) -> void {
			pGameObject->Render_GameObject();
		});
}

void CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for_each(m_RenderGroup[RENDER_NONALPHA].begin(), m_RenderGroup[RENDER_NONALPHA].end()
		, [](CGameObject* pGameObject) -> void {
			pGameObject->Render_GameObject();
		});
}

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for_each(m_RenderGroup[RENDER_ALPHA].begin(), m_RenderGroup[RENDER_ALPHA].end()
		, [](CGameObject* pGameObject) -> void {
			pGameObject->Render_GameObject();
		});

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRenderer::Render_UI(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for_each(m_RenderGroup[RENDER_UI].begin(), m_RenderGroup[RENDER_UI].end()
		, [](CGameObject* pGameObject) -> void {
			pGameObject->Render_GameObject();
		});
}

void CRenderer::Free()
{
	Clear_RenderGroup();
}