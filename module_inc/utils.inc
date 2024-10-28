#define function:%0(%1)                         forward %0(%1); public %0(%1)
#define sendSyntaxMessage(%0,%1)                SendClientMessage(%0, COLOR_GREY, \""COLOR_SV_HEX"USAGE >> {FFFFFF}"%1)
#define returnNoAcces(%0)                       SendClientMessage(%0, COLOR_SERVER, \"[!!] {ffffff}Nu aveti gradul admininistrativ necesar pentru a putea solicita acest lucru.")
#define check_queries                           if(mysql_unprocessed_queries() > 5) return SendClientMessage(playerid, COLOR_GREY, "[!] Nu poti folosi aceasta optiune in acest moment deoarece baza de date a serverului este incarcata! Reincearca in cateva momente!");
#define request_update                          SendClientMessage(playerid, COLOR_SERVER, "[!] {ffffff}Solicitarea ta de a fost trimisa catre baza de date.");
#define check_delay                             if(GetPVarInt(playerid, "commands_delay") > gettime()) return va_SendClientMessage(playerid, COLOR_GREY, "Vei putea folosi aceasta functie peste %d secunde.", GetPVarInt(playerid, "commands_delay") - gettime());
#define set_delay                               SetPVarInt(playerid, "commands_delay", gettime() + 5)
#define GetMoney(%0)                            playerInfo[%0][pMoney]
#define GetMilliards(%0)                        playerInfo[%0][pMoneyStore]
#define va_PlayerTextDrawSetString(%0,%1,%2,%3) format(returnString, sizeof(returnString),%2,%3) && PlayerTextDrawSetString(%0, %1, returnString)
#define IsKeyReleased(%0)                       (((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))
sendError(playerid, const text[], va_args<>)    return va_SendClientMessage(playerid, COLOR_GREY, ""COLOR_SV_HEX"[ERROR]: {FFFFFF}%s", va_return(text, va_start<2>));

stock bool:IsFireWeapon(weaponid)
{
    if (weaponid < 1 || weaponid > 18)
    {
        return true;
    }
    return false;
}


PreloadAnimLib(playerid, const animlib[])
{
    ApplyAnimation(playerid, animlib, "NULL", 0.0, 0, 0, 0, 0, 0);
}
new const aVehicleSeatsCount[] = {
    4, 2, 2, 2, 4, 4, 1, 2, 2, 4, 2, 2, 2, 4, 2, 2, 4, 2, 4, 2, 4, 2, 2, 2, 2, 1, 4, 4, 4, 2, 1, 9, 1, 2, 2, 1, 2, 9, 4, 2, 4, 1, 2, 
    2, 2, 4, 1, 2, 1, 2, 1, 2, 1, 1, 1, 2, 2, 2, 4, 4, 2, 2, 2, 2, 1, 1, 4, 4, 2, 2, 4, 2, 1, 1, 2, 2, 1, 2, 2, 4, 2, 1, 2, 3, 1, 1, 1, 4, 2, 2, 4, 2, 4, 1, 2, 2, 2, 4, 4, 2,
    2, 1, 2, 2, 2, 2, 2, 4, 2, 1, 1, 2, 1, 1, 2, 2, 4, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 4, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 4, 2, 2, 2, 2, 2, 4, 4, 2, 2, 4, 4, 2, 1, 2, 2, 2, 2, 2, 2, 
    4, 4, 2, 2, 1, 2, 4, 4, 1, 1, 1, 1, 1, 2, 1, 2, 2, 2, 2, 4, 4, 2, 4, 1, 1, 4, 2, 2, 2, 2, 1, 1, 2, 2, 1, 1, 4, 4, 4, 2, 2, 2, 2, 2, 4, 2, 1, 1, 1, 4, 1, 1                 
};


stock weapName(weaponid) {
    new weaponname[25]; GetWeaponName(weaponid, weaponname, 25);
    return weaponname; }
    
OnePlayAnim(playerid,const animlib[],const animname[], Float:Speed, looping, lockx, locky, lockz, lp)
{
    ApplyAnimation(playerid, animlib, animname, Speed, looping, lockx, locky, lockz, lp);
}

