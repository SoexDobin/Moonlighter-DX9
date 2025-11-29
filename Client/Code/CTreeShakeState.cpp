#include "pch.h"
#include "CTreeShakeState.h"
#include "CTreeMob.h"
#include "CTreeStateMachine.h"
#include "CTreeProjectile.h"

#include "CLayerHelper.h"

CTreeShakeState::CTreeShakeState(CTreeMob* pOwner, CTreeStateMachine* pStateMachine)
    : CMonsterState(),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine)
    , m_iCurShootCnt(0), m_iLimitShootCnt(0), m_fLastShootTime(0.f), m_fLimitShootTime(0.f)
    , m_eAtkType(ATK_TYPE::ATK_END)
{
    ZeroMemory(&m_tAtk1Dmg, sizeof(m_tAtk1Dmg));
    ZeroMemory(&m_tAtk2Dmg, sizeof(m_tAtk2Dmg));
}

CTreeShakeState::CTreeShakeState(LPDIRECT3DDEVICE9 pGraphicDev, CTreeMob* pOwner, CTreeStateMachine* pStateMachine)
    : CMonsterState(pGraphicDev),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine)
    , m_iCurShootCnt(0), m_iLimitShootCnt(0), m_fLastShootTime(0.f), m_fLimitShootTime(0.f)
    , m_eAtkType(ATK_TYPE::ATK_END)
{
    ZeroMemory(&m_tAtk1Dmg, sizeof(m_tAtk1Dmg));
    ZeroMemory(&m_tAtk2Dmg, sizeof(m_tAtk2Dmg));
}

CTreeShakeState::~CTreeShakeState()
{
}

void CTreeShakeState::Enter()
{
    CMonsterState::Enter();

    m_bCanTransit = false;
}

void CTreeShakeState::Exit()
{
    CMonsterState::Exit();
}

HRESULT CTreeShakeState::Ready_State()
{
    m_dwCurStateKey = CTreeMob::TREE_STATE::ATK_SHAKE;

    Swap_AtkType(ATK_TYPE::ATK1);

    m_pOwnerTransformCom = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Transform_Com"));

    Ready_Combat();

    return S_OK;
}

_uint CTreeShakeState::Update_State(const _float& fTimeDelta)
{
    CMonsterState::Detect_Player();

    if (ATK_TYPE::ATK1 == m_eAtkType)
    {
        // 공격 1
        Shoot_Projectile1(fTimeDelta);
    }
    else if (ATK_TYPE::ATK2 == m_eAtkType)
    {
        // 공격 2
        Shoot_Projectile2(fTimeDelta);
    }
    else if (ATK_TYPE::ATKCOMB == m_eAtkType)
    {
        // 공격 1 + 2
        Shoot_ProjectileComb(fTimeDelta);
    }

    return 0;
}

void CTreeShakeState::LateUpdate_State(const _float& fTimeDelta)
{
    CMonsterState::Check_ShouldTransit(fTimeDelta);

    if (m_bCanTransit)
        Determine_NextState();
}

void CTreeShakeState::Render_State()
{
    m_pOwner->Set_CurAnimKey(CTreeMob::TREE_STATE::IDLE);
}

void CTreeShakeState::Determine_NextState()
{
    m_pStateMachine->Change_State(CTreeMob::TREE_STATE::IDLE);
}

