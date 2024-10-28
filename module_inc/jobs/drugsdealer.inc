// ------------------------------------- //
//             Includes                  //
// ------------------------------------- //

#include <YSI_Coding\y_hooks>

// ------------------------------------- //
//             Constants                 //
// ------------------------------------- //

#define CP_DRUGS 200

// ------------------------------------- //
//             Enums                     //
// ------------------------------------- //

enum
{
    DRUGS_OFFER_PLAYER, DRUGS_OFFER_PRICE, DRUGS_OFFER_AMOUNT, DRUG_OFFER_TYPE
}

enum drugTypes {
    DRUG_TYPE_NONE,
    DRUG_TYPE_COCAINE = 1,
    DRUG_TYPE_MUSHROOMS = 2,
    DRUG_TYPE_MARIJUANA = 3
}

// ------------------------------------- //
//             Variables                 //
// ------------------------------------- //

new drugs_object[MAX_PLAYERS], drug_timer[MAX_PLAYERS], drugSell[MAX_PLAYERS][4];

new const modelIDs[] = {
    -1,     // DRUG_TYPE_NONE
    2247,  // DRUG_TYPE_COCAINE
    863,  // DRUG_TYPE_MUSHROOMS
    862   // DRUG_TYPE_MARIJUANA
};

// ------------------------------------- //
//             Functions                 //
// ------------------------------------- //

ResetDrugsVars(playerid)
{
    drugSell[playerid][DRUGS_OFFER_PLAYER] = INVALID_PLAYER_ID;
    drugSell[playerid][DRUGS_OFFER_AMOUNT] = 0;
    drugSell[playerid][DRUGS_OFFER_PRICE] = 0;
    drugSell[playerid][DRUG_OFFER_TYPE] = DRUG_TYPE_NONE;

    return true;
}

returnDrugsName(drugTypes:category)
{
    new cString[55];
    switch(category)
    {
        case DRUG_TYPE_NONE: cString = "none";
        case DRUG_TYPE_COCAINE: cString = "cocaine";
        case DRUG_TYPE_MUSHROOMS: cString = "mushrooms";
        case DRUG_TYPE_MARIJUANA: cString = "marijuana";
    }
    return cString;
}

returnDrugTime(drugTypes:category)
{
    new cString;
    switch(category)
    {
        case DRUG_TYPE_NONE: cString = 0;
        case DRUG_TYPE_COCAINE: cString = 45000;
        case DRUG_TYPE_MUSHROOMS: cString = 60000;
        case DRUG_TYPE_MARIJUANA: cString = 20000;
    }
    return cString;
}

setPlayerAddiction(playerid, drugTypes:drug) {

    playerStaticVar[playerid][var_addiction_procent] += returnDrugsAddiction(drug);
    if(returnAddictionProcent(drug, playerid) >= 100) {
        playerStaticVar[playerid][var_addiction_procent] = 100;
        UpdatePVars(playerid, "Addiction", playerStaticVar[playerid][var_addiction_procent]);
    }
    return true;
}

returnDrugsAction(drugTypes:category)
{
    new cString[55];
    switch(category)
    {
        case DRUG_TYPE_NONE: cString = "none";
        case DRUG_TYPE_COCAINE: cString = "injected himself cocaine";
        case DRUG_TYPE_MUSHROOMS: cString = "eated some mushrooms";
        case DRUG_TYPE_MARIJUANA: cString = "created himself one joint of marijuana and smokes it";
    }
    return cString;
}

returnDrugProcess(drugTypes:category)
{
    new cString[55];
    switch(category)
    {
        case DRUG_TYPE_NONE: cString = "";
        case DRUG_TYPE_COCAINE: cString = "drying leaves";
        case DRUG_TYPE_MUSHROOMS: cString = "gather mushrooms";
        case DRUG_TYPE_MARIJUANA: cString = "drying leaves";
    }
    return cString;
}


returnDrugProcess2(drugTypes:category)
{
    new cString[55];
    switch(category)
    {
        case DRUG_TYPE_NONE: cString = "";
        case DRUG_TYPE_COCAINE: cString = "planting seeds";
        case DRUG_TYPE_MUSHROOMS: cString = "gather mushrooms";
        case DRUG_TYPE_MARIJUANA: cString = "planting seeds";
    }
    return cString;
}

returnDrugsTimes(drugTypes:category, playerid)
{
    new cString;
    switch(category)
    {
        case DRUG_TYPE_NONE: cString = 0;
        case DRUG_TYPE_COCAINE: cString = playerStaticVar[playerid][var_cocaine_times];
        case DRUG_TYPE_MUSHROOMS: cString = playerStaticVar[playerid][var_mushrooms_times];
        case DRUG_TYPE_MARIJUANA: cString = playerStaticVar[playerid][var_drugs_times];
    }
    return cString;
}

updateDrugs(drugTypes:category, amount, playerid)
{
    new cString;
    switch(category)
    {
        case DRUG_TYPE_NONE: cString = 0;
        case DRUG_TYPE_COCAINE: cString = playerInfo[playerid][pCocaine] += amount;
        case DRUG_TYPE_MUSHROOMS: cString = playerInfo[playerid][pMushrooms] += amount;
        case DRUG_TYPE_MARIJUANA: cString = playerInfo[playerid][pDrugs] += amount;
    }
    return cString;
}

