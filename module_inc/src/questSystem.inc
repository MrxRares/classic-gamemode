#include <YSI_Coding\y_hooks>

hook WhenPlayerLoggedIn(playerid)
{
    if(!cache_num_rows())
        return Y_HOOKS_CONTINUE_RETURN_1;

    new loadString[100];

    cache_get_value_name(0, "Quest", loadString);
    sscanf(loadString, "a<i>[3]", playerInfo[playerid][pQuest]);
     
    cache_get_value_name(0, "QuestProgress", loadString);
    sscanf(loadString, "a<i>[3]", playerInfo[playerid][pQuestProgress]);
     
    cache_get_value_name(0, "QuestNeed", loadString);
    sscanf(loadString, "a<i>[3]", playerInfo[playerid][pQuestNeed]);

    cache_get_value_name_int(0, "QuestsUnlocked", playerInfo[playerid][pQuestsUnlocked]);
    return Y_HOOKS_CONTINUE_RETURN_1;
}

#define QUEST_TYPE_NONE 0
#define QUEST_TYPE_BASEBALL 1
#define QUEST_TYPE_CHILLIAD 2
#define QUEST_TYPE_VEHKM 3
#define QUEST_TYPE_ROB 4
#define QUEST_TYPE_ATMROB 5
#define QUEST_TYPE_MATS 6

#define QUEST_TYPE_FISH 7
#define QUEST_TYPE_PIZZA 8
#define QUEST_TYPE_BENCH 9
#define QUEST_TYPE_COPS 10
#define QUEST_TYPE_PAINT 11
#define QUEST_TYPE_GARBAGE 12

#define QUEST_TYPE_FARM 13
#define QUEST_TYPE_DRUGS 14
#define QUEST_TYPE_BAR 15
#define QUEST_TYPE_CLOTHES 16
#define QUEST_TYPE_DICE 17

SetQuestNeedPoints(questId)
{
    switch(questId)
    {
        case QUEST_TYPE_BASEBALL: return true;
        case QUEST_TYPE_CHILLIAD: return true;
        case QUEST_TYPE_VEHKM: return RandomEx(10, 25);
        case QUEST_TYPE_ROB: return true;
        case QUEST_TYPE_ATMROB: return RandomEx(5, 10);
        case QUEST_TYPE_MATS: return RandomEx(3000, 10000);
        case QUEST_TYPE_FISH: return RandomEx(5, 15);
        case QUEST_TYPE_PIZZA: return RandomEx(5, 15);
        case QUEST_TYPE_BENCH: return RandomEx(30, 50);
        case QUEST_TYPE_COPS: return RandomEx(2, 5);
        case QUEST_TYPE_PAINT: return RandomEx(5, 20);
        case QUEST_TYPE_GARBAGE: return RandomEx(60, 120);
        case QUEST_TYPE_FARM: return RandomEx(25, 50);
        case QUEST_TYPE_DRUGS: return true;
        case QUEST_TYPE_BAR: return true;
        case QUEST_TYPE_CLOTHES: return true;
        case QUEST_TYPE_DICE: return RandomEx(2, 10);
    }
    return true;
}
 
