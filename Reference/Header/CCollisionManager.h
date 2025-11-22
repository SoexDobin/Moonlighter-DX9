#pragma once

#include "CBase.h"
#include "Engine_Define.h"
#include "CCollider.h"



BEGIN(Engine)

class CSphereCollider;

class ENGINE_DLL CCollisionManager final : public CBase
{
    DECLARE_SINGLETON(CCollisionManager)
private:
    explicit CCollisionManager();
    virtual ~CCollisionManager() override;

public:
    void        Add_Collider(const wstring& wsLayerTag, CCollider* pColComonent);
    void        Release_Collider();

public:
    void        Update_Collision();

    static      _bool RectCollision();
    static      _bool SphereCollision(CSphereCollider* pSrc, CSphereCollider* pDst);


private:
    vector<CCollider*>      m_vecCollider;

private:
    void            Free() override;
};

END
