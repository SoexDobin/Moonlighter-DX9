#include "pch.h"
#include "CStat.h"

CStat::CStat()
    : m_fCurValue(0.f), m_fMinValue(0.f), m_fMaxValue(0.f)
    , m_OnValueChanged(nullptr), m_OnValueMin(nullptr), m_OnValueMax(nullptr)
{
}

CStat::CStat(Callback OnValueChanged, Callback OnValueMin, Callback OnValueMax)
    : m_fCurValue(0.f), m_fMinValue(0.f), m_fMaxValue(0.f)
    , m_OnValueChanged(OnValueChanged), m_OnValueMin(OnValueMin), m_OnValueMax(OnValueMax)
{

}

CStat::~CStat()
{
}

void CStat::Increase(const _float& fValue)
{
    _float fRes = (m_fCurValue + fValue > m_fMaxValue) ? m_fMaxValue : (m_fCurValue + m_fCurValue < m_fMinValue) ? m_fMinValue : m_fCurValue + fValue;

    if (fRes != m_fCurValue && nullptr != m_OnValueChanged)
    {
        m_fCurValue = fRes;
        m_OnValueChanged();
        return;
    }
    if (fRes >= m_fMaxValue && nullptr != m_OnValueMax)
    {
        m_fCurValue = fRes;
        m_OnValueMax();
    }
    if (fRes <= m_fMinValue && nullptr != m_OnValueMin)
    {
        m_fCurValue = fRes;
        m_OnValueMin();
    }
}

void CStat::Decrease(const _float& fValue)
{
}

CStat* CStat::Create(CRenderObject* pOwner)
{
    return nullptr;
}

void CStat::Free()
{
}
