#pragma once

#include "CGameObject.h"

namespace Engine
{
	class CRectColor;
	class CTransform;
}


class CTestRect : public CGameObject
{
private:
	explicit CTestRect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTestRect(const CTestRect& rhs);
	virtual ~CTestRect() override;

private:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float fTimeDelta) override;
	void		LateUpdate_GameObject(const _float fTimeDelta) override;
	void		Render_GameObject() override;

private:
	CRectColor*		m_pBufferCom;
	CTransform*		m_pTransformCom;

public:
	static CTestRect*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	void				Free() override;
};

