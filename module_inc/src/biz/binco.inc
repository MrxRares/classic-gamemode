#include <YSI_Coding\y_hooks>
#include <previewmodeldial>
#define DIALOG_CLOTHES22 		1337

#define SKIN_TYPE_STAFF 1
#define SKIN_TYPE_BRONZE 2
#define SKIN_TYPE_SILVER 3
#define SKIN_TYPE_GOLD 4

stock SkinCategory(category)
{
	new cString[55];
	switch(category)
	{
		case 1: cString = "{1d611a}STAFF{ffffff}";

		case 2: cString = "{415dab}NORMAL{ffffff}";
		case 3: cString = "{a62626}PREMIUM{ffffff}";
		case 4: cString = "{FFFF00}GOLDEN{ffffff}";

		default: cString = "{ffffff}DEFAULT{ffffff}";
	}
	return cString;
}

stock SkinCategoryTD(category)
{
	new skinTD[55];
	switch(category)
	{
		case 1: skinTD = "~g~STAFF~w~";

		case 2: skinTD = "~p~NORMAL~w~";
		case 3: skinTD = "~r~PREMIUM~w~";
		case 4: skinTD = "~y~GOLDEN~w~";

		default: skinTD = "~w~DEFAULT~w~";
	}
	return skinTD;
}

function:giveskinbonus(playerid)
{

    for (new i = 1; i <= playerStaticVar[playerid][pTSkins]; ++i)
    {
        switch(playerInfo[playerid][pSkinType][i])
        {
            case 3: 
            {
                new money = 1000 + random(2000);
                addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, money, false);

                va_SendClientMessage(playerid, COLOR_GREEN, "Ai primit $%s datorita skinului tau.", FormatNumber(money));
            }
            case 4: 
            {
                new money = 2000 + random(4000);
                addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, money, false);
                va_SendClientMessage(playerid, COLOR_GREEN, "Ai primit $%s datorita skinului tau.", FormatNumber(money));
            }
        }
    }
    return true;
}

stock IsSkinOwned(playerid, skin)
{
	new check_skin = 0;
	for(new i; i <= playerStaticVar[playerid][pTSkins]; i ++)
	{
		if(skin == playerInfo[playerid][pOwnedSkins][i])
		{
			check_skin = 1;
			break;
		}
	}
	return check_skin;
}
YCMD:giveskintoplayer(playerid, params[], help)
{
    if(playerInfo[playerid][pAdmin] < 4)
        return returnNoAcces(playerid);

    new returnPlayer, skinID, skinType;
    if(sscanf(params, "iii", returnPlayer, skinID, skinType))
    {
        SendClientMessage(playerid, COLOR_GREY, "Syntax: /giveskintoplayer <playerid> <skin id> <skin type>");
        SendClientMessage(playerid, COLOR_GREY, "Available options: 1 (staff), 2 (normal), 3 (premium), 4 (golden)");
        return true;
    }
    if(!IsPlayerConnected(returnPlayer))
    {
        SendClientMessage(playerid, COLOR_GREY, "Invalid player ID");
        return true;
    }

    InsertPlayerSkin(returnPlayer, skinID, skinType);
    sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s gave skin %d (%s) to %s.", GetName(playerid), skinID, SkinCategory(skinType), GetName(returnPlayer));
    return true;
}

