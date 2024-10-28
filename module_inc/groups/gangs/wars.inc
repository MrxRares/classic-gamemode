// ------------------------------------- //
//            Includes                   //
// ------------------------------------- //

#include <YSI_Coding\y_hooks>

// ------------------------------------- //
//            Macros                     //
// ------------------------------------- //

#define WARS_TIME 10 * 10
#define MAX_PICKUPSW 2000
#define WAR_PILLS_LIMIT 100

// ------------------------------------- //
//            Enums                      //
// ------------------------------------- //

static enum e_warInfo {
	warID, 
	warAttackerFaction,
	warDefenderFaction,
	warAttackerScore,
	warDefenderScore,
	warLeftTime,
	warAttackersOnTurf,
	warDefendersOnTurf,
	warInfluenceTime,
	warAttackersOffTurfSeconds,
	Timer:warTimer
}

static enum e_pickups {
	psID, psType, psWar,
}
new Pickups[MAX_PICKUPSW][e_pickups];
// ------------------------------------- //
//            Variables                  //
// ------------------------------------- //

new 
	warInfo[MAX_TURFS][e_warInfo],

	warPlayerTurfTime[MAX_PLAYERS],
	warPlayerKills[MAX_PLAYERS],
	warPlayerDeaths[MAX_PLAYERS],
	warPlayerAssists[MAX_PLAYERS],

	bool:playerOnTurf[MAX_PLAYERS],
	factionAttackDelay[MAX_FACTIONS] = {-1, ...},

	Iterator:warPlayers[MAX_TURFS]<MAX_PLAYERS>;

// ------------------------------------- //
//            Query Functions            //
// ------------------------------------- //

function:assignServerWarsLastID() {
	cache_get_value_name_int(0, "lastID", serverStuff[wars]);
} 

function:loadPlayerWarInfo(const playerid, const sqlid) {
	if(playerInfo[playerid][pSQLID] != sqlid)
		return 1;

	if(!cache_num_rows()) {	
		va_mysql_tquery("INSERT INTO `server_users_wars` (`warID`, `warPlayerID`, `warFactionID`) VALUES (%d, %d, %d)", warInfo[factionWar[playerInfo[playerid][pGroup]]][warID], playerInfo[playerid][pSQLID], groupInfo[playerInfo[playerid][pGroup]][factionID]);

		warPlayerTurfTime[playerid] = warPlayerKills[playerid] = warPlayerAssists[playerid] = warPlayerDeaths[playerid] = 0;

		return printf("[MYSQL] Insert player %s from war in database successfully.", GetName(playerid));
	}

	cache_get_value_name_int(0, "warPlayerTurfTime", warPlayerTurfTime[playerid]);
	cache_get_value_name_int(0, "warPlayerKills", warPlayerKills[playerid]);
	cache_get_value_name_int(0, "warPlayerDeaths", warPlayerDeaths[playerid]);
	cache_get_value_name_int(0, "warPlayerAssists", warPlayerAssists[playerid]);

	return printf("[MYSQL] Loaded player %s war info successfully.", GetName(playerid));
}

