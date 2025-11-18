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

public:
	HRESULT Request_ChangeScene(CScene* nextScene);    // 씬 전환 예약
	void    Commit_ChangeScene();   // 프레임 경계(다음 프레임 업데이트 시작)에서 씬 실제 전환

private:
	CScene*				m_pCurScene;
	CScene* m_pNextScene;
	_bool m_bChangeScene;

	
public:
	virtual void			Free() override;
};

END