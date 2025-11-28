#include "pch.h"
#include "CUIInven.h"
#include "CManagement.h"
#include "CRenderer.h"
#include "CTexture.h"
#include "CPrototypeManager.h"
#include "CDInputManager.h"


CUIInven::CUIInven(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pTextureCom(nullptr), m_bVisible(true)
{
}

CUIInven::CUIInven(const CUIInven& rhs)
    : CRenderObject(rhs), m_pTextureCom(nullptr) , m_bVisible(true)
{
}

CUIInven::~CUIInven()
{
}

HRESULT CUIInven::Ready_GameObject()
{
    if (FAILED(CRenderObject::Ready_GameObject()))
        return E_FAIL;

    Set_RenderPriority(UI_INVENTORY);

    // 인벤토리 base
    CComponent* pCom(nullptr); 

    pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
    if (!pCom || pCom->Get_ComponentType() != TEXTURE)
        return E_FAIL;

    m_pTextureCom = static_cast<CTexture*>(pCom);
   
    m_pTextureCom->Ready_Texture(L"Inventory_Base");
    m_pTextureCom->Ready_Texture(L"HoverSlot");
    m_pTextureCom->Ready_Texture(L"ClickSlot");
    m_pTextureCom->Set_Texture(0);

    m_umComponent[ID_STATIC].insert(pair<wstring, CComponent*>(L"Inventory", m_pTextureCom));


    m_pTransformCom->Set_Pos(m_vPanelPos.x, m_vPanelPos.y, 0.f);
    m_pTransformCom->Set_Scale(m_fPanelWidth, -m_fPanelHeight, 0.f); // 이미지가 반대로 출력해서 (-) 해둠

   
    // 슬롯
    Create_Slots();
    return S_OK;
}

_int CUIInven::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);

    if (!m_bVisible)
    {
        return 0;
    }

    // 마우스 Hover
    Slot_Hover(fTimeDelta);

    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

    return iExit;
}

void CUIInven::LateUpdate_GameObject(const _float fTimeDelta)
{
    if (!m_bVisible)
    {
        return;
    }

    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);

}

void CUIInven::Render_GameObject()
{
    if (!m_bVisible)
    {
        return ;
    }

    // 패널
    m_pTransformCom->Set_Pos(m_vPanelPos.x, m_vPanelPos.y, 0.f);
    m_pTransformCom->Set_Scale(m_fPanelWidth, -m_fPanelHeight, 0.f);
    m_pTransformCom->Update_Component(0.f); // 갱신
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
    m_pTextureCom->Set_Texture(0); // 세팅, 바인딩(
    m_pBufferCom->Render_Buffer();

    // 슬롯(아이템 아직 안넣음)
    for (auto& tSlot : m_vecSlots)
    {
        RECT rc = Set_SlotRect(tSlot);

        const _float fCenterX = rc.left + m_fSlotW * 0.5f;
        const _float fCenterY = rc.top + m_fSlotH * 0.5f;

        // 마우스 호버(작아짐)
        const _float fScale = 1.0f - tSlot.fHoverLerp;

        _vec3 vPos = { fCenterX, fCenterY, 0.f };
        _vec3 vScale = { m_fSlotW * fScale, m_fSlotH * fScale, 1.f };

        m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
        m_pTransformCom->Set_Scale(vScale.x, vScale.y, vScale.z);

        m_pTransformCom->Update_Component(0.f);
        m_pTextureCom->Set_Texture(1);

        if (tSlot.estate == SLOT_PRESSED)
        {
            m_pTextureCom->Set_Texture(2);
        }

        m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
        m_pBufferCom->Render_Buffer();
        

    }


}

