// ------------------------------------- //
//             Includes                  //
// ------------------------------------- //

#include <YSI_Coding\y_hooks>

// ------------------------------------- //
//             Variables                 //
// ------------------------------------- //
#define check_instructor if(playerInfo[playerid][pGroup] != 12) return true;
new 
   bool:playerLesson[MAX_PLAYERS],
   bool:playerGivedWeapon[MAX_PLAYERS],
   playerLessonTime[MAX_PLAYERS],
   playerNeedLicence[MAX_PLAYERS],
   playerTypeNeedLicence[MAX_PLAYERS],
   playerRequestLicenceDelay[MAX_PLAYERS],
   playerLessonPlayer[MAX_PLAYERS],
   Iterator:playersNeedLicences<MAX_PLAYERS>;

// ------------------------------------- //
//             Enums                     //
// ------------------------------------- //

enum {
    TYPE_GUN = 1,
    TYPE_FLY,
    TYPE_BOAT
}

// ------------------------------------- //
//             Functions                 //
// ------------------------------------- //

ShowLicences(playerid, targetid)
{
	va_SendClientMessage(targetid, -1, "---- %s's licences ----", GetName(playerid));

	va_SendClientMessage(targetid, -1, "Drivers licence: %s", (playerInfo[playerid][pCarLic] ? va_return("Passed (expires in %d hours)", playerInfo[playerid][pCarLic]) : ("Not Passed")));
	va_SendClientMessage(targetid, -1, "Flying licence: %s", (playerInfo[playerid][pFlyLic] ? va_return("Passed (expires in %d hours)", playerInfo[playerid][pFlyLic]) : ("Not Passed")));
	va_SendClientMessage(targetid, -1, "Boat licence: %s", (playerInfo[playerid][pBoatLic] ? va_return("Passed (expires in %d hours)", playerInfo[playerid][pBoatLic]) : ("Not Passed")));
	va_SendClientMessage(targetid, -1, "Weapon licence: %s", (playerInfo[playerid][pGunLic] ? va_return("Passed (expires in %d hours)", playerInfo[playerid][pGunLic]) : ("Not Passed")));

	SendClientMessage(targetid, COLOR_WHITE, "------------");
	return true;
}

static setPlayerLesson(const playerid, const bool: status = false) 
    return playerLesson[playerid] = status;

static isPlayerGivedWeapon(const playerid)
    return playerGivedWeapon[playerid];        

static setPlayerTypeLicenceNeed(const playerid, const type) 
    return playerTypeNeedLicence[playerid] = type;

static getPlayerLicenceTypeNeed(const playerid) 
    return playerTypeNeedLicence[playerid];    

static getPlayerLicenceNameType(const playerid) {
	gString[0] = EOS;

	switch(playerTypeNeedLicence[playerid]) {
		case 1: gString = "gun";
		case 2: gString = "fly";
		case 3: gString = "boat";
	}

	return gString;
}

isPlayerInLesson(const playerid)
    return playerLesson[playerid];

getPlayerNeedLicence(const playerid)
    return playerNeedLicence[playerid];      

setPlayerNeedLicence(const playerid, const value = INVALID_PLAYER_ID)
    return playerNeedLicence[playerid] = value;

setPlayerGivedWeapon(const playerid, const bool: gived = false)
    return playerGivedWeapon[playerid] = gived;    

// ------------------------------------- //
//            Commands                   //
// ------------------------------------- //

