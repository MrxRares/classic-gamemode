#include <YSI_Coding\y_hooks>

new Iterator: racePlayers<MAX_PLAYERS>;

enum ENUM_PLAYER_RACE_DATA
{
	racePoints, racePosition[3]
}
new p_race_info[MAX_PLAYERS][ENUM_PLAYER_RACE_DATA];

hook OnGameModeInit()
{
	CreateDynamicObject(971, 1056.254639, 1006.975342, 59.957390, -0.600000, -0.420000, -295.380005, -1, -1, -1, 400.000000, 400.000000);
	CreateDynamicObject(971, 1057.307861, 1014.866394, 59.963261, -1.200000, -1.380000, -257.939941, -1, -1, -1, 400.000000, 400.000000);
	CreateDynamicObject(971, 1053.754639, 1021.244202, 59.927361, -1.680000, -0.720000, -234.240036, -1, -1, -1, 400.000000, 400.000000);
	CreateDynamicObject(971, 1051.888916, 1001.464172, 59.478519, 0.000000, 0.000000, 28.740000, -1, -1, -1, 400.000000, 400.000000);
	CreateDynamicObject(971, 1043.834473, 999.518799, 59.586151, -1.680000, 0.840000, -0.180000, -1, -1, -1, 400.000000, 400.000000);
	CreateDynamicObject(971, 1046.643311, 1025.146851, 59.919472, -0.360000, -0.420000, -183.720093, -1, -1, -1, 400.000000, 400.000000);
	CreateDynamicObject(971, 1036.589844, 1002.612000, 59.777821, -6.360000, -0.480000, -38.459999, -1, -1, -1, 400.000000, 400.000000);
	CreateDynamicObject(971, 1038.189087, 1023.890320, 59.579411, -1.080000, -0.900000, -153.900024, -1, -1, -1, 400.000000, 400.000000);
	CreateDynamicObject(971, 1031.729980, 1009.470581, 59.510201, -1.080000, -0.300000, -71.820007, -1, -1, -1, 400.000000, 400.000000);
	CreateDynamicObject(971, 1032.254395, 1017.817505, 59.458481, -1.620000, -0.420000, -112.800003, -1, -1, -1, 400.000000, 400.000000);
	CreateDynamicObject(16271, 908.113220, 917.340088, 26.421000, 0.000000, 0.000000, -162.000000, -1, -1, -1, 400.000000, 400.000000);
	CreateDynamicObject(3037, 902.191895, 902.867004, 15.046500, 0.000000, 0.000000, 17.500000, -1, -1, -1, 400.000000, 400.000000);
	CreateDynamicObject(1536, 905.372009, 893.052979, 12.865800, 0.000000, 0.000000, 108.000000, -1, -1, -1, 400.000000, 400.000000);
	CreateDynamicObject(18245, 921.553528, 842.369873, 24.342199, 0.000000, 0.000000, 0.000000, -1, -1, -1, 400.000000, 400.000000);
	CreateDynamicObject(3398, 930.409912, 815.424622, 25.565701, 0.000000, 0.000000, -142.000000, -1, -1, -1, 400.000000, 400.000000);
	CreateDynamicObject(18249, 887.312195, 827.958679, 18.587999, 0.000000, 0.000000, 90.000000, -1, -1, -1, 400.000000, 400.000000);
	CreateDynamicObject(16302, 869.926819, 841.538025, 8.772700, 0.000000, 0.000000, -105.000000, -1, -1, -1, 400.000000, 400.000000);
	CreateDynamicObject(970, 866.749084, 841.390320, 12.905000, 0.000000, 0.000000, 90.000000, -1, -1, -1, 400.000000, 400.000000);
	CreateDynamicObject(16076, 857.322510, 827.299927, 16.473301, 0.000000, 0.000000, 0.000000, -1, -1, -1, 400.000000, 400.000000);
	CreateDynamicObject(3398, 850.737671, 820.976013, 25.565701, 0.000000, 0.000000, 135.000000, -1, -1, -1, 400.000000, 400.000000);

	CreateDynamic3DTextLabel(""COLOR_SV_HEX"Race Arena\n{FFFFFF}Type "COLOR_SV_HEX"/race {FFFFFF}to participate in the race.", -1, 2695.6150, -1704.7894, 11.8438, 25.0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	WhenPlayerLeaveRace(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

GetRaceVehicle(vType, rPoints)
{
	if(vType == 0)
	{
		switch(rPoints)
		{
			case 0..10000: return 451;
			case 10001..20000: return 541;
			default: return 411;
		}
	}
	if(vType == 1)
	{
		switch(rPoints)
		{
			case 0..10000: return 480;
			case 10001..20000: return 439;
			default: return 533;
		}
	}
	if(vType == 2)
	{
		switch(rPoints)
		{
			case 0..10000: return 567;
			case 10001..20000: return 536;
			default: return 535;
		}
	}
	return false;
}

//myracestats
YCMD:racestats(playerid, params[], help)
{
	va_SendClientMessage(playerid, -1, "---------------------------[{066D8F}%s{FFFFFF}] race stats ----------------------------", GetDBName(playerid));
	va_SendClientMessage(playerid, -1, "Race Points: {066D8F}%d", p_race_info[playerid][racePoints]);
	va_SendClientMessage(playerid, -1, "Sport vehicle available: {066D8F}%s", GetVehicleName(GetRaceVehicle(0, p_race_info[playerid][racePoints])));
	va_SendClientMessage(playerid, -1, "Convertibles vehicle available: {066D8F}%s", GetVehicleName(GetRaceVehicle(1, p_race_info[playerid][racePoints])));
	va_SendClientMessage(playerid, -1, "Lowriders vehicle available: {066D8F}%s", GetVehicleName(GetRaceVehicle(2, p_race_info[playerid][racePoints])));
	//va_SendClientMessage(playerid, -1, "Race points needed for car upgrade: {066D8F}%s", FormatNumber(p_race_info[playerid][racePoints], ","));
	va_SendClientMessage(playerid, -1, "Race won on first position: {066D8F}%d", p_race_info[playerid][racePosition][0]);
	va_SendClientMessage(playerid, -1, "Race won on second position: {066D8F}%d", p_race_info[playerid][racePosition][0]);
	va_SendClientMessage(playerid, -1, "Race won on third position: {066D8F}%d", p_race_info[playerid][racePosition][0]);

	return true;
}

YCMD:race(playerid, params[], help)
{
	if(!IsPlayerInRangeOfPoint(playerid, 5.0, 2695.6150, -1704.7894, 11.8438))
		return sendError(playerid,"Nu te aflii la race arena");

	if(GetPlayerAnimationIndex(playerid) != 1189)
		return sendError(playerid,"Nu poti folosi aceasta comanda cat timp folosesti o animatie. Daca te aflii in miscare stai pe loc.");

	if(Iter_Count(racePlayers) >= 6)
		return sendError(playerid,"Nu puteti intra in arena deoarece s-au adunat deja 6 jucatori");

	//Nu puteti intra in arena deoarece cursa se afla in desfasurare

	Iter_Add(racePlayers, playerid);
	SS(playerid, -1, "Se asteapta intrarea mai multor participanti", "Waiting for more players.");
	
	foreach(new i : racePlayers)
		va_SendClientMessage(playerid, COLOR_DCHAT, "%s has joined the race arena", GetDBName(playerid));

	SetPlayerPos(playerid, 1045.5460, 1012.3243, 55.3047);
	TogglePlayerControllable(playerid, false);

	SetPlayerVirtualWorld(playerid, 1069);
	return true;
}
YCMD:leaverace(playerid, params[], help)
{
	if(!Iter_Contains(racePlayers, playerid))
		return sendError(playerid,"You're not in race");

	WhenPlayerLeaveRace(playerid);
	return true;
}

WhenPlayerLeaveRace(playerid)
{
	if(!Iter_Contains(racePlayers, playerid))
		return true;

	SetPlayerPos(playerid, 2695.6150, -1704.7894, 11.8438);
	TogglePlayerControllable(playerid, true);

	SetPlayerVirtualWorld(playerid, 0);
	Iter_Remove(racePlayers, playerid);
	return true;
}

YCMD:gotorace(playerid, params[], help)
{
	check_admin

	SetPlayerPos(playerid, 2695.6150, -1704.7894, 11.8438);
	SetPlayerVirtualWorld(playerid, 0);
	SetPlayerInterior(playerid, 0);

	SendClientMessage(playerid, -1, "You have been teleported.");
	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s has used /gotorace", GetName(playerid));
	return true;
}
