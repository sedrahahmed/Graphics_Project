// The buttons already call the Sound functions; they do nothing until Sound.cpp is written.
#include "SoundMenu.h"
#include "StateManager.h"
#include "Sound.h"
#include "Button.h"
#include "Draw.h"
#include "Colors.h"
#include "Config.h"
#include "MainMenu.h"
static const float CENTER_X = SCREEN_W / 2.0f;

// x,y,width,height,symbol
static Button upButton = { CENTER_X + 150, 470, 120, 55, "+" };
static Button downButton = { CENTER_X - 150, 470, 120, 55, "-" };
static Button musicButton = { CENTER_X - 130, 400, 240, 55, "MUSIC: ON" };
static Button sfxButton = { CENTER_X + 130, 400, 240, 55, "EFFECTS: ON" }; 
static Button muteAll = { CENTER_X, 330, 350, 55, "MUSIC & EFFECTS: ON" };
static Button backButton = { CENTER_X, 260, 240, 55, "BACK" };

// The volume bar between the - and + buttons
static const float BAR_W = 140.0f, BAR_H = 20.0f;

void updateSoundMenu() {
    if (isClicked(upButton))
        volumeUp();

    if (isClicked(downButton)) 
        volumeDown();

    if (isClicked(musicButton)) 
        toggleMusicMute();

    if(isClicked(sfxButton)) 
        toggleSoundEffectsMute();

    if (isClicked(backButton)) {
        changeState(MAIN_MENU);
    }
    if(isClicked(muteAll)){
        // If both are already off, turn both on; otherwise turn both off
        bool muteBoth = !(isMusicMuted() && isSoundEffectsMuted());
        setMusicMuted(muteBoth);
        setSoundEffectsMuted(muteBoth);
    }
}

void drawSoundMenu() {
    // Draw background image for sound menu (nafs el background as main menu, frame 0 of 1)
    drawSprite(getMenuTexture(), 0, 1, CENTER_X, SCREEN_H / 2.0f, SCREEN_W, SCREEN_H, 0.0f, WHITE);

    // volume bar + how it increases when volume changes
    float volume = getVolume();
    float fillW = BAR_W * volume;
    float barLeft = CENTER_X - BAR_W / 2.0f;
    
    drawRect(CENTER_X, 470, BAR_W, BAR_H, 0.0f, BLACK);
    if (fillW > 0.0f){
        drawRect(barLeft + fillW / 2.0f, 470, fillW, BAR_H, 0.0f, TICKET_GOLD);
    }

    // The mute button's word changes so you know what clicking it will do
    musicButton.label = isMusicMuted() ? "MUSIC: OFF" : "MUSIC: ON";
    sfxButton.label = isSoundEffectsMuted() ? "SFX: OFF" : "SFX: ON";
    muteAll.label = isSoundEffectsMuted()&&isMusicMuted() ? "MUSIC & SFX: OFF" : "MUSIC & SFX: ON";

    drawButton(downButton);
    drawButton(upButton);
    drawButton(musicButton);
    drawButton(sfxButton);
    drawButton(muteAll);
    drawButton(backButton);
}
