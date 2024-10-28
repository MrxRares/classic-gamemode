#include <YSI_Coding\y_hooks>
hook OnGameModeInit()
{
	Command_AddAltNamed("buyfood", "eat");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:buyfood(playerid, params[], help)
{


	if(playerStaticVar[playerid][inBusiness] && bizInfo[playerStaticVar[playerid][inBusiness]][bizType] == 5 || playerStaticVar[playerid][inBusiness] > 1 && bizInfo[playerStaticVar[playerid][inBusiness]][bizType] == 7 || playerStaticVar[playerid][inBusiness] > 1 && bizInfo[playerStaticVar[playerid][inBusiness]][bizType] == 15)
	{
		if(GetPVarInt(playerid, "ate_delay") > gettime())
			return va_SendClientMessage(playerid, COLOR_GREY, "Trebuie sa astepti %d secunde pentru a putea folosi din nou aceasta comanda.", GetPVarInt(playerid, "ate_delay") - gettime());

	    returnString = "{FFFFFF}Option\t{FFFFFF}Price\n";
	    format(returnString, sizeof returnString, "%sBuy food for you\t$%d", returnString, bizInfo[playerStaticVar[playerid][inBusiness]][bizFee]);

	    Dialog_Show(playerid, DIALLG_BUYFOOD, DIALOG_STYLE_TABLIST_HEADERS, "Buy Food", returnString, "Select", "Cancel");
	    return true;
	}
	else
		SendClientMessage(playerid, -1, "{FFFFCC}You're at the place where you can eat.");
	return true;
}
Dialog:DIALLG_BUYFOOD(playerid, response, listitem, inputtext[])
{
    if(!response)
        return 1;
	switch(listitem)
	{
		case 0:
		{

		    if(GetMoney(playerid) <  bizInfo[playerStaticVar[playerid][inBusiness]][bizFee] && GetMilliards(playerid) < 1)
		        return sendError(playerid,"You don't have enough money.");

			SetPlayerHealthEx(playerid, 99);
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);

			sendNearbyMessage(playerid, COLOR_PURPLE, "* %s ate some food.", GetName(playerid));
			
			substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, bizInfo[playerStaticVar[playerid][inBusiness]][bizFee]);

			GameTextForPlayer(playerid, va_return("~r~-%d$", bizInfo[playerStaticVar[playerid][inBusiness]][bizFee]), 5000, 1);
			bizInfo[playerStaticVar[playerid][inBusiness]][bizMoney] += bizInfo[playerStaticVar[playerid][inBusiness]][bizFee];
			
			mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `bizMoney` = '%d' where `id` = '%d';", bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney], GetPlayerVirtualWorld(playerid));
			mysql_tquery(SQL, returnString, "", "");

			SetPVarInt(playerid, "ate_delay", gettime() + 10);
		}
	}

    return 1;
}

YCMD:buytoys(playerid, params[], help)
{
	if(playerStaticVar[playerid][inBusiness] < 1)
		return sendError(playerid,"You are not at sex shop.");

	if(bizInfo[playerStaticVar[playerid][inBusiness]][bizType] != 16)
		return true;

	Dialog_Show(playerid, DIALOG_SEXSHOP, DIALOG_STYLE_LIST, "SERVER: Sex Shop", "Purple Dildo ($100)\nSmall White Vibrator ($100)\nSilver Vibrator ($100)\nLarge White Vibrator ($100)\nFlowers ($150)", "Select", "Exit");
	return true;
}
Dialog:DIALOG_SEXSHOP(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	switch(listitem)
	{
		case 0:
		{
		    if(GetMoney(playerid) <  100 && GetMilliards(playerid) < 1)
		        return sendError(playerid,"You don't have enough money.");
			substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 100);
			bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney] += 100;

			GivePlayerWeapon(playerid, WEAPON_DILDO, 1);
			GameTextForPlayer(playerid, "~r~-$100", 3000, 1);
		}
		case 1:
		{
		    if(GetMoney(playerid) <  100 && GetMilliards(playerid) < 1)
		        return sendError(playerid,"You don't have enough money.");

			substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 100);
			bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney] += 100;

			GivePlayerWeapon(playerid, WEAPON_DILDO2, 1);
			GameTextForPlayer(playerid, "~r~-$100", 3000, 1);
		}
		case 2:
		{
		    if(GetMoney(playerid) <  100 && GetMilliards(playerid) < 1)
		        return sendError(playerid,"You don't have enough money.");

			substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 100);
			bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney] += 100;

			GivePlayerWeapon(playerid, WEAPON_VIBRATOR2, 1);
			GameTextForPlayer(playerid, "~r~-$100", 3000, 1);
		}
		case 3:
		{
		    if(GetMoney(playerid) <  100 && GetMilliards(playerid) < 1)
		        return sendError(playerid,"You don't have enough money.");

			substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 100);
			bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney] += 100;

			GivePlayerWeapon(playerid, WEAPON_VIBRATOR, 1);
			GameTextForPlayer(playerid, "~r~-$100", 3000, 1);
		}
		case 4:
		{
		    if(GetMoney(playerid) <  150 && GetMilliards(playerid) < 1)
		        return sendError(playerid,"You don't have enough money.");

			substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 150);
			bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney] += 150;

			GivePlayerWeapon(playerid, WEAPON_FLOWER, 1);
			GameTextForPlayer(playerid, "~r~-$150", 3000, 1);
		}
	}
	return true;
}