function:getServerWarWorstAndWinner(const turf, const bool:fail) {
	if(!cache_num_rows())
		return 1;

	new 
		warPlayerName[2][MAX_PLAYER_NAME],
		Float:warKD[2],
		warKills[2],
		warAssists[2],
		warDeaths[2];

	for(new i = 0; i < 2; ++i) {
		cache_get_value_name(i, "warPlayerName", warPlayerName[i]);

		cache_get_value_name_int(i, "warPlayerKills", warKills[i]);
		cache_get_value_name_int(i, "warPlayerDeaths", warDeaths[i]);
		cache_get_value_name_int(i, "warPlayerAssists", warAssists[i]);

		cache_get_value_name_int(i, "warPlayerKD", warKD[i]);
	}

	if(fail) {
		va_SendClientMessageToAll(COLOR_SERVER, "WAR >>: {ffffff}Gruparea mafiota %s a esuat sa cucereasca teritoriul #%d aparat de mafia %s deoarece nu s-au prezentat pe turf.", groupInfo[warInfo[turf][warAttackerFaction]][fName], turfInfo[turf][turfID], groupInfo[warInfo[turf][warDefenderFaction]][fName]);
	}
	else {
		if(warInfo[turf][warAttackerScore] > warInfo[turf][warDefenderScore]) {
			va_SendClientMessageToAll(COLOR_SERVER, "WAR >>: {ffffff}Gruparea mafiota %s a cucerit teritoriul #%d aparat de mafia %s.", groupInfo[warInfo[turf][warAttackerFaction]][fName], turfInfo[turf][turfID], groupInfo[warInfo[turf][warDefenderFaction]][fName]);

			turfInfo[turf][turfFactionID] = warInfo[turf][warAttackerFaction];

			foreach(new i : showTurfs) {
				GangZoneHideForPlayerEx(i, turfInfo[turf][turfGameID]);
				GangZoneShowForPlayerEx(i, turfInfo[turf][turfGameID], turfs_colors[turfInfo[turf][turfFactionID]] | 0x000000DC);
			}

			va_mysql_tquery("UPDATE `gang_turfs` SET `FactionID` = %d WHERE `ID` = %d", groupInfo[turfInfo[turf][turfFactionID]][factionID], turfInfo[turf][turfID]);
		}
		else {
			va_SendClientMessageToAll(COLOR_SERVER, "WAR >>: {ffffff}Gruparea mafiota %s a aparat teritoriul #%d atacat de mafia %s.", groupInfo[warInfo[turf][warDefenderFaction]][fName], turfInfo[turf][turfID], groupInfo[warInfo[turf][warAttackerFaction]][fName]);
		}
	}

	foreach(new i : warPlayers[turf]) {
		va_SendClientMessage(i, COLOR_GREEN, "------------ [ war overview #%d ] ------------", warInfo[turf][warID]);
		va_SendClientMessage(i, COLOR_GREEN, "Score: %s %d - %d %s", groupInfo[warInfo[turf][warAttackerFaction]][fName], warInfo[turf][warAttackerScore], warInfo[turf][warDefenderScore], groupInfo[warInfo[turf][warDefenderFaction]][fName]);
		va_SendClientMessage(i, COLOR_GREEN, "Best score: %s with %d kills and %d deaths. Worst score: %s with %d kills and %d deaths.", warPlayerName[0],  warKills[0], warDeaths[0], warPlayerName[1], warKills[1], warDeaths[1]);
		va_SendClientMessage(i, COLOR_GREEN, "Your personal score: %0.2f (%d kills, %d assists %d deaths).", getPlayerWarKDRatio(i), warPlayerKills[i], warPlayerAssists[i], warPlayerDeaths[i]);
		va_SendClientMessage(i, COLOR_GREEN, "----------------------------------------------");
		SpawnPlayer(i);
		if(strmatch(GetDBName(i), warPlayerName[0]))
		{
			SendClientMessage(i, COLOR_GREEN2, "Deoarece ai fost cel mai bun mafiot al acestui razboi ai primit 20.000 materiale si 150.000$.");
			playerInfo[i][pMaterials] += 20000, addMoney(i, GetMilliards(i), GetMoney(i), 0, 20000, false);

		}
	}


	Iter_Clear(warPlayers[turf]);
	factionAttackDelay[warInfo[turf][warAttackerFaction]] = gettime() + (15 * 60);

	factionWar[warInfo[turf][warAttackerFaction]] = factionWar[warInfo[turf][warDefenderFaction]] = -1;
	warInfo[turf][warAttackerFaction] = warInfo[turf][warDefenderFaction] = -1;
	return 1;
}

// ------------------------------------- //
//            Functions                  //
// ------------------------------------- //

function:CheckIDEmpty() {
	for(new i = 0; i < MAX_PICKUPSW; i++) {
		if(Pickups[i][psID] == 0) return i;
	}
	return 0;
}
joinWarMessage(playerid)
{
	if(factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_GANG)
	{
		if(factionWar[playerInfo[playerid][pGroup]] != -1 && playerStaticVar[playerid][groupDuty] && playerStaticVar[playerid][inHQ]) {
			SendClientMessage(playerid, COLOR_SERVER, "WAR >>:{ffffff} War-ul a inceput! Ai fost scos automat din hq.");
			SendClientMessage(playerid, COLOR_SERVER, "WAR >>:{ffffff} Ai fost transferat intr-un Virtual World unde sunt prezenti doar jucatorii ce participa la war.");
			SetPlayerPos(playerid, groupInfo[playerInfo[playerid][pGroup]][factionExterior][0], groupInfo[playerInfo[playerid][pGroup]][factionExterior][1], groupInfo[playerInfo[playerid][pGroup]][factionExterior][2]);
			SetPlayerInterior(playerid, 0);
		}	
	}
	return true;
}

