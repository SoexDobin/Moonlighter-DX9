#pragma once

#include "CBase.h"
#include "Engine_Define.h"


BEGIN(Engine)

class CCollider;
class CSphereCollider;
class CRectCollider;

class ENGINE_DLL CCollisionManager final : public CBase
{
    DECLARE_SINGLETON(CCollisionManager)
private:
    explicit CCollisionManager();
    virtual ~CCollisionManager() override;

public:
    void        Add_Collider(CCollider* pColComonent);

public:
    void        Update_Collision();
    void        Clear_CollisionGroup();
#ifdef _DEBUG
    void        Render_Collision();
#endif
    static      _bool RectCollision(CRectCollider* pSrc, CRectCollider* pDst);
    static      _bool SphereCollision(CSphereCollider* pSrc, CSphereCollider* pDst);
    static      _bool SphereRectCollision(CSphereCollider* pSrc, CRectCollider* pDst);

private:
    vector<CCollider*>                      m_vecCollider;
    vector<_int>                            m_vecGarbageIndex;

private:
    void            Free() override;
};

END
