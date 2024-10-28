new PlayerText: paintRank[MAX_PLAYERS][2], PlayerText: paintTD[MAX_PLAYERS][2];
#pragma warning disable 213

function:LoadServerAssests()
{
	CreateDynamicMapIcon(1510.3986, -1059.5723, 25.0625, 35, 0, -1, -1, -1, 750.0, MAPICON_LOCAL);	// Event Arena
	CreateDynamicMapIcon(1172.2798, -1323.2834, 15.4029, 22, 0, -1, -1, -1, 750.0, MAPICON_LOCAL); //HOSPITAL LS
 	CreateDynamicObject(955, 1729.780029, -1943.040039, 12.940000, 0.000000, 0.000000, 0.000000, -1, -1, -1, 400.000000, 400.000000);
 	CreateDynamicObject(19324, 1673.361450, -1884.867310, 13.174000, 0.000000, 0.000000, -91.000000, -1, -1, -1, 400.000000, 400.000000);
 	CreateDynamicObject(19324, 1557.482300, -1862.618530, 13.164900, 0.000000, 0.000000, 0.000000, -1, -1, -1, 400.000000, 400.000000);
 	CreateDynamicObject(19324, 1436.057495, -1883.461670, 13.164600, 0.000000, 0.000000, -90.000000, -1, -1, -1, 400.000000, 400.000000);
	return true;
}

function:destroyHudTextdraw(playerid) {
	PlayerTextDrawHide(playerid, playerTextdraws[playerid][PlayerFPS]);
	PlayerTextDrawHide(playerid, playerTextdraws[playerid][AdminHUD]);
	PlayerTextDrawHide(playerid, playerTextdraws[playerid][ReportsTD]);
	return true;
}