void CTreeShakeState::Ready_Combat()
{
    // ATK1
    {
        // 투사체 정보 설정 
        m_tProjectile1.wsVecTexture = { L"Tree_Projectile1_Idle", L"Tree_Projectile1_Hit" };
        m_tProjectile1.fTextureSpeed = 10;

        m_tProjectile1.fSpeed = 30.f;
        m_tProjectile1.fScale = 0.5f;
        m_tProjectile1.eColType = COL_TYPE::SPHERE_COL;
        m_tProjectile1.dwCurFlag = OBJECT_ID::MONSTER_ATK;
        m_tProjectile1.dwHitTargetFlag = OBJECT_ID::PLAYER;
        m_tProjectile1.dwDestroyOnHitFlag
            = OBJECT_ID::PLAYER_ATK | OBJECT_ID::INTERACTABLE | OBJECT_ID::ENVIRONMENT;

        // 데미지 정보 설정 
        m_tAtk1Dmg.pAttacker = m_pOwner;
        m_tAtk1Dmg.dwHitTargetFlag = m_tProjectile1.dwHitTargetFlag;
        m_tAtk1Dmg.fAmount = 60.f;
        m_tAtk1Dmg.bCanParry = m_tAtk1Dmg.bShouldKnockback = false;
        m_tAtk1Dmg.eApplyTiming = Engine::ENTER_COL;
        m_tAtk1Dmg.vDirKnockback = { 0.f, 0.f, 0.f };

        m_tProjectile1.tDamageInfo = m_tAtk1Dmg;
    }

    // ATK2
    {
        // 투사체 정보 설정 
        m_tProjectile2.wsVecTexture = { L"Tree_Projectile2_Idle", L"Tree_Projectile2_Hit" };
        m_tProjectile2.fTextureSpeed = 10;
                               
        m_tProjectile2.fSpeed = 30.f;
        m_tProjectile2.fScale = 0.5f;
        m_tProjectile2.eColType = COL_TYPE::SPHERE_COL;
        m_tProjectile2.dwCurFlag = OBJECT_ID::MONSTER_ATK;
        m_tProjectile2.dwHitTargetFlag = OBJECT_ID::PLAYER;
        m_tProjectile2.dwDestroyOnHitFlag
            = OBJECT_ID::PLAYER_ATK | OBJECT_ID::INTERACTABLE | OBJECT_ID::ENVIRONMENT;

        // 데미지 정보 설정 
        m_tAtk2Dmg.pAttacker = m_pOwner;
        m_tAtk2Dmg.dwHitTargetFlag = m_tProjectile2.dwHitTargetFlag;
        m_tAtk2Dmg.fAmount = 10.f;
        m_tAtk2Dmg.bCanParry = m_tAtk1Dmg.bShouldKnockback = false;
        m_tAtk2Dmg.eApplyTiming = Engine::ENTER_COL;
        m_tAtk2Dmg.vDirKnockback = { 0.f, 0.f, 0.f };

        m_tProjectile2.tDamageInfo = m_tAtk2Dmg;
    }

    // 8 방향 미리 계산해서 넣기
    _vec3 vStartDir = { 1.f, -0.2f, 01.f };
    _float fAngle = 0.f;
    for (_uint dwIndex = 0; dwIndex < 12; dwIndex++)
    {
        _matrix matRotY;
        D3DXMatrixRotationY(&matRotY, D3DXToRadian(30.f * dwIndex));

        _vec3 vComputedDir;
        D3DXVec3TransformNormal(&vComputedDir, &vStartDir, &matRotY);

        D3DXVec3Normalize(&vComputedDir, &vComputedDir);

        m_vecPreComputedDir[dwIndex] = vComputedDir;
    }
}

void CTreeShakeState::Shoot_Projectile1(const _float& fTimeDelta)
{
    m_fLastShootTime += fTimeDelta;

    if (m_fLastShootTime > m_fLimitShootTime)
    {
        // Shoot
        CTreeProjectile* pProjectile = CTreeProjectile::Create(m_pGraphicDev, m_tProjectile1);
        pProjectile->Set_SpawnPos(m_pOwnerTransformCom->Get_Pos());
        pProjectile->Set_ShootDir(m_vDirToPlayer);
        pProjectile->Set_Shooting();

        if (FAILED(CLayerHelper::GetInstance()->Get_Layer(L"GameLogic_Layer")->Add_GameObject(L"TreeProjectile1", pProjectile)))
            printf("TreeMob failed to create projectile!!");

        // 바로 다음 발사 위한 초기화
        m_fLastShootTime = 0.f;
        m_iCurShootCnt++;

        if (m_iCurShootCnt >= m_iLimitShootCnt)
        {
            Swap_AtkType(ATK_TYPE::ATK2);
        }
    }
}

