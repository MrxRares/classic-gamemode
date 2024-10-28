// ------------------------------------- //
//             Includes                  //
// ------------------------------------- //

#include <YSI_Coding\y_hooks>
#include <vector>
#include <memory>
#pragma warning disable 234
// ------------------------------------- //
//             Constants                 //
// ------------------------------------- //

#define TRADE_ITEMS_PAGE_LIMIT 4

// ------------------------------------- //
//             Enums                     //
// ------------------------------------- //

static enum e_tradeItemsTypes {
    TYPE_MONEY, TYPE_HOUSE, TYPE_BUSINESS, TYPE_DRUGS, TYPE_COCAINE, TYPE_MUSHROOMS, TYPE_MATERIALS, TYPE_VEHICLE
}

static enum e_tradeItemsInfo {
    e_tradeItemsTypes:itemType, itemModel, itemValue
} 

// ------------------------------------- //
//             Variables                 //
// ------------------------------------- //

static  
    PlayerText:tradePlayerTDs[MAX_PLAYERS][12] = {{PlayerText:-1, ...}, ...},
    PlayerText:tradeItemsTDs[MAX_PLAYERS][16] = {{PlayerText:-1, ...}, ...},
    Vector:tradeItems[MAX_PLAYERS],
    tradeDelay[MAX_PLAYERS],
    tradeReadyDelay[MAX_PLAYERS],
    tradeWith[MAX_PLAYERS] = {-1, ...},
    tradeInviter[MAX_PLAYERS] = {-1, ...},
    tradeItemsCount[MAX_PLAYERS],
    tradeItemsIndex[MAX_PLAYERS][2],
    tradeItemsPageCount[MAX_PLAYERS][2],
    bool:tradeItemsInserted[MAX_PLAYERS][e_tradeItemsTypes - e_tradeItemsTypes:1],
    bool:tradePlayerReady[MAX_PLAYERS],
    Timer:tradeTimer[MAX_PLAYERS],
    tradePersonalVehiclesCount[MAX_PLAYERS],
    tradeItemsMoney[MAX_PLAYERS][2],
    tradeTimerSeconds[MAX_PLAYERS];

static const Float:tradeItemsPositions[][] = {
    {51.525474, 109.124595, 166.026626, 223.230117},
    {355.997314, 412.900787, 470.304290, 527.403076}
};

// ------------------------------------- //
//             Functions                 //
// ------------------------------------- //

static destroyTradeTextdraws(const playerid) {
    for(new i = 0; i < sizeof tradeItemsTDs[]; ++i) {
        PlayerTextDrawHide(playerid, tradeItemsTDs[playerid][i]);
        PlayerTextDrawHide(tradeWith[playerid], tradeItemsTDs[tradeWith[playerid]][i]);

        if(i < sizeof tradePlayerTDs[]) {
            PlayerTextDrawHide(playerid, tradePlayerTDs[playerid][i]);
            PlayerTextDrawHide(tradeWith[playerid], tradePlayerTDs[tradeWith[playerid]][i]);
        }
    }
}

static cancelTrade(const playerid) {
    VECTOR_clear(tradeItems[playerid]);
    VECTOR_clear(tradeItems[tradeWith[playerid]]);

    destroyTradeTextdraws(playerid);

    CancelSelectTextDraw(tradeWith[playerid]);
    TogglePlayerControllable(tradeWith[playerid], true);

    CancelSelectTextDraw(playerid);
    TogglePlayerControllable(playerid, true);

    tradeWith[tradeWith[playerid]] = tradeWith[playerid] = -1;
}

static clearTradeItems(const playerid) {
    VECTOR_clear(tradeItems[playerid]);

    new i;

    for(i = 0; i < tradeItemsPageCount[playerid][0]; ++i) {
        PlayerTextDrawHide(playerid, tradeItemsTDs[playerid][i]);
        PlayerTextDrawHide(playerid, tradeItemsTDs[playerid][i + 8]);
    }

    for(i = 0; i < tradeItemsPageCount[tradeWith[playerid]][1]; ++i) {
        PlayerTextDrawHide(tradeWith[playerid], tradeItemsTDs[tradeWith[playerid]][i + 4]);
        PlayerTextDrawHide(tradeWith[playerid], tradeItemsTDs[tradeWith[playerid]][i + 12]);
    }

    tradeItemsPageCount[playerid][0] = tradeItemsPageCount[tradeWith[playerid]][1] = tradeItemsCount[playerid] = 
    tradePersonalVehiclesCount[playerid] = tradePersonalVehiclesCount[tradeWith[playerid]] = 0;

    tradeItemsIndex[playerid][0] = tradeItemsIndex[tradeWith[playerid]][1] = -1;

    memset(tradeItemsInserted[playerid], false);
}

static updateTradeItem(const playerid, const returnPlayer, const item[e_tradeItemsInfo], const textdrawidx) {
    new tradeString[400];

    switch(item[itemType]) {
        case TYPE_MONEY: {
            format(tradeString, sizeof tradeString, "$%s", formatMoney(tradeItemsMoney[returnPlayer][0], tradeItemsMoney[returnPlayer][1]));
        }
        case TYPE_HOUSE: {
            format(tradeString, sizeof tradeString, "house %d", houseInfo[item[itemValue]][houseID]);
        }
        case TYPE_BUSINESS: {
            format(tradeString, sizeof tradeString, "business %d", bizInfo[item[itemValue]][bizID]);
        }
        case TYPE_DRUGS: {
            format(tradeString, sizeof tradeString, "%d marijuana", item[itemValue]);
        }
        case TYPE_COCAINE: {
            format(tradeString, sizeof tradeString, "%d cocaine", item[itemValue]);
        }
        case TYPE_MUSHROOMS: {
            format(tradeString, sizeof tradeString, "%d mushrooms", item[itemValue]);
        }
        case TYPE_MATERIALS: {
            format(tradeString, sizeof tradeString, "%d materials", item[itemValue]);
        }
        case TYPE_VEHICLE: {
            strcat(tradeString, aVehicleNames[carInfo[item[itemValue]][cModel] - 400]);
        }
    }

    PlayerTextDrawSetString(playerid, tradeItemsTDs[playerid][textdrawidx + 8], tradeString);
    PlayerTextDrawShow(playerid, tradeItemsTDs[playerid][textdrawidx + 8]);

    PlayerTextDrawSetPreviewModel(playerid, tradeItemsTDs[playerid][textdrawidx], item[itemModel]);
    PlayerTextDrawShow(playerid, tradeItemsTDs[playerid][textdrawidx]);
}

static showTradeItems(const playerid, const returnPlayer, &index_start) {
    new 
        idx = playerid != returnPlayer,
        item[e_tradeItemsInfo];

    tradeItemsPageCount[playerid][idx] = 0;

    for(;;) {
        VECTOR_get_arr(tradeItems[returnPlayer], index_start, item);

        updateTradeItem(playerid, returnPlayer, item, (idx * 4) + tradeItemsPageCount[playerid][idx]);

        if(++tradeItemsPageCount[playerid][idx] == TRADE_ITEMS_PAGE_LIMIT) 
            return 1;

        if(index_start == getTradeItemsCount(returnPlayer) - 1)
            break;

        else 
            index_start ++;
    }

    for(new i = tradeItemsPageCount[playerid][idx]; i < 4; ++i) {
        PlayerTextDrawHide(playerid, tradeItemsTDs[playerid][i + (idx * 4)]);
        PlayerTextDrawHide(playerid, tradeItemsTDs[playerid][8 + i + (idx * 4)]);
    }

    return 1;
}

static insertTradeItem(const playerid, e_tradeItemsTypes:type, model, value) { 
    new item[e_tradeItemsInfo];

    item[itemType] = type;
    item[itemModel] = model;
    item[itemValue] = value;

    VECTOR_push_back_arr(tradeItems[playerid], item);

    tradeItemsCount[playerid] ++;

    if(type != TYPE_VEHICLE)
        tradeItemsInserted[playerid][type] = true;

    if(tradeItemsPageCount[playerid][0] < TRADE_ITEMS_PAGE_LIMIT) {
        updateTradeItem(playerid, playerid, item, tradeItemsPageCount[playerid][0]);

        tradeItemsPageCount[playerid][0] ++;
        tradeItemsIndex[playerid][0] ++;
    }

    if(tradeItemsPageCount[tradeWith[playerid]][1] < TRADE_ITEMS_PAGE_LIMIT) {
        updateTradeItem(tradeWith[playerid], playerid, item, 4 + tradeItemsPageCount[tradeWith[playerid]][1]);

        tradeItemsPageCount[tradeWith[playerid]][1] ++;
        tradeItemsIndex[tradeWith[playerid]][1] ++;
    }
}
setPersonalVehicleOwner(const vehicleDBId, const playerid) {
    if(GetVehicleGameID(vehicleDBId))
        DestroyPersonalVehicle(GetVehicleGameID(vehicleDBId), vehicleDBId);

    format(carInfo[vehicleDBId][cOwner], MAX_PLAYER_NAME, playerInfo[playerid][pName]);
   
    mysql_format(SQL, returnString, sizeof returnString, "update `personalvehicles` set `Owner` = '%s' where `ID` = '%d';", playerInfo[playerid][pName], vehicleDBId);
    mysql_tquery(SQL, returnString, "", "");

    return 1;
}

