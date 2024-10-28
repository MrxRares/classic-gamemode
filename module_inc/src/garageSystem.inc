#include <YSI_Coding\y_hooks>

#define STREAMER_BEGIN_GARAGE (STREAMER_BEGIN_HOUSE + 100)

enum ENUM_GARAGE_INFROMATIONS
{
	garageID, garageHouse, garageLocked, garageDesc[40], Float:garageExterior[4], garagePickup, Text3D:garageLabel, garageSphere, garageMechanic, garageGasStation
};
new garageInfo[MAX_HOUSES][ENUM_GARAGE_INFROMATIONS];

new returnServerGarages;
function:loadGaragesFromInclude()
{
	returnServerGarages = cache_num_rows();
	for(new i = 1; i <= cache_num_rows(); i++)
	{
		cache_get_value_name_int(i - 1, "ID", garageInfo[i][garageID]);
		cache_get_value_name_int(i - 1, "House", garageInfo[i][garageHouse]);

		cache_get_value_name_int(i - 1, "Mechanic", garageInfo[i][garageMechanic]);
		cache_get_value_name_int(i - 1, "GasStation", garageInfo[i][garageGasStation]);

		cache_get_value_name_int(i - 1, "Locked", garageInfo[i][garageLocked]);

		cache_get_value_name_float(i - 1, "garageX", garageInfo[i][garageExterior][0]);
		cache_get_value_name_float(i - 1, "garageY", garageInfo[i][garageExterior][1]);
		cache_get_value_name_float(i - 1, "garageZ", garageInfo[i][garageExterior][2]);
		cache_get_value_name_float(i - 1, "garageA", garageInfo[i][garageExterior][3]);

		cache_get_value_name(i - 1, "Description", garageInfo[i][garageDesc]);
	}
	printf("Garages: %d", returnServerGarages);
	return true;
}

