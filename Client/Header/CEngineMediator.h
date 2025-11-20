#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CMediator.h"

class CEngineMediator : public CMediator
{
    DECLARE_SINGLETON(CEngineMediator)

private:
    explicit CEngineMediator();
    virtual ~CEngineMediator();

public :
    virtual HRESULT Ready_Mediator(LPDIRECT3DDEVICE9 pGraphicDev) override;
    virtual void    Render_Mediator() override;
    virtual void    Change_Scene(_uint dwSceneIdx) override;

private:
    LPDIRECT3DDEVICE9 m_pGraphicDev;

private :
    virtual void    Free();


};

