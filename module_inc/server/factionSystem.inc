#include <YSI_Coding\y_hooks>
#define check_group if(!playerInfo[playerid][pGroup]) return sendError(playerid,"You are not a part of a group.");
#define check_duty_status if(!playerStaticVar[playerid][groupDuty]) return sendError(playerid,"You need to be ON-DUTY to do this.");

#define MAX_FACTIONS 17
#define MAX_GROUP_SLOTS 20

#define STREAMER_BEGIN_FACTIONS (STREAMER_BEGIN_BUSINESS_SPHERE + 100)

#define GROUP_TYPE_LSPD 1
#define GROUP_TYPE_FBI 2
#define GROUP_TYPE_NG 3
#define GROUP_TYPE_ITALIAN 4
#define GROUP_TYPE_DNB 5
#define GROUP_TYPE_CRIPS 6
#define GROUP_TYPE_TAXI_LV 7
#define GROUP_TYPE_LVPD 8
#define GROUP_TYPE_NR 9
#define GROUP_TYPE_RUSSIAN 10
#define GROUP_TYPE_HITMAN 11
#define GROUP_TYPE_SI 12
#define GROUP_TYPE_TAXI 13
#define GROUP_TYPE_PARAMEDIC 14
#define GROUP_TYPE_RDT 15
#define GROUP_TYPE_CORLEONE 16

#define FACTION_TYPE_OTHER 0
#define FACTION_TYPE_POLICE 1
#define FACTION_TYPE_GANG 2

new Iterator: factionMembers<MAX_FACTIONS, MAX_PLAYERS>, serverFactions, Iterator: serverLeaders<MAX_PLAYERS>, Iterator:serverContracts<MAX_PLAYERS>, Iterator: warGangsters<MAX_PLAYERS>,
	Iterator:taxiCallers<MAX_PLAYERS>, onDutyCopsCount, Iterator:departmentsMembers<MAX_PLAYERS>, Iterator:medicCallers<MAX_PLAYERS>, factionType[MAX_FACTIONS], FactionSpec[MAX_PLAYERS],
	factionWar[MAX_FACTIONS] = {-1, ...},
	raportPoints[MAX_FACTIONS][8][5], gChat[MAX_FACTIONS];

enum ENUM_GROUP_INFORMATIONS
{
	factionID, fName[32], Float:factionExterior[3], factionSphere, factionLocked, Text3D:factionLabel, factionPickup, factionIcon, factionSlots,
	gTurfScore, warActive, groupMotd[128], safeMoney, safeMaterials, safeDrugs, fRankName1[32], fRankName2[32], fRankName3[32],
	fRankName4[32], fRankName5[32], fRankName6[32], fRankName7[32]
};
new groupInfo[MAX_FACTIONS][ENUM_GROUP_INFORMATIONS];

new rank_up_days[] =
{
	7, //rank 2
	14, //rank 3
	28, //rank 4
	42 // rank 5
};

factionName(factionid)
{
	new returnName[32];
	if(!factionid)
	{
		returnName = "Civilian";
		return returnName;
	}

	format(returnName, sizeof returnName, "%s", groupInfo[factionid][fName]);
	return returnName;
}
isPlayerDuty(const playerid) {
	return playerStaticVar[playerid][groupDuty];
}


