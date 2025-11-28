#include "pch.h"
#include "CDataManager.h"
#include "CLayerHelper.h"

#include "CSoundManager.h"
#include "CPrototypeManager.h"
#include "CResourceManager.h"

#include "CTmpCustomComponent.h"

IMPLEMENT_SINGLETON(CDataManager)

const wstring CDataManager::n_wsPrototypeTag[CUSTOM_END]
{
    L"Temp_CustomProto", //  <- 여기만 추가되는 순간 부터 지워줘
    L"Player_Movement"
};

const wstring CDataManager::n_wsLayerTag[Engine::LAYER_END]
{
    L"Default_Layer",
    L"Camera_Layer",
    L"Environment_NoCollision_Layer",
    L"Environment_Layer",
    L"Default_Layer",
    L"GameLogic_Layer",
    L"Default_Layer",
    L"UI_Layer",
    L"Default_Layer",
    L"Effect_Layer",
    L"Default_Layer",
    L"Default_Layer",
    L"Default_Layer",
    L"Default_Layer",
    L"Default_Layer",
    L"Default_Layer",
};

CDataManager::CDataManager()
{
}

CDataManager::~CDataManager()
{
    CDataManager::Free();
}

HRESULT CDataManager::Ready_Data(LPDIRECT3DDEVICE9 pGraphicDev)
{
    Engine::CLayerHelper::GetInstance()->SetLayerName(n_wsLayerTag);

	{
		if (FAILED(Ready_Sound()))
			return E_FAIL;

		if (FAILED(Ready_Resource(pGraphicDev)))
			return E_FAIL;

        if (FAILED(Ready_Player_Resource(pGraphicDev)))
            return E_FAIL;
        if (FAILED(Ready_Map_Resource(pGraphicDev)))
            return E_FAIL;
		if (FAILED(Ready_Item_Resource(pGraphicDev)))
			return E_FAIL;
        if (FAILED(Ready_UI_Resource(pGraphicDev)))
            return E_FAIL;

        if (FAILED(Ready_Boss_Resource(pGraphicDev)))
            return E_FAIL;

        if (FAILED(Ready_Monster_Resource(pGraphicDev)))
            return E_FAIL;

		if (FAILED(Ready_Prototype(pGraphicDev)))
			return E_FAIL;

	}

	return S_OK;
}

HRESULT CDataManager::Ready_Prototype(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (FAILED(Engine::CPrototypeManager::GetInstance()
		->Ready_Prototype(RECTCOLOR, CRectColor::Create(pGraphicDev))))
		return E_FAIL;

	if (FAILED(Engine::CPrototypeManager::GetInstance()
		->Ready_Prototype(RECTTEX, CRectTex::Create(pGraphicDev))))
		return E_FAIL;

	if (FAILED(Engine::CPrototypeManager::GetInstance()
		->Ready_Prototype(TRANSFORM, CTransform::Create(pGraphicDev))))
		return E_FAIL;

	if (FAILED(Engine::CPrototypeManager::GetInstance()
		->Ready_Prototype(TEXTURE, CTexture::Create(pGraphicDev, n_fDefault_AnimSpeed))))
		return E_FAIL;

    if (FAILED(CPrototypeManager::GetInstance()->Ready_Prototype(TERRAINTEX, Engine::CTerrainTex::Create(pGraphicDev, VILLAGE_VTXCNTX, VILLAGE_VTXCNTX, VTXITV, L""))))
        return E_FAIL;
    if (FAILED(CPrototypeManager::GetInstance()->Ready_Prototype(TERRAINDUNGEONTEX, Engine::CTerrainDungeonTex::Create(pGraphicDev, DUNGEON_VTX, DUNGEON_VTZ, DUNGEON_ITV, L""))))
        return E_FAIL;
    if (FAILED(CPrototypeManager::GetInstance()->Ready_Prototype(TERRAINBOSSTEX, Engine::CTerrainBossTex::Create(pGraphicDev, BOSS_VTX, BOSS_VTZ, BOSS_ITV, L""))))
    {
        MSG_BOX("Boss Terrain Proto Fail");
        return E_FAIL;
    }
    if (FAILED(Engine::CPrototypeManager::GetInstance()
        ->Ready_Prototype(SPHERE_COLLIDER, CSphereCollider::Create(pGraphicDev))))
        return E_FAIL;

    if (FAILED(Engine::CPrototypeManager::GetInstance()
        ->Ready_Prototype(RECT_COLLIDER, CRectCollider::Create(pGraphicDev))))
        return E_FAIL;


    // Client Custom Prototype 정의
    // 추가되는 순간 부터 지워줘
    if (FAILED(Engine::CPrototypeManager::GetInstance()
        ->Ready_Prototype(n_wsPrototypeTag[TEMP_CUSTOMPROTO], CTmpCustomComponent::Create(pGraphicDev))))
        return E_FAIL;

	return S_OK;
}

