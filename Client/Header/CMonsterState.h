#pragma once
#include "CState.h"

namespace Engine
{
    class CTransform;
}

class CPlayer;
class CMonsterStateMachine;

class CMonsterState : public CState
{
private :
    explicit CMonsterState();
    explicit CMonsterState(CStateMachine* pStateMachine);
    virtual ~CMonsterState();

public :
    virtual void    Enter() override;
    virtual void    Exit() override;

private:
    virtual void    Find_Player();
    virtual void    Detect_Player();
    virtual void    Check_ShouldTransiti(const _float& fTimeDelta);

private:
    CMonsterStateMachine* m_pMonsterStateMachine;
    
    CPlayer* m_pPlayer;
    CTransform* m_pPlayerTransformCom;
    CTransform* m_pOwnerTransformCom;

private :
    _float      m_fCurStateElapsedTime; // 현재 지속 시간
    _float      m_fMinStateTime;              // 최소 지속 시간  
    _bool       m_bMinStateTimePassed;  // 최소 지속 시간 달성 확인용

    _float      m_fDistToPlayer;        // 플레이어로의 거리
    _vec3       m_vDirToPlayer;         // 플레이어로지 방향
};

