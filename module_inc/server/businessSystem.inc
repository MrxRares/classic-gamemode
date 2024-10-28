#include <YSI_Coding\y_hooks>

#define MAX_BUSINESSES 100
#define STREAMER_BEGIN_BUSINESS (STREAMER_BEGIN_GARAGE + 100)
#define STREAMER_BEGIN_BUSINESS_SPHERE (STREAMER_BEGIN_BUSINESS + 100)

enum ENUM_BUSINESS_INFROMATIONS
{
	bizID, bizOwner, bizType, bizStatic, Float:bizPosition[6], bizInterior, bizOwnerName[MAX_PLAYER_NAME], bizLocked,
	bizSphere, Text3D:bizLabel, bizPickup, bizIcon, bizMessage[20], bizSecSphere, Text3D:bizLabelSec, bizMoney, bizFee
};
new bizInfo[MAX_BUSINESSES][ENUM_BUSINESS_INFROMATIONS];

new returnServerBusinesses;

function:loadBusinessesFromInclude()
{
	returnServerBusinesses = cache_num_rows();
	for(new i = 1; i <= cache_num_rows(); i++)
	{
		cache_get_value_name_int(i - 1, "ID", bizInfo[i][bizID]);
		cache_get_value_name_int(i - 1, "OwnerSQL", bizInfo[i][bizOwner]);
		cache_get_value_name_int(i - 1, "Type", bizInfo[i][bizType]);
		cache_get_value_name_int(i - 1, "Static", bizInfo[i][bizStatic]);

		cache_get_value_name_int(i - 1, "bizLocked", bizInfo[i][bizLocked]);
		cache_get_value_name_int(i - 1, "bizMoney", bizInfo[i][bizMoney]);
		cache_get_value_name_int(i - 1, "bizFee", bizInfo[i][bizFee]);

		cache_get_value_name_float(i - 1, "PosX", bizInfo[i][bizPosition][0]);
		cache_get_value_name_float(i - 1, "PosY", bizInfo[i][bizPosition][1]);
		cache_get_value_name_float(i - 1, "PosZ", bizInfo[i][bizPosition][2]);

		cache_get_value_name_float(i - 1, "secPosX", bizInfo[i][bizPosition][3]);
		cache_get_value_name_float(i - 1, "secPosY", bizInfo[i][bizPosition][4]);
		cache_get_value_name_float(i - 1, "secPosZ", bizInfo[i][bizPosition][5]);

		cache_get_value_name_int(i - 1, "Interior", bizInfo[i][bizInterior]);

		cache_get_value_name(i - 1, "Message", bizInfo[i][bizMessage]);
	}

	for(new business = 1; business <= returnServerBusinesses; business ++)
	{
		new stringCache[128];
		format(stringCache, sizeof stringCache, "select `Name` from `users` where `Business` = '%d';", business);
		
		new Cache: cache_player_name = mysql_query(SQL, stringCache);
		if(cache_num_rows())
			cache_get_value_name(0, "Name", bizInfo[business][bizOwnerName]);

		else
			format(bizInfo[business][bizOwnerName], MAX_PLAYER_NAME, "AdmBot");

		cache_delete(cache_player_name);

		WhenBusinessGotUpdated(business);
	}
	printf("Businesses: %d", returnServerBusinesses);
	return true;
}