static exchangeTradeItem(const playerid, const e_tradeItemsTypes:type, const value) {
    switch(type) {
        case TYPE_MONEY: {
            if(tradeItemsMoney[playerid][0] > playerInfo[playerid][pAccountLY] || (tradeItemsMoney[playerid][0] >= playerInfo[playerid][pAccountLY] && tradeItemsMoney[playerid][1] > playerInfo[playerid][pAccount])) {
                sendError(playerid, "You don't have enough money in bank, the money didn't transfer.");
                sendError(tradeWith[playerid], "The player you are trading with does not have enough money to continue the trade, the money didn't transfer.");
                return true;
            }

            if(tradeItemsMoney[playerid][0] > playerInfo[tradeWith[playerid]][pAccountLY] || (tradeItemsMoney[playerid][0] >= playerInfo[tradeWith[playerid]][pAccountLY] && tradeItemsMoney[playerid][1] > playerInfo[tradeWith[playerid]][pAccount])){
                sendError(playerid, "You don't have enough money in bank, the money didn't transfer.");
                sendError(tradeWith[playerid], "The player you are trading with does not have enough money to continue the trade, the money didn't transfer.");
                return true;
            }


            substractBankMoney(playerid, playerInfo[playerid][pAccountLY], playerInfo[playerid][pAccount], tradeItemsMoney[playerid][0], tradeItemsMoney[playerid][1]);
            addBankMoney(tradeWith[playerid], playerInfo[tradeWith[playerid]][pAccountLY], playerInfo[tradeWith[playerid]][pAccount], tradeItemsMoney[playerid][0], tradeItemsMoney[playerid][1]);

            
            format(returnString, sizeof returnString, "- $%s", formatMoney(tradeItemsMoney[playerid][0], tradeItemsMoney[playerid][1]));

            va_SendClientMessage(playerid, -1, returnString);
            va_SendClientMessage(tradeWith[playerid], -1, returnString);
        }
        case TYPE_HOUSE: {
            playerInfo[tradeWith[playerid]][pHouse] = value;
            houseInfo[value][houseOwner] = playerInfo[tradeWith[playerid]][pSQLID];

            format(houseInfo[value][houseOwnerName], MAX_PLAYER_NAME, "%s", GetNameBySQL(playerInfo[tradeWith[playerid]][pSQLID]));


            playerInfo[playerid][pHouse] = 0;

            mysql_format(SQL, returnString, sizeof returnString, "update `users` set `House` = '0' where `id` = '%d';", playerInfo[playerid][pSQLID]);
            mysql_tquery(SQL, returnString, "", "");

            mysql_format(SQL, returnString, sizeof returnString, "update `houses` set `OwnerSQL` = '%d' where `id` = '%d';", playerInfo[tradeWith[playerid]][pSQLID], value);
            mysql_tquery(SQL, returnString, "", "");
        
            mysql_format(SQL, returnString, sizeof returnString, "update `users` set `House` = '%d' where `id` = '%d';", value, playerInfo[tradeWith[playerid]][pSQLID]);
            mysql_tquery(SQL, returnString, "", "");

            WhenHouseGotUpdated(value);

            
            format(returnString, sizeof returnString, "- House %d", houseInfo[value][houseID]);
            
            va_SendClientMessage(playerid, -1, returnString);
            va_SendClientMessage(tradeWith[playerid], -1, returnString);
            insertPlayerLog(playerInfo[playerid][pSQLID], playerInfo[tradeWith[playerid]][pSQLID], "[trade] %s [user:%d] a oferit casa %d lui %s", playerInfo[playerid][pName], playerInfo[playerid][pSQLID], houseInfo[value][houseID], playerInfo[tradeWith[playerid]][pName]);
            insertPlayerLog(playerInfo[tradeWith[playerid]][pSQLID], playerInfo[playerid][pSQLID], "[trade] %s [user:%d] a primit casa %d de la %s ", playerInfo[tradeWith[playerid]][pName], playerInfo[tradeWith[playerid]][pSQLID], houseInfo[value][houseID], playerInfo[playerid][pName]);
        }
        case TYPE_BUSINESS: {

            bizInfo[value][bizOwner] = playerInfo[tradeWith[playerid]][pSQLID];
            playerInfo[tradeWith[playerid]][pBusiness] = value;
            playerInfo[playerid][pBusiness] = 0;
            format(bizInfo[value][bizOwnerName], MAX_PLAYER_NAME, "%s", GetName(tradeWith[playerid]));

            mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Business` = '0' where `id` = '%d';", playerInfo[playerid][pSQLID]);
            mysql_tquery(SQL, returnString, "", "");

            mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `OwnerSQL` = '%d' where `id` = '%d';", playerInfo[tradeWith[playerid]][pSQLID], value);
            mysql_tquery(SQL, returnString, "", "");

            mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Business` = '%d' where `id` = '%d';", value, playerInfo[tradeWith[playerid]][pSQLID]);
            mysql_tquery(SQL, returnString, "", "");

            WhenBusinessGotUpdated(value);

            
            format(returnString, sizeof returnString, "- Business %d", bizInfo[value][bizID]);
            
            va_SendClientMessage(playerid, -1, returnString);
            va_SendClientMessage(tradeWith[playerid], -1, returnString);
            insertPlayerLog(playerInfo[playerid][pSQLID], playerInfo[tradeWith[playerid]][pSQLID], "[trade] %s [user:%d] a oferit afacerea %d lui %s ", playerInfo[playerid][pName], playerInfo[playerid][pSQLID], bizInfo[value][bizID], playerInfo[tradeWith[playerid]][pName]);
            insertPlayerLog(playerInfo[tradeWith[playerid]][pSQLID], playerInfo[playerid][pSQLID], "[trade] %s [user:%d] a primit afacerea %d de la %s ", playerInfo[tradeWith[playerid]][pName], playerInfo[tradeWith[playerid]][pSQLID], bizInfo[value][bizID], playerInfo[playerid][pName]);
        }
        case TYPE_DRUGS: {
            playerInfo[playerid][pDrugs] -= value;
            playerInfo[tradeWith[playerid]][pDrugs] += value;

            va_mysql_tquery("UPDATE `users` SET `Drugs` = CASE WHEN `ID` = %d THEN %d WHEN `ID` = %d THEN %d END WHERE `ID` IN (%d, %d)", playerInfo[playerid][pSQLID], playerInfo[playerid][pDrugs], playerInfo[tradeWith[playerid]][pSQLID], playerInfo[tradeWith[playerid]][pDrugs], playerInfo[playerid][pSQLID], playerInfo[tradeWith[playerid]][pSQLID]);

            
            format(returnString, sizeof returnString, "- %d marijuana", value);
            
            va_SendClientMessage(playerid, -1, returnString);
            va_SendClientMessage(tradeWith[playerid], -1, returnString);

            insertPlayerLog(playerInfo[playerid][pSQLID], playerInfo[tradeWith[playerid]][pSQLID], "[trade] %s [user:%d] a oferit %d marijuana lui %s ", playerInfo[playerid][pName], playerInfo[playerid][pSQLID], value, playerInfo[tradeWith[playerid]][pName]);
            insertPlayerLog(playerInfo[tradeWith[playerid]][pSQLID], playerInfo[playerid][pSQLID], "[trade] %s [user:%d] a primit %d marijuana de la %s ", playerInfo[tradeWith[playerid]][pName], playerInfo[tradeWith[playerid]][pSQLID], value, playerInfo[playerid][pName]);
        }
        case TYPE_COCAINE: {
            playerInfo[playerid][pCocaine] -= value;
            playerInfo[tradeWith[playerid]][pCocaine] += value;

            va_mysql_tquery("UPDATE `users` SET `Cocaine` = CASE WHEN `ID` = %d THEN %d WHEN `ID` = %d THEN %d END WHERE `ID` IN (%d, %d)", playerInfo[playerid][pSQLID], playerInfo[playerid][pCocaine], playerInfo[tradeWith[playerid]][pSQLID], playerInfo[tradeWith[playerid]][pCocaine], playerInfo[playerid][pSQLID], playerInfo[tradeWith[playerid]][pSQLID]);

            
            format(returnString, sizeof returnString, "- %d cocaine", value);
            
            va_SendClientMessage(playerid, -1, returnString);
            va_SendClientMessage(tradeWith[playerid], -1, returnString);

            insertPlayerLog(playerInfo[playerid][pSQLID], playerInfo[tradeWith[playerid]][pSQLID], "[trade] %s [user:%d] a oferit %d cocaine lui %s ", playerInfo[playerid][pName], playerInfo[playerid][pSQLID], value, playerInfo[tradeWith[playerid]][pName]);
            insertPlayerLog(playerInfo[tradeWith[playerid]][pSQLID], playerInfo[playerid][pSQLID], "[trade] %s [user:%d] a primit %d cocaine de la %s ", playerInfo[tradeWith[playerid]][pName], playerInfo[tradeWith[playerid]][pSQLID], value, playerInfo[playerid][pName]);
        }
        case TYPE_MUSHROOMS: {
            playerInfo[playerid][pMushrooms] -= value;
            playerInfo[tradeWith[playerid]][pMushrooms] += value;

            va_mysql_tquery("UPDATE `users` SET `Mushrooms` = CASE WHEN `ID` = %d THEN %d WHEN `ID` = %d THEN %d END WHERE `ID` IN (%d, %d)", playerInfo[playerid][pSQLID], playerInfo[playerid][pMushrooms], playerInfo[tradeWith[playerid]][pSQLID], playerInfo[tradeWith[playerid]][pMushrooms], playerInfo[playerid][pSQLID], playerInfo[tradeWith[playerid]][pSQLID]);

            
            format(returnString, sizeof returnString, "- %d mushrooms", value);
            
            va_SendClientMessage(playerid, -1, returnString);
            va_SendClientMessage(tradeWith[playerid], -1, returnString);

            insertPlayerLog(playerInfo[playerid][pSQLID], playerInfo[tradeWith[playerid]][pSQLID], "[trade] %s [user:%d] a oferit %d mushrooms lui %s ", playerInfo[playerid][pName], playerInfo[playerid][pSQLID], value, playerInfo[tradeWith[playerid]][pName]);
            insertPlayerLog(playerInfo[tradeWith[playerid]][pSQLID], playerInfo[playerid][pSQLID], "[trade] %s [user:%d] a primit %d mushrooms de la %s ", playerInfo[tradeWith[playerid]][pName], playerInfo[tradeWith[playerid]][pSQLID], value, playerInfo[playerid][pName]);
        }
        case TYPE_MATERIALS: {
            playerInfo[playerid][pMaterials] -= value;
            playerInfo[tradeWith[playerid]][pMaterials] += value;

            va_mysql_tquery("UPDATE `users` SET `Materials` = CASE WHEN `ID` = %d THEN %d WHEN `ID` = %d THEN %d END WHERE `ID` IN (%d, %d)", playerInfo[playerid][pSQLID], playerInfo[playerid][pMaterials], playerInfo[tradeWith[playerid]][pSQLID], playerInfo[tradeWith[playerid]][pMaterials], playerInfo[playerid][pSQLID], playerInfo[tradeWith[playerid]][pSQLID]);

            
            format(returnString, sizeof returnString, "- %d materials", value);
            
            va_SendClientMessage(playerid, -1, returnString);
            va_SendClientMessage(tradeWith[playerid], -1, returnString);
            insertPlayerLog(playerInfo[playerid][pSQLID], playerInfo[tradeWith[playerid]][pSQLID], "[trade] %s [user:%d] a oferit %d materiale lui %s ", playerInfo[playerid][pName], playerInfo[playerid][pSQLID], value, playerInfo[tradeWith[playerid]][pName]);
            insertPlayerLog(playerInfo[tradeWith[playerid]][pSQLID], playerInfo[playerid][pSQLID], "[trade] %s [user:%d] a primit %d materiale de la %s ", playerInfo[tradeWith[playerid]][pName], playerInfo[tradeWith[playerid]][pSQLID], value, playerInfo[playerid][pName]);
        }
        case TYPE_VEHICLE: {

            setPersonalVehicleOwner(value, tradeWith[playerid]);
            new vehicleDBId = VehicleSQL[value];
            
            format(returnString, sizeof returnString, "- %s (ID %d, %.0fkm, %d days, %d/%d colors)", aVehicleNames[carInfo[value][cModel] - 400], vehicleDBId, carInfo[value][cKM], getDaysFromTimestamp(carInfo[value][cDays]), carInfo[value][cColor][0], carInfo[value][cColor][1]);
            
            va_SendClientMessage(playerid, -1, returnString);
            va_SendClientMessage(tradeWith[playerid], -1, returnString);
            insertPlayerLog(playerInfo[playerid][pSQLID], playerInfo[tradeWith[playerid]][pSQLID], "[trade] %s [user:%d] a oferit %s (ID %d, %.0fkm, %d days, %d/%d colors) lui %s ", playerInfo[playerid][pName], playerInfo[playerid][pSQLID], aVehicleNames[carInfo[value][cModel] - 400], vehicleDBId, carInfo[value][cKM], getDaysFromTimestamp(carInfo[value][cDays]), carInfo[value][cColor][0], carInfo[value][cColor][1], playerInfo[tradeWith[playerid]][pName]);
            insertPlayerLog(playerInfo[tradeWith[playerid]][pSQLID], playerInfo[playerid][pSQLID], "[trade] %s [user:%d] a primit %s (ID %d, %.0fkm, %d days, %d/%d colors) de la %s ", playerInfo[tradeWith[playerid]][pName], playerInfo[tradeWith[playerid]][pSQLID], aVehicleNames[carInfo[value][cModel] - 400], vehicleDBId, carInfo[value][cKM], getDaysFromTimestamp(carInfo[value][cDays]), carInfo[value][cColor][0], carInfo[value][cColor][1], playerInfo[playerid][pName]);


        }
    }
    return 1;
}
function:showTradePersonalVehicles(playerid)
{
    if(!cache_num_rows())
        return SendClientMessage(playerid, COLOR_GREY, "You don't have any personal vehicles.");
    
    new i;

    returnString = "Vehicle type\tKM\tDays\tVIP\n";

    while(i < cache_num_rows())
    {
        new vehicleDBModel, vehicleDBId;

        cache_get_value_name_int(i, "Model", vehicleDBModel);
        cache_get_value_name_int(i, "ID", vehicleDBId);

        format(returnString, sizeof returnString, "%s%s\t%.0f\t%d\t%s\n", returnString, aVehicleNames[vehicleDBModel - 400], carInfo[vehicleDBId][cKM], getDaysFromTimestamp(carInfo[vehicleDBId][cDays]), (carInfo[vehicleDBId][cVIP] ? ("YES") : ("NO")));

        playerStaticVar[playerid][pDialogItems][i] = vehicleDBId;
        ++i;
    }
    Dialog_Show(playerid, TRADEADDPERSONALVEHICLES, DIALOG_STYLE_TABLIST_HEADERS, "Vehicule Personale", returnString, "Ofera", "Anuleaza");
    return true;
}
static haveTradePersonalVehicle(const playerid, const value) {
    new item[e_tradeItemsInfo];

    item[itemType] = TYPE_VEHICLE;
    item[itemValue] = value;
    item[itemModel] = carInfo[value][cModel];

    return VECTOR_find_arr(tradeItems[playerid], item) != INVALID_VECTOR_INDEX;
}