YCMD:startlesson(playerid, params[], help) {
    check_instructor
    new returnPlayer; if(sscanf(params, "u", returnPlayer))
        return sendSyntaxMessage(playerid, "/startlesson <playerid/name>");

    if(!playerStaticVar[returnPlayer][playerLogged])
        return sendError(playerid, "Acel player nu este logat.");

    if(returnPlayer == playerid)
    	return sendError(playerid, "Nu poti folosi aceasta comanda asupra ta.");

    if(!IsPlayerInRangeOfPlayer(playerid, returnPlayer, 10.0))
       return sendError(playerid, "Nu esti langa acel player.");  

    if(isPlayerInLesson(returnPlayer))
       return sendError(playerid, "Acel player este deja intr-o lectie.");

    if(playerInfo[returnPlayer][pLevel] < 3)
    	return sendError(playerid, "Acel player trebuie sa aiba minim level 3.");

    va_SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You've started %s's lesson.", GetName(returnPlayer));
    va_SendClientMessage(returnPlayer, COLOR_LIGHTBLUE, "* Instructor %s has started your lesson.", GetName(playerid));

    playerLessonTime[returnPlayer] = gettime() + 15;
    playerLessonPlayer[returnPlayer] = playerid;
    setPlayerLesson(returnPlayer, true);

    sendGroupMessage(GROUP_TYPE_SI, COLOR_YELLOW, "* Instructor %s has started %s lesson.", GetName(playerid), GetName(returnPlayer));
    return 1;           
}

YCMD:stoplesson(playerid, params[], help) {
    check_instructor
    new returnPlayer; if(sscanf(params, "u", returnPlayer))
        return sendSyntaxMessage(playerid, "/stoplesson <playerid/name>");

    if(!playerStaticVar[returnPlayer][playerLogged])
       return sendError(playerid, "Acel player nu este logat.");

    if(returnPlayer == playerid)
    	return sendError(playerid, "Nu poti folosi aceasta comanda asupra ta.");   

    if(!IsPlayerInRangeOfPlayer(playerid, returnPlayer, 10.0))
       return sendError(playerid, "Nu esti langa acel player.");  

    if(playerLessonPlayer[returnPlayer] != playerid)
    	return sendError(playerid, "Nu i-ai oferit o lectie acelui player.");

    if(!isPlayerInLesson(returnPlayer))
       return sendError(playerid, "Acel player nu este intr-o lectie.");

    va_SendClientMessage(returnPlayer, COLOR_LIGHTBLUE, "* Instructor %s has stopped your lesson.", GetName(playerid));

    RemovePlayerWeapon(returnPlayer, WEAPON_SILENCED);				        

    setPlayerLesson(returnPlayer, false);
    setPlayerGivedWeapon(playerid, false);
    playerLessonPlayer[playerid] = INVALID_PLAYER_ID;

    return sendGroupMessage(GROUP_TYPE_SI, COLOR_LIGHTBLUE, "* Instructor %s has stopped %s lesson.", GetName(playerid), GetName(returnPlayer));		
}

YCMD:giveweapon(playerid, params[], help) {
    check_instructor
    new returnPlayer; if(sscanf(params, "u", returnPlayer))
        return sendSyntaxMessage(playerid, "/giveweapon <playerid/name>");

    if(!playerStaticVar[returnPlayer][playerLogged])
       return sendError(playerid, "Acel player nu este logat.");

    if(returnPlayer == playerid)
    	return sendError(playerid, "Nu poti folosi aceasta comanda asupra ta."); 

    if(isPlayerGivedWeapon(playerid))
        return sendError(playerid, "Poti oferi un silenced o singura data pe lectie.");

    if(playerLessonPlayer[returnPlayer] != playerid)
    	return sendError(playerid, "Nu i-ai oferit o lectie acelui player.");  

    if(!IsPlayerInRangeOfPlayer(playerid, returnPlayer, 10.0))
       return sendError(playerid, "Nu esti langa acel player.");  

    if(playerInfo[returnPlayer][pLevel] < 3)
    	return sendError(playerid, "Acel player trebuie sa aiba minim level 3.");

    if(!isPlayerInLesson(returnPlayer))	
    	return sendError(playerid, "Acel player nu este intr-o lectie.");

    if(playerLessonTime[returnPlayer] > gettime())
    	return sendError(playerid, "Trebuie sa treaca minim 15 secunde de la inceperea lectiei.");

	GivePlayerWeapon(returnPlayer, WEAPON_SILENCED, 10);
    setPlayerGivedWeapon(playerid, true);

	sendGroupMessage(GROUP_TYPE_SI, COLOR_MEMBER, "# %s has given a weapon to %s.", GetName(playerid), GetName(returnPlayer));
    return 1;
}

