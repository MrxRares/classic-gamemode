#include <YSI_Coding\y_hooks>
#define COURIER_MONEY (1200)
new const Float:courierLoadingLocations[][] = {
    {2175.6521, -2267.3032, 13.5688},
    {2168.3318, -2274.0496, 13.5631},
    {2159.8472, -2280.4531, 13.5454}
};


give_courier_money(playerid)
{
    new job_money = (COURIER_MONEY) + (GetPlayerJobSkill(playerid, JOB_TYPE_COURIER) * 500 + RandomEx((GetPlayerJobSkill(playerid, JOB_TYPE_COURIER) * 400), (GetPlayerJobSkill(playerid, JOB_TYPE_COURIER) * 1000) + 700));
    
    va_SendClientMessage(playerid, COLOR_SERVER, "JOB >> {ffffff}You received $%s for your work. Time elapsed: %s.", FormatNumber(job_money), calculateTime(gettime() - playerStaticVar[playerid][var_jobtime]));
    addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, job_money, false);

    AddJobClanMoney(playerid, job_money);

    return UpdatePlayerJobSkill(playerid, JOB_TYPE_COURIER);
}

start_courier_work(playerid) {
    PlayerTextDrawSetString(playerid, playerTextdraws[playerid][serverJobTD], "~y~Go and load packages in your truck.");
    PlayerTextDrawShow(playerid, playerTextdraws[playerid][serverJobTD]);
    new const randcp = random(sizeof courierLoadingLocations);

    playerStaticVar[playerid][pCheckpoint] = 1001;
    SetPlayerCheckpoint(playerid, courierLoadingLocations[randcp][0], courierLoadingLocations[randcp][1], courierLoadingLocations[randcp][2], 3.5);
    new Float: vPos[3];

    GetPlayerPos(playerid, vPos[0], vPos[1], vPos[2]);
    CreateJobVehicle(playerid, 456, vPos[0], vPos[1], vPos[2], 90.0, 120);

    return true;
}


cancel_courier_work(playerid) {
    RemovePlayerAttachedObject(playerid, INDEX_JOB);
    playerStaticVar[playerid][var_courierstuff] = 0;
    playerStaticVar[playerid][var_courierhouse] = -1;
    return true;
}

timer courier_timer[5000](playerid, job) {
    if(!Iter_Contains(jobPlayers<job_data[playerid][playerJob]>, playerid))
        return true;

    TogglePlayerControllable(playerid, true); 

    playerStaticVar[playerid][b_ACTIVE_TIMER] = false;

    switch(job) {
        case JOB_TYPE_COURIER: {
            new const randcp = random(returnServerHouses);

            playerStaticVar[playerid][var_courierhouse] = randcp;

            SetJobCheckpoint(playerid, houseInfo[randcp][houseExterior][0], houseInfo[randcp][houseExterior][1], houseInfo[randcp][houseExterior][2], 2.0);
            playerStaticVar[playerid][pCheckpoint] = 1002;

            PlayerTextDrawSetString(playerid, playerTextdraws[playerid][serverJobTD], "Go and transport all packages loaded.~N~Packages: 6/6");
        }
    }
    return true;
}

hook OnPlayerEnterCheckpoint(playerid) {
    switch(playerStaticVar[playerid][pCheckpoint]) {
        case 1001: {
            if(!IsPlayerInJobVehicle(playerid))
                return true;

            defer courier_timer(playerid, JOB_TYPE_COURIER); 
            playerStaticVar[playerid][b_ACTIVE_TIMER] = true; 
            TogglePlayerControllable(playerid, false); 

            PlayerTextDrawSetString(playerid, playerTextdraws[playerid][serverJobTD], "~W~Loading packages..."); 

            playerStaticVar[playerid][var_courierstuff] = 6; 
            playerStaticVar[playerid][var_jobtime] = gettime() - 5;
            return true;
        }
        case 1002: {
            if(!IsPlayerAttachedObjectSlotUsed(playerid, INDEX_JOB)) {
                SendClientMessage(playerid, -1, "Nu ai un colet in mana. Du-te langa vehiculul de curierat si apasa tasta 'Y'.");
                return true;
            }

            new randcp;

            RemovePlayerAttachedObject(playerid, INDEX_JOB);
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
            ApplyAnimation(playerid, "CARRY", "PUTDWN", 4.0, 0, 0, 0, 0, 0);
            give_courier_money(playerid);
            if(playerStaticVar[playerid][var_courierstuff] <= 1) {
                randcp = random(3); 
     
                PlayerTextDrawSetString(playerid, playerTextdraws[playerid][serverJobTD], "~W~Go and reload packages in your truck.");

                playerStaticVar[playerid][pCheckpoint] = 1001;
                playerStaticVar[playerid][var_courierhouse] = -1;

                SetPlayerCheckpoint(playerid, courierLoadingLocations[randcp][0], courierLoadingLocations[randcp][1], courierLoadingLocations[randcp][2], 3.5); 
                return true;
            }
            randcp = random(returnServerHouses - 1);

            playerStaticVar[playerid][var_courierhouse] = randcp;
            playerStaticVar[playerid][pCheckpoint] = 1002;

            SetJobCheckpoint(playerid, houseInfo[randcp][houseExterior][0], houseInfo[randcp][houseExterior][1], houseInfo[randcp][houseExterior][2], 2.0);

            --playerStaticVar[playerid][var_courierstuff];

            format(returnString, sizeof returnString, "Go and transport all packages loaded.~N~Packages: %d/6", playerStaticVar[playerid][var_courierstuff]); 
            PlayerTextDrawSetString(playerid, playerTextdraws[playerid][serverJobTD], returnString);
            return true;
        }
    }
    return true;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    switch(newkeys) {
        case KEY_YES: {
            if(Iter_Contains(jobPlayers<job_data[playerid][playerJob]>, playerid) && job_data[playerid][playerJob] == JOB_TYPE_COURIER && !IsPlayerAttachedObjectSlotUsed(playerid, INDEX_JOB) && !IsPlayerInAnyVehicle(playerid) && playerStaticVar[playerid][var_courierhouse] != -1 && IsPlayerInRangeOfPoint(playerid, 75.0, houseInfo[playerStaticVar[playerid][var_courierhouse]][houseExterior][0], houseInfo[playerStaticVar[playerid][var_courierhouse]][houseExterior][1], houseInfo[playerStaticVar[playerid][var_courierhouse]][houseExterior][2])) {
                new Float: X, Float: Y, Float: Z; 
                GetVehiclePos(job_data[playerid][jobVehicle], X, Y, Z);

                if(IsPlayerInRangeOfPoint(playerid, 3.5, X, Y, Z)) {
                    SetPlayerAttachedObject(playerid, INDEX_JOB, 1221, 6, -0.004000, 0.169000, -0.216000, -112.000030, -15.200000, -11.599995, 0.362000, 0.341000, 0.386000); 
                    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
                }
            }
        }
    }
}