HRESULT CDataManager::Ready_Resource(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CResourceManager& Res = *CResourceManager::GetInstance();

	if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Roll",
		n_wsResSpritePath + L"Player/Will_Roll_Shop_Down_%d.png", 8)))
		return E_FAIL;

	return S_OK;
}

#pragma region PlayerTexture

HRESULT CDataManager::Ready_Player_Resource(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CResourceManager& Res = *CResourceManager::GetInstance();

    // PlayerIdle

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Idle_Down",
        n_wsResSpritePath + L"Player/Idle/Down/PLAYER_IDLE_DOWN_%02d.png", 10)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Idle_Up",
        n_wsResSpritePath + L"Player/Idle/Up/PLAYER_IDLE_UP_%02d.png", 10)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Idle_Left",
        n_wsResSpritePath + L"Player/Idle/Left/PLAYER_IDLE_LEFT_%02d.png", 10)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Idle_Right",
        n_wsResSpritePath + L"Player/Idle/Right/PLAYER_IDLE_RIGHT_%02d.png", 10)))
        return E_FAIL;

    // PlayerWalk

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Walk_Down",
        n_wsResSpritePath + L"Player/Walk/Down/PLAYER_WALK_DOWN_%02d.png", 8)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Walk_Up",
        n_wsResSpritePath + L"Player/Walk/Up/PLAYER_WALK_UP_%02d.png", 8)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Walk_Left",
        n_wsResSpritePath + L"Player/Walk/Left/PLAYER_WALK_LEFT_%02d.png", 8)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Walk_Right",
        n_wsResSpritePath + L"Player/Walk/Right/PLAYER_WALK_RIGHT_%02d.png", 8)))
        return E_FAIL;

    // PlayerRoll

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Roll_Down",
        n_wsResSpritePath + L"Player/Roll/Down/PLAYER_ROLL_DOWN_%02d.png", 8)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Roll_Up",
        n_wsResSpritePath + L"Player/Roll/Up/PLAYER_ROLL_UP_%02d.png", 8)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Roll_Left",
        n_wsResSpritePath + L"Player/Roll/Left/PLAYER_ROLL_LEFT_%02d.png", 8)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Roll_Right",
        n_wsResSpritePath + L"Player/Roll/Right/PLAYER_ROLL_RIGHT_%02d.png", 8)))
        return E_FAIL;

    // Player Spear Combo Attack

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Spear_Combo_Down",
        n_wsResSpritePath + L"Player/Spear/ComboAttack/Down/PLAYER_SPEAR_COMBO_DOWN_%02d.png", 23)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Spear_Combo_Up",
        n_wsResSpritePath + L"Player/Spear/ComboAttack/Up/PLAYER_SPEAR_COMBO_UP_%02d.png", 23)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Spear_Combo_Left",
        n_wsResSpritePath + L"Player/Spear/ComboAttack/Left/PLAYER_SPEAR_COMBO_LEFT_%02d.png", 23)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Spear_Combo_Right",
        n_wsResSpritePath + L"Player/Spear/ComboAttack/Right/PLAYER_SPEAR_COMBO_RIGHT_%02d.png", 23)))
        return E_FAIL;

    // Player Spear Charge Attack

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Spear_Charge_Down",
        n_wsResSpritePath + L"Player/Spear/ChargeAttack/Down/PLAYER_SPEAR_CHARGE_DOWN_%02d.png", 54)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Spear_Charge_Up",
        n_wsResSpritePath + L"Player/Spear/ChargeAttack/Up/PLAYER_SPEAR_CHARGE_UP_%02d.png", 54)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Spear_Charge_Left",
        n_wsResSpritePath + L"Player/Spear/ChargeAttack/Left/PLAYER_SPEAR_CHARGE_LEFT_%02d.png", 54)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Spear_Charge_Right",
        n_wsResSpritePath + L"Player/Spear/ChargeAttack/Right/PLAYER_SPEAR_CHARGE_RIGHT_%02d.png", 54)))
        return E_FAIL;

    // Player Bow Normal Attack

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Bow_Normal_Down",
        n_wsResSpritePath + L"Player/Bow/NormalAttack/Down/PLAYER_BOW_NORMAL_DOWN_%02d.png", 11)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Bow_Normal_Up",
        n_wsResSpritePath + L"Player/Bow/NormalAttack/Up/PLAYER_BOW_NORMAL_UP_%02d.png", 11)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Bow_Normal_Left",
        n_wsResSpritePath + L"Player/Bow/NormalAttack/Left/PLAYER_BOW_NORMAL_LEFT_%02d.png", 11)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Bow_Normal_Right",
        n_wsResSpritePath + L"Player/Bow/NormalAttack/Right/PLAYER_BOW_NORMAL_RIGHT_%02d.png", 11)))
        return E_FAIL;

    // Player Bow Charge Attack

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Bow_Charge_Down",
        n_wsResSpritePath + L"Player/Bow/ChargeAttack/Down/PLAYER_BOW_CHARGE_DOWN_%02d.png", 27)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Bow_Charge_Up",
        n_wsResSpritePath + L"Player/Bow/ChargeAttack/Up/PLAYER_BOW_CHARGE_UP_%02d.png", 27)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Bow_Charge_Left",
        n_wsResSpritePath + L"Player/Bow/ChargeAttack/Left/PLAYER_BOW_CHARGE_LEFT_%02d.png", 27)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Player_Bow_Charge_Right",
        n_wsResSpritePath + L"Player/Bow/ChargeAttack/Right/PLAYER_BOW_CHARGE_RIGHT_%02d.png", 27)))
        return E_FAIL;

    return S_OK;
}