YCMD:givelicence(playerid, params[], help) {
    check_instructor
    new licName[10], returnPlayer; 

    if(sscanf(params, "s[10]d", licName, returnPlayer)) {
    	sendSyntaxMessage(playerid, "/givelicence <licence name> <playerid/name>");
    	return SendClientMessage(playerid, COLOR_GREY, "Licences Available: {FFFFFF}gun, fly, boat.");
    }

    if(!playerStaticVar[returnPlayer][playerLogged])
       return sendError(playerid, "Acel player nu este logat.");

    if(returnPlayer == playerid)
    	return sendError(playerid, "Nu poti folosi aceasta comanda asupra ta.");   

    if(!IsPlayerInRangeOfPlayer(playerid, returnPlayer, 10.0))
       return sendError(playerid, "Nu esti langa acel player.");

    if(playerInfo[returnPlayer][pLevel] < 3)
    	return sendError(playerid, "Acel player trebuie sa aiba minim level 3.");

    if(!isPlayerInLesson(returnPlayer))
    	return sendError(playerid, "Acel player nu este intr-o lectie.");

    if(!strcmp(licName, "gun")) {
        if(playerInfo[returnPlayer][pGunLic] < 0)
            return sendError(playerid, "Acel player are licenta de arme suspendata.");
        
    	if(playerInfo[returnPlayer][pGunLic] > 10)
    		return sendError(playerid, "Acel player are licenta de arme valabila pentru mai mult de 10 ore.");

    	playerInfo[returnPlayer][pGunLic] = 100;

        setPlayerGivedWeapon(playerid, false);

        if(getPlayerLicenceTypeNeed(returnPlayer) == TYPE_GUN) {
            Iter_Remove(playersNeedLicences, returnPlayer);

            if(getPlayerNeedLicence(returnPlayer) != INVALID_PLAYER_ID) {
                new targetID = getPlayerNeedLicence(returnPlayer);

                setPlayerNeedLicence(targetID, INVALID_PLAYER_ID);
                setPlayerNeedLicence(returnPlayer, INVALID_PLAYER_ID);

                if(playerStaticVar[playerid][usedFind] == returnPlayer)
                    cancel_find(targetID);
            }
        }

    	va_mysql_tquery("UPDATE `users` SET `WeaponLicence` = %d WHERE `ID` = %d", playerInfo[returnPlayer][pGunLic], playerInfo[returnPlayer][pSQLID]);
    }
    else if(!strcmp(licName, "boat")) {	
        if(playerInfo[returnPlayer][pBoatLic] < 0)
            return sendError(playerid, "Acel player are licenta de navigatie suspendata.");

    	if(playerInfo[returnPlayer][pBoatLic] > 10)
    		return sendError(playerid, "Acel player are licenta de navigatie valabila pentru mai mult de 10 ore.");

    	playerInfo[returnPlayer][pBoatLic] = 100;

        if(getPlayerLicenceTypeNeed(returnPlayer) == TYPE_BOAT) {
            Iter_Remove(playersNeedLicences, returnPlayer);

            if(getPlayerNeedLicence(returnPlayer) != INVALID_PLAYER_ID) {
                new targetID = getPlayerNeedLicence(returnPlayer);

                setPlayerNeedLicence(targetID, INVALID_PLAYER_ID);
                setPlayerNeedLicence(returnPlayer, INVALID_PLAYER_ID);

                if(playerStaticVar[playerid][usedFind] == returnPlayer)
                    cancel_find(targetID);
            }
        }

    	va_mysql_tquery("UPDATE `users` SET `SailingLicence` = %d WHERE `ID` = %d", playerInfo[returnPlayer][pBoatLic], playerInfo[returnPlayer][pSQLID]);
    }
    else if(!strcmp(licName, "fly")) {
        if(playerInfo[returnPlayer][pFlyLic] < 0)
            return sendError(playerid, "Acel player are licenta de zbor suspendata.");

    	if(playerInfo[returnPlayer][pFlyLic] > 10)
    		return sendError(playerid, "Acel player are licenta de zbor valabila pentru mai mult de 10 ore.");

    	playerInfo[returnPlayer][pFlyLic] = 100;

        if(getPlayerLicenceTypeNeed(returnPlayer) == TYPE_FLY) {
            Iter_Remove(playersNeedLicences, returnPlayer);

            if(getPlayerNeedLicence(returnPlayer) != INVALID_PLAYER_ID) {
                new targetID = getPlayerNeedLicence(returnPlayer);

                setPlayerNeedLicence(targetID, INVALID_PLAYER_ID);
                setPlayerNeedLicence(returnPlayer, INVALID_PLAYER_ID);

                if(playerStaticVar[playerid][usedFind] == returnPlayer)
                    cancel_find(targetID);
            }
        }

    	va_mysql_tquery("UPDATE `users` SET `FlyLicence` = %d WHERE `ID` = %d", playerInfo[returnPlayer][pFlyLic], playerInfo[returnPlayer][pSQLID]);
    }
    else {
    	sendSyntaxMessage(playerid, "/givelicence <licence name> <playerid/name>");
    	return SendClientMessage(playerid, COLOR_GREY, "Licences Available: {FFFFFF}gun, fly, boat.");
    }

    playerLessonPlayer[returnPlayer] = INVALID_PLAYER_ID;

    setPlayerLesson(returnPlayer, false);

	va_SendClientMessage(playerid, COLOR_DARKPINK, "I-ai dat lui %s licenta de %s.", GetName(returnPlayer), licName);
	va_SendClientMessage(returnPlayer, COLOR_DARKPINK, "%s ti-a dat licenta de %s.", GetName(playerid), licName);
	sendGroupMessage(GROUP_TYPE_SI, COLOR_MEMBER, "(i) %s has given %s the %s licence.", GetName(playerid), GetName(returnPlayer), licName);
	insertRaportPoint(playerid);			        
    return 1;	
}

