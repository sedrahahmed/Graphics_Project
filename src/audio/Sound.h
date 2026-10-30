#pragma once
// Sound.h: music and sound effects (bonus).
// Owner: Person A

enum SoundID { SFX_CLICK, SFX_LANE, SFX_PLACE, SFX_HIT, SFX_DEATH, SFX_COIN, SFX_WIN, SFX_LOSE };

void initSound();
void shutdownSound();

void playMusic();
void stopMusic();
void playSFX(SoundID id);   // safe to call from anywhere, even before sound works

void volumeUp();
void volumeDown();
void toggleMute();
float getVolume();          // 0..1
bool isMuted();
