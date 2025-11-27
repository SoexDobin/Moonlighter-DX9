#pragma once
#include "CBase.h"
#include "Engine_Define.h"


namespace Engine
{
    class CTexture;
    class CGameObject;
}

class CItemMamager : public CBase
{
    DECLARE_SINGLETON(CItemMamager)
private:
    explicit CItemMamager();
    virtual ~CItemMamager() override;

public:
    HRESULT  Ready_ItemManager();

    ITEMDATA* Get_ItemData(const wstring& wsLayerTag);


private:
    void     Free() override;

private:
    HRESULT  Ready_Item();
    HRESULT Add_Item(const ITEMDATA& tData);

    CTexture* m_pTextureCom;
    unordered_map<wstring, ITEMDATA>     m_mapItem;
};

