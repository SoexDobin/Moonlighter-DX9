#pragma once

#include "CBase.h"
#include "CScene.h"

BEGIN(Engine)

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual	~CManagement() override;

public:
	CComponent*		Get_Component(COMPONENTID eID,
							const wstring wsLayerTag,
							const wstring wsObjTag,
							const wstring wsComponentTag);

public:
	HRESULT			Set_Scene(CScene* pScene);

public:
	_int			Update_Scene(const _float fTimeDelta);
	void			LateUpdate_Scene(const _float fTimeDelta);
	void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CScene*				m_pCurScene;
	
public:
	virtual void			Free() override;
};

END