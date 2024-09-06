#include maps\_utility;
#include common_scripts\utility;
/#
mainDebug()
{
	// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
	if( getdebugdvar( "replay_debug" ) == "1" )
		println("File: _debug.gsc. Function: mainDebug()\n");
	
	level.animsound_hudlimit = 14;
	thread lastSightPosWatch();
	
	if( level.script != "background" )
		thread camera();

	if( getdebugdvar( "debug_corner" ) == "" )
		setdvar( "debug_corner", "off" );
	else
	if( getdebugdvar( "debug_corner" ) == "on" )
		debug_corner();
	
	if( getdvar( "chain" ) == "1" )
		thread debugchains();

	if( getdebugdvar("debug_bayonet") == "" )
	{
		setdvar( "debug_bayonet", "0" );
	}
	thread debug_bayonet();

	//thread debugDvars();
	precacheShader( "white" );
	thread debugColorFriendlies();
	
	thread watchMinimap();
	
	if( getdvar( "level_transition_test" ) != "off" )
		thread complete_me();

	if(getdvar("level_completeall") != "off")
		maps\_endmission::force_all_complete();
		
	if( getdvar("level_clear_all") != "off" )
		maps\_endmission::clearall();
			
	// SRS 3/19/08: engagement distance debug dvar toggle watcher
	thread engagement_distance_debug_toggle();
		
//	thread playerNode();
//	thread colordebug();
//	thread debuggoalpos();

	// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
	if( getdebugdvar( "replay_debug" ) == "1" )
		println("File: _debug.gsc. Function: mainDebug() - COMPLETE\n");
}
#/

debugchains()
{
	nodes = getallnodes();
	fnodenum = 0;

	fnodes = [];
	for( i=0;i<nodes.size;i++ )
	{
		if(( !( nodes[ i ].spawnflags & 2 ) ) &&
		( 
		(( isdefined( nodes[ i ].target ) ) &&(( getnodearray( nodes[ i ].target, "targetname" ) ).size > 0 )   ) ||
		(( isdefined( nodes[ i ].targetname ) ) &&(( getnodearray( nodes[ i ].targetname, "target" ) ).size > 0 ) )
		 )
		 )
		{
			fnodes[ fnodenum ] = nodes[ i ];
			fnodenum++;
		}
	}

	count = 0;

	while( 1 )
	{
		if( getdvar( "chain" ) == "1" )
		{
			for( i=0;i<fnodes.size;i++ )
			{
				// SCRIPTER_MOD
				// JesseS (3/20/2007): removed level.player, added players[0]
				players = get_players();
				if (distance (players[0] getorigin(), fnodes[i].origin) < 1500)
				{
					print3d( fnodes[ i ].origin, "yo", ( 0.2, 0.8, 0.5 ), 0.45 );
					/*
					count++;
					if( count > 25 )
					{
						count = 0;
						maps\_spawner::waitframe();
					}
					*/
				}
			}

			friends = getaiarray( "allies" );
			for( i=0;i<friends.size;i++ )
			{
				node = friends[ i ] animscripts\utility::GetClaimedNode();
				if( isdefined( node ) )
					line( friends[ i ].origin +( 0, 0, 35 ), node.origin, ( 0.2, 0.5, 0.8 ), 0.5 );
			}

		}
		maps\_spawner::waitframe();
	}
}

debug_enemyPos( num )
{
	ai = getaiarray();
	
	for( i=0;i<ai.size;i++ )
	{
		if( ai[ i ] getentitynumber() != num )
			continue;
			
		ai[ i ] thread debug_enemyPosProc();
		break;	
	}
}

debug_stopEnemyPos( num )
{
	ai = getaiarray();
	
	for( i=0;i<ai.size;i++ )
	{
		if( ai[ i ] getentitynumber() != num )
			continue;
			
		ai[ i ] notify( "stop_drawing_enemy_pos" );
		break;	
	}
}

debug_enemyPosProc()
{
	self endon( "death" );
	self endon( "stop_drawing_enemy_pos" );
	for( ;; )
	{
		wait( 0.05 );

		if( isalive( self.enemy ) )
			line( self.origin +( 0, 0, 70 ), self.enemy.origin +( 0, 0, 70 ), ( 0.8, 0.2, 0.0 ), 0.5 );

		if( !self animscripts\utility::hasEnemySightPos() )
			continue;
		
		pos = animscripts\utility::getEnemySightPos();
		line( self.origin +( 0, 0, 70 ), pos, ( 0.9, 0.5, 0.3 ), 0.5 );
	}
}

debug_enemyPosReplay()
{
	ai = getaiarray();
	guy = undefined;
	
	for( i=0;i<ai.size;i++ )
	{
//		if( ai[ i ] getentitynumber() != num )
//			continue;
			
		guy = ai[ i ];
		if( !isalive( guy ) )
			continue;
	

		if( isdefined( guy.lastEnemySightPos ) )
			line( guy.origin +( 0, 0, 65 ), guy.lastEnemySightPos, ( 1, 0, 1 ), 0.5 );
			
		if( guy.goodShootPosValid )
		{
			if( guy.team == "axis" )
				color =( 1, 0, 0 );
			else
				color =( 0, 0, 1 );
			
//			nodeOffset = guy GetEye();
			nodeOffset = guy.origin +( 0, 0, 54 );
			if( isdefined( guy.node ) )
			{
				if( guy.node.type == "Cover Left" )
				{
					cornerNode = true;
					nodeOffset = anglestoright( guy.node.angles );
					nodeOffset = vectorScale( nodeOffset, -32 );
					nodeOffset =( nodeOffset[ 0 ] , nodeOffset[ 1 ], 64 );
					nodeOffset = guy.node.origin + nodeOffset;
				}
				else
				if( guy.node.type == "Cover Right" )
				{
					cornerNode = true;
					nodeOffset = anglestoright( guy.node.angles );
					nodeOffset = vectorScale( nodeOffset, 32 );
					nodeOffset =( nodeOffset[ 0 ] , nodeOffset[ 1 ], 64 );
					nodeOffset = guy.node.origin + nodeOffset;
				}
			}			
			draw_arrow( nodeOffset, guy.goodShootPos, color );
		}
//		break;	
	}
	if( 1 ) return;

	if( !isalive( guy ) )
		return;
		
	if( isalive( guy.enemy ) )
		line( guy.origin +( 0, 0, 70 ), guy.enemy.origin +( 0, 0, 70 ), ( 0.6, 0.2, 0.2 ), 0.5 );

	if( isdefined( guy.lastEnemySightPos ) )
		line( guy.origin +( 0, 0, 65 ), guy.lastEnemySightPos, ( 0, 0, 1 ), 0.5 );

	if( isalive( guy.goodEnemy ) )
		line( guy.origin +( 0, 0, 50 ), guy.goodEnemy.origin, ( 1, 0, 0 ), 0.5 );


	if( !guy animscripts\utility::hasEnemySightPos() )
		return;

	pos = guy animscripts\utility::getEnemySightPos();
	line( guy.origin +( 0, 0, 55 ), pos, ( 0.2, 0.2, 0.6 ), 0.5 );

	if( guy.goodShootPosValid )
		line( guy.origin +( 0, 0, 45 ), guy.goodShootPos, ( 0.2, 0.6, 0.2 ), 0.5 );
}

drawEntTag( num )
{
	/#
	ai = getaiarray();
	for( i=0;i<ai.size;i++ )
	{
		if( ai[ i ] getentnum() != num )
			continue;
		ai[ i ] thread dragTagUntilDeath( getdebugdvar( "debug_tag" ) );
	}
	setdvar( "debug_enttag", "" );
	#/
}

drawTag( tag, opcolor )
{
	org = self GetTagOrigin( tag );
	ang = self GetTagAngles( tag );
	drawArrow( org, ang, opcolor );
}

drawOrgForever( opcolor )
{
	for( ;; )
	{
		drawArrow( self.origin, self.angles, opcolor );
		wait( 0.05 );
	}
}


drawArrowForever( org, ang )
{
	for( ;; )
	{
		drawArrow( org, ang );
		wait( 0.05 );
	}
}

drawOriginForever()
{
	for( ;; )
	{
		drawArrow( self.origin, self.angles );
		wait( 0.05 );
	}
}

drawArrow( org, ang, opcolor )
{
	scale = 50;
	forward = anglestoforward( ang );
	forwardFar = vectorScale( forward, scale );
	forwardClose = vectorScale( forward, ( scale * 0.8 ) );
	right = anglestoright( ang );
	leftdraw = vectorScale( right, ( scale * -0.2 ) );
	rightdraw = vectorScale( right, ( scale * 0.2 ) );
	
	up = anglestoup( ang );
	right = vectorScale( right, scale );
	up = vectorScale( up, scale );
	
	red = ( 0.9, 0.2, 0.2 );
	green = ( 0.2, 0.9, 0.2 );
	blue = ( 0.2, 0.2, 0.9 );
	if ( isdefined( opcolor ) )
	{
		red = opcolor;
		green = opcolor;
		blue = opcolor;
	}
	
	line( org, org + forwardFar, red, 0.9 );
	line( org + forwardFar, org + forwardClose + rightdraw, red, 0.9 );
	line( org + forwardFar, org + forwardClose + leftdraw, red, 0.9 );

	line( org, org + right, blue, 0.9 );
	line( org, org + up, green, 0.9 );
}

drawPlayerViewForever()
{
	for ( ;; )
	{
		drawArrow( level.player.origin, level.player getplayerangles(), (1,1,1) );
		wait( 0.05 );
	}
}



drawTagForever( tag, opcolor )
{
	self endon( "death" );
	for( ;; )
	{
		drawTag( tag, opcolor );
		wait( 0.05 );
	}
}


dragTagUntilDeath( tag )
{
	for( ;; )
	{
		if( !isdefined( self.origin ) )
			break;
		drawTag( tag );
		wait( 0.05 );
	}
}

viewTag( type, tag )
{
	if( type == "ai" )
	{
		ai = getaiarray();
		for( i=0;i<ai.size;i++ )
			ai[ i ] drawTag( tag );
	}
	else
	{
		vehicle = getentarray( "script_vehicle", "classname" );
		for( i=0;i<vehicle.size;i++ )
			vehicle[ i ] drawTag( tag );
	}
}


debug_corner()
{
	// SCRIPTER_MOD
	// JesseS (3/20/2007): removed level.player, added ignoreme for all players
	players = get_players();
	for(i=0; i < players.size; i++)
	{
		players[i].ignoreme = true;
	}

	nodes = getallnodes();
	corners = [];
	for( i=0;i<nodes.size;i++ )
	{
		if( nodes[ i ].type == "Cover Left" )
			corners[ corners.size ] = nodes[ i ];
		if( nodes[ i ].type == "Cover Right" )
			corners[ corners.size ] = nodes[ i ];
	}

	ai = getaiarray();
	for( i=0;i<ai.size;i++ )
		ai[ i ] delete();
		
	level.debugspawners = getspawnerarray();
	level.activeNodes = [];
	level.completedNodes = [];
	for( i=0;i<level.debugspawners.size;i++ )
		level.debugspawners[ i ].targetname = "blah";
		
	covered = 0;	
	for( i=0;i<30;i++ )
	{
		if( i >= corners.size )
			break;
			
		corners[ i ] thread coverTest();
		covered++;
	}
	
	if( corners.size <= 30 )
		return;
		
	for( ;; )
	{
		level waittill( "debug_next_corner" );
		if( covered >= corners.size )
			covered = 0;
		corners[ covered ] thread coverTest();
		covered++;
	}
}

coverTest()
{
	coverSetupAnim();
}

#using_animtree( "generic_human" );
coverSetupAnim()
{
	spawn = undefined;
	spawner = undefined;
	for( ;; )
	{
		for( i=0;i<level.debugspawners.size;i++ )
		{
			wait( 0.05 );
			spawner = level.debugspawners[ i ];
			nearActive = false;
			for( p=0;p<level.activeNodes.size;p++ )
			{
				if( distance( level.activeNodes[ p ].origin, self.origin ) > 250 )
					continue;
				nearActive = true;
				break;
			}
			if( nearActive )
				continue;
				
			completed = false;
			for( p=0;p<level.completedNodes.size;p++ )
			{
				if( level.completedNodes[ p ] != self )
					continue;
				completed = true;
				break;
			}
			if( completed )
				continue;
				
			level.activeNodes[ level.activeNodes.size ] = self;
			spawner.origin = self.origin;
			spawner.angles = self.angles;
			spawner.count = 1;
			spawn = spawner stalingradspawn();
			if( spawn_failed( spawn ) )
			{
				removeActiveSpawner( self );
				continue;
			}
			
			break;
		}
		if( isalive( spawn ) )
			break;
	}

	wait( 1 );
	if( isalive( spawn ) )
	{
		spawn.ignoreme = true;
		spawn.team = "neutral";
		spawn setgoalpos( spawn.origin );
		thread createLine( self.origin );
		spawn thread debugorigin();
		thread createLineConstantly( spawn );
		spawn waittill( "death" );
	}
	removeActiveSpawner( self );
	level.completedNodes[ level.completedNodes.size ] = self;
}

removeActiveSpawner( spawner )
{
	newSpawners = [];	
	for( p=0;p<level.activeNodes.size;p++ )
	{
		if( level.activeNodes[ p ] == spawner )
			continue;
		newSpawners[ newSpawners.size ] = level.activeNodes[ p ];
	}
	level.activeNodes = newSpawners;
}


createLine( org )
{
	for( ;; )
	{
		line( org +( 0, 0, 35 ), org, ( 0.2, 0.5, 0.8 ), 0.5 );
		wait( 0.05 );
	}
}

createLineConstantly( ent )
{
	org = undefined;
	while( isalive( ent ) )
	{
		org = ent.origin;
		wait( 0.05 );		
	}
	
	for( ;; )
	{
		line( org +( 0, 0, 35 ), org, ( 1.0, 0.2, 0.1 ), 0.5 );
		wait( 0.05 );
	}
}

debugMisstime()
{
	self notify( "stopdebugmisstime" );
	self endon( "stopdebugmisstime" );
	self endon( "death" );
	for( ;; )
	{
		if( self.a.misstime <= 0 )
			print3d( self gettagorigin( "TAG_EYE" ) +( 0, 0, 15 ), "hit", ( 0.3, 1, 1 ), 1 );
		else
			print3d( self gettagorigin( "TAG_EYE" ) +( 0, 0, 15 ), self.a.misstime/20, ( 0.3, 1, 1 ), 1 );
		wait( 0.05 );
	}
}

debugMisstimeOff()
{
	self notify( "stopdebugmisstime" );
}

setEmptyDvar( dvar, setting )
{
	/#
	if( getdebugdvar( dvar ) == "" )
		setdvar( dvar, setting );
	#/		
}

