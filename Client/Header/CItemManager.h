#pragma once
#include "CBase.h"
#include "Engine_Define.h"


enum ITEMTYPE { ITEM_ATTACK, ITEM_POTION, ITEM_END };
namespace Engine { class CTexture; }

struct ITEMDATA
{
    ITEMTYPE      m_Type;       // 아이템 타입

    int           m_iAttack;
    int           m_iHp;

    wstring       m_sItName;    // 아이템 이름
    wstring       m_ItemTag;    // 아이템 키 값

    Engine::CTexture* m_pTexture;

    ITEMDATA() {};
     
    ITEMDATA(ITEMTYPE  _itemtype,
             int _attack, int _hp,
             CTexture* _pTexture,
             const wstring& _itemname, const wstring& _itemtag)
      : m_Type(_itemtype), m_iAttack(_attack), m_iHp(_hp),
      m_pTexture(_pTexture),
      m_sItName(_itemname), m_ItemTag(_itemtag)
    {}
};


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

