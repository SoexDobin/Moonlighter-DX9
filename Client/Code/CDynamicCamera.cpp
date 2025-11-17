#include "pch.h"
#include "CDynamicCamera.h"
#include "CRenderer.h"
#include "CDInputManager.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev, 
                               const _vec3* pEye,
                               const _vec3* pAt,
                               const _vec3* pUp,
                               const _float fFov,
                               const _float fAspect,
                               const _float fNear,
                               const _float fFar)
    : CCamera(pGraphicDev, pEye, pAt, pUp, fFov, fAspect, fNear, fFar),
    m_fSpeed(0.f), m_bFix(false), m_bCheck(false)
{
}

CDynamicCamera::CDynamicCamera(const CDynamicCamera& rhs)
    : CCamera(rhs),
    m_fSpeed(rhs.m_fSpeed), m_bFix(rhs.m_bFix), m_bCheck(rhs.m_bCheck)
{
}

CDynamicCamera::~CDynamicCamera()
{
}

HRESULT CDynamicCamera::Ready_GameObject()
{
    if (FAILED(Engine::CCamera::Ready_GameObject()))
        return E_FAIL;

    m_fSpeed = 10.f;

    return S_OK;
}

_int CDynamicCamera::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = Engine::CCamera::Update_GameObject(fTimeDelta);

    return iExit;
}

void CDynamicCamera::LateUpdate_GameObject(const _float fTimeDelta)
{
    Engine::CCamera::LateUpdate_GameObject(fTimeDelta);

    Key_Input(fTimeDelta);

    if (m_bFix)
    {
        Mouse_Fix();
        Mouse_Move();
    }
}

void CDynamicCamera::Key_Input(const _float fTimeDelta)
{
    _vec3   vLook, vRight;
    _matrix matCamWorld;
    D3DXMatrixInverse(&matCamWorld, 0, &m_matView);
    if (matCamWorld == NULL)
        return;
    
    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_D) & 0x80)
    {
        memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

        _vec3 vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * m_fSpeed;
        m_vAt += vLength;
        m_vEye += vLength;
    }
    else if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_A) & 0x80)
    {
        memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

        _vec3 vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * m_fSpeed;
        m_vAt -= vLength;
        m_vEye -= vLength;
    }

    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_W) & 0x80)
    {
        memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

        _vec3 vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * m_fSpeed;
        m_vAt += vLength;
        m_vEye += vLength;
    }
    else if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_S) & 0x80)
    {
        memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

        _vec3 vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * m_fSpeed;
        m_vAt -= vLength;
        m_vEye -= vLength;
    }

    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_TAB) & 0x80)
    {
        if (m_bCheck) return;
        m_bCheck = true;

        if (m_bFix) m_bFix = false;
        else m_bFix = true;
    }
    else
        m_bCheck = false;
    
    if (false == m_bFix) return;
}

void CDynamicCamera::Mouse_Move()
{
    _matrix matCamWorld;
    _long dwMouseMove(0);
    D3DXMatrixInverse(&matCamWorld, 0, &m_matView);
    if (matCamWorld == NULL)
        return;

    if (dwMouseMove = CDInputManager::GetInstance()->Get_DIMouseMove(DIMS_Y))
    {
        _vec3 vRight, vLook = m_vAt - m_vEye;
        _matrix matAxisY;
        memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

        D3DXMatrixRotationAxis(&matAxisY, &vRight, D3DXToRadian(dwMouseMove / 10.f));
        D3DXVec3TransformNormal(&vLook, &vLook, &matAxisY);

        m_vAt = m_vEye + vLook;
    }

    if (dwMouseMove = CDInputManager::GetInstance()->Get_DIMouseMove(DIMS_X))
    {
        _vec3 vUp(0.f, 1.f, 0.f);
        _vec3 vLook = m_vAt - m_vEye;
        _matrix matAxisX;
    
        D3DXMatrixRotationAxis(&matAxisX, &vUp, D3DXToRadian(dwMouseMove / 10.f));
        D3DXVec3TransformNormal(&vLook, &vLook, &matAxisX);
    
        m_vAt = m_vEye + vLook;
    }
}

void CDynamicCamera::Mouse_Fix()
{
    POINT		ptMouse{ WINCX >> 1, WINCY >> 1 };

    ClientToScreen(g_hWnd, &ptMouse);
    SetCursorPos(ptMouse.x, ptMouse.y);
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
                                       const _vec3* pEye, 
                                       const _vec3* pAt, 
                                       const _vec3* pUp, 
                                       const _float fFov, 
                                       const _float fAspect, 
                                       const _float fNear, 
                                       const _float fFar)
{
    CDynamicCamera* pInstance = new CDynamicCamera(pGraphicDev, pEye, pAt, pUp, fFov, fAspect, fNear, fFar);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        MSG_BOX("CDynamicCamera Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CDynamicCamera::Free()
{
    Engine::CCamera::Free();
}
