#pragma once
#include "CMonster.h"

namespace Engine {
    class CTexture;
}

class CPlayer;
class CMonsterState;
class CBossStateMachine;

class CBoss : public CMonster
{
public:
    enum BOSS_STATE { AWAKE, IDLE, ATK_JUMP, ATK_THROW, ATK_ROOT, ATK_SHAKE, DEAD, WALK, B_END };
private:
    enum BOSS_COMPONENT { HEALTH, ATTACK, SPEED, DEFENSE,  BC_END, DETECT, DAMAGE };
#pragma region Base Logic
private:
    explicit CBoss(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CBoss() override;

public:
    HRESULT		Ready_GameObject() override;
    _int		Update_GameObject(const _float fTimeDelta) override;
    void		LateUpdate_GameObject(const _float fTimeDelta) override;
    void		Render_GameObject() override;

    void    On_Collision(const Collision& tCollision) override;

#pragma endregion

private:
    HRESULT     Ready_Animation();

public :
    void    Set_CurStateKey(_uint dwStateKey, CMonsterState* pCurState);
    void    Set_CurAnimKey(_uint dwAinmKey) { m_dwAnimKey = dwAinmKey; }

    virtual void    Ready_EntityComponent() override;
    virtual void    Configure_Component() override;

private:
    CTexture* m_pDynamicTexCom;

#pragma region State
private:
    CBossStateMachine* m_pStateMachine;
    CMonsterState* m_pCurState;
    _uint                              m_dwCurStateKey;
    _uint                              m_dwAnimKey;
#pragma endregion

public:
    static CBoss* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
    void				Free() override;

#pragma region Debugging
private :
    void    Add_EditorFiled();
    void    Display_CurrentState();
    _tchar m_szState[16];
    _float  debug_fHP;
    _float debug_fSpeed;
#pragma endregion

};

