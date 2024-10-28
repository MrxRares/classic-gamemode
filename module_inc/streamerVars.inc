#include <YSI_Coding\y_hooks>
new Iterator:streamedVehicles[MAX_PLAYERS]<MAX_VEHICLES>;
public OnVehicleStreamIn(vehicleid, forplayerid)
{
	Iter_Add(streamedVehicles[forplayerid], vehicleid);
	return true;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	Iter_Remove(streamedVehicles[forplayerid], vehicleid);
	return true;
}


public OnPlayerStreamOut(playerid, forplayerid)
{
	Iter_Remove(streamedPlayers[forplayerid], playerid);
	SetPlayerMarkerForPlayer(playerid, forplayerid, (GetPlayerColor(forplayerid) & 0xFFFFFF00));
	SetPlayerMarkerForPlayer(forplayerid, playerid, (GetPlayerColor(playerid) & 0xFFFFFF00));
	return true;
}

public OnActorStreamIn(actorid, forplayerid)
{
	return true;
}

public OnActorStreamOut(actorid, forplayerid)
{
	return true;
}

public OnPlayerEnterDynamicArea(playerid, areaid)
{
	return true;
}

public OnPlayerLeaveDynamicArea(playerid, areaid)
{
	return true;
}

hook OnGameModeInit()
{
	Streamer_ToggleChunkStream(0); //for not crashing with YSF plugin :)
	return Y_HOOKS_CONTINUE_RETURN_1;
}