#pragma endregion

HRESULT CDataManager::Ready_Item_Resource(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CResourceManager& Res = *CResourceManager::GetInstance();

	if (FAILED(Res.Add_Sprite(pGraphicDev, L"Item_Potion",
		n_wsResSpritePath + L"Item/Potion/Item_Potion_Heal_%d.png", 4)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDataManager::Ready_UI_Resource(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CResourceManager& Res = *CResourceManager::GetInstance();

    //인벤토리
    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Inventory_Base",
        L"../Bin/Resource/Sprite/UI/Inven/Inventory_Base.png", 1)))
        return E_FAIL;

    //인벤토리 static
    if (FAILED(Res.Add_Sprite(pGraphicDev, L"ClickSlot",
        L"../Bin/Resource/Sprite/UI/Inven/IconBar/IconBar_Calendary_A.png", 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"ClickSlot",
        L"../Bin/Resource/Sprite/UI/Inven/IconBar/IconBar_Calendary_D.png", 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"ClickSlot",
        L"../Bin/Resource/Sprite/UI/Inven/IconBar/IconBar_Inventory_A.png", 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"ClickSlot",
        L"../Bin/Resource/Sprite/UI/Inven/IconBar/IconBar_Inventory_D.png", 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"ClickSlot",
        L"../Bin/Resource/Sprite/UI/Inven/IconBar/IconBar_Notebook_A.png", 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"ClickSlot",
        L"../Bin/Resource/Sprite/UI/Inven/IconBar/IconBar_Notebook_D.png", 1)))
        return E_FAIL;


    if (FAILED(Res.Add_Sprite(pGraphicDev, L"ClickSlot",
        L"../Bin/Resource/Sprite/UI/Inven/IconBar/IconBar_Wishlist_A.png", 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"ClickSlot",
        L"../Bin/Resource/Sprite/UI/Inven/IconBar/IconBar_Wishlist_D.png", 1)))
        return E_FAIL;


    // static
    if (FAILED(Res.Add_Sprite(pGraphicDev, L"hp_barBack",
        L"../Bin/Resource/Sprite/UI/Static/hp_barBack.png", 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"icon_bag",
        L"../Bin/Resource/Sprite/UI/Static/icon_bag.png", 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"icon_coin",
        L"../Bin/Resource/Sprite/UI/Static/icon_coin.png", 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"icon_hp",
        L"../Bin/Resource/Sprite/UI/Static/icon_hp.png", 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"icon_Pouch",
        L"../Bin/Resource/Sprite/UI/Static/icon_Pouch.png", 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"interface_key",
        L"../Bin/Resource/Sprite/UI/Static/interface_key.png", 1)))
        return E_FAIL;

    // HP Bar
    if (FAILED(Res.Add_Sprite(pGraphicDev, L"hp_barFront",
        L"../Bin/Resource/Sprite/UI/Static/hp_barFront.png", 1)))
        return E_FAIL;

    //slot
    if (FAILED(Res.Add_Sprite(pGraphicDev, L"HoverSlot",
        L"../Bin/Resource/Sprite/UI/Inven/HoverSlot.png", 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"ClickSlot",
        L"../Bin/Resource/Sprite/UI/Inven/ClickSlot.png", 1)))
        return E_FAIL;

    return S_OK;
}


