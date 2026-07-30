#include <YSI_Coding\y_hooks>

forward OnSecureLoginRequestCode(index, response_code, data[]);
public OnSecureLoginRequestCode(index, response_code, data[])
{
    if (response_code == 200)
    {
        GameTextForPlayer(index, "_", 500, 3);
        PLAYER_TEMP[index][pt_BAD_LOGIN_ATTEMP] = 0;
        ShowDialog(index, DIALOG_SECURE_LOGIN_INSERT_CODE);
    }
    else Kick(index);
}

forward OnSecureLoginCheckCode(index, response_code, data[]);
public OnSecureLoginCheckCode(index, response_code, data[])
{
    if (response_code == 200)
    {
        PLAYER_TEMP[index][pt_BAD_LOGIN_ATTEMP] = 0;

        mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO player_secure_login (ip, id_player) VALUES ('%e', %d);", PLAYER_TEMP[index][pt_IP], ACCOUNT_INFO[index][ac_ID]);
        mysql_tquery(srp_db, QUERY_BUFFER);

        SendNotification(index, "Se ha confirmado el inicio de sesiï¿½n.");
        LoginPlayer(index);
    }
    else
    {
        PLAYER_TEMP[index][pt_BAD_LOGIN_ATTEMP] ++;
        if (PLAYER_TEMP[index][pt_BAD_LOGIN_ATTEMP] > MAX_BAD_LOGIN_ATTEMPS) return Kick(index);
        ShowDialog(index, DIALOG_SECURE_LOGIN_INSERT_CODE);
    }
    return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
    if (PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_CONNECTED) // Reciï¿½n conectado
    {
        PlayAudioStreamForPlayer(playerid, Intro_Audios[random(sizeof Intro_Audios)]);

        if (!PLAYER_TEMP[playerid][pt_USER_VALID_NAME] || PLAYER_TEMP[playerid][pt_KICKED] || PLAYER_TEMP[playerid][pt_CLASSED]) Kick(playerid);
        else
        {
            PLAYER_TEMP[playerid][pt_CLASSED] = true;
            TogglePlayerSpectatingEx(playerid, true);
            ClearPlayerChat(playerid);

            //check ban
            inline OnPlayerBannedCheck()
            {
                new rows, bool:continue_proccess;
                if (cache_get_row_count(rows))
                {
                    if (rows)
                    {
                        new name[24], expire_date[24], id_player, by, text[128], date[24], expire_date_ts, now[24], now_ts, bool:isnull_name, bool:isnull_by, bool:isnull_expire_date_ts;
                        cache_is_value_name_null(0, "name", isnull_name);
                        if (!isnull_name) cache_get_value_name(0, "name", name, 24);
                        cache_get_value_name(0, "expire_date", expire_date, 24);
                        cache_get_value_name_int(0, "id_player", id_player);
                        cache_is_value_name_null(0, "by", isnull_by);
                        if (!isnull_by) cache_get_value_name_int(0, "by", by);
                        cache_get_value_name(0, "text", text, 128);
                        cache_get_value_name(0, "date", date, 24);
                        cache_is_value_name_null(0, "expire_date_ts", isnull_expire_date_ts);
                        if (!isnull_expire_date_ts) cache_get_value_name_int(0, "expire_date_ts", expire_date_ts);
                        cache_get_value_name(0, "now", now, 24);
                        cache_get_value_name_int(0, "now_ts", now_ts);

                        if (expire_date_ts)
                        {
                            //temporal
                            if (now_ts >= expire_date_ts)
                            {
                                continue_proccess = true;
                                mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM bans WHERE name = '%e' OR ip = '%e';", PLAYER_TEMP[playerid][pt_NAME], PLAYER_TEMP[playerid][pt_IP]);
                                mysql_tquery(srp_db, QUERY_BUFFER);

                                if (id_player) AddPlayerBadHistory(id_player, -1, TYPE_UNBAN, "Readmitido (de tban)");
                                SendNotification(playerid, "Tu cuenta ha sido re-admitida, no hagas que vuelvan a banearte.");
                            }
                            else
                            {
                                new dialog[600];
                                format(dialog, sizeof dialog,

                                    "\
                                    Esta IP o cuenta estï¿½ expulsada temporalmente, detalles:\n\
                                    \n\
                                    Tu nombre: %s\n\
                                    \n\
                                    Nombre: %s\n\
                                    Expulsado por: %d\n\
                                    Fecha expulsiï¿½n: %s\n\
                                    Razï¿½n: %s\n\
                                    \n\
                                    Fecha actual: %s\n\
                                    Fecha readmisiï¿½n: %s\n\
                                    \n\
                                    ",

                                    PLAYER_TEMP[playerid][pt_NAME],
                                    name, by, date, text, now, expire_date
                                );

                                ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_MSGBOX, "Aviso", dialog, "Entiendo", "");
                                KickEx(playerid, 500);
                            }

                        }
                        else
                        {
                            //permanente
                            new dialog[480];
                            format(dialog, sizeof dialog,

                                "\
                                Esta IP o cuenta estï¿½ expulsada permanentemente, detalles:\n\
                                \n\
                                Tu nombre: %s\n\
                                \n\
                                Nombre: %s\n\
                                Expulsado por: %d\n\
                                Fecha expulsiï¿½n: %s\n\
                                Razï¿½n: %s\n\
                                \n\
                                ",

                                PLAYER_TEMP[playerid][pt_NAME],
                                name, by, date, text
                            );

                            ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_MSGBOX, "Aviso", dialog, "Entiendo", "");
                            KickEx(playerid, 500);
                        }
                    }
                    else continue_proccess = true;

                    if (continue_proccess)
                    {
                        inline OnPlayerCheck()
                        {
                            if (cache_get_row_count(rows))
                            {
                                TextDrawShowForPlayer(playerid, Textdraws[textdraw_LOAD_SCREEN][2]);
                                TextDrawShowForPlayer(playerid, Textdraws[textdraw_LOAD_SCREEN][3]);

                                if (rows)
                                {
                                    cache_get_value_name_int(0, "id", ACCOUNT_INFO[playerid][ac_ID]);
                                    cache_get_value_name(0, "ip", ACCOUNT_INFO[playerid][ac_IP], 16);
                                    cache_get_value_name(0, "email", ACCOUNT_INFO[playerid][ac_EMAIL], 32);
                                    cache_get_value_name(0, "salt", ACCOUNT_INFO[playerid][ac_SALT], 16);
                                    cache_get_value_name(0, "pass", ACCOUNT_INFO[playerid][ac_PASS], 65);
                                    cache_get_value_name_int(0, "config_secure_login", PI[playerid][ac_CONFIG_SECURE_LOGIN]);

                                    mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET connected = 1, playerid = %d WHERE id = %d;", playerid, ACCOUNT_INFO[playerid][ac_ID]);
                                    mysql_tquery(srp_db, QUERY_BUFFER);
                                    PLAYER_TEMP[playerid][pt_USER_EXIST] = true;
                                }
                                else PLAYER_TEMP[playerid][pt_USER_EXIST] = false;

                                GameTextForPlayer(playerid, "_", 500, 3);
                                if (PLAYER_TEMP[playerid][pt_USER_EXIST])
                                {
                                    new intro = random(sizeof(Intro_Info));
                                    SetPlayerVirtualWorld(playerid, WORLD_INTRO);

                                    TextDrawShowForPlayer(playerid, Textdraws[textdraw_LOAD_SCREEN][1]);
                                    InterpolateCameraPos(playerid,
                                        Intro_Info[intro][intro_info_CAM_X],
                                        Intro_Info[intro][intro_info_CAM_Y],
                                        Intro_Info[intro][intro_info_CAM_Z],
                                        Intro_Info[intro][intro_info_CAM_RX],
                                        Intro_Info[intro][intro_info_CAM_RY],
                                        Intro_Info[intro][intro_info_CAM_RZ],
                                        10000,
                                        CAMERA_MOVE);
                                    InterpolateCameraLookAt(playerid,
                                        Intro_Info[intro][intro_info_CAM_LOOK_AT_X],
                                        Intro_Info[intro][intro_info_CAM_LOOK_AT_Y],
                                        Intro_Info[intro][intro_info_CAM_LOOK_AT_Z],
                                        Intro_Info[intro][intro_info_CAM_LOOK_AT_RX],
                                        Intro_Info[intro][intro_info_CAM_LOOK_AT_RY],
                                        Intro_Info[intro][intro_info_CAM_LOOK_AT_RZ],
                                        10000,
                                        CAMERA_MOVE);
                                    SetPlayerMyWeather(playerid, Intro_Info[intro][intro_info_WEATHER]);
                                    SetPlayerMyTime(playerid, Intro_Info[intro][intro_info_HOUR], 0);

                                    if (!PI[playerid][ac_CONFIG_SECURE_LOGIN] && strcmp(PLAYER_TEMP[playerid][pt_IP], ACCOUNT_INFO[playerid][ac_IP])) // La IP actual no es la misma IP de la ï¿½ltima conexiï¿½n
                                    {
                                        SendNotification(playerid, "Tu direcciï¿½n IP ha cambiado desde tu ï¿½ltima conexiï¿½n.");
                                    }
                                    ShowDialog(playerid, DIALOG_LOGIN);
                                }
                                else
                                {
                                    TextDrawShowForPlayer(playerid, Textdraws[textdraw_LOAD_SCREEN][0]);
                                    // Nuevo usuario
                                    PLAYER_TEMP[playerid][pt_REGISTER_ACTOR] = CreateActor(Intro_Skins[0][0], 444.200225, 514.112182, 1001.419494, 0.939619); //ACTOR DE LA HABITACION DE LA INTRO
                                    SetActorVirtualWorld(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], playerid + 10);
                                    ApplyActorAnimation(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], "INT_SHOP", "shop_loop", 4.1, 1, 1, 1, 0, 0);

                                    SetPlayerVirtualWorld(playerid, playerid + 10);
                                    InterpolateCameraPos(playerid,
                                        447.1717,
                                        512.9380,
                                        1001.6884,
                                        447.1717,
                                        512.9380,
                                        1001.6884,
                                        4000);
                                    InterpolateCameraLookAt(playerid,
                                        446.4043,
                                        513.5767,
                                        1001.7037,
                                        446.4043,
                                        513.5767,
                                        1001.7037,
                                        4000);
                                    SetPlayerInterior(playerid, 12);

                                    ShowDialog(playerid, DIALOG_REGISTER);
                                }
                            }
                            else Kick(playerid);
                        }
                        mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id, ip, email, salt, pass, config_secure_login FROM player WHERE name = '%e';", PLAYER_TEMP[playerid][pt_NAME]);
                        MySQL_TQueryInline(srp_db,  using inline OnPlayerCheck, QUERY_BUFFER);
                    }
                }
                else Kick(playerid);
            }
            mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT *, UNIX_TIMESTAMP(bans.expire_date) AS expire_date_ts, NOW() AS now, UNIX_TIMESTAMP(NOW()) AS now_ts FROM bans LEFT JOIN bad_history ON bans.id_history = bad_history.id WHERE bans.name = '%e' OR bans.ip = '%e';", PLAYER_TEMP[playerid][pt_NAME], PLAYER_TEMP[playerid][pt_IP]);
            MySQL_TQueryInline(srp_db, using inline OnPlayerBannedCheck, QUERY_BUFFER);
        }
    }
    else if ((PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_NORMAL || PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_DEAD) && PLAYER_TEMP[playerid][pt_USER_LOGGED]) // Viene de jugar
    {
        SetSpawnInfo(playerid,
            NO_TEAM,
            PI[playerid][ac_SKIN],
            PI[playerid][ac_POS_X],
            PI[playerid][ac_POS_Y],
            PI[playerid][ac_POS_Z],
            PI[playerid][ac_ANGLE],
            0,
            0,
            0,
            0,
            0,
            0);
        SpawnPlayer(playerid);
        return 0;
    }

    //PLAYER_TEMP[playerid][pt_GAME_STATE] = GAME_STATE_OCCUPIED;
    return 0;
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    if (PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_ENABLED])
    {
        if (playertextid == PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][2]) //Anterior
        {
            if (PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_INTERIO] == 0) PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_INTERIO] = sizeof(PROPERTY_INTERIORS) - 1;
            else PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_INTERIO] --;
            UpdatePlayerPropertyContructor(playerid);
            return 1;
        }
        else if (playertextid == PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][3]) //Siguiente
        {
            if (PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_INTERIO] == (sizeof(PROPERTY_INTERIORS) - 1)) PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_INTERIO] = 0;
            else PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_INTERIO] ++;
            UpdatePlayerPropertyContructor(playerid);
            return 1;
        }
        else if (playertextid == PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][4]) //Crear
        {
            Create_PlayerPropertyConstructo(playerid);
            return 1;
        }
        else if (playertextid == PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][5]) //Cancelar
        {
            ExitPlayerPropertyConstructor(playerid);
            return 1;
        }
        return 1;
    }

    if (PLAYER_TEMP[playerid][pt_READING_GUIDE])
    {
        if (playertextid == PlayerTextdraws[playerid][ptextdraw_GUIDE][2]) // Siguiente
        {
            if (PLAYER_TEMP[playerid][pt_GUIDE_STEP] + 1 >= sizeof(Server_Rules) )
            {
                PLAYER_TEMP[playerid][pt_READING_GUIDE] = false;
                PLAYER_TEMP[playerid][pt_GUIDE_STEP] = 0;
                DestroyPlayerRulesTextDraw(playerid);

                for (new i = 0; i < LOAD_SCREEN_TEXTDRAWS_SIZE; i++)
                {
                    TextDrawHideForPlayer(playerid, Textdraws[textdraw_LOAD_SCREEN][i]);
                }

                PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][0]);
                PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][1]);
                PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2]);
                PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3]);
                PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4]);
                PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5]);
                PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6]);
                PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7]);
                PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8]);
                PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9]);

                PI[playerid][ac_GENDER] = -1;
                PLAYER_TEMP[playerid][pt_REGISTER_SELECTING_SKIN] = true;
                PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] = 0;

                new td_str[64];
                format(td_str, sizeof td_str, "%d/%d", PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] + 1, sizeof(Intro_Skins[]));
                PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9], td_str);

                SendNotification(playerid, "Elige tu personaje, presionar 'continuar' para finalizar.");
                SelectTextDrawEx(playerid, 0xe8d08fFF);
                return 1;
            }

            PLAYER_TEMP[playerid][pt_GUIDE_STEP] ++;
            SetPlayerRulesTextDraw(playerid,
                Server_Rules[ PLAYER_TEMP[playerid][pt_GUIDE_STEP] ][Rule_Title],
                Server_Rules[ PLAYER_TEMP[playerid][pt_GUIDE_STEP] ][Rule_Info],
                Server_Rules[ PLAYER_TEMP[playerid][pt_GUIDE_STEP] ][Rule_Pages],
                PLAYER_TEMP[playerid][pt_GUIDE_STEP] + 1,
                sizeof(Server_Rules) );
            return 1;
        }
        else if (playertextid == PlayerTextdraws[playerid][ptextdraw_GUIDE][3]) // Anterior
        {
            if (PLAYER_TEMP[playerid][pt_GUIDE_STEP] <= 0) return 1;

            PLAYER_TEMP[playerid][pt_GUIDE_STEP] --;
            SetPlayerRulesTextDraw(playerid,
                Server_Rules[ PLAYER_TEMP[playerid][pt_GUIDE_STEP] ][Rule_Title],
                Server_Rules[ PLAYER_TEMP[playerid][pt_GUIDE_STEP] ][Rule_Info],
                Server_Rules[ PLAYER_TEMP[playerid][pt_GUIDE_STEP] ][Rule_Pages],
                PLAYER_TEMP[playerid][pt_GUIDE_STEP] + 1,
                sizeof(Server_Rules) );
            return 1;
        }
    }

    if (PLAYER_TEMP[playerid][pt_REGISTER_SELECTING_SKIN])
    {
        if (playertextid == PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3]) // Hombre
        {
            PI[playerid][ac_GENDER] = 0;

            PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2], -94);
            PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2]);
            PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4], -256);
            PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4]);

            SetActorSkin(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], Intro_Skins[PI[playerid][ac_GENDER]][PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN]], PLAYER_TEMP[playerid][pt_REGISTER_ACTOR]);
            ApplyActorAnimation(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], "INT_SHOP", "shop_loop", 4.1, 1, 1, 1, 0, 0);
            return 1;
        }
        else if (playertextid == PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5]) // Mujer
        {
            PI[playerid][ac_GENDER] = 1;

            PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2], -256);
            PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2]);
            PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4], -94);
            PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4]);

            SetActorSkin(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], Intro_Skins[PI[playerid][ac_GENDER]][PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN]], PLAYER_TEMP[playerid][pt_REGISTER_ACTOR]);
            ApplyActorAnimation(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], "INT_SHOP", "shop_loop", 4.1, 1, 1, 1, 0, 0);
            return 1;
        }
        else if (playertextid == PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6]) // Continuar
        {
            if (PI[playerid][ac_GENDER] == -1)
            {
                SendNotification(playerid, "Antes de continuar debes optar por un gï¿½nero para tu personaje.");
                SelectTextDrawEx(playerid, 0xe8d08fFF);
                return 1;
            }

            TextDrawShowForPlayer(playerid, Textdraws[textdraw_LOAD_SCREEN][0]);
            PI[playerid][ac_SKIN] = Intro_Skins[PI[playerid][ac_GENDER]][PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN]];

            PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][0]);
            PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][1]);
            PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2]);
            PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3]);
            PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4]);
            PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5]);
            PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6]);
            PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7]);
            PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8]);
            PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9]);

            PLAYER_TEMP[playerid][pt_REGISTER_SELECTING_SKIN] = false;
            PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] = 0;

            CancelSelectTextDrawEx(playerid);
            DestroyActor(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR]);
            PLAYER_TEMP[playerid][pt_REGISTER_ACTOR] = INVALID_ACTOR_ID;

            // Spawn
            SetPlayerScore(playerid, ACCOUNT_INFO[playerid][ac_LEVEL]);
            PLAYER_TEMP[playerid][pt_DOUBT_CHANNEL_TIME] = gettime();
            ResetPlayerWeapons(playerid);
            ResetPlayerMoney(playerid);
            GivePlayerMoney(playerid, PI[playerid][ac_CASH]);
            SetPlayerFightingStyle(playerid, PI[playerid][ac_FIGHT_STYLE]);
            SetPlayerHealthEx(playerid, PI[playerid][ac_HEALTH]);
            SetPlayerArmourEx(playerid, PI[playerid][ac_ARMOUR]);
            SetPlayerVirtualWorld(playerid, 0);
            SetSpawnInfo(playerid,
                NO_TEAM,
                PI[playerid][ac_SKIN],
                PI[playerid][ac_POS_X],
                PI[playerid][ac_POS_Y],
                PI[playerid][ac_POS_Z],
                PI[playerid][ac_ANGLE],
                0,
                0,
                0,
                0,
                0,
                0);
            SetPlayerInterior(playerid, PI[playerid][ac_INTERIOR]);

            //
            RegisterNewPlayer(playerid);
            return 1;
        }
        else if (playertextid == PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7]) // Siguiente ropa
        {
            if (PI[playerid][ac_GENDER] == -1) return SendNotification(playerid, "Debes optar por un gï¿½nero antes de cambiar de ropa.");

            if (PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] == (sizeof(Intro_Skins[]) - 1)) PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] = 0;
            else PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] ++;

            new td_str[64];
            format(td_str, sizeof td_str, "%d/%d", PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] + 1, sizeof(Intro_Skins[]));
            PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9], td_str);

            SetActorSkin(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], Intro_Skins[PI[playerid][ac_GENDER]][PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN]], PLAYER_TEMP[playerid][pt_REGISTER_ACTOR]);
            ApplyActorAnimation(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], "INT_SHOP", "shop_loop", 4.1, 1, 1, 1, 0, 0);
            return 1;
        }
        else if (playertextid == PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8]) // Anterior ropa
        {
            if (PI[playerid][ac_GENDER] == -1) return SendNotification(playerid, "Debes optar por un gï¿½nero antes de cambiar de ropa.");

            if (PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] = sizeof(Intro_Skins[]) - 1;
            else PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] --;

            new td_str[64];
            format(td_str, sizeof td_str, "%d/%d", PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] + 1, sizeof(Intro_Skins[]));
            PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9], td_str);

            SetActorSkin(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], Intro_Skins[PI[playerid][ac_GENDER]][PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN]], PLAYER_TEMP[playerid][pt_REGISTER_ACTOR]);
            ApplyActorAnimation(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], "INT_SHOP", "shop_loop", 4.1, 1, 1, 1, 0, 0);
            return 1;
        }
        return 1;
    }

    if (PLAYER_TEMP[playerid][pt_GAME_STATE] != GAME_STATE_NORMAL) return 0;

    if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP] != -1)
    {
        if (playertextid == PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2]) // Comprar
        {
            new clothe_info[3];
            switch (ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE])
            {
                case INTERIOR_BINCO:
                {
                    switch (PI[playerid][ac_GENDER])
                    {
                        case SEX_MALE: clothe_info = Binco_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
                        case SEX_FEMALE: clothe_info = Binco_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
                    }
                }
                case INTERIOR_SUBURBAN:
                {
                    switch (PI[playerid][ac_GENDER])
                    {
                        case SEX_MALE: clothe_info = Suburban_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
                        case SEX_FEMALE: clothe_info = Suburban_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
                    }
                }
                case INTERIOR_PROLAPS:
                {
                    switch (PI[playerid][ac_GENDER])
                    {
                        case SEX_MALE: clothe_info = Prolaps_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
                        case SEX_FEMALE: clothe_info = Prolaps_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
                    }
                }
                case INTERIOR_DIDIER_SACHS:
                {
                    switch (PI[playerid][ac_GENDER])
                    {
                        case SEX_MALE: clothe_info = Didier_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
                        case SEX_FEMALE: clothe_info = Didier_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
                    }
                }
                case INTERIOR_VICTIM:
                {
                    switch (PI[playerid][ac_GENDER])
                    {
                        case SEX_MALE: clothe_info = Victim_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
                        case SEX_FEMALE: clothe_info = Victim_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
                    }
                }
                case INTERIOR_ZIP:
                {
                    switch (PI[playerid][ac_GENDER])
                    {
                        case SEX_MALE: clothe_info = Zip_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
                        case SEX_FEMALE: clothe_info = Zip_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
                    }
                }
            }

            if (PI[playerid][ac_CASH] >= clothe_info[2])
            {
                if (GivePlayerCash(playerid, -clothe_info[2], true, true))
                {
                    PI[playerid][ac_SKIN] = clothe_info[0];

                    PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
                    SendFormatNotification(playerid, "Gracias por comprar ropa en ~g~%s~w~, vuelva pronto.", Clothing_Shop_Positions[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP]][clothing_shop_NAME]);

                    ClosePlayerClothingMenu(playerid);
                    ExitPlayerWardrobe(playerid);
                }
            }
            else
            {
                PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
                SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar esta ropa.", number_format_thousand(clothe_info[2] - PI[playerid][ac_CASH]));
            }
            return 1;
        }
        else if (playertextid == PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][3]) // Siguiente
        {
            PlayerPlaySoundEx(playerid, 1052, 0.0, 0.0, 0.0);

            switch (ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE])
            {
                case INTERIOR_BINCO:
                {
                    switch (PI[playerid][ac_GENDER])
                    {
                        case SEX_MALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Binco_Shop_Male_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
                        }
                        case SEX_FEMALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Binco_Shop_Female_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
                        }
                    }
                }
                case INTERIOR_SUBURBAN:
                {
                    switch (PI[playerid][ac_GENDER])
                    {
                        case SEX_MALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Suburban_Shop_Male_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
                        }
                        case SEX_FEMALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Suburban_Shop_Female_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
                        }
                    }
                }
                case INTERIOR_PROLAPS:
                {
                    switch (PI[playerid][ac_GENDER])
                    {
                        case SEX_MALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Prolaps_Shop_Male_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
                        }
                        case SEX_FEMALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Prolaps_Shop_Female_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
                        }
                    }
                }
                case INTERIOR_DIDIER_SACHS:
                {
                    switch (PI[playerid][ac_GENDER])
                    {
                        case SEX_MALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Didier_Shop_Male_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
                        }
                        case SEX_FEMALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Didier_Shop_Female_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
                        }
                    }
                }
                case INTERIOR_VICTIM:
                {
                    switch (PI[playerid][ac_GENDER])
                    {
                        case SEX_MALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Victim_Shop_Male_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
                        }
                        case SEX_FEMALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Victim_Shop_Female_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
                        }
                    }
                }
                case INTERIOR_ZIP:
                {
                    switch (PI[playerid][ac_GENDER])
                    {
                        case SEX_MALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Zip_Shop_Male_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
                        }
                        case SEX_FEMALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Zip_Shop_Female_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
                        }
                    }
                }
            }

            UpdateClothingShop(playerid);
            return 1;
        }
        else if (playertextid == PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][4]) // Anterior
        {
            PlayerPlaySoundEx(playerid, 1053, 0.0, 0.0, 0.0);

            switch (ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE])
            {
                case INTERIOR_BINCO:
                {
                    switch (PI[playerid][ac_GENDER])
                    {
                        case SEX_MALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Binco_Shop_Male_Skins) - 1;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
                        }
                        case SEX_FEMALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Binco_Shop_Female_Skins) - 1;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
                        }
                    }
                }
                case INTERIOR_SUBURBAN:
                {
                    switch (PI[playerid][ac_GENDER])
                    {
                        case SEX_MALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Suburban_Shop_Male_Skins) - 1;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
                        }
                        case SEX_FEMALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Suburban_Shop_Female_Skins) - 1;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
                        }
                    }
                }
                case INTERIOR_PROLAPS:
                {
                    switch (PI[playerid][ac_GENDER])
                    {
                        case SEX_MALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Prolaps_Shop_Male_Skins) - 1;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
                        }
                        case SEX_FEMALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Prolaps_Shop_Female_Skins) - 1;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
                        }
                    }
                }
                case INTERIOR_DIDIER_SACHS:
                {
                    switch (PI[playerid][ac_GENDER])
                    {
                        case SEX_MALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Didier_Shop_Male_Skins) - 1;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
                        }
                        case SEX_FEMALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Didier_Shop_Female_Skins) - 1;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
                        }
                    }
                }
                case INTERIOR_VICTIM:
                {
                    switch (PI[playerid][ac_GENDER])
                    {
                        case SEX_MALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Victim_Shop_Male_Skins) - 1;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
                        }
                        case SEX_FEMALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Victim_Shop_Female_Skins) - 1;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
                        }
                    }
                }
                case INTERIOR_ZIP:
                {
                    switch (PI[playerid][ac_GENDER])
                    {
                        case SEX_MALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Zip_Shop_Male_Skins) - 1;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
                        }
                        case SEX_FEMALE:
                        {
                            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Zip_Shop_Female_Skins) - 1;
                            else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
                        }
                    }
                }
            }

            UpdateClothingShop(playerid);
            return 1;
        }
    }

    if (PLAYER_TEMP[playerid][pt_TOYS_SHOP])
    {
        if (playertextid == PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5]) //Comprar
        {
            if (PI[playerid][ac_CASH] >= Toys_Shop[ PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] ][shop_toy_price])
            {
                new slot = GetEmptyPlayerToySlot(playerid);
                if (!ACCOUNT_INFO[playerid][ac_VIP] && slot >= MAX_NU_TOYS)
                {
                    PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
                    SendFormatNotification(playerid, "ï¿½Los jugadores ~y~VIP ~w~pueden tener hasta %d accesorios! Usa ~y~/ayuda ~w~si quieres ser ~y~VIP~w~.", MAX_SU_TOYS);
                    return 1;
                }
                if (slot == -1)
                {
                    PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
                    SendNotification(playerid, "No tienes mï¿½s espacio para accesorios, elimina alguno si quieres comprar otro.");
                    ClosePlayerToysMenu(playerid);
                    return 1;
                }

                if (GivePlayerCash(playerid,
                    -Toys_Shop[ PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] ][shop_toy_price],
                    true,
                    true))
                {
                    PLAYER_TOYS[playerid][slot][player_toy_VALID] = true;
                    format(PLAYER_TOYS[playerid][slot][player_toy_NAME], 24, "%s", Toys_Shop[ PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] ][shop_toy_model]);
                    PLAYER_TOYS[playerid][slot][player_toy_ATTACHED] = false;
                    PLAYER_TOYS[playerid][slot][player_toy_MODELID] = Toys_Shop[ PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] ][shop_toy_modelid];
                    PLAYER_TOYS[playerid][slot][player_toy_BONE] = -1; // Sin utilizar aï¿½n
                    PLAYER_TOYS[playerid][slot][player_toy_OFFSET_X] = 0.0;
                    PLAYER_TOYS[playerid][slot][player_toy_OFFSET_Y] = 0.0;
                    PLAYER_TOYS[playerid][slot][player_toy_OFFSET_Z] = 0.0;
                    PLAYER_TOYS[playerid][slot][player_toy_ROT_X] = 0.0;
                    PLAYER_TOYS[playerid][slot][player_toy_ROT_Y] = 0.0;
                    PLAYER_TOYS[playerid][slot][player_toy_ROT_Z] = 0.0;
                    PLAYER_TOYS[playerid][slot][player_toy_SCALE_X] = 1.0;
                    PLAYER_TOYS[playerid][slot][player_toy_SCALE_Y] = 1.0;
                    PLAYER_TOYS[playerid][slot][player_toy_SCALE_Z] = 1.0;
                    PLAYER_TOYS[playerid][slot][player_toy_COLOR_1] = 0;
                    PLAYER_TOYS[playerid][slot][player_toy_COLOR_2] = 0;
                    RegisterNewPlayerToy(playerid, slot);

                    PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
                    SendFormatNotification(playerid, "Gracias por comprar accesorios en ~g~%s~w~, para ponertelo usa ~g~/accesorios~w~.", ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_NAME]);

                    ClosePlayerToysMenu(playerid);
                }
            }
            else
            {
                PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
                SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar esta ropa.", number_format_thousand(Toys_Shop[ PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] ][shop_toy_price] - PI[playerid][ac_CASH]));
            }

        }
        else if (playertextid == PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][6]) // Siguiente
        {
            PlayerPlaySoundEx(playerid, 1052, 0.0, 0.0, 0.0);

            if (PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] == (sizeof(Toys_Shop) - 1)) PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] = 0;
            else PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] ++;

            UpdateToysShop(playerid);
        }
        else if (playertextid == PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][7]) // Anterior
        {
            PlayerPlaySoundEx(playerid, 1053, 0.0, 0.0, 0.0);

            if (PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] == 0) PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] = sizeof(Toys_Shop) - 1;
            else PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] --;

            UpdateToysShop(playerid);
        }

    }

    if (PLAYER_TEMP[playerid][pt_SHOP])
    {
        HidePlayerDialog(playerid);
        if (playertextid == PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0])
        {
            switch (PLAYER_TEMP[playerid][pt_SHOP_STATE])
            {
                case PLAYER_SHOP_STATE_ALL:
                {
                    if (ACCOUNT_INFO[playerid][ac_LEVEL] < 2) return SendNotification(playerid, "Debes ser al menos nivel 2 para publicar anuncios.");

                    new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP]);
                    if (interval < 500) return SendNotification(playerid, "Cï¿½lmate.");
                    PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP] = GetTickCount();

                    inline OnShopChecked()
                    {
                        new rows;
                        if (cache_get_row_count(rows))
                        {
                            if (rows)
                            {
                                new total_ads;
                                cache_get_value_index_int(0, 0, total_ads);

                                if (total_ads >= MAX_ADS_PER_PLAYER) SendFormatNotification(playerid, "No puedes publicar mï¿½s de %d anuncios, borra alguno.", MAX_ADS_PER_PLAYER);
                                else
                                {
                                    HidePlayerShop(playerid);
                                    ShowDialog(playerid, DIALOG_SHOP_ADD);
                                }
                            }
                        }
                    }
                    mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM shop WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
                    MySQL_TQueryInline(srp_db,  using inline OnShopChecked, QUERY_BUFFER);
                }
            }
        }
        else if (playertextid == PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1])
        {
            switch (PLAYER_TEMP[playerid][pt_SHOP_STATE])
            {
                case PLAYER_SHOP_STATE_ALL:
                {
                    PLAYER_TEMP[playerid][pt_SHOP_COME_FROM_MY_ADS] = true;
                    PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = 0;
                    PLAYER_TEMP[playerid][pt_SHOP_STATE] = PLAYER_SHOP_STATE_MY_ADS;
                    UpdatePlayerShop(playerid);
                }
            }
        }
        else if (playertextid == PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2])
        {
            switch (PLAYER_TEMP[playerid][pt_SHOP_STATE])
            {
                case PLAYER_SHOP_STATE_ALL: HidePlayerShop(playerid);
                case PLAYER_SHOP_STATE_MY_ADS: // Atras
                {
                    new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP]);
                    if (interval < 500) return SendNotification(playerid, "Cï¿½lmate.");
                    PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP] = GetTickCount();

                    PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = 0;
                    PLAYER_TEMP[playerid][pt_SHOP_COME_FROM_MY_ADS] = false;
                    PLAYER_TEMP[playerid][pt_SHOP_STATE] = PLAYER_SHOP_STATE_ALL;
                    UpdatePlayerShop(playerid);
                }
                case PLAYER_SHOP_STATE_AD: ShowDialog(playerid, DIALOG_SHOP_ARTICLE_MODIFY);
            }
        }
        else if (playertextid == PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3])
        {
            switch (PLAYER_TEMP[playerid][pt_SHOP_STATE])
            {
                case PLAYER_SHOP_STATE_ALL: // Siguiente
                {
                    new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP]);
                    if (interval < 500) return SendNotification(playerid, "Cï¿½lmate.");
                    PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP] = GetTickCount();

                    inline OnCountQueryLoad()
                    {
                        new rows;
                        if (cache_get_row_count(rows))
                        {
                            if (rows)
                            {
                                new count, pages;
                                cache_get_value_index_int(0, 0, count);
                                pages = GetDatabasePages(count, PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT]);

                                if (PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] >= pages - 1) PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = 0;
                                else PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] ++;
                                UpdatePlayerShop(playerid);
                            }
                        }
                    }
                    MySQL_TQueryInline(srp_db,  using inline OnCountQueryLoad, "SELECT COUNT(id) FROM shop;");
                }
                case PLAYER_SHOP_STATE_MY_ADS: // Siguiente
                {
                    new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP]);
                    if (interval < 500) return SendNotification(playerid, "Cï¿½lmate.");
                    PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP] = GetTickCount();

                    inline OnCountQueryLoad()
                    {
                        new rows;
                        if (cache_get_row_count(rows))
                        {
                            if (rows)
                            {
                                new count, pages;
                                cache_get_value_index_int(0, 0, count);
                                pages = GetDatabasePages(count, PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT]);

                                if (PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] >= pages - 1) PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = 0;
                                else PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] ++;
                                UpdatePlayerShop(playerid);
                            }
                        }
                    }
                    mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM shop WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
                    MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
                }
                case PLAYER_SHOP_STATE_AD: ShowDialog(playerid, DIALOG_SHOP_ARTICLE_REMOVE);
            }
        }
        else if (playertextid == PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4])
        {
            switch (PLAYER_TEMP[playerid][pt_SHOP_STATE])
            {
                case PLAYER_SHOP_STATE_ALL: // Anterior
                {
                    new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP]);
                    if (interval < 500) return SendNotification(playerid, "Cï¿½lmate.");
                    PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP] = GetTickCount();

                    inline OnCountQueryLoad()
                    {
                        new rows;
                        if (cache_get_row_count(rows))
                        {
                            if (rows)
                            {
                                new count, pages;
                                cache_get_value_index_int(0, 0, count);
                                pages = GetDatabasePages(count, PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT]);

                                if (PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] <= 0) PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = pages - 1;
                                else PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] --;
                                UpdatePlayerShop(playerid);
                            }
                        }
                    }
                    MySQL_TQueryInline(srp_db,  using inline OnCountQueryLoad, "SELECT COUNT(id) FROM shop;");
                }
                case PLAYER_SHOP_STATE_MY_ADS: // Anterior
                {
                    new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP]);
                    if (interval < 500) return SendNotification(playerid, "Cï¿½lmate.");
                    PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP] = GetTickCount();

                    inline OnCountQueryLoad()
                    {
                        new rows;
                        if (cache_get_row_count(rows))
                        {
                            if (rows)
                            {
                                new count, pages;
                                cache_get_value_index_int(0, 0, count);
                                pages = GetDatabasePages(count, PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT]);

                                if (PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] <= 0) PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = pages - 1;
                                else PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] --;
                                UpdatePlayerShop(playerid);
                            }
                        }
                    }
                    mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM shop WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
                    MySQL_TQueryInline(srp_db,  using inline OnCountQueryLoad, QUERY_BUFFER);
                }
                case PLAYER_SHOP_STATE_AD: // Atrï¿½s
                {
                    if (PLAYER_TEMP[playerid][pt_SHOP_COME_FROM_MY_ADS])
                    {
                        PLAYER_TEMP[playerid][pt_SHOP_STATE] = PLAYER_SHOP_STATE_MY_ADS;
                        UpdatePlayerShop(playerid);
                    }
                    else
                    {
                        PLAYER_TEMP[playerid][pt_SHOP_STATE] = PLAYER_SHOP_STATE_ALL;
                        UpdatePlayerShop(playerid);
                    }
                }
            }
        }
    }
    return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if (PLAYER_TEMP[playerid][pt_GAME_STATE] != GAME_STATE_NORMAL) return 1;

    if (newkeys &  KEY_CTRL_BACK)
    {
        for (new i = 0; i != E_WORKS; i++)
        {
            if (work_info[i][work_info_TYPE] == WORK_TYPE_FAMILY) continue;

            if (GetPlayerInterior(playerid) == obtain_work_coords[i][obtain_work_INTERIOR])
            {
                if (IsPlayerInRangeOfPoint(playerid,
                    1.0,
                    obtain_work_coords[i][obtain_work_X],
                    obtain_work_coords[i][obtain_work_Y],
                    obtain_work_coords[i][obtain_work_Z]))
                {
                    PC_EmulateCommand(playerid, "/trabajar");
                    return 1;
                }
            }
        }
        if (IsPlayerInRangeOfPoint(playerid,
            1.0,
            626.2347,
            116.3636,
            8.0031)) return ShowDialog(playerid,
            DIAlOG_BUY_MASO);
        if (IsPlayerInRangeOfPoint(playerid, 1.0, -474.596282, 289.679107, 2004.584960)) return PC_EmulateCommand(playerid, "/pvender");
        if (IsPlayerInRangeOfPoint(playerid, 1.0, -508.645385, 322.147644, 2004.585937)) return PC_EmulateCommand(playerid, "/grua");

        if (IsPlayerInDynamicArea(playerid, Farmer_Area))
        {
            for (new i = 0; i != MAX_PLANTS; i++)
            {
                new Float:pos[3];
                GetDynamicObjectPos(PLANTS[i][plant_OBJECT_ID], pos[0], pos[1], pos[2]);

                if (IsPlayerInRangeOfPoint(playerid, 1.5, pos[0], pos[1], pos[2]))
                {
                    PC_EmulateCommand(playerid, "/recoger");
                    return 1;
                }
            }
        }

        if (PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] != -1 && ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] != INTERIOR_NO_INFO)
        {
            new index = GetBankLocalIndexByIntType(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE]);
            if (index != -1)
            {
                if (IsPlayerInRangeOfPoint(playerid,
                    1.0,
                    Bank_Interior_Positions[index][bank_X],
                    Bank_Interior_Positions[index][bank_Y],
                    Bank_Interior_Positions[index][bank_Z]))
                {
                    PC_EmulateCommand(playerid, "/banco");
                    return 1;
                }
            }
        }

        new info[3];
        new Float:pos[3];
        Streamer_GetArrayData(STREAMER_TYPE_PICKUP,
            PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID],
            E_STREAMER_EXTRA_ID,
            info);
        if (info[0] == PICKUP_TYPE_ATM)
        {
            Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_X, pos[0]);
            Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_Y, pos[1]);
            Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_Z, pos[2]);

            if (IsPlayerInRangeOfPoint(playerid, 1.0, pos[0], pos[1], pos[2]))
            {
                PC_EmulateCommand(playerid, "/cajero");
                return 1;
            }
        }

        new indexx = GetPlayerFastFoodIndex(playerid);
        if (indexx != -10) return PC_EmulateCommand(playerid, "/pedir");

        if (PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] != -1 && ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] != INTERIOR_NO_INFO)
        {
            PLAYER_TEMP[playerid][pt_CLOTHING_SHOP] = GetClothingShopIndexByIntType(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE]);
            if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP] != -1)
            {
                if (IsPlayerInRangeOfPoint(playerid,
                    1.0,
                    Clothing_Shop_Positions[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP]][clothing_shop_X],
                    Clothing_Shop_Positions[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP]][clothing_shop_Y],
                    Clothing_Shop_Positions[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP]][clothing_shop_Z]))
                {
                    ShowDialog(playerid, DIALOG_CLOTHES);
                    return 1;
                }
            }
        }

        if (IsPlayerInRangeOfPoint(playerid, 1.0, 2119.059814, -2001.701904, 7.984375))
        {
            PC_EmulateCommand(playerid, "/mercado");
            return 1;
        }
        if (PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] != -1 && ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] == INTERIOR_247)
        {
            if (IsPlayerInRangeOfPoint(playerid, 1.0, -27.964675, -89.948631, 1003.546875))
            {
                PC_EmulateCommand(playerid, "/productos");
                return 1;
            }
        }

        if (
            IsPlayerInRangeOfPoint(playerid, 1.0, 253.280288, 76.219017, 1003.640625) ||
            IsPlayerInRangeOfPoint(playerid, 1.0, 266.547180, 119.652816, 1004.617187) ||
            IsPlayerInRangeOfPoint(playerid, 1.0, 217.816406, 187.667922, 1003.031250))
        {
            if (!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policï¿½a.");
            if (PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_NONE && PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE)
            {
                SendFormatNotification(playerid, "Tienes que dejar de estar de servicio como %s primero.", work_info[ PLAYER_TEMP[playerid][pt_WORKING_IN] ][work_info_NAME]);
                return 1;
            }

            if (!PLAYER_TEMP[playerid][pt_WORKING_IN]) ShowDialog(playerid, DIALOG_SELECT_POLICE_SKIN);
            else CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);

            return 1;
        }

        if (
            (IsPlayerInRangeOfPoint(playerid, 2.0, 219.244018, 69.982215, 1005.039062) ||
            IsPlayerInRangeOfPoint(playerid, 2.0, 262.945587, 109.785270, 1004.617187) ||
            IsPlayerInRangeOfPoint(playerid, 2.0, 222.683914, 186.956054, 1003.031250)) &&
            PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET])
        {
            if (!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policï¿½a.");

            ShowDialog(playerid, DIALOG_POLICE_SHOP);
            return 1;
        }
    }
    else if (newkeys & KEY_FIRE)
    {
        if (ACCOUNT_INFO[playerid][ac_LEVEL] == 1 && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_NONE) ApplyAnimation(playerid, "PED", "IDLE_tired", 4.1, false, false, false, false, 0);
        return 1;
    }
    else if (newkeys == KEY_F && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && TieneRoca[playerid] == 0 )
    //else if (newkeys == KEY_F && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && TieneRoca[playerid] == 0 && PI[playerid][ac_GPS] == 1 && PLAYER_TEMP[playerid][pt_WORKING_IN] == WORK_MINERO)
    {
            for ( new i; i < sizeof MinaInfo; i++ )
            {
                if (IsPlayerInRangeOfPoint(playerid, 1.8, MinaInfo[i][RocaX],MinaInfo[i][RocaY],MinaInfo[i][RocaZ]))
                {
                    if (MinaInfo[i][w_use]) return 1;
                    if (ProcesoPicando[playerid] != 0) return 1;
                    if (PI[playerid][ac_Maso] == 0) return SendNotification(playerid, "No tienes un maso para minar, compralo afuera.");
                    if ( MinaInfo[i][w_count] > 0 )
                    {
                        ProcesoPicando[playerid] = 0;
                        //TiempoTecla[playerid] = 2;
                        TieneRoca[playerid] = 0;
                        MinaInfo[i][w_use] = true;
                        ApplyAnimation(playerid, "BASEBALL","Bat_4", 4.1, 1, 0, 0, 1, 1000);
                        TimerMinando[playerid] = SetTimerEx( "TimerPicando", 300, false, "dd", playerid, i);
                        TogglePlayerControllableEx(playerid, false );
                        ApplyAnimation(playerid, "BASEBALL","Bat_4", 4.1, 1, 0, 0, 1, 1000);
                        TimerSonido[playerid] = SetTimerEx( "TimerSound", 1130, true, "d", playerid);
                        return 1;
                    }
                }
            }
        }
    else if (newkeys & KEY_CROUCH)
    {
        if (PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_NORMAL)
        {
            if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
            {
                for (new i = 0; i != sizeof San_Andreas_Barriers; i++)
                {
                    if (IsPlayerInRangeOfPoint(playerid,
                        San_Andreas_Barriers[i][barrier_DISTANCE],
                        San_Andreas_Barriers[i][barrier_PLAYER_X],
                        San_Andreas_Barriers[i][barrier_PLAYER_Y],
                        San_Andreas_Barriers[i][barrier_PLAYER_Z]))
                    {
                        if (GetPlayerInterior(playerid) == San_Andreas_Barriers[i][barrier_INTERIOR] && GetPlayerVirtualWorld(playerid) == San_Andreas_Barriers[i][barrier_WORLD])
                        {
                            if (San_Andreas_Barriers[i][barrier_VEHICLE_TYPE] == VEHICLE_TYPE_WORK)
                            {
                                if (!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET] && PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE)
                                {
                                    if (San_Andreas_Barriers[i][barrier_VEHICLE_TYPE_IN2] != WORK_NONE)
                                    {
                                        if (!PLAYER_WORKS[playerid][ San_Andreas_Barriers[i][barrier_VEHICLE_TYPE_IN] ][pwork_SET] && !PLAYER_WORKS[playerid][ San_Andreas_Barriers[i][barrier_VEHICLE_TYPE_IN2] ][pwork_SET]) break;
                                    }
                                    else
                                    {
                                        if (!PLAYER_WORKS[playerid][ San_Andreas_Barriers[i][barrier_VEHICLE_TYPE_IN] ][pwork_SET]) break;
                                    }
                                }
                            }
                            if (IsDynamicObjectMoving(San_Andreas_Barriers[i][barrier_OBJECT_ID]) && !San_Andreas_Barriers[i][barrier_CLOSING]) break;
                            if (San_Andreas_Barriers[i][barrier_PRICE] > 0)
                            {
                                if (GivePlayerCash(playerid, -San_Andreas_Barriers[i][barrier_PRICE], false, true))
                                {
                                    SendFormatNotification(playerid, "Has pagado el peaje por ~y~%s ~w~dï¿½lares.", number_format_thousand(San_Andreas_Barriers[i][barrier_PRICE]));
                                }
                                else
                                {
                                    SendNotification(playerid, "No tienes suficiente dinero para pagar el peaje.");
                                    break;
                                }
                            }

                            KillTimer(San_Andreas_Barriers[i][barrier_TIMER]);
                            San_Andreas_Barriers[i][barrier_TIMER] = SetTimerEx("CloseBarrier", 5000, false, "i", i);
                            MoveDynamicObject(San_Andreas_Barriers[i][barrier_OBJECT_ID],
                                San_Andreas_Barriers[i][barrier_X],
                                San_Andreas_Barriers[i][barrier_Y],
                                San_Andreas_Barriers[i][barrier_Z] + 0.72967 + 0.01,
                                0.0065,
                                0.0,
                                0.0,
                                San_Andreas_Barriers[i][barrier_ROTATION]);
                            San_Andreas_Barriers[i][barrier_CLOSING] = false;
                            break;
                        }
                        else break;
                    }
                }
            }

            if (PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET])
            {
                if (GetPlayerInterior(playerid) == 0 && GetPlayerVirtualWorld(playerid) == 0)
                {
                    for (new i = 0; i != sizeof POLICE_GARAGE_DOORS; i++)
                    {
                        if (IsPlayerInRangeOfPoint(playerid,
                            20.0,
                            POLICE_GARAGE_DOORS[i][police_gdoor_CLOSED_X],
                            POLICE_GARAGE_DOORS[i][police_gdoor_CLOSED_Y],
                            POLICE_GARAGE_DOORS[i][police_gdoor_CLOSED_Z]))
                        {
                            if (IsDynamicObjectMoving(POLICE_GARAGE_DOORS[i][police_gdoor_OBJECT_ID]) && !POLICE_GARAGE_DOORS[i][police_gdoor_CLOSING]) break;

                            KillTimer(POLICE_GARAGE_DOORS[i][police_gdoor_TIMER]);
                            POLICE_GARAGE_DOORS[i][police_gdoor_TIMER] = SetTimerEx("ClosePoliceGarageDoor", 5000, false, "i", i);
                            MoveDynamicObject(POLICE_GARAGE_DOORS[i][police_gdoor_OBJECT_ID],
                                POLICE_GARAGE_DOORS[i][police_gdoor_OPENED_X],
                                POLICE_GARAGE_DOORS[i][police_gdoor_OPENED_Y],
                                POLICE_GARAGE_DOORS[i][police_gdoor_OPENED_Z],
                                POLICE_GARAGE_DOORS[i][police_gdoor_MOVE_SPEED],
                                POLICE_GARAGE_DOORS[i][police_gdoor_OPENED_RX],
                                POLICE_GARAGE_DOORS[i][police_gdoor_OPENED_RY],
                                POLICE_GARAGE_DOORS[i][police_gdoor_OPENED_RZ]);
                            POLICE_GARAGE_DOORS[i][police_gdoor_CLOSING] = false;
                            break;
                        }
                    }
                }
            }
        }
        return 1;
    }
    else if (newkeys & KEY_SECONDARY_ATTACK)
    {
        if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
        {
            if (PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] != -1)
            {
                if (ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] == INTERIOR_CLUB)
                {
                    if (IsPlayerInRangeOfPoint(playerid,
                        1.5,
                        1208.744140,
                        -34.261119,
                        1000.953125) || IsPlayerInRangeOfPoint(playerid,
                        1.5,
                        1213.738037,
                        -33.667881,
                        1000.953125))
                    {
                        if (PI[playerid][ac_CASH] <= 25) return SendNotification(playerid, "Digamos que no tienes mucho dinero como para estar gastï¿½ndotelo en esto...");

                        if (GivePlayerCash(playerid, -random(25), false, true))
                        {
                            ApplyAnimation(playerid, "STRIP", RandomPayStripAnimations[random(sizeof(RandomPayStripAnimations))], 4.1, 0, 0, 0, 0, 0);
                        }
                        return 1;
                    }
                }
            }

            if (PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET])
            {
                for (new i = 0; i != sizeof POLICE_DOORS; i++)
                {
                    if (GetPlayerInterior(playerid) != POLICE_DOORS[i][police_door_INTERIOR] || GetPlayerVirtualWorld(playerid) != POLICE_DOORS[i][police_door_WORLD]) continue;
                    if (IsDynamicObjectMoving(POLICE_DOORS[i][police_door_OBJECT_ID]) || !POLICE_DOORS[i][police_door_CLOSED]) continue;

                    new Float:pos[4];

                    pos[0] = POLICE_DOORS[i][police_door_X] + (1.0 * floatsin(-POLICE_DOORS[i][police_door_RZ], degrees));
                    pos[1] = POLICE_DOORS[i][police_door_Y] + (1.0 * floatcos(-POLICE_DOORS[i][police_door_RZ], degrees));
                    pos[2] = POLICE_DOORS[i][police_door_X] + (-1.0 * floatsin(-POLICE_DOORS[i][police_door_RZ], degrees));
                    pos[3] = POLICE_DOORS[i][police_door_Y] + (-1.0 * floatcos(-POLICE_DOORS[i][police_door_RZ], degrees));

                    if (IsPlayerInRangeOfPoint(playerid, 1.8, pos[0], pos[1], POLICE_DOORS[i][police_door_Z]))
                    {
                        KillTimer(POLICE_DOORS[i][police_door_TIMER]);
                        POLICE_DOORS[i][police_door_TIMER] = SetTimerEx("ClosePoliceDoor", 3000, false, "i", i);
                        MoveDynamicObject(POLICE_DOORS[i][police_door_OBJECT_ID],
                            POLICE_DOORS[i][police_door_X],
                            POLICE_DOORS[i][police_door_Y],
                            POLICE_DOORS[i][police_door_Z] + 0.01,
                            0.0125,
                            0.0,
                            0.0,
                            POLICE_DOORS[i][police_door_RZ] - 85.0);
                        POLICE_DOORS[i][police_door_CLOSED] = false;
                        break;
                    }
                    else if (IsPlayerInRangeOfPoint(playerid, 1.8, pos[2], pos[3], POLICE_DOORS[i][police_door_Z]))
                    {
                        KillTimer(POLICE_DOORS[i][police_door_TIMER]);
                        POLICE_DOORS[i][police_door_TIMER] = SetTimerEx("ClosePoliceDoor", 3000, false, "i", i);
                        MoveDynamicObject(POLICE_DOORS[i][police_door_OBJECT_ID],
                            POLICE_DOORS[i][police_door_X],
                            POLICE_DOORS[i][police_door_Y],
                            POLICE_DOORS[i][police_door_Z] + 0.01,
                            0.0125,
                            0.0,
                            0.0,
                            POLICE_DOORS[i][police_door_RZ] + 85.0);
                        POLICE_DOORS[i][police_door_CLOSED] = false;
                        break;
                    }
                }
            }
        }
        return 1;
    }
    else if (newkeys & KEY_NO)
    {
        if (IsPlayerInDynamicArea(playerid, Lumberjack_Area))
        {
            if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
            {
                if (PLAYER_TEMP[playerid][pt_WORKING_IN] == WORK_LUMBERJACK)
                {
                    if (PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] == -1)
                    {
                        if (PLAYER_TEMP[playerid][pt_PLAYER_CARRYING_TREE]) return SendClientMessage(playerid,-1, "{"#SILVER_COLOR"}Lleva la leï¿½a cortada al punto marcado para talar mï¿½s ï¿½rboles.");

                        for (new i = 0; i != sizeof LUMBER_TREES; i++)
                        {
                            if (IsPlayerInRangeOfPoint(playerid,
                                2.0,
                                LUMBER_TREES[i][lumber_tree_X],
                                LUMBER_TREES[i][lumber_tree_Y],
                                LUMBER_TREES[i][lumber_tree_Z]))
                            {
                                if (LUMBER_TREES[i][lumber_tree_STATE] == LUMBER_TREE_STATE_NORMAL)
                                {
                                    TogglePlayerControllableEx(playerid, false);
                                    SetCameraBehindPlayer(playerid);
                                    ApplyAnimation(playerid, "CHAINSAW", "WEAPON_csaw", 4.1, true, false, false, false, 0);
                                    LUMBER_TREES[i][lumber_tree_STATE] = LUMBER_TREE_STATE_CUTTING;
                                    PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] = i;

                                    RemovePlayerAttachedObject(playerid, 9);
                                    SetPlayerAttachedObject(playerid, 9, 341, 6);
                                    SetPlayerArmedWeapon(playerid, 0);

                                    new label_str[256];
                                    format(label_str, sizeof label_str, "{f46530}ï¿½rbol (%d)\n\n{FFFFFF}Talando... {"#SILVER_COLOR"}(%.1f%%)", PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE], 0.0);
                                    UpdateDynamic3DTextLabelText(LUMBER_TREES[ PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] ][lumber_tree_LABEL], -1, label_str);
                                    SetPlayerKeyPress(playerid, minrand(30, 50));
                                }
                                return Y_HOOKS_BREAK_RETURN_1;
                            }
                        }
                    }
                }
            }
        }
        return 1;
    }

    else if (newkeys == KEY_WALK && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && TieneRoca[playerid] != 0 && EnProcesadora(playerid))
    {
            new ganancia;
            ganancia = GetPrecioRoca(TieneRoca[playerid]);
            GivePlayerCash(playerid, ganancia, true, false);
            PLAYER_WORKS[playerid][WORK_MINERO][pwork_LEVEL] ++;
            SendNotification(playerid, "+1 EXP en habilidad de minero.");
            new stringD[120];
            format(stringD, sizeof(stringD), "~g~+%d$", ganancia);
            GameTextForPlayer(playerid, stringD, 1000, 1);
            TieneRoca[playerid] = 0;
            ApplyAnimation(playerid,"CARRY","putdwn",2,0,1,1,0,0);
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
            RemovePlayerAttachedObject(playerid, 5);
            return 1;
        }

    else if (newkeys & KEY_YES)
    {
        switch (GetPlayerState(playerid))
        {
            case PLAYER_STATE_DRIVER: {
                new vehicleid = GetPlayerVehicleID(playerid);

                if (!GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE])
                {
                    if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE] == VEHICLE_TYPE_WORK)
                    {
                        if (TRUCK_VEHICLE[vehicleid][truck_vehicle_LOADING]) return SendNotification(playerid, "Debes esperar a que se cargue el vehï¿½culo para arrancar.");
                        if (TRUCK_VEHICLE[vehicleid][truck_vehicle_UNLOADING]) return SendNotification(playerid, "Debes esperar a que se descargue el vehï¿½culo para arrancar.");
                    }

                    if (PLAYER_TEMP[playerid][pt_IN_TUNING_GARAGE]) return SendNotification(playerid, "Termina primero.");
                }

                KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][7]);
                if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE])
                {
                    PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_NOFUEL][p_ac_info_IMMUNITY] = gettime() + 15;
                    GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 0;
                    UpdateVehicleParams(vehicleid);

                    if (PLAYER_VEHICLES[vehicleid][player_vehicle_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID]) Auto_SendPlayerAction(playerid, "ha detenido su vehï¿½culo.");
                    else Auto_SendPlayerAction(playerid, "ha detenido el vehï¿½culo.");
                }
                else
                {
                    if (PLAYER_VEHICLES[vehicleid][player_vehicle_VALID] && PLAYER_VEHICLES[vehicleid][player_vehicle_CLAMP] > 0)
                    {
                        SendNotification(playerid, "El vehï¿½culo tiene un cepo y no puede arrancar.");
                        return 1;
                    }
                    SendNotification(playerid, "Encendiendo...");
                    PLAYER_TEMP[playerid][pt_TIMERS][7] = SetTimerEx("StartVehicleEngine", 1000, false, "ii", playerid, vehicleid);
                }
            }
            case PLAYER_STATE_ONFOOT: {
                if (ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return 1;
                if (PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID] == 0) return 1;

                new info[3];
                new Float:pos[3];
                Streamer_GetArrayData(STREAMER_TYPE_PICKUP,
                    PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID],
                    E_STREAMER_EXTRA_ID,
                    info);
                if (info[0] == PICKUP_TYPE_NONE) return 1;

                Streamer_GetFloatData(STREAMER_TYPE_PICKUP,
                    PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID],
                    E_STREAMER_X,
                    pos[0]);
                Streamer_GetFloatData(STREAMER_TYPE_PICKUP,
                    PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID],
                    E_STREAMER_Y,
                    pos[1]);
                Streamer_GetFloatData(STREAMER_TYPE_PICKUP,
                    PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID],
                    E_STREAMER_Z,
                    pos[2]);

                if (!IsPlayerInRangeOfPoint(playerid, 1.0, pos[0], pos[1], pos[2])) return 1;

                switch (info[0])
                {
                    case PICKUP_TYPE_ENTER_EXIT:
                    {
                        PLAYER_TEMP[playerid][pt_HOSPITAL] = GetNearestHospitalForPlayer(playerid);

                        if (info[2] == 1) // Estï¿½ en el Pickup Interior y quiere ir al exterior
                        {
                            ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_NORMAL;
                            PI[playerid][ac_LOCAL_INTERIOR] = 0;
                            PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] = -1;
                            SetPlayerPosEx(playerid, ENTER_EXIT[info[1]][ee_EXT_X], ENTER_EXIT[info[1]][ee_EXT_Y], ENTER_EXIT[info[1]][ee_EXT_Z], ENTER_EXIT[info[1]][ee_EXT_ANGLE], ENTER_EXIT[info[1]][ee_EXT_INTERIOR], ENTER_EXIT[info[1]][ee_EXT_WORLD], false /*ENTER_EXIT[info[1]][ee_EXT_FREEZE]*/);
                            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
                            StopAudioStreamForPlayer(playerid);
                            FreezePlayer(playerid);
                            CallLocalFunction("OnPlayerExitInterior", "ii", playerid, info[1]);
                        }
                        else if (info[2] == 2) // Estï¿½ en el Pickup Exterior y quiere ir al interior
                        {
                            if (ENTER_EXIT[info[1]][ee_TIME_OPEN] == 0 && ENTER_EXIT[info[1]][ee_TIME_CLOSE] == 0) // 24 horas
                            {
                                ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_INTERIOR;
                                PI[playerid][ac_LOCAL_INTERIOR] = ENTER_EXIT[info[1]][ee_ID];
                                PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] = info[1];
                                SetPlayerPosEx(playerid, ENTER_EXIT[info[1]][ee_INT_X], ENTER_EXIT[info[1]][ee_INT_Y], ENTER_EXIT[info[1]][ee_INT_Z], ENTER_EXIT[info[1]][ee_INT_ANGLE], ENTER_EXIT[info[1]][ee_INT_INTERIOR], ENTER_EXIT[info[1]][ee_INT_WORLD], false /*ENTER_EXIT[info[1]][ee_INT_FREEZE]*/, true);
                                FreezePlayer(playerid);
                                CallLocalFunction("OnPlayerEnterInterior", "ii", playerid, info[1]);
                            }
                            else
                            {
                                if (is_open(GetMyWorldHour(), ENTER_EXIT[info[1]][ee_TIME_OPEN], ENTER_EXIT[info[1]][ee_TIME_CLOSE]))
                                {
                                    ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_INTERIOR;
                                    PI[playerid][ac_LOCAL_INTERIOR] = ENTER_EXIT[info[1]][ee_ID];
                                    PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] = info[1];
                                    SetPlayerPosEx(playerid, ENTER_EXIT[info[1]][ee_INT_X], ENTER_EXIT[info[1]][ee_INT_Y], ENTER_EXIT[info[1]][ee_INT_Z], ENTER_EXIT[info[1]][ee_INT_ANGLE], ENTER_EXIT[info[1]][ee_INT_INTERIOR], ENTER_EXIT[info[1]][ee_INT_WORLD], false /*ENTER_EXIT[info[1]][ee_INT_FREEZE]*/, true);
                                    FreezePlayer(playerid);
                                    CallLocalFunction("OnPlayerEnterInterior", "ii", playerid, info[1]);

                                    if (ENTER_EXIT[info[1]][ee_INTERIOR_TYPE] == INTERIOR_CLUB) PlayAudioStreamForPlayerEx(playerid, "https://www.dropbox.com/s/yeuwio26i7y00ly/striptease.mp3?dl=1");
                                }
                                else
                                {
                                    if (ENTER_EXIT[info[1]][ee_TIME_OPEN] == -1 && ENTER_EXIT[info[1]][ee_TIME_CLOSE] == -1) SendFormatNotification(playerid, "~r~%s abrirï¿½ prï¿½ximamente.", ENTER_EXIT[info[1]][ee_NAME]);
                                    else SendFormatNotification(playerid, "~r~%s ~w~estï¿½ cerrado, su horario es: ~r~%02d:00h ~w~- ~r~%02d:00h~w~.", ENTER_EXIT[info[1]][ee_NAME], ENTER_EXIT[info[1]][ee_TIME_OPEN], ENTER_EXIT[info[1]][ee_TIME_CLOSE]);
                                }
                            }
                        }
                        else return 1;
                    }
                    case PICKUP_TYPE_PROPERTY:
                    {
                        PLAYER_TEMP[playerid][pt_HOSPITAL] = GetNearestHospitalForPlayer(playerid);

                        if (info[2] == 1) // Estï¿½ en el Pickup Interior y quiere ir al exterior
                        {
                            ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_NORMAL;
                            PI[playerid][ac_LOCAL_INTERIOR] = 0;
                            PLAYER_TEMP[playerid][pt_PROPERTY_INDEX] = -1;
                            SetPlayerPosEx(playerid, PROPERTY_INFO[info[1]][property_EXT_X], PROPERTY_INFO[info[1]][property_EXT_Y], PROPERTY_INFO[info[1]][property_EXT_Z], PROPERTY_INFO[info[1]][property_EXT_ANGLE], PROPERTY_INFO[info[1]][property_EXT_INTERIOR], 0, false /*PROPERTY_INFO[info[1]][property_EXT_FREEZE]*/, false);
                            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
                            StopAudioStreamForPlayer(playerid);
                            FreezePlayer(playerid);
                        }
                        else if (info[2] == 2) // Estï¿½ en el Pickup Exterior y quiere ir al interior
                        {
                            if (
                                PROPERTY_INFO[info[1]][property_POLICE_FORCED] &&
                                (PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET] &&
                                PLAYER_TEMP[playerid][pt_WORKING_IN] == WORK_POLICE))
                            {
                                new Float:z_pos = PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_Z];
                                if (PROPERTY_INFO[info[1]][property_DIS_DEFAULT_INTERIOR]) z_pos += PROPERTY_EMPTY_INTERIOR_Z_OFFSET;
                                SetPlayerPosEx(playerid, PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_X], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_Y], z_pos, PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_ANGLE], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_INTERIOR], PROPERTY_INFO[info[1]][property_ID], false /*PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_FREEZE]*/, true);
                                FreezePlayer(playerid);
                                return 1;
                            }
                            if (!PROPERTY_INFO[info[1]][property_SOLD]) return SendNotification(playerid, "Esta propiedad estï¿½ en venta.");
                            if (PROPERTY_INFO[info[1]][property_CREW])
                            {
                                if (!PI[playerid][ac_CREW]) return SendFormatNotification(playerid, "Solo miembros de la banda pueden entrar.");
                                if (PI[playerid][ac_CREW] != PROPERTY_INFO[info[1]][property_CREW_ID]) return SendFormatNotification(playerid, "Solo miembros de la banda pueden entrar.");

                                ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_GUEST_PROPERTY;
                                PI[playerid][ac_LOCAL_INTERIOR] = PROPERTY_INFO[info[1]][property_ID];
                                PLAYER_TEMP[playerid][pt_PROPERTY_INDEX] = info[1];

                                new Float:z_pos = PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_Z];
                                if (PROPERTY_INFO[info[1]][property_DIS_DEFAULT_INTERIOR]) z_pos += PROPERTY_EMPTY_INTERIOR_Z_OFFSET;
                                SetPlayerPosEx(playerid, PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_X], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_Y], z_pos, PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_ANGLE], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_INTERIOR], PROPERTY_INFO[info[1]][property_ID], false /*PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_FREEZE]*/, true);
                                FreezePlayer(playerid);
                            }
                            else
                            {
                                if (PROPERTY_INFO[info[1]][property_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID])
                                {
                                    ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_OWN_PROPERTY;
                                    PI[playerid][ac_LOCAL_INTERIOR] = PROPERTY_INFO[info[1]][property_ID];
                                    PLAYER_TEMP[playerid][pt_PROPERTY_INDEX] = info[1];

                                    new Float:z_pos = PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_Z];
                                    if (PROPERTY_INFO[info[1]][property_DIS_DEFAULT_INTERIOR]) z_pos += PROPERTY_EMPTY_INTERIOR_Z_OFFSET;
                                    SetPlayerPosEx(playerid, PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_X], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_Y], z_pos, PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_ANGLE], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_INTERIOR], PROPERTY_INFO[info[1]][property_ID], false /*PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_FREEZE]*/, true);
                                    FreezePlayer(playerid);
                                }
                                else
                                {
                                    if (gettime() < PLAYER_TEMP[playerid][pt_ANTIFLOOD_KNOCK_PROPERTY] + 10) return SendNotification(playerid, "Espera para volver a tocar, si no te quieren abrir, vete.");

                                    new owner_playerid = GetOwnerIntProperty(PROPERTY_INFO[info[1]][property_ID]);
                                    if (owner_playerid == -1) return SendNotification(playerid, "Esta no es tu propiedad y el dueï¿½o no estï¿½ dentro para dejarte entrar.");

                                    PLAYER_TEMP[playerid][pt_ANTIFLOOD_KNOCK_PROPERTY] = gettime();
                                    PLAYER_TEMP[owner_playerid][pt_KNOCK_PLAYER_ID] = playerid;
                                    SendFormatNotification(owner_playerid, "~b~%s ~w~estï¿½ tocando la puerta, para dejarle entrar ve a la puerta y usa ~b~/puerta~w~.", PLAYER_TEMP[playerid][pt_RP_NAME]);
                                    SendNotification(playerid, "Has tocado en la puerta, espera para que te abran o vete.");
                                }
                            }
                        }
                        else return 1;
                    }
                    default: return 1;
                }
            }
        }
    }
    return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
    if (PLAYER_TEMP[playerid][pt_KICKED]) return 1;

    if (PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_NORMAL)
    {
        if (newstate != PLAYER_STATE_SPAWNED && oldstate != PLAYER_STATE_SPAWNED)
        {
            if (ac_Info[CHEAT_STATE_SPAMMER][ac_Enabled])
            {
                if (gettime() > PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_IMMUNITY])
                {
                    new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_ANTIFLOOD_STATE]);

                    if (interval < 50)
                    {
                        if (!ac_Info[CHEAT_STATE_SPAMMER][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_STATE_SPAMMER, float(interval));
                        else
                        {
                            if (gettime() - PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_STATE_SPAMMER][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_DETECTIONS] = 0;
                            else PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_DETECTIONS] ++;

                            PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_LAST_DETECTION] = gettime();
                            if (PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_STATE_SPAMMER][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_STATE_SPAMMER, float(interval));
                        }
                    }
                    PLAYER_TEMP[playerid][pt_ANTIFLOOD_STATE] = GetTickCount();
                }
            }
        }
    }

    if (newstate == PLAYER_STATE_DRIVER)
    {
        new vehicleid = GetPlayerVehicleID(playerid);
        if (ac_Info[CHEAT_CAR_JACKING][ac_Enabled])
        {
            if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_OCCUPIED] && GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] != INVALID_PLAYER_ID)
            {
                if (gettime() > PLAYER_AC_INFO[playerid][CHEAT_CAR_JACKING][p_ac_info_IMMUNITY])
                {
                    if (!ac_Info[CHEAT_CAR_JACKING][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_CAR_JACKING);
                    else
                    {
                        if (gettime() - PLAYER_AC_INFO[playerid][CHEAT_CAR_JACKING][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_CAR_JACKING][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_CAR_JACKING][p_ac_info_DETECTIONS] = 0;
                        else PLAYER_AC_INFO[playerid][CHEAT_CAR_JACKING][p_ac_info_DETECTIONS] ++;

                        PLAYER_AC_INFO[playerid][CHEAT_CAR_JACKING][p_ac_info_LAST_DETECTION] = gettime();
                        if (PLAYER_AC_INFO[playerid][CHEAT_CAR_JACKING][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_CAR_JACKING][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_CAR_JACKING);
                    }

                    PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] = INVALID_VEHICLE_ID;
                    return 1;
                }
            }
        }

        SetPlayerArmedWeapon(playerid, 0);
        PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_HEALTH][p_ac_info_IMMUNITY] = gettime() + 3;
        PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_IMMUNITY] = gettime() + 3;
        PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] = vehicleid;

        GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] = playerid;
        GLOBAL_VEHICLES[vehicleid][gb_vehicle_LAST_DRIVER] = playerid;
        GLOBAL_VEHICLES[vehicleid][gb_vehicle_OCCUPIED] = true;
        if (
            PI[playerid][ac_DRIVE_LICENSE_POINTS] == 0 &&
            !PLAYER_TEMP[playerid][pt_DL_EXAM] &&
            VEHICLE_INFO[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID] - 400][vehicle_info_NORMAL_SPEEDO])
        {
            SendNotification(playerid, "Estï¿½s conduciendo sin licencia de conducciï¿½n, la policï¿½a podrï¿½a multarte.");
        }
        if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_DOORS] && VEHICLE_INFO[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID] - 400][vehicle_info_DRIVER_DOOR]) //cerrado
        {
            if (gettime() - GLOBAL_VEHICLES[vehicleid][gb_vehicle_LAST_CLOSED_TIME] > 5) // Si el tiempo que paso desde que cerro es mayor a 5
            {
                if (ac_Info[CHEAT_PLAYER_VEHICLE_DOORS][ac_Enabled])
                {
                    if (gettime() > PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_IMMUNITY])
                    {
                        if (!ac_Info[CHEAT_PLAYER_VEHICLE_DOORS][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_DOORS);
                        else
                        {
                            if (gettime() - PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_PLAYER_VEHICLE_DOORS][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_DETECTIONS] = 0;
                            else PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_DETECTIONS] ++;

                            PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_LAST_DETECTION] = gettime();
                            if (PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_PLAYER_VEHICLE_DOORS][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_DOORS);
                        }
                    }
                }
            }
        }

        switch (GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE])
        {
            case VEHICLE_TYPE_NONE:
            {
                if (!VEHICLE_INFO[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID] - 400][vehicle_info_ENGINE])
                {
                    GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 1;
                    UpdateVehicleParams(vehicleid);
                }
            }
            case VEHICLE_TYPE_DRIVING_SCHOOL:
            {
                if (PLAYER_TEMP[playerid][pt_DL_EXAM])
                {
                    SendNotification(playerid, "Sigue los puntos de control para completar el examen.");
                    SetPlayerDrivingSchoolCP(playerid);
                }
            }
            case VEHICLE_TYPE_SELL:
            {
                PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] = vehicleid;
                new pvehicles = CountPlayerVehicles(playerid);
                if (pvehicles >= MAX_SU_VEHICLES) return SendFormatNotification(playerid, "No puedes comprar mï¿½s vehï¿½culos, el lï¿½mite es %d.", MAX_SU_VEHICLES);
                if (!ACCOUNT_INFO[playerid][ac_VIP])
                {
                    if (pvehicles >= MAX_NU_VEHICLES)
                    {
                        SendFormatNotification(playerid, "ï¿½Los jugadores ~y~VIP ~w~pueden tener hasta %d vehï¿½culos! Usa ~y~/ayuda ~w~si quieres ser ~y~VIP~w~.", MAX_SU_VEHICLES);
                        return 1;
                    }
                }

                ShowDialog(playerid, DIALOG_BUY_VEHICLE);
                return 1;
            }
            case VEHICLE_TYPE_WORK:
            {
                if (!PLAYER_WORKS[playerid][ WORK_VEHICLES[vehicleid][work_vehicle_WORK] ][pwork_SET])
                {
                    if (ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Enabled])
                    {
                        if (gettime() > PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_IMMUNITY])
                        {
                            if (!ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_WORK);
                            else
                            {
                                if (gettime() - PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] = 0;
                                else PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] ++;

                                PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_LAST_DETECTION] = gettime();
                                if (PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_WORK);
                            }
                        }
                    }
                    return 1;
                }

                if (PLAYER_TEMP[playerid][pt_WORKING_IN] && PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_VEHICLES[vehicleid][work_vehicle_WORK])
                {
                    if (ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Enabled])
                    {
                        if (gettime() > PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_IMMUNITY])
                        {
                            if (!ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_WORK);
                            else
                            {
                                if (gettime() - PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] = 0;
                                else PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] ++;

                                PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_LAST_DETECTION] = gettime();
                                if (PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_WORK);
                            }
                        }
                    }
                    return 1;
                }

                if (WORK_VEHICLES[vehicleid][work_vehicle_NEED_DUTY])
                {
                    if (PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_VEHICLES[vehicleid][work_vehicle_WORK])
                    {
                        if (ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Enabled])
                        {
                            if (gettime() > PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_IMMUNITY])
                            {
                                if (!ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_WORK);
                                else
                                {
                                    if (gettime() - PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] = 0;
                                    else PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] ++;

                                    PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_LAST_DETECTION] = gettime();
                                    if (PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_WORK);
                                }
                            }
                        }
                        return 1;
                    }
                }

                if (WORK_VEHICLES[vehicleid][work_vehicle_EXP] > PLAYER_WORKS[playerid][ WORK_VEHICLES[vehicleid][work_vehicle_WORK] ][pwork_LEVEL])
                {
                    if (ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Enabled])
                    {
                        if (gettime() > PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_IMMUNITY])
                        {
                            if (!ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_WORK);
                            else
                            {
                                if (gettime() - PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] = 0;
                                else PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] ++;

                                PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_LAST_DETECTION] = gettime();
                                if (PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_WORK);
                            }
                        }
                    }
                    return 1;
                }

                switch (WORK_VEHICLES[vehicleid][work_vehicle_WORK])
                {
                    case WORK_TAXI, WORK_HARVESTER, WORK_FUMIGATOR: CallLocalFunction("StartPlayerJob", "iii", playerid, WORK_VEHICLES[vehicleid][work_vehicle_WORK], vehicleid);
                    case WORK_TRUCK, WORK_DELIVERYMAN:
                    {
                        if (TRUCK_VEHICLE[vehicleid][truck_vehicle_LOADED])
                        {
                            if (TRUCK_VEHICLE[vehicleid][truck_vehicle_DRIVER_USER_ID] != ACCOUNT_INFO[playerid][ac_ID])
                            {
                                SendNotification(playerid, "No eres el conductor de este vehï¿½culo.");
                                RemovePlayerFromVehicle(playerid);
                                return 1;
                            }
                            else CallLocalFunction("StartPlayerJob", "iii", playerid, WORK_VEHICLES[vehicleid][work_vehicle_WORK], vehicleid);
                        }
                        else CallLocalFunction("StartPlayerJob", "iii", playerid, WORK_VEHICLES[vehicleid][work_vehicle_WORK], vehicleid);
                    }
                    case WORK_TRASH:
                    {
                        if (TRASH_VEHICLES[vehicleid][trash_vehicle_JOB_STARTED])
                        {
                            if (TRASH_VEHICLES[vehicleid][trash_vehicle_DRIVER_ID] != playerid)
                            {
                                SendNotification(playerid, "No eres el conductor de este camiï¿½n.");
                                RemovePlayerFromVehicle(playerid);
                                return 1;
                            }

                            SetPlayerTrashCheckpoint(playerid, vehicleid);
                        }
                        else
                        {
                            if (PLAYER_TEMP[playerid][pt_TRASH_DRIVER] || PLAYER_TEMP[playerid][pt_TRASH_PASSENGER])
                            {
                                SendNotification(playerid, "Ya estï¿½s trabajando con otro compaï¿½ero.");
                                RemovePlayerFromVehicle(playerid);
                                return 1;
                            }

                            PLAYER_TEMP[playerid][pt_TRASH_DRIVER] = true;
                            PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] = vehicleid;
                            TRASH_VEHICLES[vehicleid][trash_vehicle_DRIVER_ID] = playerid;

                            if (TRASH_VEHICLES[vehicleid][trash_vehicle_PASSENGER_ID] == INVALID_PLAYER_ID) SendNotification(playerid, "Necesitas un compaï¿½ero para empezar con el trabajo.");
                            else
                            {
                                new route = random(MAX_ROUTES);
                                while (!TRASH_OBJECTS[route][trash_route_VALID])
                                {
                                    route = random(MAX_ROUTES);
                                }

                                TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_JOB_STARTED] = true;
                                TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE] = route;
                                TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_CP] = 0;

                                SetPlayerTrashCheckpoint(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID]);
                                SetPlayerTrashCheckpoint(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID]);

                                CreatePlayerTrashRouteObjects(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], route);
                                CreatePlayerTrashRouteObjects(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], route);

                                SendFormatNotification(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], "Ruta ~y~#%d ~w~establecida, pueden comenzar.", TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE] + 1);
                                SendFormatNotification(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], "Ruta ~y~#%d ~w~establecida, pueden comenzar.", TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE] + 1);

                            }
                        }
                    }
                    case WORK_PIZZA:
                    {
                        if (PIZZA_VEHICLE[vehicleid][pizza_vehicle_STARTED])
                        {
                            if (PIZZA_VEHICLE[vehicleid][pizza_vehicle_DRIVER_AID] != ACCOUNT_INFO[playerid][ac_ID])
                            {
                                if (GetVehicleDistanceFromPoint(vehicleid,
                                    GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_X],
                                    GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_Y],
                                    GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_Z]) < 20.0)
                                {
                                    SetVehicleToRespawnEx(vehicleid);
                                }

                                SendNotification(playerid, "No eres el conductor de esta moto.");
                                RemovePlayerFromVehicle(playerid);
                                return 1;
                            }

                            if (!PLAYER_TEMP[playerid][pt_PIZZA_PROCCESS])
                            {
                                SendNotification(playerid, "Ya has repartido todas las pizzas, vuelve a la pizzerï¿½a para cobrar.");
                                SetPlayer_Pizza_Checkpoint(playerid, -1);
                            }
                            else
                            {
                                SendFormatNotification(playerid, "Te quedan %d pizzas por repartir.", PLAYER_TEMP[playerid][pt_PIZZA_PROCCESS]);
                                if (PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZA_POINT_DELI])
                                {
                                    PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZA_POINT] = randomEx(sizeof(PIZZA_DELIVERY_POINT), PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZA_POINT]);
                                    PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZA_POINT_DELI] = false;
                                }
                                SetPlayer_Pizza_Checkpoint(playerid, PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZA_POINT]);
                            }
                        }
                        else
                        {
                            PIZZA_VEHICLE[vehicleid][pizza_vehicle_STARTED] = true;
                            PIZZA_VEHICLE[vehicleid][pizza_vehicle_DRIVER_AID] = ACCOUNT_INFO[playerid][ac_ID];
                            PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZAS] = minrand(3, 6);
                            PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZA_POINT] = random( sizeof(PIZZA_DELIVERY_POINT) );
                            PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZA_POINT_DELI] = false;
                            PLAYER_TEMP[playerid][pt_PIZZA_PROCCESS] = PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZAS];
                            PLAYER_TEMP[playerid][pt_DELIVERED_PIZZAS] = 0;

                            SendFormatNotification(playerid, "Esta moto tiene %d pizzas, repartelas todas y vuelve a la pizzerï¿½a para cobrar.", PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZAS]);
                            SetPlayer_Pizza_Checkpoint(playerid, PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZA_POINT]);
                        }
                    }
                }
            }
        }

        if (PLAYER_TEMP[playerid][pt_WANT_TAXI])
        {
            if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_STATE] == VEHICLE_STATE_NORMAL)
            {
                PLAYER_TEMP[playerid][pt_WANT_TAXI] = false;
                DisablePlayerTaxiMark(playerid);

                SendNotification(playerid, "Tu taxi se ha cancelado porque te has subido a un vehï¿½culo.");
            }
        }
    }
    else if (newstate == PLAYER_STATE_PASSENGER)
    {
        SetPlayerArmedWeapon(playerid, 0);
        PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_IMMUNITY] = gettime() + 3;

        new vehicleid = GetPlayerVehicleID(playerid);
        PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] = vehicleid;

        if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_DOORS] && VEHICLE_INFO[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID] - 400][vehicle_info_PASSENGER_DOOR]) //cerrado
        {
            if (gettime() - GLOBAL_VEHICLES[vehicleid][gb_vehicle_LAST_CLOSED_TIME] > 5) // Si el tiempo que paso desde que cerro es mayor a 5
            {
                if (ac_Info[CHEAT_PLAYER_VEHICLE_DOORS][ac_Enabled])
                {
                    if (gettime() > PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_IMMUNITY])
                    {
                        if (!ac_Info[CHEAT_PLAYER_VEHICLE_DOORS][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_DOORS);
                        else
                        {
                            if (gettime() - PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_PLAYER_VEHICLE_DOORS][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_DETECTIONS] = 0;
                            else PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_DETECTIONS] ++;

                            PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_LAST_DETECTION] = gettime();
                            if (PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_PLAYER_VEHICLE_DOORS][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_DOORS);
                        }
                    }
                }
            }
        }

        if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE] == VEHICLE_TYPE_WORK)
        {
            switch (WORK_VEHICLES[vehicleid][work_vehicle_WORK])
            {
                case WORK_TAXI:
                {
                    if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] == INVALID_PLAYER_ID) return 1;
                    if (TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_ENABLED])
                    {
                        if (PLAYER_TEMP[playerid][pt_WANT_TAXI])
                        {
                            PLAYER_TEMP[playerid][pt_WANT_TAXI] = false;
                            DisablePlayerTaxiMark(playerid);
                        }
                        return 1;
                    }

                    ShowPlayerTaxiMeter(playerid);
                    if (PLAYER_TEMP[playerid][pt_WANT_TAXI])
                    {
                        PLAYER_TEMP[playerid][pt_WANT_TAXI] = false;
                        DisablePlayerTaxiMark(playerid);
                    }

                    TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_ENABLED] = true;
                    GetVehiclePos(vehicleid,
                        TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_OLD_X],
                        TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_OLD_Y],
                        TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_OLD_Z]);
                    KillTimer(TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_TIMER]);
                    TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_TIMER] = SetTimerEx("UpdateVehicleTaximeter", 4000, true, "iii", vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER], playerid);
                }
                case WORK_TRASH:
                {
                    if (!PLAYER_WORKS[playerid][WORK_TRASH][pwork_SET])
                    {
                        SendFormatNotification(playerid, "Necesitas ser %s para poder conducir este vehï¿½culo.", work_info[ WORK_VEHICLES[vehicleid][work_vehicle_WORK] ][work_info_NAME]);
                        RemovePlayerFromVehicle(playerid);
                        return 1;
                    }

                    if (PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_TRASH)
                    {
                        SendNotification(playerid, "No estï¿½s de servicio como basurero, ve a vestirte en el puesto de la entrada del vertedero.");
                        RemovePlayerFromVehicle(playerid);
                        return 1;
                    }

                    if (TRASH_VEHICLES[vehicleid][trash_vehicle_JOB_STARTED])
                    {
                        if (TRASH_VEHICLES[vehicleid][trash_vehicle_PASSENGER_ID] != playerid)
                        {
                            SendNotification(playerid, "No eres el acompaï¿½ante de este camiï¿½n.");
                            RemovePlayerFromVehicle(playerid);
                            return 1;
                        }

                        SetPlayerTrashCheckpoint(playerid, vehicleid);
                    }
                    else
                    {
                        if (PLAYER_TEMP[playerid][pt_TRASH_DRIVER] || PLAYER_TEMP[playerid][pt_TRASH_PASSENGER])
                        {
                            SendNotification(playerid, "Ya estï¿½s trabajando con otro compaï¿½ero.");
                            RemovePlayerFromVehicle(playerid);
                            return 1;
                        }

                        PLAYER_TEMP[playerid][pt_TRASH_PASSENGER] = true;
                        PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] = vehicleid;
                        TRASH_VEHICLES[vehicleid][trash_vehicle_PASSENGER_ID] = playerid;

                        if (TRASH_VEHICLES[vehicleid][trash_vehicle_DRIVER_ID] == INVALID_PLAYER_ID) SendNotification(playerid, "Necesitas un compaï¿½ero para empezar con el trabajo.");
                        else
                        {
                            new route = random(MAX_ROUTES);
                            while (!TRASH_OBJECTS[route][trash_route_VALID])
                            {
                                route = random(MAX_ROUTES);
                            }

                            TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_JOB_STARTED] = true;
                            TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE] = route;
                            TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_CP] = 0;

                            SetPlayerTrashCheckpoint(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID]);
                            SetPlayerTrashCheckpoint(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID]);

                            CreatePlayerTrashRouteObjects(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], route);
                            CreatePlayerTrashRouteObjects(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], route);

                            SendFormatNotification(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], "Ruta ~y~#%d ~w~establecida, pueden comenzar.", TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE] + 1);
                            SendFormatNotification(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], "Ruta ~y~#%d ~w~establecida, pueden comenzar.", TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE] + 1);

                        }
                    }
                }
            }
        }

        if (PLAYER_TEMP[playerid][pt_WANT_TAXI])
        {
            if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_STATE] == VEHICLE_STATE_NORMAL)
            {
                PLAYER_TEMP[playerid][pt_WANT_TAXI] = false;
                DisablePlayerTaxiMark(playerid);

                SendNotification(playerid, "Tu taxi se ha cancelado porque te has subido a un vehï¿½culo.");
            }
        }
    }
    else if (newstate == PLAYER_STATE_SPECTATING)
    {
        if (!PLAYER_TEMP[playerid][pt_PLAYER_SPECTATE])
        {
            if (ac_Info[CHEAT_SPECTATE][ac_Enabled])
            {
                if (gettime() > PLAYER_AC_INFO[playerid][CHEAT_SPECTATE][p_ac_info_IMMUNITY])
                {
                    if (!ac_Info[CHEAT_SPECTATE][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_SPECTATE);
                    else
                    {
                        if (gettime() - PLAYER_AC_INFO[playerid][CHEAT_SPECTATE][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_SPECTATE][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_SPECTATE][p_ac_info_DETECTIONS] = 0;
                        else PLAYER_AC_INFO[playerid][CHEAT_SPECTATE][p_ac_info_DETECTIONS] ++;

                        PLAYER_AC_INFO[playerid][CHEAT_SPECTATE][p_ac_info_LAST_DETECTION] = gettime();
                        if (PLAYER_AC_INFO[playerid][CHEAT_SPECTATE][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_SPECTATE][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_SPECTATE);
                    }
                }
            }
        }
    }

    if (oldstate == PLAYER_STATE_DRIVER)
    {
        PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_IMMUNITY] = gettime() + 3;
        if (PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] != INVALID_VEHICLE_ID)
        {
            KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][7]);

            if (IsValidDynamicRaceCP(PLAYER_TEMP[playerid][pt_DL_EXAM_CP]))
            {
                DestroyDynamicRaceCP(PLAYER_TEMP[playerid][pt_DL_EXAM_CP]);
                PLAYER_TEMP[playerid][pt_DL_EXAM_CP] = INVALID_STREAMER_ID;
            }

            if (GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_ATTACHED_TO] != INVALID_VEHICLE_ID)
            {
                if (GLOBAL_VEHICLES[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_ATTACHED_TO] ][gb_vehicle_DRIVER] != INVALID_PLAYER_ID)
                {
                    PLAYER_AC_INFO[ GLOBAL_VEHICLES[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_ATTACHED_TO] ][gb_vehicle_DRIVER]     ][CHEAT_FAKE_TRAILER_SYNC][p_ac_info_IMMUNITY] = gettime() + 5;
                }
                GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_ATTACHED_TO] = INVALID_VEHICLE_ID;
                DetachTrailerFromVehicle(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID]);
            }

            GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_TP_IMMUNITY] = gettime() + 5;
            GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_DRIVER] = INVALID_PLAYER_ID;
            GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_LAST_DRIVER] = playerid;
            GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_OCCUPIED] = false;

            StopAudioStreamForPlayer(playerid);

            if (GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_TYPE] == VEHICLE_TYPE_WORK)
            {
                switch (WORK_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][work_vehicle_WORK])
                {
                    case WORK_TAXI, WORK_HARVESTER, WORK_FUMIGATOR, WORK_TRUCK: CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);
                    case WORK_PIZZA:
                    {
                        if (PIZZA_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][pizza_vehicle_STARTED])
                        {
                            if (PLAYER_TEMP[playerid][pt_PIZZA_PROCCESS])
                            {
                                new Float:distance =    GetPlayerDistanceFromPoint
                                                                (
                                                                    playerid,
                                                                    PIZZA_DELIVERY_POINT[ PIZZA_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][pizza_vehicle_PIZZA_POINT] ][0],
                                                                    PIZZA_DELIVERY_POINT[ PIZZA_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][pizza_vehicle_PIZZA_POINT] ][1],
                                                                    PIZZA_DELIVERY_POINT[ PIZZA_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][pizza_vehicle_PIZZA_POINT] ][2]
                                                                );

                                if (distance > 20.0)
                                {
                                    SendNotification(playerid, "Vuelve a subir a la moto, aï¿½n estï¿½s muy lejos del cliente.");
                                    if (IsValidDynamicActor(PLAYER_TEMP[playerid][pt_PIZZA_ACTOR]))
                                    {
                                        DestroyDynamicActor(PLAYER_TEMP[playerid][pt_PIZZA_ACTOR]);
                                        PLAYER_TEMP[playerid][pt_PIZZA_ACTOR] = INVALID_STREAMER_ID;
                                    }
                                    if (IsValidDynamicCP(PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT]))
                                    {
                                        DestroyDynamicCP(PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT]);
                                        PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT] = INVALID_STREAMER_ID;
                                    }
                                }
                                else
                                {
                                    if (IsPlayerInDynamicCP(playerid, PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT]))
                                    {
                                        PLAYER_TEMP[playerid][pt_PIZZA_PROCCESS] --;
                                        PLAYER_TEMP[playerid][pt_DELIVERED_PIZZAS] ++;
                                        RemovePlayerAttachedObject(playerid, 9);
                                        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
                                        ApplyDynamicActorAnimation(PLAYER_TEMP[playerid][pt_PIZZA_ACTOR], "FOOD", "EAT_Pizza", 4.1, false, true, true, false, 0);
                                        ApplyDynamicActorAnimation(PLAYER_TEMP[playerid][pt_PIZZA_ACTOR], "FOOD", "EAT_Pizza", 4.1, false, true, true, false, 0);
                                        DestroyDynamicCP(PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT]);
                                        PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT] = INVALID_STREAMER_ID;
                                        PIZZA_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][pizza_vehicle_PIZZA_POINT_DELI] = true;
                                        SendNotification(playerid, "Genial, vuelve a subir a la moto para continuar.");
                                    }
                                    else
                                    {
                                        SetPlayerArmedWeapon(playerid, 0);
                                        SetPlayerAttachedObject(playerid,
                                            9,
                                            1582,
                                            6,
                                            0.094,
                                            0.077999,
                                            -0.165999,
                                            72.0,
                                            8.799999,
                                            9.899992,
                                            0.73,
                                            0.774999,
                                            1.0);
                                        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
                                    }
                                }
                            }
                            else
                            {
                                if (IsValidDynamicActor(PLAYER_TEMP[playerid][pt_PIZZA_ACTOR]))
                                {
                                    DestroyDynamicActor(PLAYER_TEMP[playerid][pt_PIZZA_ACTOR]);
                                    PLAYER_TEMP[playerid][pt_PIZZA_ACTOR] = INVALID_STREAMER_ID;
                                }
                                if (IsValidDynamicCP(PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT]))
                                {
                                    DestroyDynamicCP(PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT]);
                                    PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT] = INVALID_STREAMER_ID;
                                }
                                SendNotification(playerid, "Vuelve a subir a la moto y ve a la pizzerï¿½a para cobrar.");
                            }
                        }
                    }
                }
            }
            if (PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] == PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID])
            {
                if (PLAYER_TEMP[playerid][pt_TRASH_DRIVER])
                {
                    if (TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_JOB_STARTED])
                    {
                        if (TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID] == playerid) SendNotification(playerid, "Si quieres dejar de trabajar ve al vertedero y ponte tu ropa.");
                    }
                    else
                    {
                        TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID] = INVALID_PLAYER_ID;
                        PLAYER_TEMP[playerid][pt_TRASH_DRIVER] = false;
                        PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] = INVALID_VEHICLE_ID;
                    }
                }
            }
        }
    }
    else if (oldstate == PLAYER_STATE_PASSENGER)
    {
        if (PLAYER_TEMP[playerid][pt_TAXI_METER_ENABLED])
        {
            new driver = GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_DRIVER];

            if (driver != INVALID_PLAYER_ID)
            {
                new work_extra_payment;
                if (work_info[WORK_TAXI][work_info_EXTRA_PAY] > 0 && work_info[WORK_TAXI][work_info_EXTRA_PAY_EXP] > 0)
                {
                    work_extra_payment = (work_info[WORK_TAXI][work_info_EXTRA_PAY] * floatround(floatdiv(PLAYER_WORKS[ driver ][WORK_TAXI][pwork_LEVEL], work_info[WORK_TAXI][work_info_EXTRA_PAY_EXP])));
                    if (work_info[WORK_TAXI][work_info_EXTRA_PAY_LIMIT] != 0) if (work_extra_payment > work_info[WORK_TAXI][work_info_EXTRA_PAY_LIMIT]) work_extra_payment = work_info[WORK_TAXI][work_info_EXTRA_PAY_LIMIT];

                    if (ACCOUNT_INFO[playerid][ac_VIP]) work_extra_payment += SU_WORK_EXTRA_PAY;
                }
                PLAYER_WORKS[driver][WORK_TAXI][pwork_LEVEL] ++;

                SendFormatNotification(driver, "Has ganado ~g~%s dï¿½lares~w~ con este viaje.", number_format_thousand(TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_PAYMENT] + work_extra_payment));
                GivePlayerCash(driver,
                    TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_PAYMENT] + work_extra_payment,
                    true,
                    false);
                GivePlayerCash(playerid,
                    -TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_PAYMENT],
                    true,
                    true);

                SendFormatNotification(playerid, "El taxi te costï¿½ %s$.", number_format_thousand(TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_PAYMENT]));
                ResetVehicleTaxiMeter(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID]);
                HidePlayerTaxiMeter(playerid);
                UpdatePlayerTaxiMeterTextdraws(driver);
            }

            new passenger = GetVehicleFirstPassenger( PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] );
            if (passenger != INVALID_PLAYER_ID)
            {
                ShowPlayerTaxiMeter(passenger);
                if (PLAYER_TEMP[passenger][pt_WANT_TAXI])
                {
                    PLAYER_TEMP[passenger][pt_WANT_TAXI] = false;
                    DisablePlayerTaxiMark(passenger);
                }

                TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_ENABLED] = true;
                GetVehiclePos(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID],
                    TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_OLD_X],
                    TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_OLD_Y],
                    TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_OLD_Z]);
                KillTimer(TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_TIMER]);
                TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_TIMER] = SetTimerEx("UpdateVehicleTaximeter", 4000, true, "iii", PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID], GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_DRIVER], passenger);
            }
        }
        if (PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] == PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID])
        {
            if (PLAYER_TEMP[playerid][pt_TRASH_PASSENGER])
            {
                if (TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_JOB_STARTED])
                {
                    if (TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID] == playerid)
                    {
                        if (TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_CP] >= TRASH_OBJECTS[ TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE] ][trash_route_OBJECTS]) SendNotification(playerid, "Sube al camiï¿½n para volver al vertedero y recibir la paga.");
                        else
                        {
                            new cp = TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_CP];
                            new route = TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE];
                            new Float:distance =    GetPlayerDistanceFromPoint
                                                    (
                                                        playerid,
                                                        TRASH_OBJECTS[ route ][trash_object_X][ cp ],
                                                        TRASH_OBJECTS[ route ][trash_object_Y][ cp ],
                                                        TRASH_OBJECTS[ route ][trash_object_Z][ cp ]
                                                    );

                            if (distance > 10.0) SendNotification(playerid, "Vuelve a subir al camiï¿½n, aï¿½n estï¿½s demasiado lejos del punto de recogida.");
                            else
                            {
                                if (IsPlayerInDynamicCP(playerid, PLAYER_TEMP[playerid][pt_TRASH_CHECKPOINT]))
                                {
                                    if (!PLAYER_WORKS[playerid][WORK_TRASH][pwork_SET]) return DestroyPlayerTrashCheckpoint(playerid);
                                    if (PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_TRASH) return DestroyPlayerTrashCheckpoint(playerid);
                                    if (!PLAYER_TEMP[playerid][pt_TRASH_PASSENGER]) return 1;

                                    new driver = TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID];
                                    SendClientMessage(driver, -1, "{"#SILVER_COLOR"}Espera mientras tu compaï¿½ero recoge la basura.");

                                    new Float:offset = floatdiv(PLAYER_WORKS[playerid][WORK_TRASH][pwork_LEVEL], 10);
                                    if (offset > 30.0) offset = 30.0;

                                    PLAYER_TEMP[playerid][pt_RECYCLE_BIN_VALUE] = 10.0 + offset;
                                    UpdatePlayerTrashRecycleSize(playerid);
                                    PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][0]);
                                    PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][1]);
                                    PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][2]);

                                    TogglePlayerControllableEx(playerid, false);
                                    SetCameraBehindPlayer(playerid);
                                    ApplyAnimation(playerid, "ROB_BANK", "CAT_Safe_Rob", 4.1, true, false, false, false, 0);

                                    KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][10]);
                                    PLAYER_TEMP[playerid][pt_TIMERS][10] = SetTimerEx("RecycleUp", 1000, false, "i", playerid);
                                    return 1;
                                }
                                SendNotification(playerid, "Recoge la basura y vuelve al camiï¿½n para continuar.");
                            }
                        }
                    }
                }
                else
                {
                    TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID] = INVALID_PLAYER_ID;
                    PLAYER_TEMP[playerid][pt_TRASH_PASSENGER] = false;
                    PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] = INVALID_VEHICLE_ID;
                }
            }
        }
    }
    return 1;
}

