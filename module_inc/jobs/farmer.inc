#include <YSI_Coding\y_hooks>
#define FARMER_MONEY (1000)


// ------------------------------------- //
//             timers                    //
// ------------------------------------- //
timer farmerTimer[1000](playerid) {
    if(!(isPlayerInRectangleArea(playerid, -594, -342, -1408.5, -1258.5) || isPlayerInRectangleArea(playerid, -342, -160, -1560.5, -1306.5)))
        return PlayerTextDrawSetString(playerid, playerTextdraws[playerid][serverJobTD], "~R~You must be in the work area.");

    if(playerStaticVar[playerid][var_jobseconds] <= 1) {
        stop playerStaticVar[playerid][var_jobtimer];

        playerStaticVar[playerid][var_jobobject] = CreateDynamicObject(1454, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        AttachDynamicObjectToVehicle(playerStaticVar[playerid][var_jobobject], job_data[playerid][jobVehicle], -0.014999, -2.800016, 0.000000, 0.000000, 7.035000, -89.444953);

        if(!random(2)) 
            SetPlayerCheckpoint(playerid, -567.8726, -1477.4612, 9.9931, 3.5); 

        else 
            SetPlayerCheckpoint(playerid, -368.1149, -1437.5499, 25.6928, 3.5); 
        
        playerStaticVar[playerid][pCheckpoint] = 3;
        PlayerTextDrawSetString(playerid, playerTextdraws[playerid][serverJobTD], "~G~Go to the red marker and leave the bale.");
    }
    else if(getVehicleSpeed(job_data[playerid][jobVehicle]) >= 25) {
        --playerStaticVar[playerid][var_jobseconds];

        format(returnString, sizeof returnString, "~G~Another %d seconds until the work is done.", playerStaticVar[playerid][var_jobseconds]);
        PlayerTextDrawSetString(playerid, playerTextdraws[playerid][serverJobTD], returnString);
    }
    else 
        PlayerTextDrawSetString(playerid, playerTextdraws[playerid][serverJobTD], "~R~You must have more than 25 KM/H speed.");
    
    return 1;
}


// ------------------------------------- //
//                hooks                  //
// ------------------------------------- //

hook OnPlayerEnterCheckpoint(playerid) {
    switch(playerStaticVar[playerid][pCheckpoint]) {
        case 3: {
            DisablePlayerCheckpoint(playerid);
            give_farmer_money(playerid);
            DestroyDynamicObject(playerStaticVar[playerid][var_jobobject]); 

            playerStaticVar[playerid][var_jobobject] = -1; 
            playerStaticVar[playerid][var_jobseconds] = 60;

            playerStaticVar[playerid][var_jobtimer] = repeat farmerTimer(playerid);
        }
    }
    return 1;
}

// ------------------------------------- //
//             job_function              //
// ------------------------------------- //

start_farmer_work(playerid) {
    CreateJobVehicle(playerid, 531, -376.517, -1413.26, 25.6922, 2.7719, 120);

    playerStaticVar[playerid][var_jobseconds] = 60;

    playerStaticVar[playerid][var_jobtimer] = repeat farmerTimer(playerid);
    return true;
}
cancel_farmer_work(playerid) {
    stop playerStaticVar[playerid][var_jobtimer];
    return true;
}
give_farmer_money(playerid)
{
    new job_money = (FARMER_MONEY) + (GetPlayerJobSkill(playerid, JOB_TYPE_FARM) * 400 + RandomEx((GetPlayerJobSkill(playerid, JOB_TYPE_FARM) * 1000), (GetPlayerJobSkill(playerid, JOB_TYPE_FARM) * 1000) + 500));
    
    va_SendClientMessage(playerid, COLOR_SERVER, "JOB >> {ffffff}You received $%s money for your work.", FormatNumber(job_money));
    addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, job_money, false);

    AddJobClanMoney(playerid, job_money);

    return UpdatePlayerJobSkill(playerid, JOB_TYPE_FARM);
}