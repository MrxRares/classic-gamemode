#include <YSI_Coding\y_hooks>

new Text:trade_td[13];
new PlayerText:trade_td_p[MAX_PLAYERS][21];

enum trade_e
{
	tradeOfferId, tradePlayer, tradeMoney[64], tradeVehicles[4], tradeStringItems[128], tradeTotalVehicles, tradeHouse, tradeBiz,
	tradeApartament,
	tradeTotalItems, tradeTimer, bool: tradeInMenu, bool: tradeReady
};
new trade_data[MAX_PLAYERS][trade_e];

hook OnPlayerConnect(playerid)
{
	tradeResetPlayerVars(playerid);

	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	if(trade_data[playerid][tradeInMenu] == true)
	{
		new
			targetid = trade_data[playerid][tradePlayer];

		trade_data[playerid][tradeInMenu] = false;
		trade_data[targetid][tradeInMenu] = false;

		for(new i; i < 13; ++i)
			TextDrawHideForPlayer(playerid, trade_td[i]), TextDrawHideForPlayer(targetid, trade_td[i]);

		for(new i; i < 21; ++i)
		{
			if(trade_td_p[playerid][i] != PlayerText: INVALID_TEXT_DRAW)
				PlayerTextDrawHide(playerid, trade_td_p[playerid][i]), PlayerTextDrawHide(targetid, trade_td_p[targetid][i]);
		}

		CancelSelectTextDraw(playerid), CancelSelectTextDraw(targetid);
		tradeResetPlayerVars(playerid), tradeResetPlayerVars(targetid);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnGameModeInit()
{
	trade_td[0] = TextDrawCreate(164.000061, 128.804443, "box");
	TextDrawLetterSize(trade_td[0], 0.000000, 30.094118);
	TextDrawTextSize(trade_td[0], 516.235413, 0.000000);
	TextDrawAlignment(trade_td[0], 1);
	TextDrawColour(trade_td[0], 0xFFFFFFFF);
	TextDrawUseBox(trade_td[0], 1);
	TextDrawBoxColour(trade_td[0], 0x00000077);
	TextDrawSetShadow(trade_td[0], 0);
	TextDrawSetOutline(trade_td[0], 0);
	TextDrawBackgroundColour(trade_td[0], 0x000000FF);
	TextDrawFont(trade_td[0], 1);
	TextDrawSetProportional(trade_td[0], 1);
	TextDrawSetSelectable(trade_td[0], 0);

	trade_td[1] = TextDrawCreate(164.353165, 129.333374, "TRADE");
	TextDrawLetterSize(trade_td[1], 0.255528, 1.599997);
	TextDrawTextSize(trade_td[1], 516.176514, 0.000000);
	TextDrawAlignment(trade_td[1], 1);
	TextDrawColour(trade_td[1], 0xFFFFFFFF);
	TextDrawUseBox(trade_td[1], 1);
	TextDrawBoxColour(trade_td[1], 0x000000FF);
	TextDrawSetShadow(trade_td[1], 0);
	TextDrawSetOutline(trade_td[1], 0);
	TextDrawBackgroundColour(trade_td[1], 0x000000FF);
	TextDrawFont(trade_td[1], 2);
	TextDrawSetProportional(trade_td[1], 1);
	TextDrawSetSelectable(trade_td[1], 0);

	trade_td[2] = TextDrawCreate(334.705994, 132.249985, "box");
	TextDrawLetterSize(trade_td[2], 0.000000, 29.717678);
	TextDrawTextSize(trade_td[2], 335.411804, 0.000000);
	TextDrawAlignment(trade_td[2], 1);
	TextDrawColour(trade_td[2], 0x0000A0FF);
	TextDrawUseBox(trade_td[2], 1);
	TextDrawBoxColour(trade_td[2], 0x000000FF);
	TextDrawSetShadow(trade_td[2], 0);
	TextDrawSetOutline(trade_td[2], 0);
	TextDrawBackgroundColour(trade_td[2], 0x000000FF);
	TextDrawFont(trade_td[2], 1);
	TextDrawSetProportional(trade_td[2], 1);
	TextDrawSetSelectable(trade_td[2], 0);

	trade_td[3] = TextDrawCreate(175.646973, 192.333282, "box");
	TextDrawLetterSize(trade_td[3], 0.000000, 2.894114);
	TextDrawTextSize(trade_td[3], 316.117584, 0.000000);
	TextDrawAlignment(trade_td[3], 1);
	TextDrawColour(trade_td[3], 0xFFFFFFFF);
	TextDrawUseBox(trade_td[3], 1);
	TextDrawBoxColour(trade_td[3], 0x00000088);
	TextDrawSetShadow(trade_td[3], 0);
	TextDrawSetOutline(trade_td[3], 0);
	TextDrawBackgroundColour(trade_td[3], 0x000000FF);
	TextDrawFont(trade_td[3], 1);
	TextDrawSetProportional(trade_td[3], 1);
	TextDrawSetSelectable(trade_td[3], 0);

	trade_td[4] = TextDrawCreate(175.646973, 240.166565, "box");
	TextDrawLetterSize(trade_td[4], 0.000000, 2.894114);
	TextDrawTextSize(trade_td[4], 316.588257, 0.000000);
	TextDrawAlignment(trade_td[4], 1);
	TextDrawColour(trade_td[4], 0xFFFFFFFF);
	TextDrawUseBox(trade_td[4], 1);
	TextDrawBoxColour(trade_td[4], 0x00000088);
	TextDrawSetShadow(trade_td[4], 0);
	TextDrawSetOutline(trade_td[4], 0);
	TextDrawBackgroundColour(trade_td[4], 0x000000FF);
	TextDrawFont(trade_td[4], 1);
	TextDrawSetProportional(trade_td[4], 1);
	TextDrawSetSelectable(trade_td[4], 0);

	trade_td[5] = TextDrawCreate(175.646973, 284.500061, "box");
	TextDrawLetterSize(trade_td[5], 0.000000, 2.894114);
	TextDrawTextSize(trade_td[5], 316.588409, 0.000000);
	TextDrawAlignment(trade_td[5], 1);
	TextDrawColour(trade_td[5], 0xFFFFFFFF);
	TextDrawUseBox(trade_td[5], 1);
	TextDrawBoxColour(trade_td[5], 0x00000088);
	TextDrawSetShadow(trade_td[5], 0);
	TextDrawSetOutline(trade_td[5], 0);
	TextDrawBackgroundColour(trade_td[5], 0x000000FF);
	TextDrawFont(trade_td[5], 1);
	TextDrawSetProportional(trade_td[5], 1);
	TextDrawSetSelectable(trade_td[5], 0);

	trade_td[6] = TextDrawCreate(175.646973, 330.583496, "box");
	TextDrawLetterSize(trade_td[6], 0.000000, 2.799994);
	TextDrawTextSize(trade_td[6], 314.705658, 0.000000);
	TextDrawAlignment(trade_td[6], 1);
	TextDrawColour(trade_td[6], 0xFFFFFFFF);
	TextDrawUseBox(trade_td[6], 1);
	TextDrawBoxColour(trade_td[6], 0x00000088);
	TextDrawSetShadow(trade_td[6], 0);
	TextDrawSetOutline(trade_td[6], 0);
	TextDrawBackgroundColour(trade_td[6], 0x000000FF);
	TextDrawFont(trade_td[6], 1);
	TextDrawSetProportional(trade_td[6], 1);
	TextDrawSetSelectable(trade_td[6], 0);

	trade_td[7] = TextDrawCreate(359.646973, 193.499939, "box");
	TextDrawLetterSize(trade_td[7], 0.000000, 2.894114);
	TextDrawTextSize(trade_td[7], 500.117523, 0.000000);
	TextDrawAlignment(trade_td[7], 1);
	TextDrawColour(trade_td[7], 0xFFFFFFFF);
	TextDrawUseBox(trade_td[7], 1);
	TextDrawBoxColour(trade_td[7], 0x00000088);
	TextDrawSetShadow(trade_td[7], 0);
	TextDrawSetOutline(trade_td[7], 0);
	TextDrawBackgroundColour(trade_td[7], 0x000000FF);
	TextDrawFont(trade_td[7], 1);
	TextDrawSetProportional(trade_td[7], 1);
	TextDrawSetSelectable(trade_td[7], 0);

	trade_td[8] = TextDrawCreate(359.646973, 240.166595, "box");
	TextDrawLetterSize(trade_td[8], 0.000000, 2.894114);
	TextDrawTextSize(trade_td[8], 501.058838, 0.000000);
	TextDrawAlignment(trade_td[8], 1);
	TextDrawColour(trade_td[8], 0xFFFFFFFF);
	TextDrawUseBox(trade_td[8], 1);
	TextDrawBoxColour(trade_td[8], 0x00000088);
	TextDrawSetShadow(trade_td[8], 0);
	TextDrawSetOutline(trade_td[8], 0);
	TextDrawBackgroundColour(trade_td[8], 0x000000FF);
	TextDrawFont(trade_td[8], 1);
	TextDrawSetProportional(trade_td[8], 1);
	TextDrawSetSelectable(trade_td[8], 0);

	trade_td[9] = TextDrawCreate(359.646973, 286.250061, "box");
	TextDrawLetterSize(trade_td[9], 0.000000, 2.894114);
	TextDrawTextSize(trade_td[9], 501.058685, 0.000000);
	TextDrawAlignment(trade_td[9], 1);
	TextDrawColour(trade_td[9], 0xFFFFFFFF);
	TextDrawUseBox(trade_td[9], 1);
	TextDrawBoxColour(trade_td[9], 0x00000088);
	TextDrawSetShadow(trade_td[9], 0);
	TextDrawSetOutline(trade_td[9], 0);
	TextDrawBackgroundColour(trade_td[9], 0x000000FF);
	TextDrawFont(trade_td[9], 1);
	TextDrawSetProportional(trade_td[9], 1);
	TextDrawSetSelectable(trade_td[9], 0);

	trade_td[10] = TextDrawCreate(359.646973, 328.249908, "box");
	TextDrawLetterSize(trade_td[10], 0.000000, 2.894114);
	TextDrawTextSize(trade_td[10], 501.058655, 0.000000);
	TextDrawAlignment(trade_td[10], 1);
	TextDrawColour(trade_td[10], 0xFFFFFFFF);
	TextDrawUseBox(trade_td[10], 1);
	TextDrawBoxColour(trade_td[10], 0x00000088);
	TextDrawSetShadow(trade_td[10], 0);
	TextDrawSetOutline(trade_td[10], 0);
	TextDrawBackgroundColour(trade_td[10], 0x000000FF);
	TextDrawFont(trade_td[10], 1);
	TextDrawSetProportional(trade_td[10], 1);
	TextDrawSetSelectable(trade_td[10], 0);

	trade_td[11] = TextDrawCreate(189.764648, 376.083313, "Sterge_tot");
	TextDrawLetterSize(trade_td[11], 0.202353, 1.436666);
	TextDrawTextSize(trade_td[11], 239.470459, 10.000000);
	TextDrawAlignment(trade_td[11], 1);
	TextDrawColour(trade_td[11], 0xFF0000FF);
	TextDrawUseBox(trade_td[11], 1);
	TextDrawBoxColour(trade_td[11], 0x00000000);
	TextDrawSetShadow(trade_td[11], 0);
	TextDrawSetOutline(trade_td[11], 0);
	TextDrawBackgroundColour(trade_td[11], 0x000000FF);
	TextDrawFont(trade_td[11], 2);
	TextDrawSetProportional(trade_td[11], 1);
	TextDrawSetSelectable(trade_td[11], 1);

	trade_td[12] = TextDrawCreate(501.235260, 128.166656, "LD_BEAT:cross");
	TextDrawLetterSize(trade_td[12], 0.000000, 0.000000);
	TextDrawTextSize(trade_td[12], 13.764682, 14.750006);
	TextDrawAlignment(trade_td[12], 1);
	TextDrawColour(trade_td[12], 0xFFFFFFFF);
	TextDrawUseBox(trade_td[12], 0);
	TextDrawBoxColour(trade_td[12], 0x80808080);
	TextDrawSetShadow(trade_td[12], 0);
	TextDrawSetOutline(trade_td[12], 0);
	TextDrawBackgroundColour(trade_td[12], 0x000000FF);
	TextDrawFont(trade_td[12], 4);
	TextDrawSetProportional(trade_td[12], 0);
	TextDrawSetSelectable(trade_td[12], 1);

	return Y_HOOKS_CONTINUE_RETURN_1;
}

init_trade_tds(playerid)
{
	trade_td_p[playerid][0] = CreatePlayerTextDraw(playerid, 177.529617, 157.333221, "EquiNoxAdv");
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][0], 0.210822, 1.757500);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][0], 1280.000000, 1280.000000);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][0], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][0], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][0], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][0], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][0], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][0], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][0], 0x000000FF);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][0], 2);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][0], 1);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][0], 0);

	trade_td_p[playerid][1] = CreatePlayerTextDraw(playerid, 361.058807, 154.416611, "KsenonAdv");
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][1], 0.210822, 1.757500);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][1], 1280.000000, 1280.000000);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][1], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][1], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][1], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][1], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][1], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][1], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][1], 0x000000FF);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][1], 2);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][1], 1);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][1], 0);

	trade_td_p[playerid][2] = CreatePlayerTextDraw(playerid, 221.764740, 199.333282, "Adauga");
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][2], 0.241880, 1.442499);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][2], 302.529175, 5.000000);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][2], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][2], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][2], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][2], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][2], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][2], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][2], 0x000000FF);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][2], 2);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][2], 1);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][2], 1);

	trade_td_p[playerid][3] = CreatePlayerTextDraw(playerid, 409.529388, 199.333267, "Nimic");
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][3], 0.241880, 1.442499);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][3], 1280.000000, 1280.000000);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][3], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][3], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][3], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][3], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][3], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][3], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][3], 0x000000FF);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][3], 2);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][3], 1);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][3], 0);

	trade_td_p[playerid][4] = CreatePlayerTextDraw(playerid, 222.235367, 246.583282, "Adauga");
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][4], 0.241880, 1.442499);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][4], 302.529175, 5.000000);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][4], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][4], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][4], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][4], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][4], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][4], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][4], 0x000000FF);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][4], 2);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][4], 1);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][4], 1);

	trade_td_p[playerid][5] = CreatePlayerTextDraw(playerid, 408.588348, 247.166626, "Nimic");
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][5], 0.241880, 1.442499);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][5], 1280.000000, 1280.000000);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][5], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][5], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][5], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][5], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][5], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][5], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][5], 0x000000FF);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][5], 2);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][5], 1);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][5], 0);

	trade_td_p[playerid][6] = CreatePlayerTextDraw(playerid, 221.294235, 289.750000, "Adauga");
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][6], 0.241880, 1.442499);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][6], 302.529175, 5.000000);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][6], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][6], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][6], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][6], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][6], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][6], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][6], 0x000000FF);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][6], 2);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][6], 1);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][6], 1);

	trade_td_p[playerid][7] = CreatePlayerTextDraw(playerid, 408.117706, 292.083344, "Nimic");
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][7], 0.241880, 1.442499);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][7], 1280.000000, 1280.000000);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][7], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][7], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][7], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][7], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][7], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][7], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][7], 0x000000FF);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][7], 2);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][7], 1);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][7], 0);

	trade_td_p[playerid][8] = CreatePlayerTextDraw(playerid, 219.882446, 334.666718, "Adauga");
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][8], 0.241880, 1.442499);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][8], 302.529175, 5.000000);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][8], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][8], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][8], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][8], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][8], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][8], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][8], 0x000000FF);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][8], 2);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][8], 1);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][8], 1);

	trade_td_p[playerid][9] = CreatePlayerTextDraw(playerid, 408.588257, 334.083374, "Nimic");
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][9], 0.241880, 1.442499);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][9], 1280.000000, 1280.000000);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][9], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][9], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][9], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][9], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][9], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][9], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][9], 0x000000FF);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][9], 2);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][9], 1);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][9], 0);

	trade_td_p[playerid][10] = CreatePlayerTextDraw(playerid, 269.223328, 376.666656, "Nepregatit");
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][10], 0.202353, 1.436666);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][10], 309.517426, 10.000000);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][10], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][10], 0xFFFF00FF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][10], 1);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][10], 0x00000000);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][10], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][10], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][10], 0x000000FF);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][10], 2);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][10], 1);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][10], 1);

	trade_td_p[playerid][11] = CreatePlayerTextDraw(playerid, 411.788361, 375.608704, "Nepregatit");
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][11], 0.208470, 1.355000);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][11], 1280.000000, 1280.000000);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][11], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][11], 0xFFFF00FF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][11], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][11], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][11], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][11], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][11], 0x000000FF);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][11], 2);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][11], 1);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][11], 0);

	trade_td_p[playerid][12] = CreatePlayerTextDraw(playerid, 178.882431, 190.0, ""); // 43
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][12], 0.000000, 0.000000);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][12], 32.117626, 29.333336);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][12], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][12], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][12], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][12], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][12], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][12], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][12], 0x00000000);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][12], 5);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][12], 0);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][12], 0);

	trade_td_p[playerid][13] = CreatePlayerTextDraw(playerid, 362.882477, 190.0, ""); // 47
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][13], 0.000000, 0.000000);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][13], 32.117626, 29.333336);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][13], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][13], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][13], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][13], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][13], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][13], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][13], 0x00000000);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][13], 5);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][13], 0);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][13], 0);


	trade_td_p[playerid][14] = CreatePlayerTextDraw(playerid, 178.882431, 237.583282, ""); // 43
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][14], 0.000000, 0.000000);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][14], 32.117626, 29.333336);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][14], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][14], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][14], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][14], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][14], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][14], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][14], 0x00000000);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][14], 5);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][14], 0);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][14], 0);
	PlayerTextDrawSetPreviewModel(playerid, trade_td_p[playerid][14], 1212);
	PlayerTextDrawSetPreviewRot(playerid, trade_td_p[playerid][14], 80.000000, 30.000000, 0.000000, 0.899999);

	trade_td_p[playerid][15] = CreatePlayerTextDraw(playerid, 362.882477, 238.166626, ""); // 47
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][15], 0.000000, 0.000000);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][15], 32.117626, 29.333336);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][15], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][15], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][15], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][15], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][15], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][15], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][15], 0x00000000);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][15], 5);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][15], 0);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][15], 0);
	PlayerTextDrawSetPreviewModel(playerid, trade_td_p[playerid][15], 1212);
	PlayerTextDrawSetPreviewRot(playerid, trade_td_p[playerid][15], 80.000000, 30.000000, 0.000000, 0.899999);

	trade_td_p[playerid][16] = CreatePlayerTextDraw(playerid, 178.882431, 280.750000, ""); // 43
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][16], 0.000000, 0.000000);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][16], 32.117626, 29.333336);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][16], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][16], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][16], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][16], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][16], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][16], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][16], 0x00000000);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][16], 5);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][16], 0);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][16], 0);
	PlayerTextDrawSetPreviewModel(playerid, trade_td_p[playerid][16], 1212);
	PlayerTextDrawSetPreviewRot(playerid, trade_td_p[playerid][16], 80.000000, 30.000000, 0.000000, 0.899999);

	trade_td_p[playerid][17] = CreatePlayerTextDraw(playerid, 362.882477, 283.083344, ""); // 43
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][17], 0.000000, 0.000000);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][17], 32.117626, 29.333336);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][17], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][17], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][17], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][17], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][17], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][17], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][17], 0x00000000);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][17], 5);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][17], 0);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][17], 0);
	PlayerTextDrawSetPreviewModel(playerid, trade_td_p[playerid][17], 1212);
	PlayerTextDrawSetPreviewRot(playerid, trade_td_p[playerid][17], 80.000000, 30.000000, 0.000000, 0.899999);

	trade_td_p[playerid][18] = CreatePlayerTextDraw(playerid, 178.882431, 325.666718, ""); // 47
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][18], 0.000000, 0.000000);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][18], 32.117626, 29.333336);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][18], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][18], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][18], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][18], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][18], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][18], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][18], 0x00000000);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][18], 5);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][18], 0);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][18], 0);
	PlayerTextDrawSetPreviewModel(playerid, trade_td_p[playerid][18], 1212);
	PlayerTextDrawSetPreviewRot(playerid, trade_td_p[playerid][18], 80.000000, 30.000000, 0.000000, 0.899999);

	trade_td_p[playerid][19] = CreatePlayerTextDraw(playerid, 362.882477, 325.083374, ""); // 43
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][19], 0.000000, 0.000000);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][19], 32.117626, 29.333336);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][19], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][19], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][19], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][19], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][19], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][19], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][19], 0x00000000);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][19], 5);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][19], 0);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][19], 0);
	PlayerTextDrawSetPreviewModel(playerid, trade_td_p[playerid][19], 1212);
	PlayerTextDrawSetPreviewRot(playerid, trade_td_p[playerid][19], 80.000000, 30.000000, 0.000000, 0.899999);

	trade_td_p[playerid][20] = CreatePlayerTextDraw(playerid, 325.294159, 126.416710, ":1");
	PlayerTextDrawLetterSize(playerid, trade_td_p[playerid][20], 0.311058, 1.967499);
	PlayerTextDrawTextSize(playerid, trade_td_p[playerid][20], 1280.000000, 1280.000000);
	PlayerTextDrawAlignment(playerid, trade_td_p[playerid][20], 1);
	PlayerTextDrawColour(playerid, trade_td_p[playerid][20], 0xFFFF00FF);
	PlayerTextDrawUseBox(playerid, trade_td_p[playerid][20], 0);
	PlayerTextDrawBoxColour(playerid, trade_td_p[playerid][20], 0x80808080);
	PlayerTextDrawSetShadow(playerid, trade_td_p[playerid][20], 0);
	PlayerTextDrawSetOutline(playerid, trade_td_p[playerid][20], 0);
	PlayerTextDrawBackgroundColour(playerid, trade_td_p[playerid][20], 0x000000FF);
	PlayerTextDrawFont(playerid, trade_td_p[playerid][20], 2);
	PlayerTextDrawSetProportional(playerid, trade_td_p[playerid][20], 1);
	PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][20], 0);

	return true;
}

