/*  =============================================================================

		Credits: ionchyAdv
		Help & Others: iReplay
	
		FormatIntNumberLY - can be used to format a number less than 2.147.483.647
		CreateLocalLY - separates the string (money amount) into two variables as prefix and suffix
		GivePlayerCash - give money specific player (extra money over limit [999.999.999] will be added in bank)
		GivePlayerBank - adding money to bank using prefix and suffix
		OperatorBankCheck - check if player have enough money in bank compared to prefix and suffix

    ============================================================================== */


/*
i have to use this beacuse pawn is a goofy ass lang and i should stop using it <3
also omp sucks but is better </3
*/
#pragma warning disable 240, 239

#define MAX_LY_LENGHT (10)
#define MAX_LY_STRING (30)

#define MAX_LY_PREFIX (999999999)


function:CreateLocalLY(&varPrefix, &varSuffix, amountString[])
{
	new
		varPrefixStr[MAX_LY_STRING], varSuffixStr[MAX_LY_STRING];

	if(strlen(amountString) >= MAX_LY_LENGHT)
	{
		strmid(varPrefixStr, amountString, 0, strlen(amountString)+1 - MAX_LY_LENGHT);

		strdel(amountString, 0, strlen(amountString)+1 - MAX_LY_LENGHT);
	}
	format(varSuffixStr, MAX_LY_STRING, amountString); // setting remaining money as Suffix

	varPrefix = strval(varPrefixStr);
	varSuffix = strval(varSuffixStr);

	return 1;
}

function:hide_money_update(playerid) {
	
    if(IsValidPlayerTextDraw(playerid, playerTextdraws[playerid][moneyTD])) {
        TogglePlayerMoneyCatching(playerid, false);
    }
    else return true;
    return 0;
}
forward public GivePlayerCash(playerid, moneyAmount);
public GivePlayerCash(playerid, moneyAmount)
{
	playerInfo[playerid][pMoney] += moneyAmount;

    if(playerInfo[playerid][pMoney] > MAX_LY_PREFIX && moneyAmount)
    {
    	new
    		newBankMoney = playerInfo[playerid][pMoney] - MAX_LY_PREFIX;

    	playerInfo[playerid][pAccount] += newBankMoney;
    	playerInfo[playerid][pMoney] -= newBankMoney;

        if(playerInfo[playerid][pAccount] >= MAX_LY_PREFIX)
        {
            playerInfo[playerid][pAccount] -= MAX_LY_PREFIX;
            playerInfo[playerid][pAccountLY] ++;
        }
		new stringnew2[256];
        mysql_format(SQL, stringnew2, 256, "UPDATE `users` SET `Bank` = '%d' WHERE `id` = '%d' LIMIT 1", playerInfo[playerid][pAccount], playerInfo[playerid][pSQLID]);
        mysql_tquery(SQL, stringnew2);

        SendClientMessage(playerid, 0xFF8080FF, "(money update): money limit reached! the extra money was added into the bank.");
    }
    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid, playerInfo[playerid][pMoney]);
    new stringnew[256];
    mysql_format(SQL, stringnew, 256, "UPDATE `users` SET `Money` = '%d' WHERE `id` = '%d' LIMIT 1", GetPlayerCash(playerid), playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, stringnew);

    if(playerInfo[playerid][pHUD][8])
    {
        TogglePlayerMoneyCatching(playerid, true);
        
	    if(moneyAmount < 0)
			PlayerTextDrawSetString(playerid, playerTextdraws[playerid][moneyTD], "~r~- $%s", FormatIntNumberLY(moneyAmount));
		else
			PlayerTextDrawSetString(playerid, playerTextdraws[playerid][moneyTD], "+ $%s", FormatIntNumberLY(moneyAmount));

		PlayerTextDrawShow(playerid, playerTextdraws[playerid][moneyTD]);

		SetTimerEx("hide_money_update", 2000, false, "i", playerid);
	}
    return true;
}

forward public GivePlayerBank(playerid, moneyPrefix, moneySuffix);
public GivePlayerBank(playerid, moneyPrefix, moneySuffix)
{
	playerInfo[playerid][pAccount] += moneySuffix;

    if(playerInfo[playerid][pAccount] >= MAX_LY_PREFIX)
    {
        playerInfo[playerid][pAccount] -= MAX_LY_PREFIX;
        playerInfo[playerid][pAccountLY] ++;
    }

    playerInfo[playerid][pAccountLY] += moneyPrefix;
    return 1;
}


stock GetPlayerBank(playerid)
{
	new stringMoney[64];
	string_FromatValue(playerInfo[playerid][pAccountLY], playerInfo[playerid][pAccount], stringMoney);
	format(stringMoney, 64, FormatNumberLYEx(stringMoney));

	return stringMoney;
}

