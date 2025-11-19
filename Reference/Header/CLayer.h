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
    const unordered_map<wstring, CGameObject*>&     Get_Objects() { return m_umGameObject; }

public:
	CComponent*		Get_Component(COMPONENTID eID, const wstring& wsObjTag, const wstring& wsComponentTag);
	HRESULT			Add_GameObject(const wstring& wsObjTag, CGameObject* pGameObject);

public:
	HRESULT		Ready_Layer();
	_int		Update_Layer(const _float fTimeDelta);
	void		LateUpdate_Layer(const _float fTimeDelta);
	void		Render_Layer();

private:
	unordered_map<wstring, CGameObject*>		m_umGameObject;

public:
	static CLayer* Create(const wstring& layerTag = L"Layer");

private:
	virtual void Free() override;

#pragma region Editor
public:
	void			Display_Editor();
public :
	bool			m_bDisplayInEditor;
	wstring	m_LayerTag;
#pragma endregion

};

END
