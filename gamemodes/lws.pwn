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
	
	return true;
}

public OnGameModeExit()
{
	return true;
}

public OnPlayerConnect(playerid)
{
	SendClientMessage(playerid, X11_GREEN, "You have connected!");
	return true;
}
