# Installation Guide - Survival Terminal v1.0

## System Requirements

### Basic Dependencies
- GCC compiler with C11 support
- CMake (version 3.10 or higher)
- Make
- Git

### Necessary Libraries
- cJSON: For handling JSON files
- pthread: For handling threads (usually included in the system)
- libm: Math library (usually included)

### Tools for Testing and Coverage (Optional)
- Unity Testing Framework: Automatically downloaded during compilation
- gcov: For coverage analysis
- lcov: For generating coverage reports
- genhtml: For generating HTML reports

## Installing Dependencies

### Ubuntu/Debian

 ```bash 
sudo apt update
sudo apt install build-essential cmake git
sudo apt install libcjson-dev
sudo apt install gcc lcov
``` 

## Compiling the Project

### Using CMake

#### Basic Compilation

```bash
# Create build directory
mkdir build && cd build

# Configure the project
cmake -DBUILD_TESTS=ON -DENABLE_COVERAGE=ON ..

# Compile
make

# The executable will be in build/so-i-24-chp2-FedericaMayorga01
``` 

#### Compilation with Tests

```bash
mkdir build && cd build
cmake -DBUILD_TESTS=ON -DENABLE_COVERAGE=ON ..
make

# Run tests
ctest --verbose
```

#### Compilation with Coverage

```bash
mkdir build && cd build
cmake -DBUILD_TESTS=ON -DENABLE_COVERAGE=ON ..
make

# Run tests and generate coverage
make coverage

# The coverage report is generated in HTML format and can be viewed by opening the `coverage/index.html` file in a web browser.
firefox coverage/index.html
```

## Execution

### Interactive Mode

```bash
./build/so-i-24-chp2-FedericaMayorga01
```

## Project Structure

``` 
project/
├── src/                  # Source code
│   ├── main.c             # Entry point
│   ├── shell.c            # Main shell functions
│   ├── commands.c         # Internal commands
│   ├── executions.c       # Handling command execution
│   └── monitor.c          # Monitor integration
├── include/              # Headers
├── tests/                # Unit tests
│   ├── test_commands.c
│   └── test_shell.c
├── build/                # Compiled files
├── config.json            # Monitor configuration
├── CMakeLists.txt         # CMake configuration
├── Makefile               # Alternative Makefile
└── INSTALL.md             # This guide
``` 

## Monitor Configuration

The config.json file contains the monitoring system's configuration:
JSON

``` json
{
    "metrics": {
        "cpu": true,
        "memory": true,
        "disk": true,
        "network": true,
        "processes": true,
        "context_switches": true
    }
}
``` 

Modify this file to enable or disable specific metrics.