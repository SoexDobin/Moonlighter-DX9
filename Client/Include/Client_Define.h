#pragma once

#include "Client_Struct.h"
#include "Client_Enum.h"
enum PROTOTYPE_CUSTOM
{
    TEMP_CUSTOMPROTO = 0, //  <- 여기만 추가되는 순간 부터 지워줘
    PLAYER_MOVEMENT,
    CUSTOM_END
};

enum CLIENT_LAYERTAG
{
    DEFAULT_LAYER0 = 0,
    CAMERA_LAYER,
    DEFAULT_LAYER2,
    ENVIRONMENT_LAYER,
    DEFAULT_LAYER4,
    GAMELOGIC_LAYER,
    DEFAULT_LAYER6,
    UI_LAYER,
    DEFAULT_LAYER8,
    EFFECT_LAYER,
    DEFAULT_LAYER10,
    DEFAULT_LAYER11,
    DEFAULT_LAYER12,
    DEFAULT_LAYER13,
    DEFAULT_LAYER14,
    DEFAULT_LAYER15,
    CLIENT_LAYER_END
};

// 씬
enum SCENETYPE { SC_MAIN, SC_PLAYER, SC_MAP, SC_MONSTER, SC_UI, SC_END };
// 아이템 
enum ITEMTYPE { ITEM_POTION, ITEM_ATTACK, ITEM_WEAPON, ITEM_END };
// 슬롯
enum SLOT_STATE { SLOT_NORMAL, SLOT_HOVER, SLOT_PRESSED, SLOT_END};
// 오브젝트 ID
enum OBJECT_ID
{
    NONE                    = 0, // Engine에서 설정된 기본값
    PLAYER                 = 1 << 0,
    MONSTER             = 1 << 1,
    NPC                       = 1 << 2,
    PLAYER_ATK       = 1 << 3,
    MONSTER_ATK   = 1 << 4,
    ITEM                    = 1 << 5,
    ENVIRONMENT    = 1 << 6,
    INTERACTABLE  = 1 << 7,
    TRIGGER             = 1 << 8,
    ID_END                = 1 << 9
};

#define DUNGEON_VTX 25
#define DUNGEON_VTZ 21
#define DUNGEON_ITV 1 

#define BOSS_VTX 61
#define BOSS_VTZ 37
#define BOSS_ITV 1

// 렌더순서
enum RENDER_STATE { UI_INVENTORY = 0, UI_STATIC, UI_HPBAR, UI_END };

//아이템 데이터
struct ITEMDATA
{
    ITEMTYPE      eType = ITEM_POTION;       // 아이템 타입
    //UINT          iID;

    int           iAttack;
    int           iHp;

    wstring       sItName;          // 아이템 이름
    wstring       sIcontexKey;      // 아이템 텍스쳐 키

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
