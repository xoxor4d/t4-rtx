#include common_scripts\utility; 
#include maps\_utility;


main()
{	
	maps\_destructible_opel_blitz::init();
	level.startInvulnerableTime = GetDvarInt( "player_deathInvulnerableTime" );

	include_weapons();
	include_powerups();
	
	if( !isdefined( level.startInvulnerableTime ) )
		level.startInvulnerableTime = GetDvarInt( "player_deathInvulnerableTime" );

	maps\nazi_zombie_prototype_fx::main();
	maps\_zombiemode_prototype::main();
	
	init_sounds();
	
	thread bad_area_fixes();

	thread above_couches_death();
	thread above_roof_death();
	thread below_ground_death();
		
	// If you want to modify/add to the weapons table, please copy over the _zombiemode_weapons init_weapons() and paste it here.
	// I recommend putting it in it's own function...
	// If not a MOD, you may need to provide new localized strings to reflect the proper cost.

	//level thread intro_screen();
}

intro_screen()
{

	flag_wait( "all_players_connected" );
	wait(2);
	
	iprintlnbold("TEST!");
}

bad_area_fixes()
{
	thread disable_stances_in_zones();
}


// do point->distance checks and volume checks
disable_stances_in_zones()
{ 	
 	players = get_players();
 	
 	for (i = 0; i < players.size; i++)
 	{
 		players[i] thread fix_hax();
		players[i] thread fix_couch_stuckspot();
 		//players[i] thread in_bad_zone_watcher();	
 		players[i] thread out_of_bounds_watcher();
 	}
}




//Chris_P - added additional checks for some hax/exploits on the stairs, by the grenade bag and on one of the columns/pillars
fix_hax()
{
	self endon("disconnect");
	self endon("death");
	
	check = 15;
	check1 = 10;
	
	while(1)
	{
	
		//stairs
		wait(.5);
		if( distance2d(self.origin,( 101, -100, 40)) < check )
		{
			self setorigin ( (101, -90, self.origin[2]));
		}
		
		//crates/boxes
		else if( distance2d(self.origin, ( 816, 645, 12) ) < check )
		{
			self setorigin ( (816, 666, self.origin[2]) );
		
		}
		
		else if( distance2d( self.origin, (376, 643, 184) ) < check )
		{
			self setorigin( (376, 665, self.origin[2]) );
		}
		
		//by grandfather clock
		else	if(distance2d(self.origin,(519 ,765, 155)) < check1) 
		{
			self setorigin( (516, 793,self.origin[2]) );
		}
		
		//broken pillar
		else if( distance2d(self.origin,(315 ,346, 79))<check1)
		{
			self setorigin( (317, 360, self.origin[2]) );
		}
	
		//rubble by pillar
		else if( distance2d(self.origin,(199, 133, 18))<check)
		{
			self setorigin( (172, 123, self.origin[2]) );
		}
		
		//nook in curved stairs
		else if( distance2d(self.origin,(142 ,-100 ,91))<check1)
		{
			self setorigin( (139 ,-87, self.origin[2]) );
		}
		
		//by sawed off shotty				
		else if( distance2d(self.origin,(192, 369 ,185))<check1)
		{
			self setorigin( (195, 400 ,self.origin[2]) );
		}
		
		//rubble pile in the corner
		else if( distance2d(self.origin,(-210, 641, 247)) < check)
		{
			self setorigin( (-173 ,677,self.origin[2] ) );
		}

	}
		
}



fix_couch_stuckspot()
{
	self endon("disconnect");
	self endon("death");
	level endon("upstairs_blocker_purchased");

	while(1)
	{
		wait(.5);

		if( distance2d(self.origin, ( 181, 161, 206) ) < 10 )
		{
			self setorigin ( (175, 175 , self.origin[2]) );
		
		}		
		
	}

}




