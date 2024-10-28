#include <YSI_Coding\y_hooks>

new spawnedVehicle[MAX_PLAYERS];
new Float:vehPickupLocation[][3] =
{
	{1540.4248, -1663.7018, 13.5506},
	{634.1819, -578.0692, 16.3359},
	{162.5644, 1908.7076, 18.6073},
	{},
	{},
	{},
	{943.5558, 1737.5182, 8.8516},
	{2287.0776, 2432.2104, 10.8203},
	{-307.8939, 1538.8119, 75.5625},
	{},
	{1059.6918, -345.6928, 73.9922},
	{2427.3235, 1662.9773, 10.8203},
	{1752.6995, -1894.1465, 13.5574},
	{1607.3774, 1815.4388, 10.8203}
};


YCMD:sfv(playerid, params[], help)
{
	if(playerStaticVar[playerid][groupDuty] == false) 
		return sendError(playerid,"You need to be ON-DUTY to do this.");
	check_queries

	if(GetPVarInt(playerid, "vehicle_spawn_delay") > gettime())
		return va_SendClientMessage(playerid, COLOR_GREY, "Nu poti spawna un vehicul de factiune timp de %d secunde.", GetPVarInt(playerid, "vehicle_spawn_delay") - gettime());

	new gId = playerInfo[playerid][pGroup] - 1;
	if(IsPlayerInRangeOfPoint(playerid, 5.0, vehPickupLocation[gId][0], vehPickupLocation[gId][1], vehPickupLocation[gId][2]))
	{
		mysql_format(SQL, returnString, sizeof returnString, "select `ID`, `modelId`, `vehRank` from `vehicles` where `groupId` = '%d' and `vehRank` <= %d order by `vehRank` asc;", playerInfo[playerid][pGroup], playerInfo[playerid][pGroupRank]);
		mysql_tquery(SQL, returnString, "show_player_faction_vehicles", "i", playerid);
	}
	return true;
}

function:show_player_faction_vehicles(playerid)
{
	if(!cache_num_rows())
		return true;

	new dialogString[518] = "Model\tRank\tStock\n";
	for(new i = 1; i <= cache_num_rows(); i++)
	{	
		static vehicleDBId, vehicleModel, vehicleRank;

		cache_get_value_name_int(i - 1, "ID", vehicleDBId);
		cache_get_value_name_int(i - 1, "modelId", vehicleModel);
		cache_get_value_name_int(i - 1, "vehRank", vehicleRank);

		format(dialogString, sizeof dialogString, "%s%s (%d)\t%d\tUnlimited\n", dialogString, GetVehicleName(vehicleModel), vehicleModel, vehicleRank);
		playerStaticVar[playerid][pDialogItems][i] = vehicleDBId;
	}
	Dialog_Show(playerid, DIALOG_SFV, DIALOG_STYLE_TABLIST_HEADERS, "Vehicles", dialogString, "Spawn", "Cancel");
	return true;
}

Dialog:DIALOG_SFV(playerid, response, listitem, inputtext)
{
	if(!response)
		return true;

	mysql_format(SQL, returnString, sizeof returnString, "select * from `vehicles` where `id` = '%d';", playerStaticVar[playerid][pDialogItems][listitem + 1]);
	mysql_tquery(SQL, returnString, "spawn_faction_vehicle", "i", playerid);
	return true;
}

