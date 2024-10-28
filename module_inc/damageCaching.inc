/*
    damage caching include

    forked from samp-weapon-config by oscar-broman
    thanks a lot;


    edited by ireplay
*/


#if !defined _inc_y_iterate
    #tryinclude <YSI_Data\y_iterate>
#endif

#include <YSI_Coding\y_hooks>

#define UPDATE_TYPE_HEALTH 0
#define UPDATE_TYPE_ARMOUR 1
#define BODY_PART_HEAD                  9
#define BODY_PART_RIGHT_LEG             8
#define BODY_PART_LEFT_LEG              7
#define BODY_PART_RIGHT_ARM             6
#define BODY_PART_LEFT_ARM              5
#define BODY_PART_GROIN                 4
#define BODY_PART_TORSO                 3

enum ENUM_PLAYER_DAMAGE
{
    Float:damage_given[MAX_PLAYERS], Float:damage_taked[MAX_PLAYERS], damage_player[MAX_PLAYERS], Float:playerHealth, Float:playerArmour, playerDeath,
    cbugWarnings, lastShotTime
};
new player_data[MAX_PLAYERS][ENUM_PLAYER_DAMAGE];
static const ResetDamageEnum[ENUM_PLAYER_DAMAGE];

new returnGunNames[][] =
{
    "Nothing", "Brass knuckles", "Golf club", "Nightstick", "Knife", "Bat", "Shovel", "Pool cue", "Katana", "Chainsaw", "Purple dildo", "Dildo",
    "Vibrator", "Vibrator", "Flowers", "Cane", "Grenade", "Tear gas", "Molotov", "Vehicle gun", "Vehicle gun", "Vehicle gun", "Colt 45", "Silenced pistol",
    "Deagle", "Shotgun", "Sawn-off shotgun", "Combat shotgun", "Mac-10", "MP5", "AK-47", "M4", "Tec-9", "Rifle", "Sniper", "Rocket launcher",
    "Heat seeking RPG", "Flamethrower", "Minigun", "Satchel", "Detonator", "Spraycan", "Fire extinguisher", "Camera", "Night vision goggles",
    "Infrared goggles", "Parachute", "Fake pistol", "Pistol whip", "Vehicle", "Helicopter blades", "Explosion", "Car parking", "Drowning",
    "Collision", "Splat", "Unknown"
};
#pragma warning disable 213
enum E_DAMAGE_FEED_HIT
{
    e_Issuer, e_Name[MAX_PLAYER_NAME], Float:e_Amount, e_Weapon, e_Tick, e_BodyPart
}
new PlayerText:damageTD[MAX_PLAYERS][2], Text:healthIcon, PlayerText:hudTD[MAX_PLAYERS][2], Text3D: DeadScris[MAX_PLAYERS], Timer: label_timer[MAX_PLAYERS];

new labelTextDamage[64], damageLabelID;

static s_DamageFeedHitsGiven[MAX_PLAYERS][5][E_DAMAGE_FEED_HIT];
static s_DamageFeedHitsTaken[MAX_PLAYERS][5][E_DAMAGE_FEED_HIT];
static s_DamageFeedTimer[MAX_PLAYERS] = {-1, ...};
static s_DamageFeedLastUpdate[MAX_PLAYERS];

