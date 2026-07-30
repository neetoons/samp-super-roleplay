PlayerPayday(playerid)
{
    new
        dialog_body[80],
        dialog_header[630],
        dialog_string[140],
        date[24],
        money = 300 * ACCOUNT_INFO[playerid][ac_LEVEL];

    if (money > 4500) money = 4500;
    money += minrand(100, 200);

    getDateTime(date);

    format(dialog_body, sizeof dialog_body, "{ABD9C6}Pago diario (%s)", date);

    strcat(dialog_header, "\n{FF6746}____ [ PAGA DEL GOBIERNO ] ____\n");

    format(dialog_string, sizeof dialog_string, "{FFFFFF}Paga del gobierno: {FFFF7F}%s dólares\n", number_format_thousand(money));
    strcat(dialog_header, dialog_string);

    if (!ACCOUNT_INFO[playerid][ac_VIP])
    {
        strcat(dialog_header, "\n\n{FF6746}____ [ VEHÍCULOS ] ____\n");

        for (new i = 0, j = GetVehiclePoolSize(); i <= j; i++)
        {
            if (!PLAYER_VEHICLES[i][player_vehicle_VALID]) continue;
            if (PLAYER_VEHICLES[i][player_vehicle_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID]) continue;

            new veh_money = VEHICLE_INFO[GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400][vehicle_info_PRICE] / 100;
            money -= veh_money;

            format(dialog_string, sizeof dialog_string, "{FFFFFF}Seguro del vehículo %s: {FF6746}-%s dólares\n", VEHICLE_INFO[GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400][vehicle_info_NAME], number_format_thousand(veh_money));
            strcat(dialog_header, dialog_string);
        }

        strcat(dialog_header, "\n\n{FF6746}____ [ PROPIEDADES ] ____\n");

        for (new i = 0; i != MAX_PROPERTIES; i++)
        {
            if (!PROPERTY_INFO[i][property_VALID]) continue;
            if (PROPERTY_INFO[i][property_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID]) continue;

            if (PROPERTY_INFO[i][property_VIP_LEVEL]) format(dialog_string, sizeof dialog_string, "{FFFFFF}Seguro de la propiedad %s: {FFFF7F}PROPIEDAD VIP, NO SE PAGA SEGURO.\n", PROPERTY_INFO[i][property_NAME]);
            else
            {
                new property_money = PROPERTY_INFO[i][property_PRICE_BASE] / 200;
                money -= property_money;

                format(dialog_string, sizeof dialog_string, "{FFFFFF}Seguro de la propiedad %s: {FF6746}-%s dólares\n", PROPERTY_INFO[i][property_NAME], number_format_thousand(property_money));
            }
            strcat(dialog_header, dialog_string);
        }
    }

    if (PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET])
    {
        new work_payment;
        if (work_info[WORK_POLICE][work_info_EXTRA_PAY] > 0 && work_info[WORK_POLICE][work_info_EXTRA_PAY_EXP] > 0)
        {
            work_payment = (work_info[WORK_POLICE][work_info_EXTRA_PAY] * floatround(floatdiv(PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL], work_info[WORK_POLICE][work_info_EXTRA_PAY_EXP])));
            if (work_info[WORK_POLICE][work_info_EXTRA_PAY_LIMIT] != 0) if (work_payment > work_info[WORK_POLICE][work_info_EXTRA_PAY_LIMIT]) work_payment = work_info[WORK_POLICE][work_info_EXTRA_PAY_LIMIT];
        }

        money += work_payment;

        strcat(dialog_header, "\n\n{FF6746}____ [ TRABAJOS ] ____\n");

        format(dialog_string, sizeof dialog_string, "{FFFFFF}Paga por ser policía: {FFFF7F}%s dólares\n", number_format_thousand(work_payment));
        strcat(dialog_header, dialog_string);
    }

    if (PI[playerid][ac_CREW])
    {
        new
            territories = GetCrewTerritories(PI[playerid][ac_CREW]),
            graffitis = CountCrewGraffitis(PI[playerid][ac_CREW]);
        if (territories > 0 || graffitis > 0)
        if (territories > 0)
        {
            money += territories * 70;
            money += graffitis * 40;

            strcat(dialog_header, "\n\n{FF6746}____ [ TERRITORIOS ] ____\n");

            if (territories > 0) format(dialog_string, sizeof dialog_string, "{FFFFFF}Paga por territorios en posesión (%d): {FFFF7F}%d dólares\n", territories, territories * 70);
            if (graffitis > 0) format(dialog_string, sizeof dialog_string, "{FFFFFF}Paga por graffitis (%d): {FFFF7F}%d dólares\n", graffitis, graffitis * 40);
            strcat(dialog_header, dialog_string);
        }
    }

    if (money + PI[playerid][ac_CASH] <= 0)
    {
        money = 0;
        PI[playerid][ac_CASH] = 0;
    }

    strcat(dialog_header, "\n\n{FF6746}____ [ RESUMEN ] ____\n");

    if (money > 0) format(dialog_string, sizeof dialog_string, "{FFFFFF}Has ganado: {FFFF7F}%s dólares", number_format_thousand(money));
    else format(dialog_string, sizeof dialog_string, "{FFFFFF}Has perdido: {FF6746}%s dólares", number_format_thousand(money));

    strcat(dialog_header, dialog_string);

    ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_MSGBOX, dialog_body, dialog_header, "Aceptar", "");

    GivePlayerCash(playerid, money, true, money < 0 ? true : false);
    return 1;
}
