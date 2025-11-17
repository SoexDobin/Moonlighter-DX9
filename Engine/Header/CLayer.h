#pragma once
#include "CBase.h"
#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer() override;

public:
	CComponent*		Get_Component(COMPONENTID eID, const _tchar* pObjTag, const _tchar* pComponentTag);
	HRESULT			Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject);

public:
	HRESULT		Ready_Layer();
	_int		Update_Layer(const _float fTimeDelta);
	void		LateUpdate_Layer(const _float fTimeDelta);
	void		Render_Layer();

private:
	unordered_map<const _tchar*, CGameObject*>		m_umGameObject;

public:
	static CLayer* Create();

private:
	virtual void Free() override;
};

END