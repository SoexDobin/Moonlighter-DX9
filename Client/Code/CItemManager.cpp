#include "pch.h"
#include "CItemManager.h"
#include "CTexture.h"
#include "CPrototypeManager.h"

IMPLEMENT_SINGLETON(CItemMamager)

CItemMamager::CItemMamager()
{
}

CItemMamager::~CItemMamager()
{
    Free();
}

HRESULT CItemMamager::Ready_ItemManager()
{
    Add_Item();

    return S_OK;
}

ITEMDATA* CItemMamager::Get_ItemData(const wstring& wsLayerTag)
{
    auto iter = m_mapItem.find(wsLayerTag);
    if (iter == m_mapItem.end())
        return nullptr;

    return &iter->second;
}

// 아이템 추가 함수
void CItemMamager::Add_Item()
{
    CComponent* pCom(nullptr);

    // 포션
    {
        //pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
        //if (!pCom || pCom->Get_ComponentType() != TEXTURE)
        //    return E_FAIL;
        //
        //ITEMDATA pItemData{};
        //pItemData.m_pTexture = static_cast<CTexture*>(pCom);
        //pItemData.m_pTexture->Ready_Texture(L"");
        //pItemData.m_pTexture->Set_Texture(0, 0);
        //
        //m_umComponent[ID_DANAMIC].insert(pair<wstring, CComponent*>(L"Item_Potion", pItemData.m_pTexture));
        //
        //if (pItemData.m_pTexture)
        //{
        //    pItemData.m_Type = ITEM_POTION;
        //    pItemData.m_iAttack= 0;
        //    pItemData.m_iHp = 0;
        //    pItemData.m_sItName = L"Item_Potion";
        //    pItemData.m_ItemTag = L"Item_Potion";
        //
        //
        //    m_mapItem.emplace(L"Item_Potion", pItemData);
        //}

    }

}

void CItemMamager::Free()
{
    m_mapItem.clear();
}
