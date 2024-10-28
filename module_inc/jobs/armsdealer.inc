#include <YSI_Coding\y_hooks>
#define CP_ARMS (CP_JOBS_BEGIN) + JOB_TYPE_ARMS
#define ARMS_MATS (1000)

enum {
	MATS_OFFER_PLAYER, MATS_OFFER_PRICE, MATS_OFFER_AMOUNT
}

new
	materialSell[MAX_PLAYERS][3]; // 0 = MatsOffer[id] = playerid; 1 = MatsPrice[id] = price; MatsAmmo[id] = mats;

enum {
	GUN_OFFER_PLAYER, GUN_OFFER_PRICE, GUN_OFFER_GID
}

new
	weaponSell[MAX_PLAYERS][4];


hook WhenPlayerLoggedIn(playerid)
{
    if(!cache_num_rows())
        return Y_HOOKS_CONTINUE_RETURN_1;

    cache_get_value_name_int(0, "Materials", playerInfo[playerid][pMaterials]);
    return Y_HOOKS_CONTINUE_RETURN_1;
}

cancel_arms_work(playerid)
{
	if(job_data[playerid][jobDuty] != JOB_TYPE_ARMS)
		return 1;
	reset_mats_vars(playerid);
	reset_gun_vars(playerid);
	return 1;
}

reset_mats_vars(playerid)
{
	materialSell[playerid][MATS_OFFER_PLAYER] = INVALID_PLAYER_ID;
	materialSell[playerid][MATS_OFFER_AMOUNT] = 0;
	materialSell[playerid][MATS_OFFER_PRICE] = 0;

	return true;
}

reset_gun_vars(playerid)
{
	weaponSell[playerid][GUN_OFFER_PLAYER] = INVALID_PLAYER_ID;
	weaponSell[playerid][GUN_OFFER_PRICE] = 0;
	weaponSell[playerid][GUN_OFFER_GID] = 0;

	return true;
}

arms_returnGunAmmo(gunId)
{
	switch(gunId)
	{
		case 23: return 100;
		case 24: return 100;
		case 25: return 50;
		case 29: return 150;
		case 30: return 250;
		case 31: return 250;
		case 33: return 25;
	}
	return 0;
}

arms_returnGunCost(gunId)
{
	switch(gunId)
	{
		case 23: return 100;
		case 24: return 100;
		case 25: return 150;
		case 29: return 300;
		case 30: return 350;
		case 31: return 400;
		case 33: return 600;
	}
	return 0;
}

YCMD:matsinfo(playerid, params[], help)
{

	if(job_data[playerid][playerJob] != JOB_TYPE_ARMS)
		return sendError(playerid,"You are not an Arms Dealer.");

	format(returnString, sizeof returnString, "Materials: [%d]", playerInfo[playerid][pMaterials]);
    Dialog_Show(playerid, DIALOG_MATSINSO, DIALOG_STYLE_MSGBOX, "Materials Information:", returnString, "Close", "");
    return 1;
}

YCMD:getmats(playerid, params[], help)
{ 
	if(job_data[playerid][playerJob] != JOB_TYPE_ARMS)
		return sendError(playerid,"You are not an Arms Dealer.");

    if(playerStaticVar[playerid][pCheckpoint])
        return sendError(playerid,"You can't use this command while you've an active checkpoint.");

    if(!IsPlayerInRangeOfPoint(playerid, 2.0, jobWorkPositions[JOB_TYPE_ARMS][0], jobWorkPositions[JOB_TYPE_ARMS][1], jobWorkPositions[JOB_TYPE_ARMS][2]))
    {
	    SendClientMessage(playerid, -1, "Mergi la checkpoint pentru a colecta materialele.");
	    
	    SetPlayerCheckpoint(playerid, jobWorkPositions[JOB_TYPE_ARMS][0], jobWorkPositions[JOB_TYPE_ARMS][1], jobWorkPositions[JOB_TYPE_ARMS][2], 10.0);
	    playerStaticVar[playerid][pCheckpoint] = CP_ARMS;
	    return 1;
    }

    if(GetPlayerMoney(playerid) < 500)
    	return sendError(playerid,"Nu ai $500.");

    SendClientMessage(playerid, -1, "Mergi la checkpoint pentru a colecta materialele.");
    GivePlayerMoney(playerid, -500);

    SetPlayerCheckpoint(playerid, 1939.5686,144.8884,37.2612, 2.0);
    playerStaticVar[playerid][pCheckpoint] = CP_ARMS;

	WhenPlayerStartWorking(playerid);
	return 1;
}

