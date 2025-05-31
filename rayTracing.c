// THIS IS A C PROGRAM THAT SIMULATES BASIC RAY TRACING WITH A MOVING CIRCLE AND RAYS 

#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define WIDTH 1200
#define HEIGHT 600
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_BLACK 0x00000000
#define COLOR_RAY 0xffd43b
#define COLOR_RAY_BLUR 0xbd6800
#define RAYS_NUMBER 1000
#define RAY_THICKNESS 1

struct Circle
{
    double x;
    double y;
    double r;
};

struct Ray
{
    double x_start, y_start;
    double angle;
    double x_end, y_end;
};

void FillCircle(SDL_Surface *surface, struct Circle circle, Uint32 color)
{
    for (double x = circle.x - circle.r; x < circle.x + circle.r; x++)
    {
        for (double y = circle.y - circle.r; y < circle.y + circle.r; y++)
        {
            if ((x - circle.x) * (x - circle.x) + (y - circle.y) * (y - circle.y) <= circle.r * circle.r)
            {
                SDL_Rect pixel = (SDL_Rect){x, y, 3, 3}; // Create a rectangle of size 1x1 for the pixel
                SDL_FillRect(surface, &pixel, color);    // Fill the pixel with the specified color
            }
        }
    }
}

void generate_rays(struct Circle circle, struct Ray rays[RAYS_NUMBER])
{
    for (int i = 0; i < RAYS_NUMBER; i++)
    {
        double angle = ((double)i / RAYS_NUMBER) * 2 * M_PI;
        struct Ray ray = {circle.x , circle.y, angle};
        rays[i] = ray;
        // printf("angle : %f", angle);
    }
}

void FillRays(SDL_Surface* surface , struct Ray rays[RAYS_NUMBER], Uint32 color, Uint32 blur_color, struct Circle object)
{   
    double radius_sq = pow(object.r, 2);

    for (int i = 0; i < RAYS_NUMBER; i++)
    {
        struct Ray ray = rays[i];
        int end_of_screen = 0;
        int object_hit = 0;

        double step = 1;
        double x_draw = ray.x_start;
        double y_draw = ray.y_start;
        while( !end_of_screen & !object_hit){
            x_draw += step*cos(ray.angle);
            y_draw += step*sin(ray.angle);

            SDL_Rect pixel = (SDL_Rect){x_draw, y_draw, RAY_THICKNESS, RAY_THICKNESS}; // Create a rectangle of size 1x1 for the pixel

            double blur_size = 1.5 * RAY_THICKNESS;

            SDL_Rect pixel_blur = (SDL_Rect){x_draw, y_draw, blur_size, blur_size}; // Create a rectangle of size 1x1 for the pixel
            SDL_FillRect(surface, &pixel_blur, blur_color);
            SDL_FillRect(surface, &pixel, color); // Fill the pixel with the specified color
            if(x_draw < 0 || x_draw >= WIDTH || y_draw < 0 || y_draw >= HEIGHT)
            {
                end_of_screen = 1; // If the ray goes out of bounds, set end_of_screen to true
            }

            if ((x_draw - object.x) * (x_draw - object.x) + (y_draw - object.y) * (y_draw - object.y) <= radius_sq)
            {
              break; // If the ray hits the object, break the loop 
            }
        }
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0); // Create a window with the specified title and dimensions

    SDL_Surface *surface = SDL_GetWindowSurface(window); // Get the surface of the window to draw on

    // SAMPLE DRAWING A RECTANGLE
    // ------------------------------------------------------------
    // SDL_Rect rect = (SDL_Rect) {200, 200, 200, 200}; // Define a rectangle with position (200, 200) and size (200, 200)
    // SDL_FillRect(surface , &rect, COLOR_WHITE); // Fill the rectangle with white color
    // ------------------------------------------------------------

    struct Circle circle = {200, 200, 40}; // Define a circle with center (300, 300) and radius 100

    struct Circle shadow_circle = {650, 300, 140};

    SDL_Rect erase_rect = {0, 0, WIDTH, HEIGHT}; // Define a rectangle that covers the entire window surface

    struct Ray rays[RAYS_NUMBER];
    generate_rays(circle, rays); // Generate rays from the circle

    double obstacle_speed_y = 5; 

    int simulationRunning = 1; // Flag to control the simulation loop
    SDL_Event event;           // Event structure to handle events

    while (simulationRunning)
    {
        while (SDL_PollEvent(&event))
        { // Poll for events
            if (event.type == SDL_QUIT)
            {                          // If the quit event is triggered
                simulationRunning = 0; // Set the flag to stop the simulation
            }
            if (event.type == SDL_MOUSEMOTION && event.motion.state != 0)
            {
                circle.x = event.motion.x; // Update the circle's x position to the mouse x position
                circle.y = event.motion.y; // Update the circle's y position to the mouse y position
                generate_rays(circle, rays);
            }
        }
        SDL_FillRect(surface, &erase_rect, COLOR_BLACK); // Clear the surface with a transparent color

        FillRays(surface, rays, COLOR_RAY, COLOR_RAY_BLUR, shadow_circle); // Fill the rays with gray color, simulating shadows

        FillCircle(surface, circle, COLOR_WHITE);        // Fill the circle with white color
        FillCircle(surface, shadow_circle, COLOR_WHITE); // Fill the shadow circle with white color

        
        
        shadow_circle.y += obstacle_speed_y; // Move the shadow circle downwards

        if(shadow_circle.y - shadow_circle.r < 0 || shadow_circle.y+ shadow_circle.r > HEIGHT)
        {
            obstacle_speed_y = -obstacle_speed_y; // Reverse the direction of the shadow circle if it goes out of bounds
        }

        SDL_UpdateWindowSurface(window); // Update the window surface to reflect the changes

        SDL_Delay(10); // Delay to show the window before quitting
    }
}
