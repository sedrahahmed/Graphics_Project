// PlayerCards.cpp. Owner: Person D
#include "PlayerCards.h"
#include "Input.h"
#include "Economy.h"
#include "Draw.h"
#include "Colors.h"
#include "CharacterDraw.h"
#include <string>
#include "Text.h"


static int selectedCard =-1;
static int hoveredCard = -1;
static const float cardWidth= 80;
static const float cardHeight=100;
static const float cardSpacing=12;
static const float cardYstart=650;
static const float cardXStart=858;
void updatePlayerCards() {
    hoveredCard = -1;
    for(int i=0; i<3; i++) {
        float x = cardXStart + i * (cardWidth + cardSpacing);
        float y = cardYstart;
        bool unlocked =canAfford((PlayerType)i);
        if(unlocked && mouseInRect(x,y,cardWidth,cardHeight)) {
            hoveredCard = i;
            break;
        }
      
 }
   if(mouseClicked()&&hoveredCard!= -1){
            if(selectedCard == hoveredCard)
                selectedCard = -1;
            else
                selectedCard = hoveredCard;
        }
        if(selectedCard != -1&&!canAfford((PlayerType)selectedCard))
            selectedCard = -1;
    }
void drawPlayerCards() { 
    for(int i=0; i<3; i++) {
        float x = cardXStart + i * (cardWidth + cardSpacing);
        float y = cardYstart;
        PlayerType type = (PlayerType)i;
        bool unlocked= canAfford(type);
        if(i==selectedCard)
            drawRect(x,y,cardWidth+8,cardHeight+8,0,TICKET_GOLD);
        
        glm::vec4 panel;
        if(!unlocked)
            panel =glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
        else if (i == hoveredCard)
            panel = glm::vec4(0.45f, 0.25f, 0.55f, 1.0f);
        else 
            panel= CURSE_PURPLE;
        drawRect(x,y,cardWidth,cardHeight,0,panel);

        
        glm::vec4 tint = unlocked ? WHITE : glm::vec4(0.35,0.35,0.35,1);
        drawGuardIcon(type,x,y,cardWidth,cardHeight,tint);

        glm::vec4 priceColour = unlocked ? TICKET_GOLD : glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        drawCircle(x - 15, y - 38, 7, priceColour);
        drawText(std::to_string(costOf(type)), x - 4, y - 45, 0.4f, priceColour);
        
    
 }
}

bool hasSelectedCard() { return selectedCard != -1; }
PlayerType getSelectedCard() { return (PlayerType)selectedCard; }
void clearSelectedCard() { selectedCard = -1; }