YCMD:sellmaterials(playerid, params[], help)
{
	if(job_data[playerid][playerJob] != JOB_TYPE_ARMS)
		return sendError(playerid,"You are not an Arms Dealer.");

	new
		targetid, iMats, iPrice;

    if(sscanf(params, "uii", targetid, iMats, iPrice))
    	return sendSyntaxMessage(playerid, "/sellmaterials <Name/Playerid> <Materials> <Price>");

	if(!IsPlayerConnected(targetid))
		return SendClientMessage(playerid, COLOR_GREY, "Player not connected.");

	if(iPrice < 1 || iPrice > 150000)
		return sendError(playerid,"Price not lower then $1, or above $150,000.");

	if(playerid == targetid)
		return sendError(playerid,"You can not sell yourself materials.");

	if(!IsPlayerInRangeOfPlayer(playerid, targetid, 5.0))
		return sendError(playerid,"This player is not near you.");
	
	if(iMats < 1 || iMats > 50000)
		return sendError(playerid,"Wrong materials ammount (1-50000).");
 
    if(iMats > playerInfo[playerid][pMaterials])
    	return sendError(playerid,"You don't have so many materials to sell.");

	va_SendClientMessage(playerid, -1, "{B8FFDB}* You offerd %s, %d materials for %s$.", GetName(targetid), iMats, FormatNumber(iPrice));
	va_SendClientMessage(targetid, -1, "{B8FFDB}* %s offered you %d materials for %s$, (type /accept materials %d) to accept.", GetName(playerid), iMats, FormatNumber(iPrice), playerid);

	materialSell[targetid][MATS_OFFER_PLAYER] = playerid;
	materialSell[targetid][MATS_OFFER_AMOUNT] = iMats;
	materialSell[targetid][MATS_OFFER_PRICE] = iPrice;

    return 1;
}

accept_materials(playerid, targetid)
{
    if(materialSell[playerid][MATS_OFFER_PLAYER] == INVALID_PLAYER_ID)
        return sendError(playerid,"No-one offerd you materials.");

    if(materialSell[playerid][MATS_OFFER_PLAYER] != targetid)
        return sendError(playerid,"This player has not offered you materials.");
    
	if(!IsPlayerInRangeOfPlayer(playerid, targetid, 5.0))
		return sendError(playerid,"This player is not near you.");

    if(GetPlayerMoney(playerid) < materialSell[playerid][MATS_OFFER_PRICE])
    	return SendClientMessage(playerid,-1,"You don't have money needed.");

    playerInfo[playerid][pMaterials] += materialSell[playerid][MATS_OFFER_AMOUNT];
    playerInfo[targetid][pMaterials] -= materialSell[playerid][MATS_OFFER_AMOUNT];

    GivePlayerMoney(playerid, -materialSell[playerid][MATS_OFFER_PRICE]);
    GivePlayerMoney(targetid, materialSell[playerid][MATS_OFFER_PRICE]);

    va_SendClientMessage(playerid, -1, "{B8FFDB}You buy %d materials for $%s from %s.", materialSell[playerid][MATS_OFFER_AMOUNT], FormatNumber(materialSell[playerid][MATS_OFFER_PRICE]), GetName(targetid));
    va_SendClientMessage(targetid, -1, "{B8FFDB}%s bought your materials for $%s.", GetName(playerid), FormatNumber(materialSell[playerid][MATS_OFFER_PRICE]));
     
	UpdatePVars(playerid, "Materials", playerInfo[playerid][pMaterials]);
	UpdatePVars(targetid, "Materials", playerInfo[targetid][pMaterials]);

	reset_mats_vars(playerid);

	return 1;
}

