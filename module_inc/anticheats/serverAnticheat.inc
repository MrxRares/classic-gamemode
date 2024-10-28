#include <Pawn.RakNet>

/* AntiCheat bazat pe basina de Pawn.RakNet facut de EquiNoxAdv pentru Eclipsed.
   Inceput prin martie park terminat abia acum ca n-am avut chef de el [12.10.2019].
   E inca in beta deci sunt buguri mi se rupe pula nu merg cam 99% din troll/crashet/etc.
*/   

//#define DEBUG_PROST

#define MAX_QUAT_WARNINGS 1
#define MAX_SURF_WARNINGS 30
#define MAX_SURF_DIST_WARNINGS 30

enum {
	INVALID_POSITION = 1,
	INVALID_QUATERNION,
	INVALID_VELOCITY,
	INVALID_SURFING_OFFSETS,
	INVALID_SURFING,
	INVALID_ANIMATION_FLAGS,
	INVALID_HEALTH,
	INVALID_TRAIN_SPEED,
	INVALID_ROLL,
	INVALID_DIRECTION,
	INVALID_ANGULAR_VELOCITY,
	INVALID_ORIGIN,
	INVALID_HITPOS,
	INVALID_OFFSETS,
	INVALID_CAMFRONTVEC,
	INVALID_CAMPOS,
	INVALID_AIMZ,
	INVALID_SHOT,
	INVALID_VEHICLE_MANIPULATION,
	INVALID_PLAYER_VELOCITY,
	INVALID_PLAYER_MANIPULATION,
	INVALID_CAMERA_VELOCITY,
	INVALID_BULLET_MANIPULATION
}


getCheatName(id) {
	new muie[99];
	switch(id) {
		case 0: muie = "INVALID_POSITION";
		case 1: muie = "INVALID_QUATERNION";
		case 2: muie = "INVALID_VELOCITY";
		case 3: muie = "INVALID_SURFING_OFFSETS";
		case 4: muie = "INVALID_SURFING";
		case 5: muie = "INVALID_ANIMATION_FLAGS";
		case 6: muie = "INVALID_HEALTH";
		case 7: muie = "INVALID_TRAIN_SPEED";
		case 8: muie = "INVALID_ROLL";
		case 9: muie = "INVALID_DIRECTION";
		case 10: muie = "INVALID_ANGULAR_VELOCITY";
		case 11: muie = "INVALID_ORIGIN";
		case 12: muie = "INVALID_HITPOS";
		case 13: muie = "INVALID_OFFSETS";
		case 14: muie = "INVALID_CAMFRONTVEC";
		case 15: muie = "INVALID_CAMPOS";
		case 16: muie = "INVALID_AIMZ";
		case 17: muie = "INVALID_SHOT";
		case 18: muie = "INVALID_VEHICLE_MANIPULATION";
		case 19: muie = "INVALID_PLAYER_VELOCITY";
		case 20: muie = "INVALID_VEHICLE_MANIPULATION #2";
		case 21: muie = "INVALID_PLAYER_VELOCITY #2";
		case 22: muie = "INVALID_PLAYER_MANIPULATION";
		case 23: muie = "INVALID_PLAYER_MANIPULATION #2";
		case 24: muie = "INVALID_CAMERA_VELOCITY";
		case 25: muie = "INVALID_BULLET_MANIPULATION";
	}
	return muie;
}

const ID_PLAYER_SYNC = 207;
const ID_VEHICLE_SYNC = 200;
const ID_PASSENGER_SYNC = 211;
const ID_UNOCCUPIED_SYNC = 209;
const ID_TRAILER_SYNC = 210;
const ID_SPECTATOR_SYNC = 212;
const ID_BULLET_SYNC = 206;
const ID_AIM_SYNC = 203;
#define BulletCrasher -5.5
#define red 0xFF0000FF
#define eight 8
#define two 2
#define NULL 0


new
    Float:__scy_ab_POS_X[MAX_PLAYERS],
    Float:__scy_ab_POS_Y[MAX_PLAYERS],
    Float:__scy_ab_POS_Z[MAX_PLAYERS],
    __scy_ab__LAST_CALL[MAX_PLAYERS];



enum rakInfo {
	rakQuatCount,
	rakSurfingCount,
	rakSurfingDistCount
}
static RAKInfo[MAX_PLAYERS][rakInfo];

#if !defined isnull
	#define isnull(%1) ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif

static checkMemory(Float:muian) {
	if(floatcmp(muian, muian) != 0 || floatcmp(muian, 0x7F800000) == 0 || floatcmp(muian, 0xFF800000) == 0) return 1;
	return 0;
}

static checkOutput(Float:data, Float:max, Float:min = 0.0, bool:check = false) {
	if(floatcmp(floatabs(data), max) > 0 || check && floatcmp(data, min) < 0) return 1;
	return 0;
}

static AdmBot(playerid, const reason[], const bool:anticheat = false) {
    if(!IsPlayerConnected(playerid))
        return 1;

	if(playerInfo[playerid][pAdmin]) return 1;
    sendSplittedMessageToAll(COLOR_LIGHTRED, COLOR_DARKRED, "%s:{ffffff} %s has been kicked for using: %s.", anticheat ? "AC" : "AdmBot", GetName(playerid), reason);
	SendClientMessage(playerid, COLOR_CLIENT, "You got kicked for using %s.", reason);

    // if(anticheat)
    //     return Kick(playerid);
    
    // KickEx(playerid);
    return 1;
}


function:catchRaccoon(playerid, sync, type, subtype, times) {
	if(IsPlayerConnected(playerid) == 0) return 0;
	#if defined DEBUG_PROST
		printf("(AC): Suspicious player ID %d. Reason code: %03d (%d)", playerid, type, subtype);
	#endif
	
	#pragma unused sync

	if(playerInfo[playerid][pAdmin])
		return true;
	//multumim equi din cauza ta ne ia foc svu
	/*if(type == 10 || type == 9)
		return true;*/ //by je

	playerStaticVar[playerid][pCheatWarnings] ++;

	sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_DARKRED, "%s (%d) is suspected of using %s - detected %d times.", GetName(playerid), playerid, getCheatName(subtype), playerStaticVar[playerid][pCheatWarnings]);

	if(playerStaticVar[playerid][pCheatWarnings] == times){
        AdmBot(playerid, getCheatName(subtype), true);
        playerStaticVar[playerid][pCheatWarnings] = 0;
	}
	return 0;
}

