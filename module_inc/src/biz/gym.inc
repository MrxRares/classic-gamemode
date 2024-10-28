#include <YSI_Coding\y_hooks>
#define MAX_BENCH_PRESS 15
#pragma warning disable 213
new Float:benchPos[][6] = 
{
	{1233.66406, -770.25781, 1083.02344, 356.85840, 0.0, 180.0},
	{1230.7504, -770.2706, 1083.0234, 356.85840, 0.0, 180.0},

	{651.83289, -1864.07336, 4.44531, 3.14159, 0.00000, 0.0},
	{654.58380, -1864.04968, 4.44531, 3.14159, 0.00000, 0.0},
	{657.09918, -1864.08484, 4.44531, 3.14159, 0.00000, 0.0},
	{661.58734, -1864.11182, 4.44531, 3.14159, 0.00000, 0.0},
	{664.35931, -1864.12866, 4.44531, 3.14159, 0.00000, 0.0},
	{672.57727, -1864.66772, 4.44530, 3.14160, 0.00000, 270.0},
	{672.57727, -1869.69617, 4.44530, 3.14160, 0.00000, 270.0},

	{774.09998, 3.33610, 999.71191, 356.85840, 0.00000, 180.0},
	{774.09998, 0.31086, 999.71191, 356.85840, 0.00000, 180.0},
	{774.09998, -3.05918, 999.71191,356.85840, 0.00000, 180.0}

};

enum E_BENCH_PRESS_INFO
{
	benchId, benchKMB, benchOccupied
};
new benchInfo[MAX_BENCH_PRESS][E_BENCH_PRESS_INFO], Timer:bench_timer[MAX_PLAYERS], PlayerBar:GymBar[MAX_PLAYERS];

enum E_PLAYER_BENCH
{
	usingBenchPress, holdingKMB, secondsOnBench, workoutProgress, kmbGetOff, workoutReps, benchDeelay,
	pMuscleLevel, pMuscleReps, pTotalReps, pFightStyle
};
new playerMuscle[MAX_PLAYERS][E_PLAYER_BENCH];
static const resetPlayerBench[E_PLAYER_BENCH];

