#pragma once

#include "CRenderObject.h"

namespace Engine {
	class CTexture;
    class CSphereCollider;
    class CRectCollider;
}

class CTmpCustomComponent; // 클라이언트 컴포넌트는 Engine 소속이 아님

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

    void        On_Collision(const Collision& tCollision) override;

private:
	CTexture*	            m_pDynamicTexCom;
    CSphereCollider*        m_pColCom;
    CTmpCustomComponent*    m_pCustomCom;

public:
	static CTestRect*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	void				Free() override;
};

