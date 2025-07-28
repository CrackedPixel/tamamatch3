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
    COUNT = 5,
};

enum struct CURSOR_TYPES {
    NORMAL,
    INVALID,
    FOOD1,
    FOOD2,
    FOOD3,
    FOOD4,
    FOOD5,
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
    OUTFIT,
};

enum struct FOOD_TYPES {
    BUG1,
    BUG2,
    BUG3,
    BUG4,
    BUG5,
    COUNT = 5,
};
