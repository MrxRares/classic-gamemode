YCMD:animlist(playerid, params[], help)
{
    SendClientMessage(playerid,COLOR_GROVE, "Anim List");
    SendClientMessage(playerid, -1, "Lifejump : Exhaust : Carlock : Rcarjack1 : Lcarjack1 : Rcarjack2 : Lcarjack2 : Hoodfrisked");
    SendClientMessage(playerid, -1, "Lightcig : Tapcig : Bat : Lean : Clearanim : Dancing : Box : Lowthrow : Highthrow");
    SendClientMessage(playerid, -1, "Leftslap : Handsup : Fall : Fallback : Sup : Rap : Push : Akick : Lowbodypush");
    SendClientMessage(playerid, -1, "Spray : Headbutt : Pee : Koface : Kostomach : Kiss : Rollfall : Lay2 : Hitch");
    SendClientMessage(playerid, -1, "Beach : Medic : Scratch : Sit : Drunk : Bomb : Getarrested : Laugh : Lookout");
    SendClientMessage(playerid, -1, "Aim : Crossarms : Lay : Hide : Vomit : Eating : Wave : Shouting : Chant");
    SendClientMessage(playerid, -1, "Frisked : Exhausted : Injured : Slapass : Deal : Dealstance : Crack : Wank : Gro");
    SendClientMessage(playerid, -1, "Sit : Chat : Fucku : Taichi : Knife : Basket : JumpWater");
    return true;
}

YCMD:carhand(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    ApplyAnimation(playerid, "CAR", "Tap_hand",4.1,0,1,1 ,1,1);
    return true;
}