Float:randomDrugsFloat(Float:min, Float:max)
{
    if (max <= min) return min;
    return min + (float(random(100000)) / 100000.0 * (max - min));
}

Float:returnHealthPercent(drugTypes:category, addiction)
{
    new Float:healthPercent;
    new Float:baseHealthMin, Float:baseHealthMax;

    switch (category)
    {
        case DRUG_TYPE_NONE:
        {
            baseHealthMin = 0.0;
            baseHealthMax = 0.0;
        }

        case DRUG_TYPE_COCAINE:
        {
            baseHealthMin = 5.50;
            baseHealthMax = 10.50;
        }

        case DRUG_TYPE_MUSHROOMS:
        {
            baseHealthMin = 3.20;
            baseHealthMax = 8.40;
        }

        case DRUG_TYPE_MARIJUANA:
        {
            baseHealthMin = 19.50;
            baseHealthMax = 39.20;
        }
    }

    healthPercent = randomDrugsFloat(baseHealthMin, baseHealthMax);

    new Float:addictionFactor = 1.0 - (float(addiction) / 100.0);

    healthPercent *= addictionFactor;

    if (healthPercent < 1.0)
    {
        healthPercent = 1.0;
    }

    return healthPercent;
}

returnAddictionProcent(drugTypes:category, playerid)
{
    new cString;
    switch(category)
    {
        case DRUG_TYPE_NONE: cString = 0;
        case DRUG_TYPE_COCAINE: cString = playerStaticVar[playerid][var_addiction_procent];
        case DRUG_TYPE_MUSHROOMS: cString = playerStaticVar[playerid][var_addiction_procent];
        case DRUG_TYPE_MARIJUANA: cString = playerStaticVar[playerid][var_addiction_procent];
    }
    return cString;
}

returnDrugsAddiction(drugTypes:category)
{
    new cString;
    switch(category)
    {
        case DRUG_TYPE_NONE: cString = 0;
        case DRUG_TYPE_COCAINE: cString = RandomEx(2 , 5);
        case DRUG_TYPE_MUSHROOMS: cString = RandomEx(5 , 10);
        case DRUG_TYPE_MARIJUANA: cString = RandomEx(1 , 2);
    }
    return cString;
}


accept_drugs(playerid, targetid, drugTypes:drugType)
{
    if(drugSell[playerid][DRUGS_OFFER_PLAYER] == INVALID_PLAYER_ID)
        return sendError(playerid,"No-one offerd you any drugs.");

    if(drugSell[playerid][DRUGS_OFFER_PLAYER] != targetid)
        return sendError(playerid,"This player has not offered you drugs.");
    
    if(GetDistanceBetweenPlayers(playerid, targetid) > 5)
        return sendError(playerid,"This player is not near you.");

    if(GetMoney(playerid) <  drugSell[playerid][DRUGS_OFFER_PRICE] && GetMilliards(playerid) < 1)
        return SendClientMessage(playerid, COLOR_GREY,"You can't afford those drugs.");


    if(drugType == DRUG_TYPE_COCAINE) {
        playerInfo[playerid][pCocaine] += drugSell[playerid][DRUGS_OFFER_AMOUNT];
        playerInfo[targetid][pCocaine] -= drugSell[playerid][DRUGS_OFFER_AMOUNT];
    }
    else if(drugType == DRUG_TYPE_MUSHROOMS) {
        playerInfo[playerid][pMushrooms] += drugSell[playerid][DRUGS_OFFER_AMOUNT];
        playerInfo[targetid][pMushrooms] -= drugSell[playerid][DRUGS_OFFER_AMOUNT];
    }
    else if(drugType == DRUG_TYPE_MARIJUANA) {
        playerInfo[playerid][pDrugs] += drugSell[playerid][DRUGS_OFFER_AMOUNT];
        playerInfo[targetid][pDrugs] -= drugSell[playerid][DRUGS_OFFER_AMOUNT];
    }


    substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, drugSell[playerid][DRUGS_OFFER_PRICE]);
    addMoney(targetid, GetMilliards(targetid), GetMoney(targetid), 0, drugSell[playerid][DRUGS_OFFER_PRICE], false);

    va_SendClientMessage(playerid, -1, "{B8FFDB}* You bought %d gram of %s for $%s from Drug Manufacturer %s.", drugSell[playerid][DRUGS_OFFER_AMOUNT], returnDrugsName(drugType), FormatNumber(drugSell[playerid][DRUGS_OFFER_PRICE]), GetName(targetid));
    va_SendClientMessage(targetid, -1, "{B8FFDB}* %s has bought your drugs. the $%s was added to your PayCheck.", GetName(playerid), FormatNumber(drugSell[playerid][DRUGS_OFFER_PRICE]));
    ResetDrugsVars(playerid);
    ResetDrugsVars(targetid);
    return true;
}

