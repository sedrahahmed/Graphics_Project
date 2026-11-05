#pragma once //3ashan mesh 3ayzeen n import it twice 
//palette so everything is consistent
//r g b alpha. 1 opaque 0 shafaf
//glm::vec4 da 4D vector of floats fa we grouup el colours bi
#include <glm/glm.hpp>
//guards
const glm::vec4 GUARD_BLUE      (0.35f, 0.60f, 0.95f, 1.0f);
const glm::vec4 GUARD_SILVER    (0.80f, 0.84f, 0.90f, 1.0f);
const glm::vec4 GHOST_LIGHT     (0.70f, 0.95f, 1.00f, 1.0f);

//enemies
const glm::vec4 CURSE_RED       (0.77f, 0.14f, 0.23f, 1.0f);
const glm::vec4 CURSE_PURPLE    (0.36f, 0.16f, 0.55f, 1.0f);
const glm::vec4 CURSE_GREEN     (0.25f, 0.56f, 0.35f, 1.0f);

//bg
const glm::vec4 NIGHT_SKY       (0.10f, 0.05f, 0.16f, 1.0f);
const glm::vec4 BOARDWALK       (0.40f, 0.27f, 0.18f, 1.0f);
const glm::vec4 TICKET_GOLD     (0.98f, 0.80f, 0.20f, 1.0f);

//random
const glm::vec4 WHITE           (1.0f, 1.0f, 1.0f, 1.0f);
const glm::vec4 BLACK           (0.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 PLACEHOLDER_GRAY(0.6f, 0.6f, 0.6f, 1.0f);
const glm::vec4 HEALTH_GREEN    (0.2f, 0.9f, 0.3f, 1.0f);
const glm::vec4 HIGHLIGHT       (1.0f, 1.0f, 1.0f, 0.15f);   // lane hover (transparent)
const glm::vec4 DARK_BROWN      (0.28f, 0.18f, 0.11f, 1.0f);   // lane dividers
const glm::vec4 SELECTED_LANE   (1.0f, 1.0f, 1.0f, 0.25f);   // lane selected (transparent)

//healthbar
const glm::vec4 HEALTHBAR_RED  (0.90f, 0.10f, 0.10f, 1.0f);
const glm::vec4 HEALTHBAR_GREEN (0.45f, 0.85f, 0.15f, 1.0f); 
const glm::vec4 HEALTHBAR_ORANGE  (1.00f, 0.55f, 0.05f, 1.0f);
const glm::vec4 HEART_RED       (0.90f, 0.10f, 0.15f, 1.0f);