hook OnPlayerConnect(playerid)
{
    player_data[playerid] = ResetDamageEnum;
    damageTD[playerid][0] = CreatePlayerTextDraw(playerid, 200.0, 365.0, "_give_");
    PlayerTextDrawLetterSize(playerid, damageTD[playerid][0], 0.2, 0.9);
    PlayerTextDrawColour(playerid, damageTD[playerid][0], 0x30FF50FF);
    PlayerTextDrawFont(playerid, damageTD[playerid][0], 1);
    PlayerTextDrawSetShadow(playerid, damageTD[playerid][0], 0);
    PlayerTextDrawAlignment(playerid, damageTD[playerid][0], 2);
    PlayerTextDrawSetOutline(playerid, damageTD[playerid][0], 1);
    PlayerTextDrawBackgroundColour(playerid, damageTD[playerid][0], 0x0000001A);

    damageTD[playerid][1] = CreatePlayerTextDraw(playerid, 440.0, 365.0, "_taken_");
    PlayerTextDrawLetterSize(playerid, damageTD[playerid][1], 0.2, 0.9);
    PlayerTextDrawColour(playerid, damageTD[playerid][1], 0x33CCFFFF);
    PlayerTextDrawFont(playerid, damageTD[playerid][1], 1);
    PlayerTextDrawSetShadow(playerid, damageTD[playerid][1], 0);
    PlayerTextDrawAlignment(playerid, damageTD[playerid][1], 2);
    PlayerTextDrawSetOutline(playerid, damageTD[playerid][1], 1);
    PlayerTextDrawBackgroundColour(playerid, damageTD[playerid][1], 0x0000001A);

    hudTD[playerid][0] = CreatePlayerTextDraw(playerid, 577.000000, 68.000000, " ");
    PlayerTextDrawFont(playerid, hudTD[playerid][0], 2);
    PlayerTextDrawLetterSize(playerid, hudTD[playerid][0], 0.162499, 0.649999);
    PlayerTextDrawTextSize(playerid, hudTD[playerid][0], 403.000000, 56.500000);
    PlayerTextDrawSetOutline(playerid, hudTD[playerid][0], 1);
    PlayerTextDrawSetShadow(playerid, hudTD[playerid][0], 0);
    PlayerTextDrawAlignment(playerid, hudTD[playerid][0], 2);
    PlayerTextDrawColor(playerid, hudTD[playerid][0], 1687547391);
    PlayerTextDrawBackgroundColor(playerid, hudTD[playerid][0], 255);
    PlayerTextDrawBoxColor(playerid, hudTD[playerid][0], 50);
    PlayerTextDrawUseBox(playerid, hudTD[playerid][0], 0);
    PlayerTextDrawSetProportional(playerid, hudTD[playerid][0], 1);
    PlayerTextDrawSetSelectable(playerid, hudTD[playerid][0], 0);

    hudTD[playerid][1] = CreatePlayerTextDraw(playerid, 577.000000, 46.000000, " ");
    PlayerTextDrawFont(playerid, hudTD[playerid][1], 2);
    PlayerTextDrawLetterSize(playerid, hudTD[playerid][1], 0.162499, 0.649999);
    PlayerTextDrawTextSize(playerid, hudTD[playerid][1], 403.000000, 56.500000);
    PlayerTextDrawSetOutline(playerid, hudTD[playerid][1], 1);
    PlayerTextDrawSetShadow(playerid, hudTD[playerid][1], 0);
    PlayerTextDrawAlignment(playerid, hudTD[playerid][1], 2);
    PlayerTextDrawColor(playerid, hudTD[playerid][1], 1687547391);
    PlayerTextDrawBackgroundColor(playerid, hudTD[playerid][1], 255);
    PlayerTextDrawBoxColor(playerid, hudTD[playerid][1], 50);
    PlayerTextDrawUseBox(playerid, hudTD[playerid][1], 0);
    PlayerTextDrawSetProportional(playerid, hudTD[playerid][1], 1);
    PlayerTextDrawSetSelectable(playerid, hudTD[playerid][1], 0);

    return Y_HOOKS_CONTINUE_RETURN_1;
}


hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if(newkeys == KEY_CROUCH && oldkeys != KEY_CROUCH || newkeys == KEY_CROUCH && oldkeys == KEY_HANDBRAKE)
    {
        if(player_data[playerid][lastShotTime] && GetTickCount() - player_data[playerid][lastShotTime] < 1200)
        {
            new wID = GetPlayerWeapon(playerid);
            if(wID == 24 || wID == 25 || wID == 27 || wID == 34)
            {
                player_data[playerid][cbugWarnings] ++;

                if(player_data[playerid][cbugWarnings] >= 3)
                {
                    SetPlayerArmedWeapon(playerid, WEAPON_FIST);
                    ClearAnimations(playerid);

                    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_SERVER, "{ffffff}("COLOR_SV_HEX"Staff Info{ffffff}): %s(%d) did c-bug 3 times consecutieve and has been freezed", GetName(playerid), playerid);
                    player_data[playerid][cbugWarnings] = 0;

                    SendClientMessage(playerid, COLOR_RED, "Deoarece ai facut c-bug consecutiv ai primit freeze.");
                    GameTextForPlayer(playerid, "~w~~n~~n~~n~~n~~n~~n~~n~~n~c-bug interzis!", 2000, 3);

                    TogglePlayerControllable(playerid, false);
                    FreezePlayer(playerid, 2);
                }
            }
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

DMG_GetWeaponNameEx(weaponid, weapon[], len = sizeof(weapon))
{
    if (weaponid < 0 || weaponid >= sizeof(returnGunNames)) {
        format(weapon, len, "Weapon %d", weaponid);
    } else {
        strunpack(weapon, returnGunNames[weaponid], len);
    }

    return 1;
}

CreateDamageTextLabel(playerid, Float:amount)
{

    new Float:originX, Float:originY, Float:originZ,
        Float:hitPosX, Float:hitPosY, Float:hitPosZ;

    GetPlayerLastShotVectors(playerid, originX, originY, originZ, hitPosX, hitPosY, hitPosZ);
    
    format(labelTextDamage, sizeof(labelTextDamage), "%.2f", amount);
    if(!IsValidPlayer3DTextLabel(playerid, damageLabelID))
        damageLabelID = CreatePlayer3DTextLabel(playerid, labelTextDamage, COLOR_WHITE, hitPosX, hitPosY, hitPosZ, 40.0);

    label_timer[playerid] = defer DestroyDamageTextLabel(playerid, damageLabelID);
    return true;
}

returnBodyPartName(bodyPart)
{

    new bodystr[16];

    switch(bodyPart) {
        case BODY_PART_HEAD: bodystr = "head";
        case BODY_PART_TORSO: bodystr = "torso";
        case BODY_PART_GROIN: bodystr = "groin";
        case BODY_PART_LEFT_ARM: bodystr = "left arm";
        case BODY_PART_RIGHT_ARM: bodystr = "right arm";
        case BODY_PART_LEFT_LEG: bodystr = "left leg";
        case BODY_PART_RIGHT_LEG: bodystr = "right leg";
    }
    return bodystr;
}


public OnPlayerGiveDamage(playerid, damagedid, Float:amount, WEAPON:weaponid, bodypart)
{

    if(playerStaticVar[damagedid][playerSleeping])
        return 1;
    #if SERVER_ANTICHEAT
    if(GetPlayerTargetPlayer(playerid) == INVALID_PLAYER_ID && GetPlayerWeapon(playerid) != weaponid && weaponid != WEAPON_SPRAYCAN) {
        sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_SERVER, "(AC): {ffffff}%s is suspected for damage hack #1", GetName(playerid));
    }

    if(GetPlayerTargetPlayer(playerid) == INVALID_PLAYER_ID && amount < 0 || amount >= 100.0 && weaponid != WEAPON_SPRAYCAN && weaponid != WEAPON_KNIFE) {
        sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_SERVER, "(AC): {ffffff}%s is suspected for damage hack #2", GetName(playerid));
    }

    if((GetPlayerTargetPlayer(playerid) == INVALID_PLAYER_ID && weaponid == WEAPON_PARACHUTE && amount > 10) || (weaponid == WEAPON_FLOWER && amount > 10) || (weaponid == WEAPON_NITESTICK && amount > 10)) {
        sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_SERVER, "(AC): {ffffff}%s is suspected for fake damage #1", GetName(playerid));
    }

    if(GetPlayerTargetPlayer(playerid) == INVALID_PLAYER_ID &&  amount > 10.0 && amount != 54.12 && weaponid == WEAPON_FIST) {
        sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_SERVER, "(AC): {ffffff}%s is suspected for fake damage #2", GetName(playerid));
    }

    if(GetPlayerTargetPlayer(playerid) == INVALID_PLAYER_ID && amount < 0) {
        sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_SERVER, "(AC): {ffffff}%s este suspect de Fake Damage #3.", GetName(playerid));
    }
    #endif
    if(weaponid == 0)
        amount = GetPunchLevel(playerid);

    if(weaponid == WEAPON_SNIPER)
        amount = 400.0;

    #if SERVER_ANTICHEAT
    if((GetPlayerTargetPlayer(playerid) == INVALID_PLAYER_ID && weaponid == WEAPON_PARACHUTE && amount > 10) || (weaponid == WEAPON_FLOWER && amount > 10) || (weaponid == WEAPON_NITESTICK && amount > 10)) {
        sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_RED, "(AC):{ffffff} %s este suspectat de Fake-Damage #1.", GetName(playerid));
    }   
    if(GetPlayerTargetPlayer(playerid) == INVALID_PLAYER_ID && weaponid == WEAPON_PARACHUTE && amount > 10) {
        sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_RED, "Anti-Cheat: %s a primit kick de la AdmBot, motiv: fake damage (%0.2f).", GetName(playerid), amount);
        va_SendClientMessageToAll(COLOR_LIGHTRED, "(AdmBot): %s was kicked for possible fake damage.", GetName(playerid));
        KickEx(playerid);
    }
    if(GetPlayerTargetPlayer(playerid) == INVALID_PLAYER_ID && amount > 10.0 && amount != 54.12 && weaponid == 0 && GetPlayerTargetPlayer(playerid) != damagedid) {
        sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_RED, "Anti-Cheat: %s a primit kick de la AdmBot, motiv: fake damage (%0.2f).", GetName(playerid), amount);
        va_SendClientMessageToAll(COLOR_LIGHTRED, "(AdmBot): %s was kicked for possible fake damage.", GetName(playerid));
    }
    if(GetPlayerTargetPlayer(playerid) == INVALID_PLAYER_ID && amount < 0) {
        sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_RED, "Anti-Cheat: %s a primit kick de la AdmBot, motiv: fake damage (%0.2f).", GetName(playerid), amount);
        va_SendClientMessageToAll(COLOR_LIGHTRED, "(AdmBot): %s was kicked for possible fake damage.", GetName(playerid));
        KickEx(playerid);
    }

    if(GetPlayerTargetPlayer(playerid) == INVALID_PLAYER_ID &&  amount < 0) {
        SendClientMessage(playerid, COLOR_WHITE, "Stop cheating (Global kill #1).");
        KickEx(playerid);
    }


    if(antiCheatDetectWrongDamage(playerid, damagedid, amount, weaponid)) 
        return true;
    #endif
    if(playerInfo[playerid][pGroup] != 11 || getPlayerHit(playerid) != damagedid) {
        if(!IsACop(playerid) || !isPlayerDuty(playerid)) {
            if(playerStaticVar[playerid][var_insafezone] || playerStaticVar[damagedid][var_insafezone]) {
                SetPlayerArmedWeapon(playerid, 0); 
                FreezePlayer(playerid, 3);

                return SendClientMessage(playerid, COLOR_GREY, "You cannot do damage in a safe-zone, you got freezed for 3 seconds.");
            }
        }
    }

    if(isPlayerInWar(playerid) && isPlayerInWar(damagedid)) {
        if(playerInfo[playerid][pGroup] == playerInfo[damagedid][pGroup] || !isPlayerOnTurf(playerid))
            return true;
    }

    if(isPlayerInPaint(playerid)) {
        if(getPlayerPaintID(playerid) == getPlayerPaintID(damagedid)) {
            PlayerPlaySound(playerid, 17802, 0.0, 0.0, 0.0);
        }
    }

    else if(isCop(playerid) && isPlayerDuty(playerid) && GetPlayerWeapon(playerid) == WEAPON_SILENCED) {
        if(isCop(damagedid)) 
            return sendError(playerid, "You can't taze a cop.");    

        if(playerStaticVar[damagedid][playerSleeping])
            return sendError(playerid, "This player is sleeping.");

        if(isPlayerTazed(damagedid))
            return sendError(playerid, "This player is already tazed.");

        if(GetDistanceBetweenPlayers(playerid, damagedid) < 15.0) {
            sendNearbyMessage(playerid, COLOR_PURPLE, "* %s tazed %s for 8 seconds.", GetName(playerid), GetName(damagedid));
            TogglePlayerControllableEx(damagedid, false);

            playerStaticVar[damagedid][isTazed] = true;

            defer playerTazer(damagedid);
        }
        return true;
    }

    if(damagedid != INVALID_PLAYER_ID && amount)
    {
        DamageFeedAddHitGiven(playerid, damagedid, amount, weaponid, bodypart);
        DamageFeedAddHitTaken(damagedid, playerid, amount, weaponid, bodypart);

        OnPlayerDamage(playerid, amount, damagedid, weaponid);
        CreateDamageTextLabel(playerid, Float:amount);
    }
    return true;
}