function:LoadPlayerTextdraws(playerid)
{

	playerTextdraws[playerid][GunPTD][0] = CreatePlayerTextDraw(playerid, 322.333374, 358.414734, "M4");
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][GunPTD][0], 0.352999, 1.226667);
	PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][GunPTD][0], 1280.000000, 1280.000000);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][GunPTD][0], 2);
	PlayerTextDrawColour(playerid, playerTextdraws[playerid][GunPTD][0], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][GunPTD][0], 0);
	PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][GunPTD][0], 0x80808080);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][GunPTD][0], 0);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][GunPTD][0], 0);
	PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][GunPTD][0], 0x000000FF);
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][GunPTD][0], 3);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][GunPTD][0], 1);
	PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][GunPTD][0], 0);

	playerTextdraws[playerid][GunPTD][1] = CreatePlayerTextDraw(playerid, 324.999878, 377.496307, "price: $6.500~n~bullets: 400");
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][GunPTD][1], 0.236999, 0.998518);
	PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][GunPTD][1], 1280.000000, 1280.000000);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][GunPTD][1], 2);
	PlayerTextDrawColour(playerid, playerTextdraws[playerid][GunPTD][1], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][GunPTD][1], 0);
	PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][GunPTD][1], 0x80808080);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][GunPTD][1], 0);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][GunPTD][1], 0);
	PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][GunPTD][1], 0x000000FF);
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][GunPTD][1], 2);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][GunPTD][1], 1);
	PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][GunPTD][1], 0);

	playerTextdraws[playerid][StorePTD] = CreatePlayerTextDraw(playerid, 317.999969 + 10.0, 259.835235, "Phone~n~$1500");
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][StorePTD], 0.400000, 1.600000);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][StorePTD], 2);
	PlayerTextDrawColour(playerid, playerTextdraws[playerid][StorePTD], -1);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][StorePTD], 0);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][StorePTD], 1);
	PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][StorePTD], 255);
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][StorePTD], 3);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][StorePTD], 1);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][StorePTD], 0);

    playerTextdraws[playerid][dealerPTD][1] = CreatePlayerTextDraw(playerid, 157.764816, 315.067657, "Model: Bike~n~Price: $1.500.000~n~Stock: 2~n~Top speed: 120 km/h");
    PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][dealerPTD][1], 0.359529, 1.582500);
    PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][dealerPTD][1], 1280.000000, 1280.000000);
    PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][dealerPTD][1], 1);
    PlayerTextDrawColour(playerid, playerTextdraws[playerid][dealerPTD][1], 0xFFFFFFFF);
    PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][dealerPTD][1], 0);
    PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][dealerPTD][1], 0x80808080);
    PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][dealerPTD][1], 0);
    PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][dealerPTD][1], 1);
    PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][dealerPTD][1], 0x000000FF);
    PlayerTextDrawFont(playerid, playerTextdraws[playerid][dealerPTD][1], 1);
    PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][dealerPTD][1], 1);
    PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][dealerPTD][1], 0);

    playerTextdraws[playerid][dealerPTD][0] = CreatePlayerTextDraw(playerid, 334.175934, 304.916656, "");
    PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][dealerPTD][0], 0.000000, 0.000000);
    PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][dealerPTD][0], 97.000000, 94.000000);
    PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][dealerPTD][0], 1);
    PlayerTextDrawColour(playerid, playerTextdraws[playerid][dealerPTD][0], 0xFFFFFFFF);
    PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][dealerPTD][0], 0);
    PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][dealerPTD][0], 0x80808080);
    PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][dealerPTD][0], 0);
    PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][dealerPTD][0], 0);
    PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][dealerPTD][0], 0xFF000000);
    PlayerTextDrawFont(playerid, playerTextdraws[playerid][dealerPTD][0], 5);
    PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][dealerPTD][0], 0);
    PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][dealerPTD][0], 1);
    PlayerTextDrawSetPreviewModel(playerid, playerTextdraws[playerid][dealerPTD][0], 509);
    PlayerTextDrawSetPreviewVehicleColours(playerid, playerTextdraws[playerid][dealerPTD][0], 1, 1);
    PlayerTextDrawSetPreviewRot(playerid, playerTextdraws[playerid][dealerPTD][0], 0.000000, 0.000000, 90.000000, 1.000000);

    playerTextdraws[playerid][drugs_td] = CreatePlayerTextDraw(playerid, 316.100250, 415.531036, "drugs manufacturer~n~status: ~r~planting seeds");
    PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][drugs_td], 0.094333, 0.695778);
    PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][drugs_td], 0.000000, 132.000000);
    PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][drugs_td], 2);
    PlayerTextDrawColour(playerid, playerTextdraws[playerid][drugs_td], 0xFFFFFFFF);
    PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][drugs_td], true);
    PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][drugs_td], 0x000000FF);
    PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][drugs_td], 0);
    PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][drugs_td], 1);
    PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][drugs_td], 0x000000FF);
    PlayerTextDrawFont(playerid, playerTextdraws[playerid][drugs_td], 2);
    PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][drugs_td], true);
    PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][drugs_td], false);

	playerTextdraws[playerid][FindTD] = CreatePlayerTextDraw(playerid, 292.950165, 321.086243, "~w~TARGET:   ~r~ERARPG(1)~n~~w~DISTANCE:   ~r~1265M(LS)");
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][FindTD], 0.201049, 1.850132);
	PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][FindTD], 1280.000000, 1280.000000);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][FindTD], 1);
	PlayerTextDrawColour(playerid, playerTextdraws[playerid][FindTD], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][FindTD], false);
	PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][FindTD], 0x80808080);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][FindTD], 13);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][FindTD], 255);
	PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][FindTD], 0x000000FF);
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][FindTD], 2);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][FindTD], true);
	PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][FindTD], false);

	playerTextdraws[playerid][PlayerFPS] = CreatePlayerTextDraw(playerid, 2.000000, 432.000000, "~y~100");
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][PlayerFPS], 3);
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][PlayerFPS], 0.354166, 1.400000);
	PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][PlayerFPS], 400.000000, 17.000000);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][PlayerFPS], 1);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][PlayerFPS], 0);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][PlayerFPS], 1);
	PlayerTextDrawColor(playerid, playerTextdraws[playerid][PlayerFPS], -1);
	PlayerTextDrawBackgroundColor(playerid, playerTextdraws[playerid][PlayerFPS], 255);
	PlayerTextDrawBoxColor(playerid, playerTextdraws[playerid][PlayerFPS], 50);
	PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][PlayerFPS], 0);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][PlayerFPS], 1);
	PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][PlayerFPS], 0);


	playerTextdraws[playerid][AdminHUD] = CreatePlayerTextDraw(playerid, 320.000000, 3.000000, "~r~ticks: 195 ~b~queries: 0 ~y~anim: 1293");
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][AdminHUD], 3);
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][AdminHUD], 0.208333, 1.050001);
	PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][AdminHUD], 955.000000, 577.000000);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][AdminHUD], 1);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][AdminHUD], 0);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][AdminHUD], 2);
	PlayerTextDrawColor(playerid, playerTextdraws[playerid][AdminHUD], -1);
	PlayerTextDrawBackgroundColor(playerid, playerTextdraws[playerid][AdminHUD], 133);
	PlayerTextDrawBoxColor(playerid, playerTextdraws[playerid][AdminHUD], 50);
	PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][AdminHUD], 0);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][AdminHUD], 1);
	PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][AdminHUD], 0);


	playerTextdraws[playerid][ReportsTD] = CreatePlayerTextDraw(playerid, 320.000000, 438.000000, "REPORTS: ~r~0 ~w~/ CHEATERS: ~b~0");
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][ReportsTD], 2);
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][ReportsTD], 0.183333, 0.850001);
	PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][ReportsTD], 955.000000, 577.000000);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][ReportsTD], 1);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][ReportsTD], 0);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][ReportsTD], 2);
	PlayerTextDrawColor(playerid, playerTextdraws[playerid][ReportsTD], -1);
	PlayerTextDrawBackgroundColor(playerid, playerTextdraws[playerid][ReportsTD], 133);
	PlayerTextDrawBoxColor(playerid, playerTextdraws[playerid][ReportsTD], 50);
	PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][ReportsTD], 0);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][ReportsTD], 1);
	PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][ReportsTD], 0);
 
    playerTextdraws[playerid][Logo] = CreatePlayerTextDraw(playerid, 637.220581, 432.350585, ""SERVER_DNS"");
    PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][Logo], 0.231422, 1.429164);
    PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][Logo], 3);
    PlayerTextDrawColour(playerid, playerTextdraws[playerid][Logo], -1);
    PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][Logo], 0);
    PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][Logo], 1);
    PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][Logo], 255);
    PlayerTextDrawFont(playerid, playerTextdraws[playerid][Logo], 2);
    PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][Logo], 1);

	playerTextdraws[playerid][dmvPTD] = CreatePlayerTextDraw(playerid, 244.999969, 376.444489, "~w~CHECKPOINTS: ~r~0~w~/~r~24");
    PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][dmvPTD], 0.433333, 1.651851);
    PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][dmvPTD], 1280.000000, 1280.000000);
    PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][dmvPTD], 1);
    PlayerTextDrawColour(playerid, playerTextdraws[playerid][dmvPTD], 0xFFFFFFFF);
    PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][dmvPTD], 0);
    PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][dmvPTD], 0x80808080);
    PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][dmvPTD], 0);
    PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][dmvPTD], 1);
    PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][dmvPTD], 0x00000033);
    PlayerTextDrawFont(playerid, playerTextdraws[playerid][dmvPTD], 3);
    PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][dmvPTD], 1);
    PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][dmvPTD], 0);

	playerTextdraws[playerid][PayDayPTD] = CreatePlayerTextDraw(playerid, 13.764657, 236.666687, "bank balance:_$22,615,088~n~interest:_$19.129~n~rent:_$1.500");
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][PayDayPTD], 0.255529, 1.133332);
	PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][PayDayPTD], 1280.000000, 1280.000000);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][PayDayPTD], 1);
	PlayerTextDrawColour(playerid, playerTextdraws[playerid][PayDayPTD], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][PayDayPTD], 0);
	PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][PayDayPTD], 0x80808080);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][PayDayPTD], 0);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][PayDayPTD], 1);
	PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][PayDayPTD], 0x000000FF);
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][PayDayPTD], 3);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][PayDayPTD], 1);
	PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][PayDayPTD], 0);


    playerTextdraws[playerid][HudTD] = CreatePlayerTextDraw(playerid, 558.190612, 121.833168, "LEVEL 0 (0/0 RESPECT)");
    PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][HudTD], 0.129151, 1.169998);
    PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][HudTD], 0.000000, 390.000000);
    PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][HudTD], 2);
    PlayerTextDrawColour(playerid, playerTextdraws[playerid][HudTD], -1);
    PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][HudTD], 0);
    PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][HudTD], 1);
    PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][HudTD], 50);
    PlayerTextDrawFont(playerid, playerTextdraws[playerid][HudTD], 2);
    PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][HudTD], 1);

	for(new i; i <= 5; i++)
	{
		playerTextdraws[playerid][casePTD][i] = CreatePlayerTextDraw(playerid, 217.000000 + (i * 71.4), 207.000000, "_");
		PlayerTextDrawFont(playerid, playerTextdraws[playerid][casePTD][i], 1);
		PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][casePTD][i], 0.600000, 7.399991 - 0.050);
		PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][casePTD][i], 298.500000, 64.000000 + 1.2);
		PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][casePTD][i], 0);
		PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][casePTD][i], 0);
		PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][casePTD][i], 2);
		PlayerTextDrawColour(playerid, playerTextdraws[playerid][casePTD][i], -16776961);
		PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][casePTD][i], 255);
		PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][casePTD][i], -41);
		//PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][casePTD][i], -1288690262); MONEY
		PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][casePTD][i], 1);
		PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][casePTD][i], 1);
		PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][casePTD][i], 0);
	}

	playerTextdraws[playerid][casePTD][5] = CreatePlayerTextDraw(playerid, 353.000000, 279.000000, "_");
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][casePTD][5], 1);
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][casePTD][5], 1.349997, -8.399988);
	PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][casePTD][5], 298.500000, -5.250000);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][casePTD][5], 1);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][casePTD][5], 0);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][casePTD][5], 2);
	PlayerTextDrawColour(playerid, playerTextdraws[playerid][casePTD][5], -1);
	PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][casePTD][5], 255);
	PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][casePTD][5], -1);
	PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][casePTD][5], 1);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][casePTD][5], 1);
	PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][casePTD][5], 0);

	for(new i = 6; i <= 10; i++)
	{
		playerTextdraws[playerid][casePTD][i] = CreatePlayerTextDraw(playerid, 218.000000 + ((i - 6) * 71.4), 235.000000, "Nothing");
		PlayerTextDrawFont(playerid, playerTextdraws[playerid][casePTD][i], 1);
		PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][casePTD][i], 0.399998, 1.549998);
		PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][casePTD][i], 400.500000, 14.500000);
		PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][casePTD][i], 0);
		PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][casePTD][i], 0);
		PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][casePTD][i], 2);
		PlayerTextDrawColour(playerid, playerTextdraws[playerid][casePTD][i], 255);
		PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][casePTD][i], 255);
		PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][casePTD][i], 50);
		PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][casePTD][i], 0);
		PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][casePTD][i], 1);
		PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][casePTD][i], 0);
	}

	playerTextdraws[playerid][casePTD][11] = CreatePlayerTextDraw(playerid, 356.000000, 189.500000, "case opening");
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][casePTD][11], 3);
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][casePTD][11], 0.399998, 1.600000);
	PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][casePTD][11], 400.000000, 192.000000);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][casePTD][11], 1);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][casePTD][11], 0);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][casePTD][11], 2);
	PlayerTextDrawColour(playerid, playerTextdraws[playerid][casePTD][11], -1);
	PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][casePTD][11], 255);
	PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][casePTD][11], 50);
	PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][casePTD][11], 0);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][casePTD][11], 1);
	PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][casePTD][11], 0);

	playerTextdraws[playerid][casePTD][12] = CreatePlayerTextDraw(playerid, 332.000000, 275.000000, "1~n~Hidden~n~Color");
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][casePTD][12], 1);
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][casePTD][12], 0.395833, 1.649999);
	PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][casePTD][12], 400.000000, 17.000000);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][casePTD][12], 0);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][casePTD][12], 0);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][casePTD][12], 2);
	PlayerTextDrawColour(playerid, playerTextdraws[playerid][casePTD][12], -1);
	PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][casePTD][12], 255);
	PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][casePTD][12], 50);
	PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][casePTD][12], 0);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][casePTD][12], 1);
	PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][casePTD][12], 0);

	playerTextdraws[playerid][Tutorial][0] = CreatePlayerTextDraw(playerid, 280.000000, 377.000000, "~y~Tutorial");
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][Tutorial][0], 0.420000, 1.500000);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][Tutorial][0], 0);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][Tutorial][0], 1);
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][Tutorial][0], 2);

	playerTextdraws[playerid][Tutorial][1] = CreatePlayerTextDraw(playerid, 282.000000, 390.000000, "~w~Step: ~r~1~w~/~r~8");
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][Tutorial][1], 0.470000, 1.500000);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][Tutorial][1], 0);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][Tutorial][1], 1);
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][Tutorial][1], 2);
	
	playerTextdraws[playerid][GymTD] = CreatePlayerTextDraw(playerid, 431.000000, 166.000000, "POWER:~n~REPS:~g~ 0~n~~w~MUSCLE LEVEL:~g~ 68");
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][GymTD], 2);
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][GymTD], 0.254166, 1.399996);
	PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][GymTD], 587.000000, 17.000000);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][GymTD], 1);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][GymTD], 0);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][GymTD], 1);
	PlayerTextDrawColour(playerid, playerTextdraws[playerid][GymTD], -1);
	PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][GymTD], 255);
	PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][GymTD], 50);
	PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][GymTD], 0);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][GymTD], 1);
	PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][GymTD], 0);

	playerTextdraws[playerid][moneyTD] = CreatePlayerTextDraw(playerid, 516.000000, 102.000000, "+ $15,000");
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][moneyTD], 1);
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][moneyTD], 0.337498, 1.549998);
	PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][moneyTD], 798.500000, -18.000000);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][moneyTD], 1);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][moneyTD], 0);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][moneyTD], 1);
	PlayerTextDrawColour(playerid, playerTextdraws[playerid][moneyTD], 9109759);
	PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][moneyTD], 80);
	PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][moneyTD], 50);
	PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][moneyTD], 0);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][moneyTD], 1);
	PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][moneyTD], 0);

	paintTD[playerid][0] = CreatePlayerTextDraw(playerid, 315.300231, 397.666442, "~n~time_left:_2:29~n~your_score:_5_(10_kills_/_5_deaths)~n~best_player:_shane_(10_score)~n~Map:_RC_Battlefield");
	PlayerTextDrawLetterSize(playerid, paintTD[playerid][0], 0.145592, 0.882498);
	PlayerTextDrawTextSize(playerid, paintTD[playerid][0], 0.000000, 154.000000);
	PlayerTextDrawAlignment(playerid, paintTD[playerid][0], 2);
	PlayerTextDrawColor(playerid, paintTD[playerid][0], -1);
	PlayerTextDrawUseBox(playerid, paintTD[playerid][0], 1);
	PlayerTextDrawBoxColor(playerid, paintTD[playerid][0], 255);
	PlayerTextDrawSetShadow(playerid, paintTD[playerid][0], 0);
	PlayerTextDrawSetOutline(playerid, paintTD[playerid][0], 0);
	PlayerTextDrawBackgroundColor(playerid, paintTD[playerid][0], 255);
	PlayerTextDrawFont(playerid, paintTD[playerid][0], 2);
	PlayerTextDrawSetProportional(playerid, paintTD[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid, paintTD[playerid][0], 0);

	paintTD[playerid][1] = CreatePlayerTextDraw(playerid, 314.831787, 388.333374, "Paintball_Status");
	PlayerTextDrawLetterSize(playerid, paintTD[playerid][1], 0.400000, 1.600000);
	PlayerTextDrawAlignment(playerid, paintTD[playerid][1], 2);
	PlayerTextDrawColor(playerid, paintTD[playerid][1], -1);
	PlayerTextDrawSetShadow(playerid, paintTD[playerid][1], 0);
	PlayerTextDrawSetOutline(playerid, paintTD[playerid][1], 1);
	PlayerTextDrawBackgroundColor(playerid, paintTD[playerid][1], 255);
	PlayerTextDrawFont(playerid, paintTD[playerid][1], 0);
	PlayerTextDrawSetProportional(playerid, paintTD[playerid][1], 1);
	PlayerTextDrawSetShadow(playerid, paintTD[playerid][1], 0);

	paintRank[playerid][0] = CreatePlayerTextDraw(playerid, 212.057083, 243.083389, "LD_TATT:9gun2");
	PlayerTextDrawTextSize(playerid, paintRank[playerid][0], 32.000000, 32.000167);
	PlayerTextDrawAlignment(playerid, paintRank[playerid][0], 1);
	PlayerTextDrawColor(playerid, paintRank[playerid][0], -1);
	PlayerTextDrawSetShadow(playerid, paintRank[playerid][0], 0);
	PlayerTextDrawBackgroundColor(playerid, paintRank[playerid][0], 255);
	PlayerTextDrawFont(playerid, paintRank[playerid][0], 4);
	PlayerTextDrawSetProportional(playerid, paintRank[playerid][0], 0);

	paintRank[playerid][1] = CreatePlayerTextDraw(playerid, 328.887359, 245.399917, "~r~new_rank_unlocked!~n~~w~Master Guardian Elite");
	PlayerTextDrawLetterSize(playerid, paintRank[playerid][1], 0.244450, 1.502500);
	PlayerTextDrawTextSize(playerid, paintRank[playerid][1], 0.000000, 166.000000);
	PlayerTextDrawAlignment(playerid, paintRank[playerid][1], 2);
	PlayerTextDrawColor(playerid, paintRank[playerid][1], -1);
	PlayerTextDrawUseBox(playerid, paintRank[playerid][1], 1);
	PlayerTextDrawBoxColor(playerid, paintRank[playerid][1], 255);
	PlayerTextDrawSetShadow(playerid, paintRank[playerid][1], 0);
	PlayerTextDrawBackgroundColor(playerid, paintRank[playerid][1], 255);
	PlayerTextDrawFont(playerid, paintRank[playerid][1], 2);
	PlayerTextDrawSetProportional(playerid, paintRank[playerid][1], 1);

	playerTextdraws[playerid][SpeedTD] = CreatePlayerTextDraw(playerid, 540.601440, 316.077758, "---");
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][SpeedTD], 0.400000, 1.600000);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][SpeedTD], 3);
	PlayerTextDrawColour(playerid, playerTextdraws[playerid][SpeedTD], -1);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][SpeedTD], 0);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][SpeedTD], 1);
	PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][SpeedTD], 255);
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][SpeedTD], 3);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][SpeedTD], 1);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][SpeedTD], 0);


	playerTextdraws[playerid][kmTD] = CreatePlayerTextDraw(playerid, 542.243225, 319.866668, "km/h");
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][kmTD], 0.249999, 1.176887);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][kmTD], 1);
	PlayerTextDrawColour(playerid, playerTextdraws[playerid][kmTD], -1);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][kmTD], 0);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][kmTD], 1);
	PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][kmTD], 255);
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][kmTD], 1);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][kmTD], 1);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][kmTD], 0);

	playerTextdraws[playerid][speedometerTD][1] = CreatePlayerTextDraw(playerid, 514.667968, 330.877929, "-- --");
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][speedometerTD][1], 0.200499, 0.991554);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][speedometerTD][1], 1);
	PlayerTextDrawColour(playerid, playerTextdraws[playerid][speedometerTD][1], -1);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][speedometerTD][1], 0);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][speedometerTD][1], 1);
	PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][speedometerTD][1], 255);
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][speedometerTD][1], 2);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][speedometerTD][1], 1);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][speedometerTD][1], 0);
	return true;
}

