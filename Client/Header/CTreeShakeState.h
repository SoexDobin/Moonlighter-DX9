#pragma once
#include "CMonsterState.h"

class CTreeMob;
class CTreeStateMachine;
class CTreeProjectile;

class CTreeShakeState : public CMonsterState
{
private :
    enum ATK_TYPE { ATK1 = 0, ATK2, ATKCOMB, ATK_END};
private:
    explicit CTreeShakeState(CTreeMob* pOwner, CTreeStateMachine* pStateMachine);
    explicit CTreeShakeState(LPDIRECT3DDEVICE9 pGraphicDev, CTreeMob* pOwner, CTreeStateMachine* pStateMachine);
    virtual ~CTreeShakeState();

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

private :
    void        Ready_Combat();
    void        Shoot_Projectile1(const _float& fTimeDelta);         // 공격 1
    void        Shoot_Projectile2(const _float& fTimeDelta);        // 공격 2
    void        Shoot_ProjectileComb(const _float& fTimeDelta); // 공격 1 + 2

    void        Swap_AtkType(ATK_TYPE eType);

private:
    CTreeMob*                   m_pOwner;
    CTreeStateMachine*  m_pStateMachine;

private:
    DAMAGE_INFO       m_tAtk1Dmg, m_tAtk2Dmg; // Attack1, 2에 대한 데미지 정보
    PROJECTILE          m_tProjectile1, m_tProjectile2; // Attack1, 2에 대한 투사체 정보

    ATK_TYPE              m_eAtkType;

private :
    _float      m_fLastShootTime, m_fLimitShootTime;    // 현재 공격의 마지막 발사 시간, 최소 대기 시간 
    _int         m_iCurShootCnt, m_iLimitShootCnt;           // 현재 공격에서 발사 횟수
    _vec3       m_vecPreComputedDir[12];                            // 미리 8방향 계산 

public:
    static CTreeShakeState* Create(LPDIRECT3DDEVICE9 pGraphicDev, CTreeMob* pOwner, CTreeStateMachine* pStateMachine);

private:
    virtual void Free();


};