tradeResetPlayerVars(playerid)
{
	trade_data[playerid][tradeOfferId] = -1;
	trade_data[playerid][tradePlayer] = -1;
	trade_data[playerid][tradeInMenu] = false;
	trade_data[playerid][tradeReady] = false;
	trade_data[playerid][tradeTotalItems] = 0;
	trade_data[playerid][tradeTotalVehicles] = 0;

	trade_data[playerid][tradeHouse] = 0;
	trade_data[playerid][tradeBiz] = 0;
	trade_data[playerid][tradeApartament] = 0;

	for(new i; i < 4; ++i)
		trade_data[playerid][tradeVehicles][i] = 0;

	KillTimer(trade_data[playerid][tradeTimer]);

	DeletePVar(playerid, "trade_process_time");
	DeletePVar(playerid, "trade_slotid");

	format(trade_data[playerid][tradeMoney], 32, "null");
	format(trade_data[playerid][tradeStringItems], 128, "");
	return true;
}

init_trade(playerid)
{
	if(trade_data[playerid][tradeInMenu] == true)
		return true;

	init_trade_tds(playerid);

	trade_data[playerid][tradeInMenu] = true;

	// update nume playerid
	format(returnString, sizeof returnString, "%s", GetName(playerid));
	PlayerTextDrawSetString(playerid, trade_td_p[playerid][0], returnString);

	// update nume trade player
	format(returnString, sizeof returnString, "%s", GetName(trade_data[playerid][tradePlayer]));
	PlayerTextDrawSetString(playerid, trade_td_p[playerid][1], returnString);

	for(new i; i < 13; ++i)
		TextDrawShowForPlayer(playerid, trade_td[i]);

	for(new i; i < 12; ++i)
		PlayerTextDrawShow(playerid, trade_td_p[playerid][i]);

	SelectTextDraw(playerid, 0x00ff00FF);

	return true;
}

