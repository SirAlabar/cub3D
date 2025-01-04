#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to check if a port is in use
is_port_in_use() {
    lsof -i:"$1" >/dev/null 2>&1
}

# Check if npm is installed
if ! command_exists npm; then
    echo -e "${RED}Error: npm is not installed${NC}"
    echo "Please install Node.js and npm first"
    exit 1
fi

# Check if python3 is installed
if ! command_exists python3; then
    echo -e "${YELLOW}Warning: python3 is not installed${NC}"
    echo "Will try python instead..."
    if ! command_exists python; then
        echo -e "${RED}Error: Neither python3 nor python is installed${NC}"
        exit 1
    fi
    PYTHON_CMD="python"
else
    PYTHON_CMD="python3"
fi

# Check if port 8000 is available
if is_port_in_use 8000; then
    echo -e "${RED}Error: Port 8000 is already in use${NC}"
    echo "Please free up port 8000 and try again"
    exit 1
fi

# Create a function to cleanup background processes on script exit
cleanup() {
    echo -e "\n${YELLOW}Shutting down servers...${NC}"
    kill $SASS_PID 2>/dev/null
    kill $SERVER_PID 2>/dev/null
    exit 0
}

# Set up trap to catch script termination
trap cleanup SIGINT SIGTERM

# Start sass in background
echo -e "${GREEN}Starting Sass compiler...${NC}"
npm run sass &
SASS_PID=$!

# Start Python HTTP server in background
echo -e "${GREEN}Starting local server...${NC}"
$PYTHON_CMD -m http.server 8000 &
SERVER_PID=$!

# Print success message with URLs
echo -e "\n${GREEN}Development servers started successfully!${NC}"
echo -e "Local server: ${YELLOW}http://localhost:8000${NC}"
echo -e "\nPress Ctrl+C to stop all servers\n"

# Wait for both background processes
wait $SASS_PID $SERVER_PID
