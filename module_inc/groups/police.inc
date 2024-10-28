#include <YSI_Coding\y_hooks>

#define check_police_member if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_POLICE) return true;
#define check_police_vehicle if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER || GetPlayerState(playerid) == PLAYER_STATE_DRIVER && vehicleInfo[serverVehicleId[GetPlayerVehicleID(playerid)]][vehGroup] != GROUP_TYPE_LSPD && vehicleInfo[serverVehicleId[GetPlayerVehicleID(playerid)]][vehGroup] != GROUP_TYPE_FBI && vehicleInfo[serverVehicleId[GetPlayerVehicleID(playerid)]][vehGroup] != GROUP_TYPE_NG && vehicleInfo[serverVehicleId[GetPlayerVehicleID(playerid)]][vehGroup] != GROUP_TYPE_LVPD) return sendError(playerid,"Nu esti intr-un vehicul de factiune pentru solicitarea acestei actiuni.");

new Iterator: wantedPlayers<MAX_PLAYERS>, wantedReason[MAX_PLAYERS][6][32];

new Float: gateLocationsClosed[][] = 
{
    {1592.67676, -1638.05225, 14.95110, 1.5, -1000.0, -1000.0, -1000.0}, // gate lspd
    {2334.8999000, 2444.3999000, 8.3000000, 1.5, -1000.0, -1000.0, -1000.0}, // gate lvpd
    {135.2833, 1941.3331, 21.6932, 1.5, -1000.0, -1000.0, -1000.0}, // gate ng
    {1544.7007, -1630.7527, 13.2983, 1.5, 0.0000, 90.0200, 90.0000}, // lspd bar
    {2294.2000000, 2499.1001000, 5.7000000, 1.5, -1000.0, -1000.0, -1000.0}, // lvpd bar
    {2238.22021, 2450.44312, 10.58781, 1.5,0.0000, 90.0200, 90.0000} // lvpd bar 2
};

new Float: gateLocationsMoved[][] = 
{
    {1592.65674, -1638.05286, 9.89110, 2.0, -1000.0, -1000.0, -1000.0}, // gate lspd
    {2339.3000000, 2450.5000000, 8.3000000, 2.0, -1000.0, -1000.0, -1000.0}, // gate lvpd
    {122.0023, 1941.4100, 21.6932, 2.0, -1000.0, -1000.0, -1000.0}, // gate ng
    {1544.7007, -1630.7527, 13.2983, 1.0, 0.0000, 0.0000, 90.0000}, // lspd bar
    {2295.3000000, 2507.0000000, 5.7000000, 2.0, -1000.0, -1000.0, -1000.0}, // lvpd bar
    {2238.22021, 2450.44312, 10.58781, 1.0, 0.0000, 0.0000, 90.000} // lvpd bar 2
};

new Float: gateLocations[][] = 
{
    {1588.6552, -1637.9025, 15.0358}, // gate lspd
    {2334.8999000, 2444.3999000, 8.3000000}, // gate lvpd
    {135.2833, 1941.3331, 21.6932}, // gate ng
    {1544.7007, -1630.7527, 13.2983}, // lspd bar
    {2295.3000000, 2507.0000000, 5.7000000}, // lvpd bar
    {2238.22021, 2450.44312, 10.58781} // lvpd bar 2
};

new gateObject[sizeof gateLocations];

function:close_gate(gateId)
{
	new i = gateId;
	MoveDynamicObject(gateObject[i], gateLocationsClosed[i][0], gateLocationsClosed[i][1], gateLocationsClosed[i][2], gateLocationsClosed[i][3],
		gateLocationsClosed[i][4], gateLocationsClosed[i][5], gateLocationsClosed[i][6]);

	return true;
}

