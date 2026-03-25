#include "common/difficulty.h"

const char *difficultyToString(Difficulty difficulty)
{
    switch (difficulty)
    {
    case EASY:
        return "Easy";
    case MEDIUM:
        return "Medium";
    case HARD:
        return "Hard";
    default:
        return "Unknown";
    }
}
