#include <iostream>
#include <curl/curl.h>
#include <string>
#include "json.hpp" // Include nlohmann/json header
#include "timer.h"
#include "media.h"


//Create a structure to hold the weather report
struct Report
{   float temperature, windspeed, windDirection, hightTemp, lowTemp;
    int humidity, pressure, visibility;
    std::string conditions, city, icon;
    std::time_t sunrise, sunset, currentTime;

};



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
    std::string url = "https://api.openweathermap.org/data/2.5/weather?lat=33.5888062&lon=-111.9493601&appid=fd8117695692f303ce69211de853a0f2&units=imperial";

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

int main() 
{

    Timer weatherRefreshTimer;
    Timer animation;
    Report currentReport;





    
    media::loadMediaFiles();    //get the good stuff!
    
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
    //SetConfigFlags(FLAG_WINDOW_UNDECORATED);

    InitWindow(600,1200,"Weather");
    SetTargetFPS(30);   //30 fps should be good for a widget

    
    weatherRefreshTimer.start();
    UpdateWeather(currentReport);    //get the first report before heading into the display loop

    while(!WindowShouldClose())
    {
        std::cout<<"Entering Reporting loop...1st update\n";

        if(weatherRefreshTimer.elapsed()>6)
        {
            //UpdateWeather();
            weatherRefreshTimer.reset();
            weatherRefreshTimer.start();
            std::cout<<"************  update weather *****************\n";
            //UpdateWeather(currentReport);

        }

        BeginDrawing();
            ClearBackground(Color{0,0,0,0});   //clear the screen
            //weather window display routines  
            DrawText("Weather Widget",10,10,80,WHITE);  


        EndDrawing();
    }
    
    return 0;
}