GetQuestName(playerid, questId, needed)
{
    new questName[144];

    if(!playerInfo[playerid][pLanguage])
    {
        switch(questId)
        {
            case QUEST_TYPE_BASEBALL: questName = "Go to the baseball stadium located in Las Venturas";
            case QUEST_TYPE_CHILLIAD: questName = "Climb mount Chiliad";
            case QUEST_TYPE_VEHKM: format(questName, sizeof questName, "Travel %d KM with a personal vehicle", needed);
            case QUEST_TYPE_ROB: questName = "Rob a business";
            case QUEST_TYPE_ATMROB: format(questName, sizeof questName, "Rob %d ATMs", needed);
            case QUEST_TYPE_MATS: format(questName, sizeof questName, "Collect %d materials", needed);
            case QUEST_TYPE_FISH: format(questName, sizeof questName, "Catch %d fishes", needed);
            case QUEST_TYPE_PIZZA: format(questName, sizeof questName, "Deliver %d pizza", needed);
            case QUEST_TYPE_BENCH: format(questName, sizeof questName, "Go to a benchpress and push your chest %d times", needed);
            case QUEST_TYPE_PAINT: format(questName, sizeof questName, "Kill %d players at paintball", needed);
            case QUEST_TYPE_GARBAGE: format(questName, sizeof questName, "Collect and unload %d kilograms of garbage", needed);
            case QUEST_TYPE_FARM: format(questName, sizeof questName, "Sell %d kilograms of flour", needed);
            case QUEST_TYPE_DRUGS: questName = "Buy some drugs and use them";
            case QUEST_TYPE_BAR: questName = "Go in a bar and get drunk";
            case QUEST_TYPE_CLOTHES: questName = "Buy some new clothes, glasses or hat";
            case QUEST_TYPE_DICE: format(questName, sizeof questName, "Win %d times at dice", needed);
            case QUEST_TYPE_COPS: format(questName, sizeof questName, "%s", ((playerInfo[playerid][pGroup] == 1 || playerInfo[playerid][pGroup] == 2 || playerInfo[playerid][pGroup] == 3 || playerInfo[playerid][pGroup] == 8) ? va_return("Arrest %d suspects", needed) : va_return("Kill %d cops", needed)));
        }
    }
    else
    {
        switch(questId)
        {
            case QUEST_TYPE_BASEBALL: questName = "Mergi pe stadionul de baseball din Las Venturas";
            case QUEST_TYPE_CHILLIAD: questName = "Urca pe muntele Chiliad";
            case QUEST_TYPE_VEHKM: format(questName, sizeof questName, "Parcurge distanta de %d KM cu un vehicul personal", needed);
            case QUEST_TYPE_ROB: questName = "Jefuieste o afacere";
            case QUEST_TYPE_ATMROB: format(questName, sizeof questName, "Jefuieste %d ATM-uri", needed);
            case QUEST_TYPE_MATS: format(questName, sizeof questName, "Colecteaza %d materiale", needed);
            case QUEST_TYPE_FISH: format(questName, sizeof questName, "Vinde %d pesti", needed);
            case QUEST_TYPE_PIZZA: format(questName, sizeof questName, "Livreaza %d pizza", needed);
            case QUEST_TYPE_BENCH: format(questName, sizeof questName, "Du-te la o banca pentru haltere si impinge la piept de %d ori", needed);
            case QUEST_TYPE_PAINT: format(questName, sizeof questName, "Omoara %d playeri la Paintball", needed);
            case QUEST_TYPE_GARBAGE: format(questName, sizeof questName, "Colecteaza si descarca %d KG de gunoi", needed);
            case QUEST_TYPE_FARM: format(questName, sizeof questName, "Vinde %d KG de faina", needed);
            case QUEST_TYPE_DRUGS: questName = "Cumpara droguri si foloseste-le";
            case QUEST_TYPE_BAR: questName = "Du-te intr-un bar si imbata-te";
            case QUEST_TYPE_CLOTHES: questName = "Schimba-ti skinul la un clothing store";
            case QUEST_TYPE_DICE: format(questName, sizeof questName, "Castiga %d maini la barbut", needed);
            case QUEST_TYPE_COPS: format(questName, sizeof questName, "%s", ((playerInfo[playerid][pGroup] == 1 || playerInfo[playerid][pGroup] == 2 || playerInfo[playerid][pGroup] == 3 || playerInfo[playerid][pGroup] == 8) ? va_return("Aresteaza %d suspecti", needed) : va_return("Omoara %d politisti", needed)));
        }
    }
    return questName;
}
 
