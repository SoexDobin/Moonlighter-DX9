#pragma once

#include "CRenderObject.h"
namespace Engine {
    class CTexture;
}

class CPlayer;
class CMonsterState;
class CTreeStateMachine;
class CTreeProjectile;

class CTreeMob : public CRenderObject
{
public :
    enum TREE_STATE { AWAKE, IDLE, ATK_SHAKE, DEAD, T_END };

private:
    explicit CTreeMob(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTreeMob(const CTreeMob& rhs);
    virtual ~CTreeMob() override;

public:
    HRESULT		Ready_GameObject() override;
    _int		Update_GameObject(const _float fTimeDelta) override;
    void		LateUpdate_GameObject(const _float fTimeDelta) override;
    void		Render_GameObject() override;

private:
    HRESULT     Ready_Animation();
    HRESULT     Ready_Combat();

public:
    void    Set_CurStateKey(_uint dwStateKey, CMonsterState* pCurState);
    void    Set_CurAnimKey(_uint dwAinmKey) { m_dwAnimKey = dwAinmKey; }

private:
    void    Configure_Component();

private:
    CTexture* m_pDynamicTexCom;

private:
    CTreeStateMachine*      m_pStateMachine;
    CMonsterState*             m_pCurState;
    _uint                               m_dwCurStateKey;
    _uint                               m_dwAnimKey;


    // TEST
    DAMAGE_INFO              m_tDamage;
    PROJECTILE                  m_tProjectile;
    _float m_fElapsed = 0.f;


public:
    static CTreeMob* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
    void				Free() override;

#pragma region Debugging
private:
    void    Add_EditorFiled();
    void    Display_CurrentState();
    _tchar m_szState[16];
#pragma endregion
};

