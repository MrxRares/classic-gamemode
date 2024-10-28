#include <mSelection>
#include <confirm-box>
#include <YSI_Coding\y_hooks>
#define MAX_HOUSES 100
#define STREAMER_BEGIN_HOUSE 0

#define MAX_FURNITURE_CATEGORY 4
#define MAX_HOUSE_OBJECTS 1000

enum ENUM_HOUSE_INFROMATIONS
{
	houseID, houseOwner, houseType, houseInteriorVar, Float:houseExterior[3], Float:houseInterior[3], houseSphere, Text3D:houseLabel,
	housePickup, houseIcon, houseLocked, houseRentable, houseSafe, houseVIP, houseGarage, houseObjects,
	houseOwnerName[MAX_PLAYER_NAME], houseDescription[24], houseRent, hHealth, hArmour
};
new houseInfo[MAX_HOUSES][ENUM_HOUSE_INFROMATIONS], Iterator: houseRenters<MAX_HOUSES, MAX_PLAYERS>;

enum ENUM_FURNITURE_INFORMATIONS
{
	mappingModel[MAX_HOUSES], mappingType[MAX_HOUSES], mappingGameModel[MAX_HOUSES], mappingID[MAX_HOUSES]
};
new mappingInfo[MAX_HOUSE_OBJECTS][ENUM_FURNITURE_INFORMATIONS];
new Float:mappingPos[MAX_HOUSE_OBJECTS][6][MAX_HOUSES], returnTotalObjects;

new returnServerHouses;
function:loadHousesFromInclude()
{
	returnServerHouses = cache_num_rows();
	for(new i = 1; i <= cache_num_rows(); i++)
	{
		cache_get_value_name_int(i - 1, "ID", houseInfo[i][houseID]);
		cache_get_value_name_int(i - 1, "Type", houseInfo[i][houseType] );
		cache_get_value_name_int(i - 1, "OwnerSQL", houseInfo[i][houseOwner]);

		cache_get_value_name_int(i - 1, "Rentable", houseInfo[i][houseRentable]);
		cache_get_value_name_int(i - 1, "Rent", houseInfo[i][houseRent]);
		cache_get_value_name_int(i - 1, "Safe", houseInfo[i][houseSafe]);

	 	cache_get_value_name_int(i - 1, "Health", houseInfo[i][hHealth]);
	 	cache_get_value_name_int(i - 1, "Armour", houseInfo[i][hArmour]);

		cache_get_value_name_int(i - 1, "Garage", houseInfo[i][houseGarage]);
		cache_get_value_name_int(i - 1, "VIP", houseInfo[i][houseVIP]);

		cache_get_value_name_int(i - 1, "Interior", houseInfo[i][houseInteriorVar]);
		cache_get_value_name_int(i - 1, "Locked", houseInfo[i][houseLocked]);

		cache_get_value_name_float(i - 1, "ExteriorX", houseInfo[i][houseExterior][0]);
		cache_get_value_name_float(i - 1, "ExteriorY", houseInfo[i][houseExterior][1]);
		cache_get_value_name_float(i - 1, "ExteriorZ", houseInfo[i][houseExterior][2]);

		cache_get_value_name_float(i - 1, "InteriorX", houseInfo[i][houseInterior][0]);
		cache_get_value_name_float(i - 1, "InteriorY", houseInfo[i][houseInterior][1]);
		cache_get_value_name_float(i - 1, "InteriorZ", houseInfo[i][houseInterior][2]);

		cache_get_value_name(i - 1, "Description", houseInfo[i][houseDescription]);
	}

	for(new house = 1; house <= returnServerHouses; house ++)
	{
		new stringCache[128];
		format(stringCache, sizeof stringCache, "select `Name` from `users` where `House` = '%d' and `ID` = '%d';", house, houseInfo[house][houseOwner]);
		
		new Cache: cache_player_name = mysql_query(SQL, stringCache);
		if(cache_num_rows())
			cache_get_value_name(0, "Name", houseInfo[house][houseOwnerName]);

		else
			format(houseInfo[house][houseOwnerName], MAX_PLAYER_NAME, "AdmBot");

		cache_delete(cache_player_name);

		WhenHouseGotUpdated(house);
	
		mysql_format(SQL, returnString, sizeof returnString, "select * from `housemapping` where `House` = '%d';", house);
		mysql_tquery(SQL, returnString, "createHouseFurniture", "i", house);
	}
	printf("Houses %d", returnServerHouses);
	return true;
}