YCMD:lifejump(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "PED", "EV_dive",4.0,0,1,1,1,0);
    return true;
}
YCMD:exhaust(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "PED", "IDLE_tired",3.0,1,0,0,0,0);
    return true;
}
YCMD:rcarjack1(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "PED", "CAR_pulloutL_LHS",4.0,0,0,0,0,0);
    return true;
}
YCMD:lcarjack1(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "PED", "CAR_pulloutL_RHS",4.0,0,0,0,0,0);
    return true;
}
YCMD:rcarjack2(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "PED", "CAR_pullout_LHS",4.0,0,0,0,0,0);
    return true;
}
YCMD:lcarjack2(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "PED", "CAR_pullout_RHS",4.0,0,0,0,0,0);
    return true;
}
YCMD:hoodfrisked(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "POLICE", "crm_drgbst_01",4.0,0,1,1,1,0);
    return true;
}
YCMD:lightcig(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "SMOKING", "M_smk_in",3.0,0,0,0,0,0);
    return true;
}
YCMD:tapcig(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "SMOKING", "M_smk_tap",3.0,0,0,0,0,0);
    return true;
}
YCMD:bat(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "BASEBALL", "Bat_IDLE",4.0,1,1,1,1,0);
    return true;
}
YCMD:lean(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    new test;
    if(sscanf(params, "d", test)) return sendSyntaxMessage(playerid, "/lean <1-2>");
    switch (test)
    {
        case 1: LoopingAnim(playerid, "GANGS", "leanIDLE",4.0,0,1,1,1,0);
        case 2: LoopingAnim(playerid, "MISC", "Plyrlean_loop",4.0,0,1,1,1,0);
        default: sendSyntaxMessage(playerid, "/lean <1-2>");
    }
    return true;
}
YCMD:clearanim(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    ApplyAnimation(playerid, "CARRY", "crry_prtial", 1.0, 0, 0, 0, 0, 0);
    return true;
}
YCMD:dancing(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    new test;
    if(sscanf(params, "d", test)) return sendSyntaxMessage(playerid, "/dancing <1-7>");
    switch (test)
    {
        case 1: LoopingAnim(playerid, "STRIP", "strip_A", 4.1, 1, 1, 1, 1, 1 );
        case 2: LoopingAnim(playerid, "STRIP", "strip_B", 4.1, 1, 1, 1, 1, 1 );
        case 3: LoopingAnim(playerid, "STRIP", "strip_C", 4.1, 1, 1, 1, 1, 1 );
        case 4: LoopingAnim(playerid, "STRIP", "strip_D", 4.1, 1, 1, 1, 1, 1 );
        case 5: LoopingAnim(playerid, "STRIP", "strip_E", 4.1, 1, 1, 1, 1, 1 );
        case 6: LoopingAnim(playerid, "STRIP", "strip_F", 4.1, 1, 1, 1, 1, 1 );
        case 7: LoopingAnim(playerid, "STRIP", "strip_G", 4.1, 1, 1, 1, 1, 1 );
        default: sendSyntaxMessage(playerid, "/dancing <1-7>");
    }
    return true;
}
YCMD:box(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "GYMNASIUM", "GYMshadowbox",4.0,1,1,1,1,0);
    return true;
}
YCMD:lowthrow(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "GRENADE", "WEAPON_throwu",3.0,0,0,0,0,0);
    return true;
}
YCMD:highthrow(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "GRENADE", "WEAPON_throw",4.0,0,0,0,0,0);
    return true;
}
YCMD:leftslap(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "PED", "BIKE_elbowL",4.0,0,0,0,0,0);
    return true;
}
YCMD:handsup(playerid, params[], help)
{
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    // if(paint_data[playerid][inPaintball] > 0) return sendError(playerid, "{FFF8C6}Command is not currently accessible.");
    // {
    //     SetPlayerSpecialAction(playerid,SPECIAL_ACTION_HANDSUP);
    //     playerStaticVar[playerid][playerHandsup] = 1;
    // }
    return true;
}
YCMD:fall(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "PED", "KO_skid_front",4.1,0,1,1,1,0);
    return true;
}
YCMD:fallback(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "PED", "FLOOR_hit_f", 4.0, 1, 0, 0, 0, 0);
    return true;
}
YCMD:laugh(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "RAPPING", "Laugh_01", 4.0, 0, 0, 0, 0, 0);
    return true;
}
YCMD:lookout(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "SHOP", "ROB_Shifty", 4.0, 0, 0, 0, 0, 0);
    return true;
}
YCMD:aim(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "SHOP", "ROB_Loop_Threat", 4.0, 1, 0, 0, 0, 0);
    return true;
}
YCMD:crossarms(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "COP_AMBIENT", "Coplook_loop", 4.0, 0, 1, 1, 1, -1);
    return true;
}
YCMD:lay(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "BEACH", "bather", 4.0, 1, 0, 0, 0, 0);
    return true;
}
YCMD:hide(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "ped", "cower", 3.0, 1, 0, 0, 0, 0);
    return true;
}
YCMD:vomit(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "FOOD", "EAT_Vomit_P", 3.0, 0, 0, 0, 0, 0);
    return true;
}
YCMD:wave(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "ON_LOOKERS", "wave_loop", 4.0, 1, 0, 0, 0, 0);
    return true;
}
YCMD:shouting(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "RIOT", "RIOT_shout",4.0,1,0,0,0,0);
    return true;
}
YCMD:chant(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "RIOT", "RIOT_CHANT",4.0,1,1,1,1,0);
    return true;
}
YCMD:frisked(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "POLICE", "crm_drgbst_01",4.0,0,1,1,1,0);
    return true;
}
YCMD:exhausted(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "PED", "IDLE_tired",3.0,1,0,0,0,0);
    return true;
}
YCMD:injured(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "SWEET", "Sweet_injuredloop", 4.0, 1, 0, 0, 0, 0);
    return true;
}
YCMD:slapass(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "SWEET", "sweet_ass_slap", 4.0, 0, 0, 0, 0, 0);
    return true;
}
YCMD:deal(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "DEALER", "DEALER_DEAL", 4.0, 0, 0, 0, 0, 0);
    return true;
}
YCMD:dealstance(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "DEALER", "DEALER_IDLE",4.0,1,0,0,0,0);
    return true;
}
YCMD:crack(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "CRACK", "crckdeth2", 4.0, 1, 0, 0, 0, 0);
    return true;
}