debugJump( num )
{
	/#
	ai = getaiarray();
	for( i=0;i<ai.size;i++ )
	{
		if( ai[ i ] getentnum() != num )
			continue;
			
		// SCRIPTER_MOD
		// JesseS (3/20/2007): removed level.player, added players[0]
		players = get_players();			
		line (players[0].origin, ai[i].origin, (0.2,0.3,1.0));
		return;
	}
	#/
}

debugDvars()
{
	/#
	// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
	if( getdebugdvar( "replay_debug" ) == "1" )
		println("File: _debug.gsc. Function: debugDvars()\n");
	
	precachemodel("test_sphere_silver");
	if( getdebugdvar( "debug_vehiclesittags" ) == "" )
		setdvar( "debug_vehiclesittags", "off" );
	
	if ( getDvar( "level_transition_test" ) == "" )
		setDvar( "level_transition_test", "off" );

	if ( getDvar( "level_completeall" ) == "" )
		setDvar( "level_completeall", "off" );

	if ( getDvar( "level_clear_all" ) == "" )
		setDvar( "level_clear_all", "off" );	
	
	// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
	if( getdebugdvar( "replay_debug" ) == "1" )
		println("File: _debug.gsc. Function: debugDvars() - WAIT FINISHED\n");
	
	setEmptyDvar( "debug_accuracypreview", "off" );

	if( getdebugdvar( "debug_lookangle" ) == "" )
		setdvar( "debug_lookangle", "off" );

	if( getdebugdvar( "debug_grenademiss" ) == "" )
		setdvar( "debug_grenademiss", "off" );

	if( getdebugdvar( "debug_enemypos" ) == "" )
		setdvar( "debug_enemypos", "-1" );
		
	if( getdebugdvar( "debug_dotshow" ) == "" )
		setdvar( "debug_dotshow", "-1" );
		
	if( getdebugdvar( "debug_stopenemypos" ) == "" )
		setdvar( "debug_stopenemypos", "-1" );

	if( getdebugdvar( "debug_replayenemypos" ) == "" )
		setdvar( "debug_replayenemypos", "-1" );

	if( getdebugdvar( "debug_tag" ) == "" )
		setdvar( "debug_tag", "" );

	if( getdebugdvar( "debug_arrivals" ) == "" )
		setdvar( "debug_arrivals", "" );

	if( getdebugdvar( "debug_chatlook" ) == "" )
		setdvar( "debug_chatlook", "" );
		
	if( getdebugdvar( "debug_vehicletag" ) == "" )
		setdvar( "debug_vehicletag", "" );

		
	if( getdebugdvar( "debug_colorfriendlies" ) == "" )
		setdvar( "debug_colorfriendlies", "off" );

	if( getdebugdvar( "debug_animreach" ) ==  "" )
		setdvar( "debug_animreach", "off" );

	if( getdebugdvar( "debug_hatmodel" ) == "" )
		setdvar( "debug_hatmodel", "on" );

	if( getdebugdvar( "debug_trace" ) == "" )
		setdvar( "debug_trace", "off" );

	level.debug_badpath = false;
	if( getdebugdvar( "debug_badpath" ) == "" )
		setdvar( "debug_badpath", "off" );

	if( getdebugdvar( "anim_lastsightpos" ) == "" )
		setdvar( "debug_lastsightpos", "off" );

	if( getdebugdvar( "debug_dog_sound" ) == "" )
		setdvar( "debug_dog_sound", "" );

	if( getdvar( "debug_nuke" ) == "" )
		setdvar( "debug_nuke", "off" );

	if( getdebugdvar( "debug_deathents" ) == "on" )
		setdvar( "debug_deathents", "off" );

	if( getdvar( "debug_jump" ) == "" )
		setdvar( "debug_jump", "" );

	if( getdvar( "debug_hurt" ) == "" )
		setdvar( "debug_hurt", "" );

	if( getdebugdvar( "animsound" ) == "" )
		setdvar( "animsound", "off" );
	if( getdvar( "tag" ) == "" )
		setdvar( "tag", "" );
	
	for( i=1; i <= level.animsound_hudlimit; i++ )
	{
		if( getdvar( "tag" + i ) == "" )
			setdvar( "tag" + i, "" );
	}
		
	if( getdebugdvar( "animsound_save" ) == "" )
		setdvar( "animsound_save", "" );

	if( getdvar( "debug_depth" ) == "" )
		setdvar( "debug_depth", "" );

	if( getdebugdvar( "debug_colornodes" ) == "" )
		setdvar( "debug_colornodes", "" );
		
	if( getdebugdvar( "debug_reflection" ) == "" )
		setdvar( "debug_reflection", "0" );
	
	// SCRIPTER_MOD: JesseS (3/18/2008):  added dynamic guy spawning
	if( getdebugdvar( "debug_dynamic_ai_spawning" ) == "" )
		setdvar( "debug_dynamic_ai_spawning", "0" );
		
	level.last_threat_debug = -23430;
	if( getdebugdvar( "debug_threat" ) == "" )
		setdvar( "debug_threat", "-1" );

	waittillframeend; // for vars to get init'd elsewhere
	
	red =( 1, 0, 0 );
	blue =( 0, 0, 1 );
	yellow =( 1, 1, 0 );
	cyan =( 0, 1, 1 );
	green =( 0, 1, 0 );
	purple =( 1, 0, 1 );
	orange =( 1, 0.5, 0 );

	level.color_debug[ "r" ] = red;
	level.color_debug[ "b" ] = blue;
	level.color_debug[ "y" ] = yellow;
	level.color_debug[ "c" ] = cyan;
	level.color_debug[ "g" ] = green;
	level.color_debug[ "p" ] = purple;
	level.color_debug[ "o" ] = orange;
	
	level.debug_reflection = 0;
	

	if(getdvar("debug_character_count") == "")
		setdvar("debug_character_count","off");
		
//	thread hatmodel();	
	//thread debug_character_count();

	// MikeD (11/2/2007): Show's the player's position
	level thread debug_show_viewpos();

	noAnimscripts = getdvar( "debug_noanimscripts" ) == "on";
	for( ;; )
	{
		// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
		if( getdebugdvar( "replay_debug" ) == "1" )
			println("File: _debug.gsc. Function: debugDvars() - INNNER LOOP START\n");
		
		if( getdebugdvar( "debug_jump" ) != "" )
			debugJump( getdebugdvarint( "debug_jump" ) );
		
		if( getdebugdvar( "debug_tag" ) != "" )
		{
			thread viewTag( "ai", getdebugdvar( "debug_tag" ) );
			if( getdebugdvarInt( "debug_enttag" ) > 0 )
				thread drawEntTag( getdebugdvarInt( "debug_enttag" ) );
		}

		if( getdebugdvar( "debug_vehicletag" ) != "" )
			thread viewTag( "vehicle", getdebugdvar( "debug_vehicletag" ) );

		if( getdebugdvar( "debug_colornodes" ) == "on" )
			thread debug_colornodes();
		
		if( getdebugdvar( "debug_vehiclesittags" ) != "off" )
			thread debug_vehiclesittags();

		if( getdebugdvar( "debug_replayenemypos" ) == "on" )
			thread debug_enemyPosReplay();

		thread debug_animSound();

		if( getdvar( "tag" ) != "" )
			thread debug_animSoundTagSelected();
	
		for( i=1; i <= level.animsound_hudlimit; i++ )
		{
			if( getdvar( "tag" + i ) != "" )
				thread debug_animSoundTag( i );
		}

		if( getdebugdvar( "animsound_save" ) != "" )
			thread debug_animSoundSave();

		if( getdvar( "debug_nuke" ) != "off" )
		{
			thread debug_nuke();
		}

		if( getdvar( "debug_misstime" ) == "on" )
		{
			setdvar( "debug_misstime", "start" );
			array_thread( getaiarray(), ::debugMisstime );
		}
		else
		if( getdvar( "debug_misstime" ) == "off" )
		{
			setdvar( "debug_misstime", "start" );
			array_thread( getaiarray(), ::debugMisstimeOff );
		}

		if( getdvar( "debug_deathents" ) == "on" )
			thread deathspawnerPreview();	

		if( getdvar( "debug_hurt" ) == "on" )
		{
			setdvar( "debug_hurt", "off" );
			
			// SCRIPTER_MOD
			// JesseS (3/20/2007): dodamage on all players instead of level.player
			players = get_players();
			for(i = 0; i < players.size; i++)
			{
				players[i] dodamage(50, (324234,3423423,2323));
			}
		}

		if( getdvar( "debug_hurt" ) == "on" )
		{
			setdvar( "debug_hurt", "off" );
			
			// SCRIPTER_MOD
			// JesseS (3/20/2007): dodamage on all players instead of level.player
			players = get_players();
			for(i = 0; i < players.size; i++)
			{
				players[i] dodamage(50, (324234,3423423,2323));
			}
		}

		if( getdvar( "debug_depth" ) == "on" )
		{
			thread fogcheck();
		}

		if( getdebugdvar( "debug_threat" ) != "-1" && getdebugdvar( "debug_threat" ) != "" )
		{
			debugThreat();
		}

		level.debug_badpath = getdebugdvar( "debug_badpath" ) == "on";

		if( getdebugdvarint( "debug_enemypos" ) != -1 )
		{
			thread debug_enemypos( getdebugdvarint( "debug_enemypos" ) );
			setdvar( "debug_enemypos", "-1" );
		}
		if( getdebugdvarint( "debug_stopenemypos" ) != -1 )
		{
			thread debug_stopenemypos( getdebugdvarint( "debug_stopenemypos" ) );
			setdvar( "debug_stopenemypos", "-1" );
		}
		
		if( !noAnimscripts && getdvar( "debug_noanimscripts" ) == "on" )
		{
			anim.defaultException = animscripts\init::infiniteLoop;
			noAnimscripts = true;
		}
		
		if( noAnimscripts && getdvar( "debug_noanimscripts" ) == "off" )
		{
			anim.defaultException = animscripts\init::empty;
			anim notify( "new exceptions" );
			noAnimscripts = false;
		}

		if( getdebugdvar( "debug_trace" ) == "on" )
		{
			if( !isdefined( level.traceStart ) )
			{
				thread showDebugTrace();
			}
			
			// SCRIPTER_MOD
			// JesseS (3/20/2007): level.player replaced with players[0]
			players = get_players();
			
			level.traceStart = players[0] geteye();
			setdvar( "debug_trace", "off" );
		}
		
		// SCRIPTER_MOD: JesseS (3/18/2008):  Trying to add dynamic spawning in levels
		if( getdebugdvar( "debug_dynamic_ai_spawning" ) == "1" && (!isdefined(	level.spawn_anywhere_active) || level.spawn_anywhere_active == false))
		{
			level.spawn_anywhere_active = true;
			thread dynamic_ai_spawner();
		}
		else if (getdebugdvar( "debug_dynamic_ai_spawning" ) == "0")
		{
			level.spawn_anywhere_active = false;
			level notify ("kill dynamic spawning");	
		}
		
		debug_reflection();

		// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
		if( getdebugdvar( "replay_debug" ) == "1" )
			println("File: _debug.gsc. Function: debugDvars() - INNNER LOOP START WAIT 0.05\n");
		
		wait( 0.05 );
		
		// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
		if( getdebugdvar( "replay_debug" ) == "1" )
			println("File: _debug.gsc. Function: debugDvars() - INNNER LOOP STOP WAIT 0.05\n");

		if( getdebugdvar( "replay_debug" ) == "1" )
			println("File: _debug.gsc. Function: debugDvars() - INNNER LOOP END\n");
	}
	
	// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
	if( getdebugdvar( "replay_debug" ) == "1" )
		println("File: _debug.gsc. Function: debugDvars() - COMPLETE\n");
	#/
}

remove_reflection_objects()
{
/#
	if ( ( level.debug_reflection == 2 || level.debug_reflection == 3 ) && isdefined( level.debug_reflection_objects ) )
	{
		for( i = 0; i < level.debug_reflection_objects.size; i++ )
		{
			level.debug_reflection_objects[i] delete();
		}
		level.debug_reflection_objects = undefined;
	}
	
	if ( level.debug_reflection == 1 || level.debug_reflection == 3 )
	{
		level.debug_reflectionobject delete();
	}
#/
}

create_reflection_objects()
{
	/#
	reflection_locs = GetReflectionLocs();
	for ( i = 0; i < reflection_locs.size; i++ )
		{
		level.debug_reflection_objects[i] = spawn( "script_model", reflection_locs[i] );
		level.debug_reflection_objects[i] setmodel( "test_sphere_silver" );
	}
#/
}

create_reflection_object()
{
/#
				players = get_players();
				player = players[0];
				level.debug_reflectionobject = spawn( "script_model", player geteye() +( vector_multiply( anglestoforward( player.angles ), 100 ) ) );
				level.debug_reflectionobject setmodel( "test_sphere_silver" );
				level.debug_reflectionobject.origin = player geteye() + ( vector_multiply ( anglestoforward ( player getplayerangles() ), 100) );
				level.debug_reflectionobject linkto ( player );
				thread 	debug_reflection_buttons();
#/	
}

			
debug_reflection()
{
	/#
		if( ( getdebugdvar( "debug_reflection" ) == "2"  && level.debug_reflection != 2 ) || ( getdebugdvar( "debug_reflection" ) == "3"  && level.debug_reflection != 3 ) )
		{
				remove_reflection_objects();
				if ( getdebugdvar( "debug_reflection" ) == "2" )
				{
					create_reflection_objects();
					level.debug_reflection = 2;
		}
				else
		{
					create_reflection_objects();
					create_reflection_object();
					level.debug_reflection = 3;
				}
		}
		else if( getdebugdvar( "debug_reflection" ) == "1"  && level.debug_reflection != 1 )
		{
				remove_reflection_objects();
				create_reflection_object();
				level.debug_reflection = 1;
		}
		else if( getdebugdvar( "debug_reflection" ) == "0" && level.debug_reflection != 0 )
		{
				remove_reflection_objects();
				level.debug_reflection = 0;
		}	
		#/
}

debug_reflection_buttons()
{
	/#
	offset = 100;
	lastoffset = offset;
	offsetinc = 50;
	while( getdebugdvar( "debug_reflection" ) == "1" || getdebugdvar( "debug_reflection" ) == "3" )
	{
		// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
		if( getdebugdvar( "replay_debug" ) == "1" )
			println("File: _debug.gsc. Function: debug_reflection_buttons() - INNER LOOP START\n");
		
		players = get_players();

		if(players[0] buttonpressed ("BUTTON_X"))
		{
			offset+=offsetinc;
		}
		if(players[0] buttonpressed ("BUTTON_Y"))
		{
			offset-=offsetinc;
		}
		if( offset > 1000 )
		{
			offset = 1000;
		}
		if( offset < 64 )
		{
			offset = 64;
		}
//		if( offset!=lastoffset )
//		{
			level.debug_reflectionobject unlink();
			level.debug_reflectionobject.origin = players[0] geteye() + ( vector_multiply ( anglestoforward ( players[0] getplayerangles() ), offset) );
			lastoffset = offset;
//			}

		// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
		if( getdebugdvar( "replay_debug" ) == "1" )
			println("File: _debug.gsc. Function: debug_reflection_buttons() - INNER LOOP END\n");
		
		line(	level.debug_reflectionobject.origin, 
			getreflectionorigin( level.debug_reflectionobject.origin ),
			(1,0,0),
			true,
			1 );

		wait .05;
		level.debug_reflectionobject linkto (players[0]);
	}
	#/
}

