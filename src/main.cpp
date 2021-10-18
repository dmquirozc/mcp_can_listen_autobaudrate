//#include <Arduino.h>

#include <MCP_CAN_lib/mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

#define CAN0_INT 2 
#define MCP_CS_PIN  9                      // Set INT to pin 2
MCP_CAN can0(MCP_CS_PIN);                               // Set CS to pin 10
bool dataReaded = false;
unsigned short baud_counter = 0;

bool spi_start(void)
{
    digitalWrite(MCP_CS_PIN, LOW);

    return true;
}

uint8_t spi_transmit(uint8_t data)
{
    return SPI.transfer(data);
}

bool spi_stop(void)
{
    digitalWrite(MCP_CS_PIN, HIGH);

    return true;
}
void mcp2515_reset()
{
    spi_start();
    spi_transmit(0xC0);
    spi_stop();
}

void printBaud(unsigned short baud);
unsigned long timeToSample = 0;
unsigned long timerToSample = 10000; //10 sec of samples before change baud rate

void setup()
{
  Serial.begin(115200);
  //mcp2515_reset();
  baud_counter = CAN_500kBPS;
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(can0.begin(MCP_ANY, baud_counter, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  can0.setMode(MCP_LISTENONLY); 

  pinMode(CAN0_INT, INPUT); 
  Serial.println("MCP2515 AUTOBAUD DETECTION");
  Serial.print("Actual ");
  printBaud(baud_counter);
  timeToSample = millis();
}

void loop()
{
  
  if(!digitalRead(CAN0_INT))                         // If can0_INT pin is low, read receive buffer
  {
    if(!dataReaded){
      Serial.print("Detected ");
      printBaud(baud_counter);
    }
    can0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
     
    if((rxId & 0x80000000) == 0x80000000){
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    }     // Determine if ID is standard (11 bits) or extended (29 bits)
    else
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
    
      
  
    Serial.print(msgString);
  
    if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
      sprintf(msgString, " REMOTE REQUEST FRAME");
      Serial.print(msgString);
    } else {
      for(byte i = 0; i<len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
      }
    }

    
    delay(30);    
    Serial.println();
    dataReaded = true;
  }

  if(!dataReaded && baud_counter < CAN_5kBPS && (timeToSample + timerToSample < millis()))
  {
    baud_counter++;
    timeToSample = millis();
    mcp2515_reset();
    delay(1000);
    if(can0.begin(MCP_ANY, baud_counter, MCP_16MHZ) != CAN_OK){
      baud_counter--;
      Serial.println("Cannot modify Speed");
    }else{
      Serial.print("Actual ");
      printBaud(baud_counter);
      can0.setMode(MCP_NORMAL);
    }
  }else{
    if(baud_counter == CAN_5kBPS){
      baud_counter++;
      printBaud(baud_counter);
    }
  }
}

void printBaud(unsigned short baud)
{
switch (baud) 
  {
    case (CAN_5kBPS):                                               //   5Kbps
      Serial.println("CAN Speed: 5kBPS");
    break;

    case (CAN_10kBPS):                                              //  10Kbps
      Serial.println("CAN Speed : 10kBPS");
    break;

    case (CAN_15k6BPS):
      Serial.println("CAN Speed : 15k66BPS");
    break;

    case (CAN_20kBPS):                                              //  20Kbps
      Serial.println("CAN Speed : 20kBPS");
    break;

    case (CAN_31k25BPS):
      Serial.println("CAN Speed : 31k25BPS");
    break;

    case (CAN_32kBPS): 
      Serial.println("CAN Speed : 32kBPS");
    break;

    case (CAN_33k3BPS):                                              //  20Kbps
      Serial.println("CAN Speed : 33k33BPS");
    break;

    case (CAN_40kBPS):                                              //  40Kbps
      Serial.println("CAN Speed : 40kBPS");
    break;

    case(CAN_41k6BPS):
      Serial.println("CAN Speed : 41k66BPS");
    break;

    case(CAN_44k4BPS):
      Serial.println("CAN Speed : 44k44BPS");
    break;

    case (CAN_50kBPS):    
      Serial.println("CAN Speed : 50kBPS");
    break;

    case (CAN_55k5BPS):    
      Serial.println("CAN Speed : 55k55BPS");
    break;

    case (CAN_62k5BPS):    
      Serial.println("CAN Speed : 62k5BPS");
    break;

    case (CAN_66k6BPS):    
      Serial.println("CAN Speed : 66k66BPS");
    break;

    case (CAN_80kBPS):                                              //  80Kbps
      Serial.println("CAN Speed : 80kBPS");
    break;

    case (CAN_83k3BPS):                                              //  80Kbps
      Serial.println("CAN Speed : 83k33BPS");
    break;

    case (CAN_88k8BPS):                                              //  80Kbps
      Serial.println("CAN Speed : 88k88BPS");
    break;

    case (CAN_100kBPS):                                             // 100Kbps
      Serial.println("CAN Speed : 100kBPS");
    break;

    case (CAN_106k6BPS):                                             // 100Kbps
      Serial.println("CAN Speed : 106k66BPS");
    break;

    case (CAN_111k1BPS):                                             // 100Kbps
      Serial.println("CAN Speed : 111k11BPS");
    break;

    case (CAN_121k2BPS):                                             // 125Kbps
      Serial.println("CAN Speed : 121k21BPS");
    break;

    case (CAN_125kBPS):                                             // 125Kbps
      Serial.println("CAN Speed : 125kBPs");
    break;

    case (CAN_133k3BPS):                                             // 125Kbps
      Serial.println("CAN Speed : 133k33BPS");
    break;

    case (CAN_145k4BPS):                                             // 125Kbps
      Serial.println("CAN Speed : 145k45BPS");
    break;

    case (CAN_148k1BPS):                                             // 125Kbps
      Serial.println("CAN Speed : 148k18BPS");
    break;

    case (CAN_160kBPS):                                             // 125Kbps
      Serial.println("CAN Speed : 160kBPS");
    break;

    case (CAN_166k6BPS):                                             // 125Kbps
      Serial.println("CAN Speed : 166k66BPS");
    break;

    case (CAN_181k8BPS):                                             // 125Kbps
      Serial.println("CAN Speed : 181k81BPS");
    break;

    case (CAN_200kBPS):                                             // 200Kbps
      Serial.println("CAN Speed : 200kBPS");
    break;

    case (CAN_222k2BPS):                                             // 200Kbps
      Serial.println("CAN Speed :222k22BPS");
    break;

    case (CAN_250kBPS):                                             // 250Kbps
      Serial.println("CAN Speed : 250kBPS");
    break;

    case (CAN_266k6BPS):                                             // 200Kbps
      Serial.println("CAN Speed :266k66BPS");
    break;

    case (CAN_320kBPS):                                             // 200Kbps
      Serial.println("CAN Speed : 320kBPS");
    break;

    case (CAN_333k3BPS):                                             // 200Kbps
      Serial.println("CAN Speed : 333k33BPS");
    break;

    case (CAN_400kBPS):                                             // 500Kbps
      Serial.println("CAN Speed : 400kBPS");
    break;

    case (CAN_444k4BPS):                                             // 500Kbps
      Serial.println("CAN Speed : 444k44BPS");
    break;

    case (CAN_500kBPS):                                             // 500Kbps
      Serial.println("CAN Speed : 500kBPS");
    break;

    case (CAN_666k6BPS):                                             // 500Kbps
      Serial.println("CAN Speed : 666k66BPS");
    break;

    case (CAN_800kBPS):                                             // 500Kbps
      Serial.println("CAN Speed : 800kBPS");
    break;

    case (CAN_1000kBPS):                                            //   1Mbps
      Serial.println("CAN Speed : 1000kBPS");
    break;  

    default:
      Serial.println("CAN Speed not Found");
  }
}