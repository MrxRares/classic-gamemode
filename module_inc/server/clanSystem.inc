#include <YSI_Coding\y_hooks>

#define MAX_CLANS 100	
#define MAX_CLAN_MEMBERS 100

#define check_clan if(!playerInfo[playerid][pClan]) return sendError(playerid,"Nu faci parte dintr-un clan.");

new Iterator:clanMembers<MAX_CLANS, MAX_PLAYERS>, returnServerClans;

enum ENUM_CLAN_INFORMATIONS
{
	clanID, clanName[24], clanTag[10], clanColor[23], clanMotd[128], clanSlots,
	clanRank1[32], clanRank2[32], clanRank3[32], clanRank4[32], clanRank5[32], clanRank6[32], clanRank7[32],
};
new clanInfo[MAX_CLANS][ENUM_CLAN_INFORMATIONS];

hook WhenPlayerLoggedIn(playerid)
{
	if(!cache_num_rows())
		return Y_HOOKS_CONTINUE_RETURN_1;

	cache_get_value_name_int(0, "Clan", playerInfo[playerid][pClan]);
	cache_get_value_name_int(0, "ClanRank", playerInfo[playerid][pClanRank]);

	cache_get_value_name_int(0, "ClanTagPos", playerInfo[playerid][pClanTagPos]);
	cache_get_value_name_int(0, "ClanWarns", playerInfo[playerid][pClanWarns]);
	cache_get_value_name_int(0, "ClanDays", playerInfo[playerid][pClanDays]);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

#include "../module_inc/src/clanVehicles.inc"

hook OnPlayerSpawn(playerid)
{
	if(!playerStaticVar[playerid][playerLogged])
		return Y_HOOKS_CONTINUE_RETURN_1;

	update_player_nickname(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

GetClanSafeMoney(clanId)
{
    new Cache: querySafe, clanMoney, queryString[256];

    mysql_format(SQL, queryString, sizeof queryString, "select `safeMoney` from `clans` where `id` = '%d';", clanId);
    querySafe = mysql_query(SQL, queryString);

    cache_get_value_name_int(0, "safeMoney", clanMoney);
    cache_delete(querySafe);
    return clanMoney;
}

AddClanMoney(clanId, amount)
{
	mysql_format(SQL, returnString, sizeof returnString, "update `clans` set `safeMoney` = '%d' where `ID` = '%d';", GetClanSafeMoney(clanId) + amount, clanId);
	return mysql_tquery(SQL, returnString, "", "");
}

function:loadClansFromInclude()
{
	returnServerClans = cache_num_rows();
	for(new i; i <= cache_num_rows(); i++)
	{
		cache_get_value_name_int(i-1, "ID", clanInfo[i][clanID]);

		cache_get_value_name(i - 1, "clanMOTD", clanInfo[i][clanMotd]);

		cache_get_value_name(i - 1, "clanName", clanInfo[i][clanName]);
		cache_get_value_name(i - 1, "clanTag", clanInfo[i][clanTag]);

    	cache_get_value_name(i - 1, "clanRank7", clanInfo[i][clanRank7]);
    	cache_get_value_name(i - 1, "clanRank6", clanInfo[i][clanRank6]);
    	cache_get_value_name(i - 1, "clanRank5", clanInfo[i][clanRank5]);
    	cache_get_value_name(i - 1, "clanRank4", clanInfo[i][clanRank4]);
    	cache_get_value_name(i - 1, "clanRank3", clanInfo[i][clanRank3]);
    	cache_get_value_name(i - 1, "clanRank2", clanInfo[i][clanRank2]);
    	cache_get_value_name(i - 1, "clanRank1", clanInfo[i][clanRank1]);

		cache_get_value_name(i - 1, "clanColor", clanInfo[i][clanColor]);
		cache_get_value_name_int(i-1, "clanSlots", clanInfo[i][clanSlots]);
	}
	printf("Clans: %d", cache_num_rows());
	return true;
}


hook loadServerSystems()
{
	mysql_tquery(SQL, "select * from `clans`", "loadClansFromInclude", "");
	return Y_HOOKS_CONTINUE_RETURN_1;
}
stock GetClanColor(clanId)
{
    return clanInfo[clanId][clanColor];
}
sendClanMessage(clan, color, const string[], va_args<>)
{
	new returnClanString[144];
	va_format(returnClanString, sizeof returnClanString, string, va_start<3>);

	foreach(new i : clanMembers<clan>)
	{
		if(!playerStaticVar[i][playerLogged])
			continue;

		sendSplitMessage(i, color, returnClanString);
	}
	return true;
}

stock GetClanRankName(playerid)
{
    new rankName[32],
        clanId = playerInfo[playerid][pClan];

    switch(playerInfo[playerid][pClanRank])
    {
        case 1: format(rankName, 32, clanInfo[clanId][clanRank1]);
        case 2: format(rankName, 32, clanInfo[clanId][clanRank2]);
        case 3: format(rankName, 32, clanInfo[clanId][clanRank3]);
        case 4: format(rankName, 32, clanInfo[clanId][clanRank4]);
        case 5: format(rankName, 32, clanInfo[clanId][clanRank5]);
        case 6: format(rankName, 32, clanInfo[clanId][clanRank6]);
        case 7: format(rankName, 32, clanInfo[clanId][clanRank7]);
    }
    return rankName;
}


YCMD:c(playerid, params[], help)
{
	check_clan

	new returnMessage[144];
	if(sscanf(params, "s[144]", returnMessage))
		return sendSyntaxMessage(playerid, "/c <Message>");
	sendClanMessage(playerInfo[playerid][pClan], -1, "{%s}[CLAN] %s %s: %s", clanInfo[playerInfo[playerid][pClan]][clanColor], GetClanRankName(playerid), GetName(playerid), returnMessage);
	return true;
}

YCMD:cmembers(playerid, params[], help)
{
	check_clan

	va_SendClientMessage(playerid, COLOR_TEAL, "Clan members online (%d):", Iter_Count(clanMembers<playerInfo[playerid][pClan]>));

	new clanString[512];
	foreach(new i : Player)
		format(clanString, sizeof clanString, "%s%s [%d]", clanString, GetDBName(i), i);
	
	sendSplitMessage(playerid, -1, clanString);
	return true;
}

YCMD:clan(playerid, params[], help)
{
	check_clan

	Dialog_Show(playerid, DIALOG_CLAN, DIALOG_STYLE_LIST, "Clan", "Clan Help\nClan Members\nClan Settings\nClan Tag\nClan Vehicles\nInformatii seif", "Select", "Close");
	return true;
}

Dialog:DIALOG_CLAN(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	switch(listitem)
	{
		case 0: 
		{
			SendClientMessage(playerid, COLOR_GENANNOUNCE, "Clan help:");
			SendClientMessage(playerid, COLOR_GREY, "/clan - clan menu, /c - clan chat, /cinvite - invite member to clan, /quitclan - quit the clan");
		}
		case 1:
		{
			check_queries

			mysql_format(SQL, returnString, sizeof returnString, "select * from `users` where `Clan` = '%d' order by `ClanRank` desc;", playerInfo[playerid][pClan]);
			mysql_tquery(SQL, returnString, "showClanMembers", "i", playerid);
		}
		case 2:
		{
			if(playerInfo[playerid][pClanRank] < 6)
				return sendError(playerid,"Aceasta sectiune poate fi accesta doar de co/liderul clanului.");

			Dialog_Show(playerid, DIALOG_CMENU_SETTINGS, DIALOG_STYLE_LIST, "Clan settings", "Clan Rank Names\nChange Clan Chat Color\nBuy clan slots (+25) - 50 premium points", "Select", "Cancel");
		}	
		case 3:
		{
			new tagString[256];

			format(tagString, sizeof tagString, "%s\n", playerInfo[playerid][pName]);
			format(tagString, sizeof tagString, "%s%s%s\n", tagString, playerInfo[playerid][pName], clanInfo[playerInfo[playerid][pClan]][clanTag]);
			format(tagString, sizeof tagString, "%s%s%s\n", tagString, clanInfo[playerInfo[playerid][pClan]][clanTag], playerInfo[playerid][pName]);

			format(tagString, sizeof tagString, "%s[%s]%s\n", tagString, clanInfo[playerInfo[playerid][pClan]][clanTag], playerInfo[playerid][pName]);
			format(tagString, sizeof tagString, "%s%s[%s]\n", tagString, playerInfo[playerid][pName], clanInfo[playerInfo[playerid][pClan]][clanTag]);

			format(tagString, sizeof tagString, "%s%s.%s\n", tagString, clanInfo[playerInfo[playerid][pClan]][clanTag], playerInfo[playerid][pName]);
			format(tagString, sizeof tagString, "%s%s.%s\n", tagString, playerInfo[playerid][pName], clanInfo[playerInfo[playerid][pClan]][clanTag]);

			Dialog_Show(playerid, DIALOG_CLAN_TAG, DIALOG_STYLE_LIST, "Clan Tag", tagString, "Select", "Cancel");
		}
		case 4:
		{
			check_queries
			
			mysql_format(SQL, returnString, sizeof returnString, "select * from `clanvehicles` where `clanID` = '%d';", playerInfo[playerid][pClan]);
			mysql_tquery(SQL, returnString, "showClanVehicles", "i", playerid);
		}
		case 5:
		{
			mysql_format(SQL, returnString, sizeof returnString, "select `safeMoney` from `clans` where `id` = '%d';", playerInfo[playerid][pClan]);
			mysql_tquery(SQL, returnString, "showSafeInformations", "i", playerid);
		}
	}
	return true;
}

function:showSafeInformations(playerid)
{
	static returnClanMoney;
	cache_get_value_name_int(0, "safeMoney", returnClanMoney);

	format(returnString, sizeof returnString, "{ffffff}Pentru a depune bani in seif este necesar sa muncesti la unul din joburile de pe server\n10%s din banii castigati de tine vor intra automat in seif [tu nu vei pierde nimic]\nBani aflati in seif: $%s\nAcesti bani pot fi folositi pentru achizitionarea de noi vehicule.\n{ff0000}ATENTIE{ffffff} Liderul nu poate retrage acesti bani!", "%", FormatNumber(returnClanMoney));
	Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Clan Deposit", returnString, "Ok", "");
	return true;
}

YCMD:cinvite(playerid, params[], help)
{
	if(playerInfo[playerid][pClanRank] < 6)
		return true;

	new returnPlayer;
	if(sscanf(params, "u", returnPlayer))
		return sendSyntaxMessage(playerid, "/cinvite [playername/playerid]");

	if(!IsPlayerConnected(returnPlayer))
		return sendError(playerid,"Invalid player.");

	if(playerInfo[returnPlayer][pClan])
		return sendError(playerid,"Jucatorul specificat face deja parte dintr-un clan.");

	//if(GetClanMembers(playerInfo[playerid][pClan]) >= clanInfo[playerInfo[playerid][pClan]][clanSlots]) return sendError(playerid,"Your clan reached the maximum players already, you can't invite a new one.");

	SendClientMessage(playerid, -1, "Processing invite..");
	va_SendClientMessage(playerid, -1, "You have invited %s to join your clan.", GetDBName(returnPlayer));

	va_SendClientMessage(returnPlayer, COLOR_LIGHTBLUE, "%s has invited you to join clan %s (to accept the invitation, type '/accept cinvite %d').", GetDBName(playerid), clanInfo[playerInfo[playerid][pClan]][clanName], playerid);
	playerStaticVar[playerid][clanInvitePlayer] = returnPlayer + 1;
	return true;
}
YCMD:quitclan(playerid, params[], help)
{
	if(!playerInfo[playerid][pClan])
		return sendError(playerid,"You don't have a clan to quit.");

	if(playerInfo[playerid][pClanRank] == 7)
		Dialog_Show(playerid, DIALOG_CLAN_LEAVE, DIALOG_STYLE_MSGBOX, "Esti sigur?", "Vrei sa parasesti clanul curent?\nDeoarece esti lider, clanul va fi sters automat.", "Da", "Nu");
	
	else
		Dialog_Show(playerid, DIALOG_CLAN_LEAVE, DIALOG_STYLE_MSGBOX, "Esti sigur?", "Vrei sa parasesti clanul curent?", "Da", "Nu");
	
	return true;
}

YCMD:clanmotd(playerid, params[], help)
{
	check_clan
	return va_SendClientMessage(playerid, COLOR_GENANNOUNCE, "Clan MOTD: {FFFFFF}%s", clanInfo[playerInfo[playerid][pClan]][clanMotd]);
}
YCMD:cmotd(playerid, params[], help)
{
	check_clan

	if(playerInfo[playerid][pClanRank] < 6)
		return sendError(playerid,"You don't have rank 6+.");

	new returnMotd[128];
	if(sscanf(params, "s[128]", returnMotd))
		return sendSyntaxMessage(playerid, "/cmotd <Message>"), va_SendClientMessage(playerid, COLOR_GENANNOUNCE, "(Clan) MOTD: {FFFFFF}%s", clanInfo[playerInfo[playerid][pClan]][clanMotd]);

	format(clanInfo[playerInfo[playerid][pClan]][clanMotd], sizeof returnMotd, returnMotd);
	sendClanMessage(playerInfo[playerid][pClan], COLOR_GENANNOUNCE, "(Clan) MOTD: {FFFFFF}%s", clanInfo[playerInfo[playerid][pClan]][clanMotd]);
	
	mysql_format(SQL, returnString, sizeof returnString, "update `clans` set `clanMOTD` = '%e' where `ID` = '%d';", returnMotd, playerInfo[playerid][pClan]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

update_player_nickname(playerid)
{
	if(!playerInfo[playerid][pClan])
		return SetPlayerName(playerid, playerInfo[playerid][pName]);

	new nameFormat[MAX_PLAYER_NAME];
	switch(playerInfo[playerid][pClanTagPos])
	{
		case 1: format(nameFormat, sizeof nameFormat, "%s%s", playerInfo[playerid][pName], clanInfo[playerInfo[playerid][pClan]][clanTag]);
		case 2: format(nameFormat, sizeof nameFormat, "%s%s", clanInfo[playerInfo[playerid][pClan]][clanTag], playerInfo[playerid][pName]);
		
		case 3: format(nameFormat, sizeof nameFormat, "[%s]%s", clanInfo[playerInfo[playerid][pClan]][clanTag], playerInfo[playerid][pName]);
		case 4: format(nameFormat, sizeof nameFormat, "%s[%s]", playerInfo[playerid][pName], clanInfo[playerInfo[playerid][pClan]][clanTag]);
		
		case 5: format(nameFormat, sizeof nameFormat, "%s.%s", clanInfo[playerInfo[playerid][pClan]][clanTag], playerInfo[playerid][pName]);
		case 6: format(nameFormat, sizeof nameFormat, "%s.%s", playerInfo[playerid][pName], clanInfo[playerInfo[playerid][pClan]][clanTag]);
		default: format(nameFormat, sizeof nameFormat, "%s", playerInfo[playerid][pName]);
	}
	SetPlayerName(playerid, nameFormat);

	new stringName[64];
    va_PlayerTextDrawSetString(playerid, playerTextdraws[playerid][Logo], "%s / "SERVER_DNS"", stringName);
    PlayerTextDrawShow(playerid, playerTextdraws[playerid][Logo]);

	return true;
}

hook OnPlayerDisconnect(playerid, reason)
{
	if(playerInfo[playerid][pClan] > 0)
	{
		Iter_Remove(clanMembers<playerInfo[playerid][pClan]>, playerid);

		if(playerStaticVar[playerid][playerLogged])
			sendClanMessage(playerInfo[playerid][pClan], COLOR_GENANNOUNCE, "(Clan){FFFFFF} %s from your clan has left the server.", GetName(playerid));
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

WhenPlayerLeaveClan(playerid)
{
	Iter_Remove(clanMembers<playerInfo[playerid][pClan]>, playerid);
	
	playerInfo[playerid][pClan] = 0;
	playerInfo[playerid][pClanRank] = 0;

	update_player_nickname(playerid);
	return true;
}

function:OnPlayerUpdateClan(playerid) {
    mysql_format(SQL,returnString,sizeof(returnString),"update users set `Clan` = '%d',`ClanRank` = '1',`ClanDays` = '%d' WHERE `Name` = '%s'",playerInfo[playerid][pClan],gettime(),playerInfo[playerid][pName]);
    mysql_tquery(SQL,returnString,"","");
    return true;
}

function:showClanMembers(playerid)
{
	new dialogString[1024], returnName[MAX_PLAYER_NAME], returnRank, returnDays, returnMoney, returnWarns;
	for(new i; i < cache_num_rows(); i++)
	{
		cache_get_value_name(i, "Name", returnName);

		cache_get_value_name_int(i, "ClanRank", returnRank);
		cache_get_value_name_int(i, "ClanDays", returnDays);

		cache_get_value_name_int(i, "ClanWarns", returnWarns);
		cache_get_value_name_int(i, "ClanMoney", returnMoney);

	 	cache_get_value_name_int(i, "ID", playerStaticVar[playerid][pDialogItems][i]);
		format(dialogString, sizeof dialogString, "%s%s\t%d [%d/3]\t%d\t$%s\n", dialogString, returnName, returnRank, returnWarns, getDaysFromTimestamp(returnDays), FormatNumber(returnMoney));
	}

	format(dialogString, sizeof dialogString, "Player\tRank\tDays\tMoney deposited\n[+] Invite a new clan member\n%s", dialogString);
	Dialog_ShowPages(playerid, DIALOG_CLANMEMBERS, DIALOG_STYLE_TABLIST_HEADERS, "Clan Members", dialogString, "Select", "Back");
	return true;
}

Dialog:DIALOG_CLANMEMBERS(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	if(!listitem)
		return sendError(playerid,"Usage: /cinvite <id>");

	mysql_format(SQL, returnString, sizeof returnString, "select * from `users` where `ID` = '%d';", playerStaticVar[playerid][pDialogItems][listitem - 1]);
	mysql_tquery(SQL, returnString, "ManageClanUser", "i", playerid);

	return true;
}

new clanMemberSQL[MAX_PLAYERS], clanMemberRank[MAX_PLAYERS], clanMemberWarns[MAX_PLAYERS];
function:ManageClanUser(playerid)
{
	cache_get_value_name_int(0, "ID", clanMemberSQL[playerid]);
	cache_get_value_name_int(0, "ClanRank", clanMemberRank[playerid]);
	
	cache_get_value_name_int(0, "ClanWarns", clanMemberWarns[playerid]);

	static returnDays, lastOnline;
	cache_get_value_name_int(0, "ClanDays", returnDays),
	cache_get_value_name_int(0, "lastOnlineTime", lastOnline);

	new date[6], lastOn[6];
	returnDateFromTimestamp(returnDays, date[0], date[1], date[2], date[3], date[4], date[5], 2);
	returnDateFromTimestamp(lastOnline, lastOn[0], lastOn[1], lastOn[2], lastOn[3], lastOn[4], lastOn[5], 2);

	format(returnString, sizeof returnString, "User: %s\nRank: %d\nCWarns: %d/3\nJoined: %02d-%02d-%02d %02d:%02d:%02d\nLast login: %02d %02d %02d %02d:%02d:%02d\nDays in clan: %d", GetNameBySQL(clanMemberSQL[playerid]), clanMemberRank[playerid], clanMemberWarns[playerid], date[0], date[1], date[2], date[3], date[4], date[5], lastOn[2], lastOn[1], lastOn[0], lastOn[3], lastOn[4], lastOn[5], getDaysFromTimestamp(returnDays));
	Dialog_Show(playerid, DIALOG_CLANMEMBERS2, DIALOG_STYLE_MSGBOX, "Manage user", returnString, (playerInfo[playerid][pClanRank] > 5 ? "Options" : ""), "Back");
	return true;
}

Dialog:DIALOG_CLANMEMBERS2(playerid, response, listitem, inputtext[])
{
	if(!response || playerInfo[playerid][pClanRank] < 6)
		return Command_ReProcess(playerid, "clan", false);

	Dialog_Show(playerid, DIALOG_CLANMEMBERS3, DIALOG_STYLE_LIST, "Manage user", "Set clan rank\nSet clan warn\nUninvite from clan", "Options", "Back");
	return true;
}

Dialog:DIALOG_CLANMEMBERS3(playerid, response, listitem, inputtext[])
{
	if(!response)
		return Command_ReProcess(playerid, "clan", false);

	switch(listitem)
	{
		case 0:
		{
			new dialogString[256];
			for(new i = 1; i <= 6; i++)
				format(dialogString, sizeof dialogString, "%s\nSet rank %d\n", dialogString, i);

			if(playerInfo[playerid][pClanRank] == 7)
				format(dialogString, sizeof dialogString, "%s\nSet rank 7 - you will get demoted to rank 6", dialogString);

			Dialog_Show(playerid, DIALOG_CLAN_RANK, DIALOG_STYLE_LIST, "Set user rank", dialogString, "Select", "Back");
		}
		case 1:
		{
			Dialog_Show(playerid, DIALOG_CLAN_WARNS, DIALOG_STYLE_LIST, "Set user warns", "Warns: 0/3\nWarns: 1/3\nWarns: 2/3", "Select", "Back");
		}
		case 2:
		{
			if(playerInfo[playerid][pSQLID] == clanMemberSQL[playerid])
				return sendError(playerid,"Nu iti poti face singur aceasta modificare.");

			if(clanMemberRank[playerid] > 5 && playerInfo[playerid][pClanRank] != 7)
				return sendError(playerid,"Numai liderul (rank 7) poate da afara membrii cu rank mai mare de 5.");

			mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Clan` = '0', `ClanDays` = '0', `ClanRank` = '0', `ClanWarns` = '0', `ClanMoney` = '0' where `ID` = '%d';", clanMemberSQL[playerid]);
			mysql_tquery(SQL, returnString, "", "");

			insertPlayerMail(playerid, gettime(), "Ai fost eliminat din clanul din care faceai parte de catre %s", GetDBName(playerid));

			foreach(new i : clanMembers<playerInfo[playerid][pClan]>)
			{
				if(clanMemberSQL[playerid] != playerInfo[i][pSQLID])
					continue;

				SendClientMessage(playerid, COLOR_CLIENT, "(Clan):{ffffff} Ai fost eliminat din clanul din care faceai parte de catre %s", GetDBName(playerid));
				WhenPlayerLeaveClan(i);
				break;
			}
			sendClanMessage(playerInfo[playerid][pClan], -1, "{%s}[CLAN] %s left the clan (uninvited by %s).",  clanInfo[playerInfo[playerid][pClan]][clanColor], GetNameBySQL(clanMemberSQL[playerid]), GetDBName(playerid));
		}
	}
	return true;
}

Dialog:DIALOG_CLAN_WARNS(playerid, response, listitem, inputtext[])
{
	if(!response)
		return Command_ReProcess(playerid, "clan", false);

	if(clanMemberRank[playerid] > 5 && playerInfo[playerid][pClanRank] != 7)
		return sendError(playerid,"Numai liderul (rank 7) poate seta warn levelul membrilor cu rank mai mare de 5.");

	sendClanMessage(playerInfo[playerid][pClan], -1, "{%s}[CLAN] %s changed %s's warn level to %d.", clanInfo[playerInfo[playerid][pClan]][clanColor], GetDBName(playerid), GetNameBySQL(clanMemberSQL[playerid]), listitem);

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `ClanWarns` = '%d' where `ID` = '%d';", listitem, clanMemberSQL[playerid]);
	mysql_tquery(SQL, returnString, "", "");

	foreach(new i : clanMembers<playerInfo[playerid][pClan]>)
	{
		if(clanMemberSQL[playerid] != playerInfo[i][pSQLID])
			continue;

		playerInfo[i][pClanWarns] = listitem;
		break;
	}
	return true;
}

Dialog:DIALOG_CLAN_RANK(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	if(playerInfo[playerid][pSQLID] == clanMemberSQL[playerid])
		return sendError(playerid,"Nu iti poti face singur aceasta modificare.");

	if(clanMemberRank[playerid] > 5 && playerInfo[playerid][pClanRank] != 7)
		return sendError(playerid,"Numai liderul (rank 7) poate seta rank-ul membrilor cu rank mai mare de 5.");

	Dialog_Show(playerid, DIALOG_CONFIRM_CRANK, DIALOG_STYLE_MSGBOX, "Confirm:", va_return("Esti sigur ca doresti sa-i dai lui %s rank %d in clan?", GetNameBySQL(clanMemberSQL[playerid]), listitem + 1), "Da", "Nu");
	playerStaticVar[playerid][pDialogItems][0] = listitem + 1;
	return true;
}
	
Dialog:DIALOG_CONFIRM_CRANK(playerid, response, listitem, inputtext[])
{
	if(!response)
		return Command_ReProcess(playerid, "clan", false);

	new cRank = playerStaticVar[playerid][pDialogItems][0];

	sendClanMessage(playerInfo[playerid][pClan], -1, "{%s}[CLAN] %s changed %s's rank to %d.", clanInfo[playerInfo[playerid][pClan]][clanColor], GetDBName(playerid), GetNameBySQL(clanMemberSQL[playerid]), cRank);

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `ClanRank` = '%d' where `ID` = '%d';", cRank, clanMemberSQL[playerid]);
	mysql_tquery(SQL, returnString, "", "");

	foreach(new i : clanMembers<playerInfo[playerid][pClan]>)
	{
		if(clanMemberSQL[playerid] != playerInfo[i][pSQLID])
			continue;

		playerInfo[i][pClanRank] = cRank;
		break;
	}

	if(listitem == 6)
	{
		playerInfo[playerid][pClanRank] = 6;

		mysql_format(SQL, returnString, sizeof returnString, "update `users` set `ClanRank` = '6' where `ID` = '%d';", playerInfo[playerid][pSQLID]);
		mysql_tquery(SQL, returnString, "", "");
	}
	return true;
}

Dialog:DIALOG_CLAN_TAG(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	playerInfo[playerid][pClanTagPos] = listitem;

	new stringLengthName = strlen(playerInfo[playerid][pName]),
		stringLengthTag = strlen(clanInfo[playerInfo[playerid][pClan]][clanTag]);

	if(stringLengthName + stringLengthTag >= MAX_PLAYER_NAME)
		return sendError(playerid,"Numele tau este prea lung pentru a purta TAG-ul clanului in acest fel. Incearca o alta varianta");

	update_player_nickname(playerid);

	va_SendClientMessage(playerid, COLOR_GENANNOUNCE, "Your nickname is now: %s", GetName(playerid));

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `ClanTagPos` = '%d' where `id` = '%d';", listitem, playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

Dialog:DIALOG_CLAN_LEAVE(playerid, response, listitem, inputtext[])
{
	if(!response || !playerInfo[playerid][pClan])
		return true;

	if(playerInfo[playerid][pClanRank] == 7)
	{
		new playerClan = playerInfo[playerid][pClan];

		foreach(new i : clanMembers<playerClan>)
		{
			playerInfo[i][pClan] = 0;
			playerInfo[i][pClanRank] = 0;
			playerInfo[i][pClanDays] = 0;

			update_player_nickname(i);
		}
		Iter_Clear(clanMembers<playerClan>);

		mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Clan` = '0', `ClanRank` = '0', `ClanDays` = '0' where `Clan` = '%d';", playerClan);
		mysql_tquery(SQL, returnString, "", "");

		mysql_format(SQL, returnString, sizeof returnString, "update `clans` set `Active` = '0' where `id` = '%d';", playerClan);
		mysql_tquery(SQL, returnString, "", "");
		return true;
	}

	sendClanMessage(playerInfo[playerid][pClan], -1, "{%s}[CLAN] %s has left the clan (using /quitclan).", clanInfo[playerInfo[playerid][pClan]][clanColor], GetDBName(playerid));
	WhenPlayerLeaveClan(playerid);
	
	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Clan` = '0', `ClanDays` = '0', `ClanRank` = '0', `ClanWarns` = '0', `ClanMoney` = '0' where `ID` = '%d';", playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

Dialog:DIALOG_CMENU_SETTINGS(playerid, response, listitem, inputtext[])
{
	if(!response || !playerInfo[playerid][pClan])
		return Command_ReProcess(playerid, "clan", false);

	if(playerInfo[playerid][pClanRank] < 6)
		return sendError(playerid,"Aceasta sectiune poate fi accesta doar de co/liderul clanului.");

	new cId = playerInfo[playerid][pClan];
	switch(listitem)
	{
		case 0:
		{
			format(returnString, sizeof returnString, "Rank 1: %s\nRank 2: %s\nRank 3: %s\nRank 4: %s\nRank 5: %s\nRank 6: %s\nRank 7: %s",
				clanInfo[cId][clanRank1], clanInfo[cId][clanRank2], clanInfo[cId][clanRank3], clanInfo[cId][clanRank4], clanInfo[cId][clanRank5], clanInfo[cId][clanRank6], clanInfo[cId][clanRank7]);

			Dialog_Show(playerid, DIALOG_CLAN_CHANGERANK, DIALOG_STYLE_LIST, "Change clan ranks", returnString, "Change", "Back");
		}
		case 1: Dialog_Show(playerid, DIALOG_CLAN_CHANGE_COLOR, DIALOG_STYLE_INPUT, "Clan color", "You can go to http://www.colorpicker.com to get html color codes\nEnter the HTML code for the clan color (default FFCC99):", "Change", "Back");
		case 2: {
			clanInfo[cId][clanSlots] += 25;
			playerInfo[playerid][pPremiumPoints] -= 50;
			SendClientMessage(playerid, -1, "Ai cumparat +25 slot-uri pentru clan in schimbul a 50 premium points.");
			mysql_format(SQL, returnString, sizeof returnString, "update `clans` set `clanSlots` = '%d' where `ID` = '%d';", clanInfo[cId][clanSlots], cId);
			mysql_tquery(SQL, returnString, "", "");
		}
	}
	return true;
}	

new clanSelectColor[MAX_PLAYERS];
Dialog:DIALOG_CLAN_CHANGERANK(playerid, response, listitem, inputtext[])
{
	if(!response || !playerInfo[playerid][pClan])
		return Command_ReProcess(playerid, "clan", false);

	if(playerInfo[playerid][pClanRank] < 6)
		return sendError(playerid,"Aceasta sectiune poate fi accesta doar de co/liderul clanului.");

	new currentRank[32], cId = playerInfo[playerid][pClan];
	switch(listitem)
	{
		case 1: format(currentRank, 32, clanInfo[cId][clanRank1]);
		case 2: format(currentRank, 32, clanInfo[cId][clanRank2]);
		case 3: format(currentRank, 32, clanInfo[cId][clanRank3]);
		case 4: format(currentRank, 32, clanInfo[cId][clanRank4]);
		case 5: format(currentRank, 32, clanInfo[cId][clanRank5]);
		case 6: format(currentRank, 32, clanInfo[cId][clanRank6]);
		case 7: format(currentRank, 32, clanInfo[cId][clanRank7]);
	}
	clanSelectColor[playerid] = listitem + 1;

	format(returnString, sizeof returnString, "Current clan rank %d: %s\n\nSet a new clan rank:", listitem + 1, currentRank);
	Dialog_Show(playerid, DIALOG_CLAN_CHANGERANK2, DIALOG_STYLE_INPUT, "Change clan rank", returnString, "Change", "Back");
	return true;
}

Dialog:DIALOG_CLAN_CHANGERANK2(playerid, response, listitem, inputtext[])
{
	if(!response || !playerInfo[playerid][pClan])
		return Command_ReProcess(playerid, "clan", false);

	if(playerInfo[playerid][pClanRank] < 6)
		return sendError(playerid,"Aceasta sectiune poate fi accesta doar de co/liderul clanului.");

	if(strfind(inputtext, "'", true) == 0 || strfind(inputtext, "`", true) == 0 || strfind(inputtext, "/", true) == 0 || strfind(inputtext, "|", true) == 0)
		return sendError(playerid,"Ai folosit caractere interzise.");

	new cId = playerInfo[playerid][pClan], currentRank = clanSelectColor[playerid];
	switch(currentRank)
	{
		case 1: format(clanInfo[cId][clanRank1], 32, inputtext);
		case 2: format(clanInfo[cId][clanRank2], 32, inputtext);
		case 3: format(clanInfo[cId][clanRank3], 32, inputtext);
		case 4: format(clanInfo[cId][clanRank4], 32, inputtext);
		case 5: format(clanInfo[cId][clanRank5], 32, inputtext);
		case 6: format(clanInfo[cId][clanRank6], 32, inputtext);
		case 7: format(clanInfo[cId][clanRank7], 32, inputtext);
	}
	va_SendClientMessage(playerid, -1, "{%s}Clan rank %d: %s.", clanInfo[playerInfo[playerid][pClan]][clanColor], currentRank, inputtext);

	mysql_format(SQL, returnString, sizeof returnString, "update `clans` set `clanRank%d` = '%e' where `ID` = '%d';", currentRank, inputtext, cId);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

Dialog:DIALOG_CLAN_CHANGE_COLOR(playerid, response, listitem, inputtext[])
{
	if(!response || !playerInfo[playerid][pClan])
		return Command_ReProcess(playerid, "clan", false);

	if(playerInfo[playerid][pClanRank] < 6)
		return sendError(playerid,"Aceasta sectiune poate fi accesta doar de co/liderul clanului.");

	if(strlen(inputtext) != 6)
		return sendError(playerid,"Culoare invalida.");

	format(clanInfo[playerInfo[playerid][pClan]][clanColor], 7, "%s", inputtext);

	va_SendClientMessage(playerid, -1, "Clan color changed to {%s}%s{FFFFFF}.", clanInfo[playerInfo[playerid][pClan]][clanColor], GetClanColor(playerInfo[playerid][pClan]));
	
	mysql_format(SQL, returnString, sizeof returnString, "update `clans` set `clanColor` = '%e' where `ID` = '%d';", clanInfo[playerInfo[playerid][pClan]][clanColor], playerInfo[playerid][pClan]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}