showDebugTrace()
{
	startOverride = undefined;
	endOverride = undefined;
	startOverride =( 15.1859, -12.2822, 4.071 );
	endOverride =( 947.2, -10918, 64.9514 );

	assert( !isdefined( level.traceEnd ) );
	for( ;; )
	{
		// SCRIPTER_MOD
		// JesseS (3/20/2007): level.player changed to players[0]
		players = get_players();
		wait( 0.05 );
		start = startOverride;
		end = endOverride;
		if( !isdefined( startOverride ) )
		{
			start = level.traceStart;
		}
		if( !isdefined( endOverride ) )
		{
			end = players[0] geteye();
		}
			
		trace = bulletTrace( start, end, false, undefined );
		line( start, trace[ "position" ], ( 0.9, 0.5, 0.8 ), 0.5 );
	}	
}

hatmodel()
{
	/#
	for( ;; )
	{
		if( getdebugdvar( "debug_hatmodel" ) == "off" )
			return;
		noHat = [];
		ai = getaiarray();
		
		for( i=0;i<ai.size;i++ )
		{
			if( isdefined( ai[ i ].hatmodel ) )
				continue;
				
			alreadyKnown = false;
			for( p=0;p<noHat.size;p++ )
			{
				if( noHat[ p ] != ai[ i ].classname )
					continue;
				alreadyKnown = true;
				break;
			}
			if( !alreadyKnown )
				noHat[ noHat.size ] = ai[ i ].classname;
		}
		
		if( noHat.size )
		{
			println( " " );
			println( "The following AI have no Hatmodel, so helmets can not pop off on head-shot death:" );
			for( i=0;i<noHat.size;i++ )
				println( "Classname: ", noHat[ i ] );
			println( "To disable hatModel spam, type debug_hatmodel off" );
		}
		wait( 15 );
	}
	#/
}

 debug_character_count()
{
	/*
	//drones
	drones = NewDebugHudElem();
	drones.alignX = "left";
	drones.alignY = "middle";
	drones.x = 620;
	drones.y = 275;
	drones.label = &"DEBUG_DRONES";
	drones.alpha = 0;
	drones.fontscale = 1.2;
	
	//allies
	allies = NewDebugHudElem();
	allies.alignX = "left";
	allies.alignY = "middle";
	allies.x = 620;
	allies.y = 290;
	allies.label = &"DEBUG_ALLIES";
	allies.alpha = 0;
	allies.fontscale = 1.2;
	
	//allies
	axis = NewDebugHudElem();
	axis.alignX = "left";
	axis.alignY = "middle";
	axis.x = 620;
	axis.y = 305;
	axis.label = &"DEBUG_AXIS";
	axis.alpha = 0;
	axis.fontscale = 1.2;
	

	//vehicles
	vehicles = NewDebugHudElem();
	vehicles.alignX = "left";
	vehicles.alignY = "middle";
	vehicles.x = 620;
	vehicles.y = 320;
	vehicles.label = &"DEBUG_VEHICLES";
	vehicles.alpha = 0;
	vehicles.fontscale = 1.2;

	//total
	total = NewDebugHudElem();
	total.alignX = "left";
	total.alignY = "middle";
	total.x = 620;
	total.y = 335;
	total.label = &"DEBUG_TOTAL";
	total.alpha = 0;
	total.fontscale = 1.2;
	
	lastdvar = "off";
	for( ;; )
	{
		dvar = getdvar( "debug_character_count" );
		if( dvar == "off" )
		{
			if( dvar != lastdvar )
			{
				drones.alpha = 0;
				allies.alpha = 0;
				axis.alpha = 0;
				vehicles.alpha = 0;
				total.alpha = 0;
				lastdvar = dvar;
			}
			wait .25;
			continue;
		}
		else
		{
			if( dvar != lastdvar )
			{
				drones.alpha = 1;
				allies.alpha = 1;
				axis.alpha = 1;
				vehicles.alpha = 1;
				total.alpha = 1;
				lastdvar = dvar;

			}
		}

		//drones
		count_drones = getentarray( "drone", "targetname" ).size;
		drones setValue( count_drones );
		
		//allies
		count_allies = getaiarray( "allies" ).size;
		allies setValue( count_allies );
		
		//axis
		count_axis = getaiarray( "axis" ).size;
		axis setValue( count_axis );
		
		vehicles setValue( getentarray( "script_vehicle", "classname" ).size );
		
		//total
		total setValue( count_drones + count_allies + count_axis );
		
		wait 0.25;
	}
	*/
} 

debug_nuke()
{
	// CODER_MOD - no more level.player
	players = get_players();
	player = players[0];
	dvar = getdvar( "debug_nuke" );
	if ( dvar == "on" )
	{
	ai = getaispeciesarray( "axis", "all" );
	for( i=0;i<ai.size;i++ )
			ai[ i ] dodamage( 300, ( 0, 0, 0 ), player );
	}
	else
	if ( dvar == "ai" )
	{
		ai = getaiarray( "axis" );
		for( i=0;i<ai.size;i++ )
			ai[ i ] dodamage( 300, ( 0, 0, 0 ), player );
	}
	else
	if ( dvar == "dogs" )
	{
		ai = getaispeciesarray( "axis", "dog" );
		for( i=0;i<ai.size;i++ )
			ai[ i ] dodamage( 300, ( 0, 0, 0 ), player );
	}
	setdvar( "debug_nuke", "off" );
}

debug_missTime()
{
	
}

camera()
{
	wait( 0.05 );
	cameras = getentarray( "camera", "targetname" );
	for( i=0;i<cameras.size;i++ )
	{
		ent = getent( cameras[ i ].target, "targetname" );
		cameras[ i ].origin2 = ent.origin;
		cameras[ i ].angles = vectortoangles( ent.origin - cameras[ i ].origin );
	}
	for( ;; )
	{
		/#
		if( getdebugdvar( "camera" ) != "on" )
		{
			if( getdebugdvar( "camera" ) != "off" )
				setdvar( "camera", "off" );
			wait( 1 );
			continue;
		}
		#/
		
		ai = getaiarray( "axis" );
		if( !ai.size )
		{
			freePlayer();
			wait( 0.5 );
			continue;
		}
		cameraWithEnemy = [];
		for( i=0;i<cameras.size;i++ )
		{
			for( p=0;p<ai.size;p++ )
			{
				if( distance( cameras[ i ].origin, ai[ p ].origin ) > 256 )
					continue;
				cameraWithEnemy[ cameraWithEnemy.size ] = cameras[ i ];
				break;
			}
		}
		if( !cameraWithEnemy.size )
		{
			freePlayer();
			wait( 0.5 );
			continue;
		}

		cameraWithPlayer = [];
		for( i=0;i<cameraWithEnemy.size;i++ )
		{
			camera = cameraWithEnemy[ i ];
			
			start = camera.origin2;
			end = camera.origin;
			difference = vectorToAngles(( end[ 0 ], end[ 1 ], end[ 2 ] ) -( start[ 0 ], start[ 1 ], start[ 2 ] ) );
			angles =( 0, difference[ 1 ], 0 );
		    forward = anglesToForward( angles );
		
			// SCRIPTER_MOD
			// JesseS (3/20/2007): level.player changed to players[0]
			players = get_players();
			difference = vectornormalize(end - players[0].origin);
			dot = vectordot( forward, difference );
			if( dot < 0.85 )
				continue;
				
			cameraWithPlayer[ cameraWithPlayer.size ] = camera;
		}
		
		if( !cameraWithPlayer.size )
		{
			freePlayer();
			wait( 0.5 );
			continue;
		}
		
		// SCRIPTER_MOD
		// JesseS (3/20/2007): level.player changed to players[0]
		players = get_players();	
		dist = distance(players[0].origin, cameraWithPlayer[0].origin);
		newcam = cameraWithPlayer[ 0 ];
		for( i=1;i<cameraWithPlayer.size;i++ )
		{
			newdist = distance(players[0].origin, cameraWithPlayer[i].origin);
			if( newdist > dist )
				continue;
			
			newcam = cameraWithPlayer[ i ];
			dist = newdist;
		}
		
		setPlayerToCamera( newcam );
		wait( 3 );
	}
}
	
freePlayer()
{
	setdvar( "cl_freemove", "0" );
}

setPlayerToCamera( camera )
{
	setdvar( "cl_freemove", "2" );
	setdebugangles( camera.angles );
	setdebugorigin( camera.origin +( 0, 0, -60 ) );
}
/*
	maps\_spawner::waitframe();
	thread anglescheck();

	if( !isdefined( level.camera ) )
		return;

//	wait( 1 );
	mintime = 0;
	linker = false;
	while( getdvar( "camera" ) == "on" )
	{
		for( i=0;i<level.camera.size;i++ )
		{
			if( getdvar( "camera" ) != "on" )
				break;

			setdvar( "nextcamera", "on" );
			setdvar( "lastcamera", "on" );

                        players = get_players();
			players[0] setorigin (level.camera[i].origin);
			players[0] linkto (level.camera[i]);

			players[0] setplayerangles (level.camera[i].angles);

			timer = gettime() + 10000;
			if( timer < mintime )
				timer = mintime;

			oldorigin = players[0] getorigin();
			while( gettime() < timer )
			{
				if( gettime() > timer - 8000 )
				if ((gettime() > mintime) && (distance (players[0] getorigin(), oldorigin) > 128))
				{
					mintime = gettime() + 500000;
					timer = mintime;
				}

				if( getdvar( "camera" ) != "on" )
					break;

				if( getdvar( "nextcamera" ) == "1" )
					break;

				if( getdvar( "lastcamera" ) == "1" )
				{
					i-=2;
					if( i < 0 )
						i+=level.camera.size;
					break;
				}

				maps\_spawner::waitframe();
			}

			if(( getdvar( "nextcamera" ) == "1" ) ||( getdvar( "lastcamera" ) == "1" ) )
				mintime = gettime() + 500000;
		}
	}

	if( linker )
		players[0] unlink();
}
*/

anglescheck()
{
	while( 1 )
	{
		// SCRIPTER_MOD
		// JesseS (3/20/2007): level.player changed to players[0]	
		players = get_players();
		if( getdvar( "angles" ) == "1" )
		{
			println ("origin " + players[0] getorigin());
			println ("angles " + players[0].angles);
			setdvar( "angles", "0" );
		}
		wait( 1 );
	}
}

dolly()
{
	if( !isdefined( level.dollyTime ) )
		level.dollyTime = 5;
	setdvar( "dolly", "" );
	thread dollyStart();
	thread dollyEnd();
	thread dollyGo();
}

dollyStart()
{
	while( 1 )
	{
		// SCRIPTER_MOD
		// JesseS (3/20/2007): level.player changed to players[0]
		players = get_players();	
		if( getdvar( "dolly" ) == "start" )
		{
			level.dollystart = players[0].origin;
			setdvar( "dolly", "" );
		}
		wait( 1 );
	}
}

dollyEnd()
{
	while( 1 )
	{
		if( getdvar( "dolly" ) == "end" )
		{
			// SCRIPTER_MOD
			// JesseS (3/20/2007): level.player changed to players[0]
			players = get_players();
			level.dollyend = players[0].origin;
			setdvar( "dolly", "" );
		}
		wait( 1 );
	}
}

dollyGo()
{
	while( 1 )
	{
		// SCRIPTER_MOD
		// JesseS (3/20/2007): level.player changed to players[0]
		players = get_players();
			
		wait( 1 );
		if( getdvar( "dolly" ) == "go" )
		{
			setdvar( "dolly", "" );
			if( !isdefined( level.dollystart ) )
			{
				println( "NO Dolly Start!" );
				continue;
			}
			if( !isdefined( level.dollyend ) )
			{
				println( "NO Dolly End!" );
				continue;
			}

			org = spawn( "script_origin", ( 0, 0, 0 ) );
			org.origin = level.dollystart;
			players[0] setorigin (org.origin);
			players[0] linkto (org);

			org moveto( level.dollyend, level.dollyTime );
			wait( level.dollyTime );
			org delete();
		}
	}
}

deathspawnerPreview()
{
	waittillframeend;
	for( i=0;i<50;i++ )
	{
		if( !isdefined( level.deathspawnerents[ i ] ) )
			continue;
		array = level.deathspawnerents[ i ];
		for( p=0;p<array.size;p++ )
		{
			ent = array[ p ];
			if( isdefined( ent.truecount ) )
				print3d( ent.origin, i + ": " + ent.truecount, ( 0, 0.8, 0.6 ), 5 );
			else
				print3d( ent.origin, i + ": " +  ".", ( 0, 0.8, 0.6 ), 5 );
		}
	}
}


lastSightPosWatch()
{
	/#
	for( ;; )
	{
		wait( 0.05 );
		
		// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
		if( getdebugdvar( "replay_debug" ) == "1" )
			println("File: _debug.gsc. Function: lastSightPosWatch() - INNER LOOP START\n");
		
		num = getdvarint( "lastsightpos" );
		if( !num )
		{
			// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
			if( getdebugdvar( "replay_debug" ) == "1" )
				println("File: _debug.gsc. Function: lastSightPosWatch() - INNER LOOP END EARLY 1\n");
			continue;
		}
		
		guy = undefined;
		ai = getaiarray();
		for( i=0;i<ai.size;i++ )
		{
			if( ai[ i ] getentnum() != num )
				continue;
				
			guy = ai[ i ];
			break;
		}
		
		if( !isalive( guy ) )
		{
			// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
			if( getdebugdvar( "replay_debug" ) == "1" )
				println("File: _debug.gsc. Function: lastSightPosWatch() - INNER LOOP END EARLY 2\n");
			continue;
		}

		if( guy animscripts\utility::hasEnemySightPos() )
			org = guy animscripts\utility::getEnemySightPos();
		else
			org = undefined;
		
					
		for( ;; )
		{
			// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
			if( getdebugdvar( "replay_debug" ) == "1" )
				println("File: _debug.gsc. Function: lastSightPosWatch() - INNER INNER LOOP START\n");
			
			newnum = getdvarint( "lastsightpos" );
			if( num != newnum )
				break;
				
			if(( isalive( guy ) ) &&( guy animscripts\utility::hasEnemySightPos() ) )
				org = guy animscripts\utility::getEnemySightPos();
			
			if( !isdefined( org ) )
			{
				wait( 0.05 );
				continue;
			}
			
			range = 10;
			color =( 0.2, 0.9, 0.8 );
			line( org +( 0, 0, range ), org +( 0, 0, range * -1 ), color, 1.0 );
			line( org +( range, 0, 0 ), org +( range * -1, 0, 0 ), color, 1.0 );
			line( org +( 0, range, 0 ), org +( 0, range * -1, 0 ), color, 1.0 );
			
			// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
			if( getdebugdvar( "replay_debug" ) == "1" )
				println("File: _debug.gsc. Function: lastSightPosWatch() - INNER INNER LOOP END\n");
			
			wait( 0.05 );
		}
		
		// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
		if( getdebugdvar( "replay_debug" ) == "1" )
			println("File: _debug.gsc. Function: lastSightPosWatch() - INNER LOOP END\n");
	}
	#/
}

