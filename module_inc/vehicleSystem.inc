#include <YSI_Coding\y_hooks>
#define VEH_TYPE_NONE 1
#define VEH_TYPE_GROUP 2
#define VEH_TYPE_CLAN 3
#define VEH_TYPE_ADMIN 4
#define VEH_TYPE_JOB 5
#define VEH_TYPE_PERSONAL 6

#define MAX_VEH_TYPE 7

#define GetVehicleName(%0) aVehicleNames[%0 - 400]

new Iterator: vehicleType<MAX_VEH_TYPE, MAX_VEHICLES>;
new entercars[MAX_PLAYERS]=0;
enum ENUM_VEHICLE_INFO
{
	vehID, vehEngine, vehFuel, vehType, bool:vehRentable, vehLights, vehLocked,
    vehModel, vehGroup, vehRank, objID
};
new vehicleInfo[MAX_VEHICLES][ENUM_VEHICLE_INFO], serverVehicleId[MAX_VEHICLES],
    radioListeners[20], vehicleRadio[MAX_VEHICLES];
static stock returnVehicle;


IsPlane(carid)
{
    if(GetVehicleModel(carid) == 417 || GetVehicleModel(carid) == 425 || GetVehicleModel(carid) == 447 || GetVehicleModel(carid) == 460 || GetVehicleModel(carid) == 464 || GetVehicleModel(carid) == 465 || GetVehicleModel(carid) == 469 || GetVehicleModel(carid) == 476 || GetVehicleModel(carid) == 487 || GetVehicleModel(carid) == 488 || GetVehicleModel(carid) == 497 || GetVehicleModel(carid) == 501 || GetVehicleModel(carid) == 511 || GetVehicleModel(carid) == 512 || GetVehicleModel(carid) == 513 || GetVehicleModel(carid) == 519 || GetVehicleModel(carid) == 520 || GetVehicleModel(carid) == 548 || GetVehicleModel(carid) == 553 || GetVehicleModel(carid) == 563 || GetVehicleModel(carid) == 577 || GetVehicleModel(carid) == 592 || GetVehicleModel(carid) == 593)
        return true;

    return false;
}

IsBike(carid)
{
    if(GetVehicleModel(carid) == 481 || GetVehicleModel(carid) == 509 || GetVehicleModel(carid) == 510)
        return true;

    return false;
}

IsBoat(carid)
{
    if(GetVehicleModel(carid) == 430 || GetVehicleModel(carid) == 446 || GetVehicleModel(carid) == 452 || GetVehicleModel(carid) == 453 || GetVehicleModel(carid) == 454 || GetVehicleModel(carid) == 472 || GetVehicleModel(carid) == 473 || GetVehicleModel(carid) == 484 || GetVehicleModel(carid) == 493 || GetVehicleModel(carid) == 595)
        return true;

    return false;
}

stock GetClosestVehicle(playerid, Float: lastd = 200.0, vehicle = -1, Float: x =0.0, Float: y = 0.0, Float: z = 0.0, Float: dist = 0.0)
{
    GetPlayerPos(playerid, x, y, z);
    foreach(new i : streamedVehicles[playerid])
    {
        if(GetPlayerPos(playerid, x, y, z) && (dist = GetVehicleDistanceFromPoint(i, x, y, z)) < lastd)
            lastd = dist, vehicle = i;
    }
    return vehicle;
}

forward stock _AddStaticVehicleEx(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, color1, color2, respawn_delay, addsiren = 0, vehtype = VEH_TYPE_NONE);
stock _AddStaticVehicleEx(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, color1, color2, respawn_delay, addsiren = 0, vehtype = VEH_TYPE_NONE)
{
    if(INVALID_VEHICLE_ID != (returnVehicle = AddStaticVehicleEx(modelid, spawn_x, spawn_y, spawn_z, z_angle, color1, color2, respawn_delay, bool:addsiren)))
    {
        Iter_Add(vehicleType<vehtype>, returnVehicle);
        vehicleInfo[returnVehicle][vehType] = vehtype;

        vehicleInfo[returnVehicle][vehEngine] = 0;

        if(IsBike(returnVehicle))
        {
            vehicleInfo[returnVehicle][vehEngine] = 1;
            SetVehicleParamsEx(returnVehicle, VEHICLE_PARAMS_ON, 0, 0, 0, 0, 0, 0);
        }
    }
    return returnVehicle;
}

forward public DestroyVehicleEx(vehicleid);
stock DestroyVehicleEx(vehicleid)
{	
	DestroyVehicle(vehicleid);
	Iter_Remove(vehicleType<vehicleInfo[vehicleid][vehType]>, vehicleid);

	return true;
}

//
#if defined _ALS_AddStaticVehicleEx
    #undef AddStaticVehicleEx
#else
    #define _ALS_AddStaticVehicleEx
#endif
#define AddStaticVehicleEx _AddStaticVehicleEx

#if defined _ALS_CreateVehicle
    #undef CreateVehicle
#else
    #define _ALS_CreateVehicle
#endif
#define CreateVehicle _AddStaticVehicleEx


#if defined _ALS_DestroyVehicle
    #undef DestroyVehicle
#else
    #define _ALS_DestroyVehicle
#endif
#define DestroyVehicle DestroyVehicleEx
//

setVehicleRandomLocation(const random, &Float:fX, &Float:fY, &Float:fZ)
{
	fX = 2351.9250;
	fY = 1405.0513 + (3.6 * random);
	fZ = 42.9638;
	return true;
}