function:collect_seeds(playerid, step)
{
    new
        final_drugs = 1 + random(6), drugType = playerStaticVar[playerid][var_selected_seeds];

    if(step == 1)
    {
        if(playerStaticVar[playerid][var_selected_seeds] == DRUG_TYPE_COCAINE) {
            SendClientMessage(playerid, -1, ""COLOR_SV_HEX"DRUGS MANUFACTURER >> {FFFFFF} Go to the lab and cook the plant. Cooking process takes a few seconds.");
            PlayerTextDrawSetString(playerid, playerTextdraws[playerid][drugs_td], "drugs manufacturer~n~status: ~r~cooking");
            PlayerTextDrawShow(playerid, playerTextdraws[playerid][drugs_td]);

            TogglePlayerControllableEx(playerid, true);
            SetPVarInt(playerid, "drugs_step", 2);

            SetPlayerCheckpoint(playerid, -1101.6586,-1612.0826,76.4321, 1.0);
            playerStaticVar[playerid][pCheckpoint] = CP_DRUGS;
            DestroyDynamicObject(drugs_object[playerid]);
        }
        else if(playerStaticVar[playerid][var_selected_seeds] == DRUG_TYPE_MUSHROOMS) {
            va_SendClientMessage(playerid, -1, ""COLOR_SV_HEX"DRUGS MANUFACTURER >> {FFFFFF} You received %d %s by following the process.", final_drugs, returnDrugsName(drugType));
            
            TogglePlayerControllableEx(playerid, true);
            UpdatePlayerJobSkill(playerid, JOB_TYPE_DRUGS);
            updateDrugs(drugType, final_drugs, playerid);

            PlayerTextDrawHide(playerid, playerTextdraws[playerid][drugs_td]);
            DestroyDynamicObject(drugs_object[playerid]);
            DisablePlayerCheckpoint(playerid);
        }
        else if(playerStaticVar[playerid][var_selected_seeds] == DRUG_TYPE_MARIJUANA){
            SendClientMessage(playerid, -1, ""COLOR_SV_HEX"DRUGS MANUFACTURER >> {FFFFFF} Go to the checkpoint and dry these leaves to get good drugs to use. Drying process takes a few seconds.");

            PlayerTextDrawSetString(playerid, playerTextdraws[playerid][drugs_td], "drugs manufacturer~n~status: ~r~drying leaves");
            PlayerTextDrawShow(playerid, playerTextdraws[playerid][drugs_td]);

            TogglePlayerControllableEx(playerid, true);
            SetPVarInt(playerid, "drugs_step", 2);

            SetPlayerCheckpoint(playerid, -1107.8488, -1615.2081, 76.3739, 1.0);
            playerStaticVar[playerid][pCheckpoint] = CP_DRUGS;
            DestroyDynamicObject(drugs_object[playerid]);
        }
    }
    else if(step == 2)
    {
        if(playerStaticVar[playerid][var_selected_seeds] == DRUG_TYPE_COCAINE) {
            va_SendClientMessage(playerid, -1, ""COLOR_SV_HEX"DRUGS MANUFACTURER >> {FFFFFF} You received %d grams of %s by following the process.", final_drugs, returnDrugsName(drugType));
            
            TogglePlayerControllableEx(playerid, true);
            UpdatePlayerJobSkill(playerid, JOB_TYPE_DRUGS);
            updateDrugs(drugType, final_drugs, playerid);
        }
        else if(playerStaticVar[playerid][var_selected_seeds] == DRUG_TYPE_MARIJUANA) {
            va_SendClientMessage(playerid, -1, ""COLOR_SV_HEX"DRUGS MANUFACTURER >> {FFFFFF} You received %d grams of %s by following the process.", final_drugs, returnDrugsName(drugType));
            
            TogglePlayerControllableEx(playerid, true);
            UpdatePlayerJobSkill(playerid, JOB_TYPE_DRUGS);
            updateDrugs(drugType, final_drugs, playerid);
        }
        StopJobWork(playerid);
    }
    mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Cocaine` = '%d', `Mushrooms` = '%d', `Drugs` = '%d' where `id` = '%d';", playerInfo[playerid][pCocaine], playerInfo[playerid][pMushrooms], playerInfo[playerid][pDrugs], playerInfo[playerid][pSQLID]);
    return mysql_tquery(SQL, returnString, "", "");
}

// ------------------------------------- //
//             Commands                  //
// ------------------------------------- //

YCMD:usedrugs(playerid, params[], help)
{

    if(GetPVarInt(playerid, "delay_drugs") > gettime())
        return va_SendClientMessage(playerid, -1, "Poti folosi droguri peste %d secunde.", GetPVarInt(playerid, "delay_drugs") - gettime());
        
    if(IsPlayerInAnyVehicle(playerid)) 
        return sendError(playerid, "You can't use this command right now.");

    if(GetPVarInt(playerid, "using_drugs") == 1)
        return true;

    new drugString[64], message[128];

    drugString[0] = '\0';

    if (playerInfo[playerid][pCocaine]) {
        strcat(drugString, "cocaine");
    }

    if (playerInfo[playerid][pDrugs]) {
        if (strlen(drugString) > 0) {
            strcat(drugString, "/");
        }
        strcat(drugString, "marijuana");
    }

    if (playerInfo[playerid][pMushrooms]) {
        if (strlen(drugString) > 0) {
            strcat(drugString, "/");
        }
        strcat(drugString, "mushrooms");
    }

    if (strlen(drugString) > 0) {
        format(message, sizeof(message), "/usedrugs [type] [%s]", drugString);
    } else {
        return SendClientMessage(playerid, COLOR_GREY, "You don't any drugs to use.");
    }
    if(!strcmp(params, "cocaine")) {
        playerStaticVar[playerid][var_drug_used] = DRUG_TYPE_COCAINE; 
    }
    else if(!strcmp(params, "mushrooms")) {
        playerStaticVar[playerid][var_drug_used] = DRUG_TYPE_MUSHROOMS; 
    }
    else if(!strcmp(params, "marijuana")) {
        playerStaticVar[playerid][var_drug_used] = DRUG_TYPE_MARIJUANA;
    }
    else 
        return SendClientMessage(playerid, COLOR_SERVER, ""COLOR_SV_HEX"USAGE >> {FFFFFF}%s", message);
    
    
    SetPVarInt(playerid, "using_drugs", 1);
    startTakingDrugs(playerid, playerStaticVar[playerid][var_drug_used]);
    return mysql_tquery(SQL, returnString, "", "");
}

function:startTakingDrugs(playerid, drugTypes:drugs)
{
    if(!GetPVarInt(playerid, "using_drugs"))
        return true;

    new time = returnDrugTime(drugs);
    FreezePlayer(playerid, 4);
    switch(drugs)
    {
        case DRUG_TYPE_NONE: {
            
        }
        case DRUG_TYPE_MARIJUANA: {
            SetPlayerDrunkLevel(playerid, 4000);
            OnePlayAnim(playerid, "SMOKING", "M_smk_in", 4.0, 0, 0, 0, 0, 0);
            playerInfo[playerid][pDrugs] -= 1;
            playerStaticVar[playerid][var_drugs_times] += 1;
            setPlayerAddiction(playerid, drugs);
            mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Drugs` = '%d' where `id` = '%d';", playerInfo[playerid][pDrugs], playerInfo[playerid][pSQLID]);

        }
        case DRUG_TYPE_COCAINE:{
            SetPlayerDrunkLevel(playerid, 4000);
            playerStaticVar[playerid][var_cocaine_times] += 1;
            playerInfo[playerid][pCocaine] -= 1;
            setPlayerAddiction(playerid, drugs);
            OnePlayAnim(playerid, "CRACK", "crckdeth2", 4.0, 0, 0, 0, 0, 0);
            mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Cocaine` = '%d' where `id` = '%d';", playerInfo[playerid][pCocaine], playerInfo[playerid][pSQLID]);
        }
        case DRUG_TYPE_MUSHROOMS:{
            SetPlayerDrunkLevel(playerid, 13000);
            SetPlayerWeather(playerid, 145);
            SetPlayerTime(playerid, 14, 0);
            playerInfo[playerid][pMushrooms] -= 1;
            playerStaticVar[playerid][var_mushrooms_times] += 1;
            setPlayerAddiction(playerid, drugs);
            OnePlayAnim(playerid, "FOOD", "EAT_Burger", 4.0, 0, 0, 0, 0, 0);
            mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Mushrooms` = '%d' where `id` = '%d';", playerInfo[playerid][pMushrooms], playerInfo[playerid][pSQLID]);
        }
    }
    new Float:healthEffect = returnHealthPercent(drugs, playerStaticVar[playerid][var_addiction_procent]);
    drug_timer[playerid] = SetTimerEx("DrugEffectGone", time, false, "dd", playerid, drugs);
    sendNearbyMessage(playerid, COLOR_PURPLE, "%s (%d) %s [+%.2f health (%d times), addiction procent: %d].", GetName(playerid), playerid, returnDrugsAction(drugs), healthEffect, returnDrugsTimes(drugs, playerid), returnAddictionProcent(drugs, playerid));
   
    new Float:newHealth = player_data[playerid][playerHealth] + healthEffect;
    if (newHealth < 0.0) newHealth = 0.0;
    if (newHealth > 100.0) newHealth = 100.0;

    SetPlayerHealthEx(playerid, newHealth);
    SetPVarInt(playerid, "delay_drugs", gettime() + 100);
    DailyQuestCheck(playerid, QUEST_TYPE_DRUGS, 1);
    return true;
}