// ================================================================================================= //

stock string_FromatValue(&prefix, &suffix, value[])
{
	if(!prefix)
		format(value, 128, "%d", suffix);
	else
		format(value, 128, "%d%09d", prefix, suffix);
}

stock FormatNumber(iNum, const szChar[] = ".")
{
    new
        string[16]
    ;
    format(string, sizeof(string), "%d", iNum);

    for(new iLen = strlen(string) - 3; iLen > 0; iLen -= 3)
    {
        strins(string, szChar, iLen);
    }
    return string;
}

stock FormatNumberLY(...)
{
	new
		stringNumber[128], int_str = 0, firstchar[4];

	format(firstchar, sizeof firstchar, "%s", getarg(0, 0));

	if(strfind(firstchar, "#", true) == 0)
	{
		int_str = 1;
		
		new
			nr = 0;
		
		for(new i = 0; i < 128; i++)
		{
			stringNumber[nr] = getarg(0, i); 
			nr++;
		}
	}	
	if(int_str == 1)
	{
		strdel(stringNumber, 0, 1); 
		for(new i = strlen(stringNumber) - 3; i > 0; i -= 3)
			strins(stringNumber, ",", i);
	}

	if(int_str == 0)
		format(stringNumber, sizeof stringNumber, "%s", FormatIntNumberLY(getarg(0)));

	return stringNumber;
}

stock FormatNumberLYEx(const int[], const chars[] = ",", l=-1)
{																
	new moneyString[128] = '\0';
	l = strcat(moneyString, int);																					
	while((l = l - 3) > 0) strins(moneyString, chars, l);																				
    return moneyString;																											
}

stock FormatIntNumberLY(intNumber, const delimitator[] = ",")
{
    new
    	sugeSancky[16], bool: negativeNumber = false;

    if(intNumber < 0)
        intNumber *= -1, negativeNumber = true;

    format(sugeSancky, sizeof sugeSancky, "%i", intNumber);

    for(new i = strlen(sugeSancky) - 3; i > 0; i -= 3)
        strins(sugeSancky, delimitator, i);

    if(negativeNumber == true)
        strins(sugeSancky, "", 0);
	
    return sugeSancky;
}

stock GetPlayerCash(playerid) return playerInfo[playerid][pMoney];
stock ResetPlayerCash(playerid) return playerInfo[playerid][pMoney] = 0;

/*
	pentru prostii ca rifla si concy.
	
	nu o sa folosim niciodata formatintnumber e un simplu formatnumber
*/

// ------------------------------------- //
//         Add/Substract functions       //
// ------------------------------------- //

addBankMoney(playerid, &prefix_val, &suffix_val, prefix, suffix, const bool:query = true) { 
    suffix_val += suffix;
    
    if(suffix_val >= 1000000000) {
        ++prefix_val;
        suffix_val -= 1000000000;
    }

    prefix_val += prefix;

    playerInfo[playerid][pAccount] = suffix_val;
    playerInfo[playerid][pAccountLY] = prefix_val;

    if(query)
        va_mysql_tquery("UPDATE `users` SET `BankLY` = %d, `Bank` = %d WHERE `ID` = %d", prefix_val, suffix_val, playerInfo[playerid][pSQLID]);
    
    return 1;
}

substractBankMoney(playerid, &prefix_val, &suffix_val, prefix, suffix, const bool:query = true) {
    if(suffix > 999999999) 
        return 0;
    
    suffix_val -= suffix;
    
    if(suffix_val < 0 && prefix_val > 0) {
        --prefix_val;
        suffix_val += 1000000000;
    }

    if(suffix_val < 0 && !prefix_val)
        suffix_val = 0;

    if(prefix != 0) {
        prefix_val -= prefix;

        if(prefix_val < 0)
            prefix_val = 0;
    }

    playerInfo[playerid][pAccount] = suffix_val;
    playerInfo[playerid][pAccountLY] = prefix_val;

    if(query)
        va_mysql_tquery("UPDATE `users` SET `BankLY` = %d, `Bank` = %d WHERE `ID` = %d", prefix_val, suffix_val, playerInfo[playerid][pSQLID]);
    
    return 1;
}

stock addMoneyOffline(sqlid, &prefix_val, &suffix_val, prefix, suffix, const bool:query = true) {   
    suffix_val += suffix;
    
    if(suffix_val >= 1000000000) {
        ++prefix_val;
        suffix_val -= 1000000000;
    }

    prefix_val += prefix;

    if(query)
        va_mysql_tquery("UPDATE `users` SET `MoneyStore` = %d, `Money` = %d WHERE `ID` = %d", prefix_val, suffix_val, sqlid);
    
    return 1;
}

