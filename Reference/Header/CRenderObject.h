#pragma once

#include "CGameObject.h"

#include "CTransform.h"
#include "CRectTex.h"

BEGIN(Engine)

class ENGINE_DLL CRenderObject : public CGameObject
{
protected:
	explicit CRenderObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRenderObject(const CRenderObject& rhs);
	virtual ~CRenderObject() override;

public:
	const CTransform&	Get_Trans()		{ return *m_pTransformCom; }
	const CRectTex&		Get_Buffer()	{ return *m_pBufferCom; }

public:
	virtual		HRESULT		Ready_GameObject() override;
	virtual		_int		Update_GameObject(const _float fTimeDelta) override;
	virtual		void		LateUpdate_GameObject(const _float fTimeDelta) override;
	virtual		void		Render_GameObject() override;

protected:
	CRectTex*			m_pBufferCom;
	CTransform*			m_pTransformCom;

	const wstring		n_wsBufferKey = L"Buffer_Com";
	const wstring		n_wsTransformKey = L"Transform_Com";
	
public:
	static CRenderObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void			Free() override;
};

END