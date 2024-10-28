// ------------------------------------- //
//            defines                    //
// ------------------------------------- //


#define check_gang_member if(factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_GANG) return true;


// ------------------------------------- //
//            hooks                      //
// ------------------------------------- //

hook WhenPlayerLoggedIn(playerid)
{
	if(!cache_num_rows())
		return Y_HOOKS_CONTINUE_RETURN_1;
	
	new loadString[128];

	cache_get_value_name(0, "OrderWeapons", loadString);
	sscanf(loadString, "a<i>[3]", playerInfo[playerid][pGuns]);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

// ------------------------------------- //
//            commands                   //
// ------------------------------------- //

YCMD:order(playerid, params[], help)
{
	check_duty_status

	if(!playerInfo[playerid][pGunLic]) 
		return sendError(playerid, "You don't have a valid gun licence.");
		
	if(playerInfo[playerid][pGroup] == 11 && playerStaticVar[playerid][inHQ] == playerInfo[playerid][pGroup])
	{
		GivePlayerWeapon(playerid, WEAPON_SNIPER, 50), GivePlayerWeapon(playerid, WEAPON_KNIFE, 999);

	}
	else if(playerInfo[playerid][pGroup] != 11 && playerStaticVar[playerid][inHQ] == playerInfo[playerid][pGroup])
	{
		check_gang_member

	    if(GetMoney(playerid) <  2500 && GetMilliards(playerid) < 1)
	        return sendError(playerid,"You don't have enough money.");

		if(playerInfo[playerid][pGuns][0])
			GivePlayerWeapon(playerid, playerInfo[playerid][pGuns][0], 299);

		if(playerInfo[playerid][pGuns][1])
			GivePlayerWeapon(playerid, playerInfo[playerid][pGuns][1], 999);

		if(playerInfo[playerid][pGuns][2])
			GivePlayerWeapon(playerid, playerInfo[playerid][pGuns][2], 199);

		else if(!playerInfo[playerid][pGuns][0] && !playerInfo[playerid][pGuns][1] && !playerInfo[playerid][pGuns][2])
			return SendClientMessage(playerid, COLOR_GREY, "You don't have any weapons set for order! Type /setguns.");

		SendClientMessage(playerid, 0xff6464AA, "(Faction):{ffffff} Ai cumparat armele setate si ai platit $2.500. Au fost extrase 500 materiale din seiful factiunii.");
		groupInfo[playerInfo[playerid][pGroup]][safeMaterials] -= 500;
		addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 2500);


	}
	else
		return sendError(playerid,"{3385FF}You are not in place to get guns.");

	return true;
}

YCMD:setguns(playerid, params[], help)
{
	check_gang_member

	returnString = "Weapon name\tMinim rank\tStatus\n";
	format(returnString, sizeof returnString, "%sDeagle\t1+\t%s\nM4\t1+\t%s\nTEC-9\t1+\t%s\nRifle\t3+\t%s", returnString, (playerInfo[playerid][pGuns][0] == 24 ? ("{00ff00}selected{ffffff}") : ("{ff0000}not selected{ffffff}")),
		(playerInfo[playerid][pGuns][1] == 31 ? ("{00ff00}selected{ffffff}") : ("{ff0000}not selected{ffffff}")), (playerInfo[playerid][pGuns][1] == 32 ? ("{00ff00}selected{ffffff}") : ("{ff0000}not selected{ffffff}")),
		(playerInfo[playerid][pGuns][2] == 33 ? ("{00ff00}selected{ffffff}") : ("{ff0000}not selected{ffffff}")));

	Dialog_Show(playerid, DIALOG_SETGUNS, DIALOG_STYLE_TABLIST_HEADERS, "Order weapons:", returnString, "Select", "Exit");
	return true;
}


// ------------------------------------- //
//            dialogs                    //
// ------------------------------------- //

Dialog:DIALOG_SETGUNS(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	new returnQuery[256];
	switch(listitem)
	{
		case 0: playerInfo[playerid][pGuns][0] = playerInfo[playerid][pGuns][0] ? 0 : 24;
		case 1: 
		{
			if(playerInfo[playerid][pGuns][1] == 32)
				playerInfo[playerid][pGuns][1] = 31;

			else
				playerInfo[playerid][pGuns][1] = playerInfo[playerid][pGuns][1] ? 0 : 31;
		}
		case 2:
		{
			if(playerInfo[playerid][pGuns][1] == 31)
				playerInfo[playerid][pGuns][1] = 32;

			else
				playerInfo[playerid][pGuns][1] = playerInfo[playerid][pGuns][1] ? 0 : 32;
		}
		case 3:
		{
			if(playerInfo[playerid][pGroupRank] < 3)
				return sendError(playerid,"Nu ai rank-ul necesar pentru a putea seta aceasta arma.");

			playerInfo[playerid][pGuns][2] = playerInfo[playerid][pGuns][2] ? 0 : 33;
		}
	}
	mysql_format(SQL, returnQuery, sizeof returnQuery, "update `users` set `OrderWeapons` = '%d %d %d' where `id` = '%d';", playerInfo[playerid][pGuns][0], playerInfo[playerid][pGuns][1], playerInfo[playerid][pGuns][2], playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnQuery, "", "");

	return Command_ReProcess(playerid, "setguns", false);
}

// ------------------------------------- //
//            includes                   //
// ------------------------------------- //

#include "../module_inc/groups/gangs/turfs"
#include "../module_inc/groups/gangs/wars"