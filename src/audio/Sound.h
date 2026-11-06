#pragma once

enum SoundID { SOUND_CLICK, SOUND_LANE, SOUND_PLACEMENT, SOUND_MELEE_ATTACK, SOUND_RANGED_ATTACK, SOUND_ROLLING_BALL, SOUND_DEATH, SOUND_COIN, SOUND_WIN, SOUND_LOSE, SOUND_CASSETTE, SOUND_OWNER_VOICE, SOUND_CLOCK };


void initSound();
void shutdownSound();

void playMusic();
void stopMusic();
void playSoundEffect(SoundID id); //safe to call from anywhere even before sound works

void volumeUp();
void volumeDown();
void toggleMusicMute();
void toggleSoundEffectsMute();
void setMusicMuted(bool muted);
void setSoundEffectsMuted(bool muted);
float getVolume();
bool isMusicMuted();
bool isSoundEffectsMuted();

