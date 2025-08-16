# StealthProc

StealthProc is a Windows kernel-mode driver and client utility for hiding processes by unlinking them from the active process list. It is intended for educational and research purposes.

## Features
- Hide a process by name from the Windows process list
- C++14 client utility for sending commands to the driver
- Debug macros for detailed logging

## Components
- **StealthProc (Driver):** Implements process hiding via EPROCESS manipulation
- **StealthProcClient (User-mode):** Command-line tool to send process names to the driver

## LLD
- To view LLD, open "StealthProc LLD.excalidraw" in https://excalidraw.com/.

## Build Instructions
### Prerequisites
- Windows Driver Kit (WDK)
- Visual Studio (C++14 support)

### Building the Driver
1. Open the solution in Visual Studio
2. Build the `StealthProc` project (kernel-mode driver)

### Building the Client
1. Build the `StealthProcClient` project (C++14)

## Usage
1. Install the `StealthProc` driver (requires administrator privileges)
2. Run the client:StealthProcClient.exe <process_name>   Example:
StealthProcClient.exe notepad.exe
## Author
- Arpit Mathur

## License
This project is provided for educational purpo	ses. See individual source files for copyright.