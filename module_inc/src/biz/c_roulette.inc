#include <YSI_Coding\y_hooks>
#include <YSI_Coding\y_timers>
#define ROULETTE_WAIT_TIME 60
#define ROULETTE_COLORS 3

new Text:rouletteTD[15], PlayerText:roulettePTD[MAX_PLAYERS], Timer: timerRoulette, Iterator:roulettePlayers[MAX_PLAYERS]<ROULETTE_COLORS>,
	placedBet[MAX_PLAYERS], selectedBet[MAX_PLAYERS], inRoulette[MAX_PLAYERS];

enum ENUM_ROULETTE_INFO
{
	rouletteTime, rouletteTime2, rouletteStarted, rColor[15]
};
new rInfo[ENUM_ROULETTE_INFO];
static const resetRoulette[ENUM_ROULETTE_INFO];

YCMD:playroulette(playerid, params[], help)
{
	if(bizInfo[playerStaticVar[playerid][inBusiness]][bizType] != 9)
		return sendError(playerid,"You can use this command only in casinos.");

	if(inRoulette[playerid])
		return true;

	for(new i; i < 15; i ++)
		TextDrawShowForPlayer(playerid, rouletteTD[i]);

    createRoullete(playerid);
	PlayerTextDrawShow(playerid, roulettePTD[playerid]);
	inRoulette[playerid] = 1;

	TogglePlayerControllable(playerid, false);
	SelectTextDraw(playerid, 0x5b6430AA);

	SendClientMessage(playerid, -1, "Meniul ruletei a fost afisat. Puteti paria pe una din cele 3 optiuni posibile(rosu,verde,negru)");
	SendClientMessage(playerid, -1, "Rosul si verdele dubleaza suma pariata. Negru o tripleaza.");
	SendClientMessage(playerid, -1, "Suma minima ce poate fi pariata este de $100.000 iar cea maxima de $10.000,000.");
	SendClientMessage(playerid, -1, "Odata ce o suma este pariata aceasta nu mai poate fi recuperata. Daca dai /q sau primesti crash dupa ce pariezi nu vei primi banii inapoi.");
	return true;
}

timer updateRoulette[1000]()
{
	if(rInfo[rouletteTime] != 0)
		rInfo[rouletteTime] ++;

	if(rInfo[rouletteTime] == ROULETTE_WAIT_TIME)
		rInfo[rouletteTime] = 0, rInfo[rouletteStarted] = 1;

	if(rInfo[rouletteStarted])
	{
		for(new i; i < 6; i ++)
		{	
			new colorId = random(ROULETTE_COLORS);
			rInfo[rColor][7 + i] = colorId;

			TextDrawBoxColour(rouletteTD[7 + i], getRouletteColor(colorId));
			UpdateRouletteTextdraws(7 + i);
		}
		rInfo[rouletteTime2] ++;

		if(rInfo[rouletteTime2] == 10)
			checkRouletteWin(rInfo[rColor][7 + 2]);
	}
	WhenRouletteUpdateUI();

	if(!Iter_Count(roulettePlayers[0]) && !Iter_Count(roulettePlayers[1]) && !Iter_Count(roulettePlayers[2]))
	{
		stop timerRoulette;
		timerRoulette = Timer:0;
    }
}

WhenRouletteUpdateUI()
{
	new rouletteString[128];
	if(rInfo[rouletteTime])
	{
		format(rouletteString, sizeof rouletteString, "ROUND STARTS IN: %s", returnTimeSeconds(ROULETTE_WAIT_TIME - rInfo[rouletteTime]));
		TextDrawSetString(rouletteTD[6], rouletteString);
	}
	else if(!rInfo[rouletteTime] && !rInfo[rouletteStarted]) TextDrawSetString(rouletteTD[6], "WAITING FOR BETS");
	else if(!rInfo[rouletteTime] &&rInfo[rouletteStarted]) TextDrawSetString(rouletteTD[6], "ROLLING");

	format(rouletteString, sizeof rouletteString, "%d_players_______________%d_players_________________%d_players", Iter_Count(roulettePlayers[0]), Iter_Count(roulettePlayers[1]), Iter_Count(roulettePlayers[2]));
	TextDrawSetString(rouletteTD[5], rouletteString);

	UpdateRouletteTextdraws(5), UpdateRouletteTextdraws(6);
}

