#ifndef PHYSICS_HANDLER_H
#define PHYSICS_HANDLER_H

#include "obstacle_target_handler.h"

// Drone mass and friction constants
#define DRONE_MASS 1.0
#define DRONE_FRICTION 1.0
#define COMMAND_FORCE_STEP 1.0
#define REPULSIVE_CONSTANT 10.0
#define ATTRACTIVE_CONSTANT 0.1
#define PERCEPTION_RADIUS 5.0
#define TIME_STEP .1  // 1 s as integration interval

// Struct to represent drone dynamics
typedef struct {
    float x, y;           // Position
    float vx, vy;         // Velocity
    float ax, ay;         // Acceleration
    float prev_x, prev_y;  // For Euler’s method
    float command_force_x, command_force_y;  // Command force components
    float prev_total_command_force_x, prev_total_command_force_y;
    float force_x,force_y;
} Drone;

// Initialize the drone's physics
void init_drone(Drone *drone);

// Update drone dynamics based on external forces
void update_drone(Drone *drone);

// Calculate all forces affecting the drone (command, repulsive, attractive)
void calculate_total_forces(Drone *drone, Obstacle obstacles[], Target targets[]);

#endif
