#pragma once

#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

typedef struct tagLayerTag
{
    wstring     wsLayerName;
    LayerID     eLayerID;
} LayerTag;

class ENGINE_DLL CLayerHelper : public CBase
{
    DECLARE_SINGLETON(CLayerHelper)

public:
    inline HRESULT    SetLayerName(const wstring& wsLayerName, LayerID eLayerID)
    {
        for (size_t i = 0; i < Engine::LAYER_END; ++i)
        {
            if (m_wsLayerTag[i].eLayerID == eLayerID)
            {
                m_wsLayerTag[eLayerID].wsLayerName = wsLayerName;
                return S_OK;
            }
        }

        MSG_BOX("Layer Name Add Failed At CLayerHelper.h");
        return E_FAIL;
    }

    inline const wstring&    GetLayerNameByID(LayerID eLayerID) const
    {
        for (size_t i = 0; i < Engine::LAYER_END; ++i)
        {
            if (m_wsLayerTag[i].eLayerID == eLayerID)
            {
                return m_wsLayerTag[eLayerID].wsLayerName;
            }
        }

        MSG_BOX("Layer Name Get Failed At CLayerHelper.h");
        return L"";
    }

    inline LayerID GetLayerIDByName(const wstring& wsLayerName) const
    {
        for (size_t i = 0; i < Engine::LAYER_END; ++i)
        {
            if (m_wsLayerTag[i].wsLayerName == wsLayerName)
            {
                return m_wsLayerTag[i].eLayerID;
            }
        }

        MSG_BOX("Layer Name Get Failed At CLayerHelper.h");
        return LAYER_NONE;
    }

    inline void    ClearLayerName()
    {
        for (size_t i = 0; i < Engine::LAYER_END; ++i)
        {
            m_wsLayerTag[i].wsLayerName.clear();
            m_wsLayerTag[i].wsLayerName = L"Layer" + to_wstring(i);
        }
    }

private:
    tagLayerTag m_wsLayerTag[Engine::LAYER_END] =
    {
        {L"Layer0", Engine::LAYER0},
        {L"Layer1", Engine::LAYER1},
        {L"Layer2", Engine::LAYER2},
        {L"Layer3", Engine::LAYER3},
        {L"Layer4", Engine::LAYER4},
        {L"Layer5", Engine::LAYER5},
        {L"Layer6", Engine::LAYER6},
        {L"Layer7", Engine::LAYER7}
    };
};

END
