#include <YSI_Coding\y_hooks>

hook OnPlayerEnterDynArea(playerid, areaid)
{
	new business_action = Streamer_GetIntData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID) - STREAMER_BEGIN_BUSINESS_SPHERE;
	if(returnServerBusinesses < business_action || business_action < 0)
		return Y_HOOKS_CONTINUE_RETURN_1;

	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
		SetPVarInt(playerid, "business_sphere_action", business_action);
	
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerLeaveDynArea(playerid, areaid)
{
	if(GetPVarInt(playerid, "business_sphere_action") > 0 && bizInfo[GetPVarInt(playerid, "business_sphere_action")][bizType] == 10)
	{
		new business_action = GetPVarInt(playerid, "business_sphere_action");

		substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, bizInfo[business_action][bizFee]);

		GameTextForPlayer(playerid, va_return("~r~-%d$", bizInfo[business_action][bizFee]), 5000, 1);
		bizInfo[business_action][bizMoney] += bizInfo[business_action][bizFee];
		
		mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `bizMoney` = '%d' where `id` = '%d';", bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney], GetPlayerVirtualWorld(playerid));
		mysql_tquery(SQL, returnString, "", "");
	}
	DeletePVar(playerid, "business_sphere_action");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

public OnEnterExitModShop(playerid, enterexit, interiorid)
{
	if(enterexit)
		return SetPVarInt(playerid, "modshop_bizId", GetPVarInt(playerid, "business_sphere_action"));

	if(GetPVarInt(playerid, "modshop_bizId") && bizInfo[GetPVarInt(playerid, "modshop_bizId")][bizType] == 11)
	{
		new business_action = GetPVarInt(playerid, "modshop_bizId");

		SendClientMessage(playerid, COLOR_SERVER, "(Info):{ffffff} Coliziunile ti-au fost dezactivate timp de 15 secunde pentru a facilita iesirea din tunning.");
		va_SendClientMessage(playerid, COLOR_GREY, "You paid $3.500 because you used a vehicle mod shop (bizID: %d)", business_action), bizInfo[business_action][bizMoney] += 3500;
		
		substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, bizInfo[business_action][bizFee]);

		GameTextForPlayer(playerid, va_return("~r~-%d$", bizInfo[business_action][bizFee]), 5000, 1);
		bizInfo[business_action][bizMoney] += bizInfo[business_action][bizFee];
		
		mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `bizMoney` = '%d' where `id` = '%d';", bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney], GetPlayerVirtualWorld(playerid));
		mysql_tquery(SQL, returnString, "", "");
	}
	DeletePVar(playerid, "modshop_bizId");
	return true;
}