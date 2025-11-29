#pragma once
#include "CState.h"

namespace Engine
{
    class CTransform;
    class CTexture;
}

class CPlayer;
class CMonsterStateMachine;

class CMonsterState : public CState
{
protected :
    explicit CMonsterState();
    explicit CMonsterState(LPDIRECT3DDEVICE9 pGrahpicDev);
    virtual ~CMonsterState();

public :
    virtual void    Enter() override;
    virtual void    Exit() override;

protected :
    virtual void    Find_Player();
    virtual void    Detect_Player();
    virtual void    Check_ShouldTransit(const _float& fTimeDelta);
    virtual void    Check_EventFrame();

protected :    
    CPlayer*        m_pPlayer;
    CTransform* m_pPlayerTransformCom;
    CTransform* m_pOwnerTransformCom;
    CTexture*     m_pOwnerTextureCom;

protected :
    _float      m_fCurStateElapsedTime; // 현재 지속 시간
    _float      m_fMinStateTime;              // 최소 지속 시간  
    _bool       m_bMinStateTimePassed;  // 최소 지속 시간 달성 확인용

    _uint       m_dwCurFrame;           // 현재 애니메이션 프레임

    _float      m_fDistToPlayer;        // 플레이어로의 거리
    _vec3       m_vDirToPlayer;         // 플레이어로지 방향
};

