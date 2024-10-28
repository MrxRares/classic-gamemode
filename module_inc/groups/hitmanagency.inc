// ------------------------------------- //
//             Includes                  //
// ------------------------------------- //

#include <YSI_Coding\y_hooks>

// ------------------------------------- //
//             Variables                 //
// ------------------------------------- //

static
    playerHaveContractAssigned[MAX_PLAYERS],
    playerHaveTarget[MAX_PLAYERS];

// ------------------------------------- //
//             Functions                 //
// ------------------------------------- //

setPlayerUndercover(const playerid, const bool:undercover = false) {
	playerUndercover[playerid] = undercover;

	if(isPlayerUndercover(playerid)) {
		PlayerTextDrawShow(playerid, playerTextdraws[playerid][undercoverTD]);
		SendClientMessage(playerid, COLOR_YELLOW, "You are now undercover. No one can see your name.");
    	foreach(new i : streamedPlayers[playerid]) {
	    	ShowPlayerNameTagForPlayer(i, playerid, 0);
	    }
	}
	else {
		SendClientMessage(playerid, COLOR_YELLOW, "You are not undercover anymore. Anyone can see your name.");

    	PlayerTextDrawHide(playerid, playerTextdraws[playerid][undercoverTD]);

		foreach(new i : streamedPlayers[playerid]) {
			ShowPlayerNameTagForPlayer(i, playerid, 1);
   		}
	}
	return 1;
}

isPlayerUndercover(const playerid) {
	return playerUndercover[playerid];
} 

cancelPlayerHit(const playerid) {
	new returnPlayer = playerHaveContractAssigned[playerid];

	playerHaveContractAssigned[playerid] = playerHaveTarget[returnPlayer] = INVALID_PLAYER_ID;
}

getPlayerHit(const playerid) {
	return playerHaveContractAssigned[playerid];
}

getPlayerTarget(const playerid) {
	return playerHaveTarget[playerid];
}

// ------------------------------------- //
//            Commands                   //
// ------------------------------------- //

YCMD:undercover(playerid, params[], help) {
	if(playerInfo[playerid][pGroup] != 11)
		return sendError(playerid, "Nu faci parte din factiunea Hitman Agency.");

	if(getPlayerHit(playerid) == INVALID_PLAYER_ID && !playerUndercover[playerid])
		return sendError(playerid, "Nu poti folosi aceasta comanda daca nu ai un contract.");

    return setPlayerUndercover(playerid, !playerUndercover[playerid]);
}

YCMD:gethit(playerid, params[], help) {
	if(playerInfo[playerid][pGroup] != 11)
		return sendError(playerid, "Nu faci parte din factiunea Hitman Agency.");

	if(!Iter_Count(serverContracts))
		return sendError(playerid, "No contracts available.");

	if(playerHaveContractAssigned[playerid] != INVALID_PLAYER_ID)
		return sendError(playerid, "Ai deja un contract.");

	if(playerStaticVar[playerid][var_contract_delay] > gettime())
		return sendError(playerid, "Trebuie sa astepti %s pentru a putea lua un alt contract.", calculateTime(playerStaticVar[playerid][var_contract_delay] - gettime()));

	if(playerInfo[playerid][pWantedLevel])
		return sendError(playerid, "Nu poti folosi aceasta comanda deoarece ai wanted.");

	new const returnPlayer = Iter_Random(serverContracts);

    Iter_Remove(serverContracts, returnPlayer);

	playerHaveContractAssigned[playerid] = returnPlayer;
	playerHaveTarget[returnPlayer] = playerid;

	set_find_checkpoint(playerid, returnPlayer);

	SendClientMessage(playerid, COLOR_YELLOW, "You have a new contract (/mycontract).");

	playerStaticVar[playerid][var_contract_delay] = gettime() + 300;

	return sendGroupMessage(GROUP_TYPE_HITMAN, COLOR_TEAL, "%s received a contract. Target: %s.", GetName(playerid), GetName(returnPlayer));
}

YCMD:cancelhit(playerid, params[], help) {
	if(playerInfo[playerid][pGroup] != 11)
		return sendError(playerid, "Nu faci parte din factiunea Hitman Agency.");

    if(getPlayerHit(playerid) == INVALID_PLAYER_ID)
		return sendError(playerid, "Nu ai un contract activ.");

	new returnPlayer = getPlayerHit(playerid);

	cancelPlayerHit(playerid);
	Iter_Add(serverContracts, returnPlayer);

	if(playerStaticVar[playerid][targetFind] == returnPlayer)
		cancel_find(playerid);

	return sendGroupMessage(GROUP_TYPE_HITMAN, COLOR_TEAL, "[AGENCY NEWS] %s used /cancelhit, new target: %s, price: unknown.", GetName(playerid), GetName(returnPlayer));
}

