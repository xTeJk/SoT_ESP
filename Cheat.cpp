#include "Cheat.h"
#include <fstream>

cCheat* Cheat = new cCheat();

std::string cCheat::getNameFromIDmem(int ID) {
	try {
		DWORD_PTR fNamePtr = Mem->Read<uintptr_t>(GNames + int(ID / 0x4000) * 0x8);
		DWORD_PTR fName = Mem->Read<uintptr_t>(fNamePtr + 0x8 * int(ID % 0x4000));
		return Mem->Read<text>(fName + 0x10).word;
	}
	catch (int e)
	{
		return std::string("");
	}
}

std::string cCheat::getNameFromIDmap(int ID) 
{
	auto it = Names.find(ID);


	if (it == Names.end())
		return "";
	else
		return it->second;
}

Vector2 RotatePoint(Vector2 pointToRotate, Vector2 centerPoint, float angle, bool angleInRadians = false)
{
	if (!angleInRadians)
		angle = static_cast<float>(angle * (PI / 180.f));
	float cosTheta = static_cast<float>(cos(angle));
	float sinTheta = static_cast<float>(sin(angle));
	Vector2 returnVec = Vector2(cosTheta * (pointToRotate.x - centerPoint.x) - sinTheta * (pointToRotate.y - centerPoint.y), sinTheta * (pointToRotate.x - centerPoint.x) + cosTheta * (pointToRotate.y - centerPoint.y)
	);
	returnVec += centerPoint;
	return returnVec;
}