public OnPlayerEditObject(playerid, playerobject, objectid, response, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ)
{
    if (playerobject)
    {
        if (objectid == PLAYER_TEMP[playerid][pt_PIVOT_OBJECT])
        {
            switch (response)
            {
                case EDIT_RESPONSE_CANCEL:
                {
                    if (PLAYER_TEMP[playerid][pt_PIVOT_OBJECT] != INVALID_OBJECT_ID)
                    {
                        DestroyPlayerObject(playerid, PLAYER_TEMP[playerid][pt_PIVOT_OBJECT]);
                        PLAYER_TEMP[playerid][pt_PIVOT_OBJECT] = INVALID_OBJECT_ID;
                    }
                    VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][0] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][0];
                    VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][1] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][1];
                    VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][2] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][2];
                    VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][0] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][3];
                    VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][1] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][4];
                    VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][2] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][5];
                    UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT]);
                    Streamer_Update(playerid);

                    SendNotification(playerid, "Has cancelado la ediciï¿½n.");
                    ShowDialog(playerid, DIALOG_VOBJECT_OBJECT);
                }
                case EDIT_RESPONSE_FINAL:
                {
                    if (PLAYER_TEMP[playerid][pt_PIVOT_OBJECT] != INVALID_OBJECT_ID)
                    {
                        DestroyPlayerObject(playerid, PLAYER_TEMP[playerid][pt_PIVOT_OBJECT]);
                        PLAYER_TEMP[playerid][pt_PIVOT_OBJECT] = INVALID_OBJECT_ID;
                    }

                    new Float:v_size[3];
                    GetVehicleModelInfo(GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][gb_vehicle_MODELID],
                        VEHICLE_MODEL_INFO_SIZE,
                        v_size[0],
                        v_size[1],
                        v_size[2]);

                    if (
                        (VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][0] >= v_size[0] || -v_size[0] >= VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][0]) ||
                        (VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][1] >= v_size[1] || -v_size[1] >= VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][1]) ||
                        (VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][2] >= v_size[2] || -v_size[2] >= VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][2])
                    )
                    {
                        SendNotification(playerid, "La posiciï¿½n del objeto estï¿½ demasiado lejos del vehï¿½culo.");
                        VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][0] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][0];
                        VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][1] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][1];
                        VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][2] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][2];
                        VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][0] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][3];
                        VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][1] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][4];
                        VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][2] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][5];
                        UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT]);
                        Streamer_Update(playerid);
                    }

                    VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ATTACHED] = true;
                    SendNotification(playerid, "Objeto actualizado.");
                    ShowDialog(playerid, DIALOG_VOBJECT_OBJECT);
                }
                case EDIT_RESPONSE_UPDATE:
                {
                    VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][0] = fX - GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][gb_vehicle_POS][0];
                    VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][1] = fY - GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][gb_vehicle_POS][1];
                    VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][2] = fZ - GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][gb_vehicle_POS][2];
                    VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][0] = fRotX;
                    VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][1] = fRotY;
                    VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][2] = fRotZ;
                    UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT]);
                }
            }
        }
    }
    return 1;
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if (clickedid == Text:INVALID_TEXT_DRAW)
    {
        PLAYER_TEMP[playerid][pt_SELECT_TEXTDRAW] = false;

        if (PLAYER_TEMP[playerid][pt_READING_GUIDE])
        {
            SendNotification(playerid, "Antes de continuar debes leer el tutorial de conceptos.");
            SelectTextDrawEx(playerid, 0xe8d08fFF);
            return 1;
        }

        if (PLAYER_TEMP[playerid][pt_REGISTER_SELECTING_SKIN])
        {
            SendNotification(playerid, "Antes de continuar debes elegir tu sexo y tu ropa.");
            SelectTextDrawEx(playerid, 0xe8d08fFF);
            return 1;
        }

        if (PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_ENABLED])
        {
            ExitPlayerPropertyConstructor(playerid);
            return 1;
        }

        if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP] != -1) // Tiene abierto el menú de ropa
        {
            ClosePlayerClothingMenu(playerid);
            ExitPlayerWardrobe(playerid);
        }

        if (PLAYER_TEMP[playerid][pt_TOYS_SHOP]) ClosePlayerToysMenu(playerid);

        if (PLAYER_TEMP[playerid][pt_SHOP]) HidePlayerShop(playerid);

        if (PLAYER_TEMP[playerid][pt_TUNING_GARAGE_SHOP])
        {
            CancelPlayerTuningShop(playerid);
            ShowDialog(playerid, DIALOG_VOBJECT_MENU);
        }
        return 1;
    }

    if (
        PLAYER_TEMP[playerid][pt_SHOP] &&
        (PLAYER_TEMP[playerid][pt_SHOP_STATE] == PLAYER_SHOP_STATE_ALL ||
        PLAYER_TEMP[playerid][pt_SHOP_STATE] == PLAYER_SHOP_STATE_MY_ADS))
    {
        for (new i = 0; i != 6; i++)
        {
            if (clickedid == Textdraws[textdraw_SHOP_ARTICLES_BOX][i])
            {
                if (!PLAYER_TEMP[playerid][pt_SHOP_ARTICLE_ID][i]) return SendNotification(playerid, "Este anuncio ya no está disponible.");

                inline OnShopChecked()
                {
                    new rows;
                    if (cache_get_row_count(rows))
                    {
                        if (rows)
                        {
                            PLAYER_TEMP[playerid][pt_SHOP_SELECTED_ARTICLE_ID] = PLAYER_TEMP[playerid][pt_SHOP_ARTICLE_ID][i];
                            PLAYER_TEMP[playerid][pt_SHOP_STATE] = PLAYER_SHOP_STATE_AD;
                            UpdatePlayerShop(playerid);
                        }
                        else SendNotification(playerid, "Este anuncio ya no está disponible.");
                    }
                    else SendNotification(playerid, "Este anuncio ya no está disponible.");
                }
                mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id FROM shop WHERE id = %d;", PLAYER_TEMP[playerid][pt_SHOP_ARTICLE_ID][i]);
                MySQL_TQueryInline(srp_db,  using inline OnShopChecked, QUERY_BUFFER);
                break;
            }
        }
        return 1;
    }

    if (PLAYER_TEMP[playerid][pt_TUNING_GARAGE_SHOP])
    {
        if (clickedid == Textdraws[textdraw_SHOP_TUNING][2]) //Comprar
        {
            new slot = GetVehicleFreeObjectSlot(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID]);
            if (!ACCOUNT_INFO[playerid][ac_VIP] && slot >= MAX_NU_VOBJECTS)
            {
                PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
                SendFormatNotification(playerid, "¡Los jugadores ~y~VIP ~w~pueden tener hasta %d objetos en vehículos! Usa ~y~/ayuda ~w~si quieres ser ~y~VIP.", MAX_SU_VOBJECTS);
                return 1;
            }
            if (slot == -1)
            {
                SendNotification(playerid, "No hay más espacio para objetos en este vehículo.");
                return 1;
            }

            if (Tuning_Shop_Objects[ PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] ][tuning_object_PRICE] > PI[playerid][ac_CASH])
            {
                PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
                SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar este objeto.", number_format_thousand(Tuning_Shop_Objects[ PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] ][tuning_object_PRICE] - PI[playerid][ac_CASH]));
                return 1;
            }

            if (GivePlayerCash(playerid,
                -Tuning_Shop_Objects[ PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] ][tuning_object_PRICE],
                true,
                true))
            {
                VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_VALID] = true;
                VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_TYPE] = VOBJECT_TYPE_OBJECT;
                VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_ID] = 0;
                VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_MODELID] = Tuning_Shop_Objects[ PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] ][tuning_object_MODELID];
                format(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_NAME], 32, "%s", Tuning_Shop_Objects[ PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] ][tuning_object_NAME]);
                VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_ATTACHED] = true;
                UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], slot);

                RegisterNewVehicleObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], slot);
                CancelPlayerTuningShop(playerid);

                SendNotification(playerid, "Ajusta el objeto y haz clic en el icono de guardar, si pulsas ~r~'ESCAPE' ~w~no se guardará la configuración.");
                SendNotification(playerid, "Mantén presionado ~r~'W' ~w~para mover la cámara.");
                EditVehicleObject(playerid, PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], slot);
            }
            return 1;
        }
        else if (clickedid == Textdraws[textdraw_SHOP_TUNING][3]) //Siguiente
        {
            if (PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] == (sizeof(Tuning_Shop_Objects) - 1)) PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] = 0;
            else PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] ++;
            UpdateTuningShop(playerid);
            return 1;
        }
        else if (clickedid == Textdraws[textdraw_SHOP_TUNING][4]) //Anterior
        {
            if (PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] == 0) PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] = sizeof(Tuning_Shop_Objects) - 1;
            else PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] --;
            UpdateTuningShop(playerid);
            return 1;
        }
        return 1;
    }

    return 1;
}
