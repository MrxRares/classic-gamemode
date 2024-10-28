// ------------------------------------- //
//             Includes                  //
// ------------------------------------- //

#include <YSI_Coding\y_hooks>

// ------------------------------------- //
//             Constants                 //
// ------------------------------------- //

#define ORE_MODEL_ID 868
#define INDEX_MINER_BACKPACK            7
#define INDEX_MINER_HAMMER              8 
#define MINING_RADIUS                   3.0 

// ------------------------------------- //
//             Enums                     //
// ------------------------------------- //

enum oresType {
    ORE_TYPE_NONE,
    ORE_TYPE_RUBY = 0,
    ORE_TYPE_BRONZE = 1,
    ORE_TYPE_AMETHYST = 2
}


// ------------------------------------- //
//             Variables                 //
// ------------------------------------- //

static Float:rocks[][7] = {
    {270.0772, 2949.1191, 11.3901, -4338.9448, 14.2200, 150.0601, ORE_TYPE_RUBY},
    {267.7389, 2968.0522, 9.9261, -1.2000, -6.9600, 55.8600, ORE_TYPE_AMETHYST},
    {257.1735, 2968.6655, 12.3220, -137.5198, 168.2407, 55.8600, ORE_TYPE_RUBY},
    {257.2993, 2991.5728, 10.1939, -1.4400, -6.9600, 55.8600, ORE_TYPE_BRONZE},
    {271.2578, 2991.4995, 12.4546, -32.1600, -10.6800, 48.1200, ORE_TYPE_AMETHYST},
    {258.8506, 3001.0215, 10.7925, 7.5600, 9.1800, 55.8600, ORE_TYPE_BRONZE},
    {268.8780, 3001.8071, 11.4211,  -3.5400, -7.9200, 55.8600, ORE_TYPE_AMETHYST},
    {263.2155, 3011.3503, 10.0999, -3.5400, -7.9800, 55.8600, ORE_TYPE_BRONZE},
    {255.7587, 3008.1323, 12.8605, 41.8800, 27.8400, 55.8600, ORE_TYPE_AMETHYST}
};

new oreObjects[sizeof(rocks)],
    textLabels[sizeof(rocks)],
    oreStockLabel[sizeof(rocks)],
    oreStock[sizeof(rocks)] = 0;


// ------------------------------------- //
//             Functions                 //
// ------------------------------------- //