YCMD:drugsstats(playerid, params[], help) {
    SendClientMessage(playerid, COLOR_SERVER, "Drugs manufacturer skill:{ffffff} %d", job_data[playerid][playerJobSkill][JOB_TYPE_DRUGS]);
    SendClientMessage(playerid, COLOR_SERVER, "Drugs addiction:{ffffff} %d/100", playerStaticVar[playerid][var_addiction_procent]);
    SendClientMessage(playerid, COLOR_SERVER, "Drugs in your pocket:{ffffff} %d/20 grams of marijuana, %d/20 grams of cocaine, %d/20 mushrooms", playerInfo[playerid][pDrugs], playerInfo[playerid][pCocaine], playerInfo[playerid][pMushrooms]);
    return true;
}
function:DrugEffectGone(playerid, drugTypes:drugs)
{
    if(!GetPVarInt(playerid, "using_drugs"))
        return true;

    new returnHour, returnMinute;

    DeletePVar(playerid, "using_drugs");

    KillTimer(drug_timer[playerid]);
    gettime(returnHour, returnMinute);
    SetPlayerDrunkLevel(playerid, 0);
    SetPlayerTime(playerid, returnHour, returnMinute);
    SetPlayerWeather(playerid, 0);
    return true;
}

YCMD:getseeds(playerid, params[], help)
{
	if(job_data[playerid][playerJob] != JOB_TYPE_DRUGS)
		return sendError(playerid,"Invalid job! You are not a Drugs Manufacturer.");

	if(job_data[playerid][jobDuty] == JOB_TYPE_DRUGS)
		return sendError(playerid,"Nu poti folosi aceasta comanda acum deoarece te afli deja in procesul de a crea un drog.");

	if(!IsPlayerInRangeOfPoint(playerid, 2.0, jobWorkPositions[JOB_TYPE_DRUGS][0] + 2.5, jobWorkPositions[JOB_TYPE_DRUGS][1], jobWorkPositions[JOB_TYPE_DRUGS][2]))
	{
		SetPlayerCheckpoint(playerid, jobWorkPositions[JOB_TYPE_DRUGS][0] + 2.5, jobWorkPositions[JOB_TYPE_DRUGS][1], jobWorkPositions[JOB_TYPE_DRUGS][2], 2.0);
		playerStaticVar[playerid][pCheckpoint] = 1;
		return sendError(playerid,"You are not in right place to get seeds.");
	}

    Dialog_Show(playerid, DIALOG_GETSEEDS, DIALOG_STYLE_TABLIST_HEADERS, "Available drugs seeds:", "Drug Type\tEffect Time\tAddiction\n\
        Marijuana\t0:20 min\t1-2%\n\
        Cocaine\t0:45 min\t2-5%\n\
        Mushrooms\t1:00 min\t5-10%\n\
        ", "Buy", "Exit");

    return true;
}

