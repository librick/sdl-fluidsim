#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <SDL2/SDL.h>
#include "../lib/fluidsim/fluidsim.h"
#include "../lib/color/src/color/color.hpp"
#include <iostream>
#include <vector>
#include <limits>
#include "./particle.h"

#include "../lib/imgui/imgui_impl_sdl.h"
#include "../lib/imgui/imgui_impl_sdlrenderer.h"
#include "../lib/imgui/imgui.h"

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512
#define WINDOW_TITLE "Fluids"
#define NUM_PARTICLES 8

FluidSquare *fluid;
float theta = 0.0f;
float dTheta = 0.005f;
float strength = 0.1f;

struct RGBA {
    uint8_t r, g, b, a;
};

std::vector<Particle*> particles;


float easeOutCirc(float x) {
    return std::sqrt(1 - std::pow(x - 1, 2));
}
float minMaxNorm(float min, float max, float x) {
    return (x-min)/(max-min);
}
float mapDensityToValue(float d, float dmax) {
    float norm = minMaxNorm(0,dmax,d);
    float mapped = easeOutCirc(norm);
    float maxV = 100;
    return mapped * maxV;
}
float mapDensityToHue(float d, float dmin, float dmax) {
    float norm = minMaxNorm(dmin,dmax,d);
    float mapped = easeOutCirc(norm);
    float max = 360;
    return mapped * max;
}
RGBA getRGBFromDensity(float d, float dmin, float dmax)
{
    float h = mapDensityToHue(d, dmin, dmax);
    float v = mapDensityToValue(d, dmax);
    uint8_t a = 255;

    color::rgba<float> outColor = color::rgba<float>({0, 0, 0});
    // HSV maxes are 360, 100, 100.
    color::hsv<double> myColor = color::hsv<double>({h, 100, v});
    outColor = myColor;

    uint8_t r = floor(color::get::red(outColor) * 255);
    uint8_t g = floor(color::get::green(outColor) * 255);
    uint8_t b = floor(color::get::blue(outColor) * 255);

    return RGBA{r, g, b, a };
}

void renderDensity(SDL_Renderer *renderer)
{
    float maxDens = 0.0f;
    float minDens = std::numeric_limits<float>::infinity();
    for (int x = 0; x < N * N; x++) {
        if (fluid->density[x] > maxDens) maxDens = fluid->density[x];
        if (fluid->density[x] < minDens) minDens = fluid->density[x];
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            SDL_Rect rect;
            rect.x = i * SCALE;
            rect.y = j * SCALE;
            rect.w = SCALE;
            rect.h = SCALE;

            float d = fluid->density[IX(i, j)];
            RGBA rgba = getRGBFromDensity(d, minDens, maxDens);

            char r = rgba.r;
            char g = rgba.b;
            char b = rgba.g;
            char a = rgba.a;

            SDL_SetRenderDrawColor(renderer, r, g, b, a);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void initFluid()
{
    float diffusion = 0.1;
    float viscosity = 0.0;
    float dt = 0.01;
    fluid = FluidSquareCreate(diffusion, viscosity, dt);
}
int main(int argc, const char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Failed to initialise SDL\n";
        return -1;
    }

    // Create window
    std::string title = WINDOW_TITLE;
    const char *ctitle = title.c_str();
    int posX = SDL_WINDOWPOS_UNDEFINED;
    int posY = SDL_WINDOWPOS_UNDEFINED;
    Uint32 flags = SDL_WINDOW_OPENGL ^ SDL_WINDOW_BORDERLESS;
    SDL_Window *window = SDL_CreateWindow(ctitle, posX, posY, WINDOW_WIDTH, WINDOW_HEIGHT, flags);
    if (window == nullptr) {
        SDL_Log("Could not create a window: %s", SDL_GetError());
        return -1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_Log("Could not create a renderer: %s", SDL_GetError());
        return -1;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);
    // ImGUI controls
    bool isRunning = true;
    
    // Generate particles
    srand(time(NULL));
    for(int i = 0; i < NUM_PARTICLES; i++) {
        particles.push_back(new Particle());
    }

    int cx = floor(float(N) / 2);
    int cy = floor(float(N) / 2);
    initFluid();

    while (true) {
        if(isRunning) {
            for(Particle* p : particles) {
                int x = p->getX();
                int y = p->getY();
                float ax = cx * cos(p->angle) * strength;
                float ay = cy * sin(p->angle) * strength;
                FluidSquareAddDensity(fluid, x, y, 100);
                FluidSquareAddVelocity(fluid, x, y, ax, ay);
                p->update();
            }
        }

        theta += dTheta;

        SDL_Event event;
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            if(event.type == SDL_KEYDOWN) {
                char keyDown = event.key.keysym.scancode;
                if (keyDown == SDL_SCANCODE_SPACE) {
                    isRunning = !isRunning;
                }
            }
        }
        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Graphics Demo");
        //ImGui::Button("Start");
        //ImGui::Button("Stop");
        ImGui::Text("FluidSim Controls");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        if (ImGui::Button("Start")) {
            isRunning = true;
            std::cout << "started simulation" << std::endl;
        }   
        if (ImGui::Button("Stop")) {
            isRunning = false;
            std::cout << "stopped simulation" << std::endl;
        }
        if (ImGui::Button("Add Particles")) {
            for(int i = 0; i < NUM_PARTICLES; i++) {
                particles.push_back(new Particle());
            }
        }
        ImGui::End();
        ImGui::Render();

        if(isRunning == true) {
            FluidSquareStep(fluid);
        }
    
        // Render everything
        renderDensity(renderer);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }

    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    FluidSquareFree(fluid);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
