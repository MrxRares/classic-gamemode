#include <YSI_Coding\y_hooks>

#define CP_BUS (CP_JOBS_BEGIN) + JOB_TYPE_BUS
#define BUS_MONEY (2500)

new TotalBusPassengers[MAX_PLAYERS];

new Float: bus_stations[][] =
{
    {-2544.05688, 560.03973, 14.86740},
    {-2292.98438, 560.25891, 35.07030},
    {-2249.87549, 691.54950, 49.69960},
    {-2249.87280, 891.17133, 66.89430},
    {-2249.74463, 1037.93225, 84.0990},
    {-2438.12769, 1195.50635, 35.4445},
    {-2754.90942, 1004.68701, 54.6462},
    {-2754.99243, 884.06860, 66.59570},
    {-2755.15259, 707.34912, 41.52480},
    {-2707.04443, 559.08368, 14.96280},

    {-2587.0198, 605.5298, 14.4531}
};

hook OnGameModeInit()
{
    CreateDynamicObject(1257, 1692.89929, -2200.77808, 13.64870,   0.00000, 0.00000, -90.12000);
    CreateDynamicObject(1257, 1941.19080, -2172.63159, 13.76920,   0.00000, 0.00000, -90.36000);
    CreateDynamicObject(1257, 1967.80859, -1900.26184, 13.77870,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1257, 1921.87402, -1746.02295, 13.76840,   0.00000, 0.00000, 90.06000);
    CreateDynamicObject(1257, 1815.46008, -1815.05981, 13.80990,   0.00000, 0.00000, -179.63989);
    CreateDynamicObject(1257, 1695.64038, -1766.75049, 13.78830,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1257, 1392.64221, -1726.04602, 13.65910,   0.00000, 0.00000, 89.46000);
    CreateDynamicObject(1257, 1318.69714, -1633.10010, 13.76780,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1257, 1325.55969, -1389.08057, 13.76010,   0.00000, 0.00000, 90.24000);
    CreateDynamicObject(1257, 1212.34741, -1326.52954, 13.68430,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1257, 1083.02100, -1274.54492, 13.60720,   0.00000, 0.00000, 90.24000);
    CreateDynamicObject(1257, 964.63733, -1214.54919, 17.02340,   0.00000, 0.00000, 89.58000);
    CreateDynamicObject(1257, 818.55078, -1134.44849, 23.90680,   0.00000, 0.00000, 88.50000);
    CreateDynamicObject(1257, 791.28119, -1301.22681, 13.56870,   0.00000, 0.00000, -179.51990);
    CreateDynamicObject(1257, 679.34332, -1388.95630, 13.72830,   0.00000, 0.00000, 90.12000);
    CreateDynamicObject(1257, 378.06552, -1489.50562, 32.17630,   0.00000, 0.00000, 128.28003);
    CreateDynamicObject(1257, 261.02219, -1539.33704, 32.41950,   0.00000, 0.00000, -34.38000);
    CreateDynamicObject(1257, 485.92880, -1361.02576, 18.19000,   -4.00000, 4.00000, -65.24000);
    CreateDynamicObject(1257, 589.19830, -1236.84290, 17.97730,   0.00000, 0.00000, -67.92000);
    CreateDynamicObject(1257, 775.53870, -1062.00525, 24.75130,   0.00000, 0.00000, -77.69999);
    CreateDynamicObject(1257, 1209.70325, -952.07233, 43.01140,   0.00000, 0.00000, -79.26000);
    CreateDynamicObject(1257, 1713.93640, -736.80762, 50.49150,   -2.94000, 0.00000, -2.00000);
    CreateDynamicObject(1257, 1775.73401, 579.77661, 23.77970,   0.00000, 0.00000, -11.76000);
    CreateDynamicObject(1257, 1839.89514, 825.05157, 10.16840,   0.06000, 0.00000, 270.66010);
    CreateDynamicObject(1257, 2184.53564, 801.95203, 7.33030,   0.00000, 0.00000, -125.22000);
    CreateDynamicObject(1257, 2293.69141, 841.77661, 14.11820,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1257, 2353.69604, 1025.47546, 10.84100,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1257, 2570.79736, 1066.87720, 10.84120,   0.00000, 0.00000, -90.24001);
    CreateDynamicObject(1257, 2613.71777, 1319.62097, 10.86020,   0.00000, 0.00000, 90.36000);
    CreateDynamicObject(1257, 2476.20508, 1479.59375, 10.86030,   0.00000, 0.00000, 89.94000);
    CreateDynamicObject(1257, 2401.10645, 1619.55835, 10.89970,   0.00000, 0.00000, 89.88000);
    CreateDynamicObject(1257, 2293.08325, 1779.52710, 10.80020,   0.00000, 0.00000, 89.70000);
    CreateDynamicObject(1257, 2158.93481, 1995.43042, 10.87960,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1257, 2231.25317, 2383.93506, 10.86030,   0.00000, 0.00000, -12.60000);
    CreateDynamicObject(1257, 2085.77344, 2459.35864, 10.88090,   0.00000, 0.00000, 90.00000);
    CreateDynamicObject(1257, 1948.35791, 2399.46997, 10.92060,   0.00000, 0.00000, 89.64000);
    CreateDynamicObject(1257, 1920.80237, 2315.90601, 10.88100,   0.00000, 0.00000, -179.69991);
    CreateDynamicObject(1257, 1921.41638, 2117.40479, 10.94120,   0.00000, 0.00000, -179.70000);
    CreateDynamicObject(1257, 2093.48389, 2015.85571, 10.90140,   0.00000, 0.00000, -90.18000);
    CreateDynamicObject(1257, 2035.99133, 1521.35413, 10.90060,   0.00000, 0.00000, -180.05991);
    CreateDynamicObject(1257, 2036.35254, 1168.23950, 10.82070,   0.00000, 0.00000, -179.75990);
    CreateDynamicObject(1257, 2034.95715, 878.28998, 7.36150,   0.00000, 0.00000, -179.40010);
    CreateDynamicObject(1257, 1752.35156, 621.24127, 21.25890,   4.00000, 0.00000, 165.00000);
    CreateDynamicObject(1257, 1614.87988, 186.35570, 33.65740,   4.00000, 0.00000, -192.00000);
    CreateDynamicObject(1257, 1657.21387, -331.40521, 40.28030,   0.00000, 0.00000, -174.59990);
    CreateDynamicObject(1257, 1684.78406, -727.13708, 49.88370,   4.00000, 0.00000, -190.80000);
    CreateDynamicObject(1257, 1412.48633, -934.33838, 35.80170,   0.00000, 0.00000, 80.46000);
    CreateDynamicObject(1257, 1431.33862, -1040.25464, 23.92800,   0.00000, 0.00000, -89.94000);
    CreateDynamicObject(1257, 1571.46692, -1139.13977, 23.71380,   0.00000, 0.00000, -180.60001);
    CreateDynamicObject(1257, 1708.46350, -1263.24890, 13.60880,   0.00000, 0.00000, -179.16000);
    CreateDynamicObject(1257, 1590.66150, -1434.45093, 13.52840,   0.00000, 0.00000, 90.06000);
    CreateDynamicObject(1257, 1423.55896, -1568.12500, 13.62870,   0.00000, 0.00000, 173.27989);
    CreateDynamicObject(1257, 1446.39136, -1738.46875, 13.68900,   0.00000, 0.00000, -89.58000);
    CreateDynamicObject(1257, 1579.74524, -1878.48059, 13.62880,   0.00000, 0.00000, -90.06000);
    CreateDynamicObject(1257, 1815.37183, -1904.30969, 13.65310,   0.00000, 0.00000, -180.65990);
    CreateDynamicObject(1257, 1955.44678, -1998.47595, 13.60910,   0.00000, 0.00000, -179.52000);
    CreateDynamicObject(1257, 1935.31299, -2160.26001, 13.66810,   0.00000, 0.00000, 89.82000);
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid)
{
    RemoveBuildingForPlayer(playerid, 1257, -2292.9844, 557.2578, 35.0703, 0.25);
    TotalBusPassengers[playerid] = 0;
    return Y_HOOKS_CONTINUE_RETURN_1;
}

