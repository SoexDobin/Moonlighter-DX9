#pragma once
#include "CEntityComponent.h"
#include "CGameObject.h"

using Callback = std::function<void()>;

class CStatComponent :  public CEntityComponent
{
private :
    explicit CStatComponent(CGameObject* pOwner,
                                                const _float& fCur,
                                                const _float& fMin,
                                                const _float& fMax);
    explicit CStatComponent(const CStatComponent& rhs);
    virtual     ~CStatComponent();

public :
    void        Increase(const _float& fValue) { Set_Value(m_fCurValue + fValue); }
    void        Decrease(const _float& fValue) { Set_Value(m_fCurValue - fValue); }

    void        Set_Value(const _float& fValue);

    // Current
    _float        Get_CurValue()    const { return m_fCurValue; }
    void            Set_CurValue(const _float& fValue) { m_fCurValue = fValue; }

    // Min
    _float        Get_MinValue()    const { return m_fMinValue; }
    void            Set_MinValue(const _float& fValue) { m_fMinValue = fValue; }

    // Max
    _float        Get_MaxValue()    const { return m_fMaxValue; }
    void            Set_MaxValue(const _float& fValue) { m_fMaxValue = fValue; }

public:
    Callback      m_OnValueChanged, m_OnValueMin, m_OnValueMax;

private:
    _float          m_fCurValue,  m_fMinValue, m_fMaxValue;

public :
    static CStatComponent* Create(CGameObject* pOwner,
                                                             const _float& fCur = 0.f,
                                                             const _float& fMin = 0.f,
                                                             const _float& fMax = 0.f);
public :
    virtual CComponent* Clone() override;

private:
    virtual void Free() override;
};

