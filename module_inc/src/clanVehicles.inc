#include <YSI_Coding\y_hooks>

#define MAX_CLAN_VEHICLES 1000
#define BuyOrder:: BuyOrder_

enum ENUM_CLAN_VEH_INFO
{
	vehModel, vehClan, vehRank, objID
};
new clanVehInfo[MAX_CLAN_VEHICLES][ENUM_CLAN_VEH_INFO];
new totalClanVehicles[MAX_CLANS], selectedVehicle[MAX_PLAYERS], ClanVehicleSQL[MAX_CLAN_VEHICLES];

function:showClanVehicles(playerid)
{
	returnString = "Model\tRank\n";

	new returnVID, returnVRank, returnVModel;
	for(new i; i < cache_num_rows(); i++)
	{
		cache_get_value_name_int(i, "ID", returnVID);

		cache_get_value_name_int(i, "modelID", returnVModel);
		cache_get_value_name_int(i, "clanRank", returnVRank);

		format(returnString, sizeof returnString, "%s%s\t%d\n", returnString, aVehicleNames[returnVModel - 400], returnVRank);

		playerStaticVar[playerid][pDialogItems][i] = returnVID;
	}
	totalClanVehicles[playerInfo[playerid][pClan]] = cache_num_rows();

	format(returnString, sizeof returnString, "%s%s", returnString, (playerInfo[playerid][pClanRank] > 5 ? "\nBuy a vehicle" : ""));
	Dialog_ShowPages(playerid, DIALOG_CLANVEHICLES, DIALOG_STYLE_TABLIST_HEADERS, "Manage Clan Vehicles", returnString, "Select", "Exit");
	return true;
}

BuyOrder::vehicleModel(listitem)
{
	switch(listitem)
	{
		case 0: return 411;
		case 1: return 482;
		case 2: return 551;
		case 3: return 560;
		case 4: return 541;
	}
	return -1;
}

BuyOrder::vehiclePrice(listitem)
{
	switch(listitem)
	{
		case 0: return 200179504;
		case 1: return 10425000;
		case 2: return 13125000;
		case 3: return 131040000;
		case 4: return 164730000;
	}
	return -1;
}

Dialog:DIALOG_CLANVEHICLES(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	if(listitem == totalClanVehicles[playerInfo[playerid][pClan]])
	{
		returnString = "Vehicul\tPret\n";
		for(new i; i < 5; i++)
		{
			format(returnString, sizeof returnString, "%s%s\t$%s\n", returnString, aVehicleNames[BuyOrder::vehicleModel(i) - 400], FormatNumber(BuyOrder::vehiclePrice(i)));
		}
		return Dialog_Show(playerid, DIALOG_BUY_CLAN_VEH, DIALOG_STYLE_TABLIST_HEADERS, "Manage Vehicles", returnString, "Select", "Exit");
	}
	selectedVehicle[playerid] = listitem;
	Dialog_Show(playerid, DIALOG_CLAN_VEH_SELECT, DIALOG_STYLE_LIST, "Manage Vehicle", "Seteaza rank\nVinde vehicul\nSpawneaza vehicul\nLocalizeaza vehicul", "Select", "Exit");
	return true;
}

GetClanVehicleGameID(dbid)
{
    foreach(new vehicleid : vehicleType<VEH_TYPE_CLAN>)
    {
        if(ClanVehicleSQL[vehicleid] == dbid)
        	return vehicleid;
    }
    return INVALID_VEHICLE_ID;
}

function:DestroyClanVehicle(vehicleGameID)
{
    DestroyDynamicObject(clanVehInfo[vehicleGameID][objID]);

    ClanVehicleSQL[vehicleGameID] = 0;
    DestroyVehicle(vehicleGameID);

    clanVehInfo[vehicleGameID][vehModel] = 0;
    clanVehInfo[vehicleGameID][vehClan] = 0;
    clanVehInfo[vehicleGameID][vehRank] = 0;
    clanVehInfo[vehicleGameID][objID] = 0;

    return true;
}

