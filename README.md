<h1 align="center">CoD-WaW (cod5) rtx-remix compatibility mod</h1>

<div align="center" markdown="1"> 

This client modification is made to make the game compatible with nvidia's [rtx-remix](https://github.com/NVIDIAGameWorks/rtx-remix).  
It does __not__ come with a 'rtx mod' -> meaning no custom models nor materials.  

A demo mod can be found over at modDB: https://www.moddb.com/mods/cod5-nacht-der-untoten-demo  
Footage: https://www.youtube.com/watch?v=hCv3p1qT3q0

<br>

</div>

<div align="center" markdown="1">
	
![img](img/01.jpg)
	
</div>

<h3 align="center">sp / mp compatible (sp is mostly feature complete - mp has basic support)</h3>
<br>

# Usage

#### A. Easy way but might not feature the latest and greatest:
  1. Download the latest [release](https://github.com/xoxor4d/t4-rtx/releases) and extract the contents into your CoDWaW root directory.  
  The in-game console should show `t4-rtx-version >` if the dll was loaded correctly

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

- use console commands starting with `rtx_sky_` to spawn/change the skybox
- `r_lodScaleRigid` :: (1) adjust static model draw distances (lower values increase draw distance)
- `r_aspectRatio` :: (AUTO) is truly auto now and supports any aspect ratio
- use console commands `noborder` & `windowed` to switch between borderless and normal windowed mode  
(set dvars `vid_xpos 0` and `vid_ypos 0` to put the window in the top left corner)
- `fx_cull_elem_draw` :: (0) to not cull effects based on viewing direction

> + fog settings can be tweaked using `fog.ini` found in the `t4rtx` folder

Visit the Wiki section for more in-depth info: https://github.com/xoxor4d/t4-rtx/wiki/Home-%E2%80%90-General-Tips-%E2%80%90-Troubleshooting-%E2%80%90-etc


<br>
<br>

## Advanced Settings:

- A. Commandline Arguments:  
  - `-no_default_sky` :: disable spawning of a default sky on each map
  - `-stock_effects` :: render effects using shaders
  - `-no_fog` :: disable fog
  > eg: &ensp;`c:\path\CoDWaWmp.exe -no_default_sky -stock_effects` 

  - Notes: 
    - `r_warm_static` :: (1) force static models to LOD0
    - `r_warm_dpvs` :: (1) disable all culling 

<br>

- B. Console commands:  
  - `set export_entities 1` to export map entities and static models (misc_models) to `cod5root/t4rtx/entity-export/mapname.map`  

<br>



<br>

## Current issues:
- changing resolution or anything else that requires the game to semi-restart might crash the game
  > use commandline arguments like `c:\path\CoDWaWmp.exe +set r_fullscreen 0` or edit your player config (`appdata/local/Activision/CoDWaW/players/yourprofile`)
- captures taken with the remix runtime currently need to be captured in `USDA` format so that they can be edited with a text editor
  > see: https://github.com/xoxor4d/t4-rtx/wiki/Home-%E2%80%90-General-Tips-%E2%80%90-Troubleshooting-%E2%80%90-etc
- the gun gets a little wobbly the further from 0 0 0 you are -> "fix" with following dvars:
  > r_znear "40.0"  
  > too heigh values will cause wrong volumetrics (visualize with Surface Volume Radiance)  
  > too low values will result in wobble
- if your map is mostly black, open remix and go to textures -> hover your mouse over the sky is and select the sky category.
- effects will slow down the game (really depends on the effect and the amount - use `fx_enable 0` to disable effects completely) 
- static models might look weird at times because motion vectors are not correct (remix issue?)
- don't use shadermodel 2.0. The game itself does not support it.  

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