stock substractMoneyOffline(sqlid, &prefix_val, &suffix_val, prefix, suffix, const bool:query = true) {
    if(suffix > 999999999) 
        return 0;
    
    suffix_val -= suffix;
    
    if(suffix_val < 0 && prefix_val > 0) {
        --prefix_val;
        suffix_val += 1000000000;
    }

    if(suffix_val < 0 && !prefix_val)
        suffix_val = 0;

    if(prefix != 0) {
        prefix_val -= prefix;

        if(prefix_val < 0)
            prefix_val = 0;
    }

    if(query)
        va_mysql_tquery("UPDATE `users` SET `MoneyStore` = %d, `Money` = %d WHERE `ID` = %d", prefix_val, suffix_val, sqlid);
    
    return 1;
}

addMoney(playerid, &prefix_val, &suffix_val, prefix, suffix, const bool:query = true) { 
    suffix_val += suffix;
    
    if(suffix_val >= 1000000000) {
        ++prefix_val;
        suffix_val -= 1000000000;
    }

    prefix_val += prefix;

    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid, GetMoney(playerid) <= 999999998 && GetMilliards(playerid) < 1 ? suffix_val : 999999999);

    playerInfo[playerid][pMoney] = suffix_val;
    playerInfo[playerid][pMoneyStore] = prefix_val;

    if(query)
        va_mysql_tquery("UPDATE `users` SET `MoneyStore` = %d, `Money` = %d WHERE `ID` = %d", prefix_val, suffix_val, playerInfo[playerid][pSQLID]);


    if(playerInfo[playerid][pHUD][8])
    {
        TogglePlayerMoneyCatching(playerid, true);

        PlayerTextDrawSetString(playerid, playerTextdraws[playerid][moneyTD], "+ $%s", FormatIntNumberLY(suffix));
        PlayerTextDrawShow(playerid, playerTextdraws[playerid][moneyTD]);

        SetTimerEx("hide_money_update", 2000, false, "i", playerid);
    }
    return 1;
}

YCMD:sofutpemata(playerid, params[], help) {
    TogglePlayerMoneyCatching(playerid, true);
    return true;
}

TogglePlayerMoneyCatching(playerid, bool:show)
{
    if(show)
    {
        PlayerTextDrawShow(playerid, playerTextdraws[playerid][moneyTD]);
    }
    else 
    {
        PlayerTextDrawHide(playerid, playerTextdraws[playerid][moneyTD]);
    }
    return 0;
}

substractMoney(playerid, &prefix_val, &suffix_val, prefix, suffix, const bool:query = true) {
    if(suffix > 999999999) 
        return 0;
    
    suffix_val -= suffix;
    
    if(suffix_val < 0 && prefix_val > 0) {
        --prefix_val;
        suffix_val += 1000000000;
    }

    if(suffix_val < 0 && !prefix_val)
        suffix_val = 0;

    if(prefix != 0) {
        prefix_val -= prefix;

        if(prefix_val < 0)
            prefix_val = 0;
    }

    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid, GetMoney(playerid) <= 999999998 && GetMilliards(playerid) < 1 ? suffix_val : 999999999);

    playerInfo[playerid][pMoney] = suffix_val;
    playerInfo[playerid][pMoneyStore] = prefix_val;
    
    if(query)
        va_mysql_tquery("UPDATE `users` SET `MoneyStore` = %d, `Money` = %d WHERE `ID` = %d", prefix_val, suffix_val, playerInfo[playerid][pSQLID]);
    
    if(playerInfo[playerid][pHUD][8])
    {
        PlayerTextDrawSetString(playerid, playerTextdraws[playerid][moneyTD], "~r~- $%s", FormatIntNumberLY(suffix));
        PlayerTextDrawShow(playerid, playerTextdraws[playerid][moneyTD]);

        SetTimerEx("hide_money_update", 2000, false, "i", playerid);
    }
    return 1;
}

// ------------------------------------- //
//               Utils                   //
// ------------------------------------- //

delimiteMoneyFromString(&prefix_val, &suffix_val, const value[]) { 
    new const length = strlen(value);

    if(length <= 9) {
        prefix_val = 0;
        suffix_val = strval(value);
        return 0;
    }

    new copy_value[20];

    strcat(copy_value, value);
    copy_value[length - 9] = EOS;

    suffix_val = strval(value[length - 9]);
    prefix_val = strval(copy_value);
    return 1;
}

formatMoney(prefix_val, suffix_val) {
    new 
        str[64];

    if(prefix_val > 0) 
        format(str, sizeof str, "%d%09d", prefix_val, suffix_val);
    
    else 
        format(str, sizeof str, "%d", suffix_val); 

    for(new i = strlen(str) - 3; i > 0; i -= 3)  
        strins(str, ".", i); 
    
    return str;
}