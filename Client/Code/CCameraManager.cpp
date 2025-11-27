#include "pch.h"
#include "CCameraManager.h"
#include "CDInputManager.h"
#include "EasingFunctions.h"
#include "CPanel.h"
#include "CEditor.h"

IMPLEMENT_SINGLETON(CCameraManager)

CCameraManager::CCameraManager()
{
}

CCameraManager::~CCameraManager()
{
    Free();
}

HRESULT CCameraManager::Ready_Camera(LPDIRECT3DDEVICE9 pGraphicDev)
{
    m_fFov = 60.f;
    m_fAspect = (float)WINCX / WINCY;
    m_fNear = 1.f;
    m_fFar = 1000.f;

    _vec3 vEye, vAt, vUp;

    vEye = { 0.f, 8.f, -10.f };
    vAt = { 0.f, 0.f, 0.f };
    vUp = { 0.f, 1.f, 0.f };

    // 디버깅용 카메라
    m_pDebugCam = CCamera::Create(pGraphicDev,
        &vEye, &vAt, &vUp,
        D3DXToRadian(m_fFov), m_fAspect, m_fNear, m_fFar,
        Engine::CCamera::PROJ_PERSPECTIVE);
    m_pDebugCam->Set_Pos(vEye);

    // 인게임용 카메라
    m_pInGameCam = CCamera::Create(pGraphicDev,
        &vEye, &vAt, &vUp,
        D3DXToRadian(m_fFov), m_fAspect, m_fNear, m_fFar,
        Engine::CCamera::PROJ_PERSPECTIVE);

    m_pCurCam = m_pInGameCam;

    m_eMode = CAMERA_MODE::INGAME;
    m_eAction = CAMERA_ACTION::ACTION_END;
    m_eEffect = CAMERA_EFFECT::EFFECT_NONE;

    m_fSpeed = 7.f;
    m_fRotXToTarget = 20.f;
    m_fDistZToTarget = 7.f, m_fDistYToTarget = 5.f; // 타겟과의 유지 거리

    // ====== FOLLOW 테스트 ======
    Change_ToFollow(nullptr);

    // ====== DISPLAY 테스트 ======
    //Change_ToDisplay({ 15.f, 25.f, 15.f }, 20.f, 2.f);
    //m_eAction = CAMERA_ACTION::ACTION_DISPLAY;

    // ====== ACTION_CLOSER 테스트 ======
    //Change_ToCloser(1.f);

    CEditor::GetInstance()->On_DebugCam = [&]() {Callback_OnDebugCam(); };
    CEditor::GetInstance()->Act_DebugCam = [&]() {Callback_DoDebugCam(); };
    CEditor::GetInstance()->Off_DebugCam = [&]() {Callback_OffDebugCam(); };

    return S_OK;
}

_int CCameraManager::Update_Camera(const _float fTimeDelta)
{
    // 인게임
    if (CAMERA_MODE::INGAME == m_eMode)
    {
        Update_InGameCamera(fTimeDelta);
    }
    // Transform 컴포넌트 갱신
    m_pCurCam->Update_GameObject(fTimeDelta);

    {
        // ======================= 카메라 전환 테스트 =======================
        if (GetAsyncKeyState('P') & 0x0001)
        {
            Change_ToDisplay({ 10.f, 8.f, 10.f }, 2.f, 1.f);
        }
        if (GetAsyncKeyState('O') & 0x0001)
        {
            Change_ToCloser(1.f);
        }
        if (GetAsyncKeyState('I') & 0x0001)
        {
            Exit_Closer();
        }
        if (GetAsyncKeyState('U') & 0x0001)
        {
            Change_ToFollow(nullptr);
        }

        // ====================== 카메라 효과 테스트 ===========================
        if (GetAsyncKeyState('L') & 0x0001)
        {
            Start_Shake(1.f, 0.1f);
        }

    }

    return 0;
}

void CCameraManager::LateUpdate_Camera(const _float fTimeDelta)
{
    m_pCurCam->LateUpdate_GameObject(fTimeDelta);

}

void CCameraManager::Callback_OnDebugCam()
{
    m_eMode = CAMERA_MODE::DBG_PERSPECTIVE;
    m_pCurCam = m_pDebugCam;
    m_pCurCam->Set_Pos(m_pInGameCam->Get_Pos());
}

void CCameraManager::Callback_DoDebugCam()
{
    const _float fTimeDelta = 0.016;
    Handle_Input(fTimeDelta);
    m_pDebugCam->Update_GameObject(fTimeDelta);
    m_pDebugCam->LateUpdate_GameObject(fTimeDelta);
}

