#include <YSI_Coding\y_hooks>

#define SERVER_JOBS 12
#define JOB_INDEX 1
#define CP_JOBS_BEGIN (100)
enum
{
	JOB_TYPE_NONE, JOB_TYPE_DETECTIVE, JOB_TYPE_DRUGS, JOB_TYPE_PIZZA, JOB_TYPE_BUS, JOB_TYPE_FIREMAN, JOB_TYPE_ARMS,
	JOB_TYPE_GARBAGE, JOB_TYPE_FISHER, JOB_TYPE_TRUCKER, JOB_TYPE_FARM, JOB_TYPE_COURIER, JOB_TYPE_MINER, JOB_TYPE_SECURITY,
	JOB_TYPE_QUARRY
};
enum ENUM_PLAYER_JOB
{
	playerJob, playerJobSkill[SERVER_JOBS], playerJobTimes[SERVER_JOBS], playerJobRem[SERVER_JOBS],
	jobDuty, jobVehicle, jobPassedTime, jobMaxPassSeconds
};
new job_data[MAX_PLAYERS][ENUM_PLAYER_JOB], jobLeftTime[MAX_PLAYERS], Iterator: jobPlayers<SERVER_JOBS, MAX_PLAYERS>;

new Float: jobPositions[][] =
{
	{0.0, 0.0, 0.0}, // civil
	{607.8375, -1438.1827, 14.0654}, // detective
	{-1109.7118, -1654.2681, 76.4047}, // drugs dealer
	{2123.0237, -1788.8237, 13.5547}, // pizza boy
	{1653.2095, -2237.8726, 13.5192}, // bus driver
	{826.6611,-613.4106,16.3359}, // fireman
	{1620.6274, 701.5147, 10.8203}, // arms dealer
	{2503.0554, 2778.5754, 10.8203}, // garbage man
	{368.4112,-2076.5071,7.8359}, // fisher
	{-77.9810,-1136.3452,1.0781}, // trucker
	{-382.202, -1426.27, 26.1244}, //farm
	{2729.94, -2451.42, 17.5937}, //Courier
	{0.0, 0.0, 0.0}, //miner
	{0.0, 0.0, 0.0}, //
	{0.0, 0.0, 0.0} //
};

new Float: jobWorkPositions[][] =
{
	{0.0, 0.0, 0.0}, // civil
	{0.0, 0.0, 0.0}, // detective
	{-1120.0229, -1621.1617, 76.3672}, // drugs dealer
	{2119.5566, -1789.1682, 13.5547}, // pizza boy
	{1679.1526, -2249.9587, 13.4035}, // bus driver
	{797.7844,-612.4662,16.3432}, // fireman
	{1620.6281, 688.0684, 10.8203}, // arms dealer
	{2523.6685, 2775.4744, 10.8203}, // garbage man
	{376.5943,-2088.0820,7.8359}, // fisher
	{-76.2421,-1125.0990,1.0781}, // trucker
	{-375.3607,-1450.2823,25.7266}, //farmer
	{2760.5210,-2437.0789,13.5220}, // Courier
	{0.0, 0.0, 0.0}, //miner
	{0.0, 0.0, 0.0}, //
	{0.0, 0.0, 0.0} //
};

new pointsForSkilling[][] =
{
	{}, // civil
	{}, // detective
	{0, 50, 100, 150}, // drugs dealer
	{0, 26, 34, 49, 89, 99}, // pizza boy 
	{0, 26, 34, 49, 89, 99, 119}, // bus driver
	{0, 25, 50, 75, 100, 125, 150}, //firman
	{0, 100, 200, 300}, // arms dealer
	{0, 26, 34, 49, 89, 99, 119}, // garbage man
	{0, 26, 34, 49, 89, 99, 119}, // fisherman
	{0, 26, 34, 49, 89, 99, 119}, // trucker
	{0, 26, 34, 49, 89, 99, 119}, // farmer
	{0, 26, 34, 49, 89, 99, 119}, // courier
	{0, 26, 34, 49, 89, 99, 119}, // miner
	{},
	{}
};