static startWar(const turf, const attackerFaction, const defenderFaction) {
	warInfo[turf][warID] = ++serverStuff[wars];

	warInfo[turf][warAttackerFaction] = attackerFaction;
	warInfo[turf][warDefenderFaction] = defenderFaction;

	warInfo[turf][warDefenderScore] = warInfo[turf][warAttackersOnTurf] = warInfo[turf][warDefendersOnTurf] = 0;

	warInfo[turf][warAttackerScore] = 1;

	warInfo[turf][warAttackersOffTurfSeconds] = 7;

	warInfo[turf][warInfluenceTime] = 120;

	warInfo[turf][warLeftTime] = WARS_TIME;

	warInfo[turf][warTimer] = repeat checkWarTimer(turf);

	factionWar[attackerFaction] = factionWar[defenderFaction] = turf;

	va_mysql_tquery("INSERT INTO `server_wars` (`warAttackerFaction`, `warDefenderFaction`, `warStartTime`, `warTurfID`) VALUES (%d, %d, %d, %d)", groupInfo[attackerFaction][factionID], groupInfo[defenderFaction][factionID], gettime(), turfInfo[turf][turfID]);
	
	foreach(new i : vehicleType<VEH_TYPE_GROUP>) {
		if(vehicleInfo[i][vehGroup] != attackerFaction)
			continue;

		SetVehicleVirtualWorld(i, 1000 + warInfo[turf][warID]);
	}

	foreach(new i : vehicleType<VEH_TYPE_GROUP>) {
		if(vehicleInfo[i][vehGroup] != defenderFaction)
			continue;

		SetVehicleVirtualWorld(i, 1000 + warInfo[turf][warID]);
	}

	foreach(new i : factionMembers<attackerFaction>) {
		transferPlayerOnWar(i, turf);
	}
	foreach(new i : factionMembers<defenderFaction>) {
		transferPlayerOnWar(i, turf);
	}

	UseGangZoneCheck(turfInfo[turf][turfGameID], true);
}

static transferPlayerOnWar(const playerid, const turf) {
	if(playerInfo[playerid][pWantedLevel] || Iter_Contains(jailedPlayers, playerid)) 
		return va_SendClientMessage(playerid, COLOR_SERVER, "WAR >>: {ffffff}Your gang is in a war with %s, but you are not transferred in war because you are wanted/jailed.", groupInfo[playerInfo[playerid][pGroup] == warInfo[turf][warAttackerFaction] ? warInfo[turf][warDefenderFaction] : warInfo[turf][warAttackerFaction]][fName]);

	warPlayerTurfTime[playerid] = warPlayerKills[playerid] = warPlayerAssists[playerid] = warPlayerDeaths[playerid] = 0;

	Iter_Add(warPlayers[turf], playerid);
	
	PlayerTextDrawShow(playerid, playerTextdraws[playerid][warTD]);

	setPlayerWarEnvironment(playerid);

	if(playerStaticVar[playerid][var_contract_price] != 0)
		Iter_Remove(serverContracts, playerid);

	if(getPlayerTarget(playerid) != INVALID_PLAYER_ID) {
		new const returnPlayer = getPlayerTarget(playerid);

		cancelPlayerHit(returnPlayer);

		va_SendClientMessage(returnPlayer, COLOR_YELLOW, "* Your contract on %s has been canceled, because the target player is in a war.", GetName(playerid));
	}

	va_mysql_tquery("INSERT INTO `server_users_wars` (`warID`, `warPlayerID`, `warFactionID`) VALUES (%d, %d, %d)", warInfo[factionWar[playerInfo[playerid][pGroup]]][warID], playerInfo[playerid][pSQLID], groupInfo[playerInfo[playerid][pGroup]][factionID]);

	return va_SendClientMessage(playerid, COLOR_SERVER, "WAR >>: {ffffff}You have been transferred in a virtual world because your gang is in war with %s.", groupInfo[playerInfo[playerid][pGroup] == warInfo[turf][warAttackerFaction] ? warInfo[turf][warDefenderFaction] : warInfo[turf][warAttackerFaction]][fName]);
}