Dialog:DIALOG_GETSEEDS(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    switch(listitem)
    {
        case 0: {
            if(playerInfo[playerid][pDrugs] >= 20)
                return sendError(playerid, "You have the maximum of 20 grams of marijuana.");

            playerStaticVar[playerid][var_selected_seeds] = DRUG_TYPE_MARIJUANA;
            SendClientMessage(playerid, -1, ""COLOR_SV_HEX"DRUGS MANUFACTURER >> {FFFFFF} Go to the marked location on your map to plant your seeds.");

        }
        case 1: {
            if(playerInfo[playerid][pDrugs] >= 20)
                return sendError(playerid, "You have the maximum of 20 grams of cocaine.");

            playerStaticVar[playerid][var_selected_seeds] = DRUG_TYPE_COCAINE;
            SendClientMessage(playerid, -1, ""COLOR_SV_HEX"DRUGS MANUFACTURER >> {FFFFFF} Go to the marked location on your map to plant your seeds.");

        }

        case 2: {
            if(playerInfo[playerid][pDrugs] >= 20)
                return sendError(playerid, "You have the maximum of 20 of mushrooms.");

            playerStaticVar[playerid][var_selected_seeds] = DRUG_TYPE_MUSHROOMS;
            SendClientMessage(playerid, -1, ""COLOR_SV_HEX"DRUGS MANUFACTURER >> {FFFFFF} Go to the marked location on your map to collect mushrooms.");
        }
    }

    SetPVarInt(playerid, "drugs_step", 1);
    WhenPlayerStartWorking(playerid);
    va_PlayerTextDrawSetString(playerid, playerTextdraws[playerid][drugs_td], "drugs manufacturer~n~status: ~r~%s", returnDrugProcess2(playerStaticVar[playerid][var_selected_seeds]));

    PlayerTextDrawShow(playerid, playerTextdraws[playerid][drugs_td]);

    switch(random(3))
    {
        case 0: SetPlayerCheckpoint(playerid, -1068.9921, -1632.8057, 76.3747, 2.0);
        case 1: SetPlayerCheckpoint(playerid, -1078.4031, -1667.0356, 76.4110, 2.0);
        case 2: SetPlayerCheckpoint(playerid, -1054.3735, -1677.7054, 76.9012, 2.0);
    }
    playerStaticVar[playerid][pCheckpoint] = CP_DRUGS;
    return true;
}
YCMD:buydrugs(playerid, params[], help) {

    if(job_data[playerid][playerJob] != JOB_TYPE_DRUGS)
        return sendError(playerid,"Invalid job! You are not a Drugs Manufacturer.");

    if(job_data[playerid][jobDuty] == JOB_TYPE_DRUGS)
        return sendError(playerid,"Nu poti folosi aceasta comanda acum deoarece te afli deja in procesul de a crea un drog.");

    if(!IsPlayerInRangeOfPoint(playerid, 2.0, jobWorkPositions[JOB_TYPE_DRUGS][0] + 2.5, jobWorkPositions[JOB_TYPE_DRUGS][1], jobWorkPositions[JOB_TYPE_DRUGS][2]))
    {
        SetPlayerCheckpoint(playerid, jobWorkPositions[JOB_TYPE_DRUGS][0] + 2.5, jobWorkPositions[JOB_TYPE_DRUGS][1], jobWorkPositions[JOB_TYPE_DRUGS][2], 2.0);
        playerStaticVar[playerid][pCheckpoint] = 1;
        return sendError(playerid,"You are not in right place to buy drugs.");
    }

    new iType[20], iAmount;
    if(GetPlayerJobSkill(playerid, JOB_TYPE_DRUGS) < 3)
        return sendError(playerid,"You can use this command only on skill 3.");

    if(sscanf(params, "s[20]i", iType, iAmount))
        return sendSyntaxMessage(playerid, "/buydrugs [cocaine/mushroom/marijuana] [quantity]");

    if(iAmount <= 0)
        return sendError(playerid,"Invalid quantity, please enter a value bigger than 0");

    new totalPrice = iAmount * 50;

    if(GetMoney(playerid) <  totalPrice && GetMilliards(playerid) < 1)
        return sendError(playerid,"You don't have enough money.");


    if(strmatch(iType, "cocaine")) {
        playerInfo[playerid][pCocaine] += iAmount;
        SendClientMessage(playerid, -1, "You bought %d gram of cocaine for %d$", iAmount, totalPrice);
    }
    else if(strmatch(iType, "mushroom")) {
        playerInfo[playerid][pMushrooms] += iAmount;
        SendClientMessage(playerid, -1, "You bought %d gram of mushrooms for %d$", iAmount, totalPrice);
    }
    else if(strmatch(iType, "marijuana")) {
        playerInfo[playerid][pDrugs] += iAmount;
        SendClientMessage(playerid, -1, "You bought %d gram of marijuana for %d$", iAmount, totalPrice);
    }
    else
    {
        return SendClientMessage(playerid, -1, "Available drugs: cocaine, mushroom, marijuana.");
    }

    UpdatePVars(playerid, "Drugs", playerInfo[playerid][pDrugs]);
    UpdatePVars(playerid, "Cocaine", playerInfo[playerid][pCocaine]);
    UpdatePVars(playerid, "Mushrooms", playerInfo[playerid][pMushrooms]);
    substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, totalPrice);
    return true;
}

