#include <YSI_Coding\y_hooks>

#define CP_PIZZA (CP_JOBS_BEGIN) + JOB_TYPE_PIZZA
#define PIZZA_MONEY (3200)

enum
{
    PIZZA_TOTAL_BOXES, PIZZA_HOUSE_ID 
}

new
    pizza_data[MAX_PLAYERS][2], pizza_object[MAX_PLAYERS][5];

reset_pizza_vars(playerid)
{
    pizza_data[playerid][PIZZA_TOTAL_BOXES] = 0;
    pizza_data[playerid][PIZZA_HOUSE_ID] = 0;
}

cancel_pizza_work(playerid)
{
    if(job_data[playerid][jobDuty] != JOB_TYPE_PIZZA)
        return 1;

    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
    reset_pizza_vars(playerid);

    return 1;
}

start_pizza_work(playerid)
{
    SendClientMessage(playerid, -1, ""COLOR_SV_HEX"JOB >> {FFFFFF} Mergi la checkpoint si livreza o cutie de pizza pentru a fi platit.");

    if(!IsValidVehicle(job_data[playerid][jobVehicle]))
    {
        new
            Float: vPos[3];

        GetPlayerPos(playerid, vPos[0], vPos[1], vPos[2]);
        CreateJobVehicle(playerid, 448, vPos[0], vPos[1], vPos[2], 90.0, 120);
    }

    for(new i; i < GetPlayerJobSkill(playerid, JOB_TYPE_PIZZA); ++i)
    {
        pizza_object[playerid][i] = CreateDynamicObject(1582, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000);
        AttachDynamicObjectToVehicle(pizza_object[playerid][i], job_data[playerid][jobVehicle], -0.005000, -0.899999, 0.499999 + i * 0.0705, 0.000000, 0.000000, 0.000000);
    }
    pizza_data[playerid][PIZZA_TOTAL_BOXES] = GetPlayerJobSkill(playerid, JOB_TYPE_PIZZA);

    return give_pizza_cp(playerid);
}

give_pizza_cp(playerid)
{    
    new
        house = random(returnServerHouses - 1);
    
    if(house == pizza_data[playerid][PIZZA_HOUSE_ID] && pizza_data[playerid][PIZZA_HOUSE_ID] != 0)
        return give_pizza_cp(playerid);

    DisablePlayerCheckpoint(playerid);
    SetJobCheckpoint(playerid, houseInfo[house][houseExterior][0], houseInfo[house][houseExterior][1], houseInfo[house][houseExterior][2], 2.0);
    
    pizza_data[playerid][PIZZA_HOUSE_ID] = house;
    playerStaticVar[playerid][pCheckpoint] = CP_PIZZA;
    return 1;
}

give_pizza_money(playerid)
{
    new job_money = (PIZZA_MONEY) + (GetPlayerJobSkill(playerid, JOB_TYPE_PIZZA) * 500 + RandomEx((GetPlayerJobSkill(playerid, JOB_TYPE_PIZZA) * 400), (GetPlayerJobSkill(playerid, JOB_TYPE_PIZZA) * 900) + 500));
    
    va_SendClientMessage(playerid, -1, ""COLOR_SV_HEX"JOB >> {FFFFFF} Ai primit $%s pentru aceasta pizza livrata.", FormatNumber(job_money));
    addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, job_money, false);

    AddJobClanMoney(playerid, job_money);

    return UpdatePlayerJobSkill(playerid, JOB_TYPE_PIZZA);
}

