#pragma once
#include "Engine_Define.h"

namespace Engine
{
    class CGameObject;
}

/*
|-------- 투사체 --------------------||
|  |------ 히트박스 -----|                                  |
|  |   |-------------|     |                                  |
|  |   |  CCollider  (부모)  |    |                                  |
|  |   |______________|    |   PROJECTILE         |
|  |   |-------------|    |    (투사체 정보)           |
|  |   |   DAMAGE_INFO  |    |                                   |
|  |   |______________|    |                                   |
|  |__________________|                                   |
|                                                                               |
----------------------------------
*/

typedef struct tagDamageInfo
{
    CGameObject* pAttacker;
    _uint                   dwHitTargetFlag;  // 충돌 시 데미지 입힐 대상 

    _float fAmount;                  // 공격량
    _bool bCanParry;                // 패링 가능 여부
    _bool bShouldKnockback;   // 넉백 적용

    Engine::COL_STATE    eApplyTiming;   // 공격 적용 시점 

    //FIXME : 넉백 방향 CollisionManager에서 받아올 수 있는지 확인 
    _vec3 vDirKnockback;         // 넉백 방향 
}DAMAGE_INFO;

typedef struct tagProjectile
{
    vector<wstring>   wsVecTexture; // 등록할 애니메이션 이름 리스트
    _float                  fTextureSpeed;  // 텍스처 전환 속도

    _float                  fSpeed;                // 투사체 속도
    _float                  fScale;                 // 투사체 크기
    Engine::COL_TYPE    eColType;     // 히트박스 모양 : Rect or Sphere

    _uint                   dwCurFlag;            // 투사체의 플래그
    _uint                   dwHitTargetFlag;  // 충돌 시 데미지 입힐 대상 -> 히트박스에 넣어야 한다
    _uint                   dwDestroyOnHitFlag; // 충돌 시 해당 투사체를 사라지게 하는 대상

    DAMAGE_INFO tDamageInfo;        // 투사체 데미지 정보



}PROJECTILE;

