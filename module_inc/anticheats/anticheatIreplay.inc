#include <Pawn.RakNet>
#include <YSI_Coding\y_hooks>

new Float:maxWeaponDamage[] =
{
	8.25, 13.20, 46.20, 3.30, 3.30, 4.95, 6.60, 6.60, 8.25, 9.90, 9.90, 24.75, 41.25, 46.20
};

enum ENUM_AC_INFO
{
	lastShotTime, Float:totalDamage, aimbotDetected, aimbotTime, fireKeyPressed, numPlayerBullet, numBulletTick, shotTime, numShots, countAimbotTick,
	numAimbotDetection, numPlayerOutShoots, numAimShoots, aimTime, numInvalidBullets
}
new player_ac_data[MAX_PLAYERS][ENUM_AC_INFO];
static const reset_ac_vars[ENUM_AC_INFO];
enum rakInfo {
	rakQuatCount,
	rakSurfingCount,
	rakSurfingDistCount
}
static RAKInfo[MAX_PLAYERS][rakInfo];
hook OnGameModeInit()
{
	AddServerRule("anticheat by", "iReplay & vividsec");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid)
{
	RAKInfo[playerid][rakQuatCount] =
	RAKInfo[playerid][rakSurfingCount] =
	RAKInfo[playerid][rakSurfingDistCount] = 0;
	
	player_ac_data[playerid] = reset_ac_vars;
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(newkeys & KEY_HANDBRAKE)
		player_ac_data[playerid][fireKeyPressed] = 1;
	
	else
		player_ac_data[playerid][fireKeyPressed] = 0;

	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	new Float:hitPos[6];

	if(hittype != BULLET_HIT_TYPE_PLAYER)
	{
		player_ac_data[playerid][numPlayerBullet] = 0;
		player_ac_data[playerid][numBulletTick] = 0;
		return true;
	}

	if(!player_ac_data[playerid][fireKeyPressed] && hittype == BULLET_HIT_TYPE_PLAYER)
	{
		if(weaponid > 23 && GetDistanceBetweenPlayers(playerid, hitid) > 10 && player_ac_data[playerid][totalDamage] > 29 && player_ac_data[playerid][lastShotTime] < gettime())
		{
			if(player_ac_data[playerid][aimTime] <= gettime())
				player_ac_data[playerid][aimTime] = gettime() + 1, player_ac_data[playerid][numAimShoots] = 0;

			player_ac_data[playerid][numAimShoots] ++;

			if(player_ac_data[playerid][numAimShoots] == 6 || player_ac_data[playerid][numAimShoots] == 8)
				va_SendClientMessageToAll(COLOR_WARNING, "{f05d43}anticheat warning:{ffffff} %s (%d, level %d) may be cheating: shoots without aiming - detected %d times - weapon: %s.", GetName(playerid), playerid, GetPlayerScore(playerid), player_ac_data[playerid][numAimShoots], GetWeaponNameEx(weaponid));
			
			if(player_ac_data[playerid][numAimShoots] == 10)
			{
				va_SendClientMessageToAll(COLOR_WARNING, "%s (%d, level %d) has been kicked - reason: aimbot #4.", GetName(playerid), playerid, GetPlayerScore(playerid));
				
			}
		}
	}

	if(!(-1000.0 <= fX <= 1000.0) || !(-1000.0 <= fY <= 1000.0) || !(-1000.0 <= fZ <= 1000.0))
		va_SendClientMessageToAll(COLOR_WARNING, "{f05d43}anticheat warning:{ffffff} %s (%d, level %d) may be cheating: shoots to invalid direction. weapon: %s.", GetName(playerid), playerid, GetPlayerScore(playerid), GetWeaponNameEx(weaponid));

	if(weaponid != WEAPON_MINIGUN && weaponid != WEAPON_UZI && weaponid != WEAPON_TEC9)
	{
		if(player_ac_data[playerid][shotTime] < gettime())
			player_ac_data[playerid][shotTime] = gettime() + 1, player_ac_data[playerid][numShots] = 0;

		player_ac_data[playerid][numShots] ++;

		if(player_ac_data[playerid][numShots] > 20)
		{
			va_SendClientMessageToAll(COLOR_WARNING, "{f05d43}anticheat warning:{ffffff} %s (%d, level %d) may be cheating: shoots too fast (#2). weapon: %s.", GetName(playerid), playerid, GetPlayerScore(playerid), GetWeaponNameEx(weaponid));
			player_ac_data[playerid][numShots] = 0;
		}
	}

	if(player_ac_data[playerid][numBulletTick] < gettime())
		player_ac_data[playerid][numBulletTick] = gettime() + 5, player_ac_data[playerid][numPlayerBullet] = 0;

	if(!IsPlayerInRangeOfPoint(hitid, 10.0, hitPos[3], hitPos[4], hitPos[5]))
		player_ac_data[playerid][numPlayerBullet] ++;

	if(player_ac_data[playerid][numPlayerBullet] == 4 && weaponid == WEAPON_DEAGLE || player_ac_data[playerid][numPlayerBullet] == 6)
		va_SendClientMessageToAll(COLOR_WARNING, "{f05d43}anticheat warning:{ffffff} %s (%d, level %d) may be cheating: shoots without missing straight - detected %d times - weapon: %s.", GetName(playerid), playerid, GetPlayerScore(playerid), player_ac_data[playerid][numPlayerBullet], GetWeaponNameEx(weaponid));

	if(player_ac_data[playerid][numPlayerBullet] == 8)
	{
		va_SendClientMessageToAll( COLOR_WARNING, "%s (%d, level %d) has been kicked - reason: aimbot #2.", GetName(playerid), playerid, GetPlayerScore(playerid));
		
	}

	if(fX == 0 && fY == 0 && fZ == 0)
		va_SendClientMessageToAll(COLOR_WARNING, "{f05d43}anticheat warning:{ffffff} %s (%d, level %d) may be cheating: shoots invalid bullets. weapon: %s.", GetName(playerid), playerid, GetPlayerScore(playerid), GetWeaponNameEx(weaponid));

	if(GetPlayerTargetPlayer(playerid) != hitid && GetPlayerTargetPlayer(playerid) != INVALID_PLAYER_ID)
	{
		player_ac_data[playerid][numInvalidBullets] ++;
		if(player_ac_data[playerid][numInvalidBullets] == 5)
		{
			va_SendClientMessageToAll(COLOR_WARNING, "{f05d43}anticheat warning:{ffffff} %s (%d, level %d) may be cheating: shoots invalid bullets (#2). weapon: %s, hit: %d | TTP: %d.", GetName(playerid), playerid, GetPlayerScore(playerid), GetWeaponNameEx(weaponid), hitid, GetPlayerTargetPlayer(playerid));
			player_ac_data[playerid][numInvalidBullets] = 0;
		}
	}

	if(GetDistanceBetweenPlayers(playerid, hitid) > 105)
		va_SendClientMessageToAll(COLOR_WARNING, "{f05d43}anticheat warning:{ffffff} %s (%d, level %d) may be cheating: shoots out of stream. weapon: %s.", GetName(playerid), playerid, GetPlayerScore(playerid), GetWeaponNameEx(weaponid));

	if(!IsPlayerInRangeOfPoint(hitid, 3.0, hitPos[3], hitPos[4], hitPos[5]))
	{
		player_ac_data[playerid][numPlayerOutShoots] ++;

		if(player_ac_data[playerid][numPlayerOutShoots] == 5 || player_ac_data[playerid][numPlayerOutShoots] == 10 || player_ac_data[playerid][numPlayerOutShoots] == 15)
			va_SendClientMessageToAll(COLOR_WARNING, "{f05d43}anticheat warning:{ffffff} %s (%d, level %d) may be cheating: shoots out of range - detected %d times - weapon: %s.", GetName(playerid), playerid, GetPlayerScore(playerid), player_ac_data[playerid][numPlayerOutShoots], GetWeaponNameEx(weaponid));
	
		if(player_ac_data[playerid][numPlayerOutShoots] == 20)
		{
			va_SendClientMessageToAll( COLOR_WARNING, "%s (%d, level %d) has been kicked - reason: aimbot #3.", GetName(playerid), playerid, GetPlayerScore(playerid));
			
		}
	}

	if(GetPlayerTargetPlayer(playerid) == INVALID_PLAYER_ID)
	{
		if(player_ac_data[playerid][countAimbotTick] < gettime())
			player_ac_data[playerid][countAimbotTick] = gettime() + 2, player_ac_data[playerid][numAimbotDetection] = 0;

		player_ac_data[playerid][numAimbotDetection] ++;

		if(player_ac_data[playerid][numAimbotDetection] == 7 && GetDistanceBetweenPlayers(playerid, hitid) < 30)
			va_SendClientMessageToAll(COLOR_WARNING, "{f05d43}anticheat warning:{ffffff} %s (%d, level %d) may be cheating: shoots without targeting [may be using silent aimbot]. weapon: %s.", GetName(playerid), playerid, GetPlayerScore(playerid), GetWeaponNameEx(weaponid));
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

const SILENT_SYNC = 206;
IPacket:206(playerid, BitStream:bs)
{ 
	new bulletData[PR_BulletSync];
	BS_IgnoreBits(bs, 8);
	BS_ReadBulletSync(bs, bulletData);
 
	if(bulletData[PR_hitType] == BULLET_HIT_TYPE_PLAYER)
	{ 
		new Float: getPlayerX, Float: getPlayerY, Float: getPlayerZ, Float: getLastPlayerZ;
 
		GetPlayerPos(playerid, getPlayerX, getPlayerY, getPlayerZ);
		GetPlayerPos(bulletData[PR_hitId], getLastPlayerZ, getLastPlayerZ, getLastPlayerZ);
 
		new Float:betweenDistance = floatabs(bulletData[PR_origin][0] - getPlayerX) + floatabs(bulletData[PR_origin][1] - getPlayerY);
 
		if(bulletData[PR_offsets][0] == 0.0 && bulletData[PR_offsets][1] == 0.0 && bulletData[PR_offsets][2] == 0.0 || betweenDistance < 0.15 || floatabs(bulletData[PR_origin][2] - getPlayerZ) < 0.01 || floatabs(getLastPlayerZ - bulletData[PR_hitPos][2]) < 0.01) 
		{ 
			if(player_ac_data[playerid][aimbotTime] < gettime())
				player_ac_data[playerid][aimbotDetected] = 0;

			player_ac_data[playerid][aimbotDetected] ++;

			if(player_ac_data[playerid][aimbotTime] < gettime())
				player_ac_data[playerid][aimbotTime] = gettime() + 30;

			if(player_ac_data[playerid][aimbotDetected] == 3 || player_ac_data[playerid][aimbotDetected] == 6 || player_ac_data[playerid][aimbotDetected] == 9)
			{
				va_SendClientMessageToAll(COLOR_WARNING, "{f05d43}anticheat warning:{ffffff} %s (%d, level %d) may be cheating: possible aimbot - detected %d times - %.02fm.", GetName(playerid), playerid, GetPlayerScore(playerid), player_ac_data[playerid][aimbotDetected], betweenDistance);
			}

			if(player_ac_data[playerid][aimbotDetected] == 15)
			{
				va_SendClientMessageToAll(COLOR_WARNING, "%s (%d, level %d) has been kicked - reason: possible aimbot #1.", GetName(playerid), playerid, GetPlayerScore(playerid));
				
			}
		} 
	} 
	return 1;
}