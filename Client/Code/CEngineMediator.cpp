#include "pch.h"
#include "CEngineMediator.h"

#include "CManagement.h"

#include "CMonsterTestScene.h"
#include "CPlayerTestScene.h"
#include "CMainScene.h"
#include "CEditScene.h"

IMPLEMENT_SINGLETON(CEngineMediator)

CEngineMediator::CEngineMediator()
{
    m_pGraphicDev = nullptr;
}

CEngineMediator::~CEngineMediator()
{
}

HRESULT CEngineMediator::Ready_Mediator(LPDIRECT3DDEVICE9 pGraphicDev)
{
    m_pGraphicDev = pGraphicDev;

    // ImGui uses only char[]
    m_vecScene.resize(SCENETYPE::SC_END);
    m_vecScene[SCENETYPE::SC_MAIN] = "MAIN";
    m_vecScene[SCENETYPE::SC_PLAYER] = "PLAYER";
    m_vecScene[SCENETYPE::SC_MAP] = "MAP";
    m_vecScene[SCENETYPE::SC_MONSTER] = "MONSTER";

    // TODO : Get_CurrentScene()으로 받아와야 한다
    m_dwCurSceneIdx = static_cast<int>(SC_MAIN);
    m_dwEndScene = SCENETYPE::SC_END;

    return S_OK;
}

void CEngineMediator::Render_Mediator()
{
    CMediator::Render_Mediator();
}

void CEngineMediator::Change_Scene(_uint dwSceneIdx)
{
    _bool bFailed = false;

    switch ((SCENETYPE)dwSceneIdx)
    {
    case SC_MAIN:
        if (FAILED(CManagement::GetInstance()->Set_Scene(CMainScene::Create(m_pGraphicDev))))
        {
            bFailed = true;
        }
        break;
    case SC_PLAYER:
        if (FAILED(CManagement::GetInstance()->Set_Scene(CPlayerTestScene::Create(m_pGraphicDev))))
        {
            bFailed = true;
        }
        break;
    case SC_MAP:
        if (FAILED(CManagement::GetInstance()->Set_Scene(CEditScene::Create(m_pGraphicDev))))
        {
            bFailed = true;
        }
        break;
    case SC_MONSTER:
        if (FAILED(CManagement::GetInstance()->Set_Scene(CMonsterTestScene::Create(m_pGraphicDev))))
        {
            bFailed = true;
        }
        break;
    case SC_END:
        break;
    default:
        break;
    }



}

void CEngineMediator::Free()
{

}
