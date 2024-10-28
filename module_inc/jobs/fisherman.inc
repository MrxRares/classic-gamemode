#include <YSI_Coding\y_hooks>

enum e_fisher {
	playerHaveFish, playerFishType, playerFishPrice, playerFishTimer
};
new fish_data[MAX_PLAYERS][e_fisher];


#define MAX_PROGRESS 100
new g_FillAmount[MAX_PLAYERS];
#define DELAY_TIME 1

hook OnPlayerConnect(playerid)
{
	fish_data[playerid][playerHaveFish] = 0;
	fish_data[playerid][playerFishType] = 0;
	fish_data[playerid][playerFishPrice] = 0;

	playerTextdraws[playerid][PestePTD] = CreatePlayerTextDraw(playerid, 317.000000, 335.000000, "use ~y~'CLICK'~w~ to reel the fish~w~");
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][PestePTD], 1);
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][PestePTD], 0.183333, 0.750000);
	PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][PestePTD], 460.500000, 132.500000);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][PestePTD], 1);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][PestePTD], 0);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][PestePTD], 2);
	PlayerTextDrawColour(playerid, playerTextdraws[playerid][PestePTD], -1);
	PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][PestePTD], 255);
	PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][PestePTD], 50);
	PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][PestePTD], 0);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][PestePTD], 1);
	PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][PestePTD], 0);
	PesteTD[playerid] = CreatePlayerProgressBar(playerid, 272, 349, 96, 1.50, -16776961, 100.0);
	
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnGameModeInit()
{
	//Garduri Fish
	CreateDynamicObject(970, 382.19919, -2042.00671, 7.32180,   0.00000, 0.00000, 0.00000,-1,-1,-1,500.0);
	CreateDynamicObject(970, 378.70398, -2045.08789, 7.32180,   0.00000, 0.00000, -91.98001,-1,-1,-1,500.0);
	CreateDynamicObject(970, 378.51715, -2050.44434, 7.32180,   0.00000, 0.00000, -92.76000,-1,-1,-1,500.0);
	CreateDynamicObject(970, 352.48499, -2050.70313, 7.29520, 0.00000, 0.00000, 0.00000,-1,-1,-1,500.0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}


hook OnPlayerDeath(playerid, killerid, reason)
{
	if(fish_data[playerid][playerHaveFish])
	{
		SendClientMessage(playerid, COLOR_GREEN, ""COLOR_SV_HEX">>{ffffff} Deoarece ai murit ai pierdut pestele pe care il aveai in inventar.");
		reset_fish_vars(playerid);
	}
}
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if (newkeys & KEY_FIRE) {
        if (!GetPVarInt(playerid, "inPeste"))
            return true;


        if (gettime() < GetPVarInt(playerid, "commands_delay"))
            return SendClientMessage(playerid, COLOR_SERVER, ""COLOR_SV_HEX">>{ffffff} Mai incet, mulinezi prea repede!");

        SetPVarInt(playerid, "commands_delay", gettime() + DELAY_TIME);

        if (g_FillAmount[playerid] >= MAX_PROGRESS) {
            g_FillAmount[playerid] = 0;

            SetPVarInt(playerid, "inPeste", 0);
            updateFish(playerid, 0, 2);
			StopJobWork(playerid);
			give_fish_money(playerid);
        } else {
            if (g_FillAmount[playerid] + 25 <= MAX_PROGRESS)
                g_FillAmount[playerid] += 25;
            else
                g_FillAmount[playerid] = MAX_PROGRESS;

			ApplyAnimation(playerid,"SWORD","sword_block",50.0 ,0,1,0,1,1);
            updateFish(playerid, g_FillAmount[playerid], 1);
        }
    }
    return true;
}

