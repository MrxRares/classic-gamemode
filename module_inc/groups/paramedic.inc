#include <YSI_Coding\y_hooks>

#define check_medic if(playerInfo[playerid][pGroup] != 14) return true;
#define check_ambulance if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER || GetPlayerState(playerid) == PLAYER_STATE_DRIVER && factionVehInfo[serverVehicleId[GetPlayerVehicleID(playerid)]][vehGroup] != GROUP_TYPE_PARAMEDIC) return sendError(playerid,"Nu esti intr-un vehicul de factiune pentru solicitarea acestei actiuni.");

YCMD:mcalls(playerid, params[], help)
{
	check_medic
	check_duty_status

	SendClientMessage(playerid, COLOR_CYAN, "** Active medic calls **");

	foreach(new i : medicCallers)
	{
		va_SendClientMessage(playerid, -1, "%s (%d) - Distance: %.0fm", GetName(i), i, GetDistanceBetweenPlayers(i, playerid));
	}
	SendClientMessage(playerid, -1, "Type /accept medic [id] to accept a medic call.");
	return true;
}


/*YCMD:heal(playerid, params[], help)
{
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER && factionVehInfo[serverVehicleId[GetPlayerVehicleID(playerid)]][vehGroup] == GROUP_TYPE_PARAMEDIC)
	{
		if(playerInfo[playerid][pGroup] == GROUP_TYPE_PARAMEDIC && playerStaticVar[playerid][groupDuty])
		{
			new returnPlayer, returnMoney;
			if(sscanf(params, "ud", returnPlayer, returnMoney))
				return sendSyntaxMessage(playerid, "/heal [playerid/playername] [amount]");

			if(returnMoney < 1 || returnMoney > 2000)
				return sendError(playerid,"Amount must be between 1 and 2000.");

			if(GetPlayerVehicleID(playerid) != GetPlayerVehicleID(returnPlayer))
				return sendError(playerid,"Nu esti in masina cu acel jucator.");


		    if(GetMoney(returnPlayer) <  returnMoney && GetMilliards(returnPlayer) < 1)
		        return sendError(playerid,"This player doesn't have enough money.");

			SetPlayerHealth(returnPlayer, 99.9);

			va_SendClientMessage(returnPlayer, COLOR_LIGHTBLUE, "You got heal for $%s.", FormatNumber(returnMoney));
			va_SendClientMessage(playerid, COLOR_LIGHTBLUE, "You healed %s for $%s.", GetName(returnPlayer), FormatNumber(returnMoney));

			insertRaportPoint(playerid);
			return true;
		}
	}
	return true;
}*/