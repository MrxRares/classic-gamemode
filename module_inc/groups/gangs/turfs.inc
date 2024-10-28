// ------------------------------------- //
//            Includes                   //
// ------------------------------------- //

#include <YSI_Coding\y_hooks>

#include <beaZone>

// ------------------------------------- //
//            Macros                     //
// ------------------------------------- //

#define MAX_TURFS 54

// ------------------------------------- //
//            Enums                      //
// ------------------------------------- //

enum e_turfInfo {
	turfID, 
	turfGameID, 
	turfFactionID,

	Float:turfMinX,
	Float:turfMinY,
	Float:turfMaxX,
	Float:turfMaxY
}

// ------------------------------------- //
//            Variables                  //
// ------------------------------------- //

new 
	turfInfo[MAX_TURFS][e_turfInfo],

	bool:playerTurfsShow[MAX_PLAYERS],
	playerTurfsShowDelay[MAX_PLAYERS],

	Iterator:showTurfs<MAX_PLAYERS>;

// ------------------------------------- //
//          Load Function                //
// ------------------------------------- //

function:load_turfs() {
	serverStuff[turfs] = cache_num_rows();

	if(!serverStuff[turfs])
		return print("[MYSQL INFO] No turfs were found in the database.");

	GangZoneCreate(0.0, 0.0, 0.0, 0.0);

	for(new i = 0; i < serverStuff[turfs]; ++i) {
		cache_get_value_name_int(i, "ID", turfInfo[i][turfID]);

		cache_get_value_name_int(i, "FactionID", turfInfo[i][turfFactionID]);

		cache_get_value_name_float(i, "minX", turfInfo[i][turfMinX]);
		cache_get_value_name_float(i, "minY", turfInfo[i][turfMinY]);
		cache_get_value_name_float(i, "maxX", turfInfo[i][turfMaxX]);
		cache_get_value_name_float(i, "maxY", turfInfo[i][turfMaxY]);

		turfInfo[i][turfGameID] = GangZoneCreateEx(turfInfo[i][turfMinX], turfInfo[i][turfMinY], turfInfo[i][turfMaxX], turfInfo[i][turfMaxY], turfInfo[i][turfID], 1.0);
	}

	return printf("Gang Turfs: %d", serverStuff[turfs]);
}

// ------------------------------------- //
//            Functions                  //
// ------------------------------------- //

showPlayerTurfs(const playerid) {
	playerTurfsShow[playerid] = true;

	Iter_Add(showTurfs, playerid);

	for(new i = 0; i < serverStuff[turfs]; ++i) {
		GangZoneShowForPlayerEx(playerid, turfInfo[i][turfGameID], turfs_colors[turfInfo[i][turfFactionID]] | 0x000000C8);

		if(getTurfWar(i) != -1) {
			setPlayerWarGangZoneFlash(playerid, i);
		}
	}
}

hidePlayerTurfs(const playerid) {
	playerTurfsShow[playerid] = false;

	Iter_Remove(showTurfs, playerid);

	for(new i = 0; i < serverStuff[turfs]; ++i) {
		GangZoneHideForPlayerEx(playerid, turfInfo[i][turfGameID]);
	}
}

// ------------------------------------- //
//            Commands                   //
// ------------------------------------- //

YCMD:turfs(playerid, params[], help) {
	if(playerTurfsShowDelay[playerid] > gettime())
		return sendError(playerid, "Trebuie sa astepti %d secunde inainte de a folosi aceasta comanda.", playerTurfsShowDelay[playerid] - gettime());

	if(!playerTurfsShow[playerid]) {
		SendClientMessage(playerid, -1, "Gangs turfs are now visible.");

		showPlayerTurfs(playerid);
	}
	else {
		SendClientMessage(playerid, -1, "Gangs turfs are now hidden.");

		hidePlayerTurfs(playerid);

		playerTurfsShowDelay[playerid] = gettime() + 5;
	}
	return 1;
}

YCMD:top(playerid, params[], help) {
	new gangTurfs[MAX_FACTIONS];

	for (new i = 0; i < MAX_TURFS; ++i) {

		new futinmata = turfInfo[i][turfFactionID];
		if (futinmata >= 0 && futinmata < MAX_FACTIONS) {
			if(factionType[futinmata] != FACTION_TYPE_GANG)
				continue;
		
			gangTurfs[futinmata]++;
		}
	}

	SendClientMessage(playerid, COLOR_TEAL, "----- GANGS TOP -----");

	for (new i = 0; i < MAX_FACTIONS; ++i) {

		if (gangTurfs[i] > 0) {
			va_SendClientMessage(playerid, COLOR_WHITE, "%s - %d turfs", groupInfo[i][fName], gangTurfs[i]);
		}
	}
	
	return 1;
}


// ------------------------------------- //
//            Hooks                      //
// ------------------------------------- //

hook OnPlayerConnect(playerid) {
	playerTurfsShow[playerid] = false;

	playerTurfsShowDelay[playerid] = -1;
	return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
	if(playerTurfsShow[playerid]) 
		Iter_Remove(showTurfs, playerid);

	return 1;
}