returnJobName(jobId)
{
	new jString[24];
	switch(jobId)
	{
		case JOB_TYPE_NONE: jString = "None";
		case JOB_TYPE_DETECTIVE: jString = "Detective";
		case JOB_TYPE_DRUGS: jString = "Drugs Manufacturer";
		case JOB_TYPE_PIZZA: jString = "Pizza Boy";
		case JOB_TYPE_BUS: jString = "Bus Driver";
		case JOB_TYPE_FIREMAN: jString = "Fireman";
		case JOB_TYPE_ARMS: jString = "Arms Dealer";
		case JOB_TYPE_GARBAGE: jString = "Garbage Man";
		case JOB_TYPE_FISHER: jString = "Fisherman";
		case JOB_TYPE_TRUCKER: jString = "Trucker";
		case JOB_TYPE_FARM: jString = "Farmer";
		case JOB_TYPE_COURIER: jString = "Courier";
		case JOB_TYPE_MINER: jString = "Miner";
	}
	return jString;
}

returnJobLevel(jobId)
{
	new jobLevel;
	switch(jobId)
	{
		case 0: jobLevel = 1;
		case 1: jobLevel = 2;
		case 2: jobLevel = 3;
		case 3: jobLevel = 1;
		case 4: jobLevel = 1;
		case 5: jobLevel = 1;
		case 6: jobLevel = 3;
		case 7: jobLevel = 1;
		case 8: jobLevel = 1;
		case 9: jobLevel = 1;
		case 10: jobLevel = 1;
		case 11: jobLevel = 2;
		case 12: jobLevel = 2;
	}
	return jobLevel;
}

GetJobCommand(jobId)
{
	new jString[24];
	switch(jobId)
	{
		case JOB_TYPE_DETECTIVE: jString = "/find";
		case JOB_TYPE_DRUGS: jString = "/getseeds";
		case JOB_TYPE_ARMS: jString = "/getmats";
		case JOB_TYPE_FISHER: jString = "/fish";
		default: jString = "/work";
	}
	return jString;
}

