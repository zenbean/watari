#pragma once

enum class Stone {
    BLACK,
    WHITE
};

class Group{
    int groupID;
    int liberties;
    Stone colour;
};