// ------------------------------------- //
//              Includes                 //
// ------------------------------------- //

#include <YSI_Coding\y_hooks>

// ------------------------------------- //
//              Constants                //
// ------------------------------------- //
#define MAX_PAINT 5
#define MAX_PAINT_MAPS 5
#define MAX_PAINT_GUNS 3

#define PAINT_RC 0 
#define PAINT_TENNIS 1
#define PAINT_FARM 2
#define PAINT_BEACH 3 
#define PAINT_LSPD 4

#define GUNGAME_LAST_LEVEL 13
#define LAST_WEAPON 4

#define PAINT_INTERIOR_RC 10

#define PAINT_POS_X 1154.4122
#define PAINT_POS_Y -1440.0548
#define PAINT_POS_Z 15.7969

#define TYPE_CLASSIC_ARMOUR 1
#define TYPE_GUNGAME 2
#define TYPE_DEAGLE_ONLY 3

// ------------------------------------- //
//             Enums 	                 //
// ------------------------------------- //

static enum e_paintVars {
	paintID, paintName[40], paintMaxPlayers, paintTimer, paintVirtual, paintArena, paintMapVotes[MAX_PAINT_MAPS], paintGunVotes[MAX_PAINT_GUNS], paintVotedWeapon, bool:paintMatchStarted, Timer:paintMatchTimer, Text:paintTextdraw,
	paintLeadKills, paintLeader
};

// ------------------------------------- //
//             Arrays                    //
// ------------------------------------- //

static paintVars[][e_paintVars] = {
	{1, "Classic / Free For All", 20, 30, 888},
	{2, "Classic + Armour", 20, 30, 889},
	{3, "Gun Game", 20, 30, 890},
	{4, "Deagle Only", 20, 30, 891}
};

static const Float:paintbalRCLocations[7][3] = {
	{-969.7932, 1094.1221, 1345.0070},
	{-976.3990, 1022.5371, 1345.0310},
	{-973.6447, 1041.8181, 1352.6875},
	{-1014.5994, 1027.9786, 1342.6187},
	{-1133.7423, 1021.9453, 1345.7335},
	{-1115.6410, 1095.8212, 1341.8438},
	{-1133.1267, 1079.1073, 1353.4480}
};

static const Float:paintballTennisLocations[7][3] = {
	{-2754.8149, -245.1009, 7.1875},
	{-2779.4072, -248.5145, 7.1875},
	{-2779.9651, -265.2720, 7.1875},
	{-2744.6108, -270.1563, 7.1875},
	{-2718.3101, -252.3627, 7.1875},
	{-2726.9282, -240.9186, 7.1875},
	{-2716.3669, -269.9250, 7.1875}
};

static const Float:paintballFarmLocations[7][3] = {
	{15.3401, 2.1944, 3.1172},
	{-59.7064, -32.2439, 3.1172},
	{-102.5952, -29.0223, 3.1172},
	{-111.9006, 27.3635, 3.1172},
	{-152.7778, 49.3966, 3.1172},
	{-205.3790, 8.9561, 3.1172},
	{-75.0158, 9.1256, 3.1172}
};

static const Float:paintballBeachLocations[6][3] = {
	{586.0002, -1813.6575, 6.0625},
	{600.9373, -1876.1948, 4.2792},
	{595.5770, -1815.2834, 6.0625},
	{560.8715, -1877.9070, 3.9158},
	{498.6968, -1855.5299, 3.7908},
	{429.4129, -1844.7195, 3.7809}
};

static const Float:paintballLSPDLocations[7][3] = {
	{1499.7970, -1671.6659, 14.0469},
	{1459.4235, -1699.1788, 14.5469},
	{1491.1667, -1724.2849, 13.5469},
	{1519.1375, -1741.3630, 13.5469},
	{1501.7247, -1609.9181, 14.0469},
	{1457.3564, -1623.7700, 14.7891},
	{1483.3802, -1631.9480, 14.1484}
};

static paintballNames[5][] = {
	"rc_arena", 
	"tennis", 
	"farm", 
	"beach", 
	"lspd"
};

static paintSkins[5] = {
	250, 96, 158, 97, 285
};