function:removeMaps(playerid) {

	RemoveBuildingForPlayer(playerid, 6253, 1305.4688, -1619.7422, 13.3984, 0.25);
	RemoveBuildingForPlayer(playerid, 6046, 1305.4688, -1619.7422, 13.3984, 0.25);
    RemoveBuildingForPlayer(playerid, 955, 1729.780029, -1943.040039, 12.940000, 0.250000);
    RemoveBuildingForPlayer(playerid, 956, 1634.099976, -2237.530029, 12.890000, 0.250000);
    RemoveBuildingForPlayer(playerid, 1226, 1624.687500, -2190.375000, 16.312500, 0.250000);
    RemoveBuildingForPlayer(playerid, 956, 1634.109375, -2237.531250, 12.890600, 0.250000);
    RemoveBuildingForPlayer(playerid, 955, 1729.789063, -1943.046875, 12.945300, 0.250000);
    RemoveBuildingForPlayer(playerid, 1226, 1774.757813, -1931.312500, 16.375000, 0.250000);
    RemoveBuildingForPlayer(playerid, 700, 1810.679688, -1944.640625, 11.445300, 0.250000);
    RemoveBuildingForPlayer(playerid, 700, 1817.179688, -1944.640625, 11.445300, 0.250000);
    RemoveBuildingForPlayer(playerid, 1226, 1806.429688, -1931.601563, 16.375000, 0.250000);
    RemoveBuildingForPlayer(playerid, 5024, 1748.843750, -1883.031250, 14.187500, 0.250000);
    RemoveBuildingForPlayer(playerid, 1226, 1774.757813, -1901.539063, 16.375000, 0.250000);
    RemoveBuildingForPlayer(playerid, 1226, 1806.429688, -1901.828125, 16.375000, 0.250000);
    RemoveBuildingForPlayer(playerid, 620, 1747.187500, -1846.710938, 10.804700, 0.250000);
    RemoveBuildingForPlayer(playerid, 1226, 1783.671875, -1859.539063, 16.351601, 0.250000);
    RemoveBuildingForPlayer(playerid, 620, 1836.718750, -1898.898438, 13.734400, 0.250000);
    RemoveBuildingForPlayer(playerid, 1226, 1808.125000, -1859.539063, 16.351601, 0.250000);
    RemoveBuildingForPlayer(playerid, 713, 1304.772949, -1839.866943, 12.437000, 0.250000);
    RemoveBuildingForPlayer(playerid, 647, 1284.975952, -1828.421021, 14.070000, 0.250000);
    RemoveBuildingForPlayer(playerid, 673, 1284.890015, -1823.859009, 12.523000, 0.250000);
    RemoveBuildingForPlayer(playerid, 5043, 1843.367188, -1856.320313, 13.875000, 0.250000);
//miner
	RemoveBuildingForPlayer(playerid, 3350, 266.0625, 2879.2344, 11.8516, 0.25);
	RemoveBuildingForPlayer(playerid, 3362, 264.8516, 2893.1641, 8.9375, 0.25);
	return true;
}