hook OnVehicleDeath(vehicleid)
{
    radioListeners[vehicleRadio[vehicleid]] --;
    vehicleRadio[vehicleid] = 0;
    StopAudioStreamForPlayersInCar(vehicleid);
    return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnGameModeInit()
{
	ManualVehicleEngineAndLights();
	for(new i; i < MAX_VEHICLES; i++)
	{
		vehicleInfo[i][vehFuel] = 100;
	}

    Command_AddAltNamed("carradio", "carmusic");
    Command_AddAltNamed("fixveh", "fv");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnVehicleSpawn(vehicleid)
{
    if(Iter_Contains(vehicleType<VEH_TYPE_ADMIN>, vehicleid))
        DestroyVehicle(vehicleid);

	if(IsBike(vehicleid))
    	SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_ON, 0, 0, 0, 0, 0, 0);

    vehicleInfo[vehicleid][vehFuel] = 100;

    radioListeners[vehicleRadio[vehicleid]] --;
    vehicleRadio[vehicleid] = 0;
    StopAudioStreamForPlayersInCar(vehicleid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook loadServerSystems()
{
    mysql_tquery(SQL, "select * from `vehicles`", "loadVehicles", "");
    return Y_HOOKS_CONTINUE_RETURN_1;
}

function:loadVehicles()
{
    new countGroupVehicles, plateString[20];
    for(new i = 1; i <= cache_num_rows(); i++)
    {
        static vehicleGroup, vehicleDBId, vehicleModel, vehicleRank, vehColor1, vehColor2, Float:posX, Float:posY, Float:posZ, Float:posA;
        
        cache_get_value_name_int(i - 1, "groupId", vehicleGroup);
        
        if(vehicleGroup != 0 && vehicleGroup != 4 && vehicleGroup != 5 && vehicleGroup != 6 && vehicleGroup != 10 && vehicleGroup != 15 && vehicleGroup != 16)
            continue;

        cache_get_value_name_int(i - 1, "ID", vehicleDBId);
        cache_get_value_name_int(i - 1, "modelId", vehicleModel);
        cache_get_value_name_int(i - 1, "vehRank", vehicleRank);
        cache_get_value_name_int(i - 1, "color1", vehColor1);
        cache_get_value_name_int(i - 1, "color2", vehColor2);

        cache_get_value_name_float(i - 1, "posX", posX);
        cache_get_value_name_float(i - 1, "posY", posY);
        cache_get_value_name_float(i - 1, "posZ", posZ);
        cache_get_value_name_float(i - 1, "posA", posA);

        new serverVehicle = AddStaticVehicleEx(vehicleModel, posX, posY, posZ, posA, vehColor1, vehColor2, 300, false, (!vehicleGroup ? VEH_TYPE_NONE : VEH_TYPE_GROUP));
        new vehicleId = Iter_Count(vehicleType<VEH_TYPE_GROUP>) + 1;
        vehicleInfo[vehicleId][vehID] = vehicleDBId;

        vehicleInfo[vehicleId][vehGroup] = vehicleGroup;
        vehicleInfo[vehicleId][vehRank] = vehicleRank;

        serverVehicleId[serverVehicle] = vehicleId;

        if(vehicleGroup == 0)
            format(plateString, sizeof plateString, "XYZSR998");

        else
            format(plateString, sizeof plateString, "{ff0000}GANG %d", vehicleGroup);
        
        SetVehicleNumberPlate(serverVehicle, plateString);

        if(vehicleGroup)
            countGroupVehicles ++;
    }
    printf("Vehicles [total: %d | on server: %d]", cache_num_rows(), cache_num_rows() - countGroupVehicles);
    return true;
}

YCMD:vmove(playerid, params[], help)
{
    check_queries

    if(playerInfo[playerid][pAdmin] < 6)
        return true;

    if(!IsPlayerInAnyVehicle(playerid))
        sendError(playerid,"You are not in a vehicle.");

    if(serverVehicleId[GetPlayerVehicleID(playerid)])
    {
        new vId = serverVehicleId[GetPlayerVehicleID(playerid)], Float:pos[4];

        GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
        GetPlayerFacingAngle(playerid, pos[3]);

        mysql_format(SQL, returnString, sizeof returnString, "update `vehicles` set `posX` = '%f', `posY` = '%f', `posZ` = '%f', `posA` = '%f' where `id` = '%d';", pos[0], pos[1], pos[2], pos[3], vId);
        mysql_tquery(SQL, returnString, "", "");

        SendClientMessage(playerid, COLOR_YELLOW, "VEHICLE INFO >> {ffffff}Vehicle moved. Server restart required!");
    }
    return true;
}

YCMD:savevehicle(playerid, params[], help)
{
    check_queries

    if(playerInfo[playerid][pAdmin] < 6)
        return true;

    new returnGroup, returnColor1, returnColor2;
    if(sscanf(params, "iii", returnGroup, returnColor1, returnColor2))
        return sendSyntaxMessage(playerid, "/savevehicle [groupId] [color1] [color2]");

    if(!IsPlayerInAnyVehicle(playerid))
        sendError(playerid,"You are not in a vehicle.");

    new Float:pos[4];
    GetVehiclePos(GetPlayerVehicleID(playerid), pos[0], pos[1], pos[2]);
    GetVehicleZAngle(GetPlayerVehicleID(playerid), pos[3]);

    va_SendClientMessage(playerid, COLOR_YELLOW, "[i]{ffffff} Created vehicle %d as group vehicle | Group: %d | Color 1: %d, Color 2: %d.", GetVehicleModel(GetPlayerVehicleID(playerid)), returnGroup, returnColor1, returnColor2);
    SendClientMessage(playerid, COLOR_YELLOW, "[!!]{ffffff} In order to create the vehicle on server a restart is required.");

    mysql_format(SQL, returnString, sizeof returnString, "insert into `vehicles` (`modelId`, `groupId`, `color1`, `color2`, `posX`, `posY`, `posZ`, `posA`) values ('%d', '%d', '%d', '%d', '%f', '%f', '%f', '%f');", GetVehicleModel(GetPlayerVehicleID(playerid)), returnGroup, returnColor1, returnColor2, pos[0], pos[1], pos[2], pos[3]);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}
// END

new aVehicleNames[][] =
{
    "Landstalker", "Bravura", "Buffalo", "Linerunner", "Perrenial", "Sentinel",
	"Dumper", "Firetruck", "Trashmaster", "Stretch", "Manana", "Infernus",
	"Voodoo", "Pony", "Mule", "Cheetah", "Ambulance", "Leviathan", "Moonbeam",
    "Esperanto", "Taxi", "Washington", "Bobcat", "Whoopee", "BF Injection",
	"Hunter", "Premier", "Enforcer", "Securicar", "Banshee", "Predator", "Bus",
	"Rhino", "Barracks", "Hotknife", "Trailer", "Previon", "Coach", "Cabbie",
	"Stallion", "Rumpo", "RC Bandit", "Romero", "Packer", "Monster", "Admiral",
	"Squalo", "Seasparrow", "Pizzaboy", "Tram", "Trailer", "Turismo", "Speeder",
	"Reefer", "Tropic", "Flatbed", "Yankee", "Caddy", "Solair", "Berkley's RC Van",
	"Skimmer", "PCJ-600", "Faggio", "Freeway", "RC Baron", "RC Raider", "Glendale",
	"Oceanic","Sanchez", "Sparrow", "Patriot", "Quad", "Coastguard", "Dinghy",
	"Hermes", "Sabre", "Rustler", "ZR-350", "Walton", "Regina", "Comet", "BMX",
	"Burrito", "Camper", "Marquis", "Baggage", "Dozer", "Maverick", "News Chopper",
	"Rancher", "FBI Rancher", "Virgo", "Greenwood", "Jetmax", "Hotring Racer C", "Sandking",
	"Blista Compact", "Police Maverick", "Boxville", "Benson", "Mesa", "RC Goblin",
	"Hotring Racer A", "Hotring Racer B", "Bloodring Banger", "Rancher", "Super GT",
	"Elegant", "Journey", "Bike", "Mountain Bike", "Beagle", "Cropduster", "Stunt",
 	"Tanker", "Roadtrain", "Nebula", "Majestic", "Buccaneer", "Shamal", "Hydra",
 	"FCR-900", "NRG-500", "HPV1000", "Cement Truck", "Tow Truck", "Fortune",
 	"Cadrona", "FBI Truck", "Willard", "Forklift", "Tractor", "Combine", "Feltzer",
 	"Remington", "Slamvan", "Blade", "Freight", "Streak", "Vortex", "Vincent",
    "Bullet", "Clover", "Sadler", "Firetruck", "Hustler", "Intruder", "Primo",
	"Cargobob", "Tampa", "Sunrise", "Merit", "Utility", "Nevada", "Yosemite",
	"Windsor", "Monster", "Monster", "Uranus", "Jester", "Sultan", "Stratum",
	"Elegy", "Raindance", "RC Tiger", "Flash", "Tahoma", "Savanna", "Bandito",
    "Freight Flat", "Streak Carriage", "Kart", "Mower", "Dune", "Sweeper",
	"Broadway", "Tornado", "AT-400", "DFT-30", "Huntley", "Stafford", "BF-400",
	"News Van", "Tug", "Trailer", "Emperor", "Wayfarer", "Euros", "Hotdog", "Club",
	"Freight Box", "Trailer", "Andromada", "Dodo", "RC Cam", "Launch", "Police Car",
 	"Police Car", "Police Car", "Police Ranger", "Picador", "S.W.A.T", "Alpha",
 	"Phoenix", "Glendale", "Sadler", "Luggage", "Luggage", "Stairs", "Boxville",
 	"Tiller", "Utility Trailer"
};
stock GetVehicleMaxSpeed(model) {
	switch(model) {
		case 400: return 141;
		case 401: return 131;
		case 402: return 167;
		case 403: return 98;
		case 404: return 119;
		case 405: return 146;
		case 406: return 99;
		case 407: return 132;
		case 408: return 89;
		case 409: return 141;
		case 410: return 116;
		case 411: return 198;
		case 412: return 151;
		case 413: return 98;
		case 414: return 94;
		case 415: return 172;
		case 416: return 138;
		case 417: return 120;
		case 418: return 103;
		case 419: return 133;
		case 420: return 130;
		case 421: return 137;
		case 422: return 125;
		case 423: return 88;
		case 424: return 121;
		case 425: return 179;
		case 426: return 155;
		case 427: return 148;
		case 428: return 140;
		case 429: return 180;
		case 430: return 162;
		case 431: return 116;
		case 432: return 85;
		case 433: return 99;
		case 434: return 149;
		case 435: return 10;
		case 436: return 133;
		case 437: return 141; 
		case 438: return 127;
		case 439: return 151;
		case 440: return 122;
		case 441: return 67;
		case 442: return 124;
		case 443: return 113;
		case 444: return 99;
		case 445: return 146;
		case 446: return 137;
		case 447: return 105;
		case 448: return 109;
		case 449: return 163;
		case 450: return 5;
		case 451: return 172;
		case 452: return 130;
		case 453: return 48;
		case 454: return 100;
		case 455: return 141;
		case 456: return 95;
		case 457: return 85;
		case 458: return 140;
		case 459: return 122; 
		case 460: return 130;
		case 461: return 141;
		case 462: return 103;
		case 463: return 135;
		case 464: return 39;
		case 465: return 34;
		case 466: return 131;
		case 467: return 126;
		case 468: return 140;
		case 469: return 115;
		case 470: return 140;
		case 471: return 98;
		case 472: return 102;
		case 473: return 94;
		case 474: return 133;
		case 475: return 154;
		case 476: return 148;
		case 477: return 167;
		case 478: return 105;
		case 479: return 125;
		case 480: return 165;
		case 481: return 70;
		case 482: return 140;
		case 483: return 110;
		case 484: return 54;
		case 485: return 89;
		case 486: return 58;
		case 487: return 156;
		case 488: return 154;
		case 489: return 125;
		case 490: return 140;
		case 491: return 133;
		case 492: return 126;
		case 493: return 135;
		case 494: return 192;
		case 495: return 158;
		case 496: return 145;
		case 497: return 165;
		case 498: return 96;
		case 499: return 99;
		case 500: return 126;
		case 501: return 24;
		case 502: return 192;
		case 503: return 192;
		case 504: return 155;
		case 505: return 125;
		case 506: return 160;
		case 507: return 148;
		case 508: return 96;
		case 509: return 75;
		case 510: return 95;
		case 511: return 110;
		case 512: return 96;
		case 513: return 133;
		case 514: return 108;
		case 515: return 127;
		case 516: return 141;
		case 517: return 141;
		case 518: return 147;
		case 519: return 220;
		case 520: return 200;
		case 521: return 147;
		case 522: return 157;
		case 523: return 134;
		case 524: return 116;
		case 525: return 143;
		case 526: return 141;
		case 527: return 133;
		case 528: return 158;
		case 529: return 133;
		case 530: return 54;
		case 531: return 63;
		case 532: return 98;
		case 533: return 149;
		case 534: return 151;
		case 535: return 141;
		case 536: return 155;
		case 537: return 164;
		case 538: return 165;
		case 539: return 88;
		case 540: return 133;
		case 541: return 182;
		case 542: return 147;
		case 543: return 135;
		case 544: return 132;
		case 545: return 131;
		case 546: return 133;
		case 547: return 127;
		case 548: return 150;
		case 549: return 137;
		case 550: return 129;
		case 551: return 141;
		case 552: return 108;
		case 553: return 117;
		case 554: return 128;
		case 555: return 141;
		case 556: return 98;
		case 557: return 98;
		case 558: return 139;
		case 559: return 159;
		case 560: return 151;
		case 561: return 138;
		case 562: return 159;
		case 563: return 140;
		case 564: return 79;
		case 565: return 147;
		case 566: return 143;
		case 567: return 155;
		case 568: return 131;
		case 569, 570: return 0;
		case 571: return 83;
		case 572: return 54;
		case 573: return 98;
		case 574: return 54;
		case 575: return 141;
		case 576: return 141;
		case 577: return 241;
		case 578: return 116;
		case 579: return 141;
		case 580: return 137;
		case 581: return 135;
		case 582: return 122;
		case 583: return 76;
		case 584: return 4;
		case 585: return 137;
		case 586: return 128;
		case 587: return 147;
		case 588: return 96;
		case 589: return 145;
		case 590, 591: return 0;
		case 592: return 240;
		case 593: return 140;
		case 594: return 54;
		case 595: return 79;
		case 596, 597, 598: return 157;
		case 599: return 141;
		case 600: return 135;
		case 601: return 99;
		case 602: return 151;
		case 603: return 153;
		case 604: return 131;
		case 605: return 135;
		case 606: return 89;
		case 607: return 89;
		case 608: return 0;
		case 609: return 96;
		case 610: return 11;
		case 611: return 40;
	}
	return 1;
}
/*stock GetVehicleMaxSpeed(model)
{
    new speed;
    switch(model - 400)
    {
        case 0: speed = 157;
        case 1: speed = 147;
        case 2: speed = 186;
        case 3: speed = 110;
        case 4: speed = 133;
        case 5: speed = 164;
        case 6: speed = 110;
        case 7: speed = 148;
        case 8: speed = 100; 
        case 9: speed = 158;
        case 10: speed = 129;
        case 11: speed = 221;
        case 12: speed = 168;
        case 13: speed = 110;
        case 14: speed = 105;
        case 15: speed = 192;
        case 16: speed = 154;
        case 17: speed = 127;
        case 18: speed = 115;
        case 19: speed = 149;
        case 20: speed = 145;
        case 21: speed = 154;
        case 22: speed = 140;
        case 23: speed = 99; 
        case 24: speed = 135;
        case 25: speed = 191;
        case 26: speed = 173;
        case 27: speed = 165;
        case 28: speed = 157;
        case 29: speed = 201;
        case 30: speed = 100;
        case 31: speed = 130;
        case 32: speed = 94; 
        case 33: speed = 110;
        case 34: speed = 167;
        case 36: speed = 149;
        case 37: speed = 158;
        case 38: speed = 142;
        case 39: speed = 168;
        case 40: speed = 136;
        case 41: speed = 75; 
        case 42: speed = 139;
        case 43: speed = 126;
        case 44: speed = 110;
        case 45: speed = 164;
        case 46: speed = 270;
        case 47: speed = 270;
        case 48: speed = 111;
        case 49: speed = 169;
        case 51: speed = 193;
        case 52: speed = 270;
        case 53: speed = 270; 
        case 54: speed = 135;
        case 55: speed = 157;
        case 56: speed = 100;
        case 57: speed = 95; 
        case 58: speed = 157;
        case 59: speed = 136;
        case 60: speed = 270;  
        case 61: speed = 160;
        case 62: speed = 111;
        case 63: speed = 142;
        case 66: speed = 147;
        case 67: speed = 140;
        case 68: speed = 144;
        case 69: speed = 270;  
        case 70: speed = 157;
        case 71: speed = 110;
        case 73: speed = 190;  
        case 74: speed = 149;
        case 75: speed = 173;
        case 76: speed = 270;  
        case 77: speed = 186;
        case 78: speed = 117;
        case 79: speed = 140;
        case 80: speed = 184;
        case 81: speed = 73; 
        case 82: speed = 156;
        case 83: speed = 122;
        case 86: speed = 64; 
        case 87: speed = 270;  
        case 88: speed = 270;  
        case 89: speed = 139;
        case 90: speed = 157;
        case 91: speed = 149;
        case 92: speed = 140;
        case 93: speed = 270;  
        case 94: speed = 214;
        case 95: speed = 176;
        case 96: speed = 162;
        case 97: speed = 270;
        case 99: speed = 123;
        case 100: speed = 140;
        case 102: speed = 216;
        case 103: speed = 216;
        case 104: speed = 173;
        case 105: speed = 139;
        case 106: speed = 149;
        case 107: speed = 166;
        case 108: speed = 108;
        case 109: speed = 120; 
        case 110: speed = 130;
        case 113: speed = 270;  
        case 114: speed = 120;
        case 115: speed = 142;
        case 116: speed = 157;
        case 117: speed = 157;
        case 118: speed = 164;
        case 119: speed = 270;  
        case 120: speed = 270;  
        case 121: speed = 160;
        case 122: speed = 176;
        case 123: speed = 151;
        case 124: speed = 130;
        case 125: speed = 160;
        case 126: speed = 158;
        case 127: speed = 149;
        case 128: speed = 176;
        case 129: speed = 149;
        case 131: speed = 70; 
        case 132: speed = 110;
        case 133: speed = 167;
        case 134: speed = 168;
        case 135: speed = 158;
        case 136: speed = 173; 
        case 139: speed = 270; 
        case 140: speed = 149;
        case 141: speed = 203;
        case 142: speed = 164;
        case 143: speed = 151;
        case 144: speed = 148;
        case 145: speed = 147;
        case 146: speed = 149;
        case 147: speed = 142;
        case 148: speed = 270;
        case 149: speed = 153;
        case 150: speed = 145;
        case 151: speed = 157;
        case 154: speed = 144;
        case 155: speed = 158;
        case 156: speed = 113;
        case 157: speed = 113;
        case 158: speed = 156;
        case 159: speed = 178;
        case 160: speed = 169;
        case 161: speed = 154;
        case 162: speed = 178;
        case 164: speed = 145; 
        case 165: speed = 165;
        case 166: speed = 160;
        case 167: speed = 173;
        case 168: speed = 146;
        case 171: speed = 93; 
        case 172: speed = 60; 
        case 173: speed = 110;
        case 174: speed = 60; 
        case 175: speed = 158;
        case 176: speed = 158;
        case 178: speed = 130;
        case 179: speed = 158;
        case 180: speed = 153;
        case 181: speed = 151;
        case 182: speed = 136;
        case 183: speed = 85; 
        case 185: speed = 153;
        case 186: speed = 142;
        case 187: speed = 165;
        case 188: speed = 108;
        case 189: speed = 162;
        case 193: speed = 270;  
        case 196: speed = 175;
        case 197: speed = 175;
        case 198: speed = 175;
        case 199: speed = 157;
        case 200: speed = 151;
        case 201: speed = 110;
        case 202: speed = 169;
        case 203: speed = 171;
        case 204: speed = 147;
        case 205: speed = 151;
        case 209: speed = 102; 
        default: speed = 0;
    }
    return speed;
}*/

stock PlayAudioStreamForPlayersInCar(vehicleid, const url[])
{
    foreach(new i : playersInVehicle)
    {
        if(GetPlayerVehicleID(i) != vehicleid)
            continue;

        PlayAudioStreamForPlayer(i, url);
    }
    return true;
}

stock StopAudioStreamForPlayersInCar(vehicleid)
{
    foreach(new i : playersInVehicle)
    {
        if(GetPlayerVehicleID(i) != vehicleid)
            continue;

        StopAudioStreamForPlayer(i);
    }
    return true;
}

#define CUSTOM_URL_INDEX sizeof RadioNames

YCMD:carradio(playerid, params[], help)
{
    if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
        return true;

    if(IsBike(GetPlayerVehicleID(playerid)))
        return true;

    new radioString[2048] = "Radio Name\tListeners\n";

    for(new i; i < sizeof RadioNames; i++)
        format(radioString, sizeof radioString, "%s%s\t%s\n", radioString, RadioNames[i], (i == 0 ? "" : va_return("%d", radioListeners[i])));

    format(radioString, sizeof radioString, "%sCustom Url\t\n", radioString); // Add Custom Url option

    Dialog_Show(playerid, DIALOG_RADIO, DIALOG_STYLE_TABLIST_HEADERS, "Car Radio:", radioString, "Ok", "Cancel");
    return true;
}

Dialog:DIALOG_RADIO(playerid, response, listitem, inputtext[])
{
    if(!response || GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
        return true;

    new vehicleid = GetPlayerVehicleID(playerid);

    if(vehicleRadio[vehicleid] < sizeof RadioNames)
        radioListeners[vehicleRadio[vehicleid]]--;

    if(listitem == CUSTOM_URL_INDEX)
    {
        return Dialog_Show(playerid, DIALOG_CUSTOMRADIO, DIALOG_STYLE_INPUT, "Custom Url", "Enter a URL:", "Play", "Cancel");
    }
    else if(!listitem)
    {
        StopAudioStreamForPlayersInCar(vehicleid);
        sendNearbyMessage(playerid, COLOR_PURPLE, "** %s (%d) turned off his %s (model:%d) stereo.", GetDBName(playerid), playerid, aVehicleNames[GetVehicleModel(vehicleid) - 400], GetVehicleModel(vehicleid), RadioNames[listitem]);
    }
    else
    {
        StopAudioStreamForPlayersInCar(vehicleid);
        PlayAudioStreamForPlayersInCar(vehicleid, RadioLinks[listitem]);

        sendNearbyMessage(playerid, COLOR_PURPLE, "** %s (%d) changed his %s (model:%d) radio station to %s.", GetDBName(playerid), playerid, aVehicleNames[GetVehicleModel(vehicleid) - 400], GetVehicleModel(vehicleid), RadioNames[listitem]);
    }

    vehicleRadio[vehicleid] = listitem;
    if(listitem < sizeof RadioNames)
        radioListeners[vehicleRadio[vehicleid]]++;

    return true;
}

Dialog:DIALOG_CUSTOMRADIO(playerid, response, listitem, inputtext[])
{
    if(!response || GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
        return true;

    new vehicleid = GetPlayerVehicleID(playerid);
    new name[30];
    format(returnString, sizeof(returnString), "https://api.hpq.ro/mp3?url=%s",inputtext);
    StopAudioStreamForPlayersInCar(vehicleid);
    PlayAudioStreamForPlayersInCar(vehicleid, returnString);
    GetPlayerName(playerid, name, sizeof(name));
    sendNearbyMessage(playerid, COLOR_PURPLE, "* %s changed the radio station to a Custom URL.",name);

    return true;
}


#include "../module_inc/src/personalVehicles.inc"

YCMD:gotocar(playerid, params[], help)
{
    check_admin

    new returnVehicleId;
    if(sscanf(params, "d", returnVehicleId))
        return sendSyntaxMessage(playerid, "/gotocar <Car ID>");
    
    if(!IsValidVehicle(returnVehicleId))
        sendError(playerid,"Invalid vehicle.");

    new Float:pos[3];
    GetVehiclePos(returnVehicleId, pos[0], pos[1], pos[2]);

    SetPlayerPos(playerid, pos[0], pos[1], pos[2]);
    SetPlayerVirtualWorld(playerid, GetVehicleVirtualWorld(returnVehicleId));

    SendClientMessage(playerid, COLOR_SERVER, "[INFO]:{ffffff} You have been teleported.");
    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_DCHAT, "%s has used /gotocar %d.", GetName(playerid), returnVehicleId);
    return true;
}

CMD:entercar(playerid, params[])
{
    if(playerInfo[playerid][pAdmin] < 2)
        return returnNoAcces(playerid);

    new testcar, loc;
    if(sscanf(params, "ii", testcar, loc))
        return sendSyntaxMessage(playerid, "/entercar <Car ID> <Seat ID>");
            
    if(loc < 0 || loc > 4)
        sendError(playerid,"{FFB870}Invalid Seat ID(0-4).");

    PutPlayerInVehicle(playerid, testcar, loc);
    SendClientMessage(playerid, COLOR_SERVER, "[INFO]: {ffffff}You have been teleported.");
    return true;
}
YCMD:putplayerinvehicle(playerid, params[], help)
{
    if(playerInfo[playerid][pAdmin] < 2)
        return returnNoAcces(playerid);

    new testcar, loc, id;
    if(sscanf(params, "iii", id, testcar, loc))
        return sendSyntaxMessage(playerid, "/putplayerinvehicle <Name/Playerid> <Car ID> <Seat ID>");    

    if(loc < 0 || loc > 4)
        sendError(playerid,"{FFB870}Invalid Seat ID(0-4).");


    PutPlayerInVehicle(id, testcar, loc);
    SendClientMessage(id, COLOR_SERVER, "[INFO]: {ffffff}You have been teleported.");
    return true;
}
YCMD:getcar(playerid, params[], help)
{
    check_admin

    new returnVehicleId;
    if(sscanf(params, "d", returnVehicleId))
        return sendSyntaxMessage(playerid, "/getcar <Car ID>");
    
    if(!IsValidVehicle(returnVehicleId))
        return sendError(playerid,"Invalid vehicle.");

    new Float:pos[3];
    GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

    SetVehiclePos(returnVehicleId, pos[0], pos[1], pos[2]);
    SetVehicleVirtualWorld(returnVehicleId, GetPlayerVirtualWorld(playerid));
  
    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s teleported vehicle %d to him.", GetName(playerid), returnVehicleId);
    return true;
}

timer RACtime[10000]()
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
    SendClientMessageToAll(COLOR_SERVER, "[!] {ffffff}All unused cars got respawned.");
    return true;
}
YCMD:rac(playerid, params[], help)
{
    if(playerInfo[playerid][pAdmin] < 3)
        return returnNoAcces(playerid);
//009999
    defer RACtime();
    va_SendClientMessageToAll(COLOR_SERVER, "[!] {FFFFFF}All unused cars will be respawned in {5d0b0a}10 {FFFFFF}seconds by %s!", GetName(playerid));
    return true;
}

YCMD:window(playerid, params[], help)
{
    if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
        sendError(playerid,"You need to be driver to use this command.");
   
    new window, window_state[4], vehicleid = GetPlayerVehicleID(playerid);
    if(sscanf(params, "i", window))
    {
        SendClientMessage(playerid, -1, "Usage: /window <window>");
        GetVehicleParamsCarWindows(vehicleid, window_state[0], window_state[1], window_state[2], window_state[3]);
        va_SendClientMessage(playerid, COLOR_SERVER, "[!] {ffffff}Vehicle windows status: %d %d %d %d", window_state[0], window_state[1], window_state[2], window_state[3]);
        return true;
    }

    if(window < 1 || window > 4)
        sendError(playerid,"The value of 'window' parameter should be between 1 and 4");

    GetVehicleParamsCarWindows(vehicleid, window_state[0], window_state[1], window_state[2], window_state[3]);
    window_state[--window] = !window_state[window];
    SetVehicleParamsCarWindows(vehicleid, window_state[0], window_state[1], window_state[2], window_state[3]);
    return true;
}

YCMD:fixveh(playerid, params[], help)
{
    check_admin

    if(!IsPlayerInAnyVehicle(playerid)) 
        return sendError(playerid,"You are not in a vehicle.");

    RepairVehicle(GetPlayerVehicleID(playerid));
    vehicleInfo[GetPlayerVehicleID(playerid)][vehFuel] = 100;
    
    SendClientMessage(playerid, -1, "[INFO]: {ffffff}Car was repaired successfully.");

    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s has used /fixveh", GetDBName(playerid));
    return true;
}

YCMD:flip(playerid, params[], help)
{
    check_admin

    if(!IsPlayerInAnyVehicle(playerid)) 
        sendError(playerid,"You are not in a vehicle.");

    new Float: vAngle;
    GetVehicleZAngle(GetPlayerVehicleID(playerid), vAngle);
    SetVehicleZAngle(GetPlayerVehicleID(playerid), vAngle);

    SendClientMessage(playerid, -1, "Vehicle successfully flipped back over.");
    return true; 
}

YCMD:engine(playerid, params[], help)
{
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
		return true;

    if(!vehicleInfo[GetPlayerVehicleID(playerid)][vehFuel])
        return sendError(playerid,"This car don't have fuel.");

	if(IsBike(GetPlayerVehicleID(playerid)))
		return sendError(playerid,"Command can not be used on bike.");

	if(GetPVarInt(playerid, "refuel_veh"))
		return sendError(playerid,"Please wait, you refill your vehicle.");

	new bool:engine, bool:lights, bool:alarm, bool:doors, bool:bonnet, bool:boot, bool:objective;
	switch(vehicleInfo[GetPlayerVehicleID(playerid)][vehEngine])
	{
		case 0:
		{
			vehicleInfo[GetPlayerVehicleID(playerid)][vehEngine] = true;

			GetVehicleParamsEx(GetPlayerVehicleID(playerid), engine, lights, alarm, doors, bonnet, boot, objective);
	        SetVehicleParamsEx(GetPlayerVehicleID(playerid), VEHICLE_PARAMS_ON, lights, alarm, doors, bonnet, boot, objective);
		}
		case 1:
		{
			vehicleInfo[GetPlayerVehicleID(playerid)][vehEngine] = false;

			GetVehicleParamsEx(GetPlayerVehicleID(playerid), engine, lights, alarm, doors, bonnet, boot, objective);
	        SetVehicleParamsEx(GetPlayerVehicleID(playerid), VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);
		}
	}
	sendNearbyMessage(playerid, COLOR_PURPLE, "* %s %s the engine of %s.", (playerUndercover[playerid] ? ("An unknown hitman") : GetName(playerid)), (vehicleInfo[GetPlayerVehicleID(playerid)][vehEngine] ? ("starts") : ("stops")), aVehicleNames[GetVehicleModel(GetPlayerVehicleID(playerid)) - 400]);
	return true;
}

YCMD:colors(playerid, params[], help)
{
	new colorString[3128];
    strcat(colorString, "{FFFFFF}Normal colors:\n");
    strcat(colorString, "{000000}000 {F5F5F5}001 {2A77A1}002 {840410}003 {263739}004 {86446E}005 {D78E10}006 {4C75B7}007 {BDBEC6}008 {5E7072}009 {46597A}010 {656A79}011 {5D7E8D}012 {58595A}013 {D6DAD6}014 {9CA1A3}015 {335F3F}016 {730E1A}017 {7B0A2A}018 {9F9D94}019\n");
    strcat(colorString, "{3B4E78}020 {732E3E}021 {691E3B}022 {96918C}023 {515459}024 {3F3E45}025 {A5A9A7}026 {635C5A}027 {3D4A68}028 {979592}029 {421F21}030 {5F272B}031 {8494AB}032 {767B7C}033 {646464}034 {5A5752}035 {252527}036 {2D3A35}037 {93A396}038 {6D7A88}039\n");
    strcat(colorString, "{221918}040 {6F675F}041 {7C1C2A}042 {5F0A15}043 {193826}044 {5D1B20}045 {9D9872}046 {7A7560}047 {989586}048 {ADB0B0}049 {848988}050 {304F45}051 {4D6268}052 {162248}053 {272F4B}054 {7D6256}055 {9EA4AB}056 {9C8D71}057 {6D1822}058 {4E6881}059\n");
    strcat(colorString, "{9C9C98}060 {917347}061 {661C26}062 {949D9F}063 {A4A7A5}064 {8E8C46}065 {341A1E}066 {6A7A8C}067 {AAAD8E}068 {AB988F}069 {851F2E}070 {6F8297}071 {585853}072 {9AA790}073 {601A23}074 {20202C}075 {A4A096}076 {AA9D84}077 {78222B}078 {0E316D}079\n");
    strcat(colorString, "{722A3F}080 {7B715E}081 {741D28}082 {1E2E32}083 {4D322F}084 {7C1B44}085 {2E5B20}086 {395A83}087 {6D2837}088 {A7A28F}089 {AFB1B1}090 {364155}091 {6D6C6E}092 {0F6A89}093 {204B6B}094 {2B3E57}095 {9B9F9D}096 {6C8495}097 {4D8495}098 {AE9B7F}099\n");
    strcat(colorString, "{406C8F}100 {1F253B}101 {AB9276}102 {134573}103 {96816C}104 {64686A}105 {105082}106 {A19983}107 {385694}108 {525661}109 {7F6956}110 {8C929A}111 {596E87}112 {473532}113 {44624F}114 {730A27}115 {223457}116 {640D1B}117 {A3ADC6}118 {695853}119\n");
    strcat(colorString, "{9B8B80}120 {620B1C}121 {5B5D5E}122 {624428}123 {731827}124 {1B376D}125 {EC6AAE}126 {000000}127\n");
    strcat(colorString, "{FFFFFF}Hidden colors:\n");
    strcat(colorString, "{177517}128 {210606}129 {125478}130 {452A0D}131 {571E1E}132 {010701}133 {25225A}134 {2C89AA}135 {8A4DBD}136 {35963A}137 {B7B7B7}138 {464C8D}139 {84888C}140 {817867}141 {817A26}142 {6A506F}143 {583E6F}144 {8CB972}145 {824F78}146 {6D276A}147\n");
    strcat(colorString, "{1E1D13}148 {1E1306}149 {1F2518}150 {2C4531}151 {1E4C99}152 {2E5F43}153 {1E9948}154 {1E9999}155 {999976}156 {7C8499}157 {992E1E}158 {2C1E08}159 {142407}160 {993E4D}161 {1E4C99}162 {198181}163 {1A292A}164 {16616F}165 {1B6687}166 {6C3F99}167\n");
    strcat(colorString, "{481A0E}168 {7A7399}169 {746D99}170 {53387E}171 {222407}172 {3E190C}173 {46210E}174 {991E1E}175 {8D4C8D}176 {805B80}177 {7B3E7E}178 {3C1737}179 {733517}180 {781818}181 {83341A}182 {8E2F1C}183 {7E3E53}184 {7C6D7C}185 {020C02}186 {072407}187\n");
    strcat(colorString, "{163012}188 {16301B}189 {642B4F}190 {368452}191 {999590}192 {818D96}193 {99991E}194 {7F994C}195 {839292}196 {788222}197 {2B3C99}198 {3A3A0B}199 {8A794E}200 {0E1F49}201 {15371C}202 {15273A}203 {375775}204 {060820}205 {071326}206 {20394B}207\n");
    strcat(colorString, "{2C5089}208 {15426C}209 {103250}210 {241663}211 {692015}212 {8C8D94}213 {516013}214 {090F02}215 {8C573A}216 {52888E}217 {995C52}218 {99581E}219 {993A63}220 {998F4E}221 {99311E}222 {0D1842}223 {521E1E}224 {42420D}225 {4C991E}226 {082A1D}227\n");
    strcat(colorString, "{96821D}228 {197F19}229 {3B141F}230 {745217}231 {893F8D}232 {7E1A6C}233 {0B370B}234 {27450D}235 {071F24}236 {784573}237 {8A653A}238 {732617}239 {319490}240 {56941D}241 {59163D}242 {1B8A2F}243 {38160B}244 {041804}245 {355D8E}246 {2E3F5B}247\n");
    strcat(colorString, "{561A28}248 {4E0E27}249 {706C67}250 {3B3E42}251 {2E2D33}252 {7B7E7D}253 {4A4442}254 {28344E}255\n");
	Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Vehicle Colors List - /carcolor", colorString, "Close", "");
	return true;
}

YCMD:carcolortemp(playerid, params[], help)
{
    if(playerInfo[playerid][pAdmin] < 4)
        return returnNoAcces(playerid);

    new color1, color2;
    if(sscanf(params, "ii",color1, color2))
        return sendSyntaxMessage(playerid, "/carcolortemp <Color 1> <Color 2>");

    if(color1 < 0 || color1 > 255)
        sendError(playerid,"{FFB870}Wrong color id.");

    if(color2 < 0 || color2 > 255)
        sendError(playerid,"{FFB870}Wrong color id.");

    if(!IsPlayerInAnyVehicle(playerid))
        sendError(playerid,"{FFB870}You are not in car.");
    
    ChangeVehicleColours(GetPlayerVehicleID(playerid), color1, color2);

    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "AdmInfo: %s has used /carcolortemp", GetName(playerid));
    return true;
}

YCMD:lights(playerid, params[], help)
{
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
        return true;

    new bool:engine, bool:lights, bool:alarm, bool:doors, bool:bonnet, bool:boot, bool:objective;
	GetVehicleParamsEx(GetPlayerVehicleID(playerid), engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(GetPlayerVehicleID(playerid), engine, vehicleInfo[GetPlayerVehicleID(playerid)][vehLights] ? VEHICLE_PARAMS_OFF : VEHICLE_PARAMS_ON, alarm, doors, bonnet, boot, objective);
	
	vehicleInfo[GetPlayerVehicleID(playerid)][vehLights] = vehicleInfo[GetPlayerVehicleID(playerid)][vehLights] ? 0 : 1;
	return true;
}

YCMD:lock(playerid, params[], help)
{
    new bool:engine, bool:lights, bool:alarm, bool:doors, bool:bonnet, bool:boot, bool:objective, vehicleid = (!IsPlayerInAnyVehicle(playerid) ? GetClosestVehicle(playerid, 5.0) : GetPlayerVehicleID(playerid));

    if(!IsValidVehicle(vehicleid))
        return true;

    new vehicleDBId = VehicleSQL[vehicleid];
    
    if(!vehicleDBId)
        return true;

    if(!strmatch(playerInfo[playerid][pName], carInfo[vehicleDBId][cOwner]))
    	return true;

    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
    SetVehicleParamsEx(vehicleid, engine, lights, alarm, (carInfo[vehicleDBId][cLocked]) ? (0) : (1), bonnet, boot, objective);

    va_GameTextForPlayer(playerid, "~h~%s~n~%s", 4000, 4, aVehicleNames[GetVehicleModel(vehicleid) - 400], (carInfo[vehicleDBId][cLocked]) ? ("~g~unlocked") : ("~p~locked"));

    carInfo[vehicleDBId][cLocked] = carInfo[vehicleDBId][cLocked] ? (0) : (1);
    vehicleInfo[vehicleid][vehLocked] = vehicleInfo[vehicleid][vehLocked] ? (0) : (1);

    SetVehicleParamsForPlayer(vehicleid, playerid, 0, 0);
    return true;
}

check_player_licences(playerid, vehicleid)
{
    if(!IsValidVehicle(vehicleid))
        return true;

    if(vehicleid == playerStaticVar[playerid][examVeh])
        return true;

    new Float:posX, Float:posY, Float:posZ;
    GetPlayerPos(playerid, posX, posY, posZ);

    if(!playerInfo[playerid][pCarLic] && !IsBike(vehicleid) && !IsPlane(vehicleid) && !IsBoat(vehicleid))
    {
        SendClientMessage(playerid, COLOR_GREY, "You don't have a driving license.");

        SetPlayerPos(playerid, posX, posY, posZ + 1.5);
        return true;
    }
    if(!playerInfo[playerid][pFlyLic] && IsPlane(vehicleid))
    {
        SendClientMessage(playerid, COLOR_GREY, "You don't have a flying license.");

        SetPlayerPos(playerid, posX, posY, posZ + 1.5);
        return true;
    }
    if(!playerInfo[playerid][pBoatLic] && IsBoat(vehicleid))
    {
        SendClientMessage(playerid, COLOR_GREY, "You don't have a sailing license.");

        SetPlayerPos(playerid, posX, posY, posZ + 1.5);
        return true;
    }
    return true;
}
hook OnPlayerDisconnect(playerid, reason) {
    if(Iter_Contains(playersInVehicle, playerid))
        Iter_Remove(playersInVehicle, playerid);

    return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
    if(ispassenger)
        return Y_HOOKS_CONTINUE_RETURN_1;

    check_player_licences(playerid, vehicleid);

    playerStaticVar[playerid][enterToVehicle] = vehicleid;
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerExitVehicle(playerid, vehicleid)
{
	playerStaticVar[playerid][lastVehicle] = vehicleid;

	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	switch(newkeys)
	{
		case KEY_LOOK_BEHIND: {
			if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
				return true;

		    if(!vehicleInfo[GetPlayerVehicleID(playerid)][vehFuel])
		        return sendError(playerid,"This car don't have fuel.");

			if(IsBike(GetPlayerVehicleID(playerid)))
				return sendError(playerid,"Command can not be used on bike.");

			if(GetPVarInt(playerid, "refuel_veh"))
				return sendError(playerid,"Please wait, you refill your vehicle.");

			new bool:engine, bool:lights, bool:alarm, bool:doors, bool:bonnet, bool:boot, bool:objective;
			switch(vehicleInfo[GetPlayerVehicleID(playerid)][vehEngine])
			{
				case 0:
				{
					vehicleInfo[GetPlayerVehicleID(playerid)][vehEngine] = true;

					GetVehicleParamsEx(GetPlayerVehicleID(playerid), engine, lights, alarm, doors, bonnet, boot, objective);
			        SetVehicleParamsEx(GetPlayerVehicleID(playerid), VEHICLE_PARAMS_ON, lights, alarm, doors, bonnet, boot, objective);
				}
				case 1:
				{
					vehicleInfo[GetPlayerVehicleID(playerid)][vehEngine] = false;

					GetVehicleParamsEx(GetPlayerVehicleID(playerid), engine, lights, alarm, doors, bonnet, boot, objective);
			        SetVehicleParamsEx(GetPlayerVehicleID(playerid), VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);
				}
			}
			sendNearbyMessage(playerid, COLOR_PURPLE, "* %s %s the engine of %s.", (playerUndercover[playerid] ? ("An unknown hitman") : GetName(playerid)), (vehicleInfo[GetPlayerVehicleID(playerid)][vehEngine] ? ("starts") : ("stops")), aVehicleNames[GetVehicleModel(GetPlayerVehicleID(playerid)) - 400]);
		}
		case KEY_ACTION: {
			if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
		        return true;

		    new bool:engine, bool:lights, bool:alarm, bool:doors, bool:bonnet, bool:boot, bool:objective;
			GetVehicleParamsEx(GetPlayerVehicleID(playerid), engine, lights, alarm, doors, bonnet, boot, objective);
			SetVehicleParamsEx(GetPlayerVehicleID(playerid), engine, vehicleInfo[GetPlayerVehicleID(playerid)][vehLights] ? VEHICLE_PARAMS_OFF : VEHICLE_PARAMS_ON, alarm, doors, bonnet, boot, objective);
			
			vehicleInfo[GetPlayerVehicleID(playerid)][vehLights] = vehicleInfo[GetPlayerVehicleID(playerid)][vehLights] ? 0 : 1;
		}
		case KEY_NO: {
		    new bool:engine, bool:lights, bool:alarm, bool:doors, bool:bonnet, bool:boot, bool:objective, vehicleid = (!IsPlayerInAnyVehicle(playerid) ? GetClosestVehicle(playerid, 5.0) : GetPlayerVehicleID(playerid));

		    if(!IsValidVehicle(vehicleid))
		        return true;

		    new vehicleDBId = VehicleSQL[vehicleid];
		    
		    if(!vehicleDBId)
		        return true;

		    if(!strmatch(playerInfo[playerid][pName], carInfo[vehicleDBId][cOwner]))
		    	return true;

		    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
		    SetVehicleParamsEx(vehicleid, engine, lights, alarm, (carInfo[vehicleDBId][cLocked]) ? (0) : (1), bonnet, boot, objective);

		    va_GameTextForPlayer(playerid, "~h~%s~n~%s", 4000, 4, aVehicleNames[GetVehicleModel(vehicleid) - 400], (carInfo[vehicleDBId][cLocked]) ? ("~g~unlocked") : ("~p~locked"));

		    carInfo[vehicleDBId][cLocked] = carInfo[vehicleDBId][cLocked] ? (0) : (1);
		    vehicleInfo[vehicleid][vehLocked] = vehicleInfo[vehicleid][vehLocked] ? (0) : (1);

		    SetVehicleParamsForPlayer(vehicleid, playerid, 0, 0);
		}
        case KEY_CTRL_BACK: {
		    if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
		        return true;

		    if(IsBike(GetPlayerVehicleID(playerid)))
		        return true;

		    new radioString[2048] = "Radio Name\tListeners\n";

		    for(new i; i < sizeof RadioNames; i++)
		        format(radioString, sizeof radioString, "%s%s\t%s\n", radioString, RadioNames[i], (i == 0 ? "" : va_return("%d", radioListeners[i])));

		    Dialog_Show(playerid, DIALOG_RADIO, DIALOG_STYLE_TABLIST_HEADERS, "Car Radio:", radioString, "Ok", "Cancel");
		}
	}

	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
		return Y_HOOKS_CONTINUE_RETURN_1;

	if(IsKeyJustDown(KEY_SPRINT, newkeys, oldkeys))
		SetPVarInt(playerid, "SPRINT_KEY", 1);

	if(IsKeyReleased(KEY_SPRINT))
		DeletePVar(playerid, "SPRINT_KEY");

	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect@0001(playerid)
{

    playerTextdraws[playerid][SpeedTD] = CreatePlayerTextDraw(playerid, 540.601440, 316.077758, "---");
    PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][SpeedTD], 0.400000, 1.600000);
    PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][SpeedTD], 3);
    PlayerTextDrawColour(playerid, playerTextdraws[playerid][SpeedTD], -1);
    PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][SpeedTD], 0);
    PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][SpeedTD], 1);
    PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][SpeedTD], 255);
    PlayerTextDrawFont(playerid, playerTextdraws[playerid][SpeedTD], 3);
    PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][SpeedTD], 1);
    PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][SpeedTD], 0);


    playerTextdraws[playerid][kmTD] = CreatePlayerTextDraw(playerid, 542.243225, 319.866668, "km/h");
    PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][kmTD], 0.249999, 1.176887);
    PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][kmTD], 1);
    PlayerTextDrawColour(playerid, playerTextdraws[playerid][kmTD], -1);
    PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][kmTD], 0);
    PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][kmTD], 1);
    PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][kmTD], 255);
    PlayerTextDrawFont(playerid, playerTextdraws[playerid][kmTD], 1);
    PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][kmTD], 1);
    PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][kmTD], 0);

    playerTextdraws[playerid][speedometerTD][1] = CreatePlayerTextDraw(playerid, 514.667968, 330.877929, "-- --");
    PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][speedometerTD][1], 0.200499, 0.991554);
    PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][speedometerTD][1], 1);
    PlayerTextDrawColour(playerid, playerTextdraws[playerid][speedometerTD][1], -1);
    PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][speedometerTD][1], 0);
    PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][speedometerTD][1], 1);
    PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][speedometerTD][1], 255);
    PlayerTextDrawFont(playerid, playerTextdraws[playerid][speedometerTD][1], 2);
    PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][speedometerTD][1], 1);
    PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][speedometerTD][1], 0);

    playerTextdraws[playerid][speedometerTD][0] = CreatePlayerTextDraw(playerid, 506.045806, 341.517913, "SPEED: ~p~--- ~w~KM/H~n~~w~FUEL: ~p~---~n~~w~RADIO: ~p~None");
    PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][speedometerTD][0], 0.212472, 1.322498);
    PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][speedometerTD][0], 700.000000, 0.000000);
    PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][speedometerTD][0], 1);
    PlayerTextDrawColour(playerid, playerTextdraws[playerid][speedometerTD][0], -1);
    PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][speedometerTD][0], 0);
    PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][speedometerTD][0], 1);
    PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][speedometerTD][0], 255);
    PlayerTextDrawFont(playerid, playerTextdraws[playerid][speedometerTD][0], 2);
    PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][speedometerTD][0], 1);

    return Y_HOOKS_CONTINUE_RETURN_1;
}
function:AntiHackWarp(playerid) entercars[playerid]=0;
public OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
    if(newstate == PLAYER_STATE_DRIVER)
        check_player_licences(playerid, GetPlayerVehicleID(playerid));
    
    switch(newstate)
    {
        case PLAYER_STATE_DRIVER:
        {
            if(vehicleRadio[GetPlayerVehicleID(playerid)])
            {
                StopAudioStreamForPlayer(playerid);
                PlayAudioStreamForPlayer(playerid, RadioLinks[vehicleRadio[GetPlayerVehicleID(playerid)]]);
            }
            Iter_Add(playersInVehicle, playerid);
            SetPlayerArmedWeapon(playerid, WEAPON_FIST);
        }
        case PLAYER_STATE_PASSENGER:
        {
            if(vehicleRadio[GetPlayerVehicleID(playerid)])
            {
                StopAudioStreamForPlayer(playerid);
                PlayAudioStreamForPlayer(playerid, RadioLinks[vehicleRadio[GetPlayerVehicleID(playerid)]]);
            }
            Iter_Add(playersInVehicle, playerid);
            playerStaticVar[playerid][enterToVehicle] = GetPlayerVehicleID(playerid);

        }
        case PLAYER_STATE_ONFOOT:
        {
            PlayerTextDrawHide(playerid, playerTextdraws[playerid][speedometerTD][playerInfo[playerid][pHUD][7]]);
            PlayerTextDrawHide(playerid, playerTextdraws[playerid][kmTD]);
            PlayerTextDrawHide(playerid, playerTextdraws[playerid][SpeedTD]);
            StopAudioStreamForPlayer(playerid);
            Iter_Remove(playersInVehicle, playerid);
            playerStaticVar[playerid][enterToVehicle] = INVALID_VEHICLE_ID;
        }
    }
    new car = GetPlayerVehicleID(playerid);
	if(IsPlayerInAnyVehicle(playerid)) {
		playerStaticVar[playerid][enterToVehicle] = car;
		entercars[playerid]++;
		SetTimerEx("AntiHackWarp", 1000, false,"d", playerid);
		if(entercars[playerid] >= 2) {
            catchRaccoon(playerid, ID_VEHICLE_SYNC, INVALID_VEHICLE_MANIPULATION, 20, 2);
		}
		new Float:carhp;GetVehicleHealth(car, carhp);
		if(carhp <= 1.0 && IsValidVehicle(car) && !IsVehicleUpsideDown(car)) {
			SetVehicleHealth(car, 999.0);
            catchRaccoon(playerid, ID_VEHICLE_SYNC, INVALID_VEHICLE_MANIPULATION, 20, 1);
		}
	}
    return Y_HOOKS_CONTINUE_RETURN_1;
}