start_bus_work(playerid)
{
	CreateJobVehicle(playerid, 437, -2586.3560, 584.0962, 14.1802, 271.8224, 0);
    SetPVarInt(playerid, "bus_cp_step", 0);
   
    SetPVarInt(playerid, "checkpoint_type", CP_BUS);
    set_bus_checkpoint(playerid);

    PlayerTextDrawSetString(playerid, playerTextdraws[playerid][serverJobTD], "total pasageri: 0~n~checkpoint: 0/10");
    PlayerTextDrawShow(playerid, playerTextdraws[playerid][serverJobTD]);

    SendClientMessage(playerid, -1, ""COLOR_SV_HEX"JOB >> {FFFFFF} Urmeaza punctele rosii de pe minimap pentru a fi platit. Vei rula pe ruta 1.");
    return true;
}

set_bus_checkpoint(playerid)
{
    new i = GetPVarInt(playerid, "bus_cp_step");
    if(i < 10)
        SetJobRaceCheckpoint(playerid, CP_TYPE_GROUND_NORMAL, bus_stations[i][0], bus_stations[i][1], bus_stations[i][2], bus_stations[i + 1][0], bus_stations[i + 1][1], bus_stations[i + 1][2], 4.0, 5);
    
    else
        SetJobRaceCheckpoint(playerid, CP_TYPE_GROUND_FINISH, bus_stations[i][0], bus_stations[i][1], bus_stations[i][2], 0.0, 0.0, 0.0, 4.0, 5);
    
    return true;
}

