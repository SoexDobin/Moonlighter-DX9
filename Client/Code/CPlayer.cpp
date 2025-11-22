#include "pch.h"
#include "CPlayer.h"

#include "CRenderer.h"
#include "CPrototypeManager.h"
#include "CDInputManager.h"
#include "CTexture.h"
#include "CTransform.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pTexCom(nullptr), m_eState(IDLE), m_eDir(DIR_DOWN), m_ePrevState(STATE_END), m_ePrevDir(DIR_END), m_fRollTime(0.f), m_fRollDuration(0.5f), m_vRollDir{ 0.f, 0.f, 0.f }
{
    PANEL_NAME(L"Player");
}

CPlayer::CPlayer(const CPlayer& rhs)
    : CRenderObject(rhs), m_pTexCom(nullptr), m_eState(rhs.m_eState), m_eDir(rhs.m_eDir), m_ePrevState(STATE_END), m_ePrevDir(DIR_END), m_fRollTime(0.f), m_fRollDuration(rhs.m_fRollDuration), m_vRollDir{ 0.f, 0.f, 0.f }
{
    PANEL_NAME(L"Player");
}

CPlayer::~CPlayer()
{
    PANEL_NAME(L"Player");
}

HRESULT CPlayer::Ready_GameObject()
{
    if (FAILED(CRenderObject::Ready_GameObject()))
        return E_FAIL;

    if (FAILED(Ready_Animation()))
        return E_FAIL;

    m_pTransformCom->Set_Scale(27.f, 27.f, 1.f);
    m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);

    m_eState        = IDLE;
    m_eDir          = DIR_DOWN;
    m_ePrevState    = STATE_END;
    m_ePrevDir      = DIR_END;
    m_fRollTime     = 0.f;

    return S_OK;
}

HRESULT CPlayer::Ready_Animation()
{
    CComponent* pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
    if (!pCom || pCom->Get_ComponentType() != TEXTURE)
        return E_FAIL;

    m_pTexCom = static_cast<CTexture*>(pCom);
    m_pTexCom->Set_Speed(8.5f);

    // Idle
    m_pTexCom->Ready_Texture(L"Player_Idle_Down");
    m_pTexCom->Ready_Texture(L"Player_Idle_Up");
    m_pTexCom->Ready_Texture(L"Player_Idle_Left");
    m_pTexCom->Ready_Texture(L"Player_Idle_Right");

    // Walk
    m_pTexCom->Ready_Texture(L"Player_Walk_Down");
    m_pTexCom->Ready_Texture(L"Player_Walk_Up");
    m_pTexCom->Ready_Texture(L"Player_Walk_Left");
    m_pTexCom->Ready_Texture(L"Player_Walk_Right");

    // Roll
    m_pTexCom->Ready_Texture(L"Player_Roll_Down");
    m_pTexCom->Ready_Texture(L"Player_Roll_Up");
    m_pTexCom->Ready_Texture(L"Player_Roll_Left");
    m_pTexCom->Ready_Texture(L"Player_Roll_Right");

    m_pTexCom->Set_Texture(IDLE);
    m_umComponent[ID_DYNAMIC].insert(pair<wstring, CComponent*>(L"Player_TexCom", m_pTexCom));

    return S_OK;
}

