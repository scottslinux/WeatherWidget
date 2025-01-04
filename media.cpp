#include "media.h"
#include "raylib.h"

//  Initiaialize Static Variables to Zero State
//Texture2D media::bug={0};
Font media::digital7;

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
    //bug=LoadTexture("resources/Roach sprites.png");
    digital7=LoadFontEx("resources/digital7.ttf",20,0,0);



    return;
}

//====================================

void media::unloadMediaFiles()
{
    // Free up all of the resources
    //UnloadTexture(bug);
    UnloadFont(digital7);
    


    return;
}
