#include "pch.h"
#include "CUIInven.h"
#include "CManagement.h"
#include "CRenderer.h"
#include "CTexture.h"
#include "CPrototypeManager.h"
#include "CDInputManager.h"
#include "CLayer.h"

CUIInven::CUIInven(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pTextureCom(nullptr), m_bVisible(false)
{
}

CUIInven::CUIInven(const CUIInven& rhs)
    : CRenderObject(rhs), m_pTextureCom(nullptr), m_bVisible(false)
{
}

CUIInven::~CUIInven()
{
}

HRESULT CUIInven::Ready_GameObject()
{
    if (FAILED(CRenderObject::Ready_GameObject()))
        return E_FAIL;

    // 인벤토리 base
    CComponent* pCom(nullptr); 

    pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
    if (!pCom || pCom->Get_ComponentType() != TEXTURE)
        return E_FAIL;

    m_pTextureCom = static_cast<CTexture*>(pCom);
   
    m_pTextureCom->Ready_Texture(L"Inventory_Base");
    m_pTextureCom->Set_Texture(0);

    m_umComponent[ID_STATIC].insert(pair<wstring, CComponent*>(L"Inventory_Base", m_pTextureCom));

    m_pTransformCom->Set_Pos(WINCX * 0.5f, WINCY * 0.5f, 0.f);
    m_pTransformCom->Set_Scale(460.f * 2.f,  -480.f * 2.f, 0.f);

    // 슬롯
    Create_Slots(m_pGraphicDevice);
   

    return S_OK;
}

_int CUIInven::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);

    if (!m_bVisible)
    {
        return 0;
    }

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

    

    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
    m_pTextureCom->Set_Texture(0); // 세팅, 바인딩(
    m_pBufferCom->Render_Buffer();

}
// 슬롯 위치 선정
void CUIInven::Create_Slots(LPDIRECT3DDEVICE9 pGraphicDev)
{
    
    m_vecSlots.clear();
    m_vecSlots.reserve(2);
    
     // 패널 중심좌표
    const _vec3 vPanelPos = m_pTransformCom->Get_Pos(); // INFO_POS 가져오기

    _float fposX = -522.f; // 시작 슬롯 X 위치
    _float fposY = -522.f;  // 시작 슬롯 Y 위치
    _float fInterX = 30.f; // 슬롯 간격(X축)
    _float fInterY = 30.f; // 슬롯 간격(Y축)

    CUIInvenSlot* pSlot = CUIInvenSlot::Create(pGraphicDev);
    if (!pSlot) { return; }

    pSlot->Set_Pos(WINCX * 0.5f , WINCY * 0.5f);
    pSlot->SlotButton();
    m_vecSlots.push_back(pSlot);

    if (FAILED(Engine::CManagement::GetInstance()
        ->Add_GameObject(L"UI_Layer", L"UI_InvenSlot", pSlot)))
    {
        Safe_Release(pSlot);
        return;
    }


   
    // 상단5개

    for (int j = 0; j < 5; j++)
    {
        fposY += (54 + fInterY) * j; 
        CUIInvenSlot* pSlot = CUIInvenSlot::Create(pGraphicDev);
        pSlot->Set_Pos(fposX, fposY);
        m_vecSlots.push_back(pSlot);
    }

    fposY += 100.f; // 중간 넓어진 간격

    // 하단 15개
    for (int i = 0; i < 2; i++)
    {
        fposY += 54 * i;

        for (int j = 1; j <= 5; j++)
        {
            fposY = (54 + fInterY) * j;
            CUIInvenSlot* pSlot = CUIInvenSlot::Create(pGraphicDev);
            pSlot->Set_Pos(fposX, fposY);
            m_vecSlots.push_back(pSlot);
        }
    }
    }


void CUIInven::UI_KeyInput(const _float& fTimeDelta)
{

    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_Q) & 0x80 )
    {

    }

    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_E) & 0x80)
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
    for (auto& pSlot : m_vecSlots)
        Safe_Release(pSlot);
    
    m_vecSlots.clear();
    
    Engine::CRenderObject::Free();
}

