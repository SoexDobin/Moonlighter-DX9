#pragma once

#include "CBase.h"
#include "Engine_Define.h"
#include "CCollider.h"

BEGIN(Engine)

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
    vector<CCollider*>      m_vecCollider;

private:
    void            Free() override;
};

END
