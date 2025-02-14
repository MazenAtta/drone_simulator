#!/bin/bash

# Function to clean up background processes
cleanup() {
    echo "Terminating background processes..."
    pkill -P $$  # Kill all child processes of this script
    exit 0
}

# Trap SIGINT (Ctrl+C) and call the cleanup function
trap cleanup SIGINT

# Build the project
echo "Building the project..."
mkdir -p build
cd build || exit 1
cmake .. && make || { echo "Build failed."; exit 1; }
cd ..

echo "Select the process to run:"
echo "1) Main Application"
echo "2) Targets and Obstacles Generator"
echo "3) Both"
read -p "Enter your choice (1 or 2 or 3): " choice

case $choice in
    1)
        echo "Running Main Application..."
        ./build/blackboard_process
        ;;
    2)
        echo "Running Targets and Obstacles Generator..."
        ./build/obstacle_process &
        ./build/target_process &
        wait # Wait for both processes to finish
        ;;
    3)
        echo "Running Targets and Obstacles Generator..."
        ./build/blackboard_process &
        ./build/obstacle_process &
        ./build/target_process &
        wait # Wait for both processes to finish
        ;;
    *)
        echo "Invalid choice. Please enter 1 or 2."
        ;;
esac
