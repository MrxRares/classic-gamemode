#include <YSI_Coding\y_hooks>
#include <map>
#define TOTAL_OBJECTS_IN_STORE 6

new Text:StoreTD[3], playerStoreObject[MAX_PLAYERS];

hook OnGameModeInit()
{
	StoreTD[0] = TextDrawCreate(457.666687 + 10.0, 300.599822, "LD_BEAT:right");
	TextDrawLetterSize(StoreTD[0], 0.000000, 0.000000);
	TextDrawTextSize(StoreTD[0], 30.000000, 30.000000);
	TextDrawAlignment(StoreTD[0], 1);
	TextDrawColour(StoreTD[0], -1);
	TextDrawSetShadow(StoreTD[0], 0);
	TextDrawSetOutline(StoreTD[0], 0);
	TextDrawBackgroundColour(StoreTD[0], 255);
	TextDrawFont(StoreTD[0], 4);
	TextDrawSetProportional(StoreTD[0], 0);
	TextDrawSetShadow(StoreTD[0], 0);
	TextDrawSetSelectable(StoreTD[0], true);

	StoreTD[1] = TextDrawCreate(157.666687 + 10.0, 300.599822, "LD_BEAT:left");
	TextDrawLetterSize(StoreTD[1], 0.000000, 0.000000);
	TextDrawTextSize(StoreTD[1], 30.000000, 30.000000);
	TextDrawAlignment(StoreTD[1], 1);
	TextDrawColour(StoreTD[1], -1);
	TextDrawSetShadow(StoreTD[1], 0);
	TextDrawSetOutline(StoreTD[1], 0);
	TextDrawBackgroundColour(StoreTD[1], 255);
	TextDrawFont(StoreTD[1], 4);
	TextDrawSetProportional(StoreTD[1], 0);
	TextDrawSetShadow(StoreTD[1], 0);
	TextDrawSetSelectable(StoreTD[1], true);

	StoreTD[2] = TextDrawCreate(317.999938 + 10.0, 351.970031, "Buy");
	TextDrawLetterSize(StoreTD[2], 0.400000, 1.600000);
	TextDrawTextSize(StoreTD[2], 20.000000, 20.000000);
	TextDrawAlignment(StoreTD[2], 2);
	TextDrawColour(StoreTD[2], -1);
	TextDrawSetShadow(StoreTD[2], 0);
	TextDrawSetOutline(StoreTD[2], 1);
	TextDrawBackgroundColour(StoreTD[2], 255);
	TextDrawFont(StoreTD[2], 3);
	TextDrawSetProportional(StoreTD[2], 1);
	TextDrawSetShadow(StoreTD[2], 0);
	TextDrawSetSelectable(StoreTD[2], true);

	CreateDynamicObject(2180, 214.42130, -160.17149, 999.51892, 0.00000, 0.00000, 91.33333, -1, 14);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

show_store_name(item)
{
	new returnName[16];
	switch(item)
	{
		case 1: returnName = "Phone";
		case 2: returnName = "Phone Book";
		case 3: returnName = "Walkie Talkie";
		case 4: returnName = "Phone Credit";
		case 5: returnName = "Gascan";
		case 6: returnName = "Camera";
	}
	return returnName;
}
show_store_price(item)
{
	switch(item)
	{
		case 1: return 1500;
		case 2: return 2000;
		case 3: return 3500;
		case 4: return 500;
		case 5: return 10000;
		case 6: return 2500;
	}
	return false;
}
show_store_object(item)
{
	new returnStoreObject;
	switch(item)
	{
		case 1: returnStoreObject = 18874;
		case 2: returnStoreObject = 2894;
		case 3: returnStoreObject = 19942;
		case 4: returnStoreObject = 19234;
		case 5: returnStoreObject = 1650;
		case 6: returnStoreObject = 367;
	}
	return returnStoreObject;
}
show_store_item(playerid, item)
{
	if(playerStoreObject[playerid] != INVALID_OBJECT_ID)
		DestroyPlayerObject(playerid, playerStoreObject[playerid]);

	new Float:inZ;
	switch(item)
	{
		case 1: inZ = 0.0;
		case 2: inZ = 0.0;
		case 3: inZ = 0.1;
		case 4: inZ = 0.0;
		case 5: inZ = 0.35;
		case 6: inZ = 0.025;
	}

	playerStoreObject[playerid] = CreatePlayerObject(playerid, show_store_object(item), 214.48331, -159.74350 + 0.033, 1000.29962 + 0.025 + inZ, 0.00000, 0.00000, 90.00000, 10.0);

	new returnStoreString[32];
	format(returnStoreString, sizeof returnStoreString, "%s~n~$%s", show_store_name(item), FormatNumber(show_store_price(item)));
	PlayerTextDrawSetString(playerid, playerTextdraws[playerid][StorePTD], returnStoreString);

	for(new i = 0; i <= 2; i++)
		TextDrawShowForPlayer(playerid, StoreTD[i]);

	PlayerTextDrawShow(playerid, playerTextdraws[playerid][StorePTD]);
	return true;
}

leave_player_store(playerid)
{
	if(GetPVarInt(playerid, "store_item") <= 0)
		return true;

	DestroyPlayerObject(playerid, playerStoreObject[playerid]);

	for(new i; i <= 2; i++)
		TextDrawHideForPlayer(playerid, StoreTD[i]);

	PlayerTextDrawHide(playerid, playerTextdraws[playerid][StorePTD]);

	DestroyDynamicObject(playerStoreObject[playerid]);

	DeletePVar(playerid, "store_item");
	SetCameraBehindPlayer(playerid);

	SetPlayerPos(playerid, playerStaticVar[playerid][pLastBizCoords][0], playerStaticVar[playerid][pLastBizCoords][1], playerStaticVar[playerid][pLastBizCoords][2]);
	SetPlayerInterior(playerid, 6);
	return true;
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if(clickedid == Text:INVALID_TEXT_DRAW)
	{
		if(GetPVarInt(playerid, "store_item") > 0)
		{
			SendClientMessage(playerid, COLOR_SERVER, "(Shop):{ffffff} Multumim pentru folosirea serviciilor acestui SHOP, o zi buna in continuare!");
			leave_player_store(playerid);
		}
		return true;
	}
	
	if(clickedid == StoreTD[0])
	{
		if(GetPVarInt(playerid, "store_item") >= TOTAL_OBJECTS_IN_STORE)
			return SetPVarInt(playerid, "store_item", 1), show_store_item(playerid, 1);

		SetPVarInt(playerid, "store_item", GetPVarInt(playerid, "store_item") + 1);

		show_store_item(playerid, GetPVarInt(playerid, "store_item"));
		return true;
	}
	if(clickedid == StoreTD[1])
	{
		if(GetPVarInt(playerid, "store_item") <= 1)
			return SetPVarInt(playerid, "store_item", TOTAL_OBJECTS_IN_STORE), show_store_item(playerid, TOTAL_OBJECTS_IN_STORE);

		SetPVarInt(playerid, "store_item", GetPVarInt(playerid, "store_item") - 1);

		show_store_item(playerid, GetPVarInt(playerid, "store_item"));
		return true;
	}
	if(clickedid == StoreTD[2])
	{
		if(GetPVarInt(playerid, "store_item") < 1)
			return true;

		//check_delay

        if(GetMoney(playerid) < show_store_price(GetPVarInt(playerid, "store_item")) && GetMilliards(playerid) < 1)
            return sendError(playerid,"Nu ai suficienti bani pentru a putea cumpara acest item.");


		switch(GetPVarInt(playerid, "store_item"))
		{
			case 1:
			{
				if(playerInfo[playerid][pPhone] == 1) 
                	return sendError(playerid,"You cannot buy a phone because you have an iPhone.");

	            new const number = random(999999 - 100000 + 1) + 100000;
	            mysql_format(SQL, returnString, sizeof returnString, "SELECT (ID) FROM `users` WHERE `Phone` = %d LIMIT 1", number);
	            return mysql_tquery(SQL, returnString, "buyPhoneNumber", "dd", playerid, number);
			}
			case 2:
			{
	            if(playerInfo[playerid][pPhoneBook])
	                return SendClientMessage(playerid, COLOR_SERVER, "(Shop):{ffffff}You can't buy this item, this item can be bought one time.");

	            playerInfo[playerid][pPhoneBook] = true;
	            SendClientMessage(playerid, COLOR_SERVER, "(Shop): {FFFFFF}You purchased a phone book for $500. Use command [/number].");
			}
			case 3:
			{
				if(playerInfo[playerid][pWalkieTalkie])
					return SendClientMessage(playerid, COLOR_SERVER, "(Shop):{ffffff}You can't buy this item, this item can be bought one time.");
				
				playerInfo[playerid][pWalkieTalkie] = 1;
			}
			case 4: {
	            if(playerInfo[playerid][pPhoneNumber] == 0)
	                return sendError(playerid,"Nu ai un telefon.");

	            if(playerInfo[playerid][pPhoneCredit] > 200)
	                return sendError(playerid,"Nu poti cumpara credit deoarece detii mai mult de 200.");

	            playerInfo[playerid][pPhoneCredit] += 50;
	            SendClientMessage(playerid, COLOR_SERVER, "(Shop): {FFFFFF}You purchased 50 credit for your phone for $1.000.");
			}
			case 5:
			{
				if(playerInfo[playerid][pGascan])
					return sendError(playerid,"Nu poti achizitiona acest item, deoarece acesta poate fi achizitionat o singura data.");
				
				playerInfo[playerid][pGascan] = 1;
			}
			case 6: GivePlayerWeapon(playerid, WEAPON_CAMERA, 9999);
		}

		// va_SendClientMessage(playerid, COLOR_GREY, "Ai cumparat un item de tip %s (model:%d) pentru suma de $%s.", show_store_name(GetPVarInt(playerid, "store_item")), show_store_object(GetPVarInt(playerid, "store_item")), FormatNumber(show_store_price(GetPVarInt(playerid, "store_item"))));

		bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney] += show_store_price(GetPVarInt(playerid, "store_item"));
		substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, show_store_price(GetPVarInt(playerid, "store_item")));

	
		mysql_format(SQL, returnString, sizeof returnString, "update `users` set `PhoneBook` = '%d', `WalkieTalkie` = '%d', `PhoneCredit` = '%d', `gasCan` = '%d' where `id` = '%d';", playerInfo[playerid][pPhoneBook], playerInfo[playerid][pWalkieTalkie], playerInfo[playerid][pPhoneCredit], playerInfo[playerid][pGascan], playerInfo[playerid][pSQLID]);
		mysql_tquery(SQL, returnString, "", "");

		mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `bizMoney` = '%d' where `id` = '%d';", bizInfo[GetPlayerVirtualWorld(playerid)][bizMoney], GetPlayerVirtualWorld(playerid));
		mysql_tquery(SQL, returnString, "", "");

		//set_delay;
		return true;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}
