#pragma once

#include "CMonster.h"
namespace Engine {
    class CTexture;
}

class CPlayer;
class CMonsterState;
class CSlimeStateMachine;

class CHitRectBox;

class CSlimeMob : public CMonster
{
public:
    enum SLIME_STATE { IDLE, ATK_CIRCLE, ATK_BIG, WALK, DEAD, S_END };

private:
    explicit CSlimeMob(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CSlimeMob(const CSlimeMob& rhs);
    virtual ~CSlimeMob() override;

public:
    HRESULT		Ready_GameObject() override;
    _int		Update_GameObject(const _float fTimeDelta) override;
    void		LateUpdate_GameObject(const _float fTimeDelta) override;
    void		Render_GameObject() override;

private:
    HRESULT     Ready_Animation();

public:
    void    Set_CurStateKey(_uint dwStateKey, CMonsterState* pCurState);
    void    Set_CurAnimKey(_uint dwAinmKey) { m_dwAnimKey = dwAinmKey; }

    virtual void    Ready_EntityComponent() override;
    virtual void    Configure_Component() override;

    CHitRectBox* m_pRectBox;

private:
    CTexture* m_pDynamicTexCom;

private:
    CSlimeStateMachine* m_pStateMachine;
    CMonsterState* m_pCurState;
    _uint                               m_dwCurStateKey;
    _uint                               m_dwAnimKey;

public:
    static CSlimeMob* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
    void				Free() override;

#pragma region Debugging
private:
    void    Add_EditorFiled();
    void    Display_CurrentState();
    _tchar m_szState[16];
#pragma endregion
};

