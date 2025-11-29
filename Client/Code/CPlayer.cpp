#include "pch.h"
#include "CPlayer.h"

#include "CRenderer.h"
#include "CPrototypeManager.h"
#include "CDInputManager.h"
#include "CTexture.h"
#include "CTransform.h"

#include "CHitRectBox.h"
#include "CHitSphereBox.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pTexCom(nullptr), m_eWeapon(NONE), m_eState(IDLE), m_eDir(DIR_DOWN), m_ePrevState(STATE_END), m_ePrevDir(DIR_END), m_fRollTime(0.f), m_fRollDuration(0.5f), m_vActionDir{ 0.f, 0.f, 0.f }, m_iPrevFrame(0), m_iCurFrame(0), m_bBowBackStep(false), m_fBowBackStepTimer(0.f), m_iComboStep(0), m_bComboCheck(false), m_bFPrev(false)
{
}

CPlayer::CPlayer(const CPlayer& rhs)
    : CRenderObject(rhs), m_pTexCom(nullptr), m_eWeapon(rhs.m_eWeapon), m_eState(rhs.m_eState), m_eDir(rhs.m_eDir), m_ePrevState(STATE_END), m_ePrevDir(DIR_END), m_fRollTime(0.f), m_fRollDuration(rhs.m_fRollDuration), m_vActionDir{ 0.f, 0.f, 0.f }, m_iPrevFrame(0), m_iCurFrame(0), m_bBowBackStep(false), m_fBowBackStepTimer(0.f), m_iComboStep(0), m_bComboCheck(false), m_bFPrev(false)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
    if (FAILED(CRenderObject::Ready_GameObject()))
        return E_FAIL;

    if (FAILED(Ready_Animation()))
        return E_FAIL;

    m_pTransformCom->Set_Scale(8.f, 8.f, 1.f);
    m_pTransformCom->Set_Pos(10.f, 5.f, 0.f);

    m_pColCom = Add_Component<CRectCollider>(ID_DYNAMIC, L"Collider_Com", RECT_COLLIDER);

    m_eWeapon       = NONE;
    m_eState        = IDLE;
    m_eDir          = DIR_DOWN;
    m_ePrevState    = STATE_END;
    m_ePrevDir      = DIR_END;
    m_fRollTime     = 0.f;
    m_iPrevFrame    = 0;
    m_iCurFrame     = 0;

   m_iObjectID = OBJECT_ID::PLAYER;

    return S_OK;
}

HRESULT CPlayer::Ready_Animation()
{
    m_pTexCom = Add_Component<CTexture>(ID_DYNAMIC, L"Player_TexCom", TEXTURE);
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

    // Spear ComboAttack
    m_pTexCom->Ready_Texture(L"Player_Spear_Combo_Down");
    m_pTexCom->Ready_Texture(L"Player_Spear_Combo_Up");
    m_pTexCom->Ready_Texture(L"Player_Spear_Combo_Left");
    m_pTexCom->Ready_Texture(L"Player_Spear_Combo_Right");

    // Spear ChargeAttack
    m_pTexCom->Ready_Texture(L"Player_Spear_Charge_Down");
    m_pTexCom->Ready_Texture(L"Player_Spear_Charge_Up");
    m_pTexCom->Ready_Texture(L"Player_Spear_Charge_Left");
    m_pTexCom->Ready_Texture(L"Player_Spear_Charge_Right");

    // Bow NormalAttack
    m_pTexCom->Ready_Texture(L"Player_Bow_Normal_Down");
    m_pTexCom->Ready_Texture(L"Player_Bow_Normal_Up");
    m_pTexCom->Ready_Texture(L"Player_Bow_Normal_Left");
    m_pTexCom->Ready_Texture(L"Player_Bow_Normal_Right");

    // Bow ChargeAttack
    m_pTexCom->Ready_Texture(L"Player_Bow_Charge_Down");
    m_pTexCom->Ready_Texture(L"Player_Bow_Charge_Up");
    m_pTexCom->Ready_Texture(L"Player_Bow_Charge_Left");
    m_pTexCom->Ready_Texture(L"Player_Bow_Charge_Right");

    m_pTexCom->Set_Texture(IDLE);
    m_umComponent[ID_DYNAMIC].insert(pair<wstring, CComponent*>(L"Player_TexCom", m_pTexCom));

    return S_OK;
}