static paintGungameWeapons[13] = {
	22, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 4
};

static paintballGuns[3][] = {
	"Deagle, M4, Grenade & Spray",
	"Baseball Bat",
	"Chainsaw"
};

// ------------------------------------- //
//             Variables                 //
// ------------------------------------- //

static 
	Iterator:paintPlayers[MAX_PAINT]<MAX_PLAYERS>,
	playerVotedPaintMap[MAX_PLAYERS],
	playerVotedPaintGuns[MAX_PLAYERS],
	playerPaintID[MAX_PLAYERS],
    playerPaintKills[MAX_PLAYERS];

// ------------------------------------- //
//             Functions                 //
// ------------------------------------- //

static playerEnterPaint(const playerid, const id) {
	if(Iter_Count(paintPlayers[id]) >= paintVars[id][paintMaxPlayers])
		return sendError(playerid, "Aceasta arena a atins numarul maxim de jucatori.");

	Iter_Add(paintPlayers[id], playerid);
	playerPaintID[playerid] = id;

	playerStaticVar[playerid][afkTime] = 0;

	sendPaintMessage(id, COLOR_YELLOW, "%s has joined the paintball arena.", GetName(playerid));

	SetPlayerColor(playerid, 0xff6464FF);

	TextDrawShowForPlayer(playerid, paintVars[id][paintTextdraw]);

	if(Iter_Count(paintPlayers[id]) == 1) {
		TextDrawSetString(paintVars[id][paintTextdraw], "WAITING FOR PLAYERS...");

		GameTextForPlayer(playerid, "~y~NOT ENOUGH PLAYERS TO START...", 5000, 5);
	}
	else if(Iter_Count(paintPlayers[id]) == 2) {
		paintVars[id][paintTimer] = 30;

		gString[0] = EOS;
		format(gString, sizeof gString, "PAINTBALL ROUND STARTS IN: ~y~%s", calculateTime(paintVars[id][paintTimer]));
		TextDrawSetString(paintVars[id][paintTextdraw], gString);

		if(paintVars[id][paintMatchTimer] == Timer: 0)
			paintVars[id][paintMatchTimer] = repeat checkPlayersPaint(id);
	}

	ResetPlayerWeapons(playerid);
	return setPlayerPaintBallPos(playerid);
}

static getPaintMostVotedArena(id) {
	new 
	    voteScore = 0, 
	    voteMap = 0;

	for(new i = 0; i < sizeof paintballNames; i++) {
		if(paintVars[id][paintMapVotes][i] > voteScore) {
			voteScore = paintVars[id][paintMapVotes][i];
			voteMap = i;
		}
	}

	return voteMap;
}

static getPaintWinner(id) {
    new 
        paintScore = 0, 
        bestID = INVALID_PLAYER_ID;

    foreach(new i : paintPlayers[id]) {
        if(playerPaintKills[i] > paintScore) {
            paintScore = playerPaintKills[i];
            bestID = i;
        }
    }

    return bestID;
}

static getPaintMostVotedGuns(id) {
	new 
	    voteScore = 0, 
	    voteWeapon = 0;

	for(new i = 0; i < sizeof paintballGuns; i++) {
		if(paintVars[id][paintGunVotes][i] > voteScore) {
			voteScore = paintVars[id][paintGunVotes][i];
			voteWeapon = i;
		}
	}
	return voteWeapon;
}

static GivePlayerPaintBallWeapons(playerid) {
	switch(paintVars[playerPaintID[playerid]][paintVotedWeapon]) {
		case 0: {
			GivePlayerWeapon(playerid, 16, 1);
			GivePlayerWeapon(playerid, 24, 500);
			GivePlayerWeapon(playerid, 31, 500);
			GivePlayerWeapon(playerid, 41, 500);
		}
		case 1: {
			GivePlayerWeapon(playerid, 5, 1); 
		}
		case 2: {
			GivePlayerWeapon(playerid, 9, 500); 
		}
	}
	return 1;
}