getRouletteColor(rId)
{
	new colorRoulette;
	switch(rId)
	{
		case 0: colorRoulette = 0xbd3a2fFF;
		case 1: colorRoulette = 0x3c913cFF;
		case 2: colorRoulette = 0x000000FF;
	}
	return colorRoulette;
}

checkRouletteWin(winnerColor)
{
	for(new allPlayers; allPlayers < ROULETTE_COLORS; allPlayers++)
	{
		foreach(new i: roulettePlayers[allPlayers])
		{
			if(selectedBet[i] == winnerColor)
			{
				new winningMoney;
				if(selectedBet[i] == 2)
					winningMoney = (3 * placedBet[i]);
				else
					winningMoney = (2 * placedBet[i]);

				va_SendClientMessage(i, -1, "Ai castigat suma de $%s.", FormatNumber(winningMoney));
				addBankMoney(i, playerInfo[i][pAccountLY], playerInfo[i][pAccount], 0, winningMoney);
			}
			else
			{
				SendClientMessage(i, -1, "Din pacate nu ai fost castigator! Mai incearca");
				substractBankMoney(i, playerInfo[i][pAccountLY], playerInfo[i][pAccount], 0, placedBet[i]);
			}
			selectedBet[i] = 0;
			placedBet[i] = 0;

			PlayerTextDrawSetString(i, roulettePTD[i], "NO BET YET");
			PlayerTextDrawShow(i, roulettePTD[i]);
		}
		Iter_Clear(roulettePlayers[allPlayers]);
	}
	rInfo = resetRoulette;
}

