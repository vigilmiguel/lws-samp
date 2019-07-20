#include <YSI\y_hooks>

hook OnPlayerConnect(playerid)
{
    SendClientMessage(playerid, X11_GREEN, "OnPlayerConnect Hooked!");
    return true;
}