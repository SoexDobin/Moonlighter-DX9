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
    Ready_Item();

    return S_OK;
}

ITEMDATA* CItemMamager::Get_ItemData(const wstring& wsLayerTag)
{
    auto iter = m_mapItem.find(wsLayerTag);
    if (iter == m_mapItem.end())
        return nullptr;

    return &iter->second;
}


HRESULT CItemMamager::Ready_Item()
{
    CComponent* pCom(nullptr);

    // 포션
    {
       
        ITEMDATA tPotion{};
        //tPotion.iID = 1;
        tPotion.eType = ITEM_POTION;
        tPotion.iAttack = 0;
        tPotion.iHp = 10;
        tPotion.sItName = L"Small_HP_Potion";
        tPotion.sIcontexKey = L"Item_Potion";   // UI에서 이 키로 텍스쳐 찾아 쓸 것
        tPotion.iMaxCount = 99;
        tPotion.iPrice = 50;

        Add_Item(tPotion);
    }

    return S_OK;

}

HRESULT CItemMamager::Add_Item(const ITEMDATA& tData)
{
    // 중복 방지
    if (m_mapItem.find(tData.sItName) != m_mapItem.end())
        return E_FAIL;

    m_mapItem.insert({ tData.sItName, tData });

    return S_OK;
}

void CItemMamager::Free()
{
    m_mapItem.clear();
}