YCMD:mycontract(playerid, params[], help) {
	if(playerInfo[playerid][pGroup] != 11)
		return sendError(playerid, "Nu faci parte din factiunea Hitman Agency.");

    if(getPlayerHit(playerid) == INVALID_PLAYER_ID)
		return sendError(playerid, "Nu ai un contract activ.");

	return va_SendClientMessage(playerid, COLOR_YELLOW, "You have a contract on %s(%d).", GetName(playerHaveContractAssigned[playerid]), playerHaveContractAssigned[playerid]);
}

YCMD:contracts(playerid, params[], help) {
	if(playerInfo[playerid][pGroup] != 11)
		return sendError(playerid, "Nu faci parte din factiunea Hitman Agency.");

	if(!Iter_Count(serverContracts))
		return sendError(playerid, "No contracts available.");

	new count;

	foreach(new i : serverContracts) {
		if(playerStaticVar[i][afkTime] < 10)
			continue;

		count ++;
	}

    SendClientMessage(playerid, COLOR_TEAL, "[Contracts]");

    va_SendClientMessage(playerid, COLOR_WHITE, "Targets AFK/Sleeping: %d", count);
    va_SendClientMessage(playerid, COLOR_WHITE, "Total targets: %d", Iter_Count(serverContracts));

    SendClientMessage(playerid, COLOR_TEAL, "-------------------");
    return 1;
}

YCMD:contract(playerid, params[], help) {
	if(playerInfo[playerid][pGroup] == 11)
		return sendError(playerid,"You can't place a contract because you are a hitman.");

	if(playerStaticVar[playerid][var_placecontract_delay] > gettime())
		return sendError(playerid, "Trebuie sa astepti %s minute inainte de a folosi aceasta comanda.", calculateTime(playerStaticVar[playerid][var_placecontract_delay] - gettime()));

	new returnPlayer, money; if(sscanf(params, "ud", returnPlayer, money))
	    return sendSyntaxMessage(playerid, "/contract <playerid/name> <money>");

	if(!playerStaticVar[returnPlayer][playerLogged])
	    return sendError(playerid, "Acest player nu este logat.");

	if(money < 10000 || money > 500000) 
		return sendError(playerid, "Poti plasa intre $10.000 si $500.000.");

	if(playerInfo[returnPlayer][pGroup] == 11)
	    return sendError(playerid, "Cannot place contracts on hitmans.");

	if(playerInfo[returnPlayer][pAdmin])
	    return sendError(playerid, "Cannot place contracts on Admin.");

	if(playerInfo[returnPlayer][pHelper])
	    return sendError(playerid, "Cannot place contracts on Helper.");

	if(getPlayerFactionWar(returnPlayer) != -1)
		return sendError(playerid, "Nu poti pune contract pe acel player, deoarece se afla in war.");

	if(playerStaticVar[returnPlayer][afkTime] > 300 || playerStaticVar[returnPlayer][playerSleeping])
		return sendError(playerid, "Nu poti folosi aceasta comanda pe acel player deoarece este AFK.");

	if(returnPlayer == playerid)
	    return sendError(playerid, "You cannot contract yourself.");

	if(GetMoney(playerid) < money && GetMilliards(playerid) < 1)
	   return sendError(playerid, "Invalid transaction amount.");

	if(!playerStaticVar[returnPlayer][var_contract_price]) {
		Iter_Add(serverContracts, returnPlayer);

		sendGroupMessage(GROUP_TYPE_HITMAN, COLOR_CYAN, "New hit available! Target: %s. Price: unknown.", GetName(returnPlayer));
	}

    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);

    playerStaticVar[returnPlayer][var_contract_price] += money;

    playerStaticVar[playerid][var_placecontract_delay] = gettime() + 180;
	
	substractMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, money);

	return SendClientMessage(playerid, COLOR_GREY, "Contract placed! The hitmans will kill your target soon!");
}

// ------------------------------------- //
//            Hooks                      //
// ------------------------------------- //