static stopWar(const turf, const bool:fail = false) {

	stop warInfo[turf][warTimer];


	foreach(new i : vehicleType<VEH_TYPE_GROUP>) {
		if(vehicleInfo[i][vehGroup] != warInfo[turf][warAttackerFaction])
			continue;

		SetVehicleVirtualWorld(i, 0);
	}

	foreach(new i : vehicleType<VEH_TYPE_GROUP>) {
		if(vehicleInfo[i][vehGroup] != warInfo[turf][warDefenderFaction])
			continue;

		SetVehicleVirtualWorld(i, 0);
	}
	
	foreach(new i : warPlayers[turf]) {
		PlayerTextDrawHide(i, playerTextdraws[i][warTD] );

		if(!player_data[i][playerDeath])
			SpawnPlayer(i);

		if(playerStaticVar[i][var_contract_price] != 0)
			Iter_Add(serverContracts, i);

		savePlayerWarInfo(i);

		SetPlayerColor(i, GetPlayerColor(i) | 0x00000000);

		GangZoneStopFlashForPlayer(i, turfInfo[turf][turfGameID]);

		clearPlayerKillList(i);
	}

	for(new i = 0; i < MAX_PICKUPSW; i ++) {  
		DestroyDynamicPickup(Pickups[i][psID]);
		Pickups[i][psType] = 0;
		Pickups[i][psID] = 0;
		Pickups[i][psWar] = 0;

	}	
	va_mysql_tquery("UPDATE `server_wars` SET `warAttackerScore` = %d, `warDefenderScore` = %d, `warEndTime` = %d WHERE `ID` = %d", warInfo[turf][warAttackerScore], warInfo[turf][warDefenderScore], gettime(), warInfo[turf][warID]);

	UseGangZoneCheck(turfInfo[turf][turfGameID], false);

	
	mysql_format(SQL, returnString, sizeof returnString, 
		"( \
		    SELECT suw.`warPlayerID`, suw.`warFactionID`, suw.`warPlayerKills`, suw.`warPlayerAssists`, suw.`warPlayerDeaths`, suw.`warPlayerKD`, suw.`warPlayerTurfTime`, u.`Name` AS `warPlayerName` \
		    FROM `server_users_wars` suw \
		    JOIN `users` u ON suw.`warPlayerID` = u.`ID` \
		    WHERE suw.`warID` = %d \
		    ORDER BY suw.`warPlayerKD` DESC, suw.`warPlayerTurfTime` DESC LIMIT 1 \
		) \
		UNION ALL \
		( \
		    SELECT suw.`warPlayerID`, suw.`warFactionID`, suw.`warPlayerKills`, suw.`warPlayerAssists`, suw.`warPlayerDeaths`, suw.`warPlayerKD`, suw.`warPlayerTurfTime`, u.`Name` AS `warPlayerName` \
		    FROM `server_users_wars` suw \
		    JOIN `users` u ON suw.`warPlayerID` = u.`ID` \
		    WHERE suw.`warID` = %d \
		    ORDER BY suw.`warPlayerKD` ASC, suw.`warPlayerTurfTime` DESC LIMIT 1 \
		)", warInfo[turf][warID], warInfo[turf][warID]);

	mysql_tquery(SQL, returnString, "getServerWarWorstAndWinner", "dd", turf, fail);
}

