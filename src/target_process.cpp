#include "target_handler.hpp"
#include "../DDS/src/TargetsPublisher.hpp"


void convert_to_dds_targets(const Target& src, Targets& dest)
{
    dest.targets_x().clear();
    dest.targets_y().clear();
    for (int i = 0; i < MAX_TARGETS; ++i)
    {
        dest.targets_x().push_back(src.x[i]);
        dest.targets_y().push_back(src.y[i]);
    }
    dest.targets_number(MAX_TARGETS);
}


int main() {
    const char *log_folder = "log"; // Path to the log folder
    const char *log_file = "log/target_log.txt"; // Path to the log file

    // Create log folder if it doesn't exist
    mkdir(log_folder, 0777);

    // Initialize DDS publisher
    TargetsPublisher* publisher = new TargetsPublisher();
    if (!publisher->init("TargetTopic")) {
        error_exit("Failed to initialize DDS Publisher");
    }

    Target targets; // Structure to hold target data
    Targets dds_targets; // DDS message type
    init_targets(&targets); // Initialize the targets

    time_t last_generation_time = 0; // Last time targets were generated
    time_t last_log_time = 0; // Last time execution was logged
    while (1) {
        time_t current_time = time(NULL); // Get the current time

        // Generate new targets every 10 seconds
        if (current_time - last_generation_time >= 10) {
            generate_targets(&targets);
            convert_to_dds_targets(targets, dds_targets); // Convert to DDS message type
            last_generation_time = current_time; // Update the last generation time
        }
        publisher->publish(dds_targets); // Publish target data using DDS

        // Log execution every second
        if (current_time - last_log_time >= 1) {
            log_execution(log_file);
            last_log_time = current_time; // Update the last log time
        }
    }
    delete publisher;
    return 0;
}