YCMD:clothes(playerid, params[], help)
{
   	if (bizInfo[playerStaticVar[playerid][inBusiness]][bizType] != 8)
		return sendError(playerid,"You are not in a clothing store");

    new string[50 * 32] = "";
    for (new i = 1; i <= playerStaticVar[playerid][pTSkins]; ++i)
    {
        if (playerInfo[playerid][pOwnedSkins][i] == -1)
            continue;

        format(string, sizeof string, "%s%d\t%s\n", string, playerInfo[playerid][pOwnedSkins][i], SkinCategoryTD(playerInfo[playerid][pSkinType][i]));
    }
    SetPVarInt(playerid, "clothes_menu", 1);
    Prev_ShowPlayerDialog(playerid, DIALOG_CLOTHES22, DIALOG_STYLE_PREVIEW_MODEL, "Your skins:", string, "Select", "Cancel");
    return true;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if (dialogid == DIALOG_CLOTHES22)
    {
        if (!response || GetPVarInt(playerid, "clothes_menu") < 1)
            return Y_HOOKS_CONTINUE_RETURN_1;

        if(GetMoney(playerid) < 5000 && GetMilliards(playerid) < 1)
            return sendError(playerid,"You need 5000$ to do this action.");

        new skinID = -1;
        new currentIndex = 0;

        for (new i = 1; i <= playerStaticVar[playerid][pTSkins]; ++i)
        {
            if (playerInfo[playerid][pOwnedSkins][i] == -1)
                continue;

            if (currentIndex == listitem)
            {
                skinID = playerInfo[playerid][pOwnedSkins][i];
                break;
            }

            currentIndex++;
        }

        if (skinID == -1)
        {
            return SendClientMessage(playerid, COLOR_GREY, "Error: Invalid skin selection.");
        }

        playerInfo[playerid][pSkin] = skinID;
        SetPlayerSkin(playerid, skinID);
        substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 5000);
        GameTextForPlayer(playerid, "~r~-$5,000", 5000, 1);
        DailyQuestCheck(playerid, QUEST_TYPE_CLOTHES, 1);
        bizInfo[playerStaticVar[playerid][inBusiness]][bizMoney] += 5000;

        mysql_format(SQL, returnString, sizeof(returnString), "UPDATE `businesses` SET `bizMoney` = '%d' WHERE `id` = '%d';", bizInfo[playerStaticVar[playerid][inBusiness]][bizMoney], playerStaticVar[playerid][inBusiness]);
        mysql_tquery(SQL, returnString, "", "");

        mysql_format(SQL, returnString, sizeof(returnString), "UPDATE `users` SET `Skin` = '%d' WHERE `id` = '%d';", skinID, playerInfo[playerid][pSQLID]);
        mysql_tquery(SQL, returnString, "", "");

        DeletePVar(playerid, "clothes_menu");
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

stock InsertPlayerSkin(playerid, skinId, type)
{
    playerStaticVar[playerid][pTSkins]++;
    new currentSkinIndex = playerStaticVar[playerid][pTSkins];
   
    playerInfo[playerid][pOwnedSkins][currentSkinIndex] = skinId;
    playerInfo[playerid][pSkinType][currentSkinIndex] = type;

    mysql_format(SQL, returnString, sizeof(returnString),
        "UPDATE `users` SET `acc_skins` = CONCAT(`acc_skins`, '%d|'), `acc_skins_type` = CONCAT(`acc_skins_type`, '%d|') WHERE `id` = '%d';",
        skinId, type, playerInfo[playerid][pSQLID]);

    mysql_tquery(SQL, returnString, "", "");

    format(playerStaticVar[playerid][pStrSkins], 1024, "%s%d|", playerStaticVar[playerid][pStrSkins], skinId);
    format(playerStaticVar[playerid][pStrType], 1024, "%s%d|", playerStaticVar[playerid][pStrType], type);
    return true;
}
hook WhenPlayerLoggedIn(playerid)
{
	if(!cache_num_rows())
		return Y_HOOKS_CONTINUE_RETURN_1;

    cache_get_value_name_int(0, "Skin", playerInfo[playerid][pSkin]);
	SetPlayerSkin(playerid, playerInfo[playerid][pSkin]);

	cache_get_value_name(0, "acc_skins", playerStaticVar[playerid][pStrSkins]);
	cache_get_value_name(0, "acc_skins_type", playerStaticVar[playerid][pStrType]);
	
	new skinLoad[50], skinModule = 0, skinTotal = 0, skinString[1024];
	cache_get_value_name(0, "acc_skins", skinString);

	for(new i; i < strlen(skinString); i ++)
	{
        if(skinString[i] == '|')
        {
            playerInfo[playerid][pOwnedSkins][skinModule] = strval(skinLoad);

            skinModule ++;
            while(skinTotal)
            {
            	skinLoad[skinTotal] = ' ';
            	skinTotal --;
            }
        }
        else
        {
            skinLoad[skinTotal] = skinString[i];

            skinTotal ++;
            skinString[i] = ' ';
        }
    }

    new typeLoad[50], typeModule = 0, typeTotal = 0, typeString[1024];
    cache_get_value_name(0, "acc_skins_type", typeString);

    for(new i; i < strlen(typeString); i ++)
	{
        if(typeString[i] == '|')
        {
            playerInfo[playerid][pSkinType][typeModule] = strval(typeLoad);

            typeModule ++;
            while(typeTotal)
            {
            	typeLoad[typeTotal] = ' ';
            	typeTotal --;
            }
        }
        else
        {
            typeLoad[typeTotal] = typeString[i];

            typeTotal ++;
            typeString[i] = ' ';
        }
    }
    playerStaticVar[playerid][pTSkins] = typeModule - 1;

    return Y_HOOKS_CONTINUE_RETURN_1;
}