void CCameraManager::Callback_OffDebugCam()
{
    m_eMode = CAMERA_MODE::INGAME;

    m_pCurCam = m_pInGameCam;
}

void CCameraManager::Update_InGameCamera(const _float& fTimeDelta)
{
    switch (m_eAction)
    {
    case CCameraManager::ACTION_FOLLOW:
        Act_Follow(fTimeDelta);
        break;
    case CCameraManager::ACTION_DISPLAY:
        Act_Display(fTimeDelta);
        break;
    case CCameraManager::ACTION_CLOSER:
        Act_Closer(fTimeDelta);
        break;
    default:
        Act_Follow(fTimeDelta);
        break;
    }

    switch (m_eEffect)
    {
    case CCameraManager::EFFECT_NONE:
        return;
        break;
    case CCameraManager::EFFECT_SHAKE:
        Shaking(fTimeDelta);
        break;
    case CCameraManager::EFFECT_ZOOM:
        break;
    }

}

/// <summary>
/// 플레이어를 추적하는 카메라
/// </summary>
/// <param name="pTarget">추적할 타겟 주소</param>
void CCameraManager::Change_ToFollow(CTransform* pTarget)
{
    Finish_CurrentMode();
    // 원래 로직이지만, 현재 씬 구조상 카메라매니저 생성 시  플레이어를 받아올 수 없어 직접 설정
    //m_pFollowTarget = pTarget;
    //_vec3 vCamPos = pTarget->Get_Pos();

    if (nullptr == m_pFollowTarget)
        return;

    _vec3 vCamPos = m_pFollowTarget->Get_Pos();
    vCamPos.z -= m_fDistZToTarget;
    vCamPos.y += m_fDistYToTarget;
    m_pInGameCam->Set_Pos(vCamPos);

    m_pInGameCam->Rotate(ROT_X, D3DXToRadian(m_fRotXToTarget));

    m_eAction = CAMERA_ACTION::ACTION_FOLLOW;
}

/// <summary>
/// 특정 위치를 보여준 뒤 복귀하는 카메라
/// </summary>
/// <param name="vDisplayPos">보여줄 위치</param>
/// <param name="fSpeed">이동할 속도</param>
/// <param name="fDelayTime">대기할 시간</param>
void CCameraManager::Change_ToDisplay(const _vec3& vDisplayPos, const _float& fMoveDuration, const _float& fDelayTime)
{
    Finish_CurrentMode();
    // 사용될 변수 초기화
    m_vStartPos = m_vOriginPos = m_pCurCam->Get_Pos();
    m_vDesiredPos = vDisplayPos;
    m_vDirToDesiredPos = m_vDesiredPos - m_pCurCam->Get_Pos();
    D3DXVec3Normalize(&m_vDirToDesiredPos, &m_vDirToDesiredPos);

    m_fMoveDuration = fMoveDuration;
    m_fDelayTime = fDelayTime;

    m_bToDesiredPos = true;
    m_bInDesiredPos = m_bToOriginPos = false;

    m_fActionElapsed = m_fDisplayElapsedTime = 0.f;

    m_pInGameCam->Rotate(ROT_X, D3DXToRadian(m_fRotXToTarget));

    m_eAction = CAMERA_ACTION::ACTION_DISPLAY;
}

/// <summary>
/// 플레이어를 다른 각도에서 더 가깝게 비추는 카메라.
/// 사용 후에 Exit_Closer()를 호출해야 한다.
/// </summary>
/// <param name="fMoveDuration">이동 시간</param>
void CCameraManager::Change_ToCloser(const _float& fMoveDuration)
{
    Finish_CurrentMode();
    // 사용될 변수 초기화
    m_vOriginPos = m_vStartPos = m_pCurCam->Get_Pos();
    m_fRotXCloser = -10.f;
    // 플레이어에게 다가가기
    m_fCloseDistY = 10.f, m_fCloseDistZ = 10.f;
    m_vDesiredPos = { m_vOriginPos.x, m_vOriginPos.y - m_fCloseDistY, m_vOriginPos.z + m_fCloseDistZ };
    m_vDirToDesiredPos = m_vDesiredPos - m_vOriginPos;

    D3DXVec3Normalize(&m_vDirToDesiredPos, &m_vDirToDesiredPos);

    m_fMoveDuration = fMoveDuration;

    m_bToDesiredPos = true;
    m_bInDesiredPos = m_bToOriginPos = false;

    m_fActionElapsed = 0.f;
    m_fElapsedRotXCloser = 0.f;

    m_pInGameCam->Rotate(ROT_X, D3DXToRadian(m_fRotXToTarget));

    m_eAction = CAMERA_ACTION::ACTION_CLOSER;
}

