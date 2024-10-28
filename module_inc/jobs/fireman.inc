#include <YSI_Coding\y_hooks>

#define CP_FIRE 500
#define FIREMAN_MONEY (1600)

new Timer:fireman_timer[MAX_PLAYERS], firemanObj[MAX_PLAYERS][2], fireId[MAX_PLAYERS];

new Float:fireVehPos[][4] =
{
	{782.9948,-600.7123,15.9797,270.0000},
	{808.2132,-588.5325,16.0274,88.5998},
	{785.0286,-586.3387,15.9805,4.0517}
};

start_fireman_work(playerid)
{
	if(IsValidVehicle(job_data[playerid][jobVehicle]))
		return true;

	SetPlayerSkin(playerid, 277);
	SetPlayerAttachedObject(playerid, JOB_INDEX, 2690, 6, 0.4000, 0.0500, 0.0000, 0.0000, 270.0000, 180.0000, 1.0000, 1.0000, 1.0000);

	new rId = RandomEx(0, 2);

	CreateJobVehicle(playerid, 552, fireVehPos[rId][0], fireVehPos[rId][1], fireVehPos[rId][2], fireVehPos[rId][3], 180);
	ChangeVehicleColours(job_data[playerid][jobVehicle], 3, 3);

	return give_fireman_cp(playerid);
}

new Float:fireLocations[][3] =
{
	{1028.1431,-1361.8354,13.5691},
	{1231.1265,-1019.9742,32.6016},
	{36.3423,-288.9326,2.0964},
	{1184.4198,-1250.1411,15.1797},
	{1098.7003,-352.2504,73.9922},
	{1016.7606,-1072.9047,28.1635},
	{665.0766,-620.6155,16.3359},
	{737.9355,300.7138,20.2344},
	{1095.9993,-1184.2770,18.3206},
	{975.9055,-1294.3207,13.5469}
};

give_fireman_cp(playerid)
{
	new i = RandomEx(0, 9),
		Float: pPos[3];

	if(fireId[playerid] == i)
		return give_fireman_cp(playerid);

	fireId[playerid] = i;

	GetPlayerPos(playerid, pPos[0], pPos[1], pPos[2]);

	DisablePlayerCheckpoint(playerid);
	SetJobCheckpoint(playerid, fireLocations[i][0], fireLocations[i][1], fireLocations[i][2], 2.0);
	
	va_SendClientMessage(playerid, -1, ""COLOR_SV_HEX"JOB >> {ffffff}Mergi la checkpoint-ul plasat pe minimap si stinge incendiul pentru a fi platit [distanta: %0.2fm]",
	GetDistanceBetweenPoints(pPos[0], pPos[1], pPos[2], fireLocations[i][0], fireLocations[i][1], fireLocations[i][2]));

	if(firemanObj[playerid][0])
		DestroyPlayerObject(playerid, firemanObj[playerid][0]);

	firemanObj[playerid][0] = CreatePlayerObject(playerid, 18690, fireLocations[i][0], fireLocations[i][1], fireLocations[i][2] - 2.5, 0.0, 0.0, 0.0, 25.0);
	return playerStaticVar[playerid][pCheckpoint] = CP_FIRE;
}

timer exting_fire[7000](playerid)
{
	new job_money = FIREMAN_MONEY + (GetPlayerJobSkill(playerid, JOB_TYPE_FIREMAN) * 900 + RandomEx((GetPlayerJobSkill(playerid, JOB_TYPE_FIREMAN) * 400), (GetPlayerJobSkill(playerid, JOB_TYPE_FIREMAN) * 500) + 1000));
    AddJobClanMoney(playerid, job_money);
    addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, job_money, false);

	va_SendClientMessage(playerid, -1, ""COLOR_SV_HEX"JOB >> {ffffff}Incendiul a fost stins. Ai primit $%s.", FormatNumber(job_money, ","));
	SendClientMessage(playerid, -1, ""COLOR_SV_HEX"JOB >> {ffffff}Get back to your vehicle and drive to the checkpoint.");
	UpdatePlayerJobSkill(playerid, JOB_TYPE_FIREMAN);
	give_fireman_cp(playerid);
	DestroyPlayerObject(playerid, firemanObj[playerid][1]);

	RemovePlayerAttachedObject(playerid, JOB_INDEX);
	SetPlayerAttachedObject(playerid, JOB_INDEX, 2690, 6, 0.4000, 0.0500, 0.0000, 0.0000, 270.0000, 180.0000, 1.0000, 1.0000, 1.0000);

	ClearAnimations(playerid);
	fireman_timer[playerid] = Timer:0;

}

hook OnPlayerEnterCheckpoint(playerid)
{
	if(playerStaticVar[playerid][pCheckpoint] == CP_FIRE)
	{
		if(IsPlayerInAnyVehicle(playerid))
			return sendError(playerid,""COLOR_SV_HEX"JOB >> {ffffff}Pentru a stinge incendiul trebuie sa cobori din vehicul.");

		new Float:pPos[6],
			i = fireId[playerid];

		GetPlayerPos(playerid, pPos[0], pPos[1], pPos[2]);
		GetVehiclePos(job_data[playerid][jobVehicle], pPos[3], pPos[4], pPos[5]);

		if(GetDistanceBetweenPoints(pPos[0], pPos[1], pPos[2], pPos[3], pPos[4], pPos[5]) > 40)
			return sendError(playerid,""COLOR_SV_HEX"JOB >> {ffffff}You are not near your vehicle.");
		
		SendClientMessage(playerid, -1, ""COLOR_SV_HEX"JOB >> {ffffff}Ai inceput sa stingi incendiul. Acest proces dureaza aproximativ 7 secunde.");
		GameTextForPlayer(playerid, "~w~extinguishing the fire....", 7000, 3);

		ApplyAnimation(playerid, "SHOP", "SHP_Gun_Aim", 4.1, 1, 0, 0, 0, 0);

		RemovePlayerAttachedObject(playerid, JOB_INDEX);
		SetPlayerAttachedObject(playerid, JOB_INDEX, 2690, 6, 0.1000, 0.0500, -0.3300, 0.0000, 0.0000, 180.0000, 1.0000, 1.0000, 1.0000);

		firemanObj[playerid][1] = CreatePlayerObject(playerid, 18720, fireLocations[i][0], fireLocations[i][1], fireLocations[i][2] - 2, 0.0, 0.0, 0.0, 25.0);
		fireman_timer[playerid] = defer exting_fire(playerid);

		playerStaticVar[playerid][pCheckpoint] = 0;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook StopJobWork(playerid)
{
	if(job_data[playerid][jobDuty] != JOB_TYPE_FIREMAN)
		return Y_HOOKS_CONTINUE_RETURN_1;

	fireId[playerid] = 0;

	SetPlayerSkin(playerid, playerInfo[playerid][pSkin]);
	RemovePlayerAttachedObject(playerid, JOB_INDEX);
	ClearAnimations(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid)
{
	PreloadAnimLib(playerid, "SHOP");
	return Y_HOOKS_CONTINUE_RETURN_1;
}