_uint CPlayer::Get_AnimationIndex()
{
    _uint iBase = 0;

    switch (m_eState)
    {
    case IDLE:          iBase = 0; break;
    case WALK:          iBase = 4; break;
    case ROLL:          iBase = 8; break;
    case SPEAR_COMBO:   iBase = 12; break;
    case SPEAR_CHARGE:  iBase = 16; break;
    case BOW_NORMAL:    iBase = 20; break;
    case BOW_CHARGE:    iBase = 24; break;
    default:            iBase = 0; break;
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
    auto* pInput = CDInputManager::GetInstance();

    const bool bFNow = (pInput->Get_DIKeyState(DIK_F) & 0x80) != 0;
    const bool bFPressed = (!m_bFPrev && bFNow);

    if (m_eState == SPEAR_COMBO)
    {
        if (bFPressed)
            m_bComboCheck = true;

        m_bFPrev = bFNow;
        return;
    }

    if (IsBusy())
    {
        m_bFPrev = bFNow;
        return;
    }

    _vec3 vDir   = { 0.f, 0.f, 0.f };
    bool bMoving = false;

    //bool bAttack = false;
    bool bCharge = false;

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

    if (pInput->Get_DIKeyState(DIK_K) & 0x80)
    {
        vDir.y += 1.f;
        bMoving = true;
    }
    if (pInput->Get_DIKeyState(DIK_L) & 0x80)
    {
        vDir.y -= 1.f;
        bMoving = true;
    }

    if (pInput->Get_DIKeyPressed(DIK_F) & 0x80)
    {
        if (bFPressed)
        {
            if (m_eWeapon == SPEAR)
            {
                m_eState = SPEAR_COMBO;
                SpearComboAttackStart(1);

                m_bFPrev = bFNow;
                return;
            }
            else if (m_eWeapon == BOW)
            {
                m_eState = BOW_NORMAL;

                Save_Dir();
                m_ePrevState = STATE_END;
                m_pTexCom->Set_Loop(false);
                m_pTexCom->Set_Speed(12.f);

                _uint iIdx = Get_AnimationIndex();
                m_pTexCom->Set_Texture(iIdx, 0);
            }

                return;
        }
    }

    if (pInput->Get_DIKeyPressed(DIK_G) & 0x80)
    {
        bCharge = true;
    }

    if (pInput->Get_DIKeyPressed(DIK_LSHIFT) & 0x80)
    {
        if (m_eWeapon == NONE)
            m_eWeapon = SPEAR;
        else if (m_eWeapon == SPEAR)
            m_eWeapon = BOW;
        else
            m_eWeapon = NONE;

        m_ePrevState = STATE_END;

        return;
    }

    if (pInput->Get_DIKeyPressed(DIK_SPACE) & 0x80)
    {
        if (m_eState != ROLL)
        {
            m_eState  = ROLL;
            m_fRollTime = 0.f;

            m_pTexCom->Set_Speed(15.f);

            

            Save_Dir();

            _uint iRollIdx    = Get_AnimationIndex();
            _float fSpeed     = m_pTexCom->Get_Speed();
            _uint iFrameCount = m_pTexCom->Get_FrameCount(iRollIdx);
            
            if (fSpeed > 0.f)
                m_fRollDuration = (_float)(iFrameCount) / fSpeed;
            else
                m_fRollDuration = 0.5f;
            
            m_pTexCom->Set_Loop(false);
            
            m_pTexCom->Set_Texture(iRollIdx, 0);
            
            return;
        }
    }

    if (bCharge)
    {
        if (m_eWeapon == SPEAR)
            m_eState = SPEAR_CHARGE;
        else if (m_eWeapon == BOW)
            m_eState = BOW_CHARGE;
        else
            bCharge = false;

        Save_Dir();

        m_ePrevState = STATE_END;

        m_pTexCom->Set_Loop(false);
        m_pTexCom->Set_Speed(12.f);

        _uint iIdx = Get_AnimationIndex();

        m_pTexCom->Set_Texture(iIdx, 0);

        return;
    }
    else if (bMoving)
    {
        m_eState = WALK;

        D3DXVec3Normalize(&vDir, &vDir);
        m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 10.f);
    }
    else
    {
        m_eState = IDLE;
    }

    m_bFPrev = bFNow;
}

