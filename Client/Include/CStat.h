#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CRenderObject.h"

using Callback = void(*)();

class CStat :   public CBase
{
private :
    explicit CStat();
    explicit CStat(Callback OnValueChanged = nullptr, // 값이 변경
                              Callback OnValueMin = nullptr,         // 값이 최솟값
                              Callback OnValueMax = nullptr);       // 값이 최댓값       
    virtual     ~CStat();

public :
    void        Increase(const _float& fValue);
    void        Decrease(const _float& fValue);

    // Current
    _float        Get_CurValue()    const { return m_fCurValue; }
    void            Set_CurValue(const _float& fValue) { m_fCurValue = fValue; }

    // Min
    _float        Get_MinValue()    const { return m_fMinValue; }
    void            Set_MinValue(const _float& fValue) { m_fMinValue = fValue; }

    // Max
    _float        Get_MaxValue()    const { return m_fMaxValue; }
    void            Set_MaxValue(const _float& fValue) { m_fMaxValue = fValue; }

private:
    _float          m_fCurValue,  m_fMinValue, m_fMaxValue;
    Callback      m_OnValueChanged, m_OnValueMin, m_OnValueMax;

public :
    static CStat* Create(CRenderObject* pOwner);

private:
    virtual void Free() override;
};

