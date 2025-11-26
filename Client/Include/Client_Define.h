#pragma once

enum PROTOTYPE_CUSTOM
{
    TEMP_CUSTOMPROTO = 0, //  <- 여기만 추가되는 순간 부터 지워줘   
    CUSTOM_END
};

enum SCENETYPE { SC_MAIN, SC_PLAYER, SC_MAP, SC_MONSTER, SC_UI, SC_END };
// 아이템 
enum ITEMTYPE { ITEM_POTION, ITEM_ATTACK, ITEM_WEAPON, ITEM_END };
// 슬롯
enum SLOT_STATE { SLOT_NORMAL, SLOT_HOVER, SLOT_PRESSED, SLOT_END};


//아이템 데이터
struct ITEMDATA
{
    ITEMTYPE      eType = ITEM_POTION;       // 아이템 타입
    //UINT          iID;

    int           iAttack;
    int           iHp;

    wstring       sItName;    // 아이템 이름
    wstring       sIcontexKey;    // 아이템 텍스쳐 키

    UINT          iMaxCount = 10;
    UINT          iPrice;
};

// 슬롯 위치

struct UISLOT
{
    int         iIndex;

    RECT        rcSlot; // 화면 좌표
    SLOT_STATE  estate = SLOT_NORMAL;

    UINT        iItemID;  // 아이템 아이디(타입으로 변경할까?)
    UINT        iCount;  // 아이템 카운팅

    float       fHoverLerp = 0.f; //hover 보간값
};