GiveAllQuests()
{
    foreach(new playerid : Player)
    {
        playerInfo[playerid][pQuest][0] = RandomEx(1, 6);
        playerInfo[playerid][pQuest][1] = RandomEx(7, 12);
        playerInfo[playerid][pQuest][2] = RandomEx(13, 17);
     
        if((playerInfo[playerid][pGroup] == 1 || playerInfo[playerid][pGroup] == 2 || playerInfo[playerid][pGroup] == 3 || playerInfo[playerid][pGroup] == 8))
        {
            if(playerInfo[playerid][pQuest][0] == QUEST_TYPE_ROB || playerInfo[playerid][pQuest][0] == QUEST_TYPE_ATMROB)
            {
                playerInfo[playerid][pQuest][0] = RandomEx(1, 2);
            }
        }
     
        playerInfo[playerid][pQuestProgress][0] = 0;
        playerInfo[playerid][pQuestProgress][1] = 0;
        playerInfo[playerid][pQuestProgress][2] = 0;
     
        playerInfo[playerid][pQuestNeed][0] = SetQuestNeedPoints(playerInfo[playerid][pQuest][0]);
        playerInfo[playerid][pQuestNeed][1] = SetQuestNeedPoints(playerInfo[playerid][pQuest][1]);
        playerInfo[playerid][pQuestNeed][2] = SetQuestNeedPoints(playerInfo[playerid][pQuest][2]);
    } 
    return true;
}
GivePlayerQuests(playerid)
{
    if(playerInfo[playerid][pQuestsUnlocked] == 0)
        return true;

    playerInfo[playerid][pQuest][0] = RandomEx(1, 6);
    playerInfo[playerid][pQuest][1] = RandomEx(7, 12);
    playerInfo[playerid][pQuest][2] = RandomEx(13, 17);
 
    if((playerInfo[playerid][pGroup] == 1 || playerInfo[playerid][pGroup] == 2 || playerInfo[playerid][pGroup] == 3 || playerInfo[playerid][pGroup] == 8))
    {
        if(playerInfo[playerid][pQuest][0] == QUEST_TYPE_ROB || playerInfo[playerid][pQuest][0] == QUEST_TYPE_ATMROB)
        {
            playerInfo[playerid][pQuest][0] = RandomEx(1, 2);
        }
    }
 
    playerInfo[playerid][pQuestProgress][0] = 0;
    playerInfo[playerid][pQuestProgress][1] = 0;
    playerInfo[playerid][pQuestProgress][2] = 0;
 
    playerInfo[playerid][pQuestNeed][0] = SetQuestNeedPoints(playerInfo[playerid][pQuest][0]);
    playerInfo[playerid][pQuestNeed][1] = SetQuestNeedPoints(playerInfo[playerid][pQuest][1]);
    playerInfo[playerid][pQuestNeed][2] = SetQuestNeedPoints(playerInfo[playerid][pQuest][2]);
    return true;
}