startPlayersTrade(const playerid, const returnPlayer) {
    tradeWith[playerid] = returnPlayer;
    tradeWith[returnPlayer] = playerid;

    tradeItemsPageCount[playerid][0] = tradeItemsPageCount[playerid][1] = 
    tradeItemsPageCount[returnPlayer][0] = tradeItemsPageCount[returnPlayer][1] = 
    tradeItemsCount[playerid] = tradeItemsCount[returnPlayer] = 
    tradePersonalVehiclesCount[playerid] = tradePersonalVehiclesCount[returnPlayer] = 0;

    tradeItemsIndex[playerid][0] = tradeItemsIndex[playerid][1] = 
    tradeItemsIndex[returnPlayer][0] = tradeItemsIndex[returnPlayer][1] = 
    tradeInviter[playerid] = tradeInviter[returnPlayer] = -1;

    tradePlayerReady[playerid] = tradePlayerReady[returnPlayer] = false;

    memset(tradeItemsInserted[playerid], false);
    memset(tradeItemsInserted[returnPlayer], false);

    TogglePlayerControllable(playerid, false);
    TogglePlayerControllable(returnPlayer, false);

    SelectTextDraw(playerid, -1);
    SelectTextDraw(returnPlayer, -1);

    // Textdraws

    tradePlayerTDs[playerid][0] = CreatePlayerTextDraw(playerid, 51.761856, 149.166763, "box");
    PlayerTextDrawLetterSize(playerid, tradePlayerTDs[playerid][0], 0.000000, 5.689605);
    PlayerTextDrawTextSize(playerid, tradePlayerTDs[playerid][0], 279.399414, 0.000000);
    PlayerTextDrawAlignment(playerid, tradePlayerTDs[playerid][0], 1);
    PlayerTextDrawColour(playerid, tradePlayerTDs[playerid][0], -1);
    PlayerTextDrawUseBox(playerid, tradePlayerTDs[playerid][0], 1);
    PlayerTextDrawBoxColour(playerid, tradePlayerTDs[playerid][0], 0x00000065);
    PlayerTextDrawSetShadow(playerid, tradePlayerTDs[playerid][0], 0);
    PlayerTextDrawSetOutline(playerid, tradePlayerTDs[playerid][0], 0);
    PlayerTextDrawBackgroundColour(playerid, tradePlayerTDs[playerid][0], 255);
    PlayerTextDrawFont(playerid, tradePlayerTDs[playerid][0], 1);
    PlayerTextDrawSetProportional(playerid, tradePlayerTDs[playerid][0], 1);
    PlayerTextDrawShow(playerid, tradePlayerTDs[playerid][0]);

    tradePlayerTDs[returnPlayer][0] = CreatePlayerTextDraw(returnPlayer, 51.761856, 149.166763, "box");
    PlayerTextDrawLetterSize(returnPlayer, tradePlayerTDs[returnPlayer][0], 0.000000, 5.689605);
    PlayerTextDrawTextSize(returnPlayer, tradePlayerTDs[returnPlayer][0], 279.399414, 0.000000);
    PlayerTextDrawAlignment(returnPlayer, tradePlayerTDs[returnPlayer][0], 1);
    PlayerTextDrawColour(returnPlayer, tradePlayerTDs[returnPlayer][0], -1);
    PlayerTextDrawUseBox(returnPlayer, tradePlayerTDs[returnPlayer][0], 1);
    PlayerTextDrawBoxColour(returnPlayer, tradePlayerTDs[returnPlayer][0], 0x00000065);
    PlayerTextDrawSetShadow(returnPlayer, tradePlayerTDs[returnPlayer][0], 0);
    PlayerTextDrawSetOutline(returnPlayer, tradePlayerTDs[returnPlayer][0], 0);
    PlayerTextDrawBackgroundColour(returnPlayer, tradePlayerTDs[returnPlayer][0], 255);
    PlayerTextDrawFont(returnPlayer, tradePlayerTDs[returnPlayer][0], 1);
    PlayerTextDrawSetProportional(returnPlayer, tradePlayerTDs[returnPlayer][0], 1);
    PlayerTextDrawShow(returnPlayer, tradePlayerTDs[returnPlayer][0]);

    tradePlayerTDs[playerid][1] = CreatePlayerTextDraw(playerid, 356.103424, 149.166763, "box");
    PlayerTextDrawLetterSize(playerid, tradePlayerTDs[playerid][1], 0.000000, 5.689605);
    PlayerTextDrawTextSize(playerid, tradePlayerTDs[playerid][1], 583.725891, 0.000000);
    PlayerTextDrawAlignment(playerid, tradePlayerTDs[playerid][1], 1);
    PlayerTextDrawColour(playerid, tradePlayerTDs[playerid][1], -1);
    PlayerTextDrawUseBox(playerid, tradePlayerTDs[playerid][1], 1);
    PlayerTextDrawBoxColour(playerid, tradePlayerTDs[playerid][1], 0x00000065);
    PlayerTextDrawSetShadow(playerid, tradePlayerTDs[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, tradePlayerTDs[playerid][1], 0);
    PlayerTextDrawBackgroundColour(playerid, tradePlayerTDs[playerid][1], 255);
    PlayerTextDrawFont(playerid, tradePlayerTDs[playerid][1], 1);
    PlayerTextDrawSetProportional(playerid, tradePlayerTDs[playerid][1], 1);
    PlayerTextDrawShow(playerid, tradePlayerTDs[playerid][1]);

    tradePlayerTDs[returnPlayer][1] = CreatePlayerTextDraw(returnPlayer, 356.103424, 149.166763, "box");
    PlayerTextDrawLetterSize(returnPlayer, tradePlayerTDs[returnPlayer][1], 0.000000, 5.689605);
    PlayerTextDrawTextSize(returnPlayer, tradePlayerTDs[returnPlayer][1], 583.725891, 0.000000);
    PlayerTextDrawAlignment(returnPlayer, tradePlayerTDs[returnPlayer][1], 1);
    PlayerTextDrawColour(returnPlayer, tradePlayerTDs[returnPlayer][1], -1);
    PlayerTextDrawUseBox(returnPlayer, tradePlayerTDs[returnPlayer][1], 1);
    PlayerTextDrawBoxColour(returnPlayer, tradePlayerTDs[returnPlayer][1], 0x00000065);
    PlayerTextDrawSetShadow(returnPlayer, tradePlayerTDs[returnPlayer][1], 0);
    PlayerTextDrawSetOutline(returnPlayer, tradePlayerTDs[returnPlayer][1], 0);
    PlayerTextDrawBackgroundColour(returnPlayer, tradePlayerTDs[returnPlayer][1], 255);
    PlayerTextDrawFont(returnPlayer, tradePlayerTDs[returnPlayer][1], 1);
    PlayerTextDrawSetProportional(returnPlayer, tradePlayerTDs[returnPlayer][1], 1);
    PlayerTextDrawShow(returnPlayer, tradePlayerTDs[returnPlayer][1]);

    tradePlayerTDs[playerid][2] = CreatePlayerTextDraw(playerid, 165.815734, 134.016708, "your items (page: 1)");
    PlayerTextDrawLetterSize(playerid, tradePlayerTDs[playerid][2], 0.293644, 1.121665);
    PlayerTextDrawTextSize(playerid, tradePlayerTDs[playerid][2], 0.000000, 227.737945);
    PlayerTextDrawAlignment(playerid, tradePlayerTDs[playerid][2], 2);
    PlayerTextDrawColour(playerid, tradePlayerTDs[playerid][2], -1);
    PlayerTextDrawUseBox(playerid, tradePlayerTDs[playerid][2], 1);
    PlayerTextDrawBoxColour(playerid, tradePlayerTDs[playerid][2], COLOR_SERVER);
    PlayerTextDrawSetShadow(playerid, tradePlayerTDs[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, tradePlayerTDs[playerid][2], 1);
    PlayerTextDrawBackgroundColour(playerid, tradePlayerTDs[playerid][2], 255);
    PlayerTextDrawFont(playerid, tradePlayerTDs[playerid][2], 3);
    PlayerTextDrawSetProportional(playerid, tradePlayerTDs[playerid][2], 1);
    PlayerTextDrawShow(playerid, tradePlayerTDs[playerid][2]);

    tradePlayerTDs[returnPlayer][2] = CreatePlayerTextDraw(returnPlayer, 165.815734, 134.016708, "your items (page: 1)");
    PlayerTextDrawLetterSize(returnPlayer, tradePlayerTDs[returnPlayer][2], 0.293644, 1.121665);
    PlayerTextDrawTextSize(returnPlayer, tradePlayerTDs[returnPlayer][2], 0.000000, 227.737945);
    PlayerTextDrawAlignment(returnPlayer, tradePlayerTDs[returnPlayer][2], 2);
    PlayerTextDrawColour(returnPlayer, tradePlayerTDs[returnPlayer][2], -1);
    PlayerTextDrawUseBox(returnPlayer, tradePlayerTDs[returnPlayer][2], 1);
    PlayerTextDrawBoxColour(returnPlayer, tradePlayerTDs[returnPlayer][2], COLOR_SERVER);
    PlayerTextDrawSetShadow(returnPlayer, tradePlayerTDs[returnPlayer][2], 0);
    PlayerTextDrawSetOutline(returnPlayer, tradePlayerTDs[returnPlayer][2], 1);
    PlayerTextDrawBackgroundColour(returnPlayer, tradePlayerTDs[returnPlayer][2], 255);
    PlayerTextDrawFont(returnPlayer, tradePlayerTDs[returnPlayer][2], 3);
    PlayerTextDrawSetProportional(returnPlayer, tradePlayerTDs[returnPlayer][2], 1);
    PlayerTextDrawShow(returnPlayer, tradePlayerTDs[returnPlayer][2]);

    
    format(returnString, sizeof returnString, "%s's items (page: 1)", GetName(playerid));

    tradePlayerTDs[returnPlayer][3] = CreatePlayerTextDraw(returnPlayer, 469.906097, 134.016708, returnString);
    PlayerTextDrawLetterSize(returnPlayer, tradePlayerTDs[returnPlayer][3], 0.293644, 1.121665);
    PlayerTextDrawTextSize(returnPlayer, tradePlayerTDs[returnPlayer][3], 0.000000, 227.628005);
    PlayerTextDrawAlignment(returnPlayer, tradePlayerTDs[returnPlayer][3], 2);
    PlayerTextDrawColour(returnPlayer, tradePlayerTDs[returnPlayer][3], -1);
    PlayerTextDrawUseBox(returnPlayer, tradePlayerTDs[returnPlayer][3], 1);
    PlayerTextDrawBoxColour(returnPlayer, tradePlayerTDs[returnPlayer][3], COLOR_SERVER);
    PlayerTextDrawSetShadow(returnPlayer, tradePlayerTDs[returnPlayer][3], 0);
    PlayerTextDrawSetOutline(returnPlayer, tradePlayerTDs[returnPlayer][3], 1);
    PlayerTextDrawBackgroundColour(returnPlayer, tradePlayerTDs[returnPlayer][3], 255);
    PlayerTextDrawFont(returnPlayer, tradePlayerTDs[returnPlayer][3], 3);
    PlayerTextDrawSetProportional(returnPlayer, tradePlayerTDs[returnPlayer][3], 1);
    PlayerTextDrawShow(returnPlayer, tradePlayerTDs[returnPlayer][3]);

    
    format(returnString, sizeof returnString, "%s's items (page: 1)", GetName(returnPlayer));

    tradePlayerTDs[playerid][3] = CreatePlayerTextDraw(playerid, 469.906097, 134.016708, returnString);
    PlayerTextDrawLetterSize(playerid, tradePlayerTDs[playerid][3], 0.293644, 1.121665);
    PlayerTextDrawTextSize(playerid, tradePlayerTDs[playerid][3], 0.000000, 227.628005);
    PlayerTextDrawAlignment(playerid, tradePlayerTDs[playerid][3], 2);
    PlayerTextDrawColour(playerid, tradePlayerTDs[playerid][3], -1);
    PlayerTextDrawUseBox(playerid, tradePlayerTDs[playerid][3], 1);
    PlayerTextDrawBoxColour(playerid, tradePlayerTDs[playerid][3], COLOR_SERVER);
    PlayerTextDrawSetShadow(playerid, tradePlayerTDs[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, tradePlayerTDs[playerid][3], 1);
    PlayerTextDrawBackgroundColour(playerid, tradePlayerTDs[playerid][3], 255);
    PlayerTextDrawFont(playerid, tradePlayerTDs[playerid][3], 3);
    PlayerTextDrawSetProportional(playerid, tradePlayerTDs[playerid][3], 1);
    PlayerTextDrawShow(playerid, tradePlayerTDs[playerid][3]);

    tradePlayerTDs[playerid][4] = CreatePlayerTextDraw(playerid, 272.233093, 200.101257, ">");
    PlayerTextDrawLetterSize(playerid, tradePlayerTDs[playerid][4], 0.398124, 1.815832);
    PlayerTextDrawTextSize(playerid, tradePlayerTDs[playerid][4], 281.000000, 10.000000);
    PlayerTextDrawAlignment(playerid, tradePlayerTDs[playerid][4], 1);
    PlayerTextDrawColour(playerid, tradePlayerTDs[playerid][4], -1);
    PlayerTextDrawSetShadow(playerid, tradePlayerTDs[playerid][4], 0);
    PlayerTextDrawSetOutline(playerid, tradePlayerTDs[playerid][4], 1);
    PlayerTextDrawBackgroundColour(playerid, tradePlayerTDs[playerid][4], 255);
    PlayerTextDrawFont(playerid, tradePlayerTDs[playerid][4], 1);
    PlayerTextDrawSetProportional(playerid, tradePlayerTDs[playerid][4], 1);
    PlayerTextDrawSetSelectable(playerid, tradePlayerTDs[playerid][4], true);
    PlayerTextDrawShow(playerid, tradePlayerTDs[playerid][4]);

    tradePlayerTDs[playerid][5] = CreatePlayerTextDraw(playerid, 49.813209, 200.101257, "<");
    PlayerTextDrawLetterSize(playerid, tradePlayerTDs[playerid][5], 0.398124, 1.815832);
    PlayerTextDrawTextSize(playerid, tradePlayerTDs[playerid][5], 60.000000, 10.000000);
    PlayerTextDrawAlignment(playerid, tradePlayerTDs[playerid][5], 1);
    PlayerTextDrawColour(playerid, tradePlayerTDs[playerid][5], -1);
    PlayerTextDrawSetShadow(playerid, tradePlayerTDs[playerid][5], 0);
    PlayerTextDrawSetOutline(playerid, tradePlayerTDs[playerid][5], 1);
    PlayerTextDrawBackgroundColour(playerid, tradePlayerTDs[playerid][5], 255);
    PlayerTextDrawFont(playerid, tradePlayerTDs[playerid][5], 1);
    PlayerTextDrawSetProportional(playerid, tradePlayerTDs[playerid][5], 1);
    PlayerTextDrawSetSelectable(playerid, tradePlayerTDs[playerid][5], true);
    PlayerTextDrawShow(playerid, tradePlayerTDs[playerid][5]);

    tradePlayerTDs[playerid][6] = CreatePlayerTextDraw(playerid, 354.238098, 200.101257, "<");
    PlayerTextDrawLetterSize(playerid, tradePlayerTDs[playerid][6], 0.398124, 1.815832);
    PlayerTextDrawTextSize(playerid, tradePlayerTDs[playerid][6], 364.000000, 10.000000);
    PlayerTextDrawAlignment(playerid, tradePlayerTDs[playerid][6], 1);
    PlayerTextDrawColour(playerid, tradePlayerTDs[playerid][6], -1);
    PlayerTextDrawSetShadow(playerid, tradePlayerTDs[playerid][6], 0);
    PlayerTextDrawSetOutline(playerid, tradePlayerTDs[playerid][6], 1);
    PlayerTextDrawBackgroundColour(playerid, tradePlayerTDs[playerid][6], 255);
    PlayerTextDrawFont(playerid, tradePlayerTDs[playerid][6], 1);
    PlayerTextDrawSetProportional(playerid, tradePlayerTDs[playerid][6], 1);
    PlayerTextDrawSetSelectable(playerid, tradePlayerTDs[playerid][6], true);
    PlayerTextDrawShow(playerid, tradePlayerTDs[playerid][6]);

    tradePlayerTDs[playerid][7] = CreatePlayerTextDraw(playerid, 576.400634, 200.101257, ">");
    PlayerTextDrawLetterSize(playerid, tradePlayerTDs[playerid][7], 0.398124, 1.815832);
    PlayerTextDrawTextSize(playerid, tradePlayerTDs[playerid][7], 586.000000, 10.000000);
    PlayerTextDrawAlignment(playerid, tradePlayerTDs[playerid][7], 1);
    PlayerTextDrawColour(playerid, tradePlayerTDs[playerid][7], -1);
    PlayerTextDrawSetShadow(playerid, tradePlayerTDs[playerid][7], 0);
    PlayerTextDrawSetOutline(playerid, tradePlayerTDs[playerid][7], 1);
    PlayerTextDrawBackgroundColour(playerid, tradePlayerTDs[playerid][7], 255);
    PlayerTextDrawFont(playerid, tradePlayerTDs[playerid][7], 1);
    PlayerTextDrawSetProportional(playerid, tradePlayerTDs[playerid][7], 1);
    PlayerTextDrawSetSelectable(playerid, tradePlayerTDs[playerid][7], true);
    PlayerTextDrawShow(playerid, tradePlayerTDs[playerid][7]);

    tradePlayerTDs[returnPlayer][4] = CreatePlayerTextDraw(returnPlayer, 272.233093, 200.101257, ">");
    PlayerTextDrawLetterSize(returnPlayer, tradePlayerTDs[returnPlayer][4], 0.398124, 1.815832);
    PlayerTextDrawTextSize(returnPlayer, tradePlayerTDs[returnPlayer][4], 281.000000, 10.000000);
    PlayerTextDrawAlignment(returnPlayer, tradePlayerTDs[returnPlayer][4], 1);
    PlayerTextDrawColour(returnPlayer, tradePlayerTDs[returnPlayer][4], -1);
    PlayerTextDrawSetShadow(returnPlayer, tradePlayerTDs[returnPlayer][4], 0);
    PlayerTextDrawSetOutline(returnPlayer, tradePlayerTDs[returnPlayer][4], 1);
    PlayerTextDrawBackgroundColour(returnPlayer, tradePlayerTDs[returnPlayer][4], 255);
    PlayerTextDrawFont(returnPlayer, tradePlayerTDs[returnPlayer][4], 1);
    PlayerTextDrawSetProportional(returnPlayer, tradePlayerTDs[returnPlayer][4], 1);
    PlayerTextDrawSetSelectable(returnPlayer, tradePlayerTDs[returnPlayer][4], true);
    PlayerTextDrawShow(returnPlayer, tradePlayerTDs[returnPlayer][4]);

    tradePlayerTDs[returnPlayer][5] = CreatePlayerTextDraw(returnPlayer, 49.813209, 200.101257, "<");
    PlayerTextDrawLetterSize(returnPlayer, tradePlayerTDs[returnPlayer][5], 0.398124, 1.815832);
    PlayerTextDrawTextSize(returnPlayer, tradePlayerTDs[returnPlayer][5], 60.000000, 10.000000);
    PlayerTextDrawAlignment(returnPlayer, tradePlayerTDs[returnPlayer][5], 1);
    PlayerTextDrawColour(returnPlayer, tradePlayerTDs[returnPlayer][5], -1);
    PlayerTextDrawSetShadow(returnPlayer, tradePlayerTDs[returnPlayer][5], 0);
    PlayerTextDrawSetOutline(returnPlayer, tradePlayerTDs[returnPlayer][5], 1);
    PlayerTextDrawBackgroundColour(returnPlayer, tradePlayerTDs[returnPlayer][5], 255);
    PlayerTextDrawFont(returnPlayer, tradePlayerTDs[returnPlayer][5], 1);
    PlayerTextDrawSetProportional(returnPlayer, tradePlayerTDs[returnPlayer][5], 1);
    PlayerTextDrawSetSelectable(returnPlayer, tradePlayerTDs[returnPlayer][5], true);
    PlayerTextDrawShow(returnPlayer, tradePlayerTDs[returnPlayer][5]);

    tradePlayerTDs[returnPlayer][6] = CreatePlayerTextDraw(returnPlayer, 354.238098, 200.101257, "<");
    PlayerTextDrawLetterSize(returnPlayer, tradePlayerTDs[returnPlayer][6], 0.398124, 1.815832);
    PlayerTextDrawTextSize(returnPlayer, tradePlayerTDs[returnPlayer][6], 364.000000, 10.000000);
    PlayerTextDrawAlignment(returnPlayer, tradePlayerTDs[returnPlayer][6], 1);
    PlayerTextDrawColour(returnPlayer, tradePlayerTDs[returnPlayer][6], -1);
    PlayerTextDrawSetShadow(returnPlayer, tradePlayerTDs[returnPlayer][6], 0);
    PlayerTextDrawSetOutline(returnPlayer, tradePlayerTDs[returnPlayer][6], 1);
    PlayerTextDrawBackgroundColour(returnPlayer, tradePlayerTDs[returnPlayer][6], 255);
    PlayerTextDrawFont(returnPlayer, tradePlayerTDs[returnPlayer][6], 1);
    PlayerTextDrawSetProportional(returnPlayer, tradePlayerTDs[returnPlayer][6], 1);
    PlayerTextDrawSetSelectable(returnPlayer, tradePlayerTDs[returnPlayer][6], true);
    PlayerTextDrawShow(returnPlayer, tradePlayerTDs[returnPlayer][6]);

    tradePlayerTDs[returnPlayer][7] = CreatePlayerTextDraw(returnPlayer, 576.400634, 200.101257, ">");
    PlayerTextDrawLetterSize(returnPlayer, tradePlayerTDs[returnPlayer][7], 0.398124, 1.815832);
    PlayerTextDrawTextSize(returnPlayer, tradePlayerTDs[returnPlayer][7], 586.000000, 10.000000);
    PlayerTextDrawAlignment(returnPlayer, tradePlayerTDs[returnPlayer][7], 1);
    PlayerTextDrawColour(returnPlayer, tradePlayerTDs[returnPlayer][7], -1);
    PlayerTextDrawSetShadow(returnPlayer, tradePlayerTDs[returnPlayer][7], 0);
    PlayerTextDrawSetOutline(returnPlayer, tradePlayerTDs[returnPlayer][7], 1);
    PlayerTextDrawBackgroundColour(returnPlayer, tradePlayerTDs[returnPlayer][7], 255);
    PlayerTextDrawFont(returnPlayer, tradePlayerTDs[returnPlayer][7], 1);
    PlayerTextDrawSetProportional(returnPlayer, tradePlayerTDs[returnPlayer][7], 1);
    PlayerTextDrawSetSelectable(returnPlayer, tradePlayerTDs[returnPlayer][7], true);
    PlayerTextDrawShow(returnPlayer, tradePlayerTDs[returnPlayer][7]);

    tradePlayerTDs[playerid][8] = CreatePlayerTextDraw(playerid, 318.579925, 169.367706, "CLEAR ITEMS");
    PlayerTextDrawLetterSize(playerid, tradePlayerTDs[playerid][8], 0.187758, 1.139165);
    PlayerTextDrawTextSize(playerid, tradePlayerTDs[playerid][8], 10.000000, 52.000000);
    PlayerTextDrawAlignment(playerid, tradePlayerTDs[playerid][8], 2);
    PlayerTextDrawColour(playerid, tradePlayerTDs[playerid][8], -1);
    PlayerTextDrawUseBox(playerid, tradePlayerTDs[playerid][8], 1);
    PlayerTextDrawBoxColour(playerid, tradePlayerTDs[playerid][8], COLOR_SERVER);
    PlayerTextDrawSetShadow(playerid, tradePlayerTDs[playerid][8], 0);
    PlayerTextDrawSetOutline(playerid, tradePlayerTDs[playerid][8], 1);
    PlayerTextDrawBackgroundColour(playerid, tradePlayerTDs[playerid][8], 255);
    PlayerTextDrawFont(playerid, tradePlayerTDs[playerid][8], 2);
    PlayerTextDrawSetProportional(playerid, tradePlayerTDs[playerid][8], 1);
    PlayerTextDrawSetSelectable(playerid, tradePlayerTDs[playerid][8], true);
    PlayerTextDrawShow(playerid, tradePlayerTDs[playerid][8]);

    tradePlayerTDs[playerid][9] = CreatePlayerTextDraw(playerid, 318.579925, 185.768707, "READY");
    PlayerTextDrawLetterSize(playerid, tradePlayerTDs[playerid][9], 0.187758, 1.139165);
    PlayerTextDrawTextSize(playerid, tradePlayerTDs[playerid][9], 10.000000, 32.000000);
    PlayerTextDrawAlignment(playerid, tradePlayerTDs[playerid][9], 2);
    PlayerTextDrawColour(playerid, tradePlayerTDs[playerid][9], -1);
    PlayerTextDrawUseBox(playerid, tradePlayerTDs[playerid][9], 1);
    PlayerTextDrawBoxColour(playerid, tradePlayerTDs[playerid][9], 932573439);
    PlayerTextDrawSetShadow(playerid, tradePlayerTDs[playerid][9], 0);
    PlayerTextDrawSetOutline(playerid, tradePlayerTDs[playerid][9], 1);
    PlayerTextDrawBackgroundColour(playerid, tradePlayerTDs[playerid][9], 255);
    PlayerTextDrawFont(playerid, tradePlayerTDs[playerid][9], 2);
    PlayerTextDrawSetProportional(playerid, tradePlayerTDs[playerid][9], 1);
    PlayerTextDrawSetSelectable(playerid, tradePlayerTDs[playerid][9], true);
    PlayerTextDrawShow(playerid, tradePlayerTDs[playerid][9]);

    tradePlayerTDs[playerid][10] = CreatePlayerTextDraw(playerid, 318.579925, 152.666687, "ADD ITEM");
    PlayerTextDrawLetterSize(playerid, tradePlayerTDs[playerid][10], 0.187758, 1.139165);
    PlayerTextDrawTextSize(playerid, tradePlayerTDs[playerid][10], 10.000000, 38.000000);
    PlayerTextDrawAlignment(playerid, tradePlayerTDs[playerid][10], 2);
    PlayerTextDrawColour(playerid, tradePlayerTDs[playerid][10], -1);
    PlayerTextDrawUseBox(playerid, tradePlayerTDs[playerid][10], 1);
    PlayerTextDrawBoxColour(playerid, tradePlayerTDs[playerid][10], COLOR_SERVER);
    PlayerTextDrawSetShadow(playerid, tradePlayerTDs[playerid][10], 0);
    PlayerTextDrawSetOutline(playerid, tradePlayerTDs[playerid][10], 1);
    PlayerTextDrawBackgroundColour(playerid, tradePlayerTDs[playerid][10], 255);
    PlayerTextDrawFont(playerid, tradePlayerTDs[playerid][10], 2);
    PlayerTextDrawSetProportional(playerid, tradePlayerTDs[playerid][10], 1);
    PlayerTextDrawSetSelectable(playerid, tradePlayerTDs[playerid][10], true);
    PlayerTextDrawShow(playerid, tradePlayerTDs[playerid][10]);

    tradePlayerTDs[returnPlayer][8] = CreatePlayerTextDraw(returnPlayer, 318.579925, 169.367706, "CLEAR ITEMS");
    PlayerTextDrawLetterSize(returnPlayer, tradePlayerTDs[returnPlayer][8], 0.187758, 1.139165);
    PlayerTextDrawTextSize(returnPlayer, tradePlayerTDs[returnPlayer][8], 10.000000, 52.000000);
    PlayerTextDrawAlignment(returnPlayer, tradePlayerTDs[returnPlayer][8], 2);
    PlayerTextDrawColour(returnPlayer, tradePlayerTDs[returnPlayer][8], -1);
    PlayerTextDrawUseBox(returnPlayer, tradePlayerTDs[returnPlayer][8], 1);
    PlayerTextDrawBoxColour(returnPlayer, tradePlayerTDs[returnPlayer][8], COLOR_SERVER);
    PlayerTextDrawSetShadow(returnPlayer, tradePlayerTDs[returnPlayer][8], 0);
    PlayerTextDrawSetOutline(returnPlayer, tradePlayerTDs[returnPlayer][8], 1);
    PlayerTextDrawBackgroundColour(returnPlayer, tradePlayerTDs[returnPlayer][8], 255);
    PlayerTextDrawFont(returnPlayer, tradePlayerTDs[returnPlayer][8], 2);
    PlayerTextDrawSetProportional(returnPlayer, tradePlayerTDs[returnPlayer][8], 1);
    PlayerTextDrawSetSelectable(returnPlayer, tradePlayerTDs[returnPlayer][8], true);
    PlayerTextDrawShow(returnPlayer, tradePlayerTDs[returnPlayer][8]);

    tradePlayerTDs[returnPlayer][9] = CreatePlayerTextDraw(returnPlayer, 318.579925, 185.768707, "READY");
    PlayerTextDrawLetterSize(returnPlayer, tradePlayerTDs[returnPlayer][9], 0.187758, 1.139165);
    PlayerTextDrawTextSize(returnPlayer, tradePlayerTDs[returnPlayer][9], 10.000000, 32.000000);
    PlayerTextDrawAlignment(returnPlayer, tradePlayerTDs[returnPlayer][9], 2);
    PlayerTextDrawColour(returnPlayer, tradePlayerTDs[returnPlayer][9], -1);
    PlayerTextDrawUseBox(returnPlayer, tradePlayerTDs[returnPlayer][9], 1);
    PlayerTextDrawBoxColour(returnPlayer, tradePlayerTDs[returnPlayer][9], 932573439);
    PlayerTextDrawSetShadow(returnPlayer, tradePlayerTDs[returnPlayer][9], 0);
    PlayerTextDrawSetOutline(returnPlayer, tradePlayerTDs[returnPlayer][9], 1);
    PlayerTextDrawBackgroundColour(returnPlayer, tradePlayerTDs[returnPlayer][9], 255);
    PlayerTextDrawFont(returnPlayer, tradePlayerTDs[returnPlayer][9], 2);
    PlayerTextDrawSetProportional(returnPlayer, tradePlayerTDs[returnPlayer][9], 1);
    PlayerTextDrawSetSelectable(returnPlayer, tradePlayerTDs[returnPlayer][9], true);
    PlayerTextDrawShow(returnPlayer, tradePlayerTDs[returnPlayer][9]);

    tradePlayerTDs[returnPlayer][10] = CreatePlayerTextDraw(returnPlayer, 318.579925, 152.666687, "ADD ITEM");
    PlayerTextDrawLetterSize(returnPlayer, tradePlayerTDs[returnPlayer][10], 0.187758, 1.139165);
    PlayerTextDrawTextSize(returnPlayer, tradePlayerTDs[returnPlayer][10], 10.000000, 38.000000);
    PlayerTextDrawAlignment(returnPlayer, tradePlayerTDs[returnPlayer][10], 2);
    PlayerTextDrawColour(returnPlayer, tradePlayerTDs[returnPlayer][10], -1);
    PlayerTextDrawUseBox(returnPlayer, tradePlayerTDs[returnPlayer][10], 1);
    PlayerTextDrawBoxColour(returnPlayer, tradePlayerTDs[returnPlayer][10], COLOR_SERVER);
    PlayerTextDrawSetShadow(returnPlayer, tradePlayerTDs[returnPlayer][10], 0);
    PlayerTextDrawSetOutline(returnPlayer, tradePlayerTDs[returnPlayer][10], 1);
    PlayerTextDrawBackgroundColour(returnPlayer, tradePlayerTDs[returnPlayer][10], 255);
    PlayerTextDrawFont(returnPlayer, tradePlayerTDs[returnPlayer][10], 2);
    PlayerTextDrawSetProportional(returnPlayer, tradePlayerTDs[returnPlayer][10], 1);
    PlayerTextDrawSetSelectable(returnPlayer, tradePlayerTDs[returnPlayer][10], true);
    PlayerTextDrawShow(returnPlayer, tradePlayerTDs[returnPlayer][10]);

    tradePlayerTDs[playerid][11] = CreatePlayerTextDraw(playerid, 318.285736, 132.338973, "TRADE IN: 10");
    PlayerTextDrawLetterSize(playerid, tradePlayerTDs[playerid][11], 0.219150, 1.273333);
    PlayerTextDrawAlignment(playerid, tradePlayerTDs[playerid][11], 2);
    PlayerTextDrawColour(playerid, tradePlayerTDs[playerid][11], -1);
    PlayerTextDrawSetOutline(playerid, tradePlayerTDs[playerid][11], 1);
    PlayerTextDrawBackgroundColour(playerid, tradePlayerTDs[playerid][11], 255);
    PlayerTextDrawFont(playerid, tradePlayerTDs[playerid][11], 2);
    PlayerTextDrawSetProportional(playerid, tradePlayerTDs[playerid][11], 1);
    PlayerTextDrawSetShadow(playerid, tradePlayerTDs[playerid][11], 0);

    tradePlayerTDs[returnPlayer][11] = CreatePlayerTextDraw(returnPlayer, 318.285736, 132.338973, "TRADE IN: 10");
    PlayerTextDrawLetterSize(returnPlayer, tradePlayerTDs[returnPlayer][11], 0.219150, 1.273333);
    PlayerTextDrawAlignment(returnPlayer, tradePlayerTDs[returnPlayer][11], 2);
    PlayerTextDrawColour(returnPlayer, tradePlayerTDs[returnPlayer][11], -1);
    PlayerTextDrawSetOutline(returnPlayer, tradePlayerTDs[returnPlayer][11], 1);
    PlayerTextDrawBackgroundColour(returnPlayer, tradePlayerTDs[returnPlayer][11], 255);
    PlayerTextDrawFont(returnPlayer, tradePlayerTDs[returnPlayer][11], 2);
    PlayerTextDrawSetProportional(returnPlayer, tradePlayerTDs[returnPlayer][11], 1);
    PlayerTextDrawSetShadow(returnPlayer, tradePlayerTDs[returnPlayer][11], 0);

    // Items

    new i, x;

    for(i = 0, x = 4; i < sizeof tradeItemsPositions[]; ++i, ++x) {
        tradeItemsTDs[playerid][i] = CreatePlayerTextDraw(playerid, tradeItemsPositions[0][i], 148.416580, "");
        PlayerTextDrawLetterSize(playerid, tradeItemsTDs[playerid][i], 0.000000, 0.000000);
        PlayerTextDrawTextSize(playerid, tradeItemsTDs[playerid][i], 56.240200, 52.419929);
        PlayerTextDrawAlignment(playerid, tradeItemsTDs[playerid][i], 1);
        PlayerTextDrawColour(playerid, tradeItemsTDs[playerid][i], -1);
        PlayerTextDrawSetShadow(playerid, tradeItemsTDs[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, tradeItemsTDs[playerid][i], 0);
        PlayerTextDrawBackgroundColour(playerid, tradeItemsTDs[playerid][i], 0x00000085);
        PlayerTextDrawFont(playerid, tradeItemsTDs[playerid][i], 5);
        PlayerTextDrawSetProportional(playerid, tradeItemsTDs[playerid][i], 0);
        PlayerTextDrawSetPreviewRot(playerid, tradeItemsTDs[playerid][i], 0.000000, 0.000000, 0.000000, 1.000000);
        PlayerTextDrawSetPreviewVehCol(playerid, tradeItemsTDs[playerid][i], 1, 1);

        tradeItemsTDs[playerid][x] = CreatePlayerTextDraw(playerid, tradeItemsPositions[1][i], 148.416580, "");
        PlayerTextDrawLetterSize(playerid, tradeItemsTDs[playerid][x], 0.000000, 0.000000);
        PlayerTextDrawTextSize(playerid, tradeItemsTDs[playerid][x], 56.240200, 52.419929);
        PlayerTextDrawAlignment(playerid, tradeItemsTDs[playerid][x], 1);
        PlayerTextDrawColour(playerid, tradeItemsTDs[playerid][x], -1);
        PlayerTextDrawSetShadow(playerid, tradeItemsTDs[playerid][x], 0);
        PlayerTextDrawSetOutline(playerid, tradeItemsTDs[playerid][x], 0);
        PlayerTextDrawBackgroundColour(playerid, tradeItemsTDs[playerid][x], 0x00000085);
        PlayerTextDrawFont(playerid, tradeItemsTDs[playerid][x], 5);
        PlayerTextDrawSetProportional(playerid, tradeItemsTDs[playerid][x], 0);
        PlayerTextDrawSetPreviewRot(playerid, tradeItemsTDs[playerid][x], 0.000000, 0.000000, 0.000000, 1.000000);
        PlayerTextDrawSetPreviewVehCol(playerid, tradeItemsTDs[playerid][x], 1, 1);

        tradeItemsTDs[returnPlayer][i] = CreatePlayerTextDraw(returnPlayer, tradeItemsPositions[0][i], 148.416580, "");
        PlayerTextDrawLetterSize(returnPlayer, tradeItemsTDs[returnPlayer][i], 0.000000, 0.000000);
        PlayerTextDrawTextSize(returnPlayer, tradeItemsTDs[returnPlayer][i], 56.240200, 52.419929);
        PlayerTextDrawAlignment(returnPlayer, tradeItemsTDs[returnPlayer][i], 1);
        PlayerTextDrawColour(returnPlayer, tradeItemsTDs[returnPlayer][i], -1);
        PlayerTextDrawSetShadow(returnPlayer, tradeItemsTDs[returnPlayer][i], 0);
        PlayerTextDrawSetOutline(returnPlayer, tradeItemsTDs[returnPlayer][i], 0);
        PlayerTextDrawBackgroundColour(returnPlayer, tradeItemsTDs[returnPlayer][i], 0x00000085);
        PlayerTextDrawFont(returnPlayer, tradeItemsTDs[returnPlayer][i], 5);
        PlayerTextDrawSetProportional(returnPlayer, tradeItemsTDs[returnPlayer][i], 0);
        PlayerTextDrawSetPreviewRot(returnPlayer, tradeItemsTDs[returnPlayer][i], 0.000000, 0.000000, 0.000000, 1.000000);
        PlayerTextDrawSetPreviewVehCol(returnPlayer, tradeItemsTDs[returnPlayer][i], 1, 1);

        tradeItemsTDs[returnPlayer][x] = CreatePlayerTextDraw(returnPlayer, tradeItemsPositions[1][i], 148.416580, "");
        PlayerTextDrawLetterSize(returnPlayer, tradeItemsTDs[returnPlayer][x], 0.000000, 0.000000);
        PlayerTextDrawTextSize(returnPlayer, tradeItemsTDs[returnPlayer][x], 56.240200, 52.419929);
        PlayerTextDrawAlignment(returnPlayer, tradeItemsTDs[returnPlayer][x], 1);
        PlayerTextDrawColour(returnPlayer, tradeItemsTDs[returnPlayer][x], -1);
        PlayerTextDrawSetShadow(returnPlayer, tradeItemsTDs[returnPlayer][x], 0);
        PlayerTextDrawSetOutline(returnPlayer, tradeItemsTDs[returnPlayer][x], 0);
        PlayerTextDrawBackgroundColour(returnPlayer, tradeItemsTDs[returnPlayer][x], 0x00000085);
        PlayerTextDrawFont(returnPlayer, tradeItemsTDs[returnPlayer][x], 5);
        PlayerTextDrawSetProportional(returnPlayer, tradeItemsTDs[returnPlayer][x], 0);
        PlayerTextDrawSetPreviewRot(returnPlayer, tradeItemsTDs[returnPlayer][x], 0.000000, 0.000000, 0.000000, 1.000000);
        PlayerTextDrawSetPreviewVehCol(returnPlayer, tradeItemsTDs[returnPlayer][x], 1, 1);
    }

    for(i = 0, x = 8; i < sizeof tradeItemsPositions[]; ++i, ++x) {
        tradeItemsTDs[playerid][x] = CreatePlayerTextDraw(playerid, tradeItemsPositions[0][i] + 28.505989, 194.383819, "");
        PlayerTextDrawLetterSize(playerid, tradeItemsTDs[playerid][x], 0.135753, 0.736666);
        PlayerTextDrawAlignment(playerid, tradeItemsTDs[playerid][x], 2);
        PlayerTextDrawColour(playerid, tradeItemsTDs[playerid][x], -1);
        PlayerTextDrawSetShadow(playerid, tradeItemsTDs[playerid][x], 0);
        PlayerTextDrawSetOutline(playerid, tradeItemsTDs[playerid][x], 0);
        PlayerTextDrawBackgroundColour(playerid, tradeItemsTDs[playerid][x], 255);
        PlayerTextDrawFont(playerid, tradeItemsTDs[playerid][x], 2);
        PlayerTextDrawSetProportional(playerid, tradeItemsTDs[playerid][x], 1);
        PlayerTextDrawSetShadow(playerid, tradeItemsTDs[playerid][x], 0);

        tradeItemsTDs[returnPlayer][x] = CreatePlayerTextDraw(returnPlayer, tradeItemsPositions[0][i] + 28.505989, 194.383819, "");
        PlayerTextDrawLetterSize(returnPlayer, tradeItemsTDs[returnPlayer][x], 0.135753, 0.736666);
        PlayerTextDrawAlignment(returnPlayer, tradeItemsTDs[returnPlayer][x], 2);
        PlayerTextDrawColour(returnPlayer, tradeItemsTDs[returnPlayer][x], -1);
        PlayerTextDrawSetShadow(returnPlayer, tradeItemsTDs[returnPlayer][x], 0);
        PlayerTextDrawSetOutline(returnPlayer, tradeItemsTDs[returnPlayer][x], 0);
        PlayerTextDrawBackgroundColour(returnPlayer, tradeItemsTDs[returnPlayer][x], 255);
        PlayerTextDrawFont(returnPlayer, tradeItemsTDs[returnPlayer][x], 2);
        PlayerTextDrawSetProportional(returnPlayer, tradeItemsTDs[returnPlayer][x], 1);
        PlayerTextDrawSetShadow(returnPlayer, tradeItemsTDs[returnPlayer][x], 0);
    }

    for(i = 0, x = 12; i < sizeof tradeItemsPositions[]; ++i, ++x) {
        tradeItemsTDs[playerid][x] = CreatePlayerTextDraw(playerid, tradeItemsPositions[1][i] + 28.505989, 194.383819, "");
        PlayerTextDrawLetterSize(playerid, tradeItemsTDs[playerid][x], 0.135753, 0.736666);
        PlayerTextDrawAlignment(playerid, tradeItemsTDs[playerid][x], 2);
        PlayerTextDrawColour(playerid, tradeItemsTDs[playerid][x], -1);
        PlayerTextDrawSetShadow(playerid, tradeItemsTDs[playerid][x], 0);
        PlayerTextDrawSetOutline(playerid, tradeItemsTDs[playerid][x], 0);
        PlayerTextDrawBackgroundColour(playerid, tradeItemsTDs[playerid][x], 255);
        PlayerTextDrawFont(playerid, tradeItemsTDs[playerid][x], 2);
        PlayerTextDrawSetProportional(playerid, tradeItemsTDs[playerid][x], 1);
        PlayerTextDrawSetShadow(playerid, tradeItemsTDs[playerid][x], 0);

        tradeItemsTDs[returnPlayer][x] = CreatePlayerTextDraw(returnPlayer, tradeItemsPositions[1][i] + 28.505989, 194.383819, "");
        PlayerTextDrawLetterSize(returnPlayer, tradeItemsTDs[returnPlayer][x], 0.135753, 0.736666);
        PlayerTextDrawAlignment(returnPlayer, tradeItemsTDs[returnPlayer][x], 2);
        PlayerTextDrawColour(returnPlayer, tradeItemsTDs[returnPlayer][x], -1);
        PlayerTextDrawSetShadow(returnPlayer, tradeItemsTDs[returnPlayer][x], 0);
        PlayerTextDrawSetOutline(returnPlayer, tradeItemsTDs[returnPlayer][x], 0);
        PlayerTextDrawBackgroundColour(returnPlayer, tradeItemsTDs[returnPlayer][x], 255);
        PlayerTextDrawFont(returnPlayer, tradeItemsTDs[returnPlayer][x], 2);
        PlayerTextDrawSetProportional(returnPlayer, tradeItemsTDs[returnPlayer][x], 1);
        PlayerTextDrawSetShadow(returnPlayer, tradeItemsTDs[returnPlayer][x], 0);
    }
}

static getTradeItemsCount(const playerid) 
    return tradeItemsCount[playerid];

isPlayerInTrade(const playerid)
    return tradeWith[playerid] != -1;

getTradeInvite(const playerid)
    return tradeInviter[playerid]; 

// ------------------------------------- //
//             Callbacks                 //
// ------------------------------------- //

function:OnTradeTimerIsDone(const playerid) {
    new 
        i, 
        item[e_tradeItemsInfo];

    va_SendClientMessage(playerid, COLOR_SERVER, "(Trade): {ffffff}Your traded items:");
    va_SendClientMessage(tradeWith[playerid], COLOR_SERVER, "(Trade): {ffffff}%s's traded items:", GetName(playerid));

    for(i = 0; i < tradeItemsCount[playerid]; ++i) {
        VECTOR_get_arr(tradeItems[playerid], i, item);

        exchangeTradeItem(playerid, item[itemType], item[itemValue]);
    }

    va_SendClientMessage(playerid, -1, "");
    va_SendClientMessage(tradeWith[playerid], -1, "");

    va_SendClientMessage(tradeWith[playerid], COLOR_SERVER, "(Trade): {ffffff}Your traded items:");
    va_SendClientMessage(playerid, COLOR_SERVER, "(Trade): {ffffff}%s's traded items:", GetName(tradeWith[playerid]));

    for(i = 0; i < tradeItemsCount[tradeWith[playerid]]; ++i) {
        VECTOR_get_arr(tradeItems[tradeWith[playerid]], i, item);

        exchangeTradeItem(tradeWith[playerid], item[itemType], item[itemValue]);
    }

    cancelTrade(playerid);
    return 1;
}

// ------------------------------------- //
//             Timers                    //
// ------------------------------------- //

timer tradeTimerCountdown[1000](playerid) {
    if(--tradeTimerSeconds[playerid]) {
        
        format(returnString, sizeof returnString, "TRADE IN: %d", tradeTimerSeconds[playerid]);

        PlayerTextDrawSetString(playerid, tradePlayerTDs[playerid][11], returnString);
        PlayerTextDrawSetString(tradeWith[playerid], tradePlayerTDs[tradeWith[playerid]][11], returnString);
    }
    else {
        stop tradeTimer[playerid];
        tradeTimer[tradeWith[playerid]] = Timer: 0;

        OnTradeTimerIsDone(playerid);
    }
    return 1;
}

// ------------------------------------- //
//             Commands                  //
// ------------------------------------- //

YCMD:trade(playerid, params[], help) {
    if(playerInfo[playerid][pLevel] < 3) 
        return sendError(playerid,"Trebuie sa ai minim level 3 pentru a folosi aceasta comanda.");

    if(tradeDelay[playerid] > gettime())
        return sendError(playerid,"Trebuie sa astepti %d secunde inainte de a folosi iar aceasta comanda.", tradeDelay[playerid] - gettime());

    new returnPlayer; if(sscanf(params, "u", returnPlayer))
        return sendSyntaxMessage(playerid, "/trade <playerid/name>");

    if(returnPlayer == playerid)
        return sendError(playerid,"Nu poti face asta asupra ta.");

    if(!playerStaticVar[returnPlayer][playerLogged])
        return sendError(playerid,"Acel player nu este logat.");

    if(!IsPlayerInRangeOfPlayer(playerid, returnPlayer, 3.0))
        return sendError(playerid,"Nu esti in apropierea acelui player.");

    if(playerInfo[returnPlayer][pLevel] < 3) 
        return sendError(playerid,"Jucatorul respectiv nu are minim level 3.");

    if(isPlayerInTrade(returnPlayer))
        return sendError(playerid,"Acel player este deja intr-un trade.");

    tradeInviter[returnPlayer] = playerid; 

    va_SendClientMessage(playerid, COLOR_SERVER, "(Trade): {ffffff}You invited player %s (%d) for a trade. Wait for him to accept the invite.", GetName(returnPlayer), returnPlayer);
    va_SendClientMessage(returnPlayer, COLOR_SERVER, "(Trade): {ffffff}Player %s (%d) invited you for a trade. Accept his invitation by writing command [/accept trade %d].", GetName(playerid), playerid, playerid);
    
    tradeDelay[playerid] = gettime() + 60;
    return 1;
}

// ------------------------------------- //
//             Dialogs                   //
// ------------------------------------- //

Dialog:TRADEADDITEM(playerid, response, listitem, inputtext[]) {
    if(!response || !isPlayerInTrade(playerid))
        return 1;
    
    switch(listitem) {
        case 0: {
            if(tradeItemsInserted[playerid][TYPE_MONEY])
                return sendError(playerid,"Ai plasat deja bani.");

            if(tradeItemsInserted[tradeWith[playerid]][TYPE_MONEY])
                return sendError(playerid,"Acel player a plasat deja bani.");

            if(!playerInfo[playerid][pAccountLY] && !playerInfo[playerid][pAccount]) 
                return sendError(playerid,"Nu ai bani suficienti.");

            Dialog_Show(playerid, TRADEADDMONEY, DIALOG_STYLE_INPUT, "SERVER: Add Money", "Enter below the value of money you want to trade:", "Add", "Cancel");
        }
        case 1: {
            if(!playerInfo[playerid][pHouse] || playerInfo[playerid][pSQLID] != houseInfo[playerInfo[playerid][pHouse]][houseOwner])
                return sendError(playerid,"You don't own a house.");

            if(tradeItemsInserted[playerid][TYPE_HOUSE])
                return sendError(playerid,"Ai plasat deja casa.");

            if(playerInfo[tradeWith[playerid]][pHouse])
                return sendError(playerid,"Acel player are deja o casa.");

            insertTradeItem(playerid, TYPE_HOUSE, 1272, playerInfo[playerid][pHouse]);

            va_SendClientMessage(playerid, COLOR_SERVER, "(Trade): {ffffff}You added house %d.", houseInfo[playerInfo[playerid][pHouse]][houseID]);
            va_SendClientMessage(tradeWith[playerid], COLOR_SERVER, "(Trade): {ffffff}%s added house %d.", GetName(playerid), houseInfo[playerInfo[playerid][pHouse]][houseID]);
        }
        case 2: {
            if(!playerInfo[playerid][pBusiness])
                return sendError(playerid,"Nu ai un business.");

            if(tradeItemsInserted[playerid][TYPE_BUSINESS])
                return sendError(playerid,"Ai plasat deja un business.");

            if(playerInfo[tradeWith[playerid]][pBusiness])
                return sendError(playerid,"Acel player are deja un business.");

            insertTradeItem(playerid, TYPE_BUSINESS, 1274, playerInfo[playerid][pBusiness]);

            va_SendClientMessage(playerid, COLOR_SERVER, "(Trade): {ffffff}You added business %d.", bizInfo[playerInfo[playerid][pBusiness]][bizID]);
            va_SendClientMessage(tradeWith[playerid], COLOR_SERVER, "(Trade): {ffffff}%s added business %d.", GetName(playerid), bizInfo[playerInfo[playerid][pBusiness]][bizID]);
        }
        case 3: {
            if(tradeItemsInserted[playerid][TYPE_DRUGS]) 
                return sendError(playerid,"Ai plasat deja marijuana.");

            if(tradeItemsInserted[tradeWith[playerid]][TYPE_DRUGS])
                return sendError(playerid,"Acel player a plasat deja marijuana.");

            if(!playerInfo[playerid][pDrugs])
                return sendError(playerid,"Nu ai marijuana.");

            Dialog_Show(playerid, TRADEADDDRUGS, DIALOG_STYLE_INPUT, "SERVER: Add marijuana", "Enter below the value of marijuana you want to trade:", "Add", "Cancel");
        }
        case 4: {
            if(tradeItemsInserted[playerid][TYPE_COCAINE]) 
                return sendError(playerid,"Ai plasat deja cocaine.");

            if(tradeItemsInserted[tradeWith[playerid]][TYPE_COCAINE])
                return sendError(playerid,"Acel player a plasat deja cocaine.");

            if(!playerInfo[playerid][pCocaine])
                return sendError(playerid,"Nu ai droguri.");

            Dialog_Show(playerid, TRADEADDDRUGS2, DIALOG_STYLE_INPUT, "SERVER: Add Cocaine", "Enter below the value of cocaine you want to trade:", "Add", "Cancel");
        }
        case 5: {
            if(tradeItemsInserted[playerid][TYPE_MUSHROOMS]) 
                return sendError(playerid,"Ai plasat deja mushrooms.");

            if(tradeItemsInserted[tradeWith[playerid]][TYPE_MUSHROOMS])
                return sendError(playerid,"Acel player a plasat deja mushrooms.");

            if(!playerInfo[playerid][pMushrooms])
                return sendError(playerid,"Nu ai mushrooms.");

            Dialog_Show(playerid, TRADEADDDRUGS3, DIALOG_STYLE_INPUT, "SERVER: Add mushrooms", "Enter below the value of mushrooms you want to trade:", "Add", "Cancel");
        }
        case 6: {
            if(tradeItemsInserted[playerid][TYPE_MATERIALS]) 
                return sendError(playerid,"Ai plasat deja materiale.");

            if(tradeItemsInserted[tradeWith[playerid]][TYPE_MATERIALS])
                return sendError(playerid,"Acel player a plasat deja materiale.");

            if(!playerInfo[playerid][pMaterials])
                return sendError(playerid,"Nu ai materiale.");

            Dialog_Show(playerid, TRADEADDMATS, DIALOG_STYLE_INPUT, "SERVER: Add Materials", "Enter below the value of materials you want to trade:", "Add", "Cancel");
        }
        case 7: {
            mysql_format(SQL, returnString, sizeof returnString, "select * from `personalvehicles` where `Owner` = '%e' order by `id` asc;", playerInfo[playerid][pName]);
            mysql_tquery(SQL, returnString, "showTradePersonalVehicles", "d", playerid);
        }
    }
    return 1;
}

Dialog:TRADEADDMONEY(playerid, response, listitem, inputtext[]) {
    if(!response || !isPlayerInTrade(playerid))
        return 1;

    if(tradeItemsInserted[playerid][TYPE_MONEY])
        return sendError(playerid,"Ai plasat deja bani.");

    if(tradeItemsInserted[tradeWith[playerid]][TYPE_MONEY])
        return sendError(playerid,"Acel player a plasat deja bani.");

    if(!isnumeric(inputtext) || strlen(inputtext) > 20)
        return Dialog_Show(playerid, TRADEADDMONEY, DIALOG_STYLE_INPUT, "SERVER: Add Money", "Enter below the value of money you want to trade:", "Add", "Cancel");

    delimiteMoneyFromString(tradeItemsMoney[playerid][0], tradeItemsMoney[playerid][1], inputtext);

    if(tradeItemsMoney[playerid][0] > playerInfo[playerid][pAccountLY] || (tradeItemsMoney[playerid][0] >= playerInfo[playerid][pAccountLY] && tradeItemsMoney[playerid][1] > playerInfo[playerid][pAccount]) || (tradeItemsMoney[playerid][0] <= 0 && tradeItemsMoney[playerid][1] <= 0))
        return Dialog_Show(playerid, TRADEADDMONEY, DIALOG_STYLE_INPUT, "SERVER: Add Money", "Enter below the value of money you want to trade:\n\n{ff0000}Invalid amount of money!", "Add", "Cancel");

    insertTradeItem(playerid, TYPE_MONEY, 1212, -1);

    va_SendClientMessage(playerid, COLOR_SERVER, "(Trade): {ffffff}You added $%s money.", formatMoney(tradeItemsMoney[playerid][0], tradeItemsMoney[playerid][1]));
    va_SendClientMessage(tradeWith[playerid], COLOR_SERVER, "(Trade): {ffffff}%s added $%s money.", GetName(playerid), formatMoney(tradeItemsMoney[playerid][0], tradeItemsMoney[playerid][1]));
    return 1;
}

Dialog:TRADEADDDRUGS(playerid, response, listitem, inputtext[]) {
    if(!response || !isPlayerInTrade(playerid))
        return 1;

    if(tradeItemsInserted[playerid][TYPE_DRUGS]) 
        return sendError(playerid,"Ai plasat deja marijuana.");

    if(tradeItemsInserted[tradeWith[playerid]][TYPE_DRUGS])
        return sendError(playerid,"Acel player a plasat deja marijuana.");

    new drugs; if(sscanf(inputtext, "d", drugs))
        return Dialog_Show(playerid, TRADEADDDRUGS, DIALOG_STYLE_INPUT, "SERVER: Add Drugs", "Enter below the value of marijuana you want to trade:", "Add", "Cancel");

    if(drugs <= 0 || drugs > playerInfo[playerid][pDrugs])
        return Dialog_Show(playerid, TRADEADDDRUGS, DIALOG_STYLE_INPUT, "SERVER: Add Drugs", "Enter below the value of marijuana you want to trade:\n\n{ff0000}Invalid amount of marijuana!", "Add", "Cancel");

    insertTradeItem(playerid, TYPE_DRUGS, 1279, drugs);

    va_SendClientMessage(playerid, COLOR_SERVER, "(Trade): {ffffff}You added %s marijuana.", FormatNumber(drugs));
    va_SendClientMessage(tradeWith[playerid], COLOR_SERVER, "(Trade): {ffffff}%s added %s marijuana.", GetName(playerid), FormatNumber(drugs));
    return 1;
}

Dialog:TRADEADDDRUGS2(playerid, response, listitem, inputtext[]) {
    if(!response || !isPlayerInTrade(playerid))
        return 1;

    if(tradeItemsInserted[playerid][TYPE_COCAINE]) 
        return sendError(playerid,"Ai plasat deja cocaine.");

    if(tradeItemsInserted[tradeWith[playerid]][TYPE_COCAINE])
        return sendError(playerid,"Acel player a plasat deja cocaine.");

    new drugs; if(sscanf(inputtext, "d", drugs))
        return Dialog_Show(playerid, TRADEADDDRUGS, DIALOG_STYLE_INPUT, "SERVER: Add cocaine", "Enter below the value of cocaine you want to trade:", "Add", "Cancel");

    if(drugs <= 0 || drugs > playerInfo[playerid][pCocaine])
        return Dialog_Show(playerid, TRADEADDDRUGS, DIALOG_STYLE_INPUT, "SERVER: Add cocaine", "Enter below the value of cocaine you want to trade:\n\n{ff0000}Invalid amount of cocaine!", "Add", "Cancel");

    insertTradeItem(playerid, TYPE_COCAINE, 1279, drugs);

    va_SendClientMessage(playerid, COLOR_SERVER, "(Trade): {ffffff}You added %s drugs.", FormatNumber(drugs));
    va_SendClientMessage(tradeWith[playerid], COLOR_SERVER, "(Trade): {ffffff}%s added %s drugs.", GetName(playerid), FormatNumber(drugs));
    return 1;
}

Dialog:TRADEADDDRUGS3(playerid, response, listitem, inputtext[]) {
    if(!response || !isPlayerInTrade(playerid))
        return 1;

    if(tradeItemsInserted[playerid][TYPE_MUSHROOMS]) 
        return sendError(playerid,"Ai plasat deja mushrooms.");

    if(tradeItemsInserted[tradeWith[playerid]][TYPE_MUSHROOMS])
        return sendError(playerid,"Acel player a plasat deja mushrooms.");

    new drugs; if(sscanf(inputtext, "d", drugs))
        return Dialog_Show(playerid, TRADEADDDRUGS, DIALOG_STYLE_INPUT, "SERVER: Add mushrooms", "Enter below the value of mushrooms you want to trade:", "Add", "Cancel");

    if(drugs <= 0 || drugs > playerInfo[playerid][pCocaine])
        return Dialog_Show(playerid, TRADEADDDRUGS, DIALOG_STYLE_INPUT, "SERVER: Add mushrooms", "Enter below the value of mushrooms you want to trade:\n\n{ff0000}Invalid amount of mushrooms!", "Add", "Cancel");

    insertTradeItem(playerid, TYPE_MUSHROOMS, 1279, drugs);

    va_SendClientMessage(playerid, COLOR_SERVER, "(Trade): {ffffff}You added %s mushrooms.", FormatNumber(drugs));
    va_SendClientMessage(tradeWith[playerid], COLOR_SERVER, "(Trade): {ffffff}%s added %s mushrooms.", GetName(playerid), FormatNumber(drugs));
    return 1;
}

Dialog:TRADEADDMATS(playerid, response, listitem, inputtext[]) {
    if(!response || !isPlayerInTrade(playerid))
        return 1;

    if(tradeItemsInserted[playerid][TYPE_MATERIALS]) 
        return sendError(playerid,"Ai plasat deja materiale.");

    if(tradeItemsInserted[tradeWith[playerid]][TYPE_MATERIALS])
        return sendError(playerid,"Acel player a plasat deja materiale.");

    new mats; if(sscanf(inputtext, "d", mats))
        return Dialog_Show(playerid, TRADEADDMATS, DIALOG_STYLE_INPUT, "SERVER: Add Materials", "Enter below the value of materials you want to trade:", "Add", "Cancel");

    if(mats <= 0 || mats > playerInfo[playerid][pMaterials])
        return Dialog_Show(playerid, TRADEADDMATS, DIALOG_STYLE_INPUT, "SERVER: Add Materials", "Enter below the value of materials you want to trade:\n\n{ff0000}Invalid amount of materials!", "Add", "Cancel");

    insertTradeItem(playerid, TYPE_MATERIALS, 1575, mats);

    va_SendClientMessage(playerid, COLOR_SERVER, "(Trade): {ffffff}You added %s materials.", FormatNumber(mats));
    va_SendClientMessage(tradeWith[playerid], COLOR_SERVER, "(Trade): {ffffff}%s added %s materials.", GetName(playerid), FormatNumber(mats));
    return 1;
}

Dialog:TRADEADDPERSONALVEHICLES(playerid, response, listitem, inputtext[]) {
    if(!response || !isPlayerInTrade(playerid))
        return 1;

    if(tradePersonalVehiclesCount[playerid] >= playerInfo[tradeWith[playerid]][pCarSlots])
        return sendError(playerid,"Acel player nu are suficiente sloturi pentru vehicule.");

    if(haveTradePersonalVehicle(playerid, playerStaticVar[playerid][pDialogItems][listitem]))
        return sendError(playerid,"Ai adaugat deja acest vehicul.");

    insertTradeItem(playerid, TYPE_VEHICLE, carInfo[playerStaticVar[playerid][pDialogItems][listitem]][cModel], playerStaticVar[playerid][pDialogItems][listitem]);
    
    tradePersonalVehiclesCount[playerid] ++;

    va_SendClientMessage(playerid, COLOR_SERVER, "(Trade): {ffffff}Ai adaugat vehiculul %s cu urmatoarele caracteristici: %.0f KM, %d zile, %s culori %d %d.", aVehicleNames[carInfo[playerStaticVar[playerid][pDialogItems][listitem]][cModel] - 400], carInfo[playerStaticVar[playerid][pDialogItems][listitem]][cKM], getDaysFromTimestamp(carInfo[playerStaticVar[playerid][pDialogItems][listitem]][cDays]), (carInfo[playerStaticVar[playerid][pDialogItems][listitem]][cVIP] ? (", VIP") : ("")), carInfo[playerStaticVar[playerid][pDialogItems][listitem]][cColor][0], carInfo[playerStaticVar[playerid][pDialogItems][listitem]][cColor][1]);
    va_SendClientMessage(tradeWith[playerid], COLOR_SERVER, "(Trade): {ffffff}%s a adaugat vehiculul %s cu urmatoarele caracteristici: %.0f KM, %d zile, %s culori %d %d.", GetName(playerid), aVehicleNames[carInfo[playerStaticVar[playerid][pDialogItems][listitem]][cModel] - 400], carInfo[playerStaticVar[playerid][pDialogItems][listitem]][cKM], getDaysFromTimestamp(carInfo[playerStaticVar[playerid][pDialogItems][listitem]][cDays]), (carInfo[playerStaticVar[playerid][pDialogItems][listitem]][cVIP] ? (", VIP") : ("")), carInfo[playerStaticVar[playerid][pDialogItems][listitem]][cColor][0], carInfo[playerStaticVar[playerid][pDialogItems][listitem]][cColor][1]);
    return 1;
}

// ------------------------------------- //
//             Hooks                     //
// ------------------------------------- //

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid) {
    if(isPlayerInTrade(playerid)) {
        // if(Dialog_IsOpened(playerid))
        //     return 1;

        if(playertextid == tradePlayerTDs[playerid][8]) {
            if(!getTradeItemsCount(playerid))
                return 1;

            if(tradePlayerReady[playerid])
                return sendError(playerid,"Nu poti face asta deoarece esti pregatit pentru incheierea negocierii.");

            if(tradeItemsIndex[playerid][0] >= TRADE_ITEMS_PAGE_LIMIT)
                PlayerTextDrawSetString(playerid, tradePlayerTDs[playerid][2], "your items (page: 1)");

            if(tradeItemsIndex[tradeWith[playerid]][1] >= TRADE_ITEMS_PAGE_LIMIT) {
                
                format(returnString, sizeof returnString, "%s's items (page: 1)", GetName(playerid));
                PlayerTextDrawSetString(tradeWith[playerid], tradePlayerTDs[tradeWith[playerid]][3], returnString);
            }

            clearTradeItems(playerid);

            va_SendClientMessage(playerid, COLOR_SERVER, "(Trade): {ffffff}You cleared all your items successfully.");
            va_SendClientMessage(tradeWith[playerid], COLOR_SERVER, "(Trade): {ffffff}%s cleared his items.", GetName(playerid));
        }
        else if(playertextid == tradePlayerTDs[playerid][9]) {
            if(!tradePlayerReady[playerid]) {
                if(!getTradeItemsCount(playerid)) 
                    return sendError(playerid,"Trebuie sa ai minim 1 item plasat la negociere.");

                if(tradeReadyDelay[playerid] > gettime())
                    return 1;

                tradePlayerReady[playerid] = true;

                PlayerTextDrawBoxColour(playerid, tradePlayerTDs[playerid][9], 0xFF0000FF);
                PlayerTextDrawSetString(playerid, tradePlayerTDs[playerid][9], "CANCEL");
                PlayerTextDrawShow(playerid, tradePlayerTDs[playerid][9]);

                
                format(returnString, sizeof returnString, "(Trade): {ffffff}%s is ready. (%d/2)", GetName(playerid), 1 + _:tradePlayerReady[tradeWith[playerid]]);

                va_SendClientMessage(tradeWith[playerid], COLOR_SERVER, returnString);
                va_SendClientMessage(playerid, COLOR_SERVER, returnString);

                if(tradePlayerReady[tradeWith[playerid]]) {
                    tradeTimerSeconds[playerid] = 10;

                    PlayerTextDrawSetString(playerid, tradePlayerTDs[playerid][11], "TRADE IN: 10");
                    PlayerTextDrawSetString(tradeWith[playerid], tradePlayerTDs[tradeWith[playerid]][11], "TRADE IN: 10");
                    PlayerTextDrawShow(playerid, tradePlayerTDs[playerid][11]);
                    PlayerTextDrawShow(tradeWith[playerid], tradePlayerTDs[tradeWith[playerid]][11]);

                    tradeTimer[playerid] = tradeTimer[tradeWith[playerid]] = repeat tradeTimerCountdown(playerid); 
                }
            }
            else {
                tradePlayerReady[playerid] = false;

                PlayerTextDrawBoxColour(playerid, tradePlayerTDs[playerid][9], 932573439);
                PlayerTextDrawSetString(playerid, tradePlayerTDs[playerid][9], "READY");
                PlayerTextDrawShow(playerid, tradePlayerTDs[playerid][9]);

                if(tradePlayerReady[tradeWith[playerid]]) {
                    tradePlayerReady[tradeWith[playerid]] = false;

                    PlayerTextDrawHide(playerid, tradePlayerTDs[playerid][11]);
                    PlayerTextDrawHide(tradeWith[playerid], tradePlayerTDs[tradeWith[playerid]][11]);

                    PlayerTextDrawBoxColour(tradeWith[playerid], tradePlayerTDs[tradeWith[playerid]][9], 932573439);
                    PlayerTextDrawSetString(tradeWith[playerid], tradePlayerTDs[tradeWith[playerid]][9], "READY");
                    PlayerTextDrawShow(tradeWith[playerid], tradePlayerTDs[tradeWith[playerid]][9]);

                    stop tradeTimer[playerid];
                    tradeTimer[tradeWith[playerid]] = Timer: 0;

                    va_SendClientMessage(playerid, COLOR_SERVER, "(Trade): {ffffff}Trade countdown has been stopped by %s.", GetName(playerid));
                    va_SendClientMessage(tradeWith[playerid], COLOR_SERVER, "(Trade): {ffffff}Trade countdown has been stopped by %s.", GetName(playerid));
                }

                tradeReadyDelay[playerid] = gettime() + 2;
            }
        }
        else if(playertextid == tradePlayerTDs[playerid][10]) {
            if(tradePlayerReady[playerid])
                return sendError(playerid,"Nu poti face asta deoarece esti pregatit pentru incheierea negocierii.");

            Dialog_Show(playerid, TRADEADDITEM, DIALOG_STYLE_LIST, "SERVER: Add Item", "Money (from bank)\nHouse\nBusiness\nMarijuana\nCocaine\nMushrooms\nMaterials\nPersonal Vehicles", "Add", "Cancel");
        }
        else if(playertextid == tradePlayerTDs[playerid][5] && tradeItemsIndex[playerid][0] >= TRADE_ITEMS_PAGE_LIMIT) {
            tradeItemsIndex[playerid][0] = ((tradeItemsIndex[playerid][0] / TRADE_ITEMS_PAGE_LIMIT) * TRADE_ITEMS_PAGE_LIMIT) - TRADE_ITEMS_PAGE_LIMIT;

            
            format(returnString, sizeof returnString, "your items (page: %d)", (tradeItemsIndex[playerid][0] / TRADE_ITEMS_PAGE_LIMIT) + 1);
            PlayerTextDrawSetString(playerid, tradePlayerTDs[playerid][2], returnString);

            showTradeItems(playerid, playerid, tradeItemsIndex[playerid][0]);
        }
        else if(playertextid == tradePlayerTDs[playerid][4] && getTradeItemsCount(playerid) > tradeItemsIndex[playerid][0] + 1) {
            tradeItemsIndex[playerid][0] ++;

            
            format(returnString, sizeof returnString, "your items (page: %d)", (tradeItemsIndex[playerid][0] / TRADE_ITEMS_PAGE_LIMIT) + 1);
            PlayerTextDrawSetString(playerid, tradePlayerTDs[playerid][2], returnString);
            
            showTradeItems(playerid, playerid, tradeItemsIndex[playerid][0]);
        }
        else if(playertextid == tradePlayerTDs[playerid][6] && tradeItemsIndex[playerid][1] >= TRADE_ITEMS_PAGE_LIMIT) {
            tradeItemsIndex[playerid][1] = ((tradeItemsIndex[playerid][1] / TRADE_ITEMS_PAGE_LIMIT) * TRADE_ITEMS_PAGE_LIMIT) - TRADE_ITEMS_PAGE_LIMIT;
            
            
            format(returnString, sizeof returnString, "%s's items (page: %d)", GetName(tradeWith[playerid]), (tradeItemsIndex[playerid][1] / TRADE_ITEMS_PAGE_LIMIT) + 1);
            PlayerTextDrawSetString(playerid, tradePlayerTDs[playerid][3], returnString);

            showTradeItems(playerid, tradeWith[playerid], tradeItemsIndex[playerid][1]);
        } 
        else if(playertextid == tradePlayerTDs[playerid][7] && getTradeItemsCount(tradeWith[playerid]) > tradeItemsIndex[playerid][1] + 1) {
            tradeItemsIndex[playerid][1] ++;

            
            format(returnString, sizeof returnString, "%s's items (page: %d)", GetName(tradeWith[playerid]), (tradeItemsIndex[playerid][1] / TRADE_ITEMS_PAGE_LIMIT) + 1);
            PlayerTextDrawSetString(playerid, tradePlayerTDs[playerid][3], returnString);
            
            showTradeItems(playerid, tradeWith[playerid], tradeItemsIndex[playerid][1]);
        }
    }
    return 1;
}
hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if(clickedid == INVALID_TEXT_DRAW && isPlayerInTrade(playerid)) {
        if(tradeTimer[playerid] != Timer: 0) 
            return SelectTextDraw(playerid, -1);

        va_SendClientMessage(tradeWith[playerid], COLOR_SERVER, "(Trade): {ffffff}The trade has been canceled by %s.", GetName(playerid));
        va_SendClientMessage(playerid, COLOR_SERVER, "(Trade): {ffffff}You canceled the trade.");

        cancelTrade(playerid);
    }

    return Y_HOOKS_CONTINUE_RETURN_1;
}


hook OnPlayerDisconnect(playerid, reason) {
    if(isPlayerInTrade(playerid)) {
        VECTOR_clear(tradeItems[playerid]);
        VECTOR_clear(tradeItems[tradeWith[playerid]]);

        destroyTradeTextdraws(playerid);

        stop tradeTimer[playerid];
        tradeTimer[tradeWith[playerid]] = Timer: 0;

        tradeWith[tradeWith[playerid]] = -1;

        CancelSelectTextDraw(tradeWith[playerid]);
        TogglePlayerControllable(tradeWith[playerid], true);

        va_SendClientMessage(tradeWith[playerid], COLOR_SERVER, "(Trade): {ffffff}The trade has been canceled because %s has been disconnected from server.", GetName(playerid));
    } 
    return 1;
}

hook whenPlayerDie(playerid, killerid, reason) {
    if(isPlayerInTrade(playerid)) {
        va_SendClientMessage(tradeWith[playerid], COLOR_SERVER, "(Trade): {ffffff}The trade has been canceled because %s died.", GetName(playerid));
        va_SendClientMessage(playerid, COLOR_SERVER, "(Trade): {ffffff}The trade has been canceled because you died.");

        stop tradeTimer[playerid];
        tradeTimer[tradeWith[playerid]] = Timer: 0;

        cancelTrade(playerid);
    }
    return 1;
}

hook OnPlayerConnect(playerid) {
    tradeDelay[playerid] = tradeReadyDelay[playerid] = -1;
    return 1;
}