void CTreeShakeState::Shoot_Projectile2(const _float& fTimeDelta)
{
    m_fLastShootTime += fTimeDelta;

    if (m_fLastShootTime > m_fLimitShootTime)
    {
        // Shoot
        _vec3 vStartDir = m_vDirToPlayer;

        const float fAngleOffsetDeg = 20.f;

        for (int i = -1; i <= 1; ++i)
        {
            _matrix matRotY;
            float angle = D3DXToRadian(fAngleOffsetDeg * i);

            D3DXMatrixRotationY(&matRotY, angle);

            _vec3 vComputedDir;
            D3DXVec3TransformNormal(&vComputedDir, &vStartDir, &matRotY);
            D3DXVec3Normalize(&vComputedDir, &vComputedDir);

            CTreeProjectile* pProjectile =
                CTreeProjectile::Create(m_pGraphicDev, m_tProjectile2);

            pProjectile->Set_SpawnPos(m_pOwnerTransformCom->Get_Pos());
            pProjectile->Set_ShootDir(vComputedDir);
            pProjectile->Set_Shooting();

            if (FAILED(CLayerHelper::GetInstance()->Get_Layer(L"GameLogic_Layer")
                ->Add_GameObject(L"TreeProjectile1", pProjectile)))
            {
                printf("TreeMob failed to create projectile!!\n");
            }
        }

        // 바로 다음 발사 위한 초기화
        m_fLastShootTime = 0.f;
        m_iCurShootCnt++;

        if (m_iCurShootCnt >= m_iLimitShootCnt)
        {
            Swap_AtkType(ATK_TYPE::ATKCOMB);
        }
    }
}

void CTreeShakeState::Shoot_ProjectileComb(const _float& fTimeDelta)
{
    m_fLastShootTime += fTimeDelta;

    if (m_fLastShootTime > m_fLimitShootTime)
    {
        // Shoot
        CTreeProjectile* pProjectile = nullptr;
        if (m_iCurShootCnt % 2)
        {
            pProjectile = CTreeProjectile::Create(m_pGraphicDev, m_tProjectile1);
        }
        else
        {
            pProjectile = CTreeProjectile::Create(m_pGraphicDev, m_tProjectile2);
        }

        pProjectile->Set_SpawnPos(m_pOwnerTransformCom->Get_Pos());
        pProjectile->Set_ShootDir(m_vecPreComputedDir[(m_iCurShootCnt % 12)]);
        pProjectile->Set_Shooting();

        if (FAILED(CLayerHelper::GetInstance()->Get_Layer(L"GameLogic_Layer")->Add_GameObject(L"TreeProjectile1", pProjectile)))
            printf("TreeMob failed to create projectile!!");

        // 바로 다음 발사 위한 초기화
        m_fLastShootTime = 0.f;
        m_iCurShootCnt++;

        if (m_iCurShootCnt >= m_iLimitShootCnt)
        {
            Swap_AtkType(ATK_TYPE::ATK1);
        }
    }
}

void CTreeShakeState::Swap_AtkType(ATK_TYPE eType)
{
    // 값 초기화 
    m_iCurShootCnt = 0;
    m_fLastShootTime = 0.f;

    // 공격이 끝난 뒤에 Idle로 전환되도록 한다 
    m_bCanTransit = true;

    switch (eType)
    {
    case CTreeShakeState::ATK1:
    {
        m_fLimitShootTime = 0.2f;
        m_iLimitShootCnt = 5;
    }
        break;
    case CTreeShakeState::ATK2:
    {
        m_fLimitShootTime = 1.f;
        m_iLimitShootCnt = 3;
    }
        break;
    case CTreeShakeState::ATKCOMB:
    {
        m_fLimitShootTime = 0.06f;
        m_iLimitShootCnt = 12;
    }
        break;
    }

    m_eAtkType = eType;
}

CTreeShakeState* CTreeShakeState::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTreeMob* pOwner, CTreeStateMachine* pStateMachine)
{
    CTreeShakeState* pShake = new CTreeShakeState(pGraphicDev, pOwner, pStateMachine);

    if (FAILED(pShake->Ready_State()))
    {
        MSG_BOX("CTreeShakeState Create Failed!");
        return nullptr;
    }

    return pShake;
}

void CTreeShakeState::Free()
{
}