_uint CPlayer::Get_AnimationIndex()
{
    _uint iBase = 0;

    switch (m_eState)
    {
    case IDLE: iBase = 0; break;
    case WALK: iBase = 4; break;
    case ROLL: iBase = 8; break;
    default:   iBase = 0; break;
    }

    switch (m_eDir)
    {
    case DIR_DOWN:  return iBase + 0;
    case DIR_UP:    return iBase + 1;
    case DIR_LEFT:  return iBase + 2;
    case DIR_RIGHT: return iBase + 3;
    }

    return iBase;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
    if (m_eState == ROLL)
        return;

    auto* pInput = CDInputManager::GetInstance();

    _vec3 vDir   = { 0.f, 0.f, 0.f };
    bool bMoving = false;

    if (pInput->Get_DIKeyState(DIK_UP) & 0x80)
    {
        vDir.z += 1.f;
        m_eDir  = DIR_UP;
        bMoving = true;
    }
    if (pInput->Get_DIKeyState(DIK_DOWN) & 0x80)
    {
        vDir.z -= 1.f;
        m_eDir  = DIR_DOWN;
        bMoving = true;
    }
    if (pInput->Get_DIKeyState(DIK_LEFT) & 0x80)
    {
        vDir.x -= 1.f;
        m_eDir  = DIR_LEFT;
        bMoving = true;
    }
    if (pInput->Get_DIKeyState(DIK_RIGHT) & 0x80)
    {
        vDir.x += 1.f;
        m_eDir  = DIR_RIGHT;
        bMoving = true;
    }
    if (pInput->Get_DIKeyState(DIK_J) & 0x80)
    {
        vDir.y += 1.f;
        bMoving = true;
    }
    if (pInput->Get_DIKeyState(DIK_K) & 0x80)
    {
        vDir.y -= 1.f;
        bMoving = true;
    }

    if (pInput->Get_DIKeyState(DIK_SPACE) & 0x80)
    {
        if (m_eState != ROLL)
        {
            m_eState  = ROLL;

            m_pTexCom->Set_Speed(15.f);

            m_fRollTime = 0.f;

            switch (m_eDir)
            {
            case DIR_UP:
                m_vRollDir = { 0.f, 0.f, 1.f };
                break;
            case DIR_DOWN:
                m_vRollDir = { 0.f, 0.f, -1.f };
                break;
            case DIR_LEFT:
                m_vRollDir = { -1.f, 0.f, 0.f };
                break;
            case DIR_RIGHT:
                m_vRollDir = { 1.f, 0.f, 0.f };
                break;
            default:
                m_vRollDir = { 0.f, 0.f, 0.f };
                break;
            }

            _uint iRollIdx      = Get_AnimationIndex();
            _uint iFrameCount   = m_pTexCom->Get_FrameCount(iRollIdx);
            _float fSpeed       = m_pTexCom->Get_Speed();

            if (fSpeed > 0.f)
                m_fRollDuration = static_cast<_float>(iFrameCount) / fSpeed;
            else
                m_fRollDuration = 0.5f;

            m_pTexCom->Set_Loop(false);

            _uint iIdx = Get_AnimationIndex();
            m_pTexCom->Set_Texture(iIdx, 0);

            return;
        }
    }

    if (bMoving)
    {
        m_eState = WALK;

        D3DXVec3Normalize(&vDir, &vDir);
        m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 100.f);
    }
    else
    {
        m_eState = IDLE;
    }
}

_int CPlayer::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);

    if (m_eState == ROLL)
    {
        m_fRollTime += fTimeDelta;

        _vec3 vMove = m_vRollDir;
        if (vMove.x != 0.f || vMove.y != 0.f || vMove.z != 0.f)
        {
            D3DXVec3Normalize(&vMove, &vMove);
            m_pTransformCom->Move_Pos(&vMove, fTimeDelta, 100.f);
        }

        if (m_fRollTime >= m_fRollDuration)
        {
            m_eState = IDLE;
            m_pTexCom->Set_Loop(true);

            m_pTexCom->Set_Speed(8.5f);
        }
    }
    else
    {
        Key_Input(fTimeDelta);
    }

    if (m_ePrevState != m_eState || m_ePrevDir != m_eDir)
    {
        _uint iAnim  = Get_AnimationIndex();
        m_pTexCom->Set_Texture(iAnim, 0);

        m_ePrevState = m_eState;
        m_ePrevDir   = m_eDir;
    }

    return iExit;
}

void CPlayer::LateUpdate_GameObject(const _float fTimeDelta)
{
    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);
    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
}

void CPlayer::Render_GameObject()
{
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    _uint curTex   = m_pTexCom->Get_CurTex();
    _uint curFrame = m_pTexCom->Get_CurFrame();
    m_pTexCom->Set_Texture(curTex, curFrame);

    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    m_pBufferCom->Render_Buffer();

    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CPlayer* pInstance = new CPlayer(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        MSG_BOX("CPlayer Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CPlayer::Free()
{
    Engine::CRenderObject::Free();
}
