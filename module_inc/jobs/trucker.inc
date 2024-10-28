#include <YSI_Coding\y_hooks>
#define TRUCKER_MONEY (9000)
new Iterator:truckersChat<MAX_PLAYERS>;

enum e_ga_Trucker {
    Float:tr_X,
    Float:tr_Y,
    Float:tr_Z,
    tr_name[32]
}
returnPlayerZoneName(const playerid) {
    new 
        MapZone:zone_id = GetPlayerMapZone(playerid),
        zone_name[MAX_MAP_ZONE_NAME] = "Unknown";

    if(zone_id != INVALID_MAP_ZONE_ID)
        GetMapZoneName(zone_id, zone_name);

    return zone_name;
}

new const truckerLocations[][e_ga_Trucker] = {
    {1069.1096,-300.2799,73.9851, "Hilltop Farm"},
    {2818.9863,-1089.0309,30.7346, "Los Colinas"},
    {2133.7576, -2135.2368, 13.5469, "Willowfield"},
    {2753.3484, -2474.9907, 13.6484, "Ocean Docks"},
    {1203.2288, 264.7932, 19.5547, "Montgomery"},
    {-1642.6703, 1280.4556, 7.0391, "Esplanade North"},
    {-1968.2876, -2435.1067, 30.6250, "Angel Pine"},
    {2491.6780, -1666.8514, 13.3438, "Ganton"}
};

timer attachTruckerTrailer[1500](playerid) 
    return AttachTrailerToVehicle(playerStaticVar[playerid][var_trucker_trailer], job_data[playerid][jobVehicle]);

give_trucker_money(playerid)
{
    new job_money = (TRUCKER_MONEY) + (GetPlayerJobSkill(playerid, JOB_TYPE_TRUCKER) * 1000 + RandomEx((GetPlayerJobSkill(playerid, JOB_TYPE_TRUCKER) * 1000), (GetPlayerJobSkill(playerid, JOB_TYPE_TRUCKER) * 1000) + 2000));
    
    va_SendClientMessage(playerid, COLOR_SERVER, "JOB >> {ffffff}You received $%s money for your work. Time elapsed: %s.", FormatNumber(job_money), calculateTime(gettime() - playerStaticVar[playerid][var_jobtime]));
    SendClientMessage(playerid, COLOR_SERVER, "JOB >> {ffffff}Go at the marked red dot to load another trailer.");
    addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, job_money, false);

    AddJobClanMoney(playerid, job_money);

    return UpdatePlayerJobSkill(playerid, JOB_TYPE_TRUCKER);
}
cancel_trucker_work(playerid) {
    if(job_data[playerid][playerJob] != JOB_TYPE_TRUCKER)
        return true;

    if(IsValidVehicle(playerStaticVar[playerid][var_trucker_trailer]))
        DestroyVehicle(playerStaticVar[playerid][var_trucker_trailer]); 

    if(Iter_Contains(truckersChat, playerid))
        Iter_Remove(truckersChat, playerid);

    return true;
}

start_trucker_work(playerid) {

    showPlayerTruckerLocations(playerid); 
    TogglePlayerControllable(playerid, false);
    CreateJobVehicle(playerid, 403, -78.0424,-1125.0498,1.0781,87.8594, 120);
    return true;
}
hook OnPlayerEnterCheckpoint(playerid) {
    switch(playerStaticVar[playerid][pCheckpoint]) {
        case 10: {
            if(GetPlayerVehicleID(playerid) != job_data[playerid][jobVehicle])
                return 1;

            if(!IsTrailerAttachedToVehicle(job_data[playerid][jobVehicle]))
                return sendError(playerid,"Trebuie sa ai remorca cu marfa atasata de vehicul.");
            DisablePlayerCheckpoint(playerid);

            DestroyVehicle(playerStaticVar[playerid][var_trucker_trailer]); 

            playerStaticVar[playerid][pCheckpoint] = 11;
            SetPlayerCheckpoint(playerid, jobWorkPositions[9][0], jobWorkPositions[9][1], jobWorkPositions[9][2], 4.0);
            give_trucker_money(playerid);
		    SendClientMessage(playerid, COLOR_SERVER, "(Trucker): {FFFFFF}Go back to the job working point to get another route or use /stopwork.");
            return 1;
        }
        case 11: {
            if(GetPlayerVehicleID(playerid) != job_data[playerid][jobVehicle])
                return 1;
            DisablePlayerCheckpoint(playerid);
            TogglePlayerControllable(playerid, false); 
            showPlayerTruckerLocations(playerid);
        }
    }
    return 1;
}


hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {

	switch(newkeys) {
        case KEY_FIRE: {
            if(job_data[playerid][playerJob] == JOB_TYPE_TRUCKER && Iter_Contains(jobPlayers<job_data[playerid][playerJob]>, playerid) && IsValidVehicle(playerStaticVar[playerid][var_trucker_trailer]) && !IsTrailerAttachedToVehicle(job_data[playerid][jobVehicle]) && GetPlayerVehicleID(playerid) == job_data[playerid][jobVehicle]) {
                new Float: X, Float: Y, Float: Z;

                GetVehiclePos(playerStaticVar[playerid][var_trucker_trailer], X, Y, Z);

                if(IsPlayerInRangeOfPoint(playerid, 15.0, X, Y, Z))
                    AttachTrailerToVehicle(playerStaticVar[playerid][var_trucker_trailer], job_data[playerid][jobVehicle]);
            }
        }
	}

	return true;
}
Dialog:TRUCKER(playerid, response, listitem, inputtext[]) {
    playerStaticVar[playerid][b_ACTIVE_TIMER] = false;

    if(!Iter_Contains(jobPlayers<job_data[playerid][playerJob]>, playerid))
        return 1;

    if(!response)
        return CallLocalFunction("StopJobWork", "i", playerid);

    switch(listitem) {
        case 8..11: {
            if(GetPlayerJobSkill(playerid, JOB_TYPE_TRUCKER) < 2) {
                SendClientMessage(playerid, -1, "Ai nevoie de minim skill 2 pentru a face acest transport.");
                return showPlayerTruckerLocations(playerid);
            }

            playerStaticVar[playerid][var_trucker_cash_bonus] = 15000;
        }
        case 12..15: {
            if(GetPlayerJobSkill(playerid, JOB_TYPE_TRUCKER) < 3) {
                SendClientMessage(playerid, -1, "Ai nevoie de minim skill 3 pentru a face acest transport.");
                return showPlayerTruckerLocations(playerid);
            }

            playerStaticVar[playerid][var_trucker_cash_bonus] = 20000;
        }
        default: {
            playerStaticVar[playerid][var_trucker_cash_bonus] = 10000;
        }
    }

    if(listitem >= 8)
        listitem -= 8;

    if(GetPlayerJobSkill(playerid, JOB_TYPE_TRUCKER) > 3) 
        Iter_Add(truckersChat, playerid);

    va_SendClientMessage(playerid, COLOR_SERVER, "(Trucker): {FFFFFF}You selected the location named '%s' (type: %s). Distance: %0.2fkm.", truckerLocations[listitem][tr_name], playerStaticVar[playerid][var_trucker_cash_bonus] == 10000 ? "legal" : "illegal", GetPlayerDistanceFromPoint(playerid, truckerLocations[listitem][tr_X], truckerLocations[listitem][tr_Y], truckerLocations[listitem][tr_Z])/1000);
    SendClientMessage(playerid, COLOR_SERVER, "(Trucker): {FFFFFF}If your trailer is detached from your vehicle press 'LALT' key to attach it.");
    SendClientMessage(playerid, COLOR_SERVER, "(Trucker): {FFFFFF}If you want to speak with the truckers which are working on this job use command [/trc]. (skill 2 required)");

    new 
        Float: X, Float: Y, Float: Z;

    GetPlayerPos(playerid, X, Y, Z);


    switch(random(4)) {
        case 0: playerStaticVar[playerid][var_trucker_trailer] = AddStaticVehicleEx(435, X, Y, Z,  0.0, random(255), random(255), -1, 0, VEH_TYPE_JOB);
        case 1: playerStaticVar[playerid][var_trucker_trailer] = AddStaticVehicleEx(450, X, Y, Z,  0.0, random(255), random(255), -1, 0, VEH_TYPE_JOB);
        case 2: playerStaticVar[playerid][var_trucker_trailer] = AddStaticVehicleEx(584, X, Y, Z,  0.0, random(255), random(255), -1, 0, VEH_TYPE_JOB);
        case 3: playerStaticVar[playerid][var_trucker_trailer] = AddStaticVehicleEx(591, X, Y, Z,  0.0, random(255), random(255), -1, 0, VEH_TYPE_JOB);
    } 

    defer attachTruckerTrailer(playerid);

    TogglePlayerControllable(playerid, true);

    playerStaticVar[playerid][var_jobtime] = gettime();
    playerStaticVar[playerid][pCheckpoint] = 10; 

    SetPlayerCheckpoint(playerid, truckerLocations[listitem][tr_X], truckerLocations[listitem][tr_Y], truckerLocations[listitem][tr_Z], 4.0);
    return 1;
}