static matchPaintFinished(id, winnerID) {
	gString[0] = EOS;
	format(gString, sizeof gString, "Round over! Winner: %s with %d kills.", GetName(winnerID), playerPaintKills[winnerID]);
	resetPaintVars(id);

	foreach(new i : paintPlayers[id]) {
		playerVotedPaintMap[i] = -1;
		playerVotedPaintGuns[i] = -1;
		playerPaintKills[i] = 0;
		ResetPlayerWeapons(i);

		SendClientMessage(i, COLOR_YELLOW, gString);
	}
	return 1;
}

static GiveGungameWeapons(playerid) {
	return GivePlayerWeapon(playerid, paintGungameWeapons[playerPaintKills[playerid] / 2], 9999);
}

static resetPaintVars(id) {
	memset(paintVars[id][paintMapVotes], 0);
	memset(paintVars[id][paintGunVotes], 0);
	paintVars[id][paintLeader] = INVALID_PLAYER_ID;
	paintVars[id][paintLeadKills] = 0;
	paintVars[id][paintMatchStarted] = false;
	return 1;	
}

static paintPlayerVotedMap(const playerid) 
	return playerVotedPaintMap[playerid] != -1;

static paintPlayerVotedGuns(const playerid)
    return playerVotedPaintGuns[playerid] != -1;

playerLeavePaint(const playerid) {
	new const id = playerPaintID[playerid];
	new playerGroup = playerInfo[playerid][pGroup];

    playerPaintID[playerid] = -1;

    sendPaintMessage(id, COLOR_YELLOW, "%s has left the paintball arena.", GetName(playerid));

	Iter_Remove(paintPlayers[id], playerid);

	SetPlayerVirtualWorld(playerid, 0);
	SetPlayerInterior(playerid, 0);
	SetPlayerHealth(playerid, 100.0);
	SetPlayerArmour(playerid, 0.0);
	SetPlayerWorldBounds(playerid, 20000.0000, -20000.0000, 20000.0000, -20000.0000);
	
	if(playerInfo[playerid][pGroup] != -1) {
		if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_POLICE) {
			SetPlayerSkin(playerid, faction_skins[playerGroup][playerInfo[playerid][pGroupRank] - 1]);
			setPlayerFactionColor(playerid, playerGroup);
		}
		else {
			SetPlayerSkin(playerid, playerInfo[playerid][pSkin]);
			SetPlayerColor(playerid, COLOR_WHITE_T);
		}
	}
	else {
		SetPlayerSkin(playerid, playerInfo[playerid][pSkin]);
		SetPlayerColor(playerid, COLOR_WHITE_T);
	}

	TextDrawHideForPlayer(playerid, paintVars[id][paintTextdraw]);

	if(paintPlayerVotedMap(playerid)) {
		paintVars[id][paintMapVotes][playerVotedPaintMap[playerid]] --;
		playerVotedPaintMap[playerid] = -1;
 	}

 	if(paintPlayerVotedGuns(playerid)) {
 		paintVars[id][paintGunVotes][playerVotedPaintGuns[playerid]] --;
 		playerVotedPaintGuns[playerid] = -1;
 	}

 	if(paintVars[id][paintLeader] == playerid) {
 		new const leaderID = getPaintWinner(id);

 		paintVars[id][paintLeader] = leaderID;

 		if(leaderID != INVALID_PLAYER_ID) 
 		    paintVars[id][paintLeadKills] = playerPaintKills[leaderID];
 		
 		else 
 			paintVars[id][paintLeadKills] = 0;
 	}

 	playerPaintKills[playerid] = 0;

 	if(!Iter_Count(paintPlayers[id])) {
		resetPaintVars(id);

		TextDrawSetString(paintVars[id][paintTextdraw], "WAITING FOR PLAYERS...");

		stop paintVars[id][paintMatchTimer];
	}
	else if(Iter_Count(paintPlayers[id]) == 1) {
		new const lastPlayer = Iter_First(paintPlayers[id]);

		TextDrawSetString(paintVars[id][paintTextdraw], "WAITING FOR PLAYERS...");

		GameTextForPlayer(lastPlayer, "~y~NOT ENOUGH PLAYERS TO START...", 5000, 5);

		stop paintVars[id][paintMatchTimer];
	}

	clearPlayerKillList(playerid);
	ResetPlayerWeapons(playerid);

	return SetPlayerPos(playerid, PAINT_POS_X, PAINT_POS_Y, PAINT_POS_Z);
}