YCMD:buydrink(playerid, params[], help)
{
	if(playerStaticVar[playerid][inBusiness] < 1)
		return sendError(playerid,"You are not at bar.");

	if(bizInfo[playerStaticVar[playerid][inBusiness]][bizType] != 3)
		return true;

	Dialog_Show(playerid, DIALOG_BAR, DIALOG_STYLE_LIST, "SERVER: Bar", "Stock Bar Cigars ($500)\nSprunk ($500)\nBeer ($600)\nWine ($1000)", "Select", "Exit");
	return true;
}
Dialog:DIALOG_BAR(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	switch(listitem)
	{
		case 0:
		{
		    if(GetMoney(playerid) <  500 && GetMilliards(playerid) < 1)
		        return sendError(playerid,"You don't have enough money.");

			substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 500);
			bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney] += 500;

			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_SMOKE_CIGGY);
			GameTextForPlayer(playerid, "~r~-$500", 3000, 1);
		}
		case 1:
		{
		    if(GetMoney(playerid) <  500 && GetMilliards(playerid) < 1)
		        return sendError(playerid,"You don't have enough money.");

			substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 500);
			bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney] += 500;

			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
			GameTextForPlayer(playerid, "~r~-$500", 3000, 1);
		}
		case 2:
		{
		    if(GetMoney(playerid) <  600 && GetMilliards(playerid) < 1)
		        return sendError(playerid,"You don't have enough money.");

			substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 600);
			bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney] += 600;

			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_BEER);
			GameTextForPlayer(playerid, "~r~-$600", 3000, 1);
		}
		case 3:
		{
		    if(GetMoney(playerid) <  1000 && GetMilliards(playerid) < 1)
		        return sendError(playerid,"You don't have enough money.");

			substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 1000);
			bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney] += 1000;

			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_WINE);
			GameTextForPlayer(playerid, "~r~-$1000", 3000, 1);
		}
	}
	DailyQuestCheck(playerid, QUEST_TYPE_BAR, 1);

	mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `bizMoney` = '%d' where `id` = '%d';", bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney], GetPlayerVirtualWorld(playerid));
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
	if(newkeys == KEY_FIRE)
	{
		if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_DRINK_WINE || GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_DRINK_BEER)
		{
			SetPlayerDrunkLevel(playerid, GetPlayerDrunkLevel(playerid) + 200);
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:fillgascan(playerid, params[], help)
{
	if(GetPVarInt(playerid, "business_area_sphere") < 1 || GetPVarInt(playerid, "business_area_sphere") > 0 && bizInfo[GetPVarInt(playerid, "business_area_sphere")][bizType] != 12)
		return sendError(playerid,"You need to be close to a gas station to refuel your gas can.");

	if(playerInfo[playerid][pGascan] == 0)
		return sendError(playerid,"You don't have a gas can.");

	if(playerInfo[playerid][pGascan] == 2)
		return sendError(playerid,"Your gas can is already full.");

	new gasPrice = 20 * 50;
    if(GetMoney(playerid) <  gasPrice && GetMilliards(playerid) < 1)
        return sendError(playerid,"You don't have enough money.");

	va_SendClientMessage(playerid, COLOR_GREEN, "Ai platit $%s pentru a umple canistra cu benzina.", FormatNumber(gasPrice));
	substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, gasPrice);

	playerInfo[playerid][pGascan] = 2;
	bizInfo[GetPVarInt(playerid, "business_area_sphere")][bizMoney] += gasPrice;

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `gasCan` = '2' where `id` = '%d';", playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");

	mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `bizMoney` = '%d' where `id` = '%d';", bizInfo[GetPVarInt(playerid, "business_area_sphere")][bizMoney], GetPVarInt(playerid, "business_area_sphere"));
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