HRESULT CDataManager::Ready_Map_Resource(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CResourceManager& Res = *CResourceManager::GetInstance();

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Map_Village",
        n_wsResSpritePath + L"Map/Village_Background.png", 1)))
    {
        MSG_BOX("Village Terrain Image Load Fail");
        return E_FAIL;
    }
    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Map_Village_House",
        n_wsResSpritePath + L"Map/Village_House.png", 1)))
    {
        MSG_BOX("Village House Image Load Fail");
        return E_FAIL;
    }
    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Map_Village_Tree",
        n_wsResSpritePath + L"Map/Village_tree.png", 1)))
    {
        MSG_BOX("Village Tree Image Load Fail");
        return E_FAIL;
    }
    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Map_Dungeon",
        n_wsResSpritePath + L"Map/Terrain_dungeon_normal.png", 1)))
    {
        MSG_BOX("Terrain Dungeon Image Load Fail");
        return E_FAIL;
    }
    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Map_Dungeon_Wall",
        n_wsResSpritePath + L"Map/Wall_dungeon_normal.png", 1)))
    {
        MSG_BOX("Dungeon Wall Image Load Fail");
        return E_FAIL;
    }
    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Map_Boss",
        n_wsResSpritePath + L"Map/Boss_Dungeon_Field.png", 1)))
    {
        MSG_BOX("Boss Terrain Image Load Fail");
        return E_FAIL;
    }
    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Map_Boss_Wall_Down",
        n_wsResSpritePath + L"Map/Boss_Dungeon_Wall_Below.png", 1)))
    {
        MSG_BOX("Boss Wall Down Image Load Fail");
        return E_FAIL;
    }
    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Map_Boss_Wall_Up",
        n_wsResSpritePath + L"Map/Boss_Dungeon_Wall_Upper.png", 1)))
    {
        MSG_BOX("Boss Wall Up Image Load Fail");
        return E_FAIL;
    }
    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Map_Boss_Wall_Side",
        n_wsResSpritePath + L"Map/Boss_Dungeon_Wall_Side.png", 1)))
    {
        MSG_BOX("Boss Wall Side Image Load Fail");
        return E_FAIL;
    }
    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Map_Boss_Pumpkin",
        n_wsResSpritePath + L"Map/Boss_Dungeon_Wall_Edge_Pumpkin.png", 1)))
    {
        MSG_BOX("Boss Wall Side Image Load Fail");
        return E_FAIL;
    }
    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Map_Boss_Vine1",
        n_wsResSpritePath + L"Map/Boss_Dungeon_Wall_Object_Vine1.png", 1)))
    {
        MSG_BOX("Boss Vine1 Image Load Fail");
        return E_FAIL;
    }
    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Map_Boss_Vine2",
        n_wsResSpritePath + L"Map/Boss_Dungeon_Wall_Object_Vine2.png", 1)))
    {
        MSG_BOX("Boss Vine2 Image Load Fail");
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CDataManager::Ready_Boss_Resource(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CResourceManager& Res = *CResourceManager::GetInstance();

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Boss_Awake",
        n_wsResSpritePath + L"Monster/Boss/Boss_Awake/Boss_Awake%d.png", 129, 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Boss_Cutting_Growing",
        n_wsResSpritePath + L"Monster/Boss/Boss_Cutting_Growing/Boss_Cutting_Growing%d.png", 16, 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Boss_Cutting_Idle",
        n_wsResSpritePath + L"Monster/Boss/Boss_Cutting_Idle/Boss_Cutting_Idle%d.png", 5, 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Boss_Cutting_Shot",
        n_wsResSpritePath + L"Monster/Boss/Boss_Cutting_Shot/Boss_Cutting_Shot%d.png", 15, 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Boss_Death",
        n_wsResSpritePath + L"Monster/Boss/Boss_Death/Boss_Death%d.png", 46, 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Boss_Idle",
        n_wsResSpritePath + L"Monster/Boss/Boss_Idle/Boss_Idle%d.png", 8, 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Boss_Jump",
        n_wsResSpritePath + L"Monster/Boss/Boss_Jump/Boss_Jump%d.png", 30, 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Boss_Roots",
        n_wsResSpritePath + L"Monster/Boss/Boss_Roots/Boss_Roots%d.png", 56, 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Boss_Shake",
        n_wsResSpritePath + L"Monster/Boss/Boss_Shake/Boss_Shake%d.png", 83, 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Boss_Throwing",
        n_wsResSpritePath + L"Monster/Boss/Boss_Throwing/Boss_Throwing%d.png", 40, 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Root1",
        n_wsResSpritePath + L"Monster/Boss/Root1/Root%d.png", 54, 1)))
        return E_FAIL;

    if (FAILED(Res.Add_Sprite(pGraphicDev, L"Root2",
        n_wsResSpritePath + L"Monster/Boss/Root2/Root%d.png", 52, 2)))
        return E_FAIL;

    return S_OK;
}

