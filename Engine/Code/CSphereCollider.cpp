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

    // 상태 백업
    DWORD prevFillMode = 0;
    m_pGraphicDevice->GetRenderState(D3DRS_FILLMODE, &prevFillMode);

    BOOL prevLighting = FALSE;
    m_pGraphicDevice->GetRenderState(D3DRS_LIGHTING, (DWORD*)&prevLighting);

    D3DXMATRIX prevWorld;
    m_pGraphicDevice->GetTransform(D3DTS_WORLD, &prevWorld);

    // 월드 행렬은 단위행렬로 두고, 월드좌표로 직접 찍는다
    D3DXMATRIX matIdentity;
    D3DXMatrixIdentity(&matIdentity);
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, &matIdentity);

    // 디버그용 정점
    struct DebugVtx
    {
        float x, y, z;
        DWORD color;
    };

    const DWORD color = D3DCOLOR_ARGB(255, 0, 255, 0);
    const int   segments = 32;   // 원 세그먼트 수

    _vec3 center = m_pTrans->Get_Pos();
    float radius = m_fRadius * m_fScale; // 필요하면 충돌 공식에 맞게 조정

    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    m_pGraphicDevice->SetTexture(0, nullptr);
    m_pGraphicDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

    auto DrawCircle = [&](int plane)
        {
            // plane: 0 = XZ, 1 = XY, 2 = YZ
            DebugVtx v[segments + 1];

            for (int i = 0; i <= segments; ++i)
            {
                float t = (2.f * D3DX_PI * i) / segments;
                float ct = cosf(t);
                float st = sinf(t);

                switch (plane)
                {
                case 0: // XZ 평면 (바닥면)
                    v[i].x = center.x + ct * radius;
                    v[i].y = center.y;
                    v[i].z = center.z + st * radius;
                    break;

                case 1: // XY 평면 (정면)
                    v[i].x = center.x + ct * radius;
                    v[i].y = center.y + st * radius;
                    v[i].z = center.z;
                    break;

                case 2: // YZ 평면 (측면)
                    v[i].x = center.x;
                    v[i].y = center.y + ct * radius;
                    v[i].z = center.z + st * radius;
                    break;
                }

                v[i].color = color;
            }

            // LINESTRIP은 정점개수 - 1 이 primitiveCount
            m_pGraphicDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, segments, v, sizeof(DebugVtx));
        };

    // 필요에 따라 원하는 평면만 호출해도 된다.
    DrawCircle(0); // XZ
    DrawCircle(1); // XY
    DrawCircle(2); // YZ

    // 상태 복원
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, &prevWorld);
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
