#pragma once
#include "CBase.h"
#include "Engine_Define.h"

namespace Engine
{
    class CRenderObject;
    class CTexture;
}

class CStateMachine;

class CState : public CBase
{
protected:
    explicit CState();
    explicit CState(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CState();

public:
    virtual void    Enter() = 0;
    virtual void    Exit() = 0;

public :
    virtual HRESULT     Ready_State() { return S_OK;  }
    virtual _uint             Update_State(const _float& fTimeDelta) { return 0;  }
    virtual void               LateUpdate_State(const _float& fTimeDelta) {}
    virtual void               Render_State() {}

public:
    // enum State...
    virtual _uint   Get_StateKey() const { return m_dwCurStateKey; }

protected:
    //unordered_map<wstring>   m_pTexture;

protected:
    LPDIRECT3DDEVICE9 m_pGraphicDev;

protected:
    wstring           m_wsCurTex;
    _bool               m_bStarted;
    _bool               m_bCanTransit;
    _bool               m_bExited;
    _uint               m_dwCurStateKey;
};
