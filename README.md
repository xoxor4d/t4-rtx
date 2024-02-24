<h1 align="center">CoD-WaW (cod5) rtx-remix compatibility mod</h1>

<div align="center" markdown="1"> 

This client modification is made to make the game compatible with nvidia's [rtx-remix](https://github.com/NVIDIAGameWorks/rtx-remix).  
How? By manually reimplementing fixed function rendering :) 


It does __not__ come with a 'rtx mod' -> meaning no custom models nor materials.  

A demo mod can be found over at modDB: https://www.moddb.com/mods/cod5-nacht-der-untoten-demo  
^ Footage: https://www.youtube.com/watch?v=hCv3p1qT3q0

<h3 align="center">fully sp / mp compatible</h3>
<br>

</div>


<div align="center" markdown="1">
	
![img](img/01.jpg)
	


If you want to buy me a coffee:

[![ko-fi](https://xoxor4d.github.io/assets/img/social/kofi.png)](https://ko-fi.com/xoxor4d)
</div>


# Usage

#### A. Easy way but might not feature the latest and greatest:
  1. Download the latest [release](https://github.com/xoxor4d/t4-rtx/releases) and extract the contents into your CoDWaW root directory.  
  > The in-game console should show `t4-rtx-version >` if the dll was loaded correctly.  
  If not, look at the [troubleshoot section](#t4-rtx-is-not-being-loaded) further down.  

<br>

2) Read the `Dvars / Console Commands` and `Current issues` sections

<br>

#### B. Manual way but always up-to-date:

1) Install the latest full rtx-remix release (0.4.0 at this time)   
https://github.com/NVIDIAGameWorks/rtx-remix/tags

<br>

2) (Optional) Install the latest `github action` builds of:  
remix bridge - https://github.com/NVIDIAGameWorks/bridge-remix/actions  
remix runtime - https://github.com/NVIDIAGameWorks/dxvk-remix/actions  

<br>

3) Download the latest t4-rtx `github actions` build:  
  Release-binaries-t4-rtx - https://github.com/xoxor4d/t4-rtx/actions 
  - Drop `nvapi.dll` and the contents of the `assets` folder into your CoDWaW root directory and start the game.  
  The in-game console should show `t4-rtx-version >` if the dll was loaded correctly.

<br>

4) Read the `Dvars / Console Commands` and `Current issues` sections

<br>
<br>

## Dvars / Console Commands to further tweak the game to your liking:

> 🔸 skybox and fog settings can be tweaked per map by using `map_settings.ini` found in the `t4rtx` folder 🔸

- use console commands starting with `rtx_sky_` to spawn/change the skybox
- `rtx_disable_world_culling` :: tweak culling to your needs (set to `less` by default)
- `r_aspectRatio` :: **AUTO** is truly auto now and supports any aspect ratio

- use console commands `noborder` & `windowed` to switch between borderless and normal windowed mode  
(set dvars `vid_xpos 0` and `vid_ypos 0` to put the window in the top left corner)

- use console command `unlockall` to unlock all sp missions



<br>
<br>

## Advanced Settings:

- A. Commandline Arguments:  
  - `-no_default_sky` :: disable spawning of a default sky on each map
  - `-stock_effects` :: render effects using shaders
  - `-no_fog` :: disable fog
  - `-no_forced_lod` :: do not set `r_forceLod` to `high` by default  
  - `-load_normal_spec` :: enable loading of specular and normal maps (if you need to for some reason)
  
  > eg: &ensp;`c:\path\CoDWaWmp.exe -no_default_sky -stock_effects` 

<br>

- B. Dvars: 
    - `r_forceLod` :: force all models to a specific LOD (highest by default)
	- `r_lodScaleRigid` :: adjust static model draw distances (lower values increase draw distance)
	- `fx_cull_elem_draw_radius` :: fx elements inside this radius are not affected by culling (needs `fx_cull_elem_draw`)
	- `rtx_sky_follow_player` :: sky follows the player to prevent culling issues
	- `rtx_disable_entity_culling` :: disable culling of game entities
    - `r_warm_dpvs` :: disable all culling if true (not recommended - use `rtx_disable_world_culling`)
    - `r_warm_static` :: unlimited draw distance for static models if true (not recommended, use `r_lodScaleRigid`)

<br>

- C. Console commands:  
  - `export_entities` :: export map entities and static models (misc_models) to `cod5root/t4rtx/entity-export/mapname.map`  

<br>
<br>

## ⚠️ Current issues:
- fullscreen, changing resolution or anything else that requires the game to restart the renderer might crash the game
- effects will slow down the game (really depends on the effect and the amount - use `fx_enable 0` or `fx_cull_` dvars to adjust to your liking - marking certain effect textures as _ignore_ might help as well) 

<br>

## 💭 Troubleshoot:
#### game wont load or shows an error on startup 
- make sure you are on the latest version (1.7)
- disable your onboard gpu (BIOS)
- download a [debug build](https://github.com/xoxor4d/t4-rtx/actions) and watch for descriptive errors in the external console
- try to run the game in windowed mode: 
  - use commandline arguments like `c:\path\CoDWaW.exe +set r_fullscreen 0`
  - or edit your player config (`appdata/local/Activision/CoDWaW/players/yourprofile`)

#### t4-rtx is not being loaded
1. download [asiloader](https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases) (the non x64 variant)
2. rename the downloaded file to `dsound.dll` and place it into the cod5 root folder
3. rename `nvapi.dll` to `nvapi.asi` or any other name as long as it ends with `.asi`
4. start the game

#### other issues
- captures loaded in the toolkit dont work correctly (UTF8 error), look [here](https://github.com/xoxor4d/t4-rtx/wiki/Home-%E2%80%90-General-Tips-%E2%80%90-Troubleshooting-%E2%80%90-etc)
- black main menu? Make sure shadermodel is not set to 2.0 

<br>
<br>

## Compiling yourself:
1. setup `COD5_ROOT` environment variable with path to your cod5 directory (optional)
2. run `generate-buildfiles_vs22.bat` to generate build files (build folder)
3. compile and copy `nvapi.dll` (renamed t4-rtx.dll) to your CoDWaW root folder  
   - ^ automatically happens if you setup `COD5_ROOT`

<br>

## Questions? 
- join the [rtx-remix showcase](https://discord.gg/j6sh7JD3v9) discord and check out the cod5 thread within the `remix-projects` channel.
- join the [iw3xo](https://discord.gg/t5jRGbj) discord if you have questions related to this modification
- rtx-remix: https://github.com/NVIDIAGameWorks/rtx-remix  

Not required but makes life easier:   
https://community.pcgamingwiki.com/files/file/714-call-of-duty-world-at-war-replacement-steam-multiplayer-executable/  



<br>

## Credits
- [people of the showcase discord](https://discord.gg/j6sh7JD3v9) (for testing, feedback and ideas)
- [ineedbots/JezuzLizard](https://github.com/JezuzLizard/T4SP-Server-Plugin) (for most of the structs)
- [nvidia - rtx-remix](https://github.com/NVIDIAGameWorks/rtx-remix)

<br>
<br>

<div align="center" markdown="1">
	
![img](img/03.jpg)
![img](img/02.jpg)
	

</div>