setPlayerPaintBallPos(const playerid) {
	switch(paintVars[playerPaintID[playerid]][paintArena]) {
		case PAINT_RC: {
			new const rand = random(sizeof(paintbalRCLocations));

			SetPlayerPos(playerid, paintbalRCLocations[rand][0], paintbalRCLocations[rand][1], paintbalRCLocations[rand][2]);
		}
		case PAINT_TENNIS: {
			new const rand = random(sizeof(paintballTennisLocations));

			SetPlayerPos(playerid, paintballTennisLocations[rand][0], paintballTennisLocations[rand][1], paintballTennisLocations[rand][2]);
		
			SetPlayerWorldBounds(playerid, -2712.4683, -2792.2351,-229.3094, -274.6794);
		}
		case PAINT_FARM: {
			new const rand = random(sizeof(paintballFarmLocations));

			SetPlayerPos(playerid, paintballFarmLocations[rand][0], paintballFarmLocations[rand][1], paintballFarmLocations[rand][2]);

			SetPlayerWorldBounds(playerid, 135.3258, -287.9915, 57.6137 ,-74.2723);
		}
		case PAINT_BEACH: {
			new const rand = random(sizeof(paintballBeachLocations));

			SetPlayerPos(playerid, paintballBeachLocations[rand][0], paintballBeachLocations[rand][1], paintballBeachLocations[rand][2]);

			SetPlayerWorldBounds(playerid, 708.0417, 409.4930, -1806.0677, -1907.7119);
		}
		case PAINT_LSPD: {
			new const rand = random(sizeof(paintballLSPDLocations));

			SetPlayerPos(playerid, paintballLSPDLocations[rand][0], paintballLSPDLocations[rand][1], paintballLSPDLocations[rand][2]);

			SetPlayerWorldBounds(playerid, 1558.4261, 1387.0891, -1578.9333, -1776.6854);
		}
	}

	if(paintVars[playerPaintID[playerid]][paintMatchStarted]) {
		SetPlayerColor(playerid, 0xff6464FF);

		switch(playerPaintID[playerid]) {
			case TYPE_DEAGLE_ONLY: {
				GivePlayerWeapon(playerid, 24, 9999);
			}
			case TYPE_GUNGAME: {
				GiveGungameWeapons(playerid);
			}
			default: {
				GivePlayerPaintBallWeapons(playerid);
			}
		}
	}

	SetPlayerHealth(playerid, 100.0);

	if(paintVars[playerPaintID[playerid]][paintID] == 2) 
		SetPlayerArmour(playerid, 100.0);

	if(paintVars[playerPaintID[playerid]][paintArena] == PAINT_RC)
		SetPlayerInterior(playerid, PAINT_INTERIOR_RC);

	else 
		SetPlayerInterior(playerid, 0);

	SetPlayerSkin(playerid, paintSkins[paintVars[playerPaintID[playerid]][paintArena]]);

	return SetPlayerVirtualWorld(playerid, paintVars[playerPaintID[playerid]][paintVirtual]);
}

sendPaintMessage(const id, const color, const text[], va_args<>) {
	new
	   message[144];

	va_format(message, sizeof message, text, va_start<3>);
	
	foreach(new i : paintPlayers[id]) 
		SendClientMessage(i, color, message);
	  
	return 1;
} 

isPlayerInPaint(const playerid) 
	return playerPaintID[playerid] != -1;

getPlayerPaintID(const playerid)
	return playerPaintID[playerid];

getPlayerPaintKills(const playerid)
	return playerPaintKills[playerid];

// ------------------------------------- //
//             Commands                  //
// ------------------------------------- //