static getPlayerTurf(const playerid) {
	new Float:x, Float:y, Float:z;

	GetPlayerPos(playerid, x, y, z);

	for(new i = 0; i < serverStuff[turfs]; ++i) {
		if(isInRectangleArea(x, y, turfInfo[i][turfMinX], turfInfo[i][turfMaxX], turfInfo[i][turfMinY], turfInfo[i][turfMaxY])) 
			return i;
	}

	return -1;
}
YCMD:fvrespawn(playerid, params[], help) {
    if(!playerInfo[playerid][pGroup])
        return sendError(playerid, "Nu faci parte dintr-o factiune.");

	if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_GANG)
		return SendClientMessage(playerid, -1, "Indisponibil.");

    check_delay
    if(playerInfo[playerid][pGroupRank] < 5)
        return sendError(playerid, "Trebuie sa ai minim rank 5 pentru a folosi aceasta comanda.");

    foreach(new vehicleid : vehicleType<VEH_TYPE_GROUP>) {
		if(vehicleInfo[vehicleid][vehGroup] != playerInfo[playerid][pGroup])
			continue;

        if(getVehicleOccupantsCount(vehicleid))
            continue;

        SetVehicleToRespawn(vehicleid);

    }

    set_delay;
    return sendGroupMessage(playerInfo[playerid][pGroup], COLOR_CLIENT, "(Group) {ffffff}%s (%d) has respawned all unused faction vehicles.", GetName(playerid), playerid);
}
static updatePlayerWarTextdraw(const playerid) {
	new const warTurf = getPlayerFactionWar(playerid);

	if(warTurf != -1) {
		
		format(returnString, sizeof returnString, "Turf %d~N~%s %d - %d %s~N~Time left: %s [%d seconds on turf]~N~Kills: %d Assists: %d Deaths: %d", turfInfo[warTurf][turfID], groupInfo[warInfo[warTurf][warAttackerFaction]][fName], warInfo[warTurf][warAttackerScore], warInfo[warTurf][warDefenderScore], groupInfo[warInfo[warTurf][warDefenderFaction]][fName], calculateTime(warInfo[warTurf][warLeftTime]), warPlayerTurfTime[playerid], warPlayerKills[playerid], warPlayerAssists[playerid], warPlayerDeaths[playerid]);
		PlayerTextDrawSetString(playerid, playerTextdraws[playerid][warTD], returnString);
		PlayerTextDrawShow(playerid, playerTextdraws[playerid][warTD]);
		setPlayerWarGangZoneFlash(playerid, warTurf);
	}
}


static savePlayerWarInfo(const playerid) {
	new const warTurf = getPlayerFactionWar(playerid);

	if(warTurf != -1) {
		
		format(returnString, sizeof returnString, "UPDATE `server_users_wars` SET `warPlayerKills` = %d,  `warPlayerAssists` = %d, `warPlayerDeaths` = %d, `warPlayerKD` = %f, `warPlayerTurfTime` = %d WHERE `warPlayerID` = %d AND `warID` = %d", warPlayerKills[playerid], warPlayerAssists[playerid], warPlayerDeaths[playerid], getPlayerWarKDRatio(playerid), warPlayerTurfTime[playerid], playerInfo[playerid][pSQLID], warInfo[warTurf][warID]);
		mysql_tquery(SQL, returnString);
	}
}

getPlayerWarKDRatio(const playerid) {
	return float(warPlayerKills[playerid]) - float(warPlayerDeaths[playerid]);
}

insertPlayerIntoWar(const playerid, const turf) {
	Iter_Add(warPlayers[turf], playerid);
	PlayerTextDrawShow(playerid, playerTextdraws[playerid][warTD]);
}

setPlayerWarEnvironment(const playerid) {

	// new playerGroup = playerInfo[playerid][pGroup];
	// if(playerStaticVar[playerid][inHQ] == 0 && playerStaticVar[playerid][groupDuty] == true) {
	// 	SendClientMessage(playerid, COLOR_SERVER, "WAR >>:{ffffff} War-ul a inceput! Ai fost teleportat automat in hq.");
    //     SetPlayerPos(playerid, factionInterior[playerGroup][0], factionInterior[playerGroup][1], factionInterior[playerGroup][2]);
    //     SetPlayerFacingAngle(playerid, factionInterior[playerGroup][3]);
    //     SetPlayerVirtualWorld(playerid, playerGroup);
    //     SetPlayerInterior(playerid, getHQInterior[playerGroup]);
    //     SetPlayerColor(playerid, GetPlayerColor(playerid) | 0x000000FF);
	// }
	joinWarMessage(playerid);
	SetPlayerVirtualWorld(playerid, 1000 + warInfo[factionWar[playerInfo[playerid][pGroup]]][warID]);

	SetPlayerColor(playerid, GetPlayerColor(playerid) | 0x000000FF);
}

setPlayerWarGangZoneFlash(const playerid, const turf) {
	GangZoneFlashForPlayer(playerid, turfInfo[turf][turfGameID], 0xff4747C8);
}

getPlayerFactionWar(const playerid) {
	if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_GANG) 
		return -1;

	return factionWar[playerInfo[playerid][pGroup]];
}

getFactionWarID(const factionid) {
	if(factionid == -1 || factionWar[factionid] == -1)
		return -1;

	return warInfo[factionWar[factionid]][warID];
} 