IPacket:ID_PLAYER_SYNC(playerid, BitStream:bs) {

	new onFootData[PR_OnFootSync];

	BS_IgnoreBits(bs, 8);
	BS_ReadOnFootSync(bs, onFootData);

	if(checkMemory(onFootData[PR_position][0]) || checkMemory(onFootData[PR_position][1]) || checkMemory(onFootData[PR_position][2])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_PLAYER_SYNC]: Position x, y, z: %f, %f, %f", onFootData[PR_position][0], onFootData[PR_position][1], onFootData[PR_position][2]);
		#endif
		return catchRaccoon(playerid, ID_PLAYER_SYNC, INVALID_POSITION, 0, 1);
	}

	if(checkOutput(onFootData[PR_position][0], 20000.0) || checkOutput(onFootData[PR_position][1], 20000.0) || checkOutput(onFootData[PR_position][2], 20000.0)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_PLAYER_SYNC]: Position x, y, z: %f, %f, %f", onFootData[PR_position][0], onFootData[PR_position][1], onFootData[PR_position][2]);
		#endif
		return catchRaccoon(playerid, ID_PLAYER_SYNC, INVALID_POSITION, 1, 1);
	}

	if(checkMemory(onFootData[PR_quaternion][0]) || checkMemory(onFootData[PR_quaternion][1]) || checkMemory(onFootData[PR_quaternion][2]) || checkMemory(onFootData[PR_quaternion][3])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_PLAYER_SYNC]: Quaternion w, x, y, z: %f, %f, %f, %f", onFootData[PR_quaternion][0], onFootData[PR_quaternion][1], onFootData[PR_quaternion][2], onFootData[PR_quaternion][3]);
		#endif
		return catchRaccoon(playerid, ID_PLAYER_SYNC, INVALID_QUATERNION, 0, 1);
	}

	if(checkOutput(onFootData[PR_quaternion][0], 1.0) || checkOutput(onFootData[PR_quaternion][1], 1.0) || checkOutput(onFootData[PR_quaternion][2], 1.0) || checkOutput(onFootData[PR_quaternion][3], 1.0)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_PLAYER_SYNC]: Quaternion w, x, y, z: %f, %f, %f, %f", onFootData[PR_quaternion][0], onFootData[PR_quaternion][1], onFootData[PR_quaternion][2], onFootData[PR_quaternion][3]);
		#endif
		return catchRaccoon(playerid, ID_PLAYER_SYNC, INVALID_QUATERNION, 1, 1);
	}

	new Float:ang = atan2(floatmul(2.0, floatadd(floatmul(onFootData[PR_quaternion][2], onFootData[PR_quaternion][3]),
	floatmul(onFootData[PR_quaternion][0], onFootData[PR_quaternion][1]))), floatadd(floatsub(floatsub(floatpower(onFootData[PR_quaternion][0], 2.0),
	floatpower(onFootData[PR_quaternion][1], 2.0)), floatpower(onFootData[PR_quaternion][2], 2.0)),
	floatpower(onFootData[PR_quaternion][3], 2.0)));

	if(floatabs(ang) > 85.0 && onFootData[PR_specialAction] == SPECIAL_ACTION_NONE) {
		if(++RAKInfo[playerid][rakQuatCount] > MAX_QUAT_WARNINGS) {

			// anti roll crasher futut mm vostri
			#if defined DEBUG_PROST
				printf("[AC - ID_PLAYER_SYNC]: Quaternion w, x, y, z: %f, %f, %f, %f, angle: %f", onFootData[PR_quaternion][0], onFootData[PR_quaternion][1], onFootData[PR_quaternion][2], onFootData[PR_quaternion][3], ang);
			#endif
			return catchRaccoon(playerid, ID_PLAYER_SYNC, INVALID_QUATERNION, 2, 1);
		}
	}
	else RAKInfo[playerid][rakQuatCount] = 0;

	if(checkMemory(onFootData[PR_velocity][0]) || checkMemory(onFootData[PR_velocity][1]) || checkMemory(onFootData[PR_velocity][2])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_PLAYER_SYNC]: Velocity x, y, z: %f, %f, %f", onFootData[PR_velocity][0], onFootData[PR_velocity][1], onFootData[PR_velocity][2]);
		#endif
		return catchRaccoon(playerid, ID_PLAYER_SYNC, INVALID_VELOCITY, 0, 1);
	}

	if(checkOutput(onFootData[PR_velocity][0], 100.0) || checkOutput(onFootData[PR_velocity][1], 100.0) || checkOutput(onFootData[PR_velocity][2], 100.0)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_PLAYER_SYNC]: Velocity x, y, z: %f, %f, %f", onFootData[PR_velocity][0], onFootData[PR_velocity][1], onFootData[PR_velocity][2]);
		#endif
		return catchRaccoon(playerid, ID_PLAYER_SYNC, INVALID_VELOCITY, 1, 1);
	}

	if(checkMemory(onFootData[PR_surfingOffsets][0]) || checkMemory(onFootData[PR_surfingOffsets][1]) || checkMemory(onFootData[PR_surfingOffsets][2])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_PLAYER_SYNC]: Surfing offsets x, y, z: %f, %f, %f", onFootData[PR_surfingOffsets][0], onFootData[PR_surfingOffsets][1], onFootData[PR_surfingOffsets][2]);
		#endif
		return catchRaccoon(playerid, ID_PLAYER_SYNC, INVALID_SURFING_OFFSETS, 0, 1);
	}

	if(checkOutput(onFootData[PR_surfingOffsets][0], 50.0) || checkOutput(onFootData[PR_surfingOffsets][1], 50.0) || checkOutput(onFootData[PR_surfingOffsets][2], 50.0)) {
		if(++RAKInfo[playerid][rakSurfingCount] > MAX_SURF_WARNINGS) {
			#if defined DEBUG_PROST
				printf("[AC - ID_PLAYER_SYNC]: Surfing offsets x, y, z: %f, %f, %f", onFootData[PR_surfingOffsets][0], onFootData[PR_surfingOffsets][1], onFootData[PR_surfingOffsets][2]);
			#endif
			return catchRaccoon(playerid, ID_PLAYER_SYNC, INVALID_SURFING_OFFSETS, 1, 1);
		}
	}
	else RAKInfo[playerid][rakSurfingCount] = 0;

	if(onFootData[PR_surfingVehicleId] != 0) {
		if(0 < onFootData[PR_surfingVehicleId] <= MAX_VEHICLES) {

			new model = GetVehicleModel(onFootData[PR_surfingVehicleId]);
			if(model == 0) {
				#if defined DEBUG_PROST
					printf("[AC - ID_PLAYER_SYNC]: Veh model: %d, vehicleid: %d",
					model, onFootData[PR_surfingVehicleId]);
				#endif
				return catchRaccoon(playerid, ID_PLAYER_SYNC, INVALID_SURFING, 0, 1);
			}
			else {

				new Float:vX, Float:vY, Float:vZ;
				GetVehiclePos(onFootData[PR_surfingVehicleId], vX, vY, vZ);
				ang = VectorSize(floatsub(onFootData[PR_position][0], vX), floatsub(onFootData[PR_position][1], vY),
				floatsub(onFootData[PR_position][2], vZ));

				if(floatcmp(ang, 50.0) > 0) {
					if(++RAKInfo[playerid][rakSurfingDistCount] > MAX_SURF_DIST_WARNINGS) {

						#if defined DEBUG_PROST
							printf("[AC - ID_PLAYER_SYNC]: Veh x, y, z: %f, %f, %f, player x, y, z: %f, %f, %f, distance: %f", vX, vY, vZ, onFootData[PR_position][0], onFootData[PR_position][1], onFootData[PR_position][2], ang);
						#endif
						return catchRaccoon(playerid, ID_PLAYER_SYNC, INVALID_SURFING, 1, 1);
					}
				}
				else RAKInfo[playerid][rakSurfingDistCount] = 0;
			}
		}
		else if(MAX_VEHICLES < onFootData[PR_surfingVehicleId] <= MAX_OBJECTS + MAX_VEHICLES) {

			new object = GetObjectModel(onFootData[PR_surfingVehicleId]);

			if(object == -1) {
				#if defined DEBUG_PROST
					printf("[AC - ID_PLAYER_SYNC]: Object model: %d, objectid: %d", object, onFootData[PR_surfingVehicleId]);
				#endif
				return catchRaccoon(playerid, ID_PLAYER_SYNC, INVALID_SURFING, 2, 1);
			}
			else {

				new Float:vX, Float:vY, Float:vZ;
				GetObjectPos(onFootData[PR_surfingVehicleId], vX, vY, vZ);
				ang = VectorSize(floatsub(onFootData[PR_position][0], vX), floatsub(onFootData[PR_position][1], vY), floatsub(onFootData[PR_position][2], vZ));

				if(floatcmp(ang, 50.0) > 0) {
					if(++RAKInfo[playerid][rakSurfingDistCount] > MAX_SURF_DIST_WARNINGS) {

						#if defined DEBUG_PROST
							printf("[AC - ID_PLAYER_SYNC]: Object x, y, z: %f, %f, %f, player x, y, z: %f, %f, %f, distance: %f", vX, vY, vZ, onFootData[PR_position][0], onFootData[PR_position][1], onFootData[PR_position][2], ang);
						#endif
						return catchRaccoon(playerid, ID_PLAYER_SYNC, INVALID_SURFING, 3, 2);
					}
				}
				else RAKInfo[playerid][rakSurfingDistCount] = 0;
			}
		}
		else {
			if(onFootData[PR_surfingVehicleId] != INVALID_VEHICLE_ID) {

				#if defined DEBUG_PROST
					printf("[AC - ID_PLAYER_SYNC]: Surfing: %d", onFootData[PR_surfingVehicleId]);
				#endif
				return catchRaccoon(playerid, ID_PLAYER_SYNC, INVALID_SURFING, 4, 2);
			}
		}
	}

	if(onFootData[PR_animationFlags] == 0) {
		#if defined DEBUG_PROST
			printf("[AC - ID_PLAYER_SYNC]: Animation flags: %d", onFootData[PR_animationFlags]);
		#endif
		return catchRaccoon(playerid, ID_PLAYER_SYNC, INVALID_ANIMATION_FLAGS, 0, 2);
	}
	return 1;
}

