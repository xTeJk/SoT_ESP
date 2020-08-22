#include "SotStuff.h"

#include <clocale>
#include <cstdlib>
cSOT* SOT = new cSOT();

offsets Offsets;

Vector3 USceneComponent::GetPosition()
{
	return this->transform.Translation;
}

Vector3 USceneComponent::GetRotation()
{
	//return this->relativeAngles;
	return Vector3(this->transform.Rotation.X, this->transform.Rotation.Y, this->transform.Rotation.Z);
}

int AActor::GetID()
{
	return *(int*)(__pad0x0 + Offsets.AActor.actorId);
}

USceneComponent AActor::GetRootComponent()
{
	return Mem->Read<USceneComponent>(*(uintptr_t*)(__pad0x0 + Offsets.AActor.rootComponent));
}

APlayerState AActor::GetPlayerState()
{
	return Mem->Read<APlayerState>(*(uintptr_t*)(__pad0x0 + Offsets.AActor.PlayerState));
}

UWieldedItemComponent AActor::GetWieldedItemComponent()
{
	return Mem->Read < UWieldedItemComponent>(*(uintptr_t*)(__pad0x0 + Offsets.AActor.WieldedItemComponent));
}

UHealthComponent AActor::GetHealthComponent()
{
	return Mem->Read< UHealthComponent>(*(uintptr_t*)(__pad0x0 + Offsets.AActor.HealthComponent));
}

TArray<Chunk*> ULevel::GetActors() const
{
	return this->m_Actors;
}

Vector3 APlayerCameraManager::GetCameraPosition()
{
	return this->position;
}

Vector3 APlayerCameraManager::GetCameraRotation()
{
	return this->rotation;
}

float APlayerCameraManager::GetCameraFOV()
{
	return this->fov;
}

AActor APlayerController::GetActor()
{
	return Mem->Read<AActor>(*(uintptr_t*)( __pad0x0 + Offsets.APlayerController.Pawn ));
}


APlayerCameraManager APlayerController::GetCameraManager()
{
	return Mem->Read<APlayerCameraManager>(*(uintptr_t*)(__pad0x0 + Offsets.APlayerController.CameraManager));
}

/*
Vector3 APlayerController::GetPlayerAngles()
{
	return *(Vector3*)(__pad0x0 + Offsets.ULocalPlayer.PlayerController + 0x4C8);
}

void ULocalPlayer::SetPlayerAngles(Vector3 angles)
{
	Mem->Write<Vector3>(*(uintptr_t*)(__pad0x0 + Offsets.ULocalPlayer.PlayerController + 0x4C8), angles);
}
*/

APlayerController ULocalPlayer::GetPlayerController()
{
	return Mem->Read<APlayerController>(*(uintptr_t*)(__pad0x0 + Offsets.ULocalPlayer.PlayerController));
}

ULocalPlayer UGameInstance::GetLocalPlayer()
{
	return Mem->Read<ULocalPlayer>(Mem->Read<uintptr_t>(*(uintptr_t*)(__pad0x0 + Offsets.UGameInstance.LocalPlayers)));
}

ULevel cUWorld::GetLevel() const
{
	return Mem->Read<ULevel>(*(uintptr_t*)(__pad0x0 + Offsets.UWorld.PersistentLevel));
}

UGameInstance cUWorld::GetGameInstance() const
{
	return Mem->Read<UGameInstance>(*(uintptr_t*)(__pad0x0 + Offsets.UWorld.OwningGameInstance));
}

TArray<struct FAlliance> AAllianceService::GetAlliances()
{
	return this->Alliances;
}

TArray<struct FCrew> ACrewService::GetCrews()
{
	return *(TArray<struct FCrew>*)(__pad0x0 + Offsets.ACrewService.Crews);
}

TArray<class APlayerState*> FCrew::GetPlayers()
{
	return this->Players;
}

std::string FCrew::GetShipType()
{

	switch (this->maxPlayersOnShip)
	{
	case 2:
		return "Sloop [1-2]";
		break;
	case 3:
		return "Brigantine [1-3]";
		break;
	case 4:
		return "Galleon [1-4]";
		break;
	default:
		return "";
		break;
	}
}

FGuid FCrew::GetCrewID()
{
	return this->CrewID;
}

std::wstring APlayerState::GetName()
{
	return Mem->Read<textx>(*(uintptr_t*)(__pad0x0 + Offsets.APlayerState.PlayerName)).word;
}




int UHealthComponent::GetHealth()
{
	return this->health;
}