YCMD:requestlicence(playerid, params[], help) {
	if(playerInfo[playerid][pLevel] < 3)
		return sendError(playerid, "Trebuie sa ai minim level 3.");

    if(Iter_Contains(playersNeedLicences, playerid))
    	return sendError(playerid, "Ai o cerere de request activa.");

    if(playerInfo[playerid][pGroup] == GROUP_TYPE_SI)
    	return sendError(playerid, "Esti instructor, nu poti cere o licenta.");

    if(playerRequestLicenceDelay[playerid] > gettime())
    	return sendError(playerid, "Mai poti folosi aceasta comanda peste %d secunde.", playerRequestLicenceDelay[playerid] - gettime());

	new licName[10]; 

	if(sscanf(params, "s[10]", licName)) {
	    sendSyntaxMessage(playerid, "/requestlicence <lic name>");
	    return SendClientMessage(playerid, COLOR_GREY, "Licences Available: {FFFFFF}gun, fly, boat.");
	}

	if(!strcmp(licName, "gun")) 
		setPlayerTypeLicenceNeed(playerid, TYPE_GUN);

	else if(!strcmp(licName, "fly")) 
		setPlayerTypeLicenceNeed(playerid, TYPE_FLY);
	
	else if(!strcmp(licName, "boat")) 
		setPlayerTypeLicenceNeed(playerid, TYPE_BOAT);
	
	else {
		sendSyntaxMessage(playerid, "/requestlicence <lic name>");
	    return SendClientMessage(playerid, COLOR_GREY, "Licences Available: {FFFFFF}gun, fly, boat.");
	}

	playerRequestLicenceDelay[playerid] = gettime() + 300;

	SendClientMessage(playerid, COLOR_YELLOW, "Licence requested send to intructors, wait for answer.");

	Iter_Add(playersNeedLicences, playerid);

	return sendGroupMessage(GROUP_TYPE_SI, COLOR_MEMBER, "%s need the %s licence, use /acceptlicence %d for accept.", GetName(playerid), licName, playerid);
}
YCMD:cancellicencerequest(playerid, params[], help) {
    check_instructor
    if(getPlayerNeedLicence(playerid) == INVALID_PLAYER_ID)
    	return sendError(playerid, "Nu ai o oferta acceptata.");

    new returnPlayer = getPlayerNeedLicence(playerid);

    va_SendClientMessage(returnPlayer, COLOR_YELLOW, "Instructor %s canceled your request licence.", GetName(playerid));
    va_SendClientMessage(playerid, COLOR_YELLOW, "You canceled the request licence of %s.", GetName(returnPlayer));

    if(playerStaticVar[playerid][targetFind] == returnPlayer)
    	cancel_find(playerid);

    setPlayerNeedLicence(returnPlayer, INVALID_PLAYER_ID);
    setPlayerNeedLicence(playerid, INVALID_PLAYER_ID);
    return 1;
}

