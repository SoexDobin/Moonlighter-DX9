#pragma once

#include "CBase.h"
#include "CLayer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene() override;

public:	
    const unordered_map<_uint16, CLayer*>&  Get_Layers() { return m_umLayer; }
    CLayer* Get_Layer(_uint16 eID);
    CLayer* Get_Layer(const wstring& wsLayerTag);
    
public:
	CComponent*		Get_Component(COMPONENTID eID,
								const wstring& wsLayerTag,
								const wstring& wsObjTag,
								const wstring& wsComponentTag);
public:
	virtual		HRESULT		Ready_Scene();
	virtual		_int		Update_Scene(const _float fTimeDelta);
	virtual		void		LateUpdate_Scene(const _float fTimeDelta);
	virtual		void		Render_Scene();

protected:
	LPDIRECT3DDEVICE9							m_pGraphicDevice;
private:
	unordered_map<_uint16, CLayer*>       		m_umLayer;

protected:
	virtual void Free();

#pragma region Editor
public:
	void			Display_Editor();
	_bool		m_bDisplayInEditor;
#pragma endregion

};

END
