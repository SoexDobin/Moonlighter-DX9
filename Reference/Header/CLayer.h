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
	CComponent*		Get_Component(COMPONENTID eID, const wstring pObjTag, const wstring pComponentTag);
	HRESULT			Add_GameObject(const wstring pObjTag, CGameObject* pGameObject);

public:
	HRESULT		Ready_Layer();
	_int		Update_Layer(const _float fTimeDelta);
	void		LateUpdate_Layer(const _float fTimeDelta);
	void		Render_Layer();

private:
	unordered_map<wstring, CGameObject*>		m_umGameObject;

public:
	static CLayer* Create();

private:
	virtual void Free() override;
};

END