hook OnPlayerEnterDynArea(playerid, areaid)
{
	new garage = Streamer_GetIntData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID) - STREAMER_BEGIN_GARAGE;
	if(returnServerGarages < garage)
		return true;

	SetPVarInt(playerid, "areaID_garage", garage);
	return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnPlayerLeaveDynArea(playerid, areaid)
{
	SetPVarInt(playerid, "areaID_garage", 0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}
YCMD:enter(playerid, params[], help)
{
	if(IsPlayerInAnyDynamicArea(playerid) && GetPVarInt(playerid, "areaID_garage") > 0)
	{
		if(GetPVarInt(playerid, "enter_building_deelay") > gettime())
			return va_SendClientMessage(playerid, COLOR_GREY, "[Anti-Abuz] Nu poti intra intr-o cladire timp de %d secunde.", GetPVarInt(playerid, "enter_building_deelay") - gettime());

		if(garageInfo[GetPVarInt(playerid, "areaID_garage")][garageLocked] && houseInfo[playerInfo[playerid][pHouse]][houseGarage] != GetPVarInt(playerid, "areaID_garage"))
			return sendError(playerid,"You cannot enter the garage becuase you are not its owner or you do not have rent on the house.");

		if(GetPlayerState(playerid) == 2) {
			new returnPlayerVehicleID = GetPlayerVehicleID(playerid);
			
			SetVehiclePos(returnPlayerVehicleID, -235.8259 - 4.0, 2529.7288 - 100.0, 35.7520 + 2.0);
			SetVehicleVirtualWorld(returnPlayerVehicleID, garageInfo[GetPVarInt(playerid, "areaID_garage")][garageID]);
			SetPlayerVirtualWorld(playerid, garageInfo[GetPVarInt(playerid, "areaID_garage")][garageID]);

			
			SetVehicleZAngle(returnPlayerVehicleID, garageInfo[GetPVarInt(playerid, "areaID_garage")][garageExterior][3]);
			//PutPlayerInVehicleEx(playerid, returnPlayerVehicleID, 0);  
		}
		else
		{
			SetPlayerFacingAngle(playerid, 85.0);
			SetPlayerPos(playerid, -235.8259, 2529.7288 - 100.0, 35.7520 + 1.0);
			SetPlayerVirtualWorld(playerid, garageInfo[GetPVarInt(playerid, "areaID_garage")][garageID]);
			
			FreezePlayer(playerid, 2);
			SetCameraBehindPlayer(playerid);

		}

	}
	return true;
}
YCMD:exit(playerid, params[], help)
{
	if(IsPlayerInRangeOfPoint(playerid, 5.0, -235.8259, 2529.7288 - 100.0, 35.7520) && GetPlayerVirtualWorld(playerid) != 0)
	{

		new returnGarageVirtual = garageInfo[GetPlayerVirtualWorld(playerid)][garageID];
		if(GetPlayerState(playerid) == 2) {
			new returnPlayerVehicleID = GetPlayerVehicleID(playerid),
				Float: x = garageInfo[returnGarageVirtual][garageExterior][0], Float: y = garageInfo[returnGarageVirtual][garageExterior][1], Float: a = garageInfo[returnGarageVirtual][garageExterior][3],
				Float: distance = 5;

			x += (distance * floatsin(-a, degrees));
			y += (distance * floatcos(-a, degrees));

			SetVehiclePos(returnPlayerVehicleID, x, y, garageInfo[returnGarageVirtual][garageExterior][2]);
			SetVehicleZAngle(returnPlayerVehicleID, garageInfo[returnGarageVirtual][garageExterior][3]);

			SetVehicleVirtualWorld(returnPlayerVehicleID, 0);
			SetPlayerVirtualWorld(playerid, 0);
			SetPlayerInterior(playerid, 0);
			PutPlayerInVehicle(playerid, returnPlayerVehicleID, 0);
		}
		else
		{
			SetPlayerFacingAngle(playerid, garageInfo[returnGarageVirtual][garageExterior][3]);
			
			SetPlayerPos(playerid, garageInfo[returnGarageVirtual][garageExterior][0], garageInfo[returnGarageVirtual][garageExterior][1], garageInfo[returnGarageVirtual][garageExterior][2]);
			SetPlayerVirtualWorld(playerid, 0);
			SetPlayerInterior(playerid, 0);

		}

		SetPVarInt(playerid, "enter_building_deelay", gettime() + 5);
	}
	return true;
}

YCMD:garage(playerid, params[], help)
{
	new houseId = playerInfo[playerid][pHouse];

	if(houseInfo[houseId][houseOwner] != playerInfo[playerid][pSQLID] || !houseId)
		return sendError(playerid,"You don't own a house.");

	if(!houseInfo[playerInfo[playerid][pHouse]][houseGarage])
		return sendError(playerid,"Casa ta nu are un garaj.");

	new gId = houseInfo[playerInfo[playerid][pHouse]][houseGarage];
	format(returnString, sizeof returnString, "%s{ffffff}\nGaseste garajul\nSchimba descrierea garajului\nUpgradati garajul\nMuta locatia garajului", (!garageInfo[gId][garageLocked] ? ("{1BED30}Unlock garage") : ("{ED3D22}Lock garage")));

	Dialog_Show(playerid, DIALOG_GMENU, DIALOG_STYLE_LIST, "Managereaza-ti garajul", returnString, "Selectare", "Renunta");
	return true;
}

Dialog:DIALOG_GMENU(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	new gId = houseInfo[playerInfo[playerid][pHouse]][houseGarage];
	switch(listitem)
	{
		case 0:
		{
			va_SendClientMessage(playerid, -1, "Garajului tau a fost %s{FFFFFF}.", (!garageInfo[gId][garageLocked] ? ("{1BED30}descuiat") : ("{ED3D22}incuiat")));
		
			garageInfo[gId][garageLocked] = garageInfo[gId][garageLocked] ? (0) : (1);
			WhenGarageGotUpdated(gId);

			mysql_format(SQL, returnString, sizeof returnString, "update `garages` set `Locked` = '%d' where `ID` = '%d';", garageInfo[gId][garageLocked], gId);
			mysql_tquery(SQL, returnString, "", "");
		}
		case 1:
		{
			SetPlayerCheckpoint(playerid, garageInfo[gId][garageExterior][0], garageInfo[gId][garageExterior][1], garageInfo[gId][garageExterior][2], 5.0);
			playerStaticVar[playerid][pCheckpoint] = 1;
			
			va_SendClientMessage(playerid, -1, "Un checkpoint ti-a fost setat catre locatia garajului %d.", gId);
		}
		case 2:
		{
			Dialog_Show(playerid, DIALOG_GDESCRIPTION, DIALOG_STYLE_INPUT, "Garage Description:", "Please insert here your desired garage description:", "Set", "Exit");
		}
		case 3:
		{

			new garageText[512];
			format(garageText, sizeof(garageText), 
				"Item\t\tStatus\t\tprice\n\
				Mecanic\t\t%s\t\t"COLOR_SV_HEX"120{ffffff} premium points\n\
				Benzinarie\t\t%s\t\t"COLOR_SV_HEX"90{ffffff} premium points\n\
				", garageInfo[gId][garageMechanic] == 1 ? "{00ff00}unlocked{ffffff}" : "{990000}locked{ffffff}",garageInfo[gId][garageGasStation] == 1 ? "{00ff00}unlocked{ffffff}" : "{990000}locked{ffffff}");

			Dialog_Show(playerid, DIALOG_GUPGRADE, DIALOG_STYLE_TABLIST_HEADERS, "Garage Upgrade", garageText, "Select", "Cancel");
		}
		case 4:
		{
			Dialog_Show(playerid, DIALOG_CHANGEGARAGE, DIALOG_STYLE_MSGBOX, "House Garage", "Esti sigur ca vrei sa modifici pozitia garajului?\nAceasta modificare te va costa 50 de premium points\nAtentie: Garajul se va crea in pozitia ta actuala. Iesirea se va face in directia in care te uiti acum", "Modifica", "Renunta");
		}
	}
	return true;
}
Dialog:DIALOG_GUPGRADE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	new garageIDD = houseInfo[playerInfo[playerid][pHouse]][houseGarage];

	switch(listitem) 
	{
		case 0:
		{
			if(garageInfo[garageIDD][garageMechanic])
				return sendError(playerid,"Deja detii acest upgrade al garajului."); 

			if(playerInfo[playerid][pPremiumPoints] < 120)
				return Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Not enough points", "You don't have enough premium points to do this.", "Close", "");


			garageInfo[garageIDD][garageMechanic] = 1;
			SendClientMessage(playerid, -1, "Upgrade-ul a fost cumparat. Mecanicul este pe drum!"); 

			mysql_format(SQL, returnString, sizeof returnString, "update `garages` set `Mechanic` = '%d' where `ID` = '%d';", garageInfo[garageIDD][garageMechanic], garageIDD);
			mysql_tquery(SQL, returnString, "", "");

		}

		case 1:
		{
			if(garageInfo[garageIDD][garageGasStation])
				return sendError(playerid,"Deja detii acest upgrade al garajului."); 

			if(playerInfo[playerid][pPremiumPoints] < 90)
				return Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Not enough points", "You don't have enough premium points to do this.", "Close", "");


			garageInfo[garageIDD][garageGasStation] = 1;

			mysql_format(SQL, returnString, sizeof returnString, "update `garages` set `GasStation` = '%d' where `ID` = '%d';", garageInfo[garageIDD][garageGasStation], garageIDD);
			mysql_tquery(SQL, returnString, "", "");
			

		}
	}
	return true;
}

