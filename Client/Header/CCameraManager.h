#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CCamera.h"

class CCameraManager : public CBase
{
#pragma region 디버깅용
    _vec3 vCurPos;
    _float fDist;
#pragma endregion

    DECLARE_SINGLETON(CCameraManager);

#pragma region CAMERA ENUM
public:
    enum CAMERA_MODE
    {
        INGAME,                             // 인게임용 플레이어 고정
        DBG_PERSPECTIVE,         // 디버깅용 Dynamic Cam
        DBG_ORTHOGRAPHIC,     // 디버깅용 Orthographic Cam
        C_END
    };

    enum CAMERA_ACTION
    {
        ACTION_FOLLOW,        // 플레이어 따라가기 (기본 추적 모드)
        ACTION_DISPLAY,       // 보스/오브젝트 잠깐 보여주기
        ACTION_CLOSER,        // 부드러운 클로즈업 : npc 대화 등  

        ACTION_END
    };

    enum CAMERA_EFFECT
    {
        EFFECT_NONE,            // 기본 상태
        EFFECT_SHAKE,         // 카메라 흔들기
        EFFECT_ZOOM,          // 확대/축소
    };

#pragma endregion

private:
    explicit CCameraManager();
    virtual ~CCameraManager();

public:
    HRESULT Ready_Camera(LPDIRECT3DDEVICE9 pGraphicDev);
    _int        Update_Camera(const _float fTimeDelta);
    void        LateUpdate_Camera(const _float fTimeDelta);

public :
    // 이거 콜백을 에디터에 등록해서 카메라 자유 움직임 되는지 확인하기
    void        Callback_OnDebugCam();
    void        Callback_DoDebugCam();
    void        Callback_OffDebugCam();

private:
    void        Update_InGameCamera(const _float& fTimeDelta);

public:
#pragma region INGAME
    // InGame Mode
    // 아래 함수를 호출하여 카메라 모드 변경
    void         Change_ToFollow(CTransform* pTarget);
    void         Change_ToDisplay(const _vec3& vDisplayPos, const _float& fMoveDuration, const _float& DelayTime);
    void         Change_ToCloser(const _float& fSpeed);
    void         Exit_Closer();

private:
    void         Act_Follow(const _float& fTimeDelta);
    void         Act_Display(const _float& fTimeDelta);
    void         Act_Closer(const _float& fTimeDelta);

    void         Finish_CurrentMode();
    void         Finish_Follow();
    void         Finish_Display();
    void         Finish_Closer();

public:
    void        Start_Shake(const _float& fRange, const _float& fDuration);
    void        Shaking(const _float& fTimeDelta);

#pragma endregion

#pragma region DEBUG
    // Debug Mode
    void        Handle_Input(const _float& fTimeDelta);
#pragma endregion


private:
    // 카메라 설정
    Engine::CCamera* m_pCurCam;          // 현재 카메라
    Engine::CCamera* m_pInGameCam;   // 인게임용 카메라
    Engine::CCamera* m_pDebugCam;      // 디버깅용 카메라

    CAMERA_MODE         m_eMode;             // 인게임 or 디버깅
    CAMERA_ACTION     m_eAction;           // 플레이어 or 전시 or 근접
    CAMERA_EFFECT      m_eEffect;           // 쉐이크 or 줌



    _float m_fFov, m_fAspect, m_fNear, m_fFar;
    Engine::CCamera::PROJECTION_TYPE    m_eType;

    _float m_fSpeed;
    const _float m_fEpsilon = 1.f;

#pragma region Members
    _vec3               m_vOriginPos; // ACTION이나 EFFECT 시작 전 원래 위치
    _vec3               m_vStartPos;  // ACTION이나 EFFECT의 특정 구간 시작 위치
    _float              m_fActionElapsed = 0.f;
    _float              m_fMoveDuration;  // 움직이는 데 걸리는 시간
    _bool               m_bToDesiredPos, m_bInDesiredPos, m_bToOriginPos;   // 목표 위치로 이동 중 / 목표 위치 도착 / 원위치로 이동 중
    _vec3              m_vDesiredPos, m_vDirToDesiredPos;
    _float              m_fRotXToTarget;  // 타겟을 내려다볼 각도
#pragma endregion


#pragma region ACT_FOLLOW
    CTransform* m_pFollowTarget;    // 타겟
    _float             m_fDistZToTarget, m_fDistYToTarget; // 타겟과의 유지 거리
#pragma endregion

#pragma region ACT_DISPLAY
    _float              m_fDelayTime;
    _float              m_fDisplayElapsedTime;      // 보여줄 위치에 도착한 뒤 흐른 시간 

#pragma endregion

#pragma region ACT_CLOSE
    _float              m_fCloseDistY, m_fCloseDistZ;
    _float              m_fRotXCloser;
    _float              m_fElapsedRotXCloser;
#pragma endregion


#pragma region EFT_SHAKE
    _int                  m_iShakeRange;           // 쉐이크 범위
    _float              m_fShakeDuration;       // 쉐이크 시간
    _float              m_fShakeElapsed;
    _bool                m_bDelayShake;
    _vec3               m_vShakeStartPos;       // 쉐이크 시작 위치
#pragma endregion

#pragma region EFT_ZOOM

#pragma endregion



private:
    virtual void Free();

#pragma region TEST
public:
    void Set_Target(CTransform* pTarget) { m_pFollowTarget = pTarget; }
#pragma endregion
};

