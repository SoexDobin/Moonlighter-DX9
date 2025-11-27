#include "CCamera.h"
#include "CPrototypeManager.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev,
    const _vec3* pEye,
    const _vec3* pAt,
    const _vec3* pUp,
    _float fFov,
    _float fAspect,
    _float fNear,
    _float fFar,
    PROJECTION_TYPE eProj)
    : CGameObject(pGraphicDev),
    m_fFov(fFov), m_fAspect(fAspect), m_fNear(fNear), m_fFar(fFar),
    m_vEye(*pEye), m_vAt(*pAt), m_vUp(*pUp),
    m_eProjType(eProj), m_bProjectionChanged(false), m_pTransformCom(nullptr)
{
    D3DXMatrixIdentity(&m_matView);
    D3DXMatrixIdentity(&m_matProj);
}

CCamera::CCamera(const CCamera& rhs)
    : CGameObject(rhs),
    m_fFov(rhs.m_fFov), m_fAspect(rhs.m_fAspect), m_fNear(rhs.m_fNear), m_fFar(rhs.m_fFar),
    m_vEye(rhs.m_vEye), m_vAt(rhs.m_vAt), m_vUp(rhs.m_vUp),
    m_eProjType(rhs.m_eProjType), m_bProjectionChanged(rhs.m_bProjectionChanged)
{
    memcpy(&m_matView, &rhs.m_matView, sizeof(_matrix));
    memcpy(&m_matProj, &rhs.m_matProj, sizeof(_matrix));

    *m_pTransformCom = *rhs.m_pTransformCom;

}

CCamera::~CCamera()
{
}

HRESULT CCamera::Ready_GameObject()
{
    CComponent* pComponent = CPrototypeManager::GetInstance()->Clone_Prototype(TRANSFORM);
    if (pComponent && (TRANSFORM == pComponent->Get_ComponentType()))
    {
        m_pTransformCom = static_cast<CTransform*>(pComponent);
    }

    // 뷰 변환
    D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
    m_pGraphicDevice->SetTransform(D3DTS_VIEW, &m_matView);

    // 투영 변환
    if (PROJECTION_TYPE::PROJ_PERSPECTIVE == m_eProjType)
    {
        D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
    }
    else if (PROJECTION_TYPE::PROJ_ORTHOGRAPHIC == m_eProjType)
    {
        D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, m_fNear, m_fFar);
    }
    m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

    return S_OK;
}

_int CCamera::Update_GameObject(const _float fTimeDelta)
{
    m_pTransformCom->Update_Component(fTimeDelta);

    return 0;
}

void CCamera::LateUpdate_GameObject(const _float fTimeDelta)
{
    // 뷰 행렬 재설정
    Compute_ViewMatrix();
    D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
    m_pGraphicDevice->SetTransform(D3DTS_VIEW, &m_matView);

    // 투영 변환 행렬 재설정
    if (m_bProjectionChanged)
    {
        if (PROJECTION_TYPE::PROJ_PERSPECTIVE == m_eProjType)
        {
            D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
        }
        else if (PROJECTION_TYPE::PROJ_ORTHOGRAPHIC == m_eProjType)
        {
            D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, m_fNear, m_fFar);
        }
        m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

        m_bProjectionChanged = false;
    }
}

void CCamera::Translate(INFO eInfo, const _float& fTimeDelta, const _float& fSpeed)
{
    _vec3 vDir;
    m_pTransformCom->Get_Info(eInfo, &vDir);

    m_pTransformCom->Move_Pos(&vDir, fTimeDelta, fSpeed);
}

void CCamera::Translate(const _vec3& vDir, const _float& fTimeDelta, const _float& fSpeed)
{
    m_pTransformCom->Move_Pos(&vDir, fTimeDelta, fSpeed);
}

void CCamera::Rotate(ROTATION eAxis, const _float& fRadian)
{
    m_pTransformCom->Add_Rotation(eAxis, fRadian);
}

void CCamera::Rotate(const _vec3& vNewAt)
{
    // 타겟을 바라보도록 회전하는 함수가 필요할지?
}

void CCamera::Compute_ViewMatrix()
{
    // Eye
    m_vEye = m_pTransformCom->Get_Pos();

    // At
    _vec3 vLook;
    m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
    D3DXVec3Normalize(&vLook, &vLook);
    m_vAt = m_vEye + vLook;
}

CCamera* CCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
    const _vec3* pEye, const _vec3* pAt, const _vec3* pUp,
    _float fFov, _float fAspect, _float fNear, _float fFar,
    PROJECTION_TYPE eType)
{
    CCamera* pInstance = new CCamera(pGraphicDev, pEye, pAt, pUp, fFov, fAspect, fNear, fFar, eType);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Camera Create Failed");
        return nullptr;
    }
    return pInstance;
}

void CCamera::Free()
{
    Engine::CGameObject::Free();

    m_pTransformCom->Release();
}
