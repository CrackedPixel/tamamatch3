#pragma once

enum struct PET_STAGES {
    EGG,
    NEWBORN,
    TODDLER,
    ADOLESCENT,
    ADULT,
};

enum struct PET_STATES {
    HEALTHY,
    HUNGRY, // popup
    SAD,
    BORED, // popup
    DIRTY, // poop
    TANKDIRTY, // on-screen
    SICK,
    SLEEPING, // SKIP
    ANGRY,
    DED,
    EVOLVE, // ready to evolve
};

enum struct PET_ATTRIBUTES {
    HUNGER,
    HAPPINESS,
    BOREDOM,
    HYGIENE,
    TANKHYGIENE,
    ILLNESS,
    GROWTH,
    SLEEPINESS,
    HP,
};

enum struct OUTFIT_SLOTS {
    HAT,
    GLASSES,
    BACK,
    ACC1,
    ACC2,
    COUNT,
};

enum struct CURSOR_TYPES {
    NORMAL,
    INVALID,
    FOOD,
    SAD,
    DIRTY,
    TANKDIRTY,
    ILLNESS,
    TOY,
    STINKY,
};

enum struct ITEM_TYPES {
    FOOD,
    TOY,
    HYGIENE,
    TANK,
    ILLNESS,
};
