#include <YSI_Coding\y_hooks>
#include <YSI_Coding\y_timers>

new Text:blackjackTD[5], PlayerText:dealerRPTD[MAX_PLAYERS][6], PlayerText:blackjackPTD[MAX_PLAYERS][6];
enum ENUM_BLACKJACK_INFO
{
	bStarted, inBlackjack, bBet, bCards[5], bScore, cardHit, dealerCards[5], dealerScore
};
new bjInfo[MAX_PLAYERS][ENUM_BLACKJACK_INFO];
static const resetBlackjack[ENUM_BLACKJACK_INFO];

YCMD:playblackjack(playerid, params[], help)
{
	if(bizInfo[playerStaticVar[playerid][inBusiness]][bizType] != 9)
		return sendError(playerid,"You can use this command only in casinos.");

	if(bjInfo[playerid][inBlackjack])
		return true;

	for(new i; i < 6; i++)
		PlayerTextDrawShow(playerid, blackjackPTD[playerid][i]);

	for(new i; i < 5; i++)
		TextDrawShowForPlayer(playerid, blackjackTD[i]);

	for(new i; i < 5; i++)
		PlayerTextDrawShow(playerid, dealerRPTD[playerid][i]);

	SelectTextDraw(playerid, 0x157cd6AA);
	SetBlackjack(playerid);
	return true;
}

cardColor(colorId)
{
	new randomColor[2];
	switch(colorId)
	{
		case 0: randomColor = "c";
		case 1: randomColor = "d";
		case 2: randomColor = "h";
		case 3: randomColor = "s";
	}
	return randomColor;
}

cardScore(cardId)
{
	new pshCard;
	switch(cardId)
	{
		case 10..13: pshCard = 10;
		default: pshCard = cardId;
	}
	return pshCard;
}

GiveCards(playerid, slotId)
{
	new stringTD[24], cardId = 2 + random(12);

	bjInfo[playerid][bCards][slotId] = cardId;
	bjInfo[playerid][bScore] += cardScore(cardId);

	va_PlayerTextDrawSetString(playerid, blackjackPTD[playerid][5], "score: %d~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~score: %d", bjInfo[playerid][dealerScore], bjInfo[playerid][bScore]);
	PlayerTextDrawShow(playerid, blackjackPTD[playerid][5]);

	format(stringTD, sizeof stringTD, "ld_card:cd%d%s", cardId, cardColor(random(4)));
	PlayerTextDrawSetString(playerid, blackjackPTD[playerid][slotId], stringTD);

	PlayerTextDrawShow(playerid, blackjackPTD[playerid][slotId]);
}

GiveDealerCards(playerid, slotId)
{
	new stringTD[24], dCard = 3 + random(11);

	bjInfo[playerid][dealerCards][slotId] = dCard;
	bjInfo[playerid][dealerScore] += cardScore(dCard);

	va_PlayerTextDrawSetString(playerid, blackjackPTD[playerid][5], "score: %d~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~score: %d", bjInfo[playerid][dealerScore], bjInfo[playerid][bScore]);
	PlayerTextDrawShow(playerid, blackjackPTD[playerid][5]);


	format(stringTD, sizeof stringTD, "ld_card:cd%d%s", dCard, cardColor(random(4)));
	PlayerTextDrawSetString(playerid, dealerRPTD[playerid][slotId], stringTD);

	PlayerTextDrawShow(playerid, dealerRPTD[playerid][slotId]);
}

LeaveBlackjack(playerid)
{
	for(new i; i < 6; i++)
		PlayerTextDrawHide(playerid, blackjackPTD[playerid][i]);

	for(new i; i < 5; i++)
		TextDrawHideForPlayer(playerid, blackjackTD[i]);

	for(new i; i < 5; i++)
		PlayerTextDrawHide(playerid, dealerRPTD[playerid][i]);

	bjInfo[playerid] = resetBlackjack;
}

SetBlackjack(playerid)
{
	PlayerTextDrawSetString(playerid, blackjackPTD[playerid][5], "score: 0~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~score: 0");
	PlayerTextDrawShow(playerid, blackjackPTD[playerid][5]);

	bjInfo[playerid] = resetBlackjack;
	bjInfo[playerid][inBlackjack] = 1;

	bjInfo[playerid][dealerScore] = 0;

	for(new i; i < 5; i++)
	{
		PlayerTextDrawSetString(playerid, blackjackPTD[playerid][i], "ld_card:cdback");
		PlayerTextDrawShow(playerid, blackjackPTD[playerid][i]);

		PlayerTextDrawSetString(playerid, dealerRPTD[playerid][i], "ld_card:cdback");
		PlayerTextDrawShow(playerid, dealerRPTD[playerid][i]);

		bjInfo[playerid][dealerCards][i] = 0;
	}
}

