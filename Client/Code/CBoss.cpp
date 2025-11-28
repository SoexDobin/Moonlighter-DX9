#pragma region INCLUDE
#include "pch.h"
#include "CBoss.h"
#include "CRenderer.h"
#include "CPrototypeManager.h"

#include "CMonsterState.h"
#include "CBossStateMachine.h"

#include "CCombatStats.h"
#pragma endregion

#pragma region TEST INCLUDE
#include "CCollisionManager.h"
#include "CManagement.h"
#pragma endregion


CBoss::CBoss(LPDIRECT3DDEVICE9 pGraphicDev)
    : CMonster(pGraphicDev), m_pDynamicTexCom(nullptr), m_dwCurStateKey(BOSS_STATE::B_END), m_dwAnimKey(BOSS_STATE::B_END),
    m_pStateMachine(nullptr), m_pCurState(nullptr)
{
}

CBoss::~CBoss()
{
}

HRESULT CBoss::Ready_GameObject()
{
#pragma region Ready_Component
    // Texture, Transform 컴포넌트 생성 
    if (FAILED(Engine::CRenderObject::Ready_GameObject()))
        return E_FAIL;

    CComponent* pCom(nullptr);

    pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
    if (pCom->Get_ComponentType() != TEXTURE)
        return E_FAIL;

    if (m_pDynamicTexCom = static_cast<CTexture*>(pCom))
    {
        m_umComponent[ID_DYNAMIC].insert(pair<wstring, CComponent*>(L"Texture_Com", m_pDynamicTexCom));
    }
#pragma endregion

    m_pStateMachine = CBossStateMachine::Create(this);

    Ready_EntityComponent();
    Ready_Animation();


    // after all components are set up
    Configure_Component();


#pragma region Debbuging
    Add_EditorFiled();
#pragma endregion

    m_iObjectID = OBJECT_ID::MONSTER;

    return S_OK;
}

CGameObject* pSlime = nullptr;
CHitRectBox* pHitBox = nullptr;