hook loadServerSystems()
{
	
	mysql_tquery(SQL, "select * from `factions`", "loadFactions", "");
	mysql_tquery(SQL, "select * from `gang_turfs`", "load_turfs", "");
	return Y_HOOKS_CONTINUE_RETURN_1;
}
isCop(playerid) return playerInfo[playerid][pGroup] > 0 && playerInfo[playerid][pGroup] == 1 && playerInfo[playerid][pGroup] == 8  ? true : false;
hook OnPlayerEnterDynArea(playerid, areaid)
{
	new faction = Streamer_GetIntData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID) - STREAMER_BEGIN_FACTIONS;
	if(serverFactions + 1 < faction)
		return true;

	SetPVarInt(playerid, "areaID_factions", faction);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerLeaveDynArea(playerid, areaid)
{
	SetPVarInt(playerid, "areaID_factions", 0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnGameModeInit()
{
	Command_AddAltNamed("auninvite", "fpk");
	Command_AddAltNamed("fdeposit", "gdeposit");
	Command_AddAltNamed("government", "gov");

	Command_AddAltNamed("radio", "r");

	for(new i; i < MAX_FACTIONS; i++)
	{
		if(i == 1 || i == 2 || i == 3 || i == 8)
			factionType[i] = FACTION_TYPE_POLICE;
		else if(i == 4 || i == 5 || i == 6 || i == 10 || i == 15 || i == 16)
			factionType[i] = FACTION_TYPE_GANG;
		else
			factionType[i] = FACTION_TYPE_OTHER;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

new faction_skins[][] =
{
	{},
	{71, 284, 281, 266, 280, 267, 267, 306},
	{163, 164, 166, 166, 286, 295, 295, 307},
	{287, 287, 287, 285, 288, 273, 273, 76},
	{116, 116, 114, 114, 115, 113, 113, 148},
	{105, 105, 106, 107, 270, 271, 271, 169},
	{110, 110, 109, 108, 177, 176, 176, 191},
	{253, 155, 61, 61, 227, 228, 228, 141},
	{71, 284, 281, 266, 282, 267, 267, 309},
	{188, 188, 17, 187, 187, 147, 147, 141},
	{103, 103, 102, 104, 98, 296, 296, 195},
	{208, 117, 117, 120, 186, 294, 294, 225},
	{153, 171, 171, 240, 240, 189, 189, 194},
	{253, 155, 61, 61, 227, 228, 228, 141},
	{276, 275, 274, 277, 279, 228, 228, 308},
	{121, 121, 118, 122, 124, 120, 120, 193},
	{111, 112, 118, 118, 59, 272, 272, 192}
};
setPlayerFactionColor(playerid, groupID) {

	switch(groupID)
	{
		case 1: SetPlayerColor(playerid, 0x0049D0FF);
		case 2: SetPlayerColor(playerid, 0x005AFFFF);
		case 3: SetPlayerColor(playerid, 0x00006AFF);
		case 4: SetPlayerColor(playerid, 0x1ae5ceFF);
		case 5: SetPlayerColor(playerid, 0x075a00FF);
		case 6: SetPlayerColor(playerid, 0xFFAE00FF);
		case 7: SetPlayerColor(playerid, 0xefef04FF);
		case 8: SetPlayerColor(playerid, 0x0049D0FF);
		case 9: SetPlayerColor(playerid, 0xdfb9efFF);
		case 10: SetPlayerColor(playerid, 0x960b9dFF);
		case 11: SetPlayerColor(playerid, 0x8f1b1bFF);
		case 12: SetPlayerColor(playerid, 0x04d47cFF);
		case 13: SetPlayerColor(playerid, 0xefef04FF);
		case 14: SetPlayerColor(playerid, 0xf66249FF);
		case 15: SetPlayerColor(playerid, 0xba1511FF);
		case 16: SetPlayerColor(playerid, 0x004D4DFF);
		default: SetPlayerColor(playerid, 0xFFFFFFFF);
	}

	return true;
}
setPlayerDuty(playerid, bool: duty)
{
	new playerGroup = playerInfo[playerid][pGroup];
	switch(duty)
	{
		case true:
		{
			if(factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_POLICE)
			{

			    if(!playerInfo[playerid][pGunLic]) 
			        return sendError(playerid, "You don't have a valid gun licence.");

				onDutyCopsCount ++;

				GivePlayerWeapon(playerid, WEAPON_DEAGLE, 500);
				GivePlayerWeapon(playerid, WEAPON_NITESTICK, 1);
				GivePlayerWeapon(playerid, WEAPON_SPRAYCAN, 500);
				GivePlayerWeapon(playerid, WEAPON_MP5, 1000);
				GivePlayerWeapon(playerid, WEAPON_M4, 1000); 

				SetPlayerHealthEx(playerid, 99.0);
				SetPlayerArmourEx(playerid, 99.0);
			}

			if(factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_GANG) {
				Iter_Add(warGangsters, playerid);
			}
			TextDrawShowForPlayer(playerid, dutyTD);
			playerStaticVar[playerid][groupDuty] = true;

			setPlayerFactionColor(playerid, playerGroup);

			if(playerInfo[playerid][pGender] == 1)
				SetPlayerSkin(playerid, faction_skins[playerGroup][7]);

			else
				SetPlayerSkin(playerid, faction_skins[playerGroup][playerInfo[playerid][pGroupRank] - 1]);

		}
		case false:
		{
			SetPlayerColor(playerid, COLOR_WHITE_T);
			SetPlayerSkin(playerid, playerInfo[playerid][pSkin]);

			SetPlayerArmourEx(playerid, 0.0);
			ResetPlayerWeapons(playerid);
			TextDrawHideForPlayer(playerid, dutyTD);
			playerStaticVar[playerid][groupDuty] = false;

			if(factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_POLICE)
				onDutyCopsCount --;


			if(playerInfo[playerid][pGroup] == GROUP_TYPE_TAXI)
				PlayerTextDrawHide(playerid, playerTextdraws[playerid][transportTD]);

			if(factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_GANG)
				Iter_Remove(warGangsters, playerid);

		}
	}
	if(factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_POLICE)
	{
		sendNearbyMessage(playerid, COLOR_PURPLE, "* Officer %s %s", GetName(playerid), (playerStaticVar[playerid][groupDuty] ? ("took some guns and a badge from his locker") : ("places his badge and guns in his locker.")));
	}
	else va_SendClientMessage(playerid, -1, "You are now %s duty.", (playerStaticVar[playerid][groupDuty] ? ("on") : ("off")));
	return true;
}
new const faction_colors[17] = {
	0xFFFFFFFF,
	0x0049D0FF,
	0x005AFFFF,
	0x00006AFF,
	0x1ae5ceFF,
	0x075a00FF,
	0xFFAE00FF,
	0xefef04FF,
	0x0049D0FF,
	0xdfb9efFF,
	0x960b9dFF,
	0x8f1b1bFF,
	0x04d47cFF,
	0xefef04FF,
	0xf66249FF,
	0xba1511FF,
	0x004D4DFF

};
new const turfs_colors[17] = {
	0xFFFFFF00,
	0x0049D0FF,
	0x005AFFFF,
	0x00006AFF,

	0x1ae5ceC8,
	0x075a00C8,
	0xFFAE00C8,

	0xefef04FF,
	0x0049D0FF,
	0xdfb9efFF,

	0x960b9dC8,

	0x8f1b1bFF,
	0x04d47cFF,
	0xefef04FF,
	0xf66249FF,
	
	0xba1511C8,
	0x004D4DC8

};

YCMD:granknames(playerid, params[], help)
{
    if(playerInfo[playerid][pGroupRank] < 7)
        return true;

    new rankId, rankName[32], rankNameCmd[32], rankDbName[16];
    if(sscanf(params, "ds[32]", rankId, rankNameCmd))
        return sendSyntaxMessage(playerid, "/granknames [rankid (1-7)] [rank title]");
    
    if(rankId < 1 || rankId > 7)
        return true;

    strcpy(rankName, rankNameCmd);
    va_SendClientMessage(playerid, -1, "You have changed the title of Rank %d to '%s'.", rankId, rankName);

    switch(rankId)
    {
        case 1: format(groupInfo[playerInfo[playerid][pGroup]][fRankName1], sizeof rankName, rankName), rankDbName = "Rank1";
        case 2: format(groupInfo[playerInfo[playerid][pGroup]][fRankName2], sizeof rankName, rankName), rankDbName = "Rank2";
        case 3: format(groupInfo[playerInfo[playerid][pGroup]][fRankName3], sizeof rankName, rankName), rankDbName = "Rank3";
        case 4: format(groupInfo[playerInfo[playerid][pGroup]][fRankName4], sizeof rankName, rankName), rankDbName = "Rank4";
        case 5: format(groupInfo[playerInfo[playerid][pGroup]][fRankName5], sizeof rankName, rankName), rankDbName = "Rank5";
        case 6: format(groupInfo[playerInfo[playerid][pGroup]][fRankName6], sizeof rankName, rankName), rankDbName = "Rank6";
        case 7: format(groupInfo[playerInfo[playerid][pGroup]][fRankName7], sizeof rankName, rankName), rankDbName = "Rank7";
    }
    mysql_format(SQL, returnString, sizeof returnString, "update `factions` set `%s` = '%s' where `ID` = '%d';", rankDbName, rankName, playerInfo[playerid][pGroup]);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

YCMD:fspec(playerid, params[], help)
{
	check_admin

	new returnGroup;
	if(sscanf(params, "d", returnGroup))
		return sendSyntaxMessage(playerid, "/fspec <FactionID>");

	FactionSpec[playerid] = returnGroup;
	
	va_SendClientMessage(playerid, -1, "Now you follow the %s`s faction chat", factionName(returnGroup));
	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_DCHAT, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s is now spectating faction %s's chat.", GetDBName(playerid), factionName(returnGroup));
	return true;
}
YCMD:fspecid(playerid, params[], help)
{
	check_admin

	new returnPlayer, returnGroup;
	if(sscanf(params, "u", returnPlayer))
		return sendSyntaxMessage(playerid, "/fspecid <Name/Playerid>");

	if(!IsPlayerConnected(returnPlayer))
		return sendError(playerid,"Player not connected.");

	returnGroup = playerInfo[returnPlayer][pGroup];
	FactionSpec[playerid] = returnGroup;
	
	va_SendClientMessage(playerid, -1, "Now you follow the %s`s faction chat", factionName(returnGroup));
	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_DCHAT, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s is now spectating faction %s's chat.", GetDBName(playerid), factionName(returnGroup));
	return true;
}

YCMD:fchat(playerid, params[], help)
{
	check_admin

	new returnText[144];
	if(sscanf(params, "s[144]", returnText))
		return sendSyntaxMessage(playerid, "/fchat <Message>");

	if(!FactionSpec[playerid])
		return sendError(playerid,"You are not spectating any faction.");
	//9D72FF admin /f //0x00958cAA
	
	sendGroupMessage(FactionSpec[playerid], (factionType[FactionSpec[playerid]] == FACTION_TYPE_POLICE ? (0x7c7ce0AA) : (0x00958cAA)), "# Admin %s: %s", GetName(playerid), returnText);
	return true;
}
YCMD:f(playerid, params[], help)
{
	new returnMessage[144];
	if(sscanf(params, "s[144]", returnMessage))
		return sendSyntaxMessage(playerid, "/f <Message>");

	if(!playerInfo[playerid][pGroup] || playerInfo[playerid][pGroup] == 1 || playerInfo[playerid][pGroup] == 2 || playerInfo[playerid][pGroup] == 3 || playerInfo[playerid][pGroup] == 8)
		return SendClientMessage(playerid, COLOR_GREY, "Your group data is invalid.");

	if(gChat[playerInfo[playerid][pGroup]])
		return SendClientMessage(playerid, COLOR_GREY, "Jucatorii de rank 1-5 nu pot folosi chatul factiunii in acest moment.");

	if(FindIP(returnMessage)) /// Anti-Reclama
		sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s(%d) typed %s in /f.", GetName(playerid), playerid, returnMessage);

	sendGroupMessage(playerInfo[playerid][pGroup], (playerInfo[playerid][pGroupRank] < 6 ? COLOR_MEMBER : COLOR_LEADER), "# %s %s: %s", GetFactionRankName(playerid), GetName(playerid), returnMessage);
	return true;
}

YCMD:radio(playerid, params[], help)
{
	if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_POLICE && playerInfo[playerid][pGroup] != GROUP_TYPE_PARAMEDIC)
		return sendError(playerid,"Invalid group data.");

	new returnMessage[144];
	if(sscanf(params, "s[144]", returnMessage))
		return sendSyntaxMessage(playerid, "/r <Message>");

	if(gChat[playerInfo[playerid][pGroup]])
		return SendClientMessage(playerid, COLOR_GREY, "Jucatorii de rank 1-5 nu pot folosi chatul factiunii in acest moment.");

	if(FindIP(returnMessage)) /// Anti-Reclama
		sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s(%d) typed %s in /r.", GetName(playerid), playerid, returnMessage);

	sendGroupMessage(playerInfo[playerid][pGroup], (playerInfo[playerid][pGroupRank] < 6 ? (COLOR_RADIOCHAT) : (0x7c7ce0AA)), "%s %s %s: %s, over.", (playerInfo[playerid][pGroupRank] < 5 ? ("*") : playerInfo[playerid][pGroupRank] == 5 ? ("**") : ("***")), GetFactionRankName(playerid), GetName(playerid), returnMessage);
	return true;
}
YCMD:gchat(playerid, params[], help)
{
	if(playerInfo[playerid][pGroupRank] < 6)
		return returnNoAcces(playerid);

	new groupId = playerInfo[playerid][pGroup];
	if(gChat[groupId] == 0)
	{
		sendGroupMessage(playerInfo[playerid][pGroup], COLOR_LEADER, "Players with rank 1-5 can't now use the faction chat!");
		gChat[groupId] = 1;
	}
	else
	{
		sendGroupMessage(playerInfo[playerid][pGroup], COLOR_LEADER, "Players with rank 1-5 can now use the faction chat!");
		gChat[groupId] = 0;
	}
	return true;
}

YCMD:invite(playerid, params[], help)
{
	if(playerInfo[playerid][pGroupRank] < 6)
		return sendError(playerid,"Nu ai rank-ul necesar pentru a invita un player in factiune.");

	new returnPlayer;
	if(sscanf(params, "u", returnPlayer))
		return sendSyntaxMessage(playerid, "/invite [playername/playerid]");

	if(!IsPlayerConnected(returnPlayer))
		return sendError(playerid,"Invalid player.");

	if(playerInfo[returnPlayer][pGroupPunish])
		return sendError(playerid,"You can not invite this player because he have faction punish.");

	if(playerInfo[returnPlayer][pGroup])
		return sendError(playerid,"Jucatorul specificat face deja parte dintr-o factiune.");

	SendClientMessage(playerid, -1, "Processing invite..");
	va_SendClientMessage(playerid, -1, "You have invited %s to join your group.", GetDBName(returnPlayer));

	va_SendClientMessage(returnPlayer, COLOR_LIGHTBLUE, "%s has invited you to join group %s (to accept the invitation, type '/accept invite %d').", GetDBName(playerid), factionName(playerInfo[playerid][pGroup]), playerid);
	playerStaticVar[playerid][factionInvitePlayer] = returnPlayer + 1;
	return true;
}

YCMD:debugraport(playerid, params[], help) {

	mysql_format(SQL, returnString, sizeof returnString, "select * from `users` where `Faction` >= '1';");
	mysql_tquery(SQL, returnString, "checkFactionActivity", "");
	return true;
}
YCMD:myraport(playerid, params[], help)
{
	check_group

	va_SendClientMessage(playerid, -1, "----------------------------["COLOR_SV_HEX"%s{ffffff}] faction activity ----------------------------", playerInfo[playerid][pName]);

	if(factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_POLICE)
	{
		if(raportPoints[playerInfo[playerid][pGroup]][playerInfo[playerid][pGroupRank]][0])
			va_SendClientMessage(playerid, -1, "Runners: "COLOR_SV_HEX"%d{FFFFFF}/"COLOR_SV_HEX"%d", playerInfo[playerid][pRaportScore][0], raportPoints[playerInfo[playerid][pGroup]][playerInfo[playerid][pGroupRank]][0]);
		
		if(raportPoints[playerInfo[playerid][pGroup]][playerInfo[playerid][pGroupRank]][1])
			va_SendClientMessage(playerid, -1, "Arrested players: "COLOR_SV_HEX"%d{FFFFFF}/"COLOR_SV_HEX"%d", playerInfo[playerid][pRaportScore][1], raportPoints[playerInfo[playerid][pGroup]][playerInfo[playerid][pGroupRank]][1]);

		if(raportPoints[playerInfo[playerid][pGroup]][playerInfo[playerid][pGroupRank]][2])
			va_SendClientMessage(playerid, -1, "Tickets gaved: "COLOR_SV_HEX"%d{FFFFFF}/"COLOR_SV_HEX"%d", playerInfo[playerid][pRaportScore][2], raportPoints[playerInfo[playerid][pGroup]][playerInfo[playerid][pGroupRank]][2]);
		
		if(raportPoints[playerInfo[playerid][pGroup]][playerInfo[playerid][pGroupRank]][3])
			va_SendClientMessage(playerid, -1, "Drugs confiscated: "COLOR_SV_HEX"%d{FFFFFF}/"COLOR_SV_HEX"%d", playerInfo[playerid][pRaportScore][3], raportPoints[playerInfo[playerid][pGroup]][playerInfo[playerid][pGroupRank]][3]);
		
		if(raportPoints[playerInfo[playerid][pGroup]][playerInfo[playerid][pGroupRank]][4])
			va_SendClientMessage(playerid, -1, "Licences confiscated: "COLOR_SV_HEX"%d{FFFFFF}/"COLOR_SV_HEX"%d", playerInfo[playerid][pRaportScore][4], raportPoints[playerInfo[playerid][pGroup]][playerInfo[playerid][pGroupRank]][4]);

		va_SendClientMessage(playerid, -1, "Deposited money: "COLOR_SV_HEX"%d", playerInfo[playerid][pRaportScore][5]);
	}
	else if(factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_GANG)
	{
		va_SendClientMessage(playerid, -1, "War Kills: "COLOR_SV_HEX"%d{ffffff} / War Deaths: "COLOR_SV_HEX"%d{ffffff}", playerInfo[playerid][pRaportScore][1], playerInfo[playerid][pRaportScore][2]);
		
		if(raportPoints[playerInfo[playerid][pGroup]][playerInfo[playerid][pGroupRank]][0])
			va_SendClientMessage(playerid, -1, "Deposited materials: "COLOR_SV_HEX"%d{FFFFFF}/"COLOR_SV_HEX"%d", playerInfo[playerid][pRaportScore][0], raportPoints[playerInfo[playerid][pGroup]][playerInfo[playerid][pGroupRank]][0]);
	}
	else
	{
		new returnMessage[32];
		switch(playerInfo[playerid][pGroup])
		{
			case 7, 13: returnMessage = "Transported players";
			case 9: returnMessage = "Anunturi plasate";
			case 11: returnMessage = "Contracte efectuate";
			case 12: returnMessage = "Licente vandute";
			case 14: returnMessage = "Healed players";
		}
		if(raportPoints[playerInfo[playerid][pGroup]][playerInfo[playerid][pGroupRank]][0])
			va_SendClientMessage(playerid, -1, "%s: "COLOR_SV_HEX"%d{FFFFFF}/"COLOR_SV_HEX"%d", returnMessage, playerInfo[playerid][pRaportScore][0], raportPoints[playerInfo[playerid][pGroup]][playerInfo[playerid][pGroupRank]][0]);
		
		va_SendClientMessage(playerid, -1, "Deposited money: "COLOR_SV_HEX"%d", playerInfo[playerid][pRaportScore][5]);
	}

	new raportStatus = 1;
	for(new rId; rId <= 4; rId++)
	{
		if(playerInfo[playerid][pRaportScore][rId] < raportPoints[playerInfo[playerid][pGroup]][playerInfo[playerid][pGroupRank]][rId])
		{
			raportStatus = 0;
		}
	}

    if(playerInfo[playerid][pGroupRank] == 7)
		SendClientMessage(playerid, -1, "Raport Status: "COLOR_SV_HEX"no raport to complete");
	else
		SendClientMessage(playerid, -1, "Raport Status: %s", (raportStatus == 1) ? ""COLOR_SV_HEX"completed" : ""COLOR_SV_HEX"incompleted");
	
    new year, month, day, hour, minute, second;
    ConvertTimestampToDate(playerInfo[playerid][pRaportTime], year, month, day, hour, minute, second);

    if(playerInfo[playerid][pGroupRank] == 7)
		SendClientMessage(playerid, -1, "Your raport will be verified on: "COLOR_SV_HEX"never.");
	else
		SendClientMessage(playerid, -1, "Raport will be verified on: "COLOR_SV_HEX"%02d-%02d-%d{ffffff}, Your rank-up in "COLOR_SV_HEX"%d day(s)", day, month, year, rank_up_days[playerInfo[playerid][pGroupRank] - 1] - getDaysFromTimestamp(playerInfo[playerid][pGroupDays]));
	return true;
}

returnPlayerMissions(returnGroup, missionType)
{
	new returnMessage[32];
	if(factionType[returnGroup] == FACTION_TYPE_POLICE)
	{
		switch(missionType)
		{
			case 0: returnMessage = "Runners";
			case 1: returnMessage = "Arrested players";
			case 2: returnMessage = "Tickets gaved:";
			case 3: returnMessage = "Drugs confiscated:";
			case 4: returnMessage = "Licences confiscated:";
		}
	}
	else
	{
		switch(returnGroup)
		{
			case 4, 5, 6, 10, 15, 16: returnMessage = "Deposited materials";
			case 7, 13: returnMessage = "Transported players";
			case 9: returnMessage = "Anunturi plasate";
			case 11: returnMessage = "Contracte efectuate";
			case 12: returnMessage = "Licente vandute";
			case 14: returnMessage = "Healed players";
		}
	}
	return returnMessage;
}

updateRaportPoints(playerid)
{
	new updateString[128];
	mysql_format(SQL, updateString, sizeof updateString, "update `users` set `RaportScore` = '%d %d %d %d %d %d' where `id` = '%d';", playerInfo[playerid][pRaportScore][0], playerInfo[playerid][pRaportScore][1], playerInfo[playerid][pRaportScore][2], playerInfo[playerid][pRaportScore][3], playerInfo[playerid][pRaportScore][4], playerInfo[playerid][pRaportScore][5], playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, updateString, "", "");
	return true;
}

insertRaportPoint(playerid, raportType = 0)
{
	if(!playerInfo[playerid][pGroup])
		return true;
	
	playerInfo[playerid][pRaportScore][raportType] ++;
	va_SendClientMessage(playerid, COLOR_DCHAT, "%s %d", returnPlayerMissions(playerInfo[playerid][pGroup], raportType), playerInfo[playerid][pRaportScore][raportType]);

	return updateRaportPoints(playerid);
}

YCMD:checkdays(playerid, params[], help)
{
	if(!playerInfo[playerid][pGroup])
		return true;

	va_SendClientMessage(playerid, -1, "You have %d days in this faction.", getDaysFromTimestamp(playerInfo[playerid][pGroupDays]));
	return true;
}

YCMD:leaders(playerid, params[], help)
{
	check_queries

	if(GetPVarInt(playerid, "leaders_delay"))
		return va_SendClientMessage(playerid, -1, "You can use this command in %d seconds.", GetPVarInt(playerid, "leaders_delay") - gettime());

	mysql_tquery(SQL, "select `Name`, `Faction`, `lastOnlineTime` from `users` where `Faction` > 0 and `FactionRank` = '7';", "show_leaders", "i", playerid);
	return SetPVarInt(playerid, "leaders_delay", gettime() + 50);
}

function:show_leaders(playerid)
{
	SendClientMessage(playerid, COLOR_RED2, "{408080}-------- Server Leaders ------------------------");

	for(new i; i < cache_num_rows(); i++)
	{
		static returnGroup, onlineStatus, returnPlayerName[MAX_PLAYERS];

		cache_get_value_name_int(i, "Faction", returnGroup);
 		cache_get_value_name_int(i, "lastOnlineTime", onlineStatus);

        cache_get_value_name(i, "Name", returnPlayerName);

        va_SendClientMessage(playerid, -1, "%s - %s - %s", factionName(returnGroup), returnPlayerName, (onlineStatus ? ("{FF0000}OFFLINE") : ("{33AA33}ONLINE")));
	}

	return sendError(playerid,"{408080}------------------------------------------------");
}

YCMD:members(playerid, params[], help)
{
	check_queries
	check_group
	mysql_format(SQL, returnString, sizeof returnString, "select * from `users` where `Faction` = '%d' order by `FactionRank` desc;", playerInfo[playerid][pGroup]);
	mysql_tquery(SQL, returnString, "showGroupMembers", "i", playerid);
	return true;
}

YCMD:changerank(playerid, params[], help)
{
	if(playerInfo[playerid][pGroupRank] < 7)
		return true;

	new returnPlayer, returnRank;
	if(sscanf(params, "ii", returnPlayer, returnRank))
		return sendSyntaxMessage(playerid, "/changerank [playername/playerid] [rank (1-6)]");

	if(returnRank > 6)
		return sendSyntaxMessage(playerid, "/changerank [playername/playerid] [rank (1-6)]");

	if(!IsPlayerConnected(returnPlayer))
		return sendError(playerid,"Invalid player.");

	va_SendClientMessage(playerid, COLOR_LIGHTBLUE, "You have promoted %s to rank %d.", GetName(returnPlayer), returnRank);
	va_SendClientMessage(returnPlayer, COLOR_LIGHTBLUE, "%s has promoted you to rank %d.", GetName(playerid), returnRank);

	playerInfo[returnPlayer][pGroupRank] = returnRank;
	sendGroupMessage(playerInfo[returnPlayer][pGroup], COLOR_GENANNOUNCE, "%s changed %s's faction rank to %d.", GetName(playerid), GetName(returnPlayer), returnRank);

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `FactionRank` = '%d' where `id` = '%d';", returnRank, playerInfo[returnPlayer][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

YCMD:showmotd(playerid, params[], help)
{    
    check_group
	return sendSplitMessage(playerid, COLOR_GENANNOUNCE, "Group MOTD: {FFFFFF}%s", groupInfo[playerInfo[playerid][pGroup]][groupMotd]);
}

YCMD:gmotd(playerid, params[], help)
{
    check_group

    if(playerInfo[playerid][pGroupRank] < 6)
        return true;

    new motdString[128], motdStringCMD[128];
    if(sscanf(params, "s[128]", motdStringCMD))
        return sendSyntaxMessage(playerid, "/gmotd [message]");
    
    strcpy(motdString, motdStringCMD);
    strmid(groupInfo[playerInfo[playerid][pGroup]][groupMotd], motdString, 0, strlen(motdString), 255);
    
    va_SendClientMessage(playerid, -1, "You have changed the group MOTD to %s.", groupInfo[playerInfo[playerid][pGroup]][groupMotd]);
    SendClientMessage(playerid, -1, "{58FAAC}Foloseste /gmotd doar pentru anunturi importante. Folosirea /gmotd pentru glume, spam va fi sanctionata.");

    sendGroupMessage(playerInfo[playerid][pGroup], COLOR_GENANNOUNCE, "%s has changed the group MOTD to '%s'.", GetName(playerid), groupInfo[playerInfo[playerid][pGroup]][groupMotd]);

    mysql_format(SQL, returnString, sizeof returnString, "update `factions` set `Motd` = '%s' where `ID` = '%d';", groupInfo[playerInfo[playerid][pGroup]][groupMotd], playerInfo[playerid][pGroup]);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

new uninviteName[MAX_PLAYERS][MAX_PLAYER_NAME][MAX_GROUP_SLOTS + 1], uninvitePlayer[MAX_PLAYERS][MAX_GROUP_SLOTS + 1], MemberSelect[MAX_PLAYERS],
	uninviteDays[MAX_PLAYERS][MAX_GROUP_SLOTS + 1], uninviteRank[MAX_PLAYERS][MAX_GROUP_SLOTS + 1], groupFW[MAX_PLAYERS][MAX_GROUP_SLOTS + 1];

ResetMembersVar(playerid)
{
	DeletePVar(playerid, "uninvite_type");
	uninviteName[playerid][MemberSelect[playerid]] = "";
	uninvitePlayer[playerid][MemberSelect[playerid]] = 0;
	
	uninviteDays[playerid][MemberSelect[playerid]] = 0;
	uninviteRank[playerid][MemberSelect[playerid]] = 0;

	MemberSelect[playerid] = 0;
}

function:showGroupMembers(playerid)
{
	returnString = "Player\tRank - FW\tLast login\tFaction Days\n";

	new onlineStatus, dateFormat[6];
	for(new i; i < cache_num_rows(); i++)
	{
		cache_get_value_name(i, "Name", uninviteName[playerid][i]);

		cache_get_value_name_int(i, "ID", uninvitePlayer[playerid][i]);

		cache_get_value_name_int(i, "FactionRank", uninviteRank[playerid][i]);
		cache_get_value_name_int(i, "FactionDays", uninviteDays[playerid][i]);

		cache_get_value_name_int(i, "FactionWarns", groupFW[playerid][i]);
		cache_get_value_name_int(i, "lastOnlineTime", onlineStatus);

		returnDateFromTimestamp(onlineStatus, dateFormat[0], dateFormat[1], dateFormat[2], dateFormat[3], dateFormat[4], dateFormat[5], 2);
		format(returnString, sizeof returnString, "%s%s\t%d [%d/3]\t%s\t%d\n", returnString, uninviteName[playerid][i], uninviteRank[playerid][i], groupFW[playerid][i], (onlineStatus ? va_return("%02d-%02d-%02d %02d:%02d:%02d", dateFormat[0], dateFormat[1], dateFormat[2], dateFormat[3], dateFormat[4], dateFormat[5]) : ("online right now")), getDaysFromTimestamp(uninviteDays[playerid][i]));
	}

	if(playerInfo[playerid][pGroupRank] < 6)
		Dialog_ShowPages(playerid, 0, DIALOG_STYLE_TABLIST_HEADERS, "Faction members", returnString, "Exit", "");

	else
		Dialog_ShowPages(playerid, DIALOG_GROUpGroupS, DIALOG_STYLE_TABLIST_HEADERS, "Faction members", returnString, "Select", "Exit");
	return true;
}

Dialog:DIALOG_GROUpGroupS(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	new formatCaption[32];
	format(formatCaption, sizeof formatCaption, "%d - %s", uninviteRank[playerid][listitem], uninviteName[playerid][listitem]);

	Dialog_Show(playerid, DIALOG_MEMBERSELECT, DIALOG_STYLE_LIST, formatCaption, "Change Rank\nFaction Warn\nClear FW\nUninvite - 40 FP\nUninvite - 0 FP", "OK", "Exit");
	MemberSelect[playerid] = listitem;
	return true;
}

Dialog:DIALOG_MEMBERSELECT(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	if(playerInfo[playerid][pGroupRank] < 6)
		return sendError(playerid,"Numai liderul factiunii are acces la aceasta categorie.");

	if(playerInfo[playerid][pSQLID] == uninvitePlayer[playerid][MemberSelect[playerid]])
		return sendError(playerid,"Nu iti poti face singur modificari.");

	switch(listitem)
	{
		case 0: return sendSyntaxMessage(playerid, "/changerank [playername/playerid] [rank (1-6)]");
		case 1:
		{
			if(playerInfo[playerid][pGroupRank] >= 6 && uninviteRank[playerid][MemberSelect[playerid]] == 6)
				return sendError(playerid,"Nu-i poti da FW unui membru de rank 6-7.");
				
			Dialog_Show(playerid, DIALOG_GIVEFW, DIALOG_STYLE_MSGBOX, "Esti sigur?", "Esti sigur ca vrei sa-i dai FW acelui membru?", "Da", "Nu");
		}
		case 2:
		{
			if(playerInfo[playerid][pGroupRank] >= 6 && uninviteRank[playerid][MemberSelect[playerid]] == 6)
				return sendError(playerid,"Nu-i poti da FW clear unui membru de rank 6-7.");
				
			Dialog_Show(playerid, DIALOG_CLEARFW, DIALOG_STYLE_MSGBOX, "Esti sigur?", "Esti sigur ca vrei sa-i stergi FW acelui membru?", "Da", "Nu");
		}
		case 3: SetPVarInt(playerid, "uninvite_type", 1);
		case 4: SetPVarInt(playerid, "uninvite_type", 2);
	}

    if(GetPVarInt(playerid, "uninvite_type") > 0) {
        new formatCaption[32];
        format(formatCaption, sizeof formatCaption, "Uninvite - %d FP", (GetPVarInt(playerid, "uninvite_type") == 1 ? 40 : 0));

        format(returnString, sizeof returnString, "Atentie!!! Playerii ce au peste 14 zile se vor da afara fara FP, cu exceptia cazurilor in care acesta incalca regulile factiunii.\n\
            Esti pe cale sa-i dai uninvite lui %s %s.\nAcesta are %d zile in factiune.\nScrie motivul pentru uninvite mai jos:", uninviteName[playerid][MemberSelect[playerid]], (GetPVarInt(playerid, "uninvite_type") == 1 ? ("cu 40 fps") : ("fara FP")), getDaysFromTimestamp(uninviteDays[playerid][MemberSelect[playerid]]));																				
        Dialog_Show(playerid, DIALOG_MEMBER_UNINVITE, DIALOG_STYLE_INPUT, formatCaption, returnString, "Uninvite", "Exit");
    }
	return true;
}

Dialog:DIALOG_GIVEFW(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	if(groupFW[playerid][MemberSelect[playerid]] == 3)
		return uninviteOfflinePlayer(uninvitePlayer[playerid][MemberSelect[playerid]], 40);

	if(!IsPlayerConnected(MemberSelect[playerid])){
		groupFW[playerid][MemberSelect[playerid]] += 1;
		mysql_format(SQL, returnString, sizeof returnString, "update `users` set `FactionWarns` = '%d' where `ID` = '%d';", groupFW[playerid][MemberSelect[playerid]], uninvitePlayer[playerid][MemberSelect[playerid]]);
		mysql_tquery(SQL, returnString, "", "");
		SendClientMessage(playerid, -1, "Jucatorul a fost sanctionat cu Faction Warn! (Offline)");
		insertPlayerMail(uninvitePlayer[playerid][MemberSelect[playerid]], gettime(), "FW: %s ti-a dat un FW (faction warn). Acum ai %d/3 FW.", GetName(playerid), groupFW[playerid][MemberSelect[playerid]] + 1);
	}
	
	SendClientMessage(playerid, -1, "Jucatorul a primit clear Faction Warn!");
	foreach(new i : factionMembers<playerInfo[playerid][pGroup]>)
	{
		if(playerInfo[i][pSQLID] != uninvitePlayer[playerid][MemberSelect[playerid]])
			continue;

		playerInfo[i][pGroupWarns] ++;
		va_SendClientMessage(i, COLOR_GENANNOUNCE, "%s ti-a dat un FW (faction warn).", GetName(playerid));
		UpdatePVars(i, "FactionWarns", playerInfo[i][pGroupWarns]);
		insertPlayerMail(uninvitePlayer[playerid][MemberSelect[playerid]], gettime(), "FW: %s ti-a dat un FW (faction warn). Acum ai %d/3 FW.", GetName(playerid), groupFW[playerid][MemberSelect[playerid]] + 1);
		break;
	}
	ResetMembersVar(playerid);
	return true;
}
Dialog:DIALOG_CLEARFW(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `FactionWarns` = '0' where `ID` = '%d';", uninvitePlayer[playerid][MemberSelect[playerid]]);
	mysql_tquery(SQL, returnString, "", "");

	SendClientMessage(playerid, -1, "Jucatorul a primit clear Faction Warn!");
	
	foreach(new i : factionMembers<playerInfo[playerid][pGroup]>)
	{
		if(playerInfo[i][pSQLID] != uninvitePlayer[playerid][MemberSelect[playerid]])
			continue;

		va_SendClientMessage(i, COLOR_GENANNOUNCE, "%s ti-a sters un FW (faction warn).", GetName(playerid));
		break;
	}
	insertPlayerMail(uninvitePlayer[playerid][MemberSelect[playerid]], gettime(), "FW: %s ti-a sters un FW (faction warn). Acum ai 0/3 FW.", GetName(playerid));

	ResetMembersVar(playerid);
	return true;
}
function:uninviteOfflinePlayer(returnPlayer, returnPunish) {
	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Faction` = '0', `FactionRank` = '0', `FactionDays` = '0', `FactionPunish` = '%d' where `ID` = '%d';", returnPunish, returnPlayer);
	return mysql_tquery(SQL, returnString, "", "");
}
Dialog:DIALOG_MEMBER_UNINVITE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	new returnDialoreturnString[256], returnPunish = GetPVarInt(playerid, "uninvite_type") == 1 ? 40 : 0;

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Faction` = '0', `FactionRank` = '0', `FactionDays` = '0', `FactionPunish` = '%d' where `ID` = '%d';", returnPunish, uninvitePlayer[playerid][MemberSelect[playerid]]);
	mysql_tquery(SQL, returnString, "", "");

	sendGroupMessage(playerInfo[playerid][pGroup], COLOR_GENANNOUNCE, "%s was uninvited by %s from faction %s (rank %d) after %d days, with %d FP. Reason: %s.",
		uninviteName[playerid][MemberSelect[playerid]], playerInfo[playerid][pName], factionName(playerInfo[playerid][pGroup]), uninviteRank[playerid][MemberSelect[playerid]], getDaysFromTimestamp(uninviteDays[playerid][MemberSelect[playerid]]), returnPunish, inputtext);

	foreach(new i : factionMembers<playerInfo[playerid][pGroup]>)
	{
		if(playerInfo[i][pSQLID] != uninvitePlayer[playerid][MemberSelect[playerid]])
			continue;

		format(returnDialoreturnString, sizeof returnDialoreturnString, "Ai fost demis de liderul %s\n din factiunea din care faceai parte %s (rank %d)\n dupa %d days, cu %d FP. Motiv: %s.", playerInfo[playerid][pName], factionName(playerInfo[i][pGroup]), playerInfo[i][pGroupRank], getDaysFromTimestamp(playerInfo[i][pGroupDays]), returnPunish, inputtext);
		Dialog_Show(i, 0, DIALOG_STYLE_MSGBOX, "Info", returnDialoreturnString, "Ok", "");
		
		insertPlayerMail(uninvitePlayer[playerid][MemberSelect[playerid]], gettime(), "Ai fost demis de liderul %s din factiunea din care faceai parte %s (rank %d) dupa %d zile, cu %d FP. Motiv: %s.", playerInfo[playerid][pName], factionName(playerInfo[i][pGroup]), playerInfo[i][pGroupRank], getDaysFromTimestamp(playerInfo[i][pGroupDays]), returnPunish, inputtext);
		whenPlayerGotUninvited(i, returnPunish);
		break;
	}

	mysql_format(SQL, returnString, sizeof returnString, "insert into `faction_history` (`player`, `actionby`, `message`) values ('%d', '%d', '%s was uninvited by Admin %s from faction %s (rank %d) after %d days, with %d FP. Reason: %s.');", uninvitePlayer[playerid][MemberSelect[playerid]], playerInfo[playerid][pSQLID], uninviteName[playerid][MemberSelect[playerid]], playerInfo[playerid][pName], factionName(playerInfo[playerid][pGroup]), uninviteRank[playerid][MemberSelect[playerid]], getDaysFromTimestamp(uninviteDays[playerid][MemberSelect[playerid]]), returnPunish, inputtext);
	mysql_tquery(SQL, returnString, "", "");

	ResetMembersVar(playerid);
	return true;
}

hook WhenPlayerLoggedIn(playerid)
{
	if(!cache_num_rows())
		return Y_HOOKS_CONTINUE_RETURN_1;

 	cache_get_value_name_int(0, "Faction", playerInfo[playerid][pGroup]);
 	cache_get_value_name_int(0, "FactionRank", playerInfo[playerid][pGroupRank]);
	
	cache_get_value_name_int(0, "FactionDays", playerInfo[playerid][pGroupDays]);
   	cache_get_value_name_int(0, "FactionWarns", playerInfo[playerid][pGroupWarns]);
	
	cache_get_value_name_int(0, "FactionPunish", playerInfo[playerid][pGroupPunish]);

	new loadString[64];
	cache_get_value_name(0, "RaportScore", loadString);
    sscanf(loadString, "a<i>[6]", playerInfo[playerid][pRaportScore]);

	if(playerInfo[playerid][pGroupRank] == 7)
		Iter_Add(serverLeaders, playerid);

	return Y_HOOKS_CONTINUE_RETURN_1;
}

new Float:depositPos[][] =
{
	{0.0, 0.0, 0.0, 90.0},
	{256.5857, 64.6291, 1003.6406},
	{229.2727, 168.2897, 1003.0234},
	{259.4095, 107.6587, 1003.2188},
	{2548.7173, -1282.0735, 1060.9844},
	{2548.7173, -1282.0735, 1060.9844},
	{2548.7173, -1282.0735, 1060.9844},
	{1712.2660, -1640.1022, 20.2239}, ///7
	{256.5857, 64.6291, 1003.6406},
	{1712.2660, -1640.1022, 20.2239},
	{2548.7173, -1282.0735, 1060.9844},
	{301.1471, 187.5749, 1007.1719},
	{1712.2660, -1640.1022, 20.2239},
	{1712.2660, -1640.1022, 20.2239},
	{1712.2660, -1640.1022, 20.2239},
	{2548.7173, -1282.0735, 1060.9844},
	{2548.7173, -1282.0735, 1060.9844}
};

function:updategroupInformations(factionid)
{
	new labelString[256];
	format(labelString, sizeof labelString, ""COLOR_SV_HEX"Faction:{ffffff} %d\n"COLOR_SV_HEX"Name: {ffffff}%s\n"COLOR_SV_HEX"HQ Status: {ffffff}%s", factionid, groupInfo[factionid][fName], (groupInfo[factionid][factionLocked] == 0 ? ("unlocked") : ("(locked)")));
	groupInfo[factionid][factionLabel] = CreateDynamic3DTextLabel(labelString, -1, groupInfo[factionid][factionExterior][0], groupInfo[factionid][factionExterior][1], groupInfo[factionid][factionExterior][2], 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, false);

	groupInfo[factionid][factionPickup] = CreateDynamicPickup(1314, 23, groupInfo[factionid][factionExterior][0], groupInfo[factionid][factionExterior][1], groupInfo[factionid][factionExterior][2], 0, 0, -1, 60);

	if(factionid == 1 || factionid == 2 || factionid == 3 || factionid == 8)
		groupInfo[factionid][factionPickup] = CreateDynamicMapIcon(groupInfo[factionid][factionExterior][0], groupInfo[factionid][factionExterior][1], groupInfo[factionid][factionExterior][2], 30, 0, -1, -1, -1, 200.0, MAPICON_GLOBAL);
	
	groupInfo[factionid][factionSphere] = CreateDynamicSphere(groupInfo[factionid][factionExterior][0], groupInfo[factionid][factionExterior][1], groupInfo[factionid][factionExterior][2], 1.5, 0, 0, -1);
	Streamer_SetIntData(STREAMER_TYPE_AREA, groupInfo[factionid][factionSphere], E_STREAMER_EXTRA_ID, (factionid + STREAMER_BEGIN_FACTIONS));

	format(labelString, sizeof labelString, "{ffff99}%s\nGroup Safe", groupInfo[factionid][fName]);
	CreateDynamic3DTextLabel(labelString, -1, depositPos[factionid][0], depositPos[factionid][1], depositPos[factionid][2], 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, false, factionid);
	return true;
}

function:loadFactions()
{
	for(new i = 1; i <= cache_num_rows(); i++)
	{
		cache_get_value_name(i - 1, "Name", groupInfo[i][fName]);
		cache_get_value_name(i - 1, "Motd", groupInfo[i][groupMotd]);

		cache_get_value_name_int(i - 1, "ID", groupInfo[i][factionID]);
		cache_get_value_name_int(i - 1, "Locked", groupInfo[i][factionLocked]);

		cache_get_value_name_int(i - 1, "Slots", groupInfo[i][factionSlots]);

		cache_get_value_name_float(i - 1, "ExteriorX", groupInfo[i][factionExterior][0]);
		cache_get_value_name_float(i - 1, "ExteriorY", groupInfo[i][factionExterior][1]);
		cache_get_value_name_float(i - 1, "ExteriorZ", groupInfo[i][factionExterior][2]);

		cache_get_value_name_int(i - 1, "Money", groupInfo[i][safeMoney]);
		cache_get_value_name_int(i - 1, "Materials", groupInfo[i][safeMaterials]);
		cache_get_value_name_int(i - 1, "Drugs", groupInfo[i][safeDrugs]);

		cache_get_value_name(i - 1, "Rank1", groupInfo[i][fRankName1]);
		cache_get_value_name(i - 1, "Rank2", groupInfo[i][fRankName2]);
		cache_get_value_name(i - 1, "Rank3", groupInfo[i][fRankName3]);
		cache_get_value_name(i - 1, "Rank4", groupInfo[i][fRankName4]);
		cache_get_value_name(i - 1, "Rank5", groupInfo[i][fRankName5]);
		cache_get_value_name(i - 1, "Rank6", groupInfo[i][fRankName6]);
		cache_get_value_name(i - 1, "Rank7", groupInfo[i][fRankName7]);

		new loadString[128];
		for(new rank; rank <= 5; rank ++)
		{
			cache_get_value_name(i - 1, va_return("Raport%d", rank), loadString);
		    sscanf(loadString, "a<i>[6]", raportPoints[i][rank]);
		}
		updategroupInformations(i);
		serverFactions ++;
	}
	printf("Factions: %d", serverFactions);
	return true;
}

stock GetFactionRankName(playerid)
{
    new rankName[32],
        factionId = playerInfo[playerid][pGroup];

    switch(playerInfo[playerid][pGroupRank])
    {
        case 1: format(rankName, 32, groupInfo[factionId][fRankName1]);
        case 2: format(rankName, 32, groupInfo[factionId][fRankName2]);
        case 3: format(rankName, 32, groupInfo[factionId][fRankName3]);
        case 4: format(rankName, 32, groupInfo[factionId][fRankName4]);
        case 5: format(rankName, 32, groupInfo[factionId][fRankName5]);
        case 6: format(rankName, 32, groupInfo[factionId][fRankName6]);
        case 7: format(rankName, 32, groupInfo[factionId][fRankName7]);
    }
    return rankName;
}

function:returnfactionMembers(groupId)
{
	new returnQuery[256];
	mysql_format(SQL, returnQuery, sizeof returnQuery, "select count(`Faction`) as total_members from `users` where `Faction` = '%d' limit 20;", groupId);

	new Cache: cacheQuery = mysql_query(SQL, returnQuery), returnMembers;

 	cache_get_value_name_int(0, "total_members", returnMembers);
	cache_delete(cacheQuery);
	return returnMembers;
}

YCMD:factions(playerid, params[], help)
{
	returnString = "{ffffff}Nume\t{ffffff}Membri online\t{ffffff}Sloturi disponibile\n";
	for(new i = 1; i <= serverFactions; i++)
	{
		format(returnString, sizeof returnString, "%s%s\t%d\t%d\n", returnString, factionName(i), Iter_Count(factionMembers<i>), groupInfo[i][factionSlots]);
	}
	Dialog_Show(playerid, 0, DIALOG_STYLE_TABLIST_HEADERS, "Factiuni", returnString, "Ok", "");
	return true;
}

sendGroupMessage(group, color, const string[], va_args<>)
{
	new returnGroupString[256];
	va_format(returnGroupString, sizeof returnGroupString, string, va_start<3>);

	foreach(new i : factionMembers<group>)
	{
		if(!playerStaticVar[i][playerLogged])
			continue;

		sendSplitMessage(i, color, returnGroupString);
	}

	foreach(new i : serverAdmins)
	{
		if(FactionSpec[i] == group)
		{
			sendSplitMessage(i, 0xCC4661FF, returnGroupString);
		}
	}
	return true;
}

new Float:factionInterior[MAX_FACTIONS][4] =
{
	{0.0, 0.0, 0.0, 90.0},
	{246.869, 62.8318, 1003.64, 360.0},
	{238.6508, 138.6999, 1003.0234, 360.0},
	{246.5247, 107.2960, 1003.2188, 0.0},
	{2545.6001,-1305.1400,1054.6406, 0.0},
	{2545.6001,-1305.1400,1054.6406, 0.0},
	{2545.6001,-1305.1400,1054.6406, 0.0},
	{1726.9320, -1637.9402, 20.2230, 360.0},
	{246.869, 62.8318, 1003.64, 360.0},
	{1726.9320, -1637.9402, 20.2230, 360.0},
	{2545.6001,-1305.1400,1054.6406, 0.0},
	{288.8077, 166.9345, 1007.1719, 360.0},
	{1726.9320, -1637.9402, 20.2230, 360.0},
	{1726.9320, -1637.9402, 20.2230, 360.0},
	{1726.9320, -1637.9402, 20.2230, 360.0},
	{2545.6001,-1305.1400,1054.6406, 0.0},
	{2545.6001,-1305.1400,1054.6406, 0.0}
};
new getHQInterior[] =
{
	0, 6, 3, 10, 2, 2, 2, 18, 6, 18, 2, 3, 18, 18, 18, 2, 2
};

hook WhenPlayerSpawn(playerid)
{

	if(factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_POLICE && playerStaticVar[playerid][groupDuty] && player_data[playerid][playerDeath] && factionWar[playerInfo[playerid][pGroup]] == -1)
	{
		playerStaticVar[playerid][groupDuty] = false;
		SendClientMessage(playerid, -1, "Deoarece ai murit ai fost scos de la datorie. (off-duty)");
	}


	if(playerInfo[playerid][pSpawnLocation] == 0 && playerInfo[playerid][pGroup])
	{
		new i = playerInfo[playerid][pGroup];

		SetPlayerPos(playerid, factionInterior[i][0], factionInterior[i][1], factionInterior[i][2]);
		SetPlayerFacingAngle(playerid, factionInterior[i][3]);
		playerStaticVar[playerid][inHQ] = i;
		SetPlayerVirtualWorld(playerid, playerStaticVar[playerid][inHQ]);
		SetPlayerInterior(playerid, getHQInterior[i]);

		if(playerStaticVar[playerid][groupDuty] == true)
			SetPlayerSkin(playerid, faction_skins[i][playerInfo[playerid][pGroupRank] - 1]);

		return Y_HOOKS_CONTINUE_RETURN_0;
	}
	player_data[playerid][playerDeath] = 0;
	return Y_HOOKS_CONTINUE_RETURN_1;
}

Dialog:DIALOG_BREAKHQ(playerid, response, listitem, inputtext[])
{
	new playerGroup = GetPVarInt(playerid, "areaID_factions");

	if(!response || !playerGroup)
		return true;

	SetPlayerPos(playerid, factionInterior[playerGroup][0], factionInterior[playerGroup][1], factionInterior[playerGroup][2]);
	SetPlayerFacingAngle(playerid, factionInterior[playerGroup][3]);

	SetPlayerVirtualWorld(playerid, playerGroup);
	SetPlayerInterior(playerid, getHQInterior[playerGroup]);

	playerStaticVar[playerid][inHQ] = playerGroup;
	return true;
}


Dialog:DIALOG_QUITGROUP(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	sendGroupMessage(playerInfo[playerid][pGroup], COLOR_GENANNOUNCE, "%s has left the group %s (rank %d) after %d days, with 40 FP, using /quitgroup.", GetName(playerid), factionName(playerInfo[playerid][pGroup]), playerInfo[playerid][pGroupRank], getDaysFromTimestamp(playerInfo[playerid][pGroupDays]));
	va_SendClientMessage(playerid, COLOR_GREY, "You left the group %s (rank %d) after %d days, with 40 FP, using /quitgroup.", factionName(playerInfo[playerid][pGroup]), playerInfo[playerid][pGroupRank], getDaysFromTimestamp(playerInfo[playerid][pGroupDays]));
	
	insertPlayerMail(playerInfo[playerid][pSQLID], gettime(), "Ai parasit factiunea din care faceai parte %s (rank %d) dupa %d zile. Ai primit 40 FP.", factionName(playerInfo[playerid][pGroup]), playerInfo[playerid][pGroupRank], getDaysFromTimestamp(playerInfo[playerid][pGroupDays]));
	
	whenPlayerGotUninvited(playerid, 40);

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Faction` = '0', `FactionDays` = '0', `FactionRank` = '0', `FactionWarns` = '0', `FactionPunish` = '40' where `ID` = '%d'", playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

YCMD:quitgroup(playerid, params[], help)
{
	if(playerInfo[playerid][pGroup] == 0)
		return true;
	if(playerInfo[playerid][pGroupRank] == 7)
		return true;

	Dialog_Show(playerid, DIALOG_QUITGROUP, DIALOG_STYLE_MSGBOX, "/quitgroup", "Are you sure if you want to leave the group?\nYou will get 40 FP", "Yes", "No");
	return true;
}

YCMD:l(playerid, params[], help)
{
    if(playerInfo[playerid][pGroupRank] != 7 && !playerInfo[playerid][pAdmin])
        return returnNoAcces(playerid);

    new returnMessage[144];
    if(sscanf(params, "s[144]", returnMessage))
        return sendSyntaxMessage(playerid, "/l <Message>");

    if(FindIP(returnMessage)) /// Anti-Reclama
		sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s(%d) typed %s in /l.", GetName(playerid), playerid, returnMessage);

    foreach(new i : serverAdmins)
    {
		if(!playerStaticVar[i][playerLogged])
			continue;

		sendSplitMessage(i, COLOR_CYAN, "(/l) %s %s: %s", (playerInfo[playerid][pAdmin] ? va_return("(%d) Admin", playerInfo[playerid][pAdmin]) : va_return("Leader %s", factionName(playerInfo[playerid][pGroup]))), GetName(playerid), returnMessage);
	}
	foreach(new i : serverLeaders)
	{
		if(!playerStaticVar[i][playerLogged] || playerInfo[i][pAdmin])
			continue;

		sendSplitMessage(i, COLOR_CYAN, "(/l) %s %s: %s", (playerInfo[playerid][pAdmin] ? va_return("(%d) Admin", playerInfo[playerid][pAdmin]) : va_return("Leader %s", factionName(playerInfo[playerid][pGroup]))), GetName(playerid), returnMessage);
	}
    return true;
}

YCMD:setleader(playerid, params[], help)
{
	if(playerInfo[playerid][pAdmin] < 3)
		return returnNoAcces(playerid);

	new returnPlayer, returnGroup;
	if(sscanf(params, "ui", returnPlayer, returnGroup))
		return sendSyntaxMessage(playerid, "/setleader <Name/Playerid> <Number(1-20)>");

	if(returnGroup < 1 || returnGroup > MAX_FACTIONS)
		return sendError(playerid,"Invalid faction.");

	if(returnPlayer == INVALID_PLAYER_ID || !IsPlayerConnected(returnPlayer))
		return sendError(playerid,"Invalid player.");

	if(playerInfo[returnPlayer][pGroup] != 0)
		return sendError(playerid,"This player is already in a group.");

	format(returnString, sizeof returnString, "Esti sigur ca doresti sa-l setezi pe %s ca lider al factiunii %s", GetName(returnPlayer), factionName(returnGroup));
	Dialog_Show(playerid, DIALOG_SETLEADER, DIALOG_STYLE_MSGBOX, "/setleader", returnString, "Da", "Nu");
	
	SetPVarInt(playerid, "set_leader_player", returnPlayer),
	SetPVarInt(playerid, "set_leader_faction", returnGroup);
	return true;
}

YCMD:auninvite(playerid, params[], help)
{
	if(playerInfo[playerid][pAdmin] < 3)
		returnNoAcces(playerid);

	new targetName[MAX_PLAYER_NAME], punishAmount, stringReason[64];

	if(sscanf(params, "s[32]ds[64]", targetName, punishAmount, stringReason))
		return sendSyntaxMessage(playerid, "/auninvite <Name> <FPunish> <Reason>");

	mysql_format(SQL, returnString, sizeof returnString, "select * from `users` where `Name` = '%e';", targetName);
	mysql_tquery(SQL, returnString, "auninvitePlayer", "ids", playerid, punishAmount, stringReason);
	return true;
}
function:auninvitePlayer(playerid, punishAmount, stringReason[])
{
	if(!cache_num_rows())
		return sendError(playerid,"Acest jucator nu exista.");

	new targetName[MAX_PLAYER_NAME];
	cache_get_value_name(0, "Name", targetName);

	SetPVarInt(playerid, "au_punishAmount", punishAmount);

	static playerKey, playerFactionRank, playerFaction, playerFactionDays;
	cache_get_value_name_int(0, "ID", playerKey);
	cache_get_value_name_int(0, "FactionRank", playerFactionRank);
 	cache_get_value_name_int(0, "Faction", playerFaction);
	cache_get_value_name_int(0, "FactionDays", playerFactionDays);

	SetPVarInt(playerid, "au_targetId", playerKey);
	
	SetPVarString(playerid, "au_targetPlayer", targetName);
	SetPVarString(playerid, "au_uninvReason", stringReason);

	SetPVarInt(playerid, "au_targetRank", playerFactionRank);
	SetPVarInt(playerid, "au_targetMember", playerFaction);
	SetPVarInt(playerid, "au_targetFDays", playerFactionDays);

	if(!playerFaction)
		return sendError(playerid,"Acest jucator nu face parte dintr-o factiune.");
	
	format(returnString, sizeof returnString, "Esti sigur ca doresti sa-l demiti pe %s din factiunea sa cu %d FP pe motiv: %s", targetName, punishAmount, stringReason);
	Dialog_Show(playerid, DIALOG_AUNINVITE, DIALOG_STYLE_MSGBOX, "Confirm:", returnString, "Da", "Nu");
	return true;
}
Dialog:DIALOG_AUNINVITE(playerid, response, listitem, inputtext[])
{
	if(!response)
	{
		DeletePVar(playerid, "au_punishAmount"), DeletePVar(playerid, "au_targetPlayer"),
		DeletePVar(playerid, "au_uninvReason"), DeletePVar(playerid, "au_targetRank"),
		DeletePVar(playerid, "au_targetMember"), DeletePVar(playerid, "au_targetFDays"),
		DeletePVar(playerid, "au_targetId");

		return true;
	}

	new returnReason[64], targetName[MAX_PLAYER_NAME], stringMessage[256],
		punishAmount = GetPVarInt(playerid, "au_punishAmount"), targetRank = GetPVarInt(playerid, "au_targetRank"),
		targetMember = GetPVarInt(playerid, "au_targetMember"), targetFDays = GetPVarInt(playerid, "au_targetFDays"),
		targetId = GetPVarInt(playerid, "au_targetId");

	GetPVarString(playerid, "au_uninvReason", returnReason, sizeof returnReason);
	GetPVarString(playerid, "au_targetPlayer", targetName, MAX_PLAYER_NAME);

	foreach(new i : factionMembers<targetMember>)
	{
		if(playerInfo[i][pSQLID] == targetId)
		{
			va_SendClientMessage(i, COLOR_GREY, "Ai fost demis de catre administratorul %s din factiunea ta cu %d FP.", GetDBName(playerid), punishAmount);

			format(returnString, sizeof returnString, "Ai fost demis de administratorul %s\n din factiunea din care faceai parte %s (rank %d)\n dupa %d days, cu %d FP. Motiv: %s.", GetDBName(playerid), factionName(targetMember), targetRank, getDaysFromTimestamp(targetFDays), punishAmount, returnReason);
			Dialog_Show(i, 0, DIALOG_STYLE_MSGBOX, "Info", returnString, "Ok", "");

			whenPlayerGotUninvited(i, punishAmount);
			SendClientMessage(playerid, -1, "Acest jucator nu este intr-o factiune.");
			break;
		}
	}
	va_SendClientMessage(playerid, COLOR_LIGHTBLUE, "L-ai dat pe %s afara din factiunea sa %s FP.", targetName, (punishAmount ? va_return("cu %d", punishAmount) : "fara"));

	format(stringMessage, sizeof stringMessage, "%s was uninvited by Admin %s from faction %s (rank %d) after %d days, %s FP. Reason: %s.",
		targetName, GetDBName(playerid), factionName(targetMember), targetRank, getDaysFromTimestamp(targetFDays), (punishAmount ? va_return("with %d", punishAmount) : "without"), returnReason);

	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, stringMessage);
	sendGroupMessage(targetMember, COLOR_GENANNOUNCE, stringMessage);

	va_SendClientMessage(playerid, COLOR_GREY, "L-ai demis pe %s din factiunea sa.", targetName);

	mysql_format(SQL, returnString, sizeof returnString, "insert into `faction_history` (`message`, `player`, `actionby`) values ('%s', '%d', '%d')", stringMessage, targetId, playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");

	insertPlayerMail(targetId, gettime(), "Ai fost demis de Admin %s din factiunea din care faceai parte %s (rank %d) dupa %d zile, %s FP. Motiv: %s.", GetDBName(playerid), factionName(targetMember), targetRank, getDaysFromTimestamp(targetFDays), (punishAmount ? va_return("cu %d", punishAmount) : "fara"), returnReason);
	
	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Faction` = '0', `FactionRank` = '0', `FactionDays` = '0', `FactionPunish` = '%d' where `ID` = '%d';", punishAmount, targetId);
	mysql_tquery(SQL, returnString, "", "");

	DeletePVar(playerid, "au_punishAmount"), DeletePVar(playerid, "au_targetPlayer"),
	DeletePVar(playerid, "au_uninvReason"), DeletePVar(playerid, "au_targetRank"),
	DeletePVar(playerid, "au_targetMember"), DeletePVar(playerid, "au_targetFDays"),
	DeletePVar(playerid, "au_targetId");
	return true;
}

whenPlayerGotUninvited(returnPlayer, returnPunish)
{
	Iter_Remove(factionMembers<playerInfo[returnPlayer][pGroup]>, returnPlayer);
	playerInfo[returnPlayer][pGroup] = 0;

	if(factionType[playerInfo[returnPlayer][pGroup]] == FACTION_TYPE_POLICE)
		Iter_Remove(departmentsMembers, returnPlayer);

	if(playerInfo[returnPlayer][pGroupRank] == 7)
		Iter_Remove(serverLeaders, returnPlayer);

	playerInfo[returnPlayer][pGroupRank] = 0;
	playerInfo[returnPlayer][pGroupDays] = 0;
	playerInfo[returnPlayer][pGroupWarns] = 0;

	playerInfo[returnPlayer][pGroupPunish] = returnPunish;

	setPlayerDuty(returnPlayer, false);
	return true;
}

Dialog:DIALOG_SETLEADER(playerid, response, listitem, inputtext[])
{
	if(!response)
	{	
		DeletePVar(playerid, "set_leader_player");
		DeletePVar(playerid, "set_leader_faction");
		return true;
	}

	new playerLeader = GetPVarInt(playerid, "set_leader_player"),
		groupLeader = GetPVarInt(playerid, "set_leader_faction");

	if(!IsPlayerConnected(playerLeader) || playerLeader == INVALID_PLAYER_ID)
		return sendError(playerid,"Invalid player.");

	va_SendClientMessage(playerLeader, COLOR_DARKPINK, "Admin %s has set you to lead group %s", GetName(playerid), groupInfo[GetPVarInt(playerid, "set_leader_faction")][fName]);
	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_DARKPINK, "<< Admin %s has set %s to lead group %s >>", GetName(playerid), GetName(playerLeader), groupInfo[GetPVarInt(playerid, "set_leader_faction")][fName]);

	new returnDialoreturnString[128];
	format(returnDialoreturnString, sizeof returnDialoreturnString, "Admin %s has set you to lead group %s", GetName(playerid), groupInfo[GetPVarInt(playerid, "set_leader_faction")][fName]);
	Dialog_Show(playerLeader, 0, DIALOG_STYLE_MSGBOX, "Info", returnDialoreturnString, "Ok", "");

	playerInfo[playerLeader][pGroup] =  groupLeader;
	Iter_Add(factionMembers<playerInfo[playerLeader][pGroup]>, playerLeader);

	playerInfo[playerLeader][pGroupRank] = 7;
	Iter_Add(serverLeaders, playerLeader);

	playerInfo[playerLeader][pGroupDays] = gettime();

	mysql_format(SQL, returnString, sizeof returnString, "insert into `faction_history` (`player`, `actionby`, `message`) values ('%d', '%d', '%s is now the leader of faction %s (promoted by %s)');",
		playerInfo[playerLeader][pSQLID], playerInfo[playerid][pSQLID], playerInfo[playerLeader][pName], groupInfo[GetPVarInt(playerid, "set_leader_faction")][fName], playerInfo[playerid][pName]);
	mysql_tquery(SQL, returnString, "", "");

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Faction` = '%d', `FactionRank` = '7', `FactionDays` = '%d', `RaportTime` = '%d' where `ID` = '%d';", groupLeader, gettime(), (gettime() + (86400 * 7)), playerInfo[playerLeader][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");
	
	DeletePVar(playerid, "set_leader_player");
	DeletePVar(playerid, "set_leader_faction");
	return true;
}

YCMD:gotohq(playerid, params[], help)
{
	check_admin

	new returnFaction;
	if(sscanf(params, "i", returnFaction))
		return sendSyntaxMessage(playerid, "/gotohq [factionid]");

	if(returnFaction > serverFactions)
		return sendError(playerid,"Invalid faction.");

	va_SendClientMessage(playerid, -1, "You have been teleported to %s's HQ.", factionName(returnFaction));
	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s has used /gotohq %d.", GetName(playerid), returnFaction);

	SetPlayerPos(playerid, groupInfo[returnFaction][factionExterior][0], groupInfo[returnFaction][factionExterior][1], groupInfo[returnFaction][factionExterior][2]);
	SetPlayerInterior(playerid, 0);
	SetPlayerVirtualWorld(playerid, 0);
	return true;
}

YCMD:fdeposit(playerid, params[], help)
{
	check_group

	if(playerStaticVar[playerid][inHQ] != playerInfo[playerid][pGroup])
		return sendError(playerid,"You need to be in group HQ to do this.");
	
	new returnItem[32], returnAmount;
	if(sscanf(params, "s[32]i", returnItem, returnAmount))
	{
		va_SendClientMessage(playerid, COLOR_GREY, "Safe balance: $%d, %d materials, %d drugs", groupInfo[playerInfo[playerid][pGroup]][safeMoney], groupInfo[playerInfo[playerid][pGroup]][safeMaterials], groupInfo[playerInfo[playerid][pGroup]][safeDrugs]);
		return sendSyntaxMessage(playerid, "/f(g)deposit [money, materials or drugs] [amount]");
	}
	if(strmatch(returnItem, "money"))
	{

	    if(GetMoney(playerid) <  returnAmount && GetMilliards(playerid) < 1)
	        return sendError(playerid,"You do not have the money.");


		if(groupInfo[playerInfo[playerid][pGroup]][safeMoney] < 2000000000)
			groupInfo[playerInfo[playerid][pGroup]][safeMoney] += returnAmount;

		substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, returnAmount);
		va_SendClientMessage(playerid, COLOR_GREY, "You've put %d$ in safe.", returnAmount);

		playerInfo[playerid][pRaportScore][5] += returnAmount;
		updateRaportPoints(playerid);
	}
	else if(strmatch(returnItem, "materials"))
	{
		if(playerInfo[playerid][pMaterials] < returnAmount)
			return sendError(playerid,"You don't have that amount of materials.");

		if(groupInfo[playerInfo[playerid][pGroup]][safeMaterials] < 2000000000)
			groupInfo[playerInfo[playerid][pGroup]][safeMaterials] += returnAmount;

		va_SendClientMessage(playerid, COLOR_GREY, "You've put %d materials in safe.", returnAmount);

		if(factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_GANG)
		{
			playerInfo[playerid][pRaportScore][0] += returnAmount;
			updateRaportPoints(playerid);
		}
	}
	else if(strmatch(returnItem, "drugs"))
	{
		if(playerInfo[playerid][pDrugs] < returnAmount)
			return sendError(playerid,"You don't have that amount of drugs.");

		if(groupInfo[playerInfo[playerid][pGroup]][safeDrugs] < 2000000000)
			groupInfo[playerInfo[playerid][pGroup]][safeDrugs] += returnAmount;

		va_SendClientMessage(playerid, COLOR_GREY, "You've put %d drugs in safe.", returnAmount);
	}
	else
		return true;

	va_SendClientMessage(playerid, COLOR_GREY, "Safe balance: $%d, %d materials, %d drugs", groupInfo[playerInfo[playerid][pGroup]][safeMoney], groupInfo[playerInfo[playerid][pGroup]][safeMaterials], groupInfo[playerInfo[playerid][pGroup]][safeDrugs]);
	
	mysql_format(SQL, returnString, sizeof returnString, "update `factions` set `Money` = '%d', `Materials` = '%d', `Drugs` = '%d' where `id` = '%d';", groupInfo[playerInfo[playerid][pGroup]][safeMoney], groupInfo[playerInfo[playerid][pGroup]][safeMaterials], groupInfo[playerInfo[playerid][pGroup]][safeDrugs], playerInfo[playerid][pGroup]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

#include "../module_inc/groups/factionVehicles.inc"

#include "../module_inc/groups/police.inc"
#include "../module_inc/groups/hitmanagency.inc"
#include "../module_inc/groups/death_loot"
#include "../module_inc/groups/gangs/gangs"
#include "../module_inc/groups/newsreporters.inc"
#include "../module_inc/groups/instructors.inc"
#include "../module_inc/groups/taxi.inc"
//temp
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    switch(newkeys)
    {
        case KEY_SECONDARY_ATTACK:
        {
            if(IsPlayerInAnyDynamicArea(playerid) && GetPVarInt(playerid, "areaID_factions") > 0 && GetPVarInt(playerid, "areaID_factions") < serverFactions + 1)
            {
                if(GetPVarInt(playerid, "enter_building_delay") > gettime())
                    return va_SendClientMessage(playerid, COLOR_GREY, "[Anti-Abuz] Nu poti intra intr-o cladire timp de %d secunde.", GetPVarInt(playerid, "enter_building_delay") - gettime());

                if(groupInfo[GetPVarInt(playerid, "areaID_factions")][factionLocked] == 1 && playerInfo[playerid][pGroup] != GetPVarInt(playerid, "areaID_factions"))
                {
                    if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_POLICE || playerInfo[playerid][pGroup] != GROUP_TYPE_HITMAN)
                        return sendError(playerid,"Group HQ is Locked.");

                    Dialog_Show(playerid, DIALOG_BREAKHQ, DIALOG_STYLE_MSGBOX, "Group HQ", "This Group HQ is locked.\nYou can breach this lock and enter. Would you like to do so?", "Yes", "No");
                }

                new playerGroup = GetPVarInt(playerid, "areaID_factions");

                SetPlayerPos(playerid, factionInterior[playerGroup][0], factionInterior[playerGroup][1], factionInterior[playerGroup][2]);
                SetPlayerFacingAngle(playerid, factionInterior[playerGroup][3]);

                SetPlayerVirtualWorld(playerid, playerGroup);
                SetPlayerInterior(playerid, getHQInterior[playerGroup]);

                playerStaticVar[playerid][inHQ] = playerGroup;
                return true;
            }

            new virtualWorld = GetPlayerVirtualWorld(playerid);
            if(virtualWorld > 0 && virtualWorld <= MAX_FACTIONS)
            {
                if(virtualWorld < MAX_FACTIONS)
                {
                    if(IsPlayerInRangeOfPoint(playerid, 3.5, factionInterior[virtualWorld][0], factionInterior[virtualWorld][1], factionInterior[virtualWorld][2]) && virtualWorld != 0)
                    {
                        SetPlayerPos(playerid, groupInfo[virtualWorld][factionExterior][0], groupInfo[virtualWorld][factionExterior][1], groupInfo[virtualWorld][factionExterior][2]);
                        SetPVarInt(playerid, "areaID_factions", 0);

                        SetPlayerVirtualWorld(playerid, 0);
                        SetPlayerInterior(playerid, 0);

                        SetPVarInt(playerid, "enter_building_delay", gettime() + 15);

                        playerStaticVar[playerid][inHQ] = 0;

                        if(getPlayerFactionWar(playerid) != -1 && playerStaticVar[playerid][groupDuty])
                        {
                            setPlayerWarEnvironment(playerid);
							SendClientMessage(playerid, COLOR_SERVER, "WAR >>: {ffffff}Ai fost transferat intr-un Virtual World unde sunt prezenti doar jucatorii ce participa la war.");
                        }

                        return true;
                    }
                }
            }
            return false;
        }
    }

    return Y_HOOKS_CONTINUE_RETURN_1;
}
getCopsOnDutyCount() {
	return onDutyCopsCount;
}
/*hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	switch(newkeys)
	{
		case KEY_SECONDARY_ATTACK:
		{
			if(IsPlayerInAnyDynamicArea(playerid) && GetPVarInt(playerid, "areaID_factions") > 0 && GetPVarInt(playerid, "areaID_factions") < serverFactions + 1)
			{
				if(GetPVarInt(playerid, "enter_building_deelay") > gettime())
					return va_SendClientMessage(playerid, COLOR_GREY, "[Anti-Abuz] Nu poti intra intr-o cladire timp de %d secunde.", GetPVarInt(playerid, "enter_building_deelay") - gettime());
				
				if(groupInfo[GetPVarInt(playerid, "areaID_factions")][factionLocked] == 1 && playerInfo[playerid][pGroup] != GetPVarInt(playerid, "areaID_factions"))
				{
					if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_POLICE || playerInfo[playerid][pGroup] != GROUP_TYPE_HITMAN)
						return sendError(playerid,"Group HQ is Locked.");

					Dialog_Show(playerid, DIALOG_BREAKHQ, DIALOG_STYLE_MSGBOX, "Group HQ", "This Group HQ is locked.\nYou can breach this lock and enter. Would you like to do so?", "Yes", "No");
				}
				
				new playerGroup = GetPVarInt(playerid, "areaID_factions");

				SetPlayerPos(playerid, factionInterior[playerGroup][0], factionInterior[playerGroup][1], factionInterior[playerGroup][2]);
				SetPlayerFacingAngle(playerid, factionInterior[playerGroup][3]);

				SetPlayerVirtualWorld(playerid, playerGroup);
				SetPlayerInterior(playerid, getHQInterior[playerGroup]);

				playerStaticVar[playerid][inHQ] = playerGroup;
				joinWarMessage(playerid);
				return true;
			}
			if(GetPlayerVirtualWorld(playerid) != 0 && GetPlayerVirtualWorld(playerid) <= MAX_FACTIONS && IsPlayerInRangeOfPoint(playerid, 3.5, factionInterior[GetPlayerVirtualWorld(playerid)][0], factionInterior[GetPlayerVirtualWorld(playerid)][1], factionInterior[GetPlayerVirtualWorld(playerid)][2]))
			{
				SetPlayerPos(playerid, groupInfo[GetPlayerVirtualWorld(playerid)][factionExterior][0], groupInfo[GetPlayerVirtualWorld(playerid)][factionExterior][1], groupInfo[GetPlayerVirtualWorld(playerid)][factionExterior][2]);
				SetPVarInt(playerid, "areaID_factions", 0);
				
				SetPlayerVirtualWorld(playerid, 0);
				SetPlayerInterior(playerid, 0);
				
				SetPVarInt(playerid, "enter_building_deelay", gettime() + 15);
				playerStaticVar[playerid][inHQ] = 0;
				//sugi
				turf_data[playerid][warJoined] = groupInfo[playerInfo[playerid][pGroup]][warActive];
				if(factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_GANG && turf_data[playerid][warJoined] && playerStaticVar[playerid][groupDuty] == true)
					SetPlayerVirtualWorld(playerid, 1024 + turf_data[playerid][warJoined]);

				return true;
			}
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}
*/
hook OnPlayerDisconnect(playerid, reason)
{
	FactionSpec[playerid] = 0;

	if(playerStaticVar[playerid][groupDuty])
		onDutyCopsCount --;

	if(playerInfo[playerid][pGroup])
	{
		Iter_Remove(factionMembers<playerInfo[playerid][pGroup]>, playerid);

		if(playerStaticVar[playerid][playerLogged])
			sendGroupMessage(playerInfo[playerid][pGroup], COLOR_GENANNOUNCE, "%s from your group has disconnected (%s)%s", GetName(playerid), disconnectReason[reason], (getPlayerFactionWar(playerid) ? va_return(". His score was %i", warPlayerKills[playerid] -  warPlayerDeaths[playerid]) : ("")));

		if(factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_GANG)
		{
			updateRaportPoints(playerid);
		}
	}

	if(Iter_Contains(taxiCallers, playerid))
		Iter_Remove(taxiCallers, playerid);

	//wars
	if(Iter_Contains(warGangsters, playerid))
		Iter_Remove(warGangsters, playerid);

	return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:government(playerid, params[], help)
{
	if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_POLICE && playerInfo[playerid][pGroup] != GROUP_TYPE_PARAMEDIC)
		return sendError(playerid,"You are not a Cop/Paramedic.");

	if(playerInfo[playerid][pGroupRank] < 6)
		return sendError(playerid,"You need to be Rank 6 to be able to use this.");
	
	new returnMessage[144], groupName[32];
	if(sscanf(params, "s[144]", returnMessage))
		return sendSyntaxMessage(playerid, "/gov <Message>");

	if(FindIP(returnMessage)) /// Anti-Reclama
		sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s(%d) typed %s in /gov.", GetName(playerid), playerid, returnMessage);

	if(playerInfo[playerid][pGroup] == GROUP_TYPE_FBI) groupName = "F.B.I";
	else if(playerInfo[playerid][pGroup] == GROUP_TYPE_PARAMEDIC) groupName = "Paramedic Department";
	else format(groupName, sizeof groupName, "%s", factionName(playerInfo[playerid][pGroup]));

	va_SendClientMessageToAll(COLOR_CYAN, "------ Government Announcement (%s) ------", groupName);
	va_SendClientMessageToAll(COLOR_BLUE, "* %s: %s", GetDBName(playerid), returnMessage);
	return true;
}

YCMD:duty(playerid, params[], help)
{
	check_group
	check_delay

	if(!playerStaticVar[playerid][inHQ] && !playerStaticVar[playerid][inHouse])
		return sendError(playerid,"You are not in your house or in your faction HQ.");

	switch(playerStaticVar[playerid][groupDuty])
	{
		case false:
		{
			setPlayerDuty(playerid, true);
		}
		case true:
		{
			setPlayerDuty(playerid, false);
		}
	}
	set_delay;
	return true;
}


task check_player_raport[1800000]()
{	
	mysql_format(SQL, returnString, sizeof returnString, "select * from `users` where `Faction` >= '1' and `RaportTime` <= '%d';", gettime());
	mysql_tquery(SQL, returnString, "checkFactionActivity", "");
}

function:checkFactionActivity()
{
	//printf("[debug]: checking raports for %d players", cache_num_rows());

	new playerName[MAX_PLAYER_NAME], playerSQL,
		playerGroup, playerRank, groupDays, raportIncompleted, playerRaport[6];

	for(new i; i < cache_num_rows(); i++)
	{
		cache_get_value_name(i, "Name", playerName);
		cache_get_value_name_int(i, "ID", playerSQL);

		cache_get_value_name_int(i, "Faction", playerGroup);
		cache_get_value_name_int(i, "FactionRank", playerRank);

		cache_get_value_name_int(i, "FactionDays", groupDays);
		cache_get_value_name_int(i, "RaportIncompleted", raportIncompleted);

		new loadString[64];
		cache_get_value_name(i, "RaportScore", loadString);
   		sscanf(loadString, "a<i>[6]", playerRaport);

		new raportStatus = 1;
		for(new rId; rId <= 4; rId++)
		{
			if(playerRaport[rId] < raportPoints[playerGroup][playerRank][rId])
			{
				raportStatus = 0;
			}
			playerRaport[rId] -= raportPoints[playerGroup][playerRank][rId];
		}
        if(raportStatus == 1)
        {
        	if(playerRank < 5 && getDaysFromTimestamp(groupDays) >= rank_up_days[playerRank - 1])
			{
        		sendGroupMessage(playerGroup, COLOR_TEAL, "%s's faction rank was changed from %d to %d.", playerName, playerRank, playerRank + 1);
            	playerRank ++;

            	insertPlayerMail(playerSQL, gettime(), "Raportul tau de activitate a fost complet. Ai primit rank up la rank %d.", playerRank);
				
				mysql_format(SQL, returnString, sizeof returnString, "update `users` set `FactionRank` = '%d' where `ID` = '%d';", playerRank, playerSQL);
				mysql_tquery(SQL, returnString, "", "");
        	}
        	else
        	{
        		insertPlayerMail(playerSQL, gettime(), "Zile insuficiente, nu ai primit rank-up, foloseste /myraport.");
        	}
        	raportIncompleted = 0;
        }
        else
        {
        	if(raportIncompleted)
        	{
        		for(new rId; rId <= 4; rId++)
        			playerRaport[rId] = 0;

        		sendGroupMessage(playerGroup, COLOR_GENANNOUNCE, "%s was uninvited by AdmBot from faction %s (rank %d) after %d days, with 40 FP. Reason: raport incomplet.",
					playerName, factionName(playerGroup), playerRank, getDaysFromTimestamp(groupDays));

        		insertPlayerMail(playerSQL, gettime(), "Ai primit uninvite din factiunea %s rank %d dupa %d zile deoarece raportul tau de activitate a fost incomplet.", factionName(playerGroup), playerRank, getDaysFromTimestamp(groupDays));
        		raportIncompleted = 2;

        		mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Faction` = '0', `FactionRank` = '0', `FactionDays` = '0', `FactionWarns` = '0', `FactionPunish` = '40' where `ID` = '%d';", playerSQL);
				mysql_tquery(SQL, returnString, "", "");
        	}
        	else
        	{
        		for(new rId; rId <= 4; rId++)
        			playerRaport[rId] -= raportPoints[playerGroup][playerRank][rId];

        		new dateFormat[6];
				returnDateFromTimestamp((gettime() + (86400 * 7)), dateFormat[0], dateFormat[1], dateFormat[2], dateFormat[3], dateFormat[4], dateFormat[5], 2);

				insertPlayerMail(playerSQL, gettime(), "Raportul tau a fost incomplet. Vei avea de completat raportul pana pe data de %02d/%02d/%02d pentru a nu primi uninvite din factiune", dateFormat[2], dateFormat[1], dateFormat[0]);
        		raportIncompleted = 1;
        	}
        }
		mysql_format(SQL, returnString, sizeof returnString, "update `users` set `RaportIncompleted` = '%d', `RaportTime` = '%d', `RaportScore` = '%d %d %d %d %d %d' where `ID` = '%d';", raportIncompleted, (gettime() + (86400 * 7)), playerRaport[0], playerRaport[1], playerRaport[2], playerRaport[3], playerRaport[4], playerRaport[5], playerSQL);
		mysql_tquery(SQL, returnString, "", "");


		foreach(new pId: All(factionMembers))
		{
			if(playerSQL != playerInfo[pId][pSQLID])
				continue;

			for(new rId; rId <= 4; rId++)
        		playerInfo[pId][pRaportScore][rId] = playerRaport[rId];

			if(!raportStatus && raportIncompleted == 2)
			{
				new returnDialoreturnString[256];
				format(returnDialoreturnString, sizeof returnDialoreturnString, "Ai fost demis de AdmBot\n din factiunea din care faceai parte %s (rank %d)\n dupa %d days, cu 40 FP. Motiv: raport incomplet.", factionName(playerGroup), playerRank, getDaysFromTimestamp(groupDays));
				Dialog_Show(i, 0, DIALOG_STYLE_MSGBOX, "Info", returnDialoreturnString, "Ok", "");
		
				whenPlayerGotUninvited(pId, 40);
			}
			else
			{
				playerInfo[i][pGroupRank] = playerRank;
			}
			break;
		}
	}
	return true;
}