IPacket:ID_VEHICLE_SYNC(playerid, BitStream:bs) {

	new inCarData[PR_InCarSync];

	BS_IgnoreBits(bs, 8);
	BS_ReadInCarSync(bs, inCarData);

	if(checkMemory(inCarData[PR_quaternion][0]) || checkMemory(inCarData[PR_quaternion][1]) || checkMemory(inCarData[PR_quaternion][2]) || checkMemory(inCarData[PR_quaternion][3])) {

		#if defined DEBUG_PROST
			printf("[AC - ID_VEHICLE_SYNC]: Quaternion w, x, y, z: %f, %f, %f, %f", inCarData[PR_quaternion][0], inCarData[PR_quaternion][1], inCarData[PR_quaternion][2], inCarData[PR_quaternion][3]);
		#endif
		return catchRaccoon(playerid, ID_VEHICLE_SYNC, INVALID_QUATERNION, 0, 2);
	}

	if(checkOutput(inCarData[PR_quaternion][0], 1.0) || checkOutput(inCarData[PR_quaternion][1], 1.0) || checkOutput(inCarData[PR_quaternion][2], 1.0) || checkOutput(inCarData[PR_quaternion][3], 1.0)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_VEHICLE_SYNC]: Quaternion w, x, y, z: %f, %f, %f, %f", inCarData[PR_quaternion][0], inCarData[PR_quaternion][1], inCarData[PR_quaternion][2], inCarData[PR_quaternion][3]);
		#endif
		return catchRaccoon(playerid, ID_VEHICLE_SYNC, INVALID_QUATERNION, 1, 2);
	}

	if(checkMemory(inCarData[PR_position][0]) || checkMemory(inCarData[PR_position][1]) || checkMemory(inCarData[PR_position][2])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_VEHICLE_SYNC]: Position x, y, z: %f, %f, %f", inCarData[PR_position][0], inCarData[PR_position][1], inCarData[PR_position][2]);
		#endif
		return catchRaccoon(playerid, ID_VEHICLE_SYNC, INVALID_POSITION, 0, 2);
	}

	if(checkOutput(inCarData[PR_position][0], 20000.0) || checkOutput(inCarData[PR_position][1], 20000.0) || checkOutput(inCarData[PR_position][2], 20000.0)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_VEHICLE_SYNC]: Position x, y, z: %f, %f, %f", inCarData[PR_position][0], inCarData[PR_position][1], inCarData[PR_position][2]);
		#endif
		return catchRaccoon(playerid, ID_VEHICLE_SYNC, INVALID_POSITION, 1, 2);
	}

	if(checkMemory(inCarData[PR_velocity][0]) || checkMemory(inCarData[PR_velocity][1]) || checkMemory(inCarData[PR_velocity][2])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_VEHICLE_SYNC]: Velocity x, y, z: %f, %f, %f", inCarData[PR_velocity][0], inCarData[PR_velocity][1], inCarData[PR_velocity][2]);
		#endif
		return catchRaccoon(playerid, ID_VEHICLE_SYNC, INVALID_VELOCITY, 0, 2);
	}

	if(checkOutput(inCarData[PR_velocity][0], 100.0) || checkOutput(inCarData[PR_velocity][1], 100.0) || checkOutput(inCarData[PR_velocity][2], 100.0)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_VEHICLE_SYNC]: Velocity x, y, z: %f, %f, %f", inCarData[PR_velocity][0], inCarData[PR_velocity][1], inCarData[PR_velocity][2]);
		#endif
		return catchRaccoon(playerid, ID_VEHICLE_SYNC, INVALID_VELOCITY, 1, 2);
	}

	if(checkMemory(inCarData[PR_vehicleHealth])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_VEHICLE_SYNC]: Veh health: %f", inCarData[PR_vehicleHealth]);
		#endif
		return catchRaccoon(playerid, ID_VEHICLE_SYNC, INVALID_HEALTH, 0, 2);
	}

	if(checkMemory(inCarData[PR_trainSpeed])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_VEHICLE_SYNC]: Train speed: %f", inCarData[PR_trainSpeed]);
		#endif
		return catchRaccoon(playerid, ID_VEHICLE_SYNC, INVALID_TRAIN_SPEED, 0, 2);
	}

	if(checkOutput(inCarData[PR_trainSpeed], 3.5)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_VEHICLE_SYNC]: Train speed: %f", inCarData[PR_trainSpeed]);
		#endif
		return catchRaccoon(playerid, ID_VEHICLE_SYNC, INVALID_TRAIN_SPEED, 1, 2);
	}
	return 1;
}

