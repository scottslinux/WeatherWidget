#include "media.h"
#include "raylib.h"

//  Initiaialize Static Variables to Zero State

Font media::digital7={0};
Font media::digital7Dot={0};
Font media::digital7AdvDot={0};

Texture2D media::Gizmo={0}; //frame holding the weather report

//testing git

 
//====================================

media::media(/* args */)    //constructor--not used STATIC Functions
{
    
}
//====================================
media::~media()
{
}
//====================================


void media::loadMediaFiles()    //static function (see .h)to load media
{
    //Load all of the image files into static variables
    
    digital7 = LoadFontEx("resources/digital-7.ttf", 80, NULL, 0);
   
    digital7Dot=LoadFontEx("resources/7segment.ttf",80,NULL,0);
    digital7AdvDot=LoadFontEx("resources/advanced_dot_digital-7.ttf",80,NULL,0);

    Gizmo=LoadTexture("/resources/console3.png");


    return;
}

//====================================

void media::unloadMediaFiles()
{
    // Free up all of the resources
    //UnloadTexture(bug);
    UnloadFont(digital7);
    UnloadFont(digital7Dot);
    UnloadFont(digital7AdvDot);
    


    return;
}