YCMD:sellgun(playerid, params[], help)
{
	if(job_data[playerid][playerJob] != JOB_TYPE_ARMS)
		return sendError(playerid,"You are not an Arms Dealer.");

	new
		sellWeapon[12], weaponPrice, targetid;
	
	if(sscanf(params, "us[12]i", targetid, sellWeapon, weaponPrice))
	{
		sendSyntaxMessage(playerid, "/sellgun <Name/Playerid> <Gun Name> <Price>");
		SendClientMessage(playerid, -1, "*** Sell Guns ***");
		SendClientMessage(playerid, -1, "{B8DBFF}Weapons:{FFFFFF} SDPistol(100) Deagle(150)");
		SendClientMessage(playerid, -1, "{B8DBFF}Weapons:{FFFFFF} MP5(200) Shotgun(300)");
		SendClientMessage(playerid, -1, "{B8DBFF}Weapons:{FFFFFF} AK47(350) M4(400) Rifle(600)");
		return 1;
	}

	if(playerid == targetid)
		return sendError(playerid,"You can not sell yourself weapons.");

	if(!IsPlayerConnected(targetid))
		return SendClientMessage(playerid, COLOR_GREY, "Player not connected.");

	if(playerInfo[targetid][pJail] > 0)
		return sendError(playerid,"This player is in jail.");

	if(playerInfo[targetid][pGunLic] == 0)
		return sendError(playerid,"This player does not have arms license.");

	if(weaponPrice < 1 || weaponPrice > 100000)
		return sendError(playerid,"{B8DBFF}Price must be between $1 and $100,000.");

	if(GetPlayerMoney(targetid) < weaponPrice)
		return sendError(playerid,"{FFB870}This player don't have money needed.");

	if(strcmp(sellWeapon, "sdpistol", true) == 0)
	{
	    if(playerInfo[playerid][pMaterials] < 100)
	    	return sendError(playerid,"Not enough Materials for that Weapon.");

	    weaponSell[targetid][GUN_OFFER_GID] = 23;
	}
	else if(strcmp(sellWeapon, "deagle", true) == 0)
	{
	    if(playerInfo[playerid][pMaterials] < 150)
	    	return sendError(playerid,"Not enough Materials for that Weapon.");

	    weaponSell[targetid][GUN_OFFER_GID] = 24;
	}
	else if(strcmp(sellWeapon, "mp5", true) == 0)
	{
	    if(playerInfo[playerid][pMaterials] < 200)
	    	return sendError(playerid,"Not enough Materials for that Weapon.");

	    weaponSell[targetid][GUN_OFFER_GID] = 29;
	}
	else if(strcmp(sellWeapon, "shotgun", true) == 0)
	{
	    if(playerInfo[playerid][pMaterials] < 300)
	    	return sendError(playerid,"Not enough Materials for that Weapon.");

	    weaponSell[targetid][GUN_OFFER_GID] = 25;
	}
	else if(strcmp(sellWeapon, "ak47", true) == 0)
	{
	    if(playerInfo[playerid][pMaterials] < 350)
	    	return sendError(playerid,"Not enough Materials for that Weapon.");

	    weaponSell[targetid][GUN_OFFER_GID] = 30;
	}
	else if(strcmp(sellWeapon, "m4", true) == 0)
	{
	    if(playerInfo[playerid][pMaterials] < 400)
	    	return sendError(playerid,"Not enough Materials for that Weapon.");

	    weaponSell[targetid][GUN_OFFER_GID] = 31;
	}
	else if(strcmp(sellWeapon, "rifle", true) == 0)
	{
	    if(playerInfo[playerid][pMaterials] < 600)
	    	return sendError(playerid,"Not enough Materials for that Weapon.");

	    weaponSell[targetid][GUN_OFFER_GID] = 33;
	}
	else return sendError(playerid,"{B8DBFF}Invalid weapon name.");

	if(!IsPlayerInRangeOfPlayer(playerid, targetid, 5.0))
		return sendError(playerid,"{FFFFCC}This player is not near you.");

    if(Spectate[targetid] == playerid)
    	return sendError(playerid,"{FFFFCC}This player is not near you.");

	va_SendClientMessage(playerid, -1, "{B8FFDB}* You offerd %s %s for %s$.", GetName(targetid), GetWeaponNameID(weaponSell[targetid][GUN_OFFER_GID]), FormatNumber(weaponPrice));
	va_SendClientMessage(targetid, -1, "{B8FFDB}* %s offered you %s for %s$, (type /accept gun %d) to accept.", GetName(playerid), GetWeaponNameID(weaponSell[targetid][GUN_OFFER_GID]), FormatNumber(weaponPrice), playerid);
    
    weaponSell[targetid][GUN_OFFER_PLAYER] = playerid;
    weaponSell[targetid][GUN_OFFER_PRICE] = weaponPrice;

	return 1;
}

