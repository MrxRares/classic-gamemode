#include <YSI_Coding\y_hooks>

#define MAX_APARTAMENTS 25
#define STREAMER_BEGIN_APARTAMENT 500

enum ENUM_AP_INFROMATIONS
{
	apID, apOwner, Float:apExterior[3], apSphere, Text3D:apLabel, apLocked, apOwnerName[MAX_PLAYER_NAME], apDoorObj
};
new apInfo[MAX_APARTAMENTS][ENUM_AP_INFROMATIONS], returnServerApartaments, Timer: timerCloseApDoor[MAX_APARTAMENTS];

hook loadServerSystems()
{
	mysql_tquery(SQL, "select * from `apartaments`", "loadApartamentsFromInclude", "");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

GetApFloorLevel(apartId)
{
	new floorId;
	switch(apartId)
	{
		case 24, 23: floorId = 1;
		case 22, 21: floorId = 2;
		case 20, 19: floorId = 3;
		case 18, 17: floorId = 4;
		case 16, 15: floorId = 5;
		case 14, 13: floorId = 6;
		case 12, 11: floorId = 7;
		case 10, 9: floorId = 8;
		case 8, 7: floorId = 9;
		case 6, 5: floorId = 10;
		case 4, 3: floorId = 11;
		case 2, 1: floorId = 12;
		default: floorId = -1;
	}
	return floorId;
}

hook WhenPlayerLoggedIn(playerid)
{
	if(!cache_num_rows())
		return Y_HOOKS_CONTINUE_RETURN_1;

	cache_get_value_name_int(0, "Apartament", playerInfo[playerid][pApartament]);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

function:loadApartamentsFromInclude()
{
	returnServerApartaments = cache_num_rows();
	for(new i = 1; i <= cache_num_rows(); i++)
	{
		cache_get_value_name_int(i-1, "ID", apInfo[i][apID]);
		cache_get_value_name_int(i-1, "OwnerSQL", apInfo[i][apOwner]);
  		cache_get_value_name_int(i-1, "Locked", apInfo[i][apLocked]);

  		cache_get_value_name_float(i-1, "ExteriorX", apInfo[i][apExterior][0]);
  		cache_get_value_name_float(i-1, "ExteriorY", apInfo[i][apExterior][1]);
  		cache_get_value_name_float(i-1, "ExteriorZ", apInfo[i][apExterior][2]);

		cache_get_value_name(i-1, "OwnerName", apInfo[i][apOwnerName]);
		
		WhenApartamentGotUpdated(i);

		if(i == 1 || i == 3 || i == 5 || i == 7 || i == 9 || i == 11 || i == 13 || i == 15 || i == 17 || i == 19 || i == 21 || i == 23)
			apInfo[i][apDoorObj] = CreateDynamicObject(1505, 275.55, -1621, 41.5 + (4.5 * GetApFloorLevel(i)), 0.000000, 0.000000, 80, 0, 0); 
		else
			apInfo[i][apDoorObj] = CreateDynamicObject(1505, 295.55, -1623, 41.5 + (4.5 * GetApFloorLevel(i)), 0.000000, 0.000000, 260, 0, 0); 
	}
	printf("Apartaments: %d", returnServerApartaments);
	return true;
}

WhenApartamentGotUpdated(apartId)
{
	if(IsValidDynamic3DTextLabel(apInfo[apartId][apLabel])) DestroyDynamic3DTextLabel(apInfo[apartId][apLabel]);
	if(IsValidDynamicArea(apInfo[apartId][apSphere])) DestroyDynamicArea(apInfo[apartId][apSphere]);

	format(returnString, sizeof returnString, "Apartament "COLOR_SV_HEX"%d{ffffff}\nOwner: "COLOR_SV_HEX"%s{ffffff}\nDoor Status: %s", apartId, apInfo[apartId][apOwnerName], (apInfo[apartId][apLocked]) ? ("{ED3D22}Locked"):("{1BED30}Unlocked"));
	apInfo[apartId][apLabel] = CreateDynamic3DTextLabel(returnString, -1, apInfo[apartId][apExterior][0], apInfo[apartId][apExterior][1], apInfo[apartId][apExterior][2], 20, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);

	apInfo[apartId][apSphere] = CreateDynamicSphere(apInfo[apartId][apExterior][0], apInfo[apartId][apExterior][1], apInfo[apartId][apExterior][2], 2, 0, 0, -1);
	return Streamer_SetIntData(STREAMER_TYPE_AREA, apInfo[apartId][apSphere], E_STREAMER_EXTRA_ID, (apartId + STREAMER_BEGIN_APARTAMENT));
}

hook OnPlayerEnterDynArea(playerid, areaid)
{
	new apartId = Streamer_GetIntData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID) - STREAMER_BEGIN_APARTAMENT;
	if(returnServerApartaments < apartId)
		return Y_HOOKS_CONTINUE_RETURN_1;

	SetPVarInt(playerid, "ap_areaID", apartId);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerLeaveDynArea(playerid, areaid)
{
    SetPVarInt(playerid, "ap_areaID", 0);
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook WhenPlayerSpawn(playerid)
{
	if(playerInfo[playerid][pSpawnLocation] != 2)
		return Y_HOOKS_CONTINUE_RETURN_1;

	if(!playerInfo[playerid][pApartament])
		return SetPlayerPos(playerid, 1803.0000, -1868.0000, 13.5781);

	new i = playerInfo[playerid][pApartament], Float:a;
	if(i == 1 || i == 3 || i == 5 || i == 7 || i == 9 || i == 11 || i == 13 || i == 15 || i == 17 || i == 19 || i == 21 || i == 23)
		SetPlayerFacingAngle(playerid, 80), a = 80;
	else
		SetPlayerFacingAngle(playerid, 260), a = 260;

	new Float: fX = apInfo[i][apExterior][0], Float: fY = apInfo[i][apExterior][1];
	fX += (2 * floatsin(-a, degrees));
	fY += (2 * floatcos(-a, degrees));

	SetPlayerPos(playerid, fX, fY, apInfo[i][apExterior][2]);
	SetPlayerVirtualWorld(playerid, 0);
	SetPlayerInterior(playerid, 0);
	return Y_HOOKS_CONTINUE_RETURN_0;
}

#define ELEVATOR_SPEED      (5.0)
#define DOORS_SPEED         (5.0)

#define ELEVATOR_WAIT_TIME  (5000)  

// Position defines
#define X_DOOR_R_OPENED     (289.542419)
#define X_DOOR_L_OPENED     (286.342407)
#define Y_DOOR_R_OPENED     (-1609.640991)
#define Y_DOOR_L_OPENED     (-1609.076049)

#define X_FDOOR_R_OPENED    (289.492431)
#define X_FDOOR_L_OPENED    (286.292419)
#define Y_FDOOR_R_OPENED    (-1609.870971)
#define Y_FDOOR_L_OPENED    (-1609.306030)

#define GROUND_Z_COORD      (18.755348)     // (33.825077)
#define X_ELEVATOR_POS      (287.942413)
#define Y_ELEVATOR_POS      (-1609.341064)

// Elevator state defines
#define ELEVATOR_STATE_IDLE     (0)
#define ELEVATOR_STATE_WAITING  (1)
#define ELEVATOR_STATE_MOVING   (2)

static FloorNames[14][] =
{
	"Car Park",
	"Ground Floor",
	"First Floor",
	"Second Floor",
	"Third Floor",
	"Fourth Floor",
	"Fifth Floor",
	"Sixth Floor",
	"Seventh Floor",
	"Eighth Floor",
	"Ninth Floor",
	"Tenth Floor",
	"Eleventh Floor",
	"Twelfth Floor"
};

// Elevator floor Z heights
static Float:FloorZOffsets[14] =
{
    0.0, 		// Car Park
    15.069729,  // Ground Floor
    29.130733,	// First Floor
    33.630733,  // Second Floor = 29.130733 + 4.5
    38.130733,  // Third Floor = 33.630733 + 4.5
    42.630733,  // Fourth Floor = 38.130733 + 4.5
    47.130733,  // Fifth Floor = 42.630733 + 4.5
    51.630733,  // Sixth Floor = 47.130733 + 4.5
    56.130733,  // Seventh Floor = 51.630733 + 4.5
    60.630733,  // Eighth Floor = 56.130733 + 4.5
    65.130733,  // Ninth Floor = 60.630733 + 4.5
    69.630733,  // Tenth Floor = 65.130733 + 4.5
    74.130733,  // Eleventh Floor = 69.630733 + 4.5
    78.630733,  // Twelfth Floor = 74.130733 + 4.5
};

new Obj_Elevator, Obj_ElevatorDoors[2], Obj_FloorDoors[14][2], Text3D:Label_Elevator, Text3D:Label_Floors[14], ElevatorState, ElevatorFloor,
	ElevatorQueue[14], FloorRequestedBy[14], Timer:ElevatorBoostTimer;

forward Float:GetElevatorZCoordForFloor(floorid);
forward Float:GetDoorsZCoordForFloor(floorid);

hook OnPlayerConnect(playerid)
{
	RemoveBuildingForPlayer(playerid, 1226, 265.481, -1581.1, 32.9311, 5.0);
	RemoveBuildingForPlayer(playerid, 6518, 280.297, -1606.2, 72.3984, 250.0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

Dialog:DIALOG_ID(playerid, response, listitem, inputtext[])
{
    if(!response)
        return false;

    if(FloorRequestedBy[listitem] != INVALID_PLAYER_ID)
        GameTextForPlayer(playerid, "~r~The floor is already in the queue", 3500, 4);

	else if(DidPlayerRequestElevator(playerid))
	    GameTextForPlayer(playerid, "~r~You already requested the elevator", 3500, 4);

	else
        CallElevator(playerid, listitem);

	return true;
}

timer closeApartamentDoor[5000](i, Float:fX, Float:fY, Float:fZ)
{
	if(i == 1 || i == 3 || i == 5 || i == 7 || i == 9 || i == 11 || i == 13 || i == 15 || i == 17 || i == 19 || i == 21 || i == 23)
		MoveDynamicObject(apInfo[i][apDoorObj], fX, fY, fZ, 10.0, 0.0, 0.0, 80);
	else
		MoveDynamicObject(apInfo[i][apDoorObj], fX, fY, fZ, 10.0, 0.0, 0.0, 260);

	timerCloseApDoor[i] = Timer:0;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
	if(newkeys == KEY_SECONDARY_ATTACK)
	{
		if(IsPlayerInAnyDynamicArea(playerid) && GetPVarInt(playerid, "ap_areaID") > 0 && GetPVarInt(playerid, "ap_areaID") <= returnServerApartaments)
		{
			new i = GetPVarInt(playerid, "ap_areaID"), Float:doorPos[3];

			if(timerCloseApDoor[i] == Timer:0)
			{	
				if(!apInfo[i][apLocked] || apInfo[i][apLocked] && apInfo[i][apOwner] == playerInfo[playerid][pSQLID])
				{
					GetDynamicObjectPos(apInfo[i][apDoorObj], doorPos[0], doorPos[1], doorPos[2]);

					if(i == 1 || i == 3 || i == 5 || i == 7 || i == 9 || i == 11 || i == 13 || i == 15 || i == 17 || i == 19 || i == 21 || i == 23)
						MoveDynamicObject(apInfo[i][apDoorObj], doorPos[0], doorPos[1], doorPos[2], 10.0, 0.0, 0.0, -8);
					else
						MoveDynamicObject(apInfo[i][apDoorObj], doorPos[0], doorPos[1], doorPos[2], 10.0, 0.0, 0.0, -190);

					timerCloseApDoor[i] = defer closeApartamentDoor(i, doorPos[0], doorPos[1], doorPos[2]);
				}
			}
		}
	}
	if(newkeys & KEY_YES && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
	{
	    new Float:pos[3];
	    GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	    
	    if(pos[1] > (Y_ELEVATOR_POS - 1.8) && pos[1] < (Y_ELEVATOR_POS + 1.8) && pos[0] < (X_ELEVATOR_POS + 1.8) && pos[0] > (X_ELEVATOR_POS - 1.8))
	    {
	        new string[256];
	        for(new i; i < sizeof(ElevatorQueue); i ++)
			{
			    if(FloorRequestedBy[i] != INVALID_PLAYER_ID)
			        strcat(string, "{FF0000}");

			    strcat(string, FloorNames[i]);
			    strcat(string, "\n");
			}
            Dialog_Show(playerid, DIALOG_ID, DIALOG_STYLE_LIST, "LS BeachSide Elevator", string, "Accept", "Cancel");
	    }
		else
		{
		    if(pos[1] < (Y_ELEVATOR_POS - 1.81) && pos[1] > (Y_ELEVATOR_POS - 3.8) && pos[0] > (X_ELEVATOR_POS + 1.21) && pos[0] < (X_ELEVATOR_POS + 3.8))
		    {
				new i = 13;
				while(pos[2] < GetDoorsZCoordForFloor(i) + 3.5 && i > 0)
				    i --;

				if(i == 0 && pos[2] < GetDoorsZCoordForFloor(0) + 2.0)
				    i = -1;

				if(i <= 12)
				{
				    if(ElevatorState != ELEVATOR_STATE_MOVING && ElevatorFloor == i + 1)
				    {
						GameTextForPlayer(playerid, "~n~~n~~n~~n~~n~~n~~n~~y~~h~LS BeachSide Elevator Is~n~~y~~h~Already On This Floor...~n~~w~Walk Inside It~n~~w~And Press '~k~~CONVERSATION_YES~'", 3500, 3);
	                    SendClientMessage(playerid, 0x0077EDAA, "* The LS BeachSide elevator is already on this floor... walk inside it and press '{FFFFFF}~k~~CONVERSATION_YES~{CCCCCC}'");
	                    return true;
				    }

					CallElevator(playerid, i + 1);
					GameTextForPlayer(playerid, "~n~~n~~n~~n~~n~~n~~n~~n~~g~~h~LS BeachSide Elevator~n~~g~~h~Has Been Called...~n~~w~Please Wait", 3000, 3);

					if(ElevatorState == ELEVATOR_STATE_MOVING)
						va_SendClientMessage(playerid, 0x0077EDAA, "* The LS BeachSide elevator has been called... it is currently moving towards the %s.", FloorNames[ElevatorFloor]);

					else
						va_SendClientMessage(playerid, 0x0077EDAA, "* The LS BeachSide elevator has been called... it is currently at the %s.", FloorNames[ElevatorFloor]);

					return true;
				}
		    }
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

// ------------------------ Functions ------------------------
stock Elevator_Initialize()
{
	Obj_Elevator = CreateDynamicObject(18755, X_ELEVATOR_POS, Y_ELEVATOR_POS, GROUND_Z_COORD, 0.000000, 0.000000, 80.000000);
	Obj_ElevatorDoors[0] = CreateDynamicObject(18757, X_ELEVATOR_POS, Y_ELEVATOR_POS, GROUND_Z_COORD, 0.000000, 0.000000, 80.000000);
	Obj_ElevatorDoors[1] = CreateDynamicObject(18756, X_ELEVATOR_POS, Y_ELEVATOR_POS, GROUND_Z_COORD, 0.000000, 0.000000, 80.000000);

	Label_Elevator = CreateDynamic3DTextLabel("{CCCCCC}Press '{FFFFFF}Y{CCCCCC}' to use elevator", 0xCCCCCCAA, X_ELEVATOR_POS + 1.6, Y_ELEVATOR_POS - 1.85, GROUND_Z_COORD - 0.4, 4.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);

	new string[128], Float:z;
	for (new i; i < sizeof(Obj_FloorDoors); i ++)
	{
	    Obj_FloorDoors[i][0] = CreateDynamicObject(18757, X_ELEVATOR_POS, Y_ELEVATOR_POS - 0.245, GetDoorsZCoordForFloor(i) + 0.05, 0.000000, 0.000000, 80.000000);
		Obj_FloorDoors[i][1] = CreateDynamicObject(18756, X_ELEVATOR_POS, Y_ELEVATOR_POS - 0.245, GetDoorsZCoordForFloor(i) + 0.05, 0.000000, 0.000000, 80.000000);

		z = GetDoorsZCoordForFloor(i);

		format(string, sizeof(string), "{CCCCCC}[%s]\n{CCCCCC}Press '{FFFFFF}Y{CCCCCC}' to call", FloorNames[i]);
		Label_Floors[i] = CreateDynamic3DTextLabel(string, 0xCCCCCCAA, X_ELEVATOR_POS + 2, Y_ELEVATOR_POS -3, z - 0.2, 10.5, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);
	}
	Floor_OpenDoors(0);
	Elevator_OpenDoors();
	return true;
}

stock Elevator_OpenDoors()
{
	new Float:x, Float:y, Float:z;
	GetDynamicObjectPos(Obj_ElevatorDoors[0], x, y, z);

	MoveDynamicObject(Obj_ElevatorDoors[0], X_DOOR_L_OPENED, Y_DOOR_L_OPENED, z, DOORS_SPEED);
	MoveDynamicObject(Obj_ElevatorDoors[1], X_DOOR_R_OPENED, Y_DOOR_R_OPENED, z, DOORS_SPEED);
	return true;
}

stock Elevator_CloseDoors()
{
    if(ElevatorState == ELEVATOR_STATE_MOVING)
	    return false;

    new Float:x, Float:y, Float:z;
	GetDynamicObjectPos(Obj_ElevatorDoors[0], x, y, z);

	MoveDynamicObject(Obj_ElevatorDoors[0], X_ELEVATOR_POS, Y_ELEVATOR_POS, z, DOORS_SPEED);
	MoveDynamicObject(Obj_ElevatorDoors[1], X_ELEVATOR_POS, Y_ELEVATOR_POS, z, DOORS_SPEED);
	return true;
}

stock Floor_OpenDoors(floorid)
{
    MoveDynamicObject(Obj_FloorDoors[floorid][0], X_FDOOR_L_OPENED, Y_FDOOR_L_OPENED, GetDoorsZCoordForFloor(floorid) + 0.05, DOORS_SPEED);
	MoveDynamicObject(Obj_FloorDoors[floorid][1], X_FDOOR_R_OPENED, Y_FDOOR_R_OPENED, GetDoorsZCoordForFloor(floorid) + 0.05, DOORS_SPEED);
	return true;
}

stock Floor_CloseDoors(floorid)
{
    MoveDynamicObject(Obj_FloorDoors[floorid][0], X_ELEVATOR_POS, Y_ELEVATOR_POS - 0.245, GetDoorsZCoordForFloor(floorid) + 0.05, DOORS_SPEED);
	MoveDynamicObject(Obj_FloorDoors[floorid][1], X_ELEVATOR_POS, Y_ELEVATOR_POS - 0.245, GetDoorsZCoordForFloor(floorid) + 0.05, DOORS_SPEED);
	return true;
}

timer elevator_boost[2000](floorid)
{
	StopDynamicObject(Obj_Elevator);
	StopDynamicObject(Obj_ElevatorDoors[0]);
	StopDynamicObject(Obj_ElevatorDoors[1]);
	
	MoveDynamicObject(Obj_Elevator, X_ELEVATOR_POS, Y_ELEVATOR_POS, GetElevatorZCoordForFloor(floorid), ELEVATOR_SPEED);
    MoveDynamicObject(Obj_ElevatorDoors[0], X_ELEVATOR_POS, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(floorid), ELEVATOR_SPEED);
    MoveDynamicObject(Obj_ElevatorDoors[1], X_ELEVATOR_POS, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(floorid), ELEVATOR_SPEED);

    ElevatorBoostTimer = Timer:0;
}
timer elevator_turn_to_idle[ELEVATOR_WAIT_TIME]()
{
	ElevatorState = ELEVATOR_STATE_IDLE;
	ReadNextFloorInQueue();
}

stock Elevator_MoveToFloor(floorid)
{
	ElevatorState = ELEVATOR_STATE_MOVING;
	ElevatorFloor = floorid;

	MoveDynamicObject(Obj_Elevator, X_ELEVATOR_POS, Y_ELEVATOR_POS, GetElevatorZCoordForFloor(floorid), 0.25);
    MoveDynamicObject(Obj_ElevatorDoors[0], X_ELEVATOR_POS, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(floorid), 0.25);
    MoveDynamicObject(Obj_ElevatorDoors[1], X_ELEVATOR_POS, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(floorid), 0.25);
    
    DestroyDynamic3DTextLabel(Label_Elevator);
	ElevatorBoostTimer = defer elevator_boost(floorid);
	return true;
}

hook OnDynamicObjectMoved(objectid)
{
    new Float:x, Float:y, Float:z;
	for(new i; i < sizeof(Obj_FloorDoors); i ++)
	{
		if(objectid == Obj_FloorDoors[i][0])
		{
		    GetDynamicObjectPos(Obj_FloorDoors[i][0], x, y, z);
            if(y < Y_DOOR_L_OPENED - 0.5)
		    {
				Elevator_MoveToFloor(ElevatorQueue[0]);
				RemoveFirstQueueFloor();
			}
		}
	}
	if(objectid == Obj_Elevator)
	{
		stop ElevatorBoostTimer;
	    ElevatorBoostTimer = Timer:0;

	    FloorRequestedBy[ElevatorFloor] = INVALID_PLAYER_ID;

	    Elevator_OpenDoors();
	    Floor_OpenDoors(ElevatorFloor);

	    GetDynamicObjectPos(Obj_Elevator, x, y, z);
	    Label_Elevator = CreateDynamic3DTextLabel("{CCCCCC}Press '{FFFFFF}Y{CCCCCC}' to use elevator", 0xCCCCCCAA, X_ELEVATOR_POS + 1.6, Y_ELEVATOR_POS - 1.85, z - 0.4, 4.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);

	    ElevatorState = ELEVATOR_STATE_WAITING;
	    defer elevator_turn_to_idle();
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

stock RemoveFirstQueueFloor()
{
	for(new i; i < sizeof(ElevatorQueue) - 1; i ++)
	    ElevatorQueue[i] = ElevatorQueue[i + 1];

	ElevatorQueue[sizeof(ElevatorQueue) - 1] = -1;
	return true;
}

stock AddFloorToQueue(floorid)
{
	new slot = -1;
	for(new i; i < sizeof(ElevatorQueue); i ++)
	{
	    if(ElevatorQueue[i] == -1)
	    {
	        slot = i;
	        break;
	    }
	}
	if(slot != -1)
	{
	    ElevatorQueue[slot] = floorid;

	    if(ElevatorState == ELEVATOR_STATE_IDLE)
	        ReadNextFloorInQueue();

	    return true;
	}
	return false;
}

stock ResetElevatorQueue()
{
	for(new i; i < sizeof(ElevatorQueue); i ++)
	{
	    ElevatorQueue[i] = -1;
	    FloorRequestedBy[i] = INVALID_PLAYER_ID;
	}
	return true;
}

stock IsFloorInQueue(floorid)
{
	for(new i; i < sizeof(ElevatorQueue); i ++)
	    if(ElevatorQueue[i] == floorid)
	        return true;

	return false;
}

stock ReadNextFloorInQueue()
{
	if(ElevatorState != ELEVATOR_STATE_IDLE || ElevatorQueue[0] == -1)
	    return false;

	Elevator_CloseDoors();
	Floor_CloseDoors(ElevatorFloor);
	return true;
}

stock DidPlayerRequestElevator(playerid)
{
	for(new i; i < sizeof(FloorRequestedBy); i ++)
	    if(FloorRequestedBy[i] == playerid)
	        return true;

	return false;
}

stock CallElevator(playerid, floorid)
{
	if(FloorRequestedBy[floorid] != INVALID_PLAYER_ID || IsFloorInQueue(floorid))
	    return false;

	FloorRequestedBy[floorid] = playerid;
	AddFloorToQueue(floorid);
	return true;
}

stock Float:GetElevatorZCoordForFloor(floorid)
    return (GROUND_Z_COORD + FloorZOffsets[floorid]);


stock Float:GetDoorsZCoordForFloor(floorid)
	return (GROUND_Z_COORD + FloorZOffsets[floorid]);

hook OnGameModeInit()
{
	ResetElevatorQueue();
	Elevator_Initialize();

	for(new i; i < 12; i ++)
	{
		//AP 1
		CreateDynamicObject(11737, 294.931000, -1623.668823, 45.994598 + (4.5 * i), -0.299900, 0.399900, -99.599899);
		CreateDynamicObject(2165, 304.31979, -1625.00903, 46.00020     + (4.5 * i), 0.00000, 0.00000, -136.80009);
		CreateDynamicObject(2835, 303.97919, -1626.24268, 45.98860     + (4.5 * i), 0.00000, 0.00000, 45.59990);
		CreateDynamicObject(2069, 304.95630, -1624.35522, 46.01990     + (4.5 * i), 0.00000, 0.00000, 0.00000);
		CreateDynamicObject(2356, 303.20370, -1624.57544, 46.01510     + (4.5 * i), 0.00000, 0.00000, -139.50000);
		CreateDynamicObject(2281, 303.95551, -1625.59204, 48.29720     + (4.5 * i), 0.00000, 0.00000, -139.99989);
		CreateDynamicObject(3801, 304.00989, -1617.07581, 48.27640     + (4.5 * i), 0.00000, 0.00000, -102.19990);
		CreateDynamicObject(3801, 305.35880, -1617.36755, 48.27640     + (4.5 * i), 0.00000, 0.00000, -102.19990);
		CreateDynamicObject(2566, 302.86200, -1619.15344, 46.56280     + (4.5 * i), 0.00000, 0.00000, -9.70000);
		CreateDynamicObject(19893, 303.29361, -1616.91113, 46.61960    + (4.5 * i), 0.00000, 0.00000, -9.60000);
		CreateDynamicObject(2267, 304.71329, -1617.03613, 47.70470     + (4.5 * i), 0.00000, 0.00000, -5.99990);
		CreateDynamicObject(2161, 299.93161, -1616.08557, 45.96740     + (4.5 * i), 0.00000, 0.00000, -9.60000);
		CreateDynamicObject(2200, 297.37689, -1615.88037, 45.97490     + (4.5 * i), 0.00000, 0.00000, -11.00000);
		CreateDynamicObject(1723, 295.88589, -1617.74805, 46.12700     + (4.5 * i), 0.29990, 1.00000, -102.40000);
		CreateDynamicObject(1827, 293.645203, -1618.407349, 45.991501  + (4.5 * i), -0.100000, 0.000000, -12.099900);
		CreateDynamicObject(2840, 293.921600, -1618.520752, 46.433899  + (4.5 * i), 0.000000, 0.000000, 0.000000);
		CreateDynamicObject(2321, 291.078613, -1618.645874, 46.025803  + (4.5 * i), 0.000000, 0.000000, 80.900002);
		CreateDynamicObject(2028, 291.16199, -1617.84802, 46.65470     + (4.5 * i), 0.00000, 0.00000, 80.00000);
		CreateDynamicObject(2238, 291.46271, -1616.85596, 46.95130     + (4.5 * i), 0.00000, 0.00000, 0.00000);
		CreateDynamicObject(2238, 291.022400, -1618.917847, 46.951302  + (4.5 * i), 0.000000, 0.000000, 0.000000);
		CreateDynamicObject(2229, 290.98929, -1616.09998, 45.94520     + (4.5 * i), 0.00000, 0.00000, 80.29990);
		CreateDynamicObject(2229, 290.51111, -1618.89844, 45.94520     + (4.5 * i), 0.00000, 0.00000, 80.29990);
		CreateDynamicObject(19786, 290.63071, -1617.79065, 48.21230    + (4.5 * i), -0.10000, -0.20000, 80.30000);
		CreateDynamicObject(19786, 297.40680, -1612.74805, 48.77860    + (4.5 * i), 0.00000, 0.00000, 77.50010);
		CreateDynamicObject(19786, 298.20090, -1608.20081, 48.77860    + (4.5 * i), 0.00000, 0.00000, 77.50010);
		CreateDynamicObject(2241, 291.55350, -1615.10742, 46.51270     + (4.5 * i), -1.59990, -0.49990, -49.99990);
		CreateDynamicObject(2241, 290.87079, -1620.71960, 46.52150     + (4.5 * i), -1.59990, -0.49990, -49.99990);
		CreateDynamicObject(19159, 291.520996, -1618.094360, 50.239899 + (4.5 * i), 0.000000, 0.000000, 0.000000);
		CreateDynamicObject(1713, 307.031708, -1612.053955, 45.978302  + (4.5 * i), 0.000000, 0.000000, -98.999901);
		CreateDynamicObject(2370, 305.064697, -1613.010620, 45.743401  + (4.5 * i), 0.399900, 0.000000, -6.999900);
		CreateDynamicObject(2632, 306.172699, -1605.158813, 45.984200  + (4.5 * i), 0.000000, 0.000000, 80.199997);
		CreateDynamicObject(14651, 305.60181, -1604.59827, 48.22550    + (4.5 * i), 0.00000, 0.00000, 168.89960);
		CreateDynamicObject(2027, 308.27060, -1598.84875, 46.54590     + (4.5 * i), 0.00000, 0.00000, -49.39990);
		CreateDynamicObject(1707, 303.667511, -1597.089355, 45.930496  + (4.5 * i), 0.000000, 0.000000, -10.400000);
		CreateDynamicObject(2799, 300.42319, -1599.10754, 46.48420     + (4.5 * i), 0.00000, 0.00000, 19.00000);
		CreateDynamicObject(19174, 299.96210, -1598.92590, 48.54560    + (4.5 * i), 0.00000, 0.00000, 82.20000);
		CreateDynamicObject(1502, 301.10880, -1616.26367, 45.99680     + (4.5 * i), 0.00000, 0.00000, 0.00000);
		CreateDynamicObject(1502, 298.86380, -1604.16064, 45.97980     + (4.5 * i), 0.00000, 0.00000, 80.59990);
		CreateDynamicObject(1896, 298.92160, -1608.43176, 46.96940     + (4.5 * i), 0.00000, 0.00000, 78.99990);
		CreateDynamicObject(1824, 298.370087, -1612.987549, 46.502098  + (4.5 * i), 0.000000, 0.000000, -97.299896);
		CreateDynamicObject(11686, 299.259491, -1598.095459, 46.042099 + (4.5 * i), 0.000000, 0.000000, -98.199898);
		CreateDynamicObject(19922, 295.381287, -1596.508301, 45.998901 + (4.5 * i), 0.000000, 0.000000, 80.499901);
		CreateDynamicObject(1739, 296.59341, -1597.87537, 46.87080     + (4.5 * i), 0.00000, 0.00000, -9.89990);
		CreateDynamicObject(1739, 293.88699, -1597.38843, 46.87080     + (4.5 * i), 0.00000, 0.00000, 167.00000);
		CreateDynamicObject(1739, 296.84549, -1596.42554, 46.87080     + (4.5 * i), 0.00000, 0.00000, -9.89990);
		CreateDynamicObject(1739, 294.21539, -1595.96533, 46.87080     + (4.5 * i), 0.00000, 0.00000, 167.00000);
		CreateDynamicObject(2011, 291.01889, -1594.49451, 45.96800     + (4.5 * i), 0.00000, 0.00000, 0.00000);
		CreateDynamicObject(2011, 299.55859, -1596.08801, 47.12800     + (4.5 * i), 0.00000, 0.00000, 0.00000);
		CreateDynamicObject(1208, 290.67520, -1596.42395, 45.97870     + (4.5 * i), 0.00000, 0.00000, -104.80000);
		CreateDynamicObject(2339, 290.76251, -1597.38074, 45.94090     + (4.5 * i), 0.00000, 0.00000, 78.50000);
		CreateDynamicObject(19927, 290.31210, -1598.76282, 45.99880    + (4.5 * i), 0.00000, 0.00000, -11.10000);
		CreateDynamicObject(2832, 290.49881, -1598.28430, 46.92530     + (4.5 * i), 0.00000, 0.00000, 16.39990);
		CreateDynamicObject(19172, 289.93619, -1598.28491, 48.27110    + (4.5 * i), 0.00000, 0.00000, 79.80000);
		CreateDynamicObject(1792, 297.99081, -1607.33984, 48.28290     + (4.5 * i), 0.00000, 0.00000, -139.19991);
		CreateDynamicObject(2244, 297.85629, -1607.31421, 46.27380     + (4.5 * i), 0.00000, 0.00000, -9.30000);
		CreateDynamicObject(19924, 290.324402, -1600.749634, 49.530502 + (4.5 * i), 0.000000, 0.000000, -11.500000);
		CreateDynamicObject(19931, 289.939087, -1600.778564, 46.006897 + (4.5 * i), 0.000000, 0.000000, -10.499900);
		CreateDynamicObject(19931, 289.805908, -1601.498535, 46.006897 + (4.5 * i), 0.000000, 0.000000, -10.499900);
		CreateDynamicObject(19931, 289.671112, -1602.227173, 46.006897 + (4.5 * i), 0.000000, 0.000000, -10.499900);
		CreateDynamicObject(19931, 289.541687, -1602.925659, 46.006897 + (4.5 * i), 0.000000, 0.000000, -10.499900);
		CreateDynamicObject(19931, 289.408691, -1603.644653, 46.006897 + (4.5 * i), 0.000000, 0.000000, -10.499900);
		CreateDynamicObject(19931, 289.119202, -1605.206665, 46.006897 + (4.5 * i), 0.000000, 0.000000, -10.499900);
		CreateDynamicObject(19931, 290.070190, -1600.069824, 46.006897 + (4.5 * i), 0.000000, 0.000000, -10.499900);
		CreateDynamicObject(2149, 289.340393, -1602.787231, 47.077003  + (4.5 * i), 0.000000, 0.000000, 79.899902);
		CreateDynamicObject(2500, 289.022491, -1603.359741, 46.951797  + (4.5 * i), 0.000000, 0.000000, 80.399902);
		CreateDynamicObject(11706, 289.127808, -1605.847412, 46.006699 + (4.5 * i), 0.000000, 0.000000, -7.999900);
		CreateDynamicObject(19916, 289.279388, -1604.456543, 45.956299 + (4.5 * i), 0.000000, 0.000000, 82.099899);
		CreateDynamicObject(18873, 303.266510, -1625.632324, 46.777802 + (4.5 * i), 0.000000, 0.000000, -140.300003);
		CreateDynamicObject(2426, 289.734009, -1600.734741, 46.914001  + (4.5 * i), 0.000000, 0.000000, 80.399902);
		CreateDynamicObject(19175, 298.03851, -1615.82983, 48.83770    + (4.5 * i), 0.00000, 0.00000, -11.19990);
		CreateDynamicObject(2833, 292.582001, -1618.560059, 45.988701  + (4.5 * i), 0.000000, 0.000000, 80.999901);
		CreateDynamicObject(1502, 292.885590, -1606.942871, 45.985603  + (4.5 * i), 0.000000, 0.000000, -9.599900);
		CreateDynamicObject(2307, 293.549896, -1610.470947, 46.082397  + (4.5 * i), 0.000000, 0.000000, 79.399902);
		CreateDynamicObject(2517, 292.217285, -1612.932739, 45.971298  + (4.5 * i), 0.000000, 0.000000, 167.699997);
		CreateDynamicObject(1641, 293.598389, -1614.114624, 45.996902  + (4.5 * i), 0.000000, 0.000000, -101.799896);
		CreateDynamicObject(2526, 295.12640, -1614.46313, 45.99040	   + (4.5 * i), 0.00000, 0.00000, -10.99990);
		CreateDynamicObject(11707, 296.99301, -1613.24597, 47.52670    + (4.5 * i), 0.00000, 0.00000, -99.19990);
		CreateDynamicObject(11707, 293.622894, -1614.588745, 47.526703 + (4.5 * i), 0.000000, 0.000000, 169.500000);
		CreateDynamicObject(2528, 295.56461, -1608.01685, 45.98810	   + (4.5 * i), 0.00000, 0.00000, -9.29990);
		CreateDynamicObject(2515, 296.94501, -1607.98206, 47.07340     + (4.5 * i), 0.00000, 0.00000, -5.79990);
		CreateDynamicObject(19873, 296.73029, -1607.73462, 47.13650    + (4.5 * i), 0.00000, 0.00000, 0.00000);
		CreateDynamicObject(2741, 297.256805, -1607.831421, 47.426903  + (4.5 * i), 0.000000, 0.000000, -7.300000);
		CreateDynamicObject(2258, 297.43130, -1610.99890, 47.82030     + (4.5 * i), 0.00000, 0.00000, -101.79990);

		//AP 2
		CreateDynamicObject(11737, 276.186188, -1620.354614, 45.991600 + (4.5 * i), 0.000000, 0.000000, 80.499901);
		CreateDynamicObject(2571, 269.088501, -1619.661011, 45.981003  + (4.5 * i), 0.000000, 0.000000, 127.099998);
		CreateDynamicObject(19175, 279.098907, -1612.406860, 48.325897 + (4.5 * i), 0.000000, 0.000000, -10.199900);
		CreateDynamicObject(19786, 282.356598, -1616.360840, 48.113098 + (4.5 * i), 0.000000, 0.000000, -101.299896);
		CreateDynamicObject(2313, 282.004608, -1615.583252, 45.985298  + (4.5 * i), 0.000000, 0.000000, -100.199898);
		CreateDynamicObject(2241, 281.302704, -1618.473633, 46.475502  + (4.5 * i), 0.000000, 0.000000, 0.000000);
		CreateDynamicObject(2311, 279.904694, -1615.170166, 46.023201  + (4.5 * i), 0.000000, 0.000000, -99.400002);
		CreateDynamicObject(2069, 266.965210, -1619.356812, 46.006897  + (4.5 * i), 0.000000, 0.000000, 0.000000);
		CreateDynamicObject(2001, 271.943115, -1591.957153, 45.991302  + (4.5 * i), 0.000000, 0.000000, 0.000000);
		CreateDynamicObject(2357, 267.760315, -1612.609619, 46.339104  + (4.5 * i), -0.699900, -0.199900, 78.800003);
		CreateDynamicObject(1210, 267.447388, -1614.139648, 46.839996  + (4.5 * i), 90.899902, 0.799900, -80.099899);
		CreateDynamicObject(19893, 268.101593, -1611.010620, 46.758797 + (4.5 * i), 0.000000, 0.000000, 58.200001);
		CreateDynamicObject(2290, 277.323914, -1616.438477, 45.976700  + (4.5 * i), -0.199900, 0.699900, 79.199898);
		CreateDynamicObject(1670, 279.761292, -1615.807373, 46.544998  + (4.5 * i), 0.000000, 0.000000, 0.000000);
		CreateDynamicObject(19786, 297.406799, -1612.748047, 48.778603 + (4.5 * i), 0.000000, 0.000000, 77.500099);
		CreateDynamicObject(1742, 274.936493, -1611.532715, 45.972298  + (4.5 * i), 0.000000, 0.000000, -9.599900);
		CreateDynamicObject(2241, 277.622894, -1606.211060, 46.475502  + (4.5 * i), 0.000000, 0.000000, 0.000000);
		CreateDynamicObject(2313, 282.004608, -1615.583252, 45.985298  + (4.5 * i), 0.000000, 0.000000, -100.199898);
		CreateDynamicObject(2099, 282.772705, -1613.670532, 45.984497  + (4.5 * i), 0.000000, 0.000000, -100.400002);
		CreateDynamicObject(19175, 298.038513, -1615.829834, 48.837700 + (4.5 * i), 0.000000, 0.000000, -11.199900);
		CreateDynamicObject(1502, 271.523804, -1611.024048, 45.988998  + (4.5 * i), 0.000000, 0.000000, -6.900000);
		CreateDynamicObject(2028, 277.188507, -1609.000854, 46.566101  + (4.5 * i), 0.400000, 0.799900, -99.099998);
		CreateDynamicObject(338, 276.957489, -1604.050659, 46.836502   + (4.5 * i), -110.400002, 72.900002, 61.200001);
		CreateDynamicObject(19174, 278.584503, -1603.254150, 48.313599 + (4.5 * i), 0.000000, 0.000000, -99.999802);
		CreateDynamicObject(2001, 267.087097, -1616.012817, 45.982697  + (4.5 * i), 0.000000, 0.000000, 0.000000);
		CreateDynamicObject(1709, 269.293304, -1609.881836, 45.966698  + (4.5 * i), 0.000000, 0.000000, 84.099998);
		CreateDynamicObject(1827, 270.570892, -1606.543457, 45.959900  + (4.5 * i), 0.000000, 0.000000, 0.000000);
		CreateDynamicObject(19786, 280.508301, -1604.626343, 47.949898 + (4.5 * i), 0.000000, 0.000000, 169.999893);
		CreateDynamicObject(1731, 289.010406, -1601.039063, 47.614502  + (4.5 * i), 0.299900, -0.300000, -8.899900);
		CreateDynamicObject(1731, 289.338104, -1598.942871, 47.625504  + (4.5 * i), 0.299900, -0.300000, -8.899900);
		CreateDynamicObject(2069, 268.973907, -1603.406372, 46.037598  + (4.5 * i), 0.000000, 0.000000, 0.000000);
		CreateDynamicObject(19922, 271.688690, -1601.553955, 45.966301 + (4.5 * i), 0.000000, 0.000000, 79.199997);
		CreateDynamicObject(1739, 270.537811, -1600.833252, 46.930702  + (4.5 * i), 0.000000, 0.000000, 169.099899);
		CreateDynamicObject(1739, 270.310608, -1602.096558, 46.931702  + (4.5 * i), 0.000000, 0.000000, 169.199997);
		CreateDynamicObject(1739, 272.046509, -1599.628540, 46.886597  + (4.5 * i), 0.000000, 0.200000, 79.300003);
		CreateDynamicObject(2229, 277.165802, -1611.213135, 45.963898  + (4.5 * i), 0.000000, 0.000000, -100.100098);
		CreateDynamicObject(2229, 277.739105, -1607.590942, 45.978401  + (4.5 * i), 0.000000, 0.400000, -101.399902);
		CreateDynamicObject(2313, 277.192596, -1608.316162, 45.997597  + (4.5 * i), 0.000000, 0.000000, -99.999901);
		CreateDynamicObject(2028, 291.161987, -1617.848022, 46.654701  + (4.5 * i), 0.000000, 0.000000, 80.000000);
		CreateDynamicObject(19786, 277.644287, -1609.110840, 48.002502 + (4.5 * i), 0.000000, 0.000000, -100.099899);
		CreateDynamicObject(2241, 277.622894, -1606.211060, 46.475502  + (4.5 * i), 0.000000, 0.000000, 0.000000);
		CreateDynamicObject(2964, 277.345795, -1603.090210, 45.866997  + (4.5 * i), 0.000000, 0.000000, 80.799896);
		CreateDynamicObject(338, 305.523010, -1605.385254, 47.118797   + (4.5 * i), -14.100000, 90.000000, 85.299896);
		CreateDynamicObject(14720, 270.590698, -1595.542236, 45.965897 + (4.5 * i), 0.000000, 0.000000, -12.000000);
		CreateDynamicObject(19916, 270.948090, -1593.641235, 45.976303 + (4.5 * i), 0.000000, 0.000000, 78.199898);
		CreateDynamicObject(11706, 271.022003, -1592.867432, 45.980904 + (4.5 * i), 0.000000, 0.000000, 80.199898);
		CreateDynamicObject(2001, 271.943115, -1591.957153, 45.991302  + (4.5 * i), 0.000000, 0.000000, 0.000000);
		CreateDynamicObject(19929, 274.351105, -1591.638672, 45.969597 + (4.5 * i), 0.000000, 0.000000, 78.400002);
		CreateDynamicObject(19929, 279.008606, -1592.523071, 45.962799 + (4.5 * i), 0.000000, 0.000000, 80.199997);
		CreateDynamicObject(19927, 276.678986, -1592.095459, 45.966698 + (4.5 * i), 0.000000, 0.000000, 79.199799);
		CreateDynamicObject(19830, 275.055695, -1591.760376, 46.884102 + (4.5 * i), 0.000000, 0.000000, -11.500000);
		CreateDynamicObject(11718, 278.049591, -1592.316650, 46.893303 + (4.5 * i), 0.000000, 0.000000, 102.099899);
		CreateDynamicObject(2500, 280.236908, -1592.528076, 46.866898  + (4.5 * i), 0.000000, 0.000000, -9.999900);
		CreateDynamicObject(19172, 279.780090, -1596.460205, 48.271202 + (4.5 * i), 0.000000, 0.000000, -99.800102);
		CreateDynamicObject(2200, 290.079712, -1594.490845, 45.985001  + (4.5 * i), 0.000000, 0.000000, -99.800003);
		CreateDynamicObject(1806, 281.177612, -1596.791748, 45.947601  + (4.5 * i), 0.000000, 0.000000, 81.799896);
		CreateDynamicObject(1806, 281.371094, -1595.292847, 45.971001  + (4.5 * i), 0.000000, 0.000000, 82.699898);
		CreateDynamicObject(1502, 279.256012, -1600.709961, 45.992302  + (4.5 * i), 0.000000, 0.000000, 80.799896);
		CreateDynamicObject(1703, 280.005890, -1601.552856, 45.839104  + (4.5 * i), 0.000000, 0.000000, -8.800000);
		CreateDynamicObject(2169, 280.725891, -1595.808838, 45.959900  + (4.5 * i), -0.699900, 0.000000, 81.099899);
		CreateDynamicObject(2171, 280.484985, -1597.238770, 45.963203  + (4.5 * i), 0.000000, 0.000000, 80.900002);
		CreateDynamicObject(2313, 281.291901, -1604.192017, 45.982300  + (4.5 * i), 0.000000, 0.000000, 169.699799);
		CreateDynamicObject(19786, 297.406799, -1612.748047, 48.778603 + (4.5 * i), 0.000000, 0.000000, 77.500099);
		CreateDynamicObject(2190, 280.610107, -1594.920532, 46.724602  + (4.5 * i), 0.000000, 0.000000, 55.199902);
		CreateDynamicObject(2190, 280.263000, -1596.431763, 46.747002  + (4.5 * i), 0.000000, 0.000000, 81.800003);
		CreateDynamicObject(2200, 288.498413, -1604.147705, 45.991096  + (4.5 * i), 0.000000, 0.000000, -99.300003);
		CreateDynamicObject(2001, 288.606598, -1602.621704, 45.973602  + (4.5 * i), 0.000000, 0.000000, 0.000000);
		CreateDynamicObject(2010, 289.289490, -1597.301514, 45.978104  + (4.5 * i), 0.000000, 0.000000, 0.000000);
		CreateDynamicObject(14446, 287.591003, -1599.707642, 46.584198 + (4.5 * i), 0.000000, 0.000000, -99.499901);
		CreateDynamicObject(2108, 280.981903, -1593.387207, 45.983498  + (4.5 * i), 0.000000, 0.000000, 0.000000);
		CreateDynamicObject(2523, 279.460205, -1605.149536, 45.993301  + (4.5 * i), 0.899900, 0.099900, -9.999900);
		CreateDynamicObject(1841, 289.607788, -1597.539673, 48.864700  + (4.5 * i), 0.899900, -13.800000, -11.900000);
		CreateDynamicObject(1841, 288.593414, -1602.353638, 48.787300  + (4.5 * i), 0.899900, -13.800000, -11.900000);
		CreateDynamicObject(19173, 289.327087, -1600.006958, 48.768204 + (4.5 * i), 0.000000, 0.000000, 80.799896);
		CreateDynamicObject(1502, 282.445801, -1605.037476, 45.976097  + (4.5 * i), 0.000000, 0.000000, -8.300000);
		CreateDynamicObject(2514, 278.640900, -1607.520142, 45.980003  + (4.5 * i), 0.000000, 0.000000, 80.500198);
		CreateDynamicObject(2519, 277.967896, -1611.375122, 45.984802  + (4.5 * i), 0.900000, 0.299900, 79.199997);
		CreateDynamicObject(1641, 279.178314, -1610.903809, 46.000397  + (4.5 * i), 0.000000, 0.099900, 168.800003);
		CreateDynamicObject(2307, 280.763000, -1610.906006, 45.916901  + (4.5 * i), 0.000000, 0.000000, 172.500000);
		CreateDynamicObject(2517, 282.264496, -1613.001953, 45.978104  + (4.5 * i), 0.000000, 0.000000, -8.099900);
		CreateDynamicObject(11707, 283.238403, -1610.851440, 47.339600 + (4.5 * i), 0.000000, 0.000000, -99.199997);
		CreateDynamicObject(2257, 283.611298, -1608.958740, 47.984802  + (4.5 * i), 0.000000, 0.000000, -99.800003);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}