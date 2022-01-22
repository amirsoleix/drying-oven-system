# Drying Oven System Using 8051 Microcontrollers  
The system consists of two 8051 microcontrollers (8031 can be used as an alternative for one of the microcontrollers). After initialization, the first microcontroller sends a 1.8KHz train pulse to start the system. Upon receiving 10 pulses with the given frequency, the A/D module starts the operation and sends the temperature of the drying oven to the first microcontroller through an 8255 PPI.  
The microcontroller then transmits the temperature to the seconds microcontroller using serial communication with baud rate of 19200. The second system saves the data after the serial communication is finished, within an internal memory, and starts rewriting the data once the end of the memory is reached.  
The temperature is concurrently displayed in an LCD module using another 8255 PPI connected to the second microcontroller.  
## Initialization  
The setting for initialization of A/D module and LCD module is sent to them through 8255 PPI before the starting train pulse, so that both devices are ready to use. The setting for serial communication and timers is also set in advance to eliminate the delay in the middle of operation.  
There are two timers in an 8051 microcontroller. In the first system, the first timer is used to synchronize temperature receiving and the second timer, is first used to create a train pulse with the appropriate freqeuncy and then is set to allow for 19200 baud rate in serial communication.
## System Schematic  
The schematic for the systems is drawn in the report and the memory mapping is discussed in detail. The final layout in Proteus after minor modifications is present in the `src` folder of the project.
## Assembly Code  
The code for both systems is written in C ,commented properly and then translated into 8051 assembly code and HEX commands using Keil uVision 5. The resulting HEX file for both systems is given to microcontrollers in Proteus and the results are validated.  

P.S. More detail on the implementation is given in `report.pdf`