hook loadServerSystems()
{
	mysql_tquery(SQL, "select * from `garages`", "loadGaragesFromInclude", "");
	mysql_tquery(SQL, "select * from `houses`", "loadHousesFromInclude", "");
	mysql_tquery(SQL, "select * from `housemapping`", "loadHouseMappingFromInclude", "", "");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

forward public WhenHouseGotUpdated(house);
public WhenHouseGotUpdated(house)
{
	DestroyDynamicPickup(houseInfo[house][housePickup]);
	DestroyDynamic3DTextLabel(houseInfo[house][houseLabel]);

	if(!houseInfo[house][houseOwner])
	{
		houseInfo[house][housePickup] = CreateDynamicPickup(1273, 23, houseInfo[house][houseExterior][0], houseInfo[house][houseExterior][1], houseInfo[house][houseExterior][2], 0, 0, -1, 60);
		format(returnString, sizeof returnString, "House "COLOR_SV_HEX"%d{ffffff}\n"COLOR_SV_HEX"This house will be auctioned !{FFFFFF}\nOwner: "COLOR_SV_HEX"%s{ffffff}\nSize: "COLOR_SV_HEX"%s{ffffff}\nDescription: "COLOR_SV_HEX"%s{ffffff}\nPrice: "COLOR_SV_HEX"$-1{ffffff}\nLevel: "COLOR_SV_HEX"7{ffffff}\n%s", house, houseInfo[house][houseOwnerName], houseSize(houseInfo[house][houseType]), houseInfo[house][houseDescription], houseInfo[house][houseVIP] ? "{FFFF00}[VIP]" : "");
	}
	else
	{
		houseInfo[house][housePickup] = CreateDynamicPickup(1272, 23, houseInfo[house][houseExterior][0], houseInfo[house][houseExterior][1], houseInfo[house][houseExterior][2], 0, 0, -1, 60);
		format(returnString, sizeof returnString, "House "COLOR_SV_HEX"%d{ffffff}\n"COLOR_SV_HEX"%s{FFFFFF}\nOwner: "COLOR_SV_HEX"%s{ffffff}\nSize: "COLOR_SV_HEX"%s{ffffff}\nTenants: "COLOR_SV_HEX"%d{ffffff}\nRent: "COLOR_SV_HEX"$%s{FFFFFF}\nLevel: "COLOR_SV_HEX"7{ffffff}\n%s\n%s", house, houseInfo[house][houseDescription], houseInfo[house][houseOwnerName], houseSize(houseInfo[house][houseType]), Iter_Count(houseRenters<house>), FormatNumber(houseInfo[house][houseRent]), (houseInfo[house][houseRentable] && !houseInfo[house][houseLocked] ? ""COLOR_SV_HEX"To rent a room type /rentroom" : ""), houseInfo[house][houseVIP] ? "{FFFF00}[VIP]" : "");
	}
	houseInfo[house][houseLabel] = CreateDynamic3DTextLabel(returnString, -1, houseInfo[house][houseExterior][0], houseInfo[house][houseExterior][1] + 0.1, houseInfo[house][houseExterior][2], 25);
	
	DestroyDynamicArea(houseInfo[house][houseSphere]);
	houseInfo[house][houseSphere] = CreateDynamicSphere(houseInfo[house][houseExterior][0], houseInfo[house][houseExterior][1], houseInfo[house][houseExterior][2], 1.5, 0, 0, -1);
	Streamer_SetIntData(STREAMER_TYPE_AREA, houseInfo[house][houseSphere], E_STREAMER_EXTRA_ID, (house + STREAMER_BEGIN_HOUSE));

	DestroyDynamicMapIcon(houseInfo[house][houseIcon]);
	houseInfo[house][houseIcon] = CreateDynamicMapIcon(houseInfo[house][houseExterior][0], houseInfo[house][houseExterior][1], houseInfo[house][houseExterior][2], 31, 0, -1, -1, -1, 500.0, MAPICON_LOCAL);
	
	if(houseInfo[house][houseGarage])
		WhenGarageGotUpdated(houseInfo[house][houseGarage]);
	return true;
}

hook OnPlayerEnterDynArea(playerid, areaid)
{
	new houseId = Streamer_GetIntData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID) - STREAMER_BEGIN_HOUSE;
	if(houseId < 1 || returnServerHouses < houseId)
		return Y_HOOKS_CONTINUE_RETURN_1;

	SetPVarInt(playerid, "areaID", houseId);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerLeaveDynArea(playerid, areaid)
{
    DeletePVar(playerid, "areaID");
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	if(!playerInfo[playerid][pHouse])
		return Y_HOOKS_CONTINUE_RETURN_1;

	new houseId = playerInfo[playerid][pHouse];
	if(Iter_Contains(houseRenters<houseId>, playerid))
	{
		Iter_Remove(houseRenters<houseId>, playerid);
		WhenHouseGotUpdated(houseId);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook WhenPlayerLoggedIn(playerid)
{
	if(!cache_num_rows())
		return Y_HOOKS_CONTINUE_RETURN_1;

	cache_get_value_name_int(0, "ID", playerInfo[playerid][pSQLID]);
	cache_get_value_name_int(0, "House", playerInfo[playerid][pHouse]);

	if(playerInfo[playerid][pSQLID] != houseInfo[playerInfo[playerid][pHouse]][houseOwner] && playerInfo[playerid][pHouse])
	{
		Iter_Add(houseRenters<playerInfo[playerid][pHouse]>, playerid);
		WhenHouseGotUpdated(playerInfo[playerid][pHouse]);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
	new samiBAGPULA = GetPVarInt(playerid, "areaID");
	if(newkeys == KEY_SECONDARY_ATTACK)
	{

		if(GetPVarInt(playerid, "areaID") > 0 && IsPlayerInDynamicArea(playerid, houseInfo[samiBAGPULA][houseSphere], 0) && !isPlayerInWar(playerid))
		{
			if(GetPVarInt(playerid, "enter_building_deelay") > gettime())
				return va_SendClientMessage(playerid, COLOR_GREY, "[Anti abuz]: Nu poti intra intr-o cladire timp de %d secunde.", GetPVarInt(playerid, "enter_building_deelay") - gettime());
			

			if(houseInfo[samiBAGPULA][houseLocked] == 1 && playerInfo[playerid][pHouse] != samiBAGPULA)
				return GameTextForPlayer(playerid, "~r~~h~Locked", 5000, 1);

			SetPlayerPos(playerid, houseInfo[samiBAGPULA][houseInterior][0], houseInfo[samiBAGPULA][houseInterior][1], houseInfo[samiBAGPULA][houseInterior][2]);
			SetPlayerInterior(playerid, houseInfo[samiBAGPULA][houseInteriorVar]);
			SetPlayerVirtualWorld(playerid, samiBAGPULA);

			return playerStaticVar[playerid][inHouse] = samiBAGPULA;
		}
		if(playerStaticVar[playerid][inHouse] > 0 && GetPlayerVirtualWorld(playerid) > 0 && GetPlayerVirtualWorld(playerid) < returnServerHouses && IsPlayerInRangeOfPoint(playerid, 1.5, houseInfo[GetPlayerVirtualWorld(playerid)][houseInterior][0], houseInfo[GetPlayerVirtualWorld(playerid)][houseInterior][1], houseInfo[GetPlayerVirtualWorld(playerid)][houseInterior][2]))
		{
			new houseAreaId = GetPlayerVirtualWorld(playerid);
			SetPlayerPos(playerid, houseInfo[houseAreaId][houseExterior][0], houseInfo[houseAreaId][houseExterior][1], houseInfo[houseAreaId][houseExterior][2]);

			SetPlayerVirtualWorld(playerid, 0);
			SetPlayerInterior(playerid, 0);
			
			StopAudioStreamForPlayer(playerid);
			playerStaticVar[playerid][inHouse] = -1;

			return SetPVarInt(playerid, "enter_building_deelay", gettime() + 15);
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook WhenPlayerSpawn(playerid)
{
	if(playerInfo[playerid][pSpawnLocation] != 1)
		return Y_HOOKS_CONTINUE_RETURN_1;

	if(!playerInfo[playerid][pHouse])
	{
		playerInfo[playerid][pSpawnLocation] = 0;
		return Y_HOOKS_CONTINUE_RETURN_1;
	}

	SetPlayerPos(playerid, houseInfo[playerInfo[playerid][pHouse]][houseInterior][0], houseInfo[playerInfo[playerid][pHouse]][houseInterior][1], houseInfo[playerInfo[playerid][pHouse]][houseInterior][2]);
	SetPlayerInterior(playerid, houseInfo[playerInfo[playerid][pHouse]][houseInteriorVar]);

	SetPlayerVirtualWorld(playerid, playerInfo[playerid][pHouse]);
	playerStaticVar[playerid][inHouse] = playerInfo[playerid][pHouse];

	SetPlayerFacingAngle(playerid, 270);
	return Y_HOOKS_CONTINUE_RETURN_0;
}

returnCategory(category)
{
	new stringCategory[32];
	switch(category)
	{
		case 0: stringCategory = "TV, Audio, Video";
		case 1: stringCategory = "Fitting";
		case 2: stringCategory = "Paintings";
		case 3: stringCategory = "Doors";
	}
	return stringCategory;
}

houseSize(house)
{
	new returnHouseSize[10];
	switch(house)
	{
		case 0: returnHouseSize = "Big";
		case 1: returnHouseSize = "Medium";
		case 2: returnHouseSize = "Small";
		default: returnHouseSize = "Unknown";
	}
	return returnHouseSize;
}

YCMD:rentroom(playerid, params[], help)
{
	if(playerInfo[playerid][pHouse])
		return SendClientMessage(playerid, COLOR_GREY, "You already rented a room somewhere. Type /unrentroom before rent a room.");

	new houseAreaId = GetPVarInt(playerid, "areaID");		
	if(houseAreaId < 1 || houseAreaId > returnServerHouses)
		return true;

    if(GetMoney(playerid) <  houseInfo[houseAreaId][houseRent] && GetMilliards(playerid) < 1)
        return sendError(playerid,"You do not have the money.");


	if(houseInfo[houseAreaId][houseLocked] == 1 || houseInfo[houseAreaId][houseRentable] != 1)
		return sendError(playerid,"You cannot rent this house because it's locked by the owner.");

	substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, houseInfo[houseAreaId][houseRent]);

	houseInfo[houseAreaId][houseSafe] += houseInfo[houseAreaId][houseRent];

	mysql_format(SQL, returnString, sizeof returnString, "update `houses` set `Safe` = '%d' where `ID` = '%d';", houseInfo[houseAreaId][houseSafe], houseAreaId);
	mysql_tquery(SQL, returnString, "", "");

    SendClientMessage(playerid, COLOR_WHITE, "Congratulations, You can enter and exit here anytime.");
    SendClientMessage(playerid, COLOR_WHITE, "Type /help to review the new property help section.");

    GameTextForPlayer(playerid, "~w~Welcome Home~n~You can exit at any time by~n~moving to this door and press~n~enter", 5000, 3);

	playerInfo[playerid][pHouse] = houseAreaId;
	Iter_Add(houseRenters<houseAreaId>, playerid);

	houseAreaId = playerInfo[playerid][pHouse];
	playerStaticVar[playerid][inHouse] = houseAreaId;

	SetPlayerPos(playerid, houseInfo[houseAreaId][houseInterior][0], houseInfo[houseAreaId][houseInterior][1], houseInfo[houseAreaId][houseInterior][2]);
	SetPlayerInterior(playerid, houseInfo[houseAreaId][houseInteriorVar]);
	SetPlayerVirtualWorld(playerid, houseAreaId);
	
    playerInfo[playerid][pSpawnLocation] = 1;

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `House` = '%d', `SpawnLocation` = '1' where `id` = '%d';", playerInfo[playerid][pHouse], playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");

	return WhenHouseGotUpdated(playerInfo[playerid][pHouse]);
}

YCMD:unrentroom(playerid, params[], help)
{
	if(playerStaticVar[playerid][playerSleeping] == 1)
		return SendClientMessage(playerid, COLOR_LIGHTRED, "You sleeping, use /sleep first.");

	if(!playerInfo[playerid][pHouse])
		return sendError(playerid,"{B8DBFF}You don't have rent a house.");

	new
		houseId = playerInfo[playerid][pHouse];		

	if(houseInfo[houseId][houseOwner] == playerInfo[playerid][pSQLID])
		return sendError(playerid,"{B8DBFF}You own this house.");

	SendClientMessage(playerid, -1, "{B8FFDB}You are now homeless.");

	Iter_Remove(houseRenters<houseId>, playerid);
	WhenHouseGotUpdated(houseId);

	playerInfo[playerid][pHouse] = 0;
	playerInfo[playerid][pSpawnLocation] = 0;
	
	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `House` = '0', `SpawnLocation` = '0' where `id` = '%d';", playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

YCMD:gotohouse(playerid, params[], help)
{
    check_admin

    new returnHouse;
    if(sscanf(params, "d", returnHouse))
        return sendSyntaxMessage(playerid, "/gotohouse <House ID>");

    if(returnHouse < 1 || returnHouse > returnServerHouses)
        return sendError(playerid,"Invalid house id.");

    SetPlayerPos(playerid, houseInfo[returnHouse][houseExterior][0], houseInfo[returnHouse][houseExterior][1], houseInfo[returnHouse][houseExterior][2]);
    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s used /gotohouse %d.", GetName(playerid), returnHouse);
    return 1;
}

YCMD:findhouse(playerid, params[], help)
{
	if(playerStaticVar[playerid][pCheckpoint])
		return showActiveCheckpointDialog(playerid);

	new houseId;
	if(sscanf(params, "i", houseId))
		return sendSyntaxMessage(playerid, "/findhouse <House ID>");

	if(houseId < 1 || houseId > returnServerHouses)
		return SendClientMessage(playerid, COLOR_GREY, "Invalid House ID.");

	SendClientMessage(playerid, -1, "I have placed a checkpoint at that location. Please follow it.");
	playerStaticVar[playerid][pCheckpoint] = 1;

	return SetPlayerCheckpoint(playerid, houseInfo[houseId][houseExterior][0], houseInfo[houseId][houseExterior][1], houseInfo[houseId][houseExterior][2], 2.0);
}

YCMD:hu(playerid, params[], help)
{
	if(!playerInfo[playerid][pHouse] || playerInfo[playerid][pSQLID] != houseInfo[playerInfo[playerid][pHouse]][houseOwner])
		return sendError(playerid,"{FFB870}You don't own a house.");

	new returnOption;
	if(sscanf(params, "i", returnOption))
	{
		sendSyntaxMessage(playerid, "/hu <Item>");

		if(!houseInfo[playerInfo[playerid][pHouse]][hHealth])
			SendClientMessage(playerid, -1, "{FFF8C6}(1) Health Upgrade $50,000.");

		if(!houseInfo[playerInfo[playerid][pHouse]][hArmour])
			SendClientMessage(playerid, -1, "{FFF8C6}(2) Armour Upgrade $100,000.");

		return true;
	}

	if(returnOption == 1 && !houseInfo[playerInfo[playerid][pHouse]][hHealth])
	{

	    if(GetMoney(playerid) <  50000 && GetMilliards(playerid) < 1)
	        return sendError(playerid,"You do not have the money.");

		SendClientMessage(playerid, -1, "You can now heal yourself at home, type /heal.");
		houseInfo[playerInfo[playerid][pHouse]][hHealth] = 1;
	}
	if(returnOption == 2 && !houseInfo[playerInfo[playerid][pHouse]][hArmour])
	{
		if(!IsACop(playerid))
			return sendError(playerid,"{FFB870}You can't buy this, you are not a Cop,");

	    if(GetMoney(playerid) <  100000 && GetMilliards(playerid) < 1)
	        return sendError(playerid,"You do not have the money.");


		SendClientMessage(playerid, -1, "You can now get armour at home, Type /heal.");
		houseInfo[playerInfo[playerid][pHouse]][hArmour] = 1;
	}

	mysql_format(SQL, returnString, sizeof returnString, "update `houses` set `Armour` = '%d', `Health` = '%d' where `id` = '%d';", houseInfo[playerInfo[playerid][pHouse]][hHealth], houseInfo[playerInfo[playerid][pHouse]][hArmour], playerInfo[playerid][pHouse]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

YCMD:home(playerid, params[], help)
{
	if(!playerInfo[playerid][pHouse])
		return sendError(playerid,"You don't have house or you don't have rentroom.");

	if(playerStaticVar[playerid][pCheckpoint])
		return sendError(playerid,"You can't use this command while you have an active checkpoint.");

	new
		houseId = playerInfo[playerid][pHouse];
	
	SendClientMessage(playerid, COLOR_WHITE, "A checkpoint has been set to your house.");

	SetPlayerCheckpoint(playerid, houseInfo[houseId][houseExterior][0], houseInfo[houseId][houseExterior][1], houseInfo[houseId][houseExterior][2], 2.0);
	playerStaticVar[playerid][pCheckpoint] = 1;
	
	return true;
}

YCMD:sleep(playerid, params[], help)
{
	if(playerStaticVar[playerid][inHouse] < 1)
		return true;

	if(playerStaticVar[playerid][groupDuty])
		return SendClientMessage(playerid, COLOR_GREY, "Nu poti folosi aceasta comanda cat timp esti la datorie.");

	if(playerInfo[playerid][pHouse] != playerStaticVar[playerid][inHouse])
		return SendClientMessage(playerid, COLOR_GREY, "You are not in your own or rented house.");

	if(playerInfo[playerid][pWantedLevel])
		return sendError(playerid,"{FFB870}You can not sleep, because you have wanted.");

	if(playerStaticVar[playerid][playerSleeping] == 0)
	{
		playerStaticVar[playerid][playerSleeping] = 1;
		TogglePlayerControllable(playerid, false);

		SendClientMessage(playerid, -1, "Acum poti ramane AFK fara a primi kick. Foloseste /sleep din nou cand vrei sa revi in joc.");
		SendClientMessage(playerid, -1, "Minimizarea jocului (alt-tab) poate provoca unele buguri. Este recomandat sa apesi ESC inainte de a minimiza jocul.");
        
        LoopingAnim(playerid, "CRACK", "crckdeth2", 4.0, 1, 0, 0, 0, 0);
		SetPlayerColor(playerid, COLOR_GREY);
	}
	else
	{
		playerStaticVar[playerid][playerSleeping] = 0;
		TogglePlayerControllable(playerid, true);

		ClearAnimations(playerid);
        SendClientMessage(playerid, -1, "Bine ai revenit!");

		SetPlayerColor(playerid, COLOR_WHITE_T);
	}

	return true;
}

function:showHouseFurniture(playerid)
{
    new returnObjectModel, returnObjectType;
    format(returnString, sizeof returnString, "Item\tCategory\nBuy a new item (%d/25)\nEdit items position\nDelete item\nDelete all furniture items\n", cache_num_rows());

    if(!cache_num_rows())
        Dialog_Show(playerid, DIALOG_FURNITURE, DIALOG_STYLE_TABLIST_HEADERS, "House furniture:", returnString, "Select", "Exit");

    houseInfo[playerInfo[playerid][pHouse]][houseObjects] = cache_num_rows();
    for(new i; i < cache_num_rows(); i++)
    {
        cache_get_value_name_int(i, "Model", returnObjectModel);
        cache_get_value_name_int(i, "Type", returnObjectType);

        format(returnString, sizeof returnString, "%s\n%d\t%s\n", returnString, returnObjectModel, returnCategory(returnObjectType));
    }
    Dialog_Show(playerid, DIALOG_FURNITURE, DIALOG_STYLE_TABLIST_HEADERS, "House furniture:", returnString, "Select", "Exit");
    return true;
}

YCMD:housefurniture(playerid, params[], help)
{
	if(!playerInfo[playerid][pHouse] || playerInfo[playerid][pSQLID] != houseInfo[playerInfo[playerid][pHouse]][houseOwner])
		return sendError(playerid,"You don't own a house.");

	if(!houseInfo[playerInfo[playerid][pHouse]][houseVIP])
		return sendError(playerid,"Casa ta nu detine acest upgrade.");
	
	if(!playerStaticVar[playerid][inHouse] || playerStaticVar[playerid][inHouse] != playerInfo[playerid][pHouse])
		return sendError(playerid,"You need to be in your house in order to acces this category.");

	mysql_format(SQL, returnString, sizeof returnString, "select * from `housemapping` where `House` = '%d';", playerInfo[playerid][pHouse]);
	mysql_tquery(SQL, returnString, "showHouseFurniture", "i", playerid);
    return true;
}

YCMD:housename(playerid, params[], help)
{
	if(!playerInfo[playerid][pHouse] || playerInfo[playerid][pSQLID] != houseInfo[playerInfo[playerid][pHouse]][houseOwner])
		return sendError(playerid,"You don't own a house.");

	new inputtext[20];
	if(sscanf(params, "s[20]", inputtext))
		return sendSyntaxMessage(playerid, "/housename <Message>");

	if(strlen(inputtext) > 20)
		return sendError(playerid,"Maximum caracter is 20.");

	va_SendClientMessage(playerid, -1, "House name set to: {B8DBFF}%s", inputtext);

	new houseId = playerInfo[playerid][pHouse];
	format(houseInfo[houseId][houseDescription], 24, inputtext);

	mysql_format(SQL, returnString, sizeof returnString, "update `houses` set `Description` = '%e' where `ID` = '%d';", inputtext, houseId);
	mysql_tquery(SQL, returnString, "", "");
	
	WhenHouseGotUpdated(houseId);
	return true;
}

YCMD:renters(playerid, params[], help)
{
	if(!playerInfo[playerid][pHouse])
		return sendError(playerid,"You don't have house or you don't have rentroom.");

	new houseId = playerInfo[playerid][pHouse];

	if(!Iter_Count(houseRenters<houseId>))
		return sendError(playerid,"* No renters online.");

	foreach(new i : houseRenters<houseId>)
		va_SendClientMessage(playerid, -1, "{FFCC99}* Player %s(%d).", GetDBName(i), i);

	va_SendClientMessage(playerid, -1, "* There are %d renters online.", Iter_Count(houseRenters<houseId>));
	return true;
}

YCMD:setrentable(playerid, params[], help)
{
	if(!playerInfo[playerid][pHouse] || playerInfo[playerid][pSQLID] != houseInfo[playerInfo[playerid][pHouse]][houseOwner])
		return sendError(playerid,"You don't own a house.");

	new returnRent, houseId = playerInfo[playerid][pHouse];
	if(sscanf(params, "d", returnRent))
		return sendSyntaxMessage(playerid, "/setrentable <0(No) - 1(Yes)>");

	if(returnRent < 0 || returnRent > 1)
		return sendSyntaxMessage(playerid, "/setrentable <0(No) - 1(Yes)>");

	houseInfo[houseId][houseRentable] = returnRent;
	va_SendClientMessage(playerid, -1, "House rentable set to %d.", returnRent);

	mysql_format(SQL, returnString, sizeof returnString, "update `houses` set `Rentable` = '%d' where `ID` = '%d';", houseInfo[houseId][houseRentable], houseId);
	mysql_tquery(SQL, returnString, "", "");

	WhenHouseGotUpdated(houseId);
	return true;
}

YCMD:setrent(playerid, params[], help)
{
	if(!playerInfo[playerid][pHouse] || playerInfo[playerid][pSQLID] != houseInfo[playerInfo[playerid][pHouse]][houseOwner])
		return sendError(playerid,"You don't own a house.");

	new returnAmount, houseId = playerInfo[playerid][pHouse];
	if(sscanf(params, "d", returnAmount))
		return sendSyntaxMessage(playerid, "/setrent <Money>");

	if(returnAmount < 1 || returnAmount > 5000)
		return sendError(playerid,"{FFF8C6}Minimum rent is $1, maximum rent is $5,000.");

	houseInfo[houseId][houseRent] = returnAmount;

	mysql_format(SQL, returnString, sizeof returnString, "update `houses` set `Rent` = '%d' where `ID` = '%d';", houseInfo[houseId][houseRent], houseId);
	mysql_tquery(SQL, returnString, "", "");

	va_SendClientMessage(playerid, -1, "House rent set to $%s.", FormatNumber(returnAmount));
	WhenHouseGotUpdated(houseId);
	return true;
}

YCMD:evict(playerid, params[], help)
{
	if(!playerInfo[playerid][pHouse] || playerInfo[playerid][pSQLID] != houseInfo[playerInfo[playerid][pHouse]][houseOwner])
		return sendError(playerid,"You don't own a house.");

	new returnPlayer;
	if(sscanf(params, "u", returnPlayer))
		return sendSyntaxMessage(playerid, "/evict <Name/Playerid>");

	if(!IsPlayerConnected(returnPlayer))
		return sendError(playerid,"Player not connected.");

	if(returnPlayer == playerid)
		return sendError(playerid,"You cant evict yourself.");

	if(playerInfo[playerid][pHouse] != playerInfo[returnPlayer][pHouse])
		return sendError(playerid,"Player doesn't rent at your House.");

	va_SendClientMessage(returnPlayer, -1, "You have been evicted.");
	va_SendClientMessage(playerid, -1, "Player has been evicted.");

	insertPlayerMail(returnPlayer, gettime(), "Ai fost dat afara din casa unde aveai chirie de catre %s.", GetDBName(playerid));

	Iter_Remove(houseRenters<playerInfo[returnPlayer][pHouse]>, playerid);

	playerInfo[returnPlayer][pHouse] = 0;
	WhenHouseGotUpdated(playerInfo[returnPlayer][pHouse]);

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `House` = '0' where `ID` = '%d';", playerInfo[returnPlayer][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

YCMD:evictall(playerid, params[], help)
{
	if(!playerInfo[playerid][pHouse] || playerInfo[playerid][pSQLID] != houseInfo[playerInfo[playerid][pHouse]][houseOwner])
		return sendError(playerid,"You don't own a house.");

	new houseId = playerInfo[playerid][pHouse];
	foreach(new i : houseRenters<houseId>)
	{
		SendClientMessage(i, -1, "You have been evicted from your house.");
		insertPlayerMail(i, gettime(), "Ai fost dat afara din casa unde aveai chirie de catre %s.", GetDBName(playerid));

		playerInfo[i][pHouse] = 0;
	}

	Iter_Clear(houseRenters<houseId>);
	WhenHouseGotUpdated(houseId);

	SendClientMessage(playerid, -1, "Everyone have been evicted.");

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `House` = '0' where `House` = '%d' and `ID` != '%d';", houseId, playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

YCMD:housewithdraw(playerid, params[], help)
{
	if(!playerInfo[playerid][pHouse] || playerInfo[playerid][pSQLID] != houseInfo[playerInfo[playerid][pHouse]][houseOwner])
		return sendError(playerid,"You don't own a house.");

	new returnAmount, houseId = playerInfo[playerid][pHouse];
	if(sscanf(params, "d", returnAmount))
		return sendSyntaxMessage(playerid, "/housewithdraw <Amount of Money>"), va_SendClientMessage(playerid, -1, "{B8FFDB}You Have $%s in your cashbox.", FormatNumber(houseInfo[houseId][houseSafe]));

	if(returnAmount > 10000000)
		return sendError(playerid,"Maxim amount of money is $10,000,000.");

	if(returnAmount > houseInfo[houseId][houseSafe] || !returnAmount)
		return sendError(playerid,"You dont have that much.");

	if(playerStaticVar[playerid][inHouse] != houseId || !IsPlayerInRangeOfPoint(playerid, 5.0, houseInfo[houseId][houseExterior][0], houseInfo[houseId][houseExterior][1], houseInfo[houseId][houseExterior][2]))
		return sendError(playerid,"You are to far from your house.");

	addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, returnAmount, false);

	houseInfo[houseId][houseSafe] -= returnAmount;

	va_SendClientMessage(playerid, COLOR_YELLOW, "{B8FFDB}You Have Withdrawn $%s from your cashbox Total: $%s", FormatNumber(returnAmount), FormatNumber(houseInfo[houseId][houseSafe]));
	
	mysql_format(SQL, returnString, sizeof returnString, "update `houses` set `Safe` = '%d' where `ID` = '%d';", houseInfo[houseId][houseSafe], houseId);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

YCMD:buyhouse(playerid, params[], help)
	return sendError(playerid,"This house will be auctioned.");

YCMD:sellhouseto(playerid, params[], help)
	return sendError(playerid,"Comanda prea veche pentru era noastra, folositi /trade ca e mai eficient.");


YCMD:sellhousetostate(playerid, params[], help)
{
	if(!playerInfo[playerid][pHouse] || playerInfo[playerid][pSQLID] != houseInfo[playerInfo[playerid][pHouse]][houseOwner])
		return sendError(playerid,"You don't own a house.");

	new houseId = playerInfo[playerid][pHouse];

	PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
	GameTextForPlayer(playerid, "~w~Congratulations~n~ You have sold your property for ~n~~g~$1000000", 10000, 3);

	addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 1000000, false);

	playerInfo[playerid][pHouse] = 0;

	mysql_format(SQL, returnString, sizeof returnString, "update `houses` set `OwnerSQL` = '0', `Locked` = '0' where `id` = '%d';", houseId);
	mysql_tquery(SQL, returnString, "", "");

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `House` = '0' where `id` = '%d';", playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");

	format(houseInfo[houseId][houseOwnerName], MAX_PLAYER_NAME, "AdmBot");
	houseInfo[houseId][houseLocked] = 0;
	houseInfo[houseId][houseOwner] = 0;

	WhenHouseGotUpdated(houseId);
	return true;
}

#include "../module_inc/src/garageSystem.inc"

function:createHouseFurniture(house)
{
	if(!cache_num_rows())
		return true;

	for(new i; i < cache_num_rows(); i++)
	{
		cache_get_value_name_int(i, "ID", mappingInfo[i][mappingID][house]);
		cache_get_value_name_int(i, "Model", mappingInfo[i][mappingModel][house]);
		cache_get_value_name_int(i, "Type", mappingInfo[i][mappingType][house]);
		
	 	cache_get_value_name_float(i, "X", mappingPos[i][0][house]);
	 	cache_get_value_name_float(i, "Y", mappingPos[i][1][house]);
	 	cache_get_value_name_float(i, "Z", mappingPos[i][2][house]);
		
		cache_get_value_name_float(i, "RotX", mappingPos[i][3][house]);
		cache_get_value_name_float(i, "RotY", mappingPos[i][4][house]);
		cache_get_value_name_float(i, "RotZ", mappingPos[i][5][house]);
		
		if(mappingInfo[i][mappingGameModel][house] != INVALID_OBJECT_ID)
			DestroyDynamicObject(mappingInfo[i][mappingGameModel][house]);

		mappingInfo[i][mappingGameModel][house] = CreateDynamicObject(mappingInfo[i][mappingModel][house], mappingPos[i][0][house], mappingPos[i][1][house], mappingPos[i][2][house], mappingPos[i][3][house], mappingPos[i][4][house], mappingPos[i][5][house], house, houseInfo[house][houseInteriorVar]);
		returnTotalObjects ++;
	}
	return true;
}

public OnPlayerEditDynamicObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
	if(GetPVarInt(playerid, "house_repos_object"))
	{
		new Float:retPos[3];
		GetDynamicObjectPos(objectid, retPos[0], retPos[1], retPos[2]);
		
		if(!IsPlayerInRangeOfPoint(playerid, 30.0, retPos[0], retPos[1], retPos[2]) || !GetPlayerInterior(playerid))
			return sendError(playerid,"Anti abuz: editarea a luat sfarsit.");

		if(response == EDIT_RESPONSE_FINAL)
		{
			SendClientMessage(playerid, -1, "Pozitia item-ului a fost modificata.");
			DeletePVar(playerid, "house_repos_object");

			for(new i; i <= returnTotalObjects; i++)
			{
				if(mappingInfo[i][mappingGameModel][playerInfo[playerid][pHouse]] == objectid)
				{
					mappingPos[i][0][playerInfo[playerid][pHouse]] = x;
					mappingPos[i][1][playerInfo[playerid][pHouse]] = y;
					mappingPos[i][2][playerInfo[playerid][pHouse]] = z;

					mappingPos[i][3][playerInfo[playerid][pHouse]] = rx;
					mappingPos[i][4][playerInfo[playerid][pHouse]] = ry;
					mappingPos[i][5][playerInfo[playerid][pHouse]] = rz;

					DestroyDynamicObject(mappingInfo[i][mappingGameModel][playerInfo[playerid][pHouse]]);		
					mappingInfo[i][mappingGameModel][playerInfo[playerid][pHouse]] = CreateDynamicObject(mappingInfo[i][mappingModel][playerInfo[playerid][pHouse]], mappingPos[i][0][playerInfo[playerid][pHouse]], mappingPos[i][1][playerInfo[playerid][pHouse]], mappingPos[i][2][playerInfo[playerid][pHouse]], mappingPos[i][3][playerInfo[playerid][pHouse]], mappingPos[i][4][playerInfo[playerid][pHouse]], mappingPos[i][5][playerInfo[playerid][pHouse]], playerInfo[playerid][pHouse], houseInfo[playerInfo[playerid][pHouse]][houseInteriorVar]);

					mysql_format(SQL, returnString, sizeof returnString, "update `housemapping` set `X` = '%f', `Y` = '%f', `Z` = '%f', `RotX` = '%f', `RotY` = '%f', `RotZ` = '%f' where `ID` = '%d';", x, y, z, rx, ry, rz, mappingInfo[i][mappingID][playerInfo[playerid][pHouse]]);
					mysql_tquery(SQL, returnString, "", "");
					break;
				}
			}
		}
		if(response == EDIT_RESPONSE_CANCEL)
		{
			SendClientMessage(playerid, -1, "Pozitia item-ului a revenit la cea initiala.");
			DeletePVar(playerid, "house_repos_object");
			
			for(new i; i <= returnTotalObjects; i++)
			{
				if(mappingInfo[i][mappingGameModel][playerInfo[playerid][pHouse]] == objectid)
				{
					DestroyDynamicObject(mappingInfo[i][mappingGameModel][playerInfo[playerid][pHouse]]);
					mappingInfo[i][mappingGameModel][playerInfo[playerid][pHouse]] = CreateDynamicObject(mappingInfo[i][mappingModel][playerInfo[playerid][pHouse]], mappingPos[i][0][playerInfo[playerid][pHouse]], mappingPos[i][1][playerInfo[playerid][pHouse]], mappingPos[i][2][playerInfo[playerid][pHouse]], mappingPos[i][3][playerInfo[playerid][pHouse]], mappingPos[i][4][playerInfo[playerid][pHouse]], mappingPos[i][5][playerInfo[playerid][pHouse]], playerInfo[playerid][pHouse], houseInfo[playerInfo[playerid][pHouse]][houseInteriorVar]);
					break;
				}
			}
		}
	}
	return true;
}

new houseFurnitures[MAX_FURNITURE_CATEGORY][] =
{
	{1518, 14772, 19786, 19787, 19318, 19317, 19610, 2232, 2231, 19609, 19424, 1954},
	{1767, 1765, 1762, 1820, 1822, 1823, 1814, 1742, 1769, 2291, 2235, 1727, 1969, 1811, 1720, 2311, 2185, 2800, 2725, 1417, 2126, 11717, 2109, 2111, 2164, 1712, 1706, 11720, 1760, 2357, 14866, 2603, 2302, 1794, 11690, 2748, 2724, 1806, 1729},
	{2255, 2254, 2256, 2257, 2259, 2258, 2260, 2261, 2263, 2266, 2267, 2268, 2272, 2273, 2276, 2277, 2281, 2282, 2284, 19172, 19173, 2270, 2286, 3111},
	{2970, 19875, 2924, 2955, 3109, 2873, 2877, 2878, 2875, 2879, 2876, 2946, 1522, 1493, 1491, 1567, 1492, 2949, 1498, 1536, 2959, 1535, 1495, 1532, 1496, 1497, 1523, 1569, 1538, 1557, 1504, 1501, 1499, 1505, 1506, 1507, 1502, 19858, 19859, 19802, 1533, 1566}
};

hook OnPlayerModelSelectionEx(playerid, response, extraid, modelid)
{
	if(!response || GetPVarInt(playerid, "furniture_category") < 0)
		return Y_HOOKS_CONTINUE_RETURN_1;

	format(returnString, sizeof returnString, "Are you sure you want to buy this furniture? (category: %s)", returnCategory(GetPVarInt(playerid, "furniture_category")));
	ShowPlayerConfirmBox(playerid, HouseFurnitureBox, returnString, "House Furniture");

	SetPVarInt(playerid, "houseModelID", modelid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}
ConfirmBox:HouseFurnitureBox(playerid, response, selectable, controllable, Float:percent)
{

	new modelid = GetPVarInt(playerid, "houseModelID");
	if(response == CONFIRM_BOX_RESPONSE_NULL)
	{
		DeletePVar(playerid, "furniture_category");
		DeletePVar(playerid, "houseModelID");
		return sendError(playerid, "You just have to confirm, what's so hard?");

	}

	if(response == CONFIRM_BOX_RESPONSE_YES)
	{
		if(percent <= 25.00)
		{
			DeletePVar(playerid, "furniture_category");
			DeletePVar(playerid, "houseModelID");
			return sendError(playerid, "You confirmed too late, i'm sorry.");
		}

		if(playerInfo[playerid][pHouse] && houseInfo[playerInfo[playerid][pHouse]][houseOwner] == playerInfo[playerid][pSQLID])
		{
			new Float:retPos[3]; GetPlayerPos(playerid, retPos[0], retPos[1], retPos[2]);

			va_SendClientMessage(playerid, -1, "Ai platit $%s si ai achizitionat item-ul #%d (categorie: %s)", "20.000", modelid, returnCategory(GetPVarInt(playerid, "furniture_category")));

			mysql_format(SQL, returnString, sizeof returnString, "insert into `housemapping` (`Model`, `X`, `Y`, `Z`, `House`, `Type`) values ('%d', '%f', '%f', '%f', '%d', '%d');", modelid, retPos[0], retPos[1], retPos[2], playerInfo[playerid][pHouse], GetPVarInt(playerid, "furniture_category"));
			mysql_tquery(SQL, returnString, "request_creating_mapping", "ifffii", modelid, retPos[0], retPos[1], retPos[2], playerInfo[playerid][pHouse], GetPVarInt(playerid, "furniture_category"));
		}
		DeletePVar(playerid, "furniture_category");
		DeletePVar(playerid, "houseModelID");
	}
	return 1;
}
function:request_creating_mapping(const model, Float:X, Float:Y, Float:Z, const house, const type)
{
	returnTotalObjects ++;
	new i = returnTotalObjects - 1;

	mappingInfo[i][mappingID][house] = cache_insert_id();
	mappingInfo[i][mappingModel][house] = model;
	mappingPos[i][0][house] = X;
	mappingPos[i][1][house] = Y;
	mappingPos[i][2][house] = Z;

	mappingPos[i][3][house] = 0;
	mappingPos[i][4][house] = 0;
	mappingPos[i][5][house] = 0;
	
	if(mappingInfo[i][mappingGameModel][house] != INVALID_OBJECT_ID)
		DestroyDynamicObject(mappingInfo[i][mappingGameModel][house]);

	mappingInfo[i][mappingGameModel][house] = CreateDynamicObject(model, X, Y, Z, 0, 0, 0, house, houseInfo[house][houseInteriorVar]);
	

	#if defined TEST_SERVER
		va_SendClientMessageToAll(COLOR_YELLOW, "RQ: (%d | %d) CreateDynamicObject(%d, %f, %f, %f, 0, 0, 0, %d, %d)", i, house, model, X, Y, Z, house, houseInfo[house][houseInteriorVar]);
	#endif
	return true;
}

public OnPlayerSelectDynamicObject(playerid, objectid, modelid, Float:x, Float:y, Float:z)
{
	if(GetPVarInt(playerid, "house_repos_object") == 1)
		EditDynamicObject(playerid, objectid);

	else if(GetPVarInt(playerid, "house_delete_object") == 1)
	{
		va_SendClientMessage(playerid, -1, "Obiectul #%d a fost sters.", modelid);
		for(new i; i < returnTotalObjects + 1; i++)
		{
			if(mappingInfo[i][mappingGameModel][playerInfo[playerid][pHouse]] == objectid)
			{
				DestroyDynamicObject(mappingInfo[i][mappingGameModel][playerInfo[playerid][pHouse]]);

				#if defined TEST_SERVER
					new house = playerInfo[playerid][pHouse];
					va_SendClientMessageToAll(COLOR_YELLOW, "(%d | %d) DestroyDynamicObject(%d)", i, house, mappingInfo[i][mappingGameModel][playerInfo[playerid][pHouse]]);
				#endif

				mysql_format(SQL, returnString, sizeof returnString, "delete from `housemapping` where `ID` = '%d';", mappingInfo[i][mappingID][playerInfo[playerid][pHouse]]);
				mysql_tquery(SQL, returnString, "", "");
				break;
			}
		}
		returnTotalObjects --;
	}
	return true;
}

Dialog:DIALOG_FURNITURE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	switch(listitem)
	{
		case 0:
		{
			if(returnTotalObjects >= MAX_HOUSE_OBJECTS)
				return sendError(playerid,"Item-ul nu a fost cumparat.");

			if(houseInfo[playerInfo[playerid][pHouse]][houseObjects] >= 25)
				return sendError(playerid,"Ai prea multe obiecte cumparate. Le poti sterge folosind comanda /housefurniture.");

			Dialog_Show(playerid, DIALOG_FURNITURE_BUY, DIALOG_STYLE_TABLIST_HEADERS, "House furniture:", "Category\tItem Price\nTV, Audio, Video\t$20.000\nFitting\t$20.000\nPaintings\t$20.000\nDoors\t$20.000", "Select", "Exit");
		}
		case 1:
		{
			SendClientMessage(playerid, -1, "Foloseste mouseul pentru a selecta un obiect din mobilier apoi meniul pentru a il repozitiona."), BeginObjectSelecting(playerid);
			SetPVarInt(playerid, "house_delete_object", 0), SetPVarInt(playerid, "house_repos_object", 1);
		}
		case 2:
		{
			SendClientMessage(playerid, -1, "Foloseste mouseul pentru a selecta un obiect din mobilier apoi meniul pentru a il sterge."), BeginObjectSelecting(playerid);
			SetPVarInt(playerid, "house_delete_object", 1), SetPVarInt(playerid, "house_repos_object", 0);
		}
		case 3: Dialog_Show(playerid, DIALOG_DEL_ITEMS, DIALOG_STYLE_MSGBOX, "House furniture:", "Esti sigur ca vrei sa stergi toate obiectele din casa?\nOdata ce ai facut acest lucru, casa ta o sa ramana fara obiecte.", "Select", "Exit");
	}
	return true;
}

returnTotalItems(const listitem)
{
    switch(listitem)
    {
        case 0: return 12;
        case 1: return 39;
        case 2: return 23;
        case 3: return 42;
        default: return 0;
    }
    return 0;
}

Dialog:DIALOG_DEL_ITEMS(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	SendClientMessage(playerid, -1, "Toate itemele din casa ta au fost sterse.");
	
	for(new i; i <= returnTotalObjects ; i++)
		DestroyDynamicObject(mappingInfo[i][mappingGameModel][playerInfo[playerid][pHouse]]);

	mysql_format(SQL, returnString, sizeof returnString, "delete from `housemapping` where `House` = '%d';", playerInfo[playerid][pHouse]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

Dialog:DIALOG_FURNITURE_BUY(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    ShowModelSelectionMenuEx(playerid, houseFurnitures[listitem], returnTotalItems(listitem), returnCategory(listitem), -1, 0.0, 0.0, 0.0, 2.0, COLOR_SERVER, 0x88888899 , 0xFFFF00AA);
    SetPVarInt(playerid, "furniture_category", listitem);
    return true;
}

new Float: hInteriorCoords[][3] =
{
	{140.17, 1366.07, 1083.65}, {225.68, 1021.45, 1084.02}, {-2636.68, 1402.55, 906.46},
	{2807.48, -1174.76, 1025.57}, {377.15, 1417.41, 1081.33}, {446.99, 1397.07, 1084.30},
	{387.22, 1471.70, 1080.19}, {2237.59, -1081.64, 1049.02}, {-68.81, 1351.21, 1080.21}, {446.7725, 506.6027, 1001.4195}
};
new hInteriorID[] =
{
	5, 7, 3, 8, 15, 2, 15, 2, 6, 12
};

YCMD:createhouse(playerid, params[], help)
{	
	if(playerInfo[playerid][pAdmin] < 6)
		return returnNoAcces(playerid);

	check_queries

	new returnType;
	if(sscanf(params, "i", returnType))
		return sendSyntaxMessage(playerid, "/createhouse [0 - big | 1 - medium | 2 - small]");

	if(returnType < 0 || returnType > 2)
		return sendError(playerid,"Invalid type.");

	new Float:returnPos[3], globalVar;
	GetPlayerPos(playerid, returnPos[0], returnPos[1], returnPos[2]);

	switch(returnType)
	{
		case 0: globalVar = RandomEx(0, 2);
		case 1: globalVar = RandomEx(3, 5);
		case 2: globalVar = RandomEx(6, 9);
	}
	mysql_format(SQL, returnString, sizeof returnString, "insert into `houses` (`Interior`, `OwnerSQL`, `Type`, `ExteriorX`, `ExteriorY`, `ExteriorZ`, `InteriorX`, `InteriorY`, `InteriorZ`) values ('%d', '0', '%d', '%f', '%f', '%f', '%f', '%f', '%f');",
		hInteriorID[globalVar], returnType, returnPos[0], returnPos[1], returnPos[2], hInteriorCoords[globalVar][0], hInteriorCoords[globalVar][1], hInteriorCoords[globalVar][2]);
	mysql_tquery(SQL, returnString, "WhenHouseIsCreated", "iffffffii", returnType, returnPos[0], returnPos[1], returnPos[2], hInteriorCoords[globalVar][0], hInteriorCoords[globalVar][1], hInteriorCoords[globalVar][2], hInteriorID[globalVar], playerid);

	request_update
	return true;
}

YCMD:movehouse(playerid, params[], help)
{
	if(playerInfo[playerid][pAdmin] < 6)
		return returnNoAcces(playerid);

	check_queries

	new returnHouse;
	if(sscanf(params, "i", returnHouse))
		return sendSyntaxMessage(playerid, "/movehouse [house id]");

	new Float:returnPos[3], houseQuery[256];
	GetPlayerPos(playerid, returnPos[0], returnPos[1], returnPos[2]);

	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_RED, "%s moved house %d. [%f %f %f]", GetName(playerid), returnHouse, returnPos[0], returnPos[1], returnPos[2]);

	houseInfo[returnHouse][houseExterior][0] = returnPos[0];
	houseInfo[returnHouse][houseExterior][1] = returnPos[1];
	houseInfo[returnHouse][houseExterior][2] = returnPos[2];

	WhenHouseGotUpdated(returnHouse);

	mysql_format(SQL, houseQuery, sizeof houseQuery, "update `houses` set `ExteriorX` = '%f', `ExteriorY` = '%f', `ExteriorZ` = '%f' where `id` = '%d';", returnPos[0], returnPos[1], returnPos[2], returnHouse);
	mysql_tquery(SQL, houseQuery, "", "");
	return true;
}

forward public WhenHouseIsCreated(type, Float:X, Float:Y, Float:Z, Float:iX, Float:iY, Float:iZ, interior, requester);
public WhenHouseIsCreated(type, Float:X, Float:Y, Float:Z, Float:iX, Float:iY, Float:iZ, interior, requester)
{
	if(MAX_HOUSES <= returnServerHouses)
		return SendClientMessage(requester, -1, "You can't create a house because there are too many on server.");

	returnServerHouses ++;
	new i = returnServerHouses;

	houseInfo[i][houseID] = cache_insert_id();
	houseInfo[i][houseType] = type;

	houseInfo[i][houseOwner] = 0;

	format(houseInfo[i][houseDescription], MAX_PLAYER_NAME, ""SERVER_NAME2" House");
	format(houseInfo[i][houseOwnerName], MAX_PLAYER_NAME, "AdmBot");

	houseInfo[i][houseExterior][0] = X;
	houseInfo[i][houseExterior][1] = Y;
	houseInfo[i][houseExterior][2] = Z;

	houseInfo[i][houseInterior][0] = iX;
	houseInfo[i][houseInterior][1] = iY;
	houseInfo[i][houseInterior][2] = iZ;

	houseInfo[i][houseInteriorVar] = interior;
	WhenHouseGotUpdated(i);

	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_RED, "%s created a new house (#%d, type: %d).", GetName(requester), i, type);
	return true;
}

YCMD:howner(playerid, params[], help)
{
	if(playerInfo[playerid][pAdmin] < 6)
		return returnNoAcces(playerid);

	new returnName[MAX_PLAYER_NAME], returnHouse;
	if(sscanf(params, "s[24]d", returnName, returnHouse))
		return sendSyntaxMessage(playerid, "/howner [full name] [house]");

	format(returnString, sizeof returnString, "select `ID` from `users` where `name` = '%s';", returnName);
	new Cache: check_if_player_exist = mysql_query(SQL, returnString);
	
	if(!cache_num_rows())
	{
		cache_delete(check_if_player_exist);
		SendClientMessage(playerid, -1, "Invalid player.");
		return true;
	}
	else
	{
		static houseKey;
		cache_get_value_name_int(0, "ID", houseKey);

		SetPVarInt(playerid, "house_give_id", returnHouse);
		SetPVarInt(playerid, "house_give_player", houseKey);

		SetPVarString(playerid, "house_give_name", returnName);
	}
	cache_delete(check_if_player_exist);

	new dialogString[256];
	format(dialogString, sizeof dialogString, "Esti sigur ca vrei sa-i dai lui %s casa %d?\nDaca casa este detinuta de cineva, acesta va ramane fara aceasta.\nAbuzul de aceasta comanda duce la pierderea functiei!", returnName, returnHouse);

	Dialog_Show(playerid, DIALOG_HOWNER, DIALOG_STYLE_MSGBOX, "Esti sigur?", dialogString, "Da", "Nu");
	return true;
}

Dialog:DIALOG_HOWNER(playerid, response, listitem, inputtext[])
{
	if(!response)
		return DeletePVar(playerid, "house_give_player"), DeletePVar(playerid, "house_give_id"), DeletePVar(playerid, "house_give_name");

	new houseId = GetPVarInt(playerid, "house_give_id"), ownerId = GetPVarInt(playerid, "house_give_player");

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `House` = '0' where `House` = '%d';", houseId);
	mysql_tquery(SQL, returnString, "", "");

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `House` = '%d' where `id` = '%d';", houseId, ownerId);
	mysql_tquery(SQL, returnString, "", "");

	mysql_format(SQL, returnString, sizeof returnString, "update `houses` set `OwnerSQL` = '%d' where `id` = '%d';", ownerId, houseId);
	mysql_tquery(SQL, returnString, "", "");

	houseInfo[houseId][houseOwner] = ownerId;

	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_RED, "Administrator %s[admin:%d] has set %s[user:%d] owner for house %d.", GetDBName(playerid), playerInfo[playerid][pSQLID], GetNameBySQL(ownerId), ownerId, houseId);
	insertPlayerMail(ownerId, gettime(), "Administrator %s has set you owner for house %d.", GetDBName(playerid), houseId);

	foreach(new i : Player)
	{
		if(playerInfo[i][pSQLID] != ownerId)
			continue;

		playerInfo[i][pHouse] = houseId;
		va_SendClientMessage(i, COLOR_GREY, "Administrator %s has set you owner for house %d.", GetName(playerid), houseId);
		break;
	}

	format(houseInfo[houseId][houseOwnerName], MAX_PLAYER_NAME, "%s", GetNameBySQL(ownerId));
	WhenHouseGotUpdated(houseId);

	DeletePVar(playerid, "house_give_player"), DeletePVar(playerid, "house_give_id"), DeletePVar(playerid, "house_give_name");
	return true;
}