_int CPlayer::Update_GameObject(const _float fTimeDelta)
{
    _int iExit          = Engine::CRenderObject::Update_GameObject(fTimeDelta);

    m_iPrevFrame        = m_iCurFrame;
    m_iCurFrame         = m_pTexCom->Get_CurFrame();
    bool bFrameChanged  = (m_iPrevFrame != m_iCurFrame);
    bool bKeyGDown = (CDInputManager::GetInstance()->Get_DIKeyState(DIK_G) & 0x80);
    Key_Input(fTimeDelta);

    if (m_eState == SPEAR_COMBO)
    {
        switch (m_iComboStep)
        {
        case 1:
            if (m_iCurFrame < 6)
            {
                if (m_bComboCheck)
                {
                    m_bComboCheck = false;
                    SpearComboAttackStart(2);
                    return iExit;
                }
            }
            else
            {
                if (m_bComboCheck)
                {
                    m_bComboCheck = false;
                    SpearComboAttackStart(2);
                    return iExit;
                }

                m_pTexCom->Stop_Anim();

                if (m_iCurFrame >= 6)
                {
                    SpearComboAttackEnd();
                    return iExit;
                }
            }
            return iExit;

        case 2:
            if (m_iCurFrame < 13)
            {
                if (m_bComboCheck)
                {
                    m_bComboCheck = false;
                    SpearComboAttackStart(3);
                    return iExit;
                }
            }
            else
            {
                if (m_bComboCheck)
                {
                    m_bComboCheck = false;
                    SpearComboAttackStart(3);
                    return iExit;
                }

                m_pTexCom->Stop_Anim();

                SpearComboAttackEnd();
                return iExit;
            }
            return iExit;

        case 3:
            if (m_iCurFrame >= 22)
            {
                m_pTexCom->Stop_Anim();
                SpearComboAttackEnd();
                return iExit;
            }
            return iExit;
        }

        return iExit;
    }
    else if (m_eState == SPEAR_CHARGE)
    {
        if (m_iCurFrame <= 10)
        {
            if (!bKeyGDown)
            {
                m_eState = IDLE;

                m_pTexCom->Set_Loop(true);
                m_pTexCom->Set_Speed(12.f);

                return iExit;
            }
        }

        if (m_iCurFrame >= 11 && m_iCurFrame <= 22)
        {
            _uint iIdx = Get_AnimationIndex();
            if (bKeyGDown)
            {
                if (bFrameChanged && m_iCurFrame == 22)
                {
                    m_pTexCom->Set_Texture(iIdx, 12);
                }
            }
            else
            {
                m_pTexCom->Set_Texture(iIdx, 23);
            }

            return iExit;
        }

        if (bFrameChanged && m_iCurFrame == 23)
        {
            SpearCharge_FirstHit();
        }

        if (m_iCurFrame >= 23 && m_iCurFrame <= 45)
        {
            _vec3 vMove = m_vActionDir;
            D3DXVec3Normalize(&vMove, &vMove);

            _float fDashSpeed = 4.f;
            m_pTransformCom->Move_Pos(&vMove, fTimeDelta, fDashSpeed);

            if (m_iCurFrame == 45)
                return iExit;
        }

        if (bFrameChanged && m_iCurFrame == 46)
        {
            SpearCharge_SecondHit();
        }

        if (bFrameChanged && m_iCurFrame == 53)
        {
            m_eState = IDLE;

            m_pTexCom->Set_Loop(true);
            m_pTexCom->Set_Speed(8.5f);

            bKeyGDown = false;

            return iExit;
        }

        return iExit;
    }
    else if (m_eState == BOW_NORMAL)
    {
        if (bFrameChanged && m_iCurFrame == 5)
        {
            BowNormal_BackStep();
            BowNormal_Fire();
        }

        if (m_bBowBackStep)
        {
            m_fBowBackStepTimer += fTimeDelta;

            _vec3 vMove = m_vActionDir * -1.f;

            D3DXVec3Normalize(&vMove, &vMove);

            _float fSpeed = 8.f;
            m_pTransformCom->Move_Pos(&vMove, fTimeDelta, fSpeed);

            if (m_fBowBackStepTimer >= 0.1f)
            {
                m_bBowBackStep = false;
            }
        }

        if (bFrameChanged && m_iCurFrame == 10)
        {
            m_eState = IDLE;

            m_pTexCom->Set_Loop(true);
            m_pTexCom->Set_Speed(8.5f);

            return iExit;
        }

        return iExit;
    }
    else if (m_eState == BOW_CHARGE)
    {
        m_bBowChargeFired   = false;

        if (m_iCurFrame <= 11)
        {
            if (!bKeyGDown)
            {
                m_eState = IDLE;

                m_pTexCom->Set_Loop(true);
                m_pTexCom->Set_Speed(10.f);

                m_bBowChargeFired = false;
                return iExit;
            }
        }
        if (m_iCurFrame >= 12 && m_iCurFrame <= 17)
        {
            if (bKeyGDown)
            {
                if (bFrameChanged && m_iCurFrame == 17)
                {
                    _uint animIdx = Get_AnimationIndex();
                    m_pTexCom->Set_Texture(animIdx, 12);
                }
            }
            else
            {
                _uint animIdx = Get_AnimationIndex();
                m_pTexCom->Set_Texture(animIdx, 18);
            }

            return iExit;
        }
        if (bFrameChanged && m_iCurFrame == 18)
        {
            BowCharge_Fire();
            m_bBowChargeFired = true;
        }

        if (bFrameChanged && m_iCurFrame == 26)
        {
            m_eState = IDLE;

            m_pTexCom->Set_Loop(true);
            m_pTexCom->Set_Speed(8.5f);

            bKeyGDown = false;
            m_bBowChargeFired = false;

            return iExit;
        }

        return iExit;
    }
    else if (m_eState == ROLL)
    {
        m_fRollTime += fTimeDelta;

        _vec3 vMove = m_vActionDir;
        if (vMove.x != 0.f || vMove.y != 0.f || vMove.z != 0.f)
        {
            D3DXVec3Normalize(&vMove, &vMove);
            m_pTransformCom->Move_Pos(&vMove, fTimeDelta, 10.f);
        }

        if (m_fRollTime >= m_fRollDuration)
        {
            m_eState = IDLE;
            m_fRollTime = 0.f;

            m_pTexCom->Set_Loop(true);
            m_pTexCom->Set_Speed(8.5f);

            _uint iIdx = Get_AnimationIndex();
            m_pTexCom->Set_Texture(iIdx, 0);
            m_pTexCom->Resume_Anim();
            m_ePrevState = STATE_END;
        }

        return iExit;
    }

    if (m_eState != SPEAR_COMBO)
    {
        if (m_ePrevState != m_eState || m_ePrevDir != m_eDir)
        {
            _uint iIdx = Get_AnimationIndex();
            m_pTexCom->Set_Texture(iIdx, 0);

            m_ePrevState = m_eState;
            m_ePrevDir = m_eDir;
        }
    }

    return iExit;
}

