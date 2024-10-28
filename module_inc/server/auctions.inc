// ------------------------------------- //
//             Includes                  //
// ------------------------------------- //

#include <YSI_Coding\y_hooks>

// ------------------------------------- //
//             Constants                 //
// ------------------------------------- //

#define BID_TYPE_HOUSE 0
#define BID_TYPE_BUSINESS 1
#define BID_TYPE_APARTMENT 2

// ------------------------------------- //
//             Enums                     //
// ------------------------------------- //

enum ENUM_BID_INFORMATIONS
{
	bidStatus, bidID, bidTime, bidType, bidPropriety, bidAttempt,
	bidSQL, bidPlayer[MAX_PLAYER_NAME], bidPlayerGameId, bidMoney
}

// ------------------------------------- //
//             Variables                 //
// ------------------------------------- //

new bidInfo[ENUM_BID_INFORMATIONS], Timer:bid_timer, oldBidder;
static const resetBid[ENUM_BID_INFORMATIONS];

// ------------------------------------- //
//             Functions                 //
// ------------------------------------- //

deleteProperty(propertyID) {
	mysql_format(SQL, returnString, sizeof returnString, "delete from `auctions` where `ProprietyID` = '%d';", propertyID);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}
StartNewAuction()
{
	oldBidder = -1;
	bidInfo = resetBid;
	mysql_tquery(SQL, "select * from `auctions` where `Expected` = '0';", "WhenServerStartAuctions", "");
}

function:AddToAuction(proprietyID, proprietyType)
{

    if(proprietyType < 0 || proprietyType > 2)
    {
        sendStaffMessage(STAFF_TYPE_OWNER, COLOR_SERVER, "Invalid proprietyType. Must be 0 (house), 1 (business), or 2 (apartment).");
        return 0;
    }

    mysql_format(SQL, returnString, sizeof returnString, "insert into `auctions` (`ProprietyID`, `ProprietyType`, `Expected`) values ('%d', '%d', '0');", proprietyID, proprietyType);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}
function:WhenServerStartAuctions()
{
	if(!cache_num_rows())
		return true;

	cache_get_value_name_int(0, "ID", bidInfo[bidID]);
	cache_get_value_name_int(0, "ProprietyID", bidInfo[bidPropriety]);
	cache_get_value_name_int(0, "ProprietyType", bidInfo[bidType]); 

	new auctionString[144];
	switch(bidInfo[bidType])
	{ 
		case BID_TYPE_HOUSE: format(auctionString, sizeof auctionString, "(Licitatii):{ffffff} Casa cu id %d a fost scoasa la licitatie, licitatia nu va porni pana nu este facuta o oferta!", bidInfo[bidPropriety]);
		case BID_TYPE_BUSINESS: format(auctionString, sizeof auctionString, "(Licitatii):{ffffff} Afacerea cu id %d a fost scoasa la licitatie, licitatia nu va porni pana nu este facuta o oferta!", bidInfo[bidPropriety]);
		case BID_TYPE_APARTMENT: format(auctionString, sizeof auctionString, "(Licitatii):{ffffff} Apartamentul cu id %d aflat la etajul %d a fost scos la licitatie.", bidInfo[bidPropriety], GetApFloorLevel(bidInfo[bidPropriety]));
	}
	SendClientMessageToAll(COLOR_SERVER, auctionString);
	SendClientMessageToAll(COLOR_SERVER, "(Licitatii):{ffffff} Daca nu va licita nimeni in 60 secunde aceasta licitatie va fi amanata!");

	if(bid_timer == Timer:0)
	{
		bid_timer = repeat auctionTimer();
	}

	return bidInfo[bidStatus] = 1;
}

auctionProprietyName(auctionType)
{
	new aucString[16];
	switch(auctionType)
	{
		case BID_TYPE_HOUSE: aucString = "casa";
		case BID_TYPE_BUSINESS: aucString = "afacerea";
		case BID_TYPE_APARTMENT: aucString = "apartamentul";
	}
	return aucString;
}

// ------------------------------------- //
//             Commands                  //
// ------------------------------------- //