YCMD:selldrugs(playerid, params[], help)
{
    if(job_data[playerid][playerJob] != JOB_TYPE_DRUGS)
        return sendError(playerid,"You are not a Drugs Manufacturer.");

    new targetid, iDrugs, iPrice, iType[20];

    if(sscanf(params, "us[20]ii", targetid, iType, iDrugs, iPrice))
        return sendSyntaxMessage(playerid, "/selldrugs [playername/playerid] [cocaine/marijuana/mushrooms] [quantity] [price]");

    if(!IsPlayerConnected(targetid))
        return sendError(playerid,"Invalid player.");

    if(iPrice < 1 || iPrice > 150000)
        return sendError(playerid,"Price not lower then 1$, or above 150,000$.");

    if(playerid == targetid)
        return sendError(playerid,"You can not sell yourself drugs.");

    if(GetDistanceBetweenPlayers(playerid, targetid) > 25)
        return sendError(playerid,"This player is not near you.");
    
    if(iDrugs < 1 || iDrugs > 50)
        return sendError(playerid,"Grams not lower then 1, or above 50.");

    if(strmatch(iType, "cocaine")) {
        if(iDrugs > playerInfo[playerid][pCocaine])
            return sendError(playerid,"You dont have that much cocaine with you.");

        drugSell[targetid][DRUG_OFFER_TYPE] = DRUG_TYPE_COCAINE;
    }
    else if(strmatch(iType, "mushroom")) {
        if(iDrugs > playerInfo[playerid][pMushrooms])
            return sendError(playerid,"You dont have that much mushrooms with you.");

        drugSell[targetid][DRUG_OFFER_TYPE] = DRUG_TYPE_MUSHROOMS;

    }
    else if(strmatch(iType, "marijuana")) {
        if(iDrugs > playerInfo[playerid][pDrugs])
            return sendError(playerid,"You dont have that much marijuana with you.");

        drugSell[targetid][DRUG_OFFER_TYPE] = DRUG_TYPE_MARIJUANA;

    }
    else
    {
        return SendClientMessage(playerid, -1, "Available drugs: cocaine, mushroom, marijuana.");
    }

    va_SendClientMessage(playerid, -1, "{B8FFDB}* You offerd %s to buy %d gram of %s for $%s.", GetName(targetid), iDrugs, returnDrugsName(drugSell[targetid][DRUG_OFFER_TYPE]), FormatNumber(iPrice));
    va_SendClientMessage(targetid, -1, "{B8FFDB}* Drug Manufacturer %s wants to sell you %d grams of %s for $%s, (type /accept drugs %d) to buy.", GetName(playerid), iDrugs, returnDrugsName(drugSell[targetid][DRUG_OFFER_TYPE]), FormatNumber(iPrice), playerid);

    drugSell[targetid][DRUGS_OFFER_PLAYER] = playerid;
    drugSell[targetid][DRUGS_OFFER_AMOUNT] = iDrugs;
    drugSell[targetid][DRUGS_OFFER_PRICE] = iPrice;
    return true;
}

// ------------------------------------- //
//             Hooks                     //
// ------------------------------------- //