insertTradeItem(playerid, itemtype)
{
	new
		slot = GetPVarInt(playerid, "trade_slotid"), targetid = trade_data[playerid][tradePlayer];

	switch(itemtype)
	{
		case 1: // bani
		{
		
			new	moneySuffix = 0, moneyPrefix = 0;
    		CreateLocalLY(moneyPrefix, moneySuffix, trade_data[playerid][tradeMoney]);

    		new stringMoney[64], strMon[64];
			string_FromatValue(moneyPrefix, moneySuffix, stringMoney);
			format(strMon, 64, FormatNumberLYEx(stringMoney));

			format(trade_data[playerid][tradeStringItems], 128, "%s $%s", trade_data[playerid][tradeStringItems], strMon);

			PlayerTextDrawSetString(playerid, trade_td_p[playerid][slot], strMon), PlayerTextDrawSetString(targetid, trade_td_p[targetid][slot+1], strMon);
			PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][slot], 0);
			PlayerTextDrawShow(playerid, trade_td_p[playerid][slot]);

			PlayerTextDrawSetPreviewModel(playerid, trade_td_p[playerid][10+slot], 1212), PlayerTextDrawSetPreviewModel(targetid, trade_td_p[targetid][11+slot], 1212);
			PlayerTextDrawSetPreviewRot(playerid, trade_td_p[playerid][10+slot], 80.000000, 30.000000, 0.000000, 0.899999), PlayerTextDrawSetPreviewRot(targetid, trade_td_p[targetid][11+slot], 80.000000, 30.000000, 0.000000, 0.899999);
			PlayerTextDrawShow(playerid, trade_td_p[playerid][10+slot]), PlayerTextDrawShow(targetid, trade_td_p[targetid][11+slot]);

		}
		case 2: // masina
		{
			new
				countTVehs = trade_data[playerid][tradeTotalVehicles],
				vehicleDBId = trade_data[playerid][tradeVehicles][countTVehs];

			va_SendClientMessage(playerid, COLOR_GREY, "Ai adaugat vehiculul %s cu urmatoarele caracteristici: %.0f KM, %d zile, %s culori %d %d.", aVehicleNames[carInfo[vehicleDBId][cModel] - 400], carInfo[vehicleDBId][cKM], getDaysFromTimestamp(carInfo[vehicleDBId][cDays]), (carInfo[vehicleDBId][cVIP] ? (", VIP") : ("")), carInfo[vehicleDBId][cColor][0], carInfo[vehicleDBId][cColor][1]);
			va_SendClientMessage(targetid, COLOR_GREY, "%s a adaugat vehiculul %s cu urmatoarele caracteristici: %.0f KM, %d zile, %s culori %d %d.", GetName(playerid), aVehicleNames[carInfo[vehicleDBId][cModel] - 400], carInfo[vehicleDBId][cKM], getDaysFromTimestamp(carInfo[vehicleDBId][cDays]), (carInfo[vehicleDBId][cVIP] ? (", VIP") : ("")), carInfo[vehicleDBId][cColor][0], carInfo[vehicleDBId][cColor][1]);

			format(returnString, sizeof returnString, "%s%s", aVehicleNames[carInfo[vehicleDBId][cModel] - 400], (carInfo[vehicleDBId][cVIP] ? (" (VIP)") : ("")));

			PlayerTextDrawSetString(playerid, trade_td_p[playerid][slot], returnString), PlayerTextDrawSetString(targetid, trade_td_p[targetid][slot+1], returnString);
			PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][slot], 0);
			PlayerTextDrawShow(playerid, trade_td_p[playerid][slot]);

			PlayerTextDrawSetPreviewModel(playerid, trade_td_p[playerid][10+slot], carInfo[vehicleDBId][cModel]), PlayerTextDrawSetPreviewModel(targetid, trade_td_p[targetid][11+slot], carInfo[vehicleDBId][cModel]);
			PlayerTextDrawSetPreviewRot(playerid, trade_td_p[playerid][10+slot], 0.000000, 0.000000, 30.000000, 0.899999), PlayerTextDrawSetPreviewRot(targetid, trade_td_p[targetid][11+slot], 0.000000, 0.000000, 30.000000, 0.899999);
			PlayerTextDrawSetPreviewVehicleColours(playerid, trade_td_p[playerid][10+slot], 1, 1), PlayerTextDrawSetPreviewVehicleColours(targetid, trade_td_p[targetid][11+slot], 1, 1);
			PlayerTextDrawShow(playerid, trade_td_p[playerid][10+slot]), PlayerTextDrawShow(targetid, trade_td_p[targetid][11+slot]);
			
			trade_data[playerid][tradeTotalVehicles] ++;
		
			format(trade_data[playerid][tradeStringItems], 128, "%s [car:%d]", trade_data[playerid][tradeStringItems], vehicleDBId);
		}
		case 3:
		{
			format(returnString, sizeof returnString, "biz-ul %d", trade_data[playerid][tradeBiz]);

			format(trade_data[playerid][tradeStringItems], 128, "%s [biz:%d]", trade_data[playerid][tradeStringItems], trade_data[playerid][tradeBiz]);

			PlayerTextDrawSetString(playerid, trade_td_p[playerid][slot], returnString), PlayerTextDrawSetString(targetid, trade_td_p[targetid][slot+1], returnString);
			PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][slot], 0);
			PlayerTextDrawShow(playerid, trade_td_p[playerid][slot]);

			PlayerTextDrawSetPreviewModel(playerid, trade_td_p[playerid][10+slot], 1274), PlayerTextDrawSetPreviewModel(targetid, trade_td_p[targetid][11+slot], 1274);
			PlayerTextDrawSetPreviewRot(playerid, trade_td_p[playerid][10+slot], 0.000000, 0.000000, 180.000000, 0.899999), PlayerTextDrawSetPreviewRot(targetid, trade_td_p[targetid][11+slot], 0.000000, 0.000000, 180.000000, 0.899999);
			PlayerTextDrawShow(playerid, trade_td_p[playerid][10+slot]), PlayerTextDrawShow(targetid, trade_td_p[targetid][11+slot]);
		}
		case 4:
		{
			format(returnString, sizeof returnString, "casa %d", trade_data[playerid][tradeHouse]);

			format(trade_data[playerid][tradeStringItems], 128, "%s [house:%d]", trade_data[playerid][tradeStringItems], trade_data[playerid][tradeHouse]);

			PlayerTextDrawSetString(playerid, trade_td_p[playerid][slot], returnString), PlayerTextDrawSetString(targetid, trade_td_p[targetid][slot+1], returnString);
			PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][slot], 0);
			PlayerTextDrawShow(playerid, trade_td_p[playerid][slot]);

			PlayerTextDrawSetPreviewModel(playerid, trade_td_p[playerid][10+slot], 1272), PlayerTextDrawSetPreviewModel(targetid, trade_td_p[targetid][11+slot], 1272);
			PlayerTextDrawSetPreviewRot(playerid, trade_td_p[playerid][10+slot], 0.000000, 0.000000, 180.000000, 0.899999), PlayerTextDrawSetPreviewRot(targetid, trade_td_p[targetid][11+slot], 0.000000, 0.000000, 180.000000, 0.899999);
			PlayerTextDrawShow(playerid, trade_td_p[playerid][10+slot]), PlayerTextDrawShow(targetid, trade_td_p[targetid][11+slot]);
		}
		case 5:
		{
			format(returnString, sizeof returnString, "apartamentul %d", trade_data[playerid][tradeApartament]);

			format(trade_data[playerid][tradeStringItems], 128, "%s [apartment:%d]", trade_data[playerid][tradeStringItems], trade_data[playerid][tradeHouse]);

			PlayerTextDrawSetString(playerid, trade_td_p[playerid][slot], returnString), PlayerTextDrawSetString(targetid, trade_td_p[targetid][slot+1], returnString);
			PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][slot], 0);
			PlayerTextDrawShow(playerid, trade_td_p[playerid][slot]);

			PlayerTextDrawSetPreviewModel(playerid, trade_td_p[playerid][10+slot], 1273), PlayerTextDrawSetPreviewModel(targetid, trade_td_p[targetid][11+slot], 1273);
			PlayerTextDrawSetPreviewRot(playerid, trade_td_p[playerid][10+slot], 0.000000, 0.000000, 180.000000, 0.899999), PlayerTextDrawSetPreviewRot(targetid, trade_td_p[targetid][11+slot], 0.000000, 0.000000, 180.000000, 0.899999);
			PlayerTextDrawShow(playerid, trade_td_p[playerid][10+slot]), PlayerTextDrawShow(targetid, trade_td_p[targetid][11+slot]);
		}
	}
	trade_data[playerid][tradeTotalItems] ++;

	return DeletePVar(playerid, "trade_slotid");
}

