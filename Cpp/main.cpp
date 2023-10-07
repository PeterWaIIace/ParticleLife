#include "raylib.h"
#include "raymath.h"
#include <unistd.h>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <filesystem>

#include <ParticleSystem.hpp>

// for format
#include <fmt/core.h>
using namespace fmt::literals;

std::string time_now(){
    // Get the current timepoint
    auto currentTime = std::chrono::system_clock::now();

    // Convert the timepoint to a time_t object
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

    // Convert the time_t object to a string
    std::stringstream ss;
    ss << std::put_time(std::localtime(&currentTime_t), "%Y-%m-%d %H:%M:%S");

    // Get the string representation of the current timestamp
    return ss.str();
}

namespace fs = std::filesystem;
void mkdir(std::string directoryPath)
{
    // Check if the directory already exists
    if (!fs::exists(directoryPath)) {
        // Create the directory
        fs::create_directory(directoryPath);
        std::cout << fmt::format("Directory {} created successfully.",directoryPath)<< std::endl;
    } else {
        std::cout << fmt::format("Directory {} already exists successfully.",directoryPath) << std::endl;
    }

}

Color cflavours[6] = {BLUE,RED,GREEN,PINK,YELLOW,PURPLE};
int main(int argc, char* argv[])
{
    #define RANDOM
    #ifdef RANDOM
    std::random_device rd; // Seed for the random number generator
    std::mt19937 gen(rd()); // Standard Mersenne Twister engine
    std::uniform_real_distribution<double> dis(-1.0, 1.0); // Distribution for random double values between 0.0 and 1.0

    int rows = 6; // Number of rows in the 2D vector
    int cols = rows; // Number of columns in each row

    std::vector<std::vector<double>> flavourMatrix(rows, std::vector<double>(cols));

    // Fill the vector with random double values
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            flavourMatrix[i][j] = dis(gen);
        }
    }
    #endif

    // #define RANDOM
    #ifdef MANUAL
    std::vector<std::vector<double>> flavourMatrix = {
        {1.0 ,-0.1,0.3,-0.1},
        {-1.0,0.1,-1.0,0.1},
        {1.0 ,0.4,-1.0,0.4},
        {-1.0,0.1,-1.0,0.1},
    };
    #endif

    double beta = 0.01;
    double range = 0.1;
    ParticleSystem system(6000,0.1,range,50,0.0025,beta,flavourMatrix);
    // system.create_pool(poolSize);
    int width  = 800;
    int height = 800;

    InitWindow(width, height, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    std::cout << "ParticleUniverse generated" << std::endl;
    auto flavour = system.getFlavour();

    #ifdef RECORD
    std::string recording_name = time_now();
    mkdir(fmt::format("./recordings/{}",recording_name));
    #endif
    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose())
    {
        // sleep(1);
        BeginDrawing();
        ClearBackground(BLACK);
        
        auto [X, Y] = system.step();
        for(int n = 0 ; n < X.size() ; n++)
        {
            float x = (X[n])*width;
            float y = (Y[n])*height;
            Vector2 center = {.x = x, .y = y};
            DrawCircleGradient(x,y,1,Fade(cflavours[flavour[n]],1),Fade(cflavours[flavour[n]],0.2));


            #ifdef DEBUG
            DrawCircleGradient(x,y,width*beta,Fade(BLUE,0),Fade(GREEN,0.2));
            DrawRing(center,width*range,width*range-1,0, 360, 0,Fade(RED,1));
            #endif
            // DrawCircleGradient(x,y,10,Fade(allColors[particle.color],0.2),Fade(allColors[particle.color],0.0));
        }

        #ifdef RECORD
        TakeScreenshot(fmt::format("/recordings/{}/{}.png",recording_name,time_now()).c_str());
        #endif

        EndDrawing();
    }

    CloseWindow();

    return 0;
};