YCMD:fish(playerid, params[], help)
{
	if(job_data[playerid][playerJob] != JOB_TYPE_FISHER) 
		return sendError(playerid,"You are not a fisherman.");
	
	if(!IsPlayerInRangeOfPoint(playerid, 7.5, jobWorkPositions[JOB_TYPE_FISHER][0], jobWorkPositions[JOB_TYPE_FISHER][1], jobWorkPositions[JOB_TYPE_FISHER][2]) || IsPlayerInAnyVehicle(playerid))
		return sendError(playerid,""COLOR_SV_HEX">>{ffffff} You are not in the fishing zone. [/jobs > Fisherman]");

    if(fish_data[playerid][playerHaveFish])
    	return sendError(playerid,""COLOR_SV_HEX">>{ffffff} Mergi la un magazin 24/7 pentru a vinde pestele prins inainte de a pescui din nou.");

	if(job_data[playerid][jobDuty] == JOB_TYPE_FISHER)
		return sendError(playerid,""COLOR_SV_HEX">>{ffffff} You are already fishing.");

	ApplyAnimation(playerid, "SAMP", "FishingIdle", 4.0, 0, 1, 1, 1, 0, 1);
	SetPlayerAttachedObject(playerid, JOB_INDEX, 18632, 5, 0.217000, 0.089999, -0.139999, -34.000007, 5.899998, -61.399993);
	
	TogglePlayerControllable(playerid, 0);
	
	WhenPlayerStartWorking(playerid);
	GameTextForPlayer(playerid, "~n~~n~~n~~n~~n~~n~~w~FISHING...", 5000, 5);
	fish_data[playerid][playerFishTimer] = defer catch_fish(playerid);
	
	return true;
}

reset_fish_vars(playerid)
{
	fish_data[playerid][playerHaveFish] = 0;
	fish_data[playerid][playerFishType] = 0;
	fish_data[playerid][playerFishPrice] = 0;
	fish_data[playerid][playerFishTimer] = 0; 
}

cancel_fish_work(playerid)
{
	if(job_data[playerid][playerJob] != JOB_TYPE_FISHER)
		return 1;

	reset_fish_vars(playerid);
	return 1;
}

returnFishName(fish_type, action)
{
 	new
 		fishString[24];

 	if(action == 1)
 	{
	 	switch(fish_type)
	 	{
	 		case 1: fishString = "common";
	 		case 2: fishString = "rare";
	 		case 3:
	 		{
	 			#if !defined HOLIDAY_ACTIVE
		 			fishString = "very rare";
		 		#else
		 			fishString = "special christmas";
		 		#endif
	 		}
	 	}
	}
	else if(action == 2)
	{
	 	switch(fish_type)
	 	{
	 		case 1: fishString = "common";
	 		case 2: fishString = "rare";
	 		case 3:
	 		{
	 			#if !defined HOLIDAY_ACTIVE
		 			fishString = "very rare";
		 		#else
		 			fishString = "special christmas";
		 		#endif
	 		}
	 	}
	}
 	return fishString;
}

returnFishPrice(fish_type)
{
	new
		fish_value=0;

	switch(fish_type)
	{
		case 1: fish_value = 4000 + RandomEx(100, 1000);
		case 2: fish_value = 5000 + RandomEx(100, 1000);
		case 3:
		{
			#if !defined HOLIDAY_ACTIVE
				fish_value = 10000 + RandomEx(100, 10000);
			#else
				fish_value = 30000 + RandomEx(100, 10000);
			#endif
		}
	}
	return fish_value;
}