watchMinimap()
{
	precacheItem( "defaultweapon" );
	while( 1 )
	{
		updateMinimapSetting();
		wait .25;
	}
}

updateMinimapSetting()
{	
	// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
	/#
	if( getdebugdvar( "replay_debug" ) == "1" )
			println("File: _debug.gsc. Function: updateMinimapSetting()\n");
	#/
	
	// use 0 for no required map aspect ratio.
	requiredMapAspectRatio = getdvarfloat( "scr_requiredMapAspectRatio" );

	if( !isdefined( level.minimapheight ) ) {
		setdvar( "scr_minimap_height", "0" );
		level.minimapheight = 0;
	}
	minimapheight = getdvarfloat( "scr_minimap_height" );
	if( minimapheight != level.minimapheight )
	{
		if( isdefined( level.minimaporigin ) ) {
			level.minimapplayer unlink();
			level.minimaporigin delete();
			level notify( "end_draw_map_bounds" );
		}
		
		if( minimapheight > 0 )
		{
			level.minimapheight = minimapheight;
			
			// SCRIPTER_MOD
			// JesseS (3/20/2007): level.player changed to players[0]
			players = get_players();
			player = players[0];
			
			corners = getentarray( "minimap_corner", "targetname" );
			if( corners.size == 2 )
			{
				viewpos =( corners[ 0 ].origin + corners[ 1 ].origin );
				viewpos =( viewpos[ 0 ]*.5, viewpos[ 1 ]*.5, viewpos[ 2 ]*.5 );

				maxcorner =( corners[ 0 ].origin[ 0 ], corners[ 0 ].origin[ 1 ], viewpos[ 2 ] );
				mincorner =( corners[ 0 ].origin[ 0 ], corners[ 0 ].origin[ 1 ], viewpos[ 2 ] );
				if( corners[ 1 ].origin[ 0 ] > corners[ 0 ].origin[ 0 ] )
					maxcorner =( corners[ 1 ].origin[ 0 ], maxcorner[ 1 ], maxcorner[ 2 ] );
				else
					mincorner =( corners[ 1 ].origin[ 0 ], mincorner[ 1 ], mincorner[ 2 ] );
				if( corners[ 1 ].origin[ 1 ] > corners[ 0 ].origin[ 1 ] )
					maxcorner =( maxcorner[ 0 ], corners[ 1 ].origin[ 1 ], maxcorner[ 2 ] );
				else
					mincorner =( mincorner[ 0 ], corners[ 1 ].origin[ 1 ], mincorner[ 2 ] );
				
				viewpostocorner = maxcorner - viewpos;
				viewpos =( viewpos[ 0 ], viewpos[ 1 ], viewpos[ 2 ] + minimapheight );
				
				origin = spawn( "script_origin", player.origin );
				
				northvector =( cos( getnorthyaw() ), sin( getnorthyaw() ), 0 );
				eastvector =( northvector[ 1 ], 0 - northvector[ 0 ], 0 );
				disttotop = vectordot( northvector, viewpostocorner );
				if( disttotop < 0 )
					disttotop = 0 - disttotop;
				disttoside = vectordot( eastvector, viewpostocorner );
				if( disttoside < 0 )
					disttoside = 0 - disttoside;
				
				// extend map bounds to meet the required aspect ratio
				if( requiredMapAspectRatio > 0 )
				{
					mapAspectRatio = disttoside / disttotop;
					if( mapAspectRatio < requiredMapAspectRatio )
					{
						incr = requiredMapAspectRatio / mapAspectRatio;
						disttoside *= incr;
						addvec = vecscale( eastvector, vectordot( eastvector, maxcorner - viewpos ) *( incr - 1 ) );
						mincorner -= addvec;
						maxcorner += addvec;
					}
					else
					{
						incr = mapAspectRatio / requiredMapAspectRatio;
						disttotop *= incr;
						addvec = vecscale( northvector, vectordot( northvector, maxcorner - viewpos ) *( incr - 1 ) );
						mincorner -= addvec;
						maxcorner += addvec;
					}
				}
				
				if( level.console )
				{
					aspectratioguess = 16.0/9.0;
					// .8 would be .75 but it needs to be bigger because of safe area
					angleside = 2 * atan( disttoside * .8 / minimapheight );
					angletop = 2 * atan( disttotop * aspectratioguess * .8 / minimapheight );
				}
				else
				{
					aspectratioguess = 4.0/3.0;
					// multiply by 1.05 to give some margin to work with
					angleside = 2 * atan( disttoside * 1.05 / minimapheight );
					angletop = 2 * atan( disttotop * aspectratioguess * 1.05 / minimapheight );
				}
				if( angleside > angletop )
					angle = angleside;
				else
					angle = angletop;
				
				znear = minimapheight - 1000;
				if( znear < 16 ) znear = 16;
				if( znear > 10000 ) znear = 10000;

				player playerlinktoabsolute( origin );
				origin.origin = viewpos +( 0, 0, -62 );
				origin.angles =( 90, getnorthyaw(), 0 );
				
				// because some guns can mess up the field of view, require default weapon
				player GiveWeapon( "defaultweapon" );
				setsaveddvar( "cg_fov", angle );
				
				// Internal Dvar set: cg_drawgun - Internal Dvars cannot be changed by script. Use 'setsaveddvar' to alter SAVED internal dvars
				// setsaveddvar can only be called on dvars with the SAVED flag set
				// Error: "cg_drawgun" is not a valid dvar to set using setclientdvar
				
				level.minimapplayer = player;
				level.minimaporigin = origin;
				
				thread drawMiniMapBounds( viewpos, mincorner, maxcorner );
			}
			else
				println( "^1Error: There are not exactly 2 \"minimap_corner\" entities in the level." );
		}
	}
	// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
	/#
	if( getdebugdvar( "replay_debug" ) == "1" )
		println("File: _debug.gsc. Function: updateMinimapSetting() - COMPLETE\n");
	#/
}

getchains()
{
	chainarray = [];
	chainarray = getentarray( "minimap_line", "script_noteworthy" );
	array = [];
	for( i=0;i<chainarray.size;i++ )
	{
		array[ i ] = chainarray[ i ] getchain();
	}
	return array;
}

getchain()
{
	array = [];
	ent = self;
	while( isdefined( ent ) )
	{
		array[ array.size ] = ent;
		if( !isdefined( ent ) || !isdefined( ent.target ) )
			break;
		ent = getent( ent.target, "targetname" );
		if( isdefined( ent ) && ent == array[ 0 ] )
		{
			array[ array.size ] = ent;
			break;
		}
	}
	originarray = [];
	for( i=0;i<array.size;i++ )
		originarray[ i ] = array[ i ].origin;
	return originarray;
	
}

vecscale( vec, scalar )
{
	return( vec[ 0 ]*scalar, vec[ 1 ]*scalar, vec[ 2 ]*scalar );
}
drawMiniMapBounds( viewpos, mincorner, maxcorner )
{
	level notify( "end_draw_map_bounds" );
	level endon( "end_draw_map_bounds" );
	
	viewheight =( viewpos[ 2 ] - maxcorner[ 2 ] );
	
	diaglen = length( mincorner - maxcorner );

	mincorneroffset =( mincorner - viewpos );
	mincorneroffset = vectornormalize(( mincorneroffset[ 0 ], mincorneroffset[ 1 ], 0 ) );
	mincorner = mincorner + vecscale( mincorneroffset, diaglen * 1/800*0 );
	maxcorneroffset =( maxcorner - viewpos );
	maxcorneroffset = vectornormalize(( maxcorneroffset[ 0 ], maxcorneroffset[ 1 ], 0 ) );
	maxcorner = maxcorner + vecscale( maxcorneroffset, diaglen * 1/800*0 );
	
	north =( cos( getnorthyaw() ), sin( getnorthyaw() ), 0 );
	
	diagonal = maxcorner - mincorner;
	side = vecscale( north, vectordot( diagonal, north ) );
	sidenorth = vecscale( north, abs( vectordot( diagonal, north ) ) );
	
	corner0 = mincorner;
	corner1 = mincorner + side;
	corner2 = maxcorner;
	corner3 = maxcorner - side;
	
	toppos = vecscale( mincorner + maxcorner, .5 ) + vecscale( sidenorth, .51 );
	textscale = diaglen * .003;
	chains = getchains();

	
	while( 1 )
	{
		line( corner0, corner1 );
		line( corner1, corner2 );
		line( corner2, corner3 );
		line( corner3, corner0 );

		array_levelthread( chains, maps\_utility::plot_points );

		print3d( toppos, "This Side Up", ( 1, 1, 1 ), 1, textscale );
		
		wait .05;
	}
}


debug_vehiclesittags()
{
	vehicles = getentarray( "script_vehicle", "classname" );
	type = "none";
/#	type = getdebugdvar( "debug_vehiclesittags" );#/
	for( i=0;i<vehicles.size;i++ )
	{
//		if( !isdefined( level.vehicle_aianims[ vehicles[ i ].vehicletype ] )  || vehicles[ i ].vehicletype != type )
		if( !isdefined( level.vehicle_aianims[ vehicles[ i ].vehicletype ] ) )
			continue;
		
		anims = level.vehicle_aianims[ vehicles[ i ].vehicletype ];
		for( j=0;j<anims.size;j++ )
		{
			// SCRIPTER_MOD
			// JesseS (3/20/2007): level.player changed to players[0]
			players = get_players();
			
			if( isdefined( anims[ j ].sittag ) )
			{
				vehicles[ i ] thread drawtag( anims[ j ].sittag );
				org = vehicles[ i ] gettagorigin( anims[ j ].sittag );
				if(players[0] islookingatorigin(org))
					print3d( org+( 0, 0, 16 ), anims[ j ].sittag, ( 1, 1, 1 ), 1, 1 );
			}
		}
	}
}

islookingatorigin( origin )
{
	normalvec = vectorNormalize( origin-self getShootAtPos() );
	veccomp = vectorNormalize(( origin-( 0, 0, 24 ) )-self getShootAtPos() );
	insidedot = vectordot( normalvec, veccomp );
	
	anglevec = anglestoforward( self getplayerangles() );
	vectordot = vectordot( anglevec, normalvec );
	if( vectordot > insidedot )
		return true;
	else
		return false;
}

debug_colornodes()
{
	wait( 0.05 );
	ai = getaiarray();

	array = [];
	array[ "axis" ] = [];
	array[ "allies" ] = [];
	array[ "neutral" ] = [];
	for( i=0; i<ai.size; i++ )
	{
		guy = ai[ i ];
			
		if( !isdefined( guy.currentColorCode ) )
			continue;
			
		array[ guy.team ][ guy.currentColorCode ] = true;
		
		color =( 1, 1, 1 );
		if( isdefined( guy.script_forceColor ) )
			color = level.color_debug[ guy.script_forceColor ];

		print3d( guy.origin +( 0, 0, 50 ), guy.currentColorCode, color, 1, 1 );

		// axis dont do forcecolor behavior, they do follow the leader for force color
		if( guy.team == "axis" )
			continue;
		
		guy try_to_draw_line_to_node();
	}
	
	draw_colorNodes( array, "allies" );
	draw_colorNodes( array, "axis" );
}

draw_colorNodes( array, team )
{
	keys = getArrayKeys( array[ team ] );
	for( i=0; i<keys.size; i++ )
	{
		color =( 1, 1, 1 );
		// use the first letter of the key as the color
		color = level.color_debug[ getsubstr( keys[ i ], 0, 1 ) ];
	
		if( isdefined( level.colorNodes_debug_array[ team ][ keys[ i ] ] ) )
		{
			teamArray = level.colorNodes_debug_array[ team ][ keys[ i ] ];
			for( p=0; p < teamArray.size; p++ )
			{
				print3d( teamArray[ p ].origin, "N-" + keys[ i ], color, 1, 1 );
			}
		}
	}
}

get_team_substr()
{
	if( self.team == "allies" )
	{
		if( !isdefined( self.node.script_color_allies ) )
			return;
			
		return self.node.script_color_allies;
	}
	
	if( self.team == "axis" )
	{
		if( !isdefined( self.node.script_color_axis ) )
			return;
			
		return self.node.script_color_axis;
	}
}

try_to_draw_line_to_node()
{
	if( !isdefined( self.node ) )
		return;
		
	if( !isdefined( self.script_forceColor ) )
		return;
	
	substr = get_team_substr();
	if( !isdefined( substr ) )
		return;
		
	if( !issubstr( substr, self.script_forceColor ) )
		return;
		
	line( self.origin +( 0, 0, 64 ), self.node.origin, level.color_debug[ self.script_forceColor ] );
}

fogcheck()
{
	if( getdvar( "depth_close" ) == "" )
		setdvar( "depth_close", "0" );
		
	if( getdvar( "depth_far" ) == "" )
		setdvar( "depth_far", "1500" );
		
	close = getdvarint( "depth_close" );
	far = getdvarint( "depth_far" );
	setexpfog( close, far, 1, 1, 1, 0 );
}

debugThreat()
{
//	if( gettime() > level.last_threat_debug + 1000 )
	{
		level.last_threat_debug = gettime();
		thread debugThreatCalc();
	}
}

debugThreatCalc()
{
	// debug the threatbias from entities towards the specified ent
	/#
	ai = getaiarray();
	entnum = getdebugdvarint( "debug_threat" );
	entity = undefined;
	
	// SCRIPTER_MOD
	// JesseS (3/20/2007): level.player changed to players[0]
	// TODO: This one should be checked for co-op more closely
	players = get_players();
			
	if( entnum == 0 )
	{
		entity = players[0];
	}
	else
	{
		for( i=0; i < ai.size; i++ )
		{
			if( entnum != ai[ i ] getentnum() )
				continue;
			entity = ai[ i ];
			break;
		}
	}
	
	if( !isalive( entity ) )
		return;
	
	entityGroup = entity getthreatbiasgroup();
	array_thread( ai, ::displayThreat, entity, entityGroup );
	players[0] thread displayThreat( entity, entityGroup );
	#/
}