YCMD:fill(playerid, params[], help)
{
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
		return true;

	if(vehicleInfo[GetPlayerVehicleID(playerid)][vehEngine])
		return sendError(playerid,"You need to stop the vehicle engine for reffueling.");

	if(GetPVarInt(playerid, "business_area_sphere") < 1 || GetPVarInt(playerid, "business_area_sphere") > 0 && bizInfo[GetPVarInt(playerid, "business_area_sphere")][bizType] != 12)
	{
		if(playerInfo[playerid][pGascan] != 2)
			return sendError(playerid,"You need to be close to a gas station to refuel your vehicle.");

		if(vehicleInfo[GetPlayerVehicleID(playerid)][vehFuel] > 80)
			return sendError(playerid,"Your vehicle have enough fuel in vehicle.");

		playerInfo[playerid][pGascan] = 1;
		
		vehicleInfo[GetPlayerVehicleID(playerid)][vehFuel] += 20;
		TogglePlayerControllable(playerid, true);

		mysql_format(SQL, returnString, sizeof returnString, "update `users` set `gasCan` = '1' where `id` = '%d';", playerInfo[playerid][pSQLID]);
		mysql_tquery(SQL, returnString, "", "");
		return true;
	}
	GameTextForPlayer(playerid, "~w~~n~~n~~n~~n~~n~~n~~n~~n~~n~re-fueling vehicle, please wait", 2000, 3);
	TogglePlayerControllable(playerid, false);

	SetTimerEx("refuel_vehicle", 8000, false, "i", playerid);
	SetPVarInt(playerid, "refuel_veh", 1);
	return true;
}

function:refuel_vehicle(const playerid)
{
	if(GetPlayerVehicleID(playerid)	== INVALID_VEHICLE_ID)
		return true;

	new gasPrice = (100 - vehicleInfo[GetPlayerVehicleID(playerid)][vehFuel]) * 50;

	if(GetMoney(playerid) < gasPrice)
		return sendError(playerid, "Plinul la masina nu a fost facut deoarece nu ai destui bani. Ai nevoie de: $%d", gasPrice);

	va_SendClientMessage(playerid, COLOR_GREEN, "Ai platit $%s pentru a face plinul la masina.", FormatNumber(gasPrice));
	vehicleInfo[GetPlayerVehicleID(playerid)][vehFuel] = 100;

	substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, gasPrice);


	if(GetPVarInt(playerid, "business_area_sphere") < 1 || GetPVarInt(playerid, "business_area_sphere") > 0 && bizInfo[GetPVarInt(playerid, "business_area_sphere")][bizType] != 12)
	{
		bizInfo[GetPVarInt(playerid, "business_area_sphere")][bizMoney] += gasPrice;

		mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `bizMoney` = '%d' where `id` = '%d';", bizInfo[GetPVarInt(playerid, "business_area_sphere")][bizMoney], GetPVarInt(playerid, "business_area_sphere"));
		mysql_tquery(SQL, returnString, "", "");
	}

	TogglePlayerControllable(playerid, true);
	DeletePVar(playerid, "refuel_veh");
	return true;
}