void CUIInven::Create_Slots()
{

    const _int iTotal = m_iCols * m_iRows;

    m_vecSlots.clear();
    m_vecSlots.reserve(iTotal);

    // 윗줄
    for (_int i = 0; i < 5; ++i)
    {

        UISLOT pSlot{};
        pSlot.iIndex = i;
        pSlot.estate = SLOT_NORMAL;
        pSlot.iItemID = 0;
        pSlot.iCount = 0;

        m_vecSlots.push_back(pSlot);
    }

    // 아래 칸
    for (_int i = 5; i < iTotal; ++i)
    {
        UISLOT pSlot{};
        pSlot.iIndex = i;
        pSlot.estate = SLOT_NORMAL;
        pSlot.iItemID = 0;
        pSlot.iCount = 0;

        m_vecSlots.push_back(pSlot);
    }

 
  }
// 슬롯 위치 선정
RECT CUIInven::Set_SlotRect(const UISLOT& pSlot)
{
    // 패널의 왼쪽 위 기준점
    const _float fPanelLeft = m_vPanelPos.x - m_fPanelWidth * 0.5f;
    const _float fPanelTop = m_vPanelPos.y - m_fPanelHeight * 0.5f + 220.f; // 텍스쳐 사이즈 때문에 추가 조정

    const _int   idx = pSlot.iIndex;
    const _int   x = idx % m_iCols;
    const _int   y = idx / m_iCols;

    // 패널 첫 위치
    const _float fStartX = fPanelLeft + 86.f; 
    const _float fStartY = fPanelTop + 60.f;

    const _float fSlotX = fStartX + x * (m_fSlotW + m_fInterX);

    if (idx < 5)
    {
        const _float fSlotY = fStartY + y * (m_fSlotH + m_fInterY1);

        LONG left = (LONG)fSlotX;
        LONG top = (LONG)fSlotY;
        LONG right = (LONG)(fSlotX + m_fSlotW);
        LONG bottom = (LONG)(fSlotY + m_fSlotH);

        return RECT{ left, top, right, bottom };
    }

    if ( 5 <= idx && idx < m_iCols * m_iRows)
    {
        const _float fSlotY = fStartY + y * (m_fSlotH + m_fInterY2)  + 12.f;

        LONG left = (LONG)fSlotX;
        LONG top = (LONG)fSlotY;
        LONG right = (LONG)(fSlotX + m_fSlotW);
        LONG bottom = (LONG)(fSlotY + m_fSlotH);

        return RECT{ left, top, right, bottom };
    }
}
// 마우스 hover
void CUIInven::Slot_Hover(const _float& fTimeDelta)
{
    POINT ptMouse{};
    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    _byte bLeftClick = CDInputManager::GetInstance()->Get_DIMouseState(DIM_LB);

    for (auto& ptslot : m_vecSlots)
    {
        RECT rc = Set_SlotRect(ptslot);

        //PtInRect : 지정된 사각형에 있는지 결정하는 bool 타입 함수
        if (PtInRect(&rc, ptMouse))
        {
            ptslot.estate = SLOT_HOVER;
            ptslot.fHoverLerp = 0.2f;

            if (bLeftClick & 0x8000)
            {
                ptslot.estate = SLOT_PRESSED;
                m_bClick = true;
                Slot_Click(fTimeDelta);
            }
            else
            {
                m_bClick = false;
            }
        }
        else
        {
            ptslot.estate = SLOT_NORMAL;
            ptslot.fHoverLerp = 0.f;

        }
    }

}

void CUIInven::Slot_Click(const _float& fTimeDelta)
{

}


void CUIInven::UI_KeyInput(const _float& fTimeDelta)
{

    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_Q) & 0x80 )
    {

    }



}

// 인벤토리 온오프 확인용
void CUIInven::InvenButton()
{
   m_bVisible =  !m_bVisible;

}

CUIInven* CUIInven::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CUIInven* pUIMenu = new CUIInven(pGraphicDev);

    if (FAILED(pUIMenu->Ready_GameObject()))
    {
        MSG_BOX("UI_Menu Create Failed");
        Safe_Release(pUIMenu);
        return nullptr;
    }

    return pUIMenu;
}
void CUIInven::Free()
{
    m_vecSlots.clear();
    
    Engine::CRenderObject::Free();
}

