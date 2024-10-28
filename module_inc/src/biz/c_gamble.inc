#include <YSI_Coding\y_hooks>
#include <YSI_Coding\y_timers>

enum ENUM_CASINO_INFORMATIONS
{
	gotSpecial[9], bool:playSlots, additionalRespin, spinTime, startSlot, fruitType[10], betMoney, totalWin
};
new casinoInfo[MAX_PLAYERS][ENUM_CASINO_INFORMATIONS], PlayerText:slotsPTD[MAX_PLAYERS][11], Text:slotsTD[9], Timer:roll_timer[MAX_PLAYERS];
static const ResetCasinoEnum[ENUM_CASINO_INFORMATIONS];

hook OnPlayerConnect(playerid)
{
	casinoInfo[playerid] = ResetCasinoEnum;

	new lineId, lineSlot;
	for(new i; i <= 8; i++)
	{
		if(i >= 0 && i <= 2) lineId = 1;
		if(i >= 3 && i <= 5) lineId = 2;
		if(i >= 6 && i <= 8) lineId = 3;

		slotsPTD[playerid][i] = CreatePlayerTextDraw(playerid, 408.666779 - (lineId * 61), 282.922211 - (lineSlot * 50), returnSlotFruit(random(5)));
		PlayerTextDrawLetterSize(playerid, slotsPTD[playerid][i], 0.000000, 0.000000);
		PlayerTextDrawTextSize(playerid, slotsPTD[playerid][i], 61.000000, 68.000000);
		PlayerTextDrawAlignment(playerid, slotsPTD[playerid][i], 1);
		PlayerTextDrawColour(playerid, slotsPTD[playerid][i], 0xFFFFFFFF);
		PlayerTextDrawUseBox(playerid, slotsPTD[playerid][i], 0);
		PlayerTextDrawBoxColour(playerid, slotsPTD[playerid][i], 0x80808080);
		PlayerTextDrawSetShadow(playerid, slotsPTD[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, slotsPTD[playerid][i], 0);
		PlayerTextDrawBackgroundColour(playerid, slotsPTD[playerid][i], 0x000000FF);
		PlayerTextDrawFont(playerid, slotsPTD[playerid][i], 4);
		PlayerTextDrawSetProportional(playerid, slotsPTD[playerid][i], 0);
		PlayerTextDrawSetSelectable(playerid, slotsPTD[playerid][i], 0);

		lineSlot ++;
		if(lineSlot == 3) lineSlot = 0;
	}
	slotsPTD[playerid][9] = CreatePlayerTextDraw(playerid, 490.000000, 117.000000 + 116, "_");
	PlayerTextDrawFont(playerid, slotsPTD[playerid][9], 1);
	PlayerTextDrawLetterSize(playerid, slotsPTD[playerid][9], 0.933332, 5.350002);
	PlayerTextDrawTextSize(playerid, slotsPTD[playerid][9], 401.000000, 80.000000);
	PlayerTextDrawSetOutline(playerid, slotsPTD[playerid][9], 1);
	PlayerTextDrawSetShadow(playerid, slotsPTD[playerid][9], 0);
	PlayerTextDrawAlignment(playerid, slotsPTD[playerid][9], 2);
	PlayerTextDrawColour(playerid, slotsPTD[playerid][9], -1);
	PlayerTextDrawBackgroundColour(playerid, slotsPTD[playerid][9], 200);
	PlayerTextDrawBoxColour(playerid, slotsPTD[playerid][9], -1962934017);
	PlayerTextDrawUseBox(playerid, slotsPTD[playerid][9], 1);
	PlayerTextDrawSetProportional(playerid, slotsPTD[playerid][9], 1);
	PlayerTextDrawSetSelectable(playerid, slotsPTD[playerid][9], 0);

	slotsPTD[playerid][10] = CreatePlayerTextDraw(playerid, 490.000000, 119.000000 + 116, "~n~FREE_RESPIN~n~_");
	PlayerTextDrawFont(playerid, slotsPTD[playerid][10], 2);
	PlayerTextDrawLetterSize(playerid, slotsPTD[playerid][10], 0.216000, 1.644999);
	PlayerTextDrawTextSize(playerid, slotsPTD[playerid][10], 402.500000, 76.000000);
	PlayerTextDrawSetOutline(playerid, slotsPTD[playerid][10], 1);
	PlayerTextDrawSetShadow(playerid, slotsPTD[playerid][10], 0);
	PlayerTextDrawAlignment(playerid, slotsPTD[playerid][10], 2);
	PlayerTextDrawColour(playerid, slotsPTD[playerid][10], -119273217);
	PlayerTextDrawBackgroundColour(playerid, slotsPTD[playerid][10], 2130706687);
	PlayerTextDrawBoxColour(playerid, slotsPTD[playerid][10], 1477579519);
	PlayerTextDrawUseBox(playerid, slotsPTD[playerid][10], 1);
	PlayerTextDrawSetProportional(playerid, slotsPTD[playerid][10], 1);
	PlayerTextDrawSetSelectable(playerid, slotsPTD[playerid][10], 0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnGameModeInit()
{
	slotsTD[0] = TextDrawCreate(225.0, 182.588959, "box");
	TextDrawLetterSize(slotsTD[0], 0.000000, 16.633978);
	TextDrawTextSize(slotsTD[0], 410.0, 0.000000);
	TextDrawAlignment(slotsTD[0], 1);
	TextDrawColour(slotsTD[0], 0xFFFFFFFF);
	TextDrawUseBox(slotsTD[0], 1);
	TextDrawBoxColour(slotsTD[0], 0x000000FF);
	TextDrawSetShadow(slotsTD[0], 0);
	TextDrawSetOutline(slotsTD[0], 0);
	TextDrawBackgroundColour(slotsTD[0], 0x000000FF);
	TextDrawFont(slotsTD[0], 1);
	TextDrawSetProportional(slotsTD[0], 1);
	TextDrawSetSelectable(slotsTD[0], 0);

	slotsTD[1] = TextDrawCreate(266.433228, 164.537170, "slots machine");
	TextDrawLetterSize(slotsTD[1], 0.400000, 1.600000);
	TextDrawTextSize(slotsTD[1], 1280.000000, 1280.000000);
	TextDrawAlignment(slotsTD[1], 1);
	TextDrawColour(slotsTD[1], 0xFFFFFFFF);
	TextDrawUseBox(slotsTD[1], 0);
	TextDrawBoxColour(slotsTD[1], 0x80808080);
	TextDrawSetShadow(slotsTD[1], 0);
	TextDrawSetOutline(slotsTD[1], 1);
	TextDrawBackgroundColour(slotsTD[1], 0x000000FF);
	TextDrawFont(slotsTD[1], 3);
	TextDrawSetProportional(slotsTD[1], 1);
	TextDrawSetSelectable(slotsTD[1], 0);

	slotsTD[2] = TextDrawCreate(318.000000, 339.000000, "_");
	TextDrawFont(slotsTD[2], 1);
	TextDrawLetterSize(slotsTD[2], 1.462500, 3.450001);
	TextDrawTextSize(slotsTD[2], 334.000000, 185.500000);
	TextDrawSetOutline(slotsTD[2], 2);
	TextDrawSetShadow(slotsTD[2], 0);
	TextDrawAlignment(slotsTD[2], 2);
	TextDrawColour(slotsTD[2], -1);
	TextDrawBackgroundColour(slotsTD[2], 255);
	TextDrawBoxColour(slotsTD[2], 255);
	TextDrawUseBox(slotsTD[2], 1);
	TextDrawSetProportional(slotsTD[2], 1);
	TextDrawSetSelectable(slotsTD[2], 0);

	slotsTD[3] = TextDrawCreate(318.000000, 338.000000, "ULTRA]STAR");
	TextDrawFont(slotsTD[3], 2);
	TextDrawLetterSize(slotsTD[3], 0.250000, 1.349999);
	TextDrawTextSize(slotsTD[3], 400.000000, 17.000000);
	TextDrawSetOutline(slotsTD[3], 1);
	TextDrawSetShadow(slotsTD[3], 0);
	TextDrawAlignment(slotsTD[3], 2);
	TextDrawColour(slotsTD[3], -1);
	TextDrawBackgroundColour(slotsTD[3], 255);
	TextDrawBoxColour(slotsTD[3], 50);
	TextDrawUseBox(slotsTD[3], 0);
	TextDrawSetProportional(slotsTD[3], 1);
	TextDrawSetSelectable(slotsTD[3], 0);

	slotsTD[4] = TextDrawCreate(378.000000, 355.000000, "SET BET");
	TextDrawFont(slotsTD[4], 2);
	TextDrawLetterSize(slotsTD[4], 0.220833, 1.000000);
	TextDrawTextSize(slotsTD[4], 15.500000, 51.000000);
	TextDrawSetOutline(slotsTD[4], 1);
	TextDrawSetShadow(slotsTD[4], 0);
	TextDrawAlignment(slotsTD[4], 2);
	TextDrawColour(slotsTD[4], -1);
	TextDrawBackgroundColour(slotsTD[4], 255);
	TextDrawBoxColour(slotsTD[4], 9109706);
	TextDrawUseBox(slotsTD[4], 1);
	TextDrawSetProportional(slotsTD[4], 1);
	TextDrawSetSelectable(slotsTD[4], 1);

	slotsTD[5] = TextDrawCreate(318.000000, 355.000000, "SPIN");
	TextDrawFont(slotsTD[5], 2);
	TextDrawLetterSize(slotsTD[5], 0.220833, 1.000000);
	TextDrawTextSize(slotsTD[5], 15.500000, 51.000000);
	TextDrawSetOutline(slotsTD[5], 1);
	TextDrawSetShadow(slotsTD[5], 0);
	TextDrawAlignment(slotsTD[5], 2);
	TextDrawColour(slotsTD[5], -1);
	TextDrawBackgroundColour(slotsTD[5], 255);
	TextDrawBoxColour(slotsTD[5], -16776961);
	TextDrawUseBox(slotsTD[5], 1);
	TextDrawSetProportional(slotsTD[5], 1);
	TextDrawSetSelectable(slotsTD[5], 1);

	slotsTD[6] = TextDrawCreate(257.000000, 355.000000, "PAYMENT");
	TextDrawFont(slotsTD[6], 2);
	TextDrawLetterSize(slotsTD[6], 0.220833, 1.000000);
	TextDrawTextSize(slotsTD[6], 15.500000, 51.000000);
	TextDrawSetOutline(slotsTD[6], 1);
	TextDrawSetShadow(slotsTD[6], 0);
	TextDrawAlignment(slotsTD[6], 2);
	TextDrawColour(slotsTD[6], -1);
	TextDrawBackgroundColour(slotsTD[6], 255);
	TextDrawBoxColour(slotsTD[6], 0xff4800FF);
	TextDrawUseBox(slotsTD[6], 1);
	TextDrawSetProportional(slotsTD[6], 1);
	TextDrawSetSelectable(slotsTD[6], 1);

	slotsTD[7] = TextDrawCreate(216.000000, 191.000000, "1~n~~n~4~n~~n~~n~2~n~~n~~n~3~n~~n~5");
	TextDrawFont(slotsTD[7], 2);
	TextDrawLetterSize(slotsTD[7], 0.258333, 1.399999);
	TextDrawTextSize(slotsTD[7], 400.000000, 17.000000);
	TextDrawSetOutline(slotsTD[7], 1);
	TextDrawSetShadow(slotsTD[7], 0);
	TextDrawAlignment(slotsTD[7], 2);
	TextDrawColour(slotsTD[7], -1);
	TextDrawBackgroundColour(slotsTD[7], 255);
	TextDrawBoxColour(slotsTD[7], 50);
	TextDrawUseBox(slotsTD[7], 0);
	TextDrawSetProportional(slotsTD[7], 1);
	TextDrawSetSelectable(slotsTD[7], 0);

	slotsTD[8] = TextDrawCreate(420.000000, 191.000000, "1~n~~n~5~n~~n~~n~2~n~~n~~n~3~n~~n~4");
	TextDrawFont(slotsTD[8], 2);
	TextDrawLetterSize(slotsTD[8], 0.258333, 1.399999);
	TextDrawTextSize(slotsTD[8], 400.000000, 17.000000);
	TextDrawSetOutline(slotsTD[8], 1);
	TextDrawSetShadow(slotsTD[8], 0);
	TextDrawAlignment(slotsTD[8], 2);
	TextDrawColour(slotsTD[8], -1);
	TextDrawBackgroundColour(slotsTD[8], 255);
	TextDrawBoxColour(slotsTD[8], 50);
	TextDrawUseBox(slotsTD[8], 0);
	TextDrawSetProportional(slotsTD[8], 1);
	TextDrawSetSelectable(slotsTD[8], 0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if(clickedid == Text:INVALID_TEXT_DRAW && casinoInfo[playerid][startSlot])
	{
		if(casinoInfo[playerid][spinTime] != 0)
			return SelectTextDraw(playerid, 0x5b6430AA);

		casinoInfo[playerid][startSlot] = 0;
		casinoInfo[playerid] = ResetCasinoEnum;

		HideMachineTextdraws(playerid);

		mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Money` = '%d', `BankLY` = '%d', `Bank` = '%d' where `id` = '%d';", playerInfo[playerid][pMoney], playerInfo[playerid][pAccountLY], playerInfo[playerid][pAccount], playerInfo[playerid][pSQLID]);
		mysql_tquery(SQL, returnString, "", "");
		return true;
	}
	if(clickedid == slotsTD[4])
	{
		if(casinoInfo[playerid][spinTime])
			return true;

		return Dialog_Show(playerid, DIALOG_BET, DIALOG_STYLE_INPUT, "Bet amount", "Insert amount of money in the box below\nValid bets are between $100.000 and $5.000.000", "Insert", "Cancel");
	}
	if(clickedid == slotsTD[5])
	{
		return WhenPlayerStartRolling(playerid);
	}
	if(clickedid == slotsTD[6])
	{
		if(!casinoInfo[playerid][betMoney])
			return SendClientMessage(playerid, -1, "You can't view the payment until you set a valid bet.");

		SendClientMessage(playerid, COLOR_GREEN, "-- -- PAYMENT -- --");
		sendSplitMessage(playerid, COLOR_DCHAT, "[STARS]: SUBSTITUTES for ALL symbols on ALL positions of the reel.");
		SendClientMessage(playerid, COLOR_DCHAT, "[STARS]: HOLDS the reel and triggers RESPIN of the reels. Appears only on reel 2.");
		
		for(new i; i <= 5; i++)
		{
			va_SendClientMessage(playerid, COLOR_GREEN, "[%s]: $%s / 1x line", fruitName(i), FormatNumber(fruitPayment(i, casinoInfo[playerid][betMoney])));
		}

		return SendClientMessage(playerid, COLOR_GREEN, "-- -- -- -- -- --");
	}

	return Y_HOOKS_CONTINUE_RETURN_1;
}

//START

timer player_roll_timer[500](playerid)
{
	if(!casinoInfo[playerid][startSlot])
	{
		stop roll_timer[playerid];
		roll_timer[playerid] = Timer:0;
	}
	else
	{
		WhenServerWaitForPlayerSpin(playerid);
	}
}

WhenPlayerStartRolling(playerid)
{
	if(casinoInfo[playerid][spinTime])
		return true;

	if(!casinoInfo[playerid][betMoney])
		return SendClientMessage(playerid, -1, "You need to set a bet first. Click set bet.");


	new moneySuffix = casinoInfo[playerid][betMoney], moneyPrefix = 0;

	if(playerInfo[playerid][pAccountLY] < moneyPrefix || playerInfo[playerid][pAccount] < moneySuffix && !playerInfo[playerid][pAccountLY])
		return SendClientMessage(playerid, -1, "You don't have enough money.");

    substractBankMoney(playerid, playerInfo[playerid][pAccountLY], playerInfo[playerid][pAccount], 0, moneySuffix); 



	for(new i; i <= 8; i++)
		casinoInfo[playerid][fruitType][i] = 0;

	if(casinoInfo[playerid][additionalRespin] == 1)
	{
		PlayerTextDrawHide(playerid, slotsPTD[playerid][9]);
		PlayerTextDrawHide(playerid, slotsPTD[playerid][10]);

		casinoInfo[playerid][fruitType][3] = 6, casinoInfo[playerid][fruitType][4] = 6, casinoInfo[playerid][fruitType][5] = 6;

		casinoInfo[playerid][additionalRespin] = 2;
	}
	else
	{
		for(new i = 3; i <= 5; i++)
		{
			PlayerTextDrawHide(playerid, slotsPTD[playerid][i]);

			new lineSlot;
			if(i == 0 || i == 3 || i == 6) lineSlot = 0;
			else if(i == 1 || i == 4 || i == 7) lineSlot = 1;
			else if(i == 2 || i == 5 || i == 8) lineSlot = 2;

			slotsPTD[playerid][i] = CreatePlayerTextDraw(playerid, 408.666779 - (2 * 61), 282.922211 - (lineSlot * 50), returnSlotFruit(random(5)));
			PlayerTextDrawLetterSize(playerid, slotsPTD[playerid][i], 0.000000, 0.000000);
			PlayerTextDrawTextSize(playerid, slotsPTD[playerid][i], 61.000000, 68.000000);
			PlayerTextDrawAlignment(playerid, slotsPTD[playerid][i], 1);
			PlayerTextDrawColour(playerid, slotsPTD[playerid][i], 0xFFFFFFFF);
			PlayerTextDrawUseBox(playerid, slotsPTD[playerid][i], 0);
			PlayerTextDrawBoxColour(playerid, slotsPTD[playerid][i], 0x80808080);
			PlayerTextDrawSetShadow(playerid, slotsPTD[playerid][i], 0);
			PlayerTextDrawSetOutline(playerid, slotsPTD[playerid][i], 0);
			PlayerTextDrawBackgroundColour(playerid, slotsPTD[playerid][i], 0x000000FF);
			PlayerTextDrawFont(playerid, slotsPTD[playerid][i], 4);
			PlayerTextDrawSetProportional(playerid, slotsPTD[playerid][i], 0);
			PlayerTextDrawSetSelectable(playerid, slotsPTD[playerid][i], 0);

			PlayerTextDrawShow(playerid, slotsPTD[playerid][i]);
			casinoInfo[playerid][gotSpecial][i] = 0;

			casinoInfo[playerid][totalWin] = 0;
		}
	}

	casinoInfo[playerid][spinTime] = 1;

	roll_timer[playerid] = repeat player_roll_timer(playerid);
	return true;
}


YCMD:gamble(playerid, params[], help)
{
	if(bizInfo[playerStaticVar[playerid][inBusiness]][bizType] != 9)
		return SendClientMessage(playerid, -1, "You can use this command only in casinos.");

	if(casinoInfo[playerid][startSlot])
		return SendClientMessage(playerid, -1, "You are already gambling.");

	for(new i; i <= 8; i++)
		PlayerTextDrawShow(playerid, slotsPTD[playerid][i]);

	for(new i; i <= 8; i++)
		TextDrawShowForPlayer(playerid, slotsTD[i]);

	casinoInfo[playerid][startSlot] = 1;

	SendClientMessage(playerid, -1, "Pentru un joc cat mai placut iti recomandam sa folosesti font-ul default pentru a nu intampina probleme.");
	SendClientMessage(playerid, -1, "Pentru a putea juca ai nevoie de bani in banca, in cazul unui castig acestia vor fi virati automat in contul tau bancar.");
	SendClientMessage(playerid, -1, "Jocul are 5 linii. Miza minima este de $100.000 iar cea maxima de $5.000.000. Mult noroc!");

	SelectTextDraw(playerid, 0x5b6430AA);
	return true;
}

returnSlotFruit(fruitId)
{
	new rString[32];
	switch(fruitId)
	{
		case 0: rString = "ld_slot:bar1_o";
		case 1: rString = "ld_slot:grapes";
		case 2: rString = "ld_slot:cherry";
		case 3: rString = "ld_slot:bell";
		case 4: rString = "ld_slot:r_69";
		case 5: rString = "ld_slot:bar2_o";
	}
	return rString;
}
fruitName(fruitId)
{
	new rString[12];
	switch(fruitId)
	{
		case 0: rString = "BAR 1";
		case 1: rString = "GRAPES";
		case 2: rString = "CHERRY";
		case 3: rString = "BELL";
		case 4: rString = "SPECIAL 69";
		case 5: rString = "BAR 2";
	}
	return rString;
}
fruitPayment(fruitId, spinAmount)
{
	switch(fruitId)
	{
		case 0: return spinAmount * 2;
		case 1: return spinAmount * 5;
		case 2: return spinAmount * 4;
		case 3: return spinAmount * 6;
		case 4: return spinAmount * 8;
		case 5: return spinAmount * 3;
		case 6: return spinAmount * 6; //STAR
	}
	return false;
}

UpdateSpecialStar(playerid, firstMove)
{
	new lineId, lineSlot;

	PlayerTextDrawHide(playerid, slotsPTD[playerid][firstMove]);
	if(firstMove >= 0 && firstMove <= 2) lineId = 1;
	if(firstMove >= 3 && firstMove <= 5) lineId = 2;
	if(firstMove >= 6 && firstMove <= 8) lineId = 3;

	if(firstMove == 0 || firstMove == 3 || firstMove == 6) lineSlot = 0;
	else if(firstMove == 1 || firstMove == 4 || firstMove == 7) lineSlot = 1;
	else if(firstMove == 2 || firstMove == 5 || firstMove == 8) lineSlot = 2;

	slotsPTD[playerid][firstMove] = CreatePlayerTextDraw(playerid, 408.666779 - (lineId * 61) + 30.65, 282.922211 - (lineSlot * 50) + 3.75, "]");
	PlayerTextDrawFont(playerid, slotsPTD[playerid][firstMove], 2);
	PlayerTextDrawLetterSize(playerid, slotsPTD[playerid][firstMove], 1.187502, 4.699999);
	PlayerTextDrawTextSize(playerid, slotsPTD[playerid][firstMove], 187.000000, 54.000000);
	PlayerTextDrawSetOutline(playerid, slotsPTD[playerid][firstMove], 1);
	PlayerTextDrawSetShadow(playerid, slotsPTD[playerid][firstMove], 0);
	PlayerTextDrawAlignment(playerid, slotsPTD[playerid][firstMove], 2);
	PlayerTextDrawColour(playerid, slotsPTD[playerid][firstMove], -65281);
	PlayerTextDrawBackgroundColour(playerid, slotsPTD[playerid][firstMove], 200);
	PlayerTextDrawBoxColour(playerid, slotsPTD[playerid][firstMove], 0xcf1515FF);
	PlayerTextDrawUseBox(playerid, slotsPTD[playerid][firstMove], 1);
	PlayerTextDrawSetProportional(playerid, slotsPTD[playerid][firstMove], 1);
	PlayerTextDrawSetSelectable(playerid, slotsPTD[playerid][firstMove], 0);

	PlayerTextDrawShow(playerid, slotsPTD[playerid][firstMove]);
	casinoInfo[playerid][fruitType][firstMove] = 6;
	return true;
}

WhenServerWaitForPlayerSpin(playerid)
{
	if(casinoInfo[playerid][spinTime] != 0)
		casinoInfo[playerid][spinTime] ++;

	if(casinoInfo[playerid][spinTime] < 5)
	{
		for(new i = 8; i >= 0; i--)
		{
			if(casinoInfo[playerid][fruitType][i] == 6)
				continue;

			WhenPlayerRollSlots(playerid, i, false);
		}
	}
	else
	{
		PlayerPlaySound(playerid, 1137, 0.0, 0.0, 0.0);

		new reelId, needReel;
		switch(casinoInfo[playerid][spinTime])
		{
			case 5: reelId = 8, needReel = 6;
			case 6: reelId = 5, needReel = 3;
			case 7: reelId = 2, needReel = 0;
		}

		for(new i = reelId; i >= needReel; i--)
		{
			if(casinoInfo[playerid][fruitType][i] == 6)
				continue;

			WhenPlayerRollSlots(playerid, i, true);
			if(!i) WhenMachineStopRolling(playerid);
		}
	}
	return true;
}
WhenMachineStopRolling(playerid)
{
	stop roll_timer[playerid];
	roll_timer[playerid] = Timer:0;

	casinoInfo[playerid][spinTime] = 0;

	if(casinoInfo[playerid][additionalRespin] == 1)
	{
		UpdateSpecialStar(playerid, 3), UpdateSpecialStar(playerid, 4), UpdateSpecialStar(playerid, 5);

		PlayerPlaySound(playerid, 4203, 0.0, 0.0, 0.0);
		va_SendClientMessage(playerid, COLOR_GREEN, "(+) $%s due to STARS reel. FREE RESPIN available", FormatNumber(fruitPayment(6, casinoInfo[playerid][betMoney])));

		PlayerTextDrawShow(playerid, slotsPTD[playerid][9]);
		PlayerTextDrawShow(playerid, slotsPTD[playerid][10]);

		casinoInfo[playerid][totalWin] += fruitPayment(6, casinoInfo[playerid][betMoney]);
	}
	else
	{
		new symbolId[3];
		for(new i; i < 5; i++)
		{
			switch(i)
			{
				case 0:
				{
					symbolId[0] = casinoInfo[playerid][fruitType][2];
					symbolId[1] = casinoInfo[playerid][fruitType][5];
					symbolId[2] = casinoInfo[playerid][fruitType][8];
				}
				case 1:
				{
					symbolId[0] = casinoInfo[playerid][fruitType][1];
					symbolId[1] = casinoInfo[playerid][fruitType][4];
					symbolId[2] = casinoInfo[playerid][fruitType][7];
				}
				case 2:
				{
					symbolId[0] = casinoInfo[playerid][fruitType][0];
					symbolId[1] = casinoInfo[playerid][fruitType][3];
					symbolId[2] = casinoInfo[playerid][fruitType][6];
				}
				case 3:
				{
					symbolId[0] = casinoInfo[playerid][fruitType][0];
					symbolId[1] = casinoInfo[playerid][fruitType][4];
					symbolId[2] = casinoInfo[playerid][fruitType][8];
				}
				case 4:
				{
					symbolId[0] = casinoInfo[playerid][fruitType][6];
					symbolId[1] = casinoInfo[playerid][fruitType][4];
					symbolId[2] = casinoInfo[playerid][fruitType][2];
				}
			}
			if((symbolId[0] == symbolId[2]) && symbolId[1] == symbolId[0] ||
				(symbolId[0] == symbolId[2]) && symbolId[1] == 6)
			{
				//va_SendClientMessage(playerid, -1, "%d, %d, %d", symbolId[0], symbolId[1], symbolId[2]);
				va_SendClientMessage(playerid, COLOR_GREEN, "(+) $%s due to 3x %s. (line %d)", FormatNumber(fruitPayment(symbolId[0], casinoInfo[playerid][betMoney])), fruitName(symbolId[0]), i + 1);
			
				casinoInfo[playerid][totalWin] += fruitPayment(symbolId[0], casinoInfo[playerid][betMoney]);
			}
		}
		if(casinoInfo[playerid][totalWin])
		{
			playerInfo[playerid][pAccount] += casinoInfo[playerid][totalWin];

			if(playerInfo[playerid][pAccount] >= 999999999)
			{
				playerInfo[playerid][pAccount] -= 999999999;
				playerInfo[playerid][pAccountLY] ++;
			}
			va_SendClientMessage(playerid, COLOR_GREEN, "[SLOTS]: WINNER PAID $%s | New balance: $%s", FormatNumber(casinoInfo[playerid][totalWin]), GetPlayerBank(playerid));
			PlayerPlaySound(playerid, 4201, 0, 0, 0);
		}
		else
			va_SendClientMessage(playerid, COLOR_RED, "[SLOTS]: NO WIN | New balance: $%s", GetPlayerBank(playerid));
	}
	return true;
}
WhenPlayerRollSlots(playerid, firstMove, stopSpinning = 0)
{
	switch(random(45))
	{
		case 0..8: PlayerTextDrawSetString(playerid, slotsPTD[playerid][firstMove], "ld_slot:bar1_o"), casinoInfo[playerid][fruitType][firstMove] = 0;
		case 9..14: PlayerTextDrawSetString(playerid, slotsPTD[playerid][firstMove], "ld_slot:grapes"), casinoInfo[playerid][fruitType][firstMove] = 1;
		case 15..20: PlayerTextDrawSetString(playerid, slotsPTD[playerid][firstMove], "ld_slot:cherry"), casinoInfo[playerid][fruitType][firstMove] = 2;
		case 21..25: PlayerTextDrawSetString(playerid, slotsPTD[playerid][firstMove], "ld_slot:bell"), casinoInfo[playerid][fruitType][firstMove] = 3;
		case 26..30: PlayerTextDrawSetString(playerid, slotsPTD[playerid][firstMove], "ld_slot:r_69"), casinoInfo[playerid][fruitType][firstMove] = 4;
		case 31..40: PlayerTextDrawSetString(playerid, slotsPTD[playerid][firstMove], "ld_slot:bar2_o"), casinoInfo[playerid][fruitType][firstMove] = 5;
		default:
		{
			if(casinoInfo[playerid][additionalRespin] || stopSpinning == 0 || firstMove < 3 || firstMove > 5)
				return PlayerTextDrawSetString(playerid, slotsPTD[playerid][firstMove], "ld_slot:bell"), casinoInfo[playerid][fruitType][firstMove] = 3;

			PlayerTextDrawHide(playerid, slotsPTD[playerid][firstMove]);
			UpdateSpecialStar(playerid, firstMove);

			casinoInfo[playerid][gotSpecial][firstMove] = 1;
			casinoInfo[playerid][additionalRespin] = 1;
		}
	}
	PlayerTextDrawShow(playerid, slotsPTD[playerid][firstMove]);
	return true;
}
HideMachineTextdraws(playerid)
{
	for(new i; i <= 8; i++)
	{
		TextDrawHideForPlayer(playerid, slotsTD[i]);
	}
	for(new i; i <= 10; i++)
	{
		PlayerTextDrawHide(playerid, slotsPTD[playerid][i]);
	}
	return true;
}

Dialog:DIALOG_BET(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	if(casinoInfo[playerid][spinTime])
		return true;

	if(strval(inputtext) < 100000 || strval(inputtext) > 5000000)
		return Dialog_Show(playerid, DIALOG_BET, DIALOG_STYLE_INPUT, "Bet amount", "INVALID BET:\n\nInsert amount of money in the box below\nValid bets are between $100.000 and $5.000.000", "Insert", "Cancel");

	casinoInfo[playerid][betMoney] = strval(inputtext);
	va_SendClientMessage(playerid, COLOR_GREEN, "[SLOTS]: BET updated to $%s.", FormatNumber(strval(inputtext)));
	return true;
}