getTurfWar(const turf) {
	if(warInfo[turf][warAttackerFaction] == -1)
		return -1;

	return factionWar[warInfo[turf][warAttackerFaction]];
}

isPlayerInWar(const playerid) {
	return getPlayerFactionWar(playerid) != -1;
}

isPlayerOnTurf(const playerid) {
	return playerOnTurf[playerid];
}

// ------------------------------------- //
//            Commands                   //
// ------------------------------------- //

YCMD:attack(playerid, params[], help) {
	if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_GANG)
		return sendError(playerid, "Nu poti folosi aceasta comanda deoarece nu faci parte dintr-o mafie.");

	if(playerInfo[playerid][pGroupRank] < 6)
		return sendError(playerid, "Trebuie sa fii lider sau co-lider pentru a folosi aceasta comanda.");

	if(factionAttackDelay[playerInfo[playerid][pGroup]] > gettime())
		return sendError(playerid, "Nu poti ataca acest turf deoarece factiunea ta trebuie sa astepte %s minute de la ultimul war.", calculateTime(factionAttackDelay[playerInfo[playerid][pGroup]] - gettime()));

	if(factionWar[playerInfo[playerid][pGroup]] != -1)
		return sendError(playerid, "Factiunea ta este implicata deja intr-un war.");

	new const turf = getPlayerTurf(playerid);

	if(turf == -1 || turfInfo[turf][turfFactionID] == playerInfo[playerid][pGroup])
		return sendError(playerid, "Nu esti la turf-ul unui inamic.");

	if(factionWar[turfInfo[turf][turfFactionID]] != -1) 
		return sendError(playerid, "Acea factiune este implicata in alt war.");

	if(!Iter_Count(factionMembers<playerInfo[playerid][pGroup]>) == 1)
		return sendError(playerid,"This mafia don't have members online. 1 ");


	if(!Iter_Count(factionMembers<turfInfo[turf][turfFactionID]>)) 
		return sendError(playerid,"This mafia don't have members online. 2 ");

	startWar(turf, playerInfo[playerid][pGroup], turfInfo[turf][turfFactionID]);
	return 1;
}

// ------------------------------------- //
//            Timers                     //
// ------------------------------------- //
timer checkWarTimer[1000](turf) {
	if(--warInfo[turf][warLeftTime]) {
		foreach(new i : warPlayers[turf]) {
			if(isPlayerOnTurf(i)) 
				warPlayerTurfTime[i] ++;
			
			updatePlayerWarTextdraw(i);
		}
	}
	else {
		stopWar(turf);
		return 1;
	}

	// if(!warInfo[turf][warAttackersOnTurf] && warInfo[turf][warAttackersOffTurfSeconds]) { 
	// 	if(!--warInfo[turf][warAttackersOffTurfSeconds]) {
	// 		stopWar(turf, true);
	// 		return 1;
	// 	}
	// }

	if(!--warInfo[turf][warInfluenceTime]) {
		if(warInfo[turf][warDefendersOnTurf] > warInfo[turf][warAttackersOnTurf]) {
			warInfo[turf][warDefenderScore] ++;

			foreach(new i : warPlayers[turf]) {
				va_SendClientMessage(i, COLOR_SERVER, "WAR >>: {ffffff}Group %s has received +1 point for influence because they have more gangsters on turf.", groupInfo[warInfo[turf][warDefenderFaction]][fName]);
			}
		}
		else if(warInfo[turf][warAttackersOnTurf] > warInfo[turf][warDefendersOnTurf]) {
			warInfo[turf][warAttackerScore] ++;

			foreach(new i : warPlayers[turf]) {
				va_SendClientMessage(i, COLOR_SERVER, "WAR >>: {ffffff}Group %s has received +1 point for influence because they have more gangsters on turf.", groupInfo[warInfo[turf][warAttackerFaction]][fName]);
			}
		}

		warInfo[turf][warInfluenceTime] = 120;
	}
	return 1;
}

// ------------------------------------- //
//            Callbacks                  //
// ------------------------------------- //

