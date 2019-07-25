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

            //printf("Playerid: %d, Attempts: %d", playerid, player[playerid][e_i_invalidPassRemaining]);

			// grab their encrypted password and store it.
            db_get_field_assoc(result, "password", player[playerid][e_s_oldHashPass], OLD_HASHPASS_SIZE);
            
            //printf("Hashed Password: %s", player[playerid][e_s_oldHashPass]);
            //SendClientMessage(playerid, -1, player[playerid][e_s_oldHashPass]);

            //printf("Playerid: %d, Attempts: %d", playerid, player[playerid][e_i_invalidPassRemaining]);


            
		}

        db_free_result(result);
	}

	db_close(database);

	return isInDatabase;
}

stock PromptUserForOldPassword(playerid)
{
	inline Response(pid, dialogid, response, listitem, inputtext[])
	{
		#pragma unused dialogid, listitem
		
		if(response == NEGATIVE)
			GivePlayerTempName(pid);
		else
		{
			//SendClientMessage(pid, -1, "Positive Response");
			new s_hashPass[16];
			new s_message[128];
			new i_inputLength = strlen(inputtext);
			new s_password[G_I_MAX_PASS_LENGTH];

			// Store the inputtext(for some reason the old encrypt function deletes its contents.)
			format(s_password, sizeof(s_password), "%s", inputtext);
			
			// Grab their input and use the old encryption method to encrypt.
			format(s_hashPass, sizeof(s_hashPass) + 1, "%s", encrypt(inputtext));

			printf("Pass: [%s], Input: [%s]", player[pid][e_s_oldHashPass], s_hashPass);
			
			// If the hashes match...
			if(i_inputLength > G_I_MIN_PASS_LENGTH && strcmp(s_hashPass, player[pid][e_s_oldHashPass]) == 0)
			{
				SendClientMessage(pid, X11_GREEN, "Passwords match!");

				RegisterPlayer(pid, s_password);

				Dialog_Show(pid, DIALOG_STYLE_MSGBOX, "Authentication", "{33AA55}Access Granted!\n\n\
																			{EBEBEB}Your old account has been transferred to the new Database!", "OK");
			}
			else
			{
				//SendClientMessage(pid, -1, "No match.");
				if(player[pid][e_i_invalidPassRemaining] > 0)
				{
					//SendClientMessage(pid, -1, "Retry.");
					player[pid][e_i_invalidPassRemaining]--;

					format(s_message, sizeof(s_message), "{E62233}ERROR: Invalid Password. Attempts remaining: %d{EBEBEB}\n\n\
															Please enter your password: ", player[pid][e_i_invalidPassRemaining]);

					Dialog_ShowCallback(pid, using inline Response, DIALOG_STYLE_PASSWORD, "Authentication", s_message, "Sign In", "Cancel");
				}
				else
					GivePlayerTempName(pid);
			}
		}
	}
	Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_PASSWORD, "Authentication",
							 "{EBEBEB}Welcome back! It looks like you already have an account in the old database.\n\n\
								Please enter your password: ", "Sign In", "Cancel");
}

stock RegisterPlayer(playerid, s_password[])
{
	new s_query[300];
	new salt[G_I_SALT_SIZE];
	new hash[G_I_HASH_SIZE];

	// Randomly generate the salt for this user.
	for(new i = 0; i < sizeof(salt); i++)
	{
	    salt[i] = random(79) + 47;
	}

	// encrypt their password with the new and more reliable hashing method.
	SHA256_PassHash(s_password, salt, hash, sizeof(hash));

	// Insert this user in the new MySQL database.
	mysql_format(g_mysqlHandle, s_query, sizeof(s_query), "	INSERT INTO users \
																(username, password, salt) \
															VALUES \
																(lower('%e'), '%e', '%e')",
																player[playerid][e_s_name], hash, salt);
	mysql_tquery(g_mysqlHandle, s_query);
}

stock LoginPlayer(playerid)
{

	inline Response(pid, dialogid, response, listitem, inputtext[])
	{
		#pragma unused dialogid, listitem

		if(response == NEGATIVE)
		{
			GivePlayerTempName(pid);
		}
		else
		{
			new s_message[128];
			new hash[G_I_HASH_SIZE];

			// encrypt the user inputted password.
			SHA256_PassHash(inputtext, player[pid][e_s_salt], hash, sizeof(hash));

			printf("Hash: [%s] Salt: [%s] UserHash: [%s]", player[pid][e_s_hash], player[pid][e_s_salt], hash);

			// IF their password was correct...
			if(strlen(inputtext) > G_I_MIN_PASS_LENGTH && strcmp(hash, player[pid][e_s_hash]) == 0)
			{
				Dialog_Show(pid, DIALOG_STYLE_MSGBOX, "Authentication", "{33AA55}Access Granted!", "OK");
			}
			else
			{
			
				if(player[pid][e_i_invalidPassRemaining] > 0)
				{
				
					player[pid][e_i_invalidPassRemaining]--;

					format(s_message, sizeof(s_message), "{E62233}ERROR: Invalid Password. Attempts remaining: %d{EBEBEB}\n\n\
															Please enter your password: ", player[pid][e_i_invalidPassRemaining]);

					Dialog_ShowCallback(pid, using inline Response, DIALOG_STYLE_PASSWORD, "Authentication", s_message, "Sign In", "Cancel");
				}
				else
					GivePlayerTempName(pid);
			}


		}
	}
	Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_PASSWORD, "Authentication", "{EBEBEB}Welcome back!\n\n\
																								Please enter your password: ", "Sign In", "Cancel");
}

stock GivePlayerTempName(playerid)
{
    new s_tempName[MAX_PLAYER_NAME];
    new s_message[128];
                        
	// Set a flag indicating that the user is using a temp name.
    player[playerid][e_b_tempName] = true;

    format(s_tempName, sizeof(s_tempName), "%s%d", G_S_TEMPNAME, playerid);
    SetPlayerName(playerid, s_tempName);

    format(player[playerid][e_s_name], MAX_PLAYER_NAME, "%s", s_tempName);

    format(s_message, sizeof(s_message), "Your name has been changed to: %s", s_tempName);
    SendClientMessage(playerid, X11_ORANGE_RED, s_message);
}


/*
    Old encryption method used to generate the old password hashes.
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