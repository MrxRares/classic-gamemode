#include <YSI_Coding\y_hooks>

#define check_admin if(playerInfo[playerid][pAdmin] < 1) return returnNoAcces(playerid);
#define check_owner if(playerInfo[playerid][pAdmin] < 6) return returnNoAcces(playerid);
#define check_helper if(playerInfo[playerid][pHelper] < 1) return returnNoAcces(playerid);

#define check_admin_ver if(playerInfo[playerid][pAdmin] < 1) return 1;

#define STAFF_TYPE_HELPER 0
#define STAFF_TYPE_ADMIN 1
#define STAFF_TYPE_ALL 2
#define STAFF_TYPE_OWNER 3

#define REPORT_TYPE_DM 1
#define REPORT_TYPE_STUCK 2
#define REPORT_TYPE_OTHER 3

#define MAX_HELPER_QUESTIONS 30

new Iterator: serverAdmins<MAX_PLAYERS>, Iterator: serverHelpers<MAX_PLAYERS>, Iterator: Spectators<MAX_PLAYERS>,
	amotd_message[128], Iterator: sendReport<MAX_PLAYERS>, Iterator: Cheaters<MAX_PLAYERS>, Spectate[MAX_PLAYERS];

enum E_HELPER_DATA
{
    questionSender, questionMessage[144], questionHelper
};
new newbieQuestions[MAX_HELPER_QUESTIONS][E_HELPER_DATA];

new playerNewbieQuestion[MAX_PLAYERS], helperAssignedQuestion[MAX_PLAYERS], Iterator: helperQuestions<MAX_HELPER_QUESTIONS>, Timer:helper_check[MAX_PLAYERS],
	Iterator: helperAQuestions<MAX_HELPER_QUESTIONS>, Iterator: helpersOnDuty<MAX_PLAYERS>, Iterator: helpersWithQuestion<MAX_PLAYERS>;

function:AdminAcces(playerid) {
	if(playerInfo[playerid][pSQLID] == 1) return 1;
	return 0;
}	

//0xcc8e33FF
sendStaffMessage(staff, color, const string[], va_args<>)
{
	new staffString[144];
	va_format(staffString, sizeof staffString, string, va_start<3>);

	switch(staff)
	{
		case STAFF_TYPE_HELPER:
		{
			foreach(new i : serverHelpers)
				sendSplitMessage(i, color, staffString);
		}
		case STAFF_TYPE_ADMIN:
		{
			foreach(new i : serverAdmins)
				sendSplitMessage(i, color, staffString);
		}
		case STAFF_TYPE_OWNER:
		{
			foreach(new i : serverAdmins)
			{
				if(playerInfo[i][pAdmin] < 6)
					continue;

				sendSplitMessage(i, color, staffString);
			}
		}
		case STAFF_TYPE_ALL:
		{
			foreach(new i : serverHelpers)
			{
				if(Iter_Contains(serverAdmins, i))
					continue;

				sendSplitMessage(i, color, staffString);
			}
			foreach(new i : serverAdmins)
				sendSplitMessage(i, color, staffString);
		}
	}
	return true;
}

