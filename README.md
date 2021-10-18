# mcp_can_listen_autobaudrate
This project was made to read data from CANBUS using MCP2515 module.
## Considerations
This is a test project using Amtel SAM3X8E (Arduino Due). If you want to use it, you must re configure it for your own board.
## Usage
Connect the pins of your MCP2515 can, to a CANBUS and run the program, this will scan a variety of possible baud rates until it gets data.
After getting data, the program will show you the Baud rate and it sarts to show the data readed from the bus.


