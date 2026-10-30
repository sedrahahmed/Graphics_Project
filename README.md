# Carnival of the Cursed
DMET 502 Computer Graphics, Winter 2026 — Team: [names]

## Build
- Windows: Visual Studio → File → Open → Folder → this folder → select Carnival.exe → F5
- Mac: VS Code → Open Folder → kit "Clang" → Build → Run

## Controls
Mouse: hover/click lanes, click cards to place guards
W / Up: jump · S / Down: duck · Esc: pause

## Who owns which files
- Person A (Engine & menus): core/Draw, Shader, Input, Colors · states/* · audio/* · main.cpp
- Person B (Guards & lanes): characters/Players, HealthBar · game/Lanes, Shots
- Person C (Performers & combat): characters/Enemies, Projectiles · world/HomeBase · game/Game, Combat
- Person D (World & story): core/Text · world/Background, Environment · ui/* · characters/Collectibles · game/Waves, Economy · cutscene/*

Shared contracts (change only with the whole team): Config.h, Entities.h, Game.h, Draw.h, CharacterDraw.h, StateManager.h

## Credits
- (add every downloaded or AI-assisted asset here: source, license, what you changed)
