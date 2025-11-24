#include "CRectCollider.h"
#include "CSphereCollider.h"
#include "CCollisionManager.h"

#ifdef _DEBUG
#include "CTransform.h"
#endif

CRectCollider::CRectCollider()
    : CCollider()
{
}

CRectCollider::CRectCollider(LPDIRECT3DDEVICE9 pGraphicDev)
    : CCollider(pGraphicDev), m_vDimension( { 1.f, 1.f, 1.f } )
{
}

CRectCollider::CRectCollider(const CRectCollider& rhs)
    : CCollider(rhs), m_vDimension(rhs.m_vDimension)
{
}

CRectCollider::~CRectCollider()
{
}

HRESULT CRectCollider::Ready_RectCollider()
{
    return S_OK;
}

_int CRectCollider::Update_Component(const _float fTimeDelta)
{
    _int iExit = CCollider::Update_Component(fTimeDelta);

    return iExit;
}

void CRectCollider::LateUpdate_Component()
{
    CCollider::LateUpdate_Component();
}

_bool CRectCollider::Check_Collision(CCollider* pCol)
{
    switch (pCol->Get_ColType())
    {
    case SPHERE_COL:
        return CCollisionManager::SphereRectCollision(static_cast<CSphereCollider*>(pCol), this);
    case RECT_COL:
        return CCollisionManager::RectCollision(this, static_cast<CRectCollider*>(pCol));
    default:
        return false;
    }
}

#ifdef _DEBUG
void CRectCollider::Render_DebugCollider()
{
    if (m_pTrans == nullptr || m_pGraphicDevice == nullptr)
        return;

    struct DebugVtx { _vec3 v = { 0.f, 0.f, 0.f }; DWORD color = 0; };

    DWORD dwPrevFillMode = 0;
    m_pGraphicDevice->GetRenderState(D3DRS_FILLMODE, &dwPrevFillMode);
    BOOL bPrevLighting = FALSE;
    m_pGraphicDevice->GetRenderState(D3DRS_LIGHTING, (DWORD*)&bPrevLighting);
    D3DXMATRIX matPrevWorld;
    m_pGraphicDevice->GetTransform(D3DTS_WORLD, &matPrevWorld);
    // 월드 행렬은 단위행렬로 두고, 월드좌표로 직접 찍는다
    D3DXMATRIX matIdentity;
    D3DXMatrixIdentity(&matIdentity);
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, &matIdentity);

    DWORD color(0);
    if (m_eState == ENTER_COL || m_eState == STAY_COL)
        color = D3DCOLOR_ARGB(255, 255, 0, 0);
    else
        color = D3DCOLOR_ARGB(255, 0, 255, 0);

    _vec3 vMin = m_pTrans->Get_Pos() + m_vOffset + (m_vDimension * -0.5f);
    _vec3 vMax = m_pTrans->Get_Pos() + m_vOffset + (m_vDimension * 0.5f);

    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    m_pGraphicDevice->SetTexture(0, nullptr);
    m_pGraphicDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

    auto DrawLine = [&]()
    {
        DebugVtx v[8];
        v[0].v = { vMin.x, vMin.y, vMin.z };
        v[1].v = { vMin.x, vMax.y, vMin.z };
        v[2].v = { vMax.x, vMax.y, vMin.z };
        v[3].v = { vMax.x, vMin.y, vMin.z };

        v[4].v = { vMin.x, vMin.y, vMax.z };
        v[5].v = { vMin.x, vMax.y, vMax.z };
        v[6].v = { vMax.x, vMax.y, vMax.z };
        v[7].v = { vMax.x, vMin.y, vMax.z };
        for (size_t i = 0; i < 8; ++i)
            v[i].color = color;

        DebugVtx lines[24];

        lines[0] = v[0]; lines[1] = v[1];
        lines[2] = v[1]; lines[3] = v[2];
        lines[4] = v[2]; lines[5] = v[3];
        lines[6] = v[3]; lines[7] = v[0];

        lines[8] = v[4]; lines[9] = v[5];
        lines[10] = v[5]; lines[11] = v[6];
        lines[12] = v[6]; lines[13] = v[7];
        lines[14] = v[7]; lines[15] = v[4];

        lines[16] = v[0]; lines[17] = v[4];
        lines[18] = v[1]; lines[19] = v[5];
        lines[20] = v[2]; lines[21] = v[6];
        lines[22] = v[3]; lines[23] = v[7];

        m_pGraphicDevice->DrawPrimitiveUP(
            D3DPT_LINELIST,
            12,
            lines,
            sizeof(DebugVtx));
    };

    DrawLine();

    m_pGraphicDevice->SetTransform(D3DTS_WORLD, &matPrevWorld);
    m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, dwPrevFillMode);
    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, bPrevLighting);
}
#endif

CRectCollider* CRectCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CRectCollider* pInstance = new CRectCollider(pGraphicDev);
    if (FAILED(pInstance->Ready_RectCollider()))
    {
        MSG_BOX("CRectCollider Created Failed");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CComponent* CRectCollider::Clone()
{
    CCollider* pClone = new CRectCollider(*this);
    CCollisionManager::GetInstance()->Add_Collider(pClone);

    return pClone;
}

void CRectCollider::Free()
{
    CCollider::Free();
}
