#include <YSI_Coding\y_hooks>

#define check_taxi if(playerInfo[playerid][pGroup] != 13) return true;
#define check_taxi_vehicle if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER || GetPlayerState(playerid) == PLAYER_STATE_DRIVER && vehicleInfo[serverVehicleId[GetPlayerVehicleID(playerid)]][vehGroup] != GROUP_TYPE_TAXI) return sendError(playerid,"Nu esti intr-un vehicul de factiune pentru solicitarea acestei actiuni.");

//#error taxi.inc

enum ENUM_TRANSPORT_INFORMATIONS
{
	taxiFare, taxiDriver, transportMoney, totalEarnings
};
new transport_data[MAX_PLAYERS][ENUM_TRANSPORT_INFORMATIONS];
static const reset_transport_data[ENUM_TRANSPORT_INFORMATIONS];

new Timer:taxi_timer[MAX_PLAYERS];
hook OnPlayerConnect(playerid)
{
	transport_data[playerid] = reset_transport_data;
	transport_data[playerid][taxiDriver] = INVALID_PLAYER_ID;

	playerTextdraws[playerid][transportTD] = CreatePlayerTextDraw(playerid, 40.599983, 321.813416, "     Transport cost: ~w~~g~10000$");
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][transportTD], 0.140000, 1.250000);
	PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][transportTD], 1280.000000, 1280.000000);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][transportTD], 1);
	PlayerTextDrawColour(playerid, playerTextdraws[playerid][transportTD], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][transportTD], 0);
	PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][transportTD], 0x80808080);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][transportTD], 0);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][transportTD], 1);
	PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][transportTD], 0x00000033);
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][transportTD], 2);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][transportTD], 1);
	PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][transportTD], 0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

timer taxi_checks[5000](playerid)
{
	new countPassengers = 0;
	foreach(new i : playersInVehicle)
	{
		if(transport_data[i][taxiDriver] != playerid)
			continue;

		if(GetMoney(i) <  transport_data[playerid][taxiFare] && GetMilliards(i) < 1)
		{
			SendClientMessage(i, COLOR_GREY, "Deoarece ai ramas fara bani, nu mai poti beneficia de serviciile companiei de taximetrie.");

			new Float:posX, Float:posY, Float:posZ;

			GetPlayerPos(playerid, posX, posY, posZ);
			SetPlayerPos(playerid, posX, posY, posZ + 1.5);
			return true;
		}
		substractMoney(i, GetMilliards(i), GetMoney(i), 0, transport_data[playerid][taxiFare]);

		transport_data[i][transportMoney] += transport_data[playerid][taxiFare];

		va_PlayerTextDrawSetString(i, playerTextdraws[i][transportTD], "Transport cost: ~w~~g~%d$", transport_data[i][transportMoney]);
		PlayerTextDrawShow(i, playerTextdraws[i][transportTD]);

		if(transport_data[playerid][totalEarnings])
		{
			va_PlayerTextDrawSetString(playerid, playerTextdraws[playerid][transportTD], "     Money earned: ~w~~g~%d$", transport_data[i][transportMoney] + transport_data[playerid][totalEarnings]);
			PlayerTextDrawShow(playerid, playerTextdraws[playerid][transportTD]);
		}

		countPassengers ++;
		if(countPassengers >= 3)
			break;
	}

	if(!countPassengers)
		stop taxi_timer[playerid];

	else
		taxi_timer[playerid] = defer taxi_checks(playerid);

	return true;
}

YCMD:fare(playerid, params[], help)
{
	check_taxi
	check_duty_status

	check_taxi_vehicle

	if(transport_data[playerid][taxiFare])
	{
		va_SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You are now Off Duty and earned $%d.", transport_data[playerid][totalEarnings]);
		transport_data[playerid][taxiFare] = 0;
		return true;
	}

	new returnMoney;
	if(sscanf(params, "i", returnMoney))
		return sendSyntaxMessage(playerid, "/fare [amount]");

	if(returnMoney < 0 || returnMoney > 2000)
		return sendError(playerid,"Fare must be between 1 and 2000.");

	va_SendClientMessageToAll(0x00D900C8, "* Taxi driver %s is now on duty ($%d). [/service taxi]", GetDBName(playerid), returnMoney);
	transport_data[playerid][taxiFare] = returnMoney;
	return true;
}

YCMD:tcalls(playerid, params[], help)
{
	check_taxi
	check_duty_status

	SendClientMessage(playerid, COLOR_CYAN, "** Active taxi calls **");

	foreach(new i : taxiCallers)
	{
		va_SendClientMessage(playerid, -1, "%s (%d) - Distance: %.0fm", GetName(i), i, GetDistanceBetweenPlayers(i, playerid));
	}
	SendClientMessage(playerid, -1, "Type /accept taxi [id] to accept a taxi call.");
	return true;
}