timer DestroyDamageTextLabel[300](playerid, labelDamage)
{
    DeletePlayer3DTextLabel(playerid,labelDamage);
    stop label_timer[playerid];
    label_timer[playerid] = Timer:0;
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, WEAPON:weaponid, bodypart)
{
    if(player_data[playerid][playerDeath])
        return true;

    if(issuerid == INVALID_PLAYER_ID)
    {
        OnPlayerDamage(INVALID_PLAYER_ID, amount, playerid, weaponid);
        DamageFeedUpdate(playerid, true);
    }
    return true;
}

function:DamageFeedUpdateEx(playerid)
{
    s_DamageFeedTimer[playerid] = -1;

    if (IsPlayerConnected(playerid)) {
        DamageFeedUpdate(playerid, true);
    }
}

static DamageFeedUpdate(playerid, bool:modified = false)
{
    new
        tick = GetTickCount();
    
    if(tick == 0)
        tick = 1;
    
    new
        lowest_tick = tick + 1;

    for (new i = 0, j = 0; i < sizeof(s_DamageFeedHitsGiven[]) - 1; i++) {
        if (!s_DamageFeedHitsGiven[playerid][i][e_Tick]) {
            break;
        }

        if (tick - s_DamageFeedHitsGiven[playerid][i][e_Tick] >= 3000) {
            modified = true;

            for (j = i; j < sizeof(s_DamageFeedHitsGiven[]) - 1; j++) {
                s_DamageFeedHitsGiven[playerid][j][e_Tick] = 0;
            }

            break;
        }

        if (s_DamageFeedHitsGiven[playerid][i][e_Tick] < lowest_tick) {
            lowest_tick = s_DamageFeedHitsGiven[playerid][i][e_Tick];
        }
    }

    for (new i = 0, j = 0; i < sizeof(s_DamageFeedHitsTaken[]) - 1; i++) {
        if (!s_DamageFeedHitsTaken[playerid][i][e_Tick]) {
            break;
        }

        if (tick - s_DamageFeedHitsTaken[playerid][i][e_Tick] >= 3000) {
            modified = true;

            for (j = i; j < sizeof(s_DamageFeedHitsTaken[]) - 1; j++) {
                s_DamageFeedHitsTaken[playerid][j][e_Tick] = 0;
            }

            break;
        }

        if (s_DamageFeedHitsTaken[playerid][i][e_Tick] < lowest_tick) {
            lowest_tick = s_DamageFeedHitsTaken[playerid][i][e_Tick];
        }
    }

    if (s_DamageFeedTimer[playerid] != -1) {
        KillTimer(s_DamageFeedTimer[playerid]);
    }

    if (tick - s_DamageFeedLastUpdate[playerid] < 250 && modified) {
        s_DamageFeedTimer[playerid] = SetTimerEx("DamageFeedUpdateEx", 250 - (tick - s_DamageFeedLastUpdate[playerid]), false, "i", playerid);
    } else {
        if (lowest_tick == tick + 1) {
            s_DamageFeedTimer[playerid] = -1;
            modified = true;
        } else {
            s_DamageFeedTimer[playerid] = SetTimerEx("DamageFeedUpdateEx", 3000 - (tick - lowest_tick) + 10, false, "i", playerid);
        }

        if (modified) {
            DamageFeedUpdateText(playerid);

            s_DamageFeedLastUpdate[playerid] = tick;
        }
    }
}

