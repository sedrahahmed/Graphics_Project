#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "Sound.h"


static ma_engine engine;
static ma_sound backgroundMusic;
static bool ready = false;
static float volume = 0.5f;
static float musicVolume = 0.5f;
static float soundEffectVolume = 1.0f;
static bool musicMuted = false;
static bool soundEffectsMuted = false;
static ma_sound_group soundEffects;
static const char* soundFiles[] = {ASSET_DIR "sounds/click.mp3",
                                   ASSET_DIR "sounds/lane.mp3",
                                   ASSET_DIR "sounds/placement.mp3",
                                   ASSET_DIR "sounds/melee_attack.mp3",
                                   ASSET_DIR "sounds/ranged_attack.mp3",
                                   ASSET_DIR "sounds/rolling_ball.mp3",
                                   ASSET_DIR "sounds/death.mp3",
                                   ASSET_DIR "sounds/coin.mp3",
                                   ASSET_DIR "sounds/win.mp3",
                                   ASSET_DIR "sounds/lose.mp3",
                                   // cutscene cues (add these files to assets/sounds)
                                   ASSET_DIR "sounds/cassette_click.mp3",
                                   ASSET_DIR "sounds/owner_recording.mp3",
                                   ASSET_DIR "sounds/clock_midnight.mp3"
                        };

void initSound() {
    if(ma_engine_init(NULL, &engine)!= MA_SUCCESS) //audio fails but game shouldn't be affected
        return;
    ready = true; 
    ma_sound_group_init(&engine, 0, NULL, &soundEffects);
    ma_sound_init_from_file(&engine, ASSET_DIR "sounds/music.mp3", MA_SOUND_FLAG_STREAM, NULL, NULL, &backgroundMusic);
    ma_sound_set_looping(&backgroundMusic, MA_TRUE);
    ma_engine_set_volume(&engine, volume);
    ma_sound_set_volume(&backgroundMusic, musicVolume);
    ma_sound_group_set_volume(&soundEffects, soundEffectVolume);
 }
void shutdownSound() {
    if(ready){
        ma_sound_group_uninit(&soundEffects);
        ma_sound_uninit(&backgroundMusic);
        ma_engine_uninit(&engine);
    }
}

void playMusic() {
    if(ready)
       ma_sound_start(&backgroundMusic); 
}
void stopMusic() { 
    if(ready)
        ma_sound_stop(&backgroundMusic);
}
void playSoundEffect(SoundID id) {
    if(!ready)return;
    ma_engine_play_sound(&engine, soundFiles[id], &soundEffects);
 }

void volumeUp() {
    volume = volume + 0.1;
    if(volume > 1) volume = 1;
    if(ready){
        ma_engine_set_volume(&engine,volume);
        ma_sound_set_volume(&backgroundMusic, musicMuted ? 0.0f : musicVolume);
        ma_sound_group_set_volume(&soundEffects, soundEffectsMuted ? 0.0f : soundEffectVolume);    }
}
void volumeDown() {
    volume = volume - 0.1;
    if(volume < 0) volume = 0;
    if(ready){
        ma_engine_set_volume(&engine,volume);
        ma_sound_set_volume(&backgroundMusic, musicMuted ? 0.0f : musicVolume);
        ma_sound_group_set_volume(&soundEffects, soundEffectsMuted ? 0.0f : soundEffectVolume);
    }
}
void toggleMusicMute() {
    musicMuted = !musicMuted;
    if(ready){
        ma_sound_set_volume(&backgroundMusic, musicMuted ? 0.0f : musicVolume);
    }
}
void toggleSoundEffectsMute() {
    soundEffectsMuted = !soundEffectsMuted;
    if(ready){
        ma_sound_group_set_volume(&soundEffects, soundEffectsMuted ? 0.0f : soundEffectVolume);
    }
}
float getVolume() { return volume; }
bool isMusicMuted(){return musicMuted;}
bool isSoundEffectsMuted(){return soundEffectsMuted;}
