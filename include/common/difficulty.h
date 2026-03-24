#pragma once

enum Difficulty
{
    EASY = 1,
    MEDIUM,
    HARD
};

const char *difficultyToString(Difficulty difficulty);
