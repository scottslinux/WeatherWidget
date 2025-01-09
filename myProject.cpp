#include <iostream>
#include <curl/curl.h>
#include <string>
#include <sstream>
#include "json.hpp" // Include nlohmann/json header
#include "timer.h"
#include "media.h"
#include <chrono>


//Create a structure to hold the weather report
struct Report
{   float temperature, windspeed, windDirection, hightTemp, lowTemp;
    int humidity, pressure, visibility;
    std::string conditions, city, icon;
    std::time_t sunrise, sunset, currentTime;

};

int counter=0;
const float IMAGE_SCALE=0.75f;
const int REFRESH_INTERVAL=1800;  //refresh weather every 30 minutes

using json = nlohmann::json;
//================================================================================================
// Callback function to store the response in a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}
//================================================================================================
//                          Method to Get New Weather Report
void UpdateWeather(Report &currentreport)    //send the struct by reference to update it
{
CURL* curl;
    CURLcode res;
    std::string readBuffer;

    // Replace YOUR_API_KEY with your OpenWeatherMap API key
    //std::string url = "https://api.openweathermap.org/data/2.5/weather?lat=33.5888062&lon=-111.9493601&appid=fd8117695692f303ce69211de853a0f2&units=imperial";

    std::string url = "https://api.openweathermap.org/data/3.0/onecall?lat=33.5888062&lon=-111.9493601&appid=56901c1a22b73ad304bbdbe85a2d4c9b&units=imperial";
    


    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "cURL Error: " << curl_easy_strerror(res) << std::endl;
        } else {
            try {
                // Parse JSON response using nlohmann/json
                json data = json::parse(readBuffer);
                std::cout << "Raw JSON Response: " << data.dump(4) << std::endl;

             // ⁡⁣⁢⁣Extract temperature⁡
                if (data.contains("main") && data["main"].contains("temp")) {
                    currentreport.temperature = data["main"]["temp"].get<float>();

                    //std::cout << "Temperature: " << temperature << "\u00B0F" << std::endl;
                } else {
                    std::cerr << "Temperature data not found in the response." << std::endl;
                }

                // ⁡⁣⁢⁡⁣⁢⁣Extract weather condition⁡
               if (data.contains("weather") && data["weather"].is_array() && !data["weather"].empty()) 
                {
                    currentreport.conditions = data["weather"][0]["description"].get<std::string>();
                } else {
                    std::cerr << "Weather condition not found in the response." << std::endl;
                }
                // ⁡⁣⁢⁣Ex⁡⁣⁢⁣tract humidity⁡
                if (data.contains("main") && data["main"].contains("humidity")) {
                    currentreport.humidity = data["main"]["humidity"].get<int>();
                    
                } else {
                    std::cerr << "Humidity data not found in the response." << std::endl;
                }
                // ⁡⁣⁢⁣Extract wind speed⁡
                if (data.contains("wind") && data["wind"].contains("speed")) {
                    currentreport.windspeed = data["wind"]["speed"].get<float>();
                } else {
                    std::cerr << "Wind speed data not found in the response." << std::endl;
                }
                // ⁡⁣⁢⁣Extract Wind Direction⁡
                if (data.contains("wind") && data["wind"].contains("deg")) {
                    currentreport.windDirection = data["wind"]["deg"].get<float>();
                }
                else {
                    std::cerr << "Wind direction data not found in the response." << std::endl;
                }
                // ⁡⁣⁢⁣Extract city name⁡
                if (data.contains("name")) {
                    currentreport.city = data["name"].get<std::string>();
                } else {
                    std::cerr << "City name not found in the response." << std::endl;
                }
                // ⁡⁣⁢⁣Extract Barometric Pressure⁡
                if (data.contains("main") && data["main"].contains("pressure")) {
                    currentreport.pressure= data["main"]["pressure"].get<int>();
                } else {
                    std::cerr << "Pressure data not found in the response." << std::endl;
                }
                // ⁡⁣⁢⁣Extract visibility⁡
                if (data.contains("visibility")) {
                    currentreport.visibility = data["visibility"].get<int>();
                } else {
                    std::cerr << "Visibility data not found in the response." << std::endl;
                }
                // ⁡⁣⁢⁣Extract sunrise time⁡
                if (data.contains("sys") && data["sys"].contains("sunrise")) {
                    int sunrise = data["sys"]["sunrise"].get<int>();
                    currentreport.sunrise = sunrise;
                    //std::cout << "Sunrise Time: " << std::asctime(std::localtime(&sunriseTime));
                } else {
                    std::cerr << "Sunrise time not found in the response." << std::endl;
                }
                // ⁡⁣⁢⁣Extract sunset time⁡
                if (data.contains("sys") && data["sys"].contains("sunset")) {
                    int sunset = data["sys"]["sunset"].get<int>();
                    currentreport.sunset = sunset;
                    //std::cout << "Sunset Time: " << std::asctime(std::localtime(&sunsetTime));
                } else {
                    std::cerr << "Sunset time not found in the response." << std::endl;
                }
                // ⁡⁣⁢⁣Extract Current Time⁡
                if (data.contains("dt")) {
                    int currentTime = data["dt"].get<int>();
                    currentreport.currentTime = currentTime;
                    //std::cout << "Current Time: " << std::asctime(std::localtime(&time));
                } else {
                    std::cerr << "Current time not found in the response." << std::endl;
                }
                //⁡⁣⁢⁣extract high and low temperatures⁡
                if (data.contains("main") && data["main"].contains("temp_max") && data["main"].contains("temp_min")) {
                    currentreport.hightTemp = data["main"]["temp_max"].get<float>();
                    currentreport.lowTemp= data["main"]["temp_min"].get<float>();
                    //std::cout << "High Temperature: " << highTemp << "\u00B0F" << std::endl;
                    //std::cout << "Low Temperature: " << lowTemp << "\u00B0F" << std::endl;
                } else {
                    std::cerr << "High and low temperatures not found in the response." << std::endl;
                }
                



            } catch (const json::exception& e) {
                std::cerr << "JSON Parsing Error: " << e.what() << std::endl;
            }
        }

        curl_easy_cleanup(curl);
    }
    
    curl_global_cleanup();




}
//==========================================================================================
//weather routine re-wrtten for onecall api
void UpdateWeatherOneCall(Report &currentreport)    //send the struct by reference to update it
{
CURL* curl;
    CURLcode res;
    std::string readBuffer;

    // Replace YOUR_API_KEY with your OpenWeatherMap API key
    //std::   string url = "https://api.openweathermap.org/data/2.5/onecall?lat=33.5888062&lon=-111.9493601&exclude=minutely,hourly,daily&appid=fd8117695692f303ce69211de853a0f2&units=imperial"; //exclude=minutely,hourly,daily for current weather only                
    std::string url = "https://api.openweathermap.org/data/3.0/onecall?lat=33.5888062&lon=-111.9493601&exclude=hourly&appid=56901c1a22b73ad304bbdbe85a2d4c9b&units=imperial";
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();



    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "cURL Error: " << curl_easy_strerror(res) << std::endl;
        } else {
            try {
                // Parse JSON response using nlohmann/json
                json data = json::parse(readBuffer);
                //std::cout << "Raw JSON Response: " << data.dump(4) << std::endl;

             // ⁡⁣⁢⁣Extract temperature⁡
                if (data.contains("current") && data["current"].contains("temp")) {
                    currentreport.temperature = data["current"]["temp"].get<float>();

                    //std::cout << "Temperature: " << temperature << "\u00B0F" << std::endl;
                } else {
                    std::cerr << "Temperature data not found in the response." << std::endl;
                }

                // ⁡⁣⁢⁡⁣⁢⁣Extract weather condition⁡
               if (data.contains("current") && data["current"].contains("weather") && data["current"]["weather"].is_array() && !data["current"]["weather"].empty()) 
                {
                    currentreport.conditions = data["current"]["weather"][0]["description"].get<std::string>();
                } else {
                    std::cerr << "Weather condition not found in the response." << std::endl;
                }
                // ⁡⁣⁢⁣Ex⁡⁣⁢⁣tract humidity⁡
                if (data.contains("current") && data["current"].contains("humidity")) {
                    currentreport.humidity = data["current"]["humidity"].get<int>();
                    
                } else {
                    std::cerr << "Humidity data not found in the response." << std::endl;
                }
                // ⁡⁣⁢⁣Extract wind speed⁡
                if (data.contains("current") && data["current"].contains("wind_speed")) {
                    currentreport.windspeed = data["current"]["wind_speed"].get<float>();
                } else {
                    std::cerr << "Wind speed data not found in  the response." << std::endl;    
                }
                // ⁡⁣⁢⁣Extract Wind Direction⁡
                if (data.contains("current") && data["current"].contains("wind_deg")) {
                    currentreport.windDirection = data["current"]["wind_deg"].get<float>();
                }
                else {
                    std::cerr << "Wind direction data not found in the response." << std::endl;
                }
                // ⁡⁣⁢⁣Extract city name⁡
                if (data.contains("timezone")) {
                    currentreport.city = data["timezone"].get<std::string>();
                } else {
                    std::cerr << "City name not found in the response." << std::endl;
                }
                // ⁡⁣⁢⁣Extract Barometric Pressure⁡
                if (data.contains("current") && data["current"].contains("pressure")) {
                    currentreport.pressure= data["current"]["pressure"].get<int>();
                } else {
                    std::cerr << "Pressure data not found in the response." << std::endl;
                }
                // ⁡⁣⁢⁣Extract visibility⁡
                if (data.contains("current") && data["current"].contains("visibility")) {
                    currentreport.visibility = data["current"]["visibility"].get<int>();
                } else {
                    std::cerr << "Visibility data not found in the response." << std::endl;
                }
                // ⁡⁣⁢⁣Extract sunrise time⁡
                if (data.contains("current") && data["current"].contains("sunrise")) {
                    int sunrise = data["current"]["sunrise"].get<int>();
                    currentreport.sunrise = sunrise;
                    //std::cout << "Sunrise Time: " << std::asctime(std::localtime(&sunriseTime));
                } else {
                    std::cerr << "Sunrise time not found in the response." << std::endl;
                }   
                // ⁡⁣⁢⁣Extract sunset time⁡
                if (data.contains("current") && data["current"].contains("sunset")) {
                    int sunset = data["current"]["sunset"].get<int>();
                    currentreport.sunset = sunset;
                    //std::cout << "Sunset Time: " << std::asctime(std::localtime(&sunsetTime));
                } else {
                    std::cerr << "Sunset time not found in the response." << std::endl;
                }   
                // ⁡⁣⁢⁣Extract Current Time⁡
                if (data.contains("current") && data["current"].contains("dt")) {
                    int currentTime = data["current"]["dt"].get<int>();
                    currentreport.currentTime = currentTime;
                    //std::cout << "Current Time: " << std::asctime(std::localtime(&time));
                } else {
                    std::cerr << "Current time not found in the response." << std::endl;
                }   
                //⁡⁣⁢⁣extract high and low temperatures⁡
                if (data.contains("daily") && data["daily"].is_array() && !data["daily"].empty() && data["daily"][0].contains("temp") && data["daily"][0]["temp"].contains("max") && data["daily"][0]["temp"].contains("min")) {
                    currentreport.hightTemp = data["daily"][0]["temp"]["max"].get<float>();
                    currentreport.lowTemp= data["daily"][0]["temp"]["min"].get<float>();

                    std::cout << "High Temperature: " << currentreport.hightTemp << "\u00B0F" << std::endl;
                    std::cout << "Low Temperature: " << currentreport.lowTemp << "\u00B0F" << std::endl;
                    //std::cout << "High Temperature: " << highTemp << "\u00B0F" << std::endl;
                    //std::cout << "Low Temperature: " << lowTemp << "\u00B0F" << std::endl;
                } else {
                    std::cerr << "High and low temperatures not found in the response." << std::endl;
                }   

            } catch (const json::exception& e) {
                std::cerr << "JSON Parsing Error: " << e.what() << std::endl;
            }
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

}