void CPlayer::Save_Dir()
{
    switch (m_eDir)
    {
    case DIR_UP:
        m_vActionDir = { 0.f, 0.f, 1.f };
        break;
    case DIR_DOWN:
        m_vActionDir = { 0.f, 0.f, -1.f };
        break;
    case DIR_LEFT:
        m_vActionDir = { -1.f, 0.f, 0.f };
        break;
    case DIR_RIGHT:
        m_vActionDir = { 1.f, 0.f, 0.f };
        break;
    default:
        m_vActionDir = { 0.f, 0.f, 0.f };
        break;
    }
}

bool CPlayer::IsBusy() const
{
    switch (m_eState)
    {
    case SPEAR_CHARGE:
    case BOW_NORMAL:
    case BOW_CHARGE:

    case ROLL:
        return true;
    }

    return false;
}

void CPlayer::On_Collision(const Collision& tCollision)
{
    // ======================== TEST ============================
    {
        // 몬스터 공격이 플레이어에게 전달되는지 확인
        if (tCollision.pColTarget->Get_ObjectID() & OBJECT_ID::MONSTER_ATK
            || tCollision.pColTarget->Get_ObjectID() & OBJECT_ID::MONSTER) // ATK 이면 무조건 HITBOX 라고 가정
        {
            // RECT
            if (tCollision.pColSource->Get_ColType() == COL_TYPE::RECT_COL      // 콜라이더가 RECT
                && COL_STATE::ENTER_COL == tCollision.eColState)                           // 충돌 ENTER 시에만 데미지 적용
            {
                printf("충돌 상태 : ENTER \n");
                CHitRectBox* pHitBox = dynamic_cast<CHitRectBox*>(tCollision.pColSource);
                if (pHitBox)
                {
                    printf("충돌체 : HITBOX \n");
                    DAMAGE_INFO tCurrentDamage = pHitBox->Get_Damage();
                    printf("Player should take damage : %.2f\n", tCurrentDamage.fAmount);
                }
            }
            // SPHERE
            else if (tCollision.pColSource->Get_ColType() == COL_TYPE::SPHERE_COL   // 콜라이더가 Sphere
                && COL_STATE::ENTER_COL == tCollision.eColState)                                    // 충돌 ENTER 시에만 데미지 적용
            {
                CHitSphereBox* pHitBox = static_cast<CHitSphereBox*>(tCollision.pColSource);
                if (pHitBox)
                {
                    DAMAGE_INFO tCurrentDamage = pHitBox->Get_Damage();
                    printf("Player should take damage : %.2f\n", tCurrentDamage.fAmount);
                }
            }
        }
    }

    //if (tCollision.eColState == EXIT_COL)
    //{
    //    int a = 0;
    //}
    //else if (tCollision.eColState == STAY_COL)
    //{
    //    int a = 0;
    //}
    //else if (tCollision.eColState == ENTER_COL)
    //{
    //    int a = 0;
    //}
}

