<h1 align="center">cod-waw (cod5) client mod to make nvidia's rtx-remix compatible</h3>
<div align="center">
	<img src="https://github-production-user-asset-6210df.s3.amazonaws.com/45299104/284378021-c7e94360-2731-4428-a011-7a611e7ab4c1.png"/>
</div>

<br>
<h3 align="center">sp / mp compatible</h3>
<br>

## Usage

Drop `nvapi.dll` and the contents of the `assets` folder into your CoDWaW root directory and start the game. The in-game console should show `t4-rtx-version >` if the dll was loaded correctly.

There are 2 configs shipped with t4-rtx to help with stability.  

Execute cfg 'rtx_load' by using `exec rtx_load` when in the main menu, then use console command `devmap mp_mapname` to load a map. (I'd suggest using `mp_subway` as the first map) 

Use console command `exec rtx_game` once you are spawned.
If your map is mostly black, open remix and go to textures. Hover your mouse over what you think the sky is and select the sky category.

<br>

## Dvars / Commandline Arguments to further tweak the game to your liking:

- disable all culling: use commandline argument `-disable_culling`.  
eg: &ensp;`c:\path\CoDWaWmp.exe -disable_culling` 
- ^ outdated - use dvar `r_warm_dpvs` instead
- use dvar `r_lodScaleRigid` to adjust static model draw distances
- use dvar `r_warm_static` to force all static models to LOD0 (stable hashes)

### Advanced:
- use console command `set export_entities 1` to export map entities and static models (misc_models) to `cod5root/t4rtx-export/mapname.map`

<br>

Not required but makes life easier:   
https://community.pcgamingwiki.com/files/file/714-call-of-duty-world-at-war-replacement-steam-multiplayer-executable/  

nvidia rtx-remix:  
https://github.com/NVIDIAGameWorks/rtx-remix  

Note: Don't use shadermodel 2.0. The game itself "does not support" it.  

<br>

## Current issues:
- ~~no stable hashes~~ (depends on dvars and settings)
- ~~no proper sky replacement yet~~
- static models might look weird at times because motion vectors are not correct (remix issue?)
- the gun gets more wobbly the further from 0 0 0 you are
- > "fix" with following dvars:
- > r_znear "40.0"
- > r_znear_depthhack "39.9805"
- > too heigh values will cause wrong volumetrics (visualize with Surface Volume Radiance)
- > too low values will result in wobble
- ~~not yet SP compatible~~

<br>

## Compiling yourself:
1. setup `COD5_ROOT` environment variable with path to your cod5 directory (optional)
2. run `generate-buildfiles_vs22.bat` to generate build files (build folder)
3. compile and copy `nvapi.dll` (renamed t4-rtx.dll) to your CoDWaW root folder  
   - ^ automatically happens if you setup `COD5_ROOT`

<br>
<br>

<div align="center">
	<img src="https://github.com/xoxor4d/t4-rtx/assets/45299104/6e4aad3c-8e86-4406-b0d2-bf811cc1a448"/>
</div>