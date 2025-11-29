#pragma once
#include "CMonsterState.h"

class CBoss;
class CBossStateMachine;
class CBossGroundBrust;


class CBossRootState : public CMonsterState
{
private:
    explicit CBossRootState(CBoss* pOwner, CBossStateMachine* pStateMachine);
    explicit CBossRootState(LPDIRECT3DDEVICE9 pGraphicDev, CBoss* pOwner, CBossStateMachine* pStateMachine);
    virtual ~CBossRootState();

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
    void        Ready_CurrentCombo();
    void        Compute_SpawnPos();

private:
    CBoss* m_pOwner;
    CBossStateMachine* m_pStateMachine;

private :
    _ubyte           m_iCurrentBrustCnt;                // 현재 생성한 가지 수
    const _ubyte m_iMaxBrustCnt = 5;                // 한 콤보에 최대 생성 가지 수

    _ubyte           m_iCurrentComboCnt, m_iMaxComboCnt; // 현재 콤보 횟수, 최대 가능 한 콤보 횟수
    _vec3            m_vDetectedPos;                      // 생성 기준이 될 플레이어 위치
    vector<_vec3>            m_vecComputedSpawnPos;        // 미리 계산된 생성 위치

    const _ubyte m_iAttackStartFrame = 24;
    const _ubyte m_iAttackEndFrame = 40;
    _ubyte            m_iAttackFrameRange;
    _bool              m_bAttacking;
    _float             m_fCurComboElapsedTime, m_fCurCreateLimitTime; // 현재 콤보에서 흐른 시간 



public:
    static CBossRootState* Create(LPDIRECT3DDEVICE9 pGraphicDev, CBoss* pOwner, CBossStateMachine* pStateMachine);

private:
    virtual void Free();


};