//==========================================================================================
//                          Main Program

int main() 
{

    Timer weatherRefreshTimer;
    Timer animation;
    Report currentReport;
    std::chrono::steady_clock::time_point lastreportTime;
    std::chrono::steady_clock::time_point currentTime;
    //when checking will see if currenttime-lastreporttime>3600 seconds

    
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);

    InitWindow(1200*IMAGE_SCALE,1200*IMAGE_SCALE,"Weather");
    SetTargetFPS(30);   //30 fps should be good for a widget
    SetWindowPosition(GetMonitorWidth(0)-1200*IMAGE_SCALE,GetMonitorHeight(0)-1200*IMAGE_SCALE);
    InitAudioDevice();


    media::loadMediaFiles();

    weatherRefreshTimer.start();
    UpdateWeatherOneCall(currentReport);    //get the first report before heading into the display loop
    lastreportTime=std::chrono::steady_clock::now();

// Load the render texture
    RenderTexture2D canvas = LoadRenderTexture(1200, 1200);
    
    while(!WindowShouldClose())
    {

       
        currentTime=std::chrono::steady_clock::now();
    
        if(currentTime-lastreportTime>std::chrono::seconds(REFRESH_INTERVAL))  //timer in seconds
        {
            lastreportTime=currentTime;
            UpdateWeatherOneCall(currentReport);

        }



        BeginDrawing();


            ClearBackground(Color{0,0,0,0});   //clear the screen
           
           
            BeginTextureMode(canvas);
            ClearBackground(Color{0,0,0,0});   //clear the screen
            Rectangle source={0,0,media::Gizmo.width,media::Gizmo.height};
            Rectangle dest={0,0,media::Gizmo.width/3,media::Gizmo.height/3};
            DrawTexturePro(media::Gizmo,source,dest,{0,0},0,WHITE);

            //DrawTextPro(media::digital7,"Temperature:",{500,300},{0,0},0,50,5,Color{100,255,105,static_cast<unsigned char>(rand()%35+220)});
            currentReport.temperature=std::round(currentReport.temperature);
            std::stringstream ss;
            ss<<currentReport.temperature;
            DrawTextPro(media::digital7,ss.str().c_str(),{640,335},{0,0},0,150,5,Color{100,255,105,static_cast<unsigned char>(rand()%35+220)});
            DrawRing({813,335},15,18,0,360,0,Color{100,255,105,static_cast<unsigned char>(rand()%35+220)});

            currentReport.hightTemp=std::round(currentReport.hightTemp);
            currentReport.lowTemp=std::round(currentReport.lowTemp);
            //clear ss
            ss.str(std::string());
            ss<<currentReport.conditions;
            DrawTextPro(media::digital7,ss.str().c_str(),{483,580},{0,0},0,50,5,Color{100,255,105,static_cast<unsigned char>(rand()%35+220)});
            //clear ss
            ss.str(std::string());
            ss<<"high\n"<<currentReport.hightTemp<<"\nlow\n"<<currentReport.lowTemp;
            DrawTextPro(media::digital7,ss.str().c_str(),{873,330},{0,0},0,40,5,Color{100,255,105,static_cast<unsigned char>(rand()%35+220)});
            //clear ss
            ss.str(std::string());
            ss<<"humidity\n  "<<currentReport.humidity<<"%";
            DrawTextPro(media::digital7,ss.str().c_str(),{533,483},{0,0},0,40,5,Color{100,255,105,static_cast<unsigned char>(rand()%35+220)});
            //clear ss
            ss.str(std::string());
            ss<<"  wind\n"<<std::round(currentReport.windspeed)<<" mph";
            DrawTextPro(media::digital7,ss.str().c_str(),{270,640},{0,0},0,40,5,Color{100,255,105,static_cast<unsigned char>(rand()%35+220)});
            //clear ss
            ss.str(std::string());
            ss<<currentReport.windDirection;
            DrawTextPro(media::digital7,ss.str().c_str(),{294,730},{0,0},0,40,5,Color{100,255,105,static_cast<unsigned char>(rand()%35+220)});
            DrawRing({370,733},5,8,0,360,0,Color{100,255,105,static_cast<unsigned char>(rand()%35+220)}); 
            
            //DrawTextPro(media::digital7,std::to_string(currentReport.temperature).c_str(),{640,380},{0,0},0,120,5,Color{100,255,105,rand()%35+220});
            //display current time
            std::tm* localTime = std::localtime(&currentReport.currentTime);
            const char* timeCString = std::asctime(localTime);

            DrawTextPro(media::digital7,timeCString,{510,275},{0,0},0,35,5,Color{100,255,105,static_cast<unsigned char>(rand()%35+220)});
            DrawTextPro(media::digital7,"===>",{360,238},{0,0},currentReport.windDirection-90+(rand()%3+-1),30,5,Color{100,255,105,static_cast<unsigned char>(rand()%35+220)});

            //draw solid elipse
            DrawEllipse(1130,425,10,5,Color{242,255,0,static_cast<unsigned char>(rand()%100)});
            DrawEllipse(1130,355,20,7,Color{242,255,0,static_cast<unsigned char>(rand()%100)});
            DrawEllipse(800,180,60,7,Color{242,255,0,static_cast<unsigned char>(rand()%100)});

//          Numerate the mouse position for building console
/*          int x=GetMouseX();
            int y=GetMouseY();
            DrawText(std::to_string(x).c_str(),10,10,40,GREEN);
            DrawText(std::to_string(y).c_str(),10,60,40,GREEN);
*/
            //489,578

            EndTextureMode();

            DrawTexturePro(canvas.texture,{0,0,canvas.texture.width,-canvas.texture.height},{0,0,1200*IMAGE_SCALE,1200*IMAGE_SCALE}, {0,0},0,WHITE);


//          draw the refresh button
            DrawCircle(487,578,10,Color{50,255,105,static_cast<unsigned char>(rand()%150+105)});

            //Refresh on demand
            Vector2 mousePos = GetMousePosition();
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                std::cout<<"mouse clicked\n";
                if(CheckCollisionPointCircle(mousePos,{487,578},20))
                {
                    lastreportTime=currentTime;
                    UpdateWeatherOneCall(currentReport);     
                    std::cout<<"refreshed\n";  
                    
                    if(!IsSoundPlaying(media::beep))
                    {
                        PlaySound(media::beep);
                    }
                    

                 }
            }

        EndDrawing();
    }
    media::unloadMediaFiles();
    return 0;
}
