#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

typedef struct ENGINE_DLL TagLayerMask
{
    LAYERID             eLayerID = LAYER_NONE;
    std::wstring        wsLayerTag = L"";
    _uint16             iMask = 0xFFFF;
    std::wstring        wsObjectKey = L"";

public:
    void Add_Mask(const std::wstring& wsLayerTag);
    void Sub_Mask(const std::wstring& wsLayerTag);
} LayerMask;

END
