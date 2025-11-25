#include "pch.h"
#include "CStatComponent.h"

CStatComponent::CStatComponent(CGameObject* pOwner, const _float& fCur, const _float& fMin,  const _float& fMax)
    : CEntityComponent(pOwner)
    , m_fCurValue(fCur), m_fMinValue(fMin), m_fMaxValue(fMax)
    , m_OnValueChanged(nullptr), m_OnValueMin(nullptr), m_OnValueMax(nullptr)

{
    m_eComType = ENTITY_COM::STAT;
}

CStatComponent::CStatComponent(const CStatComponent& rhs)
    : CEntityComponent(rhs)
    , m_fCurValue(rhs.m_fCurValue), m_fMinValue(rhs.m_fMinValue), m_fMaxValue(rhs.m_fMaxValue)
    , m_OnValueChanged(rhs.m_OnValueChanged), m_OnValueMin(rhs.m_OnValueMin), m_OnValueMax(rhs.m_OnValueMax)

{
}

CStatComponent::~CStatComponent()
{
}

void CStatComponent::Set_Value(const _float& fValue)
{
    // 값 min ~ max 사이로 보정
    _float fClamped = fValue > m_fMaxValue ? m_fMaxValue : fValue < m_fMinValue ? m_fMinValue : fValue;

    // 값 변경되지 않은 경우 실행하지 않음
    if (m_fCurValue == fClamped)
        return;

    m_fCurValue = fClamped;

    // 값 변경 시
    if (m_OnValueChanged)
        m_OnValueChanged();

    // 최솟값 도달 시
    if (fClamped == m_fMinValue && m_OnValueMin)
        m_OnValueMin();

    // 최댓값 도달 시 
    if (fClamped == m_fMaxValue && m_OnValueMax)
        m_OnValueMax();
}

CStatComponent* CStatComponent::Create(CGameObject* pOwner, const _float& fCur, const _float& fMin,  const _float& fMax)
{
    return new CStatComponent(pOwner, fCur, fMin, fMax);
}

CComponent* CStatComponent::Clone()
{
    return new CStatComponent(*this);
}

void CStatComponent::Free()
{
    CComponent::Free();
}