int UHealthComponent::GetMaxHealth()
{
	return this->maxHealth;
}

FGuid UCrewOwnershipComponent::GetCrewId()
{
	return *(FGuid*)(__pad0x0 + Offsets.UCrewOwnershipComponent.CrewId);
}

UCrewOwnershipComponent AShip::GetCrewOwnershipComponent()
{
	return Mem->Read<UCrewOwnershipComponent>(*(uintptr_t*)(__pad0x0 + Offsets.AShip.CrewOwnershipComponent));
}

uintptr_t AShip::GetOwningActor()
{
	return *(uintptr_t*)(__pad0x0 + 0x468); //2.0.17.2
}

TArray<struct FXMarksTheSpotMapMark> AXMarksTheSpotMap::GetMarks()
{
	return this->Marks;
}

ABootyItemInfo AItemProxy::GetBootyItemInfo()
{
	return Mem->Read<ABootyItemInfo>(*(uintptr_t*)(__pad0x0 + Offsets.AItemProxy.AItemInfo));
}

UItemDesc ABootyItemInfo::GetItemDesc()
{
	return Mem->Read<UItemDesc>(*(uintptr_t*)(__pad0x0 + Offsets.AItemInfo.UItemDesc));
}

int ABootyItemInfo::GetBootyType()
{
	return *(int*)(__pad0x0 + Offsets.ABootyItemInfo.BootyType);
}

int ABootyItemInfo::GetRareityId()
{
	return *(int*)(__pad0x0 + Offsets.ABootyItemInfo.Rarity);
}

std::wstring UItemDesc::GetName()
{
	return Mem->Read<textx>(Mem->Read<uintptr_t>(this->m_pName)).word;
}

AWieldableItem UWieldedItemComponent::GetWieldedItem()
{
	return Mem->Read < AWieldableItem>(*(uintptr_t*)(__pad0x0 + Offsets.UWieldedItemComponent.WieldedItem));
}

AItemInfo AWieldableItem::GetItemInfo()
{
	return  Mem->Read < AItemInfo > (*(uintptr_t*)(__pad0x0 + Offsets.AWieldableItem.ItemInfo));
}

UItemDesc AItemInfo::GetItemDesc()
{
	return Mem->Read < UItemDesc>(*(uintptr_t*)(__pad0x0 + Offsets.AItemInfo.UItemDesc));
}

Vector2 AMapTable::GetServerCenter()
{
	return *(Vector2*)(__pad0x0 + Offsets.AMapTable.ServerCenter);
}

TArray<struct Vector2> AMapTable::GetMapPins()
{
	return *(TArray<struct Vector2>*)(__pad0x0 + Offsets.AMapTable.MapPins);
}

// credits: shynd <3
TArray<class FWorldMapShipLocation> AMapTable::GetTrackedShips()
{
	//return *(TArray<class FWorldMapShipLocation>*)(__pad0x0 + Offsets.AMapTable.TrackedShips);
	return *(TArray<class FWorldMapShipLocation>*)(__pad0x0 + Offsets.AMapTable.MapPins + 0x10);
}

TArray<struct Vector3> AMapTable::GetTrackedBootyItemLocations()
{
	//return *(TArray<struct Vector3>*)(__pad0x0 + 0x0500);
	return *(TArray<struct Vector3>*)(__pad0x0 + Offsets.AMapTable.MapPins + 0x20);
}
//

TArray<struct FIsland> AIslandService::GetIslandArray()
{
	return this->IslandArray;
}

FGuid FWorldMapShipLocation::GetCrewId()
{
	return this->crewId;
}

UIslandDataAsset AIslandService::GetIslandDataAsset()
{
	return Mem->Read<UIslandDataAsset>(this->m_pIslandDataAsset);
}

UObject FWorldMapShipLocation::GetUObject()
{
	return Mem->Read<UObject>(this->m_pUObject);
}

std::wstring AFauna::GetName()
{
	return Mem->Read<textx>(Mem->Read<uintptr_t>(*(uintptr_t*)(__pad0x0 + Offsets.AFauna.Name))).word;
}

TArray<class UIslandDataAssetEntry> UIslandDataAsset::GetIslandDataAssetEntry()
{
	return (this->IslandDataEntries);
}

int UIslandDataAssetEntry::GetNameID()
{
	return this->IslandNameId;
}

std::wstring UIslandDataAssetEntry::GetName()
{
	return Mem->Read<textx>(Mem->Read<uintptr_t>(this->IslandName)).word;
}