YCMD:tx(playerid, params[], help)
{
	if(playerInfo[playerid][pGroup] != GROUP_TYPE_TAXI)
		return sendError(playerid,"You are not a Taxi member");

	new returnMessage[144];
	if(sscanf(params, "s[144]", returnMessage))
		return sendSyntaxMessage(playerid, "/tx <Message>");

	new returnCity[4];
	switch(playerInfo[playerid][pGroup])
	{
		case 13: returnCity = "SF";
		//+ if uri
	}

	sendGroupMessage(GROUP_TYPE_TAXI, COLOR_DCHAT, "# %s Rank %d: %s", returnCity, playerInfo[playerid][pGroupRank], returnMessage);
	return true;
}

hook OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
	if(newstate == PLAYER_STATE_PASSENGER)
	{
		foreach(new i : factionMembers<GROUP_TYPE_TAXI>)
		{
			if(GetPlayerVehicleID(i) != GetPlayerVehicleID(playerid) && GetPlayerVehicleSeat(i) != PLAYER_STATE_DRIVER)
				continue;

			if(!transport_data[i][taxiFare])
				continue;

			if(GetPlayerState(i) != PLAYER_STATE_DRIVER || GetPlayerState(i) == PLAYER_STATE_DRIVER && vehicleInfo[serverVehicleId[GetPlayerVehicleID(i)]][vehGroup] != GROUP_TYPE_TAXI)
				continue;

			if(GetMoney(i) <  transport_data[i][taxiFare] && GetMilliards(i) < 1)
			{
				va_SendClientMessage(i, COLOR_LIGHTBLUE, "* You need $%d to enter.", transport_data[playerid][taxiFare]);

				new Float:posX, Float:posY, Float:posZ;

				GetPlayerPos(playerid, posX, posY, posZ);
				SetPlayerPos(playerid, posX, posY, posZ + 1.5);
				return true;

			}
			addMoney(i, GetMilliards(i), GetMoney(i), 0, transport_data[i][taxiFare], false);

			substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, transport_data[i][taxiFare]);

			va_SendClientMessage(i, COLOR_LIGHTBLUE, "* Passenger %s has entered in your Taxi.", GetName(playerid));
			va_SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You paid %d to the Taxi Driver.", transport_data[i][taxiFare]);
			
			SendClientMessage(playerid, -1, "Pentru a putea seta o locatie de transport taximetristului plaseaza un waypoint pe mapa.");

			transport_data[playerid][taxiDriver] = i;
			transport_data[i][totalEarnings] += transport_data[i][taxiFare];

			stop taxi_timer[playerid];
			taxi_timer[playerid] = defer taxi_checks(i);
			break;
		}
		return Y_HOOKS_CONTINUE_RETURN_1;
	}
	if(newstate == PLAYER_STATE_ONFOOT)
	{
		new driverId = transport_data[playerid][taxiDriver];
		if(driverId == INVALID_PLAYER_ID)
			return Y_HOOKS_CONTINUE_RETURN_1;

		if(oldstate != PLAYER_STATE_PASSENGER || !transport_data[driverId][taxiFare])
			return Y_HOOKS_CONTINUE_RETURN_1;

		sendNearbyMessage(playerid, COLOR_PURPLE, "* %s gets off the cab and pays $%s to %s.", GetName(playerid), FormatNumber(transport_data[playerid][transportMoney]), GetName(driverId));

		PlayerTextDrawHide(playerid, playerTextdraws[playerid][transportTD]);

		addMoney(driverId, GetMilliards(driverId), GetMoney(driverId), 0, transport_data[playerid][transportMoney], false);


		transport_data[transport_data[playerid][taxiDriver]][totalEarnings] += transport_data[playerid][transportMoney];
		insertRaportPoint(transport_data[playerid][taxiDriver]);

		transport_data[playerid][taxiDriver] = INVALID_PLAYER_ID;
		transport_data[playerid][transportMoney] = 0;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	if(IsPlayerInVehicle(playerid, GetPlayerVehicleID(transport_data[playerid][taxiDriver])) && GetPlayerState(transport_data[playerid][taxiDriver]) == PLAYER_STATE_DRIVER)
	{
		if(!transport_data[transport_data[playerid][taxiDriver]][taxiFare])
			return Y_HOOKS_CONTINUE_RETURN_1;

		va_SendClientMessage(transport_data[playerid][taxiDriver], -1, "Clientul %s (%d) doreste sa-l transporti la o locatie pe care a marcat-o pe GPS. (distanta %.2fm).", GetName(playerid), playerid, GetPlayerDistanceFromPoint(playerid, fX, fY, fZ));
		SendClientMessage(playerid, -1, "Locatia marcata pe GPS a fost transmisa taximetristului.");

		SetPlayerCheckpoint(transport_data[playerid][taxiDriver], fX, fY, fZ, 5.0);
		playerStaticVar[transport_data[playerid][taxiDriver]][pCheckpoint] = 1;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}