hook loadServerSystems()
{
	mysql_tquery(SQL, "select * from `businesses`", "loadBusinessesFromInclude", "");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

forward public WhenBusinessGotUpdated(business);
public WhenBusinessGotUpdated(business)
{
	new bizLabelString[256];
	if(!bizInfo[business][bizOwner])
		format(bizLabelString, sizeof bizLabelString, ""COLOR_SV_HEX"%s{ffffff}\nBusiness  "COLOR_SV_HEX"#%d{ffffff}\nThis Business will be auctioned{ffffff}\nOwner:  "COLOR_SV_HEX"%s{ffffff}\nEnter Price:  "COLOR_SV_HEX"$%s{ffffff}\nLevel:  "COLOR_SV_HEX"5{ffffff}\n%s", bizInfo[business][bizMessage], business, bizInfo[business][bizOwnerName], FormatNumber(bizInfo[business][bizFee]), (bizInfo[business][bizLocked] == 0 ? ("") : (""COLOR_SV_HEX"(locked)")));
	else
		format(bizLabelString, sizeof bizLabelString, ""COLOR_SV_HEX"%s{ffffff}\nBusiness  "COLOR_SV_HEX"#%d{ffffff}\nOwner:  "COLOR_SV_HEX"%s{ffffff}\nPrice:  "COLOR_SV_HEX"$-1{ffffff}\nFee:  "COLOR_SV_HEX"$%s{ffffff}\n%s", bizInfo[business][bizMessage], business, bizInfo[business][bizOwnerName], FormatNumber(bizInfo[business][bizFee]), (bizInfo[business][bizLocked] == 0 ? ("") : (""COLOR_SV_HEX"(locked)")));

	DestroyDynamic3DTextLabel(bizInfo[business][bizLabel]);
	bizInfo[business][bizLabel] = CreateDynamic3DTextLabel(bizLabelString, -1, bizInfo[business][bizPosition][0], bizInfo[business][bizPosition][1], bizInfo[business][bizPosition][2], 25, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, false, 0);
	
	DestroyDynamicPickup(bizInfo[business][bizPickup]);
	bizInfo[business][bizPickup] = CreateDynamicPickup(1239, 23, bizInfo[business][bizPosition][0], bizInfo[business][bizPosition][1], bizInfo[business][bizPosition][2], -1, -1, -1, 60);
	
	DestroyDynamicArea(bizInfo[business][bizSphere]);
	bizInfo[business][bizSphere] = CreateDynamicSphere(bizInfo[business][bizPosition][0], bizInfo[business][bizPosition][1], bizInfo[business][bizPosition][2], bizInfo[business][bizType] >= 10 && bizInfo[business][bizType] <= 12 ? 25.0 : 1.5, 0, 0, -1);	
	Streamer_SetIntData(STREAMER_TYPE_AREA, bizInfo[business][bizSphere], E_STREAMER_EXTRA_ID, (business + STREAMER_BEGIN_BUSINESS));

	if(bizInfo[business][bizType] == 10 || bizInfo[business][bizType] == 11 || bizInfo[business][bizType] == 14)
	{
		bizInfo[business][bizSecSphere] = CreateDynamicSphere(bizInfo[business][bizPosition][3], bizInfo[business][bizPosition][4], bizInfo[business][bizPosition][5], 4.75, 0, -1, -1);
		Streamer_SetIntData(STREAMER_TYPE_AREA, bizInfo[business][bizSecSphere], E_STREAMER_EXTRA_ID, (business + STREAMER_BEGIN_BUSINESS_SPHERE));
	}

	if(bizInfo[business][bizType] == 14)
	{
		bizInfo[business][bizLabelSec] = CreateDynamic3DTextLabel(va_return(""COLOR_SV_HEX"Rent Car %s{ffffff}\n\nType /rentveh\nto rent a vehicle", GetCoordinatesCity(bizInfo[business][bizPosition][3], bizInfo[business][bizPosition][4])), -1, bizInfo[business][bizPosition][3], bizInfo[business][bizPosition][4], bizInfo[business][bizPosition][5], 40, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, false, 0);
	}
	if(bizInfo[business][bizType] == 2)
	{
		CreateDynamicActor(179, 294.5639, -82.9969, 1001.5156, 41.2562, true, 100, business, 4);
		CreateDynamicActor(119, 294.8924, -84.3657, 1001.5156, 90.6851, true, 100, business, 4);

		CreateDynamic3DTextLabel(""COLOR_SV_HEX"[GUN SHOP]{ffffff}\nPress "COLOR_SV_HEX"Y{ffffff} if you want to buy a gun.", -1, 293.4223,-80.8117,1001.5156, 25.0);
	}
	if(bizInfo[business][bizType] == 4)
	{
		CreateDynamicActor(194, -23.5991, -57.3145, 1003.5469, 356.0751, true, 100, business, 6);
		CreateDynamic3DTextLabel(""COLOR_SV_HEX"[SHOP]{ffffff}\nPress "COLOR_SV_HEX"Y{ffffff} if you want to buy something.", -1, -22.2731, -55.6953, 1003.5469, 25.0);
	}

	if(bizInfo[business][bizType] != 11 && bizInfo[business][bizType] != 13)
	{
		DestroyDynamicMapIcon(bizInfo[business][bizIcon]);
		bizInfo[business][bizIcon] = CreateDynamicMapIcon(bizInfo[business][bizPosition][0], bizInfo[business][bizPosition][1], bizInfo[business][bizPosition][2], GetBizIcon(bizInfo[business][bizType]), 0, -1, -1, -1, 200.0, MAPICON_GLOBAL);
	}
	return true;
}

hook OnPlayerEnterDynArea(playerid, areaid)
{
	new business = Streamer_GetIntData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID) - STREAMER_BEGIN_BUSINESS;
	if(business < 1 || returnServerBusinesses < business)
		return Y_HOOKS_CONTINUE_RETURN_1;

	if(bizInfo[business][bizType] == 10 || bizInfo[business][bizType] == 11)
	{
		DisableRemoteVehicleCollisions(playerid, true);
	}
	SetPVarInt(playerid, "business_area_sphere", business);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerLeaveDynArea(playerid, areaid)
{	
	if(GetPVarInt(playerid, "business_area_sphere") > 0)
	{
		DisableRemoteVehicleCollisions(playerid, false);
	}
	DeletePVar(playerid, "business_area_sphere");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook WhenPlayerLoggedIn(playerid)
{
	if(!cache_num_rows())
		return Y_HOOKS_CONTINUE_RETURN_1;

	cache_get_value_name_int(0, "Business", playerInfo[playerid][pBusiness]);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

GetBizIcon(biz_type)
{
	switch(biz_type)
	{
		case 1: return 52;
		case 2: return 18;
		case 3: return 49;
		case 4: return 17;
		case 5: return 10;
		case 6: return 54;
		case 7: return 14;
		case 8: return 45;
		case 9: return 25;
		case 10: return 63;
		case 11: return -1;
		case 12: return 27;
		case 13: return -1; //16
		case 14: return 51;
		case 15: return 29;
		case 16: return 38;
		default: return -1;
	}
	return true;
}

business_name(business_id)
{
	new bizString[32];
	switch(business_id)
	{
		case 1: bizString = "Bank";
		case 2: bizString = "GunShop";
		case 3: bizString = "Bar";
		case 4: bizString = "24/7";
		case 5: bizString = "Burger Shot";
		case 6: bizString = "Gym";
		case 7: bizString = "Cluckin Bell";
		case 8: bizString = "Clothes";
		case 9: bizString = "Casino";

		case 10: bizString = "PNS";
		case 11: bizString = "Tunning";
		case 12: bizString = "Gas Station";
		case 13: bizString = "CNN";
		case 14: bizString = "Rent Car";

		case 15: bizString = "Pizza";
		case 16: bizString = "Sex Shop";
	}
	return bizString;
}

WhenPlayerEnterBusiness(playerid, type, business)
{
	SetPlayerPos(playerid, bizInfo[business][bizPosition][3], bizInfo[business][bizPosition][4], bizInfo[business][bizPosition][5]);
	SetPlayerInterior(playerid, bizInfo[business][bizInterior]);
	SetPlayerVirtualWorld(playerid, business);
	switch(type)
	{
		case 1:
		{
			SendClientMessage(playerid, COLOR_GREEN, "Inselatoriile (tepele) nu sunt permise pe server. Pentru orice incercare de a insela un player cu bunuri/bani puteti primi ban permanent.");
			SendClientMessage(playerid, COLOR_GREEN, "Imprumuturile de orice fel (inclusiv datoriile) se fac pe proprie raspundere. Nu ai nicio garantie ca ii vei recupera.");
			SendClientMessage(playerid, COLOR_GREEN, "Afacerile cu bani reali nu sunt permise. Se pedepseste cu ban 7-14 zile la prima abatere si ban permanent la urmatoarea abatere.");
			SendClientMessage(playerid, -1, "Welcome! The commands of this business are as follows: /deposit, /withdraw, /balance, /transfer");
		}
		case 2:
		{
			SendClientMessage(playerid, COLOR_GREEN, "DM-ul este interzis. Puteti face DM doar pe politisti dar o sa primiti wanted fara a avea drept de predare.");
			SendClientMessage(playerid, COLOR_GREEN, "DM-ul pe un jucator se poate sanctiona cu jail 10 minute.");
			SendClientMessage(playerid, -1, "Welcome to the Gun Shop. You can buy guns by pressing Y in the front of the NPC.");
		}
		case 3: SendClientMessage(playerid, -1, "Welcome! The commands of this business are as follows: /buydrink");
		case 4: {
			SendClientMessage(playerid, -1, "Welcome to the 24/7 Store. You can buy items by pressing Y in the front of the NPC.");
			sell_fish(playerid);
		}
		case 5: SendClientMessage(playerid, -1, "Welcome! The commands of this business are as follows: /buyfood");
		case 6: SendClientMessage(playerid, -1, "Welcome! The commands of this business are as follows: /buyfightstyle");
		case 7: SendClientMessage(playerid, -1, "Welcome! The commands of this business are as follows: /buyfood");
		case 8: SendClientMessage(playerid, -1, "Welcome! The commands of this business are as follows: /clothes, /buyaccs");
		case 9:
		{
			ResetPlayerWeapons(playerid);
			SendClientMessage(playerid, COLOR_RED, "You were disarmed.");
			SendClientMessage(playerid, -1, "Welcome! The commands of this business are as follows: /dice");
		}
		case 15: SendClientMessage(playerid, -1, "Welcome! The commands of this business are as follows: /buyfood");
		case 16: SendClientMessage(playerid, -1, "Welcome! The commands of this business are as follows: /buytoys");
		default: return true;
	}
	return true;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
	if(newkeys == KEY_SECONDARY_ATTACK)
	{
		if(IsPlayerInAnyDynamicArea(playerid) && !bizInfo[GetPVarInt(playerid, "business_area_sphere")][bizStatic] && GetPVarInt(playerid, "business_area_sphere") > 0 && GetPVarInt(playerid, "business_area_sphere") <= returnServerBusinesses && !isPlayerInWar(playerid))
		{
			new bizId = GetPVarInt(playerid, "business_area_sphere");
			
		    if(GetMoney(playerid) <  bizInfo[bizId][bizFee] && GetMilliards(playerid) < 1)
		        return GameTextForPlayer(playerid, "~r~~h~You dont have the cash", 5000, 1);


			if(GetPVarInt(playerid, "enter_building_deelay") > gettime())
				return va_SendClientMessage(playerid, COLOR_GREY, "[Anti-Abuz] Nu poti intra intr-o cladire timp de %d secunde.", GetPVarInt(playerid, "enter_building_deelay") - gettime());

			if(bizInfo[bizId][bizLocked] && playerInfo[playerid][pBusiness] != bizId)
				return GameTextForPlayer(playerid, "~r~~h~Closed", 5000, 1);

			WhenPlayerEnterBusiness(playerid, bizInfo[bizId][bizType], bizId);
			playerStaticVar[playerid][inBusiness] = bizId;
			
			if(GetPVarInt(playerid, "last_biz_enter_time") < gettime() && GetPVarInt(playerid, "last_biz_id") != bizId)
			{
				substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, bizInfo[bizId][bizFee]);
				bizInfo[bizId][bizMoney] += bizInfo[bizId][bizFee];

				GameTextForPlayer(playerid, va_return("~r~-$%d", bizInfo[bizId][bizFee]), 5000, 1);
				SetPVarInt(playerid, "last_biz_enter_time", gettime() + 30);

				mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `bizMoney` = '%d' where `id` = '%d';", bizInfo[bizId][bizMoney], bizId);
				mysql_tquery(SQL, returnString, "", "");
			}

			SetPVarInt(playerid, "last_biz_id", bizId);
			return true;
		}

		if(playerStaticVar[playerid][inBusiness] && IsPlayerInRangeOfPoint(playerid, 3.0, bizInfo[playerStaticVar[playerid][inBusiness]][bizPosition][3], bizInfo[playerStaticVar[playerid][inBusiness]][bizPosition][4], bizInfo[playerStaticVar[playerid][inBusiness]][bizPosition][5]))
		{
			new bizId = playerStaticVar[playerid][inBusiness];

			SetPlayerPos(playerid, bizInfo[bizId][bizPosition][0], bizInfo[bizId][bizPosition][1], bizInfo[bizId][bizPosition][2]);
			SetPlayerVirtualWorld(playerid, 0), SetPlayerInterior(playerid, 0);

			//TODO: rob
			// if(playerStaticVar[playerid][robStatus] == 44) 
			// {
			// 	if(playerStaticVar[playerid][robStatus] == 56)
			// 		return true;

			// 	new house = random(returnServerHouses - 1);

			// 	SetPlayerCheckpointEx(playerid, houseInfo[house][houseExterior][0], houseInfo[house][houseExterior][1], houseInfo[house][houseExterior][2], 5.0);
			// 	playerStaticVar[playerid][pCheckpoint] = 437;
			// 	SendClientMessage(playerid, COLOR_RED, "Du-te si ascunde banii pentru a termina jaful");
			// 	GivePlayerWanted(playerid, INVALID_PLAYER_ID, 6, "armed bank robbery");
			// 	playerStaticVar[playerid][inBusiness] = 0;
			// 	SetPVarInt(playerid, "enter_building_deelay", gettime() + 15);
			// 	playerStaticVar[playerid][robStatus] = 56;

			// }

			playerStaticVar[playerid][inBusiness] = 0;
			SetPVarInt(playerid, "enter_building_deelay", gettime() + 15);
			return true;
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}
YCMD:buybiz(playerid, params[], help)
	return sendError(playerid,"This business will be auctioned.");

YCMD:findbiz(playerid, params[], help)
{
	if(playerStaticVar[playerid][pCheckpoint])
		return showActiveCheckpointDialog(playerid);

	new bizId;
	if(sscanf(params, "i", bizId))
		return sendSyntaxMessage(playerid, "/findbiz <Business ID>");

	if(bizId < 1 || bizId > returnServerBusinesses)
		return sendError(playerid,"Invalid bizz ID.");

	SendClientMessage(playerid, -1, "I have placed a checkpoint at that location. Please follow it.");
	playerStaticVar[playerid][pCheckpoint] = 1;

	return SetPlayerCheckpoint(playerid, bizInfo[bizId][bizPosition][0], bizInfo[bizId][bizPosition][1], bizInfo[bizId][bizPosition][2], 5.0);
}

YCMD:gotobiz(playerid, params[], help)
{
	check_admin

	new returnBusiness;
	if(sscanf(params, "i", returnBusiness))
		return sendSyntaxMessage(playerid, "/gotobiz [businessid]");

	if(returnBusiness > returnServerBusinesses)
		return sendError(playerid,"Invalid business.");

	SetPlayerPos(playerid, bizInfo[returnBusiness][bizPosition][0], bizInfo[returnBusiness][bizPosition][1], bizInfo[returnBusiness][bizPosition][2]);
	SetPlayerInterior(playerid, 0), SetPlayerVirtualWorld(playerid, 0); SendClientMessage(playerid, -1, "You have been teleported.");
	return true;
}

new business_int[] =
{
	0, 1, 11, 6, 10, 5, 9, 18, 10, 0, 0, 0, 0, 0, 5, 3
};
new Float:business_coords[][] =
{
	{2305.0430, -16.1294, 26.7422},
	{285.4405, -41.5128, 1001.5156},
	{501.8960, -68.0889, 998.7578},
	{-27.2393, -58.1129, 1003.5469},
	{363.1238, -75.1133, 1001.5078},
	{772.1120, -3.8986, 1000.7288},
	{364.9500, -11.5759, 1001.8516},
	{161.4328, -96.5507, 1001.8047},
	{2018.6392, 1017.8764, 996.8085},
	{},
	{},
	{},
	{},
	{},
	{372.3998, -133.0169, 1001.4922},
	{-100.3334, -24.6300, 1000.7188}
};

YCMD:createbiz(playerid, params[], help)
{
	check_queries

	if(playerInfo[playerid][pAdmin] < 6)
		return returnNoAcces(playerid);

	new returnType, Float:intPos[3], interior;
	if(sscanf(params, "ifffi", returnType, intPos[0], intPos[1], intPos[2], interior))
	{
		if(!returnType)
		{
			sendSyntaxMessage(playerid, "/createbiz [type] | OPTIONAL: [X] [Y] [Z] [INTERIOR]");
			SendClientMessage(playerid, COLOR_GREY, "(1) Bank | (2) GunShop | (3) Bar | (4) 24/7 | (5) Burger Shot | (6) Gym");
			SendClientMessage(playerid, COLOR_GREY, "(7) Cluckin Bell | (8) Clothes | (9) Casino | (10) PNS | (11) Tunning");
			SendClientMessage(playerid, COLOR_GREY, "(12) Gas Station | (13) CNN | (14) Rent Car | (15) Pizza | (16) Sex Shop");
			return true;
		}
	}

	new Float:returnPos[3], bizQuery[256];
	GetPlayerPos(playerid, returnPos[0], returnPos[1], returnPos[2]);

	if(!intPos[0])
	{
		intPos[0] = business_coords[returnType - 1][0];
		intPos[1] = business_coords[returnType - 1][1];
		intPos[2] = business_coords[returnType - 1][2];
		interior = business_int[returnType - 1];
	}

	mysql_format(SQL, bizQuery, sizeof bizQuery, "insert into `businesses` (`Type`, `PosX`, `PosY`, `PosZ`, `secPosX`, `secPosY`, `secPosZ`, `Interior`, `Static`) values ('%d', '%f', '%f', '%f', '%f', '%f', '%f', '%d', '%d');",  returnType, returnPos[0], returnPos[1], returnPos[2], intPos[0], intPos[1], intPos[2], interior, check_if_is_static_biz(returnType));
	mysql_tquery(SQL, bizQuery, "WhenBusinessIsCreated", "iffffffiii", returnType, returnPos[0], returnPos[1], returnPos[2], intPos[0], intPos[1], intPos[2], interior, check_if_is_static_biz(returnType), playerid);

	request_update
	return true;
}

YCMD:movebusiness(playerid, params[], help)
{
	if(playerInfo[playerid][pAdmin] < 6)
		return returnNoAcces(playerid);

	check_queries

	new returnBusiness, returnType;
	if(sscanf(params, "ii", returnBusiness, returnType))
		return sendSyntaxMessage(playerid, "/movebusiness [business id] [type: 0 - exterior, 1 - interior/action place]");

	if(returnType < 0 || returnType > 1)
		return true;

	new Float:returnPos[3], bizQuery[256];
	GetPlayerPos(playerid, returnPos[0], returnPos[1], returnPos[2]);

	if(!returnType)
	{
		bizInfo[returnBusiness][bizPosition][0] = returnPos[0];
		bizInfo[returnBusiness][bizPosition][1] = returnPos[1];
		bizInfo[returnBusiness][bizPosition][2] = returnPos[2];

		mysql_format(SQL, bizQuery, sizeof bizQuery, "update `businesses` set `posX` = '%f', `posY` = '%f', `posZ` = '%f' where `id` = '%d';", returnPos[0], returnPos[1], returnPos[2], returnBusiness);
		mysql_tquery(SQL, bizQuery, "", "");
	}
	else
	{
		bizInfo[returnBusiness][bizPosition][3] = returnPos[0];
		bizInfo[returnBusiness][bizPosition][4] = returnPos[1];
		bizInfo[returnBusiness][bizPosition][5] = returnPos[2];

		mysql_format(SQL, bizQuery, sizeof bizQuery, "update `businesses` set `secPosX` = '%f', `secPosY` = '%f', `secPosZ` = '%f' where `id` = '%d';", returnPos[0], returnPos[1], returnPos[2], returnBusiness);
		mysql_tquery(SQL, bizQuery, "", "");
	}

	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_RED, "%s moved business %d %s. [%f %f %f]", GetName(playerid), returnBusiness, (!returnType ? "exterior" : "interior"), returnPos[0], returnPos[1], returnPos[2]);
	WhenBusinessGotUpdated(returnBusiness);
	return true;
}

check_if_is_static_biz(type)
{
	if(type >= 1 && type <= 9 || type == 15 || type == 16)
		return false;
	else
		return true;
}

forward public WhenBusinessIsCreated(type, Float:X, Float:Y, Float:Z, Float:fX, Float:fY, Float:fZ, interior, isStatic, requester);
public WhenBusinessIsCreated(type, Float:X, Float:Y, Float:Z, Float:fX, Float:fY, Float:fZ, interior, isStatic, requester)
{
	if(MAX_BUSINESSES <= returnServerBusinesses)
		return SendClientMessage(requester, -1, "You can't create a business because there are too many on server.");

	returnServerBusinesses ++;
	new i = returnServerBusinesses;

	bizInfo[i][bizID] = cache_insert_id();

	format(bizInfo[i][bizMessage], MAX_PLAYER_NAME, ""SERVER_NAME2" Business");
	format(bizInfo[i][bizOwnerName], MAX_PLAYER_NAME, "AdmBot");
	bizInfo[i][bizStatic] = isStatic;
	
	bizInfo[i][bizPosition][0] = X;
	bizInfo[i][bizPosition][1] = Y;
	bizInfo[i][bizPosition][2] = Z;

	bizInfo[i][bizPosition][3] = fX;
	bizInfo[i][bizPosition][4] = fY;
	bizInfo[i][bizPosition][5] = fZ;

	bizInfo[i][bizInterior] = interior; 
	bizInfo[i][bizType] = type;

	if(type == 10 || type == 11 || type == 14)
	{
		new choseLocation[64];
		switch(type)
		{
			case 10: choseLocation = "for the PNS to repair vehicles - sphere";
			case 11: choseLocation = "for the tunning - sphere";
			case 14: choseLocation = "for the players to rent vehicles - label + pickup + sphere";
		}

		new bizString[256];
		format(bizString, sizeof bizString, "Because you seleted to create a static business in the next 20 seconds you need to chose another location\n\
			Business type: %s (the location will be used %s)", business_name(bizInfo[i][bizType]), choseLocation);
		
		Dialog_Show(requester, DIALOG_NONE, DIALOG_STYLE_MSGBOX, "Create business:", bizString, "Ok", "");
		SendClientMessage(requester, COLOR_GREY, "Waiting for server... [labels will apear after the sphere is placed]");
		
		SetTimerEx("business_create_2nd_step", 20000, false, "iii", requester, type, i);
		return true;
	}
	WhenBusinessGotUpdated(i);

	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_RED, "%s created a business (#%d, type: %d).", GetName(requester), i, business_name(type));
	return true;
}
forward public business_create_2nd_step(playerid, type, biz);
public business_create_2nd_step(playerid, type, biz)
{
	Dialog_Show(playerid, DIALOG_CHOSE_LOCATION, DIALOG_STYLE_MSGBOX, "Create business:", "Confirm:\nAre you sure if you want to chose this position for the business action place.\nIf you aren't sure press 'No' button and then wait again 20 seconds.", "Yes", "No");
	
	SetPVarInt(playerid, "business_type", type);
	SetPVarInt(playerid, "business_id", biz);
	return true;
}
Dialog:DIALOG_CHOSE_LOCATION(playerid, response, listitem, inputtext[])
{
	if(!response)
		return sendError(playerid,"Waiting another 20 seconds.."), SetTimerEx("business_create_2nd_step", 20000, false, "iii", playerid, GetPVarInt(playerid, "business_type"), GetPVarInt(playerid, "business_id")), SendClientMessage(playerid, COLOR_GREY, "Waiting another 20 seconds for repositioning...");

	new Float:X, Float:Y, Float:Z;
	GetPlayerPos(playerid, X, Y, Z);

	new i = GetPVarInt(playerid, "business_id");

	bizInfo[i][bizPosition][3] = X;
	bizInfo[i][bizPosition][4] = Y;
	bizInfo[i][bizPosition][5] = Z;

	SendClientMessage(playerid, COLOR_GREY, "Business sphere for action was updated.");
	WhenBusinessGotUpdated(i);

	new bizQuery[256];
	mysql_format(SQL, bizQuery, sizeof bizQuery, "update `businesses` set `secPosX` = '%f', `secPosY` = '%f', `secPosZ` = '%f' where `id` = '%d';", X, Y, Z, i);
	mysql_tquery(SQL, bizQuery, "", "");

	DeletePVar(playerid, "business_id");
	DeletePVar(playerid, "business_type");
	return true;
}

YCMD:bizstatus(playerid, params[], help)
{
	if(!playerInfo[playerid][pBusiness])
		return sendError(playerid,"You don't own a business.");

	va_SendClientMessage(playerid, -1, "*** %s ***", bizInfo[playerInfo[playerid][pBusiness]][bizMessage]);
	va_SendClientMessage(playerid, -1, "Locked: %d Entry Fee: $%s Till: $%s.", bizInfo[playerInfo[playerid][pBusiness]][bizLocked], FormatNumber(bizInfo[playerInfo[playerid][pBusiness]][bizFee]), FormatNumber(bizInfo[playerInfo[playerid][pBusiness]][bizMoney]));
	SendClientMessage(playerid, COLOR_GREEN,"_______________________________________");
	return true;
}

YCMD:bizfee(playerid, params[], help)
{
	if(!playerInfo[playerid][pBusiness])
		return sendError(playerid,"You don't own a business.");

	new returnAmount;
	if(sscanf(params, "d", returnAmount))
		return sendSyntaxMessage(playerid, "/bizfee <Money>");

	if(returnAmount < 1 || returnAmount > 5000)
		return sendError(playerid,"Minimum fee is $1, maximum fee is $5,000.");

	bizInfo[playerInfo[playerid][pBusiness]][bizFee] = returnAmount;
	WhenBusinessGotUpdated(playerInfo[playerid][pBusiness]);

	va_SendClientMessage(playerid, -1, "{FFFF00}Entrance fee set to $%s.", FormatNumber(bizInfo[playerInfo[playerid][pBusiness]][bizFee]));

	mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `bizFee` = '%d' where `id` = '%d';", bizInfo[playerInfo[playerid][pBusiness]][bizFee], playerInfo[playerid][pBusiness]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

YCMD:bizwithdraw(playerid, params[], help)
{
	if(!playerInfo[playerid][pBusiness])
		return sendError(playerid,"You don't own a business.");

	new returnAmount;
	if(sscanf(params, "d", returnAmount))
		return sendSyntaxMessage(playerid, "/bizwithdraw <Amount of Money>");

	if(returnAmount > 10000000)
		return sendError(playerid,"Maxim amount of money is $10,000,000.");

	if(returnAmount > bizInfo[playerInfo[playerid][pBusiness]][bizMoney] || !returnAmount)
		return sendError(playerid,"You dont have that much.");

	if(playerStaticVar[playerid][inBusiness] != playerInfo[playerid][pBusiness] && !IsPlayerInRangeOfPoint(playerid, 5.0, bizInfo[playerInfo[playerid][pBusiness]][bizPosition][0], bizInfo[playerInfo[playerid][pBusiness]][bizPosition][1], bizInfo[playerInfo[playerid][pBusiness]][bizPosition][2]))
		return sendError(playerid,"You are to far from your business.");

	addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, returnAmount, false);

	bizInfo[playerInfo[playerid][pBusiness]][bizMoney] -= returnAmount;

	va_SendClientMessage(playerid, COLOR_YELLOW, "{FFFF00}You have withdrawn $%s from your till. Total: $%s.", FormatNumber(returnAmount), FormatNumber(bizInfo[playerInfo[playerid][pBusiness]][bizMoney]));
	
	mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `bizMoney` = '%d' where `id` = '%d';", bizInfo[playerInfo[playerid][pBusiness]][bizMoney], playerInfo[playerid][pBusiness]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}
YCMD:bizname(playerid, params[], help)
{
	if(!playerInfo[playerid][pBusiness])
		return sendError(playerid,"You don't own a business.");

	new returnDesc[20];
	if(sscanf(params, "s[20]", returnDesc))
		return sendSyntaxMessage(playerid, "/bizname <Message>");

	if(strlen(returnDesc) > 20)
		return sendError(playerid,"Maximum caracter is 20.");

	format(bizInfo[playerInfo[playerid][pBusiness]][bizMessage], sizeof returnDesc, returnDesc);
	WhenBusinessGotUpdated(playerInfo[playerid][pBusiness]);

	va_SendClientMessage(playerid, -1, "{FFFF00}Business name set to %s.", returnDesc);

	mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `Message` = '%e' where `id` = '%d';", returnDesc, playerInfo[playerid][pBusiness]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}
YCMD:sellbizto(playerid, params[], help)
	return true;

YCMD:sellbiztostate(playerid, params[], help)
{
	if(!playerInfo[playerid][pBusiness])
		return sendError(playerid,"You don't own a business.");

	new bizId = playerInfo[playerid][pBusiness];

	PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
	GameTextForPlayer(playerid, "~w~Congratulations~n~ You have sold your property for ~n~~g~$1000000", 10000, 3);

	addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 1000000, false);
	
	playerInfo[playerid][pBusiness] = 0;

	mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `OwnerSQL` = '0', `bizMoney` = '0', `bizLocked` = '0' where `id` = '%d';", bizId);
	mysql_tquery(SQL, returnString, "", "");

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Business` = '0' where `id` = '%d';", playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");

	format(bizInfo[bizId][bizOwnerName], MAX_PLAYER_NAME, "AdmBot");
	bizInfo[bizId][bizMoney] = 0;
	bizInfo[bizId][bizLocked] = 0;
	bizInfo[bizId][bizOwner] = 0;

	WhenBusinessGotUpdated(bizId);
	return true;
}

YCMD:bowner(playerid, params[], help)
{
	if(playerInfo[playerid][pAdmin] < 6)
		return returnNoAcces(playerid);

	new returnName[MAX_PLAYER_NAME], returnBusiness;
	if(sscanf(params, "s[24]d", returnName, returnBusiness))
		return sendSyntaxMessage(playerid, "/bowner [full name] [business]");

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
		static bizKey;
		cache_get_value_name_int(0, "ID", bizKey);

		SetPVarInt(playerid, "biz_give_id", returnBusiness);
		SetPVarInt(playerid, "biz_give_player", bizKey);
	}
	cache_delete(check_if_player_exist);

	new dialogString[256];
	format(dialogString, sizeof dialogString, "Esti sigur ca vrei sa-i dai lui %s afacerea %d?\nDaca afacerea este detinuta de cineva, acesta va ramane fara aceasta.\nAbuzul de aceasta comanda duce la pierderea functiei!", returnName, returnBusiness);

	Dialog_Show(playerid, DIALOG_BOWNER, DIALOG_STYLE_MSGBOX, "Esti sigur?", dialogString, "Da", "Nu");
	return true;
}

Dialog:DIALOG_BOWNER(playerid, response, listitem, inputtext[])
{
	if(!response)
		return DeletePVar(playerid, "biz_give_player"), DeletePVar(playerid, "biz_give_id"), DeletePVar(playerid, "biz_give_name");

	new bizId = GetPVarInt(playerid, "biz_give_id"), ownerId = GetPVarInt(playerid, "biz_give_player");

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Business` = '0' where `Business` = '%d';", bizId);
	mysql_tquery(SQL, returnString, "", "");

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Business` = '%d' where `id` = '%d';", bizId, ownerId);
	mysql_tquery(SQL, returnString, "", "");

	mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `OwnerSQL` = '%d' where `id` = '%d';", ownerId, bizId);
	mysql_tquery(SQL, returnString, "", "");

	bizInfo[bizId][bizOwner] = ownerId;

	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_RED, "Administrator %s[admin:%d] has set %s[user:%d] owner for business %d.", GetDBName(playerid), playerInfo[playerid][pSQLID], GetNameBySQL(ownerId), ownerId, bizId);
	foreach(new i : Player)
	{
		if(playerInfo[i][pSQLID] != ownerId)
			continue;

		playerInfo[i][pBusiness] = bizId;
		va_SendClientMessage(i, COLOR_GREY, "Administrator %s has set you owner for business %d.", GetName(playerid), bizId);
		break;
	}

	format(bizInfo[bizId][bizOwnerName], MAX_PLAYER_NAME, "%s", GetNameBySQL(ownerId));
	WhenBusinessGotUpdated(bizId);

	insertPlayerMail(ownerId, gettime(), "Administrator %s has set you owner for business %d.", GetDBName(playerid), bizId);


	DeletePVar(playerid, "biz_give_player"), DeletePVar(playerid, "biz_give_id"), DeletePVar(playerid, "biz_give_name");
	return true;
}

#include "../module_inc/src/biz/pns_tunning.inc"
#include "../module_inc/src/biz/binco.inc"
#include "../module_inc/src/biz/rentveh.inc"
#include "../module_inc/src/biz/gunshop.inc"
#include "../module_inc/src/biz/24_7.inc"
#include "../module_inc/src/biz/gym.inc"
#include "../module_inc/src/biz/others.inc"



//casino
// #include "../module_inc/src/biz/c_gamble.inc"
// #include "../module_inc/src/biz/c_roulette.inc"
// #include "../module_inc/src/biz/c_blackjack.inc"