displayThreat( entity, entityGroup )
{
	self endon( "death" );

	if( self.team == entity.team )
	{
		return;
	}

	selfthreat = 0;		
	selfthreat+= self.threatBias;

	threat = 0;
	threat+= entity.threatBias;
	myGroup = undefined;

	if( isdefined( entityGroup ) )
	{
		myGroup = self getthreatbiasgroup();
		if( isdefined( myGroup ) )
		{
			threat += getthreatbias( entityGroup, myGroup );
			selfThreat += getthreatbias( myGroup, entityGroup );
		}
	}
	
	if( entity.ignoreme || threat < -900000 )
	{
		threat = "Ignore";
	}

	if( self.ignoreme || selfthreat < -900000 )
	{
		selfthreat = "Ignore";
	}
			
	// SCRIPTER_MOD
	// JesseS (3/20/2007): level.player changed to players[0]
	// TODO: This one should be checked for co-op more closely
	players = get_players();
			
	timer = 1*20;
	col =( 1, 0.5, 0.2 );
	col2 =( 0.2, 0.5, 1 );
	pacifist = self != players[0] && self.pacifist;
	
	for( i=0; i <= timer; i++ )
	{
		print3d( self.origin +( 0, 0, 65 ), "Him to Me:", col, 3 );
		print3d( self.origin +( 0, 0, 50 ), threat, col, 5 );
		if( isdefined( entityGroup ) )
		{
			print3d( self.origin +( 0, 0, 35 ), entityGroup, col, 2 );
		}

		print3d( self.origin +( 0, 0, 15 ), "Me to Him:", col2, 3 );
		print3d( self.origin +( 0, 0, 0 ), selfThreat, col2, 5 );
		if( isdefined( mygroup ) )
		{
			print3d( self.origin +( 0, 0, -15 ), mygroup, col2, 2 );
		}

		if( pacifist )
		{
			print3d( self.origin +( 0, 0, 25 ), "( Pacifist )", col2, 5 );
		}
		
		wait( 0.05 );
	}
}

debugColorFriendlies()
{
	level.debug_color_friendlies = [];
	level.debug_color_huds = [];
	
	for( ;; )
	{
		level waittill( "updated_color_friendlies" );
		draw_color_friendlies();
	}
}

draw_color_friendlies()
{
	level endon( "updated_color_friendlies" );
	keys = getarraykeys( level.debug_color_friendlies );
	
	colored_friendlies = [];
	colors = [];
	colors[ colors.size ] = "r";
	colors[ colors.size ] = "o";
	colors[ colors.size ] = "y";
	colors[ colors.size ] = "g";
	colors[ colors.size ] = "c";
	colors[ colors.size ] = "b";
	colors[ colors.size ] = "p";
	
	rgb = get_script_palette();
	

	for( i=0; i < colors.size; i++ )
	{
		colored_friendlies[ colors[ i ] ] = 0;
	}

	for( i=0; i < keys.size; i++ )
	{
		color = level.debug_color_friendlies[ keys[ i ] ];
		colored_friendlies[ color ]++;
	}
	
	for( i=0; i < level.debug_color_huds.size; i++ )
	{
		level.debug_color_huds[ i ] destroy();
	}
	level.debug_color_huds = [];

/#
	if( getdebugdvar( "debug_colorfriendlies" ) != "on" )
		return;
#/
		
	x = 15;
	y = 365;
	offset_x = 25;
	offset_y = 25;
	for( i=0; i < colors.size; i++ )
	{
		if( colored_friendlies[ colors[ i ] ] <= 0 )
			continue;
		for( p=0; p < colored_friendlies[ colors[ i ] ]; p++ )
		{
			overlay = newHudElem();
			overlay.x = x + 25*p;
			overlay.y = y;
			overlay setshader( "white", 16, 16 );
			overlay.alignX = "left";
			overlay.alignY = "bottom";
			overlay.alpha = 1;
			overlay.color = rgb[ colors[ i ] ];
			level.debug_color_huds[ level.debug_color_huds.size ] = overlay;
		}
		
		y += offset_y;
	}
}

playerNode()
{
	for( ;; )
	{
                players = get_players();
		if( isdefined( players[0].node ) )
			print3d( players[0].node.origin +( 0, 0, 25 ), "P-Node", ( 0.3, 1, 1 ), 1 );
			
		wait( 0.05 );
	}
}


drawUsers()
{
	if( isalive( self.color_user ) )
	{
		line( self.origin +( 0, 0, 35 ), self.color_user.origin +( 0, 0, 35 ), ( 1, 1, 1 ), 1.0 );
		
		print3d( self.origin +( 0, 0, -25 ), "in-use", ( 1, 1, 1 ), 1, 1 );
	}
}


debuggoalpos()
{
	for( ;; )
	{
		ai = getaiarray();
		array_thread( ai, ::view_goal_pos );
		wait( 0.05 );
	}
}

view_goal_pos()
{
	if( !isdefined( self.goalpos ) )
		return;

	line( self.origin +( 0, 0, 35 ), self.goalpos +( 0, 0, 35 ), ( 1, 1, 1 ), 1.0 );
}

colordebug()
{
	wait( 0.5 );
	col = [];
	col[ col.size ] = "r";
	col[ col.size ] = "g";
	col[ col.size ] = "b";
	col[ col.size ] = "y";
	col[ col.size ] = "o";
	col[ col.size ] = "p";
	col[ col.size ] = "c";
	
	for( ;; )
	{
		for( i=0; i < col.size; i++ )
		{
			color = level.currentColorForced[ "allies" ][ col[ i ] ];
			if( isdefined( color ) )
				draw_colored_nodes( color );
		}
		wait( 0.05 );
	}
}

draw_colored_nodes( color )
{
	nodes = level.arrays_of_colorCoded_nodes[ "allies" ][ color ];
	array_thread( nodes, ::drawUsers );
}

init_animSounds()
{
	level.animSounds = [];
	level.animSound_aliases = [];
	waittillframeend; // now we know _load has run and the level.scr_notetracks have been defined
	waittillframeend; // wait one extra frameend because _audio.gso files waittillframeend and we have to start after them
	
	animnames = getarraykeys( level.scr_notetrack );
	for( i=0; i < animnames.size; i++ )
	{
		init_notetracks_for_animname( animnames[ i ] );
	}
	
	animnames = getarraykeys( level.scr_animSound );
	for( i=0; i < animnames.size; i++ )
	{
		init_animSounds_for_animname( animnames[ i ] );
	}
}

init_notetracks_for_animname( animname )
{
	// copy all the scr_notetracks into animsound_aliases so they show up properly
	notetracks = getarraykeys( level.scr_notetrack[ animname ] );
	
	for( i=0; i < notetracks.size; i++ )
	{
		soundalias = level.scr_notetrack[ animname ][ i ][ "sound" ];
		if( !isdefined( soundalias ) )
			continue;
			
		anime = level.scr_notetrack[ animname ][ i ][ "anime" ];
		notetrack = level.scr_notetrack[ animname ][ i ][ "notetrack" ];
		level.animSound_aliases[ animname ][ anime ][ notetrack ][ "soundalias" ] = soundalias;
		if( isdefined( level.scr_notetrack[ animname ][ i ][ "created_by_animSound" ] ) )
		{
			level.animSound_aliases[ animname ][ anime ][ notetrack ][ "created_by_animSound" ] = true;
		}
	}
}

init_animSounds_for_animname( animname )
{
	// copy all the scr_animSounds into animsound_aliases so they show up properly
	animes = getarraykeys( level.scr_animSound[ animname ] );
	
	for( i=0; i < animes.size; i++ )
	{
		anime = animes[ i ];
		soundalias = level.scr_animSound[ animname ][ anime ];
		level.animSound_aliases[ animname ][ anime ][ "#" + anime ][ "soundalias" ] = soundalias;
		level.animSound_aliases[ animname ][ anime ][ "#" + anime ][ "created_by_animSound" ] = true;
	}
}

add_hud_line( x, y, msg )
{
	hudelm = newHudElem();
	hudelm.alignX = "left";
	hudelm.alignY = "middle";
	hudelm.x = x;
	hudelm.y = y;
	hudelm.alpha = 1;
	hudelm.fontScale = 1;
	hudelm.label = msg;
	level.animsound_hud_extralines[ level.animsound_hud_extralines.size ] = hudelm;
	return hudelm;
}

debug_animSound()
{
	/#
	enabled = getdebugdvar( "animsound" ) == "on";
	if( !isdefined( level.animsound_hud ) )
	{
		if( !enabled )
			return;
			
		// init the related variables
		level.animsound_selected = 0;
		level.animsound_input = "none";
		level.animsound_hud = [];
		level.animsound_hud_timer = [];
		level.animsound_hud_alias = [];
		level.animsound_hud_extralines = [];

		level.animsound_locked = false;
		level.animsound_locked_pressed = false;

		level.animsound_hud_animname = add_hud_line( -30, 180, "Actor: " );
		level.animsound_hud_anime = add_hud_line( 100, 180, "Anim: " );

		add_hud_line( 10, 190, "Notetrack or label" );
		add_hud_line( -30, 190, "Elapsed" );
		add_hud_line( -30, 160, "Del: Delete selected soundalias" );
		add_hud_line( -30, 150, "F12: Lock selection" );
		add_hud_line( -30, 140, "Add a soundalias with /tag alias or /tag# alias" );

		level.animsound_hud_locked = add_hud_line( -30, 170, "*LOCKED*" );
		level.animsound_hud_locked.alpha = 0;
		
		for( i=0; i < level.animsound_hudlimit; i++ )
		{
			hudelm = newHudElem();
			hudelm.alignX = "left";
			hudelm.alignY = "middle";
			hudelm.x = 10;
			hudelm.y = 200 + i*10;
			hudelm.alpha = 1;
			hudelm.fontScale = 1;
			hudelm.label = "";
			level.animsound_hud[ level.animsound_hud.size ] = hudelm;

			hudelm = newHudElem();
			hudelm.alignX = "right";
			hudelm.alignY = "middle";
			hudelm.x = -10;
			hudelm.y = 200 + i*10;
			hudelm.alpha = 1;
			hudelm.fontScale = 1;
			hudelm.label = "";
			level.animsound_hud_timer[ level.animsound_hud_timer.size ] = hudelm;

			hudelm = newHudElem();
			hudelm.alignX = "right";
			hudelm.alignY = "middle";
			hudelm.x = 210;
			hudelm.y = 200 + i*10;
			hudelm.alpha = 1;
			hudelm.fontScale = 1;
			hudelm.label = "";
			level.animsound_hud_alias[ level.animsound_hud_alias.size ] = hudelm;
		}

		// selected is yellow
		level.animsound_hud[ 0 ].color =( 1, 1, 0 );
		level.animsound_hud_timer[ 0 ].color =( 1, 1, 0 );
	}
	else
	if( !enabled )
	{
		// animsound got turned off so delete the hud stuff
		for( i=0; i < level.animsound_hudlimit; i++ )
		{
			level.animsound_hud[ i ] destroy();
			level.animsound_hud_timer[ i ] destroy();
			level.animsound_hud_alias[ i ] destroy();
		}
		
		for( i=0; i < level.animsound_hud_extralines.size; i++ )
		{
			level.animsound_hud_extralines[ i ] destroy();
		}
		
		level.animsound_hud = undefined;
		level.animsound_hud_timer = undefined;
		level.animsound_hud_alias = undefined;
		level.animsound_hud_extralines = undefined;
		level.animSounds = undefined;
		return;
	}

	if( !isdefined( level.animsound_tagged ) )
		level.animsound_locked = false;

	if( level.animsound_locked )
		level.animsound_hud_locked.alpha = 1;
	else
		level.animsound_hud_locked.alpha = 0;
	
	if( !isdefined( level.animSounds ) )
		init_animSounds();
	
	/*
	if( !isdefined( level.anim_sound_was_opened ) )
	{
		thread test_animsound_file();
	}
	*/

	level.animSounds_thisframe = [];
	level.animSounds = remove_undefined_from_array( level.animSounds );
	array_thread( level.animSounds, ::display_animSound );

	players = get_players();
	
	if( level.animsound_locked )
	{
		for( i=0; i < level.animSounds_thisframe.size; i++ )
		{
			animSound = level.animSounds_thisframe[ i ];
			animSound.animsound_color =( 0.5, 0.5, 0.5 );
		}
	}
	else if ( players.size > 0 )
	{
		dot = 0.85;
                
		forward = anglestoforward( players[0] getplayerangles() );
		for( i=0; i < level.animSounds_thisframe.size; i++ )
		{
			animSound = level.animSounds_thisframe[ i ];
			animSound.animsound_color =( 0.25, 1.0, 0.5 );
						
			difference = vectornormalize(( animSound.origin +( 0, 0, 40 ) ) -( players[0].origin +( 0, 0, 55 ) ) );
			newdot = vectordot( forward, difference );
			if( newdot < dot )
				continue;
	
			dot = newdot;
			level.animsound_tagged = animSound;
		}
	}

	if( isdefined( level.animsound_tagged ) )
	{
		level.animsound_tagged.animsound_color =( 1.0, 1.0, 0.0 );
	}

	is_tagged = isdefined( level.animsound_tagged );
	for( i=0; i < level.animSounds_thisframe.size; i++ )
	{
		animSound = level.animSounds_thisframe[ i ];
		scale = 1;
		/*
		soundalias = get_alias_from_stored( animSound );
		scale = 0.9;
		
		if( is_tagged && level.animsound_tagged == animSound )
			scale = 1;
			
		if( isdefined( soundalias ) )
		{
			if( is_from_animsound( animSound.animname, animSound.anime, animSound.notetrack ) )
			{
				print3d( animSound.origin, animSound.notetrack + " " + soundalias, animSound.color, 1, scale );
			}
			else
			{
				// put in a * so they know its unchangeable
				print3d( animSound.origin, animSound.notetrack + " *" + soundalias, animSound.color, 1, scale );
			}
		}
		else
		{
			print3d( animSound.origin, animSound.notetrack, animSound.color, 1, scale );
		}
		*/
		msg = "*";
		if( level.animsound_locked )
			msg = "*LOCK";
		print3d( animSound.origin +( 0, 0, 40 ), msg + animSound.animsounds.size, animSound.animsound_color, 1, scale );
	}
	
	if( is_tagged )
	{
		draw_animsounds_in_hud();		
	}
	#/
}

