#include <YSI\y_hooks>
#include "private.inc"
#include "commands.pwn"

hook OnPlayerConnect(playerid)
{
    SendClientMessage(playerid, -1, "OnPlayerConnect");
    new s_query[128];

    GetPlayerName(playerid, player[playerid][e_s_name], MAX_PLAYER_NAME);

    player[playerid][e_i_invalidPassRemaining] = G_I_PASSWORD_ATTEMPTS;

    /*
        pid = playerid; the ID of the player that requested the query.
    */
    inline OnDataLoaded(i_playerid)
    {
        SendClientMessage(i_playerid, -1, "OnDataLoaded");
        player[i_playerid][e_cache] = cache_save();

        // If this player isn't in the MySQL Database...
        if(cache_num_rows() == 0)
        {
            // Check if they are in the SQLite(old) database and store their encrypted password.
            if(IsPlayerRegisteredInOldDB(i_playerid))
            {
                SendClientMessage(i_playerid, -1, "You are in the old DB!");

                inline Response(pid, dialogid, response, listitem, inputtext[])
                {
                    #pragma unused dialogid, listitem
                    

                    if(response == NEGATIVE)
                    {
                        SendClientMessage(pid, -1, "Negative Response");
                        GivePlayerTempName(pid);
                    }
                    else
                    {
                        SendClientMessage(pid, -1, "Positive Response");
                        new s_hashPass[16];
                        new s_message[128];

                        // Grab their input and use the old encryption method to encrypt.
                        format(s_hashPass, sizeof(s_hashPass) + 1, "%s", encrypt(inputtext));

                        printf("Pass: [%s], Input: [%s]", player[pid][e_s_oldHashPass], s_hashPass);

                        // If the hashes match...
                        if(strlen(inputtext) > G_I_MIN_PASS_LENGTH && strcmp(s_hashPass, player[pid][e_s_oldHashPass]) == 0)
                        {
                            SendClientMessage(pid, X11_GREEN, "Passwords match!");
                            RegisterPlayer(pid);
                        }
                        else
                        {
                            SendClientMessage(pid, -1, "No match.");


                            if(player[pid][e_i_invalidPassRemaining] > 1)
                            {
                                SendClientMessage(pid, -1, "Retry.");
                                player[pid][e_i_invalidPassRemaining]--;

                                format(s_message, sizeof(s_message), "{AA0000}ERROR: Invalid Password. Attempts remaining: %d{EBEBEB}\n\nPlease enter your password: ", player[pid][e_i_invalidPassRemaining]);

                                Dialog_ShowCallback(pid, using inline Response, DIALOG_STYLE_PASSWORD, "Sign In", s_message, "Sign In", "Cancel");
                            }
                        }
                    }
                    

                }
                Dialog_ShowCallback(i_playerid, using inline Response, DIALOG_STYLE_PASSWORD, "Sign In", "{EBEBEB}\n\nPlease enter your password: ", "Sign In", "Cancel");
            }
            else
                SendClientMessage(i_playerid, -1, "You are NOT in the old DB!");
        }
    }
    SendClientMessage(playerid, -1, "Before querying");
    mysql_format(g_mysqlHandle, s_query, sizeof(s_query), " SELECT * \
                                                                FROM users \
                                                            WHERE username = lower('%e')", player[playerid][e_s_name]);
    mysql_tquery_inline(g_mysqlHandle, s_query, using inline OnDataLoaded, "i", playerid);
    SendClientMessage(playerid, -1, "After querying");


    return true;
}