task check_weapon_holding[2004]()
{
    foreach(new i : Player)
    {
        if(GetPlayerSurfingVehicleID(i) != INVALID_VEHICLE_ID && GetPlayerWeapon(i) != 0)
        {
            SetPlayerArmedWeapon(i, WEAPON_FIST);
            SendClientMessage(i, -1, "You can't hold weapons on a vehicle!");
        }
    }
}

function:DamageFeedUpdateText(playerid)
{
    new buf[64 * 5] = "";

    for (new i = 0, weapon[32]; i < sizeof(s_DamageFeedHitsGiven[]) - 1; i++) {
        if (!s_DamageFeedHitsGiven[playerid][i][e_Tick]) {
            break;
        }

        if (s_DamageFeedHitsGiven[playerid][i][e_Weapon] == -1) {
            weapon = "Multiple";
        } else {
            DMG_GetWeaponNameEx(s_DamageFeedHitsGiven[playerid][i][e_Weapon], weapon);
        }

        if (s_DamageFeedHitsGiven[playerid][i][e_Issuer] == INVALID_PLAYER_ID) {
            format(
                buf,
                sizeof(buf),
                "%s%s +%.2f~n~",
                buf,
                weapon,
                s_DamageFeedHitsGiven[playerid][i][e_Amount] + 0.009
            );
        } else {
            format(
                buf,
                sizeof(buf),
                "%s%s - %s +%.2f in %s~n~",
                buf,
                s_DamageFeedHitsGiven[playerid][i][e_Name],
                weapon,
                s_DamageFeedHitsGiven[playerid][i][e_Amount] + 0.009,
                returnBodyPartName(s_DamageFeedHitsGiven[playerid][i][e_BodyPart])
            );
        }
    }

    if (buf[0]) {
        PlayerTextDrawSetString(playerid, damageTD[playerid][0], buf);
        PlayerTextDrawShow(playerid, damageTD[playerid][0]);
    } else {
        PlayerTextDrawHide(playerid, damageTD[playerid][0]);
    }

    buf = "";

    for (new i = 0, weapon[32]; i < sizeof(s_DamageFeedHitsTaken[]) - 1; i++) {
        if (!s_DamageFeedHitsTaken[playerid][i][e_Tick]) {
            break;
        }

        if (s_DamageFeedHitsTaken[playerid][i][e_Weapon] == -1) {
            weapon = "Multiple";
        } else {
            DMG_GetWeaponNameEx(s_DamageFeedHitsTaken[playerid][i][e_Weapon], weapon);
        }

        if (s_DamageFeedHitsTaken[playerid][i][e_Issuer] == INVALID_PLAYER_ID) {
            format(
                buf,
                sizeof(buf),
                "%s%s -%.2f~n~",
                buf,
                weapon,
                s_DamageFeedHitsTaken[playerid][i][e_Amount] + 0.009
            );
        } else {
            format(
                buf,
                sizeof(buf),
                "%s%s - %s -%.2f in %s~n~",
                buf,
                s_DamageFeedHitsTaken[playerid][i][e_Name],
                weapon,
                s_DamageFeedHitsTaken[playerid][i][e_Amount] + 0.009,
                returnBodyPartName(s_DamageFeedHitsGiven[playerid][i][e_BodyPart])
            );
        }
    }

    if (buf[0]) {
        PlayerTextDrawSetString(playerid, damageTD[playerid][1], buf);
        PlayerTextDrawShow(playerid, damageTD[playerid][1]);
    } else {
        PlayerTextDrawHide(playerid, damageTD[playerid][1]);
    }
}