YCMD:cancelrequestlicence(playerid, params[], help) {
	check_instructor

	if(!Iter_Contains(playersNeedLicences, playerid))
		return sendError(playerid, "Nu ai o oferta de cerere a unei licente.");

    if(getPlayerNeedLicence(playerid) != INVALID_PLAYER_ID) {
    	new returnPlayer = getPlayerNeedLicence(playerid);

    	setPlayerNeedLicence(returnPlayer, INVALID_PLAYER_ID);
    	setPlayerNeedLicence(playerid, INVALID_PLAYER_ID);

    	va_SendClientMessage(returnPlayer, COLOR_YELLOW, "%s canceled the request licence.", GetName(playerid));
    	SendClientMessage(playerid, COLOR_GREY, "You canceled the request licence.");

    	if(playerStaticVar[returnPlayer][targetFind] == playerid)
    		cancel_find(returnPlayer);
    }

    Iter_Remove(playersNeedLicences, playerid);	
	return 1;
}

YCMD:licencesrequests(playerid, params[], help) {
    check_instructor
	if(!Iter_Count(playersNeedLicences))
		return sendError(playerid, "Nu sunt cereri de licenta.");

    gDialog = "ID\tName\tLicence name\n";

    foreach(new i : playersNeedLicences) 
    	format(gDialog, sizeof gDialog, "%s%d\t%s\t%s\n", gDialog, i, GetName(i), getPlayerLicenceNameType(i));
    
    return Dialog_Show(playerid, NULL, DIALOG_STYLE_TABLIST_HEADERS, "SERVER: Request Licences", gDialog, "Ok", "");
}

YCMD:licences(playerid, params[], help)
{
    ShowLicences(playerid, playerid);
    return true;
}

YCMD:showlicences(playerid, params[], help)
{
	check_delay

	new returnPlayer;
	if(sscanf(params, "u", returnPlayer))
		return sendSyntaxMessage(playerid, "/showlicences [playerid/playername]");

	if(!IsPlayerConnected(returnPlayer) || returnPlayer == playerid)
		return SendClientMessage(playerid, -1, "Invalid player.");

	ShowLicences(playerid, returnPlayer);

	va_SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You have shown your Licenses to %s.", GetName(returnPlayer));
	va_SendClientMessage(returnPlayer, COLOR_LIGHTBLUE, "* %s has shown his/her Licenses to you.", GetName(playerid));
	return set_delay;
}

