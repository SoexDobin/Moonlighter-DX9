#pragma once
#include "CComponent.h"

class CTmpCustomComponent : public CComponent
{
private:
    explicit CTmpCustomComponent();
    explicit CTmpCustomComponent(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTmpCustomComponent(const CTmpCustomComponent& rhs);
    virtual ~CTmpCustomComponent() override;

public:
    virtual _int				Update_Component(const _float fTimeDelta) override;
    virtual void				LateUpdate_Component() override;


public:
    static CTmpCustomComponent* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual CComponent* Clone();

protected:
    virtual void				Free() override;

#pragma region Editor
public:
    virtual void Display_Editor(const char* pObjTag) override;
#pragma endregion
};