hook OnGameModeInit()
{
	for(new i; i < sizeof benchPos; i++)
	{
		CreateDynamicObject(2629, benchPos[i][0], benchPos[i][1], benchPos[i][2], benchPos[i][3], benchPos[i][4], benchPos[i][5]);

		new Float:a = benchPos[i][5],
			Float:x = benchPos[i][0] + (-0.80 * floatsin(-a, degrees)),
			Float:y = benchPos[i][1] + (-0.80 * floatcos(-a, degrees)),

			Float:fX = benchPos[i][0] + 0.50 * floatsin(-a, degrees) + 0.45 * floatsin(-a - 90.0, degrees),
			Float:fY = benchPos[i][1] + 0.50 * floatcos(-a, degrees) + 0.45 * floatcos(-a - 90.0, degrees);

		CreateDynamic3DTextLabel("Bench Press\nPress ALT to use this", -1, x, y, benchPos[i][2] + 0.75, 25.0);
		benchInfo[i][benchKMB] = CreateDynamicObject(2913, fX, fY, benchPos[i][2] + 1, benchPos[i][3], 90.0, benchPos[i][5]);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid)
{
	GymBar[playerid] = CreatePlayerProgressBar(playerid, 478.000000, 172.000000, 55.000000, 3.500000, -1962934017, 100.000000);
	SetPlayerProgressBarColour(playerid, GymBar[playerid], 0xb4191dAA);
	
	PreloadAnimLib(playerid, "BENCHPRESS");
	playerMuscle[playerid] = resetPlayerBench;

	RemoveBuildingForPlayer(playerid, 2629, 1233.6641, -770.2578, 1083.0234, 0.25);
	RemoveBuildingForPlayer(playerid, 2629, 653.9297, -1864.0859, 4.4453, 0.25);

	RemoveBuildingForPlayer(playerid, 2627, 773.4922, -2.6016, 999.6719, 0.25);
	RemoveBuildingForPlayer(playerid, 2629, 773.8984, 1.4531, 999.6719, 0.25);
	return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnPlayerDisconnect(playerid, reason)
{
	if(!playerMuscle[playerid][usingBenchPress])
		return Y_HOOKS_CONTINUE_RETURN_1;

	WhenPlayerExitBench(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

update_bench_bar(playerid)
{

	va_PlayerTextDrawSetString(playerid, playerTextdraws[playerid][GymTD], "POWER:~n~REPS:~g~ %d~n~~w~MUSCLE LEVEL:~g~ %d", playerMuscle[playerid][workoutReps], playerMuscle[playerid][pMuscleLevel]);
	PlayerTextDrawShow(playerid, playerTextdraws[playerid][GymTD]);

	SetPlayerProgressBarValue(playerid, GymBar[playerid], playerMuscle[playerid][workoutProgress]);
	ShowPlayerProgressBar(playerid, GymBar[playerid]);
}

timer update_bench_press[1000](playerid)
{
	update_bench_bar(playerid);
	playerMuscle[playerid][secondsOnBench] ++;

	if(playerMuscle[playerid][secondsOnBench] == 100)
		WhenPlayerExitBench(playerid);

	if(playerMuscle[playerid][workoutProgress] && !playerMuscle[playerid][kmbGetOff])
		playerMuscle[playerid][workoutProgress] -= 15;

	if(playerMuscle[playerid][secondsOnBench] == 3 && playerMuscle[playerid][kmbGetOff])
	{
		playerMuscle[playerid][kmbGetOff] = 0;
		ApplyAnimation(playerid, "benchpress", "gym_bp_down", 4.1, 0, 0, 0, 1, 0);
	}
	if(playerMuscle[playerid][secondsOnBench] == 4 && playerMuscle[playerid][usingBenchPress] && !playerMuscle[playerid][holdingKMB])
	{
		DestroyDynamicObject(benchInfo[playerMuscle[playerid][usingBenchPress] - 1][benchKMB]);
		playerMuscle[playerid][holdingKMB] = SetPlayerAttachedObject(playerid, 1, 2913, 6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		
		ApplyAnimation(playerid, "benchpress", "gym_bp_down", 4.1, 0, 0, 0, 1, 0);
		SendClientMessage(playerid, COLOR_YELLOW, "Apasa tasta SPACE in mod repetat pentru a ridica haltera.");
	}
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(newkeys == KEY_WALK)
	{
		if(!playerMuscle[playerid][usingBenchPress])
		{
			for(new i; i < sizeof benchPos; i++)
			{
				if(IsPlayerInRangeOfPoint(playerid, 3.0, benchPos[i][0], benchPos[i][1], benchPos[i][2]) && !benchInfo[i][benchOccupied])
				{
					if(playerMuscle[playerid][benchDeelay] > gettime())
						return va_SendClientMessage(playerid, -1, "Esti obosit! Nu poti impinge la piept timp de %d secunde.", playerMuscle[playerid][benchDeelay] - gettime());

					new Float:a = benchPos[i][5],
						Float:x = benchPos[i][0] + (-0.80 * floatsin(-a, degrees)),
						Float:y = benchPos[i][1] + (-0.80 * floatcos(-a, degrees));

					SetPlayerPos(playerid, x, y, benchPos[i][2] + 0.75);
					SetPlayerFacingAngle(playerid, benchPos[i][5]);

					TogglePlayerControllable(playerid, false);
					ApplyAnimation(playerid, "benchpress", "gym_bp_geton", 4.1, 0, 0, 0, 1, 0);

					SetPlayerCameraPos(playerid, x - 1.5, y + 1.5, benchPos[i][2] + 1.5);
					SetPlayerCameraLookAt(playerid, x, y, benchPos[i][2] + 0.75);

					update_bench_bar(playerid);
					benchInfo[i][benchOccupied] = 1;

					bench_timer[playerid] = repeat update_bench_press(playerid);
					playerMuscle[playerid][usingBenchPress] = i + 1;
					break;
				}
			}
		}
	}
	if(playerMuscle[playerid][usingBenchPress] && !playerMuscle[playerid][kmbGetOff] && playerMuscle[playerid][holdingKMB])
	{
		if(newkeys == KEY_SPRINT)
		{
			playerMuscle[playerid][workoutProgress] += 25;
			if(playerMuscle[playerid][workoutProgress] >= 100)
			{
				ApplyAnimation(playerid, "benchpress", "gym_bp_up_A", 4.1, 0, 0, 0, 1, 0);

				playerMuscle[playerid][workoutReps] ++;
				playerMuscle[playerid][pMuscleReps] ++;
				playerMuscle[playerid][pTotalReps] ++;

				playerMuscle[playerid][kmbGetOff] = 1;
				playerMuscle[playerid][workoutProgress] = 0;
				playerMuscle[playerid][secondsOnBench] = 0;

				DailyQuestCheck(playerid, QUEST_TYPE_BENCH, 1);
			}

			if(50 - playerMuscle[playerid][pMuscleReps] <= 0)
			{
				playerMuscle[playerid][pMuscleLevel] ++;
				va_SendClientMessage(playerid, COLOR_YELLOW, "Felicitari! Muschii tai sunt acum mai puternici. Nivelul lor este acum %d!", playerMuscle[playerid][pMuscleLevel]);
			}
			if(playerMuscle[playerid][workoutReps] > 30)
			{
				SendClientMessage(playerid, COLOR_YELLOW, "Vei putea sa impingi iar la piept peste 30 de secunde!");

				ApplyAnimation(playerid, "benchpress", "gym_bp_getoff", 4.1, 0, 0, 0, 0, 0);
				playerMuscle[playerid][secondsOnBench] = 98;

				playerMuscle[playerid][benchDeelay] = gettime() + 30;
			}

			update_bench_bar(playerid);
		}
		if(newkeys == KEY_SECONDARY_ATTACK)
		{
			ApplyAnimation(playerid, "benchpress", "gym_bp_getoff", 4.1, 0, 0, 0, 0, 0);
			playerMuscle[playerid][secondsOnBench] = 98;
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

WhenPlayerExitBench(playerid)
{
	new i = playerMuscle[playerid][usingBenchPress] - 1,
		Float:a = benchPos[i][5],
		Float:fX = benchPos[i][0] + 0.50 * floatsin(-a, degrees) + 0.45 * floatsin(-a - 90.0, degrees),
		Float:fY = benchPos[i][1] + 0.50 * floatcos(-a, degrees) + 0.45 * floatcos(-a - 90.0, degrees);


	benchInfo[i][benchKMB] = CreateDynamicObject(2913, fX, fY, benchPos[i][2] + 1, benchPos[i][3], 90.0, benchPos[i][5]);
	benchInfo[i][benchOccupied] = 0;

	playerMuscle[playerid][usingBenchPress] = 0;
	playerMuscle[playerid][holdingKMB] = 0;
	playerMuscle[playerid][secondsOnBench] = 0;
	playerMuscle[playerid][workoutProgress] = 0;
	playerMuscle[playerid][kmbGetOff] = 0;
	playerMuscle[playerid][workoutReps] = 0;

	TogglePlayerControllable(playerid, true);
	ClearAnimations(playerid);

	SetCameraBehindPlayer(playerid);
	RemovePlayerAttachedObject(playerid, 1);

	HidePlayerProgressBar(playerid, GymBar[playerid]);
	PlayerTextDrawHide(playerid, playerTextdraws[playerid][GymTD]);

	stop bench_timer[playerid];
	bench_timer[playerid] = Timer:0;

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `MuscleLevel` = '%d', `TotalReps` = '%d' where `id` = '%d';", playerMuscle[playerid][pMuscleLevel], playerMuscle[playerid][pTotalReps], playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");
}

function:GetPunchLevel(playerid) //OnPlayerGiveDamage / OnPlayerDamage
{
	new Float:amount;
	switch(playerMuscle[playerid][pMuscleLevel])
	{
		case 10..19: amount = 2.0;
		case 20..29: amount = 3.0;
		case 30..39: amount = 4.0;
		case 40..49: amount = 5.0;
		case 50..59: amount = 6.0;
		case 60..69: amount = 7.0;
		case 70..79: amount = 8.0;
		case 80..89: amount = 9.0;
		case 90..100: amount = 10.0;
		default: amount = 1.5;
	}
	return floatround(amount);
}

hook WhenPlayerLoggedIn(playerid)
{
	if(!cache_num_rows())
		return Y_HOOKS_CONTINUE_RETURN_1;

	cache_get_value_name_int(0, "fightStyle", playerMuscle[playerid][pFightStyle]);
	SetPlayerFightingStyle(playerid, playerMuscle[playerid][pFightStyle]);

	//playerMuscle[playerid][pMuscleReps] = cache_get_field_content_int(0, "MuscleReps");

	cache_get_value_name_int(0, "TotalReps", playerMuscle[playerid][pTotalReps]);
	cache_get_value_name_int(0, "MuscleLevel",playerMuscle[playerid][pMuscleLevel]);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:debugmusclelevel(playerid, params[], help)
{
	check_owner

	if(sscanf(params, "i", params[0]))
		return sendError(playerid,"/setmusclelevel <Level>");

	playerMuscle[playerid][pMuscleLevel] = params[0];
	va_SendClientMessage(playerid, -1, "Muscle level set to: %d", playerMuscle[playerid][pMuscleLevel]);
	return true;
}

YCMD:buyfightstyle(playerid, params[], help)
{
	if(playerStaticVar[playerid][inBusiness] && bizInfo[playerStaticVar[playerid][inBusiness]][bizType] != 6)
		return sendError(playerid,"You are not at Gym.");

	Dialog_Show(playerid, DIALOG_FIGHTSTYLE, DIALOG_STYLE_LIST, "SERVER: Fighting Styles", "Boxing ($6,000)\nKung Fu ($10,000)\nKnee Head ($10,000)\nGrab & Kick ($10,000)\nElbow ($10,000)\nDefault ($2,000)", "Select", "Exit");
	return true;
}

Dialog:DIALOG_FIGHTSTYLE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

	switch(listitem)
	{
		case 0:
		{
		    if(GetMoney(playerid) < 6000 && GetMilliards(playerid) < 1)
		        return sendError(playerid,"You don't have enough money to do this.");

			substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 6000);
			bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney] += 6000;

			SetPlayerFightingStyle(playerid, FIGHT_STYLE_BOXING);
			playerMuscle[playerid][pFightStyle] = 5;
		}
		case 1:
		{
		    if(GetMoney(playerid) < 10000 && GetMilliards(playerid) < 1)
		        return sendError(playerid,"You don't have enough money to do this.");


			substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 10000);
			bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney] += 10000;

			SetPlayerFightingStyle(playerid, FIGHT_STYLE_KUNGFU);
			playerMuscle[playerid][pFightStyle] = 6;
		}
		case 2:
		{
		    if(GetMoney(playerid) < 10000 && GetMilliards(playerid) < 1)
		        return sendError(playerid,"You don't have enough money to do this.");

			substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 10000);
			bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney] += 10000;

			SetPlayerFightingStyle(playerid, FIGHT_STYLE_KNEEHEAD);
			playerMuscle[playerid][pFightStyle] = 7;
		}
		case 3:
		{
		    if(GetMoney(playerid) < 10000 && GetMilliards(playerid) < 1)
		        return sendError(playerid,"You don't have enough money to do this.");

			substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 10000);
			bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney] += 10000;

			SetPlayerFightingStyle(playerid, FIGHT_STYLE_GRABKICK);
			playerMuscle[playerid][pFightStyle] = 15;
		}
		case 4:
		{
		    if(GetMoney(playerid) < 600 && GetMilliards(playerid) < 1)
		        return sendError(playerid,"You don't have enough money to do this.");

			substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 600);
			bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney] += 600;

			SetPlayerFightingStyle(playerid, FIGHT_STYLE_ELBOW);
			playerMuscle[playerid][pFightStyle] = 16;
		}
		case 5:
		{
		    if(GetMoney(playerid) < 2000 && GetMilliards(playerid) < 1)
		        return sendError(playerid,"You don't have enough money to do this.");

			substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 2000);
			bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney] += 2000;

			SetPlayerFightingStyle(playerid, FIGHT_STYLE_NORMAL);
			playerMuscle[playerid][pFightStyle] = 4;
		}
	}
	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `fightStyle` = '%d' where `id` = '%d';", playerMuscle[playerid][pFightStyle], GetPlayerVirtualWorld(playerid));
	mysql_tquery(SQL, returnString, "", "");

	mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `bizMoney` = '%d' where `id` = '%d';", bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney], GetPlayerVirtualWorld(playerid));
	mysql_tquery(SQL, returnString, "", "");

	SendClientMessage(playerid, -1, "You have successfully purchased this style of fighting.");
	return true;
}

hook WhenPlayerSpawn(playerid) {
	if(playerMuscle[playerid][usingBenchPress]) {
		WhenPlayerExitBench(playerid);
		TogglePlayerControllable(playerid, true);
		ClearAnimations(playerid);

		SetCameraBehindPlayer(playerid);
		RemovePlayerAttachedObject(playerid, 1);

		HidePlayerProgressBar(playerid, GymBar[playerid]);
		PlayerTextDrawHide(playerid, playerTextdraws[playerid][GymTD]);

		stop bench_timer[playerid];
		bench_timer[playerid] = Timer:0;
	}
	return true;
}