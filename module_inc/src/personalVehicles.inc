#include <YSI_Coding\y_hooks>
#include <rotations>

#define MAX_VEHICLE_SPAWN_TIME 300

enum ENUM_PERSONAL_VEH_INFORMATIONS
{
    cID, cModel, cOwner[MAX_PLAYER_NAME], Float:cPosition[4], Float:cLastPos[4], cColor[2], cPlate[12], cFuel, Float:cKM, cVIP, cDays, cSpawnTime,
    cInsurance, cInsurancePrice, cLocked, cVIPText[13], cVIPColor[7], Float:cVIPPos[6], cMod[17], cPaintjob, cSpecial, cVirtual
};
new carInfo[MAX_VEHICLES][ENUM_PERSONAL_VEH_INFORMATIONS], toSpawnCar[MAX_PLAYERS], VehicleSQL[MAX_VEHICLES], VIPObject[MAX_VEHICLES];

hook loadServerSystems()
{
    mysql_tquery(SQL, "select * from `personalvehicles` order by `ID` asc;", "loadPersonalVehicles", "");
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnGameModeInit()
{
    Command_AddAltNamed("v", "vehicles");
    Command_AddAltNamed("v", "cars");
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook WhenPlayerLoggedIn(playerid)
{
    if(!cache_num_rows())
        return Y_HOOKS_CONTINUE_RETURN_1;

    cache_get_value_name_int(0, "carSlots", playerInfo[playerid][pCarSlots]);
    return Y_HOOKS_CONTINUE_RETURN_1;
}

function:loadPersonalVehicles()
{
    for(new i; i < cache_num_rows(); i++)
    {
        static loadString[100], vehicleDBId;
        cache_get_value_name_int(i, "ID", vehicleDBId);

        cache_get_value_name(i, "Owner", carInfo[vehicleDBId][cOwner]);
        cache_get_value_name(i, "Plate", carInfo[vehicleDBId][cPlate]);
        cache_get_value_name(i, "VIPText", carInfo[vehicleDBId][cVIPText]);
        cache_get_value_name(i, "VIPColor", carInfo[vehicleDBId][cVIPColor]);

        cache_get_value_name_int(i, "ID", carInfo[vehicleDBId][cID]);
        cache_get_value_name_int(i, "Model", carInfo[vehicleDBId][cModel]);

        cache_get_value_name_float(i, "KM", carInfo[vehicleDBId][cKM]);

        cache_get_value_name_int(i, "VIP", carInfo[vehicleDBId][cVIP]);
        cache_get_value_name_int(i, "Days", carInfo[vehicleDBId][cDays]);
        cache_get_value_name_int(i, "Fuel", carInfo[vehicleDBId][cFuel]);
        cache_get_value_name_int(i, "VirtualWorld", carInfo[vehicleDBId][cVirtual]);

        cache_get_value_name_int(i, "Insurance", carInfo[vehicleDBId][cInsurance]);
        cache_get_value_name_int(i, "InsurancePrice", carInfo[vehicleDBId][cInsurancePrice]);

        cache_get_value_name_int(i, "paintJob", carInfo[vehicleDBId][cPaintjob]);

        cache_get_value_name_int(i, "Color1", carInfo[vehicleDBId][cColor][0]);
        cache_get_value_name_int(i, "Color2", carInfo[vehicleDBId][cColor][1]);

        cache_get_value_name_float(i, "posX", carInfo[vehicleDBId][cPosition][0]);
        cache_get_value_name_float(i, "posY", carInfo[vehicleDBId][cPosition][1]);
        cache_get_value_name_float(i, "posZ", carInfo[vehicleDBId][cPosition][2]);
        cache_get_value_name_float(i, "posA", carInfo[vehicleDBId][cPosition][3]);

        cache_get_value_name(i, "VIPPos", loadString);
        sscanf(loadString, "a<f>[6]", carInfo[vehicleDBId][cVIPPos]);

        cache_get_value_name(i, "Tunning", loadString);
        sscanf(loadString, "a<i>[17]", carInfo[vehicleDBId][cMod]);

        cache_get_value_name_int(i, "Special", carInfo[vehicleDBId][cSpecial]);
        carInfo[vehicleDBId][cLocked] = 1;
    }
    return true;
}

new spoiler[20][3] =
{
    {1000}, {1001}, {1002}, {1003}, {1014}, {1015}, {1016}, {1023}, {1058}, {1060}, {1049}, {1050}, {1138}, {1139}, {1146}, {1147}, {1158}, {1162}, {1163}, 
    {1164}
};

new nitro[3][3] =
{
    {1008}, {1009}, {1010}
};

new fbumper[23][3] =
{
    {1117}, {1152}, {1153}, {1155}, {1157}, {1160}, {1165}, {1167}, {1169}, {1170}, {1171}, {1172}, {1173}, {1174}, {1175}, {1179}, {1181}, {1182},
    {1185}, {1188}, {1189}, {1192}, {1193}
};

new rbumper[22][3] =
{
    {1140}, {1141}, {1148}, {1149}, {1150}, {1151}, {1154}, {1156}, {1159}, {1161}, {1166}, {1168}, {1176}, {1177}, {1178}, {1180}, {1183}, {1184},
    {1186}, {1187}, {1190}, {1191}
};

new exhaust[28][3] =
{
    {1018}, {1019}, {1020}, {1021}, {1022}, {1028}, {1029}, {1037}, {1043}, {1044}, {1045}, {1046}, {1059}, {1064}, {1065}, {1066}, {1089}, {1092},
    {1104}, {1105}, {1113}, {1114}, {1126}, {1127}, {1129}, {1132}, {1135}, {1136}
};

new bventr[2][3] =
{
    {1142}, {1144}
};

new bventl[2][3] =
{
    {1143}, {1145}
};

new bscoop[4][3] =
{
    {1004}, {1005}, {1011}, {1012}
};

new rscoop[17][3] =
{
    {1006}, {1032}, {1033}, {1035}, {1038}, {1053}, {1054}, {1055}, {1061}, {1067}, {1068}, {1088}, {1091}, {1103}, {1128}, {1130}, {1131}
};

new lskirt[21][3] =
{
    {1007}, {1026}, {1031}, {1036}, {1039}, {1042}, {1047}, {1048}, {1056}, {1057}, {1069}, {1070}, {1090}, {1093}, {1106}, {1108}, {1118}, {1119},
    {1133}, {1122}, {1134}
};

new rskirt[21][3] =
{
    {1017}, {1027}, {1030}, {1040}, {1041}, {1051}, {1052}, {1062}, {1063}, {1071}, {1072}, {1094}, {1095}, {1099}, {1101}, {1102}, {1107}, {1120}, 
    {1121}, {1124}, {1137}
};

new hydraulics[1][3] =
{
    {1087}
};

new vbase[1][3] =
{
    {1086}
};

new rbbars[4][3] =
{
    {1109}, {1110}, {1123}, {1125}
};

new fbbars[2][3] =
{
    {1115}, {1116}
};

new wheels[17][3] =
{
    {1025}, {1073}, {1074}, {1075}, {1076}, {1077}, {1078}, {1079}, {1080}, {1081}, {1082}, {1083}, {1084}, {1085}, {1096}, {1097}, {1098}
};

new vlights[2][3] =
{
    {1013}, {1024}
};

saveComponent(vehicleid, componentid)
{
    new vehicleDBId = VehicleSQL[vehicleid];
    for(new i; i < 20; i++)
    {
        if(componentid == spoiler[i][0])
            carInfo[vehicleDBId][cMod][0] = componentid;
    }
    for(new i; i < 3; i++)
    {
        if(componentid == nitro[i][0])
            carInfo[vehicleDBId][cMod][1] = componentid;
    }
    for(new i; i < 23; i++)
    {
        if(componentid == fbumper[i][0])
            carInfo[vehicleDBId][cMod][2] = componentid;
    }
    for(new i; i < 22; i++)
    {
        if(componentid == rbumper[i][0])
            carInfo[vehicleDBId][cMod][3] = componentid;
    }
    for(new i; i < 28; i++)
    {
        if(componentid == exhaust[i][0])
            carInfo[vehicleDBId][cMod][4] = componentid;
    }
    for(new i; i < 2; i++)
    {
        if(componentid == bventr[i][0])
            carInfo[vehicleDBId][cMod][5] = componentid;
    }
    for(new i; i < 2; i++)
    {
        if(componentid == bventl[i][0])
            carInfo[vehicleDBId][cMod][6] = componentid;
    }
    for(new i; i < 4; i++)
    {
        if(componentid == bscoop[i][0])
            carInfo[vehicleDBId][cMod][7] = componentid;
    }
    for(new i; i < 17; i++)
    {
        if(componentid == rscoop[i][0])
            carInfo[vehicleDBId][cMod][8] = componentid;
    }
    for(new i; i < 21; i++)
    {
        if(componentid == lskirt[i][0])
            carInfo[vehicleDBId][cMod][9] = componentid;
    }
    for(new i; i < 21; i++)
    {
        if(componentid == rskirt[i][0])
            carInfo[vehicleDBId][cMod][10] = componentid;
    }
    for(new i; i < 1; i++)
    {
        if(componentid == hydraulics[i][0])
            carInfo[vehicleDBId][cMod][11] = componentid;
    }
    for(new i; i < 1; i++)
    {
        if(componentid == vbase[i][0])
            carInfo[vehicleDBId][cMod][12] = componentid;
    }
    for(new i; i < 4; i++)
    {
        if(componentid == rbbars[i][0])
            carInfo[vehicleDBId][cMod][13] = componentid;
    }
    for(new i; i < 2; i++)
    {
        if(componentid == fbbars[i][0])
            carInfo[vehicleDBId][cMod][14] = componentid;
    }
    for(new i; i < 17; i++)
    {
        if(componentid == wheels[i][0])
            carInfo[vehicleDBId][cMod][15] = componentid;
    }
    for(new i; i < 2; i++)
    {
        if(componentid == vlights[i][0])
            carInfo[vehicleDBId][cMod][16] = componentid;
    }
    
    mysql_format(SQL, returnString, sizeof returnString, "update `personalvehicles` set `Tunning` = '%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d' where `ID` = '%d';", carInfo[vehicleDBId][cMod][0], carInfo[vehicleDBId][cMod][1], carInfo[vehicleDBId][cMod][2], carInfo[vehicleDBId][cMod][3], carInfo[vehicleDBId][cMod][4], carInfo[vehicleDBId][cMod][5], carInfo[vehicleDBId][cMod][6], carInfo[vehicleDBId][cMod][7], carInfo[vehicleDBId][cMod][8], carInfo[vehicleDBId][cMod][9], carInfo[vehicleDBId][cMod][10], carInfo[vehicleDBId][cMod][11], carInfo[vehicleDBId][cMod][12], carInfo[vehicleDBId][cMod][13], carInfo[vehicleDBId][cMod][14], carInfo[vehicleDBId][cMod][15], carInfo[vehicleDBId][cMod][16], vehicleDBId);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

hook OnVehicleMod(playerid, vehicleid, componentid)
{
    if(VehicleSQL[vehicleid] != 0)
    {
        saveComponent(vehicleid, componentid);
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
    if(VehicleSQL[vehicleid])
    {
        carInfo[VehicleSQL[vehicleid]][cPaintjob] = paintjobid;

        mysql_format(SQL, returnString, sizeof returnString, "update `personalvehicles` set `paintJob` = '%d' where `id` = '%d';", carInfo[VehicleSQL[vehicleid]][cPaintjob], VehicleSQL[vehicleid]);
        mysql_tquery(SQL, returnString, "", "");
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

/*GetPCars(playerid)
{
    new queryString[256];
    
    mysql_format(SQL, queryString, sizeof queryString, "select * from `personalvehicles` where `Owner` = '%s';", playerInfo[playerid][pName]);
    mysql_tquery(SQL, queryString, "returnPVehicles", "s", playerInfo[playerid][pName]);
    return true;
}

function:returnPVehicles()
    return cache_num_rows();*/

GetPCars(playerid)
{
    new Cache: queryCars, totalCars, queryString[256];

    mysql_format(SQL, queryString, sizeof queryString, "select * from `personalvehicles` where `Owner` = '%s';", playerInfo[playerid][pName]);
    queryCars = mysql_query(SQL, queryString);
    totalCars = cache_num_rows();
    
    cache_delete(queryCars);
    return totalCars;
}

ModVehicle(vehicleid)
{
    new vehicleDBId = VehicleSQL[vehicleid];
    for(new i; i < 17; i++)
    {
        if(carInfo[vehicleDBId][cMod][i])
            AddVehicleComponent(vehicleid, carInfo[vehicleDBId][cMod][i]);
    }
    return true;
}

validVIPVehicle(vehicleModel)
{
    switch(vehicleModel)
    {
        case 411, 541, 560, 470, 439, 451:
            return true;
    }
    return false;
}

function:DestroyPersonalVehicle(vehicleGameID, vehicleDBId)
{
    mysql_format(SQL, returnString, sizeof returnString, "update `personalvehicles` set `KM` = '%f', `Fuel` = '%d', `posX` = '%f', `posY` = '%f', `posZ` = '%f', `posA` = '%f', `Insurance` = '%d', `InsurancePrice` = '%d' where `id` = '%d';", carInfo[vehicleDBId][cKM], carInfo[vehicleDBId][cFuel], carInfo[vehicleDBId][cPosition][0], carInfo[vehicleDBId][cPosition][1], carInfo[vehicleDBId][cPosition][2], carInfo[vehicleDBId][cPosition][3], carInfo[vehicleDBId][cInsurance], carInfo[vehicleDBId][cInsurancePrice], vehicleDBId);
    mysql_tquery(SQL, returnString, "", "");

    DestroyDynamicObject(VIPObject[vehicleGameID]);

    VehicleSQL[vehicleGameID] = 0;
    DestroyVehicle(vehicleGameID);
    return true;
}

stock GetVehicleSpawnedState(vehicleid)
{
    foreach(new i : vehicleType<VEH_TYPE_PERSONAL>)
    {
        if(VehicleSQL[i] == vehicleid)
            return true;
    }
    return false;
}

GetVehicleGameID(dbid)
{
    foreach(new vehicleid : vehicleType<VEH_TYPE_PERSONAL>)
    {
        if(VehicleSQL[vehicleid] == dbid) return vehicleid;
    }
    return false;
}

SpawnPlayerVehicle(playerid, vehicleDBId)
{
    if(!GetVehicleSpawnedState(vehicleDBId))
    {
        new vehicleGameID = AddStaticVehicleEx(carInfo[vehicleDBId][cModel], carInfo[vehicleDBId][cPosition][0], carInfo[vehicleDBId][cPosition][1], carInfo[vehicleDBId][cPosition][2], carInfo[vehicleDBId][cPosition][3], carInfo[vehicleDBId][cColor][0], carInfo[vehicleDBId][cColor][1], -1, 0, VEH_TYPE_PERSONAL);
        VehicleSQL[vehicleGameID] = vehicleDBId;

        ChangeVehiclePaintjob(vehicleGameID, carInfo[vehicleDBId][cPaintjob]);
        SetVehicleNumberPlate(vehicleGameID, carInfo[vehicleDBId][cPlate]);

        SetVehicleHealth(vehicleGameID, 999.9);
        SetVehicleParamsForPlayer(vehicleGameID, playerid, 0, 0);

        vehicleInfo[vehicleGameID][vehEngine] = 0;
        carInfo[vehicleDBId][cSpawnTime] = gettime() + MAX_VEHICLE_SPAWN_TIME;
        vehicleInfo[vehicleGameID][vehFuel] = carInfo[vehicleDBId][cFuel]; 

        SetVehicleVirtualWorld(vehicleGameID, carInfo[vehicleDBId][cVirtual]);
        ModVehicle(vehicleGameID);
        attachVIPTextToVehicle(vehicleGameID);
    }
    else
    {
        new vehicleGameID = GetVehicleGameID(vehicleDBId);
        if(IsVehicleOccupied(vehicleGameID))
            return sendError(playerid,"Your car is in use!");
        
        SetVehicleToRespawn(vehicleGameID);

        ChangeVehiclePaintjob(vehicleGameID, carInfo[vehicleDBId][cPaintjob]);
        SetVehicleNumberPlate(vehicleGameID, carInfo[vehicleDBId][cPlate]);

        SetVehicleHealth(vehicleGameID, 999.9);
        SetVehicleParamsForPlayer(vehicleGameID, playerid, 0, 0);

        vehicleInfo[vehicleGameID][vehEngine] = 0;
        carInfo[vehicleDBId][cSpawnTime] = gettime() + MAX_VEHICLE_SPAWN_TIME;

        attachVIPTextToVehicle(vehicleGameID);

        va_SendClientMessage(playerid, COLOR_WHITE, "Vehiculul tau: model - %s (ID: %d) a fost respawnat.", aVehicleNames[carInfo[vehicleDBId][cModel] - 400], vehicleGameID);
    }
    return true;
}

task update_vspawn_status[15000]()
{
    foreach(new vehicleid : vehicleType<VEH_TYPE_PERSONAL>)
    {
        new vehicleDBId = VehicleSQL[vehicleid];
        if(carInfo[vehicleDBId][cSpawnTime] < gettime() && !IsVehicleOccupied(vehicleid))
        {
            DestroyPersonalVehicle(vehicleid, vehicleDBId);

            break;
        }
        else if(IsVehicleOccupied(vehicleid))
            carInfo[vehicleDBId][cSpawnTime] = gettime() + MAX_VEHICLE_SPAWN_TIME;
    }
}

YCMD:checkv(playerid, params[], help)
{
    check_admin

    new returnPlayer;
    if(sscanf(params, "u", returnPlayer))
        return sendSyntaxMessage(playerid, "/checkv [playername/playerid]");

    if(!IsPlayerConnected(returnPlayer))
        return sendError(playerid,"Invalid player.");

    mysql_format(SQL, returnString, sizeof returnString, "select * from `personalvehicles` where `Owner` = '%s' order by `id` asc;", GetDBName(returnPlayer));
    mysql_tquery(SQL, returnString, "WhenAdminCheckVehicles", "i", playerid);
    return true;
}
function:WhenAdminCheckVehicles(playerid)
{
    if(!cache_num_rows())
        return SendClientMessage(playerid, COLOR_GREY, "This player don't have any personal vehicles.");

    new returnVehicles, vehicleString[2048];

    while(returnVehicles < cache_num_rows())
    {
        static vehicleDBModel, vehicleDBId;

        cache_get_value_name_int(returnVehicles, "Model", vehicleDBModel);
        cache_get_value_name_int(returnVehicles, "ID", vehicleDBId);

        format(vehicleString, sizeof vehicleString, "%s(%s){ffffff} %s(DB:%d)\n", vehicleString, GetVehicleSpawnedState(vehicleDBId) != 0 ? ("{0C370A}[spawned]") : ("{ff0000}[despawned]"), aVehicleNames[vehicleDBModel - 400], vehicleDBId);

        playerStaticVar[playerid][pDialogItems][returnVehicles] = vehicleDBId;
        ++ returnVehicles;
    }
    Dialog_Show(playerid, 0, DIALOG_STYLE_LIST, "Personal Cars", vehicleString, "Select", "Exit");
    return true;
}

YCMD:towcar(playerid, params[], help)
{
    mysql_format(SQL, returnString, sizeof returnString, "select * from `personalvehicles` where `Owner` = '%s' order by `id` asc;", GetDBName(playerid));
    mysql_tquery(SQL, returnString, "show_tow_vehicles", "i", playerid);
    return true;
}

function:show_tow_vehicles(playerid)
{
    if(!cache_num_rows())
        return SendClientMessage(playerid, COLOR_GREY, "You don't have any personal vehicles.");
    
    new returnVehicles, vehicleString[2048];

    while(returnVehicles < cache_num_rows())
    {
        static vehicleDBModel, vehicleDBId;

        cache_get_value_name_int(returnVehicles, "Model", vehicleDBModel);
        cache_get_value_name_int(returnVehicles, "ID", vehicleDBId);

        format(vehicleString, sizeof vehicleString, "%s%s[%s{ffffff}]\n", vehicleString, aVehicleNames[vehicleDBModel - 400], GetVehicleSpawnedState(vehicleDBId) != 0 ? ("{0C370A}[spawned]") : ("{ff0000}[despawned]"));

        playerStaticVar[playerid][pDialogItems][returnVehicles] = vehicleDBId;
        ++ returnVehicles;
    }
    Dialog_Show(playerid, DIALOG_TOWCAR, DIALOG_STYLE_LIST, "personal vehicles", vehicleString, "select", "exit");
    toSpawnCar[playerid] = 0;
    return true;
}

Dialog:DIALOG_TOWCAR(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    new vehicleDBId = playerStaticVar[playerid][pDialogItems][listitem];
    return SpawnPlayerVehicle(playerid, vehicleDBId);
}
function:SlapPlayer(playerid) 
{
    TogglePlayerControllable(playerid, 0);

    new 
        Float: Pos[3];

    GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);
    SetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]+2);
    
    TogglePlayerControllable(playerid, 1);
    PlayerPlaySound(playerid, 1190, 0, 0, 10.0);
    return 1; 
}
YCMD:eject(playerid, params[], help) 
{
    
    if(!IsPlayerInAnyVehicle(playerid)) return sendError(playerid,"Trebuie sa fii intr-un vehicul pentru a folosi aceasta comanda.");
    new id,string[128],giveplayer[25];
    if(GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) return SendClientMessage(playerid,-1, "You can only eject people as the driver.");
    if(sscanf(params, "u",id)) return sendSyntaxMessage(playerid, "/eject <playerid/name>");
    if(!IsPlayerConnected(id) || id == INVALID_PLAYER_ID) return sendError(playerid,"Acel player nu este conectat.");
    new test = GetPlayerVehicleID(playerid);
    if(playerStaticVar[id][playerCuffed] > 0) return sendError(playerid,"Acel player e legat.");
    if(id == playerid) return sendError(playerid,"Nu iti poti da eject singur.");
    if(IsPlayerInVehicle(id,test) || GetPlayerSurfingVehicleID(id) == test) {
        if(GetPlayerSurfingVehicleID(id) == test) SlapPlayer(id);
        new PName[MAX_PLAYER_NAME];
        GetPlayerName(playerid,PName,sizeof(PName));
        GetPlayerName(id, giveplayer, sizeof(giveplayer));
        format(string, sizeof(string), "%s has thrown %s out of their vehicle.",GetName(playerid),GetName(id));                
        ProxDetector(15.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
        format(string, sizeof(string), "You have thrown out %s from vehicle!", giveplayer);
        SendClientMessage(playerid, COLOR_LIGHTBLUE, string);
        format(string, sizeof(string), "You have been thrown out of vehicle by %s !", PName);
        SendClientMessage(id, COLOR_LIGHTBLUE, string);
        
        RemovePlayerFromVehicle(id);
    }
    return 1; }
YCMD:v(playerid, params[], help)
{
    check_queries

    mysql_format(SQL, returnString, sizeof returnString, "select * from `personalvehicles` where `Owner` = '%s' order by `id` asc;", GetDBName(playerid));
    mysql_tquery(SQL, returnString, "show_personal_vehicles", "i", playerid);
    return true;
}

function:show_personal_vehicles(playerid)
{
    if(!cache_num_rows())
        return sendError(playerid,"You don't have any personal vehicles.");

    new returnVehicles, vehicleString[2048];
    vehicleString = "Vehicul\tStatus\tID\tTimp despawnare\n";

    toSpawnCar[playerid] = 0;
    while(returnVehicles < cache_num_rows())
    {
        static vehicleDBModel, vehicleDBId;
        cache_get_value_name_int(returnVehicles, "Model", vehicleDBModel),
        cache_get_value_name_int(returnVehicles, "ID", vehicleDBId);

        new vehicleGameID = GetVehicleGameID(vehicleDBId);

        format(vehicleString, sizeof vehicleString, "%s%s\t%s\t%s\t%s\n", vehicleString, aVehicleNames[vehicleDBModel - 400], GetVehicleSpawnedState(vehicleDBId) != 0 ? ("{0C370A}[spawned]") : ("{ff0000}[despawned]"), (vehicleGameID ? va_return("%d", vehicleGameID) : ("not spawned")), vehicleGameID == 0 ? ("not spawned") : IsVehicleOccupied(vehicleGameID) ? ("in use") : carInfo[vehicleDBId][cSpawnTime] - gettime() <= 20 ? ("soon") : iTimestampDate(carInfo[vehicleDBId][cSpawnTime] - gettime()));

        playerStaticVar[playerid][pDialogItems][returnVehicles] = vehicleDBId;
        returnVehicles ++;
    }
    format(vehicleString, sizeof vehicleString, "%s\nSpawn all vehicles", vehicleString);

    return Dialog_Show(playerid, DIALOG_CARMANAGE, DIALOG_STYLE_TABLIST_HEADERS, "vehicule personale", vehicleString, "Selecteaza", "Iesi");
}

Dialog:DIALOG_CARMANAGE(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    new playerCars = GetPCars(playerid);
    if(listitem >= playerCars)
    {
        for(new i; i < playerCars; i++)
        {
            SpawnPlayerVehicle(playerid, playerStaticVar[playerid][pDialogItems][i]);
        }
    }
    else
    {
        toSpawnCar[playerid] = playerStaticVar[playerid][pDialogItems][listitem];
        Dialog_Show(playerid, DIALOG_CARPANEL, DIALOG_STYLE_LIST, "{FFFFFF}Car Panel", "Vehicle Info\nTow vehicle(/park)\nTow vehicle(last known location)\nChange vehicle plate\nFind vehicle\nClear KM and Days - {fff000}20 premium points{FFFFFF}\nUnstuck vehicle\nUpgrade to VIP Vehicle\nBuy Insurance\nSell vehicle (to Dealership)", "Select", "Cancel");
    }
    return true;
}
Dialog:DIALOG_CARPANEL(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    new vehicleDBId = toSpawnCar[playerid];
    switch(listitem)
    {
        case 0:
        {
            format(returnString, sizeof returnString, "Vehicle Model: %s\nDistance Traveled: %.0f kilometers\nVehicle Age: %d days\nVehicle VIP: %s\nTop speed: %d km/h", aVehicleNames[carInfo[vehicleDBId][cModel] - 400], carInfo[vehicleDBId][cKM], getDaysFromTimestamp(carInfo[vehicleDBId][cDays]), carInfo[vehicleDBId][cVIP] ? ("Yes") : ("No"), GetVehicleMaxSpeed(carInfo[vehicleDBId][cModel]));
            Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2"{FFFFFF} cars", returnString, "Ok", "");
        }
        case 1:
        {
            if(!GetVehicleSpawnedState(vehicleDBId))
                SendClientMessage(playerid, -1, "Vehiculul tau a fost spawnat pe server!");

            SpawnPlayerVehicle(playerid, vehicleDBId);
        }
        case 2:
        {
            if(!GetVehicleSpawnedState(vehicleDBId))
            {
                new vehicleGameID;
                if(!carInfo[vehicleDBId][cLastPos][0] && !carInfo[vehicleDBId][cLastPos][1] && !carInfo[vehicleDBId][cLastPos][2])
                {
                    SendClientMessage(playerid, COLOR_GREY, "[ANTI ABUZ] Vehiculul tau a fost spawnat la locatia sa nativa, deoarece acesta a explodat.");
                    vehicleGameID = AddStaticVehicleEx(carInfo[vehicleDBId][cModel], carInfo[vehicleDBId][cPosition][0], carInfo[vehicleDBId][cPosition][1], carInfo[vehicleDBId][cPosition][2], carInfo[vehicleDBId][cPosition][3], carInfo[vehicleDBId][cColor][0], carInfo[vehicleDBId][cColor][1], -1, 0, VEH_TYPE_PERSONAL);
                }
                else
                {
                    vehicleGameID = AddStaticVehicleEx(carInfo[vehicleDBId][cModel], carInfo[vehicleDBId][cPosition][0], carInfo[vehicleDBId][cPosition][1], carInfo[vehicleDBId][cPosition][2], carInfo[vehicleDBId][cPosition][3], carInfo[vehicleDBId][cColor][0], carInfo[vehicleDBId][cColor][1], -1, 0, VEH_TYPE_PERSONAL);
                }
                VehicleSQL[vehicleGameID] = vehicleDBId;

                ChangeVehiclePaintjob(vehicleGameID, carInfo[vehicleDBId][cPaintjob]);
                SetVehicleNumberPlate(vehicleGameID, carInfo[vehicleDBId][cPlate]);

                SetVehicleHealth(vehicleGameID, 999.9);
                SetVehicleParamsForPlayer(vehicleGameID, playerid, 0, 0);

                vehicleInfo[vehicleGameID][vehEngine] = 0;
                carInfo[vehicleDBId][cSpawnTime] = gettime() + MAX_VEHICLE_SPAWN_TIME;
                vehicleInfo[vehicleGameID][vehFuel] = carInfo[vehicleDBId][cFuel]; 

                SetVehicleVirtualWorld(vehicleGameID, carInfo[vehicleDBId][cVirtual]);
                ModVehicle(vehicleGameID);
                attachVIPTextToVehicle(vehicleGameID);
            }
            else
            {
                new vehicleGameID = GetVehicleGameID(vehicleDBId);
                if(IsVehicleOccupied(vehicleGameID))
                    return sendError(playerid,"Your car is in use!");
                
                SetVehicleToRespawn(vehicleGameID);
                SetVehicleHealth(vehicleGameID, 999.9);

                if(!carInfo[vehicleDBId][cLastPos][0] && !carInfo[vehicleDBId][cLastPos][1] && !carInfo[vehicleDBId][cLastPos][2])
                {
                    SendClientMessage(playerid, COLOR_GREY, "[ANTI ABUZ] Vehiculul tau a fost spawnat la locatia sa nativa, deoarece acesta a explodat.");
                    SetVehiclePos(vehicleGameID, carInfo[vehicleDBId][cPosition][0], carInfo[vehicleDBId][cPosition][1], carInfo[vehicleDBId][cPosition][2]);
                }
                else
                {
                    va_SendClientMessage(playerid, COLOR_WHITE, "Vehiculul tau: model - %s (ID: %d) a fost respawnat in ultima locatie cunoscuta.", aVehicleNames[carInfo[vehicleDBId][cModel] - 400], vehicleGameID);
                    SetVehiclePos(vehicleGameID, carInfo[vehicleDBId][cLastPos][0], carInfo[vehicleDBId][cLastPos][1], carInfo[vehicleDBId][cLastPos][2]);
                }
                SetVehicleParamsForPlayer(vehicleGameID, playerid, 0, 0);

                vehicleInfo[vehicleGameID][vehEngine] = 0;
                carInfo[vehicleDBId][cSpawnTime] = gettime() + MAX_VEHICLE_SPAWN_TIME;
                vehicleInfo[vehicleGameID][vehFuel] = carInfo[vehicleDBId][cFuel]; 

                attachVIPTextToVehicle(vehicleGameID);
            }
        }
        case 3:
        {
            if(!GetVehicleSpawnedState(vehicleDBId))
                return SendClientMessage(playerid, COLOR_GREY, "Vehiculul tau nu este spawnat!");

            Dialog_Show(playerid, DIALOG_CARPLATE, DIALOG_STYLE_INPUT, "{FFFFFF}Car Plate", "{FFFFFF}Introduceti numarul de inmatriculare dorit:", "Introdu", "Anuleaza");
        }
        case 4:
        {
            if(!GetVehicleSpawnedState(vehicleDBId))
                return SendClientMessage(playerid, COLOR_GREY, "Vehiculul tau nu este spawnat!");

            if(playerStaticVar[playerid][pCheckpoint])
                return showActiveCheckpointDialog(playerid);

            new Float: vPos[3];
            GetVehiclePos(GetVehicleGameID(vehicleDBId), vPos[0], vPos[1], vPos[2]);
            
            SetPlayerCheckpoint(playerid, vPos[0], vPos[1], vPos[2], 4);
            playerStaticVar[playerid][pCheckpoint] = 1;
        }
        case 5:
        {
            SendClientMessage(playerid, -1, "Optiune scoasa!");
        }
        case 6:
        {
            new vehicleGameID = GetVehicleGameID(vehicleDBId);

            if(IsVehicleOccupied(vehicleGameID))
                return sendError(playerid,"Your car is in use!");

            Dialog_Show(playerid, DIALOG_CARUNSTUCK, DIALOG_STYLE_MSGBOX, "Unstuck vehicle", "Aceasta optiune a fost creata pentru cei ce nu-si pot gasi vehiculul folosind /findcar\nFolosind aceasta optiune vei avea masina spawnata intr-o parcare din LV\n\nEsti sigur ca doresti asta?", "Da", "Nu");
        }
        case 7:
        {
            if(!GetVehicleSpawnedState(vehicleDBId))
                return sendError(playerid,"Your vehicle needs to be spawned to use this menu option.");

            if(!validVIPVehicle(carInfo[vehicleDBId][cModel]))
                return sendError(playerid,"Poti pune VIP doar pe Infernus, Bullet, Sultan, Patriot, Stallion, Turismo.");

            if(carInfo[vehicleDBId][cVIP])
                return sendError(playerid,"Acest vehicul este deja VIP.");

            format(returnString, sizeof returnString, "Esti sigur ca doresti sa upgradezi vehiculului %s (model: %d) la unul de tip VIP?\nCostul acestui upgrade este de 300 premium points.", aVehicleNames[carInfo[vehicleDBId][cModel] - 400], carInfo[vehicleDBId][cModel]);
            Dialog_Show(playerid, DIALOG_VIPUPGRADE, DIALOG_STYLE_MSGBOX, "Upgrade to VIP Vehicle:", returnString, "Yes", "No");
            
        }
        case 8:
        {
            if(!GetVehicleSpawnedState(vehicleDBId))
                return sendError(playerid,"Your vehicle isn't spawned.");

            if(carInfo[vehicleDBId][cInsurance] >= 5)
                return SendClientMessage(playerid, COLOR_GREY, "Masina ta deja are 5 puncte de asigurare!");

            format(returnString, sizeof returnString, "Introdu cate puncte de asigurare doresti sa cumperi in casuta de mai jos.\nUn punct de asigurare costa $%s.\nUn vehicul poate avea maxim 5 puncte de asigurare.\nVehiculul tau are %d/5 puncte de asigurare.", FormatNumber(carInfo[vehicleDBId][cInsurancePrice]), carInfo[vehicleDBId][cInsurance]);
            Dialog_Show(playerid, DIALOG_BUYCARINS, DIALOG_STYLE_INPUT, "Plateste Asigurarea", returnString, "Ok", "Cancel");
        }
        case 9:
        {
            if(!GetVehicleSpawnedState(vehicleDBId))
                return sendError(playerid,"Your vehicle needs to be spawned to use this menu option.");
        
            mysql_format(SQL, returnString, sizeof returnString, "select * from `dealership` where `ds_vModel` = '%d';", carInfo[vehicleDBId][cModel]);
            mysql_tquery(SQL, returnString, "WhenPlayerSellVehicleToDS", "ii", playerid, vehicleDBId);
        }
    }
    return true;
}

Dialog:DIALOG_BUYCARINS(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    new carins = strval(inputtext), vehicleDBId = toSpawnCar[playerid];
    if(carins <= 0)
        return true;

    if(carInfo[vehicleDBId][cInsurance] + carins > 5)
        return SendClientMessage(playerid, COLOR_GREY, "Nu puteti cumpara atatea puncte de asigurare! Un vehicul poate avea maxim 5 puncte de asigurare.");

    va_SendClientMessage(playerid, -1, "You have bought %d insurance points for your vehicle.", carins);

    carInfo[vehicleDBId][cInsurance] += carins;
    substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, carins * carInfo[vehicleDBId][cInsurancePrice]);
    return true;
}

Dialog:DIALOG_PAYCARINS(playerid, response, listitem, inputtext[])
{
    new vehicleDBId = VehicleSQL[GetPlayerVehicleID(playerid)];

    if(!response || GetMoney(playerid) < carInfo[vehicleDBId][cInsurancePrice] && GetMilliards(playerid) < 1)
    {
        SendClientMessage(playerid, COLOR_GREY, "Nu puteti conduce acest vehicul fara a ii plati asigurarea.");
        return RemovePlayerFromVehicle(playerid);
    }

    va_SendClientMessage(playerid, -1, "You have bought an insurance point for your vehicle. You have paid $%s.", FormatNumber(carInfo[vehicleDBId][cInsurancePrice]));
    carInfo[vehicleDBId][cInsurance] += 1;

    substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, carInfo[vehicleDBId][cInsurancePrice]);

    return true;
}

Dialog:DIALOG_VIPUPGRADE(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    new vehicleDBId = toSpawnCar[playerid], vehicleGameID = GetVehicleGameID(vehicleDBId);

    va_SendClientMessage(playerid, -1, "Acum vehiculul tau de tip %s este unul de tip VIP. Foloseste /vipname.", aVehicleNames[carInfo[vehicleDBId][cModel] - 400]);
    carInfo[vehicleDBId][cVIP] = 1;

    new Float: x, Float:y, Float:z;
    GetVehiclePos(vehicleGameID, x, y, z);
    GetVehicleObjectPositionOffset(GetPlayerVehicleID(playerid), x, y, z, 0.0, 0.0, 0.0, carInfo[vehicleDBId][cVIPPos][0], carInfo[vehicleDBId][cVIPPos][1], carInfo[vehicleDBId][cVIPPos][2], carInfo[vehicleDBId][cVIPPos][3], carInfo[vehicleDBId][cVIPPos][4], carInfo[vehicleDBId][cVIPPos][5]);

    VIPObject[vehicleGameID] = CreateDynamicObject(19327, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    SetDynamicObjectMaterialText(VIPObject[vehicleGameID], 0, "{ffffff}vip veh", 140, "Arial", 65, 0, -1, 0, 1);

    // reseting rotation

    carInfo[vehicleDBId][cVIPPos][3] = 0.0;
    carInfo[vehicleDBId][cVIPPos][4] = 0.0;
    carInfo[vehicleDBId][cVIPPos][5] = 0.0;

    AttachDynamicObjectToVehicle(VIPObject[vehicleGameID], vehicleGameID, carInfo[vehicleDBId][cVIPPos][0], carInfo[vehicleDBId][cVIPPos][1], carInfo[vehicleDBId][cVIPPos][2], carInfo[vehicleDBId][cVIPPos][3], carInfo[vehicleDBId][cVIPPos][4], carInfo[vehicleDBId][cVIPPos][5]);  

    mysql_format(SQL, returnString, sizeof returnString, "update `personalvehicles` set `VIP` = '1' where `id` = '%d';", vehicleDBId);
    mysql_tquery(SQL, returnString, "", "");

    return true;
}

function:WhenPlayerSellVehicleToDS(playerid, vehicleDBId)
{
    static carValue = 0, carPriceSell;
    if(cache_num_rows())
        cache_get_value_name_int(0, "ds_vPrice", carPriceSell), carValue = carPriceSell / 2;

    SetPVarInt(playerid, "vehicle_sell_value", carValue);

    format(returnString, sizeof returnString, "Esti sigur ca vrei sa-ti vinzi vehiculul %s (model:%d) cu %d zile, %.0f KM%s?\nVei primi suma de $%s pentru vehiculul tau.", aVehicleNames[carInfo[vehicleDBId][cModel] - 400], carInfo[vehicleDBId][cModel], getDaysFromTimestamp(carInfo[vehicleDBId][cDays]), carInfo[vehicleDBId][cKM], carInfo[vehicleDBId][cVIP] ? (", cu VIP") : (""), FormatNumber(carValue));
    Dialog_Show(playerid, DIALOG_SELLCAR_TODS, DIALOG_STYLE_MSGBOX, "Sell vehicle", returnString, "Sell", "Cancel");
    return true;
}

Dialog:DIALOG_SELLCAR_TODS(playerid, response, listitem, inputtext[])
{
    if(!response)
        return DeletePVar(playerid, "vehicle_sell_value");

    new vehicleDBId = toSpawnCar[playerid], carValue = GetPVarInt(playerid, "vehicle_sell_value");

    va_SendClientMessage(playerid, -1, "You sold your car %s to the Dealership for $%s!", aVehicleNames[carInfo[vehicleDBId][cModel] - 400], FormatNumber(carValue));
    addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, carValue, false);


    DestroyPersonalVehicle(GetVehicleGameID(vehicleDBId), vehicleDBId);
    DeletePVar(playerid, "vehicle_sell_value");

    insertPlayerLog(playerInfo[playerid][pSQLID], 0, "%s [returnPlayer: %d] sold vehicle %s [carid: %d] to Dealership for $%s.", playerInfo[playerid][pName], playerInfo[playerid][pSQLID], aVehicleNames[carInfo[vehicleDBId][cModel] - 400], vehicleDBId, FormatNumber(carValue));
    sendStaffMessage(STAFF_TYPE_ADMIN, 0xFFC266AA, "%s[id %d] sold his %s [car %d] to Dealership for %s$.",  playerInfo[playerid][pName], playerInfo[playerid][pSQLID], aVehicleNames[carInfo[vehicleDBId][cModel] - 400], vehicleDBId, FormatNumber(carValue, ","));

    mysql_format(SQL, returnString, sizeof returnString, "delete from `personalvehicles` where `id` = '%d';", vehicleDBId);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

Dialog:DIALOG_CARUNSTUCK(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    new vehicleDBId = toSpawnCar[playerid], Float:pos[3];
    SendClientMessage(playerid, COLOR_YELLOW, "Masina ta a fost spawnata intr-o parcare din LV. Mergi la checkpoint pentru a o gasi.");

    setVehicleRandomLocation(random(27), pos[0], pos[1], pos[2]);
    if(GetVehicleSpawnedState(vehicleDBId))
    {
        SetPlayerCheckpoint(playerid, pos[0], pos[1], pos[2], 4.0);
        SetPVarInt(playerid, "checkpoint_type", 1);

        SetVehiclePos(GetVehicleGameID(vehicleDBId), pos[0], pos[1], pos[2]);
        SetVehicleZAngle(GetVehicleGameID(vehicleDBId), 90.0);
        
        for(new i; i < 3; i ++)
            carInfo[vehicleDBId][cPosition][i] = pos[i];
    }
    mysql_format(SQL, returnString, sizeof returnString, "update `personalvehicles` set `posX` = '%f', `posY` = '%f', `posZ` = '%f', `posA` = '90.0' where `id` = '%d';", pos[0], pos[1], pos[2], vehicleDBId);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

Dialog:DIALOG_CARPLATE(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    if(strlen(inputtext) > 12)
        return sendError(playerid,"Invalid carplate size.");

    new vehicleDBId = toSpawnCar[playerid], vehicleGameID = GetVehicleGameID(vehicleDBId);
    va_SendClientMessage(playerid, -1, "You changed your vehicle plate text to: %s", inputtext);
 
    strmid(carInfo[vehicleDBId][cPlate], inputtext, 0, strlen(inputtext), 999);
    SetVehicleNumberPlate(vehicleGameID, inputtext);

    mysql_format(SQL, returnString, sizeof returnString, "update `personalvehicles` set `Plate` = '%s' where `id` = '%d';", carInfo[vehicleDBId][cPlate], vehicleDBId);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

hook OnVehicleDeath(vehicleid)
{
    vehicleInfo[vehicleid][vehEngine] = 0;
    foreach(new i : vehicleType<VEH_TYPE_PERSONAL>)
    {
        if(i != vehicleid)
            continue;

        new vehicleDBId = VehicleSQL[vehicleid];
        if(carInfo[vehicleDBId][cInsurance] > 0)
        {
            carInfo[vehicleDBId][cLastPos][0] = 0.0;
            carInfo[vehicleDBId][cLastPos][1] = 0.0;
            carInfo[vehicleDBId][cLastPos][2] = 0.0;

            carInfo[vehicleDBId][cInsurance] --;
            carInfo[vehicleDBId][cInsurancePrice] += 150;
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnVehicleStreamIn(vehicleid, forplayerid)
{
    new vehicleDBId = VehicleSQL[vehicleid];
    if(VehicleSQL[vehicleid])
    {
        new engine, lights, alarm, doors, bonnet, boot, objective;
        GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
        SetVehicleParamsEx(vehicleid, engine, lights, alarm, carInfo[vehicleDBId][cLocked], bonnet, boot, objective);

        if(strmatch(playerInfo[forplayerid][pName], carInfo[vehicleDBId][cOwner]))
            SetVehicleParamsForPlayer(vehicleid, forplayerid, 0, 0);
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnVehicleSpawn(vehicleid)
{
    new engine, lights, alarm, doors, bonnet, boot, objective, vehicleDBId = VehicleSQL[vehicleid];
    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
    SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);

    if(!VehicleSQL[vehicleid])
    {
        vehicleInfo[vehicleid][vehFuel] = 100;
    }
    else
    {
        GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
        SetVehicleParamsEx(vehicleid, engine, lights, alarm, carInfo[vehicleDBId][cLocked] ? 0 : 1, bonnet, boot, objective);

        SetVehicleNumberPlate(vehicleid,carInfo[vehicleDBId][cPlate]);
        SetVehiclePos(vehicleid, carInfo[vehicleDBId][cPosition][0], carInfo[vehicleDBId][cPosition][1], carInfo[vehicleDBId][cPosition][2]);
        
        SetVehicleVirtualWorld(vehicleid, carInfo[vehicleDBId][cVirtual]);
        ModVehicle(vehicleid);

        ChangeVehicleColours(vehicleid, carInfo[VehicleSQL[vehicleid]][cColor][0], carInfo[VehicleSQL[vehicleid]][cColor][1]);
        vehicleInfo[vehicleid][vehFuel] = carInfo[vehicleDBId][cFuel];

        mysql_format(SQL, returnString, sizeof returnString, "update `personalvehicles` set `KM` = '%f', `Fuel` = '%d', `posX` = '%f', `posY` = '%f', `posZ` = '%f', `posA` = '%f' where `id` = '%d';", carInfo[vehicleDBId][cKM], carInfo[vehicleDBId][cFuel], carInfo[vehicleDBId][cPosition][0], carInfo[vehicleDBId][cPosition][1], carInfo[vehicleDBId][cPosition][2], carInfo[vehicleDBId][cPosition][3], vehicleDBId);
        mysql_tquery(SQL, returnString, "", "");
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
    if(newstate == PLAYER_STATE_DRIVER)
    {
        new vehicleDBId = VehicleSQL[GetPlayerVehicleID(playerid)];
        if(vehicleDBId)
        {
            sendSplitMessage(playerid, -1, "This %s (ID %d) is owned by %s | Age: %d days | Odometer: %.0f km | Insurance: $%s(%d points) | Colors: %d, %d", aVehicleNames[GetVehicleModel(GetPlayerVehicleID(playerid)) - 400], GetPlayerVehicleID(playerid), carInfo[vehicleDBId][cOwner], getDaysFromTimestamp(carInfo[vehicleDBId][cDays]), carInfo[vehicleDBId][cKM], FormatNumber(carInfo[vehicleDBId][cInsurancePrice]), carInfo[vehicleDBId][cInsurance], carInfo[vehicleDBId][cColor][0], carInfo[vehicleDBId][cColor][1]);
            va_SendClientMessage(playerid, -1, "Others: %s", carInfo[vehicleDBId][cVIP] ? (" {FFFF00}[VIP]") : ("No"));
            if(carInfo[vehicleDBId][cInsurance] == 0)
            {
                if(strmatch(playerInfo[playerid][pName], carInfo[vehicleDBId][cOwner]))
                {
                    format(returnString, sizeof returnString, "Your vehicle ran out of insurance points.\n\nWould you wish to buy one? (one is $%s)", FormatNumber(carInfo[vehicleDBId][cInsurancePrice]));
                    Dialog_Show(playerid, DIALOG_PAYCARINS, DIALOG_STYLE_MSGBOX, "Vehicle Insurance", returnString, "Ok", "Cancel");
                }
                else
                {
                    SendClientMessage(playerid, COLOR_GREY, "Nu puteti conduce acest vehicul deoarece proprietarul acestuia nu i-a platit asigurarea");
                    RemovePlayerFromVehicle(playerid);
                }
            }
        }
    }
    if(newstate == PLAYER_STATE_ONFOOT)
    {
        new vehicleDBId = VehicleSQL[playerStaticVar[playerid][lastVehicle]];
        if(vehicleDBId)
        {
            if(strcmp(playerInfo[playerid][pName], carInfo[vehicleDBId][cOwner], true) == 0)
                SetVehicleParamsForPlayer(playerStaticVar[playerid][lastVehicle], playerid, 0, 0);

            carInfo[vehicleDBId][cSpawnTime] = gettime() + MAX_VEHICLE_SPAWN_TIME;
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:carplate(playerid, params[], help)
{
    if(!IsPlayerInAnyVehicle(playerid))
        return sendError(playerid,"Nu te aflii intr-un vehicul.");

    new vehicleid = GetPlayerVehicleID(playerid);

    if(!VehicleSQL[vehicleid])
        return sendError(playerid,"Acest vehicul nu este unul personal.");

    if(GetMoney(playerid) < 5000 && GetMilliards(playerid) < 1)
        return sendError(playerid,"You do not have the money.");


    new plateString[12];
    if(sscanf(params, "s[12]", plateString))
        return sendSyntaxMessage(playerid, "/carplate [text]");

    va_SendClientMessage(playerid, -1, "You changed your vehicle plate text to: %s", plateString);

    strmid(carInfo[VehicleSQL[vehicleid]][cPlate], plateString, 0, strlen(plateString), 999);
    SetVehicleNumberPlate(vehicleid, plateString);

    substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 10000);


    mysql_format(SQL, returnString, sizeof returnString, "update `personalvehicles` set `Plate` = '%s' where `ID` = '%d';", carInfo[VehicleSQL[vehicleid]][cPlate], VehicleSQL[vehicleid]);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}
YCMD:swapcolors(playerid, params[], help)
{
    if(GetMoney(playerid) < 1000 && GetMilliards(playerid) < 1)
        return sendError(playerid,"You do not have the money.");

    if(!IsPlayerInAnyVehicle(playerid))
        return sendError(playerid,"Nu esti intr-un vehicul!");

    new vehicleid = GetPlayerVehicleID(playerid);

    if(!VehicleSQL[vehicleid])
        return sendError(playerid,"This isn't a personal car!");

    if(!strmatch(playerInfo[playerid][pName], carInfo[VehicleSQL[vehicleid]][cOwner]))
        return sendError(playerid,"You are not in your vehicle.");

    new newColor[2];
    newColor[0] = carInfo[VehicleSQL[vehicleid]][cColor][0];
    newColor[1] = carInfo[VehicleSQL[vehicleid]][cColor][1];

    carInfo[VehicleSQL[vehicleid]][cColor][0] = newColor[1];
    carInfo[VehicleSQL[vehicleid]][cColor][1] = newColor[0];

    substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 5000);
    SendClientMessage(playerid, -1, "Culorile au fost inversate.");
    
    ChangeVehicleColours(vehicleid, carInfo[VehicleSQL[vehicleid]][cColor][0], carInfo[VehicleSQL[vehicleid]][cColor][1]);
    return true;
}

function:WhenPlayerParkVehicle(playerid, vehicleDBId, vehicleGameID)
{
    DestroyPersonalVehicle(vehicleGameID, vehicleDBId);
    return SpawnPlayerVehicle(playerid, vehicleDBId);
}

YCMD:vipwheels(playerid, params[], help)
{
    if(!IsPlayerInAnyVehicle(playerid))
        return sendError(playerid,"Nu te afli intr-un vehicul.");

    new vehicleGameID = GetPlayerVehicleID(playerid);

    if(!VehicleSQL[GetPlayerVehicleID(playerid)])
        return SendClientMessage(playerid, COLOR_GREY, "Nu te afli intr-un vehicul personal.");

    if(!strmatch(playerInfo[playerid][pName], carInfo[VehicleSQL[vehicleGameID]][cOwner]))
        return SendClientMessage(playerid, COLOR_GREY, "Nu sunteti proprietarul vehiculului.");

    if(!carInfo[VehicleSQL[vehicleGameID]][cVIP])
        return sendError(playerid,"Acest vehicul nu este unul de tip VIP!");

    new wheelType[12];
    if(sscanf(params, "s[12]", wheelType))
    {
        sendSyntaxMessage(playerid, "/vipwheels [wheel type]");
        return SendClientMessage(playerid, COLOR_GREY, "Wheels: Switch, Dollar, Trance");
    }

    if(strcmp(wheelType, "switch", true) == 0)
    {
        AddVehicleComponent(vehicleGameID, 1080);
    }
    else if(strcmp(wheelType, "dollar", true) == 0)
    {
        AddVehicleComponent(vehicleGameID, 1083);
    }
    else if(strcmp(wheelType, "trance", true) == 0)
    {
        AddVehicleComponent(vehicleGameID, 1084);
    }
    else
        return sendError(playerid,"Acest model de roti este necunoscut!");

    SendClientMessage(playerid, -1, "Rotile vehiculului tau au fost schimbate cu succes!");
    return true;
}

YCMD:vipname(playerid, params[], help)
{
    if(!IsPlayerInAnyVehicle(playerid))
        return sendError(playerid,"You can't use this command because you're not in your vehicle.");

    new vehicleid = GetPlayerVehicleID(playerid);

    if(strcmp(playerInfo[playerid][pName], carInfo[VehicleSQL[vehicleid]][cOwner], false) == 1 || strcmp(playerInfo[playerid][pName], carInfo[VehicleSQL[vehicleid]][cOwner], true) == 0 && !carInfo[VehicleSQL[vehicleid]][cVIP])
        return sendSplitMessage(playerid, -1, "Nu poti folosi aceasta comanda deoarece nu te afli intr-un vehicul personal detinut de tine sau acest vehicul nu este unul de tip VIP.");

    if(vehicleInfo[vehicleid][vehEngine])
        return SendClientMessage(playerid, COLOR_GREY, "Motorul vehiculului tau trebuie sa fie oprit pentru accesarea acestei comenzi.");

    new vehicleDBId = VehicleSQL[vehicleid];
    format(returnString, sizeof returnString, "Option\tInfo\nText color\tcurrent: {%s}%s\nText\tcurrent: %s\nText position\nReset to default", carInfo[vehicleDBId][cVIPColor], carInfo[vehicleDBId][cVIPColor], carInfo[vehicleDBId][cVIPText]);

    Dialog_Show(playerid, DIALOG_VIPMENU, DIALOG_STYLE_TABLIST_HEADERS, "VIP Menu:", returnString, "Select", "Close");
    return true;
}

YCMD:removetunning(playerid, params[], help)
{
    new vehicleGameID = GetPlayerVehicleID(playerid);

    if(!VehicleSQL[vehicleGameID])
        return SendClientMessage(playerid, COLOR_GREY, "Nu te afli intr-un vehicul personal.");

    if(!strmatch(playerInfo[playerid][pName], carInfo[VehicleSQL[vehicleGameID]][cOwner]))
        return SendClientMessage(playerid, COLOR_GREY, "Nu sunteti proprietarul vehiculului.");

    new vehicleDBId = VehicleSQL[vehicleGameID];
    for(new i; i < 17; i++)
    {
        RemoveVehicleComponent(vehicleGameID, carInfo[vehicleDBId][cMod][i]);
        carInfo[vehicleDBId][cMod][i] = -1;
    }
    mysql_format(SQL, returnString, sizeof returnString, "update `personalvehicles` set `Tunning` = '0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0' where `ID` = '%d';", vehicleDBId);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

function:EditVIPVehicleText(playerid, vehicleGameID)
{
    new vehicleDBId = VehicleSQL[vehicleGameID], Float:newpos[6];

    GetVehicleObjectPositionWorld(GetPlayerVehicleID(playerid), carInfo[vehicleDBId][cVIPPos][0], carInfo[vehicleDBId][cVIPPos][1], carInfo[vehicleDBId][cVIPPos][2], carInfo[vehicleDBId][cVIPPos][3], carInfo[vehicleDBId][cVIPPos][4], carInfo[vehicleDBId][cVIPPos][5], newpos[0], newpos[1], newpos[2], newpos[3], newpos[4], newpos[5]);
    DestroyDynamicObject(VIPObject[vehicleGameID]);

    VIPObject[vehicleGameID] = CreateDynamicObject(19327, newpos[0], newpos[1], newpos[2], newpos[3], newpos[4], newpos[5]);
    format(returnString, sizeof returnString, "{%s}%s", carInfo[vehicleDBId][cVIPColor], carInfo[vehicleDBId][cVIPText]);
    
    SetDynamicObjectMaterialText(VIPObject[vehicleGameID], 0, returnString, 140, "Arial", 65, 0, -1, 0, 1);
    EditDynamicObject(playerid, VIPObject[vehicleGameID]);

    TogglePlayerControllable(playerid, false);
    return SetPVarInt(playerid, "edit_object_vip", 1);
}

function:attachVIPTextToVehicle(vehicleGameID)
{
    new vehicleDBId = VehicleSQL[vehicleGameID];
    if(!carInfo[vehicleDBId][cVIP])
        return true;

    format(returnString, sizeof returnString, "{%s}%s", carInfo[vehicleDBId][cVIPColor], carInfo[vehicleDBId][cVIPText]);

    if(!IsValidDynamicObject(VIPObject[vehicleGameID]))
        VIPObject[vehicleGameID] = CreateDynamicObject(19327, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    
    SetDynamicObjectMaterialText(VIPObject[vehicleGameID], 0, returnString, 140, "Arial", 65, 0, -1, 0, 1);
    AttachDynamicObjectToVehicle(VIPObject[vehicleGameID], vehicleGameID, carInfo[vehicleDBId][cVIPPos][0], carInfo[vehicleDBId][cVIPPos][1], carInfo[vehicleDBId][cVIPPos][2], carInfo[vehicleDBId][cVIPPos][3], carInfo[vehicleDBId][cVIPPos][4], carInfo[vehicleDBId][cVIPPos][5]);  
    return true;
}

hook OnPlayerEditDynObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
    if(response == EDIT_RESPONSE_FINAL)
    {
        if(GetPVarInt(playerid, "edit_object_vip"))
        {
            new vehicleGameID = GetPlayerVehicleID(playerid), vehicleDBId = VehicleSQL[vehicleGameID];

            GetVehicleObjectPositionOffset(GetPlayerVehicleID(playerid), x, y, z, rx, ry, rz, carInfo[vehicleDBId][cVIPPos][0], carInfo[vehicleDBId][cVIPPos][1], carInfo[vehicleDBId][cVIPPos][2], carInfo[vehicleDBId][cVIPPos][3], carInfo[vehicleDBId][cVIPPos][4], carInfo[vehicleDBId][cVIPPos][5]);
            AttachDynamicObjectToVehicle(objectid, vehicleGameID, carInfo[vehicleDBId][cVIPPos][0], carInfo[vehicleDBId][cVIPPos][1], carInfo[vehicleDBId][cVIPPos][2], carInfo[vehicleDBId][cVIPPos][3], carInfo[vehicleDBId][cVIPPos][4], carInfo[vehicleDBId][cVIPPos][5]);

            TogglePlayerControllable(playerid, true);

            mysql_format(SQL, returnString, sizeof returnString, "update `personalvehicles` set `VIPPos` = '%f %f %f %f %f %f' where `id` = '%d';", carInfo[vehicleDBId][cVIPPos][0], carInfo[vehicleDBId][cVIPPos][1], carInfo[vehicleDBId][cVIPPos][2], carInfo[vehicleDBId][cVIPPos][3], carInfo[vehicleDBId][cVIPPos][4], carInfo[vehicleDBId][cVIPPos][5], vehicleDBId);
            mysql_tquery(SQL, returnString, "", "");

            DeletePVar(playerid, "edit_object_vip");
        }
    }
    else if(response == EDIT_RESPONSE_CANCEL)
    {
        if(GetPVarInt(playerid, "edit_object_vip"))
        {
            new
                vehicleGameID = GetPlayerVehicleID(playerid);

            attachVIPTextToVehicle(vehicleGameID);
            TogglePlayerControllable(playerid, true);

            DeletePVar(playerid, "edit_object_vip");
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

Dialog:DIALOG_VIPMENU(playerid, response, listitem, inputtext[])
{
    if(!response)
        return DeletePVar(playerid, "vip_menu_selected");

    switch(listitem)
    {
        case 0: Dialog_Show(playerid, DIALOG_VIPMENU_ASSETS, DIALOG_STYLE_INPUT, "VIP Text Color:", "Please enter the HTML code for the text color (default FFFFFF):", "Change", "Cancel");
        case 1: Dialog_Show(playerid, DIALOG_VIPMENU_ASSETS, DIALOG_STYLE_INPUT, "VIP Text:", "Please enter your desired text (1-12 characters):", "Change", "Cancel");
        case 2:
        {
            SendClientMessage(playerid, -1, "You started to edit your vehicle text offsets. Use your mouse to do it.");
            EditVIPVehicleText(playerid, GetPlayerVehicleID(playerid));
        }
        case 3:
        {
            Dialog_Show(playerid, DIALOG_VIPMENU_ASSETS, DIALOG_STYLE_MSGBOX, "VIP Text:", "Are you sure you want to restore your vehicle's text to default settings?", "Yes", "No");
        }
    }
    if(listitem == 0 || listitem == 1 || listitem == 3)
        SetPVarInt(playerid, "vip_menu_selected", listitem);

    return true;
}

Dialog:DIALOG_VIPMENU_ASSETS(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    if(!IsPlayerInAnyVehicle(playerid))
        return true;

    new vehicleGameID = GetPlayerVehicleID(playerid), vehicleDBId = VehicleSQL[GetPlayerVehicleID(playerid)], materialString[64];

    switch(GetPVarInt(playerid, "vip_menu_selected"))
    {
        case 0:
        {
            if(strlen(inputtext) < 6 || strlen(inputtext) > 6)
                return sendError(playerid,"Invalid color.");

            for(new i; i < strlen(inputtext); ++i)
            {
                if((inputtext[i] >= 'A' && inputtext[i] <= 'F') || (inputtext[i] >= 'a' && inputtext[i] <= 'f') || (inputtext[i] >= '0' && inputtext[i] <= '9'))
                {
                    va_SendClientMessage(playerid, -1, "Server: You changed your vehicle vip text color to: {%s}%s", inputtext, inputtext);
                    format(carInfo[vehicleDBId][cVIPColor], 12, inputtext);
                    break;
                }
                else return sendError(playerid,"Cod de culoare invalid!");
            }
            format(materialString, sizeof materialString, "{%s}%s", carInfo[vehicleDBId][cVIPColor], carInfo[vehicleDBId][cVIPText]);
            SetDynamicObjectMaterialText(VIPObject[vehicleGameID], 0, materialString, 140, "Arial", 65, 0, -1, 0, 1);
        }
        case 1:
        {
            if(strlen(inputtext) < 1 || strlen(inputtext) > 12)
                return sendError(playerid,"Invalid text.");

            va_SendClientMessage(playerid, COLOR_GREY, "Your vehicle text have been changed in: {FFFFFF}%s", inputtext);
            format(carInfo[vehicleDBId][cVIPText], 14, inputtext);

            format(materialString, sizeof materialString, "{%s}%s", carInfo[vehicleDBId][cVIPColor], carInfo[vehicleDBId][cVIPText]);
            SetDynamicObjectMaterialText(VIPObject[vehicleGameID], 0, materialString, 140, "Arial", 65, 0, -1, 0, 1);
        }
        case 3:
        {
            SendClientMessage(playerid, -1, "Your vehicle text have been reseted.");

            new Float: x, Float:y, Float:z;

            GetVehiclePos(vehicleGameID, x, y, z);
            GetVehicleObjectPositionOffset(GetPlayerVehicleID(playerid), x, y, z, 0.0, 0.0, 0.0, carInfo[vehicleDBId][cVIPPos][0], carInfo[vehicleDBId][cVIPPos][1], carInfo[vehicleDBId][cVIPPos][2], carInfo[vehicleDBId][cVIPPos][3], carInfo[vehicleDBId][cVIPPos][4], carInfo[vehicleDBId][cVIPPos][5]);

            format(returnString, sizeof returnString, "{%s}%s", carInfo[vehicleDBId][cVIPColor], carInfo[vehicleDBId][cVIPText]);
            SetDynamicObjectMaterialText(VIPObject[vehicleGameID], 0, returnString, 140, "Arial", 65, 0, -1, 0, 1);

            // reseting rotation

            carInfo[vehicleDBId][cVIPPos][3] = 0.0;
            carInfo[vehicleDBId][cVIPPos][4] = 0.0;
            carInfo[vehicleDBId][cVIPPos][5] = 0.0;
            
            AttachDynamicObjectToVehicle(VIPObject[vehicleGameID], vehicleGameID, carInfo[vehicleDBId][cVIPPos][0], carInfo[vehicleDBId][cVIPPos][1], carInfo[vehicleDBId][cVIPPos][2], carInfo[vehicleDBId][cVIPPos][3], carInfo[vehicleDBId][cVIPPos][4], carInfo[vehicleDBId][cVIPPos][5]);  
        }
    }
    mysql_format(SQL, returnString, sizeof returnString, "update `personalvehicles` set `VIPText` = '%s', `VIPColor` = '%s', `VIPPos` = '%f %f %f %f %f %f' where `ID` = '%d';", carInfo[vehicleDBId][cVIPText], carInfo[vehicleDBId][cVIPColor], carInfo[vehicleDBId][cVIPPos][0], carInfo[vehicleDBId][cVIPPos][1], carInfo[vehicleDBId][cVIPPos][2], carInfo[vehicleDBId][cVIPPos][3], carInfo[vehicleDBId][cVIPPos][4], carInfo[vehicleDBId][cVIPPos][5], vehicleDBId);
    mysql_tquery(SQL, returnString, "", "");
    return DeletePVar(playerid, "vip_menu_selected");
}

//DEALERSHIP

#define MAX_DEALER_VEHICLES 150

enum ENUM_DEALER_INFORMATIONS
{
    dealer_vModel, dealer_vPrice, dealer_vStock
};
new dealerData[MAX_DEALER_VEHICLES][ENUM_DEALER_INFORMATIONS], dealerVehicles;
new Text:dealerTD[6], dealerVehicle[MAX_PLAYERS], Float:zAngle[MAX_PLAYERS];

hook loadServerSystems@0001()
{
    mysql_tquery(SQL, "select * from `dealership` order by `ds_Id` asc;", "loadServerDealership", "");
    return Y_HOOKS_CONTINUE_RETURN_1;
}

function:loadServerDealership()
{
    if(!cache_num_rows())
        return true;

    for(new i; i < cache_num_rows(); i++)
    {
        if(i > sizeof dealerData)
            break;

        static stockId;
        cache_get_value_name_int(i, "ds_Id", stockId);

        cache_get_value_name_int(i, "ds_vModel", dealerData[stockId][dealer_vModel]);
        cache_get_value_name_int(i, "ds_vPrice", dealerData[stockId][dealer_vPrice]);
        cache_get_value_name_int(i, "ds_vStock", dealerData[stockId][dealer_vStock]);
    }
    dealerVehicles = cache_num_rows();
    return printf("Dealership cars: %d", cache_num_rows());
}

insertPlayerVehicle(playerid, playerVehicleModel, extraVIP)
{
    new parkPos = random(8);

    mysql_format(SQL, returnString, sizeof returnString, "insert into `personalvehicles` (`Model`, `Owner`, `Days`, `posX`, `posY`, `posZ`, `posA`, `VIP`) values ('%d', '%s', '%d', '%f', '%f', '6.75', '133.3', '%d');", playerVehicleModel, playerInfo[playerid][pName], gettime(), (-1630.3834 - (1.2 * parkPos)), (1290.4325 + (2.7 * parkPos)), extraVIP);
    mysql_tquery(SQL, returnString, "onVehicleInserted", "ii", playerid, playerVehicleModel);
    return true;
}

function:onVehicleInserted(playerid, playerVehicleModel)
{
    mysql_format(SQL, returnString, sizeof returnString, "select * from `personalvehicles` where `ID` = '%d';", cache_insert_id());
    mysql_tquery(SQL, returnString, "loadPersonalVehicles", "");
    
    if(GetPVarInt(playerid, "in_dealer"))
        return OnShowDealership(playerid, true);
        
    return true;
}

hook OnGameModeInit@0001()
{
    dealerTD[5] = TextDrawCreate(157.294113, 302.000153, "box");
    TextDrawLetterSize(dealerTD[5], 0.000000, 7.741175);
    TextDrawTextSize(dealerTD[5], 470.000000, 0.000000);
    TextDrawAlignment(dealerTD[5], 1);
    TextDrawColour(dealerTD[5], 0xFFFFFFFF);
    TextDrawUseBox(dealerTD[5], 1);
    TextDrawBoxColour(dealerTD[5], 0x00000070);
    TextDrawSetShadow(dealerTD[5], 0);
    TextDrawSetOutline(dealerTD[5], 0);
    TextDrawBackgroundColour(dealerTD[5], 0x000000FF);
    TextDrawFont(dealerTD[5], 1);
    TextDrawSetProportional(dealerTD[5], 1);
    TextDrawSetSelectable(dealerTD[5], 0);

    dealerTD[4] = TextDrawCreate(268.353058, 299.083282, "dealership");
    TextDrawLetterSize(dealerTD[4], 0.400000, 1.600000);
    TextDrawTextSize(dealerTD[4], 1280.000000, 1280.000000);
    TextDrawAlignment(dealerTD[4], 1);
    TextDrawColour(dealerTD[4], 0xFFFFFFFF);
    TextDrawUseBox(dealerTD[4], 0);
    TextDrawBoxColour(dealerTD[4], 0x80808080);
    TextDrawSetShadow(dealerTD[4], 0);
    TextDrawSetOutline(dealerTD[4], 1);
    TextDrawBackgroundColour(dealerTD[4], 0x000000FF);
    TextDrawFont(dealerTD[4], 2);
    TextDrawSetProportional(dealerTD[4], 1);
    TextDrawSetSelectable(dealerTD[4], 0);

    dealerTD[3] = TextDrawCreate(439.586761, 345.166687, "LD_BEAT:right");
    TextDrawLetterSize(dealerTD[3], 0.000000, 0.000000);
    TextDrawTextSize(dealerTD[3], 19.000000, 17.000000);
    TextDrawAlignment(dealerTD[3], 1);
    TextDrawColour(dealerTD[3], 0xFFFFFFFF);
    TextDrawUseBox(dealerTD[3], 0);
    TextDrawBoxColour(dealerTD[3], 0x80808080);
    TextDrawSetShadow(dealerTD[3], 0);
    TextDrawSetOutline(dealerTD[3], 0);
    TextDrawBackgroundColour(dealerTD[3], 0x000000FF);
    TextDrawFont(dealerTD[3], 4);
    TextDrawSetProportional(dealerTD[3], 0);
    TextDrawSetSelectable(dealerTD[3], 1);

    dealerTD[2] = TextDrawCreate(320.061157, 345.166687, "LD_BEAT:left");
    TextDrawLetterSize(dealerTD[2], 0.000000, 0.000000);
    TextDrawTextSize(dealerTD[2], 19.000000, 17.000000);
    TextDrawAlignment(dealerTD[2], 1);
    TextDrawColour(dealerTD[2], 0xFFFFFFFF);
    TextDrawUseBox(dealerTD[2], 0);
    TextDrawBoxColour(dealerTD[2], 0x80808080);
    TextDrawSetShadow(dealerTD[2], 0);
    TextDrawSetOutline(dealerTD[2], 0);
    TextDrawBackgroundColour(dealerTD[2], 0x000000FF);
    TextDrawFont(dealerTD[2], 4);
    TextDrawSetProportional(dealerTD[2], 0);
    TextDrawSetSelectable(dealerTD[2], 1);

    dealerTD[1] = TextDrawCreate(421.235504, 301.416718, "LD_CHAT:thumbup");
    TextDrawLetterSize(dealerTD[1], 0.000000, 0.000000);
    TextDrawTextSize(dealerTD[1], 21.000000, 16.000000);
    TextDrawAlignment(dealerTD[1], 1);
    TextDrawColour(dealerTD[1], 0xFFFFFFFF);
    TextDrawUseBox(dealerTD[1], 0);
    TextDrawBoxColour(dealerTD[1], 0x80808080);
    TextDrawSetShadow(dealerTD[1], 0);
    TextDrawSetOutline(dealerTD[1], 0);
    TextDrawBackgroundColour(dealerTD[1], 0x000000FF);
    TextDrawFont(dealerTD[1], 4);
    TextDrawSetProportional(dealerTD[1], 0);
    TextDrawSetSelectable(dealerTD[1], 1);

    dealerTD[0] = TextDrawCreate(448.058868, 301.999786, "LD_CHAT:thumbdn");
    TextDrawLetterSize(dealerTD[0], 0.000000, 0.000000);
    TextDrawTextSize(dealerTD[0], 21.000000, 16.000000);
    TextDrawAlignment(dealerTD[0], 1);
    TextDrawColour(dealerTD[0], 0xFFFFFFFF);
    TextDrawUseBox(dealerTD[0], 0);
    TextDrawBoxColour(dealerTD[0], 0x80808080);
    TextDrawSetShadow(dealerTD[0], 0);
    TextDrawSetOutline(dealerTD[0], 0);
    TextDrawBackgroundColour(dealerTD[0], 0x000000FF);
    TextDrawFont(dealerTD[0], 4);
    TextDrawSetProportional(dealerTD[0], 0);
    TextDrawSetSelectable(dealerTD[0], 1);

    CreateDynamic3DTextLabel("DealerShip!\n\nType /buycar to buy a car\nYou can buy a car starting with level 3", 0x21db1aAA, 326.4294, -1517.0165, 36.0391, 25.0);
    CreateDynamicPickup(1277, 23, 326.4294, -1517.0165, 36.0391);
    CreateDynamicMapIcon(326.4294, -1517.0165, 36.0391, 55, 0, -1, -1, -1, 750.0);
    return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:givekey(playerid, params[], help) {
    check_delay

    new const vehicleid = GetPlayerVehicleID(playerid);

    if(!vehicleid) 
        return sendError(playerid, "You are not in any car.");

    if(!strmatch(playerInfo[playerid][pName], carInfo[VehicleSQL[vehicleid]][cOwner]))
        return sendError(playerid, "You are not in your personal vehicle.");

    new returnPlayer; if(sscanf(params, "u", returnPlayer))
        return sendSyntaxMessage(playerid, "/givekey [playerid/name]");

    if(!playerStaticVar[returnPlayer][playerLogged])
        return sendError(playerid, "Acel player nu este logat.");

    if(returnPlayer == playerid)
        return sendError(playerid, "Nu poti folosi aceasta comanda asupra ta.");

    if(!IsPlayerInRangeOfPlayer(playerid, returnPlayer, 5.0))
        return sendError(playerid, "Nu esti in apropierea acelui player.");

    if(playerStaticVar[returnPlayer][vehiclekey_id] != -1)
        return sendError(playerid, "Acel player are deja cheia unui vehicul personal.");

    playerStaticVar[returnPlayer][vehiclekey_id] = vehicleid;
    playerStaticVar[returnPlayer][vehiclekey_sqlid] = VehicleSQL[vehicleid];

    set_delay;

    SendClientMessage(returnPlayer, COLOR_SERVER, "KEYS >>: {ffffff}Ai primit cheia de la vehiculul %s (ID %d) detinut de %s.", aVehicleNames[carInfo[VehicleSQL[vehicleid]][cModel] - 400], VehicleSQL[vehicleid], GetName(playerid));
    SendClientMessage(playerid, COLOR_SERVER, "KEYS >>: {ffffff}%s a primit cheia de la vehiculul tau.", GetName(returnPlayer));
    return 1;
}

YCMD:throwkey(playerid, params[], help) {
    if(playerStaticVar[playerid][vehiclekey_id] == -1)
        return sendError(playerid, "Nu ai cheile unui vehicul personal.");

    playerStaticVar[playerid][vehiclekey_id] = playerStaticVar[playerid][vehiclekey_sqlid] = -1;
    
    return SendClientMessage(playerid, COLOR_YELLOW, "* You throwed successfully the personal vehicle keys you had.");
}


YCMD:gotods(playerid, params[], help)
{
    check_admin

    SetPlayerInterior(playerid, 0); 
    SetPlayerVirtualWorld(playerid, 0);

    SetPlayerPos(playerid, 326.4294, -1517.0165, 36.0391);
    SendClientMessage(playerid, -1, "You have been teleported.");
    return true;
}

YCMD:buycar(playerid, params[], help)
{
    if(GetPVarInt(playerid, "in_dealer") || isPlayerInWar(playerid))
        return true;

    if(!IsPlayerInRangeOfPoint(playerid, 7.0, 326.4294, -1517.0165, 36.0391))
        return SendClientMessage(playerid, COLOR_RED, "[!]{ffffff} Nu esti in apropierea dealership-ului.");

    if(playerInfo[playerid][pLevel] < 3)
        return SendClientMessage(playerid, COLOR_RED, "[!]{ffffff} Ai nevoie de minim nivel 3 pentru a putea cumpara un vehicul.");

    if(playerStaticVar[playerid][pDealerDelay] > gettime())
    {
        va_SendClientMessage(playerid, COLOR_GREY, "Ai cumparat un vehicul recent. Poti cumpara altul in %s.", iTimestampDate(playerStaticVar[playerid][pDealerDelay] - gettime()));
    
        if(playerInfo[playerid][pAdmin] > 3)
            SendClientMessage(playerid, COLOR_GREY, "Skipping check (admin).");

        else
            return true;
    }

    SendClientMessage(playerid, COLOR_GREEN, "Inselatoriile (tepele) nu sunt permise pe server. Pentru orice incercare de a insela un player cu bunuri/bani puteti primi ban PERMANENT.");
    SendClientMessage(playerid, COLOR_GREEN, "Impurmutarile de orice fel (inclusiv datoriile) se fac pe proprie raspundere. Nu ai nicio garantie ca ii vei recupera.");
    SendClientMessage(playerid, COLOR_GREEN, "Va recomandam sa nu va imprumutati masinile unui jucator, pentru a se evita eventualele pierderi de bunuri.");

    SetPlayerPos(playerid, -1651.9701, 1216.4083, 17.1249);
    SetPlayerCameraPos(playerid, -1651.9701, 1216.4083, 17.1249);
    SetPlayerCameraLookAt(playerid, -1660.2505, 1211.7731, 13.4021);
    zAngle[playerid] = 90.0;

    SetPlayerVirtualWorld(playerid, playerid + 1);
    return OnShowDealership(playerid);
}

OnShowDealership(playerid, bool: hide = false)
{
    if(hide == true)
    {
        for(new i; i < sizeof dealerTD; i++)
            TextDrawHideForPlayer(playerid, dealerTD[i]);

        for(new i; i < 2; i++)
            PlayerTextDrawHide(playerid, playerTextdraws[playerid][dealerPTD][i]);

        CancelSelectTextDraw(playerid);

        DeletePVar(playerid, "in_dealer");
        DeletePVar(playerid, "dealer_carId");
        SetPlayerPos(playerid, 326.4294, -1517.0165, 36.0391);
        return true;
    }
    for(new i; i < sizeof dealerTD; i++)
        TextDrawShowForPlayer(playerid, dealerTD[i]);

    for(new i; i < 2; i++)
        PlayerTextDrawShow(playerid, playerTextdraws[playerid][dealerPTD][i]);

    SelectTextDraw(playerid, 0x4287f5AA);

    SetPVarInt(playerid, "in_dealer", 1);
    SetPVarInt(playerid, "dealer_carId", 1);

    OnDealerTextDrawUpdate(playerid);
    return true;
}

OnDealerTextDrawUpdate(playerid)
{
    new ds_vStockId = GetPVarInt(playerid, "dealer_carId");

    PlayerTextDrawSetPreviewModel(playerid, playerTextdraws[playerid][dealerPTD][0], dealerData[ds_vStockId][dealer_vModel]);
    PlayerTextDrawShow(playerid, playerTextdraws[playerid][dealerPTD][0]);

    format(returnString, sizeof returnString, "Model: %s~n~Price: $%s~n~Stock: %d~n~Top speed: %d km/h", aVehicleNames[dealerData[ds_vStockId][dealer_vModel] - 400], FormatNumber(dealerData[ds_vStockId][dealer_vPrice]), dealerData[ds_vStockId][dealer_vStock], GetVehicleMaxSpeed(dealerData[ds_vStockId][dealer_vModel]));
    PlayerTextDrawSetString(playerid, playerTextdraws[playerid][dealerPTD][1], returnString);
    
    if(dealerVehicle[playerid] != INVALID_VEHICLE_ID || dealerVehicle[playerid] < 1)
        DestroyVehicle(dealerVehicle[playerid]);

    dealerVehicle[playerid] = AddStaticVehicleEx(dealerData[ds_vStockId][dealer_vModel], -1660.2505, 1211.7731, 13.4021, zAngle[playerid], 1, 1, -1);
    SetVehicleVirtualWorld(dealerVehicle[playerid], playerid + 1);
    return true;
}

hook OnPlayerClickPlayerTD(playerid, PlayerText:playertextid)
{
    if(playertextid == playerTextdraws[playerid][dealerPTD][0])
    {
        zAngle[playerid] += 10.0;
        SetVehicleZAngle(dealerVehicle[playerid], zAngle[playerid]);

        PlayerTextDrawHide(playerid, playerTextdraws[playerid][dealerPTD][0]);

        PlayerTextDrawSetPreviewRot(playerid, playerTextdraws[playerid][dealerPTD][0], 0.000000, 0.000000, zAngle[playerid], 1.000000);
        PlayerTextDrawShow(playerid, playerTextdraws[playerid][dealerPTD][0]);
        return true;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if(clickedid == Text:INVALID_TEXT_DRAW || clickedid == dealerTD[0])
    {
        if(GetPVarInt(playerid, "in_dealer"))
        {
            if(dealerVehicle[playerid] != INVALID_VEHICLE_ID)
                DestroyVehicle(dealerVehicle[playerid]);

            SetCameraBehindPlayer(playerid);
            SetPlayerVirtualWorld(playerid, 0);
            SendClientMessage(playerid, COLOR_RED, "[!]{FFFFFF} Ai iesit din Dealership!");

            return OnShowDealership(playerid, true);
        }
    }

    if(clickedid == dealerTD[2])
    {
        if(GetPVarInt(playerid, "dealer_carId") == 1)
            SetPVarInt(playerid, "dealer_carId", dealerVehicles + 1);
        
        SetPVarInt(playerid, "dealer_carId", GetPVarInt(playerid, "dealer_carId") - 1);

        return OnDealerTextDrawUpdate(playerid);
    }
    if(clickedid == dealerTD[3])
    {
        if(GetPVarInt(playerid, "dealer_carId") == dealerVehicles)
            SetPVarInt(playerid, "dealer_carId", 0);
        
        SetPVarInt(playerid, "dealer_carId", GetPVarInt(playerid, "dealer_carId") + 1);

        return OnDealerTextDrawUpdate(playerid);
    }
    if(clickedid == dealerTD[1])
    {
        new stockId = GetPVarInt(playerid, "dealer_carId");
        new moneySuffix = dealerData[stockId][dealer_vPrice], moneyPrefix = 0;
       
        if(dealerData[stockId][dealer_vStock] == 0)
            return SendClientMessage(playerid, COLOR_RED, "[!]{ffffff} Acest vehicul nu exista in stock.");

        if(playerInfo[playerid][pAccountLY] < moneyPrefix || playerInfo[playerid][pAccount] < moneySuffix && !playerInfo[playerid][pAccountLY])
            return sendError(playerid,"You don't have that sum of money in bank!");

        if(playerInfo[playerid][pAccount] - moneySuffix < 0)
        {
            if(!playerInfo[playerid][pAccountLY])
                return sendError(playerid,"You don't have that sum of money in bank!");

            playerInfo[playerid][pAccount] += 1000000000 - playerInfo[playerid][pAccount] - moneySuffix;

            playerInfo[playerid][pAccountLY] --;
        }
        else
            playerInfo[playerid][pAccount] -= moneySuffix;


        va_SendClientMessage(playerid, -1, "Felicitari! Vehiculul dorit a fost achizitionat! $%s au fost retrasi din contul tau bancar.", FormatNumber(dealerData[stockId][dealer_vPrice]));
        insertPlayerVehicle(playerid, dealerData[stockId][dealer_vModel], 0);
        playerStaticVar[playerid][pDealerDelay] = gettime() + 3600;

        if(dealerVehicle[playerid] != INVALID_VEHICLE_ID)
            DestroyVehicle(dealerVehicle[playerid]);

        SetCameraBehindPlayer(playerid);
        SetPlayerVirtualWorld(playerid, 0);

        sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_RED, "%s [user:%d] bought a %s for $%s.", GetName(playerid), playerInfo[playerid][pSQLID], aVehicleNames[dealerData[stockId][dealer_vModel] - 400], FormatNumber(dealerData[stockId][dealer_vPrice]));
        return true;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}