UpdateRouletteTextdraws(tdId)
{
	for(new allPlayers; allPlayers < ROULETTE_COLORS; allPlayers++)
	{
		foreach(new i: roulettePlayers[allPlayers])
		{
			TextDrawShowForPlayer(i, rouletteTD[tdId]);
		}
	}
}
HideRouletteTextdraws(playerid)
{
	for(new i; i < 15; i ++)
		TextDrawHideForPlayer(playerid, rouletteTD[i]);

	PlayerTextDrawHide(playerid, roulettePTD[playerid]);
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if(clickedid == Text:INVALID_TEXT_DRAW && inRoulette[playerid])
	{
		if(placedBet[playerid])
			return SelectTextDraw(playerid, 0x5b6430AA);

		HideRouletteTextdraws(playerid);
		TogglePlayerControllable(playerid, true);

		selectedBet[playerid] = 0;
		placedBet[playerid] = 0;
		inRoulette[playerid] = 0;

		mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Money` = '%d', `BankLY` = '%d', `Bank` = '%d' where `id` = '%d';", playerInfo[playerid][pMoney], playerInfo[playerid][pAccountLY], playerInfo[playerid][pAccount], playerInfo[playerid][pSQLID]);
		mysql_tquery(SQL, returnString, "", "");
	}
	if(clickedid == rouletteTD[2] || clickedid == rouletteTD[3] || clickedid == rouletteTD[4])
	{
		if(placedBet[playerid])
			return sendError(playerid,"Ai pariat deja in aceasta sesiune!");

		Dialog_Show(playerid, DIALOG_BET_ROULETTE, DIALOG_STYLE_INPUT, "Roulette:", "Introdu in casuta de mai jos suma pe care doresti sa o pariezi\nSuma trebuie sa fie cuprinsa intre $100.00 si $1.000,000", "Bet", "Cancel");

		if(clickedid == rouletteTD[2]) selectedBet[playerid] = 0;
		if(clickedid == rouletteTD[3]) selectedBet[playerid] = 1;
		if(clickedid == rouletteTD[4]) selectedBet[playerid] = 2;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}
Dialog:DIALOG_BET_ROULETTE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	if(strval(inputtext) < 100000 || strval(inputtext) > 10000000)
	{
		va_SendClientMessage(playerid, -1, "Suma de $%s nu se incadreaza in limitele stabilite de catre administratie.", FormatNumber(strval(inputtext)));
		return Dialog_Show(playerid, DIALOG_BET_ROULETTE, DIALOG_STYLE_INPUT, "Roulette:", "Introdu in casuta de mai jos suma pe care doresti sa o pariezi\nSuma trebuie sa fie cuprinsa intre $100.00 si $10.000,000", "Bet", "Cancel");
	}


    if(playerInfo[playerid][pAccount] < strval(inputtext) && playerInfo[playerid][pAccountLY] < 1)
        return sendError(playerid,"Nu ai banii necesari pentru tranzactie in banca.");

	new betString[6];
	if(selectedBet[playerid] == 0) betString = "rosu";
	if(selectedBet[playerid] == 1) betString = "verde";
	if(selectedBet[playerid] == 2) betString = "negru";

	va_SendClientMessage(playerid, -1, "Ai pariat suma de $%s pe %s.", FormatNumber(strval(inputtext)), betString);

	if(selectedBet[playerid] == 0) betString = "RED";
	if(selectedBet[playerid] == 1) betString = "GREEN";
	if(selectedBet[playerid] == 2) betString = "BLACK";

	va_PlayerTextDrawSetString(playerid, roulettePTD[playerid], "BET %s$ ON %s", FormatNumber(strval(inputtext)), betString);
	PlayerTextDrawShow(playerid, roulettePTD[playerid]);

	placedBet[playerid] = strval(inputtext);
	Iter_Add(roulettePlayers[selectedBet[playerid]], playerid);

	if(timerRoulette == Timer:0)
	{
		rInfo[rouletteTime] = 1;
		timerRoulette = repeat updateRoulette();
	}
	return true;
}


hook OnGameModeInit()
{
	rouletteTD[0] = TextDrawCreate(306.000000, 250.000000, "_");
	TextDrawFont(rouletteTD[0], 1);
	TextDrawLetterSize(rouletteTD[0], 0.600000, 8.549996);
	TextDrawTextSize(rouletteTD[0], 298.500000, 246.000000);
	TextDrawSetOutline(rouletteTD[0], 1);
	TextDrawSetShadow(rouletteTD[0], 0);
	TextDrawAlignment(rouletteTD[0], 2);
	TextDrawColour(rouletteTD[0], -1);
	TextDrawBackgroundColour(rouletteTD[0], 255);
	TextDrawBoxColour(rouletteTD[0], 73);
	TextDrawUseBox(rouletteTD[0], 1);
	TextDrawSetProportional(rouletteTD[0], 1);
	TextDrawSetSelectable(rouletteTD[0], 0);

	rouletteTD[1] = TextDrawCreate(306.000000, 250.000000, "_");
	TextDrawFont(rouletteTD[1], 1);
	TextDrawLetterSize(rouletteTD[1], 0.600000, -0.250005);
	TextDrawTextSize(rouletteTD[1], 298.500000, 246.000000);
	TextDrawSetOutline(rouletteTD[1], 1);
	TextDrawSetShadow(rouletteTD[1], 0);
	TextDrawAlignment(rouletteTD[1], 2);
	TextDrawColour(rouletteTD[1], -1);
	TextDrawBackgroundColour(rouletteTD[1], 255);
	TextDrawBoxColour(rouletteTD[1], -1);
	TextDrawUseBox(rouletteTD[1], 1);
	TextDrawSetProportional(rouletteTD[1], 1);
	TextDrawSetSelectable(rouletteTD[1], 0);

	rouletteTD[2] = TextDrawCreate(208.000000, 291.500000, "BET_ON_RED");
	TextDrawFont(rouletteTD[2], 1);
	TextDrawLetterSize(rouletteTD[2], 0.258332, 1.149996);
	TextDrawTextSize(rouletteTD[2], 15.500000, 49.500000);
	TextDrawSetOutline(rouletteTD[2], 0);
	TextDrawSetShadow(rouletteTD[2], 0);
	TextDrawAlignment(rouletteTD[2], 2);
	TextDrawColour(rouletteTD[2], -1);
	TextDrawBackgroundColour(rouletteTD[2], 255);
	TextDrawBoxColour(rouletteTD[2], -1962934017);
	TextDrawUseBox(rouletteTD[2], 1);
	TextDrawSetProportional(rouletteTD[2], 1);
	TextDrawSetSelectable(rouletteTD[2], 1);

	rouletteTD[3] = TextDrawCreate(300.000000, 292.000000, "BET_ON_GREEN");
	TextDrawFont(rouletteTD[3], 1);
	TextDrawLetterSize(rouletteTD[3], 0.258332, 1.149996);
	TextDrawTextSize(rouletteTD[3], 15.500000, 59.000000);
	TextDrawSetOutline(rouletteTD[3], 0);
	TextDrawSetShadow(rouletteTD[3], 0);
	TextDrawAlignment(rouletteTD[3], 2);
	TextDrawColour(rouletteTD[3], -1);
	TextDrawBackgroundColour(rouletteTD[3], 255);
	TextDrawBoxColour(rouletteTD[3], 9109759);
	TextDrawUseBox(rouletteTD[3], 1);
	TextDrawSetProportional(rouletteTD[3], 1);
	TextDrawSetSelectable(rouletteTD[3], 1);

	rouletteTD[4] = TextDrawCreate(400.000000, 292.000000, "BET_ON_BLACK");
	TextDrawFont(rouletteTD[4], 1);
	TextDrawLetterSize(rouletteTD[4], 0.258332, 1.149996);
	TextDrawTextSize(rouletteTD[4], 15.500000, 59.000000);
	TextDrawSetOutline(rouletteTD[4], 0);
	TextDrawSetShadow(rouletteTD[4], 0);
	TextDrawAlignment(rouletteTD[4], 2);
	TextDrawColour(rouletteTD[4], -1);
	TextDrawBackgroundColour(rouletteTD[4], 255);
	TextDrawBoxColour(rouletteTD[4], 255);
	TextDrawUseBox(rouletteTD[4], 1);
	TextDrawSetProportional(rouletteTD[4], 1);
	TextDrawSetSelectable(rouletteTD[4], 1);

	rouletteTD[5] = TextDrawCreate(304.000000, 306.000000, "0_players_______________0_players_________________0_players");
	TextDrawFont(rouletteTD[5], 1);
	TextDrawLetterSize(rouletteTD[5], 0.229166, 1.050000);
	TextDrawTextSize(rouletteTD[5], 400.000000, 221.000000);
	TextDrawSetOutline(rouletteTD[5], 1);
	TextDrawSetShadow(rouletteTD[5], 0);
	TextDrawAlignment(rouletteTD[5], 2);
	TextDrawColour(rouletteTD[5], -1);
	TextDrawBackgroundColour(rouletteTD[5], 255);
	TextDrawBoxColour(rouletteTD[5], 50);
	TextDrawUseBox(rouletteTD[5], 0);
	TextDrawSetProportional(rouletteTD[5], 1);
	TextDrawSetSelectable(rouletteTD[5], 0);

	rouletteTD[6] = TextDrawCreate(180.000000, 320.000000, "WAITING FOR BETS");
	TextDrawFont(rouletteTD[6], 2);
	TextDrawLetterSize(rouletteTD[6], 0.233333, 1.149999);
	TextDrawTextSize(rouletteTD[6], 400.000000, 221.000000);
	TextDrawSetOutline(rouletteTD[6], 0);
	TextDrawSetShadow(rouletteTD[6], 0);
	TextDrawAlignment(rouletteTD[6], 1);
	TextDrawColour(rouletteTD[6], -1);
	TextDrawBackgroundColour(rouletteTD[6], 255);
	TextDrawBoxColour(rouletteTD[6], 50);
	TextDrawUseBox(rouletteTD[6], 0);
	TextDrawSetProportional(rouletteTD[6], 1);
	TextDrawSetSelectable(rouletteTD[6], 0);


	for(new i; i < 6; i ++)
	{
		rouletteTD[7 + i] = TextDrawCreate(200.000000 + (i * 42.5), 203.000000, "_");
		TextDrawFont(rouletteTD[7 + i], 1);
		TextDrawLetterSize(rouletteTD[7 + i], 0.579166, 4.549994);
		TextDrawTextSize(rouletteTD[7 + i], 296.500000, 34.000000);
		TextDrawSetOutline(rouletteTD[7 + i], 2);
		TextDrawSetShadow(rouletteTD[7 + i], 0);
		TextDrawAlignment(rouletteTD[7 + i], 2);
		TextDrawColour(rouletteTD[7 + i], -1);
		TextDrawBackgroundColour(rouletteTD[7 + i], -1962934017);
		TextDrawBoxColour(rouletteTD[7 + i], getRouletteColor(random(ROULETTE_COLORS)));
		TextDrawUseBox(rouletteTD[7 + i], 1);
		TextDrawSetProportional(rouletteTD[7 + i], 1);
		TextDrawSetSelectable(rouletteTD[7 + i], 0);
	}

	rouletteTD[13] = TextDrawCreate(305.000000, 180.500000, "Jackpot Roulette");
	TextDrawFont(rouletteTD[13], 0);
	TextDrawLetterSize(rouletteTD[13], 0.441666, 1.849999);
	TextDrawTextSize(rouletteTD[13], 396.000000, 221.500000);
	TextDrawSetOutline(rouletteTD[13], 1);
	TextDrawSetShadow(rouletteTD[13], 0);
	TextDrawAlignment(rouletteTD[13], 2);
	TextDrawColour(rouletteTD[13], -1);
	TextDrawBackgroundColour(rouletteTD[13], 255);
	TextDrawBoxColour(rouletteTD[13], 50);
	TextDrawUseBox(rouletteTD[13], 0);
	TextDrawSetProportional(rouletteTD[13], 1);
	TextDrawSetSelectable(rouletteTD[13], 0);

	rouletteTD[14] = TextDrawCreate(303.000000, 200.500000, "_");
	TextDrawFont(rouletteTD[14], 3);
	TextDrawLetterSize(rouletteTD[14], 0.408333, 4.850005);
	TextDrawTextSize(rouletteTD[14], 298.500000, -1.000000);
	TextDrawSetOutline(rouletteTD[14], 1);
	TextDrawSetShadow(rouletteTD[14], 0);
	TextDrawAlignment(rouletteTD[14], 2);
	TextDrawColour(rouletteTD[14], -1);
	TextDrawBackgroundColour(rouletteTD[14], 255);
	TextDrawBoxColour(rouletteTD[14], -1378294017);
	TextDrawUseBox(rouletteTD[14], 1);
	TextDrawSetProportional(rouletteTD[14], 1);
	TextDrawSetSelectable(rouletteTD[14], 0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid)
{	
	placedBet[playerid] = 0, selectedBet[playerid] = 0, inRoulette[playerid] = 0;
	roulettePTD[playerid] = CreatePlayerTextDraw(playerid, 432.000000, 320.000000, "NO BET YET");
	PlayerTextDrawFont(playerid, roulettePTD[playerid], 2);
	PlayerTextDrawLetterSize(playerid, roulettePTD[playerid], 0.233333, 1.149999);
	PlayerTextDrawTextSize(playerid, roulettePTD[playerid], 400.000000, 17.000000);
	PlayerTextDrawSetOutline(playerid, roulettePTD[playerid], 0);
	PlayerTextDrawSetShadow(playerid, roulettePTD[playerid], 0);
	PlayerTextDrawAlignment(playerid, roulettePTD[playerid], 3);
	PlayerTextDrawColour(playerid, roulettePTD[playerid], -1);
	PlayerTextDrawBackgroundColour(playerid, roulettePTD[playerid], 255);
	PlayerTextDrawBoxColour(playerid, roulettePTD[playerid], 50);
	PlayerTextDrawUseBox(playerid, roulettePTD[playerid], 0);
	PlayerTextDrawSetProportional(playerid, roulettePTD[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, roulettePTD[playerid], 0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

