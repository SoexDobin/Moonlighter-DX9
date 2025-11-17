#pragma once

#include "CGameObject.h"

#include "CTransform.h"
#include "CRectTex.h"

class CRenderObject : public CGameObject
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
	CTransform*			m_pTransformCom;
	CRectTex*			m_pBufferCom;

public:
	static CRenderObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void			Free() override;
};