hook OnGameModeInit()
{
	// usa mafii 
	CreateDynamicObject(1506, 2544.06641, -1306.29781, 1053.63586,   0.00000, 0.00000, 90.00000);
	CreateDynamicObject(1506, 2544.06641, -1304.79781, 1053.63586,   0.00000, 0.00000, 90.00000);

	Command_AddAltNamed("fvrespawn", "fvr");
	return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
	if(damagedid == INVALID_PLAYER_ID)
		return Y_HOOKS_CONTINUE_RETURN_1;

	if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_GANG || factionType[playerInfo[damagedid][pGroup]] != FACTION_TYPE_GANG)
		return Y_HOOKS_CONTINUE_RETURN_1;

	if(getPlayerTurf(playerid) != factionWar[playerInfo[playerid][pGroup]] || getPlayerTurf(playerid) == -1)
		return Y_HOOKS_CONTINUE_RETURN_1;

	if(playerInfo[playerid][pGroup] == playerInfo[damagedid][pGroup] && factionWar[playerInfo[playerid][pGroup]] && getPlayerFactionWar(playerid) != -1 && getPlayerFactionWar(damagedid) != -1)
	{
		amount = 0;
		return Y_HOOKS_CONTINUE_RETURN_1;
	}

	playerStaticVar[playerid][assistDamage][damagedid] += amount;

	if(playerStaticVar[playerid][assistDamage][damagedid] > 30)
		playerStaticVar[playerid][gotAssistFrom][damagedid] = playerid;

	PlayerPlaySound(playerid, 17802, 0.0, 0.0, 0.0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}
function:onPlayerWarDeath(const playerid, const killerid, const reason) {
	if(killerid != INVALID_PLAYER_ID) {
		new const warTurf = getPlayerFactionWar(playerid);

		if(warTurf != -1 && isPlayerOnTurf(killerid) && warTurf == getPlayerFactionWar(killerid)) {

			warPlayerDeaths[playerid] ++;
			warPlayerKills[killerid] ++;

			playerInfo[killerid][pRaportScore][0] ++;
			playerInfo[playerid][pRaportScore][1] ++;
	
			if(playerInfo[killerid][pGroup] == turfInfo[warTurf][turfFactionID])
				warInfo[warTurf][warDefenderScore] ++;
			else 
				warInfo[warTurf][warAttackerScore] ++;

			new Float: Pos[3], idd;
			GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);

			idd = CheckIDEmpty();
			Pickups[idd][psID] = CreateDynamicPickup(1241, 23, Pos[0] + random(4), Pos[1] + random(4), Pos[2], GetPlayerVirtualWorld(playerid));
			Pickups[idd][psType] = 0;
			Pickups[idd][psWar] = warTurf;


			va_SendClientMessage(playerid, COLOR_SERVER, "WAR >>: {ffffff}You have been killed by %s (%d) from %0.1fm with %s.", GetName(killerid), killerid, GetDistanceBetweenPlayers(playerid, killerid), weapName(reason));
			va_SendClientMessage(killerid, COLOR_SERVER, "WAR >>: {ffffff}You killed %s (%d) from %0.1fm with %s.", GetName(playerid), playerid, GetDistanceBetweenPlayers(playerid, killerid), weapName(reason));

			new returnBeforeName[MAX_PLAYERS];
			GetPlayerName(killerid, returnBeforeName, sizeof returnBeforeName);
			foreach(new i : warPlayers[warTurf])
			{
				if(playerStaticVar[i][assistDamage][playerid] > 30 && playerStaticVar[i][gotAssistFrom][playerid] != killerid)
				{
					va_SendClientMessage(i, COLOR_SERVER, "WAR >>: {ffffff}You assisted %s (%d) in killing %s (%d) [%0.2f dmg] ({00ff00}+1{ffffff} assist).", GetName(killerid), killerid, GetName(playerid), playerid, playerStaticVar[i][assistDamage][playerid]);
					warPlayerAssists[i] ++;

					new returnPlayerName[MAX_PLAYER_NAME];
					format(returnPlayerName, sizeof returnPlayerName, "%s + %s", GetName(killerid), GetName(i)); 
					
					SetPlayerName(killerid, returnPlayerName);

					playerStaticVar[i][assistDamage][playerid] = 0;
					playerStaticVar[i][gotAssistFrom][playerid] = INVALID_PLAYER_ID;
					break;
				}
			}

			foreach(new i : warPlayers[warTurf])
				SendDeathMessageToPlayer(i, killerid, playerid, reason);

			SetPlayerName(killerid, returnBeforeName);

		}
	}

	return 1;
}
// ------------------------------------- //
//            Hooks                      //
// ------------------------------------- //