HRESULT CDataManager::Ready_Monster_Resource(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CResourceManager& Res = *CResourceManager::GetInstance();

    // Tree Monster
    {
        if (FAILED(Res.Add_Sprite(pGraphicDev, L"Tree_Awake",
            n_wsResSpritePath + L"Monster/Tree/Tree_Awake/Tree_Awake%d.png", 8, 1)))
            return E_FAIL;

        if (FAILED(Res.Add_Sprite(pGraphicDev, L"Tree_Shake",
            n_wsResSpritePath + L"Monster/Tree/Tree_Shake/Tree_Shake%d.png", 9, 1)))
            return E_FAIL;
    }

    // Slime Monster
    {
        if (FAILED(Res.Add_Sprite(pGraphicDev, L"Slime_Idle",
            n_wsResSpritePath + L"Monster/Slime/Slime_Idle/Slime_Idle%d.png", 8, 1)))
            return E_FAIL;

        if (FAILED(Res.Add_Sprite(pGraphicDev, L"Slime_Circle",
            n_wsResSpritePath + L"Monster/Slime/Slime_Circle/Slime_Circle%d.png", 16, 1)))
            return E_FAIL;

        if (FAILED(Res.Add_Sprite(pGraphicDev, L"Slime_Big",
            n_wsResSpritePath + L"Monster/Slime/Slime_Big/Slime_Big%d.png", 23, 1)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CDataManager::Ready_Sound()
{
	Engine::CSoundManager::GetInstance()->Ready_SoundManager();

	return S_OK;
}

void CDataManager::Free()
{
	Engine::CResourceManager::DestroyInstance();
	Engine::CSoundManager::DestroyInstance();
}
