#pragma once

#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev, 
					 const _vec3* pEye,
					 const _vec3* pAt,
					 const _vec3* pUp,
					 _float fFov,
					 _float fAspect,
					 _float fNear,
					 _float fFar);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() override;

public:
	virtual HRESULT		Ready_GameObject() override;
	virtual _int		Update_GameObject(const _float fTimeDelta) override;
	virtual void		LateUpdate_GameObject(const _float fTimeDelta) override;

public:
    virtual GAMEOBJECT_TYPE     Get_Type() override { return CAMERA_OBJECT; }

protected:
	_matrix		m_matView, m_matProj;
	_vec3		m_vEye, m_vAt, m_vUp;
	_float		m_fFov, m_fAspect, m_fNear, m_fFar;

protected:
	virtual void		Free();
};

END
