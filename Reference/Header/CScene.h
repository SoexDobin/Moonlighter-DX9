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
	const _int		Get_SceneIndex() const				{ return m_iSceneIdx; };
	void			Set_SceneIndex(const _int idx)		{ m_iSceneIdx = idx; };

    const unordered_map<wstring, CLayer*>&  Get_Layers() { return m_umLayer; }
    CLayer* Find_Layers(const wstring& wsLagerTag);

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
	_int										m_iSceneIdx;
	LPDIRECT3DDEVICE9							m_pGraphicDevice;
	unordered_map<wstring, CLayer*>		        m_umLayer;

protected:
	virtual void Free();

#pragma region Editor
public:
	void			Display_Editor();
	_bool		m_bDisplayInEditor;
#pragma endregion

};

END