timer catch_fish[5000](playerid)
{
	if(job_data[playerid][jobDuty] == JOB_TYPE_FISHER)
	{
		SendClientMessage(playerid, COLOR_SERVER, ">> {ffffff}Ai prins ceva in carligul unditei, apasa de 4 ori pentru a ridica captura");
		updateFish(playerid, 0, 1);
		g_FillAmount[playerid] = 0;
		SetPVarInt(playerid, "inPeste", 1);
		TogglePlayerControllable(playerid, 0);
	}
	return 1;
}
updateFish(playerid, FillAmount, type) {

	switch(type) {
		case 1: {
			SetPlayerProgressBarColour(playerid, PesteTD[playerid], COLOR_SERVER);
			SetPlayerProgressBarMaxValue(playerid, PesteTD[playerid], MAX_PROGRESS);
			SetPlayerProgressBarValue(playerid, PesteTD[playerid], FillAmount);
			ShowPlayerProgressBar(playerid, PesteTD[playerid]);
			PlayerTextDrawShow(playerid, playerTextdraws[playerid][PestePTD]);
		}

		case 2: {
			DestroyPlayerProgressBar(playerid, PesteTD[playerid]);
			PlayerTextDrawHide(playerid, playerTextdraws[playerid][PestePTD]);
			TogglePlayerControllable(playerid, 1);
		}
	}
	return true;
}
give_fish_money(playerid)
{
	new random_fish = random(99);

	switch(job_data[playerid][playerJobSkill][JOB_TYPE_FISHER])
	{
		case 1:
		{
				if(random_fish >= 0 && random_fish <= 87) fish_data[playerid][playerFishType] = 1;
				else if(random_fish >= 88 && random_fish <= 98) fish_data[playerid][playerFishType] = 2;
				else if(random_fish >= 99) fish_data[playerid][playerFishType] = 3;
		}
		case 2:
		{
				if(random_fish >= 0 && random_fish <= 83) fish_data[playerid][playerFishType] = 1;
				else if(random_fish >= 84 && random_fish <= 97) fish_data[playerid][playerFishType] = 2;
				else if(random_fish >= 98) fish_data[playerid][playerFishType] = 3;
		}
		case 3:
		{
				if(random_fish >= 0 && random_fish <= 79) fish_data[playerid][playerFishType] = 1;
				else if(random_fish >= 80 && random_fish <= 96) fish_data[playerid][playerFishType] = 2;
				else if(random_fish >= 97) fish_data[playerid][playerFishType] = 3;
		}
		case 4:
		{
				if(random_fish >= 0 && random_fish <= 75) fish_data[playerid][playerFishType] = 1;
				else if(random_fish >= 76 && random_fish <= 95) fish_data[playerid][playerFishType] = 2;
				else if(random_fish >= 96) fish_data[playerid][playerFishType] = 3;
		}
		case 5:
		{
				if(random_fish >= 0 && random_fish <= 71) fish_data[playerid][playerFishType] = 1;
				else if(random_fish >= 72 && random_fish <= 94) fish_data[playerid][playerFishType] = 2;
				else if(random_fish >= 95) fish_data[playerid][playerFishType] = 3;
		}
		case 6:
		{
				if(random_fish >= 0 && random_fish <= 67) fish_data[playerid][playerFishType] = 1;
				else if(random_fish >= 68 && random_fish <= 93) fish_data[playerid][playerFishType] = 2;
				else if(random_fish >= 94) fish_data[playerid][playerFishType] = 3;
		}
 	}

 	fish_data[playerid][playerHaveFish] = 1;
 	fish_data[playerid][playerFishPrice] = returnFishPrice(fish_data[playerid][playerFishType]);
	

	ClearAnimations(playerid);

 	va_SendClientMessage(playerid, -1, "Ai prins un peste %s care valoreaza $%s!", returnFishName(fish_data[playerid][playerFishType], 1), FormatNumber(fish_data[playerid][playerFishPrice]));
 	SendClientMessage(playerid, -1, "Pentru a vinde pestele, mergi la un magazin 24/7.");
	sendNearbyMessage(playerid, COLOR_YELLOW, "* %s caught a %s fish that can be sold for about $%s.", GetName(playerid), returnFishName(fish_data[playerid][playerFishType], 2), FormatNumber(fish_data[playerid][playerFishPrice]));

	HidePlayerProgressBar(playerid, PesteTD[playerid]);
	PlayerTextDrawHide(playerid, playerTextdraws[playerid][PestePTD]);
	TogglePlayerControllable(playerid, 1);

	return 1;
}

sell_fish(playerid)
{
	if(fish_data[playerid][playerHaveFish])
		Dialog_Show(playerid, DIALOG_SELLFISH, DIALOG_STYLE_MSGBOX, "Sell fish", "Do you want to sell the fish you are carying?", "Yes", "No");
	
	return 1;
}

Dialog:DIALOG_SELLFISH(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	va_SendClientMessage(playerid, COLOR_GREEN, ""COLOR_SV_HEX">>{ffffff} Ai vandut pestele pentru $%s.", FormatNumber(fish_data[playerid][playerFishPrice]));
	addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, fish_data[playerid][playerFishPrice], false);


	UpdatePlayerJobSkill(playerid, JOB_TYPE_FISHER);
	DailyQuestCheck(playerid, QUEST_TYPE_FISH, 1);
	AddJobClanMoney(playerid, fish_data[playerid][playerFishPrice]);
	reset_fish_vars(playerid);
	return 1;
}
