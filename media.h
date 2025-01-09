#pragma once

#include <iostream>
#include "raylib.h"



class media
{

public:


    static Font digital7;
    static Font digital7Dot;
    static Font digital7AdvDot;

    static Texture2D Gizmo;
    static Sound beep;



    media(/* args */);
    ~media();

    
    static void loadMediaFiles();
    static void unloadMediaFiles();

};


