#pragma once

#include "CGameObject.h"

BEGIN(Engine)

class CTransform;

class ENGINE_DLL CCamera : public CGameObject
{
public:
    enum PROJECTION_TYPE { PROJ_PERSPECTIVE, PROJ_ORTHOGRAPHIC };

protected:
    explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev,
        const _vec3* pEye,
        const _vec3* pAt,
        const _vec3* pUp,
        _float fFov,
        _float fAspect,
        _float fNear,
        _float fFar,
        PROJECTION_TYPE eProj);
    explicit CCamera(const CCamera& rhs);
    virtual ~CCamera() override;

public:
    virtual HRESULT		                Ready_GameObject() override;
    virtual _int		                            Update_GameObject(const _float fTimeDelta) override;
    virtual void		                            LateUpdate_GameObject(const _float fTimeDelta) override;

public:
    virtual void                                Set_ProjType(PROJECTION_TYPE eType) { m_eProjType = eType; m_bProjectionChanged = true; }
    virtual void                                Set_FOV(_float fFov) { m_fFov = fFov; m_bProjectionChanged = true; }
    virtual void                                Set_ClippingFlane(_float fN, _float fZ) { m_fNear = fN; m_fFar = fZ; m_bProjectionChanged = true; }

public:
    void        Translate(INFO eInfo, const _float& fTimeDelta, const _float& fSpeed);
    void        Translate(const _vec3& vDir, const _float& fTimeDelta, const _float& fSpeed);
    void        Rotate(ROTATION eAxis, const _float& fRadian);
    void        Rotate(const _vec3& vNewAt);

public:
    void        Set_Pos(const _vec3& vPos) { m_pTransformCom->Set_Pos(vPos); }
    const _vec3& Get_Pos() const { return m_pTransformCom->Get_Pos(); }

public:
    virtual GAMEOBJECT_TYPE     Get_Type() override { return CAMERA_OBJECT; }
    virtual PROJECTION_TYPE      Get_ProjType() { return m_eProjType; }

protected:
    void Compute_ViewMatrix();

protected:
    PROJECTION_TYPE m_eProjType;

    _matrix		    m_matView, m_matProj;
    _vec3		        m_vEye, m_vAt, m_vUp;
    _float		        m_fFov, m_fAspect, m_fNear, m_fFar;
    CTransform* m_pTransformCom;

    _bool               m_bProjectionChanged;

public:
    static CCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
        const _vec3* pEye,
        const _vec3* pAt,
        const _vec3* pUp,
        _float fFov,
        _float fAspect,
        _float fNear,
        _float fFar,
        PROJECTION_TYPE eType);

protected:
    virtual void		Free();
};

END