hook OnPlayerPickUpDynamicPickup(playerid, pickupid) {

	for(new i = 0; i < MAX_PICKUPSW; i ++) {		

		if(pickupid == Pickups[i][psID]) {

			if(Pickups[i][psType] == 0) {

				new Float: HP;
				GetPlayerHealth(playerid, HP);
				if(HP > 89)
					return SendClientMessage(playerid, -1, "You cannot pick-up this pill because you have more than 89 hp.");

				gString[0] = (EOS);
				format(gString, sizeof gString, "%s [%d] used a pill to restore his health (+20 HP).", GetName(playerid), playerid);
				ProxDetector(30.0, playerid, gString, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
				SetPlayerHealthEx(playerid, HP + 20);
			}	

			DestroyDynamicPickup(Pickups[i][psID]);
			Pickups[i][psType] = 0;
			Pickups[i][psID] = 0;
			Pickups[i][psWar] = 0;

		}	

	}
    return 1;
}
hook OnPlayerEnterGangZone(playerid, zoneid) {
	if(!playerInfo[playerid][pWantedLevel] && !Iter_Contains(jailedPlayers, playerid)) {
		new const warTurf = getPlayerFactionWar(playerid);

		if(warTurf != -1 && turfInfo[warTurf][turfGameID] == zoneid) {
			SendClientMessage(playerid, COLOR_CLIENT, "You have entered on turf.");

			PlayerPlaySound(playerid, 17802, 0.0, 0.0, 0.0);

			playerOnTurf[playerid] = true;

			if(playerInfo[playerid][pGroup] == warInfo[warTurf][warAttackerFaction]) {
				warInfo[warTurf][warAttackersOnTurf] ++;

				warInfo[warTurf][warAttackersOffTurfSeconds] = -1;
			}
			else {
				warInfo[warTurf][warDefendersOnTurf] ++;
			}
		}
	}
	return 1;
}

hook OnPlayerLeaveGangZone(playerid, zoneid) {
	if(!playerInfo[playerid][pWantedLevel] && !Iter_Contains(jailedPlayers, playerid)) {
		new const warTurf = getPlayerFactionWar(playerid);

		if(warTurf != -1 && turfInfo[warTurf][turfGameID] == zoneid) {
			SendClientMessage(playerid, COLOR_CLIENT, "You left the turf.");

			playerOnTurf[playerid] = false;

			if(playerInfo[playerid][pGroup] == warInfo[warTurf][warAttackerFaction]) {
				if(!--warInfo[warTurf][warAttackersOnTurf]) 
					warInfo[warTurf][warAttackersOffTurfSeconds] = 7;
			}
			else {
				warInfo[warTurf][warDefendersOnTurf] --;
			}
		}
	}
	return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
	if(!playerInfo[playerid][pWantedLevel] && !Iter_Contains(jailedPlayers, playerid)) {
		new const warTurf = getPlayerFactionWar(playerid);

		if(warTurf != -1) {
			savePlayerWarInfo(playerid);

			Iter_Remove(warPlayers[warTurf], playerid);	

			if(playerOnTurf[playerid]) {
				if(playerInfo[playerid][pGroup] == warInfo[warTurf][warAttackerFaction]) {
					if(!--warInfo[warTurf][warAttackersOnTurf]) 
						warInfo[warTurf][warAttackersOffTurfSeconds] = 7;
				}
				else {
					warInfo[warTurf][warDefendersOnTurf] --;
				}
			}
		}
	}

	playerOnTurf[playerid] = false;
	return 1;
}

hook OnPlayerConnect(playerid) {
	playerTextdraws[playerid][warTD] = CreatePlayerTextDraw(playerid, 558.500000, 272.133362, "loading..");
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][warTD], 0.229499, 0.946665);
	PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][warTD], 0.000000, 155.000000);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][warTD], 2);
	PlayerTextDrawColour(playerid, playerTextdraws[playerid][warTD], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][warTD], 1);
	PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][warTD], 0x000000FF);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][warTD], 0);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][warTD], 0);
	PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][warTD], 0x000000FF);
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][warTD], 1);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][warTD], 1);
	PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][warTD], 0);
	warPlayerKills[playerid] = warPlayerDeaths[playerid] = warPlayerTurfTime[playerid] = 0; warPlayerAssists[playerid] = 0;
	return 1;
}