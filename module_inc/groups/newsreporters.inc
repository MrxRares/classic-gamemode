// ------------------------------------- //
//             Includes                  //
// ------------------------------------- //

#include <YSI_Coding\y_hooks>

// ------------------------------------- //
//             Macros                    //
// ------------------------------------- //

#define MAX_LIVE_QUESTIONS 15
#define check_reporter if(playerInfo[playerid][pGroup] != 9) return sendError(playerid, "Nu faci parte din factiunea News Reporters.");
#define check_nr_vehicle if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER || GetPlayerState(playerid) == PLAYER_STATE_DRIVER && vehicleInfo[serverVehicleId[GetPlayerVehicleID(playerid)]][vehGroup] != GROUP_TYPE_NR) return sendError(playerid,"Nu esti intr-un vehicul de factiune pentru solicitarea acestei actiuni.");
// ------------------------------------- //
//             Enums                     //
// ------------------------------------- //

static enum e_questionInfo {
	questionPlayerID,
	questionPlayerName[MAX_PLAYER_NAME],
	questionPlaced[64]
}

// ------------------------------------- //
//             Variables                 //
// ------------------------------------- //

new 
	playerNewsDelay[MAX_PLAYERS],
	playerLiveChatDelay[MAX_PLAYERS],

	playerLiveInterview[MAX_PLAYERS],
	playerLiveInvited[MAX_PLAYERS],
	playerLiveInviteTime[MAX_PLAYERS],
	playerLiveMoney[MAX_PLAYERS],

	bool:playerTogNews[MAX_PLAYERS],
	bool:playerLiveQuestionPlaced[MAX_PLAYERS],

	serverLiveInterview = INVALID_PLAYER_ID,
	bool:serverLiveQuestionsOpen,

	Iterator:interviewersLive<MAX_PLAYERS>,

	liveQuestionsInfo[MAX_LIVE_QUESTIONS][e_questionInfo],
	selectedQuestion[MAX_PLAYERS][MAX_LIVE_QUESTIONS],
	Iterator:liveQuestions<MAX_LIVE_QUESTIONS>;

// ------------------------------------- //
//             Functions                 //
// ------------------------------------- //

setPlayerLive(const playerid, const reporterid) {
	playerLiveInterview[playerid] = reporterid;

	if(serverLiveInterview == INVALID_PLAYER_ID) {
		serverLiveInterview = reporterid;
		serverLiveQuestionsOpen = false;
	}

	playerLiveInvited[playerid] = INVALID_PLAYER_ID;
	playerLiveMoney[playerid] = 0;
	playerLiveInviteTime[playerid] = -1;

	TogglePlayerControllableEx(playerid, false);
	
	TogglePlayerControllableEx(reporterid, false);

	Iter_Add(interviewersLive, playerid);
	va_SendClientMessage(playerid, -1, "You accepted %s's live offer.", GetName(reporterid));
	va_SendClientMessage(reporterid, -1, "%s accepted your live offer.", GetName(playerid));
}

getInterviewersLiveCount() {
	return Iter_Count(interviewersLive);
}

getPlayerLiveInvite(const playerid) {
	return playerLiveInvited[playerid];
}

getPlayerLiveMoney(const playerid) {
	return playerLiveMoney[playerid];
}

isPlayerLiveOfferAvailable(const playerid) {
	return playerLiveInviteTime[playerid] > gettime();
}

getPlayerLive(const playerid) {
	return playerLiveInterview[playerid];
}

sendPlayersNews(const color, const text[], va_args<>) {
	new string[156];

	va_format(string, sizeof string, text, va_start<2>);

	foreach(new i : Player) {
		if(!playerTogNews[i])
			continue;

		SendClientMessage(i, color, string);
	}
	return true;
}

// ------------------------------------- //
//             Commands                  //
// ------------------------------------- //