Dialog:DIALOG_MENU_BLACKJACK(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	if(!bjInfo[playerid][bStarted])
	{
		Dialog_Show(playerid, DIALOG_BET_BLACKJACK, DIALOG_STYLE_INPUT, "Blackjack Amount", "Please enter here the amount of money you want to play\nValid amounts($50.000-$5.000.000)", "Enter", "Cancel");
	}
	else
	{
		switch(listitem)
		{
			case 0:
			{
				LeaveBlackjack(playerid);
				CancelSelectTextDraw(playerid);
			}
			case 1:
			{
				if(bjInfo[playerid][dealerScore] < 16)
				{
					if(!bjInfo[playerid][cardHit]) GiveDealerCards(playerid, 2);
					else if(bjInfo[playerid][cardHit] == 1) GiveDealerCards(playerid, 3);
					else if(bjInfo[playerid][cardHit] == 2) GiveDealerCards(playerid, 4);
				}

				if(bjInfo[playerid][dealerScore] > 21)
				{
					SendClientMessage(playerid, -1, "The dealer was busted!");
					addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, bjInfo[playerid][bBet] * 2, false);
					
					SetBlackjack(playerid);
				}
				else if(bjInfo[playerid][bScore] < 22 && bjInfo[playerid][bScore] > bjInfo[playerid][dealerScore])
				{
					SendClientMessage(playerid, -1, "You won the match!");
					addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, bjInfo[playerid][bBet] * 2, false);
					SetBlackjack(playerid);
				}
				else if(bjInfo[playerid][dealerScore] == 21 || bjInfo[playerid][dealerScore] < 22 && bjInfo[playerid][dealerScore] >= bjInfo[playerid][bScore])
				{
					SendClientMessage(playerid, -1, "You loose!");
					SetBlackjack(playerid);
				}
			}
			case 2:
			{
				if(!bjInfo[playerid][cardHit]) GiveCards(playerid, 2);
				else if(bjInfo[playerid][cardHit] == 1) GiveCards(playerid, 3);
				else if(bjInfo[playerid][cardHit] == 2) GiveCards(playerid, 4);

				if(bjInfo[playerid][bScore] > 21)
				{
					SendClientMessage(playerid, -1, "You are busted!");
					SetBlackjack(playerid);
				}
				else if(bjInfo[playerid][bScore] == 21)
				{
					SendClientMessage(playerid, -1, "The dealer was busted!");
					addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, bjInfo[playerid][bBet] * 2, false);
					SetBlackjack(playerid);
				}
			}
		}
	}
	return true;
}


Dialog:DIALOG_BET_BLACKJACK(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	if(strval(inputtext) < 50000 || strval(inputtext) > 5000000)
	{
		va_SendClientMessage(playerid, -1, "Suma de $%s nu se incadreaza in limitele stabilite de catre administratie.", FormatNumber(strval(inputtext)));
		return Dialog_Show(playerid, DIALOG_BET_BLACKJACK, DIALOG_STYLE_INPUT, "Blackjack Amount", "Please enter here the amount of money you want to play\nValid amounts($50.000-$5.000.000)", "Enter", "Cancel");
	}

    if(playerInfo[playerid][pAccount] < strval(inputtext) && playerInfo[playerid][pAccountLY] < 1)
        return sendError(playerid,"Nu ai banii necesari pentru tranzactie in banca.");

	SendClientMessage(playerid, -1, "Meciul a inceput. Amandoi jucatorii au primit cate 2 carti.");
	SendClientMessage(playerid, -1, "You have 3 moves to beat the dealer.");

	bjInfo[playerid][bStarted] = 1;
	bjInfo[playerid][bBet] = strval(inputtext);

	substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, bjInfo[playerid][bBet]);


	GiveCards(playerid, 0), GiveCards(playerid, 1);
	GiveDealerCards(playerid, 0), GiveDealerCards(playerid, 1);
	return true;
}


hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if(clickedid == Text:INVALID_TEXT_DRAW && bjInfo[playerid][inBlackjack])
	{
		if(bjInfo[playerid][bStarted])
			return SelectTextDraw(playerid, 0x5b6430AA);

		LeaveBlackjack(playerid);
	}
	if(clickedid == blackjackTD[3])
	{
		new stringActions[128];

		if(!bjInfo[playerid][bStarted])
			stringActions = "Start match";
		else
			stringActions = "Quit Match\nStand\nHit";

		Dialog_Show(playerid, DIALOG_MENU_BLACKJACK, DIALOG_STYLE_LIST, "Actions:", stringActions, "Select", "Cancel");
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid)
{
	bjInfo[playerid] = resetBlackjack;

	for(new i; i < 5; i ++)
	{
		blackjackPTD[playerid][i] = CreatePlayerTextDraw(playerid, 275.000000 + (i * 49), 317.000000, "ld_card:cdback");
		PlayerTextDrawFont(playerid, blackjackPTD[playerid][i], 4);
		PlayerTextDrawLetterSize(playerid, blackjackPTD[playerid][i], 0.600000, 2.000000);
		PlayerTextDrawTextSize(playerid, blackjackPTD[playerid][i], 48.000000, 76.000000);
		PlayerTextDrawSetOutline(playerid, blackjackPTD[playerid][i], 1);
		PlayerTextDrawSetShadow(playerid, blackjackPTD[playerid][i], 0);
		PlayerTextDrawAlignment(playerid, blackjackPTD[playerid][i], 1);
		PlayerTextDrawColour(playerid, blackjackPTD[playerid][i], -1);
		PlayerTextDrawBackgroundColour(playerid, blackjackPTD[playerid][i], 255);
		PlayerTextDrawBoxColour(playerid, blackjackPTD[playerid][i], 50);
		PlayerTextDrawUseBox(playerid, blackjackPTD[playerid][i], 1);
		PlayerTextDrawSetProportional(playerid, blackjackPTD[playerid][i], 1);
		PlayerTextDrawSetSelectable(playerid, blackjackPTD[playerid][i], 0);
	}
	for(new i; i < 5; i ++)
	{
		dealerRPTD[playerid][i] = CreatePlayerTextDraw(playerid, 275.000000 + (i * 49), 215.000000, "ld_card:cdback");
		PlayerTextDrawFont(playerid, dealerRPTD[playerid][i], 4);
		PlayerTextDrawLetterSize(playerid, dealerRPTD[playerid][i], 0.600000, 2.000000);
		PlayerTextDrawTextSize(playerid, dealerRPTD[playerid][i], 48.000000, 76.000000);
		PlayerTextDrawSetOutline(playerid, dealerRPTD[playerid][i], 1);
		PlayerTextDrawSetShadow(playerid, dealerRPTD[playerid][i], 0);
		PlayerTextDrawAlignment(playerid, dealerRPTD[playerid][i], 1);
		PlayerTextDrawColour(playerid, dealerRPTD[playerid][i], -1);
		PlayerTextDrawBackgroundColour(playerid, dealerRPTD[playerid][i], 255);
		PlayerTextDrawBoxColour(playerid, dealerRPTD[playerid][i], 50);
		PlayerTextDrawUseBox(playerid, dealerRPTD[playerid][i], 1);
		PlayerTextDrawSetProportional(playerid, dealerRPTD[playerid][i], 1);
		PlayerTextDrawSetSelectable(playerid, dealerRPTD[playerid][i], 0);
	}

	blackjackPTD[playerid][5] = CreatePlayerTextDraw(playerid, 594.000000, 234.500000, "score: 0~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~score: 5");
	PlayerTextDrawFont(playerid, blackjackPTD[playerid][5], 3);
	PlayerTextDrawLetterSize(playerid, blackjackPTD[playerid][5], 0.366666, 1.450000);
	PlayerTextDrawTextSize(playerid, blackjackPTD[playerid][5], 401.500000, 147.000000);
	PlayerTextDrawSetOutline(playerid, blackjackPTD[playerid][5], 1);
	PlayerTextDrawSetShadow(playerid, blackjackPTD[playerid][5], 0);
	PlayerTextDrawAlignment(playerid, blackjackPTD[playerid][5], 2);
	PlayerTextDrawColour(playerid, blackjackPTD[playerid][5], -1);
	PlayerTextDrawBackgroundColour(playerid, blackjackPTD[playerid][5], 255);
	PlayerTextDrawBoxColour(playerid, blackjackPTD[playerid][5], 50);
	PlayerTextDrawUseBox(playerid, blackjackPTD[playerid][5], 0);
	PlayerTextDrawSetProportional(playerid, blackjackPTD[playerid][5], 1);
	PlayerTextDrawSetSelectable(playerid, blackjackPTD[playerid][5], 0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnGameModeInit()
{
	blackjackTD[0] = TextDrawCreate(420.000000, 198.000000, "_");
	TextDrawFont(blackjackTD[0], 1);
	TextDrawLetterSize(blackjackTD[0], 1.599998, 22.950023);
	TextDrawTextSize(blackjackTD[0], 298.500000, 414.000000);
	TextDrawSetOutline(blackjackTD[0], 1);
	TextDrawSetShadow(blackjackTD[0], 0);
	TextDrawAlignment(blackjackTD[0], 2);
	TextDrawColour(blackjackTD[0], -1);
	TextDrawBackgroundColour(blackjackTD[0], 255);
	TextDrawBoxColour(blackjackTD[0], 135);
	TextDrawUseBox(blackjackTD[0], 1);
	TextDrawSetProportional(blackjackTD[0], 1);
	TextDrawSetSelectable(blackjackTD[0], 0);

	blackjackTD[1] = TextDrawCreate(389.000000, 187.500000, "blackjack");
	TextDrawFont(blackjackTD[1], 3);
	TextDrawLetterSize(blackjackTD[1], 0.404166, 1.650000);
	TextDrawTextSize(blackjackTD[1], 400.000000, 17.000000);
	TextDrawSetOutline(blackjackTD[1], 1);
	TextDrawSetShadow(blackjackTD[1], 0);
	TextDrawAlignment(blackjackTD[1], 2);
	TextDrawColour(blackjackTD[1], -1);
	TextDrawBackgroundColour(blackjackTD[1], 255);
	TextDrawBoxColour(blackjackTD[1], 50);
	TextDrawUseBox(blackjackTD[1], 0);
	TextDrawSetProportional(blackjackTD[1], 1);
	TextDrawSetSelectable(blackjackTD[1], 0);

	blackjackTD[2] = TextDrawCreate(555.500000, 199.000000, "_");
	TextDrawFont(blackjackTD[2], 2);
	TextDrawLetterSize(blackjackTD[2], 0.600000, 22.750007);
	TextDrawTextSize(blackjackTD[2], 123.000000, 1.000000);
	TextDrawSetOutline(blackjackTD[2], 1);
	TextDrawSetShadow(blackjackTD[2], 0);
	TextDrawAlignment(blackjackTD[2], 2);
	TextDrawColour(blackjackTD[2], -1);
	TextDrawBackgroundColour(blackjackTD[2], 255);
	TextDrawBoxColour(blackjackTD[2], -1);
	TextDrawUseBox(blackjackTD[2], 1);
	TextDrawSetProportional(blackjackTD[2], 1);
	TextDrawSetSelectable(blackjackTD[2], 0);

	blackjackTD[3] = TextDrawCreate(594.000000, 297.000000, "actions");
	TextDrawFont(blackjackTD[3], 1);
	TextDrawLetterSize(blackjackTD[3], 0.400000, 1.700000);
	TextDrawTextSize(blackjackTD[3], 20.000000, 53.500000);
	TextDrawSetOutline(blackjackTD[3], 0);
	TextDrawSetShadow(blackjackTD[3], 0);
	TextDrawAlignment(blackjackTD[3], 2);
	TextDrawColour(blackjackTD[3], -1);
	TextDrawBackgroundColour(blackjackTD[3], 255);
	TextDrawBoxColour(blackjackTD[3], 50);
	TextDrawUseBox(blackjackTD[3], 0);
	TextDrawSetProportional(blackjackTD[3], 1);
	TextDrawSetSelectable(blackjackTD[3], 1);

	blackjackTD[4] = TextDrawCreate(229.500000, 241.000000, " dealer~n~~n~~n~~n~~n~~n~~n~~n~~n~   you");
	TextDrawFont(blackjackTD[4], 2);
	TextDrawLetterSize(blackjackTD[4], 0.204166, 1.400000);
	TextDrawTextSize(blackjackTD[4], 400.000000, 17.000000);
	TextDrawSetOutline(blackjackTD[4], 0);
	TextDrawSetShadow(blackjackTD[4], 0);
	TextDrawAlignment(blackjackTD[4], 1);
	TextDrawColour(blackjackTD[4], -1);
	TextDrawBackgroundColour(blackjackTD[4], 255);
	TextDrawBoxColour(blackjackTD[4], 50);
	TextDrawUseBox(blackjackTD[4], 0);
	TextDrawSetProportional(blackjackTD[4], 1);
	TextDrawSetSelectable(blackjackTD[4], 0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}