cancel_bus_work(playerid)
{
    if(job_data[playerid][jobDuty] != JOB_TYPE_BUS)
        return true;

    DeletePVar(playerid, "bus_shift_step");
    DeletePVar(playerid, "bus_cp_step");

    TotalBusPassengers[playerid] = 0;
    return PlayerTextDrawHide(playerid, playerTextdraws[playerid][serverJobTD]);
}

hook OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
    if(newstate == PLAYER_STATE_ONFOOT && oldstate == PLAYER_STATE_DRIVER)
    {
        if(job_data[playerid][jobVehicle] == playerStaticVar[playerid][lastVehicle] && job_data[playerid][jobVehicle] && job_data[playerid][jobDuty] == JOB_TYPE_BUS)
        {
            SendClientMessage(playerid, COLOR_GREEN, "(-) Ai platit $2.000 companiei de autobuze pentru taxe.");
            substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 2000);

            StopJobWork(playerid);
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnPlayerEnterRaceCP(playerid)
{
    if(GetPVarInt(playerid, "checkpoint_type") == CP_BUS && job_data[playerid][jobDuty] == JOB_TYPE_BUS && IsPlayerInJobVehicle(playerid))
    {
        if(GetPVarInt(playerid, "bus_cp_step") == 10)
        {
            SendClientMessage(playerid, -1, ""COLOR_SV_HEX"JOB >> {FFFFFF} Traseul a fost finalizat. Pentru a munci in continuare tasteaza comanda /work.");
            SetPlayerPos(playerid, 1653.2095, -2237.8726, 13.5192);
            SetPVarInt(playerid, "bus_cp_step", 0);
            StopJobWork(playerid);
        }
        else
        {
            GameTextForPlayer(playerid, "~g~waiting for passengers ....", 2000, 3);

            SetTimerEx("BusDriverCheck", 3000, false, "i", playerid);
            TogglePlayerControllable(playerid, false);
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

function:BusDriverCheck(playerid)
{
    new job_money = (BUS_MONEY) + (GetPlayerJobSkill(playerid, JOB_TYPE_BUS) * 700 + RandomEx((GetPlayerJobSkill(playerid, JOB_TYPE_BUS) * 800), (GetPlayerJobSkill(playerid, JOB_TYPE_BUS) * 500) + 500)),
        pasagers = 4 + random(6),
        downPasagers = 2 + random(2);

    if(!TotalBusPassengers[playerid])
        downPasagers = 0;
    
    TotalBusPassengers[playerid] += pasagers - downPasagers;

    if(!downPasagers)
        va_SendClientMessage(playerid, -1, ""COLOR_SV_HEX"JOB >> {FFFFFF} Au urcat %d pasageri in autobuz (total %d). Ai primit $%s.", pasagers, TotalBusPassengers[playerid], FormatNumber(job_money));

    else
        va_SendClientMessage(playerid, -1, ""COLOR_SV_HEX"JOB >> {FFFFFF} Au urcat %d pasageri in autobuz si au coborat %d (total %d). Ai primit $%s.", pasagers, downPasagers, TotalBusPassengers[playerid], FormatNumber(job_money));
    
    addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, job_money, false);

    DisablePlayerRaceCheckpoint(playerid);
    TogglePlayerControllable(playerid, 1);

    SetPVarInt(playerid, "bus_cp_step", GetPVarInt(playerid, "bus_cp_step") + 1);
    SetPVarInt(playerid, "bus_shift_step", GetPVarInt(playerid, "bus_shift_step") + 1);
    AddJobClanMoney(playerid, job_money);

    va_PlayerTextDrawSetString(playerid, playerTextdraws[playerid][serverJobTD], "total pasageri: %d~n~checkpoint: %d/10", TotalBusPassengers[playerid], GetPVarInt(playerid, "bus_cp_step"));
    PlayerTextDrawShow(playerid, playerTextdraws[playerid][serverJobTD]);

    if(GetPVarInt(playerid, "bus_shift_step") >= 10)
    {
        DeletePVar(playerid, "bus_shift_step");
        UpdatePlayerJobSkill(playerid, JOB_TYPE_BUS);
    }
    set_bus_checkpoint(playerid);
    return true;
}