IPacket:ID_PASSENGER_SYNC(playerid, BitStream:bs) {

	new passengerData[PR_PassengerSync];

	BS_IgnoreBits(bs, 8);
	BS_ReadPassengerSync(bs, passengerData);

	if(checkMemory(passengerData[PR_position][0]) || checkMemory(passengerData[PR_position][1]) || checkMemory(passengerData[PR_position][2])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_PASSENGER_SYNC]: Position x, y, z: %f, %f, %f", passengerData[PR_position][0], passengerData[PR_position][1], passengerData[PR_position][2]);
		#endif
		return catchRaccoon(playerid, ID_PASSENGER_SYNC, INVALID_POSITION, 0, 1);
	}

	if(checkOutput(passengerData[PR_position][0], 20000.0) || checkOutput(passengerData[PR_position][1], 20000.0) || checkOutput(passengerData[PR_position][2], 20000.0)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_PASSENGER_SYNC]: Position x, y, z: %f, %f, %f", passengerData[PR_position][0], passengerData[PR_position][1], passengerData[PR_position][2]);
		#endif
		return catchRaccoon(playerid, ID_PASSENGER_SYNC, INVALID_POSITION, 1, 1);
	}
	return 1;
}

IPacket:ID_UNOCCUPIED_SYNC(playerid, BitStream:bs) {

	new unoccupiedData[PR_UnoccupiedSync];

	BS_IgnoreBits(bs, 8);
	BS_ReadUnoccupiedSync(bs, unoccupiedData);

	if(checkMemory(unoccupiedData[PR_roll][0]) || checkMemory(unoccupiedData[PR_roll][1]) || checkMemory(unoccupiedData[PR_roll][2])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_UNOCCUPIED_SYNC]: Roll x, y, z: %f, %f, %f", unoccupiedData[PR_roll][0], unoccupiedData[PR_roll][1], unoccupiedData[PR_roll][2]);
		#endif
		return catchRaccoon(playerid, ID_UNOCCUPIED_SYNC, INVALID_ROLL, 0, 1);
	}

	if(checkOutput(unoccupiedData[PR_roll][0], 1.0) || checkOutput(unoccupiedData[PR_roll][1], 1.0) || checkOutput(unoccupiedData[PR_roll][2], 1.0)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_UNOCCUPIED_SYNC]: Roll x, y, z: %f, %f, %f", unoccupiedData[PR_roll][0], unoccupiedData[PR_roll][1], unoccupiedData[PR_roll][2]);
		#endif
		return catchRaccoon(playerid, ID_UNOCCUPIED_SYNC, INVALID_ROLL, 1, 1);
	}

	if(checkMemory(unoccupiedData[PR_direction][0]) || checkMemory(unoccupiedData[PR_direction][1]) || checkMemory(unoccupiedData[PR_direction][2])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_UNOCCUPIED_SYNC]: Direction x, y, z: %f, %f, %f", unoccupiedData[PR_direction][0], unoccupiedData[PR_direction][1], unoccupiedData[PR_direction][2]);
		#endif
		return catchRaccoon(playerid, ID_UNOCCUPIED_SYNC, INVALID_DIRECTION, 0, 1);
	}

	if(checkOutput(unoccupiedData[PR_direction][0], 1.0) || checkOutput(unoccupiedData[PR_direction][1], 1.0) || checkOutput(unoccupiedData[PR_direction][2], 1.0)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_UNOCCUPIED_SYNC]: Direction x, y, z: %f, %f, %f", unoccupiedData[PR_direction][0], unoccupiedData[PR_direction][1], unoccupiedData[PR_direction][2]);
		#endif
		return catchRaccoon(playerid, ID_UNOCCUPIED_SYNC, INVALID_DIRECTION, 1, 1);
	}

	if(checkMemory(unoccupiedData[PR_position][0]) || checkMemory(unoccupiedData[PR_position][1]) || checkMemory(unoccupiedData[PR_position][2])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_UNOCCUPIED_SYNC]: Position x, y, z: %f, %f, %f", unoccupiedData[PR_position][0], unoccupiedData[PR_position][1], unoccupiedData[PR_position][2]);
		#endif
		return catchRaccoon(playerid, ID_UNOCCUPIED_SYNC, INVALID_POSITION, 0, 1);
	}

	if(checkOutput(unoccupiedData[PR_position][0], 20000.0) || checkOutput(unoccupiedData[PR_position][1], 20000.0) || checkOutput(unoccupiedData[PR_position][2], 20000.0)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_UNOCCUPIED_SYNC]: Position x, y, z: %f, %f, %f", unoccupiedData[PR_position][0], unoccupiedData[PR_position][1], unoccupiedData[PR_position][2]);
		#endif
		return catchRaccoon(playerid, ID_UNOCCUPIED_SYNC, INVALID_POSITION, 1, 1);
	}

	if(checkMemory(unoccupiedData[PR_velocity][0]) || checkMemory(unoccupiedData[PR_velocity][1]) || checkMemory(unoccupiedData[PR_velocity][2])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_UNOCCUPIED_SYNC]: Velocity x, y, z: %f, %f, %f", unoccupiedData[PR_velocity][0], unoccupiedData[PR_velocity][1], unoccupiedData[PR_velocity][2]);
		#endif
		return catchRaccoon(playerid, ID_UNOCCUPIED_SYNC, INVALID_VELOCITY, 0, 1);
	}

	if(checkOutput(unoccupiedData[PR_velocity][0], 100.0) || checkOutput(unoccupiedData[PR_velocity][1], 100.0) || checkOutput(unoccupiedData[PR_velocity][2], 100.0)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_UNOCCUPIED_SYNC]: Velocity x, y, z: %f, %f, %f", unoccupiedData[PR_velocity][0], unoccupiedData[PR_velocity][1], unoccupiedData[PR_velocity][2]);
		#endif
		return catchRaccoon(playerid, ID_UNOCCUPIED_SYNC, INVALID_VELOCITY, 1, 1);
	}

	if(checkMemory(unoccupiedData[PR_angularVelocity][0]) || checkMemory(unoccupiedData[PR_angularVelocity][1]) || checkMemory(unoccupiedData[PR_angularVelocity][2])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_UNOCCUPIED_SYNC]: Angular velocity x, y, z: %f, %f, %f", unoccupiedData[PR_angularVelocity][0], unoccupiedData[PR_angularVelocity][1], unoccupiedData[PR_angularVelocity][2]);
		#endif
		return catchRaccoon(playerid, ID_UNOCCUPIED_SYNC, INVALID_ANGULAR_VELOCITY, 0, 1);
	}

	if(checkOutput(unoccupiedData[PR_angularVelocity][0], 1.0) || checkOutput(unoccupiedData[PR_angularVelocity][1], 1.0) || checkOutput(unoccupiedData[PR_angularVelocity][2], 1.0)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_UNOCCUPIED_SYNC]: Angular velocity x, y, z: %f, %f, %f", unoccupiedData[PR_angularVelocity][0], unoccupiedData[PR_angularVelocity][1], unoccupiedData[PR_angularVelocity][2]);
		#endif
		return catchRaccoon(playerid, ID_UNOCCUPIED_SYNC, INVALID_ANGULAR_VELOCITY, 1, 1);
	}

	if(checkMemory(unoccupiedData[PR_vehicleHealth])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_UNOCCUPIED_SYNC]: Veh health: %f", unoccupiedData[PR_vehicleHealth]);
		#endif
		return catchRaccoon(playerid, ID_UNOCCUPIED_SYNC, INVALID_HEALTH, 0, 1);
	}
	return 1;
}