new questArea[2];
hook OnGameModeInit()
{
    questArea[0] = CreateDynamicRectangle(1299.5, 2101.5, 1399.5, 2201.5, 0, 0);
    questArea[1] = CreateDynamicRectangle(-2367.5, -1677.5, -2267.5, -1577.5, 0, 0);
    return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnPlayerEnterDynArea(playerid, areaid)
{
    if(areaid == questArea[0])
        DailyQuestCheck(playerid, QUEST_TYPE_BASEBALL, 1);

    else if(areaid == questArea[1])
        DailyQuestCheck(playerid, QUEST_TYPE_CHILLIAD, 1);

    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Quest` = '%d %d %d', `QuestNeed` = '%d %d %d' where `id` = '%d';", playerInfo[playerid][pQuest][0], playerInfo[playerid][pQuest][1], playerInfo[playerid][pQuest][2], playerInfo[playerid][pQuestNeed][0], playerInfo[playerid][pQuestNeed][1], playerInfo[playerid][pQuestNeed][2], playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");
    return Y_HOOKS_CONTINUE_RETURN_1;
}

DailyQuestCheck(playerid, questid, progress)
{
    new quest_slot = playerInfo[playerid][pQuest][0] == questid ? (0) : playerInfo[playerid][pQuest][1] == questid ? (1) : playerInfo[playerid][pQuest][2] == questid ? (2) : (-1);
    if(quest_slot == -1 || playerInfo[playerid][pQuestProgress][quest_slot] >= playerInfo[playerid][pQuestNeed][quest_slot])
        return true;
 
    if(progress + playerInfo[playerid][pQuestProgress][quest_slot] > playerInfo[playerid][pQuestNeed][quest_slot])
        playerInfo[playerid][pQuestProgress][quest_slot] = playerInfo[playerid][pQuestNeed][quest_slot];
    else
        playerInfo[playerid][pQuestProgress][quest_slot] += progress;
 
    sendSplitMessage(playerid, COLOR_YELLOW, "Progress for quest: %s, %d/%d", GetQuestName(playerid, questid, playerInfo[playerid][pQuestNeed][quest_slot]), playerInfo[playerid][pQuestProgress][quest_slot], playerInfo[playerid][pQuestNeed][quest_slot]);
   
    if(playerInfo[playerid][pQuestProgress][quest_slot] == playerInfo[playerid][pQuestNeed][quest_slot])
    {
        new money = RandomEx(15000, 30000);
        addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, money, false);

        AddRespectPoints(playerid, 1);
        
        va_SendClientMessage(playerid, COLOR_SERVER, "QUESTS >>{ffffff} %s. You got 1 respect points + %s$ for completing this quest.", GetQuestName(playerid, questid, playerInfo[playerid][pQuestNeed][quest_slot]), FormatNumber(money));
    }

    mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Quest` = '%d %d %d', `QuestProgress` = '%d %d %d', `QuestNeed` = '%d %d %d' where `id` = '%d';", playerInfo[playerid][pQuest][0], playerInfo[playerid][pQuest][1], playerInfo[playerid][pQuest][2], playerInfo[playerid][pQuestProgress][0], playerInfo[playerid][pQuestProgress][1], playerInfo[playerid][pQuestProgress][2], playerInfo[playerid][pQuestNeed][0], playerInfo[playerid][pQuestNeed][1], playerInfo[playerid][pQuestNeed][2], playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}
 
YCMD:resetquests(playerid, params[], help)
{
    if(playerInfo[playerid][pAdmin] < 7)
        return false;
    
    SendClientMessage(playerid, -1, "Resetting all players daily quests..");
    return GiveAllQuests();
}
 
YCMD:quests(playerid, params[], help)
    return ShowDailyQuests(playerid);
 
ShowDailyQuests(playerid)
{
    if(playerInfo[playerid][pQuestsUnlocked] == 0)
        return SendClientMessage(playerid, COLOR_SERVER, "QUESTS >> {ffffff}You don't have any quest unlocked. You can unlock max 3 quests (3 on every payday with 20 minutes played).");

    for(new i; i < 3; i++)
    {   
        if(playerInfo[playerid][pQuestProgress][i] >= playerInfo[playerid][pQuestNeed][i])
            continue;

        if(!playerInfo[playerid][pLanguage])
            va_SendClientMessage(playerid, COLOR_SERVER, "QUESTS >>{ffffff} %s. | Progress: %d/%d", GetQuestName(playerid, playerInfo[playerid][pQuest][i], playerInfo[playerid][pQuestNeed][i]), playerInfo[playerid][pQuestProgress][i], playerInfo[playerid][pQuestNeed][i]);
        
        else
            va_SendClientMessage(playerid, COLOR_SERVER, "MISIUNE >>{ffffff} %s. | Progres: %d/%d", GetQuestName(playerid, playerInfo[playerid][pQuest][i], playerInfo[playerid][pQuestNeed][i]), playerInfo[playerid][pQuestProgress][i], playerInfo[playerid][pQuestNeed][i]);
    }
    return true;
}