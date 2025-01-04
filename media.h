#pragma once

#include <iostream>
#include "raylib.h"



class media
{

public:

    //static Texture2D bug;
    static Font digital7;
    



    media(/* args */);
    ~media();

    
    static void loadMediaFiles();
    static void unloadMediaFiles();

};