YCMD:addbiztobid(playerid, params[], help) {
	check_owner

	if(bid_timer != Timer:0)
		return sendError(playerid, "Esti idiot ? Asteapta sa se termine..");

	new returnBizz;
    if(sscanf(params, "d", returnBizz))
        return sendSyntaxMessage(playerid, "/addbiztobid [business id]");

    AddToAuction(returnBizz, BID_TYPE_BUSINESS);
	return true;
}

YCMD:addhousetobid(playerid, params[], help) {
	check_owner

	if(bid_timer != Timer:0)
		return sendError(playerid, "Esti idiot ? Asteapta sa se termine..");

	new returnHouse;
    if(sscanf(params, "d", returnHouse))
        return sendSyntaxMessage(playerid, "/addhousetobid [house id]");

    AddToAuction(returnHouse, BID_TYPE_HOUSE);
	return true;
}

YCMD:addapartamenttobid(playerid, params[], help) {
	check_owner

	if(bid_timer != Timer:0)
		return sendError(playerid, "Esti idiot ? Asteapta sa se termine..");

	new returnApartament;
    if(sscanf(params, "d", returnApartament))
        return sendSyntaxMessage(playerid, "/addapartamenttobid [apartament id]");

    AddToAuction(returnApartament, BID_TYPE_APARTMENT);

	return true;
}

YCMD:stopauctions(playerid, params[], help) {
	check_owner
	stop bid_timer;
	bid_timer = Timer:0;
	SendClientMessageToAll(-1, "Licitatiile au fost oprite, multumim tuturor!");
	return true;
}

YCMD:forcestartauctions(playerid, params[], help)
{
	check_owner

	StartNewAuction();

	return true;
}

YCMD:auction(playerid, params[], help)
{
	if(!bidInfo[bidStatus])
		return sendError(playerid,"Se pare ca nu este nicio licitatie in desfasurare!");

	new auctionString[144], auctionPropriety = bidInfo[bidPropriety], auctionType = bidInfo[bidType];

	va_SendClientMessage(playerid, -1, "Proprietate licitata: %s %d", auctionProprietyName(auctionType), auctionPropriety);
	switch(auctionType)
	{
		case BID_TYPE_HOUSE: format(auctionString, sizeof auctionString, "Pentru a afla locatia acesteia tasteaza: /findhouse %d", auctionPropriety);
		case BID_TYPE_BUSINESS: format(auctionString, sizeof auctionString, "Pentru a afla locatia acesteia tasteaza: /findbusiness %d", auctionPropriety);
		case BID_TYPE_APARTMENT: format(auctionString, sizeof auctionString, "Apartamentul este situat la etajul %d.", GetApFloorLevel(auctionPropriety));
	}
	SendClientMessage(playerid, -1, auctionString);

	sendSplitMessage(playerid, -1, "Cel mai mult a licitat %s($%s.000.000). Daca altcineva nu liciteaza mai mult, %s va castiga proprietatea in %s.", bidInfo[bidPlayer], FormatNumber(bidInfo[bidMoney]), bidInfo[bidPlayer], returnTimeSeconds(60 - bidInfo[bidTime]));
	return true;
}