clearPlayerKillList(const playerid) {
    for(new i = 0; i < 5; ++i) 
        SendDeathMessageToPlayer(playerid, MAX_PLAYERS + 1, MAX_PLAYERS + 1, MAX_PLAYERS + 1);
}

function:getPlayerIDbySQLID(id) {
    foreach(new i : Player) if(playerInfo[i][pSQLID] == id) return i;
    return true;
}


getVehicleSpeed(const vehicleid) {
    new 
        Float:vX, Float:vY, Float:vZ;

    GetVehicleVelocity(vehicleid, vX, vY, vZ);
    return floatround(VectorSize(vX, vY, 0.0) * 170.0);
}

#define MAX_LOOK_DISTANCE 20.0
#define MAX_ANGLE_DIFFERENCE 0.98

stock IsPlayerLookingAtObject(playerid, objectid) {
    new Float:playerPos[3], Float:playerAngle;
    GetPlayerPos(playerid, playerPos[0], playerPos[1], playerPos[2]);
    GetPlayerFacingAngle(playerid, playerAngle);

    new Float:objectPos[3];
    GetObjectPos(objectid, objectPos[0], objectPos[1], objectPos[2]);

    new Float:lookVector[3];
    lookVector[0] = floatsin(-playerAngle, degrees);
    lookVector[1] = floatcos(-playerAngle, degrees);
    lookVector[2] = 0.0;

    new Float:toObjectVector[3];
    toObjectVector[0] = objectPos[0] - playerPos[0];
    toObjectVector[1] = objectPos[1] - playerPos[1];
    toObjectVector[2] = objectPos[2] - playerPos[2];

    new Float:distance = floatsqroot(toObjectVector[0] * toObjectVector[0] + toObjectVector[1] * toObjectVector[1] + toObjectVector[2] * toObjectVector[2]);
    if (distance > MAX_LOOK_DISTANCE) {
        return 0;
    }

    toObjectVector[0] /= distance;
    toObjectVector[1] /= distance;
    toObjectVector[2] /= distance;

    new Float:dotProduct = lookVector[0] * toObjectVector[0] + lookVector[1] * toObjectVector[1] + lookVector[2] * toObjectVector[2];

    if (dotProduct > MAX_ANGLE_DIFFERENCE) {
        return 1; 
    }

    return 0;
}

ConvertTimestampToDate(timestamp, &year, &month, &day, &hour, &minute, &second) {
    new secInMin = 60;
    new secInHour = 60 * secInMin;
    new secInDay = 24 * secInHour;
    new daysInYear = 365;
    new secInYear = daysInYear * secInDay;

    // Calculate the year
    year = 1970;
    new remainingSeconds = timestamp;

    while (remainingSeconds >= secInYear) {
        if (IsLeapYear(year)) {
            remainingSeconds -= (secInYear + secInDay); // Leap year
        } else {
            remainingSeconds -= secInYear;
        }
        year++;
    }

    // Calculate the month and day
    static daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (IsLeapYear(year)) {
        daysInMonth[1] = 29; // Adjust for leap year
    } else {
        daysInMonth[1] = 28;
    }

    month = 0;
    while (remainingSeconds >= daysInMonth[month] * secInDay) {
        remainingSeconds -= daysInMonth[month] * secInDay;
        month++;
    }
    month += 1; // Convert zero-based month to one-based

    day = remainingSeconds / secInDay + 1; // Convert zero-based day to one-based
    remainingSeconds %= secInDay;

    // Calculate the hour, minute, and second
    hour = remainingSeconds / secInHour;
    remainingSeconds %= secInHour;

    minute = remainingSeconds / secInMin;
    second = remainingSeconds % secInMin;
}

