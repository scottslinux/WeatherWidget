#include "media.h"
#include "raylib.h"

//  Initiaialize Static Variables to Zero State

Font media::digital7={0};
Font media::digital7Dot={0};
Font media::digital7AdvDot={0};

Texture2D media::Gizmo={0}; //frame holding the weather report
Texture2D media::Pip={0};
Sound media::beep={0};   //sound to play when refreshing the weather report

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

    Gizmo=LoadTexture("resources/console3.png");
    Pip=LoadTexture("resources/pip.png");
    // Load sound file
    beep = LoadSound("resources/beep.wav");
    if (beep.frameCount == 0) {
        std::cerr << "Failed to load sound: resources/beep.wav" << std::endl;
        
    }

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
    UnloadTexture(Gizmo);
    UnloadTexture(Pip);
    UnloadSound(beep);
    


    return;
}
