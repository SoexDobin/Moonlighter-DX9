#pragma region INCLUDE
#include "pch.h"
#include "CSlimeMob.h"
#include "CRenderer.h"
#include "CPrototypeManager.h"
#include "CManagement.h"

#include "CPlayer.h"
#include "CMonsterState.h"
#include "CSlimeStateMachine.h"

#include "CHitRectBox.h"
#pragma endregion


CSlimeMob::CSlimeMob(LPDIRECT3DDEVICE9 pGraphicDev)
    : CMonster(pGraphicDev), m_pDynamicTexCom(nullptr), m_dwCurStateKey(SLIME_STATE::S_END), m_dwAnimKey(SLIME_STATE::S_END),
    m_pStateMachine(nullptr), m_pCurState(nullptr)
{
}

CSlimeMob::CSlimeMob(const CSlimeMob& rhs)
    : CMonster(rhs), m_pDynamicTexCom(nullptr)
{
}

CSlimeMob::~CSlimeMob()
{
}

HRESULT CSlimeMob::Ready_GameObject()
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


    DAMAGE_INFO damage;
    damage.bCanParry = false;
    damage.bShouldKnockback = false;
    damage.fAmount = 60.f;
    damage.eApplyTiming = Engine::COL_STATE::ENTER_COL;
    damage.vDirKnockback = { 0.f, 1.f, 0.f };
    damage.pAttacker = this;

    m_pRectBox = CHitRectBox::Create(m_pGraphicDevice, this);
    m_pRectBox->Set_Damage(damage);

    m_umComponent[ID_DYNAMIC].insert({ L"SIVAROMA", m_pRectBox });

#pragma endregion
    Ready_Animation();

    m_pStateMachine = CSlimeStateMachine::Create(this);

    // after all components are set up
    Configure_Component();

#pragma region Debbuging
    Add_EditorFiled();
#pragma endregion

    return S_OK;
}

_int CSlimeMob::Update_GameObject(const _float fTimeDelta)
{
    m_pCurState->Update_State(fTimeDelta);

    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);

    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CSlimeMob::LateUpdate_GameObject(const _float fTimeDelta)
{
    m_pCurState->LateUpdate_State(fTimeDelta);

    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CSlimeMob::Render_GameObject()
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

HRESULT CSlimeMob::Ready_Animation()
{
    m_pDynamicTexCom->Ready_Texture(L"Slime_Idle");                //SLIME_STATE::AWAKE
    m_pDynamicTexCom->Ready_Texture(L"Slime_Circle");            //SLIME_STATE::ATK_CIRCLE
    m_pDynamicTexCom->Ready_Texture(L"Slime_Big");                 //SLIME_STATE::ATK_BIG

    // Configure boss animation values
    m_pDynamicTexCom->Set_Speed(10.f);

    return S_OK;
}

void CSlimeMob::Set_CurStateKey(_uint dwStateKey, CMonsterState* pCurState)
{
    if (dwStateKey == m_dwCurStateKey)
        return;

    m_dwCurStateKey = dwStateKey;
    m_pCurState = pCurState;
    m_pDynamicTexCom->Set_Texture(dwStateKey);
}

void CSlimeMob::Ready_EntityComponent()
{
    // Set Combat Stats                
    m_pCombatStats = CCombatStats::Create(this);
    m_pCombatStats->Set_HealthStat(CStatComponent::Create(this, 100.f, 0.f, 100.f));    // HP
    m_pCombatStats->Set_AttackStat(CStatComponent::Create(this, 10.f, 10.f, 30.f));      // Attack
    m_pCombatStats->Set_DefenseStat(CStatComponent::Create(this, 10.f, 10.f, 30.f));    // Defense
    m_pCombatStats->Set_SpeedStat(CStatComponent::Create(this, 5.f, 1.f, 10.f));            // Speed
}

void CSlimeMob::Configure_Component()
{
    m_pTransformCom->Set_Scale(4.f, 4.f, 4.f);
    m_pTransformCom->Set_Pos(10.f, 5.f, 10.f);
}

CSlimeMob* CSlimeMob::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CSlimeMob* pInstance = new CSlimeMob(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        MSG_BOX("CSlimeMob Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CSlimeMob::Free()
{
    Engine::CGameObject::Free();

    m_pStateMachine->Release();
}

void CSlimeMob::Add_EditorFiled()
{
    CGameObject::Add_EditorField("State", DATATYPE::DATA_TCHAR, &m_szState);
}

void CSlimeMob::Display_CurrentState()
{
    switch ((SLIME_STATE)m_dwCurStateKey)
    {
    case SLIME_STATE::IDLE:
    {
        wcscpy_s(m_szState, L"IDLE\n");
    }
    break;

    case SLIME_STATE::ATK_BIG:
    {
        wcscpy_s(m_szState, L"ATK_BIG\n");
    }
    break;
    case SLIME_STATE::ATK_CIRCLE:
    {
        wcscpy_s(m_szState, L"ATK_CIRCLE\n");
    }
    break;

    case SLIME_STATE::DEAD:
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
}
