#pragma once
#include "Engine_Define.h"

namespace Engine
{
    class CGameObject;
}

typedef struct tagDamageInfo
{
    CGameObject* pAttacker;

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

    _float                  fSpeed;         // 투사체 속도
    DAMAGE_INFO tDamageInfo;    // 투사체 데미지 정보

}PROJECTILE;