showPlayerTruckerLocations(playerid) {
    playerStaticVar[playerid][b_ACTIVE_TIMER] = true;
    Dialog_Show(playerid, TRUCKER, DIALOG_STYLE_TABLIST_HEADERS, "SERVER: Select Location", "Location Name\tDistance\tNeeded Skill\nHilltop Farm ("COLOR_SV_HEX"Legal{FFFFFF})\t%0.2fkm\t1\nLos Colinas ("COLOR_SV_HEX"Legal{FFFFFF})\t%0.2fkm\t1\nWillowfield ("COLOR_SV_HEX"Legal{FFFFFF})\t%0.2fkm\t1\nOcean Docks ("COLOR_SV_HEX"Legal{FFFFFF})\t%0.2fkm\t1\nMontgomery ("COLOR_SV_HEX"Legal{FFFFFF})\t%0.2fkm\t1\nEsplanade North ("COLOR_SV_HEX"Legal{FFFFFF})\t%0.2fkm\t1\nAngel Pine ("COLOR_SV_HEX"Legal{FFFFFF})\t%0.2fkm\t1\nGanton ("COLOR_SV_HEX"Legal{FFFFFF})\t%0.2fkm\t1\nHilltop Farm ({990000}Illegal{FFFFFF})\t%0.2fkm\t2\nLos Colinas ({990000}Illegal{FFFFFF})\t%0.2fkm\t2\nWillowfield ({990000}Illegal{FFFFFF})\t%0.2fkm\t2\nOcean Docks ({990000}Illegal{FFFFFF})\t%0.2fkm\t2\nMontgomery ({990000}Illegal{FFFFFF})\t%0.2fkm\t3\nEsplanade North ({990000}Illegal{FFFFFF})\t%0.2fkm\t3\nAngel Pine ({990000}Illegal{FFFFFF})\t%0.2fkm\t3\nGanton ({990000}Illegal{FFFFFF})\t%0.2fkm\t3", "Select", "Cancel", GetPlayerDistanceFromPoint(playerid, truckerLocations[0][tr_X], truckerLocations[0][tr_Y], truckerLocations[0][tr_Z])/1000, GetPlayerDistanceFromPoint(playerid, truckerLocations[1][tr_X], truckerLocations[1][tr_Y], truckerLocations[1][tr_Z])/1000, GetPlayerDistanceFromPoint(playerid, truckerLocations[2][tr_X], truckerLocations[2][tr_Y], truckerLocations[2][tr_Z])/1000, GetPlayerDistanceFromPoint(playerid, truckerLocations[3][tr_X], truckerLocations[3][tr_Y], truckerLocations[3][tr_Z])/1000, GetPlayerDistanceFromPoint(playerid, truckerLocations[4][tr_X], truckerLocations[4][tr_Y], truckerLocations[4][tr_Z])/1000, GetPlayerDistanceFromPoint(playerid, truckerLocations[5][tr_X], truckerLocations[5][tr_Y], truckerLocations[5][tr_Z])/1000, GetPlayerDistanceFromPoint(playerid, truckerLocations[6][tr_X], truckerLocations[6][tr_Y], truckerLocations[6][tr_Z])/1000, GetPlayerDistanceFromPoint(playerid, truckerLocations[7][tr_X], truckerLocations[7][tr_Y], truckerLocations[7][tr_Z])/1000, GetPlayerDistanceFromPoint(playerid, truckerLocations[0][tr_X], truckerLocations[0][tr_Y], truckerLocations[0][tr_Z])/1000, GetPlayerDistanceFromPoint(playerid, truckerLocations[1][tr_X], truckerLocations[1][tr_Y], truckerLocations[1][tr_Z])/1000, GetPlayerDistanceFromPoint(playerid, truckerLocations[2][tr_X], truckerLocations[2][tr_Y], truckerLocations[2][tr_Z])/1000, GetPlayerDistanceFromPoint(playerid, truckerLocations[3][tr_X], truckerLocations[3][tr_Y], truckerLocations[3][tr_Z])/1000, GetPlayerDistanceFromPoint(playerid, truckerLocations[4][tr_X], truckerLocations[4][tr_Y], truckerLocations[4][tr_Z])/1000, GetPlayerDistanceFromPoint(playerid, truckerLocations[5][tr_X], truckerLocations[5][tr_Y], truckerLocations[5][tr_Z])/1000, GetPlayerDistanceFromPoint(playerid, truckerLocations[6][tr_X], truckerLocations[6][tr_Y], truckerLocations[6][tr_Z])/1000, GetPlayerDistanceFromPoint(playerid, truckerLocations[7][tr_X], truckerLocations[7][tr_Y], truckerLocations[7][tr_Z])/1000);
    return true;
}


// ------------------------------------- //
//               commands                //
// ------------------------------------- //

YCMD:trc(playerid, params[], help) {
    if(job_data[playerid][playerJob] != JOB_TYPE_TRUCKER)
        return sendError(playerid,"Nu ai job-ul Trucker.");

    if(!Iter_Contains(jobPlayers<job_data[playerid][playerJob]>, playerid))
        return sendError(playerid,"Nu poti folosi aceasta comanda deoarece nu muncesti.");

    if(GetPlayerJobSkill(playerid, JOB_TYPE_TRUCKER) < 2)
        return sendError(playerid,"Ai nevoie de minim skill 2 pentru a folosi acest chat.");

    if(!Iter_Contains(truckersChat, playerid) || playerStaticVar[playerid][var_trucker_chat_delay] > gettime())
        return 1;

    if(isnull(params))
        return sendSyntaxMessage(playerid, "/trc <text>");

    format(returnString, sizeof returnString, "[truckers chat]: Trucker %s (%s): %s, over.", GetName(playerid), returnPlayerZoneName(playerid), params);

    foreach(new i : truckersChat) 
        sendSplittedMessageToPlayer(i, 0xf2cb5eff, 0xf2cb5eff, returnString);

    playerStaticVar[playerid][var_trucker_chat_delay] = gettime() + 2;
    return 1;
}