enum e_vehicleModelTypes {
    VEHICLE_MODEL_TYPE_CAR,
    VEHICLE_MODEL_TYPE_BIKE,
    VEHICLE_MODEL_TYPE_PLANE,
    VEHICLE_MODEL_TYPE_BOAT,
    VEHICLE_MODEL_TYPE_MOTOR_BIKE
}
#if SERVER_ANTICHEAT
e_vehicleModelTypes:getVehicleModelType(const modelid) {
    switch(modelid) {
        case 417, 425, 447, 460, 469, 476, 487, 488, 497, 511, 512, 513, 519, 520, 548, 553, 563, 577, 592, 593: return VEHICLE_MODEL_TYPE_PLANE;
        case 430, 446, 452, 453, 454, 472, 473, 484, 493, 595: return VEHICLE_MODEL_TYPE_BOAT;
        case 481, 509, 510: return VEHICLE_MODEL_TYPE_BIKE;
        case 448, 461, 462, 463, 468, 471, 521, 522, 523, 581, 586: return VEHICLE_MODEL_TYPE_MOTOR_BIKE;
    }
    return VEHICLE_MODEL_TYPE_CAR;
}

GetPlayerSpeed(const playerid) {
    new 
        Float: X, Float: Y, Float: Z; 

    GetPlayerVelocity(playerid, X,Y,Z); 
    return floatround(VectorSize(X, Y, Z) * 200.0); 
}
#endif


LoopingAnim(playerid,const animlib[],const animname[], Float:Speed, looping, lockx, locky, lockz, lp)
{
    gPlayerUsingLoopingAnim[playerid] = 1;
    ApplyAnimation(playerid, animlib, animname, Speed, looping, lockx, locky, lockz, lp);
}
#if SERVER_ANTICHEAT
new const aVehicleMaxSpeed[] = {
    149, 139, 176, 104, 125, 155, 104, 140, 94, 149, 122, 209, 159, 104, 100, 181, 145, 127, 109, 141, 137, 145, 132, 93, 128, 191, 164, 156, 148,
    190, 100, 123, 89, 104, 158, 0, 141, 149, 135, 159, 128, 71, 131, 119, 104, 155, 140, 126, 106, 169, 0, 182, 141, 58, 115, 119, 100, 90, 138, 128, 0,
    151, 105, 136, 0, 0, 139, 132, 136, 0, 148, 104, 0, 0, 141, 163, 0, 176, 111, 132, 174, 68, 148, 116, 0, 94, 60, 0, 0, 132, 148, 141, 132, 0, 203, 166,
    153, 0, 115, 116, 132, 0, 203, 203, 163, 132, 169, 156, 102, 74, 95, 0, 0, 0, 113, 126, 148, 148, 155, 0, 0, 150, 166, 142, 123, 151, 149, 141, 166, 141,
    57, 66, 104, 157, 159, 149, 163, 0, 0, 94, 141, 191, 155, 142, 140, 139, 141, 135, 0, 145, 137, 148, 114, 0, 136, 149, 104, 104, 147, 168, 159, 145, 168,
    0, 83, 155, 151, 163, 138, 0, 0, 88, 58, 104, 57, 149, 140, 0, 123, 149, 144, 101, 115, 66, 0, 144, 132, 155, 102, 153, 0, 0, 0, 0, 57, 0, 166, 166, 166, 149,
    142, 104, 160, 162, 139, 142, 0, 0, 0, 102, 0
};

bool:IsValidWeapon(const weaponid) 
    return !(weaponid < 1 || weaponid > 46 || weaponid == 19 || weaponid == 20 || weaponid == 21 || weaponid == 45);
   
bool:isPlayerFalling(const playerid) {
    switch(GetPlayerAnimationIndex(playerid)) {
        case 958..979, 1132, 1130, 1195: return true;
    }
    return false;
}
#endif
getVehicleOccupantsCount(const vehicleid) {
    new 
        count, modelid = GetVehicleModel(vehicleid) - 400;

    foreach(new i : playersInVehicle) {
        if(GetPlayerVehicleID(i) != vehicleid) 
            continue;

        if(++count == aVehicleSeatsCount[modelid]) 
            break;
    }
    return count;
}
bool:isInRectangleArea(const Float:ofutpemata, const Float:ilfutpetacto, const Float:minX, const Float:maxX, const Float:minY, const Float:maxY) 
    return (ofutpemata >= minX && ofutpemata <= maxX && ilfutpetacto >= minY && ilfutpetacto <= maxY);