YCMD:paint(playerid, params[], help) {
	if(!IsPlayerInRangeOfPoint(playerid, 3.0, PAINT_POS_X, PAINT_POS_Y, PAINT_POS_Z))
		return sendError(playerid, "Nu esti in locatia potrivita.");

	if(job_data[playerid][jobDuty] != JOB_TYPE_NONE)
		return sendError(playerid, "Nu poti folosi aceasta comanda in timp ce muncesti.");

	if(playerInfo[playerid][pLevel] < 5)
		return sendError(playerid, "Trebuie sa ai minim level 5.");

	if(playerInfo[playerid][pGunLic] <= 0)
		return sendError(playerid, "Trebuie sa detii licenta de arme.");

	if(isPlayerInPaint(playerid))
		return sendError(playerid, "Esti deja in arena de paint.");

	if(isPlayerUndercover(playerid))
		return sendError(playerid, "Nu poti intra deoarece esti undercover.");

	if(factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_POLICE && playerStaticVar[playerid][groupDuty])
        return sendError(playerid, "Nu poti intra in arena de paint daca esti la datorie.");

    if(playerInfo[playerid][pWantedLevel]) 
    	return sendError(playerid, "Nu poti face asta deoarece esti urmarit de politie.");

    gDialog = "Arena\tPlayers\tMap\n";

    for(new i = 0; i < sizeof paintVars; ++i) 
    	format(gDialog, sizeof gDialog, "%s{FFFFFF}%s\t%d/%d\t{1CE007}%s\n", gDialog, paintVars[i][paintName], Iter_Count(paintPlayers[i]), paintVars[i][paintMaxPlayers], paintballNames[paintVars[i][paintArena]]); 

 	Dialog_Show(playerid, DIALOGPAINT, DIALOG_STYLE_TABLIST_HEADERS, "SERVER: Choose Arena", gDialog, "Select", "Cancel");
    return true;
}

YCMD:paintleave(playerid, params[], help) {
	if(player_data[playerid][playerDeath])
		return 1;

	if(!isPlayerInPaint(playerid))
		return sendError(playerid, "Nu esti in paint.");

	playerLeavePaint(playerid);
	return true;
}

YCMD:votemap(playerid, params[], help) {
	if(!isPlayerInPaint(playerid))
		return sendError(playerid, "Nu esti in paintball.");

	if(paintPlayerVotedMap(playerid))
		return sendError(playerid, "Ai votat deja.");

	if(paintVars[playerPaintID[playerid]][paintMatchStarted])
		return sendError(playerid, "Meciul este deja in desfasurare.");

	if(paintVars[playerPaintID[playerid]][paintID] == 3)
		return sendError(playerid, "Nu poti vota mapa la gungame.");
 	Dialog_Show(playerid, PAINTVOTEMAP, DIALOG_STYLE_LIST, "SERVER: Vote Paintball Map", "%s\n%s\n%s\n%s\n%s", "Vote", "Cancel", paintballNames[0], paintballNames[1], paintballNames[2], paintballNames[3], paintballNames[4]);
	return true;
}

YCMD:votegun(playerid, params[], help) {
	if(!isPlayerInPaint(playerid))
		return sendError(playerid, "Nu esti in paintball.");

	if(paintPlayerVotedGuns(playerid))
		return sendError(playerid, "Ai votat deja.");

	if(paintVars[playerPaintID[playerid]][paintMatchStarted])
		return sendError(playerid, "Meciul este deja in desfasurare.");

	if(paintVars[playerPaintID[playerid]][paintID] == 3 || paintVars[playerPaintID[playerid]][paintID] == 4)
		return sendError(playerid, "Nu poti vota armele la acest tip de paint.");

	Dialog_Show(playerid, PAINTVOTEGUNS, DIALOG_STYLE_LIST, "SERVER: Vote Paintball Guns", "%s\n%s\n%s", "Vote", "Cancel", paintballGuns[0], paintballGuns[1], paintballGuns[2]);
    return true;
}

YCMD:gotopaint(playerid, params[], help) {
    if(!playerInfo[playerid][pAdmin])
        return sendError(playerid, "Nu ai gradul administrativ necesar pentru a folosi aceasta comanda.");

    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);

    if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
    	new const vehicleid = GetPlayerVehicleID(playerid);

        SetVehiclePos(vehicleid, 1154.4122, -1440.0548, 15.7969);

        SetVehicleVirtualWorld(vehicleid, 0);
        LinkVehicleToInterior(vehicleid, 0);
        return 1;
    }
	SetPlayerPos(playerid, 1154.4122, -1440.0548, 15.7969);
    return true;
} 