YCMD:bid(playerid, params[], help)
{
    if(playerInfo[playerid][pHouse] && bidInfo[bidType] == BID_TYPE_HOUSE)
		return sendError(playerid,"Nu poti licita pentru o casa daca deja detii una!");

    if(playerInfo[playerid][pBusiness] && bidInfo[bidType] == BID_TYPE_BUSINESS)
		return sendError(playerid,"Nu poti licita pentru o afacere daca deja detii una!");

    if(playerInfo[playerid][pApartament] && bidInfo[bidType] == BID_TYPE_APARTMENT)
		return sendError(playerid,"Nu poti licita pentru un apartament daca deja detii unul!");

	new bidMoneySuffix, bidMoneyPrefix;

	if(!bidInfo[bidPropriety])
		return sendError(playerid,"Nu este nicio licitatie in desfasurare. Urmatoarea licitatie va incepe in 00:00 daca mai sunt bunuri de licitat.");

	new returnMoney;
	if(sscanf(params, "i", returnMoney))
		return sendError(playerid,"Foloseste: /bid <milioane>");

	if(returnMoney < 2)
		return SendClientMessage(playerid, COLOR_GREY, "Suma minima ce poate fi licitata este de $2.000.000.");


	if(returnMoney < bidInfo[bidMoney]+1)
		return va_SendClientMessage(playerid, -1, "Trebuie sa licitezi mai mult de $%s.000.000. Tu ai licitat $%s.000.000.", FormatNumber(bidInfo[bidMoney]), FormatNumber(returnMoney));

	new
		moneyString[32];

	format(moneyString, sizeof moneyString, "%d000000", returnMoney);
    CreateLocalLY(bidMoneyPrefix, bidMoneySuffix, moneyString);

    if(playerInfo[playerid][pAccountLY] < bidMoneyPrefix || playerInfo[playerid][pAccount] < bidMoneySuffix && !playerInfo[playerid][pAccountLY])
        return sendError(playerid,"You don't have enough money.");

    if(bidInfo[bidSQL] != -1)
    {		
		format(moneyString, sizeof moneyString, "%d000000", bidInfo[bidMoney]);
	    CreateLocalLY(bidMoneyPrefix, bidMoneySuffix, moneyString);

	    if(strmatch(bidInfo[bidPlayer], GetName(bidInfo[bidPlayerGameId])) && bidInfo[bidPlayerGameId] != INVALID_PLAYER_ID && playerStaticVar[bidInfo[bidPlayerGameId]][playerLogged])
	    {
	    	oldBidder = bidInfo[bidPlayerGameId];

	    	SendClientMessage(oldBidder, -1, "Cineva a licitat mai mult ca tine, ti-ai primit banii inapoi.");

		    playerInfo[oldBidder][pAccountLY] += bidMoneyPrefix;
		    playerInfo[oldBidder][pAccount] += bidMoneySuffix;

	    	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `BankLY` = '%d' and `Bank` = '%d' where `id` = '%d';", playerInfo[oldBidder][pAccountLY], playerInfo[oldBidder][pAccount], playerInfo[oldBidder][pSQLID]);
	    	mysql_tquery(SQL, returnString, "", "");
	    }
	    else
	    {
			insertPlayerMail(bidInfo[bidSQL], gettime(), "Licitatii: Suma de $%s.000.000 ti-a fost returnata inapoi deoarece cineva a licitat mai mult decat tine.", FormatNumber(bidInfo[bidMoney]));

			mysql_format(SQL, returnString, sizeof returnString, "UPDATE `users` SET `BankLY` = `BankLY` + %d, `Bank` = `Bank` + %d WHERE `id` = %d;", bidMoneyPrefix, bidMoneySuffix, bidInfo[bidSQL]);
			mysql_tquery(SQL, returnString, "", "");
	    }
    }

	format(moneyString, sizeof moneyString, "%d000000", returnMoney);
    substractBankMoney(playerid, playerInfo[playerid][pAccountLY], playerInfo[playerid][pAccount], 0, returnMoney, false);

	va_SendClientMessageToAll(COLOR_SERVER, "(Licitatii):{ffffff} Jucatorul %s a licitat suma de $%s.000.000 si conduce licitatia!", GetDBName(playerid), FormatNumber(returnMoney));

	format(bidInfo[bidPlayer], MAX_PLAYER_NAME, GetDBName(playerid));
	bidInfo[bidSQL] = playerInfo[playerid][pSQLID];
	bidInfo[bidPlayerGameId] = playerid;

	bidInfo[bidMoney] = returnMoney;

	if(bid_timer == Timer:0)
	{
		bid_timer = repeat auctionTimer();
	}
	return true;
}

// ------------------------------------- //
//             Hooks                     //
// ------------------------------------- //

hook OnPlayerDisconnect(playerid, reason)
{
	oldBidder = playerInfo[playerid][pSQLID];
    return true;
}

// ------------------------------------- //
//             Timers                    //
// ------------------------------------- //