new returnTotalAds;
YCMD:ad(playerid, params[], help)
{
	new time,time2;
	if(GetPVarInt(playerid, "ad_delay") > gettime())
		return va_SendClientMessage(playerid, COLOR_GREY, "Error:{ffffff} You can send a new advertisment in %s.", iTimestampDate(GetPVarInt(playerid, "ad_delay") - gettime()));

	if(!playerInfo[playerid][pPremiumAccount] && GetPVarInt(playerid, "business_area_sphere") < 1 || GetPVarInt(playerid, "business_area_sphere") > 0 && bizInfo[GetPVarInt(playerid, "business_area_sphere")][bizType] != 13)
		return sendError(playerid,"You are not near any CNN Agency.");

	if(!strmatch(playerStaticVar[playerid][adMessage], ""))
		return sendError(playerid,"You already have an posted ad. Type /myad.");

	if(returnTotalAds > 20)
		return sendError(playerid,"There are already 20 ads placed, come back later.");

    if(!playerInfo[playerid][pPhoneNumber])
        return sendError(playerid,"You don't have a phone, a phone is require to place an ad.");


	new returnMessage[128];
	if(sscanf(params, "s[128]", returnMessage))
		return sendSyntaxMessage(playerid, "/ad [message]");

	if(strlen(returnMessage) > 128)
		return sendError(playerid,"Too many characters.");

    if(GetMoney(playerid) <  (returnTotalAds + 1) + 5500 && GetMilliards(playerid) < 1)
        return sendError(playerid,"You don't have enough money.");

	++ returnTotalAds;

	if(returnTotalAds > 0) 
	{
		if(Iter_Contains(premiumPlayers, playerid))
			time = (returnTotalAds+1)*30*1000/2, time2 = (returnTotalAds+1)*30/2;
		else 
			time = (returnTotalAds+1)*30*1000/2, time2 = (returnTotalAds+1)*30/2;
	}
	else if(returnTotalAds == 0) 
	{
		if(Iter_Contains(premiumPlayers, playerid)) 
			time = 30000/2, time2 = 30/2;
		else 
			time = 30000, time2 = 30; 
	}

	format(playerStaticVar[playerid][adMessage], 128, returnMessage);

	va_SendClientMessage(playerid, -1, "You paid $%s for your advertisement. It will be posted in %s.", FormatNumber((returnTotalAds + 10554)), calculateTime(time2));
	substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, (returnTotalAds + 10554));

	bizInfo[GetPVarInt(playerid, "business_area_sphere")][bizMoney] += (returnTotalAds + 10554);

	playerStaticVar[playerid][adTime] = time;
	SetTimerEx("WhenServerPostAdvertise", playerStaticVar[playerid][adTime], false, "i", playerid);

	SetPVarInt(playerid, "ad_delay", gettime() + 600);
	sendStaffMessage(STAFF_TYPE_ALL, 0x00FF00FF, "{FF6347}[Preview]{00FF00} Ad by %s ({FFFFFF}%d{00FF00}): %s", GetDBName(playerid), playerInfo[playerid][pPhoneNumber], returnMessage);
	
	mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `bizMoney` = '%d' where `id` = '%d';", bizInfo[GetPVarInt(playerid, "business_area_sphere")][bizMoney], GetPVarInt(playerid, "business_area_sphere"));
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

function:WhenServerPostAdvertise(const playerid)
{
	if(strmatch(playerStaticVar[playerid][adMessage], ""))
		return true;

	new string[200];
	if(Iter_Contains(premiumPlayers, playerid))
		format(string, sizeof string, "{FF0000}[!]{00FF00} Ad Premium by %s (phone: {FFFFFF}%d{00FF00}): %s", GetDBName(playerid), playerInfo[playerid][pPhoneNumber], playerStaticVar[playerid][adMessage]);
	else
		format(string, sizeof string, "Ad by %s (phone: {FFFFFF}%d{00FF00}): %s", GetDBName(playerid), playerInfo[playerid][pPhoneNumber], playerStaticVar[playerid][adMessage]);
	

	foreach(new i : Player) 
	{ 
		sendSplitMessage(i, 0x00FF00FF, string); 
	}
	
	// foreach(new i : Player)
	// 	sendSplitMessage(playerid, (i == playerid) ? COLOR_GREEN : 0x00FF00FF, "%s", string);

	SendClientMessage(playerid, -1, "Acum poti plasa din nou un /ad.");

	format(playerStaticVar[playerid][adMessage], 6, "");
	-- returnTotalAds;
	return true;
}

