// ------------------------------------- //
//              Includes                 //
// ------------------------------------- //

#include <YSI_Coding\y_hooks>

// ------------------------------------- //
//              Constant                 //
// ------------------------------------- //

#define MAX_SAFEZONES 4

// ------------------------------------- //
//              Variables                //
// ------------------------------------- //

static safezoneAreas[MAX_SAFEZONES];

// ------------------------------------- //
//          Load Query Function          //
// ------------------------------------- //

function:load_safezones() {
    serverStuff[safeZones] = cache_num_rows();
    new Float:minX, Float:minY, Float:maxX, Float:maxY;
    if(!serverStuff[safeZones]) 
        return print("[MYSQL INFO] No safe-zones were found in the database.");

    for(new i = 0; i < serverStuff[safeZones]; i++) {

        cache_get_value_name_float(i, "safezoneminX", minX);
        cache_get_value_name_float(i, "safezoneminY", minY);
        cache_get_value_name_float(i, "safezonemaxX", maxX);
        cache_get_value_name_float(i, "safezonemaxY", maxY);
        safezoneAreas[i] = CreateDynamicRectangle(minX, minY, maxX, maxY);
        Streamer_SetIntData(STREAMER_TYPE_AREA, safezoneAreas[i], E_STREAMER_EXTRA_ID, i);
    }
    return printf("Safezones: %d", serverStuff[safeZones]);
}


// ------------------------------------- //
//              Commands                 //
// ------------------------------------- //

YCMD:addsafezone(playerid, params[], help) {
    if(playerInfo[playerid][pAdmin] < 8)
        return sendError(playerid,"Nu ai acces la aceasta comanda.");

    if(serverStuff[safeZones] >= MAX_SAFEZONES-1)
        return sendError(playerid,"Limita a fost atinsa! ("#MAX_SAFEZONES")");

    new 
        Float:minX, Float:minY, Float:maxX, Float:maxY;

    if(sscanf(params, "ffff", minX, minY, maxX, maxY))
        return sendSyntaxMessage(playerid, "/addsafezone <minX> <minY> <maxX> <maxY>");

    ++serverStuff[safeZones];

    safezoneAreas[serverStuff[safeZones]] = CreateDynamicRectangle(minX, minY, maxX, maxY);
    Streamer_SetIntData(STREAMER_TYPE_AREA, safezoneAreas[serverStuff[safeZones]], E_STREAMER_EXTRA_ID, serverStuff[safeZones]);

    
    mysql_format(SQL, returnString, sizeof returnString, "INSERT INTO `server_safezones` (minX, minY, maxX, maxY) VALUES ('%f', '%f', '%f', '%f')", minX, minY, maxX, maxY);
    mysql_tquery(SQL, returnString, "", "");

    sendStaffMessage(STAFF_TYPE_ADMIN,-1, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s added a new safe-zone. (dbid: %d)", GetName(playerid), serverStuff[safeZones]); 
    va_SendClientMessage(playerid, COLOR_GREY, "* You added succesfully a new safe-zone! (dbid: %d)", serverStuff[safeZones]);
    return 1;
}


hook loadServerSystems()
{
    
    mysql_tquery(SQL, "select * from `server_safezones`", "load_safezones", "");
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEnterDynArea(playerid, areaid)
{
    new const id = Streamer_GetIntData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID);

    if(id < MAX_SAFEZONES && safezoneAreas[id] == areaid) {
        playerStaticVar[playerid][var_insafezone] = true; 


        TextDrawShowForPlayer(playerid, SafezoneTD);
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerLeaveDynArea(playerid, areaid)
{
    new const id = Streamer_GetIntData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID);

    if(id < MAX_SAFEZONES && safezoneAreas[id] == areaid && playerStaticVar[playerid][var_insafezone]) {
        playerStaticVar[playerid][var_insafezone] = false; 
        TextDrawHideForPlayer(playerid, SafezoneTD); 
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}