draw_animsounds_in_hud()
{
	// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
	/#
	if( getdebugdvar( "replay_debug" ) == "1" )
		println("File: _debug.gsc. Function: draw_animsounds_in_hud()\n");
	#/
		
	guy = level.animsound_tagged;
	animsounds = guy.animSounds;

	animname = "generic";
	if ( isdefined( guy.animname ) )
		animname = guy.animname;
	level.animsound_hud_animname.label = "Actor: " + animname;
	
        players = get_players();
	if( players[0] buttonPressed( "f12" ) )
	{
		if( !level.animsound_locked_pressed )
		{
			level.animsound_locked = !level.animsound_locked;
			level.animsound_locked_pressed = true;
		}
	}
	else
	{
		level.animsound_locked_pressed = false;
	}

	if( players[0] buttonPressed( "UPARROW" ) )
	{
		if( level.animsound_input != "up" )
		{
			level.animsound_selected--;
		}
		
		level.animsound_input = "up";
	}
	else
	if( players[0] buttonPressed( "DOWNARROW" ) )
	{
		if( level.animsound_input != "down" )
		{
			level.animsound_selected++;
		}
		
		level.animsound_input = "down";
	}
	else
		level.animsound_input = "none";

	// clear out the hudelems	
	for( i=0; i < level.animsound_hudlimit; i++ )
	{
		hudelm = level.animsound_hud[ i ];
		hudelm.label = "";
		hudelm.color =( 1, 1, 1 );
		hudelm = level.animsound_hud_timer[ i ];
		hudelm.label = "";
		hudelm.color =( 1, 1, 1 );
		hudelm = level.animsound_hud_alias[ i ];
		hudelm.label = "";
		hudelm.color =( 1, 1, 1 );
	}

	// get the highest existing animsound on the guy
	keys = getarraykeys( animsounds );
	highest = -1;
	for( i=0; i < keys.size; i++ )
	{
		if( keys[ i ] > highest )
			highest = keys[ i ];
	}
	if( highest == -1 )
		return;
		
	if( level.animsound_selected > highest )
		level.animsound_selected = highest;
	if( level.animsound_selected < 0 )
		level.animsound_selected = 0;

	// make sure the selected one exists
	for( ;; )
	{
		if( isdefined( animsounds[ level.animsound_selected ] ) )
			break;
		
		level.animsound_selected--;
		if( level.animsound_selected < 0 )
			level.animsound_selected = highest;
	}

	level.animsound_hud_anime.label = "Anim: " + animsounds[ level.animsound_selected ].anime;
	
	level.animsound_hud[ level.animsound_selected ].color =( 1, 1, 0 );
	level.animsound_hud_timer[ level.animsound_selected ].color =( 1, 1, 0 );
	level.animsound_hud_alias[ level.animsound_selected ].color =( 1, 1, 0 );
	
	time = gettime();
	for( i=0; i < keys.size; i++ )
	{
		key = keys[ i ];
		animsound = animsounds[ key ];
		hudelm = level.animsound_hud[ key ];
		soundalias = get_alias_from_stored( animSound );
		hudelm.label =( key + 1 ) + ". " + animsound.notetrack;

		hudelm = level.animsound_hud_timer[ key ];
		hudelm.label = int(( time -( animsound.end_time - 60000 ) ) * 0.001 );

		if( isdefined( soundalias ) )
		{
			hudelm = level.animsound_hud_alias[ key ];
			hudelm.label = soundalias;
			if( !is_from_animsound( animSound.animname, animSound.anime, animSound.notetrack ) )
			{
				hudelm.color =( 0.7, 0.7, 0.7 );
			}
		}
	}
	
	players = get_players();
	if( players[0] buttonPressed( "del" ) )
	{
		// delete a sound on a guy
		animsound = animsounds[ level.animsound_selected ];
		soundalias = get_alias_from_stored( animsound );
		if( !isdefined( soundalias ) )
			return;
		
		if( !is_from_animsound( animSound.animname, animSound.anime, animSound.notetrack ) )
			return;

		level.animSound_aliases[ animSound.animname ][ animSound.anime ][ animSound.notetrack ] = undefined;
		debug_animSoundSave();
	}
	
	// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
	/#
	if( getdebugdvar( "replay_debug" ) == "1" )
		println("File: _debug.gsc. Function: draw_animsounds_in_hud() - COMPLETE\n");
	#/
}

get_alias_from_stored( animSound )
{
	if( !isdefined( level.animSound_aliases[ animSound.animname ] ) )
		return;
		
	if( !isdefined( level.animSound_aliases[ animSound.animname ][ animSound.anime ] ) )
		return;
		
	if( !isdefined( level.animSound_aliases[ animSound.animname ][ animSound.anime ][ animSound.notetrack ] ) )
		return;
	return level.animSound_aliases[ animSound.animname ][ animSound.anime ][ animSound.notetrack ][ "soundalias" ];
}

is_from_animsound( animname, anime, notetrack )
{
	return isdefined( level.animSound_aliases[ animname ][ anime ][ notetrack ][ "created_by_animSound" ] );
}

/*
test_animsound_file()
{
	level.anim_sound_was_opened = true;
	
	/#
	filename = "createfx/" + level.script + "_audio.gsc";
	for( ;; )
	{
		warning = newHudElem();
		warning.alignX = "left";
		warning.alignY = "middle";
		warning.x = 10;
		warning.y = 150;
		warning.alpha = 0;
		warning.fontScale = 2;
		warning.label = filename + " is not open for edit, so you can not save your work. ";
		
		for( ;; )
		{
			file = openfile( filename, "write" );
			if( file != -1 )
				break;
			wait( 5 );
		}
	
		warning destroy();
		break;
	}
	
	closefile( file );
	#/
}
*/

display_animSound()
{
        players = get_players();
	if( distance( players[0].origin, self.origin ) > 1500 )
		return;

	level.animSounds_thisframe[ level.animSounds_thisframe.size ] = self;

	/*
	timer = gettime();
	keys = getarraykeys( self.animSounds );
	for( i=0; i < keys.size; i++ )
	{
		key = keys[ i ];
		animSound = self.animSounds[ key ];
		if( !isdefined( animSound ) )
			continue;
		
		if( timer > animSound.end_time )
		{
			self.animSounds[ key ] = undefined;
			continue;
		}
		
		animSound.origin = self.origin +( 0, 0, 50 + 10 * key );
		level.animSounds_thisframe[ level.animSounds_thisframe.size ] = animSound;
	}
	*/
}

debug_animSoundTag( tagnum )
{
	/#
	tag = getdvar( "tag" + tagnum );
	if( tag == "" )
	{
		iprintlnbold( "Enter the soundalias with /tag# aliasname" );
		return;
	}

	tag_sound( tag, tagnum - 1 );
	
	setdvar( "tag" + tagnum, "" );
	#/
}

debug_animSoundTagSelected()
{
	/#
	// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
	if( getdebugdvar( "replay_debug" ) == "1" )
		println("File: _debug.gsc. Function: debug_animSoundTagSelected()\n");
	
	tag = getdvar( "tag" );
	if( tag == "" )
	{
		iprintlnbold( "Enter the soundalias with /tag aliasname" );
		
		// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
		if( getdebugdvar( "replay_debug" ) == "1" )
			println("File: _debug.gsc. Function: debug_animSoundTagSelected() - COMPLETE EARLY\n");
		return;
	}

	tag_sound( tag, level.animsound_selected );
	
	setdvar( "tag", "" );
	
	// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
	if( getdebugdvar( "replay_debug" ) == "1" )
		println("File: _debug.gsc. Function: debug_animSoundTagSelected() - COMPLETE\n");
	#/
}

tag_sound( tag, tagnum )
{
	if( !isdefined( level.animsound_tagged ) )
		return;
	if( !isdefined( level.animsound_tagged.animsounds[ tagnum ] ) )
		return;

	animSound = level.animsound_tagged.animsounds[ tagnum ];
	// store the alias to the array of aliases
	soundalias = get_alias_from_stored( animSound );
	if( !isdefined( soundalias ) || is_from_animsound( animSound.animname, animSound.anime, animSound.notetrack ) )
	{
		level.animSound_aliases[ animSound.animname ][ animSound.anime ][ animSound.notetrack ][ "soundalias" ] = tag;
		level.animSound_aliases[ animSound.animname ][ animSound.anime ][ animSound.notetrack ][ "created_by_animSound" ] = true;
		debug_animSoundSave();
	}
}

debug_animSoundSave()
{
	/*
	tab = "     ";
	filename = "createfx/"+level.script+"_fx.gsc";
	file = openfile( filename, "write" );
	assertex( file != -1, "File not writeable( maybe you should check it out ): " + filename );
	cfxprintln( file, "//_createfx generated. Do not touch!!" );
	cfxprintln( file, "main()" );
	cfxprintln( file, "{" );
	*/
	
	/#
	filename = "createfx/" + level.script + "_audio.gsc";
	file = openfile( filename, "write" );
	if( file == -1 )
	{
		iprintlnbold( "Couldn't write to " + filename + ", make sure it is open for edit." );
		return;
	}

	iprintlnbold( "Saved to " + filename );
	print_aliases_to_file( file );
	saved = closefile( file );
	setdvar( "animsound_save", "" );
	#/
}

print_aliases_to_file( file )
{
	tab = "    ";
	fprintln( file, "#include maps\\_anim;" );
	fprintln( file, "main()" );
	fprintln( file, "{" );
	fprintln( file, tab + "// Autogenerated by AnimSounds. Threaded off so that it can be placed before _load( has to create level.scr_notetrack first )." );
	fprintln( file, tab + "thread init_animsounds();" );
	fprintln( file, "}" );
	fprintln( file, "" );
	fprintln( file, "init_animsounds()" );
	fprintln( file, "{" );
	fprintln( file, tab + "waittillframeend;" );

	animnames = getarraykeys( level.animSound_aliases );
	for( i=0; i < animnames.size; i++ )
	{
		animes = getarraykeys( level.animSound_aliases[ animnames[ i ] ] );
		for( p=0; p < animes.size; p++ )
		{
			anime = animes[ p ];
			notetracks = getarraykeys( level.animSound_aliases[ animnames[ i ] ][ anime ] );
			for( z=0; z < notetracks.size; z++ )
			{
				notetrack = notetracks[ z ];
				if( !is_from_animsound( animnames[ i ], anime, notetrack ) )
					continue;
				
				alias = level.animSound_aliases[ animnames[ i ] ][ anime ][ notetrack ][ "soundalias" ];

				if( notetrack == "#" + anime )
				{
					// this isn't really a notetrack, its from the _anim call.
					fprintln( file, tab + "addOnStart_animSound( " + tostr( animnames[ i ] ) + ", " + tostr( anime ) + ", " + tostr( alias ) + " ); " );
				}
				else
				{
					// this is attached to a notetrack					
					fprintln( file, tab + "addNotetrack_animSound( " + tostr( animnames[ i ] ) + ", " + tostr( anime ) + ", " + tostr( notetrack ) + ", " + tostr( alias ) + " ); " );
				}
				println( "^1Saved alias ^4" + alias + "^1 to notetrack ^4" + notetrack );
			}
		}
	}
	fprintln( file, "}" );
}

tostr( str )
{
	newstr = "\"";
	for( i=0; i < str.size; i++ )
	{
		if( str[ i ] == "\"" )
		{
			newstr += "\\";
			newstr += "\"";
			continue;
		}
		
		newstr += str[ i ];
	}
	newstr += "\"";
	return newstr;
}
linedraw( start, end, color, timer )
{
	if ( !isdefined( color ) )
		color = (1,1,1);
		
	if ( isdefined( timer ) )
	{
		timer *= 20;
		for ( i = 0; i < timer; i++ )
		{
			line( start, end, color );
			wait( 0.05 );
		}
	}
	else
	{
		for ( ;; )
		{
			line( start, end, color );
			wait( 0.05 );
		}
	}
}

print3ddraw( org, text, color )
{
	for ( ;; )
	{
		print3d( org, text, color );
		wait( 0.05 );
	}
}

complete_me()
{
	if( getdvar("credits_active") == "1" )
	{
		wait 7;
		setdvar( "credits_active", "0" ); 
		maps\_endmission::credits_end();
		return;
	}
	wait 7;
	nextmission();
}


debug_bayonet()
{
/*
	x = 0;
	y = 20;
	menu_name = "bayonet_menu";

	menu_bkg = new_hud( menu_name, undefined, x, y, 1 );
	menu_bkg SetShader( "white", 220, 100 );
	menu_bkg.alignX = "left";
	menu_bkg.alignY = "top";
	menu_bkg.sort = 10;
	menu_bkg.alpha = 0.6;	
	menu_bkg.color = ( 0.0, 0.0, 0.5 );
	
	menu[0] = new_hud( menu_name, "Weapon Name:",		x + 5, y + 10, 1 );
	menu[1] = new_hud( menu_name, "Bayonet Weapon:",	x + 5, y + 20, 1 );
	menu[2] = new_hud( menu_name, "Melee Time:",		x + 5, y + 30, 1 );
	menu[3] = new_hud( menu_name, "Melee Anim:",		x + 5, y + 40, 1 );
	menu[4] = new_hud( menu_name, "Charge Time:",	x + 5, y + 50, 1 );
	menu[5] = new_hud( menu_name, "Charge Anim:",	x + 5, y + 60, 1 );
	menu[6] = new_hud( menu_name, "Bayonet Damage:",	x + 5, y + 70, 1 );
	menu[7] = new_hud( menu_name, "Melee Range",		x + 5, y + 80, 1 );
	menu[8] = new_hud( menu_name, "Aim Assist Range:",	x + 5, y + 90, 1 );

	thread debug_bayonet_think( menu_name, menu_bkg, menu, x, y );
*/
}