hook OnPlayerConnect(playerid) {
	playerUndercover[playerid] = false;
	playerHaveContractAssigned[playerid] = playerHaveTarget[playerid] = INVALID_PLAYER_ID;

	playerTextdraws[playerid][undercoverTD] = CreatePlayerTextDraw(playerid, 250.000000, 374.000000, "~r~you are undercover");
	PlayerTextDrawBackgroundColor(playerid, playerTextdraws[playerid][undercoverTD], 255);
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][undercoverTD], 3);
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][undercoverTD], 0.549999, 1.500000);
	PlayerTextDrawColor(playerid, playerTextdraws[playerid][undercoverTD], -1);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][undercoverTD], 0);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][undercoverTD], 1);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][undercoverTD], 1);
 	return 1;
}

hook OnPlayerStreamIn(playerid, forplayerid) {
	if(isPlayerUndercover(playerid))
		ShowPlayerNameTagForPlayer(forplayerid, playerid, 0);

	return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
	if(playerStaticVar[playerid][var_contract_price])
        Iter_Remove(serverContracts, playerid);

	if(getPlayerHit(playerid) != INVALID_PLAYER_ID) {
		Iter_Add(serverContracts, playerHaveContractAssigned[playerid]);

		sendGroupMessage(GROUP_TYPE_HITMAN, COLOR_CYAN, "[AGENCY NEWS] New contract available %s(%d) because %s has been disconnected.", GetName(playerHaveContractAssigned[playerid]), playerHaveContractAssigned[playerid], GetName(playerid));
	
		playerHaveTarget[playerHaveContractAssigned[playerid]] = playerHaveContractAssigned[playerid] = INVALID_PLAYER_ID;
	}
	
	if(playerHaveTarget[playerid] != INVALID_PLAYER_ID) {
		va_SendClientMessage(playerHaveTarget[playerid], COLOR_YELLOW, "Your target has been disconnected.");
		
		playerHaveContractAssigned[playerHaveTarget[playerid]] = playerHaveTarget[playerid] = INVALID_PLAYER_ID;
	}
	return 1;
}

hook OnPlayerSpawn(playerid) {
	if(isPlayerUndercover(playerid) && player_data[playerid][playerDeath])
		setPlayerUndercover(playerid, false);

	return 1;
}

// ------------------------------------- //
//             Callbacks                 //
// ------------------------------------- //

function:OnHitmanKillContract(playerid, killerid, reason) {
	if(killerid != INVALID_PLAYER_ID && getPlayerHit(killerid) == playerid && playerInfo[killerid][pGroup] == 11) {
		new const contractMoney = playerStaticVar[playerid][var_contract_price];

		cancelPlayerHit(killerid);

		playerStaticVar[playerid][var_contract_price] = 0;

		playerStaticVar[killerid][var_contract_delay] = gettime() + 300;

		if(playerStaticVar[killerid][usedFind] == playerid)
			cancel_find(killerid);

        if(!isPlayerUndercover(killerid))
        	return sendGroupMessage(playerInfo[killerid][pGroup], COLOR_TEAL, "%s failed to complete the contract on %s for $%s, distance: %.0fm. Fail reason: not undercover.", GetName(killerid), GetName(playerid), FormatNumber(contractMoney), GetDistanceBetweenPlayers(killerid, playerid));
	
        if(playerStaticVar[killerid][b_PHONE_TURN] && playerInfo[killerid][pPhoneNumber])
        	return sendGroupMessage(playerInfo[killerid][pGroup], COLOR_TEAL, "%s failed to complete the contract on %s for $%s, distance: %.0fm. Fail reason: phone turned on.", GetName(killerid), GetName(playerid), FormatNumber(contractMoney), GetDistanceBetweenPlayers(killerid, playerid));
	
        if(GetDistanceBetweenPlayers(killerid, playerid) < 100.0 && reason == WEAPON_KNIFE)
        	return sendGroupMessage(playerInfo[killerid][pGroup], COLOR_TEAL, "%s failed to complete the contract on %s for $%s, distance: %.0fm. Fail reason: distance/weapon.", GetName(killerid), GetName(playerid), FormatNumber(contractMoney), GetDistanceBetweenPlayers(killerid, playerid));

        sendGroupMessage(playerInfo[killerid][pGroup], COLOR_TEAL, "%s has succesfully completed the contract on %s for $%s, distance: %.0fm.", GetName(killerid), GetName(playerid), FormatNumber(contractMoney), GetDistanceBetweenPlayers(killerid, playerid));
        
		addMoney(killerid, GetMilliards(killerid), GetMoney(killerid), 0, contractMoney, false);

		
		insertRaportPoint(killerid);
	}
	return 1;
}