void CCameraManager::Exit_Closer()
{
    m_bToDesiredPos = false;
    m_bToOriginPos = true;
}

void CCameraManager::Act_Follow(const _float& fTimeDelta)
{
    if (nullptr == m_pFollowTarget)
        return;

    // Camera Lag 효과 적용(반박자 늦게 따라가는 효과)
    _vec3 vTargetPos = m_pFollowTarget->Get_Pos();
    vTargetPos.z -= m_fDistZToTarget;
    vTargetPos.y += m_fDistYToTarget;

    _vec3 vCamPos = m_pCurCam->Get_Pos();
    _vec3 vDirToTarget = vTargetPos - vCamPos;

    // m_fSpeed 값으로 따라가는 속도 조절 
    m_pInGameCam->Set_Pos(vCamPos + vDirToTarget * m_fSpeed * fTimeDelta);
}

void CCameraManager::Act_Display(const _float& fTimeDelta)
{
    // 목표 지점으로 이동 
    if (m_bToDesiredPos)
    {
        m_fActionElapsed += fTimeDelta;
        float t = m_fActionElapsed / m_fMoveDuration;

        if (t > 1.f)
            t = 1.f;

        t = Ease::OutCubic(t);

        _vec3 vNewPos = *D3DXVec3Lerp(&vNewPos, &m_vStartPos, &m_vDesiredPos, t);
        m_pCurCam->Set_Pos(vNewPos);

        if (t >= 1.f)
        {
            m_bToDesiredPos = false;
            m_bInDesiredPos = true;

            m_fActionElapsed = 0.f;
        }
        return;
    }

    // 목표 지점에서 대기 
    if (m_bInDesiredPos)
    {
        m_fDisplayElapsedTime += fTimeDelta;

        if (m_fDisplayElapsedTime >= m_fDelayTime)
        {
            // 원래 위치로 돌아가기 준비
            m_bInDesiredPos = false;
            m_bToOriginPos = true;

            m_vStartPos = m_pCurCam->Get_Pos();
            m_fActionElapsed = 0.f;
        }

        return;
    }

    // 시작 위치로 복귀
    if (m_bToOriginPos)
    {
        m_fActionElapsed += fTimeDelta;
        float t = m_fActionElapsed / m_fMoveDuration;

        if (t > 1.f) t = 1.f;

        t = Ease::OutQuad(t);

        D3DXVECTOR3 vNewPos;
        D3DXVec3Lerp(&vNewPos, &m_vStartPos, &m_vOriginPos, t);
        m_pInGameCam->Set_Pos(vNewPos);

        if (t >= 1.f)
        {
            m_bToOriginPos = false;

            // ACTION_DISPLAY 모드 종료 시 ACTIOM_FOLLOW 모드로 자동 전환
            Change_ToFollow(nullptr/*m_pFollowTarget*/);
        }

        return;
    }
}

void CCameraManager::Act_Closer(const _float& fTimeDelta)
{
    // 목표 위치로 이동 중
    if (m_bToDesiredPos)
    {
        m_fActionElapsed += fTimeDelta;
        _float t = m_fActionElapsed / m_fMoveDuration;

        t = Ease::OutQuint(t);
        if (t >= 1.f)
            t = 1.f;

        // 위치 이동
        _vec3 vNewPos = *D3DXVec3Lerp(&vNewPos, &m_vStartPos, &m_vDesiredPos, t);
        m_pCurCam->Set_Pos(vNewPos);

        // 시점 올리기
        m_pInGameCam->Rotate(ROT_X, D3DXToRadian(-m_fElapsedRotXCloser));
        m_fElapsedRotXCloser = m_fRotXCloser * t;
        m_pInGameCam->Rotate(ROT_X, D3DXToRadian(m_fElapsedRotXCloser));

        if (t >= 1.f)
        {
            m_bToDesiredPos = false;
            m_bInDesiredPos = true;
            m_vStartPos = m_pCurCam->Get_Pos();
            m_fActionElapsed = m_fElapsedRotXCloser = 0.f;
            m_fRotXCloser *= -1.f;
        }
        return;
    }

    // 원위치로 이동 중 
    if (m_bToOriginPos)
    {
        m_fActionElapsed += fTimeDelta;

        _float t = m_fActionElapsed / m_fMoveDuration;

        t = Ease::InOutCubic(t);
        if (t >= 1.f)
            t = 1.f;

        _vec3 vNewPos = *D3DXVec3Lerp(&vNewPos, &m_vStartPos, &m_vOriginPos, t);
        m_pCurCam->Set_Pos(vNewPos);

        m_pInGameCam->Rotate(ROT_X, D3DXToRadian(-m_fElapsedRotXCloser));
        m_fElapsedRotXCloser = m_fRotXCloser * t;
        m_pInGameCam->Rotate(ROT_X, D3DXToRadian(m_fElapsedRotXCloser));

        if (t >= 1.f)
        {
            // 원위치로 이동 후 기본 ATCTION_FOLLODW 모드로 전환
            Change_ToFollow(m_pFollowTarget);
            m_fActionElapsed = 0.f;
        }
        return;
    }
}

