#pragma once


class offsets {
public:
	struct UWorld
	{
		int PersistentLevel;
		int OwningGameInstance;
	}UWorld;
	struct UGameInstance
	{
		int LocalPlayers;
	}UGameInstance;
	struct UPlayer
	{
		int PlayerController;
	}ULocalPlayer;
	struct ULevel
	{
		int AActor = 0xA0;
	}ULevel;
	struct APlayerController
	{
		int Pawn;
		int CameraManager;
		int ControlRotation;
	}APlayerController;
	struct AActor
	{
		int actorId = 0x18;
		int rootComponent;
		int PlayerState;
		int WieldedItemComponent;
		int HealthComponent;
	}AActor;
	struct APlayerState
	{
		int PlayerName;
	}APlayerState;
	struct APlayerCameraManager
	{
		int CameraCache;
	}APlayerCameraManager;
	struct AItemProxy
	{
		int AItemInfo;
	}AItemProxy;
	struct AItemInfo
	{
		int UItemDesc;
	}AItemInfo;
	struct ABootyItemInfo
	{
		int BootyType;
		int Rarity;
	}ABootyItemInfo;
	struct AShip
	{
		int CrewOwnershipComponent;
	}AShip;
	struct UCrewOwnershipComponent
	{
		int CrewId;
	}UCrewOwnershipComponent;
	struct AFauna
	{
		int Name;
	}AFauna;
	struct AMaptTable
	{
		int ServerCenter;
		int MapPins;
		int TrackedShips;
	}AMapTable;
	struct ACrewService
	{
		int Crews;
	}ACrewService;
	struct UWieldedItemComponent
	{
		int WieldedItem;
	}UWieldedItemComponent;
	struct AWieldableItem
	{
		int ItemInfo;
	}AWieldableItem;
};
extern offsets Offsets;