// ------------------------------------- //
//             Dialogs                   //
// ------------------------------------- //

Dialog:PAINTVOTEMAP(playerid, response, listitem, inputtext[]) {
	if(!response || playerPaintID[playerid] == -1)
		return 1;

    new id = playerPaintID[playerid];

    paintVars[id][paintMapVotes][listitem] ++;

    playerVotedPaintMap[playerid] = listitem;

    return sendPaintMessage(id, COLOR_YELLOW, "%s has voted for the map: [%s] and now it has [%d] votes! [/votemap]", GetName(playerid), paintballNames[listitem], paintVars[id][paintMapVotes][listitem]);
}

Dialog:PAINTVOTEGUNS(playerid, response, listitem, inputtext[]) {
	if(!response || playerPaintID[playerid] == -1)
		return 1;

	new id = playerPaintID[playerid];

	paintVars[id][paintGunVotes][listitem] ++;

	playerVotedPaintGuns[playerid] = listitem;

	return sendPaintMessage(id, COLOR_YELLOW, "%s has voted for: [%s] and now it has [%d] votes! [/votegun]", GetName(playerid), paintballGuns[listitem], paintVars[id][paintGunVotes][listitem]);
}

Dialog:DIALOGPAINT(playerid, response, listitem, inputtext[]) {
	if(!response || playerPaintID[playerid] != -1)
		return 1;

	return playerEnterPaint(playerid, listitem);
}

// ------------------------------------- //
//             Timers                    //
// ------------------------------------- //

timer checkPlayersPaint[1000](id) {
	paintVars[id][paintTimer] --;

	if(!paintVars[id][paintMatchStarted]) {
		if(paintVars[id][paintTimer]) {
			gString[0] = EOS;
			format(gString, sizeof gString, "PAINTBALL ROUND STARTS IN: ~y~%s", calculateTime(paintVars[id][paintTimer]));
			TextDrawSetString(paintVars[id][paintTextdraw], gString);
		}	
		else {
			paintVars[id][paintMatchStarted] = true;

			if(paintVars[id][paintID] != 3)
		     	paintVars[id][paintTimer] = 300;
    
          	paintVars[id][paintArena] = getPaintMostVotedArena(id);
		    paintVars[id][paintVotedWeapon] = getPaintMostVotedGuns(id);

		    paintVars[id][paintLeader] = INVALID_PLAYER_ID;
		    paintVars[id][paintLeadKills] = 0;

			foreach(new i : paintPlayers[id]) {
				SpawnPlayer(i); 
				clearPlayerKillList(i);
			}
		}
		return 1;
	}

	if(paintVars[id][paintTimer]) {
		if(paintVars[id][paintID] != 3) {
			gString[0] = EOS;
			format(gString, sizeof gString, "PAINTBALL ROUND ENDS IN: ~y~%s", calculateTime(paintVars[id][paintTimer]));
	       	TextDrawSetString(paintVars[id][paintTextdraw], gString);
	    }   	
	}
	else if(paintVars[id][paintID] != 3) {
		paintVars[id][paintTimer] = 15;
		paintVars[id][paintMatchStarted] = false;

		return matchPaintFinished(id, paintVars[id][paintLeader]);
	}

	return 1;
} 

// ------------------------------------- //
//             Callbacks                 //
// ------------------------------------- //

