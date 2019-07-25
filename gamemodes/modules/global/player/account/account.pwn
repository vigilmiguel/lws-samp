#include <YSI\y_hooks>
#include "private.inc"
#include "commands.inc"

hook OnPlayerConnect(playerid)
{
    //SendClientMessage(playerid, -1, "OnPlayerConnect");
    new s_query[128];

    GetPlayerName(playerid, player[playerid][e_s_name], MAX_PLAYER_NAME);

    // Check if the player joined with a temp name(first 2 characters 'LW').
    if(strcmp(player[playerid][e_s_name], G_S_TEMPNAME, true, 2) == 0)
    {
        // Kick them.
        //SendClientMessage(playerid, -1, "Illegal name.");
        Server_Kick(playerid, "Attempted to join with a temporary name.");
        return true;
    }

    player[playerid][e_i_invalidPassRemaining] = G_I_PASSWORD_ATTEMPTS;

    /*
        i_playerid = the id of the player that requested the data.
    */
    inline OnDataLoaded(i_playerid)
    {
        //SendClientMessage(i_playerid, -1, "OnDataLoaded");
        //player[i_playerid][e_cache] = cache_save();

        // If this player isn't in the MySQL Database...
        if(cache_num_rows() == 0)
        {
            SendClientMessage(i_playerid, X11_RED, "No record found in MySQL database.");
            // Check if they are in the SQLite(old) database and store their encrypted password.
            if(IsPlayerRegisteredInOldDB(i_playerid))
            {
                SendClientMessage(i_playerid, -1, "You are in the old DB!");

                PromptUserForOldPassword(i_playerid);
            }
            else
                SendClientMessage(i_playerid, -1, "You are NOT in the old DB!");
        }
        else
        {
            // Player is in the new DB.
            SendClientMessage(i_playerid, X11_GREEN, "You are in the MySQL Database!");

            // Load their data.(More to come soon.)
            cache_get_value_index(0, 0, player[i_playerid][e_s_hash], G_I_HASH_SIZE+1);
            cache_get_value_index(0, 1, player[i_playerid][e_s_salt], G_I_SALT_SIZE+1);

            LoginPlayer(i_playerid);
        }
    }
    //SendClientMessage(playerid, -1, "Before querying");
    mysql_format(g_mysqlHandle, s_query, sizeof(s_query), " SELECT password, salt \
                                                                FROM users \
                                                            WHERE username = lower('%e')", player[playerid][e_s_name]);
    mysql_tquery_inline(g_mysqlHandle, s_query, using inline OnDataLoaded, "i", playerid);
    //SendClientMessage(playerid, -1, "After querying");


    return true;
}