YCMD:wank(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "PAULNMAC", "wank_loop", 1.800001, 1, 0, 0, 1, 600);
    return true;
}
YCMD:gro(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "BEACH", "ParkSit_M_loop", 4.0, 1, 0, 0, 0, 0);
    return true;
}
YCMD:sup(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    new test;
    if(sscanf(params, "d", test)) return sendSyntaxMessage(playerid, "/sup <1-3>");
    switch (test)
    {
        case 1: OnePlayAnim(playerid, "GANGS", "hndshkba",4.0,0,0,0,0,0);
        case 2: OnePlayAnim(playerid, "GANGS", "hndshkda",4.0,0,0,0,0,0);
        case 3: OnePlayAnim(playerid, "GANGS", "hndshkfa_swt",4.0,0,0,0,0,0);
        default: sendSyntaxMessage(playerid, "/sup <1-3>");
    }
    return true;
}
YCMD:rap(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    new test;
    if(sscanf(params, "d", test)) return sendSyntaxMessage(playerid, "/rap <1-4>");
    switch (test)
    {
        case 1: LoopingAnim(playerid, "RAPPING", "RAP_A_Loop",4.0,1,0,0,0,0);
        case 2: LoopingAnim(playerid, "RAPPING", "RAP_C_Loop",4.0,1,0,0,0,0);
        case 3: LoopingAnim(playerid, "GANGS", "prtial_gngtlkD",4.0,1,0,0,0,0);
        case 4: LoopingAnim(playerid, "GANGS", "prtial_gngtlkH",4.0,1,0,0,1,1);
        default: sendSyntaxMessage(playerid, "/rap <1-4>");
    }
    return true;
}
YCMD:push(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "GANGS", "shake_cara",4.0,0,0,0,0,0);
    return true;
}
YCMD:akick(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "POLICE", "Door_Kick",4.0,0,0,0,0,0);
    return true;
}
YCMD:lowbodypush(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "GANGS", "shake_carSH",4.0,0,0,0,0,0);
    return true;
}
YCMD:spray(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "SPRAYCAN", "spraycan_full",4.0,0,0,0,0,0);
    return true;
}
YCMD:headbutt(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "WAYFARER", "WF_Fwd",4.0,0,0,0,0,0);
    return true;
}
YCMD:pee(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    // if(paint_data[playerid][inPaintball] > 0) return sendError(playerid, "{FFF8C6}Command is not currently accessible.");
    // {
    //     SetPlayerSpecialAction(playerid, SPECIAL_ACTION_PISSING);
    // }
    return true;
}
YCMD:koface(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "PED", "KO_shot_face",4.0,0,1,1,1,0);
    return true;
}
YCMD:kostomach(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "PED", "KO_shot_stom",4.0,0,1,1,1,0);
    return true;
}
YCMD:kiss(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "KISSING", "Grlfrd_Kiss_02", 1.800001, 1, 0, 0, 1, 600);
    return true;
}
YCMD:rollfall(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "PED", "BIKE_fallR",4.0,0,1,1,1,0);
    return true;
}
YCMD:lay2(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "SUNBATHE", "Lay_Bac_in",3.0,0,1,1,1,0);
    return true;
}
YCMD:hitch(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "MISC", "Hiker_Pose", 4.0, 1, 0, 0, 0, 0);
    return true;
}
YCMD:beach(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "BEACH", "SitnWait_loop_W",4.1,0,1,1,1,1);
    return true;
}
YCMD:medic(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "MEDIC", "CPR",4.1,0,1,1,1,1);
    return true;
}
YCMD:scratch(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "MISC", "Scratchballs_01", 4.0, 1, 0, 0, 0, 0);
    return true;
}
YCMD:sit(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "PED", "SEAT_idle", 4.0, 1, 0, 0, 0, 0);
    return true;
}
YCMD:drunk(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "PED", "WALK_DRUNK",4.0,1,1,1,1,0);
    return true;
}
YCMD:bomb(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    ClearAnimations(playerid);
    OnePlayAnim(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
    return true;
}
YCMD:getarrested(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "ped", "ARRESTgun", 4.0, 0, 1, 1, 1, -1);
    return true;
}
YCMD:chat(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "PED", "IDLE_CHAT",4.0,0,0,0,0,0);
    return true;
}
YCMD:fucku(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    OnePlayAnim(playerid, "PED", "fucku",4.0,0,0,0,0,0);
    return true;
}
YCMD:taichi(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    LoopingAnim(playerid, "PARK", "Tai_Chi_Loop",4.0,1,0,0,0,0);
    return true;
}
YCMD:knife(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    new test;
    if(sscanf(params, "d", test)) return sendSyntaxMessage(playerid, "/knife <1-4>");
    switch (test)
    {
        case 1: LoopingAnim(playerid, "KNIFE", "KILL_Knife_Ped_Damage",4.0,0,1,1,1,0);
        case 2: LoopingAnim(playerid, "KNIFE", "KILL_Knife_Ped_Die",4.0,0,1,1,1,0);
        case 3: OnePlayAnim(playerid, "KNIFE", "KILL_Knife_Player",4.0,0,0,0,0,0);
        case 4: LoopingAnim(playerid, "KNIFE", "KILL_Partial",4.0,0,1,1,1,1);
        default: sendSyntaxMessage(playerid, "/knife <1-4>");
    }
    return true;
}
YCMD:basket(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    new test;
    if(sscanf(params, "d", test)) return sendSyntaxMessage(playerid, "/basket <1-6>");
    switch (test)
    {
        case 1: LoopingAnim(playerid, "BSKTBALL", "BBALL_idleloop",4.0,1,0,0,0,0);
        case 2: OnePlayAnim(playerid, "BSKTBALL", "BBALL_Jump_Shot",4.0,0,0,0,0,0);
        case 3: OnePlayAnim(playerid, "BSKTBALL", "BBALL_pickup",4.0,0,0,0,0,0);
        case 4: LoopingAnim(playerid, "BSKTBALL", "BBALL_run",4.1,1,1,1,1,1);
        case 5: LoopingAnim(playerid, "BSKTBALL", "BBALL_def_loop",4.0,1,0,0,0,0);
        case 6: LoopingAnim(playerid, "BSKTBALL", "BBALL_Dnk",4.0,1,0,0,0,0);
        default: sendSyntaxMessage(playerid, "/basket <1-6>");
    }
    return true;
}
YCMD:dance(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    //if(paint_data[playerid][inPaintball] > 0) return sendError(playerid, "{FFF8C6}Command is not currently accessible.");
    {
        new test;
        if(sscanf(params, "d", test)) return sendSyntaxMessage(playerid, "/dance <1-4>");
        if(test < 1 || test > 4) return sendSyntaxMessage(playerid, "/dance <1-4>");
        if(test == 1)
        {
            SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE1);
        }
        else if(test== 2)
        {
            SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE2);
        }
        else if(test == 3)
        {
            SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE3);
        }
        else if(test == 4)
        {
            SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE4);
        }
    }
    return true;
}
YCMD:smoke(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    //if(paint_data[playerid][inPaintball] > 0) return sendError(playerid, "{FFF8C6}Command is not currently accessible.");
    {
        new test;
        if(sscanf(params, "d", test)) return sendSyntaxMessage(playerid, "/smoke <1-2>");
        if(test < 1 || test > 2) return sendSyntaxMessage(playerid, "/smoke <1-2>");
        if(test == 1)
        {
            ApplyAnimation(playerid, "SMOKING", "M_smk_in", 4.0, 0, 0, 0, 0, 0);
        }
        else if(test== 2)
        {
            ApplyAnimation(playerid, "SMOKING", "M_smklean_loop", 4.0, 1, 0, 0, 0, 0);
        }
    }
    return true;
}
YCMD:jumpwater(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130) return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "{999999}Animations are accessible only on foot.");
    ApplyAnimation(playerid, "DAM_JUMP", "DAM_LAUNCH",2,0,1,1,0,0);
    return true;
}
YCMD:stopanim(playerid, params[], help)
{
    if(GetPlayerAnimationIndex(playerid) == 1130)
        return sendError(playerid, "Nu puteti folosi aceasta comanda in timp ce sunteti in cadere!");

    if(IsPlayerInAnyVehicle(playerid))
        return sendError(playerid, "You can't use this command in a vehicle.");

    ClearAnimations(playerid);
    playerStaticVar[playerid][playerHandsup] = 0;
    return true;
}