hook OnPlayerDisconnect(playerid, reason)
{
    if(job_data[playerid][jobDuty] == JOB_TYPE_PIZZA)
    {
        for(new i; i < GetPlayerJobSkill(playerid, JOB_TYPE_PIZZA); ++i)
        {
            if(IsValidDynamicObject(pizza_object[playerid][i]))
                DestroyDynamicObject(pizza_object[playerid][i]);

            pizza_object[playerid][i] = 0;
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
hook DestroyVehicleEx(vehicleid)
{	
	foreach(new workers : All(jobPlayers<>))
	{
        if(job_data[workers][jobDuty] == JOB_TYPE_PIZZA)
        {
            for(new i; i < GetPlayerJobSkill(workers, JOB_TYPE_PIZZA); ++i)
            {
                if(IsValidDynamicObject(pizza_object[workers][i]))
                    DestroyDynamicObject(pizza_object[workers][i]);

                pizza_object[workers][i] = 0;
            }
        }
    }
	return true;
}
hook OnVehicleDeath(vehicleid)
{
	foreach(new workers : All(jobPlayers<>))
	{
        if(job_data[workers][jobDuty] == JOB_TYPE_PIZZA)
        {
            for(new i; i < GetPlayerJobSkill(workers, JOB_TYPE_PIZZA); ++i)
            {
                if(IsValidDynamicObject(pizza_object[workers][i]))
                    DestroyDynamicObject(pizza_object[workers][i]);

                pizza_object[workers][i] = 0;
            }
        }
    }
	return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if(newkeys == KEY_WALK)
    {
        if(job_data[playerid][jobDuty] == JOB_TYPE_PIZZA)
        {
            new
                house = pizza_data[playerid][PIZZA_HOUSE_ID];
            
            if(!IsPlayerInRangeOfPoint(playerid, 25.0, houseInfo[house][houseExterior][0], houseInfo[house][houseExterior][1], houseInfo[house][houseExterior][2]))
                return 1;

            if(!IsPlayerNearVehicle(playerid, job_data[playerid][jobVehicle], 2.0))
                return 1;

            if(IsPlayerAttachedObjectSlotUsed(playerid, JOB_INDEX))
                return 1;

            if(pizza_data[playerid][PIZZA_TOTAL_BOXES] != 0)
                pizza_data[playerid][PIZZA_TOTAL_BOXES] --;

            if(IsValidDynamicObject(pizza_object[playerid][pizza_data[playerid][PIZZA_TOTAL_BOXES]]))
                DestroyDynamicObject(pizza_object[playerid][pizza_data[playerid][PIZZA_TOTAL_BOXES]]);

            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
            SetPlayerAttachedObject(playerid, JOB_INDEX, 1582, 1, 0.002953, 0.469660, -0.009797, 269.851104, 88.443557, 0.000000, 0.804894, 1.000000, 0.822361);
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEnterCheckpoint(playerid)
{
    if(playerStaticVar[playerid][pCheckpoint] == CP_PIZZA && job_data[playerid][jobDuty] == JOB_TYPE_PIZZA)
    {
        if(!pizza_data[playerid][PIZZA_HOUSE_ID])
        {
            if(!IsPlayerInVehicle(playerid, job_data[playerid][jobVehicle]))
                return 1;

            start_pizza_work(playerid);
            return true;
        }
        else
        {
            if(!IsPlayerAttachedObjectSlotUsed(playerid, JOB_INDEX))
                return sendError(playerid,"Nu ai o pizza in mana. Du-te langa scooter-ul de pizza si apasa tasta 'ALT'.");

            ApplyAnimation(playerid, "CARRY", "LIFTUP05", 4.1, 0, 0, 0, 0, 0);

            if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_CARRY)
                SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

            if(IsPlayerAttachedObjectSlotUsed(playerid, JOB_INDEX))
                RemovePlayerAttachedObject(playerid, JOB_INDEX);

            give_pizza_money(playerid);

            if(pizza_data[playerid][PIZZA_TOTAL_BOXES] == 0)
            {
                SendClientMessage(playerid, -1, ""COLOR_SV_HEX"JOB >> {FFFFFF} Ai livrat toate cutiile de pizza. Pentru a realimenta mergi la checkpoint-ul marcat pe minimap.");
                DailyQuestCheck(playerid, QUEST_TYPE_PIZZA, 1); //2nd check

                reset_pizza_vars(playerid);

                playerStaticVar[playerid][pCheckpoint] = CP_PIZZA;
                return SetJobCheckpoint(playerid, jobWorkPositions[JOB_TYPE_PIZZA][0], jobWorkPositions[JOB_TYPE_PIZZA][1], jobWorkPositions[JOB_TYPE_PIZZA][2], 4.0);
            }
            va_SendClientMessage(playerid, -1, ""COLOR_SV_HEX"JOB >> {FFFFFF} Mai ai %d cutii de pizza. Mergi la checkpoint pentru a continua munca.", pizza_data[playerid][PIZZA_TOTAL_BOXES]);

            DailyQuestCheck(playerid, QUEST_TYPE_PIZZA, 1);
            give_pizza_cp(playerid);
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
    if(newstate == PLAYER_STATE_ONFOOT && oldstate == PLAYER_STATE_DRIVER)
    {
        if(job_data[playerid][jobVehicle] == playerStaticVar[playerid][lastVehicle] && job_data[playerid][jobVehicle] && job_data[playerid][jobDuty] == JOB_TYPE_PIZZA)
        {
            SendClientMessage(playerid, -1, ""COLOR_SV_HEX"JOB >> {FFFFFF} Pentru a lua o pizza de pe vehicul apasa ALT in spatele acestuia.");
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}