// ------------------------------------- //
//             Includes                  //
// ------------------------------------- //

#include <YSI_Coding\y_hooks>

// ------------------------------------- //
//             Variables                 //
// ------------------------------------- //

static Float:jailWorks[][3] = {
    {98.4826, 2462.0090, 16.5051},
    {63.3258, 2445.0791, 16.5051},
    {45.7674, 2428.3926, 16.5051},
    {-30.0063, 2452.8369, 16.5051}
};


// ------------------------------------- //
//             Functions                 //
// ------------------------------------- //


function:startJailWork(playerid)
{
    if(!IsPlayerInRangeOfPoint(playerid, 4.0, 98.1159, 2438.5010, 16.5051))
        return sendError(playerid, "You are not near the jail job.");

    new checkpointIndex = random(sizeof(jailWorks));
    SetPlayerCheckpointEx(playerid, jailWorks[checkpointIndex][0], jailWorks[checkpointIndex][1], jailWorks[checkpointIndex][2], 2.0);
    SendClientMessage(playerid, COLOR_SERVER, "JAIL JOB >> {ffffff}Go and collect the box and bring it to the start point.");
    playerStaticVar[playerid][pCheckpoint] = 8001;
    return true;
}


// ------------------------------------- //
//             Hooks                     //
// ------------------------------------- //

hook OnPlayerEnterCheckpoint(playerid)
{
    playerStaticVar[playerid][pCheckpointPos][0] = -1; playerStaticVar[playerid][pCheckpointPos][1] = -1; playerStaticVar[playerid][pCheckpointPos][2] = -1;

    switch(playerStaticVar[playerid][pCheckpoint])
    {
        case 8000:
        {
            if(!IsPlayerAttachedObjectSlotUsed(playerid, INDEX_JOB)) {
                SendClientMessage(playerid, COLOR_SERVER, "JAIL JOB >> {ffffff}You don't have the box, collect it and come back.");
                return true;
            }

            RemovePlayerAttachedObject(playerid, INDEX_JOB);
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
            ApplyAnimation(playerid, "CARRY", "PUTDWN", 4.0, 0, 0, 0, 0, 0);

            new checkpointIndex = random(sizeof(jailWorks));
            SetPlayerCheckpointEx(playerid, jailWorks[checkpointIndex][0], jailWorks[checkpointIndex][1], jailWorks[checkpointIndex][2], 2.0);

            playerStaticVar[playerid][pCheckpoint] = 8001;
            new seconds = random(26);
            playerInfo[playerid][pJailTime] = playerInfo[playerid][pJailTime] - seconds;
            SendClientMessage(playerid, COLOR_SERVER, "JAIL JOB >> {ffffff}Done! - %d seconds.", seconds);
            SendClientMessage(playerid, COLOR_SERVER, "JAIL JOB >> {ffffff}Go bring another box.");
        }
        case 8001:
        {
            SendClientMessage(playerid, COLOR_SERVER, "JAIL JOB >> {ffffff}Bring the box back to the start point.");
            SetPlayerAttachedObject(playerid, INDEX_JOB, 1221, 6, -0.004000, 0.169000, -0.216000, -112.000030, -15.200000, -11.599995, 0.362000, 0.341000, 0.386000); 
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
            SetPlayerCheckpoint(playerid, 98.1159, 2438.5010, 16.5051, 5.0);
            playerStaticVar[playerid][pCheckpoint] = 8000;
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}