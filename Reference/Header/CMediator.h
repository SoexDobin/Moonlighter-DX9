#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CMediator :public CBase
{
protected :
    explicit CMediator();
    virtual ~CMediator();

public :
    virtual HRESULT Ready_Mediator(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual void    Render_Mediator();
    virtual void    Display_AllScenePanel();

public :
    virtual void    Change_Scene(_uint dwSceneIdx);

protected :
    // Scene
    vector<string> m_vecScene;
    _uint                   m_dwCurSceneIdx;
    _uint                   m_dwEndScene;

private:
    virtual void Free();
};

END
