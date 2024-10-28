#include <YSI_Coding\y_hooks>

new rentVehicleModels[] =
{
	412, 419, 522, 603, 560, 561, 495, 579, 401, 426, 482
};

vehRentType(const veh)
{
	switch(veh)
	{
		case 412: return 0;
		case 419: return 1;
		case 522: return 2;
		case 603: return 3;
		case 560: return 4;
		case 561: return 5;
		case 495: return 6;
		case 579: return 7;
		case 401: return 8;
		case 426: return 9;
		case 486: return 10;
	}
	return -1;
}

new rentVehStock[12], playerRentVehicle[MAX_PLAYERS], rentVehModel[MAX_VEHICLES], Iterator: playerRentVeh<MAX_PLAYERS>;

hook OnGameModeInit()
{
	for(new i; i < 11; i++)
		rentVehStock[i] = 15;

	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid)
{
	playerRentVehicle[playerid] = 0;
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEnterDynArea(playerid, areaid)
{
	new business_action = Streamer_GetIntData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID) - STREAMER_BEGIN_BUSINESS_SPHERE;
	if(returnServerBusinesses < business_action || business_action < 0)
		return true;

	if(bizInfo[business_action][bizType] == 14)
		SetPVarInt(playerid, "rent_veh_sphere", business_action);

	DisableRemoteVehicleCollisions(playerid, true);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerLeaveDynArea(playerid, areaid)
{
	DeletePVar(playerid, "rent_veh_sphere");
	DisableRemoteVehicleCollisions(playerid, false);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason)
{	
	if(Iter_Contains(playerRentVeh, playerid))
		Iter_Remove(playerRentVeh, playerid);

	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnVehicleSpawn(vehicleid)
{
	foreach(new i : playerRentVeh)
	{
		if(vehicleid == playerRentVehicle[i])
		{
			SendClientMessage(i, COLOR_RED, "Vehiculul tau inchiriat a fost distrus.");
			playerRentVehicle[i] = 0;
			break;
		}
	}
	if(vehicleInfo[vehicleid][vehRentable] == true)
	{
		rentVehStock[vehRentType(rentVehModel[vehicleid])] ++;
		DestroyVehicle(vehicleid);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:rentveh(playerid, params[], help)
{
	if(playerRentVehicle[playerid])
		return sendError(playerid,"Detii deja un vehicul inchiriat.");
	
	if(IsPlayerInAnyVehicle(playerid))
		return sendError(playerid,"Nu poti folosi aceasta comanda in timp ce esti intr-un vehicul.");

	if(GetPVarInt(playerid, "rent_veh_sphere") < 1)
		return sendError(playerid,"Nu esti in preajma unei afaceri de tip 'Rent vehicle'.");

    if(GetMoney(playerid) < 15000 && GetMilliards(playerid) < 1)
        return sendError(playerid,"Ai nevoie de minim $15.000 pentru a inchiria un vehicul personal.");

	returnString = "{FFFFFF}Vehicul\t{FFFFFF}Pe Stoc\n";
	for(new i; i < 10; i++)
	{	
		format(returnString, sizeof returnString, "%s\n%s\t%d vehicule disponibile", returnString, aVehicleNames[rentVehicleModels[i] - 400], rentVehStock[i]);
	}
	Dialog_Show(playerid, DIALOG_RENTVEH, DIALOG_STYLE_TABLIST_HEADERS, "Rent a vehicle", returnString, "Select", "Cancel");
	return true;
}

Dialog:DIALOG_RENTVEH(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	if(rentVehStock[listitem] == 0)
		return sendError(playerid,"Acest vehicul nu poate fi inchiriat deoarece nu se afla in stock-ul afacerii.");

    if(GetMoney(playerid) < 15000 && GetMilliards(playerid) < 1)
        return sendError(playerid,"Ai nevoie de minim $15.000 pentru a inchiria un vehicul personal.");

	SendClientMessage(playerid, -1, "Ai inchiriat acest vehicul contra sumei de 15,000$. O poti incuia folosind comanda /lock sau tasta N.");
	SendClientMessage(playerid, -1, "Vehiculul se va despawna la 3 minute de nefolosinta.");
	SendClientMessage(playerid, COLOR_RED, "Ai grija sa nu-ti fure cineva masina! Tu ii vei plati calatoria!");

	new Float:pos[3];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

	playerRentVehicle[playerid] = AddStaticVehicleEx(rentVehicleModels[listitem], pos[0], pos[1], pos[2], 90, -1, -1, 180);
	PutPlayerInVehicle(playerid, playerRentVehicle[playerid], 0);

	vehicleInfo[playerRentVehicle[playerid]][vehRentable] = true;
	Iter_Add(playerRentVeh, playerid);
	rentVehModel[playerRentVehicle[playerid]] = rentVehicleModels[listitem];
	rentVehStock[listitem] --;

	substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 15000);
	if(GetPVarInt(playerid, "rent_veh_sphere") > 0)
	{
		bizInfo[GetPVarInt(playerid, "rent_veh_sphere")][bizMoney] += 15000;

		mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `bizMoney` = '%d' where `id` = '%d';", bizInfo[GetPVarInt(playerid, "rent_veh_sphere")][bizMoney], GetPVarInt(playerid, "rent_veh_sphere"));
		mysql_tquery(SQL, returnString, "", "");
	}
	return true;
}

YCMD:unrentveh(playerid, params[], help)
{
	if(!playerRentVehicle[playerid])
		return sendError(playerid,"Nu ai un vehicul inchiriat.");

	foreach(new i : playersInVehicle)
	{
		if(IsPlayerInVehicle(playerid, playerRentVehicle[playerid]))
		{
			SendClientMessage(playerid, COLOR_GREY, "Jucatorul care a inchiriat vehiculul in care te aflai a incheiat contractul de inchirere.");
		}
	}

	SendClientMessage(playerid, COLOR_GREY, "Ai renuntat la vehiculul tau inchiriat.");
	DestroyVehicle(playerRentVehicle[playerid]);

	rentVehStock[vehRentType(rentVehModel[playerRentVehicle[playerid]])] ++;
	playerRentVehicle[playerid] = 0;

	Iter_Remove(playerRentVeh, playerid);
	return true;
}