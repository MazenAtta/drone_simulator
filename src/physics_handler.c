#include "physics_handler.h"
#include <math.h>

// Initialize the drone's parameters
void init_drone(Drone *drone) {
    drone->x = 10.0;
    drone->y = 10.0;
    drone->vx = 0.0;
    drone->vy = 0.0;
    drone->ax = 0.0;
    drone->ay = 0.0;
    drone->prev_x = 10.0;
    drone->prev_y = 10.0;
    drone->command_force_x = 0.0;
    drone->command_force_y = 0.0;
    drone->prev_total_command_force_x = 0.0;
    drone->prev_total_command_force_y = 0.0;
}

// Calculate total forces (command, repulsion, attraction, and drag)
void calculate_total_forces(Drone *drone, Obstacle obstacles[], Target targets[]) {
    // Start with command forces and accumulate continuous command force
    float total_force_x = drone->command_force_x + drone->prev_total_command_force_x;
    float total_force_y = drone->command_force_y + drone->prev_total_command_force_y;

    // Calculate drag forces (viscous friction)
    float drag_force_x = -DRONE_FRICTION * drone->vx;
    float drag_force_y = -DRONE_FRICTION * drone->vy;

    // Add drag forces
    total_force_x += drag_force_x;
    total_force_y += drag_force_y;

    // Add repulsive forces
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].x != -1) {
            float dx = drone->x - obstacles[i].x;
            float dy = drone->y - obstacles[i].y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance < PERCEPTION_RADIUS && distance > 0) {
                float repulsion_factor = REPULSIVE_CONSTANT * (1.0 / distance - 1.0 / PERCEPTION_RADIUS) / (distance * distance);
                total_force_x += repulsion_factor * (dx / distance);
                total_force_y += repulsion_factor * (dy / distance);
            }
        }
    }

    // (Optional) Still under construction
    /*// Add attractive forces
    for (int i = 0; i < MAX_TARGETS; i++) {
        if (targets[i].x != -1) {
            float dx = targets[i].x - drone->x;
            float dy = targets[i].y - drone->y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance < PERCEPTION_RADIUS && distance > 0) {  // Avoid zero division
                float attraction_factor = ATTRACTIVE_CONSTANT * (1.0 / distance - 1.0 / PERCEPTION_RADIUS) / (distance * distance);
                total_force_x += attraction_factor * dx / distance;
                total_force_y += attraction_factor * dy / distance;
            }
        }
    }*/
    
    /*if (total_force_x < 1) {
        total_force_x = 0;
    } else if (total_force_y < 1) {
        total_force_y = 0;
    }*/

    // Store the total force for use in the update function
    drone->force_x = total_force_x;
    drone->force_y = total_force_y;
}

// Update the drone's dynamics using Newton's laws
void update_drone(Drone *drone) {
    // Calculate acceleration from the total force
    drone->ax = drone->force_x / DRONE_MASS;
    drone->ay = drone->force_y / DRONE_MASS;

    // Update velocity using v = v + a * TIME_STEP
    drone->vx += drone->ax * TIME_STEP;
    drone->vy += drone->ay * TIME_STEP;

    // Update position using x = x + v * TIME_STEP + 0.5 * a * TIME_STEP^2
    float new_x = drone->x + drone->vx * TIME_STEP + 0.5 * drone->ax * TIME_STEP * TIME_STEP;
    float new_y = drone->y + drone->vy * TIME_STEP + 0.5 * drone->ay * TIME_STEP * TIME_STEP;

    // Update the drone's state
    drone->prev_x = drone->x;
    drone->prev_y = drone->y;
    drone->x = new_x;
    drone->y = new_y;

    // Accumulate the command force for continuous effect
    drone->prev_total_command_force_x += drone->command_force_x;
    drone->prev_total_command_force_y += drone->command_force_y;

    // Reset accumulated command force when the velocity is small (stopping condition)
    if (fabs(drone->vx) < 0.01 && fabs(drone->vy) < 0.01) {
        drone->vx = 0.0;
        drone->vy = 0.0;
        drone->prev_total_command_force_x = 0.0;
        drone->prev_total_command_force_y = 0.0;
    }
}