function:OnPaintBallPlayerDeath(playerid, killerid, reason) {
	if(isPlayerInPaint(playerid) && paintVars[playerPaintID[playerid]][paintMatchStarted]) {
		if(playerPaintID[killerid] == playerPaintID[playerid]) {
			playerPaintKills[killerid] ++;

			va_SendClientMessage(killerid, COLOR_GENANNOUNCE, "You have killed %s [%d]. You now have %d kills.", GetName(playerid), playerid, playerPaintKills[killerid]);
			
			if(reason < sizeof GunNames || reason == 51) {
				va_SendClientMessage(playerid, COLOR_GENANNOUNCE, "You have been killed by %s [%d] with weapon %s.", GetName(killerid), killerid, reason == 51 ? "grenade" : GunNames[reason]);
			}
			else {
				va_SendClientMessage(playerid, COLOR_GENANNOUNCE, "You have been killed by %s [%d].", GetName(killerid), killerid);
			}

			if(playerPaintKills[killerid] > paintVars[playerPaintID[killerid]][paintLeadKills]) {
				paintVars[playerPaintID[killerid]][paintLeader] = killerid;
				paintVars[playerPaintID[killerid]][paintLeadKills] = playerPaintKills[killerid];

				sendPaintMessage(playerPaintID[killerid], COLOR_YELLOW, "%s is now the lead with %d kills.", GetName(killerid), playerPaintKills[killerid]);
			}

			if(playerPaintID[killerid] == TYPE_GUNGAME) {
				if(playerPaintKills[killerid] - 2 >= sizeof paintGungameWeapons * 2) {
					new const id = playerPaintID[killerid];

					paintVars[id][paintTimer] = 15;
					paintVars[id][paintMatchStarted] = false;

                    return matchPaintFinished(id, killerid);
                }    
				
				else if(!(playerPaintKills[killerid] % 2)) {
					ResetPlayerWeapons(killerid);
					GiveGungameWeapons(killerid);
					PlayerPlaySound(killerid, 6401, 0.0, 0.0, 0.0);

					va_SendClientMessage(killerid, COLOR_GENANNOUNCE, "You have gained your next level weapon (%s).", GunNames[paintGungameWeapons[playerPaintKills[playerid] / 2]]);
				}
			}
			
			foreach(new i : paintPlayers[playerPaintID[killerid]])
				SendDeathMessageToPlayer(i, killerid, playerid, reason);
		}
	}	
	return 1;
} 

// ------------------------------------- //
//             Hooks                     //
// ------------------------------------- //

hook OnPlayerConnect(playerid) {
	playerPaintID[playerid] = playerVotedPaintMap[playerid] = playerVotedPaintGuns[playerid] = -1;
	playerPaintKills[playerid] = 0;
	return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
	if(isPlayerInPaint(playerid)) {
		new 
			id = playerPaintID[playerid],
			count = Iter_Count(paintPlayers[id]);

		Iter_Remove(paintPlayers[id], playerid);

		count --;

		if(paintVars[id][paintMatchStarted]) {
			if(count == 1) 
				playerLeavePaint(Iter_First(paintPlayers[id]));
			
			else if(count > 1) 
				sendPaintMessage(id, COLOR_YELLOW, "%s has left the paintball arena. (disconnected)", GetName(playerid));
		}
		else {
			if(paintPlayerVotedMap(playerid)) 
				paintVars[id][paintMapVotes][playerVotedPaintMap[playerid]] --;

			if(paintPlayerVotedGuns(playerid)) 
				paintVars[id][paintGunVotes][playerVotedPaintGuns[playerid]] --;

			if(count == 1) {
				TextDrawSetString(paintVars[id][paintTextdraw], "WAITING FOR PLAYERS...");

				resetPaintVars(id);

				GameTextForPlayer(Iter_First(paintPlayers[id]), "~y~NOT ENOUGH PLAYERS TO START...", 5000, 5);

				stop paintVars[id][paintMatchTimer];
			}
		}
	}
	return 1;	
}

hook OnGameModeInit() {
	for(new i; i < sizeof paintVars; i++) {
		paintVars[i][paintArena] = random(sizeof paintballNames);

		paintVars[i][paintLeader] = INVALID_PLAYER_ID;
		paintVars[i][paintLeadKills] = 0;

		paintVars[i][paintTextdraw] = TextDrawCreate(71.333259, 302.829559, "");
		TextDrawLetterSize(paintVars[i][paintTextdraw], 0.148999, 1.197628);
		TextDrawAlignment(paintVars[i][paintTextdraw], 2);
		TextDrawColor(paintVars[i][paintTextdraw], -1);
		TextDrawSetShadow(paintVars[i][paintTextdraw], 0);
		TextDrawSetOutline(paintVars[i][paintTextdraw], 1);
		TextDrawBackgroundColor(paintVars[i][paintTextdraw], 255);
		TextDrawFont(paintVars[i][paintTextdraw], 2);
		TextDrawSetProportional(paintVars[i][paintTextdraw], 1);
	}
	return 1;
}