hook OnVehicleDeath(vehicleid, killerid)
{
	if(ClanVehicleSQL[vehicleid] && clanVehInfo[ClanVehicleSQL[vehicleid]][vehClan])
		sendClanMessage(clanVehInfo[ClanVehicleSQL[vehicleid]][vehClan], -1, "{%s}Vehiculul %s [db:%d] a fost distrus, $5000 au fost extrasi din seiful clanului.", clanInfo[clanVehInfo[ClanVehicleSQL[vehicleid]][vehClan]][clanColor], GetVehicleName(GetVehicleModel(vehicleid)), ClanVehicleSQL[vehicleid]);

	return Y_HOOKS_CONTINUE_RETURN_1;
}

Dialog:DIALOG_CLAN_VEH_RANK(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	if(strval(inputtext) < 1 || strval(inputtext) > 6)
		return sendError(playerid,"Invalid rank.");

	mysql_format(SQL, returnString, sizeof returnString, "update `clanvehicles` set `clanRank` = '%d' where `id` = '%d';", strval(inputtext), playerStaticVar[playerid][pDialogItems][selectedVehicle[playerid]]);
	mysql_tquery(SQL, returnString, "", "");

	new vehicleGameID = GetClanVehicleGameID(playerStaticVar[playerid][pDialogItems][selectedVehicle[playerid]]);

	if(IsValidVehicle(vehicleGameID))
		clanVehInfo[ClanVehicleSQL[vehicleGameID]][vehRank] = strval(inputtext);

	va_SendClientMessage(playerid, COLOR_GREY, "Ai setat rankul vehiculului %d la %d.", playerStaticVar[playerid][pDialogItems][selectedVehicle[playerid]], strval(inputtext));
	return true;
}

Dialog:DIALOG_CLAN_VEH_SELECT(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	switch(listitem)
	{
		case 0: Dialog_Show(playerid, DIALOG_CLAN_VEH_RANK, DIALOG_STYLE_INPUT, "Vehicle Rank", "Introdu rankul pe care doresti ca un membru al clanului sa-l aiba pentru a conduce acest vehicul:", "Set", "Exit");
		case 1:
		{
			SendClientMessage(playerid, -1, "Nu poti vinde acest vehicul.");
		}
		case 2:
		{
			check_queries

			new vehicleGameID = GetClanVehicleGameID(playerStaticVar[playerid][pDialogItems][selectedVehicle[playerid]]);

			if(vehicleGameID != INVALID_VEHICLE_ID)
				DestroyClanVehicle(vehicleGameID);

			mysql_format(SQL, returnString, sizeof returnString, "select * from `clanvehicles` where `ID` = '%d';", playerStaticVar[playerid][pDialogItems][selectedVehicle[playerid]]);
			mysql_tquery(SQL, returnString, "SpawnClanVehicle", "i", playerInfo[playerid][pClan]);

			SendClientMessage(playerid, -1, "Vehiculul a fost spawnat.");
		}
		case 3:
		{
			new vehicleGameID = GetClanVehicleGameID(playerStaticVar[playerid][pDialogItems][selectedVehicle[playerid]]),
				Float:vehiclePos[3];

			if(vehicleGameID == INVALID_VEHICLE_ID)
				return sendError(playerid,"Vehiculul trebuie sa fie spawnat pentru a-l putea spawna.");

			GetVehiclePos(vehicleGameID, vehiclePos[0], vehiclePos[1], vehiclePos[2]);
			SetPlayerCheckpoint(playerid, vehiclePos[0], vehiclePos[1], vehiclePos[2], 2.5);

			playerStaticVar[playerid][pCheckpoint] = 1;
		}
	}
	return true;
}

Dialog:DIALOG_BUY_CLAN_VEH(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	if(playerInfo[playerid][pClanRank] < 7)
		return sendError(playerid,"Numai liderul clanului are acces la aceasta sectiune.");

	if(GetClanSafeMoney(playerInfo[playerid][pClan]) < BuyOrder::vehiclePrice(listitem))
		return sendError(playerid,"Clanul tau nu are suficienti bani pentru achizitionarea acestui vehicul.");

	SendClientMessage(playerid, COLOR_GREY, "Vehiculul a fost achizitionat. Il poti spawna din meniul clanului, categoria Clan Vehicles.");

	new queryString[524], Float:pos[3];
	setVehicleRandomLocation(random(27), pos[0], pos[1], pos[2]);

	mysql_format(SQL, queryString, sizeof queryString, "insert into `clanvehicles` (`modelID`, `clanID`, `posX`, `posY`, `posZ`, `posA`) values ('%d', '%d', '%f', '%f', '%f', '90.0');", BuyOrder::vehicleModel(listitem), playerInfo[playerid][pClan], pos[0], pos[1], pos[2]);
	mysql_tquery(SQL, queryString, "", "");
	return true;
}

