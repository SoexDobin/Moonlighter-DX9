#pragma once

#include "CRenderObject.h"
namespace Engine {
	class CTexture;
}

class CTestRect : public CRenderObject
{
private:
	explicit CTestRect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTestRect(const CTestRect& rhs);
	virtual ~CTestRect() override;

public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float fTimeDelta) override;
	void		LateUpdate_GameObject(const _float fTimeDelta) override;
	void		Render_GameObject() override;

public:
	CTexture*	m_pTexCom;

public:
	static CTestRect*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	void				Free() override;
};

