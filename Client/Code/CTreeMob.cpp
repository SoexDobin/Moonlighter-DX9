#pragma region INCLUDE
#include "pch.h"
#include "CTreeMob.h"
#include "CRenderer.h"
#include "CPrototypeManager.h"
#include "CManagement.h"

#include "CPlayer.h"
#include "CMonsterState.h"
#include "CTreeStateMachine.h"
#pragma endregion


CTreeMob::CTreeMob(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pDynamicTexCom(nullptr), m_dwCurStateKey(TREE_STATE::T_END), m_dwAnimKey(TREE_STATE::T_END),
    m_pStateMachine(nullptr), m_pCurState(nullptr)
{
    PANEL_NAME(L"Tree");
}

CTreeMob::CTreeMob(const CTreeMob& rhs)
    : CRenderObject(rhs), m_pDynamicTexCom(nullptr)
{
    PANEL_NAME(L"Tree");
}

CTreeMob::~CTreeMob()
{
}

HRESULT CTreeMob::Ready_GameObject()
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
    m_pStateMachine = CTreeStateMachine::Create(this);

    // after all components are set up
    Configure_Component();

#pragma region Debbuging
    Add_EditorFiled();
#pragma endregion

    return S_OK;
}

_int CTreeMob::Update_GameObject(const _float fTimeDelta)
{
    m_pCurState->Update_State(fTimeDelta);

    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);

    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CTreeMob::LateUpdate_GameObject(const _float fTimeDelta)
{
    m_pCurState->LateUpdate_State(fTimeDelta);

    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CTreeMob::Render_GameObject()
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

HRESULT CTreeMob::Ready_Animation()
{
    m_pDynamicTexCom->Ready_Texture(L"Tree_Awake");                //BOSS_STATE::AWAKE
    m_pDynamicTexCom->Ready_Texture(L"Tree_Shake");                 //BOSS_STATE::IDLE

    // Configure boss animation values
    m_pDynamicTexCom->Set_Speed(10.f);

    return S_OK;
}

void CTreeMob::Set_CurStateKey(_uint dwStateKey, CMonsterState* pCurState)
{
    if (dwStateKey == m_dwCurStateKey)
        return;

    m_dwCurStateKey = dwStateKey;
    m_pCurState = pCurState;
    m_pDynamicTexCom->Set_Texture(dwStateKey);
}

void CTreeMob::Configure_Component()
{
    m_pTransformCom->Set_Scale(11.f, 11.f, 1.f);
    m_pTransformCom->Set_Pos(15.f, 5.f, 10.f);
}

CTreeMob* CTreeMob::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTreeMob* pInstance = new CTreeMob(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        MSG_BOX("CTreeMob Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CTreeMob::Free()
{
    Engine::CGameObject::Free();

    m_pStateMachine->Release();
}

void CTreeMob::Add_EditorFiled()
{
    CGameObject::Add_EditorField("State", DATATYPE::DATA_TCHAR, &m_szState);
}

void CTreeMob::Display_CurrentState()
{
    switch ((TREE_STATE)m_dwCurStateKey)
    {
    case TREE_STATE::AWAKE:
    {
        wcscpy_s(m_szState, L"AWAKE\n");
    }
    break;

    case TREE_STATE::IDLE:
    {
        wcscpy_s(m_szState, L"IDLE\n");
    }
    break;

    case TREE_STATE::ATK_SHAKE:
    {
        wcscpy_s(m_szState, L"SHAKE\n");
    }
    break;

    case TREE_STATE::DEAD:
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
