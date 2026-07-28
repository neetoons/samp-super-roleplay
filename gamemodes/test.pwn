#include <a_samp>

#define MAX_PLAYERS 500

public OnGameModeInit()
{
    print("[TEST] Gamemode started!");
    return 1;
}

public OnGameModeExit()
{
    print("[TEST] Gamemode stopped!");
    return 1;
}

public OnPlayerConnect(playerid)
{
    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));
    printf("[TEST] Player connected: %s (id: %d)", name, playerid);
    SendClientMessage(playerid, 0x00FF00FF, "Welcome to the test server!");
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));
    printf("[TEST] Player disconnected: %s (id: %d)", name, playerid);
    return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
    SpawnPlayer(playerid);
    return 1;
}