hook OnGameModeInit()
{
	for(new i = 1; i <= SERVER_JOBS; ++i)
	{
		format(returnString, sizeof returnString, "Job: "COLOR_SV_HEX"%s\n{FFFFFF}Use "COLOR_SV_HEX"/getjob{FFFFFF} to get the job.\nMinimum level: "COLOR_SV_HEX"%d{FFFFFF}", returnJobName(i), returnJobLevel(i));
		CreateDynamic3DTextLabel(returnString, -1, jobPositions[i][0], jobPositions[i][1], jobPositions[i][2], 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1);
		CreateDynamicPickup(1275, 23, jobPositions[i][0], jobPositions[i][1], jobPositions[i][2], -1);

		switch(i)
		{
			case JOB_TYPE_DRUGS: returnString = ""COLOR_SV_HEX"Drugs Manufacturer{ffffff}\nType "COLOR_SV_HEX"/getseeds{ffffff} to buy seeds\nType "COLOR_SV_HEX"/buydrugs{FFFFFF} to buy drugs "COLOR_SV_HEX"(skill 3+) only{FFFFFF}";
			case JOB_TYPE_PIZZA: returnString = "{FFFFFF}Job: "COLOR_SV_HEX"Pizza Boy\n{FFFFFF}Type "COLOR_SV_HEX"/work{FFFFFF} to get a scooter.";	
			case JOB_TYPE_ARMS: returnString = "{0E7BE9}Materials Pickup!\n\nType /getmats as an Arms Dealer\nto collect materials";
			default: format(returnString, sizeof returnString, "{FFFFFF}Job: "COLOR_SV_HEX"%s\n{FFFFFF}Type "COLOR_SV_HEX"/work{FFFFFF} to start working.", returnJobName(i));
		}
		if(i != JOB_TYPE_DETECTIVE && i != JOB_TYPE_FISHER && i != JOB_TYPE_BUS)
			CreateDynamic3DTextLabel(returnString, -1, jobWorkPositions[i][0], jobWorkPositions[i][1], jobWorkPositions[i][2], 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerSpawn(playerid)
{
	CallLocalFunction("StopJobWork", "i", playerid);

    if(IsPlayerAttachedObjectSlotUsed(playerid, JOB_INDEX))
        RemovePlayerAttachedObject(playerid, JOB_INDEX);
    
    if(GetPlayerSpecialAction(playerid) != SPECIAL_ACTION_NONE)
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid)
{
	jobLeftTime[playerid] = -1;

	reset_fish_vars(playerid);
	reset_pizza_vars(playerid);
	reset_garbage_vars(playerid);
	CallLocalFunction("StopJobWork", "i", playerid);
	playerTextdraws[playerid][serverJobTD] = CreatePlayerTextDraw(playerid, 558.500000, 272.133362, "loading..");
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][serverJobTD], 0.229499, 0.946665);
	PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][serverJobTD], 0.000000, 155.000000);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][serverJobTD], 2);
	PlayerTextDrawColour(playerid, playerTextdraws[playerid][serverJobTD], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][serverJobTD], 1);
	PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][serverJobTD], 0x000000FF);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][serverJobTD], 0);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][serverJobTD], 0);
	PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][serverJobTD], 0x000000FF);
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][serverJobTD], 1);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][serverJobTD], 1);
	PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][serverJobTD], 0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	if(job_data[playerid][jobDuty] != JOB_TYPE_NONE && IsValidVehicle(job_data[playerid][jobVehicle]))
	{
		DestroyVehicle(job_data[playerid][jobVehicle]);
		job_data[playerid][jobVehicle] = INVALID_VEHICLE_ID;
	}
	if(Iter_Contains(jobPlayers<job_data[playerid][playerJob]>, playerid))
	{
		Iter_Remove(jobPlayers<job_data[playerid][playerJob]>, playerid);
	}
	CallLocalFunction("StopJobWork", "i", playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
    if(newstate == PLAYER_STATE_ONFOOT && oldstate == PLAYER_STATE_DRIVER)
    {
        if(job_data[playerid][jobVehicle] == playerStaticVar[playerid][lastVehicle] && job_data[playerid][jobVehicle] && job_data[playerid][jobDuty] != JOB_TYPE_NONE)
        {
        	if(job_data[playerid][playerJob] != JOB_TYPE_BUS && job_data[playerid][playerJob] != JOB_TYPE_TRUCKER)
        	{
            	va_SendClientMessage(playerid, COLOR_GREY, "You have %d seconds to get back into the car, or you will lose the job.", jobLeftTime[playerid]);
        	}
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

check_job_time(playerid)
{
	if(job_data[playerid][jobDuty] == JOB_TYPE_NONE)
		return true;

    if(job_data[playerid][jobPassedTime] + job_data[playerid][jobMaxPassSeconds] > gettime()){
    	SendClientMessage(playerid, -1, "Ai ajuns prea repede la checkpoint.");
		return sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "%s (%d, level %d) might be using teleport-hack on job %s (%d seconds passed).", GetName(playerid), playerid, playerInfo[playerid][pLevel], returnJobName(job_data[playerid][playerJob]), gettime() - job_data[playerid][jobPassedTime]);
    }

	return true;
}

hook OnPlayerEnterCheckpoint(playerid)
{
	check_job_time(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEnterRaceCP(playerid)
{
	check_job_time(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnVehicleSpawn(vehicleid)
{
	foreach(new i : All(jobPlayers<>))
	{
		if(job_data[i][jobDuty] != JOB_TYPE_NONE && job_data[i][jobVehicle] != vehicleid)
			continue;

		SendClientMessage(i, COLOR_GREY, "You've lost the job!");
		CallLocalFunction("StopJobWork", "i", i);

		break;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnVehicleDeath(vehicleid)
{
	foreach(new i : All(jobPlayers<>))
	{
		if(job_data[i][jobDuty] != JOB_TYPE_NONE && job_data[i][jobVehicle] != vehicleid)
			continue;

		SendClientMessage(i, COLOR_GREY, "You've lost the job!");
		CallLocalFunction("StopJobWork", "i", i);

		break;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

GetJobMaxSkill(jobId)
{
	switch(jobId)
	{
		case 0: return -1;
		case 1: return -1;
		case 2: return 3;
		case 3: return 5;
		case 4: return 6;
		case 5: return 3;
		case 6: return 6;
		case 7: return 6;
		case 8: return 6;
		case 9: return 6;
		case 10: return 6;
		case 11: return 6;
		case 12: return 6;
	}
	return -1;
}

stock CreateJobVehicle(playerid, modelId, Float: vPosX, Float: vPoxY, Float: vPosZ, Float: vAngle = 0.0, vreSpawn)
{
	if(IsValidVehicle(job_data[playerid][jobVehicle]))
		return true;

	job_data[playerid][jobVehicle] = AddStaticVehicleEx(modelId, vPosX, vPoxY, vPosZ, vAngle, 1, 1, vreSpawn, 0, VEH_TYPE_JOB);


	if(vreSpawn)
		jobLeftTime[playerid] = vreSpawn;

	vehicleInfo[job_data[playerid][jobVehicle]][vehFuel] = 100;
	PutPlayerInVehicle(playerid, job_data[playerid][jobVehicle], 0);
	return true;
}

AddJobClanMoney(playerid, amount)
{
	if(!playerInfo[playerid][pClan])
		return true;

	new sMoney = (amount / 10);
	AddClanMoney(playerInfo[playerid][pClan], sMoney);

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `ClanMoney` = (select `ClanMoney` FROM `users` WHERE `id` = '%d') + %d where `id` = '%d';", 
		playerInfo[playerid][pSQLID], sMoney, playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");

	return va_SendClientMessage(playerid, -1, ""COLOR_SV_HEX"JOB >> {FFFFFF}In urma muncii tale $%s au fost depozitati in seiful clanului.", FormatNumber(sMoney, ","));
}

GetPlayerJobSkill(playerid, jobId)
	return job_data[playerid][playerJobSkill][jobId];


UpdatePlayerJobSkill(playerid, jobId)
{
	if(job_data[playerid][playerJobRem][jobId] < 1)
	{
		job_data[playerid][playerJobRem][jobId] = pointsForSkilling[jobId][job_data[playerid][playerJobSkill][jobId]];
	}
	job_data[playerid][playerJobTimes][jobId] ++;
	if(job_data[playerid][playerJobSkill][jobId] != GetJobMaxSkill(jobId))
	{
		job_data[playerid][playerJobRem][jobId] --;
		update_skills(playerid); 
		if(!job_data[playerid][playerJobRem][jobId])
		{
			job_data[playerid][playerJobSkill][jobId] ++;
			job_data[playerid][playerJobRem][jobId] = pointsForSkilling[jobId][job_data[playerid][playerJobSkill][jobId]];
			update_skills(playerid);
			va_SendClientMessage(playerid, COLOR_YELLOW, "Felicitari! Ai acum skill %d pentru acest job!", job_data[playerid][playerJobSkill][jobId]);
		}
	}

	return update_skills(playerid);
} 

function:update_skills(playerid) {

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `JobSkill` = '%d %d %d %d %d %d %d %d %d %d %d %d', `JobTimes` = '%d %d %d %d %d %d %d %d %d %d %d %d', `JobRem` = '%d %d %d %d %d %d %d %d %d %d %d %d' where `id` = '%d';",
		job_data[playerid][playerJobSkill][0], job_data[playerid][playerJobSkill][1], job_data[playerid][playerJobSkill][2], job_data[playerid][playerJobSkill][3], job_data[playerid][playerJobSkill][4], job_data[playerid][playerJobSkill][5], job_data[playerid][playerJobSkill][6], job_data[playerid][playerJobSkill][7], job_data[playerid][playerJobSkill][8], job_data[playerid][playerJobSkill][9], job_data[playerid][playerJobSkill][10], job_data[playerid][playerJobSkill][11],
		job_data[playerid][playerJobTimes][0], job_data[playerid][playerJobTimes][1], job_data[playerid][playerJobTimes][2], job_data[playerid][playerJobTimes][3], job_data[playerid][playerJobTimes][4], job_data[playerid][playerJobTimes][5], job_data[playerid][playerJobTimes][6], job_data[playerid][playerJobTimes][7], job_data[playerid][playerJobTimes][8], job_data[playerid][playerJobTimes][9], job_data[playerid][playerJobTimes][10], job_data[playerid][playerJobTimes][11],
		job_data[playerid][playerJobRem][0], job_data[playerid][playerJobRem][1], job_data[playerid][playerJobRem][2], job_data[playerid][playerJobRem][3], job_data[playerid][playerJobRem][4], job_data[playerid][playerJobRem][5], job_data[playerid][playerJobRem][6], job_data[playerid][playerJobRem][7], job_data[playerid][playerJobRem][8], job_data[playerid][playerJobRem][9], job_data[playerid][playerJobRem][10], job_data[playerid][playerJobRem][11], playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");
	return 1;
}
stock SetJobCheckpoint(playerid, Float: x, Float: y, Float: z, Float: size, max_time = 10)
{
	if(GetPlayerDistanceFromPoint(playerid, x, y, z) < 100.0)
		max_time -= 2;

	job_data[playerid][jobPassedTime] = gettime();
	job_data[playerid][jobMaxPassSeconds] = max_time;

	return SetPlayerCheckpoint(playerid, x, y, z, size);
}

stock SetJobRaceCheckpoint(playerid, CP_TYPE:type, Float: x, Float: y, Float: z, Float: nextx, Float: nexty, Float: nextz, Float: size, max_time = 10)
{
	job_data[playerid][jobPassedTime] = gettime();
	job_data[playerid][jobMaxPassSeconds] = max_time;

	return SetPlayerRaceCheckpoint(playerid, CP_TYPE:type, Float:x, Float:y, Float:z, Float:nextx, Float:nexty, Float:nextz, Float:size);
}

stock IsPlayerInJobVehicle(playerid)
{
	if(!IsValidVehicle(job_data[playerid][jobVehicle]))
		return false;

	return IsPlayerInVehicle(playerid, job_data[playerid][jobVehicle]) ? (true) : (false);
}

stock IsAJobVehicle(vehicleid)
{
	foreach(new i : All(jobPlayers<>))
	{
		if(job_data[i][jobVehicle] == vehicleid)
			return true;
	}
	return false;
}


hook WhenPlayerLoggedIn(playerid)
{
	if(!cache_num_rows())
		return Y_HOOKS_CONTINUE_RETURN_1;
	
	new loadString[128];
	cache_get_value_name_int(0, "Job", job_data[playerid][playerJob]);

	if(job_data[playerid][playerJobRem][job_data[playerid][playerJob]] < 1)
	{
		job_data[playerid][playerJobRem][job_data[playerid][playerJob]] = pointsForSkilling[job_data[playerid][playerJob]][job_data[playerid][playerJobSkill][job_data[playerid][playerJob]]];
	}
	cache_get_value_name(0, "JobSkill", loadString);
	sscanf(loadString, "a<i>[12]", job_data[playerid][playerJobSkill]);

	cache_get_value_name(0, "JobTimes", loadString);
	sscanf(loadString, "a<i>[12]", job_data[playerid][playerJobTimes]);

	cache_get_value_name(0, "JobRem", loadString);
	sscanf(loadString, "a<i>[12]", job_data[playerid][playerJobRem]);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:jobs(playerid, params[], help)
{
	if(playerStaticVar[playerid][pCheckpoint])
		return sendError(playerid,"You have an active checkpoint. Use /killcp.");

	new jobs = 1, jString[256];
	
	while(jobs < SERVER_JOBS)
	{
		format(jString, sizeof jString, "%s%s\n", jString, returnJobName(jobs));
		jobs ++;
	}
	Dialog_Show(playerid, DIALOG_JOBS, DIALOG_STYLE_LIST, "Jobs:", jString, "Select", "Close");
	return true;
}

YCMD:gotojob(playerid, params[], help)
{
	check_admin

	new jobs = 1, jString[256];
	
	while(jobs < SERVER_JOBS)
	{
		format(jString, sizeof jString, "%s%s\n", jString, returnJobName(jobs));
		jobs ++;
	}
	Dialog_Show(playerid, DIALOG_GOTOJOBS, DIALOG_STYLE_LIST, "Jobs:", jString, "Select", "Close");
	return true;
}

Dialog:DIALOG_GOTOJOBS(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

/*	if(listitem + 1 == JOB_TYPE_FISHER)
		return Dialog_Show(playerid, DIALOG_JOBLOC, DIALOG_STYLE_MSGBOX, "SERVER: Fisherman Job", "[RO] Alege orasul in care doresti sa pescuiesti\n[EN] Select the city in which you want to fish.", "Los Santos", "Las Venturas");
*/
	SetPlayerPos(playerid, jobPositions[listitem + 1][0], jobPositions[listitem + 1][1], jobPositions[listitem + 1][2]);
    SendClientMessage(playerid, -1, "You have been teleported.");
	return sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s has used /gotojob", GetName(playerid));
}

Dialog:DIALOG_JOBS(playerid, response, listitem, inputtext[])
{
	if(!response)
		return true;

/*	if(listitem + 1 == JOB_TYPE_FISHER)
		return Dialog_Show(playerid, DIALOG_JOBLOC, DIALOG_STYLE_MSGBOX, "SERVER: Fisherman Job", "[RO] Alege orasul in care doresti sa pescuiesti\n[EN] Select the city in which you want to fish.", "Los Santos", "Las Venturas");
*/
	SetPlayerCheckpointEx(playerid, jobPositions[listitem + 1][0], jobPositions[listitem + 1][1], jobPositions[listitem + 1][2], 2.0);
	playerStaticVar[playerid][pCheckpoint] = 1;
	return SendClientMessage(playerid, COLOR_YELLOW, "Mergi la checkpoint-ul marcat pe harta pentru a ajunge la locatia jobului.");
}

Dialog:DIALOG_JOBLOC(playerid, response, listitem, inputtext[])
{
	return SendClientMessage(playerid, COLOR_YELLOW, "Mergi la checkpoint-ul marcat pe harta pentru a ajunge la locatia jobului.");
}

YCMD:work(playerid, params[], help)
{
	if(Iter_Contains(jailedPlayers, playerid)) {
		return startJailWork(playerid);
	}

	if(!job_data[playerid][playerJob] || job_data[playerid][playerJob] == JOB_TYPE_DETECTIVE || job_data[playerid][playerJob] == JOB_TYPE_DRUGS || job_data[playerid][playerJob] == JOB_TYPE_ARMS || job_data[playerid][playerJob] == JOB_TYPE_FISHER)
		return sendError(playerid,"Nu ai un job sau jobul detinut de tine nu beneficiaza de aceasta comanda!");

	if(GetPVarInt(playerid, "delay_work") > gettime())
		return va_SendClientMessage(playerid, COLOR_GREY, "Vei putea folosi aceasta comanda peste %d secunde.", GetPVarInt(playerid, "delay_work") - gettime());

	if(playerStaticVar[playerid][pCheckpoint])
		return SendClientMessage(playerid, COLOR_GREY, "Nu poti folosi aceasta comanda cat timp ai un checkpoint activ!");

	if(job_data[playerid][jobDuty] != JOB_TYPE_NONE)
		return true;

	new
		jobId = job_data[playerid][playerJob];

	if(!IsPlayerInRangeOfPoint(playerid, 5.0, jobWorkPositions[jobId][0], jobWorkPositions[jobId][1], jobWorkPositions[jobId][2]))
	{
		//SendClientMessage(playerid, COLOR_RED, "Limita maxima de jucatori ce pot lucra la acest job a fost atinsa [%d]") //75 la pizza

		SetPlayerCheckpoint(playerid, jobWorkPositions[jobId][0], jobWorkPositions[jobId][1], jobWorkPositions[jobId][2], 5.0);
		playerStaticVar[playerid][pCheckpoint] = 1;
		return sendError(playerid,"Nu esti in locatia potrivita! Ti-a fost setat un checkpoint!");
	}
	#if !defined TEST_SERVER
		SetPVarInt(playerid, "delay_work", gettime() + 50);
	#endif

	return WhenPlayerStartWorking(playerid);
}

YCMD:getjob(playerid, params[], help)
{
	if(job_data[playerid][playerJob])
		return sendError(playerid,"You already have a Job, use /quitjob first.");

	for(new i = 1; i < SERVER_JOBS; i++)
	{
		if(!IsPlayerInRangeOfPoint(playerid, 2.0, jobPositions[i][0], jobPositions[i][1], jobPositions[i][2]))
			continue;

		if(playerInfo[playerid][pLevel] < returnJobLevel(i))
			return sendError(playerid, "Nu te poti angaja la acest job deoarece acesta necesita level %d", returnJobLevel(i));

		job_data[playerid][playerJob] = i;

		va_SendClientMessage(playerid, COLOR_LIGHTBLUE, "Jobul tau este acum %s.", returnJobName(i));
		va_SendClientMessage(playerid, COLOR_LIGHTBLUE, "Pentru a incepe sa muncesti foloseste comanda %s.", GetJobCommand(i));
		break;
	}
	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Job` = '%d' where `id` = '%d';", job_data[playerid][playerJob], playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

YCMD:quitjob(playerid, params[], help)
{
	if(!job_data[playerid][playerJob])
		return SendClientMessage(playerid, COLOR_WHITE, "You don't have a job.");

	if(IsPlayerInAnyVehicle(playerid))
		return SendClientMessage(playerid, COLOR_GREY, "You can't use this while you are in an vehicle!");

	CallLocalFunction("StopJobWork", "i", playerid);
	job_data[playerid][playerJob] = 0;

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Job` = '0' where `id` = '%d';", playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");
	return SendClientMessage(playerid, COLOR_GREY, "You have quit your job!");
}


YCMD:stopwork(playerid, params[], help)
{
	if(!job_data[playerid][playerJob])
		return SendClientMessage(playerid, COLOR_WHITE, "You don't have a job.");

	if(job_data[playerid][jobDuty] == JOB_TYPE_NONE)
		return true;
	
	CallLocalFunction("StopJobWork", "i", playerid);

	return SendClientMessage(playerid, COLOR_GREY, "You have stopped your work!");
}


#include "../module_inc/jobs/detective.inc"
#include "../module_inc/jobs/drugsdealer.inc"

#include "../module_inc/jobs/pizzaboy.inc"
#include "../module_inc/jobs/busdriver.inc"

#include "../module_inc/jobs/fireman.inc"
#include "../module_inc/jobs/garbage.inc"
#include "../module_inc/jobs/fisherman.inc"
#include "../module_inc/jobs/armsdealer.inc"
#include "../module_inc/jobs/quarry.inc"
#include "../module_inc/jobs/farmer.inc"
#include "../module_inc/jobs/trucker.inc"
#include "../module_inc/jobs/courier.inc"
#include "../module_inc/jobs/security.inc"

function:WhenPlayerStartWorking(playerid)
{
	switch(job_data[playerid][playerJob])
	{
		//case JOB_TYPE_NONE: { }
		//case JOB_TYPE_DETECTIVE: { }
		//case JOB_TYPE_DRUGS: { }
		case JOB_TYPE_PIZZA: start_pizza_work(playerid), PlayerTextDrawShow(playerid, playerTextdraws[playerid][serverJobTD]);
		case JOB_TYPE_BUS: start_bus_work(playerid), PlayerTextDrawShow(playerid, playerTextdraws[playerid][serverJobTD]);
		case JOB_TYPE_FIREMAN: start_fireman_work(playerid), PlayerTextDrawShow(playerid, playerTextdraws[playerid][serverJobTD]);
		case JOB_TYPE_FARM: start_farmer_work(playerid), PlayerTextDrawShow(playerid, playerTextdraws[playerid][serverJobTD]);
		//case JOB_TYPE_ARMS: { }
		case JOB_TYPE_GARBAGE: start_garbage_work(playerid), PlayerTextDrawShow(playerid, playerTextdraws[playerid][serverJobTD]);
		case JOB_TYPE_TRUCKER: start_trucker_work(playerid), PlayerTextDrawShow(playerid, playerTextdraws[playerid][serverJobTD]);
		case JOB_TYPE_COURIER : start_courier_work(playerid), PlayerTextDrawShow(playerid, playerTextdraws[playerid][serverJobTD]);
		case JOB_TYPE_SECURITY : start_security_work(playerid), PlayerTextDrawShow(playerid, playerTextdraws[playerid][serverJobTD]);
	}

	Iter_Add(jobPlayers<job_data[playerid][playerJob]>, playerid);
	job_data[playerid][jobDuty] = job_data[playerid][playerJob];
	return true;
}
forward public StopJobWork(playerid);
public StopJobWork(playerid)
{
	if(job_data[playerid][jobDuty] == JOB_TYPE_NONE)
		return true;

	switch(job_data[playerid][playerJob])
	{
		case JOB_TYPE_PIZZA: cancel_pizza_work(playerid);
		case JOB_TYPE_BUS: cancel_bus_work(playerid);
		case JOB_TYPE_FARM: cancel_farmer_work(playerid);
		case JOB_TYPE_GARBAGE: cancel_garbage_work(playerid);
		case JOB_TYPE_TRUCKER: cancel_trucker_work(playerid);
		case JOB_TYPE_COURIER : cancel_courier_work(playerid);
		case JOB_TYPE_FISHER : cancel_fish_work(playerid);
		case JOB_TYPE_ARMS : cancel_arms_work(playerid);
		case JOB_TYPE_QUARRY : cancel_quarry_work(playerid);
		case JOB_TYPE_SECURITY : cancel_security_work(playerid);

	}

	playerStaticVar[playerid][pCheckpoint] = 0, DisablePlayerCheckpoint(playerid), DisablePlayerRaceCheckpoint(playerid);

	if(IsValidVehicle(job_data[playerid][jobVehicle]))
	{
		jobLeftTime[playerid] = -1;
		DestroyVehicle(job_data[playerid][jobVehicle]);
		job_data[playerid][jobVehicle] = INVALID_VEHICLE_ID;
	}

    playerStaticVar[playerid][var_mining_state] = playerStaticVar[playerid][var_mining_checkpoint] = -1;
	if(Iter_Contains(jobPlayers<job_data[playerid][playerJob]>, playerid))
		Iter_Remove(jobPlayers<job_data[playerid][playerJob]>, playerid);

    if(IsPlayerAttachedObjectSlotUsed(playerid, JOB_INDEX))
        RemovePlayerAttachedObject(playerid, JOB_INDEX);
    
    if(GetPlayerSpecialAction(playerid) != SPECIAL_ACTION_NONE)
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

    DestroyDynamicObject(playerStaticVar[playerid][var_jobobject]); 

    playerStaticVar[playerid][var_jobseconds] = playerStaticVar[playerid][var_trucker_trailer] = playerStaticVar[playerid][var_jobobject] = -1; 
	stop playerStaticVar[playerid][var_jobtimer];


	for(new i; i < GetPlayerJobSkill(playerid, JOB_TYPE_PIZZA); ++i)
	{
		if(IsValidDynamicObject(pizza_object[playerid][i]))
			DestroyDynamicObject(pizza_object[playerid][i]);

		pizza_object[playerid][i] = 0;
	}

	TogglePlayerControllable(playerid, true);
	ResetPlayerWeapons(playerid);
	PlayerTextDrawHide(playerid, playerTextdraws[playerid][serverJobTD]);
	PlayerTextDrawHide(playerid, playerTextdraws[playerid][drugs_td]);
	job_data[playerid][jobDuty] = JOB_TYPE_NONE;
	return true;
}
