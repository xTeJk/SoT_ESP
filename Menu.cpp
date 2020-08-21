#include "Menu.h"


cMenu* Menu = new cMenu();

void cMenu::DrawMenu()
{

}

void cDrawing::Draw()
{
	Cheat->readData();
	//
	static int counter = 0;
	static float colors[3] = { 1.f, 0.f, 0.f };

	if (colors[counter] >= 1.0f)
	{
		colors[counter] = 1.0f;
		counter += 1;
		if (counter > 2)
			counter = 0;
	}
	else
	{
		int prev = counter - 1;
		if (prev < 0) prev = 2;
		colors[prev] -= 0.05f;
		colors[counter] += 0.05f;
	}
	//
	String(std::string("SEA OF TAKE - 2.0.17.2").c_str(), 10, Process->Size[1] - 20, D3DCOLOR_XRGB(163, 116, 219), false, "small");

	if (Vars.ESP.MenuV2.MenuActive)
		String(std::string("HOME » Shown » Menu").c_str(), 135, Process->Size[1] - 20, D3DCOLOR_XRGB(61, 168, 71), false, "small");
	else
		String(std::string("HOME » Hidden » Menu").c_str(), 135, Process->Size[1] - 20, D3DCOLOR_XRGB(171, 58, 75), false, "small");

	String(std::string("END » Close Cheat").c_str(), 255, Process->Size[1] - 20, D3DCOLOR_XRGB(163, 116, 219), false, "small");
	//FPS
	String(std::to_string(directX->frames).c_str(), Process->Size[0] - 35, Process->Size[1] - 20, D3DCOLOR_XRGB(163, 116, 219), false, "small");
	//


	if (Vars.GUI)
	{
		//MAIN MENU
		if (Vars.ESP.MenuV2.MenuActive && !Vars.ESP.MenuV2.F1 && !Vars.ESP.MenuV2.F2 && !Vars.ESP.MenuV2.F3 && !Vars.ESP.MenuV2.F4 && !Vars.ESP.MenuV2.F5 && !Vars.ESP.MenuV2.F6 && !Vars.ESP.MenuV2.F7)
		{
			String(std::string("F1 » Show » General").c_str(), 10, 10, D3DCOLOR_XRGB(79, 255, 167), false, "small");
			String(std::string("F2 » Show » Players/Mobs").c_str(), 10, 25, D3DCOLOR_XRGB(79, 255, 167), false, "small");
			String(std::string("F3 » Show » Animals").c_str(), 10, 40, D3DCOLOR_XRGB(79, 255, 167), false, "small");
			String(std::string("F4 » Show » Treasures").c_str(), 10, 55, D3DCOLOR_XRGB(79, 255, 167), false, "small");
			String(std::string("F5 » Show » Events").c_str(), 10, 70, D3DCOLOR_XRGB(79, 255, 167), false, "small");
			String(std::string("F6 » Show » Ships").c_str(), 10, 85, D3DCOLOR_XRGB(79, 255, 167), false, "small");
			String(std::string("F7 » Show » Objects").c_str(), 10, 100, D3DCOLOR_XRGB(79, 255, 167), false, "small");
		}

		//F1
		if (Vars.ESP.MenuV2.MenuActive && Vars.ESP.MenuV2.F1)
		{
			if (Vars.ESP.MenuV2.F1)
				String(std::string("« F1 » Return").c_str(), 10, 10, D3DCOLOR_XRGB(128, 128, 128), false, "small");

			if (Menu->menuOpen)
				String(std::string("NUM1 » Shown » Players List").c_str(), 10, 25, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM1 » Hidden » Players List").c_str(), 10, 25, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.MapPins)
				String(std::string("NUM2 » Shown » Map Pins").c_str(), 10, 40, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM2 » Hidden » Map Pins").c_str(), 10, 40, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.ColorsLegend)
				String(std::string("NUM3 » Shown » Colors Legend").c_str(), 10, 55, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM3 » Hidden » Colors Legend").c_str(), 10, 55, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.LoreBooks)
				String(std::string("NUM4 » Shown » Lore Books").c_str(), 10, 70, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM4 » Hidden » Lore Books").c_str(), 10, 70, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuV2.F1)
				String(std::string("NUM5 » Save Config").c_str(), 10, 85, D3DCOLOR_XRGB(163, 116, 219), false, "small");

			if (Vars.ESP.MenuV2.F1)
				String(std::string("NUM6 » Reload Config").c_str(), 10, 100, D3DCOLOR_XRGB(163, 116, 219), false, "small");

			if (Vars.ESP.MenuTake.Debug)
				String(std::string("NUM7 » Enabled » Debug Mode").c_str(), 10, 115, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM7 » Disabled » Debug Mode").c_str(), 10, 115, D3DCOLOR_XRGB(171, 58, 75), false, "small");
		}

		//F2
		if (Vars.ESP.MenuV2.MenuActive && Vars.ESP.MenuV2.F2)
		{
			if (Vars.ESP.MenuV2.F2)
				String(std::string("« F2 » Return").c_str(), 10, 10, D3DCOLOR_XRGB(128, 128, 128), false, "small");

			if (Vars.ESP.MenuTake.PlayerEnemy)
				String(std::string("NUM1 » Shown » Enemies ESP").c_str(), 10, 25, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM1 » Hidden » Enemies ESP").c_str(), 10, 25, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.PlayerAlly)
				String(std::string("NUM2 » Shown » Allies ESP").c_str(), 10, 40, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM2 » Hidden » Allies ESP").c_str(), 10, 40, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.PlayerHPBar)
				String(std::string("NUM3 » Shown » Player's Health Bar").c_str(), 10, 55, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM3 » Hidden » Player's Health Bar").c_str(), 10, 55, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.PlayerName)
				String(std::string("NUM4 » Shown » Player's Name").c_str(), 10, 70, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM4 » Hidden » Player's Name").c_str(), 10, 70, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.PlayerItem)
				String(std::string("NUM5 » Shown » Player's Current Item").c_str(), 10, 85, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM5 » Hidden » Player's Current Item").c_str(), 10, 85, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.Skeleton)
				String(std::string("NUM6 » Shown » Skeletons ESP").c_str(), 10, 100, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM6 » Hidden » Skeletons ESP").c_str(), 10, 100, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.SkeletonName)
				String(std::string("NUM7 » Shown » Skeleton's Name").c_str(), 10, 115, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM7 » Hidden » Skeleton's Name").c_str(), 10, 115, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.SkeletonItem)
				String(std::string("NUM8 » Shown » Skeleton's Current Item").c_str(), 10, 130, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM8 » Hidden » Skeleton's Current Item").c_str(), 10, 130, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.WaterMobs)
				String(std::string("NUM9 » Shown » Water Mobs ESP").c_str(), 10, 145, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM9 » Hidden » Water Mobs ESP").c_str(), 10, 145, D3DCOLOR_XRGB(171, 58, 75), false, "small");
		}

		//F3
		if (Vars.ESP.MenuV2.MenuActive && Vars.ESP.MenuV2.F3)
		{
			if (Vars.ESP.MenuV2.F3)
				String(std::string("« F3 » Return").c_str(), 10, 10, D3DCOLOR_XRGB(128, 128, 128), false, "small");

			if (Vars.ESP.MenuTake.AnimalsCommon)
				String(std::string("NUM1 » Shown » Common Animals").c_str(), 10, 25, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM1 » Hidden » Common Animals").c_str(), 10, 25, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.AnimalsRare)
				String(std::string("NUM2 » Shown » Rare Animals").c_str(), 10, 40, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM2 » Hidden » Rare Animals").c_str(), 10, 40, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.AnimalsMythical)
				String(std::string("NUM3 » Shown » Mythical Animals").c_str(), 10, 55, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM3 » Hidden » Mythical Animals").c_str(), 10, 55, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.AnimalsLegendary)
				String(std::string("NUM4 » Shown » Legendary Animals").c_str(), 10, 70, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM4 » Hidden » Legendary Animals").c_str(), 10, 70, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.AnimalsSnakes)
				String(std::string("NUM5 » Shown » All Snakes").c_str(), 10, 85, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM5 » Hidden » All Snakes").c_str(), 10, 85, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.AnimalsChickens)
				String(std::string("NUM6 » Shown » All Chickens").c_str(), 10, 100, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM6 » Hidden » All Chickens").c_str(), 10, 100, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.AnimalsPigs)
				String(std::string("NUM6 » Shown » All Pigs").c_str(), 10, 115, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM6 » Hidden » All Pigs").c_str(), 10, 115, D3DCOLOR_XRGB(171, 58, 75), false, "small");
		}

		//F4
		if (Vars.ESP.MenuV2.MenuActive && Vars.ESP.MenuV2.F4)
		{
			if (Vars.ESP.MenuV2.F4)
				String(std::string("« F4 » Return").c_str(), 10, 10, D3DCOLOR_XRGB(128, 128, 128), false, "small");

			if (Vars.ESP.MenuTake.ItemsCommon)
				String(std::string("NUM1 » Shown » Common Items").c_str(), 10, 25, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM1 » Hidden » Common Items").c_str(), 10, 25, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.ItemsRare)
				String(std::string("NUM2 » Shown » Rare Items").c_str(), 10, 40, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM2 » Hidden » Rare Items").c_str(), 10, 40, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.ItemsMythical)
				String(std::string("NUM3 » Shown » Mythical Items").c_str(), 10, 55, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM3 » Hidden » Mythical Items").c_str(), 10, 55, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.ItemsLegendary)
				String(std::string("NUM4 » Shown » Legendary/Special Items").c_str(), 10, 70, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM4 » Hidden » Legendary/Special Items").c_str(), 10, 70, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.ItemsUtility)
				String(std::string("NUM5 » Shown » Gunpowder Barrels").c_str(), 10, 85, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM5 » Hidden » Gunpowder Barrels").c_str(), 10, 85, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.ItemsBarrels)
				String(std::string("NUM6 » Shown » Barrels in Water").c_str(), 10, 100, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM6 » Hidden » Barrels in Water").c_str(), 10, 100, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.ItemsCages)
				String(std::string("NUM7 » Shown » Animal Cages").c_str(), 10, 115, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM7 » Hidden » Animal Cages").c_str(), 10, 115, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.ItemsOther)
				String(std::string("NUM8 » Shown » Others (e.g. Ammo Crate)").c_str(), 10, 130, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM8 » Hidden » Others (e.g. Ammo Crate)").c_str(), 10, 130, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.ItemsHarpoonSupport)
				String(std::string("NUM9 » Enabled » Harpoon Support").c_str(), 10, 145, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM9 » Disabled » Harpoon Support").c_str(), 10, 145, D3DCOLOR_XRGB(171, 58, 75), false, "small");
		}
		
		//F5
		if (Vars.ESP.MenuV2.MenuActive && Vars.ESP.MenuV2.F5)
		{
			if (Vars.ESP.MenuV2.F5)
				String(std::string("« F5 » Return").c_str(), 10, 10, D3DCOLOR_XRGB(128, 128, 128), false, "small");

			if (Vars.ESP.MenuTake.EventsFort)
				String(std::string("NUM1 » Shown » Fort Skull").c_str(), 10, 25, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM1 » Hidden » Fort Skull").c_str(), 10, 25, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.EventsCloud)
				String(std::string("NUM2 » Shown » Cloud Ship").c_str(), 10, 40, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM2 » Hidden » Cloud Ship").c_str(), 10, 40, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.EventsReapers)
				String(std::string("NUM3 » Shown » Reapers Chest").c_str(), 10, 55, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM3 » Hidden » Reapers Chest").c_str(), 10, 55, D3DCOLOR_XRGB(171, 58, 75), false, "small");
		}
		
		//F6
		if (Vars.ESP.MenuV2.MenuActive && Vars.ESP.MenuV2.F6)
		{
			if (Vars.ESP.MenuV2.F6)
				String(std::string("« F6 » Return").c_str(), 10, 10, D3DCOLOR_XRGB(128, 128, 128), false, "small");

			if (Vars.ESP.MenuTake.ShipsEnemy)
				String(std::string("NUM1 » Shown » Enemy Ships").c_str(), 10, 25, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM1 » Hidden » Enemy Ships").c_str(), 10, 25, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.ShipsAlly)
				String(std::string("NUM2 » Shown » Ally Ships").c_str(), 10, 40, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM2 » Hidden » Ally Ships").c_str(), 10, 40, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.ShipsRowboat)
				String(std::string("NUM3 » Shown » Rowboats").c_str(), 10, 55, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM3 » Hidden » Rowboats").c_str(), 10, 55, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.ShipsGhost)
				String(std::string("NUM4 » Shown » Ghost Ships").c_str(), 10, 70, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM4 » Hidden » Ghost Ships").c_str(), 10, 70, D3DCOLOR_XRGB(171, 58, 75), false, "small");
		}
		
		//F7
		if (Vars.ESP.MenuV2.MenuActive && Vars.ESP.MenuV2.F7)
		{
			if (Vars.ESP.MenuV2.F7)
				String(std::string("« F7 » Return").c_str(), 10, 10, D3DCOLOR_XRGB(128, 128, 128), false, "small");

			if (Vars.ESP.MenuTake.ObjectsSiren)
				String(std::string("NUM1 » Shown » Sirens (Back to Ship)").c_str(), 10, 25, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM1 » Hidden » Sirens (Back to Ship)").c_str(), 10, 25, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.ObjectsStatues)
				String(std::string("NUM2 » Shown » Statues").c_str(), 10, 40, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM2 » Hidden » Statues").c_str(), 10, 40, D3DCOLOR_XRGB(171, 58, 75), false, "small");

			if (Vars.ESP.MenuTake.ObjectsWrecks)
				String(std::string("NUM3 » Shown » Ship Wrecks").c_str(), 10, 55, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			else
				String(std::string("NUM3 » Hidden » Ship Wrecks").c_str(), 10, 55, D3DCOLOR_XRGB(171, 58, 75), false, "small");
		}

		//LEGEND
		if (Vars.ESP.MenuTake.ColorsLegend) {
			String(std::string("» Common").c_str(), 10, Process->Size[1] - 209, D3DCOLOR_XRGB(Vars.ESP.colorCommon[0], Vars.ESP.colorCommon[1], Vars.ESP.colorCommon[2], Vars.ESP.colorCommon[3]), false, "small");
			String(std::string("» Rare").c_str(), 10, Process->Size[1] - 197, D3DCOLOR_XRGB(Vars.ESP.colorRare[0], Vars.ESP.colorRare[1], Vars.ESP.colorRare[2], Vars.ESP.colorRare[3]), false, "small");
			String(std::string("» Mythical").c_str(), 10, Process->Size[1] - 185, D3DCOLOR_XRGB(Vars.ESP.colorMythical[0], Vars.ESP.colorMythical[1], Vars.ESP.colorMythical[2], Vars.ESP.colorMythical[3]), false, "small");
			String(std::string("» Legendary").c_str(), 10, Process->Size[1] - 173, D3DCOLOR_XRGB(Vars.ESP.colorLegendary[0], Vars.ESP.colorLegendary[1], Vars.ESP.colorLegendary[2], Vars.ESP.colorLegendary[3]), false, "small");
			String(std::string("» Special").c_str(), 10, Process->Size[1] - 161, D3DCOLOR_XRGB(Vars.ESP.colorSpecial[0], Vars.ESP.colorSpecial[1], Vars.ESP.colorSpecial[2], Vars.ESP.colorSpecial[3]), false, "small");
			String(std::string("» Other").c_str(), 10, Process->Size[1] - 149, D3DCOLOR_XRGB(Vars.ESP.colorOther[0], Vars.ESP.colorOther[1], Vars.ESP.colorOther[2], Vars.ESP.colorOther[3]), false, "small");
			String(std::string("» Default").c_str(), 10, Process->Size[1] - 137, D3DCOLOR_XRGB(Vars.ESP.colorWorld[0], Vars.ESP.colorWorld[1], Vars.ESP.colorWorld[2], Vars.ESP.colorWorld[3]), false, "small");
			String(std::string("» Ghost Ship").c_str(), 10, Process->Size[1] - 125, D3DCOLOR_XRGB(Vars.ESP.colorGhostShip[0], Vars.ESP.colorGhostShip[1], Vars.ESP.colorGhostShip[2], Vars.ESP.colorGhostShip[3]), false, "small");
			String(std::string("» Objects").c_str(), 10, Process->Size[1] - 113, D3DCOLOR_XRGB(Vars.ESP.MenuTake.ObjectsColor[0], Vars.ESP.MenuTake.ObjectsColor[1], Vars.ESP.MenuTake.ObjectsColor[2], Vars.ESP.MenuTake.ObjectsColor[3]), false, "small");
			String(std::string("» Map Pin").c_str(), 10, Process->Size[1] - 101, D3DCOLOR_XRGB(Vars.ESP.colorPins[0], Vars.ESP.colorPins[1], Vars.ESP.colorPins[2], Vars.ESP.colorPins[3]), false, "small");
			String(std::string("» Lore Books").c_str(), 10, Process->Size[1] - 89, D3DCOLOR_XRGB(Vars.ESP.MenuTake.LoreColor[0], Vars.ESP.MenuTake.LoreColor[1], Vars.ESP.MenuTake.LoreColor[2], Vars.ESP.MenuTake.LoreColor[3]), false, "small");
			String(std::string("» Allies").c_str(), 10, Process->Size[1] - 77, D3DCOLOR_XRGB(Vars.ESP.MenuTake.AllyColor[0], Vars.ESP.MenuTake.AllyColor[1], Vars.ESP.MenuTake.AllyColor[2], Vars.ESP.MenuTake.AllyColor[3]), false, "small");
			String(std::string("» Enemies").c_str(), 10, Process->Size[1] - 65, D3DCOLOR_XRGB(Vars.ESP.MenuTake.EnemyColor[0], Vars.ESP.MenuTake.EnemyColor[1], Vars.ESP.MenuTake.EnemyColor[2], Vars.ESP.MenuTake.EnemyColor[3]), false, "small");
		}
	}

	/*
	if (Vars.GUI)
	{
		if (Menu->menuOpen)
			String(std::string("F1 » Shown » Players List").c_str(), 10, 10, D3DCOLOR_XRGB(61, 168, 71), false, "small");
		else
			String(std::string("F1 » Hidden » Players List").c_str(), 10, 10, D3DCOLOR_XRGB(171, 58, 75), false, "small");

		if (Vars.ESP.Animals.bActive)
			String(std::string("F2 » Shown » Animals").c_str(), 10, 25, D3DCOLOR_XRGB(61, 168, 71), false, "small");
		else
			String(std::string("F2 » Hidden » Animals").c_str(), 10, 25, D3DCOLOR_XRGB(171, 58, 75), false, "small");

		if (Vars.ESP.Player.bActive)
			String(std::string("F3 » Shown » Players").c_str(), 10, 40, D3DCOLOR_XRGB(61, 168, 71), false, "small");
		else
			String(std::string("F3 » Hidden » Players").c_str(), 10, 40, D3DCOLOR_XRGB(171, 58, 75), false, "small");

		if (Vars.ESP.Ships.bActive)
			String(std::string("F4 » Shown » Ships").c_str(), 10, 55, D3DCOLOR_XRGB(61, 168, 71), false, "small");
		else
			String(std::string("F4 » Hidden » Ships").c_str(), 10, 55, D3DCOLOR_XRGB(171, 58, 75), false, "small");

		if (Vars.ESP.Treasure.bActive)
			String(std::string("F5 » Shown » Treasures").c_str(), 10, 70, D3DCOLOR_XRGB(61, 168, 71), false, "small");
		else
			String(std::string("F5 » Hidden » Treasures").c_str(), 10, 70, D3DCOLOR_XRGB(171, 58, 75), false, "small");

		if (Vars.ESP.Objects.bActive)
			String(std::string("F6 » Shown » Objects").c_str(), 10, 85, D3DCOLOR_XRGB(61, 168, 71), false, "small");
		else
			String(std::string("F6 » Hidden » Objects").c_str(), 10, 85, D3DCOLOR_XRGB(171, 58, 75), false, "small");

		if (Vars.ESP.Legend.bActive) {
			String(std::string("F7 » Shown » Colors Legend").c_str(), 10, 100, D3DCOLOR_XRGB(61, 168, 71), false, "small");
			String(std::string("» Common").c_str(), 24, 113, D3DCOLOR_XRGB(Vars.ESP.colorCommon[0], Vars.ESP.colorCommon[1], Vars.ESP.colorCommon[2], Vars.ESP.colorCommon[3]), false, "small");
			String(std::string("» Rare").c_str(), 24, 125, D3DCOLOR_XRGB(Vars.ESP.colorRare[0], Vars.ESP.colorRare[1], Vars.ESP.colorRare[2], Vars.ESP.colorRare[3]), false, "small");
			String(std::string("» Mythical").c_str(), 24, 137, D3DCOLOR_XRGB(Vars.ESP.colorMythical[0], Vars.ESP.colorMythical[1], Vars.ESP.colorMythical[2], Vars.ESP.colorMythical[3]), false, "small");
			String(std::string("» Legendary").c_str(), 24, 149, D3DCOLOR_XRGB(Vars.ESP.colorLegendary[0], Vars.ESP.colorLegendary[1], Vars.ESP.colorLegendary[2], Vars.ESP.colorLegendary[3]), false, "small");
			String(std::string("» Special").c_str(), 24, 161, D3DCOLOR_XRGB(Vars.ESP.colorSpecial[0], Vars.ESP.colorSpecial[1], Vars.ESP.colorSpecial[2], Vars.ESP.colorSpecial[3]), false, "small");
			String(std::string("» Other").c_str(), 24, 173, D3DCOLOR_XRGB(Vars.ESP.colorOther[0], Vars.ESP.colorOther[1], Vars.ESP.colorOther[2], Vars.ESP.colorOther[3]), false, "small");
			String(std::string("» Ghost Ship").c_str(), 24, 185, D3DCOLOR_XRGB(Vars.ESP.colorGhostShip[0], Vars.ESP.colorGhostShip[1], Vars.ESP.colorGhostShip[2], Vars.ESP.colorGhostShip[3]), false, "small");
			String(std::string("» Objects").c_str(), 24, 197, D3DCOLOR_XRGB(Vars.ESP.Objects.colorObjects[0], Vars.ESP.Objects.colorObjects[1], Vars.ESP.Objects.colorObjects[2], Vars.ESP.Objects.colorObjects[3]), false, "small");
			String(std::string("» Map Pin").c_str(), 24, 209, D3DCOLOR_XRGB(Vars.ESP.colorPins[0], Vars.ESP.colorPins[1], Vars.ESP.colorPins[2], Vars.ESP.colorPins[3]), false, "small");
		}
		else {
			String(std::string("F7 » Hidden » Colors Legend").c_str(), 10, 100, D3DCOLOR_XRGB(171, 58, 75), false, "small");
		}
		*/

	Line(Process->Size[0]/2 - 5, Process->Size[1]/2, Process->Size[0]/2 + 5, Process->Size[1]/2, D3DCOLOR_XRGB(171, 58, 75));
	Line(Process->Size[0]/2, Process->Size[1]/2-5, Process->Size[0]/2, Process->Size[1]/2+5, D3DCOLOR_XRGB(171, 58, 75));

	if (Menu->menuOpen)
	DrawScoreBoard(Process->Size[0]-300, Process->Size[1] - 380, Cheat->Crews);

	Menu->DrawMenu();
}