hook OnPlayerDisconnect(playerid, reason)
{
	if(!strmatch(playerStaticVar[playerid][adMessage], ""))
	{
		format(playerStaticVar[playerid][adMessage], sizeof returnString, "");
		-- returnTotalAds;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:myad(playerid, params[], help)
{
	if(strmatch(playerStaticVar[playerid][adMessage], ""))
		return sendError(playerid,"You don't have any posted ad.");

	sendSplitMessage(playerid, -1, "Anuntul tau (va fi postat in %s): %s", iTimestampDate(playerStaticVar[playerid][adTime] / 1000), playerStaticVar[playerid][adMessage]);
	return true;
}


//bank


YCMD:balance(playerid, params[], help)
{
	if(bizInfo[playerStaticVar[playerid][inBusiness]][bizType] != 1)
		return sendError(playerid,"You are not at the bank!");

	va_SendClientMessage(playerid, -1, "{FFFF00}You have $%s in your account.", GetPlayerBank(playerid));
	return true;
}

YCMD:withdraw(playerid, params[], help)
{
	if(bizInfo[playerStaticVar[playerid][inBusiness]][bizType] != 1)
		return SendClientMessage(playerid, -1, "You are not at the bank!");

	new returnAmount;
	if(sscanf(params, "d", returnAmount) || returnAmount < 1 || returnAmount >= 999999999)
		return sendSyntaxMessage(playerid, "/withdraw <Amount of Money>"), va_SendClientMessage(playerid, -1, "{FFFF00}You have $%s in your account.", GetPlayerBank(playerid));

	if(returnAmount == 0)
		return SendClientMessage(playerid, -1, "Nu puteti retrage 0$");


    if(playerInfo[playerid][pAccount] < returnAmount && playerInfo[playerid][pAccountLY] < 1)
        return SendClientMessage(playerid, -1, "You don't have that sum of money in bank!");

	new Float: calculateTax = (0.5 / 100) * returnAmount,
		wTax = floatround(calculateTax);

	SetPVarInt(playerid, "wAmount", returnAmount);

	format(returnString, sizeof returnString, "Taxa perceputa pentru aceasta tranzactie este de $%s.\nPrin retragerea a $%s vei primi in mana $%s.\nEsti de acord sa realizezi aceasta actiune?", FormatNumber(wTax), FormatNumber(returnAmount), FormatNumber(returnAmount - wTax));
	Dialog_Show(playerid, DIALOG_WITHDRAW, DIALOG_STYLE_MSGBOX, "Deposit:", returnString, "Da", "Nu");
	return true;
}

Dialog:DIALOG_WITHDRAW(playerid, response, listitem, inputtext[])
{
    if(!response)
        return DeletePVar(playerid, "wAmount");

    new returnAmount = GetPVarInt(playerid, "wAmount");
    new Float: calculateTax = (0.5 / 100) * returnAmount;
    new wTax = floatround(calculateTax);

    if(playerInfo[playerid][pAccount] < returnAmount && playerInfo[playerid][pAccountLY] < 1)
        return SendClientMessage(playerid, -1, "Nu ai banii necesari tranzactiei in banca.");

    addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, returnAmount - wTax, false);
    substractBankMoney(playerid, playerInfo[playerid][pAccountLY], playerInfo[playerid][pAccount], 0, returnAmount, false);

    va_SendClientMessage(playerid, -1, "{FFFF00}You have withdrawn $%s (tax: $%s) from your account. Total Remaining: $%s.", FormatNumber(returnAmount), FormatNumber(wTax), GetPlayerBank(playerid));

    return true;
}


YCMD:deposit(playerid, params[], help) {
    if(bizInfo[playerStaticVar[playerid][inBusiness]][bizType] != 1)
		return sendError(playerid,"You are not at the bank!");

    check_delay

    new money; if(sscanf(params, "d", money) || money < 1 || money >= 999999999)
        return sendSyntaxMessage(playerid, "/deposit [value]");

    if(GetMoney(playerid) < money && GetMilliards(playerid) < 1)
        return sendError(playerid, "Nu ai banii necesari in mana.");

    substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, money, false);
    addBankMoney(playerid, playerInfo[playerid][pAccountLY], playerInfo[playerid][pAccount], 0, money, false);

    va_mysql_tquery("UPDATE `users` SET `MoneyStore` = %d, `Money` = %d, `BankLY` = %d, `Bank` = %d WHERE `ID` = %d", GetMilliards(playerid), GetMoney(playerid), playerInfo[playerid][pAccountLY], playerInfo[playerid][pAccount], playerInfo[playerid][pSQLID]);

    set_delay;

    return SendClientMessage(playerid, -1, "You deposited succesfully $%s money in your bank account. New balance: $%s.", FormatNumber(money), formatMoney(playerInfo[playerid][pAccountLY], playerInfo[playerid][pAccount]));
}