in_bad_zone_watcher()
{
	self endon ("disconnect");
	level endon ("fake_death");
	
	no_prone_and_crouch_zones = [];
 	
 	// grenade wall
 	no_prone_and_crouch_zones[0]["min"] = (-205, -128, 144);
 	no_prone_and_crouch_zones[0]["max"] = (-89, -90, 269);
 
  	no_prone_zones = [];
  	
  	// grenade wall
  	no_prone_zones[0]["min"] = (-205, -128, 144);
 	no_prone_zones[0]["max"] = (-55, 30, 269);

	// near the sawed off
  	no_prone_zones[1]["min"] = (88, 305, 144);
 	no_prone_zones[1]["max"] = (245, 405, 269);
 	
	while (1)
 	{	
		array_check = 0;
		
		if ( no_prone_and_crouch_zones.size > no_prone_zones.size)
		{
			array_check = no_prone_and_crouch_zones.size;
		}
		else
		{
			array_check = no_prone_zones.size;
		}
		
 		for(i = 0; i < array_check; i++)
 		{
 			if (isdefined(no_prone_and_crouch_zones[i]) && 
 				self is_within_volume(no_prone_and_crouch_zones[i]["min"][0], no_prone_and_crouch_zones[i]["max"][0], 
 											no_prone_and_crouch_zones[i]["min"][1], no_prone_and_crouch_zones[i]["max"][1],
 											no_prone_and_crouch_zones[i]["min"][2], no_prone_and_crouch_zones[i]["max"][2]))
 			{
 				self allowprone(false);
 				self allowcrouch(false);	
 				break;
 			}
 			else if (isdefined(no_prone_zones[i]) && 
 				self is_within_volume(no_prone_zones[i]["min"][0], no_prone_zones[i]["max"][0], 
 											no_prone_zones[i]["min"][1], no_prone_zones[i]["max"][1],
 											no_prone_zones[i]["min"][2], no_prone_zones[i]["max"][2]))
 			{
 				self allowprone(false);
 				break;
 			}
 			else
 			{
 				self allowprone(true);
 				self allowcrouch(true);
 			}
 			
 			
 		}		
 		wait 0.05;
 	}	
}


is_within_volume(min_x, max_x, min_y, max_y, min_z, max_z)
{
	if (self.origin[0] > max_x || self.origin[0] < min_x)
	{
		return false;
	}
	else if (self.origin[1] > max_y || self.origin[1] < min_y)
	{
		return false;
	}
	else if (self.origin[2] > max_z || self.origin[2] < min_z)
	{
		return false;
	}	
	
	return true;
}




init_sounds()
{
	maps\_zombiemode_utility::add_sound( "break_stone", "break_stone" );
}

// Include the weapons that are only inr your level so that the cost/hints are accurate
// Also adds these weapons to the random treasure chest.
include_weapons()
{
	// Pistols
	//include_weapon( "colt" );
	//include_weapon( "colt_dirty_harry" );
	//include_weapon( "walther" );
	include_weapon( "sw_357" );
	
	// Semi Auto
	include_weapon( "m1carbine" );
	include_weapon( "m1garand" );
	include_weapon( "gewehr43" );

	// Full Auto
	include_weapon( "stg44" );
	include_weapon( "thompson" );
	include_weapon( "mp40" );
	
	// Bolt Action

	include_weapon( "kar98k" );
	include_weapon( "springfield" );

	// Scoped
	include_weapon( "ptrs41_zombie" );
	include_weapon( "kar98k_scoped_zombie" );
		
	// Grenade
	include_weapon( "molotov" );
	// JESSE: lets go all german grenades for consistency and to reduce annoyance factor
	//	include_weapon( "fraggrenade" );
	include_weapon( "stielhandgranate" );

	// Grenade Launcher
	include_weapon( "m1garand_gl" );
	include_weapon( "m7_launcher" );
	
	// Flamethrower
	include_weapon( "m2_flamethrower_zombie" );
	
	// Shotgun
	include_weapon( "doublebarrel" );
	include_weapon( "doublebarrel_sawed_grip" );
	include_weapon( "shotgun" );
	
	// Bipod
	include_weapon( "fg42_bipod" );
	include_weapon( "mg42_bipod" );
	include_weapon( "30cal_bipod" );

	// Heavy MG
	include_weapon( "bar" );

	// Rocket Launcher
	include_weapon( "panzerschrek" );

	// Special
	include_weapon( "ray_gun" );
}