void CCameraManager::Finish_CurrentMode()
{
    switch (m_eAction)
    {
    case CCameraManager::ACTION_FOLLOW:
        Finish_Follow();
        break;
    case CCameraManager::ACTION_DISPLAY:
        Finish_Display();
        break;
    case CCameraManager::ACTION_CLOSER:
        Finish_Closer();
        break;
    }
}

void CCameraManager::Finish_Follow()
{
    // 종료 시 카메라가 회전한 만큼 빼줘야 다음 회전시 원점에서 회전 시작
    m_pInGameCam->Rotate(ROT_X, D3DXToRadian(-m_fRotXToTarget));
}

void CCameraManager::Finish_Display()
{
    // 종료 시 카메라가 회전한 만큼 빼줘야 다음 회전시 원점에서 회전 시작
    m_pInGameCam->Rotate(ROT_X, D3DXToRadian(-m_fRotXToTarget));
}

void CCameraManager::Finish_Closer()
{
    // 종료 시 카메라가 회전한 만큼 빼줘야 다음 회전시 원점에서 회전 시작
    m_pInGameCam->Rotate(ROT_X, D3DXToRadian(-m_fRotXToTarget));
}

void CCameraManager::Start_Shake(const _float& fRange, const _float& fDuration)
{
    m_iShakeRange = fRange;
    m_fShakeDuration = fDuration;
    m_fShakeElapsed = 0.f;
    m_vShakeStartPos = m_pCurCam->Get_Pos();
    m_eEffect = CAMERA_EFFECT::EFFECT_SHAKE;
}

void CCameraManager::Shaking(const _float& fTimeDelta)
{
    m_fShakeElapsed += fTimeDelta;

    _float offsetX = (rand() / (float)RAND_MAX * 2.f - 1.f) * m_iShakeRange;
    _float offsetY = (rand() / (float)RAND_MAX * 2.f - 1.f) * m_iShakeRange;


    _vec3 vNewPos = m_vShakeStartPos;
    vNewPos.x += offsetX;
    vNewPos.y += offsetY;
    m_pCurCam->Set_Pos(vNewPos);

    if (m_fShakeElapsed > m_fShakeDuration)
    {
        m_eEffect = CAMERA_EFFECT::EFFECT_NONE;
    }
}

void CCameraManager::Handle_Input(const _float& fTimeDelta)
{
#pragma region KEYBOARD
    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_W) & 0x80)
    {
        m_pDebugCam->Translate(Engine::INFO::INFO_LOOK, fTimeDelta, m_fSpeed);  // 전
    }
    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_S) & 0x80)
    {
        m_pDebugCam->Translate(Engine::INFO::INFO_LOOK, fTimeDelta, -m_fSpeed); // 후
    }
    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_A) & 0x80)
    {
        m_pDebugCam->Translate(Engine::INFO::INFO_RIGHT, fTimeDelta, -m_fSpeed);    // 좌
    }
    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_D) & 0x80)
    {
        m_pDebugCam->Translate(Engine::INFO::INFO_RIGHT, fTimeDelta, m_fSpeed);     // 우
    }
#pragma endregion

#pragma region MOUSE
    _long lMouseMove;
    if (lMouseMove = CDInputManager::GetInstance()->Get_DIMouseMove(MOUSEMOVESTATE::DIMS_X))
    {
        m_pDebugCam->Rotate(ROTATION::ROT_Y, D3DXToRadian(lMouseMove / 10.f));
    }
    if (lMouseMove = CDInputManager::GetInstance()->Get_DIMouseMove(MOUSEMOVESTATE::DIMS_Y))
    {
        m_pDebugCam->Rotate(ROTATION::ROT_X, D3DXToRadian(lMouseMove / 10.f));
    }
#pragma endregion

}

void CCameraManager::Free()
{
    m_pInGameCam->Release();
    m_pDebugCam->Release();
}
