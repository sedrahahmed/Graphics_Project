#pragma once
// Colors.h: the shared palette, so everything looks consistent.
// Owner: Person A (anyone may ADD colors; tell the team)

#include <glm/glm.hpp>

// ---------- Guards: cool, ghostly ----------
const glm::vec4 GUARD_BLUE      (0.35f, 0.60f, 0.95f, 1.0f);
const glm::vec4 GUARD_SILVER    (0.80f, 0.84f, 0.90f, 1.0f);
const glm::vec4 GHOST_LIGHT     (0.70f, 0.95f, 1.00f, 1.0f);

// ---------- Cursed performers: warped ----------
const glm::vec4 CURSE_RED       (0.77f, 0.14f, 0.23f, 1.0f);
const glm::vec4 CURSE_PURPLE    (0.36f, 0.16f, 0.55f, 1.0f);
const glm::vec4 CURSE_GREEN     (0.25f, 0.56f, 0.35f, 1.0f);

// ---------- World ----------
const glm::vec4 NIGHT_SKY       (0.10f, 0.05f, 0.16f, 1.0f);
const glm::vec4 BOARDWALK       (0.40f, 0.27f, 0.18f, 1.0f);
const glm::vec4 TICKET_GOLD     (0.98f, 0.80f, 0.20f, 1.0f);

// ---------- UI ----------
const glm::vec4 WHITE           (1.0f, 1.0f, 1.0f, 1.0f);
const glm::vec4 BLACK           (0.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 PLACEHOLDER_GRAY(0.6f, 0.6f, 0.6f, 1.0f);
const glm::vec4 HEALTH_GREEN    (0.2f, 0.9f, 0.3f, 1.0f);
const glm::vec4 HIGHLIGHT       (1.0f, 1.0f, 1.0f, 0.15f);   // lane hover (transparent)
