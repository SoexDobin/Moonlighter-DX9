#pragma once
#include "CMonsterState.h"

class CBoss;
class CBossStateMachine;

class CBossThrowState : public CMonsterState
{
private:
    explicit CBossThrowState(CBoss* pOwner, CBossStateMachine* pStateMachine);
    explicit CBossThrowState(LPDIRECT3DDEVICE9 pGraphicDev, CBoss* pOwner, CBossStateMachine* pStateMachine);
    virtual ~CBossThrowState();

public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    virtual HRESULT     Ready_State() override;
    virtual _uint             Update_State(const _float& fTimeDelta) override;
    virtual void               LateUpdate_State(const _float& fTimeDelta) override;
    virtual void               Render_State() override;

private:
    virtual void    Check_EventFrame() override;

private:
    void        Determine_NextState();

private:
    void        Jump_ToPlayer(const _float fTimeDelta);
    void        Ready_Jump();
    void        Compute_DeltaY();

private:
    CBoss* m_pOwner;
    CBossStateMachine* m_pStateMachine;

public:
    static CBossThrowState* Create(LPDIRECT3DDEVICE9 pGraphicDev, CBoss* pOwner, CBossStateMachine* pStateMachine);

private:
    virtual void Free();


};

