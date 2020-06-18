# SoT ESP
Sea of Thieves - Advanced ESP  
[![Discord](https://discordapp.com/api/guilds/370909694056726528/widget.png?style=shield)](https://discord.gg/jqbq85J)
  
  
## How to:
- Start game
- Make sure you're in windowed mode
- Update offsets
- Start cheat

## Compilation:
- VS 2017 / v141 / .exe application
	- Includes +: `C:\Program Files %28x86%29\Microsoft DirectX SDK %28June 2010%29\Include`
	- Libraries +: `C:\Program Files %28x86%29\Microsoft DirectX SDK %28June 2010%29\Lib\x64`
	- /Od disabled (default)
- (Ready to compile source version for 2.0.15 is available to download in Releases)
- Compile as `Release x64` version

## Offsets:
- Download: https://github.com/gummy8unny/SoT-SDK
- Put OffsetFinder.exe in SDK directory
- Launch OffsetFinder.exe
- Copy offsets into cheat directory

## Current Offsets (18.06.2020 / 2.0.15):
```
{
	"AActor.HealthComponent": 2120,
	"AActor.PlayerState": 1072,
	"AActor.WieldedItemComponent": 2080,
	"AActor.actorId": 24,
	"AActor.rootComponent": 368,
	"ABootyItemInfo.BootyType": 1360,
	"ABootyItemInfo.Rarity": 1652,
	"ACrewService.Crews": 1496,
	"AFauna.Name": 2104,
	"AItemInfo.UItemDesc": 1128,
	"AItemProxy.AItemInfo": 1496,
	"AMapTable.MapPins": 1256,
	"AMapTable.ServerCenter": 1224,
	"AMapTable.TrackedShips": 1196,
	"APlayerCameraManager.CameraCache": 1152,
	"APlayerController.CameraManager": 1184,
	"APlayerController.ControlRotation": 684,
	"APlayerController.Pawn": 1048,
	"APlayerState.PlayerName": 1048,
	"AShip.CrewOwnershipComponent": 1920,
	"AWieldableItem.ItemInfo": 1464,
	"UCrewOwnershipComponent.CrewId": 212,
	"UGameInstance.LocalPlayers": 56,
	"ULocalPlayer.PlayerController": 48,
	"UWieldedItemComponent.WieldedItem": 656,
	"UWorld.OwningGameInstance": 448,
	"UWorld.PersistentLevel": 48
}
```

## Keybinds:
If you want to change keybinds use:  
https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes  
find interesting key `VK_` or `0x` code and put inside `Main.cpp` at correct place, eg.:  
`KeyToggle toggleF1(VK_F1);`  
replace to:  
`KeyToggle toggleF1(0x31);`  
to make F1 menu bootable by `1` instead of `F1`.  

## Functions:
All functions are possible for any configuration

![1](https://i.imgur.com/bjLxEJo.png)
![2](https://i.imgur.com/dmiCJuz.png)
![3](https://i.imgur.com/I2V64jP.png)