debug_bayonet_think( menu_name, menu_bkg, menu, x, y )
{
	x_offset = 90;

	// enum
	menu_weapon_name					= menu.size;
	menu_bayonet_weapon					= menu.size + 1;
	menu_bayonet_melee_time				= menu.size + 2;
	menu_bayonet_melee_anim				= menu.size + 3;
	menu_bayonet_charge_time			= menu.size + 4;
	menu_bayonet_charge_anim			= menu.size + 5;
	menu_bayonet_damage					= menu.size + 6;
	menu_bayonet_range					= menu.size + 7;
	menu_bayonet_aim_range				= menu.size + 8;
		
	menu[menu_weapon_name]				= new_hud( menu_name, "", x + x_offset, y + 10, 1 );
	menu[menu_bayonet_weapon]			= new_hud( menu_name, "", x + x_offset, y + 20, 1 );
	menu[menu_bayonet_melee_time]		= new_hud( menu_name, "", x + x_offset, y + 30, 1 );
	menu[menu_bayonet_melee_anim]		= new_hud( menu_name, "", x + x_offset, y + 40, 1 );
	menu[menu_bayonet_charge_time]		= new_hud( menu_name, "", x + x_offset, y + 50, 1 );
	menu[menu_bayonet_charge_anim]		= new_hud( menu_name, "", x + x_offset, y + 60, 1 );
	menu[menu_bayonet_damage]			= new_hud( menu_name, "", x + x_offset, y + 70, 1 );
	menu[menu_bayonet_range]			= new_hud( menu_name, "", x + x_offset, y + 80, 1 );
	menu[menu_bayonet_aim_range]		= new_hud( menu_name, "", x + x_offset, y + 90, 1 );

	while( true )
	{
		if( getdvar("debug_bayonet") == "0" )
		{
			menu_bkg.alpha = 0;

			for( i = 0; i < menu.size; i++ )
				menu[i].alpha = 0;
		}
		else
		{
			player = get_players()[0];

			if( IsDefined( player ) )
			{
				// weapon name
				weapon = player GetCurrentWeapon();
				menu[menu_weapon_name] setText( weapon );

				// bayonet weapon
				info_array = weaponbayonetinfo( weapon );

				if ( info_array.size > 0 )
				{
					menu[menu_bayonet_weapon]		setText( "true" );
					menu[menu_bayonet_melee_time]	setText( info_array[0] + " ms" );
					menu[menu_bayonet_melee_anim]	setText( info_array[1] );
					menu[menu_bayonet_charge_time]	setText( info_array[2] + " ms" );
					menu[menu_bayonet_charge_anim]	setText( info_array[3] );	
					menu[menu_bayonet_damage]		setText( info_array[4] );	
					menu[menu_bayonet_range]		setText( info_array[5] );	
					menu[menu_bayonet_aim_range]	setText( info_array[6] );
				}
				else
				{
					menu[menu_bayonet_weapon] setText( "false" );

					for( i = menu_bayonet_melee_time; i < menu.size; i++ )
						menu[i] setText( "" );
				}

				menu_bkg.alpha = 0.6;

				for( i = 0; i < menu.size; i++ )
					menu[i].alpha = 1;
			}
		}

		wait( 1 );
	}
}

new_hud( hud_name, msg, x, y, scale )
{
	if( !IsDefined( level.hud_array ) )
	{
		level.hud_array = [];
	}

	if( !IsDefined( level.hud_array[hud_name] ) )
	{
		level.hud_array[hud_name] = [];
	}

	hud = maps\_createmenu::set_hudelem( msg, x, y, scale );
	level.hud_array[hud_name][level.hud_array[hud_name].size] = hud;
	return hud;
}

debug_show_viewpos()
{
	/* wait_for_first_player();

	hud_title = NewDebugHudElem();
	hud_title.x = 10;
	hud_title.y = 300;
	hud_title.alpha = 0; 
	hud_title.alignX = "left";
	hud_title.fontscale = 1.2;
	hud_title SetText( &"DEBUG_POSITION" );

	x_pos = hud_title.x + 50;

	hud_x = NewDebugHudElem();
	hud_x.x = x_pos;
	hud_x.y = 300;
	hud_x.alpha = 0; 
	hud_x.alignX = "left";
	hud_x.fontscale = 1.2;
	hud_x SetValue( 0 );

	hud_y = NewDebugHudElem();
	hud_y.x = 10;
	hud_y.y = 300;
	hud_y.alpha = 0; 
	hud_y.alignX = "left";
	hud_y.fontscale = 1.2;
	hud_y SetValue( 0 );

	hud_z = NewDebugHudElem();
	hud_z.x = 10;
	hud_z.y = 300;
	hud_z.alpha = 0; 
	hud_z.alignX = "left";
	hud_z.fontscale = 1.2;
	hud_z SetValue( 0 );

	SetDvar( "debug_show_viewpos", "0" );

	players = get_players();
	while( 1 )
	{
		if( GetDvarInt( "debug_show_viewpos" ) > 0 )
		{
			hud_title.alpha = 1;
			hud_x.alpha = 1;
			hud_y.alpha = 1;
			hud_z.alpha = 1;

			x = players[0].origin[0];
			y = players[0].origin[1];
			z = players[0].origin[2];

			spacing1 = ( ( 2 + number_before_decimal( x ) ) * 8 ) + 10;
			spacing2 = ( ( 2 + number_before_decimal( y ) ) * 8 ) + 10;

			hud_y.x = x_pos + spacing1;
			hud_z.x = x_pos + spacing1 + spacing2;

			hud_x SetValue( round_to( x, 100 ) );
			hud_y SetValue( round_to( y, 100 ) );
			hud_z SetValue( round_to( z, 100 ) );
		}
		else
		{
			hud_title.alpha = 0;
			hud_x.alpha = 0;
			hud_y.alpha = 0;
			hud_z.alpha = 0;
		}

		wait( 0.5 );
	} */
}

number_before_decimal( num )
{
	abs_num = abs( num );
	count = 0;
	while( 1 )
	{
		abs_num *= 0.1; // Really doing num / 10
		count += 1;

		if( abs_num < 1 )
		{
			return count;
		}
	}
}

round_to( val, num ) 
{
	return Int( val * num ) / num; 
}

// MikeD (3/11/2008): New feature to show where you are at in the level during a playthrough
set_event_printname( text, focus )
{
/#
	level thread set_event_printname_thread( text, focus );
#/
}

clear_event_printname()
{
/#
	if( GetDvarInt( "loc_warnings" ) > 0 )
	{
		return;
	}

	level notify( "stop_event_name_thread" );

	if( IsDefined( level.event_hudelem ) )
	{
		level.event_hudelem Destroy();
	}
#/
}

set_event_printname_thread( text, focus )
{
/#
	level notify( "stop_event_name_thread" );
	level endon( "stop_event_name_thread" );

	// We don't localize these, so don't get out! :P
	if( GetDvarInt( "loc_warnings" ) > 0 )
	{
		return;
	}

	if( !isDefined( focus ) )
	{
		focus = false;	
	}

	suffix = "";
	if( focus )
	{
		suffix = " [Focus Event]";
	}

	SetDvar( "cg_zoneName", text );
	text = "Event: " + text + suffix;

	if( !IsDefined( level.event_hudelem ) )
	{
		hud = NewHudElem();
		hud.horzAlign = "center";
		hud.alignX = "center";
		hud.alignY = "top";
		hud.foreground = 1;
		hud.fontScale = 1.5;
		hud.sort = 50;
		hud.alpha = 1;
		hud.y = 15;

		level.event_hudelem = hud;
	}

	if( focus )
	{
		level.event_hudelem.color = ( 1, 1, 0 );
	}
	else
	{
		level.event_hudelem.color = ( 1, 1, 1 );
	}

	if( GetDvar( "debug_draw_event" ) == "" )
	{
		SetDvar( "debug_draw_event", "1" );
	}

	level.event_hudelem SetText( text );

	enabled = true;
	while( 1 )
	{
		toggle = false;

		if( GetDvarInt( "debug_draw_event" ) < 1 )
		{
			toggle = true;
			enabled = false;
		}
		else if( GetDvarInt( "debug_draw_event" ) > 0 )
		{
			toggle = true;
			enabled = true;
		}

		if( toggle && enabled )
		{
			level.event_hudelem.alpha = 1;
		}
		else if( toggle )
		{
			level.event_hudelem.alpha = 0;
		}

		wait( 0.5 );
	}
#/
}

// -- SRS 3/19/08: engagement distance debug.  only works for P1 --

get_playerone()
{
	return get_players()[0];
}

// this controls the engagement distance debug stuff with a dvar
engagement_distance_debug_toggle()
{
	/#
	level endon( "kill_engage_dist_debug_toggle_watcher" );
	
	lastState = GetDebugDvarInt( "debug_engage_dists" );
	
	while( 1 )
	{
		currentState = GetDebugDvarInt( "debug_engage_dists" );
		
		if( dvar_turned_on( currentState ) && !dvar_turned_on( lastState ) )
		{
			// turn it on
			weapon_engage_dists_init();
			thread debug_realtime_engage_dist();
			thread debug_ai_engage_dist();
			
			lastState = currentState;
		}
		else if( !dvar_turned_on( currentState ) && dvar_turned_on( lastState ) )
		{
			// send notify to turn off threads
			level notify( "kill_all_engage_dist_debug" );
			
			lastState = currentState;
		}
		
		wait( 0.3 );
	}
	#/
}

dvar_turned_on( val )
{
	if( val <= 0 )
	{
		return false;
	}
	else
	{
		return true;
	}
}

engagement_distance_debug_init()
{
	// set up debug stuff
	level.debug_xPos = -50;
	level.debug_yPos = 250;
	level.debug_yInc = 18;
	
	level.debug_fontScale = 1.5;
	
	level.white = ( 1, 1, 1 );
	level.green = ( 0, 1, 0 );
	level.yellow = ( 1, 1, 0 );
	level.red = ( 1, 0, 0 );
	
	level.realtimeEngageDist = NewHudElem();
	level.realtimeEngageDist.alignX = "left";
	level.realtimeEngageDist.fontScale = level.debug_fontScale;
	level.realtimeEngageDist.x = level.debug_xPos;
	level.realtimeEngageDist.y = level.debug_yPos;
	level.realtimeEngageDist.color = level.white;
	level.realtimeEngageDist SetText( "Current Engagement Distance: " );
	
	xPos = level.debug_xPos + 207;
	
	level.realtimeEngageDist_value = NewHudElem();
	level.realtimeEngageDist_value.alignX = "left";
	level.realtimeEngageDist_value.fontScale = level.debug_fontScale;
	level.realtimeEngageDist_value.x = xPos;
	level.realtimeEngageDist_value.y = level.debug_yPos;
	level.realtimeEngageDist_value.color = level.white;
	level.realtimeEngageDist_value SetValue( 0 );
	
	xPos += 37;
	
	level.realtimeEngageDist_middle = NewHudElem();
	level.realtimeEngageDist_middle.alignX = "left";
	level.realtimeEngageDist_middle.fontScale = level.debug_fontScale;
	level.realtimeEngageDist_middle.x = xPos;
	level.realtimeEngageDist_middle.y = level.debug_yPos;
	level.realtimeEngageDist_middle.color = level.white;
	level.realtimeEngageDist_middle SetText( " units, SHORT/LONG by " );
	
	xPos += 105;
	
	level.realtimeEngageDist_offvalue = NewHudElem();
	level.realtimeEngageDist_offvalue.alignX = "left";
	level.realtimeEngageDist_offvalue.fontScale = level.debug_fontScale;
	level.realtimeEngageDist_offvalue.x = xPos;
	level.realtimeEngageDist_offvalue.y = level.debug_yPos;
	level.realtimeEngageDist_offvalue.color = level.white;
	level.realtimeEngageDist_offvalue SetValue( 0 );
	
	hudObjArray = [];
	hudObjArray[0] = level.realtimeEngageDist;
	hudObjArray[1] = level.realtimeEngageDist_value;
	hudObjArray[2] = level.realtimeEngageDist_middle;
	hudObjArray[3] = level.realtimeEngageDist_offvalue;
	
	return hudObjArray;
}

engage_dist_debug_hud_destroy( hudArray, killNotify )
{
	level waittill( killNotify );
	
	for( i = 0; i < hudArray.size; i++ )
	{
		hudArray[i] Destroy();
	}
}

weapon_engage_dists_init()
{
	level.engageDists = [];
	
	// first pass ok
	genericPistol = spawnstruct();
	genericPistol.engageDistMin = 125;
	genericPistol.engageDistOptimal = 225;
	genericPistol.engageDistMulligan = 50;  // range around the optimal value that is still optimal
	genericPistol.engageDistMax = 400;
	
	// first pass ok
	shotty = spawnstruct();
	shotty.engageDistMin = 50;
	shotty.engageDistOptimal = 200;
	shotty.engageDistMulligan = 75;
	shotty.engageDistMax = 350;
	
	// first pass ok
	genericSMG = spawnstruct();
	genericSMG.engageDistMin = 100;
	genericSMG.engageDistOptimal = 275;
	genericSMG.engageDistMulligan = 100;
	genericSMG.engageDistMax = 500;
	
	// first pass NEED TEST
	genericLMG = spawnstruct();
	genericLMG.engageDistMin = 325;
	genericLMG.engageDistOptimal = 550;
	genericLMG.engageDistMulligan = 150;
	genericLMG.engageDistMax = 850;
	
	// first pass ok
	genericRifleSA = spawnstruct();
	genericRifleSA.engageDistMin = 325;
	genericRifleSA.engageDistOptimal = 550;
	genericRifleSA.engageDistMulligan = 150;
	genericRifleSA.engageDistMax = 850;
	
	// first pass ok
	genericRifleBolt = spawnstruct();
	genericRifleBolt.engageDistMin = 350;
	genericRifleBolt.engageDistOptimal = 600;
	genericRifleBolt.engageDistMulligan = 150;
	genericRifleBolt.engageDistMax = 900;
	
	// first pass NEED TEST
	genericHMG = spawnstruct();
	genericHMG.engageDistMin = 390;
	genericHMG.engageDistOptimal = 600;
	genericHMG.engageDistMulligan = 100;
	genericHMG.engageDistMax = 900;
	
	// first pass ok
	genericSniper = spawnstruct();
	genericSniper.engageDistMin = 950;
	genericSniper.engageDistOptimal = 1700;
	genericSniper.engageDistMulligan = 300;
	genericSniper.engageDistMax = 3000;
	
	// Pistols
	engage_dists_add( "colt", genericPistol );
	engage_dists_add( "sw_357", genericPistol );
	engage_dists_add( "nambu", genericPistol );
	engage_dists_add( "tokarev", genericPistol );
	engage_dists_add( "walther", genericPistol );
	
	// SMGs
	engage_dists_add( "thompson", genericSMG );
	engage_dists_add( "type100_smg", genericSMG );
	engage_dists_add( "ppsh", genericSMG );
	engage_dists_add( "mp40", genericSMG );
	engage_dists_add( "stg44", genericSMG );
	engage_dists_add( "sten", genericSMG );
	engage_dists_add( "sten_silenced", genericSMG );
	
	// shotgun
	engage_dists_add( "shotgun", shotty );
	
	// LMGs
	engage_dists_add( "bar", genericLMG );
	engage_dists_add( "bar_bipod", genericLMG );
	engage_dists_add( "type99_lmg", genericLMG );
	engage_dists_add( "type99_lmg_bipod", genericLMG );
	engage_dists_add( "dp28", genericLMG );
	engage_dists_add( "dp28_bipod", genericLMG );
	engage_dists_add( "fg42", genericLMG );
	engage_dists_add( "fg42_bipod", genericLMG );
	engage_dists_add( "bren", genericLMG );
	engage_dists_add( "bren_bipod", genericLMG );
	
	// Rifles (semiautomatic)
	engage_dists_add( "m1garand", genericRifleSA );
	engage_dists_add( "m1garand_bayonet", genericRifleSA );
	engage_dists_add( "m1carbine", genericRifleSA );
	engage_dists_add( "m1carbine_bayonet", genericRifleSA );
	engage_dists_add( "svt40", genericRifleSA );
	engage_dists_add( "gewehr43", genericRifleSA );
	
	// Rifles (bolt-action)
	engage_dists_add( "springfield", genericRifleBolt );
	engage_dists_add( "springfield_bayonet", genericRifleBolt );
	engage_dists_add( "type99_rifle", genericRifleBolt );
	engage_dists_add( "type99_rifle_bayonet", genericRifleBolt );
	engage_dists_add( "mosin_rifle", genericRifleBolt );
	engage_dists_add( "mosin_rifle_bayonet", genericRifleBolt );
	engage_dists_add( "kar98k", genericRifleBolt );
	engage_dists_add( "kar98k_bayonet", genericRifleBolt );
	engage_dists_add( "lee_enfield", genericRifleBolt );
	engage_dists_add( "lee_enfield_bayonet", genericRifleBolt );
	
	// HMGs
	engage_dists_add( "30cal", genericHMG );
	engage_dists_add( "30cal_bipod", genericHMG );
	engage_dists_add( "mg42", genericHMG );
	engage_dists_add( "mg42_bipod", genericHMG );
	
	// Sniper Rifles
	engage_dists_add( "springfield_scoped", genericSniper );
	engage_dists_add( "type99_rifle_scoped", genericSniper );
	engage_dists_add( "mosin_rifle_scoped", genericSniper );
	engage_dists_add( "kar98k_scoped", genericSniper );
	engage_dists_add( "fg42_scoped", genericSniper );
	engage_dists_add( "lee_enfield_scoped", genericSniper );
	
	// start waiting for weapon changes
	level thread engage_dists_watcher();
}

