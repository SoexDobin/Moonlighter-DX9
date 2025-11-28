#pragma region INCLUDE
#include "pch.h"
#include "CBoss.h"
#include "CRenderer.h"
#include "CPrototypeManager.h"
#include "CManagement.h"

#include "CPlayer.h"
#include "CMonsterState.h"
#include "CBossStateMachine.h"
#pragma endregion

CBoss::CBoss(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pDynamicTexCom(nullptr), m_dwCurStateKey(BOSS_STATE::B_END), m_dwAnimKey(BOSS_STATE::B_END),
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
    Ready_Animation();
   m_pStateMachine = CBossStateMachine::Create(this);

    // after all components are set up
    Configure_Component();


#pragma region Debbuging
    Add_EditorFiled();
#pragma endregion

    return S_OK;
}

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

HRESULT CBoss::Ready_Animation()
{
    m_pDynamicTexCom->Ready_Texture(L"Boss_Awake");                //BOSS_STATE::AWAKE
    m_pDynamicTexCom->Ready_Texture(L"Boss_Idle");                    //BOSS_STATE::IDLE
    m_pDynamicTexCom->Ready_Texture(L"Boss_Jump");                  //BOSS_STATE::JUMP
    m_pDynamicTexCom->Ready_Texture(L"Boss_Throwing");          //BOSS_STATE::ATK_THROW
    m_pDynamicTexCom->Ready_Texture(L"Boss_Roots");                //BOSS_STATE::ATK_ROOT
    m_pDynamicTexCom->Ready_Texture(L"Boss_Shake");                //BOSS_STATE::ATK_SHAKE
    m_pDynamicTexCom->Ready_Texture(L"Boss_Death");                //BOSS_STATE::DEAD

    // TODO : 무기 스프라이트 : 다른 데로 옮겨야 함
    //m_pDynamicTexCom->Ready_Texture(L"Boss_Cutting_Idle");
    //m_pDynamicTexCom->Ready_Texture(L"Boss_Cutting_Growing");
    //m_pDynamicTexCom->Ready_Texture(L"Boss_Cutting_Shot");
    //m_pDynamicTexCom->Ready_Texture(L"Root1");
    //m_pDynamicTexCom->Ready_Texture(L"Root2");

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

void CBoss::Configure_Component()
{
    m_pTransformCom->Set_Pos(20.f, 5.f, 10.f);
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
}

void CBoss::Add_EditorFiled()
{
    CGameObject::Add_EditorField("State", DATATYPE::DATA_TCHAR, &m_szState);
}

void CBoss::Display_CurrentState()
{
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


}