changeReadyStatus(playerid, bool: status)
{
	new 
		targetid = trade_data[playerid][tradePlayer];

	if(GetPVarInt(playerid, "trade_process_time") || GetPVarInt(targetid, "trade_process_time"))
		sendError(playerid,"Nu poti face asta acum!");
	
	if(trade_data[playerid][tradeTotalItems] == 0 && status == true)
		sendError(playerid,"Nu poti accepta deoarece nu ai oferit nimic!");

	if(trade_data[targetid][tradeTotalVehicles] != 0)
	{
		if(trade_data[playerid][tradeTotalVehicles] != 0)
		{
			if(GetPCars(playerid) - trade_data[playerid][tradeTotalVehicles] + trade_data[targetid][tradeTotalVehicles] > playerInfo[playerid][pCarSlots])
				sendError(playerid,"Tranzactia nu poate avea loc! Nu ai suficiente sloturi de vehicule! Refaceti oferta!");
		}
		else
		{
			if(GetPCars(playerid) + trade_data[targetid][tradeTotalVehicles] > playerInfo[playerid][pCarSlots])
				sendError(playerid,"Tranzactia nu poate avea loc! Nu ai suficiente sloturi de vehicule! Refaceti oferta!");
		}
	}
	else if(trade_data[playerid][tradeTotalVehicles] != 0)
	{
		if(trade_data[targetid][tradeTotalVehicles] != 0)
		{
			if(GetPCars(targetid) - trade_data[targetid][tradeTotalVehicles] + trade_data[playerid][tradeTotalVehicles] > playerInfo[targetid][pCarSlots])
				sendError(playerid,"Tranzactia nu poate avea loc! Celalalt jucator nu are suficiente sloturi de vehicule!");
		}
		else
		{
			if(GetPCars(targetid) + trade_data[playerid][tradeTotalVehicles] > playerInfo[targetid][pCarSlots])
				sendError(playerid,"Tranzactia nu poate avea loc! Celalalt jucator nu are suficiente sloturi de vehicule!");
		}
	}

	if(!strmatch(trade_data[playerid][tradeMoney], "null") && !strmatch(trade_data[targetid][tradeMoney], "null") && trade_data[playerid][tradeTotalItems] == 1 && trade_data[targetid][tradeTotalItems] == 1 && status == true)
		sendError(playerid,"Tranzactiile de tip Bani-Bani nu pot avea loc!");

	if(trade_data[playerid][tradeHouse] && !trade_data[targetid][tradeHouse] && houseInfo[playerInfo[targetid][pHouse]][houseOwner] == playerInfo[targetid][pSQLID])
		sendError(playerid,"Tranzactia nu poate avea loc! Acest jucator deja detine o casa.");

	if(trade_data[playerid][tradeBiz] && !trade_data[targetid][tradeBiz] && bizInfo[playerInfo[targetid][pBusiness]][bizOwner] == playerInfo[targetid][pSQLID])
		sendError(playerid,"Tranzactia nu poate avea loc! Acest jucator deja detine o afacere.");

	if(trade_data[playerid][tradeApartament] && !trade_data[targetid][tradeApartament] && apInfo[playerInfo[targetid][pApartament]][apOwner] == playerInfo[targetid][pSQLID])
		sendError(playerid,"Tranzactia nu poate avea loc! Acest jucator deja detine un apartament.");

	PlayerTextDrawSetString(playerid, trade_td_p[playerid][10], (status == true ? ("PREGATIT") : ("NEPREGATIT"))), PlayerTextDrawSetString(targetid, trade_td_p[targetid][11], (status == true ? ("PREGATIT") : ("NEPREGATIT")));
	PlayerTextDrawColour(playerid, trade_td_p[playerid][10], (status == true ? (COLOR_GREEN) : (0xFFFF00FF))), PlayerTextDrawColour(targetid, trade_td_p[targetid][11], (status == true ? (COLOR_GREEN) : (0xFFFF00FF)));
	PlayerTextDrawShow(playerid, trade_td_p[playerid][10]), PlayerTextDrawShow(targetid, trade_td_p[targetid][11]);

	trade_data[playerid][tradeReady] = (status == true ? (true) : (false));

	return check_traders_status(playerid, targetid);
}

