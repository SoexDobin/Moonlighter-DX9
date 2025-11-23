#include "CRenderer.h"

IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer()
	: m_iDrawCalls(0)
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

void CRenderer::Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	m_iDrawCalls = 0;

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

	pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for_each(m_RenderGroup[RENDER_ALPHA].begin(), m_RenderGroup[RENDER_ALPHA].end()
		, [](CGameObject* pGameObject) -> void {
			pGameObject->Render_GameObject();
		});

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

// 직교 투영 적용
void CRenderer::Render_UI(LPDIRECT3DDEVICE9& pGraphicDev)
{

    _matrix matOldView, matOldProj;
    pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
    pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    // 알파블렌딩
    pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    _matrix matView, matProj;
    // 뷰행렬 초기화, 카메라 적용x
    D3DXMatrixIdentity(&matView);
    pGraphicDev->SetTransform(D3DTS_VIEW, &matView);

    // 직교 투영
    //D3DXMatrixOrthoLH(&matProj, (float)WINCX , (float)WINCY, 0.f, 1.f);
    D3DXMatrixOrthoOffCenterLH(
        &matProj,
        0.f, (float)WINCX,    // left, right
        (float)WINCY, 0.f,    // bottom, top
        0.f, 1.f              // near, far
    );
    pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
    //z버퍼 끄기
    pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);


    for_each(m_RenderGroup[RENDER_UI].begin(), m_RenderGroup[RENDER_UI].end()
        , [](CGameObject* pGameObject) -> void {
            pGameObject->Render_GameObject();
        });

    

    // 복원
    pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
    pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
    pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);


}

void CRenderer::Free()
{
	Clear_RenderGroup();
}
