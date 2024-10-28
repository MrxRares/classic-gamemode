
YCMD:emails(playerid, params[], help)
{
    static emailString[144];
    mysql_format(SQL, emailString, sizeof emailString, "select * from `emails` where `iPlayer` = '%d' order by `id` desc limit 10;", playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, emailString, "show_player_emails", "i", playerid);
    return true;
}

function:check_for_emails(playerid)
{
    if(!cache_num_rows())
        return true;

    SendClientMessage(playerid, COLOR_YELLOW, "{FF6200}**{FFFF00} You have unread email(s). Write [/emails] to see them.{FF6200}**");

    mysql_format(SQL, returnString, sizeof returnString, "update `emails` set `DeliverStatus` = '1' where `iPlayer` = '%d';", playerInfo[playerid][pSQLID]);
    mysql_tquery(SQL, returnString, "", "");
    return true;
}

function:show_email_notification(playerid)
{
    if(!cache_num_rows())
        return true;

    SendClientMessage(playerid, COLOR_YELLOW, "{FF6200}**{FFFF00} You have unread email(s). Write [/emails] to see them.{FF6200}**");
    return true;
}

function:show_player_emails(playerid)
{
    if(!cache_num_rows())
        return sendError(playerid, "You don't have emails.");

    static returnMessage[24], returnStatus, emailString[512];
    emailString = "Status\tText\n";

    for(new i; i < cache_num_rows(); i++)
    {
        cache_get_value_name(i, "sMessage", returnMessage);
        cache_get_value_name_int(i, "iReadStatus", returnStatus);

        format(emailString, sizeof emailString, "%s%s\t%s%s\n", emailString, (returnStatus == 0 ? ("unread") : ("read")), returnMessage, (strlen(returnMessage) >= 22 ? ("...") : ("")));

        cache_get_value_name_int(i, "ID", playerStaticVar[playerid][pDialogItems][i]);
    }
    Dialog_Show(playerid, DIALOG_EMAILS, DIALOG_STYLE_TABLIST_HEADERS, "Emails", emailString, "Read", "Exit");
    return true;
}

Dialog:DIALOG_EMAILS(playerid, response, listitem, inputtext[])
{
    if(!response)
        return true;

    playerStaticVar[playerid][pDialogSelected] = listitem;

    mysql_format(SQL, returnString, sizeof returnString, "select `sMessage`, `iTimestamp` from `emails` where `ID` = '%d';", playerStaticVar[playerid][pDialogItems][listitem]);
    mysql_tquery(SQL, returnString, "WhenPlayerReadEmail", "ii", playerid, playerStaticVar[playerid][pDialogItems][listitem]);

    return true;
}

function:WhenPlayerReadEmail(playerid, emailid)
{
    new returnMessage[256], returnEmailFormat[412], returnTime, dateFormat[6];
    cache_get_value_name(0, "sMessage", returnMessage);
    cache_get_value_name_int(0, "iTimestamp", returnTime);

    returnDateFromTimestamp(returnTime, dateFormat[0], dateFormat[1], dateFormat[2], dateFormat[3], dateFormat[4], dateFormat[5], 2);
    format(returnEmailFormat, sizeof returnEmailFormat, "Email #%d\n%s\nSent on: %02d-%02d-%02d %02d:%02d:%02d", playerStaticVar[playerid][pDialogItems][playerStaticVar[playerid][pDialogSelected]], returnMessage, dateFormat[0], dateFormat[1], dateFormat[2], dateFormat[3], dateFormat[4], dateFormat[5]);

    Dialog_Show(playerid, DIALOG_EMAIL_READ, DIALOG_STYLE_MSGBOX, "Read email", returnEmailFormat, "Back", "Exit");
    return true;
}

Dialog:DIALOG_EMAIL_READ(playerid, response, listtiem, inputtext[])
{
    mysql_format(SQL, returnString, sizeof returnString, "update `emails` set `iReadStatus` = '1' where `id` = '%d';", playerStaticVar[playerid][pDialogItems][playerStaticVar[playerid][pDialogSelected]]);
    mysql_tquery(SQL, returnString, "", "");

    if(response)
        return Command_ReProcess(playerid, "/emails", false);

    return true;
}

insertPlayerMail(sqlid, iTime, const message[], va_args<>)
{
    new emailString[512],
        formatString[256];

    new id = getPlayerIDbySQLID(sqlid);

    if(IsPlayerConnected(id))
        SendClientMessage(id, COLOR_YELLOW, "{FF6200}**{FFFF00} You have unread email(s). Write [/emails] to see them. {FF6200}**");

    va_format(formatString, sizeof formatString, message, va_start<3>);

    mysql_format(SQL, emailString, sizeof emailString, "insert into `emails` (`sMessage`, `iPlayer`, `iTimestamp`, `iReadStatus`) values ('%s', '%d', '%d', '0');", formatString, sqlid, iTime);
    mysql_tquery(SQL, emailString, "", "");
    return true;
}