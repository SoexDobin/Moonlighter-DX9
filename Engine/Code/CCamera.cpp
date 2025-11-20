#include "CCamera.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev, 
                 const _vec3* pEye,
                 const _vec3* pAt,
                 const _vec3* pUp,
                 _float fFov,
                 _float fAspect,
                 _float fNear,
                 _float fFar)
    : CGameObject(pGraphicDev), 
    m_fFov(fFov), m_fAspect(fAspect), m_fNear(fNear), m_fFar(fFar),
    m_vEye(*pEye), m_vAt(*pAt), m_vUp(*pUp)
{
    D3DXMatrixIdentity(&m_matView);
    D3DXMatrixIdentity(&m_matProj);
}

CCamera::CCamera(const CCamera& rhs)
    : CGameObject(rhs),
    m_fFov(rhs.m_fFov), m_fAspect(rhs.m_fAspect), m_fNear(rhs.m_fNear), m_fFar(rhs.m_fFar),
    m_vEye(rhs.m_vEye), m_vAt(rhs.m_vAt), m_vUp(rhs.m_vUp)
{
    memcpy(&m_matView, &rhs.m_matView, sizeof(_matrix));
    memcpy(&m_matProj, &rhs.m_matProj, sizeof(_matrix));
}

CCamera::~CCamera()
{
}

HRESULT CCamera::Ready_GameObject()
{
    D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
    D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);

    m_pGraphicDevice->SetTransform(D3DTS_VIEW, &m_matView);
    m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

    return S_OK;
}

_int CCamera::Update_GameObject(const _float fTimeDelta)
{
    D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
    m_pGraphicDevice->SetTransform(D3DTS_VIEW, &m_matView);

    return 0;
}

void CCamera::LateUpdate_GameObject(const _float fTimeDelta)
{
}

void CCamera::Free()
{
    Engine::CGameObject::Free();
}