GetCurrentVehSpeed(const vehicleId)
{
    new Float:returnVel[3];
    GetVehicleVelocity(vehicleId, returnVel[0], returnVel[1], returnVel[2]);
   
    return floatround(((floatsqroot(((returnVel[0] * returnVel[0]) + (returnVel[1] * returnVel[1]) + (returnVel[2] * returnVel[2]))) * 180.0)) * 1);
}

task updateVehicleInformations[20000]()
{
	foreach(new iPlayer : playersInVehicle)
    {
    	if(!vehicleInfo[GetPlayerVehicleID(iPlayer)][vehEngine] || GetCurrentVehSpeed(GetPlayerVehicleID(iPlayer)) < 5)
    		continue;

    	if(GetPlayerState(iPlayer) != PLAYER_STATE_DRIVER)
    		continue;

    	if(vehicleInfo[GetPlayerVehicleID(iPlayer)][vehFuel] > 0)
    	{
			vehicleInfo[GetPlayerVehicleID(iPlayer)][vehFuel] --;
    	}
		else
		{
			TogglePlayerControllable(iPlayer, false);
			GameTextForPlayer(iPlayer, "~w~~n~~n~~n~~n~~n~~n~~n~~n~no fuel in vehicle.", 2000, 3);
		}

		new vehicleDBId = VehicleSQL[GetPlayerVehicleID(iPlayer)];
		if(vehicleDBId)
		{
        	carInfo[vehicleDBId][cKM] ++;
        	carInfo[vehicleDBId][cFuel] = vehicleInfo[GetPlayerVehicleID(iPlayer)][vehFuel];

            DailyQuestCheck(iPlayer, QUEST_TYPE_VEHKM, 1);
		}
	}
	return true;
}


