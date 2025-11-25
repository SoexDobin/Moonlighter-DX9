#pragma once
#include "CBase.h"
#include "Engine_Define.h"


namespace Engine { class CTexture; }




class CItemMamager : public CBase
{
    DECLARE_SINGLETON(CItemMamager)
private:
    explicit CItemMamager();
    virtual ~CItemMamager() override;

public:
    HRESULT  Ready_ItemManager();

    ITEMDATA* Get_ItemData(const wstring& wsLayerTag);

    void  Add_Item();

private:
    void     Free() override;
    map<wstring, ITEMDATA>     m_mapItem;
};