Dialog:DIALOG_GDESCRIPTION(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	if(isnull(inputtext) || strlen(inputtext) > 30)
		return Dialog_Show(playerid, DIALOG_GDESCRIPTION, DIALOG_STYLE_INPUT, "Garage Description:", "Please insert here your desired garage description:", "Set", "Exit");
	
	va_SendClientMessage(playerid, -1, "Garage name updated to: %s", inputtext);

	new gId = houseInfo[playerInfo[playerid][pHouse]][houseGarage];
	format(garageInfo[gId][garageDesc], 30, inputtext);

	new description[30];
	strcpy(description, inputtext);

	mysql_format(SQL, returnString, sizeof returnString, "update `garages` set `Description` = '%s' where `ID` = '%d';", description, gId);
	mysql_tquery(SQL, returnString, "", "");
	
	return WhenGarageGotUpdated(gId);
}

Dialog:DIALOG_CHANGEGARAGE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	if(playerInfo[playerid][pPremiumPoints] < 50)
    	return sendError(playerid,"Nu ai suficiente premium points.");

    new Float:returnPos[4],
    	i = houseInfo[playerInfo[playerid][pHouse]][houseGarage];

    GetPlayerPos(playerid, returnPos[0], returnPos[1], returnPos[2]);
    garageInfo[i][garageExterior][0] = returnPos[0];
	garageInfo[i][garageExterior][1] = returnPos[1];
	garageInfo[i][garageExterior][2] = returnPos[2];

	GetPlayerFacingAngle(playerid, returnPos[3]);
	garageInfo[i][garageExterior][3] = returnPos[3];

	WhenGarageGotUpdated(i);

    SendClientMessage(playerid, -1, "Pozitia garajului a fost modificata cu succes. Ai platit 50 de premium points.");
    playerInfo[playerid][pPremiumPoints] -= 50;

    mysql_format(SQL, returnString, sizeof returnString, "update `users` set `PremiumPoints` = '%d' where `id` = '%d';", playerInfo[playerid][pPremiumPoints], playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");

    mysql_format(SQL, returnString, sizeof returnString, "update `garages` set `garageX` = '%f', `garageY` = '%f', `garageZ` = '%f', `garageA` = '%f' where `ID` = '%d';", returnPos[0], returnPos[1], returnPos[2], returnPos[3], i);
    mysql_tquery(SQL, returnString, "", "");
	return true;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
	new garageInformation = houseInfo[playerInfo[playerid][pHouse]][houseGarage];
	if(newkeys == KEY_FIRE)
	{
		if(IsPlayerInRangeOfPoint(playerid, 5.0, -269.2384, 2440.7954, 35.7444))
		{
			if(garageInfo[garageInformation][garageGasStation] < 1)
				return sendError(playerid,"Nu detii acest upgrade, foloseste [/garage > garage upgrade]."); 

			if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
				return true;

			if(GetPVarInt(playerid, "fuel_garage") > gettime())
				return va_SendClientMessage(playerid, COLOR_LIGHTRED, "Nu mai ai benzina in canistra din garaj, incearca in %d secunde.", GetPVarInt(playerid, "fuel_garage") - gettime());

			SendClientMessage(playerid, COLOR_RED, "Ai umplut rezervorul masinii.");
			vehicleInfo[GetPlayerVehicleID(playerid)][vehFuel] = 100;

			SetPVarInt(playerid, "fuel_garage", gettime() + 120);
			return true;
		}
		if(IsPlayerInRangeOfPoint(playerid, 5.0, -250.3831, 2427.2549, 35.7444))
		{
			if(garageInfo[garageInformation][garageMechanic] < 1)
				return sendError(playerid,"Nu detii acest upgrade, foloseste [/garage > garage upgrade].");

			if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
				return true;

			if(GetPVarInt(playerid, "repair_garage") > gettime())
				return va_SendClientMessage(playerid, COLOR_LIGHTRED, "Mecanicul tau este prea obosit pentru a-ti repara masina, incearca peste %d secunde.", GetPVarInt(playerid, "repair_garage") - gettime());

			SendClientMessage(playerid, COLOR_RED, "Mecanicul tau ti-a reparat vehiculul.");
			RepairVehicle(GetPlayerVehicleID(playerid));

			SetPVarInt(playerid, "repair_garage", gettime() + 120);
			return true;
		}
	}
	else if(newkeys == KEY_SECONDARY_ATTACK)
	{
		if(IsPlayerInRangeOfPoint(playerid, 1.5, -236.9346, 2442.7808, 35.7444) && GetPlayerVirtualWorld(playerid) != 0 && !IsPlayerInAnyVehicle(playerid))
		{
			playerStaticVar[playerid][inHouse] = garageInfo[GetPlayerVirtualWorld(playerid)][garageHouse];

			SetPlayerPos(playerid, houseInfo[garageInfo[GetPlayerVirtualWorld(playerid)][garageHouse]][houseInterior][0], houseInfo[garageInfo[GetPlayerVirtualWorld(playerid)][garageHouse]][houseInterior][1], houseInfo[garageInfo[GetPlayerVirtualWorld(playerid)][garageHouse]][houseInterior][2]);
			SetPlayerInterior(playerid, houseInfo[garageInfo[GetPlayerVirtualWorld(playerid)][garageHouse]][houseInteriorVar]);
			SetPlayerVirtualWorld(playerid, garageInfo[GetPlayerVirtualWorld(playerid)][garageHouse]);
			return true;
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

function:WhenGarageGotUpdated(garage)
{
	if(garageInfo[garage][garageLabel]) DestroyDynamic3DTextLabel(garageInfo[garage][garageLabel]);
	if(garageInfo[garage][garagePickup]) DestroyDynamicPickup(garageInfo[garage][garagePickup]);
	if(garageInfo[garage][garageSphere]) DestroyDynamicArea(garageInfo[garage][garageSphere]);

	format(returnString, sizeof returnString, "Garage "COLOR_SV_HEX"%d{ffffff}\nDescription "COLOR_SV_HEX"%s{ffffff}\nOwner: "COLOR_SV_HEX"%s{ffffff}\nStatus: %s{FFFFFF}\nType /enter", garage, garageInfo[garage][garageDesc], houseInfo[garageInfo[garage][garageHouse]][houseOwnerName], (garageInfo[garage][garageLocked] ? ("{1BED30}Unlocked") : ("{ED3D22}Locked")));
	garageInfo[garage][garageLabel] = CreateDynamic3DTextLabel(returnString, -1, garageInfo[garage][garageExterior][0], garageInfo[garage][garageExterior][1], garageInfo[garage][garageExterior][2], 25);

	garageInfo[garage][garagePickup] = CreateDynamicPickup(1083, 23, garageInfo[garage][garageExterior][0], garageInfo[garage][garageExterior][1], garageInfo[garage][garageExterior][2], -1, -1, -1, 60);
	garageInfo[garage][garageSphere] = CreateDynamicSphere(garageInfo[garage][garageExterior][0], garageInfo[garage][garageExterior][1], garageInfo[garage][garageExterior][2], 1.5, 0, 0, -1);
	
	Streamer_SetIntData(STREAMER_TYPE_AREA, garageInfo[garage][garageSphere], E_STREAMER_EXTRA_ID, (garage + STREAMER_BEGIN_GARAGE));
	return true;
}

function:CreateGarage(house, Float:X, Float:Y, Float:Z, Float:A)
{
    returnServerGarages ++;
    new i = returnServerGarages;

    garageInfo[i][garageID] = cache_insert_id();

    garageInfo[i][garageHouse] = house;
    houseInfo[house][houseGarage] = i;

    format(garageInfo[i][garageDesc], 30, ""SERVER_NAME2" Garage");

    garageInfo[i][garageExterior][0] = X;
    garageInfo[i][garageExterior][1] = Y;
    garageInfo[i][garageExterior][2] = Z;
    garageInfo[i][garageExterior][3] = A;

    mysql_format(SQL, returnString, sizeof returnString, "update `houses` set `Garage` = '%d' where `id` = '%d';", i, house);
    mysql_tquery(SQL, returnString, "", "");

    WhenGarageGotUpdated(i);
    return true;
}

hook OnGameModeInit()
{
	CreateDynamic3DTextLabel("Garage Fuel\nPress "COLOR_SV_HEX"CLICK{ffffff} to refill your vehicle", -1, -269.2384, 2440.7954, 35.7444, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, false, -1);
	CreateDynamic3DTextLabel("Garage Mechanic\nPress "COLOR_SV_HEX"CLICK{ffffff} to repair your vehicle", -1, -250.3831, 2427.2549, 35.7444, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, false, -1);

	CreateDynamic3DTextLabel("House Entrance\nPress "COLOR_SV_HEX"ENTER{ffffff} to enter in your house", -1, -236.9346, 2442.7808, 35.7444, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, false, -1);
	CreateDynamic3DTextLabel("Garage Heal\nType "COLOR_SV_HEX"/heal{ffffff} to get health", -1, -235.3070, 2438.1167, 35.7444, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, false, -1);
	CreateDynamic3DTextLabel("Garage Exit\nType "COLOR_SV_HEX"/exit{ffffff} to exit from garage", -1, -235.8259, 2529.7288 - 100.0, 35.7520, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, false, -1);

	CreateDynamicObject(8040, -273.30, 2525.27 - 100.0, 35.51, 0.00, 0.00, 0.00);
	CreateDynamicObject(19453, -272.89, 2538.47 - 100.0, 36.49, 0.00, 0.00, 0.00);
	CreateDynamicObject(19453, -272.89, 2528.84 - 100.0, 36.49, 0.00, 0.00, 0.00);
	CreateDynamicObject(19453, -268.07, 2524.24 - 100.0, 36.49, 0.00, 0.00, 90.00);
	CreateDynamicObject(19453, -258.47, 2524.24 - 100.0, 36.49, 0.00, 0.00, 90.00);
	CreateDynamicObject(19453, -249.03, 2524.24 - 100.0, 36.49, 0.00, 0.00, 90.00);
	CreateDynamicObject(19355, -242.61, 2524.24 - 100.0, 36.49, 0.00, 0.00, 90.00);
	CreateDynamicObject(19355, -239.40, 2524.24 - 100.0, 36.49, 0.00, 0.00, 90.00);
	CreateDynamicObject(19355, -236.23, 2524.24 - 100.0, 36.49, 0.00, 0.00, 90.00);
	CreateDynamicObject(19355, -234.17, 2525.79 - 100.0, 36.49, 0.00, 0.00, 0.00);
	CreateDynamicObject(19355, -233.04, 2524.24 - 100.0, 36.49, 0.00, 0.00, 90.00);
	CreateDynamicObject(13028, -234.17, 2529.66 - 100.0, 36.34, 0.00, 0.00, 0.00);
	CreateDynamicObject(19355, -234.17, 2533.52 - 100.0, 36.49, 0.00, 0.00, 0.00);
	CreateDynamicObject(19355, -234.17, 2536.74 - 100.0, 36.49, 0.00, 0.00, 0.00);
	CreateDynamicObject(19355, -234.17, 2539.95 - 100.0, 36.49, 0.00, 0.00, 0.00);
	CreateDynamicObject(19859, -234.26, 2535.00 - 100.0, 35.93, 0.00, 0.00, 90.00);
	CreateDynamicObject(955, -234.49, 2538.10 - 100.0, 35.16, 0.00, 0.00, -90.00);
	CreateDynamicObject(1256, -234.66, 2540.27 - 100.0, 35.20, 0.00, 0.00, 0.00);
	CreateDynamicObject(1505, -237.67, 2543.19 - 100.0, 34.70, 0.00, 0.00, 0.00);
	CreateDynamicObject(15038, -234.64, 2534.58 - 100.0, 35.36, 0.00, 0.00, 0.00);
	CreateDynamicObject(15038, -234.64, 2533.80 - 100.0, 35.36, 0.00, 0.00, 0.00);
	CreateDynamicObject(15038, -234.64, 2536.95 - 100.0, 35.36, 0.00, 0.00, 0.00);
	CreateDynamicObject(3660, -262.82, 2525.20 - 100.0, 37.23, 0.00, 0.00, 0.00);
	CreateDynamicObject(1568, -254.59, 2526.28 - 100.0, 34.91, 0.00, 0.00, 0.00);
	CreateDynamicObject(1568, -270.40, 2526.28 - 100.0, 34.91, 0.00, 0.00, 0.00);
	CreateDynamicObject(1568, -262.38, 2526.28 - 100.0, 34.91, 0.00, 0.00, 0.00);
	CreateDynamicObject(1568, -234.69, 2533.12 - 100.0, 34.74, 0.00, 0.00, 0.00);
	CreateDynamicObject(2811, -235.83, 2542.84 - 100.0, 34.44, 0.00, 0.00, 0.00);
	CreateDynamicObject(2811, -238.03, 2542.84 - 100.0, 34.44, 0.00, 0.00, 0.00);
	CreateDynamicObject(1597, -248.08, 2525.20 - 100.0, 37.12, 0.00, 0.00, 90.00);
	CreateDynamicObject(1597, -241.47, 2525.20 - 100.0, 37.12, 0.00, 0.00, 90.00);
	CreateDynamicObject(19425, -239.62, 2541.51 - 100.0, 34.75, -180.28, -0.02, 90.00);
	CreateDynamicObject(638, -238.78, 2541.79 - 100.0, 35.16, 0.00, 0.00, 0.00);
	CreateDynamicObject(638, -238.78, 2539.11 - 100.0, 35.16, 0.00, 0.00, 0.00);
	CreateDynamicObject(19425, -239.62, 2538.27 - 100.0, 34.75, -180.28, -0.02, 90.00);
	CreateDynamicObject(19425, -239.62, 2535.03 - 100.0, 34.75, -180.28, -0.02, 90.00);
	CreateDynamicObject(19425, -238.25, 2533.12 - 100.0, 34.75, -180.28, -0.02, 180.00);
	CreateDynamicObject(19425, -235.00, 2533.12 - 100.0, 34.75, -180.28, -0.02, 180.00);
	CreateDynamicObject(1568, -239.60, 2542.64 - 100.0, 34.74, 0.00, 0.00, 0.00);
	CreateDynamicObject(3471, -238.78, 2536.74 - 100.0, 35.82, 0.00, 0.00, -90.00);
	CreateDynamicObject(19447, -234.09, 2529.08 - 100.0, 39.10, 0.00, 0.00, 0.00);
	CreateDynamicObject(19447, -234.07, 2529.54 - 100.0, 39.10, 0.00, 0.00, 0.00);
	CreateDynamicObject(19447, -237.94, 2524.22 - 100.0, 39.10, 0.00, 0.00, 90.00);
	CreateDynamicObject(19447, -247.56, 2524.22 - 100.0, 39.10, 0.00, 0.00, 90.00);
	CreateDynamicObject(19447, -257.17, 2524.22 - 100.0, 39.10, 0.00, 0.00, 90.00);
	CreateDynamicObject(19447, -266.80, 2524.22 - 100.0, 39.10, 0.00, 0.00, 90.00);
	CreateDynamicObject(19447, -276.43, 2524.22 - 100.0, 39.10, 0.00, 0.00, 90.00);
	CreateDynamicObject(19447, -272.95, 2529.14 - 100.0, 39.10, 0.00, 0.00, 0.00);
	CreateDynamicObject(19447, -272.95, 2538.76 - 100.0, 39.10, 0.00, 0.00, 0.00);
	CreateDynamicObject(19173, -236.92, 2543.16 - 100.0, 37.68, 0.00, 0.00, 0.00);
	CreateDynamicObject(17969, -244.60, 2543.10 - 100.0, 38.82, 0.00, 0.00, 90.00);
	CreateDynamicObject(10281, -272.03, 2536.20 - 100.0, 37.11, 0.00, 0.00, 90.00);
	CreateDynamicObject(2693, -271.93, 2526.98 - 100.0, 35.43, 0.00, 0.00, 130.20);
	CreateDynamicObject(19173, -272.78, 2529.89 - 100.0, 37.33, 0.00, 0.00, 90.00);
	CreateDynamicObject(3465, -271.20, 2542.39 - 100.0, 36.04, 0.00, 0.00, 0.00);
	CreateDynamicObject(3465, -271.20, 2540.86 - 100.0, 36.04, 0.00, 0.00, 0.00);
	CreateDynamicObject(19903, -271.25, 2539.50 - 100.0, 34.74, 0.00, 0.00, -27.84);
	CreateDynamicObject(638, -271.21, 2541.61 - 100.0, 35.22, 0.00, 0.00, 0.00);
	CreateDynamicObject(970, -271.67, 2538.55 - 100.0, 35.20, 0.00, 0.00, 0.00);
	CreateDynamicObject(970, -270.65, 2543.13 - 100.0, 35.20, 0.00, 0.00, 0.00);
	CreateDynamicObject(970, -272.71, 2540.63 - 100.0, 35.20, 0.00, 0.00, 90.00);
	CreateDynamicObject(1568, -269.39, 2538.50 - 100.0, 34.73, 0.00, 0.00, 0.00);
	CreateDynamicObject(19552, -283.07, 2503.43 - 100.0, 40.86, 0.00, 0.00, 0.00);
	return Y_HOOKS_CONTINUE_RETURN_1;
}