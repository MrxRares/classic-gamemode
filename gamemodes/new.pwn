// ------------------------------------- //
//              Defines                  //
// ------------------------------------- //

#define TEST_SERVER true
#define SERVER_MYSQL true
#define SERVER_ANTICHEAT false
#define INITIALISE_GAMEMODE_DELAY false

#if INITIALISE_GAMEMODE_DELAY 
    #warning "OnGameModeInit initialise delay is activated."
#endif

#if TEST_SERVER
    #warning "Debug mode is active."
#endif

#define MAX_PLAYERS 10

#define SERVER_PASSWORD  "12"
#define SERVER_VERSION   "0.17.278"
#define SERVER_LANGUAGE  "RO/EN"

#define SERVER_WEBURL    "t4p.ro"
#define SERVER_NAME      "t4p.ro"
#define SERVER_NAME2     "Time4Play"
#define SERVER_DNS       "rpg.t4p.ro"

#define SERVER_HOSTNAME  "TIME4PLAY - BETA"

// ------------------------------------- //
//              Includes                 //
// ------------------------------------- //

#include <open.mp>

#include <crashdetect>
#include <sscanf2>
#include <streamer>
#include <a_mysql>

#define CGEN_MEMORY 190000

#include <fly>
#include <bigInt>
#include <easyDialog>
#include <serverColors>
#include <timestamptodate>
#include <a_zones>
#include <progress2>
#include <map-zones>
#include <geolocation>
#include <sort-array>
#include <vector>
#include <memory>

#pragma dynamic 92768
#define YSI_NO_HEAP_MALLOC

#include <YSI_Coding\y_inline>
#include <YSI_Coding\y_timers>
#include <YSI_Data\y_iterate>
#include <YSI_Visual\y_commands>
#include <YSI_Coding\y_malloc>
// ------------------------------------- //
//              Defines                  //
// ------------------------------------- //

native                                          WP_Hash(buffer[], len, const str[]);
#define INDEX_JOB 3
#define INDEX_PHONE 2

#define BODY_PART_HEAD                  9
#define BODY_PART_RIGHT_LEG             8
#define BODY_PART_LEFT_LEG              7
#define BODY_PART_RIGHT_ARM             6
#define BODY_PART_LEFT_ARM              5
#define BODY_PART_GROIN                 4
#define BODY_PART_TORSO                 3

#define MAX_PLAYERS_ON_LOGIN 20
#define MAX_ATMS (30 + 1)

#define COLOR_SV_HEX  "{9D72FF}"
timer _Kick[2000](playerid) return Kick(playerid);



AntiDeAMX()
{
	new a[][] =
	{
		"Unarmed (Fist)",
		"Brass K"
	};
	#pragma unused a
}

// ------------------------------------- //
//              Variables                //
// ------------------------------------- //

new 
    MySQL:SQL, g_MysqlRaceCheck[MAX_PLAYERS], 
    returnString[2048], serverTime[6], 
    serverBacktraces, serverMysqlErrors, serverWeather,
    
    //server_iterators
    Iterator: playersInQueue<MAX_PLAYERS>, 
    Iterator: playersInLogin<MAX_PLAYERS>,
    Iterator: displayFPS<MAX_PLAYERS>, 
    Iterator: playerWT<9999, MAX_PLAYERS>, 
    Iterator: Muted<MAX_PLAYERS>,  
    Iterator: premiumPlayers<MAX_PLAYERS>, 
    Iterator: blockedPlayers[MAX_PLAYERS]<MAX_PLAYERS>, 
    Iterator: streamedPlayers[MAX_PLAYERS]<MAX_PLAYERS>, 
    Iterator: playersInVehicle<MAX_PLAYERS>,

    //server_timers
    Timer: login_timer[MAX_PLAYERS], 
    Timer: timerLoginQueue, 
    Timer: case_timer[MAX_PLAYERS], 
    Timer: tutorial_timer[MAX_PLAYERS], 
    Timer: vending_timer[MAX_PLAYERS],
    

    //server_globals
    Text:loginQueue[4], 
    Text:SafezoneTD, 
    Text:PayDayTD[2], 
    Text:dmvTD[3], 
    Text:caseTD[7],
    Text:dateTD[3], 
    Text:dutyTD,
    PlayerBar: HUDProgress[MAX_PLAYERS],
    PlayerBar:PesteTD[MAX_PLAYERS],

    //server_others_things
    lastSentMessage[MAX_PLAYERS], 
    boolean:playerUndercover[MAX_PLAYERS],
    emergencyCall[MAX_PLAYERS], kickafk,
    iGMXTick, 
    iGMXTimer, 

    Map: phoneNumbers,
    gPlayerUsingLoopingAnim[MAX_PLAYERS],
    WantName[MAX_PLAYERS][MAX_PLAYER_NAME],
    restartReason[72],
    disconnectReason[][] = { {"timeout/crash"}, {"quit"}, {"banned/kicked"}, {"server restart"}, {"restart 2"} },
    monthName[12][] = { {"Jan"}, {"Feb"}, {"Mar"}, {"Apr"}, {"May"}, {"June"}, {"July"}, {"Aug"}, {"Sept"}, {"Oct"}, {"Nov"}, {"Dec"}
};

enum e_sStuff {
    turfs, wars, safeZones
}
new serverStuff[e_sStuff];

enum E_ATM_INFO
{
    idATM, Float: fATMPos[3], Float: fATMPosRot[3], rObjectId[3], ATMHealth, Text3D: ATMLabel, ATMRepairTime
};
new AtmInfo[MAX_ATMS][E_ATM_INFO], Iterator: destroyedATM<MAX_ATMS>, returnServerATMS;

new RadioNames[][] =
{
    {"{FF0000}Turn Off Radio"}, {"Radio Bandit Romania"}, {"Radio Taraf Romania"}, {"Radio HOT Romania"}, {"Radio Gangsta Dance"},
    {"Radio Gangsta Manele"}, {"Radio Popular"}, {"Radio Hit Romania"}, {"Radio Traditional Manele"}, {"Radio ClubMix"}, {"PiratFM Dubstep"}, {"PiratFM Rap"}, {"Radio Vocea Strazii"}
};

new RadioLinks[][] =
{
    {""}, {"http://live.radiobandit.ro:8000"}, {"http://radiotaraf.com/live.m3u"}, {"http://live.radiohot.ro:8000/"}, {"http://dance.radiogangsta.ro:8800"},
    {"http://live.radiogangsta.ro:8800"}, {"http://livemp3.radiopopular.ro:7777"}, {"http://www.radio-hit.ro/asculta.m3u"}, {"http://radio.org.ro/traditional-manele"}, {"http://live.radioclubmix.ro:9999"}, {"http://mytuner-radio.com/no/radio/piratfm-497650/"}, {"http://liveonlineradio.net/ro/pirat-fm"},
    {"http://radio.trap.fm/listen128.pls"}
};

new GunNames[][] =
{
    "Nothing", "Brass Knuckles", "Golf Club", "Nitestick", "Knife", "Baseball Bat", "Showel", "Pool Cue",
    "Katana", "Chainsaw", "Purple Dildo", "Small White Dildo", "Long White Dildo", "Vibrator", "Flowers",
    "Cane", "Grenade", "Tear Gas", "Molotov", "Vehicle Missile", "Hydra Flare", "Jetpack", "Glock",
    "Silenced Colt", "Desert Eagle", "Shotgun", "Sawn Off", "Combat Shotgun", "Micro UZI", "MP5", "AK47",
    "M4", "Tec9", "Rifle", "Sniper Rifle", "Rocket Launcher", "HS Rocket Launcher", "Flamethrower",
    "Minigun", "Satchel Charge", "Detonator", "Spraycan", "Fire Extinguisher", "Camera", "Nightvision",
    "Infrared Vision", "Parachute", "Fake Pistol"
};

static HudColors[][] =
{
    {"{FFFFFF}Disable"}, {"{C0C0C0}Grey"}, {"{990000}Red"}, {"{00ff00}Green"}, {"{0000ff}Blue"}, {"{ffff00}Yellow"}
};

enum e_adminColors {
    colorName[15],
    colorValue[9]
}

new const adminColors[][e_adminColors] = {
    {"White", "AFAFAF"},
    {"Cool Red", "E70031"},
    {"Dark Red", "AB0000"},
    {"Light Red", "FF6347"}
};


enum e_premiumColors {
    colorName[32],
    colorValue[9]
}

new const premiumNameColors[][e_premiumColors] = {
    {"White", "AFAFAF"}, 
    {"Blue", "4169E1"}, 
    {"Dark Purple", "662BBE"}, 
    {"Purple", "8422E0"},
    {"Yellow", "FFE600"}, 
    {"Green", "47C930"}, 
    {"Lime Green", "85FA70"}, 
    {"Dark Green", "2A5723"}, 
    {"Orange", "E6921E"}, 
    {"Black", "2E2D2B"}, 
    {"Pink", "BA38D1"}
};


// ------------------------------------- //
//              Enumerations             //
// ------------------------------------- //

enum e_jailTypeOptions {
    JAIL_TYPE_NONE,
    JAIL_TYPE_COP = 1,
    JAIL_TYPE_ADMIN = 2
}

enum ENUM_PLAYER_INFORMATIONS
{
    pSQLID, pName[MAX_PLAYER_NAME], pMoney, pMoneyStore, pAccount, pAccountLY, pAdmin, pHelper, pHouse, pSpawnLocation, pBusiness, pClan, pClanRank, pClanTagPos, pClanDays,
    pClanWarns, pGroup, pGroupRank, pGroupDays, pGroupWarns, pGroupPunish, pEmail[32], pGender, pSkin, pLevel, pWantedLevel,
    pOwnedSkins[311], pSkinType[311], pGascan, pPhoneNumber, pWalkieTalkie, pWTFreq, pPhoneCredit, pApartament, 
    pQuest[3], pQuestProgress[3], pQuestNeed[3], pJail, pExperience, pDrugs, pCocaine, pMushrooms, pMaterials, pCarSlots, pCarLic, pGunLic, pFlyLic, pBoatLic, pWarns,
    pNMuted, pPlayersHelped, pAdminWarns, pHelperWarns, e_jailTypeOptions:pJailType, pRobPoints, pPayDay, pPaintKills, pRaportScore[6], pRaportTime, pRaportStatus, pGuns[3], pHUD[13], pPremiumPoints, pPhone, pTutorial, pLanguage, pCases, pCasesKeys, pHiddenColor, pBlockedAcc,
    pMute, pReportMute, pQuestionMute, pQuestsUnlocked, pSerial[128], pAdminSerial[128], pJailTime, pMiningPoints,
    pTOGSurf, pTOGPay, pTOGFind, pPremiumExpire, pPremiumColor, pPremiumCount, pAdminColor, 

    //server_bools
    boolean:pPhoneBook, boolean:pPremiumAccount,

    //server_floats
    Float:pLastPosX, Float:pLastPosY, Float:pLastPosZ, Float:pHours,

    //server_timers
    Timer:pLoginTimer
};
new playerInfo[MAX_PLAYERS][ENUM_PLAYER_INFORMATIONS];

static resetPlayerEnum[ENUM_PLAYER_INFORMATIONS];

enum ENUM_STATIC_PLAYER_INFORMATIONS
{
    pQueue, pDrunkLevelLast, pFPS, playerLogged, pDialogItems[512], inHouse, inBusiness, inHQ,
    pTurfDisplayed, pCurrentTurf, gotAssistFrom[MAX_PLAYERS],
    useFly, pDialogSelected, newEmail, playerAFK, wantedTime,
    pStrSkins[1024], pStrType[1024], pTSkins, pLastVirtual, pLastInterior, adMessage[128], adTime, playerDiceID, playerDiceAmount,
    lastVehicle, lastAcVehicle,  pCheckpoint, freePlayer, freeAmount, livePlayer, inInterview, clanInvitePlayer, factionInvitePlayer, liveReporter,
    playerCuffed, tazerEquiped, taxiAccepted, medicAccepted, playerSleeping, targetFind, usedFind,
    b_ACTIVE_TIMER, var_jobtimer, var_contract_price, 
    enterToVehicle, speedBoost, lastChatMessage[144], requestTitle, requestDescription, requestTime, vehiclekey_id, vehiclekey_sqlid,  
    requestLocation, requestPrize, ticketPlayer, ticketPrice, currentCaseSpin, currentCaseWin, currentCaseWinAmount, tutorialStep, 
    examStep, examVeh, pVending, adminDuty, playerKillId, playerKillTime, playerHandsup, pCheatWarnings,
    
    //server_deelays   
    pDealerDelay, var_incall_delay, var_call_delay, var_sms_delay, 
    var_placecontract_delay, var_contract_delay, var_paintball_delay, 
    var_car_unstuck_delay, var_trucker_chat_delay, var_car_lock_delay,

    //server_bools
    boolean:b_PHONE_TURN, boolean:b_PHONE_SPEAKER, boolean:bountyBag, boolean:pFreqToggled,
    boolean:groupDuty, boolean:var_premium_tog, boolean:b_calls_turn, boolean:isTazed,
    
    //server_floats
    Float:DMInfo[3], Float:assistDamage[MAX_PLAYERS], Float:playTime, Float:afkTime, Float:pLastBizCoords[3], Float:pLastCoords[3],
    Float:pCheckpointPos[3],


    //Timers
    Timer:var_wanted_timer, Timer:var_jail_timer,
    //server_vars
    var_trucker_trailer,  var_trucker_cash_bonus,    var_courierstuff, var_courierhouse, var_jobobject, var_jobseconds,
    var_incall, var_calls, var_callfrom, 
    var_cocaine_times, var_mushrooms_times, var_drugs_times, var_addiction_procent, var_drug_used, var_selected_seeds,
    var_animations_loaded, 
    var_jobtime, var_last_sms_id, var_last_sms_sqlid, var_insafezone, var_mining_state, var_mining_checkpoint
};
new playerStaticVar[MAX_PLAYERS][ENUM_STATIC_PLAYER_INFORMATIONS];

static resetStaticEnum[ENUM_STATIC_PLAYER_INFORMATIONS];

enum e_pTD {
    PlayerText:SpeedTD, PlayerText:GymTD, PlayerText:releaseTD,
    PlayerText:moneyTD, PlayerText:kmTD, PlayerText:speedometerTD[2], 
    PlayerText:serverJobTD, PlayerText:PlayerFPS, PlayerText:AdminHUD, PlayerText:ReportsTD,
    PlayerText:Logo, PlayerText:GunPTD[2], PlayerText:warTD, PlayerText:PayDayPTD,  PlayerText: FindTD, 
    PlayerText:HudTD,  PlayerText:PestePTD, PlayerText:transportTD, PlayerText: drugs_td,
    PlayerText:dmvPTD, PlayerText:casePTD[13], PlayerText:wantedTD, PlayerText: spectateTD,
    PlayerText:Tutorial[2], PlayerText: undercoverTD, PlayerText:dealerPTD[2], PlayerText:StorePTD
}
new playerTextdraws[MAX_PLAYERS][e_pTD];

// ------------------------------------- //
//              Main                     //
// ------------------------------------- //

main()
{
    print("Time4Play Gamemode");
}

// ------------------------------------- //
//              Modules                  //
// ------------------------------------- //

new gDialog[1024], gString[1024];

#include "../module_inc/utils.inc"
#include "../module_inc/streamerVars.inc"
#include "../module_inc/serverAssests.inc"
#include "../module_inc/moneyCaching.inc"

#include "../module_inc/server/staffSystem.inc"
#include "../module_inc/damageCaching.inc"
#include "../module_inc/anticheats/serverAnticheat.inc"

#include "../module_inc/src/questSystem.inc"
#include "../module_inc/vehicleSystem.inc"
#include "../module_inc/server/houseSystem.inc"
#include "../module_inc/server/apartamentSystem.inc"
#include "../module_inc/server/businessSystem.inc"
#include "../module_inc/server/factionSystem.inc"
#include "../module_inc/server/clanSystem.inc"
#include "../module_inc/server/serverJobs.inc"
#include "../module_inc/server/trade.inc"
#include "../module_inc/src/paintballSystem.inc"

#include "../module_inc/server/auctions.inc"
#include "../module_inc/server/animations"
#include "../module_inc/server/emails"
#include "../module_inc/server/safezone"
#include "../module_inc/server/robSystem"


// ------------------------------------- //
//              Functions                //
// ------------------------------------- //


function:restartTimer() {
    iGMXTick --;
    if(iGMXTick == 0) {
        KillTimer(iGMXTimer);
        SetTimer("GameModeInitExitFunc", 1000, 0);
        return 1;
    }
    if(iGMXTick == 60) SendClientMessageToAll(COLOR_SERVER, "(( AdmBot: Serverul se va restarta intr-un minut. Jucatorii nu mai pot tasta comenzi. ))", iGMXTick);
    foreach(new x : Player) {
        va_GameTextForPlayer(x, "SERVERUL SE VA RESTARTA IN %s~n~MOTIV: %s", 1000, 3, calculateTime(iGMXTick), restartReason);
    }   
    return 1; 
}
function:GameModeInitExitFunc() 
{

    CallLocalFunction("GameModeExitFunc", "");
    return 1; 
}

function:GameModeExitFunc()
{
    stop timerLoginQueue;
    timerLoginQueue = Timer:0;
    KillTimer(iGMXTimer);
    GameModeExit();
}

function:checkAdminAccount(playerid) {

    if(Iter_Contains(serverAdmins, playerid) && playerInfo[playerid][pAdminSerial] != 0) {
        if(playerInfo[playerid][pAdminSerial] == playerInfo[playerid][pSerial])
            SendClientMessage(playerid, COLOR_SERVER, ""SERVER_NAME2" >> {ffffff}HWID accepted.");
        else {

            SendClientMessage(playerid, COLOR_SERVER, ""SERVER_NAME2" >> {ffffff}HWID not accepted, you got kicked.");
            KickEx(playerid);

        }    

    }
    return 1;   
}

function:PutPlayerInVehicleEx2(playerid, vehicleid, seatid) {
    playerStaticVar[playerid][enterToVehicle] = vehicleid;
    PutPlayerInVehicle(playerid, vehicleid, seatid);
    return true;
}
#define PutPlayerInVehicle PutPlayerInVehicleEx2


function:SetPlayerCheckpointEx(playerid, Float:centreX, Float:centreY, Float:centreZ, Float:radius)
{
    SetPlayerCheckpoint(playerid, centreX, centreY, centreZ, radius);
    playerStaticVar[playerid][pCheckpointPos][0] = centreX; playerStaticVar[playerid][pCheckpointPos][1] = centreY; playerStaticVar[playerid][pCheckpointPos][2] = centreZ;
    return 1;
}

function:applyAnimation(playerid, animlib[], animname[], Float:fDelta, loop, lockx, locky, freeze, time, forcesync) {
    ClearAnimations(playerid);
    ApplyAnimation(playerid, animlib, animname, fDelta, loop, lockx, locky, freeze, time, forcesync);
    SetTimerEx("ClearAnimations", 5000, false, "d", playerid);
    return true;
}

#define SetPlayerCheckpoint SetPlayerCheckpointEx
#define ApplyAnimation applyAnimation

function:loadServerSystems()
{
    mysql_tquery(SQL, "select * from `atms`", "loadATMs", "");
    mysql_tquery(SQL, "SELECT * FROM `server_safezones`", "load_safezones", "");
    return 1;
}

function:MySQLCheckAccount(checkName[])
{
    new query[128], escstr[30];
    strcpy(escstr, checkName);

    format(query, sizeof(query), "select `ID` from `users` where `name` = '%s';", escstr);
    new Cache: ab = mysql_query(SQL,query);
    if(!cache_num_rows())
    {
        cache_delete(ab);
        return false;
    }
    else
    {
        new intid;
        cache_get_value_name_int(0, "ID", intid);
        cache_delete(ab);
        return intid;
    }
}

YCMD:gotocp(playerid, params[], help)
{
    if(playerInfo[playerid][pAdmin] < 5)
        return sendError(playerid, "Nu ai gradul administrativ necesar.");

    if(playerStaticVar[playerid][pCheckpointPos][0] == -1)
        return sendError(playerid, "Nu ai un checkpoint activ.");

    SetPlayerPos(playerid, playerStaticVar[playerid][pCheckpointPos][0], playerStaticVar[playerid][pCheckpointPos][1], playerStaticVar[playerid][pCheckpointPos][2]);

    playerStaticVar[playerid][pCheckpointPos][0] = -1; playerStaticVar[playerid][pCheckpointPos][1] = -1; playerStaticVar[playerid][pCheckpointPos][2] = -1;

    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Staff Info{ffffff}): %s teleported to checkpoint.", GetName(playerid));
    return true;
}

YCMD:queuestats(playerid, params[], help)
{
    check_owner

    if(timerLoginQueue != Timer:0)
    {
        va_SendClientMessage(playerid, -1, "%d players in queue | Server waiting for connect: %d players", Iter_Count(playersInQueue), Iter_Count(playersInLogin));
    }
    else
    {
        va_SendClientMessage(playerid, -1, "QUEUE_TIMER IS NOT RUNNING. Queue will start if there are more than %d players waiting for login", MAX_PLAYERS_ON_LOGIN);
    }
    SendClientMessage(playerid, COLOR_TEAL, "---");
    return true;
}

timer loginPlayerFromQueue[1500]()
{
    if(Iter_Count(playersInLogin) >= MAX_PLAYERS_ON_LOGIN)
    {
        foreach(new i: playersInQueue) {
            format(returnString, sizeof returnString, "%d players in login queue", Iter_Count(playersInQueue));
            TextDrawSetString(loginQueue[3], returnString);

            format(returnString, sizeof returnString, "#%d", i);
            TextDrawSetString(loginQueue[0], returnString);

            TextDrawShowForPlayer(i, loginQueue[0]);
            TextDrawShowForPlayer(i, loginQueue[1]);
            TextDrawShowForPlayer(i, loginQueue[2]);
            TextDrawShowForPlayer(i, loginQueue[3]);
        }

        return true;
    }

    while(Iter_Count(playersInLogin) < MAX_PLAYERS_ON_LOGIN && (Iter_Count(playersInQueue) > 0))
    {
        new currentQPlayer = Iter_First(playersInQueue);
        TextDrawHideForPlayer(currentQPlayer, loginQueue[0]);
        TextDrawHideForPlayer(currentQPlayer, loginQueue[1]);
        TextDrawHideForPlayer(currentQPlayer, loginQueue[2]);
        TextDrawHideForPlayer(currentQPlayer, loginQueue[3]);
        SendClientMessage(currentQPlayer, COLOR_RED, "You will be logged in now.");

        mysql_format(SQL, returnString, sizeof returnString, "select * from `bans` where `name` = '%s' limit 1;", GetName(currentQPlayer));
        mysql_tquery(SQL, returnString, "BanCheck", "id", currentQPlayer, g_MysqlRaceCheck[currentQPlayer]);

        Iter_Remove(playersInQueue, currentQPlayer), Iter_Add(playersInLogin, currentQPlayer);
    }


    if(Iter_Count(playersInQueue) == 0)
    {
        stop timerLoginQueue;
        timerLoginQueue = Timer:0;
    }
    return true;
}

RemovePlayerFromQueue(playerid)
{
    if(Iter_Contains(playersInLogin, playerid))
        Iter_Remove(playersInLogin, playerid);

    if(Iter_Contains(playersInQueue, playerid))
        Iter_Remove(playersInQueue, playerid);

    return true;
}

function:BanCheck(playerid)
{
    TextDrawHideForPlayer(playerid, loginQueue[0]);
    TextDrawHideForPlayer(playerid, loginQueue[1]);
    TextDrawHideForPlayer(playerid, loginQueue[2]);
    TextDrawHideForPlayer(playerid, loginQueue[3]);
    GameTextForPlayer(playerid, "~g~checking ban status~w~ ...", 1000, 3);
    if(!cache_num_rows())
    {
        mysql_format(SQL, returnString, sizeof returnString, "select `Email` from `users` WHERE `Name` = '%s' LIMIT 1;", GetName(playerid));
        return mysql_tquery(SQL, returnString, "CheckUserAccount", "id", playerid, g_MysqlRaceCheck[playerid]);
    }

    new bannedBy[MAX_PLAYER_NAME], bannedReason[64], bannedDays, iTimestamp;
    for(new i; i < cache_num_rows(); i++)
    {
        cache_get_value_name_int(i, "bannedDays", bannedDays);
        cache_get_value_name_int(i, "iTimestamp", iTimestamp);

        cache_get_value_name(0, "bannedBy", bannedBy);
        cache_get_value_name(0, "bannedReason", bannedReason);

        if(iTimestamp + (bannedDays * 86400) < gettime())
        {
            if(bannedDays < 1 && bannedDays != -1)
                goto next_step;

            else
            {
                static playerKey;
                cache_get_value_name_int(i, "id", playerKey);

                mysql_format(SQL, returnString, sizeof returnString, "delete from `bans` where `ID` = '%d';", playerKey);
                mysql_tquery(SQL, returnString, "", "");

                mysql_format(SQL, returnString, sizeof returnString, "select `Email` from `users` WHERE `Name` = '%s' LIMIT 1;", GetName(playerid));
                mysql_tquery(SQL, returnString, "CheckUserAccount", "id", playerid, g_MysqlRaceCheck[playerid]);
                break;
            }
        }
        else
        {
            next_step:

            for(new x; x < 10; x ++)
                SendClientMessage(playerid, -1, "");

            va_SendClientMessage(playerid, COLOR_WARNING, "This account (%s) is banned!", GetName(playerid));
            GameTextForPlayer(playerid, "Banned", 5000, 2);

            va_SendClientMessage(playerid, COLOR_GENANNOUNCE, "Banned by: %s, reason: %s.", bannedBy, bannedReason);

            new dateFormat[6];
            returnDateFromTimestamp(iTimestamp + (bannedDays * 86400), dateFormat[0], dateFormat[1], dateFormat[2], dateFormat[3], dateFormat[4], dateFormat[5], 2);

            va_SendClientMessage(playerid, COLOR_GENANNOUNCE, "Ban will %s.", (!bannedDays ? ("never expire") : va_return("expire on %02d-%02d-%02d %02d:%02d:%02d", dateFormat[0], dateFormat[1], dateFormat[2], dateFormat[3], dateFormat[4], dateFormat[5])));

            SendClientMessage(playerid, COLOR_GENANNOUNCE, "Poti posta o cerere de unban pe panel daca consideri ca ai luat ban degeaba.");
            SendClientMessage(playerid, COLOR_GENANNOUNCE, "https://panel."SERVER_NAME"");
            KickEx(playerid);
            break;
        }
    }
    return true;
}

function:updatePlayerLogs(playerid, type)
{
    new vehicles = 0, queryString[512];
    if(cache_num_rows())
    {
        while(vehicles < cache_num_rows())
        {
            new vehicleDBId;
            cache_get_value_name_int(vehicles, "ID", vehicleDBId);
            format(queryString, sizeof queryString, "%s[car: %d] ", queryString, vehicleDBId);

            ++ vehicles;
        }
    }
    if(!type)
        insertPlayerLog(playerInfo[playerid][pSQLID], 0, "%s logged in, money $%s / $%s, %d pp [%d/%d vehicles] %s", playerInfo[playerid][pName], FormatNumber(playerInfo[playerid][pMoney]), GetPlayerBank(playerid), playerInfo[playerid][pPremiumPoints], vehicles, playerInfo[playerid][pCarSlots], queryString);

    else
        insertPlayerLog(playerInfo[playerid][pSQLID], 0, "%s logged out, money $%s / $%s, %d pp [%d/%d vehicles] %s", playerInfo[playerid][pName], FormatNumber(playerInfo[playerid][pMoney]), GetPlayerBank(playerid), playerInfo[playerid][pPremiumPoints], vehicles, playerInfo[playerid][pCarSlots], queryString);

    return true;
}

function:insertPlayerLog(playerid, giverid, const log_text[], va_args<>)
{
    new giverName[MAX_PLAYER_NAME];
    if(giverid)
        format(giverName, MAX_PLAYER_NAME, playerInfo[giverid][pName]);

    else
        giverName = "-";

    va_format(returnString, sizeof returnString, log_text, va_start<3>);

    mysql_format(SQL, returnString, sizeof returnString, "insert into `player_logs` (`userID`, `userName`, `giverName`, `logText`) values ('%d', '%s', '%s', '%s');", playerid, playerInfo[playerid][pName], giverName, returnString);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

ShowLoginDialog(playerid, type)
{
    new dialogString[256];
    if(!type)
    {
        format(dialogString, sizeof dialogString, "{ffffff}Welcome, %s, on "SERVER_NAME2" RPG!\nPlease enter your password below to login.", GetName(playerid));
        Dialog_Show(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", dialogString, "Login", "Cancel");
    }
    else
    {
        format(dialogString, sizeof dialogString, "{ffffff}Welcome, %s, on "SERVER_NAME2" RPG!\nPlease enter your desired password to register.", GetName(playerid));
        Dialog_Show(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Register", dialogString, "Register", "Cancel");
    }
    return true;
}

function:checkPremiumExpire() {
    new const rows = cache_num_rows();

    if(!rows)
        return 1;

    for(new i = 0, playerid; i < rows; ++i) {


        cache_get_value_name_int(0, "ID", playerid);
        if(playerid != -1) {
            playerInfo[playerid][pPremiumAccount] = playerStaticVar[playerid][var_premium_tog] = false;

            playerInfo[playerid][pPremiumColor] = 0;

            playerInfo[playerid][pPremiumExpire] = -1;

            SendClientMessage(playerid, COLOR_YELLOW, "* Contul tau premium a expirat, il poti cumpara din nou, folosind comanda /shop -> Premium account.");
        }
    }

    mysql_tquery(SQL, "UPDATE `users` SET `PremiumAccount` = 0, `PremiumColor` = 0, `PremiumExpire` = -1 WHERE `PremiumAccount` = 1 AND `PremiumExpire` < UNIX_TIMESTAMP()");
    return 1;
}

function:checkAccountFromDatabase(playername[]) 
{
    new string22[40];
    strcpy(string22, playername);

    format(returnString, sizeof(returnString), "SELECT `id` FROM users WHERE `name` = '%s'", string22);
    new Cache: ab = mysql_query(SQL, returnString);

    if(cache_num_rows() == 0)
    {
        cache_delete(ab);
        return 0;
    }
    else
    {
        new intid;
        cache_get_value_name_int(0, "id", intid);
        cache_delete(ab);
        return intid;
    }
}

function:updatePlayerTogS(playerid) {

    playerInfo[playerid][pTOGFind] = 1;
    playerInfo[playerid][pTOGPay] = 1;
    playerInfo[playerid][pTOGSurf] = 1;
    playerStaticVar[playerid][pFreqToggled] = true;

    return true;
}

getDateTime() {
    new 
        hour, minute, second,
        year, month, day,
        str[24];

    gettime(hour, minute, second); 
    getdate(year, month, day);
    format(str, sizeof str, "%02d:%02d:%02d %02d/%02d/%d", hour, minute, second, day, month, year);
    return str;
}

function:WhenPlayerLoggedIn(playerid)
{
    GameTextForPlayer(playerid, "~g~LOGGING IN ~w~...", 1000, 3);

    if(!cache_num_rows())
    {
        SetPVarInt(playerid, "loginTries", GetPVarInt(playerid, "loginTries") + 1);
        if(GetPVarInt(playerid, "loginTries") >= 3)
        {
            new IPAddress[16];
            GetPlayerIp(playerid, IPAddress, 16);
            SendClientMessage(playerid, COLOR_RED, "You got kicked because you lost all your login tries.");
			sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Staff Info{ffffff}): %s has been kicked for 2 failed logins(on account %s).", IPAddress, GetName(playerid));
            return KickEx(playerid);
        }
        else
        {
            va_SendClientMessage(playerid, COLOR_SERVER, "Incorrect password. You have %d remaining login attempts left.", 3 - GetPVarInt(playerid, "loginTries"));
            ShowLoginDialog(playerid, 0);

            RegisterCamera(playerid);
        }
    }
    else
    {
        new loadString[512];

        stop login_timer[playerid];
        login_timer[playerid] = Timer:0;

        RemovePlayerFromQueue(playerid);
        DeletePVar(playerid, "loginTries");

        SetPlayerColor(playerid, COLOR_WHITE_T);
        playerStaticVar[playerid][playerLogged] = 1;

        cache_get_value_name_int(0, "ID", playerInfo[playerid][pSQLID]);

        cache_get_value_name(0, "Name", playerInfo[playerid][pName]);
        SetPlayerName(playerid, playerInfo[playerid][pName]);

        cache_get_value_name(0, "Email", playerInfo[playerid][pEmail]);
        cache_get_value_name(0, "gpci", playerInfo[playerid][pSerial]);
        cache_get_value_name(0, "hwid", playerInfo[playerid][pAdminSerial]);

        cache_get_value_name_int(0, "BankLY", playerInfo[playerid][pAccountLY]);
        cache_get_value_name_int(0, "Bank", playerInfo[playerid][pAccount]);
        cache_get_value_name_int(0, "Money", playerInfo[playerid][pMoney]);
        cache_get_value_name_int(0, "MoneyStore", playerInfo[playerid][pMoneyStore]);


        ResetPlayerMoney(playerid);
        GivePlayerMoney(playerid, playerInfo[playerid][pMoney]);

        cache_get_value_name_float(0, "Hours", playerInfo[playerid][pHours]);

        cache_get_value_name_int(0, "Gender", playerInfo[playerid][pGender]);
        cache_get_value_name_int(0, "Language", playerInfo[playerid][pLanguage]);

        cache_get_value_name_int(0, "SpawnLocation", playerInfo[playerid][pSpawnLocation]);

        cache_get_value_name_int(0, "PremiumPoints", playerInfo[playerid][pPremiumPoints]);

        cache_get_value_name_int(0, "HiddenColor", playerInfo[playerid][pHiddenColor]);
        cache_get_value_name_int(0, "Cases", playerInfo[playerid][pCases]);
        cache_get_value_name_int(0, "CaseKeys", playerInfo[playerid][pCasesKeys]);
        cache_get_value_name_int(0, "iPhone", playerInfo[playerid][pPhone]);

        cache_get_value_name_int(0, "NMuted", playerInfo[playerid][pNMuted]);
        cache_get_value_name_int(0, "BlockedAcc", playerInfo[playerid][pBlockedAcc]);
        cache_get_value_name_int(0, "Warn", playerInfo[playerid][pWarns]);

        cache_get_value_name_int(0, "AdminWarns", playerInfo[playerid][pAdminWarns]);
        cache_get_value_name_int(0, "HelperWarns", playerInfo[playerid][pHelperWarns]);
        cache_get_value_name_int(0, "PlayersHelped", playerInfo[playerid][pPlayersHelped]);
        cache_get_value_name_int(0, "robPoints", playerInfo[playerid][pRobPoints]);

        static muteTime[MAX_PLAYERS], reportMute[MAX_PLAYERS], questionMute[MAX_PLAYERS];
        cache_get_value_name_int(0, "Mute", muteTime[playerid]);
        cache_get_value_name_int(0, "ReportMute", reportMute[playerid]);
        cache_get_value_name_int(0, "QuestionMute", questionMute[playerid]);

        playerInfo[playerid][pMute] =  (gettime() + muteTime[playerid]);
        playerInfo[playerid][pReportMute] = (gettime() + reportMute[playerid]);
        playerInfo[playerid][pQuestionMute] = (gettime() + questionMute[playerid]);

        cache_get_value_name_int(0, "Experience", playerInfo[playerid][pExperience]);
        cache_get_value_name_int(0, "Level",  playerInfo[playerid][pLevel]);

        cache_get_value_name_int(0, "PayDay", playerInfo[playerid][pPayDay]);
        cache_get_value_name_int(0, "PaintKills", playerInfo[playerid][pPaintKills]);
        cache_get_value_name_int(0, "RaportTime", playerInfo[playerid][pRaportTime]);
        cache_get_value_name_int(0, "RaportIncompleted", playerInfo[playerid][pRaportStatus]);


        SetPlayerScore(playerid, playerInfo[playerid][pLevel]);

        cache_get_value_name(0, "HudOptions", loadString);
        sscanf(loadString, "a<i>[13]", playerInfo[playerid][pHUD]);


        cache_get_value_name_int(0, "Phone", playerInfo[playerid][pPhoneNumber]);
        cache_get_value_name_bool(0, "PhoneBook", playerInfo[playerid][pPhoneBook]);


        cache_get_value_name_bool(0, "PremiumAccount", playerInfo[playerid][pPremiumAccount]);
        cache_get_value_name_int(0, "PremiumExpire", playerInfo[playerid][pPremiumExpire]);
        cache_get_value_name_int(0, "PremiumCount", playerInfo[playerid][pPremiumCount]);
        cache_get_value_name_int(0, "PremiumColor", playerInfo[playerid][pPremiumColor]);
        cache_get_value_name_int(0, "AdminColor", playerInfo[playerid][pAdminColor]);

        cache_get_value_name_int(0, "PhoneCredit", playerInfo[playerid][pPhoneCredit]);

        cache_get_value_name_int(0, "gasCan", playerInfo[playerid][pGascan]);
        cache_get_value_name_int(0, "WalkieTalkie", playerInfo[playerid][pWalkieTalkie]);
        cache_get_value_name_int(0, "WTFreq", playerInfo[playerid][pWTFreq]);

        SetSpawnInfo(playerid, NO_TEAM, 36, 1564.5190,-2167.1038,13.5839,178.0963, WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FIST, 0);
        SpawnPlayer(playerid);

        cache_get_value_name_int(0, "Tutorial", playerInfo[playerid][pTutorial]);


        if(!playerInfo[playerid][pTutorial])
        {

            RegisterCamera(playerid);

            Dialog_Show(playerid, DIALOG_REGISTER_ONE, DIALOG_STYLE_MSGBOX, "Language", "Alege limba in care vrei sa fie afisate mesajele de pe server.\nChoose the language that you speak.", "Romana", "English");
        }
        else
        {
            gpci(playerid, playerInfo[playerid][pSerial], 128);
            mysql_format(SQL, returnString, sizeof returnString, "update users set `gpci` = '%s' WHERE `Name` = '%s'", playerInfo[playerid][pSerial], playerInfo[playerid][pName]);
            mysql_tquery(SQL, returnString, "", "");
            clearChat(playerid, 20);

            playerStaticVar[playerid][pCheckpointPos][0] = -1; playerStaticVar[playerid][pCheckpointPos][1] = -1; playerStaticVar[playerid][pCheckpointPos][2] = -1;

            if(playerInfo[playerid][pPremiumAccount]) {
                Iter_Add(premiumPlayers, playerid);

                SendClientMessage(playerid, COLOR_CLIENT, "You are an premium account user.");
            }

            new afkStaff[2];
            if(playerInfo[playerid][pAdmin])
            {
                foreach(new i : serverAdmins)
                    if(playerStaticVar[i][playerAFK]) afkStaff[0] ++;

                Iter_Add(serverAdmins, playerid);
                if(serverMysqlErrors <= 0 && serverBacktraces <= 0)
                    va_SendClientMessage(playerid, -1, "You are a level %d admin. There are %d admins online (%d afk) [no problems were found].", playerInfo[playerid][pAdmin], Iter_Count(serverAdmins), afkStaff[0]);
                else
                    va_SendClientMessage(playerid, -1, "You are a level %d admin. There are %d admins online (%d afk) [%d backtraces and %d mysql errors sincer last restart].", playerInfo[playerid][pAdmin], Iter_Count(serverAdmins), afkStaff[0], serverBacktraces, serverMysqlErrors);
            }
            if(playerInfo[playerid][pHelper])
            {
                foreach(new i : serverHelpers)
                    if(playerStaticVar[i][playerAFK]) afkStaff[1] ++;

                Iter_Add(serverHelpers, playerid);
                va_SendClientMessage(playerid, -1, "You are a level %d helper. There are %d helpers online (%d afk).", playerInfo[playerid][pHelper], Iter_Count(serverHelpers), afkStaff[1]);
            }
            if(playerInfo[playerid][pHelper] > 1 || playerInfo[playerid][pAdmin] > 1)
            {
                sendStaffMessage(STAFF_TYPE_ADMIN, 0xbf891dFF, "(B) HelloBot: %s has just logged in.", GetName(playerid));
                va_SendClientMessage(playerid, 0xbf891dFF, "Admin MOTD: %s", amotd_message);
            }
            checkAdminAccount(playerid);
            if(playerInfo[playerid][pGroup])
            {
                Iter_Add(factionMembers<playerInfo[playerid][pGroup]>, playerid);
                sendGroupMessage(playerInfo[playerid][pGroup], COLOR_GENANNOUNCE, "(Group) {FFFFFF}%s from your group has just logged in.", GetName(playerid));

                if(strlen(groupInfo[playerInfo[playerid][pGroup]][groupMotd]) > 1)
                    va_SendClientMessage(playerid, COLOR_GENANNOUNCE, "(Group) MOTD: {FFFFFF}%s", groupInfo[playerInfo[playerid][pGroup]][groupMotd]);
            }


			if(factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_POLICE)
			    Iter_Add(departmentsMembers, playerid);

            if(playerInfo[playerid][pClan])
            {
                Iter_Add(clanMembers<playerInfo[playerid][pClan]>, playerid);
                sendClanMessage(playerInfo[playerid][pClan], COLOR_GENANNOUNCE, "(Clan) {FFFFFF}%s from your clan has just logged in.", GetName(playerid));

                if(strlen(clanInfo[playerInfo[playerid][pClan]][clanMotd]) > 1)
                    va_SendClientMessage(playerid, COLOR_GENANNOUNCE, "(Clan) MOTD: {FFFFFF}%s", clanInfo[playerInfo[playerid][pClan]][clanMotd]);
            }
            if(playerInfo[playerid][pWantedLevel]) {
                setPlayerWanted(playerid, playerInfo[playerid][pWantedLevel], playerStaticVar[playerid][wantedTime]);

                SendClientMessage(playerid, COLOR_RED, "Esti inca urmarit de politie. Nivelul de wanted ti-a fost restaurat.");
            }

            new const warID = getPlayerFactionWar(playerid); 
            if(warID != -1)
            {
                if(!playerInfo[playerid][pWantedLevel] && !Iter_Contains(jailedPlayers, playerid)) {
                    SendClientMessage(playerid, COLOR_GREEN2, "retrieving your war informations ...");

                    mysql_format(SQL, returnString, sizeof returnString, "SELECT `warPlayerTurfTime`, `warPlayerKills`, `warPlayerDeaths` FROM `users_wars` WHERE `warPlayerID` = %d AND `warID` = %d LIMIT 1", playerInfo[playerid][pSQLID], getFactionWarID(playerInfo[playerid][pGroup]));
                    mysql_tquery(SQL, returnString, "loadPlayerWarInfo", "dd", playerid, playerInfo[playerid][pSQLID]);

                    playerInfo[playerid][pSpawnLocation] = 22;

                    insertPlayerIntoWar(playerid, warID);

                    SendClientMessage(playerid, COLOR_CLIENT, "War: You were transferred in another virtual world and spawned to gang HQ because you are in war.");
                }
                else {
                    SendClientMessage(playerid, COLOR_CLIENT, "War: Your gang is in a war, but you are not transferred because you are wanted/jailed.");
                }
            }
        }

        ShowDailyQuests(playerid);
        updatePlayerTogS(playerid);

        if(playerInfo[playerid][pHUD][2]) {
            Iter_Add(displayFPS, playerid);
        }


        if(playerInfo[playerid][pWTFreq])
            Iter_Add(playerWT<playerInfo[playerid][pWTFreq]>, playerid);

        if(playerInfo[playerid][pPhoneNumber] != 0)
            MAP_insert_val_val(phoneNumbers, playerInfo[playerid][pPhoneNumber], playerid);


        if(playerInfo[playerid][pHUD][5])
            updateLevelProgress(playerid);

        if(playerInfo[playerid][pHUD][9])
            TextDrawShowForPlayer(playerid, healthIcon);

        mysql_format(SQL, returnString, sizeof returnString, "select `iReadStatus` from `emails` where `iPlayer` = '%d' and `iReadStatus` = '0';", playerInfo[playerid][pSQLID]);
        mysql_tquery(SQL, returnString, "show_email_notification", "i", playerid);

        if(playerInfo[playerid][pLevel] >= 3) {
            mysql_format(SQL, returnString, sizeof returnString, "select * from `personalvehicles` where `Owner` = '%e'", playerInfo[playerid][pName]);
            mysql_tquery(SQL, returnString, "loadPersonalVehicles", "d", playerid);
        }
        mysql_format(SQL, returnString, sizeof returnString, "SELECT * FROM `accs` WHERE `Userid`='%d' LIMIT 20", playerInfo[playerid][pSQLID]);
        mysql_tquery(SQL, returnString, "loadPlayerAccs", "i", playerid);

        mysql_format(SQL, returnString, sizeof returnString, "update `users` set `lastOnlineTime` = '0', `LastLogin` = '%e', `Status` = '1' where `id` = '%d';", getDateTime(), playerInfo[playerid][pSQLID]);
        mysql_tquery(SQL, returnString, "", "");

        mysql_format(SQL, returnString, sizeof returnString, "select * from `personalvehicles` where `Owner` = '%e' order by `id` asc;", playerInfo[playerid][pName]);
        mysql_tquery(SQL, returnString, "updatePlayerLogs", "ii", playerid, 0);


        if(!strmatch(playerInfo[playerid][pEmail], "email@yahoo.com"))
        {
            mysql_format(SQL, returnString, sizeof returnString, "select * from `allowedcomputers` where `playerSQL` = '%d';", playerInfo[playerid][pSQLID]);
            mysql_tquery(SQL, returnString, "AllowPlayerToConnect", "i", playerid);
        }

        DeletePVar(playerid, "now_login");
    }
    return true;
}

// timer proceed_tutorial[6000](playerid)
// {
//
//     va_PlayerTextDrawSetString(playerid, playerTextdraws[playerid][Tutorial][1], "~w~Step: ~r~%d~w~/~r~7", playerStaticVar[playerid][tutorialStep]);
//     PlayerTextDrawShow(playerid, playerTextdraws[playerid][Tutorial][1]);
//     PlayerTextDrawShow(playerid, playerTextdraws[playerid][Tutorial][0]);

//     switch(playerStaticVar[playerid][tutorialStep])
//     {
//         case 1:
//         {
//             SetPlayerPos(playerid, 366.0407, -1892.6018, 1.6999);
//             SetPlayerCameraPos(playerid, 558.3660, -1964.6206, 85.2497);
//             SetPlayerCameraLookAt(playerid, 419.0543, -1888.1879, 23.8108);

//             clearChat(playerid);
//             va_SendClientMessage(playerid, -1, "Salut, %s! Bun venit pe serverul "SERVER_NAME2" RPG!", GetName(playerid));
//             va_SendClientMessage(playerid, -1, "In momentul de fata sunt %d/100 jucatori conectati pe server.", Iter_Count(Player));
//             SendClientMessage(playerid, -1, "In continuare iti vom prezenta cateva informatii despre despre serverul "SERVER_NAME2".");
//         }
//         case 2:
//         {
//             SetPlayerPos(playerid, 1223.5475,-1881.3280,30.6971);
//             SetPlayerCameraPos(playerid, 1199.0161,-1862.7023,46.3864);
//             SetPlayerCameraLookAt(playerid, 1220.9999,-1838.0793,29.3740);

//             clearChat(playerid);
//             SendClientMessage(playerid, -1, "Aici poti da examen pentru a obtine permisul de conducere.");
//             SendClientMessage(playerid, -1, "Nu stii cum sa ajungi aici? Nicio problema. Poti folosi comanda /gps si apoi 'DMV' din lista.");
//             SendClientMessage(playerid, -1, "Comanda /gps poate fi folosita si pentru a afla unde sunt alte locatii importante de pe server.");
//             SendClientMessage(playerid, -1, "Dupa ce vei lua examenul vei primi permisul de conducere, vei putea conduce masini, dar nu avioane sau barci.");
//             SendClientMessage(playerid, -1, "Cand vei avansa in level vei putea cumpara si licenta pentru a putea conduce un avion, o barca sau pentru a cumpara arme.");
//             SendClientMessage(playerid, -1, "Licentele vor expira dupa 100 ore de joc si va trebui sa dai examenele din nou pentru a le obtine.");
//         }
//         case 3:
//         {
//             SetPlayerPos(playerid, 2222.0063, -2244.5569, 55.5668);
//             InterpolateCameraPos(playerid, 2271.758301, -2201.653809, 55.910271, 2174.9409, -2289.9351, 16.5469, 8000);
//             InterpolateCameraLookAt(playerid, 2209.5286, -2218.1633, 18.5469, 2190.4875, -2231.3840, 13.5469, 8000);

//             clearChat(playerid);
//             SendClientMessage(playerid, -1, "Pe server ai disponibile mai multe joburi, unul dintre ele este jobul Courier.");
//             SendClientMessage(playerid, -1, "La acest job trebuie sa livrezi colete in diverse locatii de pe server.");
//             SendClientMessage(playerid, -1, "Pentru a vedea o lista cu joburile de pe server, foloseste comanda /jobs.");
//             SendClientMessage(playerid, -1, "La toate joburile se castiga la fel de multi bani, cu exceptia jobului de detectiv care este un job non-profit.");
//             SendClientMessage(playerid, -1, "Poti testa toate joburile pentru a testa care iti place mai mult.");
//             SendClientMessage(playerid, -1, "Daca te plictisesti de un job, vei putea folosi comanda /quitjob pentru a renunta la el oricand.");
//         }
//         case 4:
//         {
//             SetPlayerPos(playerid, 1504.0387,-1664.6865,14.0031);
//             SetPlayerCameraPos(playerid, 1506.8416,-1697.2029,38.8016);
//             SetPlayerCameraLookAt(playerid, 1533.4709,-1680.9561,29.7199);

//             clearChat(playerid);
//             SendClientMessage(playerid, -1, "Politistii sunt impartiti in 4 factiuni: LSPD, LVPD, FBI si National Guard.");
//             SendClientMessage(playerid, -1, "Vei putea aplica la una din cele 5 factiuni de politisti incepand cu level 7.");
//             SendClientMessage(playerid, -1, "La level 5 vei putea aplica deasemenea la factiunile Taxi, Paramedic, News Reporters, School Instructors.");
//             SendClientMessage(playerid, -1, "La factiunea Hitman Agency vei putea aplica incepand cu level 7.");
//             SendClientMessage(playerid, -1, "Poti aplica la factiuni pe panel-ul nostru: panel."SERVER_NAME".");
//             SendClientMessage(playerid, -1, "Cei care fac parte din factiuni primesc salarii saptamanale in functie de activitatea lor pe server.");
//         }
//         case 5:
//         {
//             SetPlayerPos(playerid, 2677.1677, -1688.3624, 35.4970);
//             InterpolateCameraPos(playerid, 2683.977051, -1620.485596, 38.325546, 2677.060303, -1689.432007, 15.137304, 2000);
//             InterpolateCameraLookAt(playerid, 2677.844238, -1690.039551, 15.009771, 2695.6211, -1704.2074, 11.8438, 2000);

//             clearChat(playerid);
//             SendClientMessage(playerid, -1, "Sistemul de curse este unul din sistemele speciale ale serverului nostru.");
//             SendClientMessage(playerid, -1, "Acest sistem permite jucatorilor sa se intreaca pe unul din cele 6 trasee prestabilite de catre administratie.");
//             SendClientMessage(playerid, -1, "Pentru dinamizarea actiunii se vor putea modifica parametrii de tip: vehicul, traseu, coliziuni.");
//         }
//         case 6:
//         {
//             SetPlayerPos(playerid, 2049.9509, 1008.4241, 33.2915);
//             InterpolateCameraPos(playerid, 2058.845459, 1009.434753, 105.077316, 2049.950684, 1008.424072, 13.290329, 2000);
//             InterpolateCameraLookAt(playerid, 2058.370605, 1009.429626, 104.197266, 2048.952637, 1008.429565, 13.354404, 2000);

//             clearChat(playerid);
//             SendClientMessage(playerid, -1, "Casinoul din orasul Las Venturas permite amatorilor jocurilor de noroc sa-si incerce norocul la unul din cele trei tipuri de joc.");
//             SendClientMessage(playerid, -1, "Daca te crezi norocos te asteptam la casino, dar gandeste-te ca poti pierde acei bani!");
//         }
//         case 7:
//         {
//             SetPlayerPos(playerid, 1133.0504, -2038.4034, 69.0971);
//             InterpolateCameraPos(playerid, 1236.255981, -877.239319, 136.213104, 1545.230103, -867.936890, 123.859207, 10000);
//             InterpolateCameraLookAt(playerid, 1237.187012, -877.038391, 135.908615, 1544.404541, -867.445557, 123.581573, 10000);

//             clearChat(playerid);
//             SendClientMessage(playerid, -1, "Ai ajuns la sfarsitul tutorialului. Daca ai fost atent ar trebui sa stii cateva lucruri despre serverul nostru.");
//             SendClientMessage(playerid, -1, "Daca ai intrebari legate de joc, foloseste /n si helperii de pe server vor incerca sa te ajute.");
//             SendClientMessage(playerid, -1, "Distractie placuta pe serverul nostru!");
//         }

//         case 8:
//         {
//             stop tutorial_timer[playerid];
//             tutorial_timer[playerid] = Timer:0;

//             PlayerTextDrawHide(playerid, playerTextdraws[playerid][Tutorial][1]);
//             PlayerTextDrawHide(playerid, playerTextdraws[playerid][Tutorial][0]);

//             playerInfo[playerid][pTutorial] = 1;
//             GivePlayerCash(playerid, 15000);

//             //playerInfo[playerid][pHouse] = 1 + random(returnServerHouses - 1);
//             //playerInfo[playerid][pSpawnLocation] = 1;
//             playerInfo[playerid][pCarLic] = 24;

//             // va_SendClientMessage(playerid, COLOR_GREY, "Ai primit chirie la casa %d si licenta de condus timp de 24 de ore pentru a evita lagul.",  playerInfo[playerid][pHouse]);

//             TogglePlayerSpectating(playerid, false);
//             SpawnPlayer(playerid);

//             GivePlayerQuests(playerid);
//             ShowDailyQuests(playerid);
//             mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Tutorial` = '1', `SpawnLocation` = '1', `Money` = '%d', `House` = '%d', `DrivingLicence` = '24' where `id` = '%d';", GetPlayerCash(playerid), playerInfo[playerid][pHouse], playerInfo[playerid][pSQLID]);
//             mysql_tquery(SQL, returnString, "", "");
//         }
//     }
//     playerStaticVar[playerid][tutorialStep] ++;
//     return true;
// }

Dialog:DIALOG_REGISTER_ONE(playerid, response, listitem, inputtext[])
{
    if(response)
    {
        SendClientMessage(playerid, -1, "Limba setata: romana.");
        SendClientMessage(playerid, -1, "[EN] To set the language to English, use /eng.");
        SendClientMessage(playerid, COLOR_YELLOW, "Alege sexul caracterului tau.");

        Dialog_Show(playerid, DIALOG_REGISTER_TWO, DIALOG_STYLE_LIST, "Alege sexul caracterului", "Barbat\nFemeie", "Alege", "");
    }
    else
    {
        SendClientMessage(playerid, -1, "Language set to english.");
        SendClientMessage(playerid, -1, "[RO] Pentru a seta limba romana foloseste /ro.");
        SendClientMessage(playerid, COLOR_YELLOW, "Choose your gender of your character.");

        Dialog_Show(playerid, DIALOG_REGISTER_TWO, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Gender", "Please select your character sex below:", "Male", "Female");
    }

    playerInfo[playerid][pLanguage] = response;

    mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Language` = '%d' where `id` = '%d';", response, playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

Dialog:DIALOG_REGISTER_TWO(playerid, response, listitem, inputtext[])
{
    if(response) {
        playerInfo[playerid][pSkin] = 36;
        playerInfo[playerid][pGender] = 0;
        SendClientMessage(playerid, COLOR_SERVER, "REGISTER: {ffffff}Your gender is: Male");
    }
    else {
        playerInfo[playerid][pSkin] = 145;
        playerInfo[playerid][pGender] = 1;
        SendClientMessage(playerid, COLOR_SERVER, "REGISTER: {ffffff}Your gender is: Female");
    }

    SetPlayerSkin(playerid, playerInfo[playerid][pSkin]);

    mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Gender` = '%d', `Skin` = '%d' where `id` = '%d';", playerInfo[playerid][pGender], playerInfo[playerid][pSkin], playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");

    Dialog_Show(playerid, DIALOG_REGISTER_THREE, DIALOG_STYLE_INPUT, (playerInfo[playerid][pLanguage] ? ("Varsta") : ("Character age")), (playerInfo[playerid][pLanguage] ? ("Scrie varsta caracterului tau:") : ("Type the age of your character bellow:")), "Ok", "");
    return true;
}

Dialog:DIALOG_REGISTER_THREE(playerid, response, listitem, inputtext[])
{
    if(!strmatch(playerInfo[playerid][pEmail], "email@yahoo.com"))
        return CallLocalFunction("finishPlayerRegister", "i", playerid);

    if(strval(inputtext) > 1 && strval(inputtext) < 100)
    {
        if(playerInfo[playerid][pLanguage])
            SendClientMessage(playerid, -1, "Introdu adresa de email. Daca nu vrei sa-ti setezi email-ul, poti apasa ok."), SendClientMessage(playerid, -1, "Emailul te poate ajuta sa te conectezi pe server, in caz ca te conectezi dintr-o locatie noua."), SendClientMessage(playerid, -1, "Emailul te poate ajuta pentru a-ti recupera parola, in caz ca o uiti.");

        else
            SendClientMessage(playerid, -1, "Type your email. If you don't want to set your email, just click ok."), SendClientMessage(playerid, -1, "The email will help you to recover your password, in case you loose it.");

        return Dialog_Show(playerid, DIALOG_REGISTER_FOUR, DIALOG_STYLE_INPUT, "Email", "(ex: my_email@yahoo.com)", "Ok", "");
    }
    else return Dialog_Show(playerid, DIALOG_REGISTER_THREE, DIALOG_STYLE_INPUT, (playerInfo[playerid][pLanguage] == 2 ? ("Varsta") : ("Character age")), (playerInfo[playerid][pLanguage] == 2 ? ("Scrie varsta caracterului tau:") : ("Type the age of your character bellow:")), "Ok", "");
}

Dialog:DIALOG_REGISTER_FOUR(playerid, response, listitem, inputtext[])
{
    if(isValidEmail(inputtext))
    {
        if(playerInfo[playerid][pLanguage])
            va_SendClientMessage(playerid, COLOR_YELLOW, "Email setat: %s.", inputtext);

        else
            va_SendClientMessage(playerid, COLOR_YELLOW, "Email set to: %s.", inputtext);

        mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Email` = '%s' where `id` = '%d';", inputtext, playerInfo[playerid][pSQLID]);
        mysql_tquery(SQL, returnString, "", "");
    }
    else
    {
        if(playerInfo[playerid][pLanguage])
            SendClientMessage(playerid, COLOR_YELLOW, "Ai ales sa nu introduci emailul. Daca vrei sa-ti setezi un email pe cont in viitor, il poti seta de pe panel.");

        else
            SendClientMessage(playerid, COLOR_YELLOW, "You choosed not to set an email. You can visit the panel and set an email at a later date, if you want to.");
    }
    CallLocalFunction("finishPlayerRegister", "i", playerid);
    return true;
}

function:finishPlayerRegister(playerid) 
{    
    stop tutorial_timer[playerid];
    tutorial_timer[playerid] = Timer:0;

    PlayerTextDrawHide(playerid, playerTextdraws[playerid][Tutorial][1]);
    PlayerTextDrawHide(playerid, playerTextdraws[playerid][Tutorial][0]);

    playerInfo[playerid][pTutorial] = 1;
    new country[99];
    GetPlayerCountry(playerid, country, sizeof(country));
    
    foreach(new i : serverAdmins)
    {
        va_SendClientMessage(i, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): New account: %s (%d) - from %s.",GetName(playerid), playerid, country);
        SendDeathMessageToPlayer(i, 1001, 1001, 200);
    }
    clearChat(playerid, 20);
    SendClientMessage(playerid, COLOR_SERVER, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} {ffffff}You were successfully registered!");
    va_SendClientMessage(playerid, COLOR_SERVER, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} {ffffff}Welcome, %s.", GetName(playerid)); 


    SendClientMessage(playerid, -1, "Bine ai venit pe "COLOR_SV_HEX""SERVER_NAME2"{ffffff}");
    SendClientMessage(playerid, -1, "Ai primit licenta de condus timp de 10 ore.");
    SendClientMessage(playerid, -1, "Daca ai intrebari legate de joc, foloseste /n si helperii de pe server vor incerca sa te ajute.");
    SendClientMessage(playerid, -1, "Distractie placuta pe serverul nostru!");
    playerInfo[playerid][pCarLic] = 10;

    addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 20000, false); 

    TogglePlayerSpectating(playerid, false);
    SpawnPlayer(playerid);

    gpci(playerid, playerInfo[playerid][pSerial], 128);

    mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Tutorial` = '1', `SpawnLocation` = '1', `Money` = '%d', `House` = '%d', `DrivingLicence` = '24', `gpci` = '%s' where `id` = '%d';", GetMoney(playerid), playerInfo[playerid][pHouse], playerInfo[playerid][pSerial], playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");

    return true;
}

stock calculateTime(secunde)
{
    new time = secunde;
    time = time%3600;
    new minute = time/60;
    time = time%60;
    new secunde2 = time;
    new string[10];
    format(string,sizeof(string),"%02d:%02d",minute,secunde2);
    return string;
}

stock randomString(strDest[], strLen = 10)
{
    while(strLen--) strDest[strLen] = random(2) ? (random(26) + (random(2) ? 'a' : 'A')) : (random(10) + '0');
}

function:AllowPlayerToConnect(playerid)
{
    new returnHardware[128], cacheHardware[128], returnIPAddress[16], cacheIP[16];
    GetPlayerIp(playerid, returnIPAddress, sizeof returnIPAddress);
    GPCI(playerid, returnHardware, sizeof returnHardware);

    if(!cache_num_rows())
    {
        mysql_format(SQL, returnString, sizeof returnString, "insert into `allowedcomputers` (`computer_hwid`, `playerSQL`, `connectTime`, `IPAddress`) values ('%s', '%d', '%d', '%s');", returnHardware, playerInfo[playerid][pSQLID], gettime(), returnIPAddress);
        mysql_tquery(SQL, returnString, "", "");
        return true;
    }
    for(new i; i < cache_num_rows(); i++)
    {
        cache_get_value_name(i, "computer_hwid", cacheHardware);
        cache_get_value_name(i, "IPAddress", cacheIP);

        if(!strmatch(cacheHardware, returnHardware) && !strmatch(cacheIP, returnIPAddress))
        {
            for(new asd; asd <= 40; asd++)
                SendClientMessage(playerid, -1, "");

            SendClientMessage(playerid, COLOR_YELLOW, "Contul tau a fost blocat deoarece te-ai conectat dintr-o locatie diferita sau ai reinstalat jocul.");
            SendClientMessage(playerid, COLOR_YELLOW, "Acesta poate fi deblocat doar de pe adresa de email prin intremediul panel-ului.");

            SendClientMessage(playerid, -1, "-- -- -- --");
            SendClientMessage(playerid, COLOR_YELLOW, "Intre pe panel iar apoi urmeaza pasii de deblocare.");
            SendClientMessage(playerid, COLOR_YELLOW, "https://panel."SERVER_NAME"/");
            SendClientMessage(playerid, -1, "-- -- -- --");
            playerInfo[playerid][pBlockedAcc] = 1;
            UpdatePVars(playerid, "BlockedAcc", playerInfo[playerid][pBlockedAcc]);
            KickEx(playerid);
        }
        break;
    }
    return true;
}

new freezeTime[MAX_PLAYERS];
timer SetPlayerUnfreeze[freezeTime[playerid]](playerid)
{
    TogglePlayerControllable(playerid, true);
}

function:FreezePlayer(playerid, secondsFreeze)
{
    freezeTime[playerid] = (secondsFreeze * 1000);
    defer SetPlayerUnfreeze(playerid);

    return TogglePlayerControllable(playerid, false);
}

public e_COMMAND_ERRORS:OnPlayerCommandReceived(playerid, cmdtext[], e_COMMAND_ERRORS:success)
{
    if(!playerStaticVar[playerid][playerLogged])
        return COMMAND_DENIED;

    if(!playerInfo[playerid][pTutorial])
    {
        va_SendClientMessage(playerid, -1, "%s", (playerInfo[playerid][pLanguage] ? ("Nu poti folosi comenzi in timpul tutorialului.") : ("You can't use commands while you are in tutorial.")));
        return COMMAND_DENIED;
    }

    foreach(new i : Spectators)
    {
        if (Spectate[i] == playerid && i != playerid)
        {
            va_SendClientMessage(i, COLOR_LIGHTGREEN, "%s used command %s.", GetName(Spectate[i]), cmdtext);
        }
    }

    if(playerStaticVar[playerid][playerSleeping] && !strmatch("/sleep", cmdtext))
    {
        SendClientMessage(playerid, COLOR_GREY, ""COLOR_SV_HEX"ERROR >> {FFFFFF}Nu poti folosi aceasta comanda cand te aflii pe sleep.");
        return COMMAND_DENIED;
    }

    if(playerInfo[playerid][pMute] > gettime()) {
        SendClientMessage(playerid, -1, ""COLOR_SV_HEX"ERROR >> {FFFFFF}Nu poti folosi comenzi deoarece ai mute pentru inca %d %s.", ((playerInfo[playerid][pMute] - gettime()) > 60) ? ((playerInfo[playerid][pMute] - gettime()) / 60) : (playerInfo[playerid][pMute] - gettime()), ((playerInfo[playerid][pMute] - gettime()) > 60) ? "minute" : "secunde");
        return COMMAND_DENIED;
    }

    if(success == COMMAND_UNDEFINED)
    {
        SendClientMessage(playerid, -1, ""COLOR_SV_HEX"ERROR >> {FFFFFF}The command doesn't exist. Type /help.");
        return COMMAND_OK;
    }
    return COMMAND_OK;
}

function:UpdatePVars(playerid, const varname[], ingamevarname)
{
    mysql_format(SQL, returnString, sizeof returnString, "update `users` SET `%s` = '%d' WHERE `id` = '%d' LIMIT 1", varname, ingamevarname, playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");
    return 1;
}

function:UpdatePVarsString(playerid, const varname[], ingamevarname)
{
    mysql_format(SQL, returnString, sizeof returnString, "update `users` SET `%s` = '%s' WHERE `id` = '%d' LIMIT 1", varname, ingamevarname, playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");
    return 1;
}

function:getDaysFromTimestamp(time)
{
    new days = 0, actualTimeStamp = gettime() - time;
    days = (actualTimeStamp / 86400);
    return (!days) ? 0 : days;
}

timer process_login[60000](playerid)
{
    if(!playerStaticVar[playerid][playerLogged])
    {
        login_timer[playerid] = Timer:0;
        Kick(playerid);
    }
}

function:CheckUserAccount(playerid, race_check)
{
    if(race_check != g_MysqlRaceCheck[playerid]) return sendError(playerid, "Invalid session. Please reconnect."), KickEx(playerid);

    login_timer[playerid] = defer process_login(playerid);


    SendClientMessage(playerid, COLOR_SERVER, ""SERVER_NAME2" >> {FFFFFF}Welcome to "SERVER_DNS".");
    if(!cache_num_rows())
        SendClientMessage(playerid, -1, ""COLOR_SV_HEX""SERVER_NAME2" >> {ffffff}This account ("COLOR_SV_HEX"%s{FFFFFF}) wasn't founded in server database... To register type your desired password!", GetName(playerid));
    else
        SendClientMessage(playerid, -1, ""COLOR_SV_HEX""SERVER_NAME2" >> {ffffff}This account ("COLOR_SV_HEX"%s{FFFFFF}) was founded in server database... Type your password to login!", GetName(playerid));

    
    cache_get_value_name(0, "Email", playerInfo[playerid][pEmail]);
    
    if(!cache_num_rows())
    {
        new playerIP[16], playerHardware[64];
        GetPlayerIp(playerid, playerIP, sizeof playerIP);
        GPCI(playerid, playerHardware, sizeof playerHardware);

        mysql_format(SQL, returnString, sizeof returnString, "select * from `bans` where `banIP` = '%s' or `banHardware` = '%s';", playerIP, playerHardware);
        mysql_tquery(SQL, returnString, "check_hardware_ban", "i", playerid);
        return true;
    }
    ShowLoginDialog(playerid, 0);
    return true;
}

function:check_hardware_ban(playerid)
{
    if(!cache_num_rows())
        return ShowLoginDialog(playerid, 1);

    new iTimestamp, playerIP[16];
    GetPlayerIp(playerid, playerIP, sizeof playerIP);

    for(new i; i < cache_num_rows(); i++)
    {
        cache_get_value_name_int(i, "iTimestamp", iTimestamp);
        if((iTimestamp + 300) > gettime())
        {
            sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Staff Info{ffffff}): User %s was not able to register with IP: %s. (recently banned)", GetName(playerid), playerIP);
            SendClientMessage(playerid, COLOR_RED, "You are not able to register because you got recently banned. Try again later!");

            KickEx(playerid);
            break;
        }
        else
        {
            ShowLoginDialog(playerid, 1);
        }
    }
    return true;
}

Dialog:DIALOG_REGISTER(playerid, response, listitem, inputtext[])
{
    stop tutorial_timer[playerid];
    tutorial_timer[playerid] = Timer:0;
    if(strlen(inputtext) < 3 || strlen(inputtext) > 20)
        return sendError(playerid, "Your password must exceed 4 characters!"), ShowLoginDialog(playerid, 1);

    if(!string_checker(inputtext))
        return sendError(playerid, "Parola ta trebuie sa contina cel putin o litera mare si o cifra.");

    new hash[129];
    WP_Hash(hash, 129, inputtext);

    mysql_format(SQL, returnString, sizeof returnString, "insert into `users` (`name`, `password`) values ('%e', '%e');", GetName(playerid), hash);
    mysql_tquery(SQL, returnString, "", "");

    ShowLoginDialog(playerid, 0);
    return true;
}

Dialog:DIALOG_LOGIN(playerid, response, listitem, inputtext[])
{
    TogglePlayerSpectating(playerid, false);

    new parola[129];
    WP_Hash(parola, 129, inputtext);

    mysql_format(SQL, returnString, sizeof returnString, "select * from `users` where `name` = '%s' and `password` = '%s' limit 1;", GetName(playerid), parola);
    mysql_tquery(SQL, returnString, "WhenPlayerLoggedIn", "i", playerid);
    return true;
}

RegisterCamera(playerid)
{
    // if(playerStaticVar[playerid][tutorialStep])
    // {
    //     return proceed_tutorial(playerid);
    // }

    SetPlayerPos(playerid, 1564.2843,-2166.8777,13.5839 );
    SetPlayerInterior(playerid, 0);

    InterpolateCameraPos(playerid, 1595.332519, -2324.490722, 86.425498, 1330.248168, -2143.668945, 96.707298, 25000);
    InterpolateCameraLookAt(playerid, 1591.204101, -2321.674560, 86.585632, 1326.119750, -2140.852783, 96.867431, 25000);

    TogglePlayerControllable(playerid, false);
    return SetPlayerVirtualWorld(playerid, playerid + 1);
}

ShowPlayerLogo(playerid)
{
    if(!playerStaticVar[playerid][playerLogged])
        PlayerTextDrawSetString(playerid, playerTextdraws[playerid][Logo], ""SERVER_DNS"");

    else {
        va_PlayerTextDrawSetString(playerid, playerTextdraws[playerid][Logo], "%s / "SERVER_DNS"", GetDBName(playerid));
        PlayerTextDrawShow(playerid, playerTextdraws[playerid][Logo]);
    }

    return PlayerTextDrawShow(playerid, playerTextdraws[playerid][Logo]);
}



function:WhenPlayerSpawn(playerid)
{

    ShowPlayerLogo(playerid);

    TextDrawShowForPlayer(playerid, dateTD[0]);
    TextDrawShowForPlayer(playerid, dateTD[1]);
    TextDrawShowForPlayer(playerid, dateTD[2]);


    if(isPlayerInPaint(playerid)) 
        setPlayerPaintBallPos(playerid);

    if(!playerStaticVar[playerid][var_animations_loaded])
    {
        PreloadAnimLib(playerid, "BOMBER");
        PreloadAnimLib(playerid, "RAPPING");
        PreloadAnimLib(playerid, "SHOP");
        PreloadAnimLib(playerid, "BEACH");
        PreloadAnimLib(playerid, "SMOKING");
        PreloadAnimLib(playerid, "FOOD");
        PreloadAnimLib(playerid, "ON_LOOKERS");
        PreloadAnimLib(playerid, "DEALER");
        PreloadAnimLib(playerid, "MISC");
        PreloadAnimLib(playerid, "SAMP");
        PreloadAnimLib(playerid, "SWEET");
        PreloadAnimLib(playerid, "RIOT");
        PreloadAnimLib(playerid, "PED");
        PreloadAnimLib(playerid, "POLICE");
        PreloadAnimLib(playerid, "CRACK");
        PreloadAnimLib(playerid, "CARRY");
        PreloadAnimLib(playerid, "COP_AMBIENT");
        PreloadAnimLib(playerid, "PARK");
        PreloadAnimLib(playerid, "INT_HOUSE");
        PreloadAnimLib(playerid, "FOOD");
        PreloadAnimLib(playerid, "SWORD");
        PreloadAnimLib(playerid, "BASEBALL");
        playerStaticVar[playerid][var_animations_loaded] = 1;
    }
    return true;
}
public OnPlayerSpawn(playerid)
{
    if(!playerStaticVar[playerid][playerLogged] || !playerInfo[playerid][pTutorial])
    {
        RegisterCamera(playerid);
    }
    else
    {
        SetPlayerPos(playerid, 1564.5190,-2167.1038,13.5839);
        SetPlayerSkin(playerid, playerInfo[playerid][pSkin]);

        SetPlayerInterior(playerid, 0);
        SetPlayerVirtualWorld(playerid, 0);

        SetCameraBehindPlayer(playerid);
        TogglePlayerControllable(playerid, true);
    }
    return CallLocalFunction("WhenPlayerSpawn", "i", playerid);
}

public OnQueryError(errorid, const error[], const callback[], const query[], MySQL:handle) { 
    sendStaffMessage(STAFF_TYPE_OWNER, COLOR_DARKRED, "(!) Warning: An mysql error occurred while processing a query. (errorid: %d)", errorid);

    print("-------------------");
    printf("Handle: %d", handle);
    print("-------------------");
    printf("Error ID: %d", errorid);
    printf("Error: %s", error);
    printf("Callback: %s", callback);
    printf("Query Unprocessed: %s", query);
    print("-------------------"); 
    return serverMysqlErrors ++;
}

public OnRuntimeError(code, &bool:suppress)
{
    sendStaffMessage(STAFF_TYPE_OWNER, COLOR_WARNING, "[ERROR] Runtime error (crashdetect).");
    return serverBacktraces ++;
}

YCMD:detox(playerid, params[], help)
{
    if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER && vehicleInfo[serverVehicleId[GetPlayerVehicleID(playerid)]][vehGroup] == GROUP_TYPE_PARAMEDIC)
    {
        if(playerInfo[playerid][pGroup] == GROUP_TYPE_PARAMEDIC && playerStaticVar[playerid][groupDuty])
        {
            new returnPlayer, returnMoney;
            if(sscanf(params, "ud", returnPlayer, returnMoney))
                return sendSyntaxMessage(playerid, "/detox [playerid/playername] [amount]");

            if(returnMoney < 1 || returnMoney > 2000)
                return sendError(playerid, "Detoxing price not below $1 or above $2.000.");

            if(GetPlayerVehicleID(playerid) != GetPlayerVehicleID(returnPlayer))
                return sendError(playerid, "One of you is not in the ambulance/chopper.");

            if(GetMoney(returnPlayer) < returnMoney && GetMilliards(returnPlayer) < 1)
                return sendError(playerid, "This player doesn't have enough money.");
               
            playerStaticVar[returnPlayer][var_addiction_procent] = 0;
            UpdatePVars(returnPlayer, "Addiction", playerStaticVar[returnPlayer][var_addiction_procent]);


            va_GameTextForPlayer(playerid, "~w~You detoxified ~n~~w~%s~n~~g~$%d", 5000, 1, GetDBName(returnPlayer), returnMoney);
            va_SendClientMessage(returnPlayer, COLOR_LIGHTBLUE, "You have been detoxified for $%s.", FormatNumber(returnMoney));

            insertRaportPoint(playerid);
            return true;
        }
    }
    return true;
}

YCMD:heal(playerid, params[], help)
{
    if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER && vehicleInfo[serverVehicleId[GetPlayerVehicleID(playerid)]][vehGroup] == GROUP_TYPE_PARAMEDIC)
    {
        if(playerInfo[playerid][pGroup] == GROUP_TYPE_PARAMEDIC && playerStaticVar[playerid][groupDuty])
        {
            new returnPlayer, returnMoney;
            if(sscanf(params, "ud", returnPlayer, returnMoney))
                return sendSyntaxMessage(playerid, "/heal [playerid/playername] [amount]");

            if(returnMoney < 1 || returnMoney > 2000)
                return sendError(playerid, "Healing price not below $1 or above $2.000.");

            if(GetPlayerVehicleID(playerid) != GetPlayerVehicleID(returnPlayer))
                return sendError(playerid, "One of you is not in the ambulance/chopper.");

            if(GetMoney(returnPlayer) < returnMoney && GetMilliards(returnPlayer) < 1)
                return sendError(playerid, "This player doesn't have enough money.");
               
            SetPlayerHealthEx(returnPlayer, 99.9);

            va_GameTextForPlayer(playerid, "~w~You healed ~n~~w~%s~n~~g~$%d", 5000, 1, GetDBName(returnPlayer), returnMoney);
            va_SendClientMessage(returnPlayer, COLOR_LIGHTBLUE, "You have been healed for $%s.", FormatNumber(returnMoney));

            insertRaportPoint(playerid);
            return true;
        }
    }
    if(IsPlayerInRangeOfPoint(playerid, 5.0, -235.3070, 2438.1167, 35.7444) && GetPlayerVirtualWorld(playerid))
    {
        if(GetPVarInt(playerid, "deelay_heal") > gettime())
            return va_SendClientMessage(playerid, COLOR_RED, "Trebuie sa astepti inca %d secunde pentru a-ti reface viata.", GetPVarInt(playerid, "deelay_heal") - gettime());

        SendClientMessage(playerid, -1, "Ti-ai refacut viata.");
        SetPlayerHealthEx(playerid, 99);

        SetPVarInt(playerid, "deelay_heal", gettime() + 60);
        return true;
    }
    if(playerStaticVar[playerid][inHQ] || playerStaticVar[playerid][inHouse])
    {
        if(playerStaticVar[playerid][inHouse] >= 1 && !houseInfo[playerStaticVar[playerid][inHouse]][hHealth])
            return true;

        if(GetPVarInt(playerid, "deelay_heal") > gettime())
            return va_SendClientMessage(playerid, -1, "You need to wait 30 seconds before using /heal again.", GetPVarInt(playerid, "deelay_heal") - gettime());

        SetPlayerHealthEx(playerid, 99);

        if(IsACop(playerid) && houseInfo[playerStaticVar[playerid][inHouse]][hArmour])
            SetPlayerArmourEx(playerid, 99);

        sendNearbyMessage(playerid, 0xd3b3b2FF, "* %s ate some food.", GetName(playerid));
        GameTextForPlayer(playerid, "healed", 5000, 1);

        SetPVarInt(playerid, "deelay_heal", gettime() + 30);
    }
    if(playerStaticVar[playerid][inBusiness] && IsPlayerInRangeOfPoint(playerid, 3.0, 304.6744, -65.3353, 1001.5156))
    {
        if(GetPVarInt(playerid, "deelay_heal") > gettime())
            return va_SendClientMessage(playerid, COLOR_RED, "Trebuie sa astepti inca %d secunde pentru a-ti reface viata.", GetPVarInt(playerid, "deelay_heal") - gettime());

        SendClientMessage(playerid, -1, "Ti-ai refacut viata.");
        SetPlayerHealthEx(playerid, 99);

        SetPVarInt(playerid, "deelay_heal", gettime() + 30);
    }
    // alte if uri
    return true;
}

YCMD:open(playerid, params[], help)
{
    if(playerInfo[playerid][pApartament] && GetPVarInt(playerid, "ap_areaID") == playerInfo[playerid][pApartament])
    {
        new apartId = playerInfo[playerid][pApartament];

        va_SendClientMessage(playerid, -1, "Your apartment is now %s.", apInfo[apartId][apLocked] ? ("unlocked") : ("locked"));
        apInfo[apartId][apLocked] = apInfo[apartId][apLocked] ? 0 : 1;

        WhenApartamentGotUpdated(apartId);

        mysql_format(SQL, returnString, sizeof returnString, "update `apartaments` set `Locked` = '%d' where `id` = '%d';", apInfo[apartId][apLocked], apartId);
        mysql_tquery(SQL, returnString, "", "");
    }
    else if(playerInfo[playerid][pBusiness] && GetPVarInt(playerid, "business_area_sphere") == playerInfo[playerid][pBusiness])
    {
        new bizId = playerInfo[playerid][pBusiness];

        GameTextForPlayer(playerid, bizInfo[bizId][bizLocked] ? ("~w~Bussiness ~g~Open") : ("~w~Bussiness ~r~~h~Closed"), 5000, 6);
        PlayerPlaySound(playerid, 1145, 0.0, 0.0, 0.0);

        bizInfo[bizId][bizLocked] = bizInfo[bizId][bizLocked] ? 0 : 1;
        WhenBusinessGotUpdated(bizId);

        mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `bizLocked` = '%d' where `id` = '%d';", bizInfo[bizId][bizLocked], bizId);
        mysql_tquery(SQL, returnString, "", "");
    }
    else if(playerInfo[playerid][pHouse] && GetPVarInt(playerid, "areaID") == playerInfo[playerid][pHouse])
    {
        new houseId = playerInfo[playerid][pHouse];

        GameTextForPlayer(playerid, houseInfo[houseId][houseLocked] ? ("~w~Door ~g~Unlocked") : ("~w~Door ~r~Locked"), 5000, 6);
        PlayerPlaySound(playerid, 1145, 0.0, 0.0, 0.0);

        houseInfo[houseId][houseLocked] = houseInfo[houseId][houseLocked] ? 0 : 1;
        WhenHouseGotUpdated(houseId);

        mysql_format(SQL, returnString, sizeof returnString, "update `houses` set `Locked` = '%d' where `id` = '%d';", houseInfo[houseId][houseLocked], houseId);
        mysql_tquery(SQL, returnString, "", "");
    }
    return true;
}

timer opencase_timer[500](playerid)
{
    playerStaticVar[playerid][currentCaseSpin] ++;

    if(playerStaticVar[playerid][currentCaseSpin] < 6)
        UpdateCaseOpening(playerid);

    if(playerStaticVar[playerid][currentCaseSpin] == 6)
    {
        TextDrawHideForPlayer(playerid, caseTD[3]);

        switch(playerStaticVar[playerid][currentCaseWin])
        {
            case 21..25:
            {
                playerStaticVar[playerid][currentCaseWinAmount] = 15 + random(10);
                playerInfo[playerid][pPremiumPoints] += playerStaticVar[playerid][currentCaseWinAmount];

                va_SendClientMessageToAll(COLOR_GENANNOUNCE, "(( %s a deschis o cutie si a primit %d premium points ))", GetName(playerid), playerStaticVar[playerid][currentCaseWinAmount]);
            }
            case 26..32:
            {
                playerStaticVar[playerid][currentCaseWinAmount] = 1;
                playerInfo[playerid][pHiddenColor] ++;

                va_SendClientMessageToAll(COLOR_GENANNOUNCE, "(( %s a deschis o cutie si a primit o culoare hidden ))", GetName(playerid));
            }
            case 33..38:
            {
                insertPlayerVehicle(playerid, 424, 0);
                va_SendClientMessageToAll(COLOR_GENANNOUNCE, "(( %s a deschis o cutie si a primit un vehicul de tip 'BF Injection' ))", GetName(playerid));
            }
            case 39..45:
            {
                new skinId = random(300);
                new skinType = RandomEx(2, 4);

                playerStaticVar[playerid][currentCaseWinAmount] = skinId;
                va_SendClientMessage(playerid, COLOR_DCHAT, "Ai primit skinul cu id %d de tip %s.", skinId, SkinCategory(skinType));
                InsertPlayerSkin(playerid, skinId, skinType);

            }
            default:
            {
                playerStaticVar[playerid][currentCaseWinAmount] = 1000000 + random(3000000);
                addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, playerStaticVar[playerid][currentCaseWinAmount], false); 
            }
        }
    }

    if(playerStaticVar[playerid][currentCaseSpin] == 8)
    {
        for(new i; i <= 11; i++)
        {
            PlayerTextDrawHide(playerid, playerTextdraws[playerid][casePTD][i]);

            if(i <= 2)
                TextDrawHideForPlayer(playerid, caseTD[i]);
        }
        switch(playerStaticVar[playerid][currentCaseWin])
        {
            case 21..25: va_PlayerTextDrawSetString(playerid, playerTextdraws[playerid][casePTD][12], "%d~n~Premium~n~Points", playerStaticVar[playerid][currentCaseWinAmount]);
            case 26..32: PlayerTextDrawSetString(playerid, playerTextdraws[playerid][casePTD][12], "1~n~Hidden~n~Color");
            case 33..38: PlayerTextDrawSetString(playerid, playerTextdraws[playerid][casePTD][12], "1~n~BF Injection");
            case 39..45: va_PlayerTextDrawSetString(playerid, playerTextdraws[playerid][casePTD][12], "Skin~n~%d", playerStaticVar[playerid][currentCaseWinAmount]);
            default: va_PlayerTextDrawSetString(playerid, playerTextdraws[playerid][casePTD][12], "%s$", FormatNumber(playerStaticVar[playerid][currentCaseWinAmount], ","));
        }

        PlayerTextDrawShow(playerid, playerTextdraws[playerid][casePTD][12]);

        TextDrawShowForPlayer(playerid, caseTD[4]);
        TextDrawShowForPlayer(playerid, caseTD[5]);
        TextDrawShowForPlayer(playerid, caseTD[6]);
    }
    if(playerStaticVar[playerid][currentCaseSpin] == 10)
    {
        stop case_timer[playerid];
        case_timer[playerid] = Timer:0;

        WhenPlayerExitCase(playerid);
    }
}

function:UpdateCaseOpening(playerid)
{
    for(new i; i <= 4; i++)
    {
        playerStaticVar[playerid][currentCaseWin] = random(45);
        switch(playerStaticVar[playerid][currentCaseWin])
        {
            //+ Clear FP ||| -1 Warn
            case 21..25:
            {
                PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][casePTD][i], 0x74942cAA);
                TextDrawBoxColour(caseTD[5], 0x74942cAA);

                PlayerTextDrawSetString(playerid, playerTextdraws[playerid][casePTD][i + 6], "premium points");

                PlayerTextDrawShow(playerid, playerTextdraws[playerid][casePTD][i]);
                PlayerTextDrawShow(playerid, playerTextdraws[playerid][casePTD][i + 6]);
            }
            case 26..32:
            {
                PlayerTextDrawSetString(playerid, playerTextdraws[playerid][casePTD][i + 6], "Hidden");

                PlayerTextDrawShow(playerid, playerTextdraws[playerid][casePTD][i]);
                PlayerTextDrawShow(playerid, playerTextdraws[playerid][casePTD][i + 6]);
            }
            case 33..38:
            {
                PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][casePTD][i], 0xdccca4AA);
                TextDrawBoxColour(caseTD[5], 0xdccca4AA);

                PlayerTextDrawSetString(playerid, playerTextdraws[playerid][casePTD][i + 6], "Vehicle");

                PlayerTextDrawShow(playerid, playerTextdraws[playerid][casePTD][i]);
                PlayerTextDrawShow(playerid, playerTextdraws[playerid][casePTD][i + 6]);
            }
            case 39..45:
            {
                PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][casePTD][i], 0xfcba03AA); //per skin type
                TextDrawBoxColour(caseTD[5], 0xfcba03AA);

                PlayerTextDrawSetString(playerid, playerTextdraws[playerid][casePTD][i + 6], "Skin");

                PlayerTextDrawShow(playerid, playerTextdraws[playerid][casePTD][i]);
                PlayerTextDrawShow(playerid, playerTextdraws[playerid][casePTD][i + 6]);
            }
            default:
            {
                PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][casePTD][i], -1288690262);
                TextDrawBoxColour(caseTD[5], -1288690262);

                PlayerTextDrawSetString(playerid, playerTextdraws[playerid][casePTD][i + 6], "Money");

                PlayerTextDrawShow(playerid, playerTextdraws[playerid][casePTD][i]);
                PlayerTextDrawShow(playerid, playerTextdraws[playerid][casePTD][i + 6]);
            }
        }
    }
}

YCMD:opencase(playerid, params[], help)
{
    if(!playerInfo[playerid][pCases])
        return sendError(playerid, "Nu detii o cutie");

    if(!playerInfo[playerid][pCasesKeys])
        return sendError(playerid, "Nu ai o cheie");

    for(new i; i <= 11; i++)
    {
        if(i >= 6 && i <= 10)
            PlayerTextDrawSetString(playerid, playerTextdraws[playerid][casePTD][i], "Nothing");

        if(i <= 5)
            PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][casePTD][i], 0xf7f9fcAA);//0xf7f9fcAA

        PlayerTextDrawShow(playerid, playerTextdraws[playerid][casePTD][i]);

        if(i <= 2)
            TextDrawShowForPlayer(playerid, caseTD[i]);
    }
    SelectTextDraw(playerid, 0x0c4999AA);
    SetPVarInt(playerid, "case_opening", 1);
    return true;
}

YCMD:sellkey(playerid, params[], help)
    return sendError(playerid,"Comanda dezactivata. Cheile si cutiile nu se pot vinde momentan.");

YCMD:sellcase(playerid, params[], help)
    return sendError(playerid, "Comanda dezactivata. Cheile si cutiile nu se pot vinde momentan.");

WhenPlayerExitCase(playerid)
{
    for(new i; i <= 12; i++)
        PlayerTextDrawHide(playerid, playerTextdraws[playerid][casePTD][i]);

    for(new i; i <= 6; i++)
        TextDrawHideForPlayer(playerid, caseTD[i]);

    DeletePVar(playerid, "case_opening");
    CancelSelectTextDraw(playerid);

    playerStaticVar[playerid][currentCaseSpin] = 0;
    playerStaticVar[playerid][currentCaseWin] = 0;
    playerStaticVar[playerid][currentCaseWinAmount] = 0;
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if(GetPVarInt(playerid, "case_opening"))
    {
        if(clickedid == Text:INVALID_TEXT_DRAW)
        {
            if(!playerStaticVar[playerid][currentCaseSpin])
                WhenPlayerExitCase(playerid);

            else
                SelectTextDraw(playerid, 0x0c4999AA);

        }
        if(clickedid == caseTD[2])
        {
            TextDrawHideForPlayer(playerid, caseTD[2]);
            TextDrawShowForPlayer(playerid, caseTD[3]);

            case_timer[playerid] = repeat opencase_timer(playerid);
            playerStaticVar[playerid][currentCaseSpin] = 0;

            UpdateCaseOpening(playerid);

            playerInfo[playerid][pCases] --;
            playerInfo[playerid][pCasesKeys] --;

            mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Cases` = '%d', `CaseKeys` = '%d' where `id` = '%d';", playerInfo[playerid][pCases], playerInfo[playerid][pCasesKeys], playerInfo[playerid][pSQLID]);
            mysql_tquery(SQL, returnString, "", "");
        }
    }
    return true;
}

#if TEST_SERVER
YCMD:champagne(playerid, params[], help) {
    if(!IsPlayerAttachedObjectSlotUsed(playerid, 5) && !IsPlayerAttachedObjectSlotUsed(playerid, 6)) {
        ApplyAnimation(playerid, "ped", "IDLE_armed", 4.1, 1, 0, 0, 0, 0, 0);

        SetPlayerAttachedObject(playerid, 5, 19821, 6, 0.073999, 0.020000, 0.055999, -0.000000, 69.899986, 0.000000, 0.791000, 0.860000, 0.593999);
        SetPlayerAttachedObject(playerid, 6, 18717, 6, 0.414999, 0.020999, -1.485998, 0.000000, 0.000000, 0.000000);
    }
    else {
        RemovePlayerAttachedObject(playerid, 5);
        RemovePlayerAttachedObject(playerid, 6);

        ClearAnimations(playerid);
    }
    return 1;
}

YCMD:gimmeadmin(playerid, params[], help)
{
    playerInfo[playerid][pAdmin] = 8;
    Iter_Add(serverAdmins, playerid);

    SendClientMessage(playerid, COLOR_DCHAT, "Bine ai venit regele meu an staff.");

    mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Admin` = '8' where `ID` = '%d'", playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

YCMD:testver(playerid, params[], help) {

    new returnAmount = 2000000000;
    printf("%s", FormatNumber(returnAmount));
    if(playerInfo[playerid][pAccount] < returnAmount && playerInfo[playerid][pAccountLY] < 1)
        return sendError(playerid,"Nu ai banii necesari in banca.");
    return true;
}
YCMD:testmoney(playerid, params[], help)
{
    check_owner

    if(sscanf(params, "s[32]", params[0]))
        return sendError(playerid, "/testmoney suma");

    substractBankMoney(playerid, playerInfo[playerid][pAccountLY], playerInfo[playerid][pAccount], 0, params[0]);
    return 1;
}
#endif


YCMD:park(playerid, params[], help)
{
    if(playerStaticVar[playerid][var_insafezone])
        return sendError(playerid, "Nu poti parca vehiculul personal intr-o zona protejata.");

    if(!IsPlayerInAnyVehicle(playerid))
        return sendError(playerid,"Nu te aflii intr-un vehicul.");

    new vehicleid = GetPlayerVehicleID(playerid), Float: vAngle, Float: vPos[3];

    GetVehiclePos(vehicleid, vPos[0], vPos[1], vPos[2]);
    GetVehicleZAngle(vehicleid, vAngle);

    if(VehicleSQL[vehicleid])
    {
        if(vehicleInfo[vehicleid][vehEngine])
            return sendError(playerid, "Nu poti face asta cu motorul pornit.");

        if(strcmp(playerInfo[playerid][pName], carInfo[VehicleSQL[vehicleid]][cOwner], false) == 1)
            return sendError(playerid, "Nu te aflii intr-un vehicul personal.");

        new vehicleDBId = VehicleSQL[vehicleid], Float: vHealth;
        GetVehicleHealth(vehicleid, vHealth);

        if(vHealth < 750.0)
            return sendError(playerid,"Vehiculul este defect. Mergi la un PNS pentru a-l repara.");

        carInfo[vehicleDBId][cPosition][0] = vPos[0];
        carInfo[vehicleDBId][cPosition][1] = vPos[1];
        carInfo[vehicleDBId][cPosition][2] = vPos[2];

        carInfo[vehicleDBId][cPosition][3] = vAngle;
        carInfo[vehicleDBId][cVirtual] = GetPlayerVirtualWorld(playerid);

        SendClientMessage(playerid, -1, "Vehiculul a fost parcat!");
        WhenPlayerParkVehicle(playerid, vehicleDBId, vehicleid);

        mysql_format(SQL, returnString, sizeof returnString, "update `personalvehicles` set `VirtualWorld` = '%d' where `id` = '%d';", carInfo[vehicleDBId][cVirtual], vehicleDBId);
        mysql_tquery(SQL, returnString, "", "");
    }
    else if(ClanVehicleSQL[vehicleid])
    {
        va_SendClientMessage(playerid, -1, "You have sucessfully parked vehicle %s [database id %d]", GetVehicleName(GetVehicleModel(vehicleid)), ClanVehicleSQL[vehicleid]);

        mysql_format(SQL, returnString, sizeof returnString, "update `clanvehicles` set `posX` = '%f', `posY` = '%f', `posZ` = '%f', `posA` = '%f' where `id` = '%d';", vPos[0], vPos[1], vPos[2], vAngle, ClanVehicleSQL[vehicleid]);
        mysql_tquery(SQL, returnString, "", "");
    }
    else
        return sendError(playerid,"Acest vehicul nu este unul personal.");

    return true;
}

YCMD:carcolor(playerid, params[], help) {
    new const vehicleid = GetPlayerVehicleID(playerid);

    if(!vehicleid) 
        return sendError(playerid, "You are not in any car.");

    if(!strmatch(playerInfo[playerid][pName], carInfo[VehicleSQL[vehicleid]][cOwner]))
        return sendError(playerid, "You are not in your personal vehicle.");

    new colorSlot, color; if(sscanf(params, "dd", colorSlot, color))
        return sendSyntaxMessage(playerid, "/carcolor [slot (1/2)> <color]");

    if(colorSlot < 1 || colorSlot > 2)
        return sendError(playerid, "Slot-ul trebuie sa fie doar 1 sau 2.");

    if(color < 0 || color > 255)
        return sendError(playerid, "Culoare invalida! (0 - 255)");

    if(color > 127) {
        if(!playerInfo[playerid][pHiddenColor])
            return sendError(playerid, "Nu ai o culoare hidden.");

        SetPVarInt(playerid, "carColorTemp", color);
        SetPVarInt(playerid, "carColorVehicleTemp", vehicleid);
        SetPVarInt(playerid, "carColorSlotTemp", colorSlot);

        return Dialog_Show(playerid, CHANGECOLORHIDDEN, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Hidden Color", "Are you sure you want to apply the hidden color (%d) on this vehicle?", "Yes", "No", color);
    }
    else {
        if(GetMoney(playerid) < 5000 && GetMilliards(playerid) < 1)
            return sendError(playerid, "Ai nevoie de $5.000 pentru a schimba culoarea.");

        substractMoney(playerid, GetMilliards(playerid), playerInfo[playerid][pMoney], 0, 5000, false);
    }

    if(colorSlot == 1) {
        carInfo[VehicleSQL[vehicleid]][cColor][0] = color;
    }
    else {
        carInfo[VehicleSQL[vehicleid]][cColor][1] = color;
    }

    ChangeVehicleColor(vehicleid, carInfo[VehicleSQL[vehicleid]][cColor][0], carInfo[VehicleSQL[vehicleid]][cColor][0]);

    mysql_format(SQL, returnString, sizeof returnString, "UPDATE `personalvehicles`, `users` SET `users`.`MoneyStore` = %d, `users`.`Money` = %d, `personalvehicles`.`Color1` = %d, `personalvehicles`.`Color2` = %d WHERE `users`.`ID` = %d AND `personalvehicles`.`ID` = %d", GetMilliards(playerid), playerInfo[playerid][pMoney], carInfo[VehicleSQL[vehicleid]][cColor][0], carInfo[VehicleSQL[vehicleid]][cColor][1], playerInfo[playerid][pSQLID], VehicleSQL[vehicleid]);
    mysql_tquery(SQL, returnString);

    return SendClientMessage(playerid, COLOR_SERVER, "HIDDEN >>: {ffffff}You changed your vehicle color successfully for $5.000. (%d/%d)", carInfo[VehicleSQL[vehicleid]][cColor][0], carInfo[VehicleSQL[vehicleid]][cColor][1]);
}

Dialog:CHANGECOLORHIDDEN(playerid, response, listitem, inputtext[]) {   
    if(!response) {
        DeletePVar(playerid, "carColorTemp");
        DeletePVar(playerid, "carColorVehicleTemp");
        DeletePVar(playerid, "carColorSlotTemp");
        return 1;
    }

    new 
        color = GetPVarInt(playerid, "carColorTemp"),
        vehicleid = GetPVarInt(playerid, "carColorVehicleTemp"),
        colorSlot = GetPVarInt(playerid, "carColorSlotTemp");


    if(!playerInfo[playerid][pHiddenColor])
        return sendError(playerid, "Nu ai o culoare hidden.");

    playerInfo[playerid][pHiddenColor] --;

    if(colorSlot == 1) {
        carInfo[VehicleSQL[vehicleid]][cColor][0] = color;
    }
    else {
        carInfo[VehicleSQL[vehicleid]][cColor][1] = color;
    }

    ChangeVehicleColor(vehicleid, carInfo[VehicleSQL[vehicleid]][cColor][0], carInfo[VehicleSQL[vehicleid]][cColor][0]);

    mysql_format(SQL, returnString, sizeof returnString, "UPDATE `personalvehicles`, `users` SET `users`.`HiddenColor` = %d, `personalvehicles`.`Color1` = %d, `personalvehicles`.`Color2` = %d WHERE `users`.`ID` = %d AND `personalvehicles`.`ID` = %d", playerInfo[playerid][pHiddenColor], carInfo[VehicleSQL[vehicleid]][cColor][0], carInfo[VehicleSQL[vehicleid]][cColor][1], playerInfo[playerid][pSQLID], VehicleSQL[vehicleid]);
    mysql_tquery(SQL, returnString);

    DeletePVar(playerid, "carColorTemp");
    DeletePVar(playerid, "carColorVehicleTemp");
    DeletePVar(playerid, "carColorSlotTemp");

    return SendClientMessage(playerid, COLOR_SERVER, "HIDDEN >>: {ffffff}You changed your vehicle color successfully in a hidden one. (%d/%d)", carInfo[VehicleSQL[vehicleid]][cColor][0], carInfo[VehicleSQL[vehicleid]][cColor][1]);
}

YCMD:vw(playerid, params[], help)
    return va_SendClientMessage(playerid, -1, "Current virtual world: %d", GetPlayerVirtualWorld(playerid));

YCMD:oldcar(playerid, params[], help)
    return va_SendClientMessage(playerid, COLOR_GREY, "Your old car was: %d", playerStaticVar[playerid][lastVehicle]);

YCMD:setfreq(playerid, params[], help)
{
    if(!playerInfo[playerid][pWalkieTalkie])
        return sendError(playerid, "You don't have a walkie talkie.");

    new returnFreq;
    if(sscanf(params, "d", returnFreq))
        return SendClientMessage(playerid, COLOR_GREY, "Syntax: /setfreq [frequency]");

    if(returnFreq == 0)
    {
        SendClientMessage(playerid, -1, "You have turned off your walkie talkie.");

        mysql_format(SQL, returnString, sizeof returnString, "update `users` set `WTFreq` = '0' where `id` = '%d';", playerInfo[playerid][pSQLID]);
        mysql_tquery(SQL, returnString, "", "");
        return true;
    }
    if(returnFreq < 1000)
        return SendClientMessage(playerid, COLOR_GREY, "Invalid frequency..");

    if(returnFreq > 9999)
        return true;

    va_SendClientMessage(playerid, -1, "Your frequency is now: #%d", returnFreq);

    if(playerInfo[playerid][pWTFreq] > 1)
        Iter_Remove(playerWT<playerInfo[playerid][pWTFreq]>, playerid);

    playerInfo[playerid][pWTFreq] = returnFreq;

    if(playerInfo[playerid][pWTFreq] != 0)
        Iter_Add(playerWT<returnFreq>, playerid);

    mysql_format(SQL, returnString, sizeof returnString, "update `users` set `WTFreq` = '%d' where `id` = '%d';", playerInfo[playerid][pWTFreq], playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

YCMD:freqmembers(playerid, params[], help)
{
    if(playerInfo[playerid][pWTFreq] == 0)
        return sendError(playerid, "You walkie talkie is turned off.");

    if(Iter_Count(playerWT<playerInfo[playerid][pWTFreq]>) <= 1)
        return true;

    new freqString[512];
    freqString = "Walkie-Talkie members: ";

    foreach(new i : playerWT<playerInfo[playerid][pWTFreq]>)
        format(freqString, sizeof freqString, "%s, ", GetDBName(i));

    sendSplitMessage(playerid, -1, freqString);
    return true;
}

YCMD:wt(playerid, params[], help)
{
    if(!playerInfo[playerid][pWalkieTalkie])
        return sendError(playerid, "You don't have a Walkie Talkie.");

    if(playerInfo[playerid][pWTFreq] < 1)
        return sendError(playerid, "You don't broadcasting any frequency. Use /setfreq(uency).");

    if(playerStaticVar[playerid][pFreqToggled] == false)
        return sendError(playerid, "Your walkie talkie is turned off.");

    if(GetPVarInt(playerid, "wt_dee") > gettime())
        return va_SendClientMessage(playerid, -1, "Nu poti scrie pe chatul frecventei %d pentru inca %d secunde.", playerInfo[playerid][pWTFreq], GetPVarInt(playerid, "wt_dee") - gettime());

    new returnMessage[144];
    if(sscanf(params, "s[144]", returnMessage))
        return sendSyntaxMessage(playerid, "/wt [message]");

    if(FindIP(returnMessage)) /// Anti-Reclama
		sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s(%d) typed %s in /wt.", GetName(playerid), playerid, returnMessage);

    foreach(new i : playerWT<playerInfo[playerid][pWTFreq]>)
    {
        if(playerStaticVar[playerid][pFreqToggled] == false)
            continue;

        sendSplitMessage(i, 0xeba657FF, "(frequency %d) %s: %s", playerInfo[playerid][pWTFreq], GetName(playerid), returnMessage);
    }

    SetPVarInt(playerid, "wt_dee", gettime() + 3);
    return true;
}

YCMD:pcolor(playerid, params[], help) {
    if(!playerInfo[playerid][pPremiumAccount])
        return sendError(playerid, "Nu detii cont premium.");

    gDialog[0] = EOS;

    for(new i = 0; i < sizeof premiumNameColors; ++i) 
        format(gDialog, sizeof gDialog, "%s{%s}%s\n", gDialog, premiumNameColors[i][colorValue], premiumNameColors[i][colorName]);

    Dialog_Show(playerid, PREMIUMCOLORS, DIALOG_STYLE_LIST, "SERVER: Premium Colors", gDialog, "Select", "Exit");
    return true;
}

YCMD:pc(playerid, params[], help) {
    if(!playerInfo[playerid][pPremiumAccount] && !playerInfo[playerid][pAdmin])
        return sendError(playerid, "Nu detii cont premium.");

    if(!playerStaticVar[playerid][var_premium_tog])
        return sendError(playerid, "Nu ai activat chat-ul premium. Foloseste [/tog].");

    if(playerInfo[playerid][pMute] > gettime()) 
        return sendError(playerid, "Nu poti scrie in chat deoarece ai mute pentru inca %s minute.", calculateTime(playerInfo[playerid][pMute] - gettime()));

    if(isnull(params))
        return sendSyntaxMessage(playerid, "/premiumchat <message>");

    gString[0] = EOS;
    format(gString, sizeof gString, "[PREMIUM CHAT] %s: %s", GetName(playerid), params);


    foreach(new i : premiumPlayers) {
        if(!playerStaticVar[i][var_premium_tog] || playerInfo[i][pAdmin])
            continue;

        SendClientMessage(i, 0xC2B85FFF, gString);
    }

    foreach(new i : serverAdmins) {
        if(!playerStaticVar[i][var_premium_tog])
            continue;

        SendClientMessage(i, 0xC2B85FFF, gString);
    }


    return 1;
}


Dialog:PREMIUMCOLORS(playerid, response, listitem, inputtext[]) {
    if(!response)
        return 1;

    playerInfo[playerid][pPremiumColor] = listitem;

    va_mysql_tquery("UPDATE `users` SET `PremiumColor` = %d WHERE `ID` = %d", listitem, playerInfo[playerid][pSQLID]);

    SendClientMessage(playerid, -1, "You have set your color to {%s}%s{ffffff}.", premiumNameColors[listitem][colorValue], premiumNameColors[listitem][colorName]);
    return true;
}

YCMD:shop(playerid, params[], help)
{
    new captionFormat[32];
    format(captionFormat, sizeof captionFormat, "premium points: "COLOR_SV_HEX"%d", playerInfo[playerid][pPremiumPoints]);

    Dialog_Show(playerid, DIALOG_SHOP, DIALOG_STYLE_TABLIST_HEADERS, captionFormat, "Item\tprice\n"COLOR_SV_HEX"[+]{ffffff} Get premium points?\n\
        Premium account\t"COLOR_SV_HEX"250{ffffff} premium points\nChange Nickname\t"COLOR_SV_HEX"150{ffffff} premium points\n\
        Clear FP\t"COLOR_SV_HEX"50{ffffff} premium points\nClear Faction Warnings\t"COLOR_SV_HEX"100{ffffff} premium points\n\
        Create Clan\t"COLOR_SV_HEX"500{ffffff} premium points\nVehicle hidden color\t"COLOR_SV_HEX"50{ffffff} premium points\n\
        Buy vehicle slot\t"COLOR_SV_HEX"100{ffffff} premium points\niPhone\t"COLOR_SV_HEX"100{ffffff} premium points\n\
        Upgrade House Interior\t"COLOR_SV_HEX"More Details{ffffff}\n\
        Personal Garage\t"COLOR_SV_HEX"More Details{ffffff}\nClear All Warnings\t"COLOR_SV_HEX"150{ffffff} premium points\nChange Sex\t"COLOR_SV_HEX"150{ffffff} premium points", "Buy", "Exit");
    return true;
}

Dialog:DIALOG_SHOP(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    switch(listitem)
    {
        case 0: Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Info", "Playerii ce vor sa ajute comunitatea pot cumpara premium points (cu bani reali).\nSe pot cumpara premium points folosind PayPal/PaySafe/Transfer bancar.\nPoti vedea preturile premium pointslor pe panel."SERVER_NAME".", "Close", "");
        case 1:
        {
            if(playerInfo[playerid][pPremiumPoints] < 250)
                return Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Not enough points", "You don't have enough premium points to do this.", "Close", "");

            if(playerInfo[playerid][pPremiumAccount])
                return sendError(playerid, "Ai deja cont premium.");

            Dialog_Show(playerid, BUYPREMIUM, DIALOG_STYLE_MSGBOX, "SERVER: Buy Premium", "Esti sigur ca vrei sa cumperi cont premium?\n\nBeneficii:\n\n- +2 sloturi de vehicule\n- acces la comanda /pcolor\n- acces la chat-ul [/pc]\nUn case si premium points la payday \n [/ad] de oriunde", "Da", "Nu");
        }
        case 2:
        {
            if(playerInfo[playerid][pPremiumPoints] < 150)
                return Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Not enough points", "You don't have enough premium points to do this.", "Close", "");

            Dialog_Show(playerid, DIALOG_CHANGENAME, DIALOG_STYLE_INPUT, "Change name:", "Please enter the new name below", "Select", "Cancel");
        }
        case 3: {
            if(playerInfo[playerid][pPremiumPoints] < 50) 
                return Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Not enough points", "You don't have enough premium points to do this.", "Close", "");
            
            if(playerInfo[playerid][pGroupPunish] <= 0) 
                return sendError(playerid, "You don't have faction punish.");

            SendClientMessage(playerid, -1, "Ai platit 50 premium points pentru a-ti sterge Faction Punish-urile.");

            playerInfo[playerid][pPremiumPoints] -= 50;
            playerInfo[playerid][pGroupPunish] = 0;

            mysql_format(SQL, returnString, sizeof returnString, "update `users` set `FactionPunish` = '%d', `PremiumPoints` = '%d' where `id` = '%d'", playerInfo[playerid][pGroupPunish], playerInfo[playerid][pPremiumPoints], playerInfo[playerid][pSQLID]);
            mysql_tquery(SQL, returnString, "", "");
        }
        case 4: {
            if(playerInfo[playerid][pPremiumPoints] < 100) 
                return Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Not enough points", "You don't have enough premium points to do this.", "Close", "");
            
            if(playerInfo[playerid][pGroupWarns] <= 0) 
                return sendError(playerid, "You don't have faction warns.");

            playerInfo[playerid][pPremiumPoints] -= 100;
            playerInfo[playerid][pGroupWarns] = 0;

            SendClientMessage(playerid, -1, "Ai platit 100 premium points pentru a-ti sterge warn-urile factiunii.");

            mysql_format(SQL, returnString, sizeof returnString, "update `users` set `FactionWarns` = '%d', `PremiumPoints` = '%d' where `id` = '%d'", playerInfo[playerid][pGroupWarns], playerInfo[playerid][pPremiumPoints], playerInfo[playerid][pSQLID]);
            mysql_tquery(SQL, returnString, "", "");    
        }
        case 5:
        {
            if(playerInfo[playerid][pPremiumPoints] < 500)
                return Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Not enough points", "You don't have enough premium points to do this.", "Close", "");

            if(playerInfo[playerid][pClan])
                return sendError(playerid, "You already are in a clan! Use /quitclan first.");

            Dialog_Show(playerid, DIALOG_CHOOSECLANNAME, DIALOG_STYLE_INPUT, "Clan name", "Insert clan name in the box below:\n(You can't change it after that!)", "Ok", "");
        }
        case 6: {
            if(playerInfo[playerid][pPremiumPoints] < 50) 
                return Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Not enough points", "You don't have enough premium points to do this.", "Close", "");
            
            playerInfo[playerid][pPremiumPoints] -= 50;
            playerInfo[playerid][pHiddenColor] += 1;
            
            SendClientMessage(playerid, -1, "Ai platit 50 premium points pentru a cumpara o culoare hidden.");

            mysql_format(SQL, returnString, sizeof returnString, "update `users` set `HiddenColor` = '%d', `PremiumPoints` = '%d' where `id` = '%d'", playerInfo[playerid][pHiddenColor], playerInfo[playerid][pPremiumPoints], playerInfo[playerid][pSQLID]);
            mysql_tquery(SQL, returnString, "", "");
        }
        case 7: {
            if(playerInfo[playerid][pPremiumPoints] < 100) 
                return Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Not enough points", "You don't have enough premium points to do this.", "Close", "");

            if(playerInfo[playerid][pCarSlots] >= 19) 
                return sendError(playerid, "You can't buy more than 19 vehicle slots.");

            playerInfo[playerid][pPremiumPoints] -= 100;
            playerInfo[playerid][pCarSlots] += 1;

            SendClientMessage(playerid, -1,  "Ai platit 100 premium points pentru a cumpara un slot pentru masini.");

            mysql_format(SQL, returnString, sizeof returnString, "update `users` set `carSlots` = '%d', `PremiumPoints` = '%d' where `id` = '%d'", playerInfo[playerid][pCarSlots], playerInfo[playerid][pPremiumPoints], playerInfo[playerid][pSQLID]);
            mysql_tquery(SQL, returnString, "", "");
        }
        case 8: {
            if(playerInfo[playerid][pPremiumPoints] <= 100) 
                return Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Not enough points", "You don't have enough premium points to do this.", "Close", "");
            
            if(playerInfo[playerid][pPhone] == 1) 
                return sendError(playerid, "You have already an iPhone.");

            format(returnString, sizeof returnString, "Avantaje iPhone:\n- numar de telefon din 4 cifre la alegere.\n- poti dezactiva apelurile daca vrei sa primesti doar SMS-uri.\n- in chat va aparea `* Player turns of his iPhone`.\n- optiune de /reply raspunde la ultimul SMS primit.\n- optiune /block pentru a bloca temporar un numar de telefon.\n\nChoose a phone number:");
            // Dialog_Show(playerid, DIALOG_IPHONE, DIALOG_STYLE_INPUT, "iPhone", returnString, "Ok", "Cancel");
            Dialog_Show(playerid, DIALOG_IPHONE, DIALOG_STYLE_INPUT, "iPhone", returnString, "Choose", "Cancel");
        }

        case 9:
        {
            if(playerInfo[playerid][pHouse] && houseInfo[playerInfo[playerid][pHouse]][houseOwner] != playerInfo[playerid][pSQLID] || houseInfo[playerInfo[playerid][pHouse]][houseVIP])
                return sendError(playerid, "Nu ai o casa sau casa ta detine acest upgrade.");

            Dialog_Show(playerid, DIALOG_CONFIRMVIPHOUSE, DIALOG_STYLE_MSGBOX, "Buy House Interior Upgrade", "Folosind aceasta optiune va puteti adauga pana la 25 de obiecte custom in interiorul casei\nVa puteti alege obiectele prin comanda (/housefurniture).\nPretul acestui upgrade este de 100 premium points\nDoriti sa continuati achizitia?", "Da", "Renunta");
        }
        case 10:
        {
            if(playerInfo[playerid][pHouse] && houseInfo[playerInfo[playerid][pHouse]][houseOwner] != playerInfo[playerid][pSQLID] || houseInfo[playerInfo[playerid][pHouse]][houseGarage])
                return sendError(playerid, "Nu ai o casa sau casa ta detine un garaj.");

            Dialog_Show(playerid, DIALOG_BUYGARAGE, DIALOG_STYLE_MSGBOX, "Buy Garage", "Folosind aceasta optiune va puteti achizitiona un garaj personal la casa dumneavoastra\nIn garaj va ve putea parca masinile personale.\nVa puteti controla garajul prin comanda (/garage).\nDoriti sa continuati achizitia?", "Da", "Renunta");
        }
        case 11: {
            if(playerInfo[playerid][pPremiumPoints] <= 150) 
                return Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Not enough points", "You don't have enough premium points to do this.", "Close", "");
            
            if(playerInfo[playerid][pWarns] == 0)
                    return sendError(playerid, "You don't have any warnings.");

            SendClientMessage(playerid, -1, "Ai platit 150 premium points pentru a-ti sterge warning-urile.");
            
            playerInfo[playerid][pWarns] = 0;
            playerInfo[playerid][pPremiumPoints] -= 150;

            UpdatePVars(playerid, "Warn", playerInfo[playerid][pWarns]);
            UpdatePVars(playerid, "PremiumPoints", playerInfo[playerid][pPremiumPoints]);

        }

        case 12: {
            if(playerInfo[playerid][pPremiumPoints] <= 150) 
                return Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Not enough points", "You don't have enough premium points to do this.", "Close", "");
            

            switch(playerInfo[playerid][pGender]) {
                case 0: {
                    SendClientMessage(playerid, -1, "Ai platit 150 premium points pentru a-ti schimba sex-ul in feminin.");
                    playerInfo[playerid][pSkin] = 145;
                    playerInfo[playerid][pGender] = 1;

                }

                case 1: {
                    playerInfo[playerid][pGender] = 0;
                    playerInfo[playerid][pSkin] = 36;
                    SendClientMessage(playerid, -1, "Ai platit 150 premium points pentru a-ti schimba sex-ul in masculin.");
                }
            }
            SetPlayerSkin(playerid, playerInfo[playerid][pSkin]);
            playerInfo[playerid][pPremiumPoints] -= 150;

            UpdatePVars(playerid, "Gender", playerInfo[playerid][pGender]);
            UpdatePVars(playerid, "Skin", playerInfo[playerid][pSkin]);
            UpdatePVars(playerid, "PremiumPoints", playerInfo[playerid][pPremiumPoints]);
        }
    }
    return true;
}

Dialog:BUYPREMIUM(playerid, response, listitem, inputtext[]) {
    if(!response)
        return 1;

    if(playerInfo[playerid][pPremiumPoints] < 250)
        return sendError(playerid, "Nu ai suficiente puncte premium.");

    playerInfo[playerid][pPremiumPoints] -= 250;
    playerInfo[playerid][pPremiumAccount] = true;
    playerInfo[playerid][pPremiumExpire] = gettime() + (30 * 86400);
    playerStaticVar[playerid][var_premium_tog] = true;

    Iter_Add(premiumPlayers, playerid);

    if(!playerInfo[playerid][pPremiumCount]) {
        playerInfo[playerid][pPremiumCount] = 1;

        playerInfo[playerid][pCarSlots] = 3; 
    }

    SendClientMessage(playerid, COLOR_YELLOW, "You bought premium account for %d premium points.", 250);

    va_mysql_tquery("UPDATE `users` SET `carSlots` = %d, `PremiumPoints` = %d, `PremiumExpire` = %d, `PremiumCount` = 1, `PremiumAccount` = 1 WHERE `ID` = %d", playerInfo[playerid][pCarSlots], playerInfo[playerid][pPremiumPoints], playerInfo[playerid][pPremiumExpire], playerInfo[playerid][pSQLID]);
    return 1;
}

function:sql_check_phone_(playerid, number) {
    if(cache_num_rows()) {
        SendClientMessage(playerid, -1, "Acest numar este deja luat.");
        Dialog_Show(playerid, DIALOG_IPHONE, DIALOG_STYLE_INPUT, "iPhone", "Scrie mai jos ce numar de telefon doresti. (minim 4 cifre)", "Choose", "Cancel");
        return true;
    }
    playerInfo[playerid][pPremiumPoints] -= 100;
    playerInfo[playerid][pPhone] = 1;
    
    MAP_remove_val(phoneNumbers, playerInfo[playerid][pPhoneNumber]);

    playerInfo[playerid][pPhoneNumber] = number;
    MAP_insert_val_val(phoneNumbers, playerInfo[playerid][pPhoneNumber], playerid);

    va_SendClientMessage(playerid, -1, "Ai cumparat un iPhone pentru 100 de premium points. Numarul tau de telefon este %d.", number);

    SendClientMessage(playerid, -1, "Pentru a dezactiva apelurile si a primi doar SMS-uri, foloseste /turn off calls.");
    SendClientMessage(playerid, -1, "Pentru a raspunde la ultimul SMS primit, foloseste /reply <Text>.");

    format(returnString, sizeof(returnString), "UPDATE `users` SET `Phone` = '%d', `iPhone` = '%d', `PremiumPoints` = '%d' WHERE `name` = '%s'", playerInfo[playerid][pPhoneNumber], playerInfo[playerid][pPhone], playerInfo[playerid][pPremiumPoints], playerInfo[playerid][pName]);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

Dialog:DIALOG_IPHONE(playerid, response, listitem, inputtext[])
{
    if(!response) return Dialog_Show(playerid, DIALOG_IPHONE, DIALOG_STYLE_INPUT, "iPhone", "Scrie mai jos ce numar de telefon doresti. (minim 4 cifre)", "Choose", "");
    if(strlen(inputtext) != 4) return Dialog_Show(playerid, DIALOG_IPHONE, DIALOG_STYLE_INPUT, "iPhone", "Scrie mai jos ce numar de telefon doresti. (minim 4 cifre)\n{d34747}Error: Min 4 and max 4 characters.", "Choose", "");
    if(!IsNumeric(inputtext)) return Dialog_Show(playerid, DIALOG_IPHONE, DIALOG_STYLE_INPUT, "iPhone", "Scrie mai jos ce numar de telefon doresti. (minim 4 cifre)\n{d34747}Error: Numar invalid (trebuie sa fie intre 1000 si 9999).", "Choose", "");
    if((inputtext[0] == '0')) return Dialog_Show(playerid, DIALOG_IPHONE, DIALOG_STYLE_INPUT, "iPhone", "Scrie mai jos ce numar de telefon doresti. (minim 4 cifre)\n{d34747}Error: Numar invalid (trebuie sa fie intre 1000 si 9999).", "Choose", "");

    mysql_format(SQL, returnString, 128, "SELECT * FROM `users` WHERE `Phone`='%d' LIMIT 1", strval(inputtext));
    mysql_tquery(SQL, returnString, "sql_check_phone_", "ii", playerid, strval(inputtext));

    return true;
}


Dialog:DIALOG_CHANGENAME(playerid, response, listitem, inputtext[])
{
    if(strlen(inputtext) < 3 || strlen(inputtext) > MAX_PLAYER_NAME){
        Dialog_Show(playerid, DIALOG_CHANGENAME, DIALOG_STYLE_INPUT, "Change name:", "Please enter the new name below", "Select", "Cancel");

        return sendError(playerid, "Invalid new nickname.");
    }

    mysql_format(SQL, returnString, sizeof returnString, "select * from `users` where `Name` = '%e';", inputtext);
    mysql_tquery(SQL, returnString, "ChangePlayerNickname", "ds", playerid, inputtext);
    return true;
}

function:ChangePlayerNickname(playerid, newname[])
{
    if(cache_num_rows())
    {
        Dialog_Show(playerid, DIALOG_CHANGENAME, DIALOG_STYLE_INPUT, "Change name:", "Please enter the new name below", "Select", "Cancel");
        return sendError(playerid, "This name exist.");
    }

    format(WantName[playerid], MAX_PLAYER_NAME, newname);
    Dialog_Show(playerid, DIALOG_CONFIRMNAME, DIALOG_STYLE_MSGBOX, "Confirm:", va_return("Esti sigur ca vrei sa-ti schimbi numele in '%s'?", WantName[playerid]), "Da", "Nu");
    return true;
}

Dialog:DIALOG_CONFIRMNAME(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    sendStaffMessage(STAFF_TYPE_ADMIN, -1, "("COLOR_SV_HEX"Name Change{ffffff}): %s wants to change his/her name to %s. Type /an %d to accept this change.", GetName(playerid), WantName[playerid], playerid);

    sendSplitMessage(playerid, -1, "Cererea ta a fost trimisa adminilor. Asteapta pana acestia vor confirma. In caz ca se va da restart la server, va rugam sa refaceti cererea.");
    return true;
}

YCMD:acceptname(playerid, params[], help)
{
    check_admin

    new returnPlayer;
    if(sscanf(params, "u", returnPlayer))
        return sendSyntaxMessage(playerid, "/acceptname [name/playerid]");

    if(!IsPlayerConnected(returnPlayer))
        return SendClientMessage(playerid, COLOR_GREY, "Player not connected.");

    if(strmatch(WantName[returnPlayer], "NULL"))
        return SendClientMessage(playerid, COLOR_GREY, "This player didn't request to change his name.");

    new newname[MAX_PLAYER_NAME];

    format(newname, MAX_PLAYER_NAME, WantName[returnPlayer]);

    if(MySQLCheckAccount(WantName[returnPlayer]) != 0) 
        return SendClientMessage(playerid, COLOR_GREY, "This name already exist.");

    if(playerInfo[returnPlayer][pPremiumPoints] < 150)
    {
        SendClientMessage(returnPlayer, COLOR_GREY, "Your change name request was rejected because you don't have necesary premium points.");
        SendClientMessage(playerid, COLOR_GREY, "This player don't have necesary premium points and his change name request was rejected.");

        WantName[returnPlayer] = "NULL";
        return true;
    }
    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_RED, "{ffffff}("COLOR_SV_HEX"Staff Info{ffffff}): Admin %s changed %s name to %s.", GetDBName(playerid), GetDBName(returnPlayer), newname);

    va_SendClientMessage(returnPlayer, COLOR_YELLOW, "Administratorul %s ti-a acceptat cererea de schimbare a numelui!", GetName(playerid));
    SendClientMessage(returnPlayer, COLOR_YELLOW, "Ti-au fost retrase 150 premium points");

    playerInfo[returnPlayer][pPremiumPoints] -= 150;

    //updates
    mysql_format(SQL, returnString, sizeof returnString, "UPDATE `personalvehicles` SET `Owner` = '%s' WHERE `Owner` = '%s'", newname, GetDBName(returnPlayer));
    mysql_tquery(SQL, returnString, "", "");

    format(playerInfo[returnPlayer][pName], MAX_PLAYER_NAME, newname);
    update_player_nickname(returnPlayer);


    if(playerInfo[playerid][pHouse]) {
        format(houseInfo[playerInfo[returnPlayer][pHouse]][houseOwnerName], MAX_PLAYER_NAME, "%s", newname);
        WhenHouseGotUpdated(playerInfo[returnPlayer][pHouse]);
    }

    if(playerInfo[playerid][pBusiness]) {
        format(bizInfo[playerInfo[returnPlayer][pBusiness]][bizOwnerName], MAX_PLAYER_NAME, "%s", newname);
        WhenBusinessGotUpdated(playerInfo[returnPlayer][pBusiness]);
    }

    if(playerInfo[playerid][pBusiness]) {
        format(apInfo[playerInfo[returnPlayer][pApartament]][apOwnerName], MAX_PLAYER_NAME, "%s", newname);
        WhenApartamentGotUpdated(playerInfo[returnPlayer][pApartament]);
    }
    mysql_format(SQL, returnString, sizeof returnString, "UPDATE `bans` SET `name` = '%e' WHERE `name` = '%e' AND `bannedPlayer` = %d", newname, playerInfo[playerid][pName], playerInfo[playerid][pSQLID]); 
    mysql_tquery(SQL, returnString, "", "");

    mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Name` = '%e', `PremiumPoints` = '%d' where `id` = '%d';", newname, playerInfo[returnPlayer][pPremiumPoints], playerInfo[returnPlayer][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

YCMD:cancelname(playerid, params[], help)
{
    check_admin

    new returnPlayer;
    if(sscanf(params, "u", returnPlayer))
        return sendSyntaxMessage(playerid, "/cancelname [name/playerid]");

    if(!IsPlayerConnected(returnPlayer))
        return SendClientMessage(playerid, COLOR_GREY, "Player not connected.");

    if(strmatch(WantName[returnPlayer], "NULL"))
        return SendClientMessage(playerid, COLOR_GREY, "This player didn't request to change his name.");

    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_RED, "{ffffff}("COLOR_SV_HEX"Staff Info{ffffff}): Admin %s rejected %s change name request.", GetDBName(playerid), GetDBName(returnPlayer));

    va_SendClientMessage(returnPlayer, COLOR_YELLOW, "Administratorul %s ti-a respins cererea de schimbare a numelui!", GetName(playerid));
    WantName[returnPlayer] = "NULL";
    return true;
}

YCMD:forcenamechange(playerid, params[], help) {
    if(playerInfo[playerid][pAdmin] < 1)
        return sendError(playerid, "Nu ai gradul administrativ necesar pentru a folosi aceasta comanda.");

    new userID; gString[0] = EOS; if(sscanf(params, "us[144]", userID, gString))
        return sendSyntaxMessage(playerid, "/forcenamechange [playerid/name] [reason]");

    if(!playerStaticVar[userID][playerLogged])
        return sendError(playerid, "Acel player nu este logat pe server.");

    new const length = strlen(gString);

    if(length < 3 || length > 64) 
        return sendError(playerid, "Motiv-ul trebuie sa cuprinda intre 3 si 64 caractere.");

    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_RED, "{ffffff}("COLOR_SV_HEX"Staff Info{ffffff}): Admin %s forced player %s (%d) to change his nickname. Reason: %s", GetName(playerid), GetName(userID), userID, gString);
    SendClientMessage(userID, COLOR_GREY, "* Admin %s forced you to change your nickname. Reason: %s", GetName(playerid), gString);

    TogglePlayerControllableEx(userID, false);

    Dialog_Show(userID, FORCENAMECHANGE, DIALOG_STYLE_INPUT, "SERVER: Force Name Change", "Enter a new nickname in the box below:", "Ok", "Kick");
    return true;
}

Dialog:FORCENAMECHANGE(playerid, response, listitem, inputtext[]) {
    if(!response) {
        sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_RED, "{ffffff}("COLOR_SV_HEX"Staff Info{ffffff}): Player %s don't changed his nickname via [/forcenamechange] and got kicked from the server.", GetName(playerid)); 
        return Kick(playerid);
    }
    mysql_format(SQL, returnString, sizeof returnString, "SELECT (ID) FROM `users` WHERE `Name` = '%e'", inputtext); 
    mysql_tquery(SQL, returnString, "forceNameChange", "ds", playerid, inputtext);
    return 1;
}

function:forceNameChange(playerid, const inputtext[]) {
    if(!IsPlayerConnected(playerid))
        return 1;

    new const success = SetPlayerName(playerid, inputtext);
    SetPlayerName(playerid, GetName(playerid));

    if(cache_num_rows() || success != 1 || strlen(inputtext) < 3)
        return Dialog_Show(playerid, FORCENAMECHANGE, DIALOG_STYLE_INPUT, "SERVER: Force Name Change", "Enter a new nickname in the box below:\n{FF0000}Nickname must be not used and needs to have 3 - 24 characters long and only contain valid characters.", "Ok", "Kick");

    if(playerInfo[playerid][pHouse]) {
        format(houseInfo[playerInfo[playerid][pHouse]][houseOwnerName], MAX_PLAYER_NAME, "%s", inputtext);
        WhenHouseGotUpdated(playerInfo[playerid][pHouse]);
    }

    if(playerInfo[playerid][pBusiness]) {
        format(bizInfo[playerInfo[playerid][pBusiness]][bizOwnerName], MAX_PLAYER_NAME, "%s", inputtext);
        WhenBusinessGotUpdated(playerInfo[playerid][pBusiness]);
    }

    if(playerInfo[playerid][pBusiness]) {
        format(apInfo[playerInfo[playerid][pApartament]][apOwnerName], MAX_PLAYER_NAME, "%s", inputtext);
        WhenApartamentGotUpdated(playerInfo[playerid][pApartament]);
    }


    mysql_format(SQL, returnString, sizeof returnString, "UPDATE `personalvehicles` SET `Owner` = '%s' WHERE `Owner` = '%s'", inputtext, GetDBName(playerid));
    mysql_tquery(SQL, returnString, "", "");

    mysql_format(SQL, returnString, sizeof returnString, "UPDATE `bans` SET `name` = '%e' WHERE `name` = '%e' AND `bannedPlayer` = %d", inputtext, playerInfo[playerid][pName], playerInfo[playerid][pSQLID]); 
    mysql_tquery(SQL, returnString, "", "");

    mysql_format(SQL, returnString, sizeof returnString, "UPDATE `users` SET `Name` = '%e' WHERE `ID` = %d", inputtext, playerInfo[playerid][pSQLID]); 
    mysql_tquery(SQL, returnString, "", "");
    


    SendClientMessage(playerid, -1, "Your nickname was changed succesfully. You need to reconnect with the new nickname on the server to apply the changes."); 
    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_RED, "{ffffff}("COLOR_SV_HEX"Staff Info{ffffff}): %s changed his nickname via [/forcenamechange] to %s.", GetName(playerid), inputtext);
    KickEx(playerid);
    return 1;
}
new r_clanName[MAX_PLAYERS][25], r_clanTag[MAX_PLAYERS][15];
Dialog:DIALOG_CHOOSECLANNAME(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    if(strlen(inputtext) < 3 || strlen(inputtext) > 20)
        return Dialog_Show(playerid, DIALOG_CHOOSECLANNAME, DIALOG_STYLE_INPUT, "Clan name", "Insert clan name in the box below:\nMinimum 3 characters and maximum 20 characters.\n(You can't change it after that!)", "Ok", "");

    strcpy(r_clanName[playerid], inputtext);

    mysql_format(SQL, returnString, sizeof returnString, "select * from `clans` where `clanName` = '%e';", r_clanName[playerid]);
    mysql_tquery(SQL, returnString, "CheckClanName", "d", playerid);
    return true;
}

function:CheckClanName(playerid)
{
    if(playerInfo[playerid][pPremiumPoints] < 500)
        return Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Not enough points", "You don't have enough premium points to do this.", "Close", "");

    if(cache_num_rows())
    {
        SendClientMessage(playerid, -1, "Exista deja un clan inregistrat cu acest nume.");
        return Dialog_Show(playerid, DIALOG_CHOOSECLANNAME, DIALOG_STYLE_INPUT, "Clan name", "Insert clan name in the box below:\n(You can't change it after that!)", "Ok", "");
    }

    va_SendClientMessage(playerid, COLOR_GENANNOUNCE, "Clan name: %s.", r_clanName[playerid]);
    Dialog_Show(playerid, DIALOG_CHOOSECLANTAG, DIALOG_STYLE_INPUT, "Clan tag", "Insert clan tag in the box below:\n(You can't change it after that!)", "Ok", "");
    return true;
}

Dialog:DIALOG_CHOOSECLANTAG(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    if(isnull(inputtext))
        return Dialog_Show(playerid, DIALOG_CHOOSECLANTAG, DIALOG_STYLE_INPUT, "Clan tag", "Insert clan tag in the box below:\n(You can't change it after that!)", "Ok", "");

    if(strfind(inputtext, "[", true) != -1 || strfind(inputtext, "]", true) != -1 || strfind(inputtext, ".", true) != -1)
    {
        SendClientMessage(playerid, -1, "Nu poti folosi aceste simboluri.");
        return Dialog_Show(playerid, DIALOG_CHOOSECLANTAG, DIALOG_STYLE_INPUT, "Clan tag", "Insert clan tag in the box below:\n(You can't change it after that!)", "Ok", "");
    }

    strcpy(r_clanTag[playerid], inputtext);

    mysql_format(SQL, returnString, sizeof returnString, "select * from `clans` where `clanTag` = '%e';", r_clanTag[playerid]);
    mysql_tquery(SQL, returnString, "CheckClanTag", "d", playerid);
    return true;
}

function:CheckClanTag(playerid)
{
    if(playerInfo[playerid][pPremiumPoints] < 500)
        return Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Not enough points", "You don't have enough premium points to do this.", "Close", "");

    if(cache_num_rows())
    {
        SendClientMessage(playerid, -1, "Exista deja un clan inregistrat cu acest TAG.");
        return Dialog_Show(playerid, DIALOG_CHOOSECLANTAG, DIALOG_STYLE_INPUT, "Clan tag", "Insert clan tag in the box below:\n(You can't change it after that!)", "Ok", "");
    }

    va_SendClientMessage(playerid, COLOR_GENANNOUNCE, "Clan tag: %s.", r_clanTag[playerid]);

    mysql_format(SQL, returnString, sizeof returnString, "insert into `clans` (`clanName`, `clanTag`) values ('%e', '%e');", r_clanName[playerid], r_clanTag[playerid]);
    mysql_tquery(SQL, returnString, "CreateClan", "dss", playerid, r_clanName[playerid], r_clanTag[playerid]);

    SendClientMessage(playerid, COLOR_GREY, "Cererea de inregistrare a clanului tau a fost trimisa catre baza de date! Crearea acestuia poate dura pana la 5 minute.");
    return true;
}

function:CreateClan(playerid, cName[], cTag[])
{
    new cId = cache_insert_id();

    clanInfo[cId][clanID] = cId;
    format(clanInfo[cId][clanName], 25, cName);
    format(clanInfo[cId][clanTag], 15, cTag);

    format(clanInfo[cId][clanColor], 15, "FFCC99");
    format(clanInfo[cId][clanRank1], 32, "Newbie");
    format(clanInfo[cId][clanRank2], 32, "Member");
    format(clanInfo[cId][clanRank3], 32, "Advanced");
    format(clanInfo[cId][clanRank4], 32, "Expert");
    format(clanInfo[cId][clanRank5], 32, "Legend");
    format(clanInfo[cId][clanRank6], 32, "Manager");
    format(clanInfo[cId][clanRank7], 32, "Owner");

    SendClientMessage(playerid, COLOR_GREY, "Clanul tau a fost inregistrat cu succes! Ti-au fost retrase 500 premium points!");
    playerInfo[playerid][pPremiumPoints] -= 500;

    playerInfo[playerid][pClan] = cId;
    Iter_Add(clanMembers<playerInfo[playerid][pClan]>, playerid);

    playerInfo[playerid][pClanRank] = 7;
    playerInfo[playerid][pClanDays] = gettime();

    mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Clan` = '%d', `ClanRank` = '7', `ClanDays` = '%d', `PremiumPoints` = '%d' where `id` = '%d';", cId, gettime(), playerInfo[playerid][pPremiumPoints], playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

Dialog:DIALOG_CONFIRMVIPHOUSE(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    if(playerInfo[playerid][pPremiumPoints] < 100)
        return sendError(playerid, "You don't have enough premium points.");

    mysql_format(SQL, returnString, sizeof returnString, "update `houses` set `VIP` = '1' where `ID` = '%d';", playerInfo[playerid][pHouse]);
    mysql_tquery(SQL, returnString, "", "");

    SendClientMessage(playerid, COLOR_GREY, "Upgrade-ul a fost achizitionat. Foloseste /housefurniture. (-100 premium points)");

    houseInfo[playerInfo[playerid][pHouse]][houseVIP] = 1;
    WhenHouseGotUpdated(playerInfo[playerid][pHouse]);

    playerInfo[playerid][pPremiumPoints] -= 100;

    mysql_format(SQL, returnString, sizeof returnString, "update `users` set `PremiumPoints` = '%d' where `id` = '%d';", playerInfo[playerid][pPremiumPoints], playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

Dialog:DIALOG_BUYGARAGE(playerid, response, listitem, inputtext[])
{
    if(response)
        return Dialog_Show(playerid, DIALOG_BUYGARAGE2, DIALOG_STYLE_LIST, "Buy Garage", "Cumpara cu bani in joc ($2,000,000,000)\nCumpara folosind 100 premium points", "Cumpara", "Nu cumpara");

    return true;
}

Dialog:DIALOG_BUYGARAGE2(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    Dialog_Show(playerid, DIALOG_BUYGARAGE3, DIALOG_STYLE_MSGBOX, "Buy Garage", "Atentie: Garajul se va crea in pozitia ta actuala. Iesirea se va face in directia in care te uiti acum", "Creeaza", "Anuleaza");
    playerStaticVar[playerid][pDialogItems][0] = listitem;
    return true;
}

Dialog:DIALOG_BUYGARAGE3(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    if(!IsPlayerInRangeOfPoint(playerid, 80.0, houseInfo[playerInfo[playerid][pHouse]][houseExterior][0], houseInfo[playerInfo[playerid][pHouse]][houseExterior][1], houseInfo[playerInfo[playerid][pHouse]][houseExterior][2]))
        return SendClientMessage(playerid, COLOR_GREY, "Esti prea departe de casa.");

    new Float:returnPos[4];

    GetPlayerPos(playerid, returnPos[0], returnPos[1], returnPos[2]);
    GetPlayerFacingAngle(playerid, returnPos[3]);

    if(!playerStaticVar[playerid][pDialogItems][0])
    {

        new returnAmount = 2000000000;

        if(playerInfo[playerid][pAccount] < returnAmount && playerInfo[playerid][pAccountLY] < 1)
            return sendError(playerid, "Nu ai banii necesari in banca.");

        substractBankMoney(playerid, playerInfo[playerid][pAccountLY], playerInfo[playerid][pAccount], 0, returnAmount); 

        SendClientMessage(playerid, -1, "Garajul a fost creat. Ai platit $2,000,000,000.");

        mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Money` = '%d', `BankLY` = '%d', `Bank` = '%d' where `id` = '%d';", playerInfo[playerid][pMoney], playerInfo[playerid][pAccountLY], playerInfo[playerid][pAccount], playerInfo[playerid][pSQLID]);
        mysql_tquery(SQL, returnString, "", "");
    }
    else
    {
        if(playerInfo[playerid][pPremiumPoints] < 100)
            return sendError(playerid, "You don't have enough premium points.");

        playerInfo[playerid][pPremiumPoints] -= 100;
        SendClientMessage(playerid, -1, "Garajul a fost creat. Ai platit 100 de premium points.");

        mysql_format(SQL, returnString, sizeof returnString, "update `users` set `PremiumPoints` = '%d' where `id` = '%d';", playerInfo[playerid][pPremiumPoints], playerInfo[playerid][pSQLID]);
        mysql_tquery(SQL, returnString, "", "");
    }
    SendClientMessage(playerid, -1, "Pentru setarile garajului te poti folosi de comanda /garage.");


    mysql_format(SQL, returnString, sizeof returnString, "insert into `garages` (`House`, `garageX`, `garageY`, `garageZ`, `garageA`) values ('%d', '%f', '%f', '%f', '%f');", playerInfo[playerid][pHouse], returnPos[0], returnPos[1], returnPos[2], returnPos[3]);
    mysql_tquery(SQL, returnString, "CreateGarage", "iffff", playerInfo[playerid][pHouse], returnPos[0], returnPos[1], returnPos[2], returnPos[3]);
    return true;
}

YCMD:help(playerid, params[], help)
{
    Dialog_Show(playerid, DIALOG_HELP, DIALOG_STYLE_LIST, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Commands", "General\nChat\nGroup\nHouses\nJobs\nBusinesses\nHelpers\nVehicles\nBank\nLeader", "Select", "Exit");
    return true;
}

Dialog:DIALOG_HELP(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    switch(listitem)
    {

        case 0: Dialog_Show(playerid, DIALOG_HELP_RETURN, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} General Commands", "{FFFFFF}/stats /accept /eject\n/killcp /ad /admins\n/helpers /time /id /changepass /pay\n/buylevel /fill /fillgascan /gps /shop\n/contract /service /buydrink /licences", "Back", "Exit");
        case 1: Dialog_Show(playerid, DIALOG_HELP_RETURN, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Chat Commands", "{FFFFFF}/n (newbie chat message)\n/sms (OOCly SMS another player)\n/w(hisper) /wt (walkie talkie)", "Back", "Exit");
        case 2:
        {
            if(!playerInfo[playerid][pGroup])
                return Dialog_Show(playerid, DIALOG_HELP_RETURN, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Group Commands", "{FFFFFF}You're not in a group.", "Back", "Exit");

            switch(playerInfo[playerid][pGroup])
            {
                case 1, 2, 3, 8:
                {
                    Dialog_Show(playerid, DIALOG_HELP_RETURN, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Group Commands", "{FFFFFF}/mdc /su /arrest /free /confiscate /wanted\n\
                        /m(egaphone) /tazer /cuff /untie /duty /backup /ram\n\
                        /uncuff /frisk /clear /d(epartments) /r(adio) /checkdays", "Back", "Exit");
                }
                case 4, 5, 6, 10: Dialog_Show(playerid, DIALOG_HELP_RETURN, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Group Commands", "{FFFFFF}/order /attack /f /fdrugs\n/fmats /fdeposit /tie /untie /checkdays", "Back", "Exit");
                case 9: Dialog_Show(playerid, DIALOG_HELP_RETURN, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Group Commands", "{FFFFFF}/news /live /checkdays", "Back", "Exit");
                case 12: Dialog_Show(playerid, DIALOG_HELP_RETURN, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Group Commands", "{FFFFFF}/givelicence /licencesrequest /acceptlic\n/closelic /clicense /license /f /checkdays", "Back", "Exit");
                case 13: Dialog_Show(playerid, DIALOG_HELP_RETURN, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Group Commands", "{FFFFFF}/fare /accept taxi /f /checkdays /tcalls", "Back", "Exit");
                case 14: Dialog_Show(playerid, DIALOG_HELP_RETURN, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Group Commands", "{FFFFFF}/heal /f /checkdays", "Back", "Exit");
            }
        }
        case 3: Dialog_Show(playerid, DIALOG_HELP_RETURN, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} House Commands", "{FFFFFF}/home /sellhouseto /open /setrent /setrentable /sellhousetostate\n/renters /evict /evictall/housewithdraw /housename\n/heal /hu", "Back", "Exit");
        case 4:
        {
            if(!job_data[playerid][playerJob])
                return Dialog_Show(playerid, DIALOG_HELP_RETURN, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Job Commands", "{FFFFFF}You don't have a public job or your job does not have any commands.", "Back", "Exit");

            Dialog_Show(playerid, DIALOG_HELP_RETURN, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Job Commands", va_return("{FFFFFF}%s", GetJobCommand(job_data[playerid][playerJob])), "Back", "Exit");
        }
        case 5: Dialog_Show(playerid, DIALOG_HELP_RETURN, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Business Commands", "{FFFFFF}/bizstatus /bizfee /bizname /open /sellbizto /sellbiztostate /bizwithdraw", "Back", "Exit");
        case 6:
        {
            if(!playerInfo[playerid][pHelper])
                return SendClientMessage(playerid, COLOR_GREY, "You aren't an official helper.");

            Dialog_Show(playerid, DIALOG_HELP_RETURN, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Helper Commands", "{FFFFFF}Helper 1:\n/n /nsend /spawnhere /slap /spawn /nmute /nreport /nqueue /ncurrent /ndelete /nskip /nnext /hquestions /hre\nHelper 2:\n/freeze /unfreeze /mute /unmute", "Back", "Exit");
        }
        case 7: Dialog_Show(playerid, DIALOG_HELP_RETURN, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Vehicle Commands", "{FFFFFF}/v /lock /findcar /park /towcar /engine /removetunning /givekey /sellcarto", "Back", "Exit");
        case 8: Dialog_Show(playerid, DIALOG_HELP_RETURN, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Bank Commands", "{FFFFFF}/balance /withdraw /deposit", "Back", "Exit");
        case 9:
        {
            if(playerInfo[playerid][pGroupRank] != 7)
                return sendError(playerid, "You are not a leader.");

            Dialog_Show(playerid, DIALOG_HELP_RETURN, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Leader Help", "{FFFFFF}/invite /l /lockhq\n/granknames /fvrespawn /members\n/gov", "Back", "Exit");
        }
    }
    return true;
}

Dialog:DIALOG_HELP_RETURN(playerid, response, listitem, inputtext[])
{
    if(response)
        //return Dialog_Show(playerid, DIALOG_HELP, DIALOG_STYLE_LIST, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Commands", "General\nChat\nFactions\nHouses\nJobs\nBusinesses\nHelpers\nVehicles\nBank\nRob\nLeaders\nClans", "Select", "Exit");
        return Dialog_Show(playerid, DIALOG_HELP, DIALOG_STYLE_LIST, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Commands", "General\nChat\nGroup\nHouses\nJobs\nBusinesses\nHelpers\nVehicles\nBank\nLeader", "Select", "Exit");
    return true;
}

YCMD:tog(playerid, params[], help)  {
    Dialog_Show(playerid, TOG, DIALOG_STYLE_TABLIST_HEADERS, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Tog", "Function Name\tStatus\nWalkie Talkie Chat\t%s\nVehicle Surf\t%s\nPay\t%s\nFind\t%s\nPremium Chat\t%s", "Select", "Exit", playerStaticVar[playerid][pFreqToggled] ? "{0AC917}enabled" : "{990000}disabled", playerInfo[playerid][pTOGSurf] ? "{0AC917}enabled" : "{990000}disabled", playerInfo[playerid][pTOGPay] ? "{0AC917}enabled" : "{990000}disabled", playerInfo[playerid][pTOGFind] ? "{0AC917}enabled" : "{990000}disabled", playerStaticVar[playerid][var_premium_tog] ? "{0AC917}enabled" : "{990000}disabled");
    return true;
}

Dialog:TOG(playerid, response, listitem, inputtext[]) {
    if(!response) 
        return 1;

    switch(listitem) {
        case 0: {
            if(!playerInfo[playerid][pWalkieTalkie])
                return sendError(playerid, "Nu ai un walkie talkie.");

            if(playerStaticVar[playerid][pFreqToggled] == true) 
                return sendError(playerid, "Ai frecventa oprita.");

            if(playerStaticVar[playerid][pFreqToggled] == true) {
                playerStaticVar[playerid][pFreqToggled] = false;
                return SendClientMessage(playerid, -1, "Your TOG has been updated! (Walkie Talkie disabled)");
            }

            playerStaticVar[playerid][pFreqToggled] = true;
            return SendClientMessage(playerid, COLOR_YELLOW, "Your TOG has been updated! (Walkie Talkie enabled)");
        }
        case 1: {
            playerInfo[playerid][pTOGSurf] = !playerInfo[playerid][pTOGSurf];

            return SendClientMessage(playerid, COLOR_YELLOW, playerInfo[playerid][pTOGSurf] ? "Your TOG has been updated! (Vehicle Surf disabled)" : "Your TOG has been updated! (Vehicle Surf enabled)");
        }
        case 2: {
            playerInfo[playerid][pTOGPay] = !playerInfo[playerid][pTOGPay];

            return SendClientMessage(playerid, COLOR_YELLOW, playerInfo[playerid][pTOGPay] ? "Your TOG has been updated! (Pay enabled)" : "Your TOG has been updated! (Pay disabled)");
        }
        case 3: {
            if(!playerInfo[playerid][pAdmin])
                return sendError(playerid, "Nu ai gradul administrativ necesar.");

            playerInfo[playerid][pTOGFind] = !playerInfo[playerid][pTOGFind];

            return SendClientMessage(playerid, COLOR_YELLOW, playerInfo[playerid][pTOGFind] ? "Your TOG has been updated! (Find enabled)" : "Your TOG has been updated! (Find disabled)");
        }
        case 4: {
            if(!playerInfo[playerid][pAdmin] && !playerInfo[playerid][pPremiumAccount])
                return sendError(playerid, "Nu ai gradul administrativ necesar.");

            playerStaticVar[playerid][var_premium_tog] = !playerStaticVar[playerid][var_premium_tog];

            return SendClientMessage(playerid, COLOR_YELLOW, "* Premium chat has been %s.", playerStaticVar[playerid][var_premium_tog] ? "enabled" : "disabled");
        }
    }
    return 1;
}

YCMD:premium(playerid, params[], help) {


    new captionFormat[32];
    format(captionFormat, sizeof captionFormat, "Premium Account Informations");
    Dialog_Show(playerid, DIALOG_CONFIRMPREMIUM, DIALOG_STYLE_MSGBOX, captionFormat, "- Acces la comanda /pcolor, /vip\n- 2 Faction Punish-uri in minus la payday.\n- Mai multe respect points la payday\n- Limita marita la 100 la rob points in loc de 10\n- Mai putine rob points la /rob\n- Un "SERVER_NAME2" CASE in plus la payday.\n- /ad oriunde.\n- timp mai mic la /ad.\n- Cu 3 mai multe slot-uri la vehiculele personale.", "Buy", "Exit");
    return true;
}
YCMD:givepayday(playerid, params[], help) {
    check_owner

    WhenPlayersGotPayday();
    return true;
}
YCMD:pay(playerid, params[], help) {
    check_delay
    new returnPlayer, money; if(sscanf(params, "ud", returnPlayer, money))
        return sendSyntaxMessage(playerid, "/pay [playerid/name] [money]");

    if(returnPlayer == playerid)
        return sendError(playerid, "Nu poti folosi aceasta comanda pe tine.");

    if(!IsPlayerConnected(returnPlayer)) 
        return sendError(playerid, "Acel player nu este conectat.");

    if(!IsPlayerInRangeOfPlayer(playerid, returnPlayer, 4.5))
        return sendError(playerid, "Acel player nu este langa tine.");

    if(!playerInfo[returnPlayer][pTOGPay])
        return sendError(playerid, "Acel player a dezactivat optiunea de a primii bani de la alti jucatori prin intermediul acestei comenzi.");

    if(!playerInfo[playerid][pPremiumAccount] && money > 100000)
        return sendError(playerid, "Nu poti trimite mai mult de $100.000, deoarece nu ai cont premium.");

    if(GetMoney(playerid) < money && GetMilliards(playerid) < 1)
        return sendError(playerid, "You don't have enough money.");
       
    substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, money);

    addMoney(returnPlayer, GetMilliards(returnPlayer), GetMoney(returnPlayer), 0, money, false);



    va_SendClientMessage(playerid, COLOR_YELLOW, "* You sended $%s money to %s.", FormatNumber(money), GetName(returnPlayer));
    va_SendClientMessage(returnPlayer, COLOR_YELLOW, "* %s sended $%s money to you.", GetName(playerid), FormatNumber(money));

    sendNearbyMessage(playerid, COLOR_PURPLE, "* %s takes out $%s in cash, and hands it to %s.", GetName(playerid), FormatNumber(money), GetName(returnPlayer));

    set_delay;

    return true;
}

YCMD:whisper(playerid, params[], help) {
    if(playerInfo[playerid][pLevel] < 3)
        return sendError(playerid, "Iti trebuie minim level 3 pentru a folosi aceasta comanda.");

    new returnPlayer, whisper[128];

    if(sscanf(params, "us[128]", returnPlayer, whisper))
        return sendSyntaxMessage(playerid, "/whisper [playerid/name] [message]");

    if(returnPlayer == playerid) 
        return sendError(playerid, "Nu poti folosi aceasta comanda asupra ta.");

    if(!playerStaticVar[returnPlayer][playerLogged])
        return sendError(playerid, "Acel player nu este logat.");

    if(!IsPlayerInRangeOfPlayer(playerid, returnPlayer, 5.0))
        return sendError(playerid, "Nu esti in apropierea acelui player.");

    va_SendClientMessage(playerid, COLOR_LIGHTBLUE, "(Whisper) sent to %s (%d): %s", GetName(returnPlayer), returnPlayer, whisper);
    va_SendClientMessage(returnPlayer, COLOR_LIGHTBLUE, "(Whisper) from %s (%d): %s", GetName(playerid), playerid, whisper);
    return true;
}
YCMD:carwhisper(playerid, params[], help) {
    new const vehicleid = GetPlayerVehicleID(playerid);

    if(!vehicleid)
        return 1;

    if(isnull(params) || strlen(params) >= 128)
        return sendSyntaxMessage(playerid, "/carwhisper [message]");

    new cWhisper[128];
    format(cWhisper, sizeof cWhisper, "(Car Whisper) %s %s (%d): %s", GetPlayerState(playerid) == PLAYER_STATE_DRIVER ? "Driver" : "Passenger", GetName(playerid), playerid, params);

    foreach(new i : playersInVehicle) {
        if(GetPlayerVehicleID(i) != vehicleid) 
            continue;

        SendClientMessage(i, COLOR_YELLOW, cWhisper);
    }
    return 1;
}



YCMD:gps(playerid, params[], help)
{
    if(playerStaticVar[playerid][pCheckpoint])
        return showActiveCheckpointDialog(playerid);

    Dialog_Show(playerid, DIALOG_LOCATIONS, DIALOG_STYLE_LIST, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Server Locations", "Driving School(DMV)\nServer Businesses\nFaction HQS\nVehicle Mod Shops\nPaintball\nDealership\nCNN\nATM's", "Select", "Close");
    return true;
}

YCMD:hqs(playerid, params[], help)
{
    if(playerStaticVar[playerid][pCheckpoint])
        return showActiveCheckpointDialog(playerid);

    new groupString[512];
    for(new i = 1; i <= serverFactions; i++)
        format(groupString, sizeof groupString, "%s%s\n", groupString, factionName(i));

    Dialog_Show(playerid, DIALOG_HQS, DIALOG_STYLE_LIST, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} HQ Locations", groupString, "Checkpoint", "Back");
    return true;
}

YCMD:romana(playerid, params[], help)
{
    playerInfo[playerid][pLanguage] = 1;
    SendClientMessage(playerid, -1, "Ti-ai setat limba romana.");

    mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Language` = '%d' where `id` = '%s';", playerInfo[playerid][pLanguage], playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

YCMD:english(playerid, params[], help)
{
    playerInfo[playerid][pLanguage] = 0;
    SendClientMessage(playerid, -1, "You have set your language english.");

    mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Language` = '%d' where `id` = '%s';", playerInfo[playerid][pLanguage], playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

Dialog:DIALOG_HQS(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    SendClientMessage(playerid, -1, "I have placed a checkpoint at that location. Please follow it.");

    SetPlayerCheckpoint(playerid, groupInfo[listitem + 1][factionExterior][0], groupInfo[listitem + 1][factionExterior][1], groupInfo[listitem + 1][factionExterior][2], 5.0);
    playerStaticVar[playerid][pCheckpoint] = 1;
    return true;
}

Dialog:DIALOG_LOCATIONS(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    switch(listitem)
    {
        case 0:
        {
            SetPlayerCheckpoint(playerid, 1219.1234, -1812.3678, 16.5938, 4.0);
            playerStaticVar[playerid][pCheckpoint] = 1;

            SendClientMessage(playerid, COLOR_YELLOW, "Ti-a fost pus un checkpoint catre locatia importanta aleasa de tine!");
        }
        case 1: {

            returnString = "ID\tName\tDistance\n";

            for(new i = 1; i < returnServerBusinesses; i ++)
                format(returnString, sizeof returnString, "%s#%d\t%s\t%.2f km\n", returnString, bizInfo[i][bizID], business_name(bizInfo[i][bizType]), GetPlayerDistanceFromPoint(playerid, bizInfo[i][bizPosition][0], bizInfo[i][bizPosition][1], bizInfo[i][bizPosition][2]) / 1000);

            Dialog_Show(playerid, DIALOG_LOCATIONBUSINESS, DIALOG_STYLE_TABLIST_HEADERS, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Businesses", returnString, "Checkpoint", "Back");
            return 1;
        } 
        case 2: Command_ReProcess(playerid, "hqs", false);
        case 3:
        {
            returnString = "{ffffff}ID\t{ffffff}Descriere\t{ffffff}Distanta\n";

            for(new i = 1; i <= returnServerBusinesses; i++)
            {
                if(bizInfo[i][bizType] != 11)
                    continue;

                format(returnString, sizeof returnString, "%s%d\t%s\t%.02f km\n", returnString, bizInfo[i][bizID], GetCoordinatesCity(bizInfo[i][bizPosition][0], bizInfo[i][bizPosition][1]), (GetPlayerDistanceFromPoint(playerid, bizInfo[i][bizPosition][0], bizInfo[i][bizPosition][1], bizInfo[i][bizPosition][2]) / 1000));
                playerStaticVar[playerid][pDialogItems][i] = bizInfo[i][bizID];
            }
            Dialog_Show(playerid, DIALOG_LOCATIONS_FINDBIZ, DIALOG_STYLE_TABLIST_HEADERS, "Vehicle Mod Shops", returnString, "Checkpoint", "Back");
        }
        case 4: SetPlayerCheckpoint(playerid, 1154.4122, -1440.0548, 15.7969, 5.0), playerStaticVar[playerid][pCheckpoint] = 1;
        case 5: SetPlayerCheckpoint(playerid, 326.4294, -1517.0165, 36.0391, 5.0), playerStaticVar[playerid][pCheckpoint] = 1;
        case 6:
        {
            returnString = "{ffffff}ID\t{ffffff}Descriere\t{ffffff}Distanta\n";

            for(new i = 1; i <= returnServerBusinesses; i++)
            {
                if(bizInfo[i][bizType] != 13)
                    continue;

                format(returnString, sizeof returnString, "%s%d\t%s\t%.02f km\n", returnString, bizInfo[i][bizID], GetCoordinatesCity(bizInfo[i][bizPosition][0], bizInfo[i][bizPosition][1]), (GetPlayerDistanceFromPoint(playerid, bizInfo[i][bizPosition][0], bizInfo[i][bizPosition][1], bizInfo[i][bizPosition][2]) / 1000));
                playerStaticVar[playerid][pDialogItems][i] = bizInfo[i][bizID];
            }
            Dialog_Show(playerid, DIALOG_LOCATIONS_FINDBIZ, DIALOG_STYLE_TABLIST_HEADERS, "CNNs", returnString, "Checkpoint", "Back");
        }
        case 7:
        {
            returnString = "{ffffff}ID\t{ffffff}Descriere\t{ffffff}Distanta\n";

            for(new i = 1; i <= returnServerATMS; i++)
            {
                format(returnString, sizeof returnString, "%s%d\t%s\t%.02f km\n", returnString, AtmInfo[i][idATM], GetCoordinatesCity(AtmInfo[i][fATMPos][0], AtmInfo[i][fATMPos][1]), (GetPlayerDistanceFromPoint(playerid, AtmInfo[i][fATMPos][0], AtmInfo[i][fATMPos][1], AtmInfo[i][fATMPos][2]) / 1000));
                playerStaticVar[playerid][pDialogItems][i] = AtmInfo[i][idATM];
            }
            Dialog_Show(playerid, DIALOG_LOCATIONSCP, DIALOG_STYLE_TABLIST_HEADERS, ""SERVER_NAME2" ATM's", returnString, "Checkpoint", "Back");
        }
    }
    return true;
}

Dialog:DIALOG_LOCATIONBUSINESS(playerid, response, listitem, inputtext[])
{
    if(!response)
        return 1;

    new i = listitem + 1;

    SendClientMessage(playerid, COLOR_YELLOW, "A checkpoint was placed on your map."); 
    SetPlayerCheckpoint(playerid, bizInfo[i][bizPosition][0], bizInfo[i][bizPosition][1], bizInfo[i][bizPosition][2], 5.0);
    playerStaticVar[playerid][pCheckpoint] = 1;
    return true;
}

Dialog:DIALOG_LOCATIONSCP(playerid, response, listitem, inputtext[]) {

    if(!response)
        return 1;

    new i = listitem + 1;

    SendClientMessage(playerid, COLOR_YELLOW, "A checkpoint was placed on your map."); 
    SetPlayerCheckpoint(playerid, AtmInfo[i][fATMPos][0], AtmInfo[i][fATMPos][1], AtmInfo[i][fATMPos][2], 5.0);
    playerStaticVar[playerid][pCheckpoint] = 1;
    return true;
}

Dialog:DIALOG_LOCATIONS_FINDBIZ(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    new i = listitem + 1;

    SetPlayerCheckpoint(playerid, bizInfo[i][bizPosition][0], bizInfo[i][bizPosition][1], bizInfo[i][bizPosition][2], 4.0);
    playerStaticVar[playerid][pCheckpoint] = 1;
    return true;
}

public OnPlayerDisconnect(playerid, reason)
{
    if(playerStaticVar[playerid][var_wanted_timer] != Timer:0)
        stop playerStaticVar[playerid][var_wanted_timer];
    
    if(playerStaticVar[playerid][var_jail_timer] != Timer:0)
        stop playerStaticVar[playerid][var_jail_timer];

    if(case_timer[playerid] != Timer:0)
        stop case_timer[playerid], case_timer[playerid] = Timer:0;

    if(tutorial_timer[playerid] != Timer:0)  
        stop tutorial_timer[playerid], tutorial_timer[playerid] = Timer:0;

    if(login_timer[playerid] != Timer:0)  
        stop login_timer[playerid], login_timer[playerid] = Timer:0;
    
    if(vending_timer[playerid] != Timer:0)  
        vending_timer[playerid] = Timer:0;
        
    if(free_timer[playerid] != Timer:0)  
        free_timer[playerid] = Timer:0;

    switch(reason)
    {
        case 0: sendNearbyMessage(playerid, COLOR_GENANNOUNCE, "%s has timed out from the server (crash).", GetDBName(playerid));
        case 1: sendNearbyMessage(playerid, COLOR_GENANNOUNCE, "%s has left the server.", GetDBName(playerid));
        case 2: sendNearbyMessage(playerid, COLOR_GENANNOUNCE, "%s has been kicked or banned from the server.", GetDBName(playerid));
    }

    if(playerStaticVar[playerid][var_incall] != -1) {
        SetPlayerSpecialAction(playerStaticVar[playerid][var_incall], SPECIAL_ACTION_STOPUSECELLPHONE); 
        RemovePlayerAttachedObject(playerStaticVar[playerid][var_incall], INDEX_PHONE);

        SendClientMessage(playerStaticVar[playerid][var_incall], -1, "The line just went dead....");

        playerStaticVar[playerStaticVar[playerid][var_incall]][b_PHONE_SPEAKER] = false;
        playerStaticVar[playerid][var_incall] = playerStaticVar[playerStaticVar[playerid][var_incall]][var_incall] = playerStaticVar[playerStaticVar[playerid][var_incall]][var_calls] = playerStaticVar[playerid][var_calls] = playerStaticVar[playerid][var_callfrom] = playerStaticVar[playerStaticVar[playerid][var_incall]][var_callfrom] = -1;
    }

    g_MysqlRaceCheck[playerid] ++;
    RemovePlayerFromQueue(playerid);

    if(playerStaticVar[playerid][examStep])
        StopExam(playerid);

    if(Iter_Contains(displayFPS, playerid))
        Iter_Remove(displayFPS, playerid);

    savePlayerInfo(playerid);
    resetVarsOnDisconnect(playerid);
    return true;
}

function:resetVarsOnDisconnect(const playerid) {
    playerInfo[playerid] = resetPlayerEnum;
    playerStaticVar[playerid] = resetStaticEnum;
    if(drug_timer[playerid] != Timer:0)
        drug_timer[playerid] = Timer:0;

    playerStaticVar[playerid][var_animations_loaded] = playerStaticVar[playerid][var_cocaine_times] =
    playerStaticVar[playerid][var_mushrooms_times] = playerStaticVar[playerid][var_drugs_times] = 0;

    if(playerStaticVar[playerid][var_jobtimer] != Timer:0)
        stop playerStaticVar[playerid][var_jobtimer];

    playerStaticVar[playerid][var_selected_seeds] = playerStaticVar[playerid][var_drug_used] = DRUG_TYPE_NONE;

    playerStaticVar[playerid][var_last_sms_id] = playerStaticVar[playerid][var_last_sms_sqlid] = 
    playerStaticVar[playerid][var_jobseconds] = playerStaticVar[playerid][var_trucker_trailer] = playerStaticVar[playerid][var_jobobject] =
    playerStaticVar[playerid][pCheckpointPos][0] = playerStaticVar[playerid][pCheckpointPos][1] = playerStaticVar[playerid][pCheckpointPos][2] = 
    playerStaticVar[playerid][var_incall] = playerStaticVar[playerid][var_calls] = playerStaticVar[playerid][var_callfrom] = -1;


    RemovePlayerAttachedObject(playerid, INDEX_PHONE);


    if(factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_POLICE)
        Iter_Remove(departmentsMembers, playerid);

    if(Iter_Contains(Muted, playerid)) Iter_Remove(Muted, playerid);

    if(playerInfo[playerid][pPhoneNumber] != 0) 
        MAP_remove_val(phoneNumbers, playerInfo[playerid][pPhoneNumber]);

    Iter_Clear(blockedPlayers[playerid]);

    if(playerInfo[playerid][pPremiumAccount]) 
        Iter_Remove(premiumPlayers, playerid);

    return true;
}

function:savePlayerInfo(playerid) {
    if(playerStaticVar[playerid][playerLogged])
    {
        mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Hours` = '%.2f', `LastLogin` = '%e', `lastOnlineTime` = '%d', \
            `Money` = '%d', `MoneyStore` = '%d', `BankLY` = '%d', `Bank` = '%d', \
            `PhoneCredit` = '%d', `Cases` = '%d', `CaseKeys` = '%d', `Mute` = %d, `QuestionMute` = %d, `ReportMute` = %d, \
            `WeaponLicence` = '%d', `FlyLicence` = '%d', `SailingLicence` = '%d', `DrivingLicence` = '%d',\
            `PremiumPoints` = '%d', `PlayersHelped` = '%d', `Cocaine` = '%d', `Mushrooms` = '%d', `Addiction` = '%d', \
            `PayDay` = '%d', `JailTime` = '%d', `wantedTime` = '%d', `Status` = '0' where `id` = '%d';",

            playerInfo[playerid][pHours], getDateTime(), gettime(), playerInfo[playerid][pMoney], playerInfo[playerid][pMoneyStore], playerInfo[playerid][pAccountLY],
            playerInfo[playerid][pAccount], playerInfo[playerid][pPhoneCredit],
            playerInfo[playerid][pCases], playerInfo[playerid][pCasesKeys], 
            (playerInfo[playerid][pMute] > gettime()) ? (playerInfo[playerid][pMute] - gettime()) : (0), 
            (playerInfo[playerid][pQuestionMute] > gettime()) ? (playerInfo[playerid][pQuestionMute] - gettime()) : (0), 
            (playerInfo[playerid][pReportMute] > gettime()) ? (playerInfo[playerid][pReportMute] - gettime()) : (0), 
            playerInfo[playerid][pGunLic], playerInfo[playerid][pFlyLic], playerInfo[playerid][pBoatLic], playerInfo[playerid][pCarLic],
            playerInfo[playerid][pPremiumPoints], playerInfo[playerid][pPlayersHelped], 
            playerInfo[playerid][pCocaine], playerInfo[playerid][pMushrooms], playerStaticVar[playerid][var_addiction_procent], 
            playerInfo[playerid][pPayDay], playerInfo[playerid][pJailTime], playerStaticVar[playerid][wantedTime],  playerInfo[playerid][pSQLID]);
        mysql_tquery(SQL, returnString, "", "");

        mysql_format(SQL, returnString, sizeof returnString, "select * from `personalvehicles` where `Owner` = '%e' order by `id` asc;", playerInfo[playerid][pName]);
        mysql_tquery(SQL, returnString, "updatePlayerLogs", "ii", playerInfo[playerid][pSQLID], 1);
    }
    print("saved all players informations");
    return true;
}
public OnPlayerText(playerid, text[])
{

    if(!playerStaticVar[playerid][playerLogged] || !playerInfo[playerid][pTutorial])
        return false;

    if(playerInfo[playerid][pMute] > gettime()) {
        SendClientMessage(playerid, -1, "Nu poti scrie in chat deoarece ai mute pentru inca %d %s.", ((playerInfo[playerid][pMute] - gettime()) > 60) ? ((playerInfo[playerid][pMute] - gettime()) / 60) : (playerInfo[playerid][pMute] - gettime()), ((playerInfo[playerid][pMute] - gettime()) > 60) ? "minute" : "secunde");
        return false;
    }

    if(playerStaticVar[playerid][lastChatMessage] && strmatch(playerStaticVar[playerid][lastChatMessage], text))
        return false;

    if(FindIP(text)) return sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s(%d) typed %s in chat.", GetName(playerid), playerid, text);

    if(isPlayerInPaint(playerid)) {
        if(playerStaticVar[playerid][var_paintball_delay] > gettime())
            return false;

        sendPaintMessage(getPlayerPaintID(playerid), -1, "{BA3FC1}%s [%d kills]{ffffff}: %s", GetName(playerid), getPlayerPaintKills(playerid), text);
    
        playerStaticVar[playerid][var_paintball_delay] = gettime() + 2;

        return false;
    }

    if(playerStaticVar[playerid][var_incall] != -1) {
        if(playerStaticVar[playerid][var_incall_delay] > gettime())
            return false;

        
        format(returnString, sizeof returnString, "(cellphone) %s", text);
        SetPlayerChatBubble(playerid, returnString, COLOR_WHITE, 25.0, 5000);

        
        format(returnString, sizeof returnString, "(cellphone) %s (%d) says: %s", GetName(playerid), playerid, text);
        SendClientMessage(playerStaticVar[playerid][var_incall], COLOR_YELLOW, returnString); 
        SendClientMessage(playerid, COLOR_YELLOW, returnString);

        if(playerStaticVar[playerStaticVar[playerid][var_incall]][b_PHONE_SPEAKER])
            sendNearbyMessage(playerStaticVar[playerid][var_incall], COLOR_PURPLE, "(cellphone speaker) %s (%d) says to %s (%d): %s", GetName(playerid), playerid, GetName(playerStaticVar[playerid][var_incall]), playerStaticVar[playerid][var_incall], text);


        playerStaticVar[playerid][var_incall_delay] = gettime() + 3;
        return false;
    }
    format(playerStaticVar[playerid][lastChatMessage], 144, text);
    sendChatMessage(playerid, text);

    foreach(new i : Spectators)
    {
        if(Spectate[i] == playerid && i != playerid)
        {
            va_SendClientMessage(i, COLOR_LIGHTGREEN, "[CHAT]: %s: %s", GetName(Spectate[i]), text);
        }
    }

    if(playerStaticVar[playerid][var_calls] == MAX_PLAYERS + 1)
    {
        if(!strmatch(text, "police") && !emergencyCall[playerid])
        {
            SendClientMessage(playerid, -1, "(cellphone) 112: Sorry, I didn't quite understand that... speak again?");
        }
        else if(strmatch(text, "police"))
        {
            SendClientMessage(playerid, -1, "(cellphone) 112: You have reached the Police emergency hotline; can you describe the crime?");
            emergencyCall[playerid] = 1;
        }
        else if(emergencyCall[playerid])
        {
            SendClientMessage(playerid, -1, "(cellphone) 112: 112: Thank you for reporting this incident;");
            SendClientMessage(playerStaticVar[playerid][var_incall], -1, "Your call has been terminated by the other party.");

            playerStaticVar[playerid][var_calls] = INVALID_PLAYER_ID;
            playerStaticVar[playerid][b_PHONE_SPEAKER] = 0;

            sendDepartmentMessage(true, COLOR_LIGHTBLUE, "Dispatch: %s has reported: '%s'", GetName(playerid), text);
        }
    }
    return false;
}


function:SS(playerid, color, const ro[], const en[])
    return sendSplitMessage(playerid, color, playerInfo[playerid][pLanguage] ? ro : en);

function:sendChatMessage(playerid, chatString[])
{
    if(!playerStaticVar[playerid][playerLogged])
        return true;

    new Float: returnDistance[3];
    GetPlayerPos(playerid, returnDistance[0], returnDistance[1], returnDistance[2]);

    foreach(new i : streamedPlayers[playerid])
    {
        if(!playerStaticVar[i][playerLogged])
            continue;

        if(GetPlayerInterior(playerid) != GetPlayerInterior(i) || GetPlayerVirtualWorld(playerid) != GetPlayerVirtualWorld(i))
            continue;

        if(GetPlayerDistanceFromPoint(i, returnDistance[0], returnDistance[1], returnDistance[2]) > 40)
            continue;

        if(Iter_Contains(serverAdmins, playerid))
            va_SendClientMessage(i, -1, "{%s}(A) %s: {FFFFFF}%s", adminColors[playerInfo[playerid][pAdminColor]][colorValue], GetName(playerid), chatString);

        else
            va_SendClientMessage(i, -1, "{%s}%s: {FFFFFF}%s", premiumNameColors[playerInfo[playerid][pPremiumColor]][colorValue], GetName(playerid), chatString);
    }

    if(Iter_Contains(serverAdmins, playerid))
        va_SendClientMessage(playerid, -1, "{%s}(A) %s: {FFFFFF}%s", adminColors[playerInfo[playerid][pAdminColor]][colorValue], GetName(playerid), chatString);

    else
        va_SendClientMessage(playerid, -1, "{%s}%s: {FFFFFF}%s", premiumNameColors[playerInfo[playerid][pPremiumColor]][colorValue], GetName(playerid), chatString);

    new bubbleString[144];
    format(bubbleString, sizeof bubbleString, "%s", chatString);
    SetPlayerChatBubble(playerid, bubbleString, 0xFFFFFFCC, 15.0, 10000);
    return true;
}

public OnRconLoginAttempt(ip[], password[], success)
{
    sendStaffMessage(STAFF_TYPE_OWNER, COLOR_RED, "[RCON]: Login attempt %s from IP adress: %s.", (!success ? ("failed") : ("succesfully")), ip);
    return true;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
    switch(playerStaticVar[playerid][pCheckpoint])
    {
        case 100..123:
        {
            if(!IsPlayerInVehicle(playerid, playerStaticVar[playerid][examVeh]))
                return true;

            playerStaticVar[playerid][examStep] ++;
            SetExamCheckpoint(playerid, playerStaticVar[playerid][examStep]);
        }
        case 124:
        {
            SendClientMessage(playerid, -1, "Felicitari! Ai primit permisul de conducere!");
            playerInfo[playerid][pCarLic] = 100;

            StopExam(playerid);

            mysql_format(SQL, returnString, sizeof returnString, "update `users` set `DrivingLicence` = '100' where `id` = '%d';", playerInfo[playerid][pSQLID]);
            mysql_tquery(SQL, returnString, "", "");
        }
    }
    return true;
}

StopExam(playerid)
{
    for(new i; i <= 2; i++)
        TextDrawHideForPlayer(playerid, dmvTD[i]);

    PlayerTextDrawHide(playerid, playerTextdraws[playerid][dmvPTD]);

    DisablePlayerRaceCheckpoint(playerid);
    playerStaticVar[playerid][pCheckpoint] = 0;

    DisableRemoteVehicleCollisions(playerid, false);

    DestroyVehicle(playerStaticVar[playerid][examVeh]);
    playerStaticVar[playerid][examVeh] = -1;
    playerStaticVar[playerid][examStep] = 0;
    return true;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
    if(playerStaticVar[playerid][examStep])
    {
        SendClientMessage(playerid, -1, "Ai picat examenul pentru ca ai coborat din masina.");
        StopExam(playerid);
    }
    return true;
}

public OnPlayerEnterCheckpoint(playerid)
{
    playerStaticVar[playerid][pCheckpointPos][0] = -1; playerStaticVar[playerid][pCheckpointPos][1] = -1; playerStaticVar[playerid][pCheckpointPos][2] = -1;

    switch(playerStaticVar[playerid][pCheckpoint])
    {
        case 1:
        {
            SendClientMessage(playerid, -1, "You have reached your checkpoint.");
            playerStaticVar[playerid][pCheckpoint] = 0;

            PlayerPlaySound(playerid, 1139, 0.0, 0.0, 0.0);
            DisablePlayerCheckpoint(playerid);
        }
    }
    return true;
}


timer use_vending_machine[2300](playerid)
{
    ClearAnimations(playerid);
    ApplyAnimation(playerid, "VENDING", "VEND_Drink_P", 4.0, 0, 0, 0, 0, 0, 0);

    SetPlayerHealthEx(playerid, (player_data[playerid][playerHealth] >= 75.0 ? 100.0 : player_data[playerid][playerHealth] + 25.0));
    playerStaticVar[playerid][pVending] = 0;

    vending_timer[playerid] = Timer:0;
    return true;
}

public OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if(newkeys == KEY_SPRINT)
    {
       if(gPlayerUsingLoopingAnim[playerid] == 1)
           StopLoopingAnim(playerid);
    }
    if(newkeys == KEY_SECONDARY_ATTACK)
    {
        for(new i; i < MAX_ATMS; i++)
        {
            if(!IsPlayerInRangeOfPoint(playerid, 2.0, AtmInfo[i][fATMPos][0], AtmInfo[i][fATMPos][1], AtmInfo[i][fATMPos][2]))
                continue;

            if(AtmInfo[i][ATMHealth] < 1)
                return sendError(playerid, "Acest bancomat este stricat! Asteapta cateva secunde!");

            Dialog_Show(playerid, DIALOG_ATM_MENU, DIALOG_STYLE_LIST, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Automated Teller Machine", "Check Balance\nWithdraw", "OK", "Cancel");
            break;
        }

        if(IsPlayerInRangeOfPoint(playerid, 1.0, 1552.5898, -1683.4698, 13.5504) && !playerStaticVar[playerid][pVending]) //lspd
        {

            if(GetMoney(playerid) < 200 && GetMilliards(playerid) < 1)
                return sendError(playerid, "You don't have enough money.");
               

            SetPlayerFacingAngle(playerid, 272.63);

            ApplyAnimation(playerid, "VENDING", "VEND_USE", 4.0, 0, 0, 0, 0, 0, 0);
            PlayerPlaySound(playerid, 42600, 0.0, 0.0, 0.0);

            playerStaticVar[playerid][pVending] = 1;
            vending_timer[playerid] = defer use_vending_machine(playerid);

            sendNearbyMessage(playerid, COLOR_PURPLE, "* %s bought a drink from vending machine.", GetDBName(playerid));
        }
        else if(IsPlayerInRangeOfPoint(playerid, 1.0, 2273.5654, 2428.7834, 10.8203) && !playerStaticVar[playerid][pVending]) //lvpd
        {
            if(GetMoney(playerid) < 200 && GetMilliards(playerid) < 1)
                return sendError(playerid, "You don't have enough money.");
               
            SetPlayerFacingAngle(playerid, 3.20);

            ApplyAnimation(playerid, "VENDING", "VEND_USE", 4.0, 0, 0, 0, 0, 0, 0);
            PlayerPlaySound(playerid, 42600, 0.0, 0.0, 0.0);

            playerStaticVar[playerid][pVending] = 1;
            vending_timer[playerid] = defer use_vending_machine(playerid);

            sendNearbyMessage(playerid, COLOR_PURPLE, "* %s bought a drink from vending machine.", GetDBName(playerid));
        }

    }
    return true;
}

public OnPlayerShootDynamicObject(playerid, weaponid, objectid, Float:x, Float:y, Float:z)
{
    for(new i; i < MAX_ATMS; i++)
    {
        if(objectid == AtmInfo[i][rObjectId][0])
        {
            if(AtmInfo[i][ATMHealth] < 1 || playerInfo[playerid][pGroup] == 1 || playerInfo[playerid][pGroup] == 2 || playerInfo[playerid][pGroup] == 3 || playerInfo[playerid][pGroup] == 8)
                break;

            if(playerInfo[playerid][pWantedLevel] > 4)
                return va_SendClientMessage(playerid, -1, "Nu poti jefui un bancomat daca ai wanted mai mare de 4.");

            new dmgAmount = RandomEx(5, 10);
            AtmInfo[i][ATMHealth] -= dmgAmount;

            if(AtmInfo[i][ATMHealth] < 1)
            {
                new atmMoney = RandomEx(50000, 60000);

                va_SendClientMessage(playerid, -1, "Ai primit $%s pentru distrugerea acestui bancomat.", FormatNumber(atmMoney));
                addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, atmMoney, false);


                GivePlayerWanted(playerid, INVALID_PLAYER_ID, 2, "ATM robbery*");

                DestroyDynamicObject(AtmInfo[i][rObjectId][0]);
                AtmInfo[i][rObjectId][0] = CreateDynamicObject(2943, AtmInfo[i][fATMPos][0], AtmInfo[i][fATMPos][1], AtmInfo[i][fATMPos][2], AtmInfo[i][fATMPosRot][0], AtmInfo[i][fATMPosRot][1], AtmInfo[i][fATMPosRot][2], -1, -1, -1, 500.0);
                AtmInfo[i][ATMRepairTime] = gettime() + 65;

                Iter_Add(destroyedATM, i);
                Streamer_Update(playerid);

                DailyQuestCheck(playerid, QUEST_TYPE_ATMROB, 1);
            }
            UpdateATMLabel(i);
            break;
        }
    }
    return true;
}

function:resetServerVars(playerid)
{
    emergencyCall[playerid] = 0;
    lastSentMessage[playerid] = 0;

    WantName[playerid] = "NULL";
}

public OnPlayerConnect(playerid)
{
    new returnHour2, returnMinute2;
    gettime(returnHour2, returnMinute2);
    SetPlayerTime(playerid, returnHour2, returnMinute2);

    playerInfo[playerid] = resetPlayerEnum;
    playerStaticVar[playerid] = resetStaticEnum;
    g_MysqlRaceCheck[playerid] ++;


    //baloti dealer drugs

    RemoveBuildingForPlayer(playerid, 1454, -1089.2031, -1608.7344, 76.1484, 0.25);
    RemoveBuildingForPlayer(playerid, 1454, -1093.1563, -1608.7344, 76.1484, 0.25);
    RemoveBuildingForPlayer(playerid, 1454, -1097.1094, -1608.7344, 76.1484, 0.25);
    RemoveBuildingForPlayer(playerid, 1454, -1101.0625, -1608.7344, 76.1484, 0.25);
    RemoveBuildingForPlayer(playerid, 1454, -1085.2500, -1608.7344, 76.1484, 0.25);
    RemoveBuildingForPlayer(playerid, 1454, -1078.3125, -1608.7344, 76.1484, 0.25);
    RemoveBuildingForPlayer(playerid, 1454, -1074.3594, -1608.7344, 76.1484, 0.25);
    RemoveBuildingForPlayer(playerid, 1454, -1070.4063, -1608.7344, 76.1484, 0.25);
    RemoveBuildingForPlayer(playerid, 1454, -1066.4531, -1608.7344, 76.1484, 0.25);
    RemoveBuildingForPlayer(playerid, 1454, -1062.5000, -1608.7344, 76.1484, 0.25);

    //fish shop ls
    RemoveBuildingForPlayer(playerid, 1340, 388.8594, -2071.6641, 7.9453, 0.25);
    resetServerVars(playerid);

    foreach(new i : All(clanMembers))
    {
        if(strmatch(GetName(playerid), playerInfo[i][pName]))
        {
            KickEx(playerid);
            break;
        }
    }

    LoadPlayerTextdraws(playerid);
    removeMaps(playerid);


    SetPlayerColor(playerid, COLOR_GREY);
    TogglePlayerSpectating(playerid, true);
    RegisterCamera(playerid);
    ShowPlayerLogo(playerid);

    Iter_Add(playersInQueue, playerid);

    if(Iter_Count(playersInQueue) >= MAX_PLAYERS_ON_LOGIN && timerLoginQueue == Timer:0)
        timerLoginQueue = repeat loginPlayerFromQueue();

    if(timerLoginQueue != Timer:0)
    {
        format(returnString, sizeof returnString, "retrieving login queue informations");
        TextDrawSetString(loginQueue[3], returnString);
        TextDrawShowForPlayer(playerid, loginQueue[0]);
        TextDrawShowForPlayer(playerid, loginQueue[1]);
        TextDrawShowForPlayer(playerid, loginQueue[2]);
        TextDrawShowForPlayer(playerid, loginQueue[3]);
        SendClientMessage(playerid, COLOR_RED, "Please wait in login queue.. you'll be logged in soon. Don't quit the game.");
    }
    else
    {
        mysql_format(SQL, returnString, sizeof returnString, "select * from `bans` where `name` = '%s' limit 1;", GetName(playerid));
        mysql_tquery(SQL, returnString, "BanCheck", "id", playerid, g_MysqlRaceCheck[playerid]);
    }
    
    PreloadAnimLib(playerid, "BOMBER");
    PreloadAnimLib(playerid, "RAPPING");
    PreloadAnimLib(playerid, "SHOP");
    PreloadAnimLib(playerid, "BEACH");
    PreloadAnimLib(playerid, "SMOKING");
    PreloadAnimLib(playerid, "FOOD");
    PreloadAnimLib(playerid, "ON_LOOKERS");
    PreloadAnimLib(playerid, "DEALER");
    PreloadAnimLib(playerid, "MISC");
    PreloadAnimLib(playerid, "SAMP");
    PreloadAnimLib(playerid, "SWEET");
    PreloadAnimLib(playerid, "RIOT");
    PreloadAnimLib(playerid, "PED");
    PreloadAnimLib(playerid, "POLICE");
    PreloadAnimLib(playerid, "CRACK");
    PreloadAnimLib(playerid, "CARRY");
    PreloadAnimLib(playerid, "COP_AMBIENT");
    PreloadAnimLib(playerid, "PARK");
    PreloadAnimLib(playerid, "INT_HOUSE");
    PreloadAnimLib(playerid, "FOOD");
    PreloadAnimLib(playerid, "SWORD");
    PreloadAnimLib(playerid, "BASEBALL");

    resetVarsOnConn(playerid);
    return true;
}

resetVarsOnConn(const playerid) {

    playerStaticVar[playerid][var_premium_tog] = playerStaticVar[playerid][b_PHONE_TURN] = playerStaticVar[playerid][b_calls_turn] = true;

    playerStaticVar[playerid][vehiclekey_id] = 
    playerStaticVar[playerid][vehiclekey_sqlid] = playerStaticVar[playerid][var_incall] = playerStaticVar[playerid][var_calls] = 
    playerStaticVar[playerid][var_callfrom] = playerStaticVar[playerid][var_last_sms_id] = playerStaticVar[playerid][var_last_sms_sqlid] = 
    playerStaticVar[playerid][var_call_delay] = playerStaticVar[playerid][var_sms_delay] = -1;

    __scy_ab_POS_Z[playerid] = __scy_ab_POS_Y[playerid] = __scy_ab_POS_X[playerid] = 
    playerInfo[playerid][pLastPosX] = playerInfo[playerid][pLastPosY] = playerInfo[playerid][pLastPosZ] = 0.0;

    playerInfo[playerid][pPremiumColor] = playerStaticVar[playerid][var_animations_loaded] =
    playerStaticVar[playerid][var_cocaine_times] = playerStaticVar[playerid][var_mushrooms_times] = playerStaticVar[playerid][var_drugs_times] =
    playerStaticVar[playerid][wantedTime] = playerInfo[playerid][pJailTime] = entercars[playerid] = __scy_ab__LAST_CALL[playerid] = 0;
   
    DeletePlayer3DTextLabel(playerid,damageLabelID);

    playerStaticVar[playerid][var_selected_seeds] = playerStaticVar[playerid][var_drug_used] = DRUG_TYPE_NONE;
    if(playerStaticVar[playerid][var_wanted_timer] != Timer:0)
        playerStaticVar[playerid][var_wanted_timer] = Timer:0;
}

UpdateWeather()
{
    switch(random(17))
    {
        case 0: SendRconCommand("weather 0");
        case 1: SendRconCommand("weather 1");
        case 2: SendRconCommand("weather 2");
        case 3: SendRconCommand("weather 3");
        case 4: SendRconCommand("weather 4");
        case 5: SendRconCommand("weather 5");
        case 6: SendRconCommand("weather 6");
        case 7: SendRconCommand("weather 7");
        case 8: SendRconCommand("weather 10");
        case 9: SendRconCommand("weather 11");
        case 10: SendRconCommand("weather 12");
        case 11: SendRconCommand("weather 13");
        case 12: SendRconCommand("weather 14");
        case 13: SendRconCommand("weather 15");
        case 14: SendRconCommand("weather 17");
        case 15: SendRconCommand("weather 18");
        case 16: SendRconCommand("weather 20");
    }
    gettime(serverTime[0], serverTime[1], serverTime[2]);
    SetWorldTime(serverTime[0]);
    return true;
}

#if INITIALISE_GAMEMODE_DELAY
timer loadServer[20000]() {
    SendRconCommand("name "SERVER_HOSTNAME""); 
    SendRconCommand("password "SERVER_PASSWORD""); 
    SetGameModeText(""SERVER_NAME2" v0.3.7, "SERVER_VERSION"");
    return 1;
}
#endif


public OnGameModeInit()
{
    #if SERVER_MYSQL
        SQL = mysql_connect("188.212.100.198", "client894_samp", "ER1QVpSeXfUEYSzH", "client894_samp");
    #else
        SQL = mysql_connect("localhost", "root", "", "test");
    #endif
    
    if(mysql_errno()) 
        return printf("failed connecting to mysql server... response: %d. (gamemode initialisation stopped)", mysql_errno());

    AntiDeAMX();
   
    AddPlayerClass(0, 0, 0, 0, 1.0, WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FIST, 0);
    ShowNameTags(true);
    SetNameTagDrawDistance(40.0);
    EnableStuntBonusForAll(false);
    DisableInteriorEnterExits();
    AllowInteriorWeapons(true);
    UsePlayerPedAnims();
    ShowPlayerMarkers(PLAYER_MARKERS_MODE_STREAMED);

    gettime(serverTime[0], serverTime[1], serverTime[2]), getdate(serverTime[3], serverTime[4], serverTime[5]);
    SetWorldTime(serverTime[0]);

    SendRconCommand("game.map LS"); SendRconCommand("language RO/EN");

    AddServerRule("allowed_clients", "0.3.7, 0.3 DL");

    SendRconCommand("website "SERVER_WEBURL"");
    SendRconCommand("language "SERVER_LANGUAGE"");

    UpdateWeather();
    format(returnString, sizeof returnString, ""SERVER_NAME2", %02d %s %d %02d:%02d:%02d", serverTime[5], monthName[serverTime[4]-1], serverTime[3], serverTime[0], serverTime[1], serverTime[2]);
    SetGameModeText(returnString);

    CallLocalFunction("loadServerSystems", "");
    CallLocalFunction("LoadServerAssests", "");
    CallLocalFunction("loadTextDrawsFromInclude", "");

    Command_SetDeniedReturn(true);

    Command_AddAltNamed("emails", "email");
    Command_AddAltNamed("gps", "where");
    Command_AddAltNamed("gps", "locations");
    Command_AddAltNamed("skill", "skills");
    Command_AddAltNamed("forcenamechange", "fnc");
    Command_AddAltNamed("romana", "ro");
    Command_AddAltNamed("english", "en");
    Command_AddAltNamed("setfreq", "setfrequency");
    Command_AddAltNamed("reply", "re");
    Command_AddAltNamed("stopanim", "stop");

    Command_AddAltNamed("pickup", "p");
    Command_AddAltNamed("hangup", "h");

    Command_AddAltNamed("acceptname", "an");
    Command_AddAltNamed("cancelname", "cn");


    Command_AddAltNamed("carwhisper", "cw");
    Command_AddAltNamed("whisper", "w");

    //pompe gas station vinewood
    CreateDynamicObject(1676, 1009.18927, -936.09637, 42.78190,   0.00000, 0.00000, 6.66000);
    CreateDynamicObject(1676, 1005.57520, -936.67499, 42.78190,   0.00000, 0.00000, 6.66000);
    CreateDynamicObject(1676, 1002.23541, -937.06488, 42.78190,   0.00000, 0.00000, 6.66000);
    CreateDynamicObject(1676, 998.64508, -937.57654, 42.78190,   0.00000, 0.00000, 6.66000);

    //gard cnn
    CreateDynamicObject(970, 1113.81226, -1415.95642, 12.99760,   0.00000, 0.00000, 1.02000);
    CreateDynamicObject(970, 1118.95190, -1415.88806, 12.99760,   0.00000, 0.00000, 0.24000);
    CreateDynamicObject(970, 1123.87134, -1415.89172, 13.01760,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(970, 1128.84937, -1415.87390, 13.05760,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(970, 1133.62927, -1415.89539, 13.05760,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(970, 1138.54712, -1415.90955, 13.07760,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(970, 1143.48718, -1415.95984, 13.07760,   0.00000, 0.00000, -0.54000);
    CreateDynamicObject(970, 1186.10474, -1495.67102, 12.96110,   0.00000, 0.00000, -90.06000);
    CreateDynamicObject(970, 1186.14282, -1490.92114, 12.96110,   0.00000, 0.00000, -90.06000);
    CreateDynamicObject(970, 1186.13611, -1486.25720, 12.96110,   0.00000, 0.00000, -90.06000);
    CreateDynamicObject(970, 1186.10059, -1481.48560, 12.96110,   0.00000, 0.00000, -90.06000);
    CreateDynamicObject(970, 1124.00598, -1562.16187, 12.95910,   0.00000, 0.00000, -0.48000);
    CreateDynamicObject(970, 1133.22400, -1562.16504, 12.95910,   0.00000, 0.00000, 0.12000);
    CreateDynamicObject(970, 1128.58142, -1562.17664, 12.95910,   0.00000, 0.00000, -0.48000);
    CreateDynamicObject(970, 1061.49329, -1492.59277, 12.96960,   0.00000, 0.00000, -105.78001);
    CreateDynamicObject(970, 1059.12000, -1501.51147, 12.96960,   0.00000, 0.00000, -104.70000);
    CreateDynamicObject(970, 1060.26794, -1497.01086, 12.98960,   0.00000, 0.00000, -105.30000);

    CreateDynamic3DTextLabel("{FFFF6B}Driving School!\n \nType /exam\nto start the test!", -1, 1219.0502, -1812.7823, 16.5938, 20.0);
    CreateDynamicPickup(1239, 23, 219.0502, -1812.7823, 16.5938);

    // //fish shop ls
    // CreateDynamicObject(3861, 396.03409, -2075.40210, 7.98513,   0.00000, 0.00000, -90.35999);
    // CreateDynamicObject(1641, 397.38278, -2075.37500, 7.80438,   -269.69974, 269.03922, 0.48000);
    // CreateDynamicObject(2390, 396.02597, -2076.21973, 8.56258,   0.00000, 0.00000, -80.52000);
    // CreateDynamicObject(2500, 395.06543, -2074.54858, 7.61041,   0.00000, 0.00000, -91.91998);
    // CreateDynamicObject(19137, 395.17474, -2076.44141, 7.78649,   -715.25903, -79.68024, 83.58002);
    // CreateDynamicObject(19136, 394.96750, -2074.45044, 8.31165,   -1.56000, -91.92003, -17.52000);
    // CreateDynamicObject(19169, 397.34592, -2074.61499, 7.88252,   -0.48000, -85.98003, 0.18000);
    // CreateDynamicObject(18655, 395.46335, -2072.96338, 6.63395,   0.00000, 0.00000, 57.59998);
    // CreateDynamicObject(18637, 394.59042, -2076.80957, 7.33727,   64.31998, -39.24004, -71.04000);
    // CreateDynamicObject(11704, 395.47729, -2076.20972, 7.82396,   -79.31998, 51.06003, -83.39999);
    
    // spawn civil
    CreateObject(8034, 1564.34900, -2157.05029, 17.15990,   0.00000, 0.00000, -180.00000);
    CreateObject(8661, 1551.05127, -2174.82202, 12.58390,   0.00000, 0.00000, 0.00000);
    CreateObject(8661, 1576.79700, -2174.80200, 12.56390,   0.00000, 0.00000, 0.00000);
    CreateObject(8661, 1559.16785, -2154.82080, 12.56390,   0.00000, 0.00000, 0.00000);
    CreateObject(984, 1531.05945, -2162.14990, 13.19990,   0.00000, 0.00000, 0.00000);
    CreateObject(984, 1531.05945, -2149.36719, 13.19990,   0.00000, 0.00000, 0.00000);
    CreateObject(621, 1556.83203, -2165.50000, 12.57250,   0.00000, 0.00000, 0.00000);
    CreateObject(621, 1571.54724, -2165.50000, 12.57250,   0.00000, 0.00000, 0.00000);
    CreateObject(10281, 1564.39392, -2167.85742, 20.23210,   0.00000, 0.00000, 0.00000);
    CreateObject(16061, 1505.07898, -2182.36230, 12.52930,   0.00000, 0.00000, 90.00000);
    CreateObject(16061, 1630.00830, -2179.09912, 12.52930,   0.00000, 0.00000, 90.00000);
    CreateObject(984, 1596.80286, -2175.24854, 13.17990,   0.00000, 0.00000, 0.00000);
    CreateObject(984, 1596.80286, -2162.44189, 13.17990,   0.00000, 0.00000, 0.00000);
    CreateObject(984, 1596.80286, -2149.62354, 13.17990,   0.00000, 0.00000, 0.00000);
    CreateObject(984, 1596.80286, -2143.22559, 13.17990,   0.00000, 0.00000, 0.00000);
    CreateObject(640, 1595.58948, -2161.95020, 13.18990,   0.00000, 0.00000, 0.00000);
    CreateObject(640, 1595.58948, -2156.22632, 13.18990,   0.00000, 0.00000, 0.00000);
    CreateObject(640, 1595.58948, -2150.52441, 13.18990,   0.00000, 0.00000, 0.00000);
    CreateObject(640, 1595.58948, -2144.84326, 13.18990,   0.00000, 0.00000, 0.00000);
    CreateObject(640, 1532.79077, -2162.01880, 13.22500,   0.00000, 0.00000, 0.00000);
    CreateObject(640, 1532.76233, -2156.50098, 13.18550,   0.00000, 0.00000, 0.00000);
    CreateObject(640, 1532.72693, -2150.90430, 13.46300,   0.00000, 0.00000, 0.00000);
    CreateObject(640, 1532.72351, -2145.31323, 13.79990,   0.00000, 0.00000, 0.00000);

    mysql_tquery(SQL, "UPDATE `users` SET `Status` = 0 WHERE `Status` = 1");

    #if INITIALISE_GAMEMODE_DELAY
        SetGameModeText("loading...");
        SendRconCommand("name "SERVER_HOSTNAME" initialising...");
        SendRconCommand("password nidsandiasidbsoadhw821838SAgsyagsySBuiuSYYYSioiii@!3");
        defer loadServer();
    #else
        SendRconCommand("name "SERVER_HOSTNAME"");
        //SendRconCommand("password "SERVER_PASSWORD""); 
        SetGameModeText(SERVER_VERSION);
    #endif


//banca


    return true;
}

static Float:examCP[][] =
{
    {1135.3868, -1850.5981, 13.0366},
    {1045.2626, -1809.4169, 13.2637},
    {849.1470, -1767.0336, 13.0397},
    {635.5740, -1712.8594, 13.8757},
    {605.3907, -1672.5651, 15.5764},
    {485.4009, -1658.0165, 21.8713},
    {325.8313, -1640.2993, 32.7597},
    {246.6969, -1561.6211, 32.6957},
    {274.5565, -1478.2930, 29.9500},
    {193.8137, -1506.6576, 12.2538},
    {324.4604, -1400.7098, 13.7601},
    {509.5384, -1282.4365, 15.4881},
    {599.4775, -1228.8478, 17.5563},
    {623.2209, -1238.7283, 17.5613},
    {627.9891, -1319.5573, 13.2599},
    {716.7220, -1322.1327, 13.0510},
    {864.1711, -1327.4478, 13.1957},
    {1046.0955, -1328.6143, 13.0441},
    {1054.7998, -1383.8990, 13.1580},
    {1055.3480, -1464.9629, 13.0095},
    {1035.1870, -1557.0361, 13.0122},
    {1084.2502, -1574.0663, 13.0356},
    {1295.7023, -1582.3715, 13.0348},
    {1295.5782, -1832.5750, 13.3828},
    {0.0, 0.0, 0.0}
};

YCMD:exam(playerid, params[], help)
{
    if(IsPlayerInAnyVehicle(playerid))
        return sendError(playerid, "You can't use this command when you are in a car.");

    if(!IsPlayerInRangeOfPoint(playerid, 5.0, 1219.0502, -1812.7823, 16.5938))
        return sendError(playerid, "You are not near DMV.");

    if(playerInfo[playerid][pCarLic])
        return sendError(playerid, "You already have a drivers license.");

    SendClientMessage(playerid, -1, "Examenul a inceput. Mergi la checkpoint.");
    SendClientMessage(playerid, -1, "Scrie /engine sau apasa tasta 2 pentru a porni motorul masinii.");

    for(new i; i <= 2; i++) TextDrawShowForPlayer(playerid, dmvTD[i]);

    DisableRemoteVehicleCollisions(playerid, true);

    playerStaticVar[playerid][examVeh] = AddStaticVehicle(410, 1242.0013, -1850.0372, 13.0384, 90.7065, -1, -1);
    PutPlayerInVehicle(playerid, playerStaticVar[playerid][examVeh], 0);
    playerStaticVar[playerid][examStep] = 1;
    SetExamCheckpoint(playerid, playerStaticVar[playerid][examStep]);
    PlayerTextDrawShow(playerid, playerTextdraws[playerid][dmvPTD]);
    return true;
}

SetExamCheckpoint(playerid, i)
{
    va_PlayerTextDrawSetString(playerid, playerTextdraws[playerid][dmvPTD], "~w~CHECKPOINTS: ~r~%d~w~/~r~24", i - 1);
    PlayerTextDrawShow(playerid, playerTextdraws[playerid][dmvPTD]);

    SetPlayerRaceCheckpoint(playerid, (i == 24 ? CP_TYPE_GROUND_NORMAL : CP_TYPE_GROUND_FINISH), examCP[i - 1][0], examCP[i - 1][1], examCP[i - 1][2], examCP[i][0], examCP[i][1], examCP[i][2], 4.0);
    return playerStaticVar[playerid][pCheckpoint] = (100 + i);
}

function:SetPlayerPositionIV(playerid, Float:x, Float:y, Float:z, interior, virtual)
{
    SetPlayerPos(playerid, x, y, z);
    SetPlayerVirtualWorld(playerid, virtual);
    SetPlayerInterior(playerid, interior);
    return 1;
}

YCMD:speaker(playerid, params[], help) {
    if(playerStaticVar[playerid][var_incall] == -1)
        return sendError(playerid, "Nu esti intr-un apel.");

    playerStaticVar[playerid][b_PHONE_SPEAKER] = !playerStaticVar[playerid][b_PHONE_SPEAKER]; 
    return SendClientMessage(playerid, COLOR_GREY, playerStaticVar[playerid][b_PHONE_SPEAKER] ? "* You turned on your speaker." : "* You turned off your speaker.");
}

YCMD:hangup(playerid, params[], help) {
    if(playerStaticVar[playerid][var_incall] == -1)
        return sendError(playerid, "Nu esti intr-un apel.");

    new const returnPlayer = playerStaticVar[playerid][var_incall];

    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_STOPUSECELLPHONE); 
    SetPlayerSpecialAction(returnPlayer, SPECIAL_ACTION_STOPUSECELLPHONE);
    
    RemovePlayerAttachedObject(playerid, INDEX_PHONE); 
    RemovePlayerAttachedObject(returnPlayer, INDEX_PHONE);

    playerStaticVar[playerid][b_PHONE_SPEAKER] = playerStaticVar[returnPlayer][b_PHONE_SPEAKER] = false;
    playerStaticVar[playerid][var_incall] = playerStaticVar[returnPlayer][var_incall] = playerStaticVar[returnPlayer][var_calls] = playerStaticVar[playerid][var_calls] = playerStaticVar[playerid][var_callfrom] = playerStaticVar[returnPlayer][var_callfrom] = -1;
    
    SendClientMessage(returnPlayer, -1, "The other person ended the call.");
    SendClientMessage(playerid, -1, "You ended the call.");
    return true;
}


YCMD:pickup(playerid, params[], help) {
    if(playerStaticVar[playerid][var_callfrom] == -1)
        return sendError(playerid, "Nu ai primit niciun apel.");

    if(playerStaticVar[playerid][var_incall] != -1)
        return sendError(playerid, "Esti deja intr-un apel.");

    if(playerStaticVar[playerStaticVar[playerid][var_callfrom]][var_incall] != -1)
        return sendError(playerid, "Acel player este intr-un apel.");

    new const returnPlayer = playerStaticVar[playerid][var_callfrom];

    SetPlayerAttachedObject(playerid, INDEX_PHONE, 18874, 6, 0.086043, 0.027719, 0.003817, 95.232246, 178.651031, 1.691840, 1.002077, 1.000000, 1.000000);
    SetPlayerAttachedObject(returnPlayer, INDEX_PHONE, 18874, 6, 0.086043, 0.027719, 0.003817, 95.232246, 178.651031, 1.691840, 1.002077, 1.000000, 1.000000); 
    
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE); 
    SetPlayerSpecialAction(returnPlayer, SPECIAL_ACTION_USECELLPHONE);

    playerStaticVar[playerid][var_incall] = returnPlayer;
    playerStaticVar[returnPlayer][var_incall] = playerid; 
    playerStaticVar[playerid][var_calls] = playerStaticVar[playerid][var_callfrom] = playerStaticVar[returnPlayer][var_callfrom] = playerStaticVar[playerid][var_calls] = -1;

    va_SendClientMessage(returnPlayer, COLOR_YELLOW, "* %s answered to your call.", GetName(playerid));
    va_SendClientMessage(playerid, COLOR_YELLOW, "* You answered to %s call.", GetName(returnPlayer));
    return true;
}

YCMD:call(playerid, params[], help) {

    new returnPhone;

    if(sscanf(params, "i", returnPhone))
        return sendSyntaxMessage(playerid, "/call <Number>");

    if(playerInfo[playerid][pPhoneNumber] == 0)
        return sendError(playerid, "Nu ai un telefon.");

    if(!playerStaticVar[playerid][b_PHONE_TURN])
        return sendError(playerid, "Nu ai telefonul pornit.");

    if(playerInfo[playerid][pPhoneCredit] < 5)
        return sendError(playerid, "Nu ai destul credit.");


    if(playerStaticVar[playerid][var_call_delay] > gettime())
        return sendError(playerid, "Trebuie sa astepti %d secunde inainte de a face asta.", playerStaticVar[playerid][var_call_delay] - gettime());

    if(playerStaticVar[playerid][var_incall] != -1)
        return sendError(playerid, "Esti deja intr-un apel.");

    if(!isnumeric(params)) 
        return sendSyntaxMessage(playerid, "/call <number>");

    if(returnPhone == 112)
    {
        if(playerStaticVar[playerid][playerKillTime] > gettime())
        {
            new wantedPlayer = playerStaticVar[playerid][playerKillId];
            va_SendClientMessage(playerid, COLOR_LIGHTBLUE, "You reported %s for a crime.", GetName(wantedPlayer));
            GivePlayerWanted(wantedPlayer, playerid, 1, "first degree murder");
        }
        else
        {
            if(GetPVarInt(playerid, "emergency_deelay") > gettime())
                return va_SendClientMessage(playerid, COLOR_GREY, "You can't call emergency services for %d seconds.", GetPVarInt(playerid, "emergency_deelay") - gettime());

            playerStaticVar[playerid][var_calls] = MAX_PLAYERS + 1;
            SendClientMessage(playerid, -1, "You've called Emergency services, please select the department you desire (i.e: police)");

            SetPVarInt(playerid, "emergency_deelay", gettime() + 60);
        }
        return true;
    }

    new const returnPlayer = MAP_contains_val(phoneNumbers, strval(params)) ? MAP_get_val_val(phoneNumbers, strval(params)) : -1;

    if(strlen(params) > 6 || returnPlayer == -1)
        return sendError(playerid, "Acel numar de telefon este invalid.");

    if(returnPlayer == playerid)
        return sendError(playerid, "Nu poti folosi aceasta comanda asupra ta.");

    if(Iter_Contains(blockedPlayers[returnPlayer], playerid))
        return sendError(playerid, "Nu poti contacta acest numar.");

    if(playerStaticVar[returnPlayer][b_calls_turn])
        return sendError(playerid, "Acel player permite doar primirea de sms-uri.");

    if(playerStaticVar[returnPlayer][var_incall] != -1)
        return sendError(playerid, "Acel player este intr-un apel.");

    if(!playerStaticVar[returnPlayer][b_PHONE_TURN])
        return sendError(playerid, "Acel player are telefonul oprit.");

    playerStaticVar[playerid][var_calls] = returnPlayer; 
    playerStaticVar[returnPlayer][var_callfrom] = playerid;
    playerStaticVar[playerid][var_call_delay] = gettime() + 15; 
    playerInfo[playerid][pPhoneCredit] -= 5;

    va_mysql_tquery("UPDATE `users` SET `PhoneCredit` = %d WHERE `ID` = %d", playerInfo[playerid][pPhoneCredit], playerInfo[playerid][pSQLID]);

    sendNearbyMessage(playerid, COLOR_PURPLE, "* %s took the phone from pocket and dial a number.", GetName(playerid));
    sendNearbyMessage(returnPlayer, COLOR_PURPLE, "* %s's phone is ringing...", GetName(returnPlayer));

    va_SendClientMessage(returnPlayer, COLOR_YELLOW, "* %s (%d) is calling you, write [/p] to answer.", GetName(playerid), playerid);
    
    PlayerPlaySound(returnPlayer, 3600, 0.0, 0.0, 0.0);
    return true;
}

YCMD:block(playerid, params[], help) {
    if(!playerInfo[playerid][pPhone])
        return sendError(playerid, "You don't have an iPhone.");

    new returnPlayer; 
    if(sscanf(params, "u", returnPlayer))
        return sendSyntaxMessage(playerid, "/block [playerid/name]");

    if(!playerStaticVar[returnPlayer][playerLogged]) 
        return sendError(playerid, "Acest player nu este logat.");

    if(Iter_Add(blockedPlayers[playerid], returnPlayer) == INVALID_ITERATOR_SLOT)
        return sendError(playerid, "Il ai deja blocat pe %s.", GetName(returnPlayer));

    SendClientMessage(playerid, -1, "User %s has been temporary blocked. He won't be able to call/sms you until you reconnect to the server.", GetName(returnPlayer));
                    
    return true;
}

YCMD:unblock(playerid, params[], help) {
    if(!playerInfo[playerid][pPhone])
        return sendError(playerid, "You don't have an iPhone.");

    new returnPlayer; if(sscanf(params, "u", returnPlayer))
        return sendSyntaxMessage(playerid, "/unblock [playerid/name]");

    if(!playerStaticVar[returnPlayer][playerLogged])
        return sendError(playerid, "Acest player nu este logat.");

    if(Iter_Remove(blockedPlayers[playerid], returnPlayer) == INVALID_ITERATOR_SLOT)
        return sendError(playerid, "Nu il ai blocat pe %s.", GetName(returnPlayer));
            
    SendClientMessage(playerid, -1, "User %s has been unblocked.", GetName(returnPlayer));        
    return true;
}

YCMD:restart(playerid, params[], help) {   
    check_owner

    new time;
    if(sscanf(params, "is[80]", time, restartReason)) 
        return sendSyntaxMessage(playerid, "/restart <minutes> <reason>");

    iGMXTick = time * 60;
    iGMXTimer = SetTimer("restartTimer", 1000, true);
    foreach(new x : Player) { 
        savePlayerInfo(x);
    }
    SendClientMessageToAll(COLOR_CLIENT, "(( AdmBot: Serverul se va restarta in %d minute, reason: %s. ))", time, restartReason);
    return 1; 
}

YCMD:reply(playerid, params[], help) {
    if(playerStaticVar[playerid][var_sms_delay] > gettime())
        return 1;

    if(playerInfo[playerid][pMute] > gettime()) 
        return sendError(playerid, "Nu poti scrie in chat deoarece ai mute pentru inca %s minute.", calculateTime(playerInfo[playerid][pMute] - gettime()));

    if(!playerInfo[playerid][pPhone])
        return sendError(playerid, "Nu ai un iPhone.");

    if(!playerStaticVar[playerid][b_PHONE_TURN])
        return sendError(playerid, "Nu ai telefonul pornit.");

    if(playerInfo[playerid][pPhoneCredit] < 1)
        return sendError(playerid, "Nu ai credit disponibil.");

    if(playerStaticVar[playerid][var_incall] != -1)
        return sendError(playerid, "Nu poti face asta deoarece esti intr-un apel.");

    if(playerStaticVar[playerid][var_last_sms_id] == -1 || playerStaticVar[playerid][var_last_sms_sqlid] != playerInfo[playerStaticVar[playerid][var_last_sms_id]][pSQLID])
        return sendError(playerid, "Nu ai trimis un SMS cuiva anterior sau acea persoana s-a deconectat.");

    check_delay
    new returnStringg[128]; 
    if(sscanf(params, "s[128]", returnStringg))
        return sendSyntaxMessage(playerid, "/reply <message>");

    new const returnPlayer = playerStaticVar[playerid][var_last_sms_id];

    if(Iter_Contains(blockedPlayers[returnPlayer], playerid))
        return sendError(playerid, "Nu poti contacta acest numar.");

    if(playerStaticVar[returnPlayer][var_incall] != -1)
        return sendError(playerid, "Nu poti face asta deoarece acel player este intr-un apel.");

    if(!playerStaticVar[returnPlayer][b_PHONE_TURN])
        return sendError(playerid, "Acel player nu are telefonul pornit.");

    playerInfo[playerid][pPhoneCredit] --;
    set_delay;

    va_mysql_tquery("UPDATE `users` SET `PhoneCredit` = %d WHERE `ID` = %d", playerInfo[playerid][pPhoneCredit], playerInfo[playerid][pSQLID]);

    SendClientMessage(returnPlayer, COLOR_YELLOW, "SMS from %s (%d): %s", GetName(playerid), playerInfo[playerid][pPhoneNumber], gString);
    SendClientMessage(playerid, COLOR_TEAL, "SMS sent to %s (%d): %s", GetName(returnPlayer), playerInfo[returnPlayer][pPhoneNumber], gString); 

    PlayerPlaySound(returnPlayer, 1138, 0.0, 0.0, 0.0);
    return true;
}

YCMD:turn(playerid, params[], help) {
    if(playerInfo[playerid][pPhoneNumber] == 0)
        return true;

    if(!strcmp(params, "on")) {
        if(playerStaticVar[playerid][b_PHONE_TURN])
            return sendError(playerid, "Ai deja pornit telefonul.");

        playerStaticVar[playerid][b_PHONE_TURN] = true;
        SendClientMessage(playerid, COLOR_GREY, "* You turned on your phone.");   
    }
    else if(!strcmp(params, "off")) {
        if(!playerStaticVar[playerid][b_PHONE_TURN]) 
            return sendError(playerid, "Ai deja oprit telefonul.");

        playerStaticVar[playerid][b_PHONE_TURN] = false;
        SendClientMessage(playerid, COLOR_GREY, "* You turned off your phone.");  
    }
    else if(!strcmp(params, "calls")) {
        playerStaticVar[playerid][b_calls_turn] = playerStaticVar[playerid][b_calls_turn] ? false : true;
        SendClientMessage(playerid, COLOR_GREY, "* You turned %s your calls.", (playerStaticVar[playerid][b_calls_turn] ? ("off") : ("on")));  
    }
    else 
        return sendSyntaxMessage(playerid, "/turn <on/off/calls>");

    return true;   
}

YCMD:sms(playerid, params[], help) {
    if(playerStaticVar[playerid][var_sms_delay] > gettime())
        return 1;

    if(playerInfo[playerid][pPhoneNumber] == 0)
        return sendError(playerid, "Nu ai un telefon.");

    if(!playerStaticVar[playerid][b_PHONE_TURN])
        return sendError(playerid, "Nu ai telefonul pornit.");

    if(playerInfo[playerid][pPhoneCredit] < 1)
        return sendError(playerid, "Nu ai credit disponibil.");

    if(playerStaticVar[playerid][var_incall] != -1)
        return sendError(playerid, "Nu poti face asta deoarece esti intr-un apel.");

    new number, phoneCV[128]; if(sscanf(params, "ds[128]", number, phoneCV))
        return sendSyntaxMessage(playerid, "/sms <number> <message>");

    new const returnPlayer = MAP_contains_val(phoneNumbers, number) ? MAP_get_val_val(phoneNumbers, number) : -1;

    if((number > 999999 || number < 0) || returnPlayer == -1)
        return sendError(playerid, "Acel numar de telefon este invalid.");

    if(number == playerInfo[playerid][pPhoneNumber])
        return sendError(playerid, "Nu poti folosi aceasta comanda asupra ta.");

    if(playerStaticVar[returnPlayer][var_incall] != -1)
        return sendError(playerid, "Nu poti face asta deoarece acel player este intr-un apel.");

    if(!playerStaticVar[returnPlayer][b_PHONE_TURN])
        return sendError(playerid, "Acel player nu are telefonul pornit.");

    if(Iter_Contains(blockedPlayers[returnPlayer], playerid))
        return sendError(playerid, "Nu poti contacta acest numar.");

    playerInfo[playerid][pPhoneCredit] --;
    playerStaticVar[playerid][var_sms_delay] = gettime() + 2;

    if(playerInfo[playerid][pPhone]) {
        playerStaticVar[playerid][var_last_sms_id] = returnPlayer;
        playerStaticVar[playerid][var_last_sms_sqlid] = playerInfo[returnPlayer][pSQLID];
    }

    if(playerInfo[returnPlayer][pPhone]) {
        playerStaticVar[returnPlayer][var_last_sms_id] = playerid;
        playerStaticVar[returnPlayer][var_last_sms_sqlid] = playerInfo[playerid][pSQLID];
    }

    va_mysql_tquery("UPDATE `users` SET `PhoneCredit` = %d WHERE `ID` = %d", playerInfo[playerid][pPhoneCredit], playerInfo[playerid][pSQLID]);

    va_SendClientMessage(returnPlayer, COLOR_YELLOW, "SMS from %s (%d): %s", GetName(playerid), playerInfo[playerid][pPhoneNumber], phoneCV);
    va_SendClientMessage(playerid, COLOR_YELLOW, "SMS sended to %s (%d): %s", GetName(returnPlayer), number, phoneCV); 

    PlayerPlaySound(returnPlayer, 1138, 0.0, 0.0, 0.0);
    return true;
}


YCMD:number(playerid, params[], help) {
    if(!playerInfo[playerid][pPhoneBook])
        return sendError(playerid, "Nu ai o carte de telefoane.");

    new returnPlayer; if(sscanf(params, "u", returnPlayer))
        return sendSyntaxMessage(playerid, "/number <playerid/name>");

    if(!playerStaticVar[returnPlayer][playerLogged]) 
        return sendError(playerid, "Acel player nu este logat.");

    if(playerInfo[returnPlayer][pPhoneNumber] != 0) 
        return va_SendClientMessage(playerid, -1, "%s's phone number is: %d", GetName(returnPlayer), playerInfo[returnPlayer][pPhoneNumber]);

    return va_SendClientMessage(playerid, -1, "%s's phone number is: None", GetName(returnPlayer));
}

YCMD:spawnchange(playerid, params[], help)
{
    format(returnString, sizeof returnString, "%s%s%s", (!playerInfo[playerid][pGroup] ? ("La spawn civil") : va_return("La HQ-ul factiunii %s", factionName(playerInfo[playerid][pGroup]))), (!playerInfo[playerid][pHouse] ? ("") : va_return("\nLa casa cu ID %d", playerInfo[playerid][pHouse])), (!playerInfo[playerid][pApartament] ? ("") : va_return("\nLa apartamentul cu ID %d", playerInfo[playerid][pApartament])));
    Dialog_Show(playerid, DIALOG_SPAWNCHANGE, DIALOG_STYLE_LIST, "Locatie spawn", returnString, "Selectare", "Iesire");
    return true;
}

Dialog:DIALOG_SPAWNCHANGE(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    playerInfo[playerid][pSpawnLocation] = listitem;
    SendClientMessage(playerid, -1, "%d", listitem);
    SendClientMessage(playerid, -1, "Locatia de spawn a fost modificata cu succes.");

    mysql_format(SQL, returnString, sizeof returnString, "update `users` set `SpawnLocation` = '%d' where `id` = '%d';", playerInfo[playerid][pSpawnLocation], playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

Dialog:DIALOG_CANCELCP(playerid, response, listitem, inputtext[])
{
    if(response)
        return Command_ReProcess(playerid, "killcp", false);

    return true;
}

GetPlayerFPS(playerid)
{
    if(GetPlayerDrunkLevel(playerid) < 100)
    {
        SetPlayerDrunkLevel(playerid, 2000);
    }
    else
    {
        if(playerStaticVar[playerid][pDrunkLevelLast] != GetPlayerDrunkLevel(playerid))
        {
            if(playerStaticVar[playerid][pDrunkLevelLast] - GetPlayerDrunkLevel(playerid) < 0)
                playerStaticVar[playerid][pFPS] = 100;

            else
                playerStaticVar[playerid][pFPS] = playerStaticVar[playerid][pDrunkLevelLast] - GetPlayerDrunkLevel(playerid);
        }
        playerStaticVar[playerid][pDrunkLevelLast] = GetPlayerDrunkLevel(playerid);
    }
    return playerStaticVar[playerid][pFPS];
}

YCMD:sstats(playerid, params[], help)
{
    check_admin

    #if TEST_SERVER
        returnString = "{FF0000}[TEST_SERVER]{ffffff}";
    #endif

    SendClientMessage(playerid, COLOR_TEAL, "------------------------------- {ffffff}System variables (current){67AAB1} -------------------------------");

    va_SendClientMessage(playerid, -1, "Tick: %d | MYSQL Unprocessed Queries: %d | Objects: %d | Pickups: %d | 3D Text Labels: %d", GetServerTickRate(), mysql_unprocessed_queries(), CountDynamicObjects(), CountDynamicPickups(), CountDynamic3DTextLabels());
    va_SendClientMessage(playerid, -1, "Vehicles: %d [%d personal, %d admin, %d server (group+free+clan), %d job]", MAX_VEHICLES, Iter_Count(vehicleType<VEH_TYPE_PERSONAL>), Iter_Count(vehicleType<VEH_TYPE_ADMIN>), (Iter_Count(vehicleType<VEH_TYPE_GROUP>) + Iter_Count(vehicleType<VEH_TYPE_NONE>) + Iter_Count(vehicleType<VEH_TYPE_CLAN>)), Iter_Count(vehicleType<VEH_TYPE_JOB>));

    va_SendClientMessage(playerid, -1, "Houses: %d | Businesses: %d | Clans: %d | Weather: %d | /n on: %d", returnServerHouses, returnServerBusinesses, returnServerClans, serverWeather, Iter_Count(helperQuestions));
    va_SendClientMessage(playerid, -1, "On: %d | In login queue: %d | Auction: %d houses, %d businesses, %d apartments", Iter_Count(Player), Iter_Count(playersInQueue), 0, 0, 0);
    va_SendClientMessage(playerid, -1, "Last restart: %02d-%02d-%d %02d:%02d:%02d | %d backtraces since last restart", serverTime[5], serverTime[4], serverTime[3], serverTime[0], serverTime[1], serverTime[2], serverBacktraces);
    return SendClientMessage(playerid, COLOR_TEAL, "-----------------------------------------------------------------------------------");
}

stock CalculeazaTimp2(secunde) {
    new time = secunde;
    time = time%3600;
    new minute = time/60;
    time = time%60;
    new secunde2 = time;
    new string[10];
    format(string, sizeof(string), "%02d:%02d",minute,secunde2);
    return string;
}

YCMD:time(playerid, params[], help)
{
    new hour, minute, second;
    gettime(hour, minute, second);

    va_SendClientMessage(playerid, -1, "The current time is %d:%s%d (%d seconds).", hour, (minute < 10) ? ("0") : (""), minute, second);
    va_SendClientMessage(playerid, -1, "Playing for %d seconds [%s].", playerInfo[playerid][pPayDay], CalculeazaTimp2(playerInfo[playerid][pPayDay]));
    sendNearbyMessage(playerid, COLOR_PURPLE, "* %s raises her hand and looks down at her watch.", GetDBName(playerid));
    return true;
}

YCMD:low(playerid, params[], help) return SendClientMessage(playerid, -1, "i'm in love with the coco.");

YCMD:afklist(playerid, params[], help)
{
    if(playerInfo[playerid][pAdmin] < 5)
        return returnNoAcces(playerid);

    SendClientMessage(playerid, COLOR_CYAN, "AFK Players:");
    foreach(new i : Player)
    {
        if(!playerStaticVar[playerid][playerSleeping])
            continue;

        va_SendClientMessage(playerid, -1, "Player: %s(%d)", GetDBName(i), i);
    }
    return true;
}

YCMD:togkickafk(playerid, params[], help)
{
    if(playerInfo[playerid][pAdmin] < 6)
        return returnNoAcces(playerid);

    if(kickafk == 1)
    {
        kickafk = 0;
        SendClientMessage(playerid, -1, "You disable afk kick.");
    }
    else
    {
        kickafk = 1;
        SendClientMessage(playerid, -1, "You enable afk kick.");
    }
    return true;
}


function:loadATMs()
{
    if(!cache_num_rows())
        return printf("ATM's: %d", cache_num_rows());

    for(new i = 1; i <= cache_num_rows(); ++i)
    {
        cache_get_value_name_int(i - 1, "atmId", AtmInfo[i][idATM]);

        cache_get_value_name_float(i - 1, "atmPosX", AtmInfo[i][fATMPos][0]);
        cache_get_value_name_float(i - 1, "atmPosY", AtmInfo[i][fATMPos][1]);
        cache_get_value_name_float(i - 1, "atmPosZ", AtmInfo[i][fATMPos][2]);

        cache_get_value_name_float(i - 1, "atmPosRotX", AtmInfo[i][fATMPosRot][0]);
        cache_get_value_name_float(i - 1, "atmPosRotY", AtmInfo[i][fATMPosRot][1]);
        cache_get_value_name_float(i - 1, "atmPosRotZ", AtmInfo[i][fATMPosRot][2]);

        AtmInfo[i][ATMHealth] = 100;

        new Float: x = AtmInfo[i][fATMPos][0], Float: y = AtmInfo[i][fATMPos][1], Float: a = AtmInfo[i][fATMPosRot][2];
        x += (-0.60 * floatsin(-a, degrees));
        y += (-0.60 * floatcos(-a, degrees));

        AtmInfo[i][rObjectId][0] = CreateDynamicObject(19324, AtmInfo[i][fATMPos][0], AtmInfo[i][fATMPos][1], AtmInfo[i][fATMPos][2], AtmInfo[i][fATMPosRot][0], AtmInfo[i][fATMPosRot][1], AtmInfo[i][fATMPosRot][2], -1, -1, -1, 500.0);
        AtmInfo[i][rObjectId][1] = CreateDynamicPickup(1239, 23, x, y, AtmInfo[i][fATMPos][2], -1, -1, -1, 60);

        UpdateATMLabel(i);
    }
    returnServerATMS = cache_num_rows();
    return printf("ATM's: %d", returnServerATMS);
}

UpdateATMLabel(ATMId)
{
    new colorString[24];
    switch(AtmInfo[ATMId][ATMHealth])
    {
        case 0..19: colorString = "ad0303";
        case 20..59: colorString = "f0a316";
        case 60..100: colorString = "498e04";
    }

    if(AtmInfo[ATMId][ATMHealth] >= 1)
        format(returnString, sizeof returnString, "ATM %d\nWithdraw your cash here!\nPress F to use this ATM.\nATM Health: {%s}%d", ATMId, colorString, AtmInfo[ATMId][ATMHealth]);

    else
        format(returnString, sizeof returnString, "ATM %d\nOut of service\nRepair in {498e04}%d{FFFFFF} seconds.", ATMId, AtmInfo[ATMId][ATMRepairTime] - gettime());

    if(AtmInfo[ATMId][ATMLabel])
        UpdateDynamic3DTextLabelText(AtmInfo[ATMId][ATMLabel], -1, returnString);
    else
        AtmInfo[ATMId][ATMLabel] = CreateDynamic3DTextLabel(returnString, -1, AtmInfo[ATMId][fATMPos][0], AtmInfo[ATMId][fATMPos][1], AtmInfo[ATMId][fATMPos][2], 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1);
}

Dialog:DIALOG_ATM_MENU(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    if(!listitem)
    {
        format(returnString, sizeof returnString, "Your bank account balance is currently standing at $%s.", GetPlayerBank(playerid));
        Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Automated Teller Machine", returnString, "OK", "");
    }
    else if(listitem)
        Dialog_Show(playerid, DIALOG_ATM_WITHDRAW, DIALOG_STYLE_INPUT, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} ATM", "Please specify the amount of money that you'd like to withdraw.\n\nYou can withdraw up to $10,000 from this ATM.\n\nNote: This ATM charges $2 from any withdrawals you make.", "OK", "Cancel");

    return true;
}

Dialog:DIALOG_ATM_WITHDRAW(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    new atmMoney = strval(inputtext);
    if(atmMoney < 2 || atmMoney > 10000)
        return SendClientMessage(playerid, COLOR_GREY, "You can't withdraw this amount of money."), Dialog_Show(playerid, DIALOG_ATM_WITHDRAW, DIALOG_STYLE_INPUT, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} ATM", "Please specify the amount of money that you'd like to withdraw.\n\nYou can withdraw up to $10,000 from this ATM.\n\nNote: This ATM charges $2 from any withdrawals you make.", "OK", "Cancel");

    new moneySuffix = atmMoney, moneyPrefix = 0;

    if(playerInfo[playerid][pAccountLY] < moneyPrefix || playerInfo[playerid][pAccount] < moneySuffix && !playerInfo[playerid][pAccountLY])
        return SendClientMessage(playerid, COLOR_GREY, "You do not have enough money to withdraw this amount."), Dialog_Show(playerid, DIALOG_ATM_WITHDRAW, DIALOG_STYLE_INPUT, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} ATM", "Please specify the amount of money that you'd like to withdraw.\n\nYou can withdraw up to $10,000 from this ATM.\n\nNote: This ATM charges $2 from any withdrawals you make.", "OK", "Cancel");

    substractBankMoney(playerid, playerInfo[playerid][pAccountLY], playerInfo[playerid][pAccount], 0, moneySuffix);

    addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, atmMoney, false);

    Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} Automated Teller Machine", "Your money has been withdrawn.\n\nThank you for using our ATM today!", "OK", "");
    return true;
}

YCMD:gotoatm(playerid, params[], help)
{
    new returnATM;
    if(sscanf(params, "i", returnATM))
        return SendClientMessage(playerid, COLOR_GREY, "Syntax: {FFFFFF}/gotoatm [id]");

    if(returnATM > MAX_ATMS)
        return SendClientMessage(playerid, COLOR_GREY, "Invalid atm id!");

    check_admin

    SetPlayerPos(playerid, AtmInfo[returnATM][fATMPos][0], AtmInfo[returnATM][fATMPos][1], AtmInfo[returnATM][fATMPos][2]);
    SendClientMessage(playerid, -1, "You have been teleported.");

    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerInterior(playerid, 0);
    return true;
}

YCMD:hud(playerid, params[], help)
{
    returnString = "{FFFFFF}Option\t{FFFFFF}Status\n";
    format(returnString, sizeof returnString, "%sHealth Text\t%s\nAmour Text\t%s\nDisplay FPS\t%s\nDamage Informer\t%s\nPayday\t%s\nLevel progress\t%s\nSkills option\t%s\nSpeedometer style\t%s\nMoney Update\t%s\n'Plus' icon\t%s\nSpeedometer\t%s", returnString, playerInfo[playerid][pHUD][0] ? (HudColors[playerInfo[playerid][pHUD][0]]) : ("{990000}Disabled"), playerInfo[playerid][pHUD][1] ? (HudColors[playerInfo[playerid][pHUD][1]]) : ("{990000}Disabled"), playerInfo[playerid][pHUD][2] ? ("{00ff00}Enabled") : ("{990000}Disabled"), playerInfo[playerid][pHUD][3] ? ("{00ff00}Enabled") : ("{990000}Disabled"), playerInfo[playerid][pHUD][4] ? ("{00ff00}Textdraw") : ("{00ff00}Message"), playerInfo[playerid][pHUD][5] ? ("{00ff00}Enabled") : ("{990000}Disabled"), playerInfo[playerid][pHUD][6] ? ("{00ff00}Dialog") : ("{00ff00}Message"), playerInfo[playerid][pHUD][7] ? ("{00ff00}#2") : ("{00ff00}#1"), playerInfo[playerid][pHUD][8] ? ("{00ff00}Enabled") : ("{990000}Disabled"), playerInfo[playerid][pHUD][9] ? ("{00ff00}Enabled") : ("{990000}Disabled"), playerInfo[playerid][pHUD][10] ? ("{00ff00}Enabled") : ("{990000}Disabled"));

    Dialog_Show(playerid, DIALOG_HUD, DIALOG_STYLE_TABLIST_HEADERS, "HUD Options", returnString, "Select", "Cancel");
    return true;
}

Dialog:DIALOG_HUD(playerid, response, listitem, inputtext[])
{
    if(!response)
        return 1;

    switch(listitem)
    {
        case 0:
        {
            SetPVarInt(playerid, "hud_selected", 1);
            Dialog_Show(playerid, DIALOG_HUD_COLOR, DIALOG_STYLE_LIST, "HUD Options: HP", "{FFFFFF}Disable\n{C0C0C0}Grey\n{990000}Red\n{00ff00}Green\n{0000ff}Blue\n{ffff00}Yellow", "Select", "Cancel");
        }
        case 1:
        {
            SetPVarInt(playerid, "hud_selected", 2);
            Dialog_Show(playerid, DIALOG_HUD_COLOR, DIALOG_STYLE_LIST, "HUD Options: Armour", "{FFFFFF}Disable\n{C0C0C0}Grey\n{990000}Red\n{00ff00}Green\n{0000ff}Blue\n{ffff00}Yellow", "Select", "Cancel");
        }
        case 2:
        {
            destroyHudTextdraw(playerid);
            PlayerTextDrawHide(playerid, playerTextdraws[playerid][PlayerFPS]);
            va_SendClientMessage(playerid, -1, "FPS option updated! (now %s)", playerInfo[playerid][pHUD][2] ? ("disabled") : ("enabled"));
            playerInfo[playerid][pHUD][2] = playerInfo[playerid][pHUD][2] ? 0 : 1;
            if(Iter_Contains(displayFPS, playerid))
                Iter_Remove(displayFPS, playerid);
            else {
                Iter_Add(displayFPS, playerid);
            }
        }
        case 3:
        {
            SendClientMessage(playerid, -1, "Damage informer updated!");
            playerInfo[playerid][pHUD][3] = playerInfo[playerid][pHUD][3] ? 0 : 1;
        }
        case 4:
        {
            va_SendClientMessage(playerid, -1, "Payday option updated! (now %s)", playerInfo[playerid][pHUD][4] ? ("message") : ("textdraw"));
            playerInfo[playerid][pHUD][4] = playerInfo[playerid][pHUD][4] ? 0 : 1;

        }
        case 5:
        {
            va_SendClientMessage(playerid, -1, "Level progress option updated! (now %s)", playerInfo[playerid][pHUD][5] ? ("disabled") : ("enabled"));
            playerInfo[playerid][pHUD][5] = playerInfo[playerid][pHUD][5] ? 0 : 1;
            if(playerInfo[playerid][pHUD][5])
                updateLevelProgress(playerid);
            else
                destroyLevelProgress(playerid);
        }
        case 6:
        {
            va_SendClientMessage(playerid, -1, "Skills option updated! (now %s)", playerInfo[playerid][pHUD][6] ? ("message") : ("dialog"));
            playerInfo[playerid][pHUD][6] = playerInfo[playerid][pHUD][6] ? 0 : 1;
        }
        case 7:
        {
            va_SendClientMessage(playerid, -1, "Speedometer option updated! (now %s)", playerInfo[playerid][pHUD][7] ? ("#1") : ("#2"));
            playerInfo[playerid][pHUD][7] = playerInfo[playerid][pHUD][7] ? 0 : 1;

            if(!playerInfo[playerid][pHUD][10])
                return true;


            if(playerInfo[playerid][pHUD][7])
                PlayerTextDrawHide(playerid, playerTextdraws[playerid][speedometerTD][0]), PlayerTextDrawShow(playerid, playerTextdraws[playerid][kmTD]), PlayerTextDrawShow(playerid, playerTextdraws[playerid][SpeedTD]), PlayerTextDrawShow(playerid, playerTextdraws[playerid][speedometerTD][1]);
            else
                PlayerTextDrawHide(playerid, playerTextdraws[playerid][kmTD]), PlayerTextDrawHide(playerid, playerTextdraws[playerid][SpeedTD]), PlayerTextDrawHide(playerid, playerTextdraws[playerid][speedometerTD][1]), PlayerTextDrawShow(playerid, playerTextdraws[playerid][speedometerTD][0]);
        }
        case 8:{
            va_SendClientMessage(playerid, -1, "Money Update option updated! (now %s)", playerInfo[playerid][pHUD][8] ? ("disabled") : ("enabled"));
            playerInfo[playerid][pHUD][8] = playerInfo[playerid][pHUD][8] ? 0 : 1;
        }
        case 9:
        {
            va_SendClientMessage(playerid, -1, "'Plus' icon option updated! (now %s)", playerInfo[playerid][pHUD][9] ? ("disabled") : ("enabled"));
            playerInfo[playerid][pHUD][9] = playerInfo[playerid][pHUD][9] ? 0 : 1;
            if(playerInfo[playerid][pHUD][9])
                TextDrawShowForPlayer(playerid, healthIcon);
            else
                TextDrawHideForPlayer(playerid, healthIcon);
        }
        case 10:
        {
            va_SendClientMessage(playerid, -1, "Speedometer option updated! (now %s)", playerInfo[playerid][pHUD][10] ? ("disabled") : ("enabled"));
            playerInfo[playerid][pHUD][10] = playerInfo[playerid][pHUD][10] ? 0 : 1;

            if(!playerInfo[playerid][pHUD][10])
            {
                PlayerTextDrawHide(playerid, playerTextdraws[playerid][playerTextdraws[playerid][SpeedTD]]);
                PlayerTextDrawHide(playerid, playerTextdraws[playerid][kmTD]);
                PlayerTextDrawHide(playerid, playerTextdraws[playerid][speedometerTD][0]);
                PlayerTextDrawHide(playerid, playerTextdraws[playerid][speedometerTD][1]);
            }
        }
    }

    if(!GetPVarInt(playerid, "hud_selected"))
    {
        mysql_format(SQL, returnString, sizeof returnString, "update `users` set `HudOptions` = '%d %d %d %d %d %d %d %d %d %d %d' where `id` = '%d';", playerInfo[playerid][pHUD][0], playerInfo[playerid][pHUD][1], playerInfo[playerid][pHUD][2], playerInfo[playerid][pHUD][3], playerInfo[playerid][pHUD][4], playerInfo[playerid][pHUD][5], playerInfo[playerid][pHUD][6], playerInfo[playerid][pHUD][7], playerInfo[playerid][pHUD][8], playerInfo[playerid][pHUD][9], playerInfo[playerid][pHUD][10], playerInfo[playerid][pSQLID]);
        mysql_tquery(SQL, returnString, "", "");
    }
    return true;
}

Dialog:DIALOG_HUD_COLOR(playerid, response, listitem, inputtext[])
{
    new listItemId = GetPVarInt(playerid, "hud_selected");
        

    if(!response)
        return DeletePVar(playerid, "hud_selected");

    playerInfo[playerid][pHUD][listItemId - 1] = listitem;
    SendClientMessage(playerid, -1, "HUD options !");

    if(listItemId == 1)
        update_health_bar(playerid, UPDATE_TYPE_HEALTH);
    else
        update_health_bar(playerid, UPDATE_TYPE_ARMOUR);

    mysql_format(SQL, returnString, sizeof returnString, "update `users` set `HudOptions` = '%d %d %d %d %d %d %d' where `id` = '%d';", playerInfo[playerid][pHUD][0], playerInfo[playerid][pHUD][1], playerInfo[playerid][pHUD][2], playerInfo[playerid][pHUD][3], playerInfo[playerid][pHUD][4], playerInfo[playerid][pHUD][5], playerInfo[playerid][pHUD][6], playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, CLICK_SOURCE:source) return va_Command_ReProcess(playerid, "id %d", false, clickedplayerid);

YCMD:vehname(playerid, params[], help)
{
	
	if(playerInfo[playerid][pAdmin] >= 1)
	{
		SendClientMessage(playerid, COLOR_TEAL, "--------------------------------------------------------------------------------------------------------------------------------");
		SendClientMessage(playerid, COLOR_WHITE, "Vehicle Search:");
		new
			rcount,
			string[128];

		if(isnull(params)) return SendClientMessage(playerid, -1, "No keyword specified.");
		if(strlen(params) < 3) return SendClientMessage(playerid, -1, "Search keyword too short.");

		for(new v; v < sizeof(aVehicleNames); v++) {
			if(strfind(aVehicleNames[v], params, true) != -1) {

				if(rcount == 0) format(string, sizeof(string), "%s (ID %d)", aVehicleNames[v], v+400);
				else format(string, sizeof(string), "%s | %s (ID %d)", string, aVehicleNames[v], v+400);

				rcount++;
			}
		}

		if(rcount == 0) SendClientMessage(playerid,-1, "No results found.");

		else if(strlen(string) >= 128) SendClientMessage(playerid, -1, "Too many results found.");

		else return SendClientMessage(playerid, COLOR_WHITE, string);

		SendClientMessage(playerid, COLOR_TEAL, "--------------------------------------------------------------------------------------------------------------------------------");
	}
	else return SendClientMessage(playerid, COLOR_RED2, "You don't have acces to this command");
	return 1; 
}


YCMD:id(playerid, params[], help)
{
    new findPlayer[10], i;
    if(sscanf(params, "?<MATCH_NAME_PARTIAL=1>u[10]", findPlayer))
        return sendSyntaxMessage(playerid, "/id [playername/playerid]");

    if(IsNumeric(findPlayer[0]) && IsPlayerConnected(findPlayer[0]) && !playerStaticVar[findPlayer[0]][playerLogged])
        return SendClientMessage(playerid, COLOR_GREY, "That player in login queue. Please try again later.");

    for(i = 0; findPlayer[i] != INVALID_PLAYER_ID; i++)
    {
        if(findPlayer[i] == cellmin) //max users findPlayer can handle
            break;

        new targetid = findPlayer[i],
            stringAFK[32], stringFaction[64], clanStringName[MAX_PLAYER_NAME + 16], playerClan = playerInfo[targetid][pClan];

        if(!playerStaticVar[targetid][playerLogged])
            continue;

        if(playerStaticVar[targetid][playerAFK] && playerInfo[playerid][pAdmin])
            format(stringAFK, sizeof stringAFK, "{FFFFFF}| AFK: %.0f", playerStaticVar[targetid][afkTime]);

        if(playerInfo[targetid][pClanTagPos] == 2) format(clanStringName, sizeof clanStringName, "{%s}%s{FFFFFF}%s", clanInfo[playerClan][clanColor], clanInfo[playerClan][clanTag], playerInfo[targetid][pName]);
        else if(playerInfo[targetid][pClanTagPos] == 3) format(clanStringName, sizeof clanStringName, "{%s}[%s]{FFFFFF}%s", clanInfo[playerClan][clanColor], clanInfo[playerClan][clanTag], playerInfo[targetid][pName]);
        else if(playerInfo[targetid][pClanTagPos] == 5) format(clanStringName, sizeof clanStringName, "{%s}.%s{FFFFFF}%s", clanInfo[playerClan][clanColor], clanInfo[playerClan][clanTag], playerInfo[targetid][pName]);

        else if(playerInfo[targetid][pClanTagPos] == 1) format(clanStringName, sizeof clanStringName, "%s{%s}%s{FFFFFF}", playerInfo[targetid][pName], clanInfo[playerClan][clanColor], clanInfo[playerClan][clanTag]);
        else if(playerInfo[targetid][pClanTagPos] == 4) format(clanStringName, sizeof clanStringName, "%s{%s}[%s]{FFFFFF}", playerInfo[targetid][pName], clanInfo[playerClan][clanColor], clanInfo[playerClan][clanTag]);
        else if(playerInfo[targetid][pClanTagPos] == 6) format(clanStringName, sizeof clanStringName, "%s{%s}%s.{FFFFFF}", playerInfo[targetid][pName], clanInfo[playerClan][clanColor], clanInfo[playerClan][clanTag]);
        else format(clanStringName, sizeof clanStringName, "%s", playerInfo[targetid][pName]);

        format(stringFaction, sizeof stringFaction, (playerInfo[targetid][pGroup] ? va_return("%s (rank %d)", factionName(playerInfo[targetid][pGroup]), playerInfo[targetid][pGroupRank]) : ("None")));
        va_SendClientMessage(playerid, (playerStaticVar[targetid][playerSleeping] ? (COLOR_GREY) : (-1)), "(%d) %s | Level: %d | Faction: %s | Ping: %d | FPS: %d %s", targetid, clanStringName, playerInfo[targetid][pLevel], stringFaction, GetPlayerPing(targetid), (playerInfo[targetid][pAdmin] ? 100 : GetPlayerFPS(targetid)), stringAFK);
    }
    if(i == 0)
        return SendClientMessage(playerid, COLOR_GREY, "Jucatorul specificat nu este conectat.");

    return true;
}

timer hide_payday_textdraws[10000](playerid) return TextDrawHideForPlayer(playerid, PayDayTD[0]), TextDrawHideForPlayer(playerid, PayDayTD[1]), PlayerTextDrawHide(playerid, playerTextdraws[playerid][PayDayPTD]);

new lastGivedPayday = -1;
WhenPlayersGotPayday()
{
    new returnQuery[256];
    foreach(new playerid : Player)
    {
        new BankInterest;
        BankInterest = floatround( (2.20/100) * playerInfo[playerid][pAccount], floatround_round),

        SendClientMessage(playerid, COLOR_SERVER, "----------------------------------------------------------------------------");
        SendClientMessage(playerid, COLOR_SERVER, "PAYDAY >>{ffffff} Your paycheck has arrived; please visit the bank to withdraw your money.");

        va_SendClientMessage(playerid, COLOR_SERVER, "PAYDAY >>{ffffff} Ai primit %.2f ore jucate.(%d minute)", (playerInfo[playerid][pPayDay] / 3600.0), floatround(playerInfo[playerid][pPayDay] / 60.0));

        playerInfo[playerid][pHours] += (playerInfo[playerid][pPayDay] / 3600.0);

        playerInfo[playerid][pPayDay] = 0;
        UpdatePVars(playerid, "PayDay", playerInfo[playerid][pPayDay]);
        AddRespectPoints(playerid, playerInfo[playerid][pLevel] * 1 + RandomEx(2, 10) );

        new robPointsLimit = Iter_Contains(premiumPlayers, playerid) ? 100 : 10;

        if(playerInfo[playerid][pRobPoints] >= robPointsLimit) {
            va_SendClientMessage(playerid, COLOR_SERVER, "PAYDAY >>{ffffff} Nu ai primit niciun %s point pentru ca ai deja 20 %s points.", (factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_POLICE ? ("Free") : ("Rob")), (factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_POLICE ? ("Free") : ("Rob")));
        }
        else {
            playerInfo[playerid][pRobPoints] += RandomEx(4, 8);

            if(playerInfo[playerid][pRobPoints] > robPointsLimit)
            {
                playerInfo[playerid][pRobPoints] = robPointsLimit;
            }

        }

        if(Iter_Contains(premiumPlayers, playerid))
        {
            new damibonus[MAX_PLAYERS];
            damibonus[playerid] = random(4) + playerInfo[playerid][pPremiumAccount];
            AddRespectPoints(playerid, damibonus[playerid]);
            playerInfo[playerid][pCases] += 1;
            UpdatePVars(playerid, "Cases", playerInfo[playerid][pCases]);
            playerInfo[playerid][pCasesKeys] += 1;
            UpdatePVars(playerid, "CaseKeys", playerInfo[playerid][pCasesKeys]);
            va_SendClientMessage(playerid, COLOR_SERVER, "PAYDAY >>{ffffff} Deoarece ai cont premium, ai primit %d respect points si un case [/opencase].", damibonus[playerid]);
            damibonus[playerid] = 0;
        }

        if(playerInfo[playerid][pHUD][4] == 0)
        {
            va_SendClientMessage(playerid, COLOR_SERVER, "PAYDAY >>{ffffff} Interest Rate: 2.20%" );
            va_SendClientMessage(playerid, COLOR_SERVER, "PAYDAY >>{ffffff} Bank balance: $%s | Bank interest: $%s", GetPlayerBank(playerid), FormatNumber(BankInterest));
            if(playerInfo[playerid][pHouse])
                va_SendClientMessage(playerid, COLOR_SERVER, "PAYDAY >>{ffffff} Rent: $%d | Total earnings: $%d", houseInfo[playerInfo[playerid][pHouse]][houseRent], houseInfo[playerInfo[playerid][pHouse]][houseSafe]);
        }
        else
        {
            va_PlayerTextDrawSetString(playerid, playerTextdraws[playerid][PayDayPTD], "BANK BALANCE: $%s~n~INTEREST: $%s~n~RENT: $%s", GetPlayerBank(playerid), FormatNumber(BankInterest), houseInfo[playerInfo[playerid][pHouse]][houseRent]);
            PlayerTextDrawShow(playerid, playerTextdraws[playerid][PayDayPTD]);

            for(new i; i < 2; i++)
            {
                TextDrawShowForPlayer(playerid, PayDayTD[i]);
            }
            defer hide_payday_textdraws(playerid);
        }
        SendClientMessage(playerid, COLOR_SERVER, "----------------------------------------------------------------------------");
        if(playerInfo[playerid][pPayDay] > 1201) {
            playerInfo[playerid][pQuestsUnlocked] = 1;
            UpdatePVars(playerid, "QuestsUnlocked", playerInfo[playerid][pQuestsUnlocked]);
            GivePlayerQuests(playerid);
            SendClientMessage(playerid, COLOR_SERVER, "QUESTS >> {ffffff} You unlocked the quests, use [/quests].");
        }
        else {
            SendClientMessage(playerid, COLOR_SERVER, "QUESTS >> {ffffff} You don't have minimum 20 minutes played this payday to unlock the quests.");
        }
        if(playerInfo[playerid][pGroup])
        {
            va_SendClientMessage(playerid, COLOR_SERVER, "FACTION BONUS >>{ffffff} You received $%s as a member of %s (rank %d).", FormatNumber(playerInfo[playerid][pGroupRank] * 100000), factionName(playerInfo[playerid][pGroup]), playerInfo[playerid][pGroupRank]);
            addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, playerInfo[playerid][pGroupRank] * 100000, false);

        }

        new houseId = playerInfo[playerid][pHouse];
        if(houseInfo[houseId][houseRent] && playerInfo[playerid][pSQLID] != houseInfo[houseId][houseOwner])
        {
            houseInfo[houseId][houseSafe] += houseInfo[houseId][houseRent];

            new moneySuffix = houseInfo[houseId][houseRent];
            if(playerInfo[playerid][pAccount] < moneySuffix && !playerInfo[playerid][pAccountLY])
                return playerInfo[playerid][pHouse] = 0;

            if(playerInfo[playerid][pAccount] - moneySuffix < 0)
            {
                if(!playerInfo[playerid][pAccountLY])
                    return playerInfo[playerid][pHouse] = 0;

                playerInfo[playerid][pAccount] += 1000000000 - playerInfo[playerid][pAccount] - moneySuffix;

                playerInfo[playerid][pAccountLY] --;
            }
            else
                playerInfo[playerid][pAccount] -= moneySuffix;

            mysql_format(SQL, returnString, sizeof returnString, "update `houses` set `Safe` = '%d' where `ID` = '%d';", houseInfo[houseId][houseSafe], houseId);
            mysql_tquery(SQL, returnString, "", "");
        }

        if(playerInfo[playerid][pGroupPunish] > 0) 
        { 
            if(Iter_Contains(premiumPlayers, playerid))
                playerInfo[playerid][pGroupPunish] -= 2;
            else
                playerInfo[playerid][pGroupPunish] --;

            if(playerInfo[playerid][pGroupPunish] < 0)
                playerInfo[playerid][pGroupPunish] = 0;

        }
        mysql_format(SQL, returnQuery, sizeof returnQuery, "update `users` set `Hours` = '%.2f', `FactionPunish` = '%d' where `id` = '%d';", playerInfo[playerid][pHours], playerInfo[playerid][pGroupPunish], playerInfo[playerid][pSQLID]);
        mysql_tquery(SQL, returnQuery, "", "");


        if(factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_POLICE || playerInfo[playerid][pGroup] == GROUP_TYPE_SI)
        {
            playerInfo[playerid][pGunLic] = 100;
            playerInfo[playerid][pFlyLic] = 100;
            playerInfo[playerid][pBoatLic] = 100;
        }
        else
        {
            if(playerInfo[playerid][pCarLic] > 0) playerInfo[playerid][pCarLic] --;
            if(playerInfo[playerid][pGunLic] > 0) playerInfo[playerid][pGunLic] --;
            if(playerInfo[playerid][pFlyLic] > 0) playerInfo[playerid][pFlyLic] --;
            if(playerInfo[playerid][pBoatLic] > 0) playerInfo[playerid][pBoatLic] --;
        }
        mysql_format(SQL, returnQuery, sizeof returnQuery, "update `users` set `WeaponLicence` = '%d', `FlyLicence` = '%d', `SailingLicence` = '%d' where `id` = '%d';", playerInfo[playerid][pGunLic], playerInfo[playerid][pFlyLic], playerInfo[playerid][pBoatLic], playerInfo[playerid][pSQLID]);
        mysql_tquery(SQL, returnQuery, "", "");

    }
    return true;
}

function:formatSeconds(time_ms)
{
    new seconds = time_ms / 1000;
    return seconds;
}

YCMD:stats(playerid, params[], help) return showPlayerStats(playerid, playerid);

YCMD:check(playerid, params[], help)
{
    check_admin

    new returnPlayer;
    if(sscanf(params, "u", returnPlayer))
        return sendSyntaxMessage(playerid, "/check <Name/Playerid>");

    if(!IsPlayerConnected(returnPlayer))
        return sendError(playerid, "Player not connected.");

    showPlayerStats(playerid, returnPlayer);
	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s has used /check on %s.", GetName(playerid), GetName(returnPlayer));
    return true;
}

function:showPlayerStats(playerid, targetid)
{


    SendClientMessage(playerid, COLOR_SERVER, "----------------------------------------------------------------------------------------------------------");

    va_SendClientMessage(playerid, COLOR_SERVER, "General:{ffffff} %s [%d] | played time: %.2f hours | phone: %s | job: %s", playerInfo[targetid][pName], targetid, playerInfo[targetid][pHours], (!playerInfo[targetid][pPhoneNumber] ? ("no") : va_return("yes (number: %d, credit: $%s)", playerInfo[targetid][pPhoneNumber], FormatNumber(playerInfo[targetid][pPhoneCredit]))), returnJobName(job_data[targetid][playerJob]));
    va_SendClientMessage(playerid, COLOR_SERVER, "Account:{ffffff} level %d (%d/%d respect points) | next level cost: %s | next level hours: %d | gender: %s | warns: %d/3 | referral: %d", playerInfo[targetid][pLevel], playerInfo[targetid][pExperience], returnLevelReq(playerInfo[playerid][pLevel], 1), FormatNumber(returnLevelReq(playerInfo[playerid][pLevel], 2)), returnLevelReq(playerInfo[playerid][pLevel], 3), (!playerInfo[targetid][pGender] ? ("male") : ("female")), playerInfo[targetid][pWarns], playerInfo[targetid][pSQLID]);
    va_SendClientMessage(playerid, COLOR_SERVER, "Economy:{ffffff} money: $%s (cash) | $%s (bank) | %s | %s | %s", formatMoney(GetMilliards(targetid), playerInfo[targetid][pMoney]), formatMoney(playerInfo[targetid][pAccountLY], playerInfo[targetid][pAccount]), (!playerInfo[targetid][pHouse] ? ("no house") : va_return("house: %d", playerInfo[targetid][pHouse])), (!playerInfo[targetid][pBusiness] ? ("no business") : va_return("business: %d", playerInfo[targetid][pBusiness])), (!playerInfo[targetid][pApartament] ? ("no apartment") : va_return("apartment: %d (floor %d)", playerInfo[targetid][pApartament], GetApFloorLevel(playerInfo[targetid][pApartament]))));

    if(playerInfo[targetid][pGroup])
        va_SendClientMessage(playerid, COLOR_SERVER, "Faction:{ffffff} %s | %s | %d/20 faction punish | %d/3 faction warns", (playerInfo[targetid][pGroup] ? va_return("%s", factionName(playerInfo[targetid][pGroup])) : ("none")), (playerInfo[targetid][pGroupRank] ? va_return("rank: %d", playerInfo[targetid][pGroupRank]) : ("none")), playerInfo[targetid][pGroupPunish], playerInfo[targetid][pGroupWarns]);
    
    va_SendClientMessage(playerid, COLOR_SERVER, "Shop:{ffffff} premium account: %s | %d premium points | %d hiddens | keys: %d | cases: %d | mining points: %d", (playerInfo[playerid][pPremiumAccount] == true) ? "yes" : "no", playerInfo[targetid][pPremiumPoints], playerInfo[targetid][pHiddenColor], playerInfo[playerid][pCasesKeys], playerInfo[playerid][pCases], playerInfo[playerid][pMiningPoints]);

    if(playerInfo[targetid][pClan])
        va_SendClientMessage(playerid, COLOR_SERVER, "Clan:{ffffff} %s | rank %d (%s) | clan warns: %d/3", clanInfo[playerInfo[playerid][pClan]][clanName], playerInfo[playerid][pClanRank], GetClanRankName(playerid), playerInfo[playerid][pClanWarns]);

    va_SendClientMessage(playerid, COLOR_SERVER, "Other:{ffffff} %s points: %d/%s | materials: %d | %d/%d vehicle slots used", (factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_POLICE ? ("free") : ("rob")), playerInfo[playerid][pRobPoints], (playerInfo[playerid][pPremiumAccount] == true) ? "100" : "10", playerInfo[playerid][pMaterials], GetPCars(playerid), playerInfo[playerid][pCarSlots]);

    if(playerInfo[playerid][pAdmin])
        va_SendClientMessage(playerid, COLOR_ADMCOMMANDS, "Admin Info:{ffffff} virtual world: %d, interior: %d, duty: %s | seconds: %d this hour [%d minutes]", GetPlayerVirtualWorld(targetid), GetPlayerInterior(targetid), (playerStaticVar[targetid][groupDuty] ? ("on") : ("off")), floatround(playerInfo[targetid][pPayDay]), floatround(playerInfo[targetid][pPayDay]) / 60);

    SendClientMessage(playerid, COLOR_SERVER, "----------------------------------------------------------------------------------------------------------");
    return true;
}

stock IsPlayerInRangeOfPlayer(playerid, targetId, Float: radius) 
{
    static Float: pPRPos[3];
    GetPlayerPos(targetId, pPRPos[0], pPRPos[1], pPRPos[2]);

    return IsPlayerInRangeOfPoint(playerid, radius, pPRPos[0], pPRPos[1], pPRPos[2]);
}

stock GetWeaponNameID(id)
{
    static weapString[24];
    format(weapString, sizeof weapString, "%s", GunNames[id]);
    return weapString;
}

stock returnLevelReq(level, type)
{
    if(type == 1) //rp
    {
        switch(level)
        {
            case 1..4: return level * 2;
            default: return level * 3;
        }
    }
    else if(type == 2) //money
    {
        switch(level)
        {
            case 1..4: return level * 4000;
            default: return level * 28000;
        }
    }
    else if(type == 3) //hours
    {
        switch(level)
        {
            case 1..4: return level;
            default: return level * 3;
        }
    }
    return -1;
}

YCMD:buylevel(playerid, params[], help)
{
    if(strmatch(playerInfo[playerid][pEmail], "email@yahoo.com"))
        return SendClientMessage(playerid, COLOR_RED, "Pentru a avansa in level trebuie sa ai o adresa de email valida setata pe cont.");

    if(GetMoney(playerid) < returnLevelReq(playerInfo[playerid][pLevel], 2) && GetMilliards(playerid) < 1)
        return SendClientMessage(playerid, COLOR_GREY, "You do not have enough cash.");


    if(playerInfo[playerid][pExperience] < returnLevelReq(playerInfo[playerid][pLevel], 1))
        return SendClientMessage(playerid, COLOR_GREY, "You do not have the necessary number of respect points.");

    if(playerInfo[playerid][pHours] < returnLevelReq(playerInfo[playerid][pLevel], 3))
         return va_SendClientMessage(playerid, COLOR_GREY, "You need %d played hours to get to level %d.", returnLevelReq(playerInfo[playerid][pLevel], 3), playerInfo[playerid][pLevel] + 1);

    substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, returnLevelReq(playerInfo[playerid][pLevel], 2));

    playerInfo[playerid][pExperience] -= returnLevelReq(playerInfo[playerid][pLevel], 1);

    playerInfo[playerid][pLevel] ++;
    SetPlayerScore(playerid, playerInfo[playerid][pLevel]);

    destroyLevelProgress(playerid);
    va_SendClientMessage(playerid, -1, "{3A8EBA}Felicitari, acum ai level %d!", playerInfo[playerid][pLevel]);

    sendNearbyMessage(playerid, COLOR_PURPLE, "* %s are acum level %d.", GetName(playerid), playerInfo[playerid][pLevel]);

    mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Level` = '%d', `Experience` = '%d' where `id` = '%d';", playerInfo[playerid][pLevel], playerInfo[playerid][pExperience], playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");

    return updateLevelProgress(playerid);
}

AddRespectPoints(playerid, amount)
{
    destroyLevelProgress(playerid);
    playerInfo[playerid][pExperience] += amount;
    updateLevelProgress(playerid);

    mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Experience` = '%d' where `id` = '%d';", playerInfo[playerid][pExperience], playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");
}

updateLevelProgress(playerid)
{
    if(!playerInfo[playerid][pHUD][5])
        return true;

    format(returnString, sizeof returnString, "Level: %d (%d/%d RESPECT)", playerInfo[playerid][pLevel], playerInfo[playerid][pExperience], playerInfo[playerid][pLevel] * 3);
    PlayerTextDrawSetString(playerid, playerTextdraws[playerid][HudTD], returnString);
    PlayerTextDrawShow(playerid, playerTextdraws[playerid][HudTD]);

    HUDProgress[playerid] = CreatePlayerProgressBar(playerid, 513.00, 136.00, 92.50, 1.50, -16776961, 100.0);
    SetPlayerProgressBarMaxValue(playerid, HUDProgress[playerid], playerInfo[playerid][pLevel] * 3);
    SetPlayerProgressBarValue(playerid, HUDProgress[playerid], playerInfo[playerid][pExperience]);
    SetPlayerProgressBarColour(playerid, HUDProgress[playerid], COLOR_SERVER); //0c4999

    return ShowPlayerProgressBar(playerid, HUDProgress[playerid]);
}

destroyLevelProgress(playerid) { DestroyPlayerProgressBar(playerid, HUDProgress[playerid]); PlayerTextDrawHide(playerid, playerTextdraws[playerid][HudTD]); }

YCMD:killcp(playerid, params[], help)
{
    if(job_data[playerid][jobDuty] != JOB_TYPE_NONE)
        return CallLocalFunction("StopJobWork", "i", playerid);

    if(playerStaticVar[playerid][pCheckpoint])
    {
        SendClientMessage(playerid, -1, "You have disabled the checkpoint.");
        playerStaticVar[playerid][pCheckpoint] = 0;

        DisablePlayerCheckpoint(playerid);
        DisablePlayerRaceCheckpoint(playerid);

        GameTextForPlayer(playerid, "~r~redmarker gone!", 5000, 1);
        if(playerStaticVar[playerid][usedFind]){
            cancel_find(playerid);
        }
    }
    return true;
}

YCMD:spawncar(playerid, params[], help)
{
    check_admin

    new returnVehicle, Float: returnPlayerPos[3], playerVehicle;
    if(sscanf(params, "i", returnVehicle))
        return sendSyntaxMessage(playerid, "/spawncar [vehicleid]");

    if(returnVehicle < 400 || returnVehicle > 611)
        return sendError(playerid, "Invalid car ID! (411-611)");

    GetPlayerPos(playerid, returnPlayerPos[0], returnPlayerPos[1], returnPlayerPos[2]);

    playerVehicle = AddStaticVehicleEx(returnVehicle, returnPlayerPos[0], returnPlayerPos[1], returnPlayerPos[2], 90.0, 1, 1, -1, 300, VEH_TYPE_ADMIN);
    PutPlayerInVehicle(playerid, playerVehicle, 0);

	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s spawned an %s.", GetName(playerid), GetVehicleName(returnVehicle));
    return true;
}

YCMD:vre(playerid, params[], help)
{
    check_admin

    static vehicleId;
    if(IsPlayerInAnyVehicle(playerid))
        vehicleId = GetPlayerVehicleID(playerid);
    else
        if(sscanf(params, "i", vehicleId)) return sendSyntaxMessage(playerid, "/vre [vehicleid]");
    
    new vehicleDBId = VehicleSQL[vehicleId];

    if(!IsValidVehicle(vehicleId))
        return sendError(playerid, "Invalid vehicle id.");

    if(IsAJobVehicle(vehicleId))
        return SendClientMessage(playerid, COLOR_RED, "Nu poti despawna o masina de job.");

    if(Iter_Contains(vehicleType<VEH_TYPE_PERSONAL>, vehicleId))
        DestroyPersonalVehicle(vehicleId, vehicleDBId);

    if(Iter_Contains(vehicleType<VEH_TYPE_ADMIN>, vehicleId))
        DestroyVehicle(vehicleId);
    else
        SetVehicleToRespawn(vehicleId);

    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): Admin %s respawned vehicle %d.", GetName(playerid), vehicleId);
    return true;
}

YCMD:va(playerid, params[], help)
{
    check_admin

    new gameRange, streamCountVeh = 0;

    if (sscanf(params, "d", gameRange)) {
        return sendSyntaxMessage(playerid, "/va [range]");
    }

    if (gameRange < 1 || gameRange > 200) 
        return sendError(playerid, "Range must be between 1 and 200.");

    new streamedCarsHandle = malloc(200 * 200);

    if (streamedCarsHandle == NO_ALLOC) {
        return sendError(playerid, "invalid memory allocation, please try again later or restart the server.");
    }

    new streamedCars[200];
    #pragma unused streamedCars


    new Float:posX, Float:posY, Float:posZ, streamDistance;
    GetPlayerPos(playerid, posX, posY, posZ);

    foreach (new vehicleid : streamedVehicles[playerid]) {
        if (IsVehicleOccupied(vehicleid)) {
            continue;
        }

        streamDistance = floatround(GetVehicleDistanceFromPoint(vehicleid, posX, posY, posZ), floatround_round);

        if (streamDistance <= gameRange) {
            streamedCars[streamCountVeh] = vehicleid;
            streamCountVeh++;
        }
    }

    for (new i = 0; i < streamCountVeh; ++i) {
        if (streamedCars[i] != INVALID_VEHICLE_ID) {
            SetVehicleToRespawn(streamedCars[i]);
        }
    }

    free(streamedCarsHandle);

    return va_SendClientMessage(playerid, COLOR_GREY, "You respawned %d vehicles near you (%d.00 game units).", streamCountVeh, gameRange);
}

YCMD:dice(playerid, params[], help)
{
    if(bizInfo[playerStaticVar[playerid][inBusiness]][bizType] != 9)
        return sendError(playerid, "You can use this command only in casinos.");

    if(playerInfo[playerid][pLevel] < 3)
        return sendError(playerid, "You don't have level 3.");

    new returnPlayer, returnAmount;
    if(sscanf(params, "ud", returnPlayer, returnAmount))
        return sendSyntaxMessage(playerid, "/dice [playername/playerid] [money]");

    if(returnAmount < 1 || returnAmount > playerInfo[playerid][pLevel] * 100000)
        return va_SendClientMessage(playerid, -1, "You can bet maximum $%s.", FormatNumber(playerInfo[playerid][pLevel] * 100000));

    if(!IsPlayerConnected(returnPlayer) || returnPlayer == INVALID_PLAYER_ID || returnPlayer == playerid)
        return SendClientMessage(playerid, COLOR_GREY, "Player not connected.");

    if(GetDistanceBetweenPlayers(playerid, returnPlayer) > 10)
        return sendError(playerid, "You need to be near your opponent.");

    if(GetMoney(playerid) < returnAmount && GetMilliards(playerid) < 1 || GetMoney(returnPlayer) < returnAmount && GetMilliards(returnPlayer) < 1)
        return sendError(playerid, "Tu sau adversarul tau nu aveti suficienti bani pentru a juca barbut.");

    SendClientMessage(playerid, COLOR_DARKPINK, "Cerere de barbut trimisa.");
    va_SendClientMessage(returnPlayer, COLOR_DARKPINK, "%s te-a invitat sa joci barbut cu el pe $%s. Scrie /accept dice %d pentru a juca.", GetName(playerid), FormatNumber(returnAmount), playerid);

    playerStaticVar[playerid][playerDiceID] = returnPlayer + 1;
    playerStaticVar[playerid][playerDiceAmount] = returnAmount;
    return true;
}

YCMD:accept(playerid, params[], help)
{
    new returnItem[24], returnPlayer;
    if(sscanf(params, "s[24]u", returnItem, returnPlayer))
    {
        sendSyntaxMessage(playerid, "/accept <Name> <Name/Playerid>");

        SendClientMessage(playerid, -1, "Available names: Drugs, Repair, Materials, Gun, Dice, Call, Lesson.");
        SendClientMessage(playerid, -1, "Available names: Taxi, Medic, Live, Mechanic, Ticket, Refill, Invite, CInvite, Free, Licence");
        return true;
    }

    if(!IsPlayerConnected(returnPlayer))
        return sendError(playerid, "Player not connected.");

    if(strmatch(returnItem, "drugs")) return accept_drugs(playerid, returnPlayer, drugSell[playerid][DRUG_OFFER_TYPE]);

    if(strmatch(returnItem, "materials")) return accept_materials(playerid, returnPlayer);
    
    if(strmatch(returnItem, "gun")) return accept_gun(playerid, returnPlayer);
    
    if(strmatch(returnItem, "licence"))
    {
        if(playerInfo[playerid][pGroup] != 12) return true;
        return sendError(playerid, "Use /acceptlicence");
    }

    if(strmatch(returnItem, "trade"))
    {
        if(getTradeInvite(playerid) != returnPlayer)
            return sendError(playerid, "Nu ai primit vreo invitatie recenta de la acel player.");

        if(!IsPlayerInRangeOfPlayer(playerid, returnPlayer, 3.0))
            return sendError(playerid, "Nu esti in apropierea acelui player.");

        if(isPlayerInTrade(playerid)) 
            return sendError(playerid, "Nu poti face asta deoarece esti deja intr-un trade.");

        if(isPlayerInTrade(returnPlayer))
            return sendError(playerid, "Acel player este deja intr-un trade.");

        if(IsPlayerInAnyVehicle(returnPlayer) || IsPlayerInAnyVehicle(playerid))
            return sendError(playerid, "Nu puteti negocia dintr-un vehicul.");

        startPlayersTrade(returnPlayer, playerid);

        va_SendClientMessage(playerid, COLOR_SERVER, "(Trade): {ffffff}You accepted %s's trade invite.", GetName(returnPlayer));
        va_SendClientMessage(returnPlayer, COLOR_SERVER, "(Trade): {ffffff}%s accepted your trade invite.", GetName(playerid));
        return true;
    }

    if(strmatch(returnItem, "medic"))
    {
        if(!Iter_Contains(medicCallers, returnPlayer))
            return sendError(playerid, "This player has not called ambulance.");

        playerStaticVar[returnPlayer][medicAccepted] = playerid + 1;
        Iter_Remove(medicCallers, playerid);

        va_SendClientMessage(returnPlayer, -1, "{B8FFDB}* Medic %s has accepted your call. Please wait here for the ambulance.", GetName(playerid));

        va_SendClientMessage(playerid, COLOR_YELLOW, "{B8FFDB}* You have accepted the call from %s, you will see the marker untill you have reached it.", GetName(returnPlayer));
        sendGroupMessage(GROUP_TYPE_PARAMEDIC, -1, "{FFFF00}* Medic %s accepted %s (%d) medic call.", GetName(playerid), GetName(returnPlayer));

        set_find_checkpoint(playerid, returnPlayer);
        return true;
    }

    if(strmatch(returnItem, "taxi"))
    {
        if(!Iter_Contains(taxiCallers, returnPlayer))
            return sendError(playerid, "This player has not called taxi.");

        playerStaticVar[returnPlayer][taxiAccepted] = playerid + 1;
        Iter_Remove(taxiCallers, playerid);

        va_SendClientMessage(returnPlayer, -1, "{B8FFDB}* Taxi driver %s has accepted your call. Please wait here for the taxi.", GetName(playerid));

        va_SendClientMessage(playerid, COLOR_YELLOW, "{B8FFDB}* You have accepted the taxi call from %s, you will see the marker untill you have reached it.", GetName(returnPlayer));
        sendGroupMessage(GROUP_TYPE_PARAMEDIC, -1, "{FFFF00} Dispatch: Taxi driver %s accepted %s taxi call.", GetName(playerid), GetName(returnPlayer));

        set_find_checkpoint(playerid, returnPlayer);
        return true;
    }
    
    if(strmatch(returnItem, "ticket"))
    {
        if(!playerStaticVar[returnPlayer][ticketPlayer])
            return sendError(playerid, "No-one offered you a ticket.");

        if(playerStaticVar[returnPlayer][ticketPlayer] != playerid + 1)
            return sendError(playerid, "This player has not offered you ticket.");

        if(GetDistanceBetweenPlayers(playerid, returnPlayer) > 5)
            return sendError(playerid, "The officer is not near you.");

        if(GetMoney(playerid) < playerStaticVar[returnPlayer][ticketPrice] && GetMilliards(playerid) < 1)
            return SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You don't have enough money.");


        va_SendClientMessage(playerid, -1, "{B8FFDB}* You have paid the Ticket of $%s to Officer %s.", FormatNumber(playerStaticVar[returnPlayer][ticketPrice]), GetDBName(returnPlayer));
        va_SendClientMessage(returnPlayer, -1, "{B8FFDB}* %s has paid your Ticket of $%s.", GetDBName(playerid), FormatNumber(playerStaticVar[returnPlayer][ticketPrice]));

        substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, playerStaticVar[returnPlayer][ticketPrice]);
        addMoney(returnPlayer, GetMilliards(returnPlayer), GetMoney(returnPlayer), 0, playerStaticVar[returnPlayer][ticketPrice], false);


        insertRaportPoint(playerid, 2);

        playerStaticVar[returnPlayer][ticketPlayer] = 0;
        playerStaticVar[returnPlayer][ticketPrice] = 0;
        return true;
    }
    
    if(strmatch(returnItem, "dice"))
    {
        if(playerStaticVar[returnPlayer][playerDiceID] != playerid + 1)
            return sendError(playerid, "Acel player nu te-a invitat sa joci barbut cu el.");

        if(GetMoney(playerid) <  playerStaticVar[returnPlayer][playerDiceAmount] && GetMilliards(playerid) < 1 || GetMoney(returnPlayer) <  playerStaticVar[returnPlayer][playerDiceAmount] && GetMilliards(returnPlayer) < 1)
            return sendError(playerid, "Tu sau adversarul tau nu aveti suficienti bani pentru a juca barbut.");


        if(GetDistanceBetweenPlayers(playerid, returnPlayer) > 10)
            return sendError(playerid, "Nu esti langa acel player.");

        new playerDice = random(6) + 1, playerDice2 = random(6) + 1,
            accDice = random(6) + 1, accDice2 = random(6) + 1,
            diceAmount = playerStaticVar[returnPlayer][playerDiceAmount],
            playerWon;

        playerStaticVar[returnPlayer][playerDiceID] = 0;

        if((playerDice + playerDice2) == (accDice + accDice2))
            return sendNearbyMessage(playerid, COLOR_PURPLE, "%s rolls %d %d. %s rolls %d %d. No one won.", GetDBName(playerid), playerDice, playerDice2, GetDBName(returnPlayer), accDice, accDice2);

        if((playerDice + playerDice2) > (accDice + accDice2))
        {
            va_SendClientMessage(playerid, COLOR_GREEN, "(+) You won $%s!", FormatNumber(diceAmount));
            va_SendClientMessage(returnPlayer, COLOR_GREEN, "(-) You lost $%s!", FormatNumber(diceAmount));

            playerWon = playerid;
            addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, diceAmount, false);
            substractMoney(returnPlayer, GetMilliards(returnPlayer), GetMoney(returnPlayer), 0, diceAmount);

        }
        if((playerDice + playerDice2) < (accDice + accDice2))
        {
            va_SendClientMessage(playerid, COLOR_GREEN, "(-) You lost $%s!", FormatNumber(diceAmount));
            va_SendClientMessage(returnPlayer, COLOR_GREEN, "(+) You won $%s!", FormatNumber(diceAmount));

            playerWon = returnPlayer;
            addMoney(returnPlayer, GetMilliards(returnPlayer), GetMoney(returnPlayer), 0, diceAmount, false);
            substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, diceAmount);

        }
        sendNearbyMessage(playerid, COLOR_PURPLE, "%s rolls %d %d. %s rolls %d %d. %s won $%s.", GetDBName(playerid), playerDice, playerDice2, GetDBName(returnPlayer), accDice, accDice2, GetDBName(playerWon), FormatNumber(diceAmount));
        return true;
    }
    
    if(strmatch(returnItem, "invite"))
    {
        if(playerStaticVar[returnPlayer][factionInvitePlayer] != playerid + 1 || playerInfo[playerid][pGroup] || !playerInfo[returnPlayer][pGroup])
            return true;

        playerInfo[playerid][pGroup] = playerInfo[returnPlayer][pGroup];
        Iter_Add(factionMembers<playerInfo[playerid][pGroup]>, playerid);

        playerInfo[playerid][pGroupRank] = 1;
        playerInfo[playerid][pGroupDays] = gettime();

        if(factionType[playerInfo[returnPlayer][pGroup]] == FACTION_TYPE_POLICE)
            Iter_Add(departmentsMembers, playerid);

        va_SendClientMessage(playerid, COLOR_LIGHTBLUE, "You are now member of the %s.", factionName(playerInfo[returnPlayer][pGroup]));
        sendGroupMessage(playerInfo[returnPlayer][pGroup], COLOR_GENANNOUNCE, "%s has joined the group (invited by %s).", GetDBName(playerid), GetDBName(returnPlayer));

        playerStaticVar[returnPlayer][factionInvitePlayer] = 0;

        mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Faction` = '%d', `FactionRank` = '1', `FactionDays` = '%d', `RaportTime` = '%d' where `id` = '%d';", playerInfo[playerid][pGroup], gettime(), (gettime() + (86400 * 7)), playerInfo[playerid][pSQLID]);
        mysql_tquery(SQL, returnString, "", "");

        mysql_format(SQL, returnString, sizeof returnString, "insert into `faction_history` (`player`, `actionby`, `message`) values ('%d', '%d', '%s has joined the group %s (invited by %s)');",
            playerInfo[playerid][pSQLID], playerInfo[returnPlayer][pSQLID], playerInfo[playerid][pName], factionName(playerInfo[playerid][pGroup]), playerInfo[returnPlayer][pName]);
        mysql_tquery(SQL, returnString, "", "");
        return true;
    }
    
    if(strmatch(returnItem, "cinvite"))
    {
        if(playerStaticVar[returnPlayer][clanInvitePlayer] != playerid + 1 || playerInfo[playerid][pClan] || !playerInfo[returnPlayer][pClan])
            return true;

        playerInfo[playerid][pClan] = playerInfo[returnPlayer][pClan];
        Iter_Add(clanMembers<playerInfo[playerid][pClan]>, playerid);

        playerInfo[playerid][pClanRank] = 1;
        playerInfo[playerid][pClanDays] = gettime();

        va_SendClientMessage(playerid, COLOR_LIGHTBLUE, "You are now member of the %s.", clanInfo[playerInfo[returnPlayer][pClan]][clanName]);
        sendClanMessage(playerInfo[returnPlayer][pClan], COLOR_GENANNOUNCE, "%s has joined the clan (invited by %s).", GetDBName(playerid), GetDBName(returnPlayer));
        playerStaticVar[returnPlayer][clanInvitePlayer] = 0;

        OnPlayerUpdateClan(playerid);
        return true;
    }
    
    if(strmatch(returnItem, "live"))
    {
        if(getPlayerLive(playerid) != INVALID_PLAYER_ID)
            return sendError(playerid, "Esti deja intr-un live.");

        if(getPlayerLiveInvite(playerid) != returnPlayer || !isPlayerLiveOfferAvailable(playerid))
            return sendError(playerid, "Nu ai primit vreo invitatie de la acel player sau aceasta a expirat.");

        if(getInterviewersLiveCount() >= 2)
            return sendError(playerid, "Nu poti accepta aceasta oferta deoarece sunt deja 2 invitati in interviu.");

        if(playerInfo[playerid][pWantedLevel])
            return sendError(playerid, "Nu poti face asta deoarece esti urmarit de politie.");

        new const money = getPlayerLiveMoney(playerid);

        if(GetMoney(playerid) < money && GetMilliards(playerid) < 1)
            return sendError(playerid, "Nu ai suma de bani necesara.");

        check_nr_vehicle
        
        substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, money);
        addMoney(returnPlayer, GetMilliards(returnPlayer), GetMoney(returnPlayer), 0, money);

        setPlayerLive(playerid, returnPlayer);

        va_SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You accepted %s's live interview offer for $%s. Use [/lv] to write on the live interview.", GetName(returnPlayer), FormatNumber(money));
        va_SendClientMessage(returnPlayer, COLOR_LIGHTBLUE, "* Player %s accepted your live interview offer for $%s.", GetName(playerid), FormatNumber(money));
        
        return true;
    }

    if(strmatch(returnItem, "licence"))
    {
        check_instructor 
        if(sscanf(params, "d", returnPlayer)) 
            return sendSyntaxMessage(playerid, "/acceptlicence <playerid/name>");
        
        if(!playerStaticVar[returnPlayer][playerLogged])
            return sendError(playerid, "Acel player nu este logat.");

        if(!Iter_Contains(playersNeedLicences, returnPlayer))
            return sendError(playerid, "Acel player nu are nevoie de licenta."); 

        if(getPlayerNeedLicence(playerid) != INVALID_PLAYER_ID)
            return sendError(playerid, "Ai deja o oferta acceptata.");
        
        Iter_Remove(playersNeedLicences, returnPlayer);

        setPlayerNeedLicence(playerid, returnPlayer);
        setPlayerNeedLicence(returnPlayer, playerid);

        va_SendClientMessage(returnPlayer, COLOR_YELLOW, "%s has accepted your request licence, wait for him here.", GetName(playerid));

        set_find_checkpoint(playerid, returnPlayer);
    
        return sendGroupMessage(GROUP_TYPE_SI, COLOR_MEMBER, "%s has accepted the %s requested licence.", GetName(playerid), GetName(returnPlayer));
    }
    
    if(strmatch(returnItem, "free"))
    {
        if(playerStaticVar[returnPlayer][freePlayer] != playerid + 1)
            return sendError(playerid, "That player has not offered you free.");

        if(GetMoney(playerid) < playerStaticVar[returnPlayer][freeAmount] && GetMilliards(playerid) < 1)
            return sendError(playerid, "You don't have enough money.");


        va_SendClientMessage(returnPlayer, COLOR_GREEN2, "(+) %s a acceptat oferta de a fi eliberat iar tu ai primit $%s.", GetName(playerid), FormatNumber(playerStaticVar[returnPlayer][freeAmount]));

        if(playerInfo[returnPlayer][pPremiumAccount]) {
            playerInfo[returnPlayer][pRobPoints] -= 7;
        }
        else {
            playerInfo[returnPlayer][pRobPoints] -= 5;
        }

        substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, playerStaticVar[returnPlayer][freeAmount]);

        addMoney(returnPlayer, GetMilliards(returnPlayer), GetMoney(returnPlayer), 0, playerStaticVar[returnPlayer][freeAmount], false);

        ReleasePlayer(playerid);
        va_SendClientMessage(playerid, -1, "Ai platit $%s pentru a fi eliberat din inchisoare.", FormatNumber(playerStaticVar[returnPlayer][freeAmount]));

        playerStaticVar[returnPlayer][freePlayer] = 0;
        playerStaticVar[returnPlayer][freeAmount] = 0;
        return true;
    }
    
    else
    {
        SendClientMessage(playerid, -1, "Available names: Drugs, Repair, Materials, Gun, Dice, Call, Licence.");
        SendClientMessage(playerid, -1, "Available names: Taxi, Medic, Live, Mechanic, Ticket, Refill, Invite, CInvite, Free");
    }
    return true;
}


YCMD:cancel(playerid, params[], help)
{
    new returnItem[24];
    if(sscanf(params, "s[24]", returnItem))
    {
        sendSyntaxMessage(playerid, "/cancel <Name>");
        SendClientMessage(playerid, -1, "Available name: Drugs, Gun, Materials, Free, Live.");
        SendClientMessage(playerid, -1, "Available name: Taxi, Medic, Ticket, Dice.");
        return true;
    }
    if(strmatch(returnItem, "dice"))
    {
        if(!playerStaticVar[playerid][playerDiceID])
            return sendError(playerid, "You don't have any dice offer sent.");

        playerStaticVar[playerid][playerDiceAmount] = 0;
        playerStaticVar[playerid][playerDiceID] = INVALID_PLAYER_ID;
        SendClientMessage(playerid, COLOR_GREY, "You canceled sell dice offer!");
        return true;
    }
    if(strmatch(returnItem, "live"))
    {
        if(!playerStaticVar[playerid][livePlayer])
            return sendError(playerid, "You don't have any live offer sent.");

        playerStaticVar[playerid][livePlayer] = 0;
        playerStaticVar[playerid][inInterview] = 0;
        playerStaticVar[playerid][liveReporter] = INVALID_PLAYER_ID;
        SendClientMessage(playerid, COLOR_GREY, "You canceled your live offer!");
        return true;
    }
    if(strmatch(returnItem, "free"))
    {
        if(!playerStaticVar[playerid][freePlayer])
            return sendError(playerid, "You don't have any free offer sent.");

        SendClientMessage(playerid, -1, "Oferta de free a expirat, acum poti oferi free altcuiva.");

        playerStaticVar[playerid][freePlayer] = INVALID_PLAYER_ID;
        playerStaticVar[playerid][freeAmount] = 0;

        free_timer[playerid] = Timer:0;
        return true;
    }
    if(strmatch(returnItem, "ticket"))
    {
        if(!playerStaticVar[playerid][ticketPlayer])
            return sendError(playerid, "You don't have any ticket offer sent.");

        SendClientMessage(playerid, COLOR_GREY, "You canceled the ticket!");
        playerStaticVar[playerid][ticketPlayer] = INVALID_PLAYER_ID;
        playerStaticVar[playerid][ticketPrice] = 0;
        return true;
    }
    if(strmatch(returnItem, "gun"))
    {
        if(weaponSell[playerid][GUN_OFFER_PLAYER] == INVALID_PLAYER_ID)
            return sendError(playerid, "You don't have any gun offer sent.");

        SendClientMessage(playerid, COLOR_GREY, "You canceled sell gun offer!");
        return reset_gun_vars(playerid);
    }
    if(strmatch(returnItem, "materials"))
    {
        if(materialSell[playerid][MATS_OFFER_PLAYER] == INVALID_PLAYER_ID)
            return sendError(playerid, "You don't have any gun materials sent.");

        SendClientMessage(playerid, COLOR_GREY, "You canceled materials offer!");
        return reset_mats_vars(playerid);
    }
    if(strmatch(returnItem, "drugs"))
    {
        if(drugSell[playerid][DRUGS_OFFER_PLAYER] == INVALID_PLAYER_ID)
            return sendError(playerid, "You don't have any gun drugs sent.");

        SendClientMessage(playerid, COLOR_GREY, "You canceled drugs offer!");
        return ResetDrugsVars(playerid);
    }
    if(strmatch(returnItem, "taxi"))
    {
        if(playerStaticVar[playerid][taxiAccepted])
        {
            va_SendClientMessage(playerStaticVar[playerid][taxiAccepted] - 1, COLOR_LIGHTBLUE, "Taxi caller canceled the call.");
            playerStaticVar[playerid][taxiAccepted] = 0;
        }
        Iter_Remove(taxiCallers, playerid);

        SendClientMessage(playerid, COLOR_GREY, "You canceled your taxi call!");
        return true;
    }
    if(strmatch(returnItem, "medic"))
    {
        if(playerStaticVar[playerid][medicAccepted])
        {
            va_SendClientMessage(playerStaticVar[playerid][medicAccepted] - 1, COLOR_LIGHTBLUE, "Medic caller canceled the call.");
            playerStaticVar[playerid][medicAccepted] = 0;
        }
        Iter_Remove(medicCallers, playerid);

        SendClientMessage(playerid, COLOR_GREY, "You canceled your medic call!");
        return true;
    }
    return true;
}

YCMD:service(playerid, params[], help)
{
    new returnItem[24];
    if(sscanf(params, "s[24]", returnItem))
    {
        sendSyntaxMessage(playerid, "/service <Name>");
        SendClientMessage(playerid, COLOR_GREY, "available options: taxi, medic");
        return true;
    }

    new Float:callerPos[3], playerZone[28 + 1];
    Get3DZone(callerPos[0], callerPos[1], callerPos[2], playerZone, sizeof playerZone);

    if(strmatch(returnItem, "taxi"))
    {
        if(GetPVarInt(playerid, "taxi_call_delay") > gettime())
            return va_SendClientMessage(playerid, -1, "Vei putea folosi aceasta comanda in %d secunde.", GetPVarInt(playerid, "taxi_call_delay") - gettime());

        if(Iter_Contains(taxiCallers, playerid))
            return sendError(playerid, "Nu puteti trimite o comanda in acest moment deoarece aveti o cerere acceptata solicitata.");

        if(Iter_Count(taxiCallers) > 19)
            return sendError(playerid, "There are too many taxi calls. Try again later!");

        new taxiDrivers;
        foreach(new i : factionMembers<GROUP_TYPE_TAXI>)
        {
            if(!playerStaticVar[i][groupDuty])
                continue;

            sendSplitMessage(i, -1, "{B8FFDB}** %s is in need of a Taxi Driver, Location: %s (%s) - %0.2f. (use /accept taxi %d to accept the call)", GetName(playerid), playerZone, GetCoordinatesCity(callerPos[0], callerPos[1]), GetDistanceBetweenPlayers(i, playerid), playerid);
            taxiDrivers ++;
        }

        if(!taxiDrivers)
            return sendError(playerid, "There are no Taxi drivers On Duty at the moment, try again later.");

        SendClientMessage(playerid, -1, "{3385FF}* You have called for a Taxi, wait for a reply.");
        Iter_Add(taxiCallers, playerid);

        SetPVarInt(playerid, "taxi_call_delay", gettime() + 200);
        return true;
    }
    if(strmatch(returnItem, "medic"))
    {
        if(GetPVarInt(playerid, "medic_call_delay") > gettime())
            return va_SendClientMessage(playerid, -1, "Vei putea folosi aceasta comanda in %d secunde.", GetPVarInt(playerid, "medic_call_delay") - gettime());

        if(Iter_Contains(medicCallers, playerid))
            return sendError(playerid, "Nu puteti trimite o comanda in acest moment deoarece aveti o cerere acceptata solicitata.");

        if(Iter_Count(medicCallers) > 19)
            return sendError(playerid, "There are too many medic calls. Try again later!");

        new medicDrivers;
        foreach(new i : factionMembers<GROUP_TYPE_PARAMEDIC>)
        {
            if(!playerStaticVar[i][groupDuty])
                continue;

            sendSplitMessage(i, -1, "{B8FFDB}** %s is in need of a Medic, Location: %s (%s) - %0.2f. (use /accept medic %d to accept the call).", GetName(playerid), playerZone, GetCoordinatesCity(callerPos[0], callerPos[1]), GetDistanceBetweenPlayers(i, playerid), playerid);
            medicDrivers ++;
        }

        if(!medicDrivers)
            return sendError(playerid, "There are no Medics On Duty at the moment, try again later.");

        SendClientMessage(playerid, -1, "{3385FF}* You have called for a Medic, wait for a reply.");
        Iter_Add(medicCallers, playerid);

        SetPVarInt(playerid, "medic_call_delay", gettime() + 200);
        return true;
    }
    return true;
}

YCMD:findcar(playerid, params[], help)
{
    mysql_format(SQL, returnString, sizeof returnString, "select * from `personalvehicles` where `Owner` = '%s';", playerInfo[playerid][pName]);
    mysql_tquery(SQL, returnString, "show_find_vehicles", "i", playerid);
    return true;
}


function:show_find_vehicles(playerid)
{
    if(!cache_num_rows() && playerRentVehicle[playerid] != INVALID_VEHICLE_ID)
        return SendClientMessage(playerid, COLOR_GREY, "You don't have any personal vehicles or a rented car.");

    new returnVehicles, vehicleString[2048];

    while(returnVehicles < cache_num_rows())
    {
        new vehicleDBModel, vehicleDBId, carId[12] = "";

        cache_get_value_name_int(returnVehicles, "Model", vehicleDBModel);
        cache_get_value_name_int(returnVehicles, "ID", vehicleDBId);

        if(GetVehicleGameID(vehicleDBId))
            format(carId, 12, "(%d)", GetVehicleGameID(vehicleDBId));

        format(vehicleString, sizeof vehicleString, "%s%s {FFFFFF}%s %s\n", vehicleString, GetVehicleSpawnedState(vehicleDBId) != 0 ? ("{0C370A}[spawned]") : ("{ff0000}[despawned]"), aVehicleNames[vehicleDBModel - 400], carId);

        playerStaticVar[playerid][pDialogItems][returnVehicles] = vehicleDBId;
        ++ returnVehicles;
    }
    if(playerRentVehicle[playerid])
        format(vehicleString, sizeof vehicleString, "%s[rented] %s (%d)", vehicleString, aVehicleNames[rentVehModel[playerRentVehicle[playerid]] - 400], playerRentVehicle[playerid]);

    return Dialog_Show(playerid, DIALOG_CARFIND, DIALOG_STYLE_LIST, "Find Vehicles", vehicleString, "Select", "Exit");
}

Dialog:DIALOG_CARFIND(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    new vehicleDBId = playerStaticVar[playerid][pDialogItems][listitem],
        vehicleGameID = GetVehicleGameID(vehicleDBId);

    if(!GetVehicleSpawnedState(vehicleDBId) && vehicleDBId)
        return SendClientMessage(playerid, COLOR_GREY, "This vehicle isn't spawned!");

    if(playerStaticVar[playerid][pCheckpoint])
        return sendError(playerid, "Detii deja un checkpoint activ!");

    if(!vehicleDBId)
        vehicleGameID = playerRentVehicle[playerid];

    new Float: vPos[3];
    GetVehiclePos(vehicleGameID, vPos[0], vPos[1], vPos[2]);

    playerStaticVar[playerid][pCheckpoint] = 1;
    return SetPlayerCheckpoint(playerid, vPos[0], vPos[1], vPos[2], 4);
}

GetPlayerJobSkillName(playerid, jobId)
{
    new jobString[32];

    switch(jobId)
    {
        case JOB_TYPE_DRUGS: format(jobString, sizeof jobString, "%d planted seeds", job_data[playerid][playerJobTimes][jobId]);
        case JOB_TYPE_PIZZA: format(jobString, sizeof jobString, "delivered %d pizzas", job_data[playerid][playerJobTimes][jobId]);
        case JOB_TYPE_FIREMAN: format(jobString, sizeof jobString, "extinguished fires %d", job_data[playerid][playerJobTimes][jobId]);
        case JOB_TYPE_BUS: format(jobString, sizeof jobString, "%d shifts completed", job_data[playerid][playerJobTimes][jobId]);
        case JOB_TYPE_ARMS: format(jobString, sizeof jobString, "delivered materials %d times", job_data[playerid][playerJobTimes][jobId]);
        case JOB_TYPE_GARBAGE: format(jobString, sizeof jobString, "%d shifts completed", job_data[playerid][playerJobTimes][jobId]);
        case JOB_TYPE_FISHER: format(jobString, sizeof jobString, "%d fish caught", job_data[playerid][playerJobTimes][jobId]);
        case JOB_TYPE_COURIER: format(jobString, sizeof jobString, "%d packages delivered", job_data[playerid][playerJobTimes][jobId]);
        case JOB_TYPE_FARM: format(jobString, sizeof jobString, "%d bales deposited", job_data[playerid][playerJobTimes][jobId]);
        case JOB_TYPE_TRUCKER: format(jobString, sizeof jobString, "%d routes completed", job_data[playerid][playerJobTimes][jobId]);
        case JOB_TYPE_MINER: format(jobString, sizeof jobString, "%d ores crushed", job_data[playerid][playerJobTimes][jobId]);

    }
    return jobString;
}

YCMD:skill(playerid, params[], help)
{
    if(!playerInfo[playerid][pHUD][6])
    {
        SendClientMessage(playerid, -1, "Rob skill: 1/6 (0 robs) | Needed for next skill level: 0");

        for(new i = 1; i < SERVER_JOBS; i++)
        {
            if(i == JOB_TYPE_DETECTIVE)
                continue;

            if(job_data[playerid][playerJobSkill][i] != GetJobMaxSkill(i))
                va_SendClientMessage(playerid, -1, "%s skill: %d/%d (%s) | Needed for next skill level: %d", returnJobName(i), GetPlayerJobSkill(playerid, i), GetJobMaxSkill(i), GetPlayerJobSkillName(playerid, i), job_data[playerid][playerJobRem][i]);

            else
                va_SendClientMessage(playerid, -1, "%s skill: %d/%d (%s)", returnJobName(i), GetPlayerJobSkillName(playerid, i), GetJobMaxSkill(i), GetPlayerJobSkillName(playerid, i));
        }
        va_SendClientMessage(playerid, -1, "Lifting skill (muscle level): %d (%d total reps) | Needed for the next lifting skill: %d", playerMuscle[playerid][pMuscleLevel], playerMuscle[playerid][pTotalReps], 50 - playerMuscle[playerid][pMuscleReps]);
    }
    else
    {
        new commandString[2048] = "{FFFFFF}Rob skill: 1/6 (0 robs) | Needed for next skill level: 0\n";

        for(new i = 1; i < SERVER_JOBS; i++)
        {
            if(i == JOB_TYPE_DETECTIVE)
                continue;

            if(job_data[playerid][playerJobSkill][i] != GetJobMaxSkill(i))
                format(commandString, sizeof commandString, "%s{FFFFFF}%s skill: %d/%d (%s) | Needed for next skill level: %d\n", commandString, returnJobName(i), GetPlayerJobSkill(playerid, i), GetJobMaxSkill(i), GetPlayerJobSkillName(playerid, i), job_data[playerid][playerJobRem][i]);

            else
                format(commandString, sizeof commandString, "%s{FFFFFF}%s skill: %d/%d (%s)\n", commandString, returnJobName(i), GetPlayerJobSkill(playerid, i), GetJobMaxSkill(i), GetPlayerJobSkillName(playerid, i));
        }
        format(commandString, sizeof commandString, "%sLifting skill (muscle level): %d (%d total reps) | Needed for the next lifting skill: %d", commandString, playerMuscle[playerid][pMuscleLevel], playerMuscle[playerid][pTotalReps], 50 - playerMuscle[playerid][pMuscleReps]);

        Dialog_Show(playerid, DIALOG_SKILLS, DIALOG_STYLE_MSGBOX, "Skills", commandString, "Ok", "");
    }
    return true;
}

YCMD:giveall(playerid, params[], help)
{
    if(playerInfo[playerid][pAdmin] < 4)
        return returnNoAcces(playerid);

    new itemName[20], iAmount;
    if(sscanf(params, "s[20]i", itemName, iAmount))
    {
        SendClientMessage(playerid, COLOR_GREY, "Syntax: /giveall <item> <amount>");
        SendClientMessage(playerid, COLOR_GREY, "Available options: money, premiumpoints, respect, case, key");
        return true;
    }

    if(strcmp(itemName, "money", true) == 0)
    {
        if(iAmount > 1000000)
            return sendError(playerid, "You cannot give money to all server with an amount more than $1.000.000");

        foreach(new i : Player)
            addMoney(i, GetMilliards(i), GetMoney(i), 0, iAmount, false);

        va_SendClientMessageToAll(COLOR_GREY, "Administratorul %s a oferit %s$ tuturor jucatorilor conectati.", GetDBName(playerid), FormatNumber(iAmount));
    }
    if(strcmp(itemName, "respect", true) == 0)
    {
        if(iAmount > 10)
            return sendError(playerid, "You cannot give respect points to all server with an amount more than 10");

        foreach(new i : Player)
            AddRespectPoints(i, iAmount);

        va_SendClientMessageToAll(COLOR_GREY, "Administratorul %s a oferit %d respect points tuturor jucatorilor conectati.", GetDBName(playerid), iAmount);
    }
    if(strcmp(itemName, "premiumpoints", true) == 0)
    {
        if(iAmount > 30)
            return sendError(playerid, "You cannot give premium points to all server with an amount more than 30");

        foreach(new i : Player)
            playerInfo[i][pPremiumPoints] += iAmount;

        va_SendClientMessageToAll(COLOR_GREY, "Administratorul %s a oferit %d premium points tuturor jucatorilor conectati.", GetDBName(playerid), iAmount);
    }
    if(strcmp(itemName, "case", true) == 0)
    {
        if(iAmount > 10)
            return sendError(playerid, "You cannot give cases to all server with an amount more than 10");

        foreach(new i : Player)
            playerInfo[i][pCases] += iAmount;

        va_SendClientMessageToAll(COLOR_GREY, "Administratorul %s a oferit %d cutii tuturor jucatorilor conectati.", GetDBName(playerid), iAmount);
    }
    if(strcmp(itemName, "key", true) == 0)
    {
        if(iAmount > 10)
            return sendError(playerid, "You cannot give case keys to all server with an amount more than 10");

        foreach(new i : Player)
            playerInfo[i][pCasesKeys] += iAmount;

        va_SendClientMessageToAll(COLOR_GREY, "Administratorul %s a oferit %d chei tuturor jucatorilor conectati.", GetDBName(playerid), iAmount);
    }
    return true;
}

YCMD:set(playerid, params[], help)
{
    if(playerInfo[playerid][pAdmin] < 4)
        return returnNoAcces(playerid);

    new targetId, itemName[20], iAmount;
    if(sscanf(params, "us[20]i", targetId, itemName, iAmount))
    {
        sendSyntaxMessage(playerid, "/set <playerid> <option> <value>");
        SendClientMessage(playerid, COLOR_GREY, "Available options: money, bankmoney, bankly, pp, hours, group, grouprank, respect, skin(a4+), licence, gunlicence, boatlicence, flylicence");
        SendClientMessage(playerid, COLOR_GREY, "Available options: job, addiction, marijuana, cocaine, mushrooms, minepoints, materials, level, cases, caseskeys, gender, robpoints");
        return true;
    }

    if(!IsPlayerConnected(targetId))
        return sendError(playerid, "Invalid player.");

    if(strcmp(itemName,"licence",true) == 0) {
        if(iAmount == 0) playerInfo[targetId][pCarLic] = 0;
        else playerInfo[targetId][pCarLic] = 1;
        playerInfo[targetId][pCarLic] = iAmount;
    }
    else if(strcmp(itemName,"materials",true) == 0) {
        if(playerInfo[playerid][pAdmin] < 5)
            return returnNoAcces(playerid);

        playerInfo[targetId][pMaterials] = iAmount;
        UpdatePVars(targetId, "Materials", playerInfo[targetId][pMaterials]);
    }
    else if(strcmp(itemName,"gender",true) == 0) {
        if(iAmount < 0 || iAmount > 1) return sendError(playerid, "Gender must be 1 (male) or 0 (female).");

        playerInfo[targetId][pGender] = iAmount;
        UpdatePVars(targetId, "Gender", playerInfo[targetId][pGender]);

    }

    else if(strcmp(itemName,"marijuana",true) == 0) {
        if(playerInfo[playerid][pAdmin] < 5)
            return returnNoAcces(playerid);

        playerInfo[targetId][pDrugs] = iAmount;
        UpdatePVars(targetId, "Drugs", playerInfo[targetId][pDrugs]);

    }       
    else if(strcmp(itemName,"addiction",true) == 0) {
        if(playerInfo[playerid][pAdmin] < 5)
            return returnNoAcces(playerid);
        
        if(iAmount < 0 || iAmount > 100) return sendError(playerid, "The addiction must be between 1 and 100.");

        playerStaticVar[targetId][var_addiction_procent] = iAmount;
        UpdatePVars(targetId, "Addiction", playerStaticVar[targetId][var_addiction_procent]);
    }
    else if(strcmp(itemName,"skill",true) == 0) {
        if(playerInfo[playerid][pAdmin] < 5)
            return returnNoAcces(playerid);

        if(!job_data[targetId][playerJob])
            return sendError(playerid,"This player doesn't have a job.");

        if(iAmount < 0 || iAmount > GetJobMaxSkill(job_data[targetId][playerJob]))
            return SendClientMessage(playerid, -1, "Invalid Amount! (1 - %s)", GetJobMaxSkill(job_data[targetId][playerJob]));
        job_data[targetId][playerJobSkill][job_data[targetId][playerJob]] = iAmount;
        job_data[targetId][playerJobRem][job_data[targetId][playerJob]] = pointsForSkilling[job_data[targetId][playerJob]][job_data[targetId][playerJobSkill][job_data[playerid][playerJob]]];
        update_skills(targetId);
    }
    else if(strcmp(itemName,"robpoints",true) == 0) {
        if(playerInfo[playerid][pAdmin] < 5)
            return returnNoAcces(playerid);

        playerInfo[targetId][pRobPoints] = iAmount;
        UpdatePVars(targetId, "robPoints", playerInfo[targetId][pRobPoints]);

    }
    else if(strcmp(itemName,"cases",true) == 0) {
        if(playerInfo[playerid][pAdmin] < 5)
            return returnNoAcces(playerid);

        playerInfo[targetId][pCases] = iAmount;
        UpdatePVars(targetId, "Cases", playerInfo[targetId][pCases]);

    }
    else if(strcmp(itemName,"cocaine",true) == 0) {
        if(playerInfo[playerid][pAdmin] < 5)
            return returnNoAcces(playerid);

        playerInfo[targetId][pCocaine] = iAmount;
        UpdatePVars(targetId, "Cocaine", playerInfo[targetId][pCocaine]);

    }
    else if(strcmp(itemName,"mushrooms",true) == 0) {
        if(playerInfo[playerid][pAdmin] < 5)
            return returnNoAcces(playerid);

        playerInfo[targetId][pMushrooms] = iAmount;
        UpdatePVars(targetId, "Mushrooms", playerInfo[targetId][pMushrooms]);

    }
    else if(strcmp(itemName,"caseskeys",true) == 0) {
        if(playerInfo[playerid][pAdmin] < 5)
            return returnNoAcces(playerid);

        playerInfo[targetId][pCasesKeys] = iAmount;
        UpdatePVars(targetId, "CaseKeys", playerInfo[targetId][pCasesKeys]);

    }
    else if(strcmp(itemName,"minepoints",true) == 0) {
        if(playerInfo[playerid][pAdmin] < 5)
            return returnNoAcces(playerid);

        playerInfo[targetId][pMiningPoints] = iAmount;
        UpdatePVars(targetId, "MiningPoints", playerInfo[targetId][pMiningPoints]);

    }
    else if(strcmp(itemName,"gunlicence",true) == 0) {
        if(iAmount == 0) playerInfo[targetId][pGunLic] = 0;
        playerInfo[targetId][pGunLic] = iAmount;
        UpdatePVars(targetId, "WeaponLicence", playerInfo[targetId][pGunLic]);
    }
    else if(strcmp(itemName,"boatlicence",true) == 0)
    {
        if(iAmount == 0) playerInfo[targetId][pBoatLic] = 0;
        playerInfo[targetId][pBoatLic] = iAmount;
    }
    else if(strcmp(itemName,"level",true) == 0)
    {
        if(playerInfo[playerid][pAdmin] < 6) return returnNoAcces(playerid);
        if(iAmount < 1) return sendError(playerid, "Level must be more than 0.");
        playerInfo[targetId][pLevel] = iAmount;
        UpdatePVars(targetId, "Level", playerInfo[targetId][pLevel]);
        SetPlayerScore(targetId, playerInfo[targetId][pLevel]);
        destroyLevelProgress(playerid);
        updateLevelProgress(targetId);
    }
    else if(strcmp(itemName,"flylicence",true) == 0)
    {
        if(iAmount == 0) playerInfo[targetId][pFlyLic] = 0;
        playerInfo[targetId][pFlyLic] = iAmount;
    }
    else if(strcmp(itemName, "money", true) == 0)
    {
        if(playerInfo[playerid][pAdmin] < 5) 
            return returnNoAcces(playerid);

        if(iAmount < 0 || iAmount > 999999999)
            return sendError(playerid, "Invalid Amount! ($1 - $999.999.999)");

        playerInfo[targetId][pMoney] = iAmount;
        GetMilliards(targetId) = 0;

        ResetPlayerMoney(targetId);
        GivePlayerMoney(targetId, iAmount);

        va_mysql_tquery("UPDATE `users` SET `Money` = %d, `MoneyStore` = 0 WHERE `ID` = %d", iAmount, playerInfo[targetId][pSQLID]);
    }
    else if(strcmp(itemName,"job",true) == 0) {
        job_data[playerid][playerJob] = iAmount;
        UpdatePVars(playerid, "Job", job_data[playerid][playerJob]);
    }
    else if(strcmp(itemName, "group", true) == 0)
    {
        playerInfo[targetId][pGroup] = iAmount;
        playerInfo[targetId][pGroupRank] = 1;
        Iter_Add(factionMembers<playerInfo[targetId][pGroup]>, playerid);

        mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Faction` = '%d', `FactionRank` = '1', `FactionDays` = '%d' where `id` = '%d';", iAmount, gettime(), playerInfo[targetId][pSQLID]);
        mysql_tquery(SQL, returnString, "", "");
    }
    else if(strcmp(itemName, "grouprank", true) == 0)
    {
        if(!playerInfo[targetId][pGroup])
            return SendClientMessage(playerid, COLOR_GREY, "This player is not part of a group.");

        playerInfo[targetId][pGroupRank] = iAmount;

        mysql_format(SQL, returnString, sizeof returnString, "update `users` set `FactionRank` = '%d' where `id` = '%d';", iAmount, playerInfo[targetId][pSQLID]);
        mysql_tquery(SQL, returnString, "", "");
    }

    else if(strcmp(itemName, "respect", true) == 0)
    {
        playerInfo[targetId][pExperience] = 0;
        AddRespectPoints(targetId, iAmount);
    }
    else if(strcmp(itemName, "skin", true) == 0)
    {
        playerInfo[targetId][pSkin] = iAmount;
        SetPlayerSkin(playerid, iAmount);

        mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Skin` = '%d' where `id` = '%d';", iAmount, playerInfo[targetId][pSQLID]);
        mysql_tquery(SQL, returnString, "", "");

        InsertPlayerSkin(playerid, iAmount, 1);
    }
    else if(strcmp(itemName, "hours", true) == 0)
    {
        playerInfo[targetId][pHours] = iAmount;

        mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Hours` = '%d' where `id` = '%d';", iAmount, playerInfo[targetId][pSQLID]);
        mysql_tquery(SQL, returnString, "", "");
    }
    else if(strcmp(itemName, "bankmoney", true) == 0)
    {
        playerInfo[targetId][pAccount] = iAmount;

        mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Bank` = '%d' where `id` = '%d';", iAmount, playerInfo[targetId][pSQLID]);
        mysql_tquery(SQL, returnString, "", "");
    }
    else if(strcmp(itemName, "bankly", true) == 0)
    {
        playerInfo[targetId][pAccountLY] = iAmount;

        mysql_format(SQL, returnString, sizeof returnString, "update `users` set `BankLY` = '%d' where `id` = '%d';", iAmount, playerInfo[targetId][pSQLID]);
        mysql_tquery(SQL, returnString, "", "");
    }
    else if(strcmp(itemName, "pp", true) == 0)
    {
        playerInfo[targetId][pPremiumPoints] = iAmount;

        mysql_format(SQL, returnString, sizeof returnString, "update `users` set `PremiumPoints` = '%d' where `id` = '%d';", iAmount, playerInfo[targetId][pSQLID]);
        mysql_tquery(SQL, returnString, "", "");
    }
    else
        return SendClientMessage(playerid, COLOR_GREY, "Invalid item.");

    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_SERVER, "Admin %s [user: %d] updated %s [user: %d] %s to %d.", GetName(playerid), playerInfo[playerid][pSQLID], GetName(targetId), playerInfo[targetId][pSQLID], itemName, iAmount);
    return true;
}

public OnPlayerRequestSpawn(playerid)
{
    if(!playerStaticVar[playerid][playerLogged])
    {
        SendClientMessage(playerid, COLOR_LIGHTRED, "** This server requires a Login BEFORE spawn (Kicked) **");
        return KickEx(playerid);
    }
    return true;
}

public OnPlayerRequestClass(playerid, classid)
{
    SpawnPlayer(playerid);
    return true;
}

function:whenPlayerDie(playerid, killerid, WEAPON:reason) {
    if(gPlayerUsingLoopingAnim[playerid])
        gPlayerUsingLoopingAnim[playerid] = 0;

    if(playerStaticVar[playerid][examStep])
        StopExam(playerid);

    player_data[playerid][playerDeath] = true;

    TogglePlayerControllable(playerid, false);
    ResetPlayerWeapons(playerid);

    DeadScris[playerid] = CreateDynamic3DTextLabel("{035efc}[dead]", COLOR_WHITE, 0, 0, -20, 20, playerid);
    Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, DeadScris[playerid], E_STREAMER_ATTACH_OFFSET_Z, 0.31);

    if(job_data[playerid][jobDuty] != JOB_TYPE_NONE) 
        CallLocalFunction("StopJobWork", "i", playerid);


    if(killerid != INVALID_PLAYER_ID) {
        onPlayerWarDeath(playerid, killerid, reason);
        OnPaintBallPlayerDeath(playerid, killerid, reason);
    }
    if(killerid != INVALID_PLAYER_ID) {
        if(playerInfo[playerid][pWantedLevel]) {
            if(factionType[playerInfo[killerid][pGroup]] == FACTION_TYPE_POLICE && playerStaticVar[killerid][groupDuty]) {
                if(IsPlayerInRangeOfPlayer(playerid, killerid, 70.0))
                    onCopKillWantedPlayer(playerid, killerid);
            }

            else if(getCopsOnDutyCount()) {
                foreach(new i : streamedPlayers[playerid]) {
                    if(factionType[playerInfo[i][pGroup]] == FACTION_TYPE_POLICE && playerStaticVar[i][groupDuty] && IsPlayerInRangeOfPlayer(playerid, i, 70.0)) {
                        onCopKillWantedPlayer(playerid, i);
                        break;
                    }
                }
            }
        }

        if(killerid != INVALID_PLAYER_ID)
        {

            if(!isPlayerInPaint(playerid) || !isPlayerInWar(playerid) || !isPlayerInWar(killerid))
            {
                if(getPlayerHit(killerid) == playerid && playerInfo[killerid][pGroup] == 11 || factionType[playerInfo[killerid][pGroup]] == FACTION_TYPE_POLICE || factionType[playerInfo[killerid][pGroup]] == GROUP_TYPE_HITMAN || playerInfo[killerid][pJail])
                    return 1;

                va_SendClientMessage(playerid, COLOR_GREY, "Ai fost atacat de %s. Ai 60 de secunde sa folosesti /call 112 pentru a raporta incidentul.", GetName(killerid));

                playerStaticVar[playerid][playerKillId] = killerid;
                playerStaticVar[playerid][playerKillTime] = gettime() + 60;
            }
        }
    }
    else {
        if(playerInfo[playerid][pWantedLevel] && getCopsOnDutyCount()) {
            foreach(new i : streamedPlayers[playerid]) {
                if(factionType[playerInfo[i][pGroup]] == FACTION_TYPE_POLICE && playerStaticVar[i][groupDuty] && IsPlayerInRangeOfPlayer(playerid, i, 70.0)) {
                    onCopKillWantedPlayer(playerid, i);
                    break;
                }
            }
        }
    }

    return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
    Iter_Add(streamedPlayers[forplayerid], playerid);

    if(playerInfo[playerid][pGroup] && factionType[playerInfo[playerid][pGroup]] == FACTION_TYPE_GANG && isPlayerInWar(playerid) && playerInfo[forplayerid][pGroup] && factionType[playerInfo[forplayerid][pGroup]] == FACTION_TYPE_GANG && isPlayerInWar(forplayerid) && getFactionWarID(playerInfo[playerid][pGroup]) == getFactionWarID(playerInfo[forplayerid][pGroup]))
    {
        SetPlayerMarkerForPlayer(playerid, forplayerid, faction_colors[playerInfo[forplayerid][pGroup]]);
        SetPlayerMarkerForPlayer(forplayerid, playerid, faction_colors[playerInfo[playerid][pGroup]]);
    }
    if(playerInfo[playerid][pWantedLevel] && factionType[playerInfo[forplayerid][pGroup]] == FACTION_TYPE_POLICE && playerStaticVar[forplayerid][groupDuty])
    {
        SetPlayerMarkerForPlayer(forplayerid, playerid, 0xf13464FF);
        SetPlayerMarkerForPlayer(playerid, forplayerid, (GetPlayerColor(forplayerid) & 0xFFFFFF00));
    }

    return true;
}

task check_some_things[10000]()
{
    new returnHour, returnMinute, returnSeconds;
    gettime(returnHour, returnMinute, returnSeconds);

    if(returnMinute < 1 && lastGivedPayday != returnHour)
    {
        lastGivedPayday = returnHour;
        va_SendClientMessageToAll(-1, ""COLOR_SV_HEX""SERVER_NAME2" >> {FFFFFF} The time is now %d:00 hours", lastGivedPayday);

        UpdateWeather();
        WhenPlayersGotPayday();
        if(returnHour == 7)
        {
            GiveAllQuests();
        }
        mysql_tquery(SQL, "SELECT `ID` FROM `users` WHERE `PremiumExpire` < UNIX_TIMESTAMP() AND `PremiumAccount` = 1", "checkPremiumExpire", "");
    }

    foreach(new i : Player)
    {
        if(!playerStaticVar[i][playerLogged])
            continue;

        if(playerStaticVar[i][playerKillTime] != 0 && playerStaticVar[i][playerKillTime] < gettime())
        {
            playerStaticVar[i][playerKillId] = 0;
            playerStaticVar[i][playerKillTime] = 0;
        }
        if(kickafk)
        {
            if(playerStaticVar[i][playerSleeping] || playerInfo[i][pAdmin])
                continue;

            if(playerStaticVar[i][afkTime] >= 1800)
            {
                va_SendClientMessageToAll(COLOR_LIGHTRED, "%s was kicked by AdmBot for being AFK without sleep for more than 30 minutes.", GetDBName(i));
                KickEx(i);
            }
        }
  
        mysql_format(SQL, returnString, sizeof returnString, "select `DeliverStatus` from `emails` where `iPlayer` = '%d' and `DeliverStatus` = '0';", playerInfo[i][pSQLID]);
        mysql_tquery(SQL, returnString, "check_for_emails", "i", i);

        update_report_deelay(i);
    }
    return true;
}

stock Calculate(seconds) {
    new string[20], time = seconds, hr, mn, se; string[0] = (EOS);
    if(seconds < 3600) { time = time % 3600; mn = time / 60; time = time % 60; se = time; format(string, sizeof string, "%02d:%02d", mn, se); }
    else { hr = time / 3600; time = time % 3600; mn = time / 60; time = time % 60; se = time; format(string, sizeof string, "%02d:%02d:%02d", hr, mn, se); }
    return string;
}

ptask playerSecondTimer[1000](playerid) {
    new const surfingVehicleDriver = GetVehicleDriver(GetPlayerSurfingVehicleID(playerid));

    if(surfingVehicleDriver != INVALID_PLAYER_ID && playerStaticVar[surfingVehicleDriver][pTOGSurf])
        SlapPlayer(playerid);
    return 1;
}

task one_second_task[1000]()
{
    new timeString[64], returnHour, returnMinute, returnYear, returnMonth, returnDay, returnSeconds;

    foreach(new i : Player)
    {
        if(!playerStaticVar[i][playerLogged])
            continue;

        GetPlayerPos(i, playerInfo[i][pLastPosX], playerInfo[i][pLastPosY], playerInfo[i][pLastPosZ]);
        if(GetPlayerDistanceFromPoint(i, playerStaticVar[i][pLastCoords][0], playerStaticVar[i][pLastCoords][1], playerStaticVar[i][pLastCoords][2]) < 0.5)
            playerStaticVar[i][afkTime] ++;
        else
            playerStaticVar[i][afkTime] = 0;


        if(!playerStaticVar[i][playerAFK])
            playerInfo[i][pPayDay] ++;

        GetPlayerPos(i, playerStaticVar[i][pLastCoords][0], playerStaticVar[i][pLastCoords][1], playerStaticVar[i][pLastCoords][2]);

        if(playerInfo[i][pNMuted] > 0)
        {
            playerInfo[i][pNMuted] --;
            UpdatePVars(i, "NMuted", playerInfo[i][pNMuted]);
            if(playerInfo[i][pNMuted] == 0) 
                SendClientMessage(i, -1, "You may now speak in the newbie chat channel again.");
        }

        if(playerStaticVar[i][afkTime] >= 10 && !playerStaticVar[i][playerAFK])
        {
            playerStaticVar[i][playerAFK] = 1;

        }
        else if(!playerStaticVar[i][afkTime] && playerStaticVar[i][playerAFK])
        {
            playerStaticVar[i][playerAFK] = 0;
        }

        if(GetPlayerMoney(i) != playerInfo[i][pMoney])
            ResetPlayerMoney(i), GivePlayerMoney(i, playerInfo[i][pMoney]);

        getdate(returnYear, returnMonth, returnDay);
        gettime(returnHour, returnMinute, returnSeconds);



        format(timeString, sizeof timeString, "%02d~p~:~w~~h~~h~~h~%02d", returnHour, returnMinute);
        TextDrawSetString(dateTD[0], timeString);
        TextDrawShowForPlayer(i, dateTD[0]);

        format(timeString, sizeof timeString, ":%02d", returnSeconds);
        TextDrawSetString(dateTD[2], timeString);
        TextDrawShowForPlayer(i, dateTD[2]);


        format(timeString, sizeof timeString, "~p~%02d ~w~~h~~h~~h~%s %02d", returnDay, monthName[returnMonth-1], returnYear);
        TextDrawSetString(dateTD[1], timeString);
        TextDrawShowForPlayer(i, dateTD[1]);
    }

    foreach(new i : displayFPS)
    {
        if(!playerInfo[i][pAdmin]) {
            format(returnString, sizeof returnString, "~y~%d", GetPlayerFPS(i)), PlayerTextDrawSetString(i, playerTextdraws[i][PlayerFPS], returnString), PlayerTextDrawShow(i, playerTextdraws[i][PlayerFPS]);
        }
        else if(playerInfo[i][pAdmin] <= 5){
            format(returnString, sizeof returnString, "~y~%d", GetPlayerFPS(i)), PlayerTextDrawSetString(i, playerTextdraws[i][PlayerFPS], returnString), PlayerTextDrawShow(i, playerTextdraws[i][PlayerFPS]);

            format(returnString, sizeof returnString, "REPORTS: ~R~%d~W~~H~ / CHEATERS: ~B~%d", Iter_Count(sendReport), Iter_Count(Cheaters));
            PlayerTextDrawSetString(i, playerTextdraws[i][ReportsTD], returnString);
            PlayerTextDrawShow(i, playerTextdraws[i][ReportsTD]);
        }
        else if(playerInfo[i][pAdmin] > 5){
            format(returnString, sizeof returnString, "~y~%d", GetPlayerFPS(i)), PlayerTextDrawSetString(i, playerTextdraws[i][PlayerFPS], returnString), PlayerTextDrawShow(i, playerTextdraws[i][PlayerFPS]);

            format(returnString, sizeof returnString, "REPORTS: ~R~%d~W~~H~ / CHEATERS: ~B~%d", Iter_Count(sendReport), Iter_Count(Cheaters));
            PlayerTextDrawSetString(i, playerTextdraws[i][ReportsTD], returnString);
            PlayerTextDrawShow(i, playerTextdraws[i][ReportsTD]);

            format(returnString, sizeof returnString, "~r~ticks: %d ~b~queries: %d ~y~anim: %d", GetServerTickRate(), mysql_unprocessed_queries(), GetPlayerAnimationIndex(i));
            PlayerTextDrawSetString(i, playerTextdraws[i][AdminHUD], returnString);
            PlayerTextDrawShow(i, playerTextdraws[i][AdminHUD]); 
        }
    }

    foreach(new i : destroyedATM)
    {
        if(AtmInfo[i][ATMHealth] >= 1 && AtmInfo[i][ATMRepairTime] == -1)
            continue;

        UpdateATMLabel(i);

        if(AtmInfo[i][ATMRepairTime] <= gettime())
        {
            AtmInfo[i][ATMHealth] = 100;
            AtmInfo[i][ATMRepairTime] = -1;

            DestroyDynamicObject(AtmInfo[i][rObjectId][0]);
            AtmInfo[i][rObjectId][0] = CreateDynamicObject(19324, AtmInfo[i][fATMPos][0], AtmInfo[i][fATMPos][1], AtmInfo[i][fATMPos][2], AtmInfo[i][fATMPosRot][0], AtmInfo[i][fATMPosRot][1], AtmInfo[i][fATMPosRot][2], -1, -1, -1, 500.0);

            foreach(new playerid : Player)
                Streamer_Update(playerid);

            Iter_Remove(destroyedATM, i);
            UpdateATMLabel(i);
            break;
        }
    }
    return true;
}



static randomMSG[][] =
{
    {"Recomandam vizitarea forumului ("SERVER_NAME"/forum) pentru a fi la curent cu toate modificarile aduse."},
    {"Puteti pune intrebari despre server pe /n."},
    {"Foloseste /locations pentru a afla locatiile de pe server."},
    {"Foloseste /jobs pentru a vedea job-urile disponibile."},
    {"Pentru a afla detalii despre contul premium foloseste /premium."},
    {"Daca ai probleme sau nelamuriri foloseste /report."},
    {"Foloseste /carradio pentru a asculta radio in masina."},
    {"Daca ai 10/10 Rob Points poti jefui banca(/rob)."},
    {"Afacerile cu bani reali sunt interzise. Cei ce-s prinsi ca incearca sa faca afaceri cu bani reali vor primi ban permanent."},
    {"Pentru a vedea teritoriile mafiilor foloseste /turfs."}
};

task update_server_informations[600000]()
{
    for(new i; i < MAX_VEHICLES; i++)
    {
        if(VehicleSQL[i])
            continue;

        if(!IsVehicleOccupied(i))
        {
            SetVehicleToRespawn(i);
        }
    }

    SendClientMessageToAll(COLOR_GREEN, randomMSG[random(sizeof randomMSG)]);
    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_YELLOW, "There are %d pending questions and %d on-duty helpers.", Iter_Count(helperQuestions) - Iter_Count(helperAQuestions), Iter_Count(helpersOnDuty));
    foreach(new i : Player)
    {
        if(!playerStaticVar[i][playerLogged])
            continue;
    
        savePlayerInfo(i);

        #if TEST_SERVER
            print("Saving progress [10 minutes].");
        #endif
    }
    return true;
}
