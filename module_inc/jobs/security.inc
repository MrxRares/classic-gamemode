
// ------------------------------------- //
//             includes                  //
// ------------------------------------- //

#include <YSI_Coding\y_hooks>

// ------------------------------------- //
//             macros                    //
// ------------------------------------- //

#define SECURITY_MONEY (9000)

new const Float:securityLoadLocations[][] = {
    {1482.8604,-1828.3691,13.5469},
    {2052.5574,-1906.5865,13.5469},
    {2741.6008,-1851.6696,9.6408}
};

// ------------------------------------- //
//             functions                 //
// ------------------------------------- //

give_security_money(playerid)
{
    new job_money = (SECURITY_MONEY) + (GetPlayerJobSkill(playerid, JOB_TYPE_SECURITY) * 500 + RandomEx((GetPlayerJobSkill(playerid, JOB_TYPE_SECURITY) * 400), (GetPlayerJobSkill(playerid, JOB_TYPE_SECURITY) * 700) + 3000));
    
    va_SendClientMessage(playerid, COLOR_SERVER, "JOB >> {ffffff}You received $%s for your work.", FormatNumber(job_money));
    addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, job_money, false);

    AddJobClanMoney(playerid, job_money);

    return UpdatePlayerJobSkill(playerid, JOB_TYPE_SECURITY);
}


start_security_work(playerid) {
    PlayerTextDrawSetString(playerid, playerTextdraws[playerid][serverJobTD], "~y~Go and load the money in the armored truck.");
    PlayerTextDrawShow(playerid, playerTextdraws[playerid][serverJobTD]);
    new const randcp = random(sizeof securityLoadLocations);

    playerStaticVar[playerid][pCheckpoint] = 4446;
    SetPlayerCheckpoint(playerid, securityLoadLocations[randcp][0], securityLoadLocations[randcp][1], securityLoadLocations[randcp][2], 4.5);

    CreateJobVehicle(playerid, 428, 1259.2731,-2024.8851,59.4407, 264.0360, 60);

    return true;
}


cancel_security_work(playerid) {
    DisablePlayerCheckpoint(playerid);
    return true;
}

// ------------------------------------- //
//             hooks                     //
// ------------------------------------- //

hook OnPlayerEnterCheckpoint(playerid) {
    switch(playerStaticVar[playerid][pCheckpoint]) {
        case 4446: {
            if(!IsPlayerInJobVehicle(playerid))
                return true;
           

            PlayerTextDrawSetString(playerid, playerTextdraws[playerid][serverJobTD], "~g~Go to the back of bank and leave the money.");
            PlayerTextDrawShow(playerid, playerTextdraws[playerid][serverJobTD]);

            playerStaticVar[playerid][pCheckpoint] = 4447;
            SetPlayerCheckpoint(playerid, 1427.2821,-966.1925,37.4282, 4.5);
            SendClientMessage(playerid, COLOR_SERVER, "JOB >> {ffffff}Objective updated, check the job informations.");


            return true;
        }
        case 4447: {
            if(!IsPlayerAttachedObjectSlotUsed(playerid, INDEX_JOB)) {
                sendError(playerid, "You don't have the box of the money in hand, go in the back of armored car and use 'Y'.");
                return true;
            }

            RemovePlayerAttachedObject(playerid, INDEX_JOB);
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
            ApplyAnimation(playerid, "CARRY", "PUTDWN", 4.0, 0, 0, 0, 0, 0);
            give_security_money(playerid);
            Dialog_Show(playerid, DIALOG_SECURITY_TRANSPORTER, DIALOG_STYLE_MSGBOX, "Security Info", "Do you want to continue the work or to stop it right now?", "Continue", "Stop");

        }
    }
    return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    switch(newkeys) {
        case KEY_YES: {
            if(Iter_Contains(jobPlayers<job_data[playerid][playerJob]>, playerid) && job_data[playerid][playerJob] == JOB_TYPE_SECURITY && !IsPlayerAttachedObjectSlotUsed(playerid, INDEX_JOB) && !IsPlayerInAnyVehicle(playerid) && IsPlayerInRangeOfPoint(playerid, 40.0, 1427.2821,-966.1925,37.4282)) {
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
// ------------------------------------- //
//             dialogs                   //
// ------------------------------------- //


Dialog:DIALOG_SECURITY_TRANSPORTER(playerid, response, listitem, inputtext[])
{
    if(response)
    {
        start_security_work(playerid);
        SendClientMessage(playerid, COLOR_SERVER, "JOB >> {ffffff}You choosed to continue the job.");

    }
    else
    {
        CallLocalFunction("StopJobWork", "i", playerid);
        SendClientMessage(playerid, COLOR_SERVER, "JOB >> {ffffff}The job was stopped.");
    }
    return true;
}