hook OnPlayerConnect(playerid)
{

	playerTextdraws[playerid][wantedTD] = CreatePlayerTextDraw(playerid, 18.470577, 315.083252, "wanted scade in: ~r~05:46");
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][wantedTD], 0.270588, 1.290833);
	PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][wantedTD], 1280.000000, 1280.000000);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][wantedTD], 1);
	PlayerTextDrawColour(playerid, playerTextdraws[playerid][wantedTD], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][wantedTD], 0);
	PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][wantedTD], 0x80808080);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][wantedTD], 0);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][wantedTD], 1);
	PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][wantedTD], 0x000000FF);
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][wantedTD], 3);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][wantedTD], 1);
	PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][wantedTD], 0);
	RemoveBuildingForPlayer(playerid, 14843, 266.3516, 81.1953, 1001.2813, 0.25); //gratii pd

	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnGameModeInit()
{
	Command_AddAltNamed("tazer", "ta");

	CreateDynamicPickup(1314, 23, 1526.4021, -1678.0525, 5.8906, 0);
	CreateDynamicPickup(1314, 23, 2282.1470, 2425.6570, 3.4766, 0);
	CreateDynamicPickup(1247, 23, 242.5888, 66.3510, 1003.6406, -1, 6);

	CreateDynamicPickup(1239, 23, 1568.6094, -1690.2958, 5.8906, -1, 0);
	CreateDynamicPickup(1239, 23, 1564.9668, -1666.4725, 28.3956, -1, 0);
	CreateDynamicPickup(1239, 23, 610.5227, -579.4794, 26.1432, -1, 0);
	CreateDynamicPickup(1239, 23, 611.1556, -584.1481, 17.9021, -1, 0);
	CreateDynamicPickup(1239, 23, 2340.2478, 2456.4846, 14.9688, -1, 0);
	CreateDynamicPickup(1239, 23, 2339.2241, 2461.1719, 38.6837, -1, 0);
	CreateDynamicPickup(1239, 23, 187.8355, 1931.4283, 17.6874, -1, 0); //ng nuj pt ce

	CreateDynamicObject(955, 2273.52612, 2429.61206, 10.21940, 0.00000, 0.00000, 0.00000,-1,-1,-1,300.0); //dozator
	CreateDynamicObject(1967, 2294.09497, 2494.32983, 3.40540, 0.00000, 0.00000, -4.68000,-1,-1,-1,300.0); //usa

	  //Lspd Object
	CreateDynamicObject(1500, 1582.6122, -1637.9562, 12.3776, 0.000000, 0.000000, 0.000000,-1,-1,-1,300.0);
    CreateDynamicObject(983, 1545.0706787109, -1635.6511230469, 13.237774848938, 0, 0, 0,-1,-1,-1,300.0);
    CreateDynamicObject(983, 1544.4979248047, -1620.7434082031, 13.238116264343, 0, 0, 0,-1,-1,-1,300.0);

    //FBI DOZATOR
    CreateDynamicObject(955, 650.36047, -609.29718, 15.69630,   0.00000, 0.00000, 172.56000); //dozator

	//PD EXTERIOR
	CreateDynamicObject(11326, 1605.05212, -1623.62476, 14.96770,   0.00000, 0.00000, 89.46000);
	CreateDynamicObject(955, 1553.64233, -1683.42175, 12.92650,   0.00000, 0.00000, -90.00000);
	CreateDynamicObject(983, 1542.62939, -1641.17981, 27.70590,   0.00000, 0.00000, -180.48010);
	CreateDynamicObject(983, 1545.80750, -1650.77563, 27.68590,   0.00000, 0.00000, -270.12021);
	CreateDynamicObject(983, 1542.60828, -1647.55884, 27.70590,   -0.16000, -0.04000, -180.12010);
	CreateDynamicObject(983, 1550.38745, -1650.77563, 27.68590,   0.00000, 0.00000, -270.12021);
	CreateDynamicObject(983, 1542.62939, -1640.45984, 27.70590,   0.00000, 0.00000, -180.48010);
	CreateDynamicObject(983, 1545.85986, -1637.26941, 27.70590,   0.00000, 0.00000, -270.18002);
	CreateDynamicObject(983, 1552.21460, -1637.27222, 27.70590,   0.00000, 0.00000, -270.18002);
	CreateDynamicObject(983, 1558.61877, -1637.28772, 27.70590,   0.00000, 0.00000, -270.18002);
	CreateDynamicObject(983, 1565.02625, -1637.32434, 27.70590,   0.00000, 0.00000, -270.18002);
	CreateDynamicObject(983, 1571.40149, -1637.35168, 27.70590,   0.00000, 0.00000, -270.18002);
	CreateDynamicObject(983, 1574.66931, -1637.35522, 27.70590,   0.00000, 0.00000, -270.17999);
	CreateDynamicObject(983, 1577.83557, -1640.57202, 27.70590,   0.00000, 0.00000, -180.48010);
	CreateDynamicObject(983, 1577.77295, -1646.93713, 27.70590,   0.00000, 0.00000, -180.48010);
	CreateDynamicObject(983, 1577.78162, -1653.32056, 27.70590,   0.00000, 0.00000, -180.06012);
	CreateDynamicObject(983, 1577.78674, -1659.73962, 27.70590,   0.00000, 0.00000, -180.06012);
	CreateDynamicObject(983, 1577.77917, -1666.16150, 27.70590,   0.00000, 0.00000, -180.06012);
	CreateDynamicObject(983, 1577.75732, -1672.53748, 27.70590,   0.00000, 0.00000, -180.06012);
	CreateDynamicObject(983, 1577.72766, -1678.91931, 27.70590,   0.00000, 0.00000, -180.06012);
	CreateDynamicObject(983, 1577.73621, -1685.32568, 27.70590,   0.00000, 0.00000, -180.06012);
	CreateDynamicObject(983, 1577.72400, -1691.71228, 27.70590,   0.00000, 0.00000, -180.06012);
	CreateDynamicObject(983, 1577.71289, -1698.10791, 27.70590,   0.00000, 0.00000, -180.06012);
	CreateDynamicObject(983, 1577.72131, -1704.51245, 27.70590,   0.00000, 0.00000, -180.06012);
	CreateDynamicObject(983, 1577.69897, -1710.88159, 27.70590,   0.00000, 0.00000, -180.06012);
	CreateDynamicObject(983, 1577.69897, -1711.24158, 27.70590,   0.00000, 0.00000, -180.06010);
	CreateDynamicObject(983, 1574.44556, -1714.38306, 27.70590,   0.00000, 0.00000, -270.35995);
	CreateDynamicObject(983, 1568.07471, -1714.34656, 27.70590,   0.00000, 0.00000, -270.35995);
	CreateDynamicObject(983, 1561.66760, -1714.30737, 27.70590,   0.00000, 0.00000, -270.35995);
	CreateDynamicObject(983, 1555.31116, -1714.26440, 27.70590,   0.00000, 0.00000, -270.35995);
	CreateDynamicObject(983, 1548.95508, -1714.22168, 27.70590,   0.00000, 0.00000, -270.35995);
	CreateDynamicObject(983, 1545.76025, -1714.20190, 27.70590,   0.00000, 0.00000, -270.35995);
	CreateDynamicObject(983, 1542.57971, -1710.99304, 27.70590,   0.00000, 0.00000, -359.87982);
	CreateDynamicObject(983, 1542.54773, -1704.60864, 27.70590,   0.00000, 0.00000, -359.87982);
	CreateDynamicObject(983, 1542.54773, -1703.70862, 27.70590,   0.00000, 0.00000, -359.87979);
	CreateDynamicObject(983, 1545.74390, -1700.54211, 27.70590,   0.00000, 0.00000, -270.00009);
	CreateDynamicObject(983, 1550.38757, -1700.53650, 27.70590,   0.00000, 0.00000, -270.00009);
	CreateDynamicObject(983, 1553.58850, -1697.36475, 27.70590,   0.00000, 0.00000, -179.70013);
	CreateDynamicObject(983, 1553.56555, -1690.94910, 27.70590,   0.00000, 0.00000, -179.70013);
	CreateDynamicObject(983, 1553.56580, -1656.84106, 27.70590,   0.00000, 0.00000, -179.70010);
	CreateDynamicObject(983, 1553.55737, -1653.96729, 27.70590,   0.00000, 0.00000, -179.70010);
	CreateDynamicObject(3934, 1562.89099, -1700.73755, 27.38710,   0.00000, -0.02000, -0.84000);
	CreateDynamicObject(3934, 1560.78516, -1650.95300, 27.38710,   0.00000, -0.02000, 1.02000);

	gateObject[0] = CreateDynamicObject(10184, 1592.67676, -1638.05225, 14.95110, 0.00000, 0.00000, -89.70000); // gate lspd
	gateObject[1] = CreateDynamicObject(971, 2334.8999000, 2444.3999000, 8.3000000, 0.0000000, 0.0000000, 60.0000000,-1,-1,-1,300.0); //lvpd 1
	gateObject[2] = CreateDynamicObject(19313, 135.2833, 1941.3331, 21.6932, 0.0000, 0.0000, 0.0000,-1,-1,-1,300.0); // gate ng
	gateObject[3] = CreateDynamicObject(968, 1544.7007, -1630.7527, 13.2983, 0.0000, 90.0200, 90.0000,-1,-1,-1,300.0); // lspd bar
	gateObject[4] = CreateDynamicObject(971, 2294.2000000, 2499.1001000, 5.7000000, 0.0000000, 0.0000000, 272.0000000,-1,-1,-1,300.0); //lvpd 2
	gateObject[5] = CreateDynamicObject(968, 2238.22021, 2450.44312, 10.58781, 0.0000, 90.0200, 90.0000,-1,-1,-1,300.0); //lvpd 3

	CreateDynamic3DTextLabel("{26C4EC}Los Santos Police Department\nProcessing Entrance\n\n(/arrest)", -1, 1526.4021, -1678.0525, 5.8906, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, true);
    CreateDynamic3DTextLabel("{26C4EC}Las Venturas Police Department\nProcessing Entrance\n\n(/arrest)", -1, 2282.1470, 2425.6570, 3.4766, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, true);
	
	//CreateDynamic3DTextLabel("{26C4EC}Restricted to Faction (Officers Only)\nPolice Department Staff HQ", -1, 242.5888, 66.3510, 1003.6406, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, true);
	CreateDynamic3DTextLabel(""COLOR_SV_HEX"Licence Suspend{ffffff}\n\nType "COLOR_SV_HEX"/confiscate{ffffff} to confiscate a player driving licence", -1, 253.8616,68.9618,1003.6406, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, true);

	CreateDynamic3DTextLabel("{26C4EC}Door ID: 30\nRestricted to faction(Los Santos Police Department)\nLSPD Roof | /elevator", -1, 1568.6094, -1690.2958, 5.8906, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, true);
	CreateDynamic3DTextLabel("{26C4EC}LSPD | /elevator", -1, 1564.9668, -1666.4725, 28.3956, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, true);
	
	CreateDynamic3DTextLabel("{26C4EC}Door ID: 30\nRestricted to faction(FBI)\nFBI Roof | /elevator", -1, 610.5227, -579.4794, 26.1432, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, true);
	CreateDynamic3DTextLabel("{26C4EC}FBI | /elevator", -1, 611.1556, -584.1481, 17.9021, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, true);

	CreateDynamic3DTextLabel("{26C4EC}Door ID: 24\nRestricted to faction(Las Venturas Police Department)\nLVPD Roof | /elevator", -1, 2340.2478, 2456.4846, 14.9688, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, true);
	CreateDynamic3DTextLabel("{26C4EC}LVPD | /elevator", -1, 2339.2241, 2461.1719, 38.6837, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, true);

	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook WhenPlayerLoggedIn(playerid)
{
	if(!cache_num_rows())
		return Y_HOOKS_CONTINUE_RETURN_1;

	cache_get_value_name_int(0, "wantedTime", playerStaticVar[playerid][wantedTime]);
	cache_get_value_name_int(0, "wantedLevel", playerInfo[playerid][pWantedLevel]);

	for(new i; i < 6; i++)
	{
		cache_get_value_name(0, va_return("wantedCrime%d", i + 1), wantedReason[playerid][i]);
	}

	return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:elevator(playerid, params[], help)
{
	if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_POLICE)
		return sendError(playerid,"You are not a cop.");

	if(IsPlayerInRangeOfPoint(playerid, 5.0, 611.1556, -584.1481, 17.9021))
		SetPlayerPos(playerid, 610.5227, -579.4794, 26.1432);
	
	if(IsPlayerInRangeOfPoint(playerid, 5.0, 610.5227, -579.4794, 26.1432))
		SetPlayerPos(playerid, 611.1556, -584.1481, 17.9021);

	if(IsPlayerInRangeOfPoint(playerid, 5.0, 2339.2241, 2461.1719, 38.6837))
		SetPlayerPos(playerid, 2340.2478, 2456.4846, 14.9688);
	
	if(IsPlayerInRangeOfPoint(playerid, 5.0, 2340.2478, 2456.4846, 14.9688))
		SetPlayerPos(playerid, 2339.2241, 2461.1719, 38.6837);

	if(IsPlayerInRangeOfPoint(playerid, 5.0, 1568.6094, -1690.2958, 5.8906))
		SetPlayerPos(playerid, 1564.9668, -1666.4725, 28.3956);
	
	if(IsPlayerInRangeOfPoint(playerid, 5.0, 1564.9668, -1666.4725, 28.3956))
		SetPlayerPos(playerid, 1568.6094, -1690.2958, 5.8906);

	return true;
}

stock IsACop(playerid)
	return factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_POLICE ? 1 : 0;



isPlayerTazed(const playerid) {
	return playerStaticVar[playerid][isTazed];
}
static getChasedWantedPlayerCount(const playerid) {
	new count;

	foreach(new i : departmentsMembers) {
		if(isCop(i) && isPlayerDuty(i) && playerStaticVar[i][targetFind] == playerid)
			++count;
	}

	return count;
}
sendDepartmentMessage(dutyStatus, color, const string[], va_args<>)
{
	new returnGroupString[144];
	va_format(returnGroupString, sizeof returnGroupString, string, va_start<3>);

	foreach(new i : factionMembers<GROUP_TYPE_LSPD>)
	{
		if(!playerStaticVar[i][playerLogged] || dutyStatus == 1 && !playerStaticVar[i][groupDuty])
			continue;

		SendClientMessage(i, color, returnGroupString);
	}
	foreach(new i : factionMembers<GROUP_TYPE_FBI>)
	{
		if(!playerStaticVar[i][playerLogged] || dutyStatus == 1 && !playerStaticVar[i][groupDuty])
			continue;

		SendClientMessage(i, color, returnGroupString);
	}
	foreach(new i : factionMembers<GROUP_TYPE_NG>)
	{
		if(!playerStaticVar[i][playerLogged] || dutyStatus == 1 && !playerStaticVar[i][groupDuty])
			continue;

		SendClientMessage(i, color, returnGroupString);
	}
	foreach(new i : factionMembers<GROUP_TYPE_LVPD>)
	{
		if(!playerStaticVar[i][playerLogged] || dutyStatus == 1 && !playerStaticVar[i][groupDuty])
			continue;

		SendClientMessage(i, color, returnGroupString);
	}

	return true;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(newkeys == KEY_CROUCH)
    {
        if(factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_POLICE)
        {
            for(new i; i < sizeof gateLocations; i++)
            {
                if(IsPlayerInRangeOfPoint(playerid, 15.0, gateLocations[i][0], gateLocations[i][1], gateLocations[i][2]))
                {
                    MoveDynamicObject(gateObject[i], gateLocationsMoved[i][0], gateLocationsMoved[i][1], gateLocationsMoved[i][2], gateLocationsMoved[i][3],
                        gateLocationsMoved[i][4], gateLocationsMoved[i][5], gateLocationsMoved[i][6]);

                    SetTimerEx("close_gate", 9000, false, "i", i);
                    break;
                }
            }
            return true;
        }
	}
	if(newkeys == KEY_SECONDARY_ATTACK)
	{
		if(IsPlayerInRangeOfPoint(playerid, 2.5, 242.5888, 66.3510, 1003.6406))
		{
			check_police_member

			if(GetPVarInt(playerid, "enter_building_deelay") > gettime())
				return va_SendClientMessage(playerid, COLOR_GREY, "[Anti-Abuz] Nu poti intra intr-o cladire timp de %d secunde.", GetPVarInt(playerid, "enter_building_deelay") - gettime());

			playerStaticVar[playerid][pLastVirtual] = GetPlayerVirtualWorld(playerid);
			SetPlayerPos(playerid, 288.8077, 166.9345, 1007.1719);

			SetPlayerInterior(playerid, 3);
			SetPlayerVirtualWorld(playerid, 2048 + playerInfo[playerid][pGroup]);

			GameTextForPlayer(playerid, "~w~faction staff hq", 5000, 1);
			SetPVarInt(playerid, "enter_building_deelay", gettime() + 15);
			return true;
		}
		else if(IsPlayerInRangeOfPoint(playerid, 2.5, 288.8077, 166.9345, 1007.1719) && GetPlayerVirtualWorld(playerid) > 2048)
		{
			check_police_member

			SetPlayerPos(playerid, 242.5888, 66.3510, 1003.6406);

			SetPlayerVirtualWorld(playerid, playerStaticVar[playerid][pLastVirtual]);
			SetPlayerInterior(playerid, 6);
			return true;
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:arrest(playerid, params[], help)
{
	if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_POLICE)
		return sendError(playerid,"You are not a police officer");

	check_duty_status

	check_police_vehicle

	new returnPlayer;
	if(sscanf(params, "u", returnPlayer))
		return sendSyntaxMessage(playerid, "/arrest [playername/playerid]");

	if(!IsPlayerConnected(returnPlayer))
		return sendError(playerid,"Invalid player.");

	if(!playerInfo[returnPlayer][pWantedLevel])
		return sendError(playerid,"Acest jucator nu are wanted.");

	if(returnPlayer == playerid)
		return SendClientMessage(playerid, COLOR_LIGHTRED, "You cannot arrest yourself!");

	if(!IsPlayerInRangeOfPoint(playerid, 15.0, 1526.4021, -1678.0525, 5.8906) || !IsPlayerInRangeOfPoint(playerid, 15.0, 2282.1470, 2425.6570, 3.4766) || GetDistanceBetweenPlayers(playerid, returnPlayer) > 15)
		return sendError(playerid,"Nu poti aresta acest jucator deoarece esti prea departe de el sau nu te afli intr-o locatie de arestare.");

	va_SendClientMessageToAll(0xFFA500AA, "%s arrested suspect %s, issuing a fine of $%s with a sentance of %d seconds.", GetName(playerid), GetName(returnPlayer), playerInfo[returnPlayer][pWantedLevel] * 100000, (playerInfo[playerid][pWantedLevel] * 300));


	//va_SendClientMessageToAll(COLOR_LIGHTRED, "%s arrested suspect %s, issuing a fine of $%s with a sentance of %d seconds.", GetName(playerid), GetName(returnPlayer), playerInfo[returnPlayer][pWantedLevel] * 100000, (playerInfo[playerid][pWantedLevel] * 300));
	
	new time = playerInfo[returnPlayer][pWantedLevel] * (5 * 60);
	playerInfo[returnPlayer][pJailTime] = time;
	put_player_in_jail(returnPlayer, playerInfo[returnPlayer][pJailTime], JAIL_TYPE_COP);
	insertRaportPoint(playerid, 1);

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Jail` = '1', `JailType` = '1', `JailTime` = '%d' where `id` = '%d';", playerInfo[returnPlayer][pWantedLevel] * 200, playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

YCMD:cuff(playerid, params[], help)
{
	if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_POLICE)
		return sendError(playerid,"Nu esti politist/nu esti on duty.");

	check_duty_status

	check_police_vehicle

	new returnPlayer;
	if(sscanf(params, "u", returnPlayer))
		return sendSyntaxMessage(playerid, "/cuff [playername/playerid]");

	if(!IsPlayerConnected(returnPlayer))
		return sendError(playerid,"Invalid player.");

	if(returnPlayer == playerid)
		return SendClientMessage(playerid, COLOR_LIGHTRED, "You cannot cuff yourself!");

	if(playerStaticVar[returnPlayer][playerCuffed])
		return sendError(playerid,"This player is already cuffed.");

	if(!playerInfo[returnPlayer][pWantedLevel])
		return sendError(playerid,"This player is not a suspect.");

	if(!IsPlayerInVehicle(returnPlayer, GetPlayerVehicleID(playerid)) && IsValidVehicle(GetPlayerVehicleID(playerid)) && GetPlayerState(returnPlayer) == PLAYER_STATE_PASSENGER)
		return sendError(playerid,"The suspect needs to be in the same car as you.");

	va_SendClientMessage(returnPlayer, -1, "* You were Cuffed by %s, till uncuff.", GetName(playerid));
	va_SendClientMessage(playerid, -1, "* You Cuffed %s, till uncuff.", GetName(returnPlayer));

	sendNearbyMessage(playerid, COLOR_PURPLE, "* %s Hand Cuffs %s, so he wont go anywhere.", GetName(playerid), GetName(returnPlayer));
	sendDepartmentMessage(true, COLOR_ALLDEPT, "%s (%d) handcuffed player %s (%d) (wanted:%d).", GetName(playerid), playerid, GetName(returnPlayer), returnPlayer, playerInfo[returnPlayer][pWantedLevel]);
	
	playerStaticVar[returnPlayer][playerCuffed] = 1;
	GameTextForPlayer(returnPlayer, "~r~~h~Cuffed", 2500, 3);

	SetPlayerSpecialAction(returnPlayer, SPECIAL_ACTION_CUFFED);
	TogglePlayerControllable(returnPlayer, false);
	return true;
}

YCMD:uncuff(playerid, params[], help)
{
	if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_POLICE)
		return sendError(playerid,"You are not a Cop.");

	check_duty_status

	check_police_vehicle

	new returnPlayer;
	if(sscanf(params, "u", returnPlayer))
		return sendSyntaxMessage(playerid, "/uncuff [playername/playerid]");

	if(!IsPlayerConnected(returnPlayer))
		return sendError(playerid,"Invalid player.");

	if(returnPlayer == playerid)
		return SendClientMessage(playerid, COLOR_LIGHTRED, "You cannot uncuff yourself!");

	if(!playerStaticVar[returnPlayer][playerCuffed])
		return sendError(playerid,"This player is not cuffed.");

	if(!IsPlayerInVehicle(returnPlayer, GetPlayerVehicleID(playerid)) && IsValidVehicle(GetPlayerVehicleID(playerid)) && GetPlayerState(returnPlayer) == PLAYER_STATE_PASSENGER)
		return sendError(playerid,"The suspect needs to be in the same car as you.");

	va_SendClientMessage(returnPlayer, -1, "* You were Uncuffed by %s.", GetName(playerid));
	va_SendClientMessage(playerid, -1, "* You Uncuffed %s.", GetName(returnPlayer));

	playerStaticVar[returnPlayer][playerCuffed] = 0;
	GameTextForPlayer(returnPlayer, "~g~Uncuffed", 2500, 3);

	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
	TogglePlayerControllable(returnPlayer, true);
	return true;
}

YCMD:tazer(playerid, params[], help)
{
	if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_POLICE)
		return sendError(playerid,"You are not a police officer");

	check_duty_status

	if(playerStaticVar[playerid][tazerEquiped])
		GivePlayerWeapon(playerid, 24, 999), playerStaticVar[playerid][tazerEquiped] = 0, SendClientMessage(playerid, -1, "You disabled the tazer.");

	else
		GivePlayerWeapon(playerid, 23, 20), playerStaticVar[playerid][tazerEquiped] = 1, SendClientMessage(playerid, -1, "You enabled the tazer.");

	return true;
}

timer playerTazer[8000](playerid) {
	if(!playerStaticVar[playerid][playerLogged])
		return 1;

	playerStaticVar[playerid][isTazed] = false;

    return TogglePlayerControllableEx(playerid, true);
}


YCMD:d(playerid, params[], help)
{
	new returnMessage[128];
	if(sscanf(params, "s[128]", returnMessage))
		return sendSyntaxMessage(playerid, "/d <Message>");

	if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_POLICE)
		return sendError(playerid,"You are not part of a department.");

	if(FindIP(returnMessage)) /// Anti-Reclama
		sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s(%d) typed %s in /d.", GetName(playerid), playerid, returnMessage);

	sendDepartmentMessage(false, COLOR_ALLDEPT, "%s %s %s: %s, over.", (playerInfo[playerid][pGroupRank] < 5 ? ("*") : playerInfo[playerid][pGroupRank] == 5 ? ("**") : ("***")), GetFactionRankName(playerid), GetDBName(playerid), returnMessage);
	return true;
}

task update_wanted_deelay[5000]()
{
	foreach(new i : wantedPlayers)
	{	
		if (Iter_Contains(wantedPlayers, i))
		{
			if (playerStaticVar[i][afkTime] >= 300)
			{
				va_SendClientMessageToAll(COLOR_LIGHTRED, "%s was kicked by AdmBot, reason: AFK with wanted for more than 5 minutes.", GetDBName(i));
				Iter_Remove(wantedPlayers, i);
				playerStaticVar[i][afkTime] = 0;
				KickEx(i);
			}
		}
	}
	return true;
}

timer wantedTimer[1000](playerid) {
	if(--playerStaticVar[playerid][wantedTime] < 1) {
		SetPlayerWantedLevel(playerid, --playerInfo[playerid][pWantedLevel]);

		va_mysql_tquery("UPDATE `users` SET `wantedLevel` = %d WHERE `ID` = %d", playerInfo[playerid][pWantedLevel], playerInfo[playerid][pSQLID]);

		if(playerInfo[playerid][pWantedLevel]) {
			SendClientMessage(playerid, COLOR_YELLOW, "* You lost one wanted point. (new wanted: %d, expire time: 15:00)", playerInfo[playerid][pWantedLevel]);
		}
		else {
			clearWanted(playerid);

			SendClientMessage(playerid, COLOR_YELLOW, "* You lost all wanted points and escaped the police.");

			sendDepartmentMessage(true, COLOR_DBLUE, "Dispatch: %s(%d) lost all wanted points because cops didn't catch him.", GetName(playerid), playerid);
			return 1;
		}
		
		sendDepartmentMessage(true, COLOR_DBLUE, "Dispatch: %s(%d) lost one wanted points because it ran 15 minutes by police.", GetName(playerid), playerid);

		playerStaticVar[playerid][wantedTime] = 15 * 60;
	}

	format(returnString, sizeof returnString, "WANTED SCADE IN: ~R~%s ~W~~H~MINUTE", CalculateTime(playerStaticVar[playerid][wantedTime]));
	PlayerTextDrawSetString(playerid, playerTextdraws[playerid][wantedTD], returnString);
	PlayerTextDrawShow(playerid, playerTextdraws[playerid][wantedTD]);
	return 1;
}
setPlayerWanted(playerid, level, time)
{
	if(!playerInfo[playerid][pWantedLevel])
		return clearWanted(playerid);


	playerStaticVar[playerid][wantedTime] = time;
	SetPlayerWantedLevel(playerid, level);
	Iter_Add(wantedPlayers, playerid);
	playerStaticVar[playerid][var_wanted_timer] = repeat wantedTimer(playerid);
	new returnQuery[256];
	mysql_format(SQL, returnQuery, sizeof returnQuery, "update `users` set `wantedLevel` = '%d', `wantedTime` = '%d' where `id` = '%d';", level, playerStaticVar[playerid][wantedTime], playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnQuery, "", "");
	return true;
}

clearWanted(const playerid)
{
	playerInfo[playerid][pWantedLevel] = 0;
	stop playerStaticVar[playerid][var_wanted_timer];
	Iter_Remove(wantedPlayers, playerid);
	playerStaticVar[playerid][wantedTime] = -1;

	PlayerTextDrawHide(playerid, playerTextdraws[playerid][wantedTD]);

	for(new i; i < 6; i++)
		format(wantedReason[playerid][i], 32, "Fara Crima");

	SetPlayerWantedLevel(playerid, 0);

	new returnQuery[512];
	mysql_format(SQL, returnQuery, sizeof returnQuery, "update `users` set `wantedCrime1` = 'Fara Crima', `wantedCrime2` = 'Fara Crima', `wantedCrime3` = 'Fara Crima', `wantedCrime4` = 'Fara Crima', `wantedCrime5` = 'Fara Crima', `wantedCrime6` = 'Fara Crima', \
		`wantedTime` = '0', `wantedLevel` = '0' where `id` = '%d';", playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnQuery, "", "");
	return true;
}

YCMD:wantedplayers(playerid, params[], help)
{
	check_admin

	if(!Iter_Count(wantedPlayers))
		return SendClientMessage(playerid, COLOR_LIGHTRED, "* Currently no wanted players online.");

    SendClientMessage(playerid, COLOR_CYAN, "Wanted Players");

    foreach(new i : wantedPlayers)
    {
        va_SendClientMessage(playerid, -1, "%s (%d) - %s left.", GetName(i), i, CalculateTime(playerStaticVar[i][wantedTime]));
    }
    va_SendClientMessage(playerid, COLOR_LIGHTRED, "* Now are %d wanted players online.", Iter_Count(wantedPlayers));
    return true; 
}

YCMD:wanted(playerid, params[], help)
{
	check_police_member

	if(!Iter_Count(wantedPlayers))
		return sendError(playerid,"No active felons found.");

	new listId;
	returnString = "Name\tWanted level\tTime left\tDistance\n";

	foreach(new i : wantedPlayers)
	{
		format(returnString, sizeof returnString, "%s%s\t%d\t%s\t%.0fm\n", returnString, GetName(i), playerInfo[i][pWantedLevel], CalculateTime(playerStaticVar[i][wantedTime]), GetDistanceBetweenPlayers(playerid, i));
		playerStaticVar[playerid][pDialogItems][listId] = i;

		listId ++;
	}
	Dialog_Show(playerid, DIALOG_WANTED, DIALOG_STYLE_TABLIST_HEADERS, "Server: Active Felons", returnString, "Find", "Exit");
	return true;
}
Dialog:DIALOG_WANTED(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	new returnPlayer = playerStaticVar[playerid][pDialogItems][listitem];

	if(!playerInfo[returnPlayer][pWantedLevel])
		return va_SendClientMessage(playerid, -1, "-- MDC [ID %d - %s][not wanted]", returnPlayer, GetName(returnPlayer));

	va_SendClientMessage(playerid, -1, "-- MDC [ID %d - %s][{FFFF00}W:%d{FFFFFF}][Chased by %d cops][Wanted expires in %d mins]", returnPlayer, GetName(returnPlayer), playerInfo[returnPlayer][pWantedLevel], getChasedWantedPlayerCount(returnPlayer), CalculateTime(playerStaticVar[returnPlayer][wantedTime]));

	for(new i; i < 6; i++)
	{
		if(strmatch(wantedReason[returnPlayer][i], "Fara Crima"))
			continue;

		va_SendClientMessage(playerid, -1, "MDC Crimes: [%s]", wantedReason[returnPlayer][i]);
	}
	set_find_checkpoint(playerid, returnPlayer);
	return true;
}

YCMD:frisk(playerid, params[], help)
{
	new giveplayerid;
	if(sscanf(params, "u", giveplayerid))
		return sendSyntaxMessage(playerid, "/frisk <Name/Playerid>");

	if(!playerStaticVar[playerid][groupDuty])
		return sendError(playerid,"To use this command you must be ON-DUTY!");
		
	if(!IsACop(playerid))
		return sendError(playerid,"{FFB870}You are not a Cop.");

	if(!IsPlayerConnected(giveplayerid))
		return sendError(playerid,"Player not connected.");

	if(GetDistanceBetweenPlayers(playerid, giveplayerid) > 8)
		return sendError(playerid,"{FFFFCC}This player is not near you.");

	if(giveplayerid == playerid)
		return SendClientMessage(playerid, COLOR_LIGHTRED, "You cannot Frisk yourself.");

	new text1[20], text2[20], text4[20];
	if(playerInfo[giveplayerid][pDrugs] > 0) { text1 = "| Drugs."; } else { text1 = "| Empty Pocket."; }
	if(playerInfo[giveplayerid][pMaterials] > 0) { text2 = "| Materials."; } else { text2 = "| Empty Pocket."; }
	if(playerInfo[giveplayerid][pPhoneBook] > 0) { text4 = "| Phone Book."; } else { text4 = "| Empty Pocket."; }
				    
    va_SendClientMessage(playerid, -1, "** Items with {B8DBFF}%s{FFFFFF} **", GetName(giveplayerid));
    va_SendClientMessage(playerid, -1, "%s, Grams:%d", text1, playerInfo[giveplayerid][pDrugs]);
	va_SendClientMessage(playerid, -1, "%s, Materials:%d", text2, playerInfo[giveplayerid][pMaterials]);
	va_SendClientMessage(playerid, -1, "%s.", text4);

    new Player_Weapons[13], Player_Ammos[13];
    for(new i = 1; i <= 12; i++)
    {
        GetPlayerWeaponData(giveplayerid, i, Player_Weapons[i], Player_Ammos[i]);
        if(Player_Weapons[i])
			va_SendClientMessage(playerid, -1, "{B8DBFF}* %s, Gun:{FFFFFF} %s{B8DBFF}, Ammo:{FFFFFF} %d", GetName(giveplayerid), GetWeaponNameEx(Player_Weapons[i]), Player_Ammos[i]);

    }
	sendNearbyMessage(playerid, COLOR_PURPLE, "* %s has frisked %s for any illegal items.", GetName(playerid), GetName(giveplayerid));				
    return true;
}

YCMD:su(playerid, params[], help)
{
	new returnPlayer, returnWanted, returnReason[32];
	if(sscanf(params, "uis[32]", returnPlayer, returnWanted, returnReason))
		return sendSyntaxMessage(playerid, "/su <playerid/playername> <wanted level> <wanted reason>");

	if(FindIP(returnReason)) /// Anti-Reclama
		sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s(%d) typed %s in /su.", GetName(playerid), playerid, returnReason);

	if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_POLICE)
		return SendClientMessage(playerid, COLOR_GREY, "{FFB870}You are not a cop.");

	check_duty_status

	if(returnPlayer == playerid)
		return SendClientMessage(playerid, COLOR_LIGHTRED, "You cannot suspect yourself!");

	if(IsACop(returnPlayer))
		return sendError(playerid,"You can't suspect a Cop!");

	if(playerInfo[playerid][pJailTime] != 0)
		return sendError(playerid,"You can't suspect an arrested player!");

	if(isPlayerInWar(returnPlayer))
		return sendError(playerid, "You cannot do that because the player is in war.");
	
	if(returnPlayer == INVALID_PLAYER_ID || !IsPlayerConnected(playerid))
		return sendError(playerid,"Player not connected.");

	if(returnWanted < 1 || returnWanted > 7)
		return sendError(playerid,"Invalid wanted level.");

	GivePlayerWanted(returnPlayer, playerid, returnWanted, returnReason);
	sendDepartmentMessage(true, COLOR_DBLUE, "Dispatch: %s [%d] has committed a crime: %s. Reporter: %s. New wanted level: %d.", GetName(returnPlayer), returnPlayer, returnReason, GetName(playerid), playerInfo[returnPlayer][pWantedLevel]);
	return true;
}

GivePlayerWanted(returnPlayer, reporterId, returnWanted, const returnReason[32])
{
	if(returnWanted + playerInfo[returnPlayer][pWantedLevel] > 6)
		playerInfo[returnPlayer][pWantedLevel] = 6;

	else
		playerInfo[returnPlayer][pWantedLevel] += returnWanted;


	setPlayerWanted(returnPlayer, playerInfo[returnPlayer][pWantedLevel], 15 * 60);

    new name[MAX_PLAYER_NAME + 1] = "Unknown";

    if (reporterId != INVALID_PLAYER_ID)
    {
        GetPlayerName(reporterId, name, sizeof(name));
    }

    va_SendClientMessage(returnPlayer, COLOR_WARNING, "Ai comis o infractiune: %s, raportat de: %s. W: +%d. Nivel de urmarire nou: %d.", returnReason, name, returnWanted, playerInfo[returnPlayer][pWantedLevel]);
		
	for(new i; i < 6; i++)
	{
		if(!strmatch(wantedReason[returnPlayer][i], "Fara Crima"))
			continue;

		format(wantedReason[returnPlayer][i], 32, "%s", returnReason);
		
		mysql_format(SQL, returnString, sizeof returnString, "update `users` set `wantedCrime%d` = '%e' where `id` = '%d';", i + 1, returnReason, playerInfo[returnPlayer][pSQLID]);
		mysql_tquery(SQL, returnString, "", "");
		break;
	}

	return true;
}

YCMD:permis(playerid, params[], help)
{
	check_delay

	if(playerInfo[playerid][pGroup] != 8 && playerInfo[playerid][pGroup] != 1) 
		return SendClientMessage(playerid, COLOR_GREY, "Invalid group id!");

	new returnPlayer;
	if(sscanf(params, "u", returnPlayer))
		return sendSyntaxMessage(playerid, "/permis [playername/playerid]");

	if(returnPlayer == playerid)
		return SendClientMessage(playerid, COLOR_LIGHTRED, "You cannot do that on yourself!");

	if(GetDistanceBetweenPlayers(playerid, returnPlayer) > 25)
		return sendError(playerid,"Esti prea departe de acel jucator.");

	sendNearbyMessage(playerid, COLOR_YELLOW, "Politist %s: {1560BD}%s{fff000}, ai fost surprins conducand neregulamenter.", GetName(playerid), GetName(returnPlayer));
	sendNearbyMessage(playerid, COLOR_YELLOW, "Foloseste /handsup pentru a-ti confisca permisul sau vei primi wanted 3!");
	return set_delay;
}

YCMD:casca(playerid, params[], help)
{
	check_delay

	if(playerInfo[playerid][pGroup] != 8 && playerInfo[playerid][pGroup] != 1) 
		return SendClientMessage(playerid, COLOR_GREY, "Invalid group id!");

	new returnPlayer;
	if(sscanf(params, "u", returnPlayer))
		return sendSyntaxMessage(playerid, "/casca [playername/playerid]");

	if(returnPlayer == playerid)
		return SendClientMessage(playerid, COLOR_LIGHTRED, "You cannot do that on yourself!");

	if(GetDistanceBetweenPlayers(playerid, returnPlayer) > 25)
		return sendError(playerid,"Esti prea departe de acel jucator.");

	sendNearbyMessage(playerid, COLOR_YELLOW, "Politist %s: {1560BD}%s{fff000}, ai fost surprins conducand fara casca.", GetName(playerid), GetName(returnPlayer));
	sendNearbyMessage(playerid, COLOR_YELLOW, "Trage pe dreapta pentru a primi o amenda in valoare de $20.000. Risti wanted 3!");
	return set_delay;
}

YCMD:nos(playerid, params[], help)
{
	check_delay

	if(playerInfo[playerid][pGroup] != 8 && playerInfo[playerid][pGroup] != 1) 
		return SendClientMessage(playerid, COLOR_GREY, "Invalid group id!");

	new returnPlayer;
	if(sscanf(params, "u", returnPlayer))
		return sendSyntaxMessage(playerid, "/nos [playername/playerid]");

	if(returnPlayer == playerid)
		return SendClientMessage(playerid, COLOR_LIGHTRED, "You cannot do that on yourself!");

	if(GetDistanceBetweenPlayers(playerid, returnPlayer) > 25)
		return sendError(playerid,"Esti prea departe de acel jucator.");

	sendNearbyMessage(playerid, COLOR_YELLOW, "Politist %s: {1560BD}%s{fff000}, ai fost surprins conducand cu NOS.", GetName(playerid), GetName(returnPlayer));
	sendNearbyMessage(playerid, COLOR_YELLOW, "Trage pe dreapta pentru a primi o amenda in valoare de $50.000. Risti wanted 3!");
	return set_delay;
}

YCMD:so(playerid, params[], help)
{
	if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_POLICE)
		return SendClientMessage(playerid, COLOR_LIGHTRED, "You are not a cop.");

	if(GetPVarInt(playerid, "commands_delay") > gettime())
		return sendError(playerid,"{3385FF}Wait 5 seconds to use again /so.");

	new returnPlayer;
	if(sscanf(params, "u", returnPlayer))
		return sendSyntaxMessage(playerid, "/so [playername/playerid]");

	if(returnPlayer == playerid)
		return SendClientMessage(playerid, COLOR_LIGHTRED, "You cannot do that on yourself!");

	if(GetDistanceBetweenPlayers(playerid, returnPlayer) > 25)
		return sendError(playerid,"Acel player este prea departe de tine.");

	sendNearbyMessage(playerid, COLOR_YELLOW, "(megaphone) Politist %s: {1560BD}%s{FFFF00}, esti urmarit de politie! %s", GetName(playerid), GetName(returnPlayer), (!playerInfo[returnPlayer][pWantedLevel] ? ("Ramai pe loc!") : ("Te predai?")));
	return set_delay;
}

YCMD:ticket(playerid, params[], help)
{
	if(!playerStaticVar[playerid][groupDuty])
		return sendError(playerid,"You are not on Duty.");

	if(playerInfo[playerid][pGroup] != 1 && playerInfo[playerid][pGroup] != 8)
		return sendError(playerid,"You are not a LSPD/LVPD member.");

	new returnPlayer, returnAmount, returnReason[24];
	if(sscanf(params, "uis[24]", returnPlayer, returnAmount, returnReason))
		return sendSyntaxMessage(playerid, "/ticket <Name/Playerid> <Amount of Money> <Reason>");

	if(returnAmount < 1 || returnAmount > 99999)
		return sendError(playerid,"Ticket money can't be below 1 or higher then 99999.");

	if(factionType[playerInfo[returnPlayer][pGroup]] == FACTION_TYPE_POLICE)
		return sendError(playerid,"You can't give a ticket to a Cop!");

	if(returnPlayer == playerid)
		return SendClientMessage(playerid, COLOR_LIGHTRED, "You cannot do that on yourself!");
	
	if(!IsPlayerConnected(returnPlayer) || returnPlayer == INVALID_PLAYER_ID)
		return sendError(playerid,"Player not connected.");

	if(GetDistanceBetweenPlayers(playerid, returnPlayer) > 8)
		return sendError(playerid,"{FFFFCC}This player is not near you.");

	va_SendClientMessage(playerid, -1, "{B8FFDB}* You gave %s a Ticket costing $%s, reason: %s", GetDBName(returnPlayer), FormatNumber(returnAmount), returnReason);
	va_SendClientMessage(returnPlayer, -1, "{B8FFDB}* Officer %s has given you a Ticket costing $%s, reason: %s, type /accept ticket %d to accept it", GetDBName(playerid), FormatNumber(returnAmount), returnReason, playerid);
	
	playerStaticVar[playerid][ticketPlayer] = returnPlayer + 1;
	playerStaticVar[playerid][ticketPrice] = returnAmount;
	return true;
}

YCMD:mdc(playerid, params[], help)
{
	if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_POLICE && !playerInfo[playerid][pAdmin])
		return sendError(playerid,"Nu aveti gradul administrativ necesar.");

	new returnPlayer;
	if(sscanf(params, "u", returnPlayer))
		return sendSyntaxMessage(playerid, "/mdc <Name/Playerid>");

	if(!IsPlayerConnected(returnPlayer))
		return sendError(playerid,"Invalid player.");

	if(!playerInfo[returnPlayer][pWantedLevel])
		return va_SendClientMessage(playerid, -1, "-- MDC [ID %d - %s][not wanted]", returnPlayer, GetName(returnPlayer));

	va_SendClientMessage(playerid, -1, "-- MDC [ID %d - %s][{FFFF00}W:%d{FFFFFF}][Chased by %d cops][Wanted expires in %d mins]", returnPlayer, GetName(returnPlayer), playerInfo[returnPlayer][pWantedLevel], getChasedWantedPlayerCount(returnPlayer), CalculateTime(playerStaticVar[returnPlayer][wantedTime]));

	for(new i; i < 6; i++)
	{
		if(strmatch(wantedReason[returnPlayer][i], "Fara Crima"))
			continue;

		va_SendClientMessage(playerid, -1, "%s", wantedReason[returnPlayer][i]);
	}
	return true;
}

YCMD:confiscate(playerid, params[], help)
{
	if(playerInfo[playerid][pGroup] != 8 && playerInfo[playerid][pGroup] != 1 && playerInfo[playerid][pGroup] != 2) 
		return sendError(playerid,"You are not a police officer.");

	new returnItem[15], returnPlayer;
	if(sscanf(params, "us[15]", returnPlayer, returnItem))
	{
		sendSyntaxMessage(playerid, "/confiscate <Name/Playerid> <Item>");
		SendClientMessage(playerid, -1, "Items: Licence, Drugs, Materials, Weapons.");
		return true;
	}

	if(!IsPlayerConnected(returnPlayer) || returnPlayer == INVALID_PLAYER_ID)
			return sendError(playerid,"Player not connected.");

	if(returnPlayer == playerid)
		return SendClientMessage(playerid, COLOR_LIGHTRED, "You cannot confiscate your licenses!");

	if(!ProxDetectorS(8.0, playerid, returnPlayer))	
		return sendError(playerid, "This player is not near you.");


	if(strmatch(returnItem, "licence"))
	{
		if(!playerInfo[playerid][pCarLic])
			return sendError(playerid,"This player don't have a driving licence.");

		new stringConfiscate[128];
		format(stringConfiscate, sizeof stringConfiscate, "* %s has confiscated %s's driving licence.", GetDBName(playerid), GetDBName(returnPlayer));

		sendNearbyMessage(playerid, COLOR_PURPLE, stringConfiscate);
		sendDepartmentMessage(1, COLOR_LIGHTBLUE, stringConfiscate);

		va_SendClientMessage(playerid, -1, "You have confiscated %s's driving licence.", GetDBName(returnPlayer));
		va_SendClientMessage(returnPlayer, -1, "%s has confiscated your driving license.", GetDBName(playerid));

		playerInfo[returnPlayer][pCarLic] = 0;
		insertRaportPoint(playerid, 4);

		mysql_format(SQL, returnString, sizeof returnString, "update `users` set `DrivingLicence` = '0' where `id` = '%d';", playerInfo[returnPlayer][pSQLID]);
		mysql_tquery(SQL, returnString, "", "");
		return true;
	}
	if(strmatch(returnItem, "drugs"))
	{
		if(!playerInfo[playerid][pDrugs])
			return sendError(playerid,"This player don't have drugs.");

		new stringConfiscate[128];
		format(stringConfiscate, sizeof stringConfiscate, "* %s has confiscated %s's drugs.", GetDBName(playerid), GetDBName(returnPlayer));

		sendNearbyMessage(playerid, COLOR_PURPLE, stringConfiscate);
		sendDepartmentMessage(1, COLOR_LIGHTBLUE, stringConfiscate);

		va_SendClientMessage(playerid, -1, "You have confiscated %s's drugs.", GetDBName(returnPlayer));
		va_SendClientMessage(returnPlayer, -1, "%s has confiscated your drugs.", GetDBName(playerid));

		playerInfo[returnPlayer][pDrugs] = 0;
		insertRaportPoint(playerid, 3);

		mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Drugs` = '0' where `id` = '%d';", playerInfo[returnPlayer][pSQLID]);
		mysql_tquery(SQL, returnString, "", "");
		return true;
	}

	else
		SendClientMessage(playerid, COLOR_GREY, "Unknown take name.");

	return true;
}

YCMD:aclear(playerid, params[], help)
{
	check_admin

	new returnPlayer;
	if(sscanf(params, "u", returnPlayer))
		return sendSyntaxMessage(playerid, "/aclear [playername/playerid]");

	if(!IsPlayerConnected(returnPlayer))
		return sendError(playerid,"Invalid player.");

	if(!Iter_Contains(wantedPlayers, returnPlayer))
		return sendError(playerid,"Acest jucator nu are wanted.");

	clearWanted(returnPlayer);
	Iter_Remove(wantedPlayers, returnPlayer);
	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_DCHAT, "Admin %s cleared %s records.", GetName(playerid), GetName(returnPlayer));
	va_SendClientMessage(returnPlayer, COLOR_GREY, "Administratorul %s ti-a curatat cazierul.", GetName(playerid));
	return true;
}

YCMD:clear(playerid, params[], help)
{
	if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_POLICE)
		return sendError(playerid,"You are not an Cop/FBI/National Guard.");

	new returnPlayer;
	if(sscanf(params, "u", returnPlayer))
		return sendSyntaxMessage(playerid, "/clear [playername/playerid]");

	if(!IsPlayerConnected(returnPlayer))
		return sendError(playerid,"Invalid player.");

	if(!Iter_Contains(wantedPlayers, returnPlayer))
		return sendError(playerid,"Acest jucator nu are wanted.");

	clearWanted(returnPlayer);

	va_SendClientMessage(returnPlayer, COLOR_LIGHTBLUE, "%s has cleared all your warrants!", GetName(playerid));
	sendDepartmentMessage(1, COLOR_LIGHTBLUE, "Dispatch: %s has cleared all the warrants on %s.", GetName(playerid), GetName(returnPlayer));


	new returnQuery[256];
	mysql_format(SQL, returnQuery, sizeof returnQuery, "update `users` set `wantedLevel` = '%d', `wantedTime` = '%d' where `id` = '%d';", playerInfo[returnPlayer][pWantedLevel], playerStaticVar[returnPlayer][wantedTime], playerInfo[returnPlayer][pSQLID]);
	mysql_tquery(SQL, returnQuery, "", "");

	return true;
}

#include "../module_inc/src/jailSystem.inc"
#include "../module_inc/src/jailWork.inc"