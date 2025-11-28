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
    const unordered_map<wstring, list<CGameObject*>>&     Get_Objects() { return m_umGameObject; }

public:
    const wstring&  Get_LayerName() { return m_wsLayerTag; }
	CComponent*		Get_Component(COMPONENTID eID, const wstring& wsObjTag, const wstring& wsComponentTag);
	HRESULT			Add_GameObject(const wstring& wsObjTag, CGameObject* pGameObject);

public:
	HRESULT		Ready_Layer(const wstring& wsLayerName);
	_int		Update_Layer(const _float fTimeDelta);
	void		LateUpdate_Layer(const _float fTimeDelta);
	void		Render_Layer();

private:
	unordered_map<wstring, list<CGameObject*>>		m_umGameObject;
    wstring                                         m_wsLayerTag;

public:
	static CLayer* Create(const wstring& layerTag = L"Layer");

private:
	virtual void Free() override;

#pragma region Editor
public:
	void			Display_Editor();
public :
	_bool			m_bDisplayInEditor;
	char            	m_LayerTag[32];
    static wstring ws_selectedLayer;
#pragma endregion

};

END
