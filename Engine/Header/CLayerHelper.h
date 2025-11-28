#pragma once

#include "CBase.h"
#include "Engine_Define.h"
#include "CLayer.h"
#include "CManagement.h"

BEGIN(Engine)

class ENGINE_DLL CLayerHelper final : public CBase
{
    DECLARE_SINGLETON(CLayerHelper)

public:
    explicit CLayerHelper();
    virtual ~CLayerHelper() override;

    inline HRESULT    SetLayerName(const wstring(& wsLayerName)[LAYER_END])
    {
        for (size_t i = 0; i < Engine::LAYER_END; ++i)
        {
            m_tLayerMask[i].wsLayerTag = wsLayerName[i];
        }
        return S_OK;
    }

    inline const wstring&    GetLayerNameByID(LAYERID eLayerID) const
    {
        for (size_t i = 0; i < Engine::LAYER_END; ++i)
        {
            if (m_tLayerMask[i].eLayerID & eLayerID)
            {
                return m_tLayerMask[i].wsLayerTag;
            }
        }

        MSG_BOX("Layer Name Get Failed At CLayerHelper.h");
        return n_wsEmptyLayer;
    }

    inline LAYERID GetLayerIDByName(const wstring& wsLayerName) const
    {
        for (size_t i = 0; i < Engine::LAYER_END; ++i)
        {
            if (m_tLayerMask[i].wsLayerTag == wsLayerName)
            {
                return m_tLayerMask[i].eLayerID;
            }
        }

        MSG_BOX("Layer Name Get Failed At CLayerHelper.h");
        return LAYER_NONE;
    }

    inline CLayer* Get_Layer(const wstring& wsLayerTag)
    {
        CScene* pCurScene = CManagement::GetInstance()->Get_CurScene();

        if (pCurScene == nullptr)
        {
            MSG_BOX("There is No Scene");
            return nullptr;
        }

        if (CLayer* pLayer = pCurScene->Get_Layer(wsLayerTag))
        {
            return pLayer;
        }

        MSG_BOX("Layer Get Failed At CLayerHelper.h");
        return nullptr;
    }

    inline void    ClearLayerName()
    {
        for (size_t i = 0; i < Engine::LAYER_END; ++i)
        {
            m_tLayerMask[i].wsLayerTag.clear();
            m_tLayerMask[i].wsLayerTag = L"Layer" + to_wstring(i);
        }
    }

private:
    LayerMask m_tLayerMask[Engine::LAYER_END] =
    {
        {Engine::LAYER0, L"Layer0"},
        {Engine::LAYER1, L"Layer1"},
        {Engine::LAYER2, L"Layer2"},
        {Engine::LAYER3, L"Layer3"},
        {Engine::LAYER4, L"Layer4"},
        {Engine::LAYER5, L"Layer5"},
        {Engine::LAYER6, L"Layer6"},
        {Engine::LAYER7, L"Layer7"},

        {Engine::LAYER8, L"Layer8"},
        {Engine::LAYER9, L"Layer9"},
        {Engine::LAYER10, L"Layer10"},
        {Engine::LAYER11, L"Layer11"},
        {Engine::LAYER12, L"Layer12"},
        {Engine::LAYER13, L"Layer13"},
        {Engine::LAYER14, L"Layer14"},
        {Engine::LAYER15, L"Layer15"},
    };

    const wstring n_wsEmptyLayer = L"";
private:
    void   Free() override;
};

END