accept_gun(playerid, targetid)
{
    if(weaponSell[playerid][GUN_OFFER_PLAYER] == INVALID_PLAYER_ID)
        return sendError(playerid,"No-one offerd you gun.");

    if(weaponSell[playerid][GUN_OFFER_PLAYER] != targetid)
    	return sendError(playerid,"This player has not offered you gun.");

	if(!IsPlayerInRangeOfPlayer(playerid, targetid, 5.0))
		return sendError(playerid,"This player is not near you.");

    if(GetPlayerMoney(playerid) < weaponSell[playerid][GUN_OFFER_PRICE])
    	return sendError(playerid,"You don't have money needed.");

    playerInfo[targetid][pMaterials] -= arms_returnGunCost(weaponSell[playerid][GUN_OFFER_GID]);

	GivePlayerMoney(playerid, -weaponSell[playerid][GUN_OFFER_PRICE]);
	GivePlayerMoney(targetid, weaponSell[playerid][GUN_OFFER_PRICE]);

	va_SendClientMessage(playerid, COLOR_GREY, "You have recieved a %s with %d ammo from %s.", GetWeaponNameID(weaponSell[playerid][GUN_OFFER_GID]), arms_returnGunAmmo(weaponSell[playerid][GUN_OFFER_GID]), GetName(targetid));
	va_SendClientMessage(targetid, COLOR_GREY, "You have given %s, a %s with %d ammo, for %d materials.", GetName(playerid), GetWeaponNameID(weaponSell[playerid][GUN_OFFER_GID]), arms_returnGunAmmo(weaponSell[playerid][GUN_OFFER_GID]), arms_returnGunCost(weaponSell[playerid][GUN_OFFER_GID]));

	format(returnString, sizeof returnString, "* %s created a gun from materials, and hands it to %s.", GetName(targetid), GetName(playerid));
	ProxDetector(30.0, playerid, returnString, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	
	GivePlayerWeapon(playerid, weaponSell[playerid][GUN_OFFER_GID], arms_returnGunAmmo(weaponSell[playerid][GUN_OFFER_GID]));
	
	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Materials` = '%d' where `id` = '%d';", playerInfo[targetid][pMaterials], playerInfo[targetid][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");

	reset_gun_vars(playerid);

	return 1;
}

hook OnPlayerSpawn(playerid)
{
	if(materialSell[playerid][MATS_OFFER_PLAYER] != INVALID_PLAYER_ID)
		reset_mats_vars(playerid);

	if(weaponSell[playerid][GUN_OFFER_PLAYER] != INVALID_PLAYER_ID)
		reset_gun_vars(playerid);

	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid)
{
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnGameModeInit()
{
	Command_AddAltNamed("sellmaterials", "sellmats");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEnterCheckpoint(playerid)
{
	if(playerStaticVar[playerid][pCheckpoint] == CP_ARMS)
	{
		playerStaticVar[playerid][pCheckpoint] = 0;
		DisablePlayerCheckpoint(playerid);

		if(job_data[playerid][jobDuty] == JOB_TYPE_NONE)
		{
			playerStaticVar[playerid][pCheckpoint] = 0;
		    DisablePlayerCheckpoint(playerid);

		    SendClientMessage(playerid, COLOR_YELLOW, "Foloseste /getmats aici pentru a colecta materiale.");
		}
		else
		{
			new
				job_mats = (ARMS_MATS) + (GetPlayerJobSkill(playerid, JOB_TYPE_ARMS) * 100 + RandomEx((GetPlayerJobSkill(playerid, JOB_TYPE_ARMS) * 100), (GetPlayerJobSkill(playerid, JOB_TYPE_ARMS) * 100) + 100));
			
			va_SendClientMessage(playerid, -1, "Ai colectat %s materiale.", FormatNumber(job_mats));

			playerInfo[playerid][pMaterials] += job_mats;
			UpdatePlayerJobSkill(playerid, JOB_TYPE_ARMS);

			DailyQuestCheck(playerid, QUEST_TYPE_MATS, job_mats);

			mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Materials` = '%d' where `id` = '%d';", playerInfo[playerid][pMaterials], playerInfo[playerid][pSQLID]);
			mysql_tquery(SQL, returnString, "", "");
			
			CallLocalFunction("StopJobWork", "i", playerid);
		}
	} 
	return Y_HOOKS_CONTINUE_RETURN_1;
}