_int CBoss::Update_GameObject(const _float fTimeDelta)
{
    m_pCurState->Update_State(fTimeDelta);

    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);

    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CBoss::LateUpdate_GameObject(const _float fTimeDelta)
{
    m_pCurState->LateUpdate_State(fTimeDelta);

    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CBoss::Render_GameObject()
{
    m_pCurState->Render_State();

#pragma region Base Implementation
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    _uint dwCurFrame = m_pDynamicTexCom->Get_CurFrame();
    m_pDynamicTexCom->Set_Texture(m_dwAnimKey, dwCurFrame);
    m_pBufferCom->Render_Buffer();

    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
#pragma endregion

#pragma region Debugging
    Display_CurrentState();
#pragma endregion

}

void CBoss::On_Collision(const Collision& tCollision)
{
    //================= 충돌 테스트 =========================
    // 충돌 상대가 슬라임이라 가정 : 슬라임 -> 보스에게 몸박 공격

    m_iObjectID = PLAYER;

    if (COL_TYPE::RECT_COL == tCollision.pColSource->Get_ColType()
        && nullptr != dynamic_cast<CHitRectBox*>(tCollision.pColSource))
    {
        m_pCombatComponent->Take_Damage(
            static_cast<CHitRectBox*>(tCollision.pColSource),                                   // 충돌 정보 전달
            static_cast<CHitRectBox*>(tCollision.pColSource)->Get_Damage());      // 데미지 정보 전달
    }
    else if (COL_TYPE::SPHERE_COL == tCollision.pColSource->Get_ColType()
        && nullptr != dynamic_cast<CHitSphereBox*>(tCollision.pColSource))
    {
        m_pCombatComponent->Take_Damage(
            static_cast<CHitSphereBox*>(tCollision.pColSource),                                   // 충돌 정보 전달
            static_cast<CHitSphereBox*>(tCollision.pColSource)->Get_Damage());      // 데미지 정보 전달
    }

    // ====================================================
}

HRESULT CBoss::Ready_Animation()
{
    m_pDynamicTexCom->Ready_Texture(L"Boss_Awake");                //BOSS_STATE::AWAKE
    m_pDynamicTexCom->Ready_Texture(L"Boss_Idle");                    //BOSS_STATE::IDLE
    m_pDynamicTexCom->Ready_Texture(L"Boss_Jump");                  //BOSS_STATE::JUMP
    m_pDynamicTexCom->Ready_Texture(L"Boss_Throwing");          //BOSS_STATE::ATK_THROW
    m_pDynamicTexCom->Ready_Texture(L"Boss_Roots");                //BOSS_STATE::ATK_ROOT
    m_pDynamicTexCom->Ready_Texture(L"Boss_Shake");                //BOSS_STATE::ATK_SHAKE
    m_pDynamicTexCom->Ready_Texture(L"Boss_Death");                //BOSS_STATE::DEAD

    {
        // TODO : 무기 스프라이트 : 다른 데로 옮겨야 함
        //m_pDynamicTexCom->Ready_Texture(L"Boss_Cutting_Idle");
        //m_pDynamicTexCom->Ready_Texture(L"Boss_Cutting_Growing");
        //m_pDynamicTexCom->Ready_Texture(L"Boss_Cutting_Shot");
        //m_pDynamicTexCom->Ready_Texture(L"Root1");
        //m_pDynamicTexCom->Ready_Texture(L"Root2");
    }

    // Configure boss animation values
    m_pDynamicTexCom->Set_Speed(10.f);

    return S_OK;
}


void CBoss::Set_CurStateKey(_uint dwStateKey, CMonsterState* pCurState)
{
    if (dwStateKey == m_dwCurStateKey)
        return;

    m_dwCurStateKey = dwStateKey;
    m_pCurState = pCurState;
    m_pDynamicTexCom->Set_Texture(dwStateKey);
}

void CBoss::Ready_EntityComponent()
{
    // Ready Collider
    m_pColCom = Add_Component<CRectCollider>(ID_DYNAMIC, L"Collider_Com", RECT_COLLIDER);

    // Ready Combat Stats                
    m_pCombatStats = CCombatStats::Create(this);
    m_pCombatComponent = CCombatComponent::Create(this, m_pCombatStats);
}

void CBoss::Configure_Component()
{
    // Set Basic Components 
    m_pTransformCom->Set_Pos(20.f, 5.f, 10.f);
    static_cast<CRectCollider*>(m_pColCom)->Set_Dimension({ 10.f, 10.f, 5.f });

    // Set Combat Stats
    m_pCombatStats->Set_HealthStat(CStatComponent::Create(this, 100.f, 0.f, 100.f));
    m_pCombatStats->Set_AttackStat(CStatComponent::Create(this, 10.f, 10.f, 30.f));
    m_pCombatStats->Set_DefenseStat(CStatComponent::Create(this, 10.f, 10.f, 30.f));
    m_pCombatStats->Set_SpeedStat(CStatComponent::Create(this, 5.f, 1.f, 10.f));

    // Set Combat Events
    m_pCombatStats->Get_HealthStat()->m_OnValueMin = [&]() {m_pStateMachine->Change_State(CBoss::DEAD); };
}

CBoss* CBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBoss* pInstance = new CBoss(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        MSG_BOX("CBoss Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CBoss::Free()
{
    Engine::CGameObject::Free();

    m_pStateMachine->Release();
    m_pCombatComponent->Release();
}

void CBoss::Add_EditorFiled()
{
    CGameObject::Add_EditorField("State", DATATYPE::DATA_TCHAR, &m_szState);
    CGameObject::Add_EditorField("HP", DATATYPE::DATA_FLOAT, &debug_fHP);
    CGameObject::Add_EditorField("Speed", DATATYPE::DATA_FLOAT, &debug_fSpeed);
}

void CBoss::Display_CurrentState()
{
#pragma region STATE
    switch ((BOSS_STATE)m_dwCurStateKey)
    {
    case BOSS_STATE::AWAKE:
    {
        wcscpy_s(m_szState, L"AWAKE\n");
    }
    break;

    case BOSS_STATE::IDLE:
    {
        wcscpy_s(m_szState, L"IDLE\n");
    }
    break;

    case BOSS_STATE::WALK:
    {
        wcscpy_s(m_szState, L"WALK\n");
    }
    break;

    case BOSS_STATE::ATK_JUMP:
    {
        wcscpy_s(m_szState, L"ATK_JUMP\n");
    }
    break;

    case BOSS_STATE::ATK_THROW:
    {
        wcscpy_s(m_szState, L"ATK_THROW\n");
    }
    break;

    case BOSS_STATE::ATK_ROOT:
    {
        wcscpy_s(m_szState, L"ATK_ROOT\n");
    }
    break;

    case BOSS_STATE::ATK_SHAKE:
    {
        wcscpy_s(m_szState, L"ATK_THROW\n");
    }
    break;

    case BOSS_STATE::DEAD:
    {
        wcscpy_s(m_szState, L"DEAD\n");
    }
    break;
    default:
    {
        wcscpy_s(m_szState, L"NONE\n");

    }
    break;
    }
#pragma endregion


#pragma region COMBAT TEST
    debug_fHP = m_pCombatStats->Get_HealthStat()->Get_CurValue();
    debug_fSpeed = m_pCombatStats->Get_SpeedStat()->Get_CurValue();

    //if (GetAsyncKeyState('P') & 0x01)
    //{
    //    m_pCombatStats->Get_HealthStat()->Decrease(30.f);
    //}
    //if (GetAsyncKeyState('O') & 0x01)
    //{
    //    m_pCombatStats->Get_SpeedStat()->Increase(30.f);
    //}
#pragma endregion


}
