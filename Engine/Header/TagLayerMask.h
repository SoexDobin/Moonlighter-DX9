#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

// 충돌 레이어 비교를 위한 CGameObject의 레이어 설정 구조체 
typedef struct ENGINE_DLL TagLayerMask
{
    LAYERID             eLayerID = LAYER_NONE;      // Engine에서 자신의 레이어 Layer ID
    std::wstring        wsLayerTag = L"";           // Client에서 자신의 레이어 Layer Tag
    _uint16             iMask = 0xFFFF;             // Engine에서 비교에 사용할 레이어 그룹 
    std::wstring        wsObjectKey = L"";          // Add_GameObject시 레이어에 등록된 오브젝트 키

public:
    void Add_Mask(const std::wstring& wsLayerTag);  // iMask에 비교 레이어 추가
    void Sub_Mask(const std::wstring& wsLayerTag);  // iMask에 비교 레이어 제거
} LayerMask;

END
