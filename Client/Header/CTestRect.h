#pragma once

#include "CRenderObject.h"
namespace Engine {
	class CTexture;
}

class CTestRect : public CRenderObject
{
private:
	enum STATE { POTION, ROLL, STATE_END };
private:
	explicit CTestRect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTestRect(const CTestRect& rhs);
	virtual ~CTestRect() override;

public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float fTimeDelta) override;
	void		LateUpdate_GameObject(const _float fTimeDelta) override;
	void		Render_GameObject() override;

private:
	CTexture*	m_pDynamicTexCom;
	CTexture*	m_pStaticTexCom;

public:
	static CTestRect*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	void				Free() override;
};

