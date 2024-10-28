#include <YSI_Coding\y_hooks>
new Text:GunTD[4], arenaPickup[2], gsItem[MAX_PLAYERS];


hook OnGameModeInit()
{
	GunTD[0] = TextDrawCreate(314.000092, 399.481598, "~r~buy");
	TextDrawLetterSize(GunTD[0], 0.246998, 1.139556);
	TextDrawTextSize(GunTD[0], 333.000000, 8.000000);
	TextDrawAlignment(GunTD[0], 1);
	TextDrawColour(GunTD[0], 0xFFFFFFFF);
	TextDrawUseBox(GunTD[0], 1);
	TextDrawBoxColour(GunTD[0], 0x000000FF);
	TextDrawSetShadow(GunTD[0], 0);
	TextDrawSetOutline(GunTD[0], 0);
	TextDrawBackgroundColour(GunTD[0], 0x000000FF);
	TextDrawFont(GunTD[0], 2);
	TextDrawSetProportional(GunTD[0], 1);
	TextDrawSetSelectable(GunTD[0], 1);

	GunTD[1] = TextDrawCreate(257.333405, 380.399933, "<");
	TextDrawLetterSize(GunTD[1], 0.400000, 1.600000);
	TextDrawTextSize(GunTD[1], 267.000000, 8.000000);
	TextDrawAlignment(GunTD[1], 1);
	TextDrawColour(GunTD[1], 0xFFFFFFFF);
	TextDrawUseBox(GunTD[1], 1);
	TextDrawBoxColour(GunTD[1], 0x000000FF);
	TextDrawSetShadow(GunTD[1], 0);
	TextDrawSetOutline(GunTD[1], 255);
	TextDrawBackgroundColour(GunTD[1], 0x000000FF);
	TextDrawFont(GunTD[1], 1);
	TextDrawSetProportional(GunTD[1], 1);
	TextDrawSetSelectable(GunTD[1], 1);

	GunTD[2] = TextDrawCreate(380.000122, 379.155457, ">");
	TextDrawLetterSize(GunTD[2], 0.400000, 1.600000);
	TextDrawTextSize(GunTD[2], 389.000000, 8.000000);
	TextDrawAlignment(GunTD[2], 1);
	TextDrawColour(GunTD[2], 0xFFFFFFFF);
	TextDrawUseBox(GunTD[2], 1);
	TextDrawBoxColour(GunTD[2], 0x000000FF);
	TextDrawSetShadow(GunTD[2], 0);
	TextDrawSetOutline(GunTD[2], 255);
	TextDrawBackgroundColour(GunTD[2], 0x000000FF);
	TextDrawFont(GunTD[2], 1);
	TextDrawSetProportional(GunTD[2], 1);
	TextDrawSetSelectable(GunTD[2], 1);

	GunTD[3] = TextDrawCreate(270.999969, 360.074310, "box");
	TextDrawLetterSize(GunTD[3], 0.000000, 5.600000);
	TextDrawTextSize(GunTD[3], 377.000000, 0.000000);
	TextDrawAlignment(GunTD[3], 1);
	TextDrawColour(GunTD[3], 0x00000032);
	TextDrawUseBox(GunTD[3], 1);
	TextDrawBoxColour(GunTD[3], 0x00000096);
	TextDrawSetShadow(GunTD[3], 0);
	TextDrawSetOutline(GunTD[3], 0);
	TextDrawBackgroundColour(GunTD[3], 0x00000032);
	TextDrawFont(GunTD[3], 1);
	TextDrawSetProportional(GunTD[3], 1);
	TextDrawSetSelectable(GunTD[3], 0);

	CreateDynamicObject(351, 292.527008, -81.734200, 1001.542297, 90.000000, 0.000000, 0.000000);
    CreateDynamicObject(347, 293.589203, -81.815002, 1001.502319, 90.000000, 0.000000, 0.000000);
    CreateDynamicObject(353, 294.940186, -81.753998, 1001.543396, -90.000000, 0.000000, 180.683701);
    CreateDynamicObject(348, 295.579895, -81.765404, 1001.544189, -90.000000, 0.000000, 180.318604);
    CreateDynamicObject(355, 296.349213, -81.578796, 1001.563416, -90.000000, -25.000000, 180.000000);
    CreateDynamicObject(349, 297.705505, -81.788399, 1001.583130, -90.000000, 0.000000, 180.000000);
    CreateDynamicObject(356, 298.672699, -81.648300, 1001.563477, -90.000000, -47.000000, 180.170593);

    CreateDynamicObject(1977, 317.196106, -132.419006, 1002.983582, 356.958405, 0.140000, -90.570801); //old gs

    CreateDynamicPickup(1241, 23, 304.6744, -65.3353, 1001.5156, -1, -1);
   	CreateDynamic3DTextLabel("Health\nType "COLOR_SV_HEX"/heal{ffffff} to get health.", -1, 304.6744, -65.3353, 1001.5156, 10.0);

    arenaPickup[0] = CreateDynamicPickup(1318, 23, 301.6851, -76.5393, 1001.5156, -1, -1);
    arenaPickup[1] = CreateDynamicPickup(1318, 23, 305.4821, -57.5924, 1001.5156, -1, -1);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerPickUpDynPickup(playerid, pickupid)
{
	if(pickupid == arenaPickup[0] || pickupid == arenaPickup[1])
	{
		if(GetPVarInt(playerid, "duel_deelay") > gettime())
			return va_SendClientMessage(playerid, -1, "You need to wait %d seconds to enter in duel arena.", GetPVarInt(playerid, "duel_deelay") - gettime());
		
		if(pickupid == arenaPickup[0])
			SetPlayerPos(playerid, 305.1833, -69.9575, 1001.5156);

		else if(pickupid == arenaPickup[1])
			SetPlayerPos(playerid, 301.6851, -76.5393, 1001.5156);

		SetPVarInt(playerid, "duel_deelay", gettime() + 10);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

GetWeapName(item)
{
	new returnName[16];
	switch(item)
	{
		case 1: returnName = "M4";
        case 2: returnName = "Shotgun";
        case 3: returnName = "AK47";
        case 4: returnName = "Desert Eagle";
        case 5: returnName = "MP5";
        case 6: returnName = "Silenced Colt";
        case 7: returnName = "Combat Shotgun";
	}
	return returnName;
}

GetWeapPrice(item)
{
	switch(item)
	{
		case 1: return 6500;
        case 2: return 5000;
        case 3: return 6000;
        case 4: return 6500;
        case 5: return 5000;
        case 6: return 5000;
        case 7: return 5000;
	}
	return true;
}
GetWeapAmmo(item)
{
	switch(item)
	{
		case 1: return 400;
        case 2: return 80;
        case 3: return 450;
        case 4: return 100;
        case 5: return 450;
        case 6: return 100;
        case 7: return 100;
	}
	return true;
}


new Float:playerPos[][3] =
{
    {298.523346, -81.679710, 1001.632568},
    {297.374146, -81.679710, 1001.695007},
    {296.108704, -81.679710, 1001.695007},
    {295.343109, -81.679710, 1001.695007},
    {294.683563, -81.679710, 1001.695007},
    {293.664520, -81.679710, 1001.730286},
    {292.664520, -81.679710, 1001.730286}
};

WhenPlayerChangeWeapon(playerid, weapon)
{

	SetPlayerCameraPos(playerid, playerPos[weapon - 1][0] + 0.1 * floatsin(-360, degrees), playerPos[weapon - 1][1] + 0.1 * floatcos(-360, degrees), playerPos[weapon - 1][2] + 3.0);
	SetPlayerCameraLookAt(playerid, playerPos[weapon - 1][0], playerPos[weapon - 1][1], playerPos[weapon - 1][2], (weapon == 1 ? CAMERA_CUT : CAMERA_MOVE));

	new returnGsString[64], returnGsStringg[16];
	format(returnGsString, sizeof returnGsString, "price: $%s~n~bullets: %d", FormatNumber(GetWeapPrice(weapon)), GetWeapAmmo(weapon));
	PlayerTextDrawSetString(playerid, playerTextdraws[playerid][GunPTD][1], returnGsString);

	format(returnGsStringg, sizeof returnGsStringg, "%s", GetWeapName(weapon));
	PlayerTextDrawSetString(playerid, playerTextdraws[playerid][GunPTD][0], returnGsStringg);

	for(new i = 0; i <= 3; i++)
		TextDrawShowForPlayer(playerid, GunTD[i]);

	for(new i = 0; i <= 1; i++)
		PlayerTextDrawShow(playerid, playerTextdraws[playerid][GunPTD][i]);
	return true;
}

WhenPlayerLeaveShop(playerid)
{
	if(!gsItem[playerid])
		return true;

	for(new i = 0; i <= 3; i++)
		TextDrawHideForPlayer(playerid, GunTD[i]);

	for(new i = 0; i <= 1; i++)
		PlayerTextDrawHide(playerid, playerTextdraws[playerid][GunPTD][i]);

	gsItem[playerid] = 0;
	SetCameraBehindPlayer(playerid);
	return true;
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if(clickedid == Text:INVALID_TEXT_DRAW && gsItem[playerid])
		return WhenPlayerLeaveShop(playerid);

	if(clickedid == GunTD[1])
	{
		if(gsItem[playerid] <= 1)
			return gsItem[playerid] = 7, WhenPlayerChangeWeapon(playerid, 7);

		gsItem[playerid] = gsItem[playerid] - 1;
		WhenPlayerChangeWeapon(playerid, gsItem[playerid]);
		return true;
	}
	if(clickedid == GunTD[2])
	{
		if(gsItem[playerid] >= 7)
			return gsItem[playerid] = 1, WhenPlayerChangeWeapon(playerid, 1);

		gsItem[playerid] = gsItem[playerid] + 1;
		WhenPlayerChangeWeapon(playerid, gsItem[playerid]);
		return true;
	}
	if(clickedid == GunTD[0])
	{
		if(!gsItem[playerid])
			return true;

		check_delay

		new weap;
		switch(gsItem[playerid])
		{
			case 1: weap = 31;
			case 2: weap = 25;
			case 3: weap = 30;
			case 4: weap = 24;
			case 5: weap = 29;
			case 6: weap = 23;
			case 7: weap = 27;
		}


	    if(GetMoney(playerid) < GetWeapPrice(gsItem[playerid]) && GetMilliards(playerid) < 1)
	        return sendError(playerid,"You don't have enough money to do this.");


		va_SendClientMessage(playerid, COLOR_GREY, "Ai cumparat o arma %s cu 100 gloante pentru $%s.", GetWeaponNameEx(weap), FormatNumber(GetWeapPrice(gsItem[playerid])));
		GivePlayerWeapon(playerid, weap, GetWeapAmmo(gsItem[playerid]));

		substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, GetWeapPrice(gsItem[playerid]));
		
		bizInfo[playerStaticVar[playerid][inBusiness]][bizMoney] += GetWeapPrice(gsItem[playerid]);

		mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `bizMoney` = '%d' where `id` = '%d';", bizInfo[playerStaticVar[playerid][inBusiness]][bizMoney], playerStaticVar[playerid][inBusiness]);
		mysql_tquery(SQL, returnString, "", "");

		set_delay;

		return true;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(newkeys == KEY_YES)
	{
		if(IsPlayerInRangeOfPoint(playerid, 2.5, 293.4223, -80.8117, 1001.5156))
		{
			if(!playerInfo[playerid][pGunLic])
				return sendError(playerid,"You don't have gun licence");

			SendClientMessage(playerid, COLOR_GREY, "You can exit this menu by pressing ESC on your keyboard.");

			SelectTextDraw(playerid, 0x4c92d9FF);

			gsItem[playerid] = 1;
			WhenPlayerChangeWeapon(playerid, 1);
			return true;
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}