YCMD:news(playerid, params[], help) {
	check_reporter

	if(playerNewsDelay[playerid] > gettime())
		return sendError(playerid, "Trebuie sa astepti %d secunde inainte de a folosi iar aceasta comanda.", playerNewsDelay[playerid] - gettime());

	check_nr_vehicle

	if(isnull(params))
		return sendSyntaxMessage(playerid, "/news <message>");

	if(FindIP(params)) {
        sendStaffMessage(STAFF_TYPE_ADMIN, COLOR_ADMCOMMANDS, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s may be advertising using command [/news]. (text: %s)", GetName(playerid), params); 
        return true;
    }

    sendPlayersNews(0xFFA500FF, "NR %s: %s", GetName(playerid), params);

    playerNewsDelay[playerid] = gettime() + 60;
    insertRaportPoint(playerid);
	return true;
}

YCMD:live(playerid, params[], help) {
	check_reporter

	if(playerInfo[playerid][pGroupRank] < 2)
		return sendError(playerid, "Ai nevoie de minim rank 2 pentru a folosi aceasta comanda.");

	if(serverLiveInterview != INVALID_PLAYER_ID && serverLiveInterview != playerid)
		return sendError(playerid, "Este deja un live inceput in momentul acesta.");

	if(Iter_Count(interviewersLive) >= 2)
		return sendError(playerid, "Nu poti invita mai mult de 2 persoane la interviu.");

	new returnPlayer, money; if(sscanf(params, "ud", returnPlayer, money))
		return sendSyntaxMessage(playerid, "/live <playerid/name> <money>");

	if(returnPlayer == playerid)
		return sendError(playerid, "Nu poti folosi aceasta comanda asupra ta.");

	if(!playerStaticVar[returnPlayer][playerLogged])
		return sendError(playerid, "Acel player nu este logat.");

	new const vehicleid = GetPlayerVehicleID(playerid);

	check_nr_vehicle

	if(vehicleid != GetPlayerVehicleID(returnPlayer))
		return sendError(playerid, "Acel player nu este in vehiculul tau.");

	if(playerInfo[returnPlayer][pLevel] < 3)
		return sendError(playerid, "Acel player nu are level 3.");

	if(playerLiveInterview[returnPlayer] == serverLiveInterview && serverLiveInterview != INVALID_PLAYER_ID)
		return sendError(playerid, "Acel player este deja in interviu.");

	if(money < 10000 || money > 100000)
		return sendError(playerid, "Suma de bani trebuie sa fie cuprinsa intre $10.000 si $100.000.");

	if(playerLiveInviteTime[returnPlayer] > gettime())
		return sendError(playerid, "Acel player are o oferta disponibila in momentul acesta.");

	playerLiveInvited[returnPlayer] = playerid;
	playerLiveMoney[returnPlayer] = money;
	playerLiveInviteTime[returnPlayer] = gettime() + 30;

	va_SendClientMessage(playerid, COLOR_LIGHTBLUE, "* You invited player %s (%d) to an interview for $%s. He have 30 seconds available to accept the live.", GetName(returnPlayer), returnPlayer, FormatNumber(money));
	
	va_SendClientMessage(returnPlayer, COLOR_LIGHTBLUE, "* Reporter %s (%d) invited you to an interview for $%s. Write [/accept %d live] to accept his interview.", GetName(playerid), playerid, FormatNumber(money), playerid);
	SendClientMessage(returnPlayer, COLOR_LIGHTBLUE, "* If you don't accept the invite in 30 seconds, it will be canceled automatically.");
	return true;
}

YCMD:endlive(playerid, params[], help) {
	if(serverLiveInterview == INVALID_PLAYER_ID)
		return true;

	if(!playerInfo[playerid][pAdmin]) {
		check_reporter

		if(serverLiveInterview != playerid)
			return sendError(playerid, "Nu ai un interviu pornit.");
	}

	if(playerid == serverLiveInterview) {
		sendPlayersNews(0xFFA500FF, "Reporter %s has ended the live.", GetName(playerid));
		insertRaportPoint(playerid);
		insertRaportPoint(playerid);
		insertRaportPoint(playerid);
	}
	else {
		sendPlayersNews(0xFFA500FF, "Admin %s has ended the live.", GetName(playerid));
	}

	TogglePlayerControllableEx(serverLiveInterview, true);

	serverLiveInterview = INVALID_PLAYER_ID;
	serverLiveQuestionsOpen = false;

	foreach(new i : interviewersLive) {
		playerLiveInterview[i] = INVALID_PLAYER_ID;

		TogglePlayerControllableEx(i, true);

		SendClientMessage(i, COLOR_YELLOW, "* The live has been ended.");
	}

	foreach(new i : liveQuestions) 
		playerLiveQuestionPlaced[liveQuestionsInfo[i][questionPlayerID]] = false;

	Iter_Clear(interviewersLive);
	Iter_Clear(liveQuestions);
	return true;
}

YCMD:lv(playerid, params[], help) {
	if(serverLiveInterview != playerid && playerLiveInterview[playerid] == INVALID_PLAYER_ID)
		return sendError(playerid, "Nu faci parte dintr-un interviu.");

	if(playerLiveChatDelay[playerid] > gettime())
		return sendError(playerid, "Trebuie sa astepti %d secunde inainte de a face asta din nou.", playerLiveChatDelay[playerid] - gettime());

	if(isnull(params))
		return sendSyntaxMessage(playerid, "/lv <message>");

	playerLiveChatDelay[playerid] = gettime() + 3;

	return sendPlayersNews(0x9ACD32FF, "%s %s: %s", serverLiveInterview == playerid ? "NR" : "LivePlayer", GetName(playerid), params);
}

YCMD:startq(playerid, params[], help) {
	if(serverLiveInterview != playerid || serverLiveQuestionsOpen)
		return true;

	serverLiveQuestionsOpen = true;

	return va_SendClientMessageToAll(0x9ACD32FF, "Reporter %s has started the questions session for this live. Use [/question] to place a question.", GetName(playerid));
}

YCMD:stopq(playerid, params[], help) {
	if(serverLiveInterview != playerid || !serverLiveQuestionsOpen)
		return true;

	serverLiveQuestionsOpen = false;

	return sendPlayersNews(0x9ACD32FF, "Reporter %s has stopped the questions session.", GetName(playerid));
}

YCMD:tognews(playerid, params[], help) {
	if(!playerStaticVar[playerid][playerLogged])
		return true;

    playerTogNews[playerid] = !playerTogNews[playerid];

    return va_SendClientMessage(playerid, COLOR_YELLOW, "News tog: %s.", playerTogNews[playerid] ? "enabled" : "disabled");
}

YCMD:question(playerid, params[], help) {
	if(!serverLiveQuestionsOpen)
		return true;

	if(playerLiveQuestionPlaced[playerid])
		return sendError(playerid, "Ai plasat deja o intrebare.");

	if(Iter_Count(liveQuestions) >= MAX_LIVE_QUESTIONS)
		return sendError(playerid, "Nu poti plasa o intrebare deoarece limita maxima a fost atinsa.");

	if(serverLiveInterview == playerid || playerLiveInterview[playerid] == serverLiveInterview)
		return sendError(playerid, "Nu iti poti trimite singur intrebari pe live.");

	if(isnull(params))
		return sendSyntaxMessage(playerid, "/question <text>");

	new const length = strlen(params);

	if(length < 3 || length > 63)
		return sendError(playerid, "Intrebarea trebuie sa fie cuprinsa intre 3 si 64 de caractere.");

	playerLiveQuestionPlaced[playerid] = true;

	new const i = Iter_Free(liveQuestions);

	Iter_Add(liveQuestions, i);

	liveQuestionsInfo[i][questionPlayerID] = playerid;

	strcpy(liveQuestionsInfo[i][questionPlayerName], GetName(playerid), MAX_PLAYER_NAME);
	strcpy(liveQuestionsInfo[i][questionPlaced], params);

	//insertPlayerChatLog(playerInfo[playerid][pSQLID], params, "/question");

	SendClientMessage(playerid, COLOR_YELLOW, "* You placed a question successfully for this interview.");
	va_SendClientMessage(serverLiveInterview, 0x9ACD32FF, "* Question placed by %s, use [/livequestions].", GetName(playerid));
	return true;
}

YCMD:livequestions(playerid, params[], help) {
	if(serverLiveInterview != playerid)
		return true;

	if(!Iter_Count(liveQuestions))
		return sendError(playerid, "Nu au fost gasite intrebari.");

	new index;

	gDialog = "No.\tPlayer\n";

	foreach(new i : liveQuestions) {
		format(gDialog, sizeof gDialog, "%s%d.\t%s\n", gDialog, index + 1, liveQuestionsInfo[i][questionPlayerName]);

		selectedQuestion[playerid][index++] = i;
	}
	
	
	format(returnString, sizeof returnString, "SERVER: Live Questions (%d available)", Iter_Count(liveQuestions));
	return Dialog_Show(playerid, SHOWLIVEQUESTIONS, DIALOG_STYLE_TABLIST_HEADERS, returnString, gDialog, "Select", "Exit");
}

YCMD:kicklive(playerid, params[], help) {
	if(serverLiveInterview == INVALID_PLAYER_ID || serverLiveInterview != playerid && !playerInfo[playerid][pAdmin])
		return true;

	new returnPlayer; if(sscanf(params, "u", returnPlayer) || returnPlayer == playerid)
		return sendSyntaxMessage(playerid, "/kicklive <playerid/name>");

	if(!playerStaticVar[returnPlayer][playerLogged])
		return sendError(playerid, "Acel player nu este logat.");

	if(playerLiveInterview[returnPlayer] != serverLiveInterview)
		return sendError(playerid, "Acel player nu face parte din interviu.");

	TogglePlayerControllableEx(returnPlayer, true);

	Iter_Remove(interviewersLive, returnPlayer);

	playerLiveInterview[returnPlayer] = INVALID_PLAYER_ID;

	va_SendClientMessage(playerid, COLOR_YELLOW, "* You kicked %s sucessfully from this live.", GetName(returnPlayer));
	va_SendClientMessage(returnPlayer, COLOR_YELLOW, "* %s %s kicked you from this live.", serverLiveInterview == playerid ? "Reporter" : "Admin", GetName(playerid));
	return true;
}

// ------------------------------------- //
//             Dialogs                   //
// ------------------------------------- //

Dialog:SHOWLIVEQUESTIONS(playerid, response, listitem, inputtext[]) {
	if(!response || serverLiveInterview != playerid || !serverLiveQuestionsOpen)
		return true;

	SetPVarInt(playerid, "selectedLiveQuestionTemp", listitem);

	
	format(returnString, sizeof returnString, "SERVER: %s's question", liveQuestionsInfo[selectedQuestion[playerid][listitem]][questionPlayerName]);
	return Dialog_Show(playerid, MANAGELIVEQUESTIONS, DIALOG_STYLE_MSGBOX, returnString, "Question placed: %s\n\nSelect below the button 'Show' if you want to show the question to the public, or 'Remove' if you want to remove it.", "Show", "Remove", liveQuestionsInfo[selectedQuestion[playerid][listitem]][questionPlaced]);
}

Dialog:MANAGELIVEQUESTIONS(playerid, response, listitem, inputtext[]) {
	if(serverLiveInterview != playerid || !serverLiveQuestionsOpen)
		return DeletePVar(playerid, "selectedLiveQuestionTemp");

	new const id = selectedQuestion[playerid][GetPVarInt(playerid, "selectedLiveQuestionTemp")];

	DeletePVar(playerid, "selectedLiveQuestionTemp");

	Iter_Remove(liveQuestions, id);

	playerLiveQuestionPlaced[liveQuestionsInfo[id][questionPlayerID]] = false;

	if(!response) 
		return va_SendClientMessage(playerid, COLOR_YELLOW, "* You removed player %s's question successfully.", liveQuestionsInfo[id][questionPlayerName]);

	va_SendClientMessage(playerid, COLOR_YELLOW, "* You showed player %s's question successfully.", liveQuestionsInfo[id][questionPlayerName]);

	return sendPlayersNews(0x9ACD32FF, "Question by %s: %s", liveQuestionsInfo[id][questionPlayerName], liveQuestionsInfo[id][questionPlaced]);
}

// ------------------------------------- //
//             Hooks                     //
// ------------------------------------- //

hook OnPlayerConnect(playerid) {
	playerNewsDelay[playerid] = playerLiveInviteTime[playerid] = -1;
	playerLiveInterview[playerid] = playerLiveInvited[playerid] = INVALID_PLAYER_ID;
	playerLiveMoney[playerid] = 0;
	playerLiveQuestionPlaced[playerid] = false;
	playerTogNews[playerid] = true;
	return true;
}

hook OnPlayerDisconnect(playerid, reason) {
	if(serverLiveInterview != INVALID_PLAYER_ID) {
		if(playerInfo[playerid][pGroup] == GROUP_TYPE_NR && serverLiveInterview == playerid) {
			foreach(new i : interviewersLive) {
				va_SendClientMessage(i, COLOR_YELLOW, "* Live has been ended because reporter %s has been disconnected.", GetName(playerid));

				playerLiveInterview[i] = INVALID_PLAYER_ID;

				TogglePlayerControllableEx(i, true);
			}

			foreach(new i : liveQuestions) 
				playerLiveQuestionPlaced[liveQuestionsInfo[i][questionPlayerID]] = false;
		
			Iter_Clear(interviewersLive);
			Iter_Clear(liveQuestions);

			serverLiveInterview = INVALID_PLAYER_ID;

			sendPlayersNews(0x9ACD32FF, "Live has been ended because reporter %s has been disconnected.", GetName(playerid));
		}
		else if(playerLiveInterview[playerid] == serverLiveInterview) {
			Iter_Remove(interviewersLive, playerid);

			va_SendClientMessage(serverLiveInterview, COLOR_YELLOW, "* Player %s has been disconnected from server and was removed from interview.", GetName(playerid));
		}
	}
	return true;
}