void cCheat::readData()
{
	if (!baseModule)
	{
		baseModule = Mem->Module("SoTGame.exe");
		baseSize = Mem->ModuleSize("SoTGame.exe");
	}

	uintptr_t address = 0;

	if (!UWorld)
	{
		address = Mem->FindSignature(baseModule, baseSize,
			(BYTE*)("\x48\x8B\x05\x00\x00\x00\x00\x48\x8B\x88\x00\x00\x00\x00\x48\x85\xC9\x74\x06\x48\x8B\x49\x70"),
			(char*)"xxx????xxx????xxxxxxxxx");
		auto uworldoffset = Mem->Read<int32_t>(address + 3);
		UWorld = address + uworldoffset + 7;
	}
	if (!GNames)
	{
		address = Mem->FindSignature(baseModule, baseSize, (BYTE*)"\x48\x8B\x1D\x00\x00\x00\x00\x48\x85\x00\x75\x3A", (char*)"xxx????xx?xx");
		auto gnamesoffset = Mem->Read<int32_t>(address + 3);
		GNames = Mem->Read<uintptr_t>(address + gnamesoffset + 7);
	}
	if (!GObjects)
	{
		address = Mem->FindSignature(baseModule, baseSize, (BYTE*)"\x48\x8B\x15\x00\x00\x00\x00\x3B\x42\x1C", (char*)"xxx????xxx");
		auto gobjectsoffset = Mem->Read<int32_t>(address + 3);
		auto offset = gobjectsoffset + 7;
		GObjects = Mem->Read<uintptr_t>(address + gobjectsoffset + 7);
	}

	if (Names.empty())
	{
		std::ofstream myfile;
		//myfile.open("Gnames.txt");

		for (int i = 0; i < 206000; i++)
		{

			auto temp = getNameFromIDmem(i);

			if (temp != "FAIL")
			{
				Names.insert(std::pair<int, std::string>(i, temp));
				//myfile << i << "  |  " << getNameFromIDmem(i) << "\n";
			}
		}

		//myfile.close();

	}
	auto world = Mem->Read<cUWorld>(Mem->Read<uintptr_t>(UWorld));
	//auto world = Mem->Read<cUWorld>(baseModule + 0x59B12D8);
	auto LocalPlayer = world.GetGameInstance().GetLocalPlayer();
	auto player_controller = LocalPlayer.GetPlayerController();
	auto CameraManager = player_controller.GetCameraManager();

	SOT->localPlayer.name = Misc->wstringToString(player_controller.GetActor().GetPlayerState().GetName());

	SOT->localPlayer.position = player_controller.GetActor().GetRootComponent().GetPosition();
	SOT->localCamera.fov = CameraManager.GetCameraFOV();
	SOT->localCamera.angles = CameraManager.GetCameraRotation();
	SOT->localCamera.position = CameraManager.GetCameraPosition();

	auto level = world.GetLevel();
	auto actors = level.GetActors();
	if (!actors.IsValid())
		return;

	for (int i = 0; i < actors.Length(); ++i)
	{

		auto actor = *reinterpret_cast<AActor*>(&actors[i]);
		if (actor == player_controller.GetActor())
			continue;

		auto id = actor.GetID();
		auto name = getNameFromIDmem(id);


		//SKELETON
		if (name.find("BP_Skeleton") != std::string::npos && name.find("Pawn") != std::string::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				Vector3 headPos = Vector3(pos.x, pos.y, pos.z + 100);

				Vector2 ScreenTop;
				Color boxColor = { Vars.ESP.MenuTake.CaptainColor[0],Vars.ESP.MenuTake.CaptainColor[1],Vars.ESP.MenuTake.CaptainColor[2],Vars.ESP.MenuTake.CaptainColor[3] };

				if (Misc->WorldToScreen(headPos, &ScreenTop))
				{
					int hi = (Screen.y - ScreenTop.y) * 2;
					int wi = hi * 0.65;

					if (Vars.ESP.MenuTake.Skeleton)
					{
						DrawBox(ScreenTop.x - wi / 2, ScreenTop.y, wi, hi, boxColor);
					}
					if (Vars.ESP.MenuTake.SkeletonName)
					{
						if ((int)distance < 25) {
							DrawString("» • «", Screen.x, Screen.y, boxColor, true, "dist_3");
						}
						else if ((int)distance < 15) {
							DrawString("» • «", Screen.x, Screen.y, boxColor, true, "dist_2");
						}
						else if ((int)distance < 5) {
							DrawString("» • «", ScreenTop.x, ScreenTop.y - 14, boxColor, true, "dist_1");
						}
						else {
							DrawString(std::string("Skeleton » " + std::to_string((int)distance) + "m").c_str(), ScreenTop.x, ScreenTop.y - 14, boxColor, true, "small");
						}
					}
					if (Vars.ESP.MenuTake.SkeletonItem)
					{
						auto ItemName = actor.GetWieldedItemComponent().GetWieldedItem().GetItemInfo().GetItemDesc().GetName();

						if (ItemName.length() < 32)
							DrawString(ItemName.c_str(), ScreenTop.x, ScreenTop.y + hi, boxColor, true, "small");
					}

				}
			}
		}

		//AMMO CRATE
		else if (name.find("Ammo") != std::string::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorCommon[0],Vars.ESP.colorCommon[1],Vars.ESP.colorCommon[2],Vars.ESP.colorCommon[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if (Vars.ESP.MenuTake.ItemsOther || Vars.ESP.MenuTake.ItemsCommon)
				{
					if ((int)distance > 10 && (int)distance < 20) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance >= 20 && (int)distance < 200){
						DrawString(std::string("Ammo Crate » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
					}
				}
		}

		//LORE BOOK
		else if (name.find("Lore") != std::string::npos || name.find("Tale") != std::string::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.MenuTake.LoreColor[0],Vars.ESP.MenuTake.LoreColor[1],Vars.ESP.MenuTake.LoreColor[2],Vars.ESP.MenuTake.LoreColor[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if (Vars.ESP.MenuTake.LoreBooks)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::string("Lore Book » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
					}
				}
		}

		//STORAGE BOX
		else if (name.find("BP_MerchantCrate_AnyItemCrate_ItemInfo_C") != std::string::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorCommon[0],Vars.ESP.colorCommon[1],Vars.ESP.colorCommon[2],Vars.ESP.colorCommon[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if (Vars.ESP.MenuTake.ItemsOther || Vars.ESP.MenuTake.ItemsCommon)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::string("Storage Crate » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
						if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
						{
							DrawCircle(Screen.x, Screen.y, 3, 25, color);
						}
					}
				}
		}

		//BARREL
		else if (name.find("BP_BuoyantStorageBarrel_LockedToWater") != std::string::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorCommon[0],Vars.ESP.colorCommon[1],Vars.ESP.colorCommon[2],Vars.ESP.colorCommon[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if (Vars.ESP.MenuTake.ItemsBarrels || Vars.ESP.MenuTake.ItemsCommon)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::string("Barrel » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
						if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
						{
							DrawCircle(Screen.x, Screen.y, 3, 25, color);
						}
					}
				}
		}

		//BOTTLE
		else if (name.find("Bottle") != std::string::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorRare[0], Vars.ESP.colorRare[1], Vars.ESP.colorRare[2], Vars.ESP.colorRare[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if (Vars.ESP.MenuTake.ItemsOther || Vars.ESP.MenuTake.ItemsLegendary)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::string("Message in Bottle » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					}
				}
		}
		//CAGE - CHICKEN
		else if (Vars.ESP.MenuTake.ItemsCages && name.find("ChickenCrate") != std::wstring::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorCommon[0],Vars.ESP.colorCommon[1],Vars.ESP.colorCommon[2],Vars.ESP.colorCommon[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if ((int)distance < 10) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
				}
				else if ((int)distance < 5) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
				}
				else if ((int)distance < 2) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
				}
				else {
					DrawString(std::string("Chicken Cage » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
					{
						DrawCircle(Screen.x, Screen.y, 3, 25, color);
					}
				}
		}
		//CAGE - PIG
		else if (Vars.ESP.MenuTake.ItemsCages && name.find("PigCrate") != std::wstring::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorCommon[0],Vars.ESP.colorCommon[1],Vars.ESP.colorCommon[2],Vars.ESP.colorCommon[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if ((int)distance < 10) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
				}
				else if ((int)distance < 5) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
				}
				else if ((int)distance < 2) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
				}
				else {
					DrawString(std::string("Pig Cage » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
					{
						DrawCircle(Screen.x, Screen.y, 3, 25, color);
					}
				}
		}
		//CAGE - SNAKE
		else if (Vars.ESP.MenuTake.ItemsCages && name.find("SnakeBasket") != std::wstring::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorCommon[0],Vars.ESP.colorCommon[1],Vars.ESP.colorCommon[2],Vars.ESP.colorCommon[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if ((int)distance < 10) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
				}
				else if ((int)distance < 5) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
				}
				else if ((int)distance < 2) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
				}
				else {
					DrawString(std::string("Snake Cage » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
					{
						DrawCircle(Screen.x, Screen.y, 3, 25, color);
					}
				}
		}
		// MERCHANT - BANANA
		else if (Vars.ESP.MenuTake.ItemsOther && name.find("BananaCrate") != std::wstring::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorCommon[0],Vars.ESP.colorCommon[1],Vars.ESP.colorCommon[2],Vars.ESP.colorCommon[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if ((int)distance < 10) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
				}
				else if ((int)distance < 5) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
				}
				else if ((int)distance < 2) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
				}
				else {
					DrawString(std::string("Fruit Crate » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
					{
						DrawCircle(Screen.x, Screen.y, 3, 25, color);
					}
				}
		}
		// MERCHANT - TEA
		else if (Vars.ESP.MenuTake.ItemsOther && name.find("TeaCrate") != std::wstring::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorCommon[0],Vars.ESP.colorCommon[1],Vars.ESP.colorCommon[2],Vars.ESP.colorCommon[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if ((int)distance < 10) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
				}
				else if ((int)distance < 5) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
				}
				else if ((int)distance < 2) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
				}
				else {
					DrawString(std::string("Tea Crate » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
					{
						DrawCircle(Screen.x, Screen.y, 3, 25, color);
					}
				}
		}
		// MERCHANT - SUGAR
		else if (Vars.ESP.MenuTake.ItemsOther && name.find("SugarCrate") != std::wstring::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorCommon[0],Vars.ESP.colorCommon[1],Vars.ESP.colorCommon[2],Vars.ESP.colorCommon[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if ((int)distance < 10) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
				}
				else if ((int)distance < 5) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
				}
				else if ((int)distance < 2) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
				}
				else {
					DrawString(std::string("Sugar Crate » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
					{
						DrawCircle(Screen.x, Screen.y, 3, 25, color);
					}
				}
		}
		// COLLECTOR
		else if (Vars.ESP.MenuTake.ItemsCommon && name.find("Collector") != std::wstring::npos && name.find("Ashen") == std::wstring::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorCommon[0],Vars.ESP.colorCommon[1],Vars.ESP.colorCommon[2],Vars.ESP.colorCommon[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if ((int)distance < 10) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
				}
				else if ((int)distance < 5) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
				}
				else if ((int)distance < 2) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
				}
				else {
					DrawString(std::string("Collector Chest » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
					{
						DrawCircle(Screen.x, Screen.y, 3, 25, color);
					}
				}
		}
		// MERCHANT - SILK
		else if (Vars.ESP.MenuTake.ItemsOther && name.find("BP_MerchantCrate_Commodity_SilkCrate_Proxy_C") != std::wstring::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorCommon[0],Vars.ESP.colorCommon[1],Vars.ESP.colorCommon[2],Vars.ESP.colorCommon[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if ((int)distance < 10) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
				}
				else if ((int)distance < 5) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
				}
				else if ((int)distance < 2) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
				}
				else {
					DrawString(std::string("Silk Crate » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
					{
						DrawCircle(Screen.x, Screen.y, 3, 25, color);
					}
				}
		}
		// ASHEN CHEST
		else if (Vars.ESP.MenuTake.ItemsLegendary && name.find("AshenCollector") != std::wstring::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorSpecial[0],Vars.ESP.colorSpecial[1],Vars.ESP.colorSpecial[2],Vars.ESP.colorSpecial[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if ((int)distance < 10) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
				}
				else if ((int)distance < 5) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
				}
				else if ((int)distance < 2) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
				}
				else {
					DrawString(std::string("Ashen Chest » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
					{
						DrawCircle(Screen.x, Screen.y, 3, 25, color);
					}
				}
		}
		// SPICE - SILK
		else if (Vars.ESP.MenuTake.ItemsOther && name.find("BP_MerchantCrate_Commodity_SpiceCrate_Proxy_C") != std::wstring::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorCommon[0],Vars.ESP.colorCommon[1],Vars.ESP.colorCommon[2],Vars.ESP.colorCommon[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if ((int)distance < 10) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
				}
				else if ((int)distance < 5) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
				}
				else if ((int)distance < 2) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
				}
				else {
					DrawString(std::string("Spice Crate » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
					{
						DrawCircle(Screen.x, Screen.y, 3, 25, color);
					}
				}
		}
		// MERCHANT - WOOD
		else if (Vars.ESP.MenuTake.ItemsOther && name.find("BP_MerchantCrate_WoodCrateProxy_C") != std::wstring::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorCommon[0],Vars.ESP.colorCommon[1],Vars.ESP.colorCommon[2],Vars.ESP.colorCommon[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if ((int)distance < 10) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
				}
				else if ((int)distance < 5) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
				}
				else if ((int)distance < 2) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
				}
				else {
					DrawString(std::string("Wood Crate » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
					{
						DrawCircle(Screen.x, Screen.y, 3, 25, color);
					}
				}
		}
		// MERCHANT - CANNONBALL
		else if (Vars.ESP.MenuTake.ItemsOther && name.find("BP_MerchantCrate_CannonballCrateProxy") != std::wstring::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorCommon[0],Vars.ESP.colorCommon[1],Vars.ESP.colorCommon[2],Vars.ESP.colorCommon[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if ((int)distance < 10) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
				}
				else if ((int)distance < 5) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
				}
				else if ((int)distance < 2) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
				}
				else {
					DrawString(std::string("Cannonball Crate » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
					{
						DrawCircle(Screen.x, Screen.y, 3, 25, color);
					}
				}
		}
		//HUMBLE GIFT
		else if (Vars.ESP.MenuTake.ItemsLegendary && name.find("Gift") != std::wstring::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorSpecial[0],Vars.ESP.colorSpecial[1],Vars.ESP.colorSpecial[2],Vars.ESP.colorSpecial[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if ((int)distance < 10) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
				}
				else if ((int)distance < 5) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
				}
				else if ((int)distance < 2) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
				}
				else {
					DrawString(std::string("Event Gift » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
					{
						DrawCircle(Screen.x, Screen.y, 3, 25, color);
					}
				}
		}
		//GUNPOWDER
		else if (Vars.ESP.MenuTake.ItemsUtility && name.find("Gunpowder") != std::wstring::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorCommon[0],Vars.ESP.colorCommon[1],Vars.ESP.colorCommon[2],Vars.ESP.colorCommon[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if ((int)distance < 10) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
				}
				else if ((int)distance < 5) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
				}
				else if ((int)distance < 2) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
				}
				else {
					DrawString(std::string("Gunpowder Barrel » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
					{
						DrawCircle(Screen.x, Screen.y, 3, 25, color);
					}
				}
		}
		//DAMNED KEY
		else if (name.find("BP_FotD_StrongholdKey") != std::string::npos || name.find("BP_FotD_StrongholdKey_Proxy") != std::string::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			Color color = { Vars.ESP.colorDKey[0],Vars.ESP.colorDKey[1],Vars.ESP.colorDKey[2],Vars.ESP.colorDKey[3] };
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if (Vars.ESP.MenuTake.ItemsOther || Vars.ESP.MenuTake.ItemsLegendary)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::string("Damned Key » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "medium");
					}
				}
		}
		// GEM
		else if (Vars.ESP.MenuTake.ItemsRare && name.find("Gem") != std::wstring::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorRare[0],Vars.ESP.colorRare[1],Vars.ESP.colorRare[2],Vars.ESP.colorRare[3] };

			if (name.find("Ruby") != std::string::npos)
			{
				Color color = Color{ Vars.ESP.MenuTake.Ruby[0], Vars.ESP.MenuTake.Ruby[1], Vars.ESP.MenuTake.Ruby[2], Vars.ESP.MenuTake.Ruby[3] };
			}
			else if (name.find("Emerald") != std::string::npos)
			{
				Color color = Color{ Vars.ESP.MenuTake.Emerald[0], Vars.ESP.MenuTake.Emerald[1], Vars.ESP.MenuTake.Emerald[2], Vars.ESP.MenuTake.Emerald[3] };
			}
			else if (name.find("Sapphire") != std::string::npos)
			{
				Color color = Color{ Vars.ESP.MenuTake.Sapphire[0], Vars.ESP.MenuTake.Sapphire[1], Vars.ESP.MenuTake.Sapphire[2], Vars.ESP.MenuTake.Sapphire[3] };
			}

			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if ((int)distance < 10) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
				}
				else if ((int)distance < 5) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
				}
				else if ((int)distance < 2) {
					DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
				}
				else {
					DrawString(std::string("Marmaid Gem » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
					{
						DrawCircle(Screen.x, Screen.y, 3, 25, color);
					}
				}
		}
		//CLOUD
		else if (name.find("BP_SkellyShip_ShipCloud") != std::string::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorWorld[0],Vars.ESP.colorWorld[1],Vars.ESP.colorWorld[2],Vars.ESP.colorWorld[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if (Vars.ESP.MenuTake.EventsCloud)
				{
					if ((int)distance >= 1500)
					{
						DrawString(std::string("Ship Cloud » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
					}
					else if ((int)distance < 1500) {
						DrawString(std::string("Ship Cloud » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
					}
					else if ((int)distance < 100) {
						DrawString(std::string("Ship Cloud » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "mini");
					}
					else
					{
						DrawString(std::string("Ship Cloud » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
					}
				}
		}

		//FOTD
		else if (name.find("RitualSkullCloud") != std::string::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorWorld[0],Vars.ESP.colorWorld[1],Vars.ESP.colorWorld[2],Vars.ESP.colorWorld[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if (Vars.ESP.MenuTake.EventsFort)
				{
					if ((int)distance >= 1500)
					{
						DrawString(std::string("FOTD » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
					}
					else if ((int)distance < 1500) {
						DrawString(std::string("FOTD » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
					}
					else if ((int)distance < 100) {
						DrawString(std::string("FOTD » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "mini");
					}
					else
					{
						DrawString(std::string("FOTD » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
					}
				}
		}


		//FORT
		else if (name.find("SkullCloud") != std::string::npos && name.find("Ritual") == std::string::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorWorld[0],Vars.ESP.colorWorld[1],Vars.ESP.colorWorld[2],Vars.ESP.colorWorld[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if (Vars.ESP.MenuTake.EventsFort)
				{
					if ((int)distance >= 1500)
					{
						DrawString(std::string("Skull Fort » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
					}
					else if ((int)distance < 1500) {
						DrawString(std::string("Skull Fort » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
					}
					else if ((int)distance < 100) {
						DrawString(std::string("Skull Fort » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "mini");
					}
					else
					{
						DrawString(std::string("Skull Fort » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
					}
				}
		}

		//MAP PINS & REAPERS CHEST
		else if (name.find("MapTable_C") != std::string::npos)
		{
			auto Table = *reinterpret_cast<AMapTable*>(&actors[i]);


			auto Ships = Table.GetTrackedShips();

			if (Ships.IsValid())
			{

				for (int p = 0; p < Ships.Length(); ++p)
				{
					auto Ship = Ships[p];

					for (int c = 0; c < 6; ++c)
					{
						if (Ship.GetCrewId() == SOT->Ships[c].crewID)
						{
							int id = Ship.GetUObject().nameId;
							auto temp = getNameFromIDmem(id);
							SOT->Ships[c].type = getNameFromIDmem(Ship.GetUObject().nameId);
						}
					}

				}
			}

			auto Chests = Table.GetTrackedBootyItemLocations();

			if (Chests.IsValid())
			{

				for (int p = 0; p < Chests.Length(); ++p)
				{
					auto Chest = Chests[p];
					Vector2 Screen;

					Color color = { Vars.ESP.colorSpecial[0],Vars.ESP.colorSpecial[1],Vars.ESP.colorSpecial[2],Vars.ESP.colorSpecial[3] };

					int dist = (int)(SOT->localPlayer.position.DistTo(Chest) / 100.f);

					if (Misc->WorldToScreen(Chest, &Screen))
						if (Vars.ESP.MenuTake.EventsReapers || Vars.ESP.MenuTake.ItemsLegendary)
						{
							if (dist < 10) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
							}
							else if (dist < 5) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
							}
							else if (dist < 2) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
							}
							else {
								DrawString(std::string("Reapers Chest » " + std::to_string(dist) + "m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
								if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
								{
									DrawCircle(Screen.x, Screen.y, 3, 25, color);
								}
							}
						}
				}
			}

			auto pins = Table.GetMapPins();

			if (!pins.IsValid())
				continue;
		
			Color color = { Vars.ESP.colorPins[0],Vars.ESP.colorPins[1],Vars.ESP.colorPins[2],Vars.ESP.colorPins[3] };

			for (int p = 0; p < pins.Length(); ++p)
			{
				auto pin = pins[p];
				
				Vector3 worldPin = Vector3(pin.x * 100, pin.y * 100, 5000);

				Vector2 Screen;
				if (Misc->WorldToScreen(worldPin, &Screen))
					if (Vars.ESP.MenuTake.MapPins)
					{
						DrawString(std::string("» " + std::to_string((int)(SOT->localPlayer.position.DistTo(worldPin) / 100.f)) + "m «").c_str(), Screen.x, Screen.y, color, true, "small");
					}
			}
		}

		//PLAYER LIST
		else if (name.find("CrewService") != std::string::npos)
		{
			Crews.clear();

			Team tempTeam;
			TeamMate tempPlayers;
			auto crewService = *reinterpret_cast<ACrewService*>(&actors[i]);;
			auto crews = crewService.GetCrews();
			if (!crews.IsValid())
				continue;

			for (int c = 0; c < crews.Length(); ++c)
			{
				auto players = crews[c].GetPlayers();
				if (!players.IsValid())
					continue;

				tempTeam.teamName = crews[c].GetShipType();
				if (SOT->Ships[c].type.find("azure") != std::string::npos)
					tempTeam.color = Color(0, 255, 255);
				else if (SOT->Ships[c].type.find("regal") != std::string::npos)
					tempTeam.color = Color(255, 0, 255);
				else if (SOT->Ships[c].type.find("lucky") != std::string::npos)
					tempTeam.color = Color(0, 255, 0);
				else if (SOT->Ships[c].type.find("flaming") != std::string::npos)
					tempTeam.color = Color(255, 0, 0);
				else if (SOT->Ships[c].type.find("golden") != std::string::npos)
					tempTeam.color = Color(255, 255, 0);
				else
					tempTeam.color = Color(255, 255, 0);

				SOT->Ships[c].crewID = crews[c].GetCrewID();

				for (int p = 0; p < players.Length(); ++p)
				{
					tempPlayers.PlayerName = Misc->wstringToString(players[p].GetName());
					if (tempPlayers.PlayerName == SOT->localPlayer.name)
						SOT->localPlayer.crewID = crews[c].GetCrewID();

					SOT->Pirates[(c * 4) + p].crewID = crews[c].GetCrewID();
					SOT->Pirates[(c * 4) + p].name = tempPlayers.PlayerName;

					tempTeam.Players.push_back(tempPlayers);
				}
				Crews.push_back(tempTeam);
				tempTeam.Players.clear();
			}

		}

		//PLAYERS
		else if (name.find("BP_PlayerPirate_C") != std::string::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			int health = actor.GetHealthComponent().GetHealth();
			if (health <= 0)
				continue;

			auto ItemName = actor.GetWieldedItemComponent().GetWieldedItem().GetItemInfo().GetItemDesc().GetName();

			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				Vector3 headPos = Vector3(pos.x, pos.y, pos.z + 100);

				Vector2 ScreenTop;
				Color boxColor = { Vars.ESP.MenuTake.EnemyColor[0],Vars.ESP.MenuTake.EnemyColor[1],Vars.ESP.MenuTake.EnemyColor[2],Vars.ESP.MenuTake.EnemyColor[3] };
				bool bTeammate = false;

				if (Misc->WorldToScreen(headPos, &ScreenTop))
				{
					int hi = (Screen.y - ScreenTop.y) * 2;
					int wi = hi * 0.65;

					auto pirateName = Misc->wstringToString(actor.GetPlayerState().GetName());
				
					for (int pirates = 0; pirates < 24; ++pirates)
					{
						if (SOT->Pirates[pirates].name == "")
							continue;

						if (pirateName == SOT->Pirates[pirates].name)
						{
							SOT->Pirates[pirates].distance = distance;
							if (SOT->Pirates[pirates].crewID == SOT->localPlayer.crewID)
							{
								boxColor = Color{ Vars.ESP.MenuTake.AllyColor[0],Vars.ESP.MenuTake.AllyColor[1],Vars.ESP.MenuTake.AllyColor[2],Vars.ESP.MenuTake.AllyColor[3] };
								bTeammate = true;
								if (Vars.ESP.MenuTake.PlayerAlly)
								{
									DrawBox(ScreenTop.x - wi / 2, ScreenTop.y, wi, hi, boxColor);
								}
							}
						}
					}

					if (Vars.ESP.MenuTake.PlayerEnemy)
					{
						DrawBox(ScreenTop.x - wi / 2, ScreenTop.y, wi, hi, boxColor);
					}

					if (Vars.ESP.MenuTake.PlayerName)
						DrawString(std::string(pirateName + " » " + std::to_string((int)distance) + "m").c_str(), ScreenTop.x, ScreenTop.y - 14, boxColor, true, "small");
					if (Vars.ESP.MenuTake.PlayerItem)
					{
						if (ItemName.length() < 32)
							DrawString(ItemName.c_str(), ScreenTop.x, ScreenTop.y + hi, Color{ 255,255,255 }, true, "small");
					}
					if (Vars.ESP.MenuTake.PlayerHPBar)
					{
						float maxHealth = actor.GetHealthComponent().GetMaxHealth();
						DrawHealthBar(health, maxHealth, ScreenTop.x, ScreenTop.y + 1, wi, hi);
					}
					
					
				}
			}
		}

#ifndef Ships

		//SLOOP
		else if (name.find("BP_SmallShipTemplate_C") != std::string::npos || name.find("BP_SmallShipNetProxy") != std::string::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			auto Ship = *reinterpret_cast<AShip*>(&actors[i]);

			FGuid crewid = Ship.GetCrewOwnershipComponent().GetCrewId();

			if (name.find("NetProxy") != std::string::npos)
			{
				if (Ship.GetOwningActor())
					continue;

				crewid = FGuid();
			}

			Color color = { Vars.ESP.MenuTake.EnemyColor[0],Vars.ESP.MenuTake.EnemyColor[1],Vars.ESP.MenuTake.EnemyColor[2],Vars.ESP.MenuTake.EnemyColor[3] };

			if (SOT->localPlayer.crewID == crewid)
			{
				color = { Vars.ESP.MenuTake.AllyColor[0],Vars.ESP.MenuTake.AllyColor[1],Vars.ESP.MenuTake.AllyColor[2],Vars.ESP.MenuTake.AllyColor[3] };
			}

			Vector2 Screen;
			if (Misc->WorldToScreen(Vector3(pos.x,pos.y,pos.z + 2000), &Screen))
				if ((int)distance >= 1500 & SOT->localPlayer.crewID != crewid & Vars.ESP.MenuTake.ShipsEnemy)
				{
					DrawString(std::string("Sloop » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
				}
				else if ((int)distance < 1500 & SOT->localPlayer.crewID != crewid & Vars.ESP.MenuTake.ShipsEnemy) {
					DrawString(std::string("Sloop » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "medium");
				}
				else if ((int)distance < 50 & SOT->localPlayer.crewID != crewid & Vars.ESP.MenuTake.ShipsEnemy) {
					DrawString(std::string("Sloop » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "mini");
				}
				else if (SOT->localPlayer.crewID == crewid & Vars.ESP.MenuTake.ShipsAlly) {
					DrawString(std::string("Own Sloop » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
				}
		}
		//BRIGANTINE
		else if (name.find("BP_MediumShipTemplate_C") != std::string::npos || name.find("BP_MediumShipNetProxy") != std::string::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			auto Ship = *reinterpret_cast<AShip*>(&actors[i]);

			FGuid crewid = Ship.GetCrewOwnershipComponent().GetCrewId();

			if (name.find("NetProxy") != std::string::npos)
			{
				if (Ship.GetOwningActor())
					continue;

				crewid = FGuid();
			}

			Color color = { Vars.ESP.MenuTake.EnemyColor[0],Vars.ESP.MenuTake.EnemyColor[1],Vars.ESP.MenuTake.EnemyColor[2],Vars.ESP.MenuTake.EnemyColor[3] };

			if (SOT->localPlayer.crewID == crewid)
			{
				color = { Vars.ESP.MenuTake.AllyColor[0],Vars.ESP.MenuTake.AllyColor[1],Vars.ESP.MenuTake.AllyColor[2],Vars.ESP.MenuTake.AllyColor[3] };
			}

			Vector2 Screen;
			if (Misc->WorldToScreen(Vector3(pos.x, pos.y, pos.z + 2000), &Screen))
				if ((int)distance >= 1500 & SOT->localPlayer.crewID != crewid & Vars.ESP.MenuTake.ShipsEnemy)
				{
					DrawString(std::string("Brigantine » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
				}
				else if ((int)distance < 1500 & SOT->localPlayer.crewID != crewid & Vars.ESP.MenuTake.ShipsEnemy) {
					DrawString(std::string("Brigantine » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "medium");
				}
				else if ((int)distance < 50 & SOT->localPlayer.crewID != crewid & Vars.ESP.MenuTake.ShipsEnemy) {
					DrawString(std::string("Brigantine » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "mini");
				}
				else if (SOT->localPlayer.crewID == crewid & Vars.ESP.MenuTake.ShipsAlly) {
					DrawString(std::string("Own Brigantine » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
				}
		}
		//GALLEON
		else if (name.find("BP_LargeShipTemplate_C") != std::string::npos || name.find("BP_LargeShipNetProxy") != std::string::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			auto Ship = *reinterpret_cast<AShip*>(&actors[i]);

			FGuid crewid = Ship.GetCrewOwnershipComponent().GetCrewId();

			if (name.find("NetProxy") != std::string::npos)
			{
				if (Ship.GetOwningActor())
					continue;

				crewid = FGuid();
			}

			Color color = { Vars.ESP.MenuTake.EnemyColor[0],Vars.ESP.MenuTake.EnemyColor[1],Vars.ESP.MenuTake.EnemyColor[2],Vars.ESP.MenuTake.EnemyColor[3] };

			if (SOT->localPlayer.crewID == crewid)
			{
				color = { Vars.ESP.MenuTake.AllyColor[0],Vars.ESP.MenuTake.AllyColor[1],Vars.ESP.MenuTake.AllyColor[2],Vars.ESP.MenuTake.AllyColor[3] };
			}

			Vector2 Screen;
			if (Misc->WorldToScreen(Vector3(pos.x, pos.y, pos.z + 2000), &Screen))
				if ((int)distance >= 1500 & SOT->localPlayer.crewID != crewid & Vars.ESP.MenuTake.ShipsEnemy)
				{
					DrawString(std::string("Galleon » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
				}
				else if ((int)distance < 1500 & SOT->localPlayer.crewID != crewid & Vars.ESP.MenuTake.ShipsEnemy) {
					DrawString(std::string("Galleon » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "medium");
				}
				else if ((int)distance < 50 & SOT->localPlayer.crewID != crewid & Vars.ESP.MenuTake.ShipsEnemy) {
					DrawString(std::string("Galleon » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "mini");
				}
				else if (SOT->localPlayer.crewID == crewid & Vars.ESP.MenuTake.ShipsAlly) {
					DrawString(std::string("Own Galleon » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
				}
		}
		//ROWBOAT
		else if (name.find("BP_Rowboat_C") != std::string::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			auto Ship = *reinterpret_cast<AShip*>(&actors[i]);

			Color color = { Vars.ESP.colorCommon[0],Vars.ESP.colorCommon[1],Vars.ESP.colorCommon[2],Vars.ESP.colorCommon[3] };

			Vector2 Screen;
			if (Misc->WorldToScreen(Vector3(pos.x, pos.y, pos.z + 2000), &Screen))
				if (Vars.ESP.MenuTake.ShipsRowboat)
				{
					DrawString(std::string("Rowboat » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
				}
		}
		
		//GHOST SLOOP
		else if (name.find("BP_AISmallShipTemplate") != std::string::npos || name.find("BP_AISmallShipNetProxy") != std::string::npos)
		{
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			auto Ship = *reinterpret_cast<AShip*>(&actors[i]);

			if (name.find("NetProxy") != std::string::npos)
			{
				if (Ship.GetOwningActor())
					continue;
			}
			Color color = { Vars.ESP.colorGhostShip[0],Vars.ESP.colorGhostShip[1],Vars.ESP.colorGhostShip[2],Vars.ESP.colorGhostShip[3] };

			Vector2 Screen;
			if (Misc->WorldToScreen(Vector3(pos.x, pos.y, pos.z + 2000), &Screen))
				if (Vars.ESP.MenuTake.ShipsGhost)
				{
					if ((int)distance >= 1500)
					{
						DrawString(std::string("Ghost Sloop » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
					}
					else if ((int)distance < 1500) {
						DrawString(std::string("Ghost Sloop » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "medium");
					}
					else if ((int)distance < 50) {
						DrawString(std::string("Ghost Sloop » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "mini");
					}
					else
					{
						DrawString(std::string("Ghost Sloop » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
					}
				}
		}
		//GHOST GALLEON
		else if (name.find("BP_AILargeShipTemplate") != std::string::npos || name.find("BP_AILargeShipNetProxy") != std::string::npos)
		{
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			auto Ship = *reinterpret_cast<AShip*>(&actors[i]);

			if (name.find("NetProxy") != std::string::npos)
			{
				if (Ship.GetOwningActor())
					continue;
			}
			Color color = { Vars.ESP.colorGhostShip[0],Vars.ESP.colorGhostShip[1],Vars.ESP.colorGhostShip[2],Vars.ESP.colorGhostShip[3] };

			Vector2 Screen;
			if (Misc->WorldToScreen(Vector3(pos.x, pos.y, pos.z + 2000), &Screen))
				if (Vars.ESP.MenuTake.ShipsGhost)
				{
					if ((int)distance >= 1500)
					{
						DrawString(std::string("Ghost Galleon » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
					}
					else if ((int)distance < 1500) {
						DrawString(std::string("Ghost Galleon » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "medium");
					}
					else if ((int)distance < 50) {
						DrawString(std::string("Ghost Galleon » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "mini");
					}
					else
					{
						DrawString(std::string("Ghost Galleon » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
					}
				}
		}

#endif // !Ships
		//SHIP WRECK
		else if (name.find("BP_Shipwreck_") != std::string::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.MenuTake.ObjectsColor[0],Vars.ESP.MenuTake.ObjectsColor[1],Vars.ESP.MenuTake.ObjectsColor[2],Vars.ESP.MenuTake.ObjectsColor[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(Vector3(pos.x, pos.y, 2000), &Screen))
				if (Vars.ESP.MenuTake.ObjectsWrecks)
				{
					if ((int)distance >= 1500)
					{
						DrawString(std::string("Ship Wreck » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
					}
					else if ((int)distance < 1500) {
						DrawString(std::string("Ship Wreck » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "medium");
					}
					else if ((int)distance < 50) {
						DrawString(std::string("Ship Wreck » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "mini");
					}
					else
					{
						DrawString(std::string("Ship Wreck » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
					}
				}
		}

		//DEBUG
		else if (name.find("") != std::string::npos)
			{
			if (!Vars.ESP.MenuTake.Debug)
				continue;

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.colorWorld[0],Vars.ESP.colorWorld[1],Vars.ESP.colorWorld[2],Vars.ESP.colorWorld[3] };

			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				DrawString((name).c_str(), Screen.x, Screen.y, color, true, "small");
			}

		//SIREN
		else if (name.find("BP_Mermaid_C") != std::string::npos)
		{

			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			Color color = { Vars.ESP.MenuTake.ObjectsColor[0],Vars.ESP.MenuTake.ObjectsColor[1],Vars.ESP.MenuTake.ObjectsColor[2],Vars.ESP.MenuTake.ObjectsColor[3] };


			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
				if (Vars.ESP.MenuTake.ObjectsSiren)
				{
					DrawString(std::string("Siren » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
				}
		}

		//STATUES
		else if (name.find("BP_SunkenCurseArtefact_") != std::string::npos)
		{
			if (Vars.ESP.MenuTake.ObjectsStatues)
			{
				auto pos = actor.GetRootComponent().GetPosition();
				auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

				Color color = Color{ Vars.ESP.colorWorld[0], Vars.ESP.colorWorld[1], Vars.ESP.colorWorld[2], Vars.ESP.colorWorld[3] };

				if (name.find("Ruby") != std::string::npos)
				{
					Color color = Color{ Vars.ESP.MenuTake.Ruby[0], Vars.ESP.MenuTake.Ruby[1], Vars.ESP.MenuTake.Ruby[2], Vars.ESP.MenuTake.Ruby[3] };
				}
				else if (name.find("Emerald") != std::string::npos)
				{
					Color color = Color{ Vars.ESP.MenuTake.Emerald[0], Vars.ESP.MenuTake.Emerald[1], Vars.ESP.MenuTake.Emerald[2], Vars.ESP.MenuTake.Emerald[3] };
				}
				else if (name.find("Sapphire") != std::string::npos)
				{
					Color color = Color{ Vars.ESP.MenuTake.Sapphire[0], Vars.ESP.MenuTake.Sapphire[1], Vars.ESP.MenuTake.Sapphire[2], Vars.ESP.MenuTake.Sapphire[3] };
				}

				Vector2 Screen;
				if (Misc->WorldToScreen(Vector3(pos.x, pos.y, pos.z), &Screen))
				{
					if ((int)distance >= 150)
					{
						DrawString(std::string("Marmaid Statue » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "small");
					}
					else if ((int)distance < 150) {
						DrawString(std::string("Marmaid Statue » " + std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y, color, true, "medium");
					}
					else if ((int)distance < 10 && (int)distance > 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
				}
			}
		}
		
		//CHICKEN
		else if ((name.find("BP_Chicken_") != std::string::npos))
		{
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			auto Fauna = *reinterpret_cast<AFauna*>(&actors[i]);

			Color color = Color{ Vars.ESP.colorOther[0], Vars.ESP.colorOther[1], Vars.ESP.colorOther[2], Vars.ESP.colorOther[3] };

			if (name.find("Common") != std::wstring::npos)
				color = Color{ Vars.ESP.colorCommon[0], Vars.ESP.colorCommon[1], Vars.ESP.colorCommon[2], Vars.ESP.colorCommon[3] };

			else if (name.find("Rare") != std::wstring::npos)
				color = Color{ Vars.ESP.colorRare[0], Vars.ESP.colorRare[1], Vars.ESP.colorRare[2], Vars.ESP.colorRare[3] };

			else if (name.find("Mythical") != std::wstring::npos)
				color = Color{ Vars.ESP.colorMythical[0], Vars.ESP.colorMythical[1], Vars.ESP.colorMythical[2], Vars.ESP.colorMythical[3] };

			else if (name.find("Legendary") != std::wstring::npos)
				color = Color{ Vars.ESP.colorLegendary[0], Vars.ESP.colorLegendary[1], Vars.ESP.colorLegendary[2], Vars.ESP.colorLegendary[3] };

			Vector2 Screen;

			if (Misc->WorldToScreen(pos, &Screen))
				if (Vars.ESP.MenuTake.AnimalsChickens)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::wstring(Fauna.GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					}
				}
				else if (Vars.ESP.MenuTake.AnimalsCommon & name.find("Common") != std::wstring::npos)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::wstring(Fauna.GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					}
				}
				else if (Vars.ESP.MenuTake.AnimalsRare & name.find("Rare") != std::wstring::npos)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::wstring(Fauna.GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					}
				}
				else if (Vars.ESP.MenuTake.AnimalsMythical & name.find("Mythical") != std::wstring::npos)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::wstring(Fauna.GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					}
				}
				else if (Vars.ESP.MenuTake.AnimalsLegendary & name.find("Legendary") != std::wstring::npos)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::wstring(Fauna.GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					}
				}
		}

		//SNAKE
		else if ((name.find("BP_Snake_") != std::string::npos))
		{
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			auto Fauna = *reinterpret_cast<AFauna*>(&actors[i]);

			Color color = Color{ Vars.ESP.colorOther[0], Vars.ESP.colorOther[1], Vars.ESP.colorOther[2], Vars.ESP.colorOther[3] };

			if (name.find("Common") != std::wstring::npos)
				color = Color{ Vars.ESP.colorCommon[0], Vars.ESP.colorCommon[1], Vars.ESP.colorCommon[2], Vars.ESP.colorCommon[3] };

			else if (name.find("Rare") != std::wstring::npos)
				color = Color{ Vars.ESP.colorRare[0], Vars.ESP.colorRare[1], Vars.ESP.colorRare[2], Vars.ESP.colorRare[3] };

			else if (name.find("Mythical") != std::wstring::npos)
				color = Color{ Vars.ESP.colorMythical[0], Vars.ESP.colorMythical[1], Vars.ESP.colorMythical[2], Vars.ESP.colorMythical[3] };

			else if (name.find("Legendary") != std::wstring::npos)
				color = Color{ Vars.ESP.colorLegendary[0], Vars.ESP.colorLegendary[1], Vars.ESP.colorLegendary[2], Vars.ESP.colorLegendary[3] };

			Vector2 Screen;

			if (Misc->WorldToScreen(pos, &Screen))
				if (Vars.ESP.MenuTake.AnimalsSnakes)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::wstring(Fauna.GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					}
				}
				else if (Vars.ESP.MenuTake.AnimalsCommon & name.find("Common") != std::wstring::npos)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::wstring(Fauna.GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					}
				}
				else if (Vars.ESP.MenuTake.AnimalsRare & name.find("Rare") != std::wstring::npos)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::wstring(Fauna.GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					}
				}
				else if (Vars.ESP.MenuTake.AnimalsMythical & name.find("Mythical") != std::wstring::npos)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::wstring(Fauna.GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					}
				}
				else if (Vars.ESP.MenuTake.AnimalsLegendary & name.find("Legendary") != std::wstring::npos)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::wstring(Fauna.GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					}
				}
		}

		//PIG
		else if ((name.find("BP_Pig_") != std::string::npos))
		{
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			auto Fauna = *reinterpret_cast<AFauna*>(&actors[i]);

			Color color = Color{ Vars.ESP.colorOther[0], Vars.ESP.colorOther[1], Vars.ESP.colorOther[2], Vars.ESP.colorOther[3] };

			if (name.find("Common") != std::wstring::npos)
				color = Color{ Vars.ESP.colorCommon[0], Vars.ESP.colorCommon[1], Vars.ESP.colorCommon[2], Vars.ESP.colorCommon[3] };

			else if (name.find("Rare") != std::wstring::npos)
				color = Color{ Vars.ESP.colorRare[0], Vars.ESP.colorRare[1], Vars.ESP.colorRare[2], Vars.ESP.colorRare[3] };

			else if (name.find("Mythical") != std::wstring::npos)
				color = Color{ Vars.ESP.colorMythical[0], Vars.ESP.colorMythical[1], Vars.ESP.colorMythical[2], Vars.ESP.colorMythical[3] };

			else if (name.find("Legendary") != std::wstring::npos)
				color = Color{ Vars.ESP.colorLegendary[0], Vars.ESP.colorLegendary[1], Vars.ESP.colorLegendary[2], Vars.ESP.colorLegendary[3] };

			Vector2 Screen;

			if (Misc->WorldToScreen(pos, &Screen))
				if (Vars.ESP.MenuTake.AnimalsPigs)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::wstring(Fauna.GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					}
				}
				else if (Vars.ESP.MenuTake.AnimalsCommon & name.find("Common") != std::wstring::npos)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::wstring(Fauna.GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					}
				}
				else if (Vars.ESP.MenuTake.AnimalsRare & name.find("Rare") != std::wstring::npos)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::wstring(Fauna.GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					}
				}
				else if (Vars.ESP.MenuTake.AnimalsMythical & name.find("Mythical") != std::wstring::npos)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::wstring(Fauna.GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					}
				}
				else if (Vars.ESP.MenuTake.AnimalsLegendary & name.find("Legendary") != std::wstring::npos)
				{
					if ((int)distance < 10) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
					}
					else if ((int)distance < 5) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
					}
					else if ((int)distance < 2) {
						DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
					}
					else {
						DrawString(std::wstring(Fauna.GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
					}
				}
		}

		//ITEMS
		else if (name.find("Proxy") != std::string::npos)
		{
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;

			auto treasure = *reinterpret_cast<AItemProxy*>(&actors[i]);

			int booty = treasure.GetBootyItemInfo().GetBootyType();

			if (booty > EBootyTypes::EBootyTypes__EBootyTypes_MAX || booty < 1)
				continue;

			auto rarity = getNameFromIDmem(treasure.GetBootyItemInfo().GetRareityId());

			Color color = Color{ Vars.ESP.colorOther[0], Vars.ESP.colorOther[1], Vars.ESP.colorOther[2], Vars.ESP.colorOther[3] };

			Vector2 Screen;

			if (Misc->WorldToScreen(pos, &Screen))
			{
				if (name.find("Common") != std::wstring::npos && name.find("GunpowderBarrel") == std::wstring::npos && name.find("ChickenCrate") == std::wstring::npos && name.find("PigCrate") == std::wstring::npos && name.find("BananaCrate") == std::wstring::npos && name.find("WoodCrate") == std::wstring::npos && name.find("SnakeBasket") == std::wstring::npos && name.find("CannonballCrate") == std::wstring::npos)
				{
					color = Color{ Vars.ESP.colorCommon[0], Vars.ESP.colorCommon[1], Vars.ESP.colorCommon[2], Vars.ESP.colorCommon[3] };

					if (Vars.ESP.MenuTake.ItemsCommon)
					{
						if (Misc->WorldToScreen(pos, &Screen))
						{
							if ((int)distance < 10) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
							}
							else if ((int)distance < 5) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
							}
							else if ((int)distance < 2) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
							}
							else {
								DrawString(std::wstring(treasure.GetBootyItemInfo().GetItemDesc().GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
								if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
								{
									DrawCircle(Screen.x, Screen.y, 3, 25, color);
								}
							}
						}
					}
				}
				else if (name.find("Rare") != std::wstring::npos)
				{
					color = Color{ Vars.ESP.colorRare[0], Vars.ESP.colorRare[1], Vars.ESP.colorRare[2], Vars.ESP.colorRare[3] };

					if (Vars.ESP.MenuTake.ItemsRare)
					{
						if (Misc->WorldToScreen(pos, &Screen))
						{
							if ((int)distance < 10) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
							}
							else if ((int)distance < 5) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
							}
							else if ((int)distance < 2) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
							}
							else {
								DrawString(std::wstring(treasure.GetBootyItemInfo().GetItemDesc().GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
								if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
								{
									DrawCircle(Screen.x, Screen.y, 3, 25, color);
								}
							}
						}
					}
				}
				else if (name.find("Mythical") != std::wstring::npos)
				{
					color = Color{ Vars.ESP.colorMythical[0], Vars.ESP.colorMythical[1], Vars.ESP.colorMythical[2], Vars.ESP.colorMythical[3] };

					if (Vars.ESP.MenuTake.ItemsMythical)
					{
						if (Misc->WorldToScreen(pos, &Screen))
						{
							if ((int)distance < 10) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
							}
							else if ((int)distance < 5) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
							}
							else if ((int)distance < 2) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
							}
							else {
								DrawString(std::wstring(treasure.GetBootyItemInfo().GetItemDesc().GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
								if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
								{
									DrawCircle(Screen.x, Screen.y, 3, 25, color);
								}
							}
						}
					}
				}
				else if (name.find("Legendary") != std::wstring::npos)
				{
					color = Color{ Vars.ESP.colorLegendary[0], Vars.ESP.colorLegendary[1], Vars.ESP.colorLegendary[2], Vars.ESP.colorLegendary[3] };

					if (Vars.ESP.MenuTake.ItemsLegendary)
					{
						if (Misc->WorldToScreen(pos, &Screen))
						{
							if ((int)distance < 10) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
							}
							else if ((int)distance < 5) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
							}
							else if ((int)distance < 2) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
							}
							else {
								DrawString(std::wstring(treasure.GetBootyItemInfo().GetItemDesc().GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
								if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
								{
									DrawCircle(Screen.x, Screen.y, 3, 25, color);
								}
							}
						}
					}
				}
				else if (name.find("Fort") != std::string::npos || name.find("Stronghold") != std::string::npos || name.find("PirateLegend") != std::string::npos || name.find("Drunken") != std::string::npos || name.find("Weeping") != std::string::npos || name.find("AIShip") != std::string::npos || name.find("Ashen") != std::string::npos || name.find("DVR") != std::string::npos)
				{
					color = Color{ Vars.ESP.colorSpecial[0], Vars.ESP.colorSpecial[1], Vars.ESP.colorSpecial[2], Vars.ESP.colorSpecial[3] };

					if (Vars.ESP.MenuTake.ItemsLegendary)
					{
						if (Misc->WorldToScreen(pos, &Screen))
						{
							if ((int)distance < 10) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
							}
							else if ((int)distance < 5) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
							}
							else if ((int)distance < 2) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
							}
							else {
								DrawString(std::wstring(treasure.GetBootyItemInfo().GetItemDesc().GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
								if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
								{
									DrawCircle(Screen.x, Screen.y, 3, 25, color);
								}
							}
						}
					}
				}
				else if (rarity.find("Fort") != std::string::npos || rarity.find("Stronghold") != std::string::npos || rarity.find("PirateLegend") != std::string::npos || rarity.find("Drunken") != std::string::npos || rarity.find("Weeping") != std::string::npos || rarity.find("AIShip") != std::string::npos || rarity.find("Ashen") != std::string::npos || rarity.find("DVR") != std::string::npos)
				{
					color = Color{ Vars.ESP.colorSpecial[0], Vars.ESP.colorSpecial[1], Vars.ESP.colorSpecial[2], Vars.ESP.colorSpecial[3] };

					if (Vars.ESP.MenuTake.ItemsLegendary)
					{
						if (Misc->WorldToScreen(pos, &Screen))
						{
							if ((int)distance < 10) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_3");
							}
							else if ((int)distance < 5) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_2");
							}
							else if ((int)distance < 2) {
								DrawString("» • «", Screen.x, Screen.y, color, true, "dist_1");
							}
							else {
								DrawString(std::wstring(treasure.GetBootyItemInfo().GetItemDesc().GetName() + L" » " + std::to_wstring((int)distance) + L"m").c_str(), Screen.x, Screen.y + 5, color, true, "small");
								if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
								{
									DrawCircle(Screen.x, Screen.y, 3, 25, color);
								}
							}
						}
					}
				}
			}
		}
	}
}