# SoT ESP
Sea of Thieves - Advanced ESP - 2.0.17.2  
[![Discord](https://discordapp.com/api/guilds/370909694056726528/widget.png?style=shield)](https://discord.gg/jqbq85J)
  
  
## How to:
- Check latest game version (https://www.seaofthieves.com/pl/release-notes)
  - If it is the same as the version the ESP fits, then continue
  - If it is not the same, go to https://github.com/xTeJk/SoT_ESP#offsets and update the offsets with the appropriate SDK for that version of the game
- Start game
- Make sure you're in windowed mode
- Update offsets
- Start cheat

## Compilation:
- VS 2017 / v141 / .exe application
	- Includes +: `C:\Program Files %28x86%29\Microsoft DirectX SDK %28June 2010%29\Include`
	- Libraries +: `C:\Program Files %28x86%29\Microsoft DirectX SDK %28June 2010%29\Lib\x64`
	- /Od disabled (default)
- (Ready to compile source version for 2.0.17.2 is available to download in Releases)
- Compile as `Release x64` version

## Offsets:
- Download: https://github.com/Shat-sky/SoT-SDK (2.0.17.2)
  - If ^ SDK is outdated, you should look for new one here: https://github.com/search?o=desc&q=sot+sdk&s=updated&type=Repositories
- Put OffsetFinder.exe in SDK directory
- Launch OffsetFinder.exe
- Copy `offsets` into cheat directory

## Current Offsets (21.08.2020 / 2.0.17.2):
```
{
    "AActor.HealthComponent": 2216,
    "AActor.PlayerState": 1144,
    "AActor.WieldedItemComponent": 2176,
    "AActor.actorId": 24,
    "AActor.rootComponent": 368,
    "ABootyItemInfo.BootyType": 1432,
    "ABootyItemInfo.Rarity": 1796,
    "ACrewService.Crews": 1568,
    "AFauna.Name": 2184,
    "AItemInfo.UItemDesc": 1200,
    "AItemProxy.AItemInfo": 1568,
    "AMapTable.MapPins": 1352,
    "AMapTable.ServerCenter": 1320,
    "AMapTable.TrackedShips": 1368,
    "APlayerCameraManager.CameraCache": 1232,
    "APlayerController.CameraManager": 1256,
    "APlayerController.ControlRotation": 684,
    "APlayerController.Pawn": 1120,
    "APlayerState.PlayerName": 1120,
    "AShip.CrewOwnershipComponent": 2000,
    "AWieldableItem.ItemInfo": 1544,
    "UCrewOwnershipComponent.CrewId": 212,
    "UGameInstance.LocalPlayers": 56,
    "ULocalPlayer.PlayerController": 48,
    "UWieldedItemComponent.WieldedItem": 656,
    "UWorld.OwningGameInstance": 448,
    "UWorld.PersistentLevel": 48
}
```

## SDK:
If you want to use new features or get to know the SDK better, you can use this: http://sot-guide.igromanru.com

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
  
- ESP:
  - Players (name, health bar, current weapon/item in hands, box)
  - Mobs (name, health bar, current weapon/item in hands, box)
  - Animals (rarity, type)
  - Treasures (including all supply crates and barrels)
  - Ships (enemy ships, cloud ships, ally ships)
  - Lore Books
  - Events (Forts, Reaper's Chest etc.)
- List of players by ships and their type
- Debug mode to aid in coding new functions
  
![1](https://i.imgur.com/bjLxEJo.png)
![2](https://i.imgur.com/dmiCJuz.png)
![3](https://i.imgur.com/I2V64jP.png)
