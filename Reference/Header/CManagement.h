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
    CGameObject*               Get_Object(const wstring& wsLayerTag, const wstring& wsObjTag);
    CGameObject*               Get_Object(const wstring& wsObjTag);
    const list<CGameObject*>*  Get_Object_List(const wstring& wsObjTag);

	CComponent*		Get_Component(COMPONENTID eID,
							const wstring& wsLayerTag,
							const wstring& wsObjTag,
							const wstring& wsComponentTag);

public:
	HRESULT			Set_Scene(CScene* pScene);
    // 존재하는 레이어에 오브젝트 등록
    HRESULT         Add_GameObject(const wstring& wsLayerTag, const wstring& wsObjTag, CGameObject* pGameObject);

public:
	_int			Update_Scene(const _float fTimeDelta);
	void			LateUpdate_Scene(const _float fTimeDelta);
	void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    _bool               m_bIsInit;
	CScene*				m_pCurScene;

	
public:
	virtual void			Free() override;
};

END
