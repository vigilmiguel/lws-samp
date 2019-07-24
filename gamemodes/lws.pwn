#include "lws_h.pwn"

main()
{
	print("\n----------------------------------");
	print(" Blank Gamemode by your name here");
	print("----------------------------------\n");
}

public OnGameModeInit()
{
	AddPlayerClass(0, 1958.3783, 1343.1572, 15.3746, 269.1425, 0, 0, 0, 0, 0, 0);

	new s_gmText[128];
	format(s_gmText, sizeof(s_gmText), "LW %i.%i.%i", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);
	SetGameModeText(s_gmText);

	g_mysqlHandle = mysql_connect_file("mysql.ini");
	

    if(mysql_errno(g_mysqlHandle) != MYSQL_SUCCESS)  
        printf("** [MySQL] Couldn't connect to the database (%d).", mysql_errno(g_mysqlHandle)); 
    else  
        printf("** [MySQL] Connected to the database successfully (%d).", _:g_mysqlHandle); 

    
	
	return true;
}

public OnGameModeExit()
{
	print("Closing database.");
	mysql_close(g_mysqlHandle);
	return true;
}

public OnPlayerConnect(playerid)
{
	SendClientMessage(playerid, X11_GREEN, "You have connected!");
	return true;
}