YCMD:instructors(playerid, params[], help) 
{
    format(returnString, sizeof returnString, "name\tid\tphone\n");
    foreach(new i : factionMembers<GROUP_TYPE_SI>)
    {
        format(returnString, sizeof returnString, "%s%s\t(ID: %d)\t%d\n", returnString, GetName(i), i, playerInfo[i][pPhone]);
        Dialog_Show(playerid, 0, DIALOG_STYLE_TABLIST_HEADERS, "SERVER: Instructors", returnString, "Close", "");
    }
    SendClientMessage(playerid, COLOR_WHITE, "Use /requestlicense to request a licence.");
    return true; 
}

// ------------------------------------- //
//             Hooks                     //
// ------------------------------------- //

hook OnPlayerConnect(playerid) {
	playerLesson[playerid] = playerGivedWeapon[playerid] = false;
	playerNeedLicence[playerid] = playerLessonPlayer[playerid] = INVALID_PLAYER_ID;
	playerLessonTime[playerid] = playerTypeNeedLicence[playerid] = playerRequestLicenceDelay[playerid] = -1;
	return 1;
}

hook OnPlayerSpawn(playerid) {
    if(isPlayerInLesson(playerid)) {
        setPlayerLesson(playerid, false);

        if(playerLessonPlayer[playerid] != INVALID_PLAYER_ID) {
            new returnPlayer = playerLessonPlayer[playerid];

            setPlayerGivedWeapon(returnPlayer, false);

            playerLessonPlayer[playerid] = INVALID_PLAYER_ID;
        }
    }
    return 1;
}

hook OnPlayerDisconnect(playerid) {
    if(isPlayerInLesson(playerid)) {
        if(playerLessonPlayer[playerid] != INVALID_PLAYER_ID) {
            new const returnPlayer = playerLessonPlayer[playerid];

            setPlayerGivedWeapon(returnPlayer, false);
        } 
        setPlayerLesson(playerid, false);
    }
	if(playerTypeNeedLicence[playerid] != -1) {
		Iter_Remove(playersNeedLicences, playerid);

		if(getPlayerNeedLicence(playerid) != INVALID_PLAYER_ID) {
			new returnPlayer = getPlayerNeedLicence(playerid);

			setPlayerNeedLicence(returnPlayer, INVALID_PLAYER_ID);

			if(playerStaticVar[returnPlayer][targetFind] == playerid)
				cancel_find(returnPlayer);

            setPlayerGivedWeapon(returnPlayer, false);

		    va_SendClientMessage(returnPlayer, COLOR_YELLOW, "* Player %s has been disconnected, the request has been canceled.", GetName(playerid));
		}
	}
	else if(getPlayerNeedLicence(playerid) != INVALID_PLAYER_ID) {
		new returnPlayer = getPlayerNeedLicence(playerid);

		setPlayerNeedLicence(returnPlayer, INVALID_PLAYER_ID);

		if(playerStaticVar[returnPlayer][targetFind] == playerid)
			cancel_find(returnPlayer);

		va_SendClientMessage(returnPlayer, COLOR_YELLOW, "* Instructor %s has been disconnected, use again /requestlicence.", GetName(playerid));
	}
	return 1;
}

hook WhenPlayerLoggedIn(playerid)
{
	if(!cache_num_rows())
		return Y_HOOKS_CONTINUE_RETURN_1;


    cache_get_value_name_int(0, "DrivingLicence", playerInfo[playerid][pCarLic]);
    cache_get_value_name_int(0, "WeaponLicence", playerInfo[playerid][pGunLic]);
    
    cache_get_value_name_int(0, "FlyLicence", playerInfo[playerid][pFlyLic]);
    cache_get_value_name_int(0, "SailingLicence", playerInfo[playerid][pBoatLic]);

	return Y_HOOKS_CONTINUE_RETURN_1;	
}