static DamageFeedAddHitGiven(playerid, issuerid, Float:amount, weapon, bodypart)
{
    foreach(new i : Spectators)
    {
        if (Spectate[i] == playerid && i != playerid)
        {
            DamageFeedAddHit(s_DamageFeedHitsGiven[i], i, issuerid, amount, weapon, bodypart);
        }
    }

    DamageFeedAddHit(s_DamageFeedHitsGiven[playerid], playerid, issuerid, amount, weapon, bodypart);
}

static DamageFeedAddHitTaken(playerid, issuerid, Float:amount, weapon, bodypart)
{
    foreach(new i : Spectators)
    {
        if (Spectate[i] == playerid && i != playerid)
        {
            DamageFeedAddHit(s_DamageFeedHitsTaken[i], i, issuerid, amount, weapon,bodypart);
        }
    }

    DamageFeedAddHit(s_DamageFeedHitsTaken[playerid], playerid, issuerid, amount, weapon, bodypart);
}

static DamageFeedAddHit(arr[5][E_DAMAGE_FEED_HIT], playerid, issuerid, Float:amount, weapon, bodypart)
{
    new tick = GetTickCount();
    if (tick == 0) tick = 1;
    new idx = -1;

    for (new i = 0; i < sizeof(arr) - 1; i++) {
        if (!arr[i][e_Tick]) {
            break;
        }

        if (tick - arr[i][e_Tick] >= 3000) {
            DamageFeedRemoveHit(arr, i);
            break;
        }

        // Remove it and add it on top below
        if (arr[i][e_Issuer] == issuerid) {
            // Multiple weapons
            if (arr[i][e_Weapon] != weapon) {
                //weapon = -1;
            }

            amount += arr[i][e_Amount];
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        idx = 0;

        // Insert it at the top
        for (new i = sizeof(arr) - 1; i >= 1; i--) {
            arr[i] = arr[i - 1];
        }
    }

    arr[idx][e_Tick] = tick;
    arr[idx][e_Amount] = amount;
    arr[idx][e_Issuer] = issuerid;
    arr[idx][e_Weapon] = weapon;

    if(playerUndercover[issuerid])
        format(arr[idx][e_Name], MAX_PLAYER_NAME, "Unknown Hitman");

    else 
        GetPlayerName(issuerid, arr[idx][e_Name], MAX_PLAYER_NAME);

    arr[idx][e_BodyPart] = bodypart;
    DamageFeedUpdate(playerid, true);
}

static DamageFeedRemoveHit(arr[5][E_DAMAGE_FEED_HIT], idx)
{
    for (new i = 0; i < 5; i++) {
        if (i >= idx) {
            arr[i][e_Tick] = 0;
        }
    }
}

GetWeaponNameEx(weaponid)
{
    new weapString[32];
    if(weaponid > 46)
        weapString = "Unknown";

    else
       format(weapString, sizeof weapString, "%s", returnGunNames[weaponid]);
    
    return weapString;
}

update_player_health_armour(const playerid)
{
    if(playerInfo[playerid][pHUD][6])
        TextDrawShowForPlayer(playerid, healthIcon);

    player_data[playerid][playerHealth] = 100.0;
    player_data[playerid][playerArmour] = 0.0;

    update_health_bar(playerid, UPDATE_TYPE_HEALTH);
    update_health_bar(playerid, UPDATE_TYPE_ARMOUR);
    return true;
}

update_health_bar(const playerid, const update)
{
    switch(update)
    {
        case UPDATE_TYPE_HEALTH:
        {
            SetPlayerHealth(playerid, player_data[playerid][playerHealth]);

            if(playerInfo[playerid][pHUD][0])
            {
                if(player_data[playerid][playerHealth] > 32768)
                    PlayerTextDrawSetString(playerid, hudTD[playerid][0], "0");
                
                else
                    va_PlayerTextDrawSetString(playerid, hudTD[playerid][0], "%.0f", player_data[playerid][playerHealth]);

                switch(playerInfo[playerid][pHUD][0])
                {
                    case 1: PlayerTextDrawColour(playerid, hudTD[playerid][0], 0xC0C0C0FF);
                    case 2: PlayerTextDrawColour(playerid, hudTD[playerid][0], 0x990000FF);
                    case 3: PlayerTextDrawColour(playerid, hudTD[playerid][0], 0x00ff00FF);
                    case 4: PlayerTextDrawColour(playerid, hudTD[playerid][0], 0x0000ffFF);
                    case 5: PlayerTextDrawColour(playerid, hudTD[playerid][0], 0xffff00FF);
                }
                PlayerTextDrawShow(playerid, hudTD[playerid][0]);
            }
            else
                PlayerTextDrawHide(playerid, hudTD[playerid][0]);
        }
        case UPDATE_TYPE_ARMOUR:
        {
            SetPlayerArmour(playerid, player_data[playerid][playerArmour]);

            if(player_data[playerid][playerArmour] != 0 && playerInfo[playerid][pHUD][1])
            {
                va_PlayerTextDrawSetString(playerid, hudTD[playerid][1], "%.0f", player_data[playerid][playerArmour]);
                switch(playerInfo[playerid][pHUD][1])
                {
                    case 1: PlayerTextDrawColour(playerid, hudTD[playerid][1], 0xC0C0C0FF);
                    case 2: PlayerTextDrawColour(playerid, hudTD[playerid][1], 0x990000FF);
                    case 3: PlayerTextDrawColour(playerid, hudTD[playerid][1], 0x00ff00FF);
                    case 4: PlayerTextDrawColour(playerid, hudTD[playerid][1], 0x0000ffFF);
                    case 5: PlayerTextDrawColour(playerid, hudTD[playerid][1], 0xffff00FF);
                }
                PlayerTextDrawShow(playerid, hudTD[playerid][1]);
            }
            else
                PlayerTextDrawHide(playerid, hudTD[playerid][1]);
        }
    }
    return true;
}

hook OnGameModeInit()
{
    healthIcon = TextDrawCreate(610.555542, 65.851852, "hud:radar_hostpital");
    TextDrawLetterSize(healthIcon, 0.000000, 0.000000);
    TextDrawTextSize(healthIcon, 10.555536, 9.851860);
    TextDrawAlignment(healthIcon, 1);
    TextDrawColour(healthIcon, 0xFFFFFFFF);
    TextDrawUseBox(healthIcon, 0);
    TextDrawBoxColour(healthIcon, 0x80808080);
    TextDrawSetShadow(healthIcon, 0);
    TextDrawSetOutline(healthIcon, 0);
    TextDrawBackgroundColour(healthIcon, 0x000000FF);
    TextDrawFont(healthIcon, 4);
    TextDrawSetProportional(healthIcon, 1);
    TextDrawSetSelectable(healthIcon, 0);

    return true;
}
function:OnPlayerDamage(playerid, Float:amount, damagedid, reason)
{
    if(playerid == INVALID_PLAYER_ID || player_data[damagedid][playerDeath])
        return true;


    // Remove armor if the player has any
    if(player_data[damagedid][playerArmour] > 0)
    {
        player_data[damagedid][playerArmour] = player_data[damagedid][playerArmour] - amount;
        update_health_bar(damagedid, UPDATE_TYPE_ARMOUR);
    }
    else if (player_data[damagedid][playerArmour] == 0) {
        // Apply full damage to health
        player_data[damagedid][playerHealth] -= amount;
        update_health_bar(damagedid, UPDATE_TYPE_HEALTH);
    }
    // Check if player is dead
    if(player_data[damagedid][playerHealth] < 1)
    {
        whenPlayerDie(damagedid, playerid, reason);
        player_data[damagedid][playerDeath] = 1;
    }

    return true;
}


hook OnPlayerSpawn(playerid)
{   
    update_player_health_armour(playerid);
    player_data[playerid][playerDeath] = 0;

    DestroyDynamic3DTextLabel(DeadScris[playerid]);
    return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:checkhp(playerid, params[], help)
{
    check_admin

    new returnPlayer;
    if(sscanf(params, "u", returnPlayer))
        return sendSyntaxMessage(playerid, "/checkhp <Name/Playerid>");

    if(!IsPlayerConnected(returnPlayer))
        return sendError(playerid, "Player not connected.");

    va_SendClientMessage(playerid, -1, "%s's health: %0.2f", GetName(returnPlayer), player_data[returnPlayer][playerHealth]);
    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_SERVER, "{ffffff}("COLOR_SV_HEX"Staff Info{ffffff}): %s has used /checkhp on %s.", GetName(playerid), GetName(returnPlayer));
    return true;
}

///hook native prostule

forward public SetPlayerHealthEx(playerid, Float:health);
public SetPlayerHealthEx(playerid, Float:health)
{
    player_data[playerid][playerHealth] = health;
    update_health_bar(playerid, UPDATE_TYPE_HEALTH);
    return false;
}

forward public SetPlayerArmourEx(playerid, Float:armour);
public SetPlayerArmourEx(playerid, Float:armour)
{
    player_data[playerid][playerArmour] = armour;
    update_health_bar(playerid, UPDATE_TYPE_ARMOUR);
    return true;
}