bool:isPlayerInRectangleArea(const playerid, const Float:sugemata1, const Float:sugemata12, const Float:sugemata1234, const Float:sugemata123) {
    new Float:ics, Float:igrec, Float:z;

    GetPlayerPos(playerid, ics, igrec, z);

    return (ics >= sugemata1 && ics <= sugemata12 && igrec >= sugemata1234 && igrec <= sugemata123);
}

clearChat(playerid, const lines = 50) {
    for(new i; i < lines; i++) SendClientMessage(playerid, -1, "");
    return 1;
}

CalculateTime(seconds) {
    new time[12];

    if(seconds < 3600) 
        format(time, sizeof time, "%02d:%02d", (seconds / 60) % 60, seconds % 60);
    
    else 
        format(time, sizeof time, "%02d:%02d:%02d", (seconds / 3600), (seconds / 60) % 60, seconds % 60);

    return time;
}

StopLoopingAnim(playerid)
{
    gPlayerUsingLoopingAnim[playerid] = 0;
    ApplyAnimation(playerid, "CARRY", "crry_prtial", 4.0, 0, 0, 0, 0, 0);
}

RandomEx(minim, max) return random(max + 1 - minim) + minim;

KickEx(playerid)
{
    defer _Kick(playerid);
    return true;
}

function:ProxDetectorS(Float:radi, playerid, targetid)
{
    if(IsPlayerConnected(playerid)&&IsPlayerConnected(targetid))
    {
        new Float:posx, Float:posy, Float:posz;
        new Float:oldposx, Float:oldposy, Float:oldposz;
        new Float:tempposx, Float:tempposy, Float:tempposz;
        GetPlayerPos(playerid, oldposx, oldposy, oldposz);
        GetPlayerPos(targetid, posx, posy, posz);
        tempposx = (oldposx -posx);
        tempposy = (oldposy -posy);
        tempposz = (oldposz -posz);
        if(((tempposx < radi) && (tempposx > -radi)) && ((tempposy < radi) && (tempposy > -radi)) && ((tempposz < radi) && (tempposz > -radi)))
        {
            return 1;
        }
    }
    return 0;
}


strmatch(const string1[], const string2[]) return !strcmp(string1, string2, true) ? (true) : (false);

sendSplitMessage(playerid, color, const string[], va_args<>)
{
    new final[256];
    va_format(final, sizeof final, string, va_start<3>);

    new len = strlen(final),
        _iL = len / 128,
        _Line[128 + 5],
        _:_i@Index;

    if((len % 128 )) _iL ++;
    while(_i@Index < _iL)
    {
        if(_i@Index == 0)
            strmid(_Line, final, (_i@Index * 128), (_i@Index * 128) + 128);

        else
            strmid(_Line, final, (_i@Index * 128), (_i@Index * 128) + 128);

        if(_iL > 1)
        {
            if(_i@Index > 0)
                format(_Line, sizeof _Line, "... %s", _Line);
        }
        SendClientMessage(playerid, color, _Line);
        _i@Index ++;
    }
    return true;
}

sendNearbyMessage(playerid, color, const string[], va_args<>)
{
    new streamerString[144], Float: returnPos[3];
    GetPlayerPos(playerid, returnPos[0], returnPos[1], returnPos[2]);

    va_format(streamerString, sizeof streamerString, string, va_start<3>);
    SendClientMessage(playerid, color, streamerString);

    foreach(new i : streamedPlayers[playerid])
    {
        if(IsPlayerInRangeOfPoint(i, 25.0, returnPos[0], returnPos[1], returnPos[2]))
        {
            SendClientMessage(i, color, streamerString);
        }
    }
    return true;
}