IPacket:ID_TRAILER_SYNC(playerid, BitStream:bs) {

	new trailerData[PR_TrailerSync];

	BS_IgnoreBits(bs, 8);
	BS_ReadTrailerSync(bs, trailerData);

	if(checkMemory(trailerData[PR_position][0]) || checkMemory(trailerData[PR_position][1]) || checkMemory(trailerData[PR_position][2])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_TRAILER_SYNC]: Position x, y, z: %f, %f, %f", trailerData[PR_position][0], trailerData[PR_position][1], trailerData[PR_position][2]);
		#endif
		return catchRaccoon(playerid, ID_TRAILER_SYNC, INVALID_POSITION, 0, 1);
	}

	if(checkOutput(trailerData[PR_position][0], 20000.0) || checkOutput(trailerData[PR_position][1], 20000.0) || checkOutput(trailerData[PR_position][2], 20000.0)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_TRAILER_SYNC]: Position x, y, z: %f, %f, %f", trailerData[PR_position][0], trailerData[PR_position][1], trailerData[PR_position][2]);
		#endif
		return catchRaccoon(playerid, ID_TRAILER_SYNC, INVALID_POSITION, 1, 1);
	}

	if(checkMemory(trailerData[PR_quaternion][0]) || checkMemory(trailerData[PR_quaternion][1]) || checkMemory(trailerData[PR_quaternion][2]) || checkMemory(trailerData[PR_quaternion][3])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_TRAILER_SYNC]: Quaternion w, x, y, z: %f, %f, %f, %f", trailerData[PR_quaternion][0], trailerData[PR_quaternion][1], trailerData[PR_quaternion][2], trailerData[PR_quaternion][3]);
		#endif
		return catchRaccoon(playerid, ID_TRAILER_SYNC, INVALID_QUATERNION, 0, 1);
	}

	if(checkOutput(trailerData[PR_quaternion][0], 1.0) || checkOutput(trailerData[PR_quaternion][1], 1.0) || checkOutput(trailerData[PR_quaternion][2], 1.0) || checkOutput(trailerData[PR_quaternion][3], 1.0)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_TRAILER_SYNC]: Quaternion w, x, y, z: %f, %f, %f, %f", trailerData[PR_quaternion][0], trailerData[PR_quaternion][1], trailerData[PR_quaternion][2], trailerData[PR_quaternion][3]);
		#endif
		return catchRaccoon(playerid, ID_TRAILER_SYNC, INVALID_QUATERNION, 1, 1);
	}

	if(checkMemory(trailerData[PR_velocity][0]) || checkMemory(trailerData[PR_velocity][1]) || checkMemory(trailerData[PR_velocity][2])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_TRAILER_SYNC]: Velocity x, y, z: %f, %f, %f", trailerData[PR_velocity][0], trailerData[PR_velocity][1], trailerData[PR_velocity][2]);
		#endif
		return catchRaccoon(playerid, ID_TRAILER_SYNC, INVALID_VELOCITY, 0, 1);
	}

	if(checkOutput(trailerData[PR_velocity][0], 100.0) || checkOutput(trailerData[PR_velocity][1], 100.0) || checkOutput(trailerData[PR_velocity][2], 100.0)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_TRAILER_SYNC]: Velocity x, y, z: %f, %f, %f", trailerData[PR_velocity][0], trailerData[PR_velocity][1], trailerData[PR_velocity][2]);
		#endif
		return catchRaccoon(playerid, ID_TRAILER_SYNC, INVALID_VELOCITY, 1, 1);
	}

	if(checkMemory(trailerData[PR_angularVelocity][0]) || checkMemory(trailerData[PR_angularVelocity][1]) || checkMemory(trailerData[PR_angularVelocity][2])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_TRAILER_SYNC]: Angular velocity x, y, z: %f, %f, %f", trailerData[PR_angularVelocity][0], trailerData[PR_angularVelocity][1], trailerData[PR_angularVelocity][2]);
		#endif
		return catchRaccoon(playerid, ID_TRAILER_SYNC, INVALID_ANGULAR_VELOCITY, 0, 1);
	}

	if(checkOutput(trailerData[PR_angularVelocity][0], 1.0) || checkOutput(trailerData[PR_angularVelocity][1], 1.0) || checkOutput(trailerData[PR_angularVelocity][2], 1.0)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_TRAILER_SYNC]: Angular velocity x, y, z: %f, %f, %f", trailerData[PR_angularVelocity][0], trailerData[PR_angularVelocity][1], trailerData[PR_angularVelocity][2]);
		#endif
		return catchRaccoon(playerid, ID_TRAILER_SYNC, INVALID_ANGULAR_VELOCITY, 1, 1);
	}
	return 1;
}

