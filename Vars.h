#pragma once

struct sColor
{
	int r = 255;
	int g = 255;
	int b = 255;
	int a = 255;

	sColor operator=(int color[4])
	{
		r = color[0];
		g = color[1];
		b = color[2];
		a = color[3];
		return *this;
	}
};

class vars {
public:

	bool GUI = true;

	struct ESP
	{
		int colorCommon[4], colorRare[4], colorMythical[4], colorLegendary[4], colorSpecial[4], colorOther[4], colorPins[4], colorDKey[4], colorGhostShip[4], colorWorld[4];

		//////////

		struct MenuV2
		{
			bool MenuActive, F1, F2, F3, F4, F5, F6, F7, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9;
		}MenuV2;

		struct MenuTake
		{
			bool MapPins, ColorsLegend, LoreBooks, PlayerEnemy, PlayerAlly, PlayerHPBar, PlayerName, PlayerItem, Skeleton, SkeletonName, SkeletonItem, WaterMobs, AnimalsCommon, AnimalsRare, AnimalsMythical, AnimalsLegendary, AnimalsSnakes, AnimalsChickens, AnimalsPigs, ItemsCommon, ItemsRare, ItemsMythical, ItemsLegendary, ItemsUtility, ItemsBarrels, ItemsCages, ItemsOther, ItemsWorthless, ItemsHarpoonSupport, EventsFort, EventsCloud, EventsReapers, ShipsEnemy, ShipsAlly, ShipsRowboat, ShipsGhost, ObjectsSiren, ObjectsStatues, ObjectsWrecks, Debug;
			int EnemyColor[4], AllyColor[4], ObjectsColor[4], CaptainColor[4], LoreColor[4], Emerald[4], Ruby[4], Sapphire[4];
		}MenuTake;
	}ESP;

};

extern vars Vars;