function:ProxDetector(Float:radi, playerid, string[],col1,col2,col3,col4,col5)
{
    if(IsPlayerConnected(playerid))
    {
        static Float:posx, Float:posy, Float:posz;
        static Float:oldposx, Float:oldposy, Float:oldposz;
        static Float:tempposx, Float:tempposy, Float:tempposz;
        GetPlayerPos(playerid, oldposx, oldposy, oldposz);
        foreach(new i : Player)
        {
            if(GetPlayerInterior(playerid) == GetPlayerInterior(i) && GetPlayerVirtualWorld(playerid) == GetPlayerVirtualWorld(i))
            {
                GetPlayerPos(i, posx, posy, posz);
                tempposx = (oldposx -posx);
                tempposy = (oldposy -posy);
                tempposz = (oldposz -posz);
                if(((tempposx < radi/16) && (tempposx > -radi/16)) && ((tempposy < radi/16) && (tempposy > -radi/16)) && ((tempposz < radi/16) && (tempposz > -radi/16)))
                {
                    sendSplitMessage(i, col1, string);
                }
                else if(((tempposx < radi/8) && (tempposx > -radi/8)) && ((tempposy < radi/8) && (tempposy > -radi/8)) && ((tempposz < radi/8) && (tempposz > -radi/8)))
                {
                    sendSplitMessage(i, col2, string);
                }
                else if(((tempposx < radi/4) && (tempposx > -radi/4)) && ((tempposy < radi/4) && (tempposy > -radi/4)) && ((tempposz < radi/4) && (tempposz > -radi/4)))
                {
                    sendSplitMessage(i, col3, string);
                }
                else if(((tempposx < radi/2) && (tempposx > -radi/2)) && ((tempposy < radi/2) && (tempposy > -radi/2)) && ((tempposz < radi/2) && (tempposz > -radi/2)))
                {
                    sendSplitMessage(i, col4, string);
                }
                else if(((tempposx < radi) && (tempposx > -radi)) && ((tempposy < radi) && (tempposy > -radi)) && ((tempposz < radi) && (tempposz > -radi)))
                {
                    sendSplitMessage(i, col5, string);
                }
                else
                {
                    sendSplitMessage(i, col1, string);
                }
            }
        }
    }
    return 1; 
}
isValidEmail(const email[])
{
    new at_pos = strfind(email, "@", true) + 1;
    if(email[0] == '@' || at_pos == -1)
        return false;

    static providers[][] = { "gmail.com" };
    for(new i; i < sizeof providers; i++)
    {
        if(!strcmp(email[at_pos], providers[i], true))
            return true;
    }
    return false;
}

string_checker(const string[], pos = -1, length = -1)
{
    if (length == -1)
        length = strlen(string);
    
    while(pos < length) { 
        if (string[pos] >= 'A' && string[pos] <= 'Z') // string[pos] de 2 ori pt ca samp
            pos++;
        else
            break;
    }
    
    while(pos < length) { 
        if (string[pos] >= '0' && string[pos] <= '9') // string[pos] de 2 ori pt ca samp x2
            pos++;
        else
            return true;
    }
    
    return false;
}

FindIP(const StrToChk[])
{
    new IpLevel = 0;
    for(new a = 0; a < strlen(StrToChk); a++)
    {
        switch(IpLevel)
        {
            case 0:
            {
                if(IsNum(StrToChk[a])) IpLevel++;
            }
            case 1, 2:
            {
                if(IsNum(StrToChk[a])) IpLevel++;
                else if(StrToChk[a] == '.') IpLevel = 4;
                else IpLevel = 0;
            }
            case 3:
            {
                if(StrToChk[a] == '.') IpLevel++;
                else IpLevel = 0;
            }
            case 4:
            {
                if(IsNum(StrToChk[a])) IpLevel++;
                else IpLevel = 0;
            }
            case 5, 6:
            {
                if(IsNum(StrToChk[a])) IpLevel++;
                else if(StrToChk[a] == '.') IpLevel = 8;
                else IpLevel = 0;
            }
            case 7:
            {
                if(StrToChk[a] == '.') IpLevel++;
                else IpLevel = 0;
            }
            case 8:
            {
                if(IsNum(StrToChk[a])) IpLevel++;
                else IpLevel = 0;
            }
            case 9, 10:{
                if(IsNum(StrToChk[a])) IpLevel++;
                else if(StrToChk[a] == '.') IpLevel = 12;
                else IpLevel = 0;
            }
            case 11:
            {
                if(StrToChk[a] == '.') IpLevel++;
                else IpLevel = 0;
            }
            case 12:
            {
                if(IsNum(StrToChk[a])) return 1;
                else IpLevel = 0;
            }
        }
    }
    return 0;
}
IsNum(textchar)
{
    if(textchar == '0') return 1;
    if(textchar == '1') return 1;
    if(textchar == '2') return 1;
    if(textchar == '3') return 1;
    if(textchar == '4') return 1;
    if(textchar == '5') return 1;
    if(textchar == '6') return 1;
    if(textchar == '7') return 1;
    if(textchar == '8') return 1;
    if(textchar == '9') return 1;
    return 0;
}