void CPlayer::LateUpdate_GameObject(const _float fTimeDelta)
{
    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);
    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
}

void CPlayer::Render_GameObject()
{
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
    

    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    m_pTexCom->SetUp_Texture();
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

void CPlayer::BowNormal_BackStep()
{
    m_bBowBackStep = true;
    m_fBowBackStepTimer = 0.f;
}

void CPlayer::BowNormal_Fire()
{
    // 추후 추가 예정
}

void CPlayer::BowCharge_Fire()
{
    // 추후 추가 예정
}

void CPlayer::SpearComboAttackStart(_uint iStep)
{
    m_iComboStep = iStep;
    m_bComboCheck = false;

    m_pTexCom->Set_Loop(false);
    m_pTexCom->Set_Speed(8.5f);

    _uint iStartFrame = (iStep == 1 ? 0 : iStep == 2 ? 7 : 14);

    _uint iIdx = Get_AnimationIndex();
    m_pTexCom->Set_Texture(iIdx, iStartFrame);
}

void CPlayer::SpearComboAttackEnd()
{
    m_eState = IDLE;
    m_iComboStep = 0;
    m_bComboCheck = false;

    m_bFPrev = false;

    _uint iIdx = Get_AnimationIndex();
    m_pTexCom->Set_Loop(true);
    m_pTexCom->Resume_Anim();
    m_pTexCom->Set_Texture(iIdx, 0);

    m_ePrevState = STATE_END;
}

void CPlayer::SpearCombo_FirstHit()
{
    // 추후 추가 예정
}

void CPlayer::SpearCombo_SecondHit()
{
    // 추후 추가 예정
}

void CPlayer::SpearCombo_ThirdHit()
{
    // 추후 추가 예정
}

void CPlayer::SpearCharge_FirstHit()
{
    // 추후 추가 예정
}

void CPlayer::SpearCharge_SecondHit()
{
    // 추후 추가 예정
}
