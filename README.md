# Castlevania 64 - Extended Stage Select Mod

This repository contains source code for an extended stage select mod for Castlevania (Nintendo 64). Although the game already has an unused stage select game state, it's very basic and only allows choosing a small number of maps.

This mod replaces that with a newly-created stage selection with the ability to select all maps, plus some other options.

Press L + R + Z at any time during gameplay to go to the stage select.

Controls:
* DPad Up and Down: Select maps
* A: Warp to map
* L: Change character
* R: Change alt costume
* DPad Left and Right: Select cutscenes
* CRight and Left: Select spawns

## Building (Ubuntu)
### Prerequisites
* [IDO 7.1](https://github.com/decompals/ido-static-recomp/releases) (place `cc` and all the other files in `tools/ido71`)
* [nOVL](https://github.com/Bsquo/nOVL/releases) (place `novl` in `tools/nOVL`)
* `mips64-ld`
* [ZAPD](https://github.com/NEstelami/ZAPD) (place `ZAPD.out` in `tools`)
* [LZKN64](https://github.com/Fluvian/lzkn64) (place `lzkn64` in `tools`)
* Python 3

### Build
* Place a Castlevania (USA v1.0) ROM (`sha1: 989A28782ED6B0BC489A1BBBD7BEC355D8F2707E`) in the root of the project and rename it to `cv64.z64`. Then, run `make`

## Credits:
* ZZT32, ariahiro64, z64me, sauraen: For [nOVL](https://github.com/z64tools/nOVL), which this repository uses to convert textures from PNG to raw data.
* CrookedPoe, z64me: For [z64ovl](https://github.com/CrookedPoe/z64ovl). Although intended for the Zelda 64 games, they provide examples for rebuilding overlays.
* Fluvian: For [LZKN64](https://github.com/Fluvian/lzkn64), which can decompress and compress back files in the Nagano format, used in several Konami N64 games.
* [OoT Decomp](https://github.com/zeldaret/oot): For the Ultra 64 headers.

## More information:
* [Castlevania 64 - Research Spreadsheets](https://docs.google.com/spreadsheets/d/1nzh_nFf26oVZy6uWeNYiYGXAto6Yz3xypZwWqwJBBJQ/edit#gid=74717405)
* [CV64&LoD Speedruns Discord.](https://discord.gg/eKht382) Discord server for the CV64 / LoD community. Go to the #tech-talk channel for reverse engineering discussion.