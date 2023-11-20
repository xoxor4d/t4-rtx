<h1 align="center">cod-waw (cod5) client mod to make nvidia's rtx-remix compatible</h3>

<div align="center">
	<img src="https://github-production-user-asset-6210df.s3.amazonaws.com/45299104/284378021-c7e94360-2731-4428-a011-7a611e7ab4c1.png"/>
</div>

<br>

Drop `nvapi.dll` and the contents of the `assets` folder into your CoDWaW root directory and start the game.  
Use console command `devmap mp_mapname` and once in-game execute the cfg by using `exec rtx`  
Your map will probably be black. Open remix, go to textures and hover with your mouse over what you think is the sky and
select the sky category.

Not required but makes life easier:   
https://community.pcgamingwiki.com/files/file/714-call-of-duty-world-at-war-replacement-steam-multiplayer-executable/  

nvidia rtx-remix:  
https://github.com/NVIDIAGameWorks/rtx-remix  

Note: Don't use shadermodel 2.0. The game itself "does not support" it.  

<br>

## Current issues:
- no stable hashes
- ~~no proper sky replacement yet~~
- the gun gets more wobbly the further from 0 0 0 you are (tweak r_znear and r_znear_depthhack and let me know if you find values to fix that)
- not yet SP compatible

<br>

## Compiling yourself:
1. setup `COD5_ROOT` environment variable with path to your cod5 directory (optional)
2. run `generate-buildfiles_vs22.bat` to generate build files (build folder)
3. compile and copy `nvapi.dll` (renamed t4-rtx.dll) to your CoDWaW root folder  
   - ^ automatically happens if you setup `COD5_ROOT`