IPacket:ID_SPECTATOR_SYNC(playerid, BitStream:bs) {
	new spectatorData[PR_SpectatingSync];

	BS_IgnoreBits(bs, 8);
	BS_ReadSpectatingSync(bs, spectatorData);

	if(checkMemory(spectatorData[PR_position][0]) || checkMemory(spectatorData[PR_position][1]) || checkMemory(spectatorData[PR_position][2])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_SPECTATOR_SYNC]: Position x, y, z: %f, %f, %f", spectatorData[PR_position][0], spectatorData[PR_position][1], spectatorData[PR_position][2]);
		#endif
		return catchRaccoon(playerid, ID_SPECTATOR_SYNC, INVALID_POSITION, 0, 1);
	}

	if(checkOutput(spectatorData[PR_position][0], 20000.0) || checkOutput(spectatorData[PR_position][1], 20000.0) || checkOutput(spectatorData[PR_position][2], 20000.0)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_SPECTATOR_SYNC]: Position x, y, z: %f, %f, %f", spectatorData[PR_position][0], spectatorData[PR_position][1], spectatorData[PR_position][2]);
		#endif
		return catchRaccoon(playerid, ID_SPECTATOR_SYNC, INVALID_POSITION, 1, 1);
	}
	return 1;
}

IPacket:ID_BULLET_SYNC(playerid, BitStream:bs) {
	new bulletData[PR_BulletSync];

	BS_IgnoreBits(bs, 8);
	BS_ReadBulletSync(bs, bulletData);

	if(checkMemory(bulletData[PR_origin][0]) || checkMemory(bulletData[PR_origin][1]) || checkMemory(bulletData[PR_origin][2])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_BULLET_SYNC]: Origin x, y, z: %f, %f, %f", bulletData[PR_origin][0], bulletData[PR_origin][1], bulletData[PR_origin][2]);
		#endif
		return catchRaccoon(playerid, ID_BULLET_SYNC, INVALID_ORIGIN, 0, 1);
	}

	if(checkOutput(bulletData[PR_origin][0], 20000.0) || checkOutput(bulletData[PR_origin][1], 20000.0) || checkOutput(bulletData[PR_origin][2], 20000.0)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_BULLET_SYNC]: Origin x, y, z: %f, %f, %f", bulletData[PR_origin][0], bulletData[PR_origin][1], bulletData[PR_origin][2]);
		#endif
		return catchRaccoon(playerid, ID_BULLET_SYNC, INVALID_ORIGIN, 1, 1);
	}

	if(checkMemory(bulletData[PR_hitPos][0]) || checkMemory(bulletData[PR_hitPos][1]) || checkMemory(bulletData[PR_hitPos][2])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_BULLET_SYNC]: Hitpos x, y, z: %f, %f, %f", bulletData[PR_hitPos][0], bulletData[PR_hitPos][1], bulletData[PR_hitPos][2]);
		#endif
		return catchRaccoon(playerid, ID_BULLET_SYNC, INVALID_HITPOS, 0, 1);
	}

	if(checkOutput(bulletData[PR_hitPos][0], 20000.0) || checkOutput(bulletData[PR_hitPos][1], 20000.0) || checkOutput(bulletData[PR_hitPos][2], 20000.0)) {
		#if defined DEBUG_PROST
			printf("[AC - ID_BULLET_SYNC]: Hitpos x, y, z: %f, %f, %f", bulletData[PR_hitPos][0], bulletData[PR_hitPos][1], bulletData[PR_hitPos][2]);
		#endif
		return catchRaccoon(playerid, ID_BULLET_SYNC, INVALID_HITPOS, 1, 1);
	}

	if(checkMemory(bulletData[PR_offsets][0]) || checkMemory(bulletData[PR_offsets][1]) || checkMemory(bulletData[PR_offsets][2])) {
		#if defined DEBUG_PROST
			printf("[AC - ID_BULLET_SYNC]: Offsets x, y, z: %f, %f, %f", bulletData[PR_offsets][0], bulletData[PR_offsets][1], bulletData[PR_offsets][2]);
		#endif
		return catchRaccoon(playerid, ID_BULLET_SYNC, INVALID_OFFSETS, 0, 1);
	}

	
	if(bulletData[PR_hitType] != BULLET_HIT_TYPE_NONE) {

		new Float:X, Float:Y, Float:Z;
		GetPlayerPos(playerid, X, Y, Z);

		if(floatcmp(bulletData[PR_origin][0], X) == 0 || floatcmp(bulletData[PR_origin][1], Y) == 0 || floatcmp(bulletData[PR_origin][2], Z) == 0) {
			#if defined DEBUG_PROST
				printf("[AC - ID_BULLET_SYNC]: Pos x, y, z: %f, %f, %f, sync x, y, z: %f, %f, %f", X, Y, Z, bulletData[PR_origin][0], bulletData[PR_origin][1], bulletData[PR_origin][2]);
			#endif
			return catchRaccoon(playerid, ID_BULLET_SYNC, INVALID_SHOT, 0, 1);
		}
	}
	

	switch(bulletData[PR_hitType])
	{
		case BULLET_HIT_TYPE_PLAYER: {
			if(checkOutput(bulletData[PR_offsets][0], 2.0) || checkOutput(bulletData[PR_offsets][1], 2.0) || checkOutput(bulletData[PR_offsets][2], 2.0)) {
				#if defined DEBUG_PROST
					printf("[AC - ID_BULLET_SYNC]: Offsets x, y, z: %f, %f, %f", bulletData[PR_offsets][0], bulletData[PR_offsets][1], bulletData[PR_offsets][2]);
				#endif
				return catchRaccoon(playerid, ID_BULLET_SYNC, INVALID_OFFSETS, 1, 1);
			}

			
			if(bulletData[PR_hitId] != INVALID_PLAYER_ID) {

				new Float:vX, Float:vY, Float:vZ;
				GetPlayerPos(bulletData[PR_hitId], vX, vY, vZ);

				if(floatcmp(bulletData[PR_hitPos][0], vX) == 0 || floatcmp(bulletData[PR_hitPos][1], vY) == 0 || floatcmp(bulletData[PR_hitPos][2], vZ) == 0) {
					#if defined DEBUG_PROST
						printf("[AC - ID_BULLET_SYNC]: Pos x, y, z: %f, %f, %f, sync x, y, z: %f, %f, %f", vX, vY, vZ, bulletData[PR_hitPos][0], bulletData[PR_hitPos][1], bulletData[PR_hitPos][2]);
					#endif
					return catchRaccoon(playerid, ID_BULLET_SYNC, INVALID_SHOT, 1, 1);
				}
			}
			
		}
		case BULLET_HIT_TYPE_VEHICLE: {

			if(checkOutput(bulletData[PR_offsets][0], 300.0) || checkOutput(bulletData[PR_offsets][1], 300.0) || checkOutput(bulletData[PR_offsets][2], 300.0)) {
				#if defined DEBUG_PROST
					printf("[AC - ID_BULLET_SYNC]: Offsets x, y, z: %f, %f, %f",
					bulletData[PR_offsets][0], bulletData[PR_offsets][1], bulletData[PR_offsets][2]);
				#endif
				return catchRaccoon(playerid, ID_BULLET_SYNC, INVALID_OFFSETS, 2, 1);
			}

			
			if(bulletData[PR_hitId] != INVALID_VEHICLE_ID) {

				new Float:vX, Float:vY, Float:vZ;
				GetVehiclePos(bulletData[PR_hitId], vX, vY, vZ);

				if(floatcmp(bulletData[PR_hitPos][0], vX) == 0 || floatcmp(bulletData[PR_hitPos][1], vY) == 0 || floatcmp(bulletData[PR_hitPos][2], vZ) == 0) {
					#if defined DEBUG_PROST
						printf("[AC - ID_BULLET_SYNC]: Pos x, y, z: %f, %f, %f, sync x, y, z: %f, %f, %f", vX, vY, vZ, bulletData[PR_hitPos][0], bulletData[PR_hitPos][1], bulletData[PR_hitPos][2]);
					#endif
					return catchRaccoon(playerid, ID_BULLET_SYNC, INVALID_SHOT, 2, 1);
				}
			}	
		}
		default: {
			if(checkOutput(bulletData[PR_offsets][0], 20000.0) || checkOutput(bulletData[PR_offsets][1], 20000.0) || checkOutput(bulletData[PR_offsets][2], 20000.0)) {
				#if defined DEBUG_PROST
					printf("[AC - ID_BULLET_SYNC]: Offsets x, y, z: %f, %f, %f", bulletData[PR_offsets][0], bulletData[PR_offsets][1], bulletData[PR_offsets][2]);
				#endif
				return catchRaccoon(playerid, ID_BULLET_SYNC, INVALID_OFFSETS, 3, 1);
			}
		}
	}
	return 1;
}