stock ModifyVehicleSpeed(vehicleid, mph)
{
    new Float:Vx, Float:Vy, Float:Vz, Float:DV, Float:multiple;
    GetVehicleVelocity(vehicleid, Vx, Vy, Vz);
    DV = floatsqroot(Vx * Vx + Vy * Vy + Vz * Vz);

    if(DV > 0) //Directional velocity must be greater than 0 (display strobes if 0)
    {
        multiple = ((mph + DV * 100) / (DV * 100)); //Multiplying DV by 100 calculates speed in MPH
        return SetVehicleVelocity(vehicleid, Vx * multiple, Vy * multiple, Vz * multiple);
    }
    return 0;
}

task checkInfoAboutVehicle[1001]()
{
	new speedString[256];
	foreach(new iPlayer : playersInVehicle)
    {
        if(!playerInfo[iPlayer][pHUD][10])
            continue;

        new vehicleDBId = VehicleSQL[GetPlayerVehicleID(iPlayer)];

        if(GetCurrentVehSpeed(GetPlayerVehicleID(iPlayer)) > GetVehicleMaxSpeed(GetVehicleModel(GetPlayerVehicleID(iPlayer))) + 40){
            if(playerInfo[iPlayer][pAdmin] > 0) return true;
            sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "%s(%d) is suspected of using Speed-Hack | Speed: %d | Max Vehicle Speed: %d", GetName(iPlayer), iPlayer, GetCurrentVehSpeed(GetPlayerVehicleID(iPlayer)), GetVehicleMaxSpeed(GetVehicleModel(GetPlayerVehicleID(iPlayer))));
        }

        if(playerInfo[iPlayer][pHUD][7] == 0)
        {
    		format(speedString, sizeof speedString, "SPEED: ~p~%d ~w~~h~KM/H~n~", GetCurrentVehSpeed(GetPlayerVehicleID(iPlayer)));

            if(vehicleInfo[GetPlayerVehicleID(iPlayer)][vehFuel] < 20){
                if(IsBike(GetPlayerVehicleID(iPlayer)))
                {
                    format(speedString, sizeof speedString, "%s", speedString);
                }
                else {
                    format(speedString, sizeof speedString, "%sFUEL: ~p~%d~n~", speedString, vehicleInfo[GetPlayerVehicleID(iPlayer)][vehFuel]);
                }
            }
    		else {
                if(IsBike(GetPlayerVehicleID(iPlayer)))
                {
                    format(speedString, sizeof speedString, "%s", speedString);
                }
                else {
                    format(speedString, sizeof speedString, "%sFUEL: ~p~%d~n~", speedString, vehicleInfo[GetPlayerVehicleID(iPlayer)][vehFuel]);
                }
            }
            if(IsBike(GetPlayerVehicleID(iPlayer)))
            {
                format(speedString, sizeof speedString, "%s", speedString);
            }
            else {
                format(speedString, sizeof speedString, "%s~w~~h~RADIO: ~p~%s~n~", speedString, (vehicleRadio[GetPlayerVehicleID(iPlayer)]) ? RadioNames[vehicleRadio[GetPlayerVehicleID(iPlayer)]] : "none");
            }
    		if(vehicleDBId)
            {
                if(!IsPlane(GetPlayerVehicleID(iPlayer)))
                    format(speedString, sizeof speedString, "%s~w~~h~ODOMETER: ~p~%.0fKM~n~", speedString, carInfo[vehicleDBId][cKM]);
                
                format(speedString, sizeof speedString, "%s%s", speedString, carInfo[vehicleDBId][cLocked] ? ("~p~LOCKED") : ("~p~~h~UNLOCKED"));
            }
        }
        else
        {
            va_PlayerTextDrawSetString(iPlayer, playerTextdraws[iPlayer][SpeedTD], "%d", GetCurrentVehSpeed(GetPlayerVehicleID(iPlayer)));
            if(IsBike(GetPlayerVehicleID(iPlayer)))
            {
                format(speedString, sizeof speedString, "");
            }
            else {
                format(speedString, sizeof speedString, "Radio: ~p~%s", (vehicleRadio[GetPlayerVehicleID(iPlayer)]) ? RadioNames[vehicleRadio[GetPlayerVehicleID(iPlayer)]] : "none");
            }
            if(IsBike(GetPlayerVehicleID(iPlayer)))
            {
                format(speedString, sizeof speedString, "%s", speedString);
            }
            else {
                format(speedString, sizeof speedString, "%s~N~~W~~H~Fuel: ~p~%d", speedString, vehicleInfo[GetPlayerVehicleID(iPlayer)][vehFuel]);
            }
            if(vehicleDBId)
            {
                if(!IsPlane(GetPlayerVehicleID(iPlayer)))
                    format(speedString, sizeof speedString, "%s~n~~w~~h~Odometer: ~p~%.0fKM~n~~p~%s", speedString, carInfo[vehicleDBId][cKM], carInfo[vehicleDBId][cLocked] ? ("~p~Locked") : ("~p~Unlocked"));
            }
            PlayerTextDrawShow(iPlayer, playerTextdraws[iPlayer][kmTD]);
            PlayerTextDrawShow(iPlayer, playerTextdraws[iPlayer][SpeedTD]);
        }
		PlayerTextDrawSetString(iPlayer, playerTextdraws[iPlayer][speedometerTD][playerInfo[iPlayer][pHUD][7]], speedString);
		PlayerTextDrawShow(iPlayer, playerTextdraws[iPlayer][speedometerTD][playerInfo[iPlayer][pHUD][7]]);
    }
    return true;
}