forward Admin_Kick(playerid, adminid, reason[]);
forward Server_Kick(playerid, reason[]);

// Kicks the player without an admin.
public Server_Kick(playerid, reason[])
{
    SendClientMessage(playerid, -1, "Server Kick.");
    Admin_Kick(playerid, INVALID_PLAYER_ID, reason);
}

// Kicks the player with an admin.
public Admin_Kick(playerid, adminid, reason[])
{
    
    new orange = X11_ORANGE;
    new yellow = X11_YELLOW;
    new s_message[128];

    SendClientMessage(playerid, -1, "Admin Kick.");

    if(adminid == INVALID_PLAYER_ID)
        format(s_message, sizeof(s_message), ">> ** %s has been kicked. Reason: %s",
            player[playerid][e_s_name], reason);
    else
        format(s_message, sizeof(s_message), ">> ** %s has been kicked by %s. Reason: %s",
            player[playerid][e_s_name], player[adminid][e_s_name], reason);

    

    SendClientMessageToAll(orange, s_message);

    SendClientMessage(playerid, yellow, "");
    SendClientMessage(playerid, yellow, "");
    SendClientMessage(playerid, yellow, "");
    SendClientMessage(playerid, yellow, "");
    SendClientMessage(playerid, yellow, ">> ** You have been kicked.");

    format(s_message, sizeof(s_message), ">> ** Reason: %s", reason);
    SendClientMessage(playerid, yellow, s_message);
    
    defer DelayKick(playerid);

}

// When called with defer, wait 1 second before calling this function.
timer DelayKick[1000](playerid)
{
    Kick(playerid);
}