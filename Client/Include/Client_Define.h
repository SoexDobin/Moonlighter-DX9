#pragma once


enum SCENETYPE { SC_MAIN, SC_PLAYER, SC_MAP, SC_MONSTER, SC_UI, SC_END };
// 아이템 
enum ITEMTYPE { ITEM_ATTACK, ITEM_POTION, ITEM_END };
// 슬롯
enum SLOT_STATE { SLOT_NORMAL, SLOT_HOVER, SLOT_PRESSED, SLOT_END};


//아이템 데이터
struct ITEMDATA
{
    ITEMTYPE      m_Type;       // 아이템 타입

    int           m_iAttack;
    int           m_iHp;

    wstring       m_sItName;    // 아이템 이름
    wstring       m_ItemTag;    // 아이템 키 값
    
};

// 슬롯 위치

struct UISLOT
{
    int         iIndex;

    RECT        rcSlot; // 화면 좌표
    SLOT_STATE  estate = SLOT_NORMAL;

    UINT        SlotID;
    UINT        iCount;
};
