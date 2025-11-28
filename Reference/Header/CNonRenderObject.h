#pragma once

#include "CGameObject.h"

#include "CTransform.h"

BEGIN(Engine)

class ENGINE_DLL CNonRenderObject : public CGameObject
{
protected:
    explicit CNonRenderObject(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CNonRenderObject(const CNonRenderObject& rhs);
    virtual ~CNonRenderObject() override;

public:
    CTransform* Get_Trans() { return m_pTransformCom; }

public:
    virtual GAMEOBJECT_TYPE     Get_Type() override { return NONRENDER_OBJECT; }

public:
    virtual		HRESULT		Ready_GameObject() override;
    virtual		_int		Update_GameObject(const _float fTimeDelta) override;
    virtual		void		LateUpdate_GameObject(const _float fTimeDelta) override;
    virtual		void		Render_GameObject() override;

protected:
    CTransform* m_pTransformCom;

protected:
    static const wstring& n_wsTransformKey;
public:
    static const wstring& Get_TransformKey();
public:
    static CNonRenderObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    virtual void			Free() override;
};

END

