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
							const _tchar* pLayerTag,
							const _tchar* pObjTag,
							const _tchar* pComponentTag);

public:
	const _int		Get_SceneIndex() { return m_iCurIndex; }
	void			Set_SceneIndex(const _int iSceneIdx) { m_iCurIndex = iSceneIdx; }
	HRESULT			Set_Scene(_int iSceneIdx);
	HRESULT			Add_Scene(CScene* pScene);

	_int			Update_Scene(const _float fTimeDelta);
	void			LateUpdate_Scene(const _float fTimeDelta);
	void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	vector<CScene*>		m_vecScene;
	CScene*				m_pCurScene;
	_int				m_iCurIndex;

public:
	virtual void			Free() override;
};

END