YCMD:transfer(playerid, params[], help)
{
	if(bizInfo[playerStaticVar[playerid][inBusiness]][bizType] != 1)
		return SendClientMessage(playerid, -1, "You are not at the bank!");

	/*if(GetPVarInt(playerid, "transfer_dee") > gettime())
		return va_SendClientMessage(playerid, COLOR_GREY, "You can't use this command for %d seconds.", GetPVarInt(playerid, "transfer_dee") - gettime());*/

	if(playerInfo[playerid][pLevel] < 5)
		return SendClientMessage(playerid, -1, "You need level 5 to do this.");

	new returnPlayer, returnAmount;
	if(sscanf(params, "ud", returnPlayer, returnAmount) || returnAmount < 1 || returnAmount >= 999999999)
		return sendSyntaxMessage(playerid, "/transfer <Name/Playerid> <Amount of Money>");

	if(returnPlayer == playerid)
		return SendClientMessage(playerid, -1, "You can not transfer the money to you");

	if(!IsPlayerConnected(returnPlayer))
		return SendClientMessage(playerid, -1, "{FFFFCC}Error: Player not connected.");

	if(!playerStaticVar[returnPlayer][playerLogged])
		return SendClientMessage(playerid, COLOR_LIGHTRED, "This player need to login.");



    if(playerInfo[playerid][pAccount] < returnAmount && playerInfo[playerid][pAccountLY] < 1)
        return sendError(playerid, "Nu ai banii necesari in banca.");

	new Float: calculateTax = (0.5 / 100) * returnAmount,
		wTax = floatround(calculateTax);

	SetPVarInt(playerid, "wAmount", returnAmount);
	SetPVarInt(playerid, "wPlayer", returnPlayer);

	format(returnString, sizeof returnString, "Costul acestei tranzactii este de $%s.\nPrin urmare playerul %s va primi $%s.\nEsti sigur ca vrei sa-i transferi $%s jucatorului %s?", FormatNumber(wTax), GetName(returnPlayer), FormatNumber(returnAmount - wTax), FormatNumber(returnAmount), GetName(returnPlayer));
	Dialog_Show(playerid, DIALOG_TRANSFER, DIALOG_STYLE_MSGBOX, "Transfer:", returnString, "Yes", "No");
	
	SetPVarInt(playerid, "transfer_dee", gettime() + 60);
	return true;
}
Dialog:DIALOG_TRANSFER(playerid, response, listitem, inputtext[])
{
    if (!response)
        return DeletePVar(playerid, "wAmount"), DeletePVar(playerid, "wPlayer");

    new returnPlayer = GetPVarInt(playerid, "wPlayer"),
        returnAmount = GetPVarInt(playerid, "wAmount");

    new Float: calculateTax = (0.5 / 100) * returnAmount,
        wTax = floatround(calculateTax);

    if(playerInfo[playerid][pAccount] < returnAmount && playerInfo[playerid][pAccountLY] < 1)
        return sendError(playerid, "Nu ai banii necesari in banca.");

    substractBankMoney(playerid, playerInfo[playerid][pAccountLY], playerInfo[playerid][pAccount], 0, (returnAmount - wTax));
    addBankMoney(returnPlayer, playerInfo[returnPlayer][pAccountLY], playerInfo[returnPlayer][pAccount], 0, (returnAmount - wTax));

    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
    PlayerPlaySound(returnPlayer, 1052, 0.0, 0.0, 0.0);

    va_SendClientMessage(playerid, -1, "{52CC52}You have transferred $%s to %s's account.", FormatNumber(returnAmount - wTax), GetName(returnPlayer));
    va_SendClientMessage(returnPlayer, -1, "{52CC52}You have received $%s into your account from %s.", FormatNumber(returnAmount - wTax), GetName(playerid));

    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_YELLOW, "($) %s has transferred $%s to %s.", GetName(playerid), FormatNumber(returnAmount - wTax, ","), GetName(returnPlayer));
    insertPlayerLog(playerid, playerInfo[playerid][pSQLID], "($) %s has transferred $%s to %s.", GetName(playerid), FormatNumber(returnAmount), GetName(returnPlayer));

    return true;
}