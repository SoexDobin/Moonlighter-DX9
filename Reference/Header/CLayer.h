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
	CComponent*		Get_Component(COMPONENTID eID, const wstring wsObjTag, const wstring wsComponentTag);
	HRESULT			Add_GameObject(const wstring wsObjTag, CGameObject* pGameObject);

public:
	HRESULT		Ready_Layer();
	_int		Update_Layer(const _float fTimeDelta);
	void		LateUpdate_Layer(const _float fTimeDelta);
	void		Render_Layer();

private:
	unordered_map<wstring, CGameObject*>		m_umGameObject;

public:
	static CLayer* Create(wstring layerTag = L"Layer");

private:
	virtual void Free() override;

#pragma region Editor
public:
	void			Display_Editor();
public :
	_bool			m_bDisplayInEditor;
	char            	m_LayerTag[32];
#pragma endregion

};

END