va_Command_ReProcess(playerid, const command[], help, va_args<>)
{
    new commandString[48];
    va_format(commandString, sizeof commandString, command, va_start<3>);

    return Command_ReProcess(playerid, commandString, help);
}

va_mysql_tquery(const text[], va_args<>) {
    new query[1024];
    mysql_format(SQL, query, sizeof query, text, va_start<1>);
    return mysql_tquery(SQL, query);
}

stock Float:GetDistanceBetweenPoints(Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2) return floatsqroot(floatpower(floatabs(floatsub(x2, x1)), 2) + floatpower(floatabs(floatsub(y2, y1)), 2) + floatpower(floatabs(floatsub(z2, z1)), 2));

stock Float:GetDistanceBetweenPlayers(playerid, id)
{
    new Float:x, Float:y, Float:z, Float:xx, Float:yy, Float:zz;
    if(!IsPlayerConnected(playerid) && !IsPlayerConnected(id))
        return -1.00;

    GetPlayerPos(playerid, x, y, z);
    GetPlayerPos(id, xx, yy, zz);
    return floatsqroot(floatpower(floatabs(floatsub(x, xx)), 2) + floatpower(floatabs(floatsub( y, yy )), 2) + floatpower(floatabs(floatsub(z, zz)), 2));
}

stock IsPlayerNearVehicle(playerid, vehicleid, Float: range)
{
    new Float: veh_pos[3];

    GetVehiclePos(vehicleid, veh_pos[0], veh_pos[1], veh_pos[2]);
    return (IsPlayerInRangeOfPoint(playerid, range, veh_pos[0], veh_pos[1], veh_pos[2])) ? (1) : (0);
}

IsKeyJustDown(key, newkeys, oldkeys)
{
    if((newkeys & key) && !(oldkeys & key)) return true;
    return false;
}

iTimestampDate(seconds)
{
    new returnTime[15];
    if(seconds / 3600)
        format(returnTime, sizeof returnTime, "%02d:%02d:%02d", seconds/3600, seconds/60, seconds%60);

    else
        format(returnTime, sizeof returnTime, "%02d:%02d", seconds / 60, seconds % 60);

    return returnTime;
}

returnTimeSeconds(seconds)
{
    new returnTime[15], time = seconds, minute, secunde;
    time = time % 3600;
    minute = time / 60;
    time = time % 60;
    secunde = time;

    format(returnTime, sizeof returnTime, "%02d:%02d", minute, secunde);
    return returnTime;
}

showActiveCheckpointDialog(playerid)
{
    return Dialog_Show(playerid, DIALOG_CANCELCP, DIALOG_STYLE_MSGBOX, "Anulare Checkpoint", "Esti sigur ca vrei sa anulezi checkpoint-ul curent?", "Da", "Nu");
}