hook WhenPlayerLoggedIn(playerid)
{
	if(!cache_num_rows())
		return Y_HOOKS_CONTINUE_RETURN_1;
	
	cache_get_value_name_int(0, "Helper", playerInfo[playerid][pHelper]);
	cache_get_value_name_int(0, "Admin", playerInfo[playerid][pAdmin]);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnGameModeInit()  
{
    Command_AddAltNamed("newbie", "n");
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid)
{
    playerTextdraws[playerid][spectateTD] = CreatePlayerTextDraw(playerid, 320.0, 329.575000, "owner: ionchyadv");
    PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][spectateTD], 0.22, 1.055);
    PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][spectateTD], 2);
    PlayerTextDrawColour(playerid, playerTextdraws[playerid][spectateTD], -1);
    PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][spectateTD], 0);
    PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][spectateTD], 1);
    PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][spectateTD], 255);
    PlayerTextDrawFont(playerid, playerTextdraws[playerid][spectateTD], 1);
    PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][spectateTD], 1);
    PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][spectateTD], 0);

    Spectate[playerid] = INVALID_PLAYER_ID;

    helperAssignedQuestion[playerid] = -1;
    if(playerNewbieQuestion[playerid] != -1)
    {
	   format(newbieQuestions[playerNewbieQuestion[playerid]][questionMessage], 16, "");
	   newbieQuestions[playerNewbieQuestion[playerid]][questionSender] = -1;
	   newbieQuestions[playerNewbieQuestion[playerid]][questionHelper] = -1;

       playerNewbieQuestion[playerid] = -1;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	if(!playerStaticVar[playerid][playerLogged])
		return Y_HOOKS_CONTINUE_RETURN_1;


	new questionId = playerNewbieQuestion[playerid], questionHelperId = helperAssignedQuestion[playerid];
    if(questionId != -1)
    {
        SendClientMessage(newbieQuestions[questionId][questionHelper], COLOR_YELLOW, "Jucatorul ce a pus intrebarea a parasit serverul.");

        Iter_Remove(helperQuestions, questionId);
        Iter_Remove(helperAQuestions, questionId);

        DeletePVar(newbieQuestions[questionId][questionHelper], "newbieQuestionAnswer");

        playerNewbieQuestion[playerid] = -1;
        newbieQuestions[questionId][questionSender] = -1;
        newbieQuestions[questionId][questionHelper] = -1;
        format(newbieQuestions[questionId][questionMessage], 144, "");
    }
    if(questionHelperId != -1)
    {
        Iter_Remove(helperAQuestions, questionId);
        Iter_Remove(helpersWithQuestion, playerid);

        helperAssignedQuestion[playerid] = -1;
    }

	if(Iter_Contains(helpersWithQuestion, playerid))
		Iter_Remove(helpersWithQuestion, playerid);

	if(Iter_Contains(helpersOnDuty, playerid))
		Iter_Remove(helpersOnDuty, playerid);

	if(Iter_Contains(sendReport, playerid))
		Iter_Remove(sendReport, playerid);

	if(playerInfo[playerid][pAdmin] || playerInfo[playerid][pHelper])
	{
		if(Iter_Contains(serverHelpers, playerid))
			Iter_Remove(serverHelpers, playerid);

		if(Iter_Contains(serverAdmins, playerid))
			Iter_Remove(serverAdmins, playerid);

		if(Iter_Contains(Spectators, playerid))
	    	Iter_Remove(Spectators, playerid);

		sendStaffMessage(STAFF_TYPE_ALL, 0xbf891dFF, "Quitbot: %s has left the server (%s).", GetName(playerid), disconnectReason[reason]);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:amotd(playerid, params[], help)
{
    if(playerInfo[playerid][pAdmin] < 5)
        return true;

    new returnMotd[128];
    if(sscanf(params, "s[128]", returnMotd))
        return sendSyntaxMessage(playerid, "/amotd <Message>");

    format(amotd_message, sizeof amotd_message, returnMotd);
    sendStaffMessage(STAFF_TYPE_ALL, 0xbf891dFF, "Admin MOTD: %s", returnMotd);
    return true;
}
YCMD:staff(playerid, params[], help) {
    check_owner

	Dialog_Show(playerid, STAFF, DIALOG_STYLE_LIST, "SERVER: Staff Category", "Admins (%d online)\nHelpers (%d online)", "Select", "Exit", Iter_Count(serverAdmins), Iter_Count(serverHelpers));

    return true;
}
Dialog:STAFF(playerid, response, listitem, inputtext[]) {
    if(!response)
        return 1;

    return mysql_tquery(SQL, listitem == 0 ? "SELECT * FROM `users` WHERE `Admin` >= 1 ORDER BY `users`.`Admin` DESC LIMIT 25" : "SELECT * FROM `users` WHERE `Helper` >= 1 ORDER BY `users`.`Helper` DESC LIMIT 25", "checkStaffCategory", "dd", playerid, listitem);
}
YCMD:spawnhere(playerid, params[], help)
{
	if (playerInfo[playerid][pAdmin] >= 1 || playerInfo[playerid][pHelper] >= 1) {
		new id,sendername[30],giveplayer[30],string[256],Float:x,Float:y,Float:z;
		if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_GREY, "Syntax:{FFFFFF} /spawnhere [name/playerid]");
		{
			if(IsPlayerConnected(id))
			{
			    if(id != INVALID_PLAYER_ID)
				{
					GetPlayerPos(id, x, y, z);
					GetPlayerName(id, giveplayer, sizeof(giveplayer));
					SpawnPlayer(id);
					SetTimerEx("SetPlayerPositionIV", 1000, false, "ufffdd", id, x, y, z, GetPlayerInterior(id), GetPlayerVirtualWorld(id));
					GetPlayerName(playerid, sendername, sizeof(sendername));
					format(string, sizeof(string), "You have been spawned by %s.", sendername);
					SendClientMessage(id, COLOR_YELLOW, string);
					format(string, sizeof(string), "Done: You have /spawn-ed %s.", giveplayer);
					SendClientMessage(playerid, COLOR_YELLOW, string);
					format(string, sizeof(string), "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s has /spawn-ed %s and restored his position.",sendername, giveplayer);
					sendStaffMessage(STAFF_TYPE_ALL, -1, string);
					return 1;
				}
			}
            else return SendClientMessage(playerid, COLOR_GREY, "Player not connected.");
		}
	}
	else return sendError(playerid,"You are not an admin/helper.");
	return 1;
}
YCMD:freeze(playerid, params[], help) {
    if(playerInfo[playerid][pAdmin] < 1 && playerInfo[playerid][pHelper] < 2)
        return sendError(playerid,"Nu ai gradul administrativ necesar pentru a folosi aceasta comanda.");

    if(!isnumeric(params))
        return sendSyntaxMessage(playerid, "/freeze <playerid>");

    new const returnPlayer = strval(params);

    if(!IsPlayerConnected(returnPlayer))
        return sendError(playerid,"Invalid ID!");

    TogglePlayerControllableEx(returnPlayer, false);

    va_SendClientMessage(playerid, COLOR_GREY, "* You freezed player %s.", GetName(returnPlayer));
    va_SendClientMessage(returnPlayer, COLOR_GREY, "* %s freezed you.", GetName(playerid));
    return sendStaffMessage(STAFF_TYPE_ALL, -1, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s freezed player %s.", GetName(playerid), GetName(returnPlayer));   
}

YCMD:unfreeze(playerid, params[], help) {
    if(playerInfo[playerid][pAdmin] < 1 && playerInfo[playerid][pHelper] < 2)
        return sendError(playerid,"Nu ai gradul administrativ necesar pentru a folosi aceasta comanda."); 

    if(!isnumeric(params))
        return sendSyntaxMessage(playerid, "/unfreeze <playerid>");

    new const returnPlayer = strval(params);

    if(!IsPlayerConnected(returnPlayer))
        return sendError(playerid,"Invalid ID!");

    TogglePlayerControllableEx(returnPlayer, true);

    va_SendClientMessage(playerid, COLOR_GREY, "* You unfreezed player %s.", GetName(returnPlayer));
    va_SendClientMessage(returnPlayer, COLOR_GREY, "* %s unfreezed you.", GetName(playerid));
    return sendStaffMessage(STAFF_TYPE_ALL, -1, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s unfreezed player %s.", GetName(playerid), GetName(returnPlayer));
}
YCMD:slap(playerid, params[], help) 
{
	if(playerInfo[playerid][pAdmin] < 1 && playerInfo[playerid][pHelper] < 1)
		return returnNoAcces(playerid);

	new 
		targetID, Float: pos[3];

	if(sscanf(params, "u", targetID)) 
		return sendSyntaxMessage(playerid, "/slap <Name/PlayerID>");

	if(!IsPlayerConnected(targetID))
		return sendError(playerid,"Invalid or unlogged player.");

	va_SendClientMessage(playerid, COLOR_RED2, "You have slapped %s.", GetName(targetID));
	sendStaffMessage(STAFF_TYPE_ALL, -1, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s used command /slap and slapped %s (%d)", GetName(playerid), GetName(targetID), targetID);

	GetPlayerPos(targetID, pos[0], pos[1], pos[2]);
	SetPlayerPos(targetID, pos[0], pos[1], pos[2] + 5.0);

	PlayerPlaySound(playerid, 1190, pos[0], pos[1], pos[2]);

	return 1;
}
YCMD:pm(playerid, params[], help) {
    check_admin

    new id, ahaha[144]; if(sscanf(params, "us[144]", id, ahaha))
        return sendSyntaxMessage(playerid, "/pm [playerid/name] [message]");

    if(!playerStaticVar[id][playerLogged])
        return sendError(playerid,"Acel player nu este logat.");

    if(id == playerid)
        return sendError(playerid,"Nu poti folosi aceasta comanda asupra ta.");

    va_SendClientMessage(playerid, COLOR_YELLOW, "(( PM sent to %s: %s ))", GetName(id), ahaha);
    va_SendClientMessage(id, COLOR_YELLOW, "(( PM from %s: %s ))", GetName(playerid), ahaha);
    return true;
}
YCMD:serverpass(playerid, params[], help) {
    check_owner

    if(isnull(params))
        return sendSyntaxMessage(playerid, "/serverpass <password (0 = none)>");

    if(params[0] == '0') 
        sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s removed the password from server.", GetName(playerid));

    else sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s setted password '%s' on server.", GetName(playerid), params);

    format(returnString, sizeof returnString, "password %s", params);
    SendRconCommand(returnString);
    return 1;
}
YCMD:servername(playerid, params[], help) {
    check_owner

    if(isnull(params))
        return sendSyntaxMessage(playerid, "/servername <name>");

	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s setted server name to '%s'.", GetName(playerid), params);

    format(returnString, sizeof returnString, "name %s", params);
    SendRconCommand(returnString);
    return 1;
}
YCMD:slapcar(playerid, params[], help)
{
	
	if(playerInfo[playerid][pAdmin] >= 1)
	{
		new Float: x, Float: y, Float: z, string[128], id;
		if(sscanf(params, "d", id)) return sendSyntaxMessage(playerid, "/slapcar [car id]");
		GetVehiclePos(id, x, y, z);
		SetVehiclePos(id, x, y, z+5);
		format(string, sizeof(string), "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s slapped vehicle %d.", playerInfo[playerid][pName], id);
		sendStaffMessage(STAFF_TYPE_ALL, -1, string);

	}
	else return returnNoAcces(playerid);
	return 1; }

YCMD:cc(playerid, params[], help)
{
	check_admin

	foreach(new i : Player)
	{
		if(playerInfo[i][pAdmin])
			continue;

		for(new x; x < 30; x++)
			SendClientMessage(i, -1, "");
	}
	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_DCHAT, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): Admin %s a curatat chat-ul.", GetName(playerid));
	return true;
}


YCMD:cmc(playerid, params[], help)
{
	check_admin

	clearChat(playerid, 40);

	return true;
}


YCMD:setadmin(playerid, params[], help)
{
    if(playerInfo[playerid][pAdmin] < 6 && !IsPlayerAdmin(playerid))
		return returnNoAcces(playerid);

	new targetID, adminLevel;
	if(sscanf(params, "ud", targetID, adminLevel))
		return sendSyntaxMessage(playerid, "/setadmin` <Name/Playerid> <Admin Level>");

	if(!IsPlayerConnected(targetID))
		return sendError(playerid,"Player not connected.");

	if(adminLevel < 0 || adminLevel > 8)
		return sendError(playerid,"Invalid admin level.");

	va_SendClientMessage(playerid, COLOR_YELLOW, "You have promoted %s to a level %d admin.", GetName(targetID), adminLevel);
	va_SendClientMessage(targetID, COLOR_YELLOW, "You've been promoted to level %d admin, by %s.", adminLevel, GetName(playerid));

	sendStaffMessage(STAFF_TYPE_ALL, COLOR_ADMCHAT, "* Admin %s set %s's admin level to %d.", GetName(playerid), GetName(targetID), adminLevel);

	playerInfo[targetID][pAdmin] = adminLevel;
	if(!adminLevel) 
	{
		playerStaticVar[playerid][useFly] = 0;
		StopFly(playerid);
		Iter_Remove(serverAdmins, targetID);
	}

	if(!Iter_Contains(serverAdmins, targetID) && adminLevel)
		Iter_Add(serverAdmins, targetID);

	playerInfo[targetID][pPlayersHelped] = 0;

	gpci(targetID, playerInfo[targetID][pSerial], 128);
	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Admin` = '%d', `gpci` = '%s', `hwid` = '%s', `PlayersHelped`= '0' where `id` = '%d';", adminLevel, playerInfo[targetID][pSerial], playerInfo[targetID][pSerial], playerInfo[targetID][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

YCMD:setadminoff(playerid, params[], help)
{
	if(playerInfo[playerid][pAdmin] < 6 && !IsPlayerAdmin(playerid))
		return returnNoAcces(playerid);

	new targetID[MAX_PLAYER_NAME], adminLevel;
	if(sscanf(params, "s[24]d", targetID, adminLevel))
		return sendSyntaxMessage(playerid, "/setadminoff <Exact Name> <Admin Level>");

	format(returnString, sizeof returnString, "select `ID` from `users` where `name` = '%s';", targetID);
	new Cache: check_if_player_exist = mysql_query(SQL, returnString);
	
	if(!cache_num_rows())
	{
		cache_delete(check_if_player_exist);
		SendClientMessage(playerid, -1, "Player not connected.");
		return true;
	}
	cache_delete(check_if_player_exist);

	if(adminLevel < 0 || adminLevel > 8)
		return sendError(playerid,"Invalid admin level.");

	va_SendClientMessage(playerid, COLOR_YELLOW, "You have promoted %s to a level %d admin.", targetID, adminLevel);
	sendStaffMessage(STAFF_TYPE_ALL, COLOR_ADMCHAT, "* Admin %s set %s's admin level to %d.", GetName(playerid), targetID, adminLevel);

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Admin` = '%d', `PlayersHelped`= '0' where `Name` = '%s';", adminLevel, targetID);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

YCMD:sethelper(playerid, params[], help)
{
	if(playerInfo[playerid][pAdmin] < 4)
		return returnNoAcces(playerid);

	new targetID, helperLevel;
	if(sscanf(params, "ud", targetID, helperLevel))
		return sendSyntaxMessage(playerid, "/sethelper <Name/Playerid> <Helper Level>");

	if(!IsPlayerConnected(targetID))
		return sendError(playerid,"Player not connected.");

	if(helperLevel < 0 || helperLevel > 3)
		return sendError(playerid,"Invalid helper level.");

	va_SendClientMessage(playerid, COLOR_YELLOW, "You have promoted %s to a level %d helper.", GetName(targetID), helperLevel);
	va_SendClientMessage(targetID, COLOR_YELLOW, "You've been promoted to level %d helper, by %s.", helperLevel, GetName(playerid));

	sendStaffMessage(STAFF_TYPE_ALL, COLOR_ADMIN, "* Admin %s set %s's helper level to %d.", GetName(playerid), GetName(targetID), helperLevel);

	playerInfo[targetID][pHelper] = helperLevel;
	if(!helperLevel) Iter_Remove(serverHelpers, targetID);

	if(!Iter_Contains(serverHelpers, targetID) && helperLevel)
		Iter_Add(serverHelpers, targetID);

	playerInfo[targetID][pPlayersHelped] = 0;
	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Helper` = '%d', `PlayersHelped` = '0' where `id` = '%d';", helperLevel, playerInfo[targetID][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

YCMD:e(playerid, params[], help)
{
	if(!Iter_Contains(serverHelpers, playerid) && !Iter_Contains(serverAdmins, playerid))
		return returnNoAcces(playerid);

	new returnMessage[144];
	if(sscanf(params, "s[144]", returnMessage))
		return sendSyntaxMessage(playerid, "/e [message]");

	sendStaffMessage(STAFF_TYPE_ALL, COLOR_ADMIN, "(%d) %s %s: %s", (playerInfo[playerid][pAdmin] == 0 ? (playerInfo[playerid][pHelper]) : playerInfo[playerid][pAdmin]), (playerInfo[playerid][pAdmin] == 0 ? ("Helper") : ("Admin")), GetName(playerid), returnMessage);
	return true;
}

YCMD:spawn(playerid, params[], help)
{
    if(!playerInfo[playerid][pAdmin] && playerInfo[playerid][pHelper] < 2)
    	return returnNoAcces(playerid);

    new targetID, Float: playerPoss[3];
    
    if(sscanf(params, "u", targetID)) 
        return sendSyntaxMessage(playerid, "/spawn [playername/playerid]");
        
    if(!IsPlayerConnected(targetID) || targetID == INVALID_PLAYER_ID)
        return sendError(playerid,"Player not connected.");

    if(IsPlayerInAnyVehicle(targetID)) 
    {
        GetPlayerPos(targetID, playerPoss[0], playerPoss[1], playerPoss[2]); 
        SetPlayerPos(targetID, playerPoss[0], playerPoss[1], playerPoss[2] + 5.0);
        
        RemovePlayerFromVehicle(targetID); 
        SpawnPlayer(targetID);
    }

    va_SendClientMessage(targetID, COLOR_RED2, "You have respawned by %s.", GetName(playerid));
    va_SendClientMessage(playerid, COLOR_RED2, "You respawned %s.", GetName(targetID));

    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s has respawned %s.", GetName(playerid), GetName(targetID));
    
    SpawnPlayer(targetID);
    return true; 
}

YCMD:kick(playerid, params[], help)
{
	check_admin

	if(GetPVarInt(playerid, "delay_actions") > gettime()) 
		return va_SendClientMessage(playerid, -1, "Asteapta %d secunde pentru a folosi din nou aceasta comanda.", GetPVarInt(playerid, "delay_actions") - gettime());

	new returnPlayer, returnReason[64];
	if(sscanf(params, "us[64]", returnPlayer, returnReason))
		return sendSyntaxMessage(playerid, "/kick [playername/playerid] [reason]");

	if(!IsPlayerConnected(returnPlayer) || returnPlayer == playerid)
		return sendError(playerid,"Player not connected.");

	if(playerInfo[returnPlayer][pAdmin] >= playerInfo[playerid][pAdmin] && !AdminAcces(playerid)) 
		return sendError(playerid,"You can't kick a higher (or equal) level administrator.");

	va_SendClientMessageToAll(COLOR_LIGHTRED, "Kick: %s was kicked by %s, reason: %s.", GetName(returnPlayer), GetName(playerid), returnReason);
	
	Kick(returnPlayer);
	SetPVarInt(playerid, "delay_actions", gettime() + 20);
	return true;
}

YCMD:kickall(playerid, params[], help)
{
	if(playerInfo[playerid][pAdmin] < 7)
		return returnNoAcces(playerid);

	new returnReason[64];
	if(sscanf(params, "s[64]", returnReason))
		return sendSyntaxMessage(playerid, "/kickall [reason]");

	va_SendClientMessageToAll(COLOR_LIGHTRED, "Kick all: %s has kicked all players, reason: %s.", GetName(playerid), returnReason);
	
	foreach(new i : Player)
	{
		if(playerInfo[i][pAdmin] || playerInfo[i][pHelper])
			continue;

		KickEx(i);
	}
	return true;
}
givePlayerBan(playerid, adminid, days, const reason[]) {
    new adminName[MAX_PLAYER_NAME] = "AdmBot";

    if(adminid != INVALID_PLAYER_ID) {
        GetPlayerName(adminid, adminName, MAX_PLAYER_NAME); 
    }

	new returnHardware[64];
	GPCI(playerid, returnHardware, sizeof returnHardware);

	new returnIP[16];
	GetPlayerIp(playerid, returnIP, sizeof returnIP);

	new returnQuery[512];
	mysql_format(SQL, returnQuery, sizeof returnQuery, "insert into `bans` (`name`, `bannedPlayer`, `bannedBy`, `bannedReason`, `bannedDays`, `iTimestamp`, `banHardware`, `banIP`) values ('%s', '%d', '%s', '%s', '%d', '%d', '%s', '%s');", GetName(playerid), playerInfo[playerid][pSQLID], adminName, reason, days, gettime(), returnHardware, returnIP);
	mysql_tquery(SQL, returnQuery, "", "");

	if(days == 0)
		va_SendClientMessageToAll(COLOR_LIGHTRED, "Ban: %s has been permanent banned by %s, reason: %s.", GetName(playerid), adminName, reason);
	
	else
		va_SendClientMessageToAll(COLOR_LIGHTRED, "Ban: %s has been banned by %s for %d days, reason: %s.", GetName(playerid), adminName, days, reason);
	
	KickEx(playerid);
    return 1;
}
givePlayerWarn(playerid, adminid, const reason[]) {
    new adminName[MAX_PLAYER_NAME] = "AdmBot";

    if(adminid != INVALID_PLAYER_ID) {
        GetPlayerName(adminid, adminName, MAX_PLAYER_NAME); 
        SetPVarInt(playerid, "delay_actions", gettime() + 20);
    }

    playerInfo[playerid][pWarns] ++;

    if(playerInfo[playerid][pWarns] == 3) {
        playerInfo[playerid][pWarns] = 0; 
        givePlayerBan(playerid, INVALID_PLAYER_ID, 3, "3/3 warns");
    }

    mysql_format(SQL, returnString, sizeof returnString, "UPDATE `users` SET `Warn` = %d WHERE `ID` = %d", playerInfo[playerid][pWarns], playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");

    sendSplittedMessageToAll(COLOR_LIGHTRED, COLOR_LIGHTRED, "AdmBot: %s has been warned by %s, reason: %s", GetName(playerid), adminName, reason);
    va_SendClientMessage(playerid, COLOR_GREY, "* You are warned by %s, reason: %s", adminName, reason);

    insertPlayerMail(playerInfo[playerid][pSQLID], gettime(), "Admin %s warned you, reason: %s", adminName, reason);
    return 1;
}
givePlayerMute(playerid, adminid, minutes, const reason[]) {
    new adminName[MAX_PLAYER_NAME] = "AdmBot";

    if(adminid != INVALID_PLAYER_ID) {
        GetPlayerName(adminid, adminName, MAX_PLAYER_NAME); 
        SetPVarInt(playerid, "delay_actions", gettime() + 20);
    }

    playerInfo[playerid][pMute] = gettime() + minutes * 60;

    mysql_format(SQL, returnString, sizeof returnString, "UPDATE `users` SET `Mute` = %d WHERE `ID` = %d", minutes * 60, playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");
    
    sendSplittedMessageToAll(COLOR_LIGHTRED, COLOR_LIGHTRED, "AdmBot: %s has been muted for %d minutes by %s, reason: %s", GetName(playerid), minutes, adminName, reason);
    va_SendClientMessage(playerid, COLOR_GREY, "* You are muted now for %d minutes by %s.", minutes, adminName);

    insertPlayerMail(playerInfo[playerid][pSQLID], gettime(), "Admin %s muted you for %d minutes, reason: %s", adminName, minutes, reason);
    return 1;
}

YCMD:mute(playerid, params[], help) {
    if(!playerInfo[playerid][pAdmin] && playerInfo[playerid][pHelper] < 2)
        return sendError(playerid,"Nu ai gradul administrativ necesar pentru a folosi aceasta comanda.");

	if(GetPVarInt(playerid, "delay_actions") > gettime()) 
		return va_SendClientMessage(playerid, -1, "Asteapta %d secunde pentru a folosi din nou aceasta comanda.", GetPVarInt(playerid, "delay_actions") - gettime());

    new returnPlayer, minutes, macacpemata[64]; if(sscanf(params, "uds[64]", returnPlayer, minutes, macacpemata))
        return sendSyntaxMessage(playerid, "/mute <playerid/name> <minutes> <reason>");

    if(!IsPlayerConnected(returnPlayer))
        return sendError(playerid,"Acel player nu este conectat.");

    if(returnPlayer == playerid)
        return sendError(playerid,"Nu poti folosi aceasta comanda asupra ta.");

    if(minutes < 1 || minutes > 120)
        return sendError(playerid,"Invalid minutes! (1 - 120).");

    if(playerInfo[returnPlayer][pMute] > gettime() && playerInfo[playerid][pAdmin] < 2)
        return sendError(playerid,"Jucatorul are deja mute.");

    if(playerInfo[returnPlayer][pAdmin] >= playerInfo[playerid][pAdmin])
        return sendError(playerid,"Nu poti sa-i dai mute acelui jucator.");

    return givePlayerMute(returnPlayer, playerid, minutes, macacpemata);
}

YCMD:unmute(playerid, params[], help) {
    if(!playerInfo[playerid][pAdmin] && playerInfo[playerid][pHelper] < 2)
        return sendError(playerid,"Nu ai gradul administrativ necesar pentru a folosi aceasta comanda.");

    new returnPlayer, morr[32]; if(sscanf(params, "us[32]", returnPlayer, morr))
        return sendSyntaxMessage(playerid, "/unmute <playerid/name> <reason>");

    if(!IsPlayerConnected(returnPlayer))
        return sendError(playerid,"Jucatorul nu este conectat.");

    if(playerInfo[returnPlayer][pMute] < gettime())
        return sendError(playerid,"Jucatorul nu are mute.");

    playerInfo[returnPlayer][pMute] = 0;

    mysql_format(SQL, returnString, sizeof returnString, "UPDATE `users` SET `Mute` = 0 WHERE `ID` = %d", playerInfo[returnPlayer][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");
    
    va_SendClientMessage(returnPlayer, COLOR_GREY, "* You are unmuted by admin %s, reason: %s.", GetName(playerid), morr);
    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_LIGHTRED, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): Admin %s unmuted %s, reason: %s.", GetName(playerid), GetName(returnPlayer), morr);
    return 1;
}

YCMD:warn(playerid, params[], help) {
    if(!playerInfo[playerid][pAdmin])
        return sendError(playerid,"Nu ai gradul administrativ necesar pentru a folosi aceasta comanda.");

	if(GetPVarInt(playerid, "delay_actions") > gettime()) 
		return va_SendClientMessage(playerid, -1, "Asteapta %d secunde pentru a folosi din nou aceasta comanda.", GetPVarInt(playerid, "delay_actions") - gettime());

    new returnPlayer; new sofutpemata[64]; 
    if(sscanf(params, "us[64]", returnPlayer, sofutpemata))
        return sendSyntaxMessage(playerid, "/warn <playerid> <reason>");

    if(!IsPlayerConnected(returnPlayer))
        return sendError(playerid,"Acel player nu este conectat.");

    if(playerInfo[playerid][pAdmin] < playerInfo[returnPlayer][pAdmin])
        return sendError(playerid,"Nu poti sa-i dai warn acelui player.");

    return givePlayerWarn(returnPlayer, playerid, sofutpemata);
}

YCMD:unwarn(playerid, params[], help) {
    if(playerInfo[playerid][pAdmin] < 3)
        return sendError(playerid,"Nu ai gradul administrativ necesar pentru a folosi aceasta comanda.");

	if(GetPVarInt(playerid, "delay_actions") > gettime()) 
		return va_SendClientMessage(playerid, -1, "Asteapta %d secunde pentru a folosi din nou aceasta comanda.", GetPVarInt(playerid, "delay_actions") - gettime());

    new returnPlayer;
    new ofutpemata[46];

    if(sscanf(params, "us[64]", returnPlayer, ofutpemata))
        return sendSyntaxMessage(playerid, "/unwarn <playerid> <reason>");

    if(!IsPlayerConnected(returnPlayer))
        return sendError(playerid,"Acel player nu este conectat.");

    if(playerInfo[returnPlayer][pWarns] == 0)
        return sendError(playerid,"Acel player nu are niciun warn.");

    playerInfo[returnPlayer][pWarns] --;
    SetPVarInt(playerid, "delay_actions", gettime() + 20);


    mysql_format(SQL, returnString, sizeof returnString, "UPDATE `users` SET `Warn` = %d WHERE `ID` = %d", playerInfo[returnPlayer][pWarns], playerInfo[returnPlayer][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");

    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_LIGHTRED, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s received one clear warn from Admin %s, reason: %s", GetName(returnPlayer), GetName(playerid), ofutpemata),
    va_SendClientMessage(returnPlayer, COLOR_GREY, "* You received one clear warn, reason: %s", ofutpemata);
    return 1;
}

YCMD:ban(playerid, params[], help)
{
	check_admin

	if(GetPVarInt(playerid, "delay_actions") > gettime()) 
		return va_SendClientMessage(playerid, -1, "Asteapta %d secunde pentru a folosi din nou aceasta comanda.", GetPVarInt(playerid, "delay_actions") - gettime());

	new returnPlayer, returnDays, returnReason[64];
	if(sscanf(params, "uds[64]", returnPlayer, returnDays, returnReason))
		return sendSyntaxMessage(playerid, "/ban [playername/playerid] [days | 0 for permanent] [reason]");

	if(!IsPlayerConnected(returnPlayer) || returnPlayer == playerid)
		return sendError(playerid,"Player not connected.");

	if(playerInfo[returnPlayer][pAdmin] >= playerInfo[playerid][pAdmin] && !AdminAcces(playerid)) 
		return sendError(playerid,"You can't ban a higher (or equal) level administrator.");

	new returnHardware[64];
	GPCI(returnPlayer, returnHardware, sizeof returnHardware);

	new returnIP[16];
	GetPlayerIp(returnPlayer, returnIP, sizeof returnIP);

	new returnQuery[512];
	mysql_format(SQL, returnQuery, sizeof returnQuery, "insert into `bans` (`name`, `bannedPlayer`, `bannedBy`, `bannedReason`, `bannedDays`, `iTimestamp`, `banHardware`, `banIP`) values ('%s', '%d', '%s', '%s', '%d', '%d', '%s', '%s');", GetName(returnPlayer), playerInfo[returnPlayer][pSQLID], GetName(playerid), returnReason, returnDays, gettime(), returnHardware, returnIP);
	mysql_tquery(SQL, returnQuery, "", "");

	if(returnDays == 0)
		va_SendClientMessageToAll(COLOR_LIGHTRED, "Ban: %s has been permanent banned by %s, reason: %s.", GetName(returnPlayer), GetName(playerid), returnReason);
	
	else
		va_SendClientMessageToAll(COLOR_LIGHTRED, "Ban: %s has been banned by %s for %d days, reason: %s.", GetName(returnPlayer), GetName(playerid), returnDays, returnReason);
	
	Kick(returnPlayer);
	SetPVarInt(playerid, "delay_actions", gettime() + 20);
	return true;
}

YCMD:unban(playerid, params[], help)
{
	check_admin

	if(GetPVarInt(playerid, "delay_actions") > gettime()) 
		return va_SendClientMessage(playerid, -1, "Asteapta %d secunde pentru a folosi din nou aceasta comanda.", GetPVarInt(playerid, "delay_actions") - gettime());

	new returnPlayer[MAX_PLAYERS];
	if(sscanf(params, "s[24]", returnPlayer))
		return sendSyntaxMessage(playerid, "/unban [exact name]");

	new returnQuery[512];
	mysql_format(SQL, returnQuery, sizeof returnQuery, "select * from `bans` where `name` = '%s';", returnPlayer);
	mysql_tquery(SQL, returnQuery, "unban_player", "is", playerid, returnPlayer);

	//SendClientMessage(playerid, -1, "Unbaning account...");
	//SetPVarInt(playerid, "delay_actions", gettime() + 20);
	return true;
}
function:unban_player(playerid, returnPlayer[])
{
	if(!cache_num_rows())
		return sendError(playerid,"This player is not banned.");

	new returnQuery[512];
	mysql_format(SQL, returnQuery, sizeof returnQuery, "delete from `bans` where `Name` = '%s';", returnPlayer);
	mysql_tquery(SQL, returnQuery, "", "");

	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_DCHAT, "Admin %s unbanned player %s.", GetName(playerid), returnPlayer);
	va_SendClientMessage(playerid, -1, "Player %s was succesfully unbanned.", returnPlayer);
	return true;
}

YCMD:banip(playerid, params[], help)
{
	check_admin

	if(GetPVarInt(playerid, "delay_actions") > gettime()) 
		return va_SendClientMessage(playerid, -1, "Asteapta %d secunde pentru a folosi din nou aceasta comanda.", GetPVarInt(playerid, "delay_actions") - gettime());

	new returnPlayer[16];
	if(sscanf(params, "s[16]", returnPlayer))
		return sendSyntaxMessage(playerid, "/banip [ip address]");

	new returnQuery[512];
	mysql_format(SQL, returnQuery, sizeof returnQuery, "insert into `bans` (`banIP`) values ('%s');", returnPlayer);

	va_SendClientMessage(playerid, -1, "IP Address %s was banned.", returnPlayer);
	//SendClientMessage(playerid, -1, "Unbaning IP...");
	SetPVarInt(playerid, "delay_actions", gettime() + 20);
	return true;
}

YCMD:unbanip(playerid, params[], help)
{
	check_admin

	if(GetPVarInt(playerid, "delay_actions") > gettime()) 
		return va_SendClientMessage(playerid, -1, "Asteapta %d secunde pentru a folosi din nou aceasta comanda.", GetPVarInt(playerid, "delay_actions") - gettime());

	new returnPlayer[16];
	if(sscanf(params, "s[16]", returnPlayer))
		return sendSyntaxMessage(playerid, "/unbanip [ip address]");

	new returnQuery[512];
	mysql_format(SQL, returnQuery, sizeof returnQuery, "select * from `bans` where `banIP` = '%s';", returnPlayer);
	mysql_tquery(SQL, returnQuery, "unban_ip", "is", playerid, returnPlayer);

	//SendClientMessage(playerid, -1, "Unbaning IP...");
	SetPVarInt(playerid, "delay_actions", gettime() + 20);
	return true;
}
function:unban_ip(playerid, returnPlayer[])
{
	if(!cache_num_rows())
		return sendError(playerid,"This IP is not banned.");

	new returnQuery[512];
	mysql_format(SQL, returnQuery, sizeof returnQuery, "delete from `bans` where `banIP` = '%s';", returnPlayer);
	mysql_tquery(SQL, returnQuery, "", "");

	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_DCHAT, "Admin %s unbanned IP Address %s.", GetName(playerid), returnPlayer);
	va_SendClientMessage(playerid, -1, "IP Address %s was succesfully unbanned.", returnPlayer);
	return true;
}

YCMD:closestcar(playerid, params[], help)
{
    check_admin

    if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT)
        return SendClientMessage(playerid, COLOR_WHITE, "You can only use this command while on foot.");

    new vehicleid = GetClosestVehicle(playerid, 200);

    if(vehicleid == INVALID_VEHICLE_ID || vehicleid < 1)
        return SendClientMessage(playerid, COLOR_WHITE, "No vehicles are in range.");

	PutPlayerInVehicle(playerid, vehicleid, 0);
	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_PURPLE, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): Admin %s has teleported to vehicle %d via [/closestcar].", GetName(playerid), vehicleid);
    return true;
}

YCMD:gotoxyz(playerid, params[], help)
{
	check_admin

	new Float:retPos[3], retVirtual, retInterior;
	if(sscanf(params, "fffii", retPos[0], retPos[1], retPos[2], retVirtual, retInterior))
		return sendSyntaxMessage(playerid, "/gotoxyz [X_OFFSET] [Y_OFFSET] [Z_OFFSET] | [VIRTUAL] [INTERIOR]");

	SendClientMessage(playerid, -1, "You have been teleported.");
	SetPlayerPos(playerid, retPos[0], retPos[1], retPos[2]);

	SetPlayerVirtualWorld(playerid, retVirtual);
	SetPlayerInterior(playerid, retInterior);
    return true;
}

YCMD:gotoa(playerid, params[], help)
{
	check_admin

	SetPlayerInterior(playerid, 0);	
	SetPlayerVirtualWorld(playerid, 0);

	if(IsPlayerInAnyVehicle(playerid))
		SetVehiclePos(GetPlayerVehicleID(playerid), 1477.1122, 1713.9915, 10.8125), SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), 0), LinkVehicleToInterior(GetPlayerVehicleID(playerid), 0);

	else
		SetPlayerPos(playerid, 1477.1122, 1713.9915, 10.8125);

	SendClientMessage(playerid, COLOR_GREY, "Welcome to LV airport.");
	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s has used /gotoa.", GetName(playerid));
	return true;
}

YCMD:aaa2(playerid, params[], help)
{
	check_admin

	SetPlayerInterior(playerid, 0);	
	SetPlayerVirtualWorld(playerid, 1337);

	if(IsPlayerInAnyVehicle(playerid))
		SetVehiclePos(GetPlayerVehicleID(playerid), 1477.1122, 1713.9915, 10.8125), SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), 0), LinkVehicleToInterior(GetPlayerVehicleID(playerid), 0);

	else
		SetPlayerPos(playerid, 1477.1122, 1713.9915, 10.8125);

	SendClientMessage(playerid, -1, "Welcome to LV airport (in virtual world).");
	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s has used /aaa2.", GetName(playerid));
	return true;
}

YCMD:aaa3(playerid, params[], help)
{
	check_admin

	SetPlayerInterior(playerid, 0);	
	SetPlayerVirtualWorld(playerid, 1338);

	if(IsPlayerInAnyVehicle(playerid))
		SetVehiclePos(GetPlayerVehicleID(playerid), 1477.1122, 1713.9915, 10.8125), SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), 0), LinkVehicleToInterior(GetPlayerVehicleID(playerid), 0);

	else
		SetPlayerPos(playerid, 1477.1122, 1713.9915, 10.8125);

	SendClientMessage(playerid, -1, "Welcome to LV airport (in virtual world).");
	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s has used /aaa3.", GetName(playerid));
	return true;
}

YCMD:gotols(playerid, params[], help)
{
	check_admin

	SetPlayerInterior(playerid, 0);	
	SetPlayerVirtualWorld(playerid, 0);

	if(IsPlayerInAnyVehicle(playerid))
		SetVehiclePos(GetPlayerVehicleID(playerid), 1529.6, -1691.2, 13.3), SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), 0), LinkVehicleToInterior(GetPlayerVehicleID(playerid), 0);

	else
		SetPlayerPos(playerid, 1529.6, -1691.2, 13.3);

	SendClientMessage(playerid, COLOR_GREY, "You have been teleported.");
	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s has used /gotols.", GetName(playerid));
	return true;
}

YCMD:gotolv(playerid, params[], help)
{
	check_admin

	SetPlayerInterior(playerid, 0);	
	SetPlayerVirtualWorld(playerid, 0);

	if(IsPlayerInAnyVehicle(playerid))
		SetVehiclePos(GetPlayerVehicleID(playerid), 1699.2, 1435.1, 10.7), SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), 0), LinkVehicleToInterior(GetPlayerVehicleID(playerid), 0);

	else
		SetPlayerPos(playerid, 1699.2, 1435.1, 10.7);

	SendClientMessage(playerid, COLOR_GREY, "You have been teleported.");
	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s has used /gotolv.", GetName(playerid));
	return true;
}

YCMD:gotosf(playerid, params[], help)
{
	check_admin

	SetPlayerInterior(playerid, 0);	
	SetPlayerVirtualWorld(playerid, 0);

	if(IsPlayerInAnyVehicle(playerid))
		SetVehiclePos(GetPlayerVehicleID(playerid), -1417.0, -295.8, 14.1), SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), 0), LinkVehicleToInterior(GetPlayerVehicleID(playerid), 0);

	else
		SetPlayerPos(playerid, -1417.0, -295.8, 14.1);

	SendClientMessage(playerid, COLOR_GREY, "You have been teleported.");
	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s has used /gotosf.", GetName(playerid));
	return true;
}

YCMD:a(playerid, params[], help)
{
	check_admin

	new returnMessage[144];
	if(sscanf(params, "s[144]", returnMessage))
		return sendSyntaxMessage(playerid, "/a [message]");

	sendStaffMessage(STAFF_TYPE_ADMIN, 0xFFC266AA, "(%d) Admin %s: %s", playerInfo[playerid][pAdmin], GetName(playerid), returnMessage);
	return true;
}

YCMD:flymode(playerid, params[], help)
{
	check_admin

    if(!playerStaticVar[playerid][useFly]) {
        if(!IsPlayerInAnyVehicle(playerid)) { 
            InitFly(playerid);
            StartFly(playerid);

            playerStaticVar[playerid][useFly] = 1;
            SetPlayerHealthEx(playerid, 99999);
            sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s is now in flymode.", GetName(playerid));
        }
        else {
            new Float:posX, Float:posY, Float:posZ, Float:posA;
            GetVehiclePos(GetPlayerVehicleID(playerid), posX, posY, posZ), GetVehicleZAngle(GetPlayerVehicleID(playerid), posA);
            
            posX += (7.5 * floatsin(-posA, degrees));
            posY += (7.5 * floatcos(-posA, degrees));

            SetVehiclePos(GetPlayerVehicleID(playerid), posX, posY, posZ + 5);
        }
    }
    else {
        StopFly(playerid);
        playerStaticVar[playerid][useFly] = 0;
        SetPlayerHealthEx(playerid, 99.0);

        sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s stopped flymode.", GetName(playerid));
    }
	return true;
}


function:checkStaffCategory(const playerid, const listitem) {
    if(!IsPlayerConnected(playerid))
        return 1;

    new 
        name[MAX_PLAYER_NAME], lastlogin[100],
		lastOn[6],
        rows = cache_num_rows();

    if(listitem) {
        if(!rows)
            return sendError(playerid,"Nu au fost gasiti helperi in baza de date.");

        

        returnString = "Name\tStatus [last login]\tHWarns\tRaport\n";

        for(new i; i < rows; i++) {
            cache_get_value_name(i, "Name", name);
            cache_get_value_name(i, "lastOnlineTime", lastlogin);
			
			new lastloginInt = strval(lastlogin);
			returnDateFromTimestamp(lastloginInt, lastOn[0], lastOn[1], lastOn[2], lastOn[3], lastOn[4], lastOn[5], 2);
            
			static playerHelper, playerStatus, playerHelperWarns, playerHelpedPlayers;

			cache_get_value_name_int(i, "Helper", playerHelper);
			cache_get_value_name_int(i, "Status", playerStatus);
			cache_get_value_name_int(i, "HelperWarns", playerHelperWarns);
			cache_get_value_name_int(i, "PlayersHelped", playerHelpedPlayers);

			format(returnString, sizeof returnString, "%s(%d) %s\t%s [%s]\t%d/3\t%d (effectued)\n", returnString, playerHelper, name, !playerStatus ? "offline" : "online", lastlogin, playerHelperWarns, playerHelpedPlayers);
        }
        return Dialog_Show(playerid, 0, DIALOG_STYLE_TABLIST_HEADERS, "SERVER: Helpers", returnString, "Exit", "");
    }
    
    if(!rows)
        return sendError(playerid,"Nu au fost gasiti admini in baza de date.");

    

    returnString = "Name\tStatus [last login]\tAWarns\tRaport\n";

    for(new i; i < rows; i++) {
        cache_get_value_name(i, "Name", name);
        cache_get_value_name(i, "LastLogin", lastlogin);

		new lastloginInt2 = strval(lastlogin);
		returnDateFromTimestamp(lastloginInt2, lastOn[0], lastOn[1], lastOn[2], lastOn[3], lastOn[4], lastOn[5], 2);

		static playerAdmin, playerStatus, playerAdminWarns, playerPlayersHelped;

		cache_get_value_name_int(i, "Admin", playerAdmin);
		cache_get_value_name_int(i, "Status", playerStatus);
		cache_get_value_name_int(i, "AdminWarns", playerAdminWarns);
		cache_get_value_name_int(i, "PlayersHelped", playerPlayersHelped);

        format(returnString, sizeof returnString, "%s(%d) %s\t%s [%s]\t%d/3\t%d (effectued)\n", returnString, playerAdmin, name, !playerStatus ? "offline" : "online", lastlogin, playerAdminWarns, playerPlayersHelped);
    }
    return Dialog_Show(playerid, 0, DIALOG_STYLE_TABLIST_HEADERS, "SERVER: Admins", returnString, "Exit", "");
} 

hook OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	if(playerStaticVar[playerid][useFly])
		SetPlayerHealthEx(playerid, 99999);

	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(newkeys == KEY_SECONDARY_ATTACK)
	{
		if(playerStaticVar[playerid][useFly])
		{
			StopFly(playerid);
			playerStaticVar[playerid][useFly] = 0;
			SetPlayerHealthEx(playerid, 99.0);

			return true;
		}
	}
	if(newkeys & KEY_FIRE && IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		if(playerInfo[playerid][pAdmin] && playerStaticVar[playerid][speedBoost])
		{
			new Float:x, Float:y, Float:z;

			GetVehicleVelocity(GetPlayerVehicleID(playerid), x, y, z);
			SetVehicleVelocity(GetPlayerVehicleID(playerid), x * 1.8, y * 1.8, z * 1.8);
			return true;
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:goto(playerid, params[], help)
{
	check_admin
	
	new teleportPlayer;
	if(sscanf(params, "u", teleportPlayer))
		return sendSyntaxMessage(playerid, "/goto [playername/playerid]");

	if(!IsPlayerConnected(teleportPlayer) || teleportPlayer == playerid)
        return sendError(playerid,"Player not connected.");

	if(playerInfo[teleportPlayer][pAdmin] >= playerInfo[playerid][pAdmin] && !AdminAcces(playerid)) 
		return sendError(playerid,"You can't teleport to a higher (or equal) level administrator.");

	new Float:Pos[3];
	GetPlayerPos(teleportPlayer, Pos[0], Pos[1], Pos[2]);
	SetPlayerPos(playerid, Pos[0], Pos[1] + 2, Pos[2]);
	
	SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(teleportPlayer));
	SetPlayerInterior(playerid, GetPlayerInterior(teleportPlayer));

	playerStaticVar[playerid][inBusiness] = playerStaticVar[teleportPlayer][inBusiness];
	playerStaticVar[playerid][inHouse] = playerStaticVar[teleportPlayer][inHouse];
	playerStaticVar[playerid][inHQ] = playerStaticVar[teleportPlayer][inHQ];

	va_SendClientMessage(playerid, COLOR_SERVER, "[INFO]: {ffffff}You have been teleported to %s.", GetName(teleportPlayer));
	va_SendClientMessage(teleportPlayer, COLOR_WHITE, "Admin %s has teleported to you.", GetName(playerid));

	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s used /goto on %s.", GetName(playerid), GetName(teleportPlayer));
	return true;
}
YCMD:gethere(playerid, params[], help) {
    check_admin

    new returnPlayer, Float:X, Float:Y, Float:Z; if(sscanf(params, "u", returnPlayer))
        return sendSyntaxMessage(playerid, "/gethere <playerid/name>");

    if(!playerStaticVar[returnPlayer][playerLogged])
        return sendError(playerid,"Acel player nu este logat.");

    if(returnPlayer == playerid)
        return sendError(playerid,"Nu poti folosi aceasta comanda asupra ta.");

    if(playerInfo[returnPlayer][pAdmin] > playerInfo[playerid][pAdmin])
        return sendError(playerid,"Nu poti folosi aceasta comanda pe player-ul respectiv.");

    GetPlayerPos(playerid, X,Y,Z);

    if(GetPlayerState(returnPlayer) == PLAYER_STATE_DRIVER) 
        SetVehiclePos(GetPlayerVehicleID(returnPlayer), X,Y+1.5,Z);
    
    else SetPlayerPos(returnPlayer, X,Y+1.5,Z);

    SetPlayerVirtualWorld(returnPlayer, GetPlayerVirtualWorld(playerid));
    SetPlayerInterior(returnPlayer, GetPlayerInterior(playerid));

	playerStaticVar[returnPlayer][inBusiness] = playerStaticVar[playerid][inBusiness];
	playerStaticVar[returnPlayer][inHouse] = playerStaticVar[playerid][inHouse];
	playerStaticVar[returnPlayer][inHQ] = playerStaticVar[playerid][inHQ];

    va_SendClientMessage(playerid, COLOR_GREY, "* L-ai teleportat pe %s la tine.", GetName(returnPlayer)); 
    va_SendClientMessage(returnPlayer, COLOR_GREY, "* %s te-a teleportat la el.", GetName(playerid));
    return 1;
}

timer set_admin_off_duty[60000](playerid)
{	
	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "Notice: {FFFFFF}Admin %s is now off administrative duty(60 seconds passed).", GetDBName(playerid));
	
	SetPlayerHealthEx(playerid, 100);
	va_Command_ReProcess(playerid, "turn on", false);
}
YCMD:giveadminwarn(playerid, params[], help) {
    check_owner

    new returnPlayer;  if(sscanf(params, "us[64]", returnPlayer, returnString))
        return sendSyntaxMessage(playerid, "/giveadminwarn <playerid/name> <reason>");

    if(returnPlayer == playerid)
        return sendError(playerid,"Nu poti folosi aceasta comanda asupra ta.");

    if(!IsPlayerConnected(returnPlayer))
        return sendError(playerid,"Player not connected.");

    if(!playerInfo[returnPlayer][pAdmin])
        return sendError(playerid,"Acel player nu este admin.");

    if(strlen(returnString) < 1 || strlen(returnString) >= 64)
        return sendError(playerid,"Motiv-ul trebuie sa fie cuprins intre 1 si 64 caractere.");

    if(playerInfo[returnPlayer][pAdmin] > playerInfo[playerid][pAdmin])
        return sendError(playerid,"Nu poti face asta pe acel player.");

    playerInfo[returnPlayer][pAdminWarns] ++;

    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): Admin %s gived an admin warn to %s (%d/3). Reason: %s.", GetName(playerid), GetName(returnPlayer), playerInfo[returnPlayer][pAdminWarns], returnString);
    
    if(playerInfo[returnPlayer][pAdminWarns] >= 3) {
        sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s was demoted from admin because he has accumulated 3/3 admin warns.", GetName(returnPlayer));
        Iter_Remove(serverAdmins, returnPlayer);

        playerInfo[returnPlayer][pAdmin] = playerInfo[returnPlayer][pPlayersHelped] = playerInfo[returnPlayer][pAdminWarns] = 0;
    }
    return va_mysql_tquery("UPDATE `server_users` SET `AdminWarns` = %d, `Admin` = %d, `PlayersHelped` = %d WHERE `ID` = %d", playerInfo[returnPlayer][pAdminWarns], playerInfo[returnPlayer][pAdmin], playerInfo[returnPlayer][pPlayersHelped], playerInfo[returnPlayer][pSQLID]);
}
YCMD:givehelperwarn(playerid, params[], help) {
    check_owner

    new returnPlayer;  if(sscanf(params, "us[64]", returnPlayer, returnString))
        return sendSyntaxMessage(playerid, "/givehelperwarn <playerid/name> <reason>");

    if(returnPlayer == playerid)
        return sendError(playerid,"Nu poti folosi aceasta comanda asupra ta.");

    if(!IsPlayerConnected(returnPlayer))
        return sendError(playerid,"Player not connected.");
		
    if(!playerInfo[returnPlayer][pHelper])
        return sendError(playerid,"Acel player nu este helper.");

    if(strlen(returnString) < 1 || strlen(returnString) >= 64)
        return sendError(playerid,"Motiv-ul trebuie sa fie cuprins intre 1 si 64 caractere.");

    playerInfo[returnPlayer][pHelperWarns] ++;

    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): Admin %s gived an helper warn to %s (%d/3). Reason: %s.", GetName(playerid), GetName(returnPlayer), playerInfo[returnPlayer][pHelperWarns], returnString);
    
    if(playerInfo[returnPlayer][pHelperWarns] >= 3) {
        sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s was demoted from helper because he has accumulated 3/3 helper warns.", GetName(returnPlayer));

        Iter_Remove(serverHelpers, returnPlayer);

        playerInfo[returnPlayer][pHelper] = playerInfo[returnPlayer][pPlayersHelped] = playerInfo[returnPlayer][pHelperWarns] = 0;
    }
    return va_mysql_tquery("UPDATE `users` SET `HelperWarns` = %d, `Helper` = %d, `PlayersHelped` = %d WHERE `ID` = %d", playerInfo[returnPlayer][pHelperWarns], playerInfo[returnPlayer][pHelper], playerInfo[returnPlayer][pPlayersHelped], playerInfo[returnPlayer][pSQLID]);
}
YCMD:aduty(playerid, params[], help)
{
	check_admin

	if(!playerStaticVar[playerid][adminDuty])
	{
		sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "Notice: {FFFFFF}Admin %s is now on administrative duty.", GetDBName(playerid));
	
		SetPlayerHealthEx(playerid, cellmax);
		va_Command_ReProcess(playerid, "turn off", false);

		if(playerInfo[playerid][pAdmin] < 3)
			defer set_admin_off_duty(playerid);
	}
	else
	{
		sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "Notice: {FFFFFF}Admin %s is now off administrative duty.", GetDBName(playerid));
	
		SetPlayerHealthEx(playerid, 100);
		va_Command_ReProcess(playerid, "turn on", false);
	}
	playerStaticVar[playerid][adminDuty] = playerStaticVar[playerid][adminDuty] ? 0 : 1;
	return true;
}

hook OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	if(!playerStaticVar[playerid][adminDuty])
		return Y_HOOKS_CONTINUE_RETURN_1;

	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
		SetVehiclePos(GetPlayerVehicleID(playerid), fX, fY, fZ);
	else
		SetPlayerPos(playerid, fX, fY, fZ);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

CMD:anno(playerid, params[])
{
    if(!playerInfo[playerid][pAdmin])
    	return true;

    new returnText[128];
    if(sscanf(params, "s[128]", returnText))
    	return sendSyntaxMessage(playerid, "/anno <Message>");

	va_SendClientMessageToAll(COLOR_GENANNOUNCE, "(( %s says: %s ))", GetDBName(playerid), returnText);
	return true;
}
CMD:anno2(playerid, params[])
{
    if(!playerInfo[playerid][pAdmin])
    	return true;

    new returnText[128];
    if(sscanf(params, "s[128]", returnText))
    	return sendSyntaxMessage(playerid, "/anno2 <Message>");

	va_SendClientMessageToAll(0xff3333FF, "(( %s says: %s ))", GetDBName(playerid), returnText);
	return true;
}

YCMD:setvw(playerid, params[], help)
{
  	check_admin

    new returnPlayer, returnVirtual;
    
    if(sscanf(params, "ud", returnPlayer, returnVirtual)) 
        return sendSyntaxMessage(playerid, "/setvw [playerid/playername] [virtual]");

    if(!IsPlayerConnected(returnPlayer))
        return sendError(playerid,"Player not connected.");

    va_SendClientMessage(playerid, -1, "You updated %s's virtual world to %d.", GetName(returnPlayer), returnVirtual);
    va_SendClientMessage(returnPlayer, -1, "Admin %s has set your virtual world to %d.", GetName(playerid), returnVirtual);
    SetPlayerVirtualWorld(returnPlayer, returnVirtual);
    return true;
}

YCMD:sethp(playerid, params[], help)
{
  	check_admin

    new returnPlayer, returnHP;
    
    if(sscanf(params, "ud", returnPlayer, returnHP)) 
        return sendSyntaxMessage(playerid, "/sethp [playerid/playername] [hp]");

    if(!IsPlayerConnected(returnPlayer))
        return sendError(playerid,"Player not connected.");

    va_SendClientMessage(playerid, -1, "You updated %s's health to %d.", GetName(returnPlayer), returnHP);
    va_SendClientMessage(returnPlayer, -1, "Admin %s has set your health to %d.", GetName(playerid), returnHP);
	
	SetPlayerHealthEx(returnPlayer, returnHP);
    return true;
}

YCMD:givegun(playerid, params[], help)
{
	check_admin

    new targetID, weapID, weapBullets;
    if(sscanf(params, "udd", targetID, weapID, weapBullets)) 
        return sendSyntaxMessage(playerid, "/givegun [playername/playerid] [weapon id] [ammo]");

    if(!IsPlayerConnected(targetID))
        return sendError(playerid,"Player not connected.");

    if(weapID < 1 || weapID > 46 || weapID == 19 || weapID == 20 || weapID == 21 || weapID == 45) 
        return sendError(playerid,"Invalid weapon.");

    if(!playerInfo[targetID][pGunLic]) 
        return SendClientMessage(playerid, COLOR_GREY, "This player doesn't have gun licence.");

    GivePlayerWeapon(targetID, weapID, weapBullets);


    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Staff Info{ffffff}): %s has given weapon %s to %s with %d bullets.", GetName(playerid), GetWeaponNameEx(weapID), GetName(targetID), weapBullets);
    return true;
}

YCMD:agl(playerid, params[], help)
{
	check_admin

	new returnPlayer, returnLicense[32];
	if(sscanf(params, "us[32]", returnPlayer, returnLicense))
	{
		sendSyntaxMessage(playerid, "/agl [playerid/playername] [license]");
		return SendClientMessage(playerid, COLOR_GREY, "Licences: Driving, Weapon, Flying, Sailing");
	}

	if(strmatch(returnLicense, "Driving"))
	{	
		playerInfo[returnPlayer][pCarLic] = 100;
		va_SendClientMessage(returnPlayer, -1, "Admin %s has given you drivers licence.", GetName(playerid));

		sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_DCHAT, "Admin %s has given %s drivers licence.", GetName(playerid), GetName(returnPlayer));
	}
	else if(strmatch(returnLicense, "weapon"))
	{	
		playerInfo[returnPlayer][pGunLic] = 100;
		va_SendClientMessage(returnPlayer, -1, "Admin %s has given you weapon licence.", GetName(playerid));

		sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_DCHAT, "Admin %s has given %s weapon licence.", GetName(playerid), GetName(returnPlayer));
	}
	else if(strmatch(returnLicense, "flying"))
	{	
		playerInfo[returnPlayer][pFlyLic] = 100;
		va_SendClientMessage(returnPlayer, -1, "Admin %s has given you flying licence.", GetName(playerid));

		sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_DCHAT, "Admin %s has given %s flying licence.", GetName(playerid), GetName(returnPlayer));
	}
	else if(strmatch(returnLicense, "sailing"))
	{	
		playerInfo[returnPlayer][pBoatLic] = 100;
		va_SendClientMessage(returnPlayer, -1, "Admin %s has given you sailing licence.", GetName(playerid));

		sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_DCHAT, "Admin %s has given %s sailing licence.", GetName(playerid), GetName(returnPlayer));
	}
	else
		return SendClientMessage(playerid, COLOR_GREY, "Licences: Weapon, Flying, Sailing");

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `WeaponLicence` = '%d', `FlyLicence` = '%d', `SailingLicence` = '%d', `DrivingLicence` = '%d' where `id` = '%d';", playerInfo[returnPlayer][pGunLic], playerInfo[returnPlayer][pFlyLic], playerInfo[returnPlayer][pBoatLic], playerInfo[returnPlayer][pCarLic], playerInfo[returnPlayer][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}
YCMD:healme(playerid, params[], help)
{
	check_admin

	SetPlayerHealth(playerid, 100);
	return true;
}

YCMD:acolor(playerid, params[], help) {
    if(!playerInfo[playerid][pAdmin])
        return sendError(playerid, "Nu ai gradul administrativ necesar pentru a folosi aceasta comanda.");

    gDialog[0] = EOS;

    for(new i = 0; i < sizeof adminColors; ++i) 
        format(gDialog, sizeof gDialog, "%s{%s}%s\n", gDialog, adminColors[i][colorValue], adminColors[i][colorName]);

    Dialog_Show(playerid, ADMINCOLORS, DIALOG_STYLE_LIST, "SERVER: Admin Colors", gDialog, "Select", "Exit");
    return true;
}

Dialog:ADMINCOLORS(playerid, response, listitem, inputtext[]) {
    if(!response)
        return 1;

    playerInfo[playerid][pAdminColor] = listitem;

    va_mysql_tquery("UPDATE `users` SET `AdminColor` = %d WHERE `ID` = %d", listitem, playerInfo[playerid][pSQLID]);
    SendClientMessage(playerid, -1, "You have set your color to {%s}%s{ffffff}.", adminColors[listitem][colorValue], adminColors[listitem][colorName]);
    return true;
}

YCMD:speed(playerid, params[], help)
{
	check_admin

	va_SendClientMessage(playerid, -1, "Ai %s Speed Boost!", playerStaticVar[playerid][speedBoost] ? ("dezactivat") : ("activat"));
	playerStaticVar[playerid][speedBoost] = playerStaticVar[playerid][speedBoost] ? 0 : 1;
	return true;
}

YCMD:admins(playerid, params[], help)
{
	SendClientMessage(playerid, COLOR_RED2, "{408080}-----Admins Online------------------------");

	foreach(new i : serverAdmins)
	{
    	if(playerInfo[playerid][pAdmin] < 5)
			va_SendClientMessage(playerid, -1, " {EEEEEE}(%d) %s - %s", i, GetName(i), (playerInfo[i][pAdmin] == 8 ? ("scripter") : (playerInfo[i][pAdmin] == 7 ? ("administrator") : (playerInfo[i][pAdmin] == 6 ? ("manager") : va_return("admin level %d", playerInfo[i][pAdmin])))));
		
		else
			va_SendClientMessage(playerid, -1, " {EEEEEE}(%d) %s - %s%s%s", i, GetName(i), (playerInfo[i][pAdmin] == 8 ? ("scripter") : (playerInfo[i][pAdmin] == 7 ? ("administrator") : (playerInfo[i][pAdmin] == 6 ? ("manager") : va_return("admin level %d", playerInfo[i][pAdmin])))), (playerStaticVar[playerid][afkTime] ? va_return(" - AFK: %0.0f", playerStaticVar[i][afkTime]) : ("")), (Iter_Contains(helpersOnDuty, i) ? va_return(" spectating %s (%d)", GetName(Spectate[i]), Spectate[i]) : ("")));
	}

	SendClientMessage(playerid, -1, "{408080}--------------------------------------------");
	SendClientMessage(playerid, -1, "Daca ai vreo problema, poti folosi /report. Pentru intrebari legate de joc poti folosi /n.");
	return SendClientMessage(playerid, -1, "{408080}--------------------------------------------");
}

YCMD:helpers(playerid, params[], help)
{
	SendClientMessage(playerid, COLOR_RED2, "{408080}-----Helpers Online------------------------");

	foreach(new i : serverHelpers)
	{
    	if(playerInfo[playerid][pAdmin] < 4)
			va_SendClientMessage(playerid, -1, " {EEEEEE}(%d) %s - helper level %d", i, GetName(i), playerInfo[i][pHelper]);

		else
			va_SendClientMessage(playerid, -1, " {EEEEEE}(%d) %s - helper level %d%s%s", i, GetName(i), playerInfo[i][pHelper], (playerStaticVar[playerid][afkTime] ? va_return(" - AFK (%0.0f)", playerStaticVar[i][afkTime]) : ("")), (Iter_Contains(helpersOnDuty, i) ? (" - on duty") : ("")));
	}

	SendClientMessage(playerid, -1, "{408080}--------------------------------------------");
	SendClientMessage(playerid, -1, "Daca ai vreo intrebare legata de server, foloseste /n.");
	return SendClientMessage(playerid, -1, "{408080}--------------------------------------------");
}

WhenPlayerSpecOff(playerid)
{
    if(!Iter_Contains(Spectators, playerid))
        return true;

	TogglePlayerControllable(playerid, 1);
	TogglePlayerSpectating(playerid, 0);

	PlayerTextDrawHide(playerid, playerTextdraws[playerid][spectateTD]);
	Spectate[playerid] = INVALID_PLAYER_ID;

	Iter_Remove(Spectators, playerid);    
	DeletePVar(playerid, "spectate_state");

	for(new l; l < 5; l++)
		SendDeathMessageToPlayer(playerid, 1001, 1001, 1001);
    return true;
}

task WhenSpecGotUpdated[2000]()
{
	if(!Iter_Count(Spectators))
		return true;

	foreach(new i : Spectators)
	{
		new spectatedPlayer = Spectate[i];
		if(!IsPlayerConnected(spectatedPlayer))
		    return WhenPlayerSpecOff(i);

		new vehicleid = GetPlayerVehicleID(spectatedPlayer), Float: playerHP, Float: vehicleHealth;

		GetPlayerHealth(spectatedPlayer, playerHP);
		GetVehicleHealth(vehicleid, vehicleHealth);

		va_PlayerTextDrawSetString(i, playerTextdraws[i][spectateTD], "%s (%d) (level %d, %.2f hours)~n~State: %s (%.1f health)~n~Jailed: %s (wanted: %d)~n~Framerate %d~n~Ping: %d (%.2f packet loss)", GetName(spectatedPlayer), spectatedPlayer, playerInfo[spectatedPlayer][pLevel], playerInfo[spectatedPlayer][pHours], GetPlayerState(spectatedPlayer) == 1 ? ("ON-FOOT") : GetPlayerState(spectatedPlayer) == 2 ? ("DRIVER") : GetPlayerState(spectatedPlayer) == 3 ? ("PASSENGER") : ("UNKNOWN"), playerHP, playerInfo[spectatedPlayer][pJail] ? ("~r~YES") : ("NO"), playerInfo[spectatedPlayer][pWantedLevel], GetPlayerFPS(Spectate[i]), GetPlayerPing(Spectate[i]), NetStats_PacketLossPercent(Spectate[i]));
		PlayerTextDrawShow(i, playerTextdraws[i][spectateTD]);

		if(GetPlayerInterior(spectatedPlayer) != GetPlayerInterior(i))
		    SetPlayerInterior(i, GetPlayerInterior(spectatedPlayer)), PlayerSpectatePlayer(i, spectatedPlayer);

		if(GetPlayerVirtualWorld(spectatedPlayer) != GetPlayerVirtualWorld(i))
		    SetPlayerVirtualWorld(i, GetPlayerVirtualWorld(spectatedPlayer)), PlayerSpectatePlayer(i, spectatedPlayer);

		if(IsPlayerInAnyVehicle(spectatedPlayer))
			PlayerSpectateVehicle(i, GetPlayerVehicleID(spectatedPlayer));

		else PlayerSpectatePlayer(i, spectatedPlayer);
	}
	return true;
}

YCMD:spec(playerid, params[], help)
{
	check_admin

	new returnPlayer;
	if(sscanf(params, "u", returnPlayer))
		return sendSyntaxMessage(playerid, "/spec <Name/Playerid>");

	if(returnPlayer == INVALID_PLAYER_ID || returnPlayer == playerid || !IsPlayerConnected(returnPlayer))
		return sendError(playerid,"Player not connected.");

	TogglePlayerSpectating(playerid, true);
	PlayerSpectatePlayer(playerid, returnPlayer);

	Spectate[playerid] = returnPlayer;
	Iter_Add(Spectators, playerid);

	SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(returnPlayer));
	SetPlayerInterior(playerid, GetPlayerInterior(returnPlayer));

	PlayerTextDrawSetString(playerid, playerTextdraws[playerid][spectateTD] , "LOADING...");
	PlayerTextDrawShow(playerid, playerTextdraws[playerid][spectateTD]);

	if(playerInfo[playerid][pAdmin])
    {
        if(Iter_Contains(Cheaters, returnPlayer)) 
        {
            new reportedByPlayer = GetPVarInt(returnPlayer, "reported_by_player");
            SendClientMessage(reportedByPlayer, COLOR_GREY, "Reportul dvs este in curs de verificare.");

            playerInfo[playerid][pPlayersHelped] ++;

            sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCHAT, "Admin %s is now spectating %s (reported for cheats).", GetName(playerid), GetName(Spectate[playerid]));
            updateCheaterVariables(returnPlayer);
        }
        else if(GetPVarInt(returnPlayer, "reported_for") == REPORT_TYPE_DM) 
        {
            new reportedByPlayer = GetPVarInt(returnPlayer, "reported_by");
            SendClientMessage(reportedByPlayer, COLOR_GREY, "Reportul dvs este in curs de verificare.");

			playerInfo[playerid][pPlayersHelped] ++;

            sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCHAT, "Admin %s is now spectating %s (reported for DM).", GetName(playerid), GetName(returnPlayer));
            updateReportVariables(reportedByPlayer);
        }
        else if(GetPVarInt(returnPlayer, "report_type") == REPORT_TYPE_STUCK) 
        {
            SendClientMessage(returnPlayer, COLOR_GREY, "Reportul dvs este in curs de verificare.");
            
			playerInfo[playerid][pPlayersHelped] ++;

            sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCHAT, "Admin %s is now spectating %s (reported for being stuck).", GetName(playerid), GetName(Spectate[playerid]));    
            updateReportVariables(returnPlayer);
        }
    }

    new Float:returnHealth, weaponId, haveGun, weaponAmmo;
    GetPlayerHealth(Spectate[playerid], returnHealth);
	
    format(returnString, sizeof returnString, "%s's guns:", GetName(returnPlayer));
    for(new i; i < 13; i++) 
    {
    	GetPlayerWeaponData(returnPlayer, WEAPON_SLOT:i, weaponId, weaponAmmo);

        if(weaponId >= 16 && weaponAmmo >= 1)
			format(returnString, sizeof returnString, "%s [%s-%d]", returnString, GetWeaponNameEx(weaponId), weaponAmmo), haveGun = 1;
    }
    if(haveGun) SendClientMessage(playerid, COLOR_GREY, returnString);
    else va_SendClientMessage(playerid, COLOR_GREY, "%s doesn't have any weapon!", GetName(returnPlayer));

    va_SendClientMessage(playerid, -1, "(%d) %s | Level: %d | Health: %.0f | AFK: %.0f | Duty: %d | Ping: %d | Virtual: %d | Interior: %d", Spectate[playerid], GetName(Spectate[playerid]), playerInfo[Spectate[playerid]][pLevel], returnHealth, playerStaticVar[playerid][afkTime], playerStaticVar[Spectate[playerid]][groupDuty], GetPlayerPing(Spectate[playerid]), GetPlayerVirtualWorld(Spectate[playerid]), GetPlayerInterior(Spectate[playerid]));

	new specFormat[144], i;
	specFormat[0] = EOS;

    foreach(i : Spectators)
	{
		if(Spectate[i] != returnPlayer)
			continue;

		format(specFormat, sizeof specFormat, "%s%s", specFormat, GetDBName(i));
	}

	if(i)
		va_SendClientMessage(playerid, -1, "{FF0000}%s (%d) is also spectated by %s", GetName(returnPlayer), returnPlayer, specFormat);

	va_mysql_tquery("UPDATE `users` SET `PlayersHelped` = %d WHERE `ID` = %d", playerInfo[playerid][pPlayersHelped], playerInfo[playerid][pSQLID]);
	return true;
}

YCMD:specoff(playerid, params[], help)
	return WhenPlayerSpecOff(playerid);

hook OnPlayerWeaponShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	foreach(new i : Spectators)
	{
		if(Spectate[i] != playerid)
			continue;

		if(hittype == BULLET_HIT_TYPE_PLAYER)
		{
			playerStaticVar[playerid][DMInfo][0] = fX;
			playerStaticVar[playerid][DMInfo][1] = fY;
			playerStaticVar[playerid][DMInfo][2] = fZ;
		}
		else if(hittype == BULLET_HIT_TYPE_VEHICLE)
			va_SendClientMessage(i, COLOR_DARKPINK, "DMG: %s did damage to vehicle %d using weapon %s", GetName(playerid), hitid, GetWeaponNameEx(weaponid));
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
	foreach(new i : Spectators)
	{
		if(Spectate[i] != playerid)
			continue;

		va_SendClientMessage(i, COLOR_DARKPINK, "DMG: %s did %f damage to %s using weapon %s. [hit: %f %f %f] [bodypart: %d]", GetName(playerid), amount, damagedid, GetWeaponNameEx(weaponid), playerStaticVar[playerid][DMInfo][0], playerStaticVar[playerid][DMInfo][1], playerStaticVar[playerid][DMInfo][2], bodypart);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook whenPlayerDie(playerid, killerid, reason)
{
	foreach(new i : Spectators)
	{
		if(Spectate[i] != playerid)
			continue;

		va_SendClientMessage(i, COLOR_RED, "%s was killed by %s with %s, distance: %f", GetName(playerid), GetName(killerid), GetWeaponNameEx(reason), GetDistanceBetweenPlayers(playerid, killerid));
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}


update_report_deelay(playerid)
{
	if(GetPVarInt(playerid, "report_delay") <= gettime() && GetPVarInt(playerid, "report_delay"))
	{
		SendClientMessage(playerid, COLOR_WHITE, "Acum puteti trimite un nou report.");
		return updateReportVariables(playerid, 1);
	}
	return true;
}

onCheaterReported(reportID, cheaterID = -1, const text[], time, autoassign)
{
    if(!IsPlayerConnected(reportID))
        return true;

    if(!autoassign)
    {
        if(!Iter_Contains(Cheaters, cheaterID)) Iter_Add(Cheaters, cheaterID);

        SetPVarInt(cheaterID, "cheater_report_time", time);
        SetPVarInt(cheaterID, "reported_by_player", reportID);
        SetPVarString(cheaterID, "cheater_report_reason", text);

        SetPVarInt(cheaterID, "cheater_score", GetPVarInt(cheaterID, "cheater_score") + 1);
    }

    SetPVarInt(reportID, "cheats_report_delay", 240);
    return true;
}

onPlayerReportSent(reportID, reportPlayer = -1, type, time, delay, autoassign = 0)
{
    if(!IsPlayerConnected(reportID))
        return true;

    if(!autoassign)
    {
        Iter_Add(sendReport, reportID);

        SetPVarInt(reportID, "report_type", type);
        SetPVarInt(reportID, "report_time", time);

        if(type == REPORT_TYPE_DM && reportPlayer)
        {
            SetPVarInt(reportID, "report_player", reportPlayer),
            SetPVarInt(reportPlayer, "reported_for", type), SetPVarInt(reportPlayer, "reported_by", reportID);
        }
    }

    if(delay)
        SetPVarInt(reportID, "report_delay", delay + gettime());
    
    return true;
}

updateCheaterVariables(cheaterID)
{
    if(!Iter_Contains(Cheaters, cheaterID))
        return true;

    DeletePVar(cheaterID, "cheater_report_time");
    DeletePVar(cheaterID, "reported_by_player");
   
	DeletePVar(cheaterID, "cheater_report_reason");
    DeletePVar(cheaterID, "cheater_score");

    Iter_Remove(Cheaters, cheaterID);
    return true;
}

updateReportVariables(reportID, delayend = 0)
{
    if(Iter_Contains(sendReport, reportID))
    {
        new reportPlayer = GetPVarInt(reportID, "report_player");
        if(reportPlayer)
        {
            DeletePVar(reportPlayer, "reported_for");
            DeletePVar(reportPlayer, "reported_by");
            DeletePVar(reportID, "report_player");
        }

        DeletePVar(reportID, "report_time");
        DeletePVar(reportID, "report_type");
        DeletePVar(reportID, "report_reason");

        if(!GetPVarInt(reportID, "o_report_type"))
            Iter_Remove(sendReport, reportID);
    }
    if(delayend)
		DeletePVar(reportID, "report_delay");

    return true;
}
YCMD:nmute(playerid, params[], help) {
    if(playerInfo[playerid][pHelper] < 1)
        return sendError(playerid,"Nu ai gradul administrativ necesar pentru a folosi aceasta comanda.");

	if(GetPVarInt(playerid, "delay_actions") > gettime()) 
		return va_SendClientMessage(playerid, -1, "Asteapta %d secunde pentru a folosi din nou aceasta comanda.", GetPVarInt(playerid, "delay_actions") - gettime());

    new returnPlayer, minutes, returnReason2[64];

    if(sscanf(params, "uds[64]", returnPlayer, minutes, returnReason2))
        return sendSyntaxMessage(playerid, "/nmute <playerid/name> <minutes> <reason>");

    if(!playerStaticVar[returnPlayer][playerLogged])
        return sendError(playerid,"Acel player nu este logat.");

    if(strlen(returnReason2) > 64)
        return sendError(playerid,"Motiv-ul trebuie sa aiba maxim 64 caractere.");

    if(minutes > 60 || minutes < 1)
        return sendError(playerid,"Invalid Minutes! (1 - 60)");

    playerInfo[returnPlayer][pQuestionMute] = gettime() + minutes*60;
    SetPVarInt(playerid, "delay_actions", gettime() + 20);

    mysql_format(SQL, returnString, sizeof returnString, "UPDATE `users` SET `QuestionMute` = %d WHERE `ID` = %d", (playerInfo[returnPlayer][pQuestionMute] - gettime()), playerInfo[returnPlayer][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");

    return sendSplittedMessageToAll(COLOR_LIGHTRED, COLOR_LIGHTRED, "AdmBot: %s has been muted on [/n] for %d minutes by %s, reason: %s", GetName(returnPlayer), minutes, GetName(playerid), returnReason2);
}
YCMD:rmute(playerid, params[], help) {
    check_admin

	if(GetPVarInt(playerid, "delay_actions") > gettime()) 
		return va_SendClientMessage(playerid, -1, "Asteapta %d secunde pentru a folosi din nou aceasta comanda.", GetPVarInt(playerid, "delay_actions") - gettime());


    new returnPlayer, minutes, reason[64]; if(sscanf(params, "uds[64]", returnPlayer, minutes, reason))
        return sendSyntaxMessage(playerid, "/rmute <playerid/name> <minutes> <reason>");

    if(playerInfo[returnPlayer][pAdmin] || playerInfo[returnPlayer][pHelper])
        return sendError(playerid,"Acel player face parte din staff.");

    if(!playerStaticVar[returnPlayer][playerLogged])
        return sendError(playerid,"Acel player nu este logat.");

    if(strlen(reason) > 32 || strlen(reason) < 5)
        return sendError(playerid,"Motiv-ul tau trebuie sa contina minim 5 caractere si maxim 32.");

    if(minutes > 60 || minutes < 1)
        return sendError(playerid,"Invalid Minutes! (1 - 60)");

    playerInfo[returnPlayer][pReportMute] = gettime() + minutes*60;
    SetPVarInt(playerid, "delay_actions", gettime() + 20);

    mysql_format(SQL, returnString, sizeof returnString, "UPDATE `users` SET `ReportMute` = %d WHERE `ID` = %d", playerInfo[returnPlayer][pReportMute] - gettime(), playerInfo[returnPlayer][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");

    return sendSplittedMessageToAll(COLOR_LIGHTRED, COLOR_LIGHTRED, "AdmBot: %s has been muted on [/report] by admin %s for %d minutes. Reason: %s", GetName(returnPlayer), GetName(playerid), minutes, reason);
}
YCMD:report(playerid, params[], help)
{
	if(playerInfo[playerid][pAdmin] > 1) return sendError(playerid,"You are an admin, you can't do that.");

    if(GetPVarInt(playerid, "report_delay"))
        return va_SendClientMessage(playerid, COLOR_WHITE, "Nu poti trimite un report timp de %d secunde.", GetPVarInt(playerid, "report_delay") - gettime());

    if(playerInfo[playerid][pReportMute] > gettime())
        return sendError(playerid,"Poti folosi aceasta comanda peste %d secunde deoarece recent ai primit mute pe /report.", playerInfo[playerid][pReportMute] - gettime());

    if(playerInfo[playerid][pAdmin] && playerInfo[playerid][pHelper]) 
        return SendClientMessage(playerid, COLOR_WHITE, "You can't send an report because you are an admin/helper.");

    Dialog_Show(playerid, DIALOG_REPORT, DIALOG_STYLE_LIST, "Report", "Sunt blocat/Cad prin mapa\nVreau sa fac o afacere si am nevoie de ajutor\nContul meu este spart sau blocat\nRaporteaza DM\nRaporteaza un cheater\nIntrebari despre donatii", "Ok", "Inchide");
    return true; 
}

Dialog:DIALOG_REPORT(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    switch(listitem)
    {
        case 0:
        {
            SendClientMessage(playerid, COLOR_WHITE, "Reportul tau a fost trimis administratiei cu succes!");
            foreach(new i : Spectators)
            {
                if(Spectate[i] == playerid)
                {
                    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_REPORT, "%s [%d] is {FFC000}stuck - {FFFFFF}auto assigned to %s (%d)", GetName(playerid), playerid, GetName(i), i);
                    onPlayerReportSent(playerid, -1, REPORT_TYPE_STUCK, gettime(), 300, 1);
                    return true;
                }
            }

            sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_REPORT, "%s [%d] is {FFC000}stuck", GetName(playerid), playerid);
            onPlayerReportSent(playerid, -1, REPORT_TYPE_STUCK, gettime(), 300);
        }
        case 1: sendSplitMessage(playerid, COLOR_WHITE, "Optiune dezactivata. Pentru efectuarea negoturilor pe server este implementat un sistem de 'TRADE'. Pentru a face un negot cu un jucator tasteaza comanda /trade [nume / id].");
        case 2: Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Cont blocat", "Cont blocat\nDaca ai contul blocat automat trebuie sa il deblochezi de pe adresa de email.\nDaca ai contul blocat de catre un admin, deschide un tichet pe panel."SERVER_NAME"\nDe acolo vei putea deschide un ticket pentru a discuta cu adminii despre ce a cauzat blocarea contului tau.\nAdminii NU iti vor debloca contul din joc. Nu are rost sa intrebi pe /report cum poti sa iti deblochezi contul. Nu vei primi raspuns.\nCont spart\nDaca ai contul spart, tot ce poti face e sa deschizi un ticket si sa astepti un raspuns. De obicei se primesc raspunsuri in mai putin de 24 ore.\nDaca ti-a fost spart contul e DOAR vina ta. Nu sunt adminii vinovati si nimeni nu are vreo obligatie de a te ajuta.\nTotusi, incercam sa ajutam playerii ce pot fi ajutati. Deci, deschide un ticket si asteapta un raspuns.\nPentru a deschide un ticket, intra pe panel.convicted.ro > Ticket > Deschide ticket nou.\nNu da /report pentru a zice adminilor sa raspunda mai repede la tickete. Vei primi suspend pe /report si atat.", "Inchide", "");
        case 3: Dialog_Show(playerid, DIALOG_REPORT_TYPE_DM, DIALOG_STYLE_INPUT, "Raportare DM", "Introduceti ID-ul/numele playerului care v-a atacat.", "Introdu", "Inapoi");
        case 4: Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Raportare Cheater", "Daca cunosti numele jucatorului ce a folosit coduri tasteaza comanda /cheats [id] [cod]", "Ok", "");
        case 5: Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Donatii/Plati", "Pentru a dona si a primi premium points pe acest server, intrati pe panelul nostru.", "Inchide", "");
    }

    return true;
}

Dialog:DIALOG_REPORT_TYPE_DM(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	return va_Command_ReProcess(playerid, "dm %s", false, inputtext);
}

YCMD:reports(playerid, params[], help)
{
    check_admin

    SendClientMessage(playerid, COLOR_WHITE, "______REPORTS______");

    if(!Iter_Count(sendReport))
    	SendClientMessage(playerid, COLOR_WHITE, "No reports available.");

    foreach(new i : sendReport)
    {
        if(GetPVarInt(i, "report_type") == REPORT_TYPE_DM)
        {
            new reportedByPlayer = GetPVarInt(i, "report_player");
            va_SendClientMessage(playerid, 0x2f4fefFF, "%s(%d) - level %d - dm - %02d minutes ago", GetName(reportedByPlayer), reportedByPlayer, playerInfo[reportedByPlayer][pLevel], (gettime() - GetPVarInt(i, "report_time")) / 60);
        }
        if(GetPVarInt(i, "o_report_type") == REPORT_TYPE_OTHER)
        {
            new reportString[144];
            GetPVarString(i, "o_report_reason", reportString, sizeof reportString);

            va_SendClientMessage(playerid, 0xd35845ff, "%s(%d) - level %d - %s - %02d minutes ago", GetName(i), i, playerInfo[i][pLevel], reportString, (gettime() - GetPVarInt(i, "o_report_time")) / 60);
        }
        if(GetPVarInt(i, "report_type") == REPORT_TYPE_STUCK)
            va_SendClientMessage(playerid, COLOR_GREY, "%s is stuck. [%d]", GetName(i), i);
    }
    return true;
}

YCMD:cheaters(playerid, params[], help)
{
    check_admin

    SendClientMessage(playerid, COLOR_WHITE, "______CHEATERS______");
    foreach(new i : Cheaters)
    {
        GetPVarString(i, "cheater_report_reason", returnString, sizeof returnString);
        va_SendClientMessage(playerid, COLOR_WHITE, "%s(%d) - %s - level %d, %d playing hours - score %d - %02d minutes ago", GetName(i), i, returnString, playerInfo[i][pLevel], floatround(playerInfo[i][pHours], floatround_round), GetPVarInt(i, "cheater_score"), (gettime() - GetPVarInt(i, "cheater_report_time")) / 60);
    }

    if(Iter_Count(Cheaters))
        va_SendClientMessage(playerid, COLOR_WHITE, "Total cheaters: %d", Iter_Count(Cheaters));
    
    else
        SendClientMessage(playerid, COLOR_WHITE, "No cheaters available.");
    return true;
}

YCMD:dm(playerid, params[], help)
{   
    new id;
    if(sscanf(params, "u", id)) 
        return sendSyntaxMessage(playerid, "/dm [name/playerid]");

    if(id == playerid)
        return SendClientMessage(playerid, COLOR_WHITE, "Nu poti folosi aceasta comanda asupra ta.");

    if(!IsPlayerConnected(id))
        return SendClientMessage(playerid, COLOR_GREY, "Player not connected.");

    if(GetPVarInt(playerid, "report_delay"))
        return va_SendClientMessage(playerid, COLOR_WHITE, "Nu poti trimite un report timp de %d secunde.", GetPVarInt(playerid, "report_delay") - gettime());

    SendClientMessage(playerid, COLOR_WHITE, "Reportul tau a fost trimis cu succes!");
    
    foreach(new i : Spectators)
    {
        if(Spectate[i] == id)
        {
            sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_REPORT, "%s (%d, level %d) - {FFC000}deathmatch - {FFFFFF}by %s - auto assigned to %s (%d)", GetName(id), id, playerInfo[id][pLevel], GetName(playerid), GetName(i), i);
            onPlayerReportSent(playerid, id, REPORT_TYPE_DM, gettime(), 120, 1);
            return true;
        }
    }
    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_REPORT, "%s (%d, level %d) - {FFC000}deathmatch - {FFFFFF}by %s", GetName(id), id, playerInfo[id][pLevel], GetName(playerid));
    onPlayerReportSent(playerid, id, REPORT_TYPE_DM, gettime(), 120);
    return true;
}

YCMD:ajail(playerid, params[], help) {
    if(playerInfo[playerid][pAdmin] < 1)
        return sendError(playerid, "Nu ai gradul administrativ necesar pentru a folosi aceasta comanda.");

    new returnPlayer, minutes, reason[64];
    if(sscanf(params, "uiS[64]", returnPlayer, minutes, reason)) 
        return sendSyntaxMessage(playerid, "/ajail <playerid/name> <minutes> <reason>");

    new jailTime = minutes * 60;  // Convert minutes to seconds

    put_player_in_jail(returnPlayer, jailTime, JAIL_TYPE_ADMIN);
    playerInfo[returnPlayer][pJailType] = JAIL_TYPE_ADMIN;
    va_SendClientMessageToAll(COLOR_LIGHTRED, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s has been jailed for %d minutes by %s, reason: %s.", GetName(returnPlayer), minutes, GetName(playerid), reason);
    va_SendClientMessage(playerid, COLOR_YELLOW, "* You jailed player %s for %d minutes, reason: %s.", GetName(returnPlayer), minutes, reason);
    va_SendClientMessage(returnPlayer, COLOR_YELLOW, "* %s jailed you for %d minutes, reason: %s.", GetName(playerid), minutes, reason);

    return true;
}
YCMD:cheats(playerid, params[], help)
{
	if(playerInfo[playerid][pAdmin] > 1) return sendError(playerid,"You are an admin, you can't do that.");

    new cheaterID, reason[144];
    	
    if(sscanf(params, "us[144]", cheaterID, reason))
        return sendSyntaxMessage(playerid, "/cheats <id cheater> <reason>");

    if(!IsPlayerConnected(cheaterID) || cheaterID == playerid)
        return SendClientMessage(playerid, COLOR_WHITE, "Invalid player name/player id.");

    SendClientMessage(playerid, COLOR_GREY, "Report submitted.");

    foreach(new i : Spectators)
    {
        if(Spectate[i] == cheaterID)
        {
            sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_REPORT, "%s (%d) - cheats - by %s (%d), reason: %s - auto assigned to %s (%d)", GetName(cheaterID), cheaterID, GetName(playerid), playerid, reason, GetName(i), i);
            return onCheaterReported(playerid, cheaterID, reason, gettime(), 1);
        }
    }

    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_REPORT, "%s (%d) - cheats - by %s (%d), reason: %s", GetName(cheaterID), cheaterID, GetName(playerid), playerid, reason);
    onCheaterReported(playerid, cheaterID, reason, gettime(), 0);
    return true;
}

YCMD:cr(playerid, params[], help)
{
    check_admin

    new id, closeReason[144];
    if(sscanf(params, "uS(in curs de rezolvare)[144]", id, closeReason))
        return sendSyntaxMessage(playerid, "/cr [name/playerid] [text(optional)]");

    if(!IsPlayerConnected(id))
        return SendClientMessage(playerid, COLOR_WHITE, "Player not connected.");

    if(GetPVarInt(id, "o_report_type") != REPORT_TYPE_OTHER) 
        return true;

    new reportString[144];
    GetPVarString(id, "o_report_reason", reportString, sizeof reportString);

    sendStaffMessage(STAFF_TYPE_ADMIN, 0xa39fffFF, "%s closed report from %s [%d, level %d]: %s", GetName(playerid), GetName(id), id, playerInfo[id][pLevel], reportString);
    
    if(strcmp(closeReason, "in curs de rezolvare", true))
        sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "%s -> %s: %s", GetName(playerid), GetName(id), closeReason);

    va_SendClientMessage(id, 0xd35845FF, "Reportul tau a fost inchis de catre administratorul %s, motiv %s.", GetName(playerid), closeReason);
	playerInfo[playerid][pPlayersHelped] ++;
	va_mysql_tquery("UPDATE `users` SET `PlayersHelped` = %d WHERE `ID` = %d", playerInfo[playerid][pPlayersHelped], playerInfo[playerid][pSQLID]);
    DeletePVar(id, "o_report_reason");
    DeletePVar(id, "o_report_type");

    if(!GetPVarInt(playerid, "report_type"))
        Iter_Remove(sendReport, id);

    return true;
}

YCMD:dr(playerid, params[], help)
{
    check_admin

    new id;
    if(sscanf(params, "u", id))
        return sendSyntaxMessage(playerid, "/dr [name/playerid]");

    if(!IsPlayerConnected(id))
        return SendClientMessage(playerid, COLOR_GREY, "Player not connected.");

    if(GetPVarInt(id, "o_report_type") != REPORT_TYPE_OTHER) 
        return true;

    new reportString[144];
    GetPVarString(id, "o_report_reason", reportString, sizeof reportString);

    sendStaffMessage(STAFF_TYPE_ADMIN, 0xa39fffFF, "%s deleted report from %s [%d, level %d]: %s", GetName(playerid), GetName(id), id, playerInfo[id][pLevel], reportString);

    DeletePVar(id, "o_report_reason");
    DeletePVar(id, "o_report_type");

    if(!GetPVarInt(playerid, "report_type"))
        Iter_Remove(sendReport, id);

    return true;
}

timer update_hquestions[1000](playerid)
{
    if(!Iter_Contains(helpersWithQuestion, playerid))
        stop helper_check[playerid];

    helper_check[playerid] = defer update_hquestions(playerid);
    if(Iter_Contains(helpersOnDuty, playerid) && playerStaticVar[playerid][afkTime] >= 120)
    {
        SendClientMessage(playerid, COLOR_LIGHTRED, "Ai fost trecut ca si helper OFF DUTY deoarece ai fost AFK peste 120 secunde.");
        Iter_Remove(helpersOnDuty, playerid);

        new questionId = helperAssignedQuestion[playerid];
        if(questionId != -1)
        {
            Iter_Remove(helperAQuestions, questionId);
            Iter_Remove(helpersWithQuestion, playerid);

            newbieQuestions[questionId][questionHelper] = -1;
        }   
    }
}

AssignHelperQuestion(helperid, questionId)
{
    Iter_Add(helpersWithQuestion, helperid);
    Iter_Add(helperAQuestions, questionId);

    helperAssignedQuestion[helperid] = questionId;
    newbieQuestions[questionId][questionHelper] = helperid;

    va_SendClientMessage(helperid, COLOR_YELLOW, "** Question from %s (%d, level %d) **", GetName(newbieQuestions[questionId][questionSender]), newbieQuestions[questionId][questionSender], playerInfo[newbieQuestions[questionId][questionSender]][pLevel]);
    va_SendClientMessage(helperid, COLOR_YELLOW, newbieQuestions[questionId][questionMessage]);

    helper_check[helperid] = defer update_hquestions(helperid);
}

task SendNewbieQuestionToHelper[2000]()
{
	if(Iter_Count(helperQuestions) != 0 && Iter_Count(helpersOnDuty))
    {
        new questionId = Iter_Random(helperQuestions);
        if(newbieQuestions[questionId][questionHelper] == -1)
        {
            new helperid = Iter_Random(helpersOnDuty);
            AssignHelperQuestion(helperid, questionId);
        }
    }
	return true;
}

YCMD:nreport(playerid, params[], help)
{
    if(playerInfo[playerid][pHelper] < 1 && !playerInfo[playerid][pAdmin] || helperAssignedQuestion[playerid] == -1 && !playerInfo[playerid][pAdmin])
        return SendClientMessage(playerid, COLOR_LIGHTRED, "Nu esti helper/admin sau nu ai o intrebare activa!");

    new questionId = helperAssignedQuestion[playerid];


    if(questionId == -1)
        return sendError(playerid,"Nu ai o intrebare activa!");
    
    if(playerInfo[playerid][pAdmin])
    {
        new id;
        if(sscanf(params, "u", id))
            return sendSyntaxMessage(playerid, "/nreport [playername/playerid]");
    
        if(!IsPlayerConnected(id))
            return SendClientMessage(playerid, COLOR_GREY, "Player not connected.");

        questionId = playerInfo[playerid][pHelper] ? helperAssignedQuestion[playerid] : playerNewbieQuestion[id];

        if(playerNewbieQuestion[id] == -1)
            return va_SendClientMessage(playerid, COLOR_GREY, "Jucatorul %s nu are nicio intrebare activa.", GetName(id));

        if(newbieQuestions[questionId][questionSender] == id && newbieQuestions[questionId][questionHelper] != playerid)
            return va_SendClientMessage(playerid, COLOR_GREY, "Jucatorul %s are o intrebare care nu iti este alocata.", GetName(id));

    }
    sendStaffMessage(STAFF_TYPE_ALL, COLOR_ORANGE, "%s marked %s (%d) question as report: %s", GetName(playerid), GetName(newbieQuestions[questionId][questionSender]), newbieQuestions[questionId][questionSender], newbieQuestions[questionId][questionMessage]);
    va_SendClientMessage(newbieQuestions[questionId][questionSender], COLOR_LIGHTRED, "(!) Intrebarea ta a fost marcata drept report de catre helperul/administratorul %s.", GetName(playerid));

	playerInfo[playerid][pPlayersHelped] ++;
    SetPVarString(newbieQuestions[questionId][questionSender], "o_report_reason", newbieQuestions[questionId][questionMessage]);
    SetPVarInt(newbieQuestions[questionId][questionSender], "o_report_time", gettime());
    SetPVarInt(newbieQuestions[questionId][questionSender], "o_report_type", REPORT_TYPE_OTHER);

    if(!Iter_Contains(sendReport, newbieQuestions[questionId][questionSender]))
        Iter_Add(sendReport, newbieQuestions[questionId][questionSender]);

    Iter_Remove(helperQuestions, questionId);
    Iter_Remove(helperAQuestions, questionId);
    
    if(Iter_Contains(helpersWithQuestion, playerid))
        Iter_Remove(helpersWithQuestion, playerid);

    playerNewbieQuestion[newbieQuestions[questionId][questionSender]] = -1;
    format(newbieQuestions[questionId][questionMessage], 144, "");

    DeletePVar(playerid, "newbieQuestionAnswer");

    newbieQuestions[questionId][questionSender] = -1;
    newbieQuestions[questionId][questionHelper] = -1;
    helperAssignedQuestion[playerid] = -1;
    return true;
}

YCMD:nqueue(playerid, params[], help)
{
    if(playerInfo[playerid][pHelper] < 1 && playerInfo[playerid][pAdmin] < 1)
        return returnNoAcces(playerid);

    return va_SendClientMessage(playerid, COLOR_YELLOW, "There are %d pending questions and %d on-duty helpers.", Iter_Count(helperQuestions) - Iter_Count(helperAQuestions), Iter_Count(helpersOnDuty));
}

YCMD:myquestion(playerid, params[], help)
{
    if(playerNewbieQuestion[playerid] == -1)
        return SendClientMessage(playerid, COLOR_NEWBIE, "Nu ai nicio intrebare pusa pe chatul incepatorilor!");

    return va_SendClientMessage(playerid, COLOR_NEWBIE, "[N] Intrebarea ta: %s", newbieQuestions[playerNewbieQuestion[playerid]][questionMessage]);
}

YCMD:ncurrent(playerid, params[], help)
{
    if(playerInfo[playerid][pHelper] < 1 && playerInfo[playerid][pAdmin] < 1 || helperAssignedQuestion[playerid] == -1)
        return SendClientMessage(playerid, COLOR_LIGHTRED, "Nu esti helper/admin sau nu ai o intrebare activa!");

    new
        questionId = helperAssignedQuestion[playerid];

    va_SendClientMessage(playerid, COLOR_YELLOW, "** Question from %s (%d, level %d) **", GetName(newbieQuestions[questionId][questionSender]), newbieQuestions[questionId][questionSender], playerInfo[newbieQuestions[questionId][questionSender]][pLevel]);
    va_SendClientMessage(playerid, COLOR_YELLOW, newbieQuestions[questionId][questionMessage]);
    return true;
}

YCMD:newbie(playerid, params[], help)
{
    if(!playerInfo[playerid][pHelper] && !playerInfo[playerid][pAdmin])
    {
        new
            questionText[144];

	    if(playerInfo[playerid][pQuestionMute] > gettime())
	        return sendError(playerid,"Poti folosi aceasta comanda peste %d secunde deoarece recent ai primit mute pe /n.", playerInfo[playerid][pQuestionMute] - gettime());

        if(sscanf(params, "s[144]", questionText))
            return SendClientMessage(playerid, COLOR_NEWBIE, "Syntax: /n(ewbiechat) <text>");

        if(FindIP(questionText)) /// Anti-Reclama
			sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s(%d) typed %s in /n.", GetName(playerid), playerid, questionText);

		if(playerInfo[playerid][pNMuted] > 0)
			return va_SendClientMessage(playerid, -1, "Nu poti pune o intrebare deoarece ai mute timp de %s (%d secunde).", calculateTime(playerInfo[playerid][pNMuted]), playerInfo[playerid][pNMuted]);


        if(playerNewbieQuestion[playerid] != -1)
            return va_SendClientMessage(playerid, COLOR_NEWBIE, "Deja ai o intrebare pusa! Foloseste /myquestion pentru a-ti vedea intrebarea. Aceasta va expira in %d secunde.", GetPVarInt(playerid, "questionDelay") - gettime());
    
        if(strlen(questionText) < 10)
            return SendClientMessage(playerid, COLOR_NEWBIE, "[N] Intrebarea ta trebuie sa contina macar 10 caractere!");

        if(!Iter_Count(helpersOnDuty))
            return SendClientMessage(playerid, COLOR_NEWBIE, "[N] Ne pare rau dar nu sunt helperi/administratori on-duty pentru a iti prelua intrebarea!");

        SendClientMessage(playerid, COLOR_NEWBIE, "Intrebarea ta a fost trimisa cu succes!");

        playerNewbieQuestion[playerid] = Iter_Count(helperQuestions) + 1;
    
        new
            questionId = playerNewbieQuestion[playerid];
        
        Iter_Add(helperQuestions, questionId);

        newbieQuestions[questionId][questionSender] = playerid;
        newbieQuestions[questionId][questionHelper] = -1;

        format(newbieQuestions[questionId][questionMessage], 144, questionText);
    }   
    else
    {
        if(helperAssignedQuestion[playerid] == -1)
            return sendError(playerid,"Nu ai o intrebare activa!");
        
        new
            questionAddQuest[144], questionAnswer[512];
        
        if(sscanf(params, "s[144]", questionAddQuest))
            return sendSyntaxMessage(playerid, "/n [raspuns]");

        GetPVarString(playerid, "newbieQuestionAnswer", questionAnswer, sizeof questionAnswer);

        if(strlen(questionAnswer) + strlen(questionAddQuest) >= 510)
        {
            SendClientMessage(playerid, -1, "Nu pot adauga input-ul curent raspunului tau deoarece acesta depaseste limita maxima admisa (512). Incearca sa scrii mai putin.");
            return va_SendClientMessage(playerid, COLOR_GREY, "Raspunsul tau pana acum:%s", questionAnswer);
        }

        format(questionAnswer, 512, "%s %s", questionAnswer, questionAddQuest);

        SetPVarString(playerid, "newbieQuestionAnswer", questionAnswer);
        return va_SendClientMessage(playerid, COLOR_GREY, "Raspunsul tau pana acum:%s", questionAnswer);
    }
    return true;
}

YCMD:nsend(playerid, params[], help)
{
    if(playerInfo[playerid][pHelper] < 1 && playerInfo[playerid][pAdmin] < 1 || helperAssignedQuestion[playerid] == -1)
        return SendClientMessage(playerid, COLOR_LIGHTRED, "Nu esti helper/admin sau nu ai o intrebare activa!");

    new
        questionAnswer[512];

    GetPVarString(playerid, "newbieQuestionAnswer", questionAnswer, sizeof questionAnswer);

    if(!strlen(questionAnswer))
        return sendError(playerid,"Nu ai raspuns intrebarii.");

    new
        questionId = helperAssignedQuestion[playerid];

    foreach(new i : Player)
    {
        sendSplitMessage(i, COLOR_NEWBIE, "* Newbie %s: %s", GetName(newbieQuestions[questionId][questionSender]), newbieQuestions[questionId][questionMessage]);

        if(i != newbieQuestions[questionId][questionSender])
            sendSplitMessage(i, COLOR_NEWBIE, "* Helper %s: %s %s", GetName(playerid), GetName(newbieQuestions[questionId][questionSender]), questionAnswer);
    }
    sendSplitMessage(newbieQuestions[questionId][questionSender], COLOR_YELLOW, "* Helper %s: %s %s", GetName(playerid), GetName(newbieQuestions[questionId][questionSender]), questionAnswer);

    Iter_Remove(helperQuestions, questionId);
    Iter_Remove(helperAQuestions, questionId);
    Iter_Remove(helpersWithQuestion, playerid);

    playerNewbieQuestion[newbieQuestions[questionId][questionSender]] = -1;
    format(newbieQuestions[questionId][questionMessage], 144, "");

    DeletePVar(playerid, "newbieQuestionAnswer");

    newbieQuestions[questionId][questionSender] = -1;
    newbieQuestions[questionId][questionHelper] = -1;
    helperAssignedQuestion[playerid] = -1;
    return true;
}

YCMD:ndelete(playerid, params[], help)
{
    if(playerInfo[playerid][pHelper] < 1 && playerInfo[playerid][pAdmin] < 1 || helperAssignedQuestion[playerid] == -1)
        return SendClientMessage(playerid, COLOR_LIGHTRED, "Nu esti helper/admin sau nu ai o intrebare activa!");

    new
        questionAnswer[144];

    if(sscanf(params, "s[144]", questionAnswer))
        return sendSyntaxMessage(playerid, "/ndelete [reason]");

    new questionId = helperAssignedQuestion[playerid];

    sendSplitMessage(newbieQuestions[questionId][questionSender], COLOR_NEWBIE, "* Newbie %s: %s", GetName(newbieQuestions[questionId][questionSender]), newbieQuestions[questionId][questionMessage]);

    foreach(new i : serverAdmins)
        sendSplitMessage(i, COLOR_NEWBIE, "(/nd) * Helper %s: %s %s", GetName(playerid), GetName(newbieQuestions[questionId][questionSender]), questionAnswer);

    foreach(new i: serverHelpers)
        sendSplitMessage(i, COLOR_NEWBIE, "(/nd) * Helper %s: %s %s", GetName(playerid), GetName(newbieQuestions[questionId][questionSender]), questionAnswer);

    sendSplitMessage(newbieQuestions[questionId][questionSender], COLOR_YELLOW, "(/nd) * Helper %s: %s %s", GetName(playerid), GetName(newbieQuestions[questionId][questionSender]), questionAnswer);
	playerInfo[playerid][pPlayersHelped] ++;
    Iter_Remove(helperQuestions, questionId);
    Iter_Remove(helperAQuestions, questionId);
    Iter_Remove(helpersWithQuestion, playerid);

    playerNewbieQuestion[newbieQuestions[questionId][questionSender]] = -1;
    format(newbieQuestions[questionId][questionMessage], 144, "");

    DeletePVar(playerid, "newbieQuestionAnswer");

    newbieQuestions[questionId][questionSender] = -1;
    newbieQuestions[questionId][questionHelper] = -1;
    helperAssignedQuestion[playerid] = -1;
    return true;
}

YCMD:nskip(playerid, params[], help)
{
    if(playerInfo[playerid][pHelper] < 1 && playerInfo[playerid][pAdmin] < 1 || helperAssignedQuestion[playerid] == -1)
        return SendClientMessage(playerid, COLOR_LIGHTRED, "Nu esti helper/admin sau nu ai o intrebare activa!");

    SendClientMessage(playerid, COLOR_YELLOW, "Ai sarit peste aceasta intrebare!");
    SendClientMessage(playerid, -1, "Daca nu sunt alte intrebari este posibil sa primesti tot aceasta intrebare.");
    SendClientMessage(playerid, COLOR_NEWBIE, "Getting a new question...");

    new
        questionId = helperAssignedQuestion[playerid];

    DeletePVar(playerid, "newbieQuestionAnswer");

    Iter_Remove(helperAQuestions, questionId);
    Iter_Remove(helpersWithQuestion, playerid);
    newbieQuestions[questionId][questionHelper] = -1;

    helperAssignedQuestion[playerid] = -1;

    return true;
}

YCMD:nnext(playerid, params[], help)
{
    if(playerInfo[playerid][pHelper] < 1 && playerInfo[playerid][pAdmin] < 1 || helperAssignedQuestion[playerid] != -1)
        return SendClientMessage(playerid, COLOR_LIGHTRED, "Nu esti helper/admin sau ai o intrebare activa!");

    if(!Iter_Contains(helpersOnDuty, playerid))
        return SendClientMessage(playerid, COLOR_GREY, "Pentru a folosi aceasta comanda trebuie sa fii on duty ca si helper.");

    if(!Iter_Count(helperQuestions))
        return sendError(playerid,"Nu exista intrebari care sa poata fi alocate.");

    if(Iter_Count(helperQuestions) >= Iter_Count(helpersOnDuty))
    {
        new questionId = Iter_Random(helperQuestions);

        if(newbieQuestions[questionId][questionHelper] == -1)
            AssignHelperQuestion(playerid, questionId);
    }
    else
        SendClientMessage(playerid, COLOR_GREY, "Nu poti folosi aceasta comanda in acest moment deoarece sunt mai putine intrebari in asteptare decat helperi on-duty!");
    
    return true;
}

YCMD:hquestions(playerid, params[], help)
{
    if(playerInfo[playerid][pHelper] < 0 && playerInfo[playerid][pAdmin] < 0)
        return returnNoAcces(playerid);

    foreach(new questionId : helperQuestions)
    {
        if(newbieQuestions[questionId][questionHelper] == -1)
            va_SendClientMessage(playerid, COLOR_NEWBIE, "Question: '%s' by %s (%d).", newbieQuestions[questionId][questionMessage], GetName(newbieQuestions[questionId][questionSender]), newbieQuestions[questionId][questionSender]);
        else
            va_SendClientMessage(playerid, COLOR_NEWBIE, "Question: '%s' by %s (%d) - assigned to %s (%d).", newbieQuestions[questionId][questionMessage], GetName(newbieQuestions[questionId][questionSender]), newbieQuestions[questionId][questionSender], GetName(newbieQuestions[questionId][questionHelper]), newbieQuestions[questionId][questionHelper]);
    }
    return va_SendClientMessage(playerid, COLOR_NEWBIE, "Total newbie questions: %d (%d assigned to helpers). Total helpers on-duty: %d", Iter_Count(helperQuestions), Iter_Count(helperAQuestions), Iter_Count(helpersOnDuty));
}

YCMD:hduty(playerid, params[], help)
{
	if(!Iter_Contains(serverHelpers, playerid) && !Iter_Contains(serverAdmins, playerid))
		return returnNoAcces(playerid);

    if(!Iter_Contains(helpersOnDuty, playerid))
    {
        SendClientMessage(playerid, COLOR_YELLOW, "Acum esti on-duty ca helper.");
        Iter_Add(helpersOnDuty, playerid);
    }
    else
    {
        SendClientMessage(playerid, COLOR_YELLOW, "Nu mai esti duty on ca helper.");
        Iter_Remove(helpersOnDuty, playerid);

        new
            questionId = helperAssignedQuestion[playerid];

        if(questionId != -1)
        {
            DeletePVar(playerid, "newbieQuestionAnswer");

            Iter_Remove(helperAQuestions, questionId);
            Iter_Remove(helpersWithQuestion, playerid);
            newbieQuestions[questionId][questionHelper] = -1;
        }
    }
    return true;
}

YCMD:ah(playerid, params[], help)
{//9460df
	check_admin
	SendClientMessage(playerid, COLOR_SERVER, "-------------------------------------------------------------------");
	if(playerInfo[playerid][pAdmin] >= 1) 
	{
		SendClientMessage(playerid, COLOR_WHITE, ""COLOR_SV_HEX"ADMIN 1: {FFFFFF}/acceptname, /cancelname, /gotoatm, /vehname, /check, /spawncar, /vre, /va, /kick, /ban, /banip");
		SendClientMessage(playerid, COLOR_WHITE, ""COLOR_SV_HEX"ADMIN 1: {FFFFFF}/closestcar, /gotoa, /aaa2, /aaa3, /gotols, /gotolv, /gotosf, /flymode, /fly, /goto, /aduty, /setvw, ");
		SendClientMessage(playerid, COLOR_WHITE, ""COLOR_SV_HEX"ADMIN 1: {FFFFFF}/sethp, /givegun, /healme, /spec, /reports, /cheaters, /cr, /dr, /checkhp, /gotocar");
		SendClientMessage(playerid, COLOR_WHITE, ""COLOR_SV_HEX"ADMIN 1: {FFFFFF}/getcar, /fixveh, /flip, /checklicences, /wantedplayers, /aclear, /gotobiz, /fspec, /fspecid");
		SendClientMessage(playerid, COLOR_WHITE, ""COLOR_SV_HEX"ADMIN 1: {FFFFFF}/fchat, /gotohq, /gotohouse, /gotorace, /gotojob, /jailedplayers, /gotopaint, /checkv, /gotods, /unmute");
		SendClientMessage(playerid, COLOR_WHITE, ""COLOR_SV_HEX"ADMIN 1: {FFFFFF}/muteo, /slap, /rmute, /nmute, /nqueue, /ncurrent, /nsend, /ndelete, /nskip, /nnext, /hquestions, /hduty, /hre, /warn");
		SendClientMessage(playerid, COLOR_WHITE, ""COLOR_SV_HEX"ADMIN 1: {FFFFFF}/acceptname, /cancelname, /sstats, /gotoatm, /vehname, /check, /spawncar, /vre, /va");
	}
	if(playerInfo[playerid][pAdmin] >= 2)
	{
		SendClientMessage(playerid, COLOR_WHITE, ""COLOR_SV_HEX"ADMIN 2: {FFFFFF}/entercar, /putplayerinvehicle");
	}
	if(playerInfo[playerid][pAdmin] >= 3)
	{
		SendClientMessage(playerid, COLOR_WHITE, ""COLOR_SV_HEX"ADMIN 3: {FFFFFF}/agl, /rac, /setleader");
	}

	if(playerInfo[playerid][pAdmin] >= 4)
		SendClientMessage(playerid, COLOR_WHITE, ""COLOR_SV_HEX"ADMIN 4: {FFFFFF}/set, /carcolortemp, /sethelper");

	if(playerInfo[playerid][pAdmin] >= 5)
		SendClientMessage(playerid, COLOR_WHITE, ""COLOR_SV_HEX"ADMIN 5: {FFFFFF}/unban, /unbanip, /gotoxyz, /speed, /afklist");

	if(playerInfo[playerid][pAdmin] >= 6)
	{
		SendClientMessage(playerid, COLOR_WHITE, ""COLOR_SV_HEX"ADMIN 6: {FFFFFF}/queuestats, /sstats, /setpvar, /debugauctions, /debugmusclelevel, /staff");
		SendClientMessage(playerid, COLOR_WHITE, ""COLOR_SV_HEX"ADMIN 6: {FFFFFF}/createbiz, /movebusiness, /bowner, /createhouse, /movehouse, /togkickafk");
		SendClientMessage(playerid, COLOR_WHITE, ""COLOR_SV_HEX"ADMIN 6: {FFFFFF}/howner, /amotd, /givepayday, /vmove, /savevehicle, /howner, /amotd, /setadmin, /setadminoff");
	}
	if(playerInfo[playerid][pAdmin] >= 7)
	{
		SendClientMessage(playerid, COLOR_WHITE, ""COLOR_SV_HEX"ADMIN 7: {FFFFFF}/kickall, /resetquests");
	}
	SendClientMessage(playerid, COLOR_SERVER, "-------------------------------------------------------------------");

	return 1;
}
YCMD:hre(playerid, params[], help)
{
    if(playerInfo[playerid][pAdmin] < 1 && playerInfo[playerid][pHelper] < 1)
        return sendError(playerid,"Nu aveti gradul administrativ necesar.");

    new id, sMessage[128];

    if(sscanf(params, "us[128]", id, sMessage))
        return sendSyntaxMessage(playerid, "/hre [name/playerid] [text]");

    if(!IsPlayerConnected(id))
        return SendClientMessage(playerid, COLOR_GREY, "Player not connected.");

    if(playerid == id)
        return sendError(playerid,"You can't use this command on yourself'.");

    sendStaffMessage(STAFF_TYPE_ALL, COLOR_YELLOW, "/hre: %s->%s: %s", GetName(playerid), GetName(id), sMessage);
    va_SendClientMessage(id, COLOR_YELLOW, "> PM from %s: %s", GetName(playerid), sMessage);

    PlayerPlaySound(id, 1139, 0.0, 0.0, 0.0);
    return true;
}