//gata cu besina lu equi

stock IsVehicleUpsideDown(vehicleid)
{
    new Float:quat_w, Float:quat_x, Float:quat_y, Float:quat_z;
    GetVehicleRotationQuat(vehicleid, quat_w, quat_x, quat_y, quat_z);
    return (
        floatabs(
            atan2(
                2 * (quat_y * quat_z + quat_w * quat_x),
                quat_w * quat_w - quat_x * quat_x - quat_y * quat_y + quat_z * quat_z
            )
        ) > 90.0
    );
}


forward OnVehicleRequestDeath(vehicleid, killerid);
public OnVehicleRequestDeath(vehicleid, killerid)
{
    new Float:health;

    GetVehicleHealth(vehicleid, health);

    if(health >= 250.0 && !IsVehicleUpsideDown(vehicleid)) return 0;
    

    return 1;
}


public OnIncomingPacket(playerid, packetid, BitStream:bs){
	if(playerInfo[playerid][pAdmin]) return 1;
	switch(packetid) {
		case ID_UNOCCUPIED_SYNC: {
			new unoccupiedData[PR_UnoccupiedSync];
 
		    BS_IgnoreBits(bs, 8);
		    BS_ReadUnoccupiedSync(bs, unoccupiedData);
 
		    if ((unoccupiedData[PR_roll] == unoccupiedData[PR_direction]) &&
			(unoccupiedData[PR_roll][1] == unoccupiedData[PR_direction][1]) &&
		        (unoccupiedData[PR_roll][2] == unoccupiedData[PR_direction][2])
		    ) {
				catchRaccoon(playerid, ID_UNOCCUPIED_SYNC, INVALID_VEHICLE_MANIPULATION, 18, 1);
				return 0;
		    }

		}

		case ID_VEHICLE_SYNC:{
				new inCarData[PR_InCarSync];
				BS_IgnoreBits(bs, eight);
				BS_ReadInCarSync(bs, inCarData);
				if inCarData[PR_position][two] == BulletCrasher *then {
					catchRaccoon(playerid, ID_VEHICLE_SYNC, INVALID_VEHICLE_MANIPULATION, 25, 1);
					return 0;
				}
				if(IsPlayerInAnyVehicle(playerid) && playerStaticVar[playerid][enterToVehicle] != inCarData[PR_vehicleId]) {
					catchRaccoon(playerid, ID_VEHICLE_SYNC, INVALID_VEHICLE_MANIPULATION, 20, 1);
					return 0;
				}
			}

		case ID_PLAYER_SYNC:{
				new onFootData[PR_OnFootSync];
				BS_IgnoreBits(bs,eight);
				BS_ReadOnFootSync(bs, onFootData);
				if onFootData[PR_position][two] == BulletCrasher *then {
					catchRaccoon(playerid, ID_PLAYER_SYNC, INVALID_PLAYER_VELOCITY, 21, 1);
					return 0;
				}
				if(onFootData[PR_animationId] == 958 && WEAPON:onFootData[PR_weaponId] != WEAPON:WEAPON_PARACHUTE || onFootData[PR_animationId] == 959 && WEAPON:onFootData[PR_weaponId] != WEAPON:WEAPON_PARACHUTE){
					catchRaccoon(playerid, ID_PLAYER_SYNC, INVALID_PLAYER_VELOCITY, 13, 1);
					return false;
				}
				new Float:player_speed;
		        player_speed = floatsqroot(floatpower(floatabs(onFootData[PR_velocity][0]), 2.0) +
		        floatpower(floatabs(onFootData[PR_velocity][1]), 2.0) +
		        floatpower(floatabs(onFootData[PR_velocity][2]), 2.0)) * 179.28625; 
		        if(player_speed > 450.0 && GetPlayerState(playerid)==PLAYER_STATE_ONFOOT)
		        {
		            if(__scy_ab_POS_X[playerid] != 0.0)
		            {
		                if(onFootData[PR_surfingVehicleId] == 0) 
		                {
		                    if(onFootData[PR_position][0] > __scy_ab_POS_X[playerid] + 1.3 || onFootData[PR_position][1] > __scy_ab_POS_Y[playerid] + 1.3 || onFootData[PR_position][2] > __scy_ab_POS_Z[playerid] + 1.3
		                    || onFootData[PR_position][0] < __scy_ab_POS_X[playerid] - 1.3 || onFootData[PR_position][1] < __scy_ab_POS_Y[playerid] - 1.3
		                    || onFootData[PR_position][2] < __scy_ab_POS_Z[playerid] - 1.3)
		                    {
								catchRaccoon(playerid, ID_PLAYER_SYNC, INVALID_PLAYER_VELOCITY, 19, 1);
								return false;
		                    }
		                }
		            }
		        }

		        __scy_ab_POS_X[playerid] = onFootData[PR_position][0];
		        __scy_ab_POS_Y[playerid] = onFootData[PR_position][1];
		        __scy_ab_POS_Z[playerid] = onFootData[PR_position][2];
			}

		case ID_PASSENGER_SYNC:{
				new passengerData[PR_PassengerSync];
				BS_IgnoreBits(bs, eight);
				BS_ReadPassengerSync(bs, passengerData);
				if GetPlayerVehicleSeat(playerid) == NULL *then {
					catchRaccoon(playerid, ID_PASSENGER_SYNC, INVALID_CAMERA_VELOCITY, 24, 1);

					return 0;
				}
				if passengerData[PR_position][two] == BulletCrasher *then {
					catchRaccoon(playerid, ID_PASSENGER_SYNC, INVALID_CAMERA_VELOCITY, 24, 1);
					return 0;
				}
			}

		case ID_AIM_SYNC: {
			new aimData[PR_AimSync];
		
			BS_IgnoreBits(bs, 8);
			BS_ReadAimSync(bs, aimData);
			if (aimData[PR_camMode] == 45 || aimData[PR_camMode] == 49)
			{
				catchRaccoon(playerid, ID_AIM_SYNC, INVALID_CAMERA_VELOCITY, 24, 1);
				return 0;
			}
		}

		case ID_BULLET_SYNC: {
			new bulletData[PR_BulletSync];

			BS_IgnoreBits(bs, 8);
			BS_ReadBulletSync(bs, bulletData);
			if (!(-35000.0 <= bulletData[PR_origin][0] <= 35000.0) || !(-35000.0 <= bulletData[PR_origin][1] <= 35000.0) || !(-35000.0 <= bulletData[PR_origin][2] <= 35000.0))
			{
				catchRaccoon(playerid, ID_BULLET_SYNC, INVALID_BULLET_MANIPULATION, 25, 1);
				return 0;
			}

			if (!(-35000.0 <= bulletData[PR_hitPos][0] <= 35000.0) || !(-35000.0 <= bulletData[PR_hitPos][1] <= 35000.0) || !(-35000.0 <= bulletData[PR_hitPos][2] <= 35000.0))
			{
				catchRaccoon(playerid, ID_BULLET_SYNC, INVALID_BULLET_MANIPULATION, 25, 1);
				return 0;
			}
		}

		case 136: {
			new vehicleid;

		    BS_ReadUint16(bs, vehicleid);

		    if(GetVehicleModel(vehicleid) < 400)
		    {
		        return 0;
		    }

		    return OnVehicleRequestDeath(vehicleid, playerid);
		}
	}
	return true;
}