timer auctionTimer[1000]()
{
	bidInfo[bidTime] ++;

	if(bidInfo[bidTime] == 30 && bidInfo[bidAttempt])
		va_SendClientMessageToAll(COLOR_SERVER, "(Licitatii):{ffffff} Daca in 30 secunde nu liciteaza altcineva %s va castiga %s %d cu $%s.000.000.", bidInfo[bidPlayer], auctionProprietyName(bidInfo[bidType]), bidInfo[bidPropriety], FormatNumber(bidInfo[bidMoney]));

	if(bidInfo[bidTime] == 60)
	{
		if(!bidInfo[bidSQL])
		{

			if(bidInfo[bidPropriety] == 0) {
				stop bid_timer;
				bid_timer = Timer:0;
				bidInfo = resetBid;
				oldBidder = -1;
				return SendClientMessageToAll(COLOR_SERVER, "(Licitatii):{ffffff} Deoarece nu mai sunt proprietatii pentru licitatie, aceasta a fost oprita!");
			}


			va_SendClientMessageToAll(COLOR_SERVER, "(Licitatii):{ffffff} Licitatia pentru %s %d s-a inchis deoarece nu a licitat nimeni! O alta licitatie va avea loc in curand!", auctionProprietyName(bidInfo[bidType]), bidInfo[bidPropriety]);
			
			mysql_format(SQL, returnString, sizeof returnString, "update `auctions` set `Expected` = '1' where `ID` = '%d';", bidInfo[bidID]);
			mysql_tquery(SQL, returnString, "", "");

			bidInfo = resetBid;
			oldBidder = -1;
			return true;
		}
		if(!bidInfo[bidAttempt])
		{
			bidInfo[bidTime] = 0;
			bidInfo[bidAttempt] = 1;

			va_SendClientMessageToAll(COLOR_SERVER, "(Licitatii):{ffffff} Daca in 60 secunde nu liciteaza altcineva %s va castiga %s %d cu $%s.000.000.", bidInfo[bidPlayer], auctionProprietyName(bidInfo[bidType]), bidInfo[bidPropriety], FormatNumber(bidInfo[bidMoney]));
		}
		else
		{
			va_SendClientMessageToAll(COLOR_SERVER, "(Licitatii):{ffffff} Jucatorul %s a castigat licitatia cu $%s.", bidInfo[bidPlayer], FormatNumber(bidInfo[bidMoney] * 1000000));
			new winner = GetSQLByName(bidInfo[bidPlayer]);
			new playerBidded = bidInfo[bidPlayerGameId];
			switch(bidInfo[bidType])
			{
				case BID_TYPE_HOUSE: {
					new const house = bidInfo[bidPropriety];

					if(!IsPlayerConnected(playerBidded)) {
						insertPlayerMail(bidInfo[bidSQL], gettime(), "Ai castigat licitatia, casa cu id-ul %d ti-a fost transferata", house);


			            houseInfo[house][houseOwner] = winner;

			            format(houseInfo[house][houseOwnerName], MAX_PLAYER_NAME, "%s", bidInfo[bidPlayer]);

			            mysql_format(SQL, returnString, sizeof returnString, "update `houses` set `OwnerSQL` = '%d' where `id` = '%d';", winner, house);
			            mysql_tquery(SQL, returnString, "", "");
			        
			            mysql_format(SQL, returnString, sizeof returnString, "update `users` set `House` = '%d' where `id` = '%d';", house, winner);
			            mysql_tquery(SQL, returnString, "", "");

			            WhenHouseGotUpdated(house);
						deleteProperty(house);
					}
					else {
			            houseInfo[house][houseOwner] = winner;

			            format(houseInfo[house][houseOwnerName], MAX_PLAYER_NAME, "%s", bidInfo[bidPlayer]);

			            mysql_format(SQL, returnString, sizeof returnString, "update `houses` set `OwnerSQL` = '%d' where `id` = '%d';", winner, house);
			            mysql_tquery(SQL, returnString, "", "");
			        
			            mysql_format(SQL, returnString, sizeof returnString, "update `users` set `House` = '%d' where `id` = '%d';", house, winner);
			            mysql_tquery(SQL, returnString, "", "");

			            WhenHouseGotUpdated(house);

						playerInfo[playerBidded][pHouse] = house;
						SendClientMessage(playerBidded, -1, "Ai castigat licitatia, casa cu id-ul %d ti-a fost transferata", house);
						deleteProperty(house);
					}
				}
				case BID_TYPE_BUSINESS: {
					new const biz = bidInfo[bidPropriety];

					if(!IsPlayerConnected(playerBidded)) {
						insertPlayerMail(bidInfo[bidSQL], gettime(), "Ai castigat licitatia, afacerea cu id-ul %d ti-a fost transferata", biz);
			            bizInfo[biz][bizOwner] = winner;

			            format(bizInfo[biz][bizOwnerName], MAX_PLAYER_NAME, "%s", bidInfo[bidPlayer]);

			            mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `OwnerSQL` = '%d' where `id` = '%d';", winner, biz);
			            mysql_tquery(SQL, returnString, "", "");

			            mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Business` = '%d' where `id` = '%d';", biz, winner);
			            mysql_tquery(SQL, returnString, "", "");

			            WhenBusinessGotUpdated(biz);
						deleteProperty(biz);
					}
					else {
			            bizInfo[biz][bizOwner] = winner;
			            playerInfo[playerBidded][pBusiness] = biz;

			            format(bizInfo[biz][bizOwnerName], MAX_PLAYER_NAME, "%s", bidInfo[bidPlayer]);

			            mysql_format(SQL, returnString, sizeof returnString, "update `businesses` set `OwnerSQL` = '%d' where `id` = '%d';", winner, biz);
			            mysql_tquery(SQL, returnString, "", "");

			            mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Business` = '%d' where `id` = '%d';", biz, winner);
			            mysql_tquery(SQL, returnString, "", "");

			            WhenBusinessGotUpdated(biz);
						SendClientMessage(playerBidded, -1, "Ai castigat licitatia, afacerea cu id-ul %d ti-a fost transferata", biz);
						deleteProperty(biz);
					}
				}
				case BID_TYPE_APARTMENT: {
					new const apartment = bidInfo[bidPropriety];

					if(!IsPlayerConnected(playerBidded)) {
						insertPlayerMail(bidInfo[bidSQL], gettime(), "Ai castigat licitatia, apartamentul cu id-ul %d ti-a fost transferat", apartment);
						apInfo[apartment][apOwner] = winner;

			            format(apInfo[apartment][apOwnerName], MAX_PLAYER_NAME, "%s", bidInfo[bidPlayer]);

			            mysql_format(SQL, returnString, sizeof returnString, "update `apartaments` set `OwnerSQL` = '%d', `OwnerName` = '%s'  where `id` = '%d';", winner, bidInfo[bidPropriety], bidInfo[bidPlayer]);
			            mysql_tquery(SQL, returnString, "", "");
			        
			            mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Apartament` = '%d' where `id` = '%d';", bidInfo[bidPropriety], winner);
			            mysql_tquery(SQL, returnString, "", "");

			            WhenApartamentGotUpdated(bidInfo[bidPropriety]);
						deleteProperty(bidInfo[bidPropriety]);
					}
					else {
						apInfo[apartment][apOwner] = winner;

			            format(apInfo[apartment][apOwnerName], MAX_PLAYER_NAME, "%s", bidInfo[bidPlayer]);

			            mysql_format(SQL, returnString, sizeof returnString, "update `apartaments` set `OwnerSQL` = '%d', `OwnerName` = '%s'  where `id` = '%d';", winner, apartment, bidInfo[bidPlayer]);
			            mysql_tquery(SQL, returnString, "", "");
			        
			            mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Apartament` = '%d' where `id` = '%d';", apartment, winner);
			            mysql_tquery(SQL, returnString, "", "");

			            WhenApartamentGotUpdated(apartment);

						playerInfo[playerBidded][pApartament] = apartment;
						SendClientMessage(playerBidded, -1, "Ai castigat licitatia, apartamentul cu id-ul %d ti-a fost transferat", bidInfo[bidPropriety]);
						deleteProperty(bidInfo[bidPropriety]);
					}
				}
			}
			bidInfo = resetBid;
			oldBidder = -1;

		}
	}
	return true;
}

task CheckForAuctions[120000]()
{
	new returnHour, returnMinute, returnSeconds;
	gettime(returnHour, returnMinute, returnSeconds);

	if(returnHour >= 20 && returnHour <= 23)
	{
		if(!bidInfo[bidStatus])
			StartNewAuction();
	}
}