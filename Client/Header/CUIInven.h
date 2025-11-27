#pragma once
#include "CRenderObject.h"
#include "Client_Define.h"

namespace Engine { class CTexture; }

class CUIInven : public CRenderObject
{
private:
    explicit CUIInven(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CUIInven(const CUIInven& rhs);
    virtual ~CUIInven();

public:
    virtual			HRESULT		Ready_GameObject()override;
    virtual			_int		Update_GameObject(const _float fTimeDelta)override;
    virtual			void		LateUpdate_GameObject(const _float fTimeDelta)override;
    virtual			void		Render_GameObject()override;

public:
    void          InvenButton();

private:
    CTexture*     m_pTextureCom;
    _bool         m_bVisible; // 창 켜짐 여부
    _bool         m_bClick = false;
    void          UI_KeyInput(const _float& fTimeDelta);


public:
    static CUIInven* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free() override;

    // 슬롯 관련 정보
private:
    // 패널 정보
    _vec2   m_vPanelPos = _vec2 (WINCX * 0.5f , WINCY * 0.5f); // 패널 중앙값
    _float  m_fPanelWidth = 460.f * 2.1f;    // 패널 크기
    _float  m_fPanelHeight = 480.f * 2.f;


    //슬롯
    _int    m_iCols = 5;      
    _int    m_iRows = 4;       
    _float  m_fSlotW = 72.f * 0.74f ;    // 슬롯 네모 크기
    _float  m_fSlotH = 72.f * 0.74f;
    _float  m_fInterX = 13.f;    // 슬롯 사이 간격
    _float  m_fInterY1 = 20.f;   // 위 간격
    _float  m_fInterY2 = 13.5f;  // 아래 간격

private:
    void         Create_Slots();
    RECT         Set_SlotRect(const UISLOT& pslot);
    vector<UISLOT>   m_vecSlots;

// hover
private:
    void        Slot_Hover(const _float& fTimeDelta);
    void        Slot_Click(const _float& fTimeDelta);

};