function:buyPhoneNumber(const playerid, number) {
    if(!IsPlayerConnected(playerid))
        return 1;

    if(cache_num_rows()) {
        number = random(999999 - 100000 + 1) + 100000;

        mysql_format(SQL, returnString, sizeof returnString, "SELECT (ID) FROM `users` WHERE `Phone` = %d LIMIT 1", number);
        return mysql_tquery(SQL, returnString, "buyPhoneNumber", "dd", playerid, number);
    }

    if(playerInfo[playerid][pPhoneNumber] != 0)
        MAP_remove_val(phoneNumbers, playerInfo[playerid][pPhoneNumber]);

    playerInfo[playerid][pPhoneNumber] = number;
    MAP_insert_val_val(phoneNumbers, playerInfo[playerid][pPhoneNumber], playerid);

    substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 1500);

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Phone` = '%d' where `id` = '%d';", playerInfo[playerid][pPhoneNumber], playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");

    return va_SendClientMessage(playerid, COLOR_GREEN, "(Shop): {ffffff}You purchased a new phone for $1.500. Your actual number is: %d.", number);
}
hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
	if(newkeys == KEY_YES)
	{
		if(IsPlayerInRangeOfPoint(playerid, 4.0, -23.5991, -57.3145, 1003.5469))
		{
			GetPlayerPos(playerid, playerStaticVar[playerid][pLastBizCoords][0], playerStaticVar[playerid][pLastBizCoords][1], playerStaticVar[playerid][pLastBizCoords][2]);

			SetPlayerPos(playerid, 216.9780, -159.6668, 1000.5306);
			SetPlayerCameraPos(playerid, 216.9780 - 0.2, -159.6668, 1000.5306 + 0.5);
			SetPlayerCameraLookAt(playerid, 214.48331, -159.74350, 1000.29962);

			SendClientMessage(playerid, -1, "Pentru a iesi din meniul de cumparare al itemelor apasa tasta ESC.");
			SelectTextDraw(playerid, 0x4c92d9FF);
			SetPlayerInterior(playerid, 14);

			SetPVarInt(playerid, "store_item", 1);
			DeletePVar(playerid, "gs_item");

			show_store_item(playerid, 1);
			return true;
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}