#include <YSI_Coding\y_hooks>


set_find_checkpoint(playerid, targetid)
{
	new Float:X, Float:Y, Float:Z;
	GetPlayerPos(targetid, X, Y, Z);
	
	DisablePlayerCheckpoint(playerid);
	SetPlayerCheckpoint(playerid, X, Y, Z, 3.5);

	playerStaticVar[playerid][usedFind] = 1;
	playerStaticVar[playerid][targetFind] = targetid;
	playerStaticVar[playerid][pCheckpoint] = 2;
	PlayerTextDrawShow(playerid, playerTextdraws[playerid][FindTD]);
	return true;
}

YCMD:find(playerid, params[], help)
{

	if(job_data[playerid][playerJob] != 1 && factionType[playerInfo[playerid][pGroup]] != FACTION_TYPE_POLICE && playerInfo[playerid][pGroup] != 11)
		return sendError(playerid,"You are not a detective.");
	
	if(playerStaticVar[playerid][pCheckpoint])
		return showActiveCheckpointDialog(playerid);

	new targetid;
	if(sscanf(params, "u", targetid))
		return sendSyntaxMessage(playerid, "/find [name/playerid]");
	
	if(!IsPlayerConnected(targetid) || targetid == playerid)
		return sendError(playerid,"Invalid player.");

    if(!playerInfo[targetid][pTOGFind])
        return sendError(playerid, "Acel jucator nu poate fi localizat.");


	set_find_checkpoint(playerid, targetid);
	va_SendClientMessage(playerid, -1, "{FFFF00}Checkpoint-ul va afisa locatia playerului %s. Distanta pana la player: %.0f.", GetName(targetid), GetDistanceBetweenPlayers(playerid, targetid));
 	return true;
}

cancel_find(playerid)
{
	playerStaticVar[playerid][targetFind] = INVALID_PLAYER_ID;
	playerStaticVar[playerid][usedFind] = 0;

	DisablePlayerCheckpoint(playerid);
	playerStaticVar[playerid][pCheckpoint] = 0;
	PlayerTextDrawHide(playerid, playerTextdraws[playerid][FindTD]);
	return true;
}

bool:IsValidBizPosition(bizId) {
    return bizInfo[bizId][bizPosition][0] != 0.0 || bizInfo[bizId][bizPosition][1] != 0.0 || bizInfo[bizId][bizPosition][2] != 0.0;
}

bool:IsValidHousePosition(houseId) {
    return houseInfo[houseId][houseExterior][0] != 0.0 || houseInfo[houseId][houseExterior][1] != 0.0 || houseInfo[houseId][houseExterior][2] != 0.0;
}

bool:IsValidFactionPosition(factionid) {
    return groupInfo[factionid][factionExterior][0] != 0.0 || groupInfo[factionid][factionExterior][1] != 0.0 || groupInfo[factionid][factionExterior][2] != 0.0;
}
ptask findOutPlayer[1000](playerid) {
    if (IsPlayerConnected(playerid)) {
        if (playerStaticVar[playerid][usedFind] == 1) {
            new Float:player_pos[6], findId = playerStaticVar[playerid][targetFind], Float:distance,
                bizId = playerStaticVar[findId][inBusiness], houseId = playerStaticVar[findId][inHouse], factionid = playerStaticVar[findId][inHQ];

            if (!IsPlayerConnected(findId)) {
                SendClientMessage(playerid, COLOR_GREY, "The player you had the checkpoint on has disconnected!");
                GameTextForPlayer(playerid, "target left the game!", 3000, 1);
                return cancel_find(playerid);
            }

            GetPlayerPos(playerid, player_pos[0], player_pos[1], player_pos[2]);
            GetPlayerPos(findId, player_pos[3], player_pos[4], player_pos[5]);

            SetPlayerCheckpoint(playerid, player_pos[3], player_pos[4], player_pos[5], 4);
            distance = GetDistanceBetweenPlayers(playerid, findId);

            if (bizId > 0) {
                if (IsValidBizPosition(bizId)) {
                    SetPlayerCheckpoint(playerid, bizInfo[bizId][bizPosition][0], bizInfo[bizId][bizPosition][1], bizInfo[bizId][bizPosition][2], 4);
                    distance = GetDistanceBetweenPoints(player_pos[0], player_pos[1], player_pos[2], bizInfo[bizId][bizPosition][0], bizInfo[bizId][bizPosition][1], bizInfo[bizId][bizPosition][2]);
                }
            } else if (houseId > -1) {
                if (IsValidHousePosition(houseId)) {
                    SetPlayerCheckpoint(playerid, houseInfo[houseId][houseExterior][0], houseInfo[houseId][houseExterior][1], houseInfo[houseId][houseExterior][2], 4);
                    distance = GetDistanceBetweenPoints(player_pos[0], player_pos[1], player_pos[2], houseInfo[houseId][houseExterior][0], houseInfo[houseId][houseExterior][1], houseInfo[houseId][houseExterior][2]);
                }
            } else if (factionid > 0) {
                if (IsValidFactionPosition(factionid)) {
                    SetPlayerCheckpoint(playerid, groupInfo[factionid][factionExterior][0], groupInfo[factionid][factionExterior][1], groupInfo[factionid][factionExterior][2], 4);
                    distance = GetDistanceBetweenPoints(player_pos[0], player_pos[1], player_pos[2], groupInfo[factionid][factionExterior][0], groupInfo[factionid][factionExterior][1], groupInfo[factionid][factionExterior][2]);
                }
            }


            format(returnString, sizeof(returnString), "~w~TARGET:   ~r~%s(%d)~n~~w~DISTANCE:   ~r~%.0f(%s)", GetName(findId), findId, distance, GetCoordinatesCity(player_pos[3], player_pos[4]));

            PlayerTextDrawSetString(playerid, playerTextdraws[playerid][FindTD], returnString);
            PlayerTextDrawShow(playerid, playerTextdraws[playerid][FindTD]);
        }
    }
    return true;
}