sendSplittedMessageToPlayer(const playerid, const first_line_color, const last_line_color, const message[], va_args<>) {
    new string[2048];

    va_format(string, sizeof string, message, va_start<4>);

    new 
        len = strlen(string), 
        linesCount = len / 128,
        idx, 
        messageLine[128 + 5];

    if(len % 128) 
        linesCount++;

    while(idx < linesCount) {
        if(!idx)
            strmid(messageLine, string, (idx * 128), (idx * 128) + 128);

        else
            strmid(messageLine, string, (idx * 128), (idx * 128) + 128);

        if(linesCount > 1) {
            if(!idx) {
                format(messageLine, sizeof messageLine, "%s", messageLine);
            }
            else if(idx > 0 && (idx + 1) < linesCount) {
                format(messageLine, sizeof messageLine, "... %s", messageLine);
            }
            else {
                format(messageLine, sizeof messageLine, "... %s", messageLine);
            }
        }

        if(strlen(messageLine)) 
            SendClientMessage(playerid, !idx ? first_line_color : last_line_color, messageLine);

        idx++;
    }
    return 1;
}
IsPositionInArea(Float:x, Float:y, Float:minx, Float:miny, Float:maxx, Float:maxy)
{
    if(x > minx && x < maxx && y > miny && y < maxy)
        return true;

    return false;
}
GetCoordinatesCity(Float:x, Float:y)
{
    new cityName[9] = "Unknown";

    if(IsPositionInArea(x, y, -3000, -3000, -776, 3000)) cityName = "SF";
    else if(IsPositionInArea(x, y, -778, -3000, 3000, 412)) cityName = "LS";
    else if(IsPositionInArea(x, y, -778, 412, 3000, 3000)) cityName = "LV";
    return cityName;
}

sendSplittedMessageToAll(const first_line_color, const last_line_color, const message[], va_args<>) {
    new string[2048];

    va_format(string, sizeof string, message, va_start<3>);

    new 
        len = strlen(string), 
        linesCount = len / 128,
        idx, 
        messageLine[128 + 5];

    if(len % 128) 
        linesCount++;

    while(idx < linesCount) {
        if(!idx)
            strmid(messageLine, string, (idx * 128), (idx * 128) + 128);

        else
            strmid(messageLine, string, (idx * 128), (idx * 128) + 128);

        if(linesCount > 1) {
            if(!idx) {
                format(messageLine, sizeof messageLine, "%s", messageLine);
            }
            else if(idx > 0 && (idx + 1) < linesCount) {
                format(messageLine, sizeof messageLine, "... %s", messageLine);
            }
            else {
                format(messageLine, sizeof messageLine, "... %s", messageLine);
            }
        }

        if(strlen(messageLine)) 
            SendClientMessageToAll(!idx ? first_line_color : last_line_color, messageLine);

        idx++;
    }
    return 1;
}


stock TogglePlayerControllableEx(playerid, type) {
    TogglePlayerControllable(playerid, type);
    return 1;
}


stock GetName(playerid)
{
    new returnNickname[MAX_PLAYER_NAME];

    if(IsPlayerConnected(playerid))
        GetPlayerName(playerid, returnNickname, sizeof returnNickname);

    return returnNickname;
}

stock GetDBName(playerid)
{
    new returnNickname[MAX_PLAYER_NAME];
    if(!playerStaticVar[playerid][playerLogged])
    {
        GetPlayerName(playerid, returnNickname, sizeof returnNickname);
        return returnNickname;
    }

    format(returnNickname, sizeof returnNickname, "%s", playerInfo[playerid][pName]);
    return returnNickname;
}

stock GetNameBySQL(sqlid)
{
    new stringCache[128], returnNickname[MAX_PLAYER_NAME];
    format(stringCache, sizeof stringCache, "select `Name` from `users` where `id` = '%d';", sqlid);

    new Cache: cache_player_name = mysql_query(SQL, stringCache);
    if(cache_num_rows())
        cache_get_value_name(0, "Name", returnNickname);

    else
        returnNickname = "NULL";

    cache_delete(cache_player_name);
    return returnNickname;
}

stock GetSQLByName(name[])
{
    new stringCache[128], returnSQLID[32];
    format(stringCache, sizeof stringCache, "select `id` from `users` where `Name` = '%s';", name);

    new Cache: cache_player_id = mysql_query(SQL, stringCache);
    if (cache_num_rows())
    {
        cache_get_value_name(0, "id", returnSQLID);
    }
    else
    {
        returnSQLID = "NULL";
    }

    cache_delete(cache_player_id);
    return strval(returnSQLID);
}