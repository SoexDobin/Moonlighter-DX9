#pragma once
#include "CMonsterState.h"

class CSlimeMob;
class CSlimeStateMachine;

class CSlimeCircleState : public CMonsterState
{
private:
    explicit CSlimeCircleState(CSlimeMob* pOwner, CSlimeStateMachine* pStateMachine);
    explicit CSlimeCircleState(LPDIRECT3DDEVICE9 pGraphicDev, CSlimeMob* pOwner, CSlimeStateMachine* pStateMachine);
    virtual ~CSlimeCircleState();

public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    virtual HRESULT     Ready_State() override;
    virtual _uint             Update_State(const _float& fTimeDelta) override;
    virtual void               LateUpdate_State(const _float& fTimeDelta) override;
    virtual void               Render_State() override;

private:
    void        Determine_NextState();

private:
    CSlimeMob* m_pOwner;
    CSlimeStateMachine* m_pStateMachine;
public:
    static CSlimeCircleState* Create(LPDIRECT3DDEVICE9 pGraphicDev, CSlimeMob* pOwner, CSlimeStateMachine* pStateMachine);

private:
    virtual void Free();


};