hook StopJobWork(playerid)
{
    if(job_data[playerid][jobDuty] != JOB_TYPE_DRUGS)
        return Y_HOOKS_CONTINUE_RETURN_1;

    if(IsValidDynamicObject(drugs_object[playerid]))
        DestroyDynamicObject(drugs_object[playerid]);

    PlayerTextDrawHide(playerid, playerTextdraws[playerid][drugs_td]);
    DeletePVar(playerid, "drugs_step");

    ResetDrugsVars(playerid);
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook WhenPlayerLoggedIn(playerid)
{
    if(!cache_num_rows())
        return Y_HOOKS_CONTINUE_RETURN_1;

    cache_get_value_name_int(0, "Drugs", playerInfo[playerid][pDrugs]);
    cache_get_value_name_int(0, "Cocaine", playerInfo[playerid][pCocaine]);
    cache_get_value_name_int(0, "Mushrooms", playerInfo[playerid][pMushrooms]);
    cache_get_value_name_int(0, "Addiction", playerStaticVar[playerid][var_addiction_procent]);
    return Y_HOOKS_CONTINUE_RETURN_1;
}



hook OnPlayerEnterCheckpoint(playerid)
{
	if(playerStaticVar[playerid][pCheckpoint] == CP_DRUGS && job_data[playerid][jobDuty] == JOB_TYPE_DRUGS)
	{
		if(GetPVarInt(playerid, "drugs_step") == 1)
		{
			DisablePlayerCheckpoint(playerid);

			va_PlayerTextDrawSetString(playerid, playerTextdraws[playerid][drugs_td], "drugs manufacturer~n~status: ~r~%s", returnDrugProcess(playerStaticVar[playerid][var_selected_seeds]));
			PlayerTextDrawShow(playerid, playerTextdraws[playerid][drugs_td]);

			new Float: actualPos[3], modelID = modelIDs[playerStaticVar[playerid][var_selected_seeds]];
			GetPlayerPos(playerid, actualPos[0], actualPos[1], actualPos[2]);

			actualPos[2] = actualPos[2] - 2;

			drugs_object[playerid] = CreateDynamicObject(modelID, actualPos[0], actualPos[1], actualPos[2], 0.0, 0.0, 0.0, 0, 0, playerid);
			MoveDynamicObject(drugs_object[playerid], actualPos[0], actualPos[1], actualPos[2]+1, 0.1, 0.0, 0.0, 0.0);

            TogglePlayerControllableEx(playerid, false);
            ClearAnimations(playerid);
            ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 1, 0, 0, 0, 0);

			return SetTimerEx("collect_seeds", 10000, false, "dd", playerid, 1);
		}
		else if(GetPVarInt(playerid, "drugs_step") == 2)
		{
			DisablePlayerCheckpoint(playerid);
			TogglePlayerControllableEx(playerid, false);

			return SetTimerEx("collect_seeds", 5000, false, "dd", playerid, 2);
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnGameModeInit()
{
    CreateDynamicActor(29, -1120.0229, -1621.1617, 76.3672, 265.9845, 1, 100.0, 0, 0, -1); // actor job

    //mapping
    CreateDynamicObject(956, 662.41998, -552.15997, 15.71000,   0.00000, 0.00000, 180.00000);
    CreateDynamicObject(3242, -1113.09412, -1656.71301, 77.19380,   0.00000, 0.00000, 91.00000);
    CreateDynamicObject(1348, -1098.52124, -1627.18213, 76.12950,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1348, -1100.27295, -1627.18213, 76.12950,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1348, -1102.00073, -1627.18213, 76.12950,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1348, -1103.80334, -1627.18213, 76.12950,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(9093, -1097.15576, -1622.88428, 77.09070,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1348, -1105.57422, -1627.18213, 76.12950,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(9093, -1097.14575, -1622.88428, 79.13070,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(988, -1097.22021, -1620.46204, 76.41320,   0.00000, 0.00000, 89.10000);
    CreateDynamicObject(929, -1108.56726, -1626.93396, 76.28650,   0.00000, 0.00000, 89.49990);
    CreateDynamicObject(929, -1112.92981, -1626.94421, 76.28650,   0.00000, 0.00000, -89.99990);
    CreateDynamicObject(1575, -1098.04980, -1614.51184, 75.43190,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1580, -1098.07495, -1614.56226, 75.73850,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(2463, -1098.74133, -1614.41040, 75.35130,   0.00000, 0.00000, 0.09980);
    CreateDynamicObject(1575, -1098.90063, -1614.51184, 75.43190,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1575, -1098.06982, -1614.42175, 76.10180,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1580, -1098.88574, -1614.56226, 75.73850,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1580, -1098.07495, -1614.56226, 76.44850,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1575, -1098.92065, -1614.42175, 76.10180,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1575, -1098.06982, -1614.42175, 76.81190,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1580, -1098.87415, -1614.56226, 76.44850,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1580, -1098.06335, -1614.56226, 77.17850,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1575, -1098.88062, -1614.42175, 76.80190,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1580, -1098.87415, -1614.56226, 77.17850,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(3632, -1101.21521, -1614.72534, 75.82270,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(3632, -1101.89795, -1614.72302, 75.82270,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(3632, -1102.56714, -1614.74646, 75.82270,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(925, -1104.63647, -1615.26111, 76.37860,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(3577, -1118.89941, -1625.73364, 76.08030,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(2345, -1107.70715, -1614.71655, 76.82130,   9.90000, -0.10000, 2.80000);
    CreateDynamicObject(0, -1118.63330, -1622.48120, 76.12930,   0.00000, 0.00000, -89.79990);
    CreateDynamicObject(2060, -1118.66736, -1622.51733, 76.23350,   0.00000, -0.29990, 91.39980);
    CreateDynamicObject(0, -1107.40295, -1614.66956, 78.05780,   0.00000, 0.00000, 90.90000);
    CreateDynamicObject(19087, -1108.79871, -1614.65283, 76.94700,   -0.29990, -89.80000, -0.10000);
    CreateDynamicObject(2357, -1118.68225, -1621.21753, 75.75410,   0.00000, 0.00000, 89.69990);
    CreateDynamicObject(18648, -1109.39465, -1614.70032, 78.05780,   0.00000, 0.00000, 90.90000);
    CreateDynamicObject(2345, -1110.80457, -1614.66040, 76.85200,   9.90000, -0.10000, 0.70000);
    CreateDynamicObject(19087, -1111.21973, -1614.65051, 76.95540,   -0.29990, -89.80000, -0.10000);
    CreateDynamicObject(2295, -1118.72107, -1619.90576, 75.94640,   -2.60010, 2.40000, -3.60010);
    CreateDynamicObject(2926, -1118.66211, -1619.83276, 76.14930,   0.00000, 0.00000, -89.99990);
    CreateDynamicObject(18648, -1111.35645, -1614.73157, 78.05780,   0.00000, 0.00000, 90.90000);
    CreateDynamicObject(19087, -1113.64172, -1614.64856, 76.96380,   -0.29990, -89.80000, -0.10000);
    CreateDynamicObject(2345, -1113.81982, -1614.68396, 76.83780,   9.90000, -0.10000, 0.00000);
    CreateDynamicObject(18648, -1113.43835, -1614.72327, 78.05550,   -0.09990, 3.30000, 89.70000);
    CreateDynamicObject(19087, -1116.03406, -1614.64673, 76.97210,   -0.29990, -89.80000, -0.10000);
    CreateDynamicObject(18648, -1115.42017, -1614.71277, 78.05200,   -0.09990, 3.30000, 89.70000);
    CreateDynamicObject(2345, -1116.69775, -1614.69702, 76.86500,   9.90000, -0.10000, 2.80000);
    CreateDynamicObject(18648, -1117.41187, -1614.70203, 78.04850,   -0.09990, 3.30000, 89.70000);
    CreateDynamicObject(19087, -1118.44641, -1614.64526, 76.98060,   -0.29990, -89.80000, -0.10000);
    CreateDynamicObject(2345, -1119.45447, -1614.83167, 76.86010,   9.90000, -0.10000, 2.80000);
    CreateDynamicObject(18648, -1119.36230, -1614.69275, 78.04510,   -0.09990, 3.30000, 89.70000);
    CreateDynamicObject(18648, -1119.89282, -1614.69226, 78.04430,   -0.09990, 3.30000, 89.70000);
    CreateDynamicObject(19087, -1120.88733, -1614.64124, 76.98900,   -0.29990, -89.80000, -0.10000);
    CreateDynamicObject(905, -1101.03674, -1549.04626, 76.80190,   0.00000, -185.00000, -62.00000);
    CreateDynamicObject(905, -981.43860, -1973.72876, 75.66650,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(19321, -1100.39075, -1612.32715, 76.79410,   0.00000, -0.26000, 89.46000);
    CreateDynamicObject(19364, -1098.60364, -1612.35388, 75.31313,   -0.60000, 90.42011, -0.30000);
    CreateDynamicObject(19364, -1102.08044, -1612.33215, 75.31310,   -0.60000, 90.42010, -0.30000);
    CreateDynamicObject(2180, -1101.12537, -1613.49939, 75.41240,   0.00000, 0.00000, 179.64023);
    CreateDynamicObject(19923, -1103.23389, -1612.12378, 75.39470,   0.00000, 0.00000, 90.11990);
    CreateDynamicObject(1329, -1102.94739, -1613.32336, 75.89361,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1575, -1101.44666, -1613.68591, 76.22979,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(2060, -1118.66736, -1622.51733, 76.23350,   0.00000, -0.29990, 91.39980);
    CreateDynamicObject(2926, -1101.80408, -1613.48889, 76.21297,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1575, -1102.00647, -1613.71277, 76.22979,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1575, -1102.03088, -1613.35266, 76.22979,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1575, -1101.47083, -1613.32581, 76.22979,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(11719, -1102.81677, -1611.99146, 76.40298,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(11719, -1103.13501, -1612.33374, 76.40298,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(3042, -1103.08545, -1610.76257, 77.91814,   0.00000, 0.00000, -1.68005);
    CreateDynamicObject(18726, -1102.74731, -1610.13989, 79.36458,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(2926, -1118.67554, -1622.44165, 76.14930,   0.00000, 0.00000, -89.99990);
    CreateDynamicObject(18648, -1103.95618, -1612.80396, 78.14970,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(18648, -1103.95764, -1611.71265, 78.14970,   0.00000, 0.00000, 0.00000);
    return Y_HOOKS_CONTINUE_RETURN_1;
}