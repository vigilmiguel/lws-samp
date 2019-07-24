// Opens the old SQLite database.
stock DB:DBOpen(playerid=-1)
{
	new DB:database = db_open(SQLITE_DB);

	if (!database && playerid >= 0 && IsPlayerConnected(playerid))
		printf("** [SQLite] Couldn't connect to the database (%d).", _:database); 

	return database;
}

stock IsPlayerRegisteredInOldDB(playerid)
{
    new DB:database = DBOpen(); // Open the database
    new username[MAX_PLAYER_NAME];
    new bool:isInDatabase = false;

    GetPlayerName(playerid, username, sizeof(username));

	if(database) // If the database opened successfully...
	{
	    new DBResult:result;
		new query[100];

		// search the database for the username provided...
		format(query, sizeof(query), "  SELECT password \
                                            FROM lw_lvdm_users \
                                        WHERE name = lower('%q')", username);
		result = db_query(database, query);

        // If this player's name is in the database...
		if( db_num_rows(result) )
		{
			isInDatabase = true;

            printf("Playerid: %d, Attempts: %d", playerid, player[playerid][e_i_invalidPassRemaining]);
            db_get_field_assoc(result, "password", player[playerid][e_s_oldHashPass], OLD_HASHPASS_SIZE);
            
            printf("Hashed Password: %s", player[playerid][e_s_oldHashPass]);
            SendClientMessage(playerid, -1, player[playerid][e_s_oldHashPass]);

            printf("Playerid: %d, Attempts: %d", playerid, player[playerid][e_i_invalidPassRemaining]);


            
		}

        db_free_result(result);
	}

	db_close(database);

	return isInDatabase;
}

stock RegisterPlayer(playerid)
{

}

stock GivePlayerTempName(playerid)
{
    new s_tempName[MAX_PLAYER_NAME];
    new s_message[128];
                        

    player[playerid][e_b_tempName] = true;

    format(s_tempName, sizeof(s_tempName), "%s%d", G_S_TEMPNAME, playerid);
    SetPlayerName(playerid, s_tempName);

    format(player[playerid][e_s_name], MAX_PLAYER_NAME, "%s", s_tempName);

    format(s_message, sizeof(s_message), "Your name has been changed to: %s", s_tempName);
    SendClientMessage(playerid, X11_ORANGE_RED, s_message);
}


/*
    Old encryption method used to generate the old passwords.
    
*/
stock encrypt(pass[])
{
	// JSCHash - Created by Y_Less's Dad and Y_Less!
	
	static
		charset[] = "qwertyaQWERTYUIOPZXCVBNMLKJHGFDSAsdfghzxcvbnuiopjklm1324657809_",
		css = 63;
		
	new
		target[OLD_HASHPASS_SIZE + 1],
		j = strlen(pass);
	new
		sum = j,
		tmp = 0,
		i,
		mod;
		
	for (i = 0; i < OLD_HASHPASS_SIZE || i < j; i++)
	{
		mod = i % OLD_HASHPASS_SIZE;
		tmp = (i >= j) ? charset[(7 * i) % css] : pass[i];
		sum = (sum + chrFind(tmp, charset) + 1)		% css;
		target[mod] = charset[(sum + target[mod])	% css];
	}
	target[OLD_HASHPASS_SIZE] = '\0';
	return target;
}

stock chrFind(needle, haystack[], start = 0)
{
	// Created by Y_Less!
	
	while (haystack[start]) if (haystack[start++] == needle) return start - 1;
	return -1;
}