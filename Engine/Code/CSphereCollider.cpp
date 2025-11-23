#include "CSphereCollider.h"
#include "CCollisionManager.h"

#ifdef _DEBUG
#include "CTransform.h"
#endif

CSphereCollider::CSphereCollider()
    : m_fRadius(1.f), m_fScale(1.f)
{
}

CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev)
    : CCollider(pGraphicDev), m_fRadius(1.f), m_fScale(1.f)
{
}

CSphereCollider::CSphereCollider(const CSphereCollider& rhs)
    : CCollider(rhs), m_fRadius(rhs.m_fRadius), m_fScale(rhs.m_fScale)
{
}

CSphereCollider::~CSphereCollider()
{
}

HRESULT CSphereCollider::Ready_SphereCollider()
{
    return S_OK;
}

_bool CSphereCollider::Check_Collision(CCollider* pCol)
{
    switch (pCol->Get_ColType())
    {
    case SPHERE_COL:
        return CCollisionManager::GetInstance()->SphereCollision(this, static_cast<CSphereCollider*>(pCol));
    }
}

_int CSphereCollider::Update_Component(const _float fTimeDelta)
{
    _int iExit = CCollider::Update_Component(fTimeDelta);

    return iExit;
}

void CSphereCollider::LateUpdate_Component()
{
    CCollider::LateUpdate_Component();
}

#ifdef _DEBUG
void CSphereCollider::Render_DebugCollider()
{
    if (m_pTrans == nullptr || m_pGraphicDevice == nullptr)
        return;

    // Save states we'll change
    DWORD prevFillMode = 0;
    m_pGraphicDevice->GetRenderState(D3DRS_FILLMODE, &prevFillMode);

    BOOL prevLighting = FALSE;
    m_pGraphicDevice->GetRenderState(D3DRS_LIGHTING, (DWORD*)&prevLighting);

    // Build world matrix (scale by radius/scale, then translate)
    _vec3 vPos = m_pTrans->Get_Pos();
    float radius = (m_fRadius * m_fScale); // m_fRadius * m_fScale 조합 사용
    D3DXMATRIX matS, matT, matWorld;
    D3DXMatrixScaling(&matS, radius, radius, radius);
    D3DXMatrixTranslation(&matT, vPos.x, vPos.y, vPos.z);
    matWorld = matS * matT;
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, &matWorld);

    // Set render states for debug lines
    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    m_pGraphicDevice->SetTexture(0, nullptr);

    struct DebugVtx { float x, y, z; DWORD color; };
    const DWORD color = D3DCOLOR_ARGB(255, 0, 255, 0);
    m_pGraphicDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

    const int slices = 16;
    const int stacks = 12;

    // latitude lines
    for (int i = 1; i < stacks; ++i)
    {
        float phi = D3DX_PI * float(i) / float(stacks);
        float y = cosf(phi);
        float r = sinf(phi);
        for (int j = 0; j < slices; ++j)
        {
            float t1 = 2.f * D3DX_PI * float(j) / float(slices);
            float t2 = 2.f * D3DX_PI * float(j + 1) / float(slices);
            DebugVtx v[2] = {
                { r * cosf(t1), y, r * sinf(t1), color },
                { r * cosf(t2), y, r * sinf(t2), color }
            };
            m_pGraphicDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, v, sizeof(DebugVtx));
        }
    }

    // longitude lines
    for (int j = 0; j < slices; ++j)
    {
        float theta = 2.f * D3DX_PI * float(j) / float(slices);
        for (int i = 0; i < stacks; ++i)
        {
            float p1 = D3DX_PI * float(i) / float(stacks);
            float p2 = D3DX_PI * float(i + 1) / float(stacks);
            DebugVtx v[2] = {
                { sinf(p1) * cosf(theta), cosf(p1), sinf(p1) * sinf(theta), color },
                { sinf(p2) * cosf(theta), cosf(p2), sinf(p2) * sinf(theta), color }
            };
            m_pGraphicDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, v, sizeof(DebugVtx));
        }
    }

    // Restore states
    m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, prevFillMode);
    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, prevLighting);
}
#endif

CSphereCollider* CSphereCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CSphereCollider* pInstance = new CSphereCollider(pGraphicDev);

    if (FAILED(pInstance->Ready_SphereCollider()))
    {
        MSG_BOX("CCollier Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

CComponent* CSphereCollider::Clone()
{
    CCollider* pClone = new CSphereCollider(*this);
    CCollisionManager::GetInstance()->Add_Collider(pClone);

    return pClone;
}

void CSphereCollider::Free()
{
    CCollider::Free();
}