check_traders_status(playerid, targetid)
{
	if(trade_data[playerid][tradeReady] == true && trade_data[targetid][tradeReady] == true)
	{
		returnString = "Schimbul de bunuri va fi efectuat in 10 secunde!";
		SendClientMessage(playerid, -1, returnString), SendClientMessage(targetid, -1, returnString);
	
		PlayerTextDrawSetString(playerid, trade_td_p[playerid][20], ":10"), PlayerTextDrawShow(playerid, trade_td_p[playerid][20]);
		PlayerTextDrawSetString(targetid, trade_td_p[targetid][20], ":10"), PlayerTextDrawShow(targetid, trade_td_p[targetid][20]);

		trade_data[playerid][tradeTimer] = SetTimerEx("process_trade", 1000, 1, "d", playerid);
		SetPVarInt(playerid, "trade_process_time", gettime() + 9);
	}
	return true;
}

function:process_trade(playerid)
{
	new
		targetid = trade_data[playerid][tradePlayer];

	format(returnString, sizeof returnString, ":%d", GetPVarInt(playerid, "trade_process_time") - gettime());

	PlayerTextDrawSetString(playerid, trade_td_p[playerid][20], returnString);
	PlayerTextDrawSetString(targetid, trade_td_p[targetid][20], returnString);

	if(GetPVarInt(playerid, "trade_process_time") <= gettime())
	{
		KillTimer(trade_data[playerid][tradeTimer]);

		for(new i; i < 13; ++i)
			TextDrawHideForPlayer(playerid, trade_td[i]), TextDrawHideForPlayer(targetid, trade_td[i]);

		for(new i; i < 21; ++i)
		{
			if(trade_td_p[playerid][i] != PlayerText: INVALID_TEXT_DRAW)
				PlayerTextDrawHide(playerid, trade_td_p[playerid][i]), PlayerTextDrawHide(targetid, trade_td_p[targetid][i]);
		}

		trade_data[playerid][tradeInMenu] = false;
		trade_data[targetid][tradeInMenu] = false;

		CancelSelectTextDraw(playerid), CancelSelectTextDraw(targetid);

		va_SendClientMessage(playerid, -1, "In urma acestei tranzactii ai primit:%s", trade_data[targetid][tradeStringItems]);
		va_SendClientMessage(targetid, -1, "In urma acestei tranzactii ai primit:%s", trade_data[playerid][tradeStringItems]);
	
		sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCHAT, "%s [user:%d] a oferit:%s, %s [user:%d] a oferit:%s", playerInfo[playerid][pName], playerInfo[playerid][pSQLID], trade_data[playerid][tradeStringItems], playerInfo[targetid][pName], playerInfo[targetid][pSQLID], trade_data[targetid][tradeStringItems]);

		insertPlayerLog(playerInfo[playerid][pSQLID], 0, "%s [user:%d] a oferit:%s, %s [user:%d] a oferit:%s", playerInfo[playerid][pName], playerInfo[playerid][pSQLID], trade_data[playerid][tradeStringItems], playerInfo[targetid][pName], playerInfo[targetid][pSQLID], trade_data[targetid][tradeStringItems]);


		new
			sqlString[128];

		// transferare bunuri
		if(!strmatch(trade_data[playerid][tradeMoney], "null"))
		{
            substractBankMoney(playerid, playerInfo[playerid][pAccountLY], playerInfo[playerid][pAccount], 0, trade_data[playerid][tradeMoney]);
            addBankMoney(targetid, playerInfo[targetid][pAccountLY], playerInfo[targetid][pAccount], 0, trade_data[playerid][tradeMoney]);
/*
			new moneySuffix = 0, moneyPrefix = 0;
		    CreateLocalLY(moneyPrefix, moneySuffix, trade_data[playerid][tradeMoney]);

		    //retrage banii
		    playerInfo[playerid][pAccountLY] -= moneyPrefix;

		    if(playerInfo[playerid][pAccount] - moneySuffix < 0)
		    {
		        playerInfo[playerid][pAccount] += 1000000000 - playerInfo[playerid][pAccount] - moneySuffix;
		        playerInfo[playerid][pAccountLY] --;
		    }
		    else
		        playerInfo[playerid][pAccount] -= moneySuffix;

		    //give money
		    playerInfo[targetid][pAccountLY] += moneyPrefix;

		    if(playerInfo[targetid][pAccount] - moneySuffix < 0)
		    {
		        playerInfo[targetid][pAccount] += 1000000000 - playerInfo[targetid][pAccount] - moneySuffix;
		        playerInfo[targetid][pAccountLY] ++;
		    }
		    else
		        playerInfo[targetid][pAccount] += moneySuffix;*/
		}

		if(trade_data[playerid][tradeTotalVehicles] != 0)
		{
			sqlString[0] = (EOS);
			for(new i; i < trade_data[playerid][tradeTotalVehicles]; ++i)
			{
				new
					vehicleDBId = trade_data[playerid][tradeVehicles][i];

				if(vehicleDBId != 0)
				{
					format(sqlString, sizeof sqlString, "%s'%d'%s", sqlString, vehicleDBId, (i+1 != trade_data[playerid][tradeTotalVehicles] ? (", ") : ("")));
					
					if(GetVehicleGameID(vehicleDBId))
						DestroyPersonalVehicle(GetVehicleGameID(vehicleDBId), vehicleDBId);

					format(carInfo[vehicleDBId][cOwner], MAX_PLAYER_NAME, playerInfo[targetid][pName]);
				}
			}
			mysql_format(SQL, returnString, sizeof returnString, "update `personalvehicles` set `Owner` = '%s' where `ID` in (%s);", playerInfo[targetid][pName], sqlString);
			mysql_tquery(SQL, returnString, "", "");
		}
		if(trade_data[playerid][tradeHouse])
		{
			new houseTraded = trade_data[playerid][tradeHouse];
			//! trade per trade
			if(!trade_data[targetid][tradeHouse])
			{
				playerInfo[playerid][pHouse] = 0;

				mysql_format(SQL, returnString, sizeof returnString, "update `users` set `House` = '0' where `id` = '%d';", playerInfo[playerid][pSQLID]);
				mysql_tquery(SQL, returnString, "", "");
			}
			//transfer

			playerInfo[targetid][pHouse] = houseTraded;
			houseInfo[houseTraded][houseOwner] = playerInfo[targetid][pSQLID];

			format(houseInfo[houseTraded][houseOwnerName], MAX_PLAYER_NAME, "%s", GetNameBySQL(playerInfo[targetid][pSQLID]));
			WhenHouseGotUpdated(houseTraded);

			mysql_format(SQL, returnString, sizeof returnString, "update `houses` set `OwnerSQL` = '%d' where `id` = '%d';", playerInfo[targetid][pSQLID], houseTraded);
			mysql_tquery(SQL, returnString, "", "");
		
			mysql_format(SQL, returnString, sizeof returnString, "update `users` set `House` = '%d' where `id` = '%d';", houseTraded, playerInfo[targetid][pSQLID]);
			mysql_tquery(SQL, returnString, "", "");
		}
		if(trade_data[playerid][tradeBiz])
		{
			new bizTraded = trade_data[playerid][tradeBiz];
			//! trade per trade
			if(!trade_data[targetid][tradeBiz])
			{
				playerInfo[playerid][pBusiness] = 0;

				mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Business` = '0' where `id` = '%d';", playerInfo[playerid][pSQLID]);
				mysql_tquery(SQL, returnString, "", "");
			}
			//transfer

			playerInfo[targetid][pBusiness] = bizTraded;
			bizInfo[bizTraded][bizOwner] = playerInfo[targetid][pSQLID];

			format(bizInfo[bizTraded][bizOwnerName], MAX_PLAYER_NAME, "%s", GetNameBySQL(playerInfo[targetid][pSQLID]));
			WhenBusinessGotUpdated(bizTraded);

			mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `OwnerSQL` = '%d' where `id` = '%d';", playerInfo[targetid][pSQLID], bizTraded);
			mysql_tquery(SQL, returnString, "", "");

			mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Business` = '%d' where `id` = '%d';", bizTraded, playerInfo[targetid][pSQLID]);
			mysql_tquery(SQL, returnString, "", "");
		}
		if(trade_data[playerid][tradeApartament])
		{
			new apTraded = trade_data[playerid][tradeApartament];
			//! trade per trade
			if(!trade_data[targetid][tradeApartament])
			{
				playerInfo[playerid][pApartament] = 0;

				mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Apartament` = '0' where `id` = '%d';", playerInfo[playerid][pSQLID]);
				mysql_tquery(SQL, returnString, "", "");
			}
			//transfer

			playerInfo[targetid][pApartament] = apTraded;
			apInfo[apTraded][apOwner] = playerInfo[targetid][pSQLID];

			format(apInfo[apTraded][apOwnerName], MAX_PLAYER_NAME, "%s", GetNameBySQL(playerInfo[targetid][pSQLID]));
			WhenApartamentGotUpdated(apTraded);

			mysql_format(SQL, returnString, sizeof returnString, "update `apartaments` set `OwnerSQL` = '%d' where `id` = '%d';", playerInfo[targetid][pSQLID], apTraded);
			mysql_tquery(SQL, returnString, "", "");

			mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Apartament` = '%d' where `id` = '%d';", apTraded, playerInfo[targetid][pSQLID]);
			mysql_tquery(SQL, returnString, "", "");
		}

		//separare bunuri


		if(!strmatch(trade_data[targetid][tradeMoney], "null"))
		{
            substractBankMoney(targetid, playerInfo[targetid][pAccountLY], playerInfo[targetid][pAccount], 0, trade_data[targetid][tradeMoney]);
            addBankMoney(playerid, playerInfo[playerid][pAccountLY], playerInfo[playerid][pAccount], 0, trade_data[targetid][tradeMoney]);
/*
			new moneySuffix = 0, moneyPrefix = 0;
    		CreateLocalLY(moneyPrefix, moneySuffix, trade_data[targetid][tradeMoney]);

    		//retrage banii
		    playerInfo[targetid][pAccountLY] -= moneyPrefix;

		    if(playerInfo[targetid][pAccount] - moneySuffix < 0)
		    {
		        playerInfo[targetid][pAccount] += 1000000000 - playerInfo[targetid][pAccount] - moneySuffix;
		        playerInfo[targetid][pAccountLY] --;
		    }
		    else
		        playerInfo[targetid][pAccount] -= moneySuffix;

		    //give money
		    playerInfo[playerid][pAccountLY] += moneyPrefix;

		    if(playerInfo[playerid][pAccount] - moneySuffix < 0)
		    {
		        playerInfo[playerid][pAccount] += 1000000000 - playerInfo[playerid][pAccount] - moneySuffix;
		        playerInfo[playerid][pAccountLY] ++;
		    }
		    else
		        playerInfo[playerid][pAccount] += moneySuffix;*/
		}
		if(trade_data[targetid][tradeTotalVehicles] != 0)
		{
			sqlString[0] = (EOS);
			for(new i; i < trade_data[targetid][tradeTotalVehicles]; ++i)
			{
				new
					vehicleDBId = trade_data[targetid][tradeVehicles][i];

				if(vehicleDBId != 0)
				{
					format(sqlString, sizeof sqlString, "%s'%d'%s", sqlString, vehicleDBId, (i+1 != trade_data[targetid][tradeTotalVehicles] ? (", ") : ("")));
					
					if(GetVehicleGameID(vehicleDBId))
						DestroyPersonalVehicle(GetVehicleGameID(vehicleDBId), vehicleDBId);

					format(carInfo[vehicleDBId][cOwner], MAX_PLAYER_NAME, playerInfo[playerid][pName]);
				}
			}
			mysql_format(SQL, returnString, sizeof returnString, "update `personalvehicles` set `Owner` = '%s' where `ID` in (%s);", playerInfo[playerid][pName], sqlString);
			mysql_tquery(SQL, returnString, "", "");
		}
		if(trade_data[targetid][tradeHouse])
		{
			new houseTraded = trade_data[targetid][tradeHouse];
			//! trade per trade
			if(!trade_data[playerid][tradeHouse])
			{
				playerInfo[targetid][pHouse] = 0;

				mysql_format(SQL, returnString, sizeof returnString, "update `users` set `House` = '0' where `id` = '%d';", playerInfo[targetid][pSQLID]);
				mysql_tquery(SQL, returnString, "", "");
			}
			//transfer

			playerInfo[playerid][pHouse] = houseTraded;
			houseInfo[houseTraded][houseOwner] = playerInfo[playerid][pSQLID];

			format(houseInfo[houseTraded][houseOwnerName], MAX_PLAYER_NAME, "%s", GetNameBySQL(playerInfo[playerid][pSQLID]));
			WhenHouseGotUpdated(houseTraded);

			mysql_format(SQL, returnString, sizeof returnString, "update `houses` set `OwnerSQL` = '%d' where `id` = '%d';", playerInfo[playerid][pSQLID], houseTraded);
			mysql_tquery(SQL, returnString, "", "");
		
			mysql_format(SQL, returnString, sizeof returnString, "update `users` set `House` = '%d' where `id` = '%d';", houseTraded, playerInfo[playerid][pSQLID]);
			mysql_tquery(SQL, returnString, "", "");
		}
		if(trade_data[targetid][tradeBiz])
		{
			new bizTraded = trade_data[targetid][tradeBiz];
			//! trade per trade
			if(!trade_data[playerid][tradeBiz])
			{
				playerInfo[targetid][pBusiness] = 0;

				mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Business` = '0' where `id` = '%d';", playerInfo[targetid][pSQLID]);
				mysql_tquery(SQL, returnString, "", "");
			}
			//transfer

			playerInfo[playerid][pBusiness] = bizTraded;
			bizInfo[bizTraded][bizOwner] = playerInfo[playerid][pSQLID];

			format(bizInfo[bizTraded][bizOwnerName], MAX_PLAYER_NAME, "%s", GetNameBySQL(playerInfo[playerid][pSQLID]));
			WhenBusinessGotUpdated(bizTraded);

			mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `OwnerSQL` = '%d' where `id` = '%d';", playerInfo[playerid][pSQLID], bizTraded);
			mysql_tquery(SQL, returnString, "", "");

			mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Business` = '%d' where `id` = '%d';", bizTraded, playerInfo[playerid][pSQLID]);
			mysql_tquery(SQL, returnString, "", "");
		}
		if(trade_data[targetid][tradeApartament])
		{
			new apTraded = trade_data[targetid][tradeApartament];
			//! trade per trade
			if(!trade_data[playerid][tradeApartament])
			{
				playerInfo[targetid][pApartament] = 0;

				mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Apartament` = '0' where `id` = '%d';", playerInfo[targetid][pSQLID]);
				mysql_tquery(SQL, returnString, "", "");
			}
			//transfer

			playerInfo[playerid][pApartament] = apTraded;
			apInfo[apTraded][apOwner] = playerInfo[playerid][pSQLID];

			format(apInfo[apTraded][apOwnerName], MAX_PLAYER_NAME, "%s", GetNameBySQL(playerInfo[playerid][pSQLID]));
			WhenApartamentGotUpdated(apTraded);

			mysql_format(SQL, returnString, sizeof returnString, "update `apartaments` set `OwnerSQL` = '%d' where `id` = '%d';", playerInfo[playerid][pSQLID], apTraded);
			mysql_tquery(SQL, returnString, "", "");

			mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Apartament` = '%d' where `id` = '%d';", apTraded, playerInfo[playerid][pSQLID]);
			mysql_tquery(SQL, returnString, "", "");
		}

		tradeResetPlayerVars(playerid);
		tradeResetPlayerVars(targetid);
	}
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if(clickedid == Text: INVALID_TEXT_DRAW || clickedid == trade_td[12])
	{
		if(trade_data[playerid][tradeInMenu] == true)
		{
			// cancel trade

			new
				targetid = trade_data[playerid][tradePlayer];

			trade_data[playerid][tradeInMenu] = false;
			trade_data[targetid][tradeInMenu] = false;

			for(new i; i < 13; ++i)
				TextDrawHideForPlayer(playerid, trade_td[i]), TextDrawHideForPlayer(targetid, trade_td[i]);

			for(new i; i < 21; ++i)
			{
				if(trade_td_p[playerid][i] != PlayerText: INVALID_TEXT_DRAW)
					PlayerTextDrawHide(playerid, trade_td_p[playerid][i]), PlayerTextDrawHide(targetid, trade_td_p[targetid][i]);
			}

			CancelSelectTextDraw(playerid), CancelSelectTextDraw(targetid);

			va_SendClientMessage(targetid, -1, "Jucatorul %s(%d) a anulat afacerea!", GetName(playerid), playerid);

			tradeResetPlayerVars(playerid), tradeResetPlayerVars(targetid);
		}
	}
	if(clickedid == trade_td[11])
	{
		if(trade_data[playerid][tradeInMenu] == true)
		{
			if(GetPVarInt(playerid, "trade_process_time") || GetPVarInt(trade_data[playerid][tradePlayer], "trade_process_time"))
				sendError(playerid,"Nu poti folosi aceasta optiune in timpul finalizarii unei afaceri!");

			// delete trade items
			new
				targetid = trade_data[playerid][tradePlayer];

			for(new i=2; i < 9; ++i)
			{
				switch(i)
				{
					case 2, 4, 6, 8:
					{
						PlayerTextDrawSetString(playerid, trade_td_p[playerid][i], "Adauga");
						PlayerTextDrawSetSelectable(playerid, trade_td_p[playerid][i], 1);
						PlayerTextDrawShow(playerid, trade_td_p[playerid][i]);
					}
					case 3, 5, 7, 9:
						PlayerTextDrawSetString(targetid, trade_td_p[targetid][i], "Nimic");
				}
			}

			for(new i=12; i < 19; ++i)
			{
				switch(i)
				{
					case 12, 14, 16, 18:
						PlayerTextDrawHide(playerid, trade_td_p[playerid][i]);
					case 13, 15, 17, 19:
						PlayerTextDrawHide(targetid, trade_td_p[targetid][i]);
				}
			}

			for(new i; i < 4; ++i)
				trade_data[playerid][tradeVehicles][i] = 0;

			trade_data[playerid][tradeTotalItems] = 0;
			trade_data[playerid][tradeTotalVehicles] = 0;

			trade_data[playerid][tradeHouse] = 0;
			trade_data[playerid][tradeBiz] = 0;
			trade_data[playerid][tradeApartament] = 0;

			format(trade_data[playerid][tradeMoney], 32, "null");
			format(trade_data[playerid][tradeStringItems], 128, "");
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerClickPlayerTD(playerid, PlayerText:playertextid)
{
	if(playertextid == trade_td_p[playerid][2] || playertextid ==trade_td_p[playerid][4] || playertextid ==trade_td_p[playerid][6] || playertextid ==trade_td_p[playerid][8])
	{
		if(!GetPVarInt(playerid, "trade_slotid") && trade_data[playerid][tradeInMenu] == false)
			return true;

		new
			slot = (playertextid == trade_td_p[playerid][2] ? (2) : playertextid == trade_td_p[playerid][4] ? (4) : playertextid == trade_td_p[playerid][6] ? (6) : playertextid == trade_td_p[playerid][8] ? (8) : (-1));

		SetPVarInt(playerid, "trade_slotid", slot);

		return Dialog_Show(playerid, DIALOG_TRADE, DIALOG_STYLE_LIST, "Trade", "Bani\nVehicule\nBusiness\nCasa\nApartament", "Adauga", "Iesi");
	}
	else if(playertextid == trade_td_p[playerid][10])
	{
		if(trade_data[playerid][tradeInMenu] == false)
			return true;

		if(!trade_data[playerid][tradeReady])
			changeReadyStatus(playerid, true);
		else
			changeReadyStatus(playerid, false);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

Dialog:DIALOG_TRADE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return DeletePVar(playerid, "trade_slotid");

	switch(listitem)
	{
		case 0:
		{
			if(!strmatch(trade_data[playerid][tradeMoney], "null"))
				sendError(playerid,"Ai adaugat deja bani in aceasta tranzactie.");


			Dialog_Show(playerid, DIALOG_TRADE_MONEY, DIALOG_STYLE_INPUT, "Money", "Introduceti suma de bani", "Adauga", "Iesi");
		}
		case 1:
		{
		    mysql_format(SQL, returnString, sizeof returnString, "select * from `personalvehicles` where `Owner` = '%e' order by `id` asc;", playerInfo[playerid][pName]);
		    mysql_tquery(SQL, returnString, "OnShowTradeCars", "d", playerid);
		}
		case 2:
		{
			if(bizInfo[playerInfo[playerid][pBusiness]][bizOwner] != playerInfo[playerid][pSQLID])
				sendError(playerid,"Nu detii o afacere.");

			if(trade_data[playerid][tradeBiz])
				sendError(playerid,"Nu mai poti adauga acest bun inca odata!");

			trade_data[playerid][tradeBiz] = playerInfo[playerid][pBusiness];
			insertTradeItem(playerid, 3);
		}
		case 3:
		{
			if(houseInfo[playerInfo[playerid][pHouse]][houseOwner] != playerInfo[playerid][pSQLID])
				sendError(playerid,"Nu detii o casa.");

			if(trade_data[playerid][tradeHouse])
				sendError(playerid,"Nu mai poti adauga acest bun inca odata!");

			trade_data[playerid][tradeHouse] = playerInfo[playerid][pHouse];
			insertTradeItem(playerid, 4);
		}
		case 4:
		{
			if(apInfo[playerInfo[playerid][pApartament]][apOwner] != playerInfo[playerid][pSQLID])
				sendError(playerid,"Nu detii un apartament.");

			if(trade_data[playerid][tradeApartament])
				sendError(playerid,"Nu mai poti adauga acest bun inca odata!");

			trade_data[playerid][tradeApartament] = playerInfo[playerid][pApartament];
			insertTradeItem(playerid, 5);
		}
	}
	return true;
}

function:OnShowTradeCars(playerid)
{
    if(!cache_num_rows())
        return SendClientMessage(playerid, COLOR_GREY, "You don't have any personal vehicles.");
    
    new i;

    returnString = "Vehicle type\tKM\tDays\tVIP\n";

    while(i < cache_num_rows())
    {
        new
            vehicleDBModel = cache_get_field_content_int(i, "Model"),
            vehicleDBId = cache_get_field_content_int(i, "ID");

        format(returnString, sizeof returnString, "%s%s\t%.0f\t%d\t%s\n", returnString, aVehicleNames[vehicleDBModel - 400], carInfo[vehicleDBId][cKM], getDaysFromTimestamp(carInfo[vehicleDBId][cDays]), (carInfo[vehicleDBId][cVIP] ? ("YES") : ("NO")));

        playerStaticVar[playerid][pDialogItems][i] = vehicleDBId;
        ++i;
    }
    Dialog_Show(playerid, DIALOG_TRADE_CARS, DIALOG_STYLE_TABLIST_HEADERS, "Vehicule Personale", returnString, "Ofera", "Anuleaza");
    return true;
}

Dialog:DIALOG_TRADE_CARS(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	for(new i; i < 4; ++i)
	{
		if(trade_data[playerid][tradeVehicles][i] && trade_data[playerid][tradeVehicles][i] == playerStaticVar[playerid][pDialogItems][listitem])
			sendError(playerid,"Nu mai poti adauga acest bun inca odata!");
	}

	new
		countTVehs = trade_data[playerid][tradeTotalVehicles];

	trade_data[playerid][tradeVehicles][countTVehs] = playerStaticVar[playerid][pDialogItems][listitem];

	insertTradeItem(playerid, 2);

	return true;
}

Dialog:DIALOG_TRADE_MONEY(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;


    if(strval(inputtext) < 1)
    	return Dialog_Show(playerid, DIALOG_TRADE_MONEY, DIALOG_STYLE_INPUT, "Money", "Nu poti adauga mai putin de $1 in afacere.\nIntroduceti suma de bani", "Adauga", "Iesi");
    
    format(trade_data[playerid][tradeMoney], 64, inputtext);
    
    new moneySuffix = 0, moneyPrefix = 0;
    CreateLocalLY(moneyPrefix, moneySuffix, trade_data[playerid][tradeMoney]);

    if(playerInfo[playerid][pAccountLY] < moneyPrefix || playerInfo[playerid][pAccount] < moneySuffix && !playerInfo[playerid][pAccountLY])
        return Dialog_Show(playerid, DIALOG_TRADE_MONEY, DIALOG_STYLE_INPUT, "Money", "Nu detii suma de bani introdusa sau aceasta este invaldia\nIntroduceti suma de bani", "Adauga", "Iesi");


    insertTradeItem(playerid, 1);
    return true;
}


YCMD:trade(playerid, params[], help)
{
	if(GetPVarInt(playerid, "delay_trade") > gettime())
		return va_SendClientMessage(playerid, -1, "You can use this command in %d seconds.", GetPVarInt(playerid, "delay_trade") - gettime());

	if(mysql_unprocessed_queries() > 50) 
		return SendClientMessage(playerid, COLOR_GREY, "Nu poti folosi aceasta comanda in acest moment deoarece baza de date a serverului este incarcata! Reincearca in cateva momente!");

	if(playerInfo[playerid][pLevel] < 3)
		return SendClientMessage(playerid, COLOR_LIGHTRED, "Din pacate nu ai level 3.");

	new
		targetid;

	if(sscanf(params, "u", targetid))
		sendError(playerid,"USAGE: /trade [playername/playerid]");

	if(targetid == playerid)
		return SendClientMessage(playerid, COLOR_PURPLE, "Error: {FFFFFF}You can't transfer money to yourself!");

	if(targetid == INVALID_PLAYER_ID || GetDistanceBetweenPlayers(playerid, targetid) > 15)
		return SendClientMessage(playerid, COLOR_LIGHTRED, "That player is not near you.");

	if(trade_data[targetid][tradeInMenu] == true)
		return true;

	if(playerInfo[targetid][pLevel] < 3)
		return SendClientMessage(playerid, COLOR_LIGHTRED, "Din pacate jucatorul nu are level 3.");

	va_SendClientMessage(playerid, -1, "I-ai trimis o cerere jucatorului %s(%d)", GetName(targetid), targetid);
	va_SendClientMessage(targetid, -1, "Jucatorul %s(%d) doreste sa inceapa o afacere cu tine. Daca doresti si tu acest lucru tasteaza comanda /accepttrade %d.", GetName(playerid), playerid, playerid);

	trade_data[playerid][tradeOfferId] = targetid;
	SetPVarInt(playerid, "delay_trade", gettime() + 20);

	return true;
}

YCMD:accepttrade(playerid, params[], help)
{
	new targetid;

	if(sscanf(params, "u", targetid))
		sendError(playerid,"USAGE: /trade [playername/playerid]");

	if(targetid == INVALID_PLAYER_ID || GetDistanceBetweenPlayers(playerid, targetid) > 15)
		return SendClientMessage(playerid, COLOR_LIGHTRED, "That player is not near you.");

	if(trade_data[targetid][tradeOfferId] != playerid)
		sendError(playerid,"Acest jucator nu si-a exercitat dorinta de a face o afacere cu tine!");

	if(trade_data[playerid][tradeInMenu] == true)
		return true;

	trade_data[playerid][tradePlayer] = targetid;
	trade_data[targetid][tradePlayer] = playerid;

	init_trade(playerid), init_trade(targetid);

	va_SendClientMessage(playerid, -1, "Ai acceptat cererea jucatorului %s(%d). Ti-a fost afisat meniul de negociere!", GetName(targetid), targetid);
	va_SendClientMessage(targetid, -1, "Jucatorul %s(%d) a acceptat cererea ta. Ti-a fost afisat meniul de negociere!", GetName(playerid), playerid);

	returnString = "Puteti negocia cu maxim 4 iteme per sesiune, indiferent de natura acestora";
	SendClientMessage(playerid, -1, returnString), SendClientMessage(targetid, -1, returnString);

	returnString = "Puteti anula afacerea apasand tasta ESC sau dand click pe X!";
	SendClientMessage(playerid, -1, returnString), SendClientMessage(targetid, -1, returnString);

	return true;
}