resetOres() {

    for (new i = 0; i < sizeof(rocks); i++) {
        if(oreStock[i] == 0){
            new color, labelText[32];

            switch(rocks[i][6])
            {
                case ORE_TYPE_RUBY: {
                    color = COLOR_RUBY;
                    strmid(labelText, "Ruby", 0, sizeof(labelText));
                }
                case ORE_TYPE_BRONZE:{
                    color = COLOR_BRONZE;
                    strmid(labelText, "Bronze", 0, sizeof(labelText));
                }
                case ORE_TYPE_AMETHYST:{
                    color = COLOR_AMTH;
                    strmid(labelText, "Amethyst", 0, sizeof(labelText));
                }
            }
            if(IsValidObject(oreObjects[i]))
                DestroyDynamicObject(oreObjects[i]);

            if(IsValidDynamic3DTextLabel(textLabels[i]))
                DestroyDynamic3DTextLabel(textLabels[i]);

            if(IsValidDynamic3DTextLabel(oreStockLabel[i]))
                DestroyDynamic3DTextLabel(oreStockLabel[i]);
        
            oreStock[i] = 10; 
            format(returnString, sizeof returnString, "%d/10", oreStock[i]);
            oreStockLabel[i] = CreateDynamic3DTextLabel(returnString, color, rocks[i][0], rocks[i][1], rocks[i][2] + 1.0, 6.0);

            oreObjects[i] = CreateDynamicObject(ORE_MODEL_ID, rocks[i][0], rocks[i][1], rocks[i][2],rocks[i][3], rocks[i][4], rocks[i][5]);

            textLabels[i] = CreateDynamic3DTextLabel(labelText, color, rocks[i][0], rocks[i][1], rocks[i][2], 6.0);

        }
    }
    return true;
}
IsPlayerNearOre(playerid, &nearestOre) {
    new playerX, playerY, playerZ;
    GetPlayerPos(playerid, playerX, playerY, playerZ);

    for (new i = 0; i < sizeof(rocks); i++) {
        if (oreStock[i] > 0) {
            if(IsPlayerInRangeOfPoint(playerid, 2.0, rocks[i][0], rocks[i][1], rocks[i][2])) {
                nearestOre = i;
                return true;
            }
        }
    }
    return false;
}
oreCategory(oresType:category)
{
    new cString[55];
    switch(category)
    {
        case ORE_TYPE_RUBY: cString = "{e0115f}ruby{ffffff}";
        case ORE_TYPE_BRONZE: cString = "{CD7F32}bronze{ffffff}";
        case ORE_TYPE_AMETHYST: cString = "{9966cc}amethyst{ffffff}";
    }
    return cString;
}
returnOreMoney(oresType:category, playerid)
{
    new cString;
    switch(category)
    {
        case ORE_TYPE_RUBY: cString = RandomEx(50_000, 135_000) * GetPlayerJobSkill(playerid, JOB_TYPE_MINER);
        case ORE_TYPE_BRONZE: cString = RandomEx(10_000, 40_000) * GetPlayerJobSkill(playerid, JOB_TYPE_MINER);
        case ORE_TYPE_AMETHYST: cString = RandomEx(200_000, 800_000) * GetPlayerJobSkill(playerid, JOB_TYPE_MINER);
    }
    return cString;
}
returnOreTime(oresType:category)
{
    new cString;
    switch(category)
    {

        case ORE_TYPE_RUBY: cString = 4000 * 1;
        case ORE_TYPE_BRONZE: cString = 4000 * ORE_TYPE_BRONZE;
        case ORE_TYPE_AMETHYST: cString = 4000 * ORE_TYPE_AMETHYST;
    }
    return cString;
}
oreColors(oresType:category)
{
    new cString;
    switch(category)
    {
        case ORE_TYPE_RUBY: cString = COLOR_RUBY;
        case ORE_TYPE_BRONZE: cString = COLOR_AMTH;
        case ORE_TYPE_AMETHYST: cString = COLOR_BRONZE;
    }
    return cString;
}

