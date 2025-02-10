#!/bin/bash

echo "Select the process to run:"
echo "1) Main Application"
echo "2) Targets and Obstacles Generator"
read -p "Enter your choice (1 or 2): " choice

case $choice in
    1)
        echo "Running Main Application..."
        ./blackboard_process
        ;;
    2)
        echo "Running Targets and Obstacles Generator..."
        ./obstacle_process &
        ./target_process &
        wait # Wait for both processes to finish
        ;;
    *)
        echo "Invalid choice. Please enter 1 or 2."
        ;;
esac
