#pragma once
#include "CRenderObject.h"

class CHitRectBox;

class CBossGroundBrust : public CRenderObject
{
private :
    enum ROOT_TYPE { ROOT1, ROOT2, ROOT_END};
private:
    explicit CBossGroundBrust(LPDIRECT3DDEVICE9 pGraphicDev, CRenderObject* pOwner);
    explicit CBossGroundBrust(const CBossGroundBrust& rhs);
    virtual ~CBossGroundBrust();

public:
    HRESULT     Ready_GameObject() override;
    _int        Update_GameObject(const _float fTimeDelta) override;
    void        LateUpdate_GameObject(const _float fTimeDelta) override;
    void        Render_GameObject() override;
    void        On_Collision(const Collision& tCollision) override;

public :
    void        Set_SpawnPos(const _vec3& vPos) { m_pTransformCom->Set_Pos(vPos); }
    void        Set_Root1();    // 두 개의 뿌리 타입 중 Root1
    void        Set_Root2();    // 두 개의 뿌리 타입 중 Root2

private :
    void        Ready_Combat();
    void        Ready_Texture();
    void        Check_EventFrame();

private:
    CTexture* m_pDynamicTexCom;     // 텍스처
    CHitRectBox* m_pColCom;             // 콜라이더 겸 히트박스

private:
    CRenderObject* m_pOwner;
    DAMAGE_INFO m_tDamage;

    ROOT_TYPE m_eType;
    _uint             m_dwEndFrame;

public:
    static CBossGroundBrust* Create(LPDIRECT3DDEVICE9 pGraphicDev, CRenderObject* pOwner);

protected:
    void                Free() override;
};

