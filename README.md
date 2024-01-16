<h1 align="center">CoD-WaW (cod5) rtx-remix compatibility mod</h3>
<div align="center">
	<img src="https://github-production-user-asset-6210df.s3.amazonaws.com/45299104/284378021-c7e94360-2731-4428-a011-7a611e7ab4c1.png"/>
</div>

<br>
<h3 align="center">sp / mp compatible</h3>
<br>

## Usage

1) Install the latest full rtx-remix release (0.3.0 at this time) 
- https://github.com/NVIDIAGameWorks/rtx-remix/tags

<br>

2) Install the latest 'nightly' (github actions build) of the brdige
- https://github.com/NVIDIAGameWorks/bridge-remix/actions

<br>

3) Install the lastest 'nightly' (github actions build) of the runtime 
- https://github.com/NVIDIAGameWorks/dxvk-remix/actions

<br>

4) Drop `nvapi.dll` and the contents of the `assets` folder into your CoDWaW root directory and start the game. The in-game console should show `t4-rtx-version >` if the dll was loaded correctly.


<br>

## Dvars / Commandline Arguments to further tweak the game to your liking:

- render effects using shaders: `-stock_effects` (commandline argument) 
eg: &ensp;`c:\path\CoDWaWmp.exe -stock_effects` 
- ^ outdated - use dvar `r_warm_dpvs` instead

- `r_warm_dpvs` :: disable all culling (dvar)
- `r_lodScaleRigid` :: adjust static model draw distances (dvar)
- `r_warm_static` :: force all static models to LOD0 (dvar)

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
- no proper sky replacement yet
- effects will slow down the game (really depends on the effect and the amount - use `fx_enable 0` to disable effects completely) 
- static models might look weird at times because motion vectors are not correct (remix issue?)
- the gun gets a little wobbly the further from 0 0 0 you are
- > "fix" with following dvars:
- > r_znear "40.0"
- > r_znear_depthhack "39.9805"
- > too heigh values will cause wrong volumetrics (visualize with Surface Volume Radiance)
- > too low values will result in wobble
- if your map is mostly black, open remix and go to textures. Hover your mouse over what you think the sky is and select the sky category.

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