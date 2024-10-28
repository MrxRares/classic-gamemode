
// ------------------------------------- //
//             includes                  //
// ------------------------------------- //

#include <YSI_Coding\y_hooks>

// ------------------------------------- //
//             macros                    //
// ------------------------------------- //

#define QUARRY_MONEY (10000)

new const Float:quarryCheckpointLocations[][] = {
    {554.9274,909.0286,-42.9609},
    {586.5934,918.7127,-43.0034},
    {633.2866,857.4874,-42.7172}
};

// ------------------------------------- //
//             functions                 //
// ------------------------------------- //

give_quarry_money(playerid)
{
    new job_money = (QUARRY_MONEY) + (GetPlayerJobSkill(playerid, JOB_TYPE_QUARRY) * 500 + RandomEx((GetPlayerJobSkill(playerid, JOB_TYPE_QUARRY) * 400), (GetPlayerJobSkill(playerid, JOB_TYPE_QUARRY) * 700) + 700));
    
    va_SendClientMessage(playerid, COLOR_SERVER, "JOB >> {ffffff}You received $%s for your work.", FormatNumber(job_money));
    addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, job_money, false);

    AddJobClanMoney(playerid, job_money);

    return UpdatePlayerJobSkill(playerid, JOB_TYPE_QUARRY);
}


start_quarry_worker(playerid) {
    PlayerTextDrawSetString(playerid, playerTextdraws[playerid][serverJobTD], "~y~Go and load the ores in the truck.");
    PlayerTextDrawShow(playerid, playerTextdraws[playerid][serverJobTD]);
    new const randcp = random(sizeof quarryCheckpointLocations);

    playerStaticVar[playerid][pCheckpoint] = 4444;
    SetPlayerCheckpoint(playerid, quarryCheckpointLocations[randcp][0], quarryCheckpointLocations[randcp][1], quarryCheckpointLocations[randcp][2], 4.5);
    new Float: vPos[3];

    GetPlayerPos(playerid, vPos[0], vPos[1], vPos[2]);
    CreateJobVehicle(playerid, 486, vPos[0], vPos[1], vPos[2], 90.0, 20);

    return true;
}


cancel_quarry_work(playerid) {
    DisablePlayerCheckpoint(playerid);
    return true;
}

// ------------------------------------- //
//             hooks                     //
// ------------------------------------- //

hook OnPlayerEnterCheckpoint(playerid) {
    switch(playerStaticVar[playerid][pCheckpoint]) {
        case 4444: {
            if(!IsPlayerInJobVehicle(playerid))
                return true;
           

            PlayerTextDrawSetString(playerid, playerTextdraws[playerid][serverJobTD], "~g~Go back and unload the ores to get the money.");
            PlayerTextDrawShow(playerid, playerTextdraws[playerid][serverJobTD]);

            playerStaticVar[playerid][pCheckpoint] = 4445;
            SetPlayerCheckpoint(playerid, 807.9865,862.7345,12.2061, 4.5);
            SendClientMessage(playerid, COLOR_SERVER, "JOB >> {ffffff}Objective updated, check the job informations.");


            return true;
        }
        case 4445: {
            give_quarry_money(playerid);
            Dialog_Show(playerid, DIALOG_QUARRY_WORKER, DIALOG_STYLE_MSGBOX, "Quarry Info", "Do you want to continue the work or to stop it right now?", "Continue", "Stop");

        }
    }
    return 1;
}


// ------------------------------------- //
//             dialogs                   //
// ------------------------------------- //


Dialog:DIALOG_QUARRY_WORKER(playerid, response, listitem, inputtext[])
{
    if(response)
    {
        start_quarry_worker(playerid);
        SendClientMessage(playerid, COLOR_SERVER, "JOB >> {ffffff}You choosed to continue the job.");

    }
    else
    {
        CallLocalFunction("StopJobWork", "i", playerid);
        SendClientMessage(playerid, COLOR_SERVER, "JOB >> {ffffff}The job was stopped.");
    }
    return true;
}