engage_dists_add( weapontypeStr, values )
{
	level.engageDists[weapontypeStr] = values;
}

// returns a script_struct, or undefined, if the lookup failed
get_engage_dists( weapontypeStr )
{
	if( IsDefined( level.engageDists[weapontypeStr] ) )
	{
		return level.engageDists[weapontypeStr];
	}
	else
	{
		return undefined;
	}
}

// checks currently equipped weapon to make sure that engagement distance values are correct
engage_dists_watcher()
{
	level endon( "kill_all_engage_dist_debug" );
	level endon( "kill_engage_dists_watcher" );
	
	while( 1 )
	{
		player = get_playerone();
		playerWeapon = player GetCurrentWeapon();
		
		if( !IsDefined( player.lastweapon ) )
		{
			player.lastweapon = playerWeapon;
		}
		else
		{
			if( player.lastweapon == playerWeapon )
			{
				wait( 0.05 );
				continue;
			}
		}
		
		values = get_engage_dists( playerWeapon );
		
		if( IsDefined( values ) )
		{
			level.weaponEngageDistValues = values;
		}
		else
		{
			level.weaponEngageDistValues = undefined;
		}
		
		player.lastweapon = playerWeapon;
		
		wait( 0.05 );
	}
}

debug_realtime_engage_dist()
{
	level endon( "kill_all_engage_dist_debug" );
	level endon( "kill_realtime_engagement_distance_debug" );
	
	hudObjArray = engagement_distance_debug_init();
	level thread engage_dist_debug_hud_destroy( hudObjArray, "kill_all_engage_dist_debug" );
	
	level.debugRTEngageDistColor = level.green;
	
	player = get_playerone();
	
	while( 1 )
	{
		lastTracePos = ( 0, 0, 0 );
		
		// Trace to where the player is looking
		direction = player GetPlayerAngles();
		direction_vec = AnglesToForward( direction );
		eye = player GetEye();
		
		trace = BulletTrace( eye, eye + vector_multiply( direction_vec, 10000 ), true, player );
		tracePoint = trace["position"];
		traceNormal =  trace["normal"];
		traceDist = int( Distance( eye, tracePoint ) );  // just need an int, thanks
		
		if( tracePoint != lastTracePos )
		{
			lastTracePos = tracePoint;
			
			if( !IsDefined( level.weaponEngageDistValues ) )
			{
				hudobj_changecolor( hudObjArray, level.white );
				hudObjArray engagedist_hud_changetext( "nodata", tracedist );
			}
			else
			{
				// for convenience
				engageDistMin = level.weaponEngageDistValues.engageDistMin;
				engageDistOptimal = level.weaponEngageDistValues.engageDistOptimal;
				engageDistMulligan = level.weaponEngageDistValues.engageDistMulligan;
				engageDistMax = level.weaponEngageDistValues.engageDistMax;
						
				// if inside our engagement distance range...
				if( ( traceDist >= engageDistMin ) && ( traceDist <= engageDistMax ) )
				{
					// if in the optimal range...
					if( ( traceDist >= ( engageDistOptimal - engageDistMulligan ) )
					&& ( traceDist <= ( engageDistOptimal + engageDistMulligan ) ) )
					{
						hudObjArray engagedist_hud_changetext( "optimal", tracedist );
						hudobj_changecolor( hudObjArray, level.green );
					}
					else
					{
						hudObjArray engagedist_hud_changetext( "ok", tracedist );
						hudobj_changecolor( hudObjArray, level.yellow );
					}
				}
				else if( traceDist < engageDistMin )
				{
					hudobj_changecolor( hudObjArray, level.red );
					hudObjArray engagedist_hud_changetext( "short", tracedist );
				}
				else if( traceDist > engageDistMax )
				{
					hudobj_changecolor( hudObjArray, level.red );
					hudObjArray engagedist_hud_changetext( "long", tracedist );
				}
			}
		}
		
		// draw our trace spot
		// plot_circle_fortime(radius1,radius2,time,color,origin,normal)
		thread plot_circle_fortime( 1, 5, 0.05, level.debugRTEngageDistColor, tracePoint, traceNormal );
		thread plot_circle_fortime( 1, 1, 0.05, level.debugRTEngageDistColor, tracePoint, traceNormal );
		
		wait( 0.05 );
	}
}

hudobj_changecolor( hudObjArray, newcolor )
{
	for( i = 0; i < hudObjArray.size; i++ )
	{
		hudObj = hudObjArray[i];
		
		if( hudObj.color != newcolor )
		{
			hudObj.color = newcolor;
			level.debugRTEngageDistColor = newcolor;
		}
	}
}

// self = an array of hud objects
engagedist_hud_changetext( engageDistType, units )
{
	if( !IsDefined( level.lastDistType ) )
	{
		level.lastDistType = "none";
	}
	
	if( engageDistType == "optimal" )
	{
		self[1] SetValue( units );
		self[2] SetText( "units: OPTIMAL!" );
		self[3].alpha = 0;
	}
	else if( engageDistType == "ok" )
	{
		self[1] SetValue( units );
		self[2] SetText( "units: OK!" );
		self[3].alpha = 0;
	}
	else if( engageDistType == "short" )
	{
		amountUnder = level.weaponEngageDistValues.engageDistMin - units;
		self[1] SetValue( units );
		self[3] SetValue( amountUnder );
		self[3].alpha = 1;
		
		if( level.lastDistType != engageDistType )
		{
			self[2] SetText( "units: SHORT by " );
		}
	}
	else if( engageDistType == "long" )
	{
		amountOver = units - level.weaponEngageDistValues.engageDistMax;
		self[1] SetValue( units );
		self[3] SetValue( amountOver );
		self[3].alpha = 1;
		
		if( level.lastDistType != engageDistType )
		{
			self[2] SetText( "units: LONG by " );
		}
	}
	else if( engageDistType == "nodata" )
	{
		self[1] SetValue( units );
		self[2] SetText( " units: (NO CURRENT WEAPON VALUES)" );
		self[3].alpha = 0;
	}
	
	level.lastDistType = engageDistType;
}

// draws print3ds above enemy AI heads to show contact distances
debug_ai_engage_dist()
{
	level endon( "kill_all_engage_dist_debug" );
	level endon( "kill_ai_engagement_distance_debug" );
	
	player = get_playerone();
	
	while( 1 )
	{
		axis = GetAIArray( "axis" );
		
		if( IsDefined( axis ) && axis.size > 0 )
		{	
			playerEye = player GetEye();
		
			for( i = 0; i < axis.size; i++ )
			{
				ai = axis[i];
				aiEye = ai GetEye();
				
				if( SightTracePassed( playerEye, aiEye, false, player ) )
				{
					dist = Distance( playerEye, aiEye );
					
					drawColor = level.white;
					drawString = "-";
					
					if( !IsDefined( level.weaponEngageDistValues ) )
					{
						drawColor = level.white;
					}
					else
					{
						// for convenience
						engageDistMin = level.weaponEngageDistValues.engageDistMin;
						engageDistOptimal = level.weaponEngageDistValues.engageDistOptimal;
						engageDistMulligan = level.weaponEngageDistValues.engageDistMulligan;
						engageDistMax = level.weaponEngageDistValues.engageDistMax;
								
						// if inside our engagement distance range...
						if( ( dist >= engageDistMin ) && ( dist <= engageDistMax ) )
						{
							// if in the optimal range...
							if( ( dist >= ( engageDistOptimal - engageDistMulligan ) )
							&& ( dist <= ( engageDistOptimal + engageDistMulligan ) ) )
							{
								drawColor = level.green;
								drawString = "RAD";
							}
							// else it's just ok
							else
							{
								drawColor = level.yellow;
								drawString = "MEH";
							}
						}
						else if( dist < engageDistMin )
						{
							drawColor = level.red;
							drawString = "BAD";
						}
						else if( dist > engageDistMax )
						{
							drawColor = level.red;
							drawString = "BAD";
						}
					}		
					
					scale = dist / 525;
					Print3d( ai.origin + ( 0, 0, 67 ), drawString, drawColor, 1, scale );
				}
			}
		}
		
		wait( 0.05 );
	}
}

// draws a circle in script
plot_circle_fortime(radius1,radius2,time,color,origin,normal)
{
	if(!isdefined(color))
		color = (0,1,0);
	hangtime = .05;
	circleres = 6;
	hemires = circleres/2;
	circleinc = 360/circleres;
	circleres++;
	plotpoints = [];

	rad = 0.00;
	timer = gettime()+(time*1000);
	radius = radius1;

	while(gettime()<timer)
	{
		// radius = radius1+((radius2-radius1)*(1-((timer-gettime())/(time*1000))));
		radius = radius2;
		angletoplayer = vectortoangles(normal);
		for(i=0;i<circleres;i++)
		{
			plotpoints[plotpoints.size] = origin+vector_multiply(anglestoforward((angletoplayer+(rad,90,0))),radius);
			rad+=circleinc;
		}
		maps\_utility::plot_points(plotpoints,color[0],color[1],color[2],hangtime);
		plotpoints = [];
		wait hangtime;
	}
}

// -- end engagement distance debug --

// -- dynamic AI spawning --
// This allows guys to be spawned in whereever. Need an enemy spawner in the map,
// or a specially designated spawner.
dynamic_ai_spawner()
{
	if (!isdefined(level.debug_dynamic_ai_spawner))
	{
		dynamic_ai_spawner_init();
		level.debug_dynamic_ai_spawner = true;
	}
	
	get_players()[0] thread spawn_guy_placement(level.enemy_spawner); 
	
	// Cleanup hudelems, dummy models, etc.
	level waittill ("kill dynamic spawning");
	
	if (isdefined(level.dynamic_spawn_hud))
	{
		level.dynamic_spawn_hud destroy();
	}
	
	if (isdefined(level.dynamic_spawn_dummy_model))
	{
		level.dynamic_spawn_dummy_model delete();
	}

}

dynamic_ai_spawner_init()
{
	level.enemy_spawner = dynamic_ai_spawner_create_enemy_spawner();
	if (!isdefined(level.enemy_spawner))
	{
		return;
	}
		
}

// We want to create a spawner here eventually, but grabbing one in the map will
// do for now.
dynamic_ai_spawner_create_enemy_spawner()
{
	spawners = getspawnerarray();
	
	for (i = 0; i < spawners.size; i++)
	{
		if (isdefined(spawners[i].targetname) && issubstr(spawners[i].targetname, "debug_spawner"))
		{
			enemy_spawner = spawners[i];
			enemy_spawner.script_forcespawn = 1;
			return enemy_spawner;
		}
	}
	
	// if we made it through, find any enemy spawner and use him
	for (i = 0; i < spawners.size; i++)
	{
		if (issubstr(spawners[i].classname, "jap") || issubstr(spawners[i].classname, "ger"))
		{
			enemy_spawner = spawners[i];
			enemy_spawner.script_forcespawn = 1;		// we want to make sure he will spawn in
			return enemy_spawner;
		}
	}
}

// Where to spawn the AI
spawn_guy_placement(spawner)
{	
	level endon ("kill dynamic spawning");
	
	if (!isdefined(spawner))
	{
		assertEX( isDefined ( spawner ) , "No spawners in the level!");
		return;
	}
	
	level.dynamic_spawn_hud = NewClientHudElem(get_players()[0]);
	level.dynamic_spawn_hud.alignX = "right";
	level.dynamic_spawn_hud.x = 110;
	level.dynamic_spawn_hud.y = 225;
	level.dynamic_spawn_hud.fontscale = 2;
		
	level.dynamic_spawn_hud settext("Press X to spawn AI");
	
	level.dynamic_spawn_dummy_model = spawn("script_model",(0,0,0));
	level.dynamic_spawn_dummy_model setmodel("defaultactor");
	
	wait 0.1;
	
	for (;;)
	{
		// Trace to where the player is looking
		direction = self getPlayerAngles();
		direction_vec = anglesToForward( direction );
		eye = self getEye();

		// offset 2 units on the Z to fix the bug where it would drop through the ground sometimes
		trace = bullettrace( eye, eye + vector_multiply( direction_vec , 8000 ), 0, undefined );

		dist = distance (eye, trace["position"]);		
		position = eye + vector_multiply( direction_vec , (dist - 64) );
			
		// debug		
		//thread draw_line_for_time( eye - (1,1,1), position, 1, 0, 0, 0.05 );
			
		spawner.origin = position;
		spawner.angles = self.angles+ (0,180,0);
				
		level.dynamic_spawn_dummy_model.origin = position;
		level.dynamic_spawn_dummy_model.angles = self.angles+ (0,180,0);
				
		self spawn_anywhere(spawner);

		wait (0.05);
	}
}
 
spawn_anywhere(spawner)
{
	// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
	/#
	if( getdebugdvar( "replay_debug" ) == "1" )
		println("File: _debug.gsc. Function: spawn_anywhere()\n");
	#/
	
	level endon ("kill dynamic spawning");
	
	if(self UseButtonPressed())
	{
		spawn = spawner spawn_ai();

		if ( spawn_failed( spawn ) )
		{
			assertex( 0, "spawn failed from spawn anywhere guy" );
			return;			
		}
		wait 0.4;	
	}
	spawner.count = 50;

	// CODER_MOD: Bryce (05/08/08): Useful output for debugging replay system
	/#
	if( getdebugdvar( "replay_debug" ) == "1" )
		println("File: _debug.gsc. Function: spawn_anywhere() - COMPLETE\n");
	#/
}
// -- end dynamic AI spawning --
