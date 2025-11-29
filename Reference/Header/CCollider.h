#pragma once

#include "CComponent.h"

BEGIN(Engine)

class CCollider;

typedef struct tagCollision         // 충돌 시 CGameObject::On_Collision 이 반환하는 매개인수
{
    friend class CCollider;
    CGameObject*    pColTarget;     // 충돌 당한 오브젝트
    CCollider*      pColSource;     // 충돌 당한 오브젝트의 콜라이더
    COL_STATE       eColState;      // 충돌 상태 첫 충돌, 충돌중, 충돌 종료

} Collision;

class ENGINE_DLL CCollider : public CComponent
{
protected:
    explicit CCollider();
    explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CCollider(const CCollider& rhs);
    virtual ~CCollider() override;

public:
    virtual void        On_Enable() override {
        m_bEnable = true;
        m_bIsCol = false;
        m_eState = COL_STATE_END;
        m_tCollision = { nullptr, nullptr, COL_STATE_END };

        m_usetOverlapCol.clear();
    };
    virtual void        On_Disable() override {
        m_bEnable = false;
        m_bIsCol = false;
        m_eState = COL_STATE_END;
        m_tCollision = { nullptr, nullptr, COL_STATE_END };

        m_usetOverlapCol.clear();
    };

public:
    virtual PROTOTYPE_COMPONENT Get_ComponentType() override { return COLLIDER; }
    virtual COL_TYPE            Get_ColType() PURE;


    unordered_set<CCollider*>&  Get_OverlapMember()                  { return m_usetOverlapCol; }
    _bool                       Get_IsCol() const                    { return m_bIsCol; }
    void                        Set_IsCol(const _bool& bIsCol)       { m_bIsCol = bIsCol; }
    const Collision&            Get_Collision() const                { return m_tCollision; }
    void                        Set_Collision(const Collision& tCol) { m_tCollision = tCol; }
    COL_STATE                   Get_ColState() const                 { return m_eState; }
    void                        Set_ColState(COL_STATE eState)       { m_eState = eState; }

    const _vec3&                Get_Offset() const                   { return m_vOffset; }
    void                        Set_Offset(const _vec3& vOffset)     { m_vOffset = vOffset; }
    _float                      Get_Scale() const                    { return m_fScale; }
    void                        Set_Scale(const _float fScale)       { m_fScale = fScale; }

public:
    _bool                       Is_Overlapped(CCollider* pOverlap);             // 이전에 겹친 멤버인가?
    void                        Add_OverlapMember(CCollider* pOverlap);         // 겹쳐진 멤버로 추가
    void                        Release_OverlapMember(CCollider* pOverlap);     // 겹쳐진 멤버 해제

public:    
    virtual _bool               Check_Collision(CCollider* pCol) PURE;      // 충돌 기능 오버라이드
#ifdef _DEBUG
    virtual void                Render_DebugCollider() PURE;                // 디버그용 그리기 함수
#endif

protected:
    Collision                      m_tCollision;            // 충돌시 반환할 충돌 정보 구조체
    COL_STATE                      m_eState;                // 마지막 프레임 기준 충돌 상태
    _bool                          m_bIsCol;                // 마지막 프레임 기준 충돌 여부
    unordered_set<CCollider*>      m_usetOverlapCol;        // 마지막 프레임 기준 겹쳐진 콜라이더 

    _vec3                          m_vOffset;               // 담당 CGameObject transform 기준 오프셋 위치
    _float                         m_fScale = 1.f;          // 크기 비율
    
public:
    CComponent* Clone() PURE;
protected:
    virtual void        Free();
#pragma region Editor
#ifdef _DEBUG
public:
    virtual void Display_Editor(const char* pObjTag) override;
protected:
    _bool m_bShowOverlapsInEditor = false;
#endif
#pragma endregion
};

END