function:spawn_faction_vehicle(playerid)
{
	if(!cache_num_rows())
		return true;

	new vehicleId = Iter_Count(vehicleType<VEH_TYPE_GROUP>) + 1;

	static vehicleModel, vehicleGroup, vehicleRank, vehColor1, vehColor2, Float:posX, Float:posY, Float:posZ, Float:posA;

	cache_get_value_name_int(0, "modelId", vehicleModel);
	cache_get_value_name_int(0, "groupId", vehicleGroup);
	cache_get_value_name_int(0, "vehRank", vehicleRank);
	cache_get_value_name_int(0, "color1", vehColor1);
	cache_get_value_name_int(0, "color2", vehColor2);

	cache_get_value_name_float(0, "posX", posX);
	cache_get_value_name_float(0, "posY", posY);
	cache_get_value_name_float(0, "posZ", posZ);
	cache_get_value_name_float(0, "posA", posA);

	va_SendClientMessage(playerid, -1, "Vehicle %s (modelID: %d) spawned (vehicleRank: %d). Please follow the yellow marker to get to it.", GetVehicleName(vehicleModel), vehicleModel, vehicleRank);
	spawnedVehicle[playerid] = AddStaticVehicleEx(vehicleModel, posX, posY, posZ, posA, vehColor1, vehColor2, 30, false, VEH_TYPE_GROUP);

	vehicleInfo[vehicleId][vehGroup] = vehicleGroup;
	vehicleInfo[vehicleId][vehRank] = vehicleRank;

	serverVehicleId[spawnedVehicle[playerid]] = vehicleId;

	new plateString[20];
	format(plateString, sizeof plateString, "{ff0000}Faction %d", vehicleGroup);
	
	SetVehicleNumberPlate(spawnedVehicle[playerid], plateString);
	SetVehicleHealth(spawnedVehicle[playerid], 1500.0);

	if(vehicleModel == 411 && factionType[vehicleGroup] == FACTION_TYPE_POLICE)
	{
		vehicleInfo[vehicleId][objID] = CreateDynamicObject(19419, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		AttachDynamicObjectToVehicle(vehicleInfo[vehicleId][objID], spawnedVehicle[playerid], 0.010000, -0.060000, 0.689999, 0.000000, 0.000000, 0.000000);
	}
	else if(vehicleModel == 541 && factionType[vehicleGroup] == FACTION_TYPE_POLICE)
	{
		vehicleInfo[vehicleId][objID] = CreateDynamicObject(19620, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		AttachDynamicObjectToVehicle(vehicleInfo[vehicleId][objID], spawnedVehicle[playerid], 0.010000, -0.060000, 0.659999, 0.000000, 0.000000, 0.000000);
	}
	else if(vehicleModel == 560 && vehicleGroup == 13)
	{
		vehicleInfo[vehicleId][objID] = CreateDynamicObject(19308, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		AttachDynamicObjectToVehicle(vehicleInfo[vehicleId][objID], spawnedVehicle[playerid], 0.000000, -0.249999, 0.939999, 0.000000, 0.000000, 0.000000);
	}
	else if(vehicleModel == 411 && vehicleGroup == 14)
	{
		vehicleInfo[vehicleId][objID] = CreateDynamicObject(11701, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		AttachDynamicObjectToVehicle(vehicleInfo[vehicleId][objID], spawnedVehicle[playerid], 0.010000, -0.060000, 0.719999, 0.000000, 0.000000, 0.000000);
	}

	SetPVarInt(playerid, "vehicle_spawn_delay", gettime() + 120);
	return true;
}

timer update_faction_vehicle[15000](vehicleid)
{
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, false);
	return true;
}

hook OnVehicleSpawn(vehicleid)
{
	if(!Iter_Contains(vehicleType<VEH_TYPE_GROUP>, vehicleid))
		return Y_HOOKS_CONTINUE_RETURN_1;

	new i = serverVehicleId[vehicleid];
	if(vehicleInfo[i][vehGroup] != 4 && vehicleInfo[i][vehGroup] != 5 && vehicleInfo[i][vehGroup] != 6 && vehicleInfo[i][vehGroup] != 10)
	{
		DestroyDynamicObject(vehicleInfo[i][objID]);
	    DestroyVehicle(vehicleid);

	    vehicleInfo[i][vehModel] = 0;
	    vehicleInfo[i][vehGroup] = 0;

	    vehicleInfo[i][vehRank] = 0;
	    vehicleInfo[i][objID] = 0;

	    serverVehicleId[vehicleid] = 0;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnVehicleStreamIn(vehicleid, forplayerid)
{
	if(vehicleid != spawnedVehicle[forplayerid])
		return Y_HOOKS_CONTINUE_RETURN_1;

	SetVehicleParamsForPlayer(vehicleid, forplayerid, 1, 0);
	defer update_faction_vehicle(vehicleid);

	spawnedVehicle[forplayerid] = INVALID_VEHICLE_ID;
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnGameModeInit()
{
	new factionString[128];
	for(new i; i < sizeof vehPickupLocation; i++)
	{
		new groupName[36];
		switch(i)
		{
			case 0: groupName = "Los Santos Police Department";
			case 1: groupName = "Federal Bureau Of Investigations";
			case 2: groupName = "National Guard";
			case 6: groupName = "Taxi Las Venturas";
			case 7: groupName = "Las Venturas Police Department";
			case 8: groupName = "News Reporters";
			case 10: groupName = "Hitman Agency";
			case 11: groupName = "School Instructors LV";
			case 12: groupName = "Taxi Los Santos";
			case 13: groupName = "Paramedic Department LV";
		}
		format(factionString, sizeof factionString, ""COLOR_SV_HEX"Faction:{ffffff} %s\nType (/sfv) to spawn a vehicle", groupName);
		
		CreateDynamic3DTextLabel(factionString, -1, vehPickupLocation[i][0], vehPickupLocation[i][1], vehPickupLocation[i][2], 25.0, INVALID_PLAYER_ID, INVALID_PLAYER_ID, true, 0);
		CreateDynamicPickup(19133, 23, vehPickupLocation[i][0], vehPickupLocation[i][1], vehPickupLocation[i][2], 0);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	if(ispassenger || !Iter_Contains(vehicleType<VEH_TYPE_GROUP>, vehicleid))
		return Y_HOOKS_CONTINUE_RETURN_1;

	new i = serverVehicleId[vehicleid];

	if(vehicleInfo[i][vehGroup] && vehicleInfo[i][vehGroup] == playerInfo[playerid][pGroup] && !playerStaticVar[playerid][groupDuty])
		SendClientMessage(playerid, COLOR_GREY, "Nu poti conduce acest vehicul deoarece nu esti la datorie.");

	else if(vehicleInfo[i][vehGroup] && vehicleInfo[i][vehGroup] == playerInfo[playerid][pGroup] && playerInfo[playerid][pGroupRank] < vehicleInfo[i][vehRank])
		va_SendClientMessage(playerid, COLOR_GREY, "Aceasta masina poate fi condusa doar de membrii cu rank %d+.", vehicleInfo[i][vehRank]);

	else if(vehicleInfo[i][vehGroup] != playerInfo[playerid][pGroup])
		va_SendClientMessage(playerid, COLOR_GREY, "Aceasta masina poate fi condusa doar de membrii %s.", factionName(vehicleInfo[i][vehGroup]));

	else if(playerInfo[playerid][pGroup] && vehicleInfo[i][vehGroup] == playerInfo[playerid][pGroup] && groupInfo[playerInfo[playerid][pGroup]][warActive] && GetVehicleModel(vehicleid) == 487)
		return sendError(playerid,"Nu poti conduce acest vehicul in timpul war-ului.");

	else
		return true;

	new Float:posX, Float:posY, Float:posZ;

	GetPlayerPos(playerid, posX, posY, posZ);
	SetPlayerPos(playerid, posX, posY, posZ + 1.5);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
	if(newstate != PLAYER_STATE_DRIVER || !Iter_Contains(vehicleType<VEH_TYPE_GROUP>, GetPlayerVehicleID(playerid)))
		return Y_HOOKS_CONTINUE_RETURN_1;

	if(playerStaticVar[playerid][enterToVehicle] != GetPlayerVehicleID(playerid) && !playerInfo[playerid][pAdmin])
	{
		sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_RED, "%s [%d] (level %d) got kicked by AdmBot for trying to enter in faction vehicles with cheats.", GetName(playerid), playerid, GetPlayerScore(playerid));
		SendClientMessage(playerid, -1, "You were kicked for using NOP (enter in faction vehicle with cheats).");
		KickEx(playerid);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}