include_powerups()
{
	include_powerup( "nuke" );
	include_powerup( "insta_kill" );
	include_powerup( "double_points" );
	include_powerup( "full_ammo" );
}

include_weapon( weapon_name )
{
	maps\_zombiemode_weapons::include_zombie_weapon( weapon_name );
}

include_powerup( powerup_name )
{
	maps\_zombiemode_powerups::include_zombie_powerup( powerup_name );
}

above_couches_death()
{
	level endon ("junk purchased");
	
	while (1)
	{
		wait 0.2;
				
		players = get_players();
		
		for (i = 0; i < players.size; i++)
		{
			if (players[i].origin[2] > 145)
			{
				setsaveddvar("player_deathInvulnerableTime", 0);
				players[i] DoDamage( players[i].health + 1000, players[i].origin, undefined, undefined, "riflebullet" );
				setsaveddvar("player_deathInvulnerableTime", level.startInvulnerableTime);	
			}
		}
	}
}

above_roof_death()
{
	while (1)
	{
		wait 0.2;
		
		players = get_players();
		
		for (i = 0; i < players.size; i++)
		{
			if (players[i].origin[2] > 235)
			{
				setsaveddvar("player_deathInvulnerableTime", 0);
				players[i] DoDamage( players[i].health + 1000, players[i].origin, undefined, undefined, "riflebullet" );
				setsaveddvar("player_deathInvulnerableTime", level.startInvulnerableTime);	
			}
		}
	}
}

below_ground_death()
{
	while (1)
	{
		wait 0.2;
		
		players = get_players();
		
		for (i = 0; i < players.size; i++)
		{
			if (players[i].origin[2] < -11)
			{
				setsaveddvar("player_deathInvulnerableTime", 0);
				players[i] DoDamage( players[i].health + 1000, players[i].origin, undefined, undefined, "riflebullet" );
				setsaveddvar("player_deathInvulnerableTime", level.startInvulnerableTime);	
			}
		}
	}
}


out_of_bounds_watcher()
{
	self endon ("disconnect");
	
	outside_of_map = [];
 	
 	outside_of_map[0]["min"] = (361, 591, -11);
 	outside_of_map[0]["max"] = (1068, 1031, 235);
 	
 	outside_of_map[1]["min"] = (-288, 591, -11);
 	outside_of_map[1]["max"] = (361, 1160, 235);
 	
 	outside_of_map[2]["min"] = (-272, 120, -11);
 	outside_of_map[2]["max"] = (370, 591, 235);

 	outside_of_map[3]["min"] = (-272, -912, -11);
 	outside_of_map[3]["max"] = (273, 120, 235);
 	 	
	while (1)
 	{	
		array_check = outside_of_map.size;
		
		kill_player = true;
 		for(i = 0; i < array_check; i++)
 		{
 			if (self is_within_volume(	outside_of_map[i]["min"][0], outside_of_map[i]["max"][0], 
 										outside_of_map[i]["min"][1], outside_of_map[i]["max"][1],
 										outside_of_map[i]["min"][2], outside_of_map[i]["max"][2]))
 			{
 				kill_player = false;

 			} 			
 		}		
 		
 		if (kill_player)
 		{
 			setsaveddvar("player_deathInvulnerableTime", 0);
			self DoDamage( self.health + 1000, self.origin, undefined, undefined, "riflebullet" );
			setsaveddvar("player_deathInvulnerableTime", level.startInvulnerableTime);	
 		}
 		
 		wait 0.2;
 	}	
}