function:loadMinerMap() {
    CreateDynamicObject(899, 236.71986, 2890.67651, 12.55793,   -173.60002, 0.00000, -27.40001);
    CreateDynamicObject(899, 279.66611, 2923.31567, 10.97547,   13.00000, -1.60000, 108.89988);
    CreateDynamicObject(899, 234.97769, 2892.57959, 18.05059,   -173.60002, 0.00000, -27.40001);
    CreateDynamicObject(19329, 259.67139, 2911.60864, 10.91827,   0.00000, 0.00000, -36.20002);
    CreateDynamicObject(18762, 259.43304, 2911.23560, 8.97827,   0.00000, 90.00000, 0.00000);
    CreateDynamicObject(19980, 259.59296, 2911.60620, 8.31826,   0.00000, 0.00000, 145.29996);
    CreateDynamicObject(18228, 280.48245, 2931.68115, 40.42403,   27.39995, 0.00000, 0.00000);
    CreateDynamicObject(18762, 254.46315, 2911.23560, 9.01827,   0.00000, 90.00000, 0.00000);
    CreateDynamicObject(337, 259.08612, 2917.11768, 10.23828,   0.00000, 90.00000, 0.00000);
    CreateDynamicObject(337, 259.17615, 2917.63818, 10.23828,   0.00000, 90.00000, 0.00000);
    CreateDynamicObject(899, 241.37231, 2905.11060, 14.04851,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(3066, 254.55571, 2914.36890, 9.32894,   0.00000, 0.00000, 80.20004);
    CreateDynamicObject(337, 259.30624, 2918.18872, 10.23828,   0.00000, 90.00000, 0.00000);
    CreateDynamicObject(899, 241.57756, 2906.72583, 18.35560,   -173.60002, 0.00000, 0.00000);
    CreateDynamicObject(3862, 258.79337, 2918.37183, 10.49827,   0.00000, 0.00000, 75.20001);
    CreateDynamicObject(337, 259.42627, 2918.89941, 10.21828,   0.00000, 90.00000, 0.00000);
    CreateDynamicObject(19631, 259.82285, 2920.13037, 10.12828,   0.00000, 90.00000, 90.00000);
    CreateDynamicObject(19631, 259.94284, 2920.56079, 10.12828,   0.00000, 90.00000, 90.00000);
    CreateDynamicObject(19631, 260.26288, 2921.11133, 10.12828,   0.00000, 90.00000, 90.00000);
    CreateDynamicObject(19631, 260.30289, 2921.71191, 10.12828,   0.00000, 90.00000, 90.00000);
    CreateDynamicObject(3862, 259.59024, 2921.38965, 10.49827,   0.00000, 0.00000, 75.20001);
    CreateDynamicObject(8661, 260.32236, 2924.39380, 9.48827,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(899, 285.59000, 2942.47534, 11.84953,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(17029, 249.97578, 2929.00854, 30.84988,   -70.99995, 0.00000, 0.00000);
    CreateDynamicObject(1431, 267.12460, 2937.63013, 10.10364,   0.00000, 0.00000, 105.79994);
    CreateDynamicObject(10984, 271.51422, 2950.14087, 10.64894,   0.50000, -12.50000, 4.90000);
    CreateDynamicObject(1307, 248.89561, 2964.64063, 17.17149,   0.00000, 180.00000, 0.00000);
    CreateDynamicObject(6233, 260.88736, 2977.16357, 16.90548,   180.00000, 3.80000, 0.00000);
    CreateDynamicObject(6233, 260.88290, 2977.85425, 13.09421,   0.00000, 7.60000, 0.00000);
    CreateDynamicObject(10984, 257.25058, 2981.15918, 9.84879,   0.00000, 16.99999, -1.70000);
    CreateDynamicObject(10984, 257.90094, 3003.08130, 9.84879,   0.00000, 16.99999, -1.70000);
    CreateDynamicObject(19464, 274.99139, 3013.68579, 13.85281,   0.00000, 0.00000, 90.00000);
    CreateDynamicObject(19464, 269.18747, 3013.30054, 11.67280,   0.00000, 0.00000, 97.99995);
    CreateDynamicObject(10984, 268.82709, 3014.16040, 11.26183,   82.39998, 0.00000, 0.00000);
    CreateDynamicObject(19464, 262.38153, 3014.16895, 11.67280,   0.00000, 0.00000, 90.00000);
    CreateDynamicObject(19464, 259.81183, 3014.19653, 11.58278,   0.00000, 0.00000, 90.00000);
    CreateDynamicObject(19464, 253.89180, 3014.17651, 13.85281,   0.00000, 0.00000, 90.00000);
    CreateDynamicObject(10984, 260.22739, 3015.62964, 10.41585,   89.99992, 3.30000, -3.80000);
    CreateDynamicObject(11533, 247.89743, 3029.00879, 10.65329,   -4.20000, -23.90001, 90.00000);
    CreateDynamicObject(11694, 233.12906, 3032.26416, -21.71289,   16.79999, 21.99998, 135.19995);
    CreateDynamicObject(956, -253.74001, 2599.75000, 62.24000,   0.00000, 0.00000, 90.00000);
    CreateDynamicObject(955, -253.74001, 2597.94995, 62.24000,   0.00000, 0.00000, 90.00000);
    CreateDynamicObject(1408, 254.04373, 2848.99414, 22.36481,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(8229, 256.71555, 2849.09937, 21.13285,   0.00000, 0.00000, 180.00000);
    CreateDynamicObject(1408, 259.48358, 2848.99414, 22.36481,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1408, 264.93359, 2848.99414, 22.36481,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1408, 273.19330, 2848.99414, 22.36481,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(1408, 278.63315, 2848.99414, 22.36481,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(6916, 261.24796, 2867.55640, 15.26390,   0.00000, 0.00000, 720.00000);
    CreateDynamicObject(8229, 281.43555, 2849.09937, 21.13285,   0.00000, 0.00000, 180.00000);
    CreateDynamicObject(3498, 250.52066, 2883.61987, 11.82949,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(3498, 250.46118, 2887.94214, 16.12948,   0.00000, 90.00000, 90.00000);
    CreateDynamicObject(3498, 250.46118, 2887.94214, 11.99947,   0.00000, 90.00000, 90.00000);
    CreateDynamicObject(3498, 250.48068, 2892.62646, 12.86950,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(3498, 253.60124, 2892.68652, 16.10950,   0.00000, 90.00000, 0.00000);
    CreateDynamicObject(3498, 255.18118, 2892.68652, 11.97947,   0.00000, 90.00000, 0.00000);
    CreateDynamicObject(3498, 256.78058, 2892.62646, 12.86950,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(19450, 256.00815, 2893.64526, 10.98105,   0.00000, 0.00000, 90.00000);
    CreateDynamicObject(3498, 259.56046, 2892.64624, 11.33950,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(3498, 260.59091, 2892.71655, 16.10950,   0.00000, 90.00000, 0.00000);
    CreateDynamicObject(1479, 259.89499, 2897.18579, 10.72668,   0.00000, 180.00000, 90.00000);
    CreateDynamicObject(3498, 265.96039, 2892.62646, 12.86950,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(19975, 264.95724, 2892.93384, 8.48517,   0.00000, 0.00000, -31.70000);
    CreateDynamicObject(3406, 261.82654, 2897.81836, 13.78593,   90.00000, 270.00000, 0.00000);
    CreateDynamicObject(899, 255.24963, 2904.41748, 19.45893,   -173.60002, 0.00000, -3.80000);
    CreateDynamicObject(19464, 263.05563, 2896.92871, 9.36309,   0.00000, 90.00000, 180.00000);
    CreateDynamicObject(3498, 269.59064, 2892.71655, 16.10950,   0.00000, 90.00000, 0.00000);
    CreateDynamicObject(3498, 270.42111, 2892.68652, 11.97947,   0.00000, 90.00000, 0.00000);
    CreateDynamicObject(19450, 270.23807, 2893.60522, 10.98105,   0.00000, 0.00000, 90.00000);
    CreateDynamicObject(1479, 266.48486, 2897.17603, 10.72668,   0.00000, 180.00000, 270.00000);
    CreateDynamicObject(1479, 259.89499, 2903.88110, 10.72668,   0.00000, 180.00000, 90.00000);
    CreateDynamicObject(3498, 273.29044, 2892.71655, 12.86950,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(8661, 260.32236, 2904.44092, 9.48827,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(19464, 263.05563, 2902.84326, 9.36309,   0.00000, 90.00000, 180.00000);
    CreateDynamicObject(3406, 261.82654, 2905.95557, 13.78593,   90.00000, 270.00000, 0.00000);
    CreateDynamicObject(1479, 266.48486, 2903.87109, 10.72668,   0.00000, 180.00000, 270.00000);
    CreateDynamicObject(18980, 278.79465, 2892.81470, 9.09628,   90.00000, 0.00000, 90.00000);
    CreateDynamicObject(899, 268.41437, 2904.34058, 19.71927,   -173.60002, 0.00000, -3.80000);
    CreateDynamicObject(19464, 263.05563, 2908.75952, 9.36309,   0.00000, 90.00000, 180.00000);
    CreateDynamicObject(17859, 266.98398, 2906.54346, 11.72389,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(18762, 264.39307, 2911.23560, 8.98827,   0.00000, 90.00000, 0.00000);
    CreateDynamicObject(899, 281.64511, 2922.32373, 17.68741,   13.00000, -1.60000, 108.89988);
    return true;
}


function:CreateRocks() {
    for (new i = 0; i < sizeof(rocks); i++) {

        oreObjects[i] = CreateDynamicObject(ORE_MODEL_ID, rocks[i][0], rocks[i][1], rocks[i][2],rocks[i][3], rocks[i][4], rocks[i][5]);

        new color, labelText[32];

        switch(rocks[i][6])
        {
            case ORE_TYPE_RUBY: {
                color = COLOR_RUBY;
                strmid(labelText, "Ruby", 0, sizeof(labelText));
            }
            case ORE_TYPE_BRONZE:{
                color = COLOR_BRONZE;
                strmid(labelText, "Bronze", 0, sizeof(labelText));
            }
            case ORE_TYPE_AMETHYST:{
                color = COLOR_AMTH;
                strmid(labelText, "Amethyst", 0, sizeof(labelText));
            }
        }

        textLabels[i] = CreateDynamic3DTextLabel(labelText, color, rocks[i][0], rocks[i][1], rocks[i][2], 6.0);
        SetDynamicObjectMaterial(oreObjects[i], 0, -1, "", "", oreColors(rocks[i][6]));
        if(oreStock[i] == 0){
        
            oreStock[i] = 10; 
            format(returnString, sizeof returnString, "%d/10", oreStock[i]);
            oreStockLabel[i] = CreateDynamic3DTextLabel(returnString, color, rocks[i][0], rocks[i][1], rocks[i][2] + 1.0, 6.0);
        }
    }
    CreateDynamic3DTextLabel(""COLOR_SV_HEX"[MINE INFO]\n{ffffff}Use "COLOR_SV_HEX"/mineinfo{ffffff}", -1, 260.7942,2917.8037,10.4883, 20.0);
    CreateDynamicPickup(1239, 23, 260.7942,2917.8037,10.4883);

    CreateDynamic3DTextLabel(""COLOR_SV_HEX"[MINE SHOP]\n{ffffff}Use "COLOR_SV_HEX"/mineshop{ffffff}", -1, 261.6718,2920.8494,10.4883, 20.0);
    CreateDynamicPickup(1239, 23, 261.6718,2920.8494,10.4883);


    return true;
}

// ------------------------------------- //
//             Commands                  //
// ------------------------------------- //
YCMD:mineinfo(playerid, params[], help) {
    if(!IsPlayerInRangeOfPoint(playerid, 2.0, 260.7942,2917.8037,10.4883))
        return true;

    Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Info:", ""COLOR_SV_HEX"Miner Job:{ffffff}\nPentru a munci la acest job ai nevoie de un 'Sledgehammer'.\nAceasta este un accesoriu "COLOR_SV_HEX"special{ffffff} pe care il poti cumpara din "COLOR_SV_HEX"/mineshop{ffffff}\n\
        \nIn mina se afla "COLOR_SV_HEX"9 pietre{ffffff} pe care le puteti sparge apasand "COLOR_SV_HEX"'Left Click'{ffffff}.\nOdata ce minereurile au fost sparte, aceastea se vor reseta in maxim 10 minute.\n\
        Fiecare minereu valoreaza intre {00af0d}$10.000 - $800.000{ffffff} in functie de raritate.\nIn functie de skill-ul tau la job-ul miner "COLOR_SV_HEX"[/skills]{ffffff} vei primi mai multi bani.", "Close", "");
    return true;
}

YCMD:mineshop(playerid, params[], help) {
    if(!IsPlayerInRangeOfPoint(playerid, 2.0, 261.6718,2920.8494,10.4883))
        return true;

    new captionFormat[32];
    format(captionFormat, sizeof captionFormat, "Mining Points: "COLOR_SV_HEX"%d", playerInfo[playerid][pMiningPoints]);

    Dialog_Show(playerid, DIALOG_MINESHOP, DIALOG_STYLE_TABLIST_HEADERS, captionFormat, "Item\t\tprice\t\tStock\t\n\
        SledgeHammer\t\t{00af0d}$200.000{ffffff}\t\tEnough\n\
        Case Key\t\t"COLOR_SV_HEX"20 mining points{ffffff}\t\tEnough\n\
        Case\t\t"COLOR_SV_HEX"80 mining points{ffffff}\t\tEnough\n\
        ", "Buy", "Exit");

    return true;
}
YCMD:gotominer(playerid, params[], help)
{
    check_admin

    SetPlayerPos(playerid, 267.4914,2839.2295,22.0801);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerInterior(playerid, 0);

    SendClientMessage(playerid, -1, "You have been teleported.");
    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s has used /gotominer", GetName(playerid));
    return true;
}

Dialog:DIALOG_MINESHOP(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    switch(listitem)
    {
        case 0: {
            new hammermoney = 200000;
            if(GetMoney(playerid) < hammermoney && GetMilliards(playerid) < 1)
                return sendError(playerid, "Nu ai banii necesari in mana.");

            SendClientMessage(playerid, COLOR_SERVER, ">> {ffffff}Search for the 'Sledgehammer' in the shown menu.");
            //SetPVarInt(playerid, "objectID", 1);
           // show_accs_tds(playerid);
            return true;
        }
        case 1: {
            if(playerInfo[playerid][pMiningPoints] < 20)
                return sendError(playerid, "You don't have enough mining points.");

            SendClientMessage(playerid, COLOR_SERVER, ">> {ffffff}You bought 1 'Case key' for 20 mining points.");

            playerInfo[playerid][pCasesKeys] += 1;
            UpdatePVars(playerid, "CaseKeys", playerInfo[playerid][pCasesKeys]);

            return true;
        }
        case 2: {
            if(playerInfo[playerid][pMiningPoints] < 80)
                return sendError(playerid, "You don't have enough mining points.");

            SendClientMessage(playerid, COLOR_SERVER, ">> {ffffff}You bought 1 'Case' for 80 mining points.");

            playerInfo[playerid][pCases] += 1;
            UpdatePVars(playerid, "Case", playerInfo[playerid][pCases]);

            return true;
        }
    }

    return true;
}
// ------------------------------------- //
//             Timers                    //
// ------------------------------------- //


timer minerTimer[4000](playerid, nearestOre) {
    if(job_data[playerid][jobDuty] != JOB_TYPE_NONE)
        return true;
    new oreColor;
    TogglePlayerControllableEx(playerid, true); 
    switch (rocks[nearestOre][6])
    {
        case ORE_TYPE_RUBY: {
            oreColor = COLOR_RUBY;
        }
        case ORE_TYPE_BRONZE:{
            oreColor = COLOR_BRONZE;
        }
        case ORE_TYPE_AMETHYST:{
            oreColor = COLOR_AMTH;
        }
    }
    oreStock[nearestOre]--;
    format(returnString, sizeof returnString, "%d/10", oreStock[nearestOre]);
    UpdateDynamic3DTextLabelText(oreStockLabel[nearestOre], oreColor, returnString);

    if (oreStock[nearestOre] <= 0) {
        DestroyDynamicObject(oreObjects[nearestOre]);
        DestroyDynamic3DTextLabel(textLabels[nearestOre]);
        DestroyDynamic3DTextLabel(oreStockLabel[nearestOre]);
        SetPlayerAttachedObject(playerid, INDEX_JOB, 3930, 8,  0.000000, 0.354000, -0.161000,  61.699974, -98.399963, 97.300018,  0.623998, 0.608998, 0.613999);
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
        SetPlayerCheckpointEx(playerid, 260.4450,2913.6108,10.4883, 2.0);
        playerStaticVar[playerid][pCheckpoint] = 20009;
        playerStaticVar[playerid][haveore] = true;
    } 
    else {
        SetPlayerAttachedObject(playerid, INDEX_JOB, 3930, 8,  0.000000, 0.354000, -0.161000,  61.699974, -98.399963, 97.300018,  0.623998, 0.608998, 0.613999);
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
        SetPlayerCheckpointEx(playerid, 260.4450,2913.6108,10.4883, 2.0);
        playerStaticVar[playerid][pCheckpoint] = 20009;
        playerStaticVar[playerid][haveore] = true;
    }
    playerStaticVar[playerid][nearOre] = nearestOre;
    playerStaticVar[playerid][b_ACTIVE_TIMER] = false;
    return true;
}

// ------------------------------------- //
//             Hooks                     //
// ------------------------------------- //


hook WhenPlayerLoggedIn(playerid)
{
    if(!cache_num_rows())
        return Y_HOOKS_CONTINUE_RETURN_1;

    cache_get_value_name_int(0, "MiningPoints", playerInfo[playerid][pMiningPoints]);

    return Y_HOOKS_CONTINUE_RETURN_1;
}


hook OnPlayerEnterCheckpoint(playerid)
{

    switch(playerStaticVar[playerid][pCheckpoint])
    {
        case 20009:
        {
            playerStaticVar[playerid][pCheckpoint] = 0;
            new money, minePoints;
            money = returnOreMoney(rocks[playerStaticVar[playerid][nearOre]][6], playerid);
            minePoints = random(10);
            playerInfo[playerid][pMiningPoints] += minePoints;
            UpdatePVars(playerid, "MiningPoints", playerInfo[playerid][pMiningPoints]);
            SendClientMessage(playerid, COLOR_SERVER, "JOB >> {ffffff}You have crushed a %s and earned {00af0d}$%s{ffffff}.", oreCategory(rocks[playerStaticVar[playerid][nearOre]][6]), FormatNumber(money));
            SendClientMessage(playerid, COLOR_SERVER, ">> {ffffff}Ai primit "COLOR_SV_HEX"%d{ffffff} mine points. (total:"COLOR_SV_HEX"%d{ffffff})", minePoints, playerInfo[playerid][pMiningPoints]);

            PlayerPlaySound(playerid, 1139, 0.0, 0.0, 0.0);
            DisablePlayerCheckpoint(playerid);
            RemovePlayerAttachedObject(playerid, INDEX_JOB);
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
            ApplyAnimation(playerid, "CARRY", "PUTDWN", 4.0, 0, 0, 0, 0, 0);
            playerStaticVar[playerid][haveore] = false;
            playerStaticVar[playerid][nearOre] = -1;
            return UpdatePlayerJobSkill(playerid, JOB_TYPE_MINER);
        }
    }
    return true;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if(newkeys == KEY_SECONDARY_ATTACK)
    {
        if(!IsPlayerInRangeOfPoint(playerid, 2.0, 445.9095,-850.7404,29.8050))
            return true;

        SetPlayerPos(playerid, 440.0024,-861.1229,2.7160);
    }

    if(newkeys == KEY_FIRE) {
        new nearestOre;
        if (!IsPlayerNearOre(playerid, nearestOre))
            return true;

        if(getPlayerFactionWar(playerid) != -1)
            return true;

        if(IsACop(playerid) && isPlayerDuty(playerid))
            return true;

        if(playerInfo[playerid][pWantedLevel] || Iter_Contains(jailedPlayers, playerid))
            return true;

        // if(!player_has_model_equipped(playerid, 19631))
        //     return sendError(playerid, "You need to equip special accessory from [/myaccs] to continue.");

        if(playerStaticVar[playerid][b_ACTIVE_TIMER] == true)
            return true;

        if(playerStaticVar[playerid][haveore] == true) 
            return true;
        new oreTime = returnOreTime(rocks[nearestOre][6]);
        if (!IsPlayerNearOre(playerid, nearestOre))
            return true;

        TogglePlayerControllableEx(playerid, false);
        ClearAnimations(playerid);
        ApplyAnimation(playerid, "BASEBALL", "Bat_4", 4.1, 1, 0, 0, 0, 0);
        GameTextForPlayer(playerid, "~W~MINING...", oreTime, 3);
        defer minerTimer[oreTime](playerid, nearestOre);
        playerStaticVar[playerid][b_ACTIVE_TIMER] = true;
    }
    return true;
}