function:loadTextDrawsFromInclude()
{

	SafezoneTD = TextDrawCreate(339.666687, 12.555577, "safe-zone");
	TextDrawFont(SafezoneTD, 3);
	TextDrawLetterSize(SafezoneTD, 0.231000, 1.052444);
	TextDrawAlignment(SafezoneTD, 2);
	TextDrawColor(SafezoneTD, 0xFFFFFFFF9D72FFFF);
	TextDrawSetProportional(SafezoneTD, 1);
	TextDrawTextSize(SafezoneTD, 1280.000000, 1280.000000);
	TextDrawSetShadow(SafezoneTD, 0);
	TextDrawSetOutline(SafezoneTD, 1);
	TextDrawBackgroundColor(SafezoneTD, 0x000000FF);
	TextDrawSetSelectable(SafezoneTD, 0);

	dateTD[0] = TextDrawCreate(574.604065, 19.437004, "02~p~:~w~~h~~h~~h~07");
	TextDrawFont(dateTD[0], 3);
	TextDrawLetterSize(dateTD[0], 0.385332, 1.645629);
	TextDrawAlignment(dateTD[0], 2);
	TextDrawColor(dateTD[0], 0xFFFFFFFFFFFFFFFF);
	TextDrawSetProportional(dateTD[0], 1);
	TextDrawTextSize(dateTD[0], 1280.000000, 1280.000000);
	TextDrawSetShadow(dateTD[0], 0);
	TextDrawSetOutline(dateTD[0], 1);
	TextDrawBackgroundColor(dateTD[0], 0x000000FF);
	TextDrawSetSelectable(dateTD[0], 0);


	dateTD[1] = TextDrawCreate(577.433411, 32.666748, "~p~7 ~w~~h~~h~~h~July 2024");
	TextDrawFont(dateTD[1], 2);
	TextDrawLetterSize(dateTD[1], 0.218333, 1.313778);
	TextDrawAlignment(dateTD[1], 2);
	TextDrawColor(dateTD[1], 0xFFFFFFFFFFFFFFFF);
	TextDrawSetProportional(dateTD[1], 1);
	TextDrawTextSize(dateTD[1], 1280.000000, 1280.000000);
	TextDrawSetShadow(dateTD[1], 0);
	TextDrawSetOutline(dateTD[1], 1);
	TextDrawBackgroundColor(dateTD[1], 0x000000FF);
	TextDrawSetSelectable(dateTD[1], 0);

	dateTD[2] = TextDrawCreate(594.166992, 25.914892, ":58");
	TextDrawFont(dateTD[2], 2);
	TextDrawLetterSize(dateTD[2], 0.158665, 0.882369);
	TextDrawAlignment(dateTD[2], 1);
	TextDrawColor(dateTD[2], 0xFFFFFFFFFFFFFFFF);
	TextDrawSetProportional(dateTD[2], 1);
	TextDrawTextSize(dateTD[2], 1280.000000, 1280.000000);
	TextDrawSetShadow(dateTD[2], 0);
	TextDrawSetOutline(dateTD[2], 1);
	TextDrawBackgroundColor(dateTD[2], 0x000000FF);
	TextDrawSetSelectable(dateTD[2], 0);

	loginQueue[0] = TextDrawCreate(318.333221, 230.240723, "#..");
	TextDrawFont(loginQueue[0], 3);
	TextDrawLetterSize(loginQueue[0], 0.535000, 2.321776);
	TextDrawAlignment(loginQueue[0], 2);
	TextDrawColor(loginQueue[0], 0xFFFFFFFFFFFFFFFF);
	TextDrawSetProportional(loginQueue[0], 1);
	TextDrawTextSize(loginQueue[0], 1280.000000, 1280.000000);
	TextDrawSetShadow(loginQueue[0], 0);
	TextDrawSetOutline(loginQueue[0], 2);
	TextDrawBackgroundColor(loginQueue[0], 0x000000FF);
	TextDrawSetSelectable(loginQueue[0], 0);

	loginQueue[1] = TextDrawCreate(0.666697, 220.281448, "box");
	TextDrawFont(loginQueue[1], 1);
	TextDrawLetterSize(loginQueue[1], 0.000000, 6.333333);
	TextDrawAlignment(loginQueue[1], 1);
	TextDrawColor(loginQueue[1], 0xFFFFFFFF800000FF);
	TextDrawSetProportional(loginQueue[1], 1);
	TextDrawUseBox(loginQueue[1], 1);
	TextDrawBoxColor(loginQueue[1], 0xFFFFFFFF854173DD);
	TextDrawTextSize(loginQueue[1], 654.000000, 0.000000);
	TextDrawSetShadow(loginQueue[1], 0);
	TextDrawSetOutline(loginQueue[1], 0);
	TextDrawBackgroundColor(loginQueue[1], 0x000000FF);
	TextDrawSetSelectable(loginQueue[1], 0);

	loginQueue[2] = TextDrawCreate(84.399628, 433.022095, "_");
	TextDrawFont(loginQueue[2], 2);
	TextDrawLetterSize(loginQueue[2], 0.219000, 1.218370);
	TextDrawAlignment(loginQueue[2], 2);
	TextDrawColor(loginQueue[2], 0xFFFFFFFFFFFFFFFF);
	TextDrawSetProportional(loginQueue[2], 1);
	TextDrawTextSize(loginQueue[2], 1280.000000, 1280.000000);
	TextDrawSetShadow(loginQueue[2], 0);
	TextDrawSetOutline(loginQueue[2], 1);
	TextDrawBackgroundColor(loginQueue[2], 0x000000FF);
	TextDrawSetSelectable(loginQueue[2], 0);

	loginQueue[3] = TextDrawCreate(318.333221, 259.742523, "...");
	TextDrawFont(loginQueue[3], 2);
	TextDrawLetterSize(loginQueue[3], 0.242333, 1.210072);
	TextDrawAlignment(loginQueue[3], 2);
	TextDrawColor(loginQueue[3], 0xFFFFFFFFFFFFFFFF);
	TextDrawSetProportional(loginQueue[3], 1);
	TextDrawTextSize(loginQueue[3], 1280.000000, 1280.000000);
	TextDrawSetShadow(loginQueue[3], 0);
	TextDrawSetOutline(loginQueue[3], 1);
	TextDrawBackgroundColor(loginQueue[3], 0x000000FF);
	TextDrawSetSelectable(loginQueue[3], 0);


	dmvTD[2] = TextDrawCreate(199.444443, 376.444458, "hud:radar_impound");
	TextDrawLetterSize(dmvTD[2], 0.000000, 0.000000);
	TextDrawTextSize(dmvTD[2], 21.111130, 15.037019);
	TextDrawAlignment(dmvTD[2], 1);
	TextDrawColour(dmvTD[2], 0xFFFFFFFF);
	TextDrawUseBox(dmvTD[2], 0);
	TextDrawBoxColour(dmvTD[2], 0x80808080);
	TextDrawSetShadow(dmvTD[2], 0);
	TextDrawSetOutline(dmvTD[2], 0);
	TextDrawBackgroundColour(dmvTD[2], 0x000000FF);
	TextDrawFont(dmvTD[2], 4);
	TextDrawSetProportional(dmvTD[2], 1);
	TextDrawSetSelectable(dmvTD[2], 0);

	dmvTD[1] = TextDrawCreate(402.777771, 376.444458, "hud:radar_impound");
	TextDrawLetterSize(dmvTD[1], 0.000000, 0.000000);
	TextDrawTextSize(dmvTD[1], 20.555580, 15.555543);
	TextDrawAlignment(dmvTD[1], 1);
	TextDrawColour(dmvTD[1], 0xFFFFFFFF);
	TextDrawUseBox(dmvTD[1], 0);
	TextDrawBoxColour(dmvTD[1], 0x80808080);
	TextDrawSetShadow(dmvTD[1], 0);
	TextDrawSetOutline(dmvTD[1], 0);
	TextDrawBackgroundColour(dmvTD[1], 0x000000FF);
	TextDrawFont(dmvTD[1], 4);
	TextDrawSetProportional(dmvTD[1], 1);
	TextDrawSetSelectable(dmvTD[1], 0);

	dmvTD[0] = TextDrawCreate(427.000000, 377.944458, "usebox");
	TextDrawLetterSize(dmvTD[0], 0.000000, 1.469340);
	TextDrawTextSize(dmvTD[0], 195.222214, 0.000000);
	TextDrawAlignment(dmvTD[0], 1);
	TextDrawColour(dmvTD[0], 0x00000000);
	TextDrawUseBox(dmvTD[0], 1);
	TextDrawBoxColour(dmvTD[0], 0x00000066);
	TextDrawSetShadow(dmvTD[0], 0);
	TextDrawSetOutline(dmvTD[0], 0);
	TextDrawBackgroundColour(dmvTD[0], 0x000000FF);
	TextDrawFont(dmvTD[0], 0);
	TextDrawSetProportional(dmvTD[0], 1);
	TextDrawSetSelectable(dmvTD[0], 0);
    
    dutyTD = TextDrawCreate(638.832642, 1.259473, "faction duty");
	TextDrawLetterSize(dutyTD, 0.172333, 0.944592);
	TextDrawTextSize(dutyTD, 1280.000000, 1280.000000);
	TextDrawAlignment(dutyTD, 3);
	TextDrawColour(dutyTD, 0xFFFFFFFF);
	TextDrawUseBox(dutyTD, 0);
	TextDrawBoxColour(dutyTD, 0x80808080);
	TextDrawSetShadow(dutyTD, 0);
	TextDrawSetOutline(dutyTD, 1);
	TextDrawBackgroundColour(dutyTD, 0x000000FF);
	TextDrawFont(dutyTD, 2);
	TextDrawSetProportional(dutyTD, 1);
	TextDrawSetSelectable(dutyTD, 0);

	PayDayTD[0] = TextDrawCreate(13.294083, 216.250015, "box");
	TextDrawLetterSize(PayDayTD[0], 0.000000, 7.223526);
	TextDrawTextSize(PayDayTD[0], 181.000000, 0.000000);
	TextDrawAlignment(PayDayTD[0], 1);
	TextDrawColour(PayDayTD[0], 0xFFFF00FF);
	TextDrawUseBox(PayDayTD[0], 1);
	TextDrawBoxColour(PayDayTD[0], 0x00000070);
	TextDrawSetShadow(PayDayTD[0], 28);
	TextDrawSetOutline(PayDayTD[0], 2);
	TextDrawBackgroundColour(PayDayTD[0], 0x000000FF);
	TextDrawFont(PayDayTD[0], 1);
	TextDrawSetProportional(PayDayTD[0], 1);
	TextDrawSetSelectable(PayDayTD[0], 0);

	PayDayTD[1] = TextDrawCreate(74.470596, 216.833420, "PAYDAY");
	TextDrawLetterSize(PayDayTD[1], 0.400000, 1.600000);
	TextDrawTextSize(PayDayTD[1], 1280.000000, 1280.000000);
	TextDrawAlignment(PayDayTD[1], 1);
	TextDrawColour(PayDayTD[1], 0xFFFFFFFF);
	TextDrawUseBox(PayDayTD[1], 0);
	TextDrawBoxColour(PayDayTD[1], 0x80808080);
	TextDrawSetShadow(PayDayTD[1], 0);
	TextDrawSetOutline(PayDayTD[1], 1);
	TextDrawBackgroundColour(PayDayTD[1], 0x000000FF);
	TextDrawFont(PayDayTD[1], 3);
	TextDrawSetProportional(PayDayTD[1], 1);
	TextDrawSetSelectable(PayDayTD[1], 0);

	caseTD[0] = TextDrawCreate(360.000000, 208.000000, "_");
	TextDrawFont(caseTD[0], 1);
	TextDrawLetterSize(caseTD[0], 0.583333, 20.999942);
	TextDrawTextSize(caseTD[0], 298.500000, 350.500000);
	TextDrawSetOutline(caseTD[0], 0);
	TextDrawSetShadow(caseTD[0], 0);
	TextDrawAlignment(caseTD[0], 2);
	TextDrawColour(caseTD[0], 1433087914);
	TextDrawBackgroundColour(caseTD[0], 35839);
	TextDrawBoxColour(caseTD[0], 373844032);
	TextDrawUseBox(caseTD[0], 1);
	TextDrawSetProportional(caseTD[0], 1);
	TextDrawSetSelectable(caseTD[0], 0);

	caseTD[1] = TextDrawCreate(360.000000, 278.000000, "_");
	TextDrawFont(caseTD[1], 1);
	TextDrawLetterSize(caseTD[1], 1.349997, -0.349994);
	TextDrawTextSize(caseTD[1], 298.500000, 350.500000);
	TextDrawSetOutline(caseTD[1], 1);
	TextDrawSetShadow(caseTD[1], 0);
	TextDrawAlignment(caseTD[1], 2);
	TextDrawColour(caseTD[1], -1);
	TextDrawBackgroundColour(caseTD[1], 255);
	TextDrawBoxColour(caseTD[1], -1);
	TextDrawUseBox(caseTD[1], 1);
	TextDrawSetProportional(caseTD[1], 1);
	TextDrawSetSelectable(caseTD[1], 0);

	caseTD[2] = TextDrawCreate(353.000000, 362.000000, "USE KEY");
	TextDrawFont(caseTD[2], 2);
	TextDrawLetterSize(caseTD[2], 0.399998, 1.600000);
	TextDrawTextSize(caseTD[2], 399.000000, 67.000000);
	TextDrawSetOutline(caseTD[2], 0);
	TextDrawSetShadow(caseTD[2], 0);
	TextDrawAlignment(caseTD[2], 2);
	TextDrawColour(caseTD[2], -1);
	TextDrawBackgroundColour(caseTD[2], 255);
	TextDrawBoxColour(caseTD[2], 9109759);
	TextDrawUseBox(caseTD[2], 1);
	TextDrawSetProportional(caseTD[2], 1);
	TextDrawSetSelectable(caseTD[2], 1);

	caseTD[3] = TextDrawCreate(357.000000, 310.000000, "opening...");
	TextDrawFont(caseTD[3], 3);
	TextDrawLetterSize(caseTD[3], 0.404166, 1.600000);
	TextDrawTextSize(caseTD[3], 399.000000, 62.000000);
	TextDrawSetOutline(caseTD[3], 0);
	TextDrawSetShadow(caseTD[3], 0);
	TextDrawAlignment(caseTD[3], 2);
	TextDrawColour(caseTD[3], -1);
	TextDrawBackgroundColour(caseTD[3], 255);
	TextDrawBoxColour(caseTD[3], 255);
	TextDrawUseBox(caseTD[3], 1);
	TextDrawSetProportional(caseTD[3], 1);
	TextDrawSetSelectable(caseTD[3], 0);

	caseTD[4] = TextDrawCreate(331.000000, 213.000000, "_");
	TextDrawFont(caseTD[4], 1);
	TextDrawLetterSize(caseTD[4], 0.600000, 15.200020);
	TextDrawTextSize(caseTD[4], 298.500000, 120.500000);
	TextDrawSetOutline(caseTD[4], 1);
	TextDrawSetShadow(caseTD[4], 0);
	TextDrawAlignment(caseTD[4], 2);
	TextDrawColour(caseTD[4], -1);
	TextDrawBackgroundColour(caseTD[4], 255);
	TextDrawBoxColour(caseTD[4], 255);
	TextDrawUseBox(caseTD[4], 1);
	TextDrawSetProportional(caseTD[4], 1);
	TextDrawSetSelectable(caseTD[4], 0);

	caseTD[5] = TextDrawCreate(333.000000, 240.000000, "_");
	TextDrawFont(caseTD[5], 1);
	TextDrawLetterSize(caseTD[5], 0.600000, 11.500000);
	TextDrawTextSize(caseTD[5], 298.500000, 108.500000);
	TextDrawSetOutline(caseTD[5], 1);
	TextDrawSetShadow(caseTD[5], 0);
	TextDrawAlignment(caseTD[5], 2);
	TextDrawColour(caseTD[5], -1);
	TextDrawBackgroundColour(caseTD[5], 255);
	TextDrawBoxColour(caseTD[5], -1061109505);
	TextDrawUseBox(caseTD[5], 1);
	TextDrawSetProportional(caseTD[5], 1);
	TextDrawSetSelectable(caseTD[5], 0);

	caseTD[6] = TextDrawCreate(290.000000, 216.500000, "YOU WON");
	TextDrawFont(caseTD[6], 2);
	TextDrawLetterSize(caseTD[6], 0.404166, 1.649999);
	TextDrawTextSize(caseTD[6], 400.000000, 16.500000);
	TextDrawSetOutline(caseTD[6], 0);
	TextDrawSetShadow(caseTD[6], 0);
	TextDrawAlignment(caseTD[6], 1);
	TextDrawColour(caseTD[6], -1);
	TextDrawBackgroundColour(caseTD[6], 255);
	TextDrawBoxColour(caseTD[6], 50);
	TextDrawUseBox(caseTD[6], 0);
	TextDrawSetProportional(caseTD[6], 1);
	TextDrawSetSelectable(caseTD[6], 0);
	return true;
}