function:SpawnClanVehicle(clanId)
{
	if(!cache_num_rows())
		return true;

	static vModel, vColor1, vColor2, Float:vPosX, Float:vPosY, Float:vPosZ, Float:vPosA;
	cache_get_value_name_int(0, "modelID", vModel);
	cache_get_value_name_int(0, "vehColor1", vColor1);
	cache_get_value_name_int(0, "vehColor2", vColor2);

	cache_get_value_name_float(0, "posX", vPosX);
	cache_get_value_name_float(0, "posY", vPosY);
	cache_get_value_name_float(0, "posZ", vPosZ);
	cache_get_value_name_float(0, "posA", vPosA);

	new returnClanVeh = AddStaticVehicleEx(vModel, vPosX, vPosY, vPosZ, vPosA, vColor1, vColor2, 300, false, VEH_TYPE_CLAN);
 	cache_get_value_name_int(0, "ID", ClanVehicleSQL[returnClanVeh]);

	new vehicleId = ClanVehicleSQL[returnClanVeh];

	clanVehInfo[vehicleId][vehClan] = clanId;
	cache_get_value_name_int(0, "clanRank", clanVehInfo[vehicleId][vehRank]);

	new clanString[48];
	format(clanString, sizeof clanString, "{%s}%s\n%s", clanInfo[clanId][clanColor], clanInfo[clanId][clanName], clanInfo[clanId][clanTag]);
	
	clanVehInfo[vehicleId][objID] = CreateDynamicObject(19327, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	SetDynamicObjectMaterialText(clanVehInfo[vehicleId][objID], 0, clanString, 140, "Arial", 65, false, -1, false, 1);
	
	switch(vModel)
	{
		case 411: AttachDynamicObjectToVehicle(clanVehInfo[vehicleId][objID], returnClanVeh, 0.000000, -1.899999, 0.250000, -90.000000, 0.000000, 0.000000);
		case 482: AttachDynamicObjectToVehicle(clanVehInfo[vehicleId][objID], returnClanVeh, -1.019999, -1.149999, 0.200000, 0.000000, 0.000000, -90.000000);
		case 551: AttachDynamicObjectToVehicle(clanVehInfo[vehicleId][objID], returnClanVeh, 0.000000, -2.599999, 0.390000, -90.000000, 0.000000, 0.000000);
		case 560: AttachDynamicObjectToVehicle(clanVehInfo[vehicleId][objID], returnClanVeh, 0.000000, -1.899999, 0.400000, -90.000000, 0.000000, 0.000000);
		case 541: AttachDynamicObjectToVehicle(clanVehInfo[vehicleId][objID], returnClanVeh, 0.000000, -1.700000, 0.400000, -90.000000, 0.000000, 0.000000);
	}
	return SetVehicleNumberPlate(returnClanVeh, clanInfo[clanId][clanTag]);
}

hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	if(ispassenger || !ClanVehicleSQL[vehicleid])
		return Y_HOOKS_CONTINUE_RETURN_1;

	new i = ClanVehicleSQL[vehicleid];
	if(clanVehInfo[i][vehClan] == playerInfo[playerid][pClan])
	{
		if(clanVehInfo[i][vehRank] <= playerInfo[playerid][pClanRank])
		{
			va_SendClientMessage(playerid, -1, "Poti conduce acest vehicul deparece apartii clanului %s [%s] si ai rank %d+", clanInfo[clanVehInfo[i][vehClan]][clanName], clanInfo[clanVehInfo[i][vehClan]][clanTag], clanVehInfo[i][vehRank]);
		}
		else
		{
			va_SendClientMessage(playerid, -1, "Nu poti conduce acest vehicul deparece nu ai rank %d+", clanVehInfo[i][vehRank]);
			ClearAnimations(playerid);
		}
	}
	else
	{
		va_SendClientMessage(playerid, -1, "Acest vehicul poate fi condus doar de catre jucatorii din clanul %s [%s]", clanInfo[clanVehInfo[i][vehClan]][clanName], clanInfo[clanVehInfo[i][vehClan]][clanTag]);
		ClearAnimations(playerid);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}