#include <Arduino.h>
#include <esp32ModbusRTU.h>
#include <algorithm>  // for std::reverse
#include <WiFi.h>
esp32ModbusRTU modbus(&Serial1, 16);  // use Serial1 and pin 16 as RTS

#define ENDIAN 1

//set pin 4 and 17 for Modbus RTU RS485
#define RXD1 17
#define TXD1 4

// set Slave Address 
#define SlaveAddress 0x01
#define Function 0x03

// set Starting Address of Registers

#define StateRegister 0x7000
#define VTRegister 0x7001
#define V_15Register   0x7003
#define WeightRegister 0x7005
#define SumVTRegister 0x7007
#define SumV15Register 0x7009
#define SumWghtRegister 0x700B         
#define BatchQuantityRegister 0x700D
#define AvgDensityXRegister 0x700F
#define AvgTemp1Register 0x7011
#define FlowRateRegister 0x7013
#define ProductNoRegister 0x7015
#define AdditivTypeRegister 0x7016
#define AddVTRegister 0x7017
#define SumAddVTRegister  0x7019
#define TargetAddRateRegister 0x701B
#define DateTimeRegister 0x701C

//set Quantity of Registers 

#define StateRegNum 0x0001
#define VTRegNum 0x0002
#define V_15RegNum   0x0002
#define WeightRegNum 0x0002
#define SumVTRegNum 0x0002
#define SumV15RegNum 0x0002
#define SumWghtRegNum 0x0002         
#define BatchQuantityRegNum 0x0002
#define AvgDensityXRegNum 0x0002
#define AvgTemp1RegNum 0x0002
#define FlowRateRegNum 0x0002
#define ProductNoRegNum 0x0001
#define AdditivTypeRegNum 0x0001
#define AddVTRegNum 0x0002
#define SumAddVTRegNum  0x0002
#define TargetAddRateRegNum 0x0001
#define DateTimeRegNum 0x0008


//set variables for loop
#define State 1
#define VT  2
#define V_15  3
#define Weight  4
#define SumVT 5
#define SumV15  6
#define SumWght 7         
#define BatchQuantity 8
#define AvgDensityX 9
#define AvgTemp1  10
#define FlowRate  11
#define ProductNo 12
#define AdditivType 13
#define AddVT 14
#define SumAddVT  15
#define TargetAddRate 16
#define DateTime  17


uint32_t UI4Set(char byte_0, char byte_1, char byte_2, char byte_3) {
#if ENDIAN == 1
  return byte_0 | ((uint32_t)byte_1) << 8 | ((uint32_t)byte_2) << 16 |
         ((uint32_t)byte_3) << 24;
#else
  return byte_3 | ((uint32_t)byte_2) << 8 | ((uint32_t)byte_1) << 16 |
         ((uint32_t)byte_0) << 24;
#endif
}

float FLTSet(char byte_0, char byte_1, char byte_2, char byte_3) {
  uint32_t flt = UI4Set(byte_0, byte_1, byte_2, byte_3);
  return *reinterpret_cast<float*>(&flt);
}

uint32_t FLTSet(char byte_0, char byte_1, char byte_2, char byte_3,
                   float* destination) {
  uint32_t FloatValue = UI4Set (byte_0, byte_1, byte_2, byte_3);
  *reinterpret_cast<uint32_t*>(destination) = FloatValue;
  return FloatValue;
}

void Start_Publish();

// Replace with your network credentials
const char* ssid = "Vodafone-315197";
const char* password = "7Px3UNgYhrH3eMD9";
String StateValue = "3";
String VTValue = "1002";
String V15Value = "1000";
String WeightValue = "950";
String SumVtValue = "100000111";
String SumV15Value = "99999999";
String SumWghtValue = "99999885";
String BatchQuantityValue = "3000";
String AvgDensityXValue = "840.2";
String AvgTemp1Value = "22.2";
String FlowRateValue = "3";
String ProductNoValue = "1";
String AdditivTypeValue = "1";
String AddVTValue = "1.123";
String SumAddVTValue = "12000000";
String TargetAddRateValue = "1000";
String DateTimeValue = "not set";

// Variable to store the HTTP request
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;


volatile int Register;
uint16_t Int_Data;
float Flt_Data;


// Set web server port number to 80
WiFiServer server(80);


void setup(){

  Serial.begin(115200); 
  Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1);  // Modbus connection
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

   modbus.onData([](uint8_t serverAddress, esp32Modbus::FunctionCode fc, uint16_t address, uint8_t* data, size_t length) {

 
     switch (Register-1){
      case State:
 //       Int_Data = (256*data[0]) + data[1];
        StateValue = String((256*data[0]) + data[1]);
        Serial.print("State=");
        Serial.print(StateValue);
        Serial.println("\n");
        break;
      case VT:
//        Flt_Data = FLTSet (data[3], data[2], data[1], data[0]);
        VTValue = String(FLTSet (data[3], data[2], data[1], data[0]));
        Serial.print("VT=");
        Serial.print(VTValue);
        Serial.println("\n");
        break;
      case V_15:
//        Flt_Data = FLTSet (data[3], data[2], data[1], data[0]);
        V15Value = String(FLTSet (data[3], data[2], data[1], data[0]));
        Serial.print("V15=");
        Serial.print(V15Value);
        Serial.println("\n");
        break;
      case Weight:
 //       Flt_Data = FLTSet (data[3], data[2], data[1], data[0]);
        WeightValue = String(FLTSet (data[3], data[2], data[1], data[0]));
        Serial.print("Weight=");
        Serial.print(WeightValue);
        Serial.println("\n");
        break;
      case SumVT:
//        Flt_Data = FLTSet (data[3], data[2], data[1], data[0]);
        SumVtValue = String(FLTSet (data[3], data[2], data[1], data[0]));
        Serial.print("SumVT=");
        Serial.print(SumVtValue);
        Serial.println("\n");
        break;
      case SumV15:
//        Flt_Data = FLTSet (data[3], data[2], data[1], data[0]);
        SumV15Value = String(FLTSet (data[3], data[2], data[1], data[0]));
        Serial.print("SumV15=");
        Serial.print(SumV15Value);
        Serial.println("\n");      
        break;
      case SumWght:
   //     Flt_Data = FLTSet (data[3], data[2], data[1], data[0]);
        SumWghtValue = String(FLTSet (data[3], data[2], data[1], data[0]));
        Serial.print("SumWght=");
        Serial.print(SumWghtValue);
        Serial.println("\n");     
        break;
      case BatchQuantity:
//        Flt_Data = FLTSet (data[3], data[2], data[1], data[0]);
        BatchQuantityValue = String(FLTSet (data[3], data[2], data[1], data[0]));
        Serial.print("BatchQuantity=");
        Serial.print(BatchQuantityValue);
        Serial.println("\n");     
        break;
      case AvgDensityX:
//        Flt_Data = FLTSet (data[3], data[2], data[1], data[0]);
        AvgDensityXValue = String(FLTSet (data[3], data[2], data[1], data[0]));
        Serial.print("AvgDensityX=");
        Serial.print(AvgDensityXValue);
        Serial.println("\n");
        break;
      case AvgTemp1:
//        Flt_Data = FLTSet (data[3], data[2], data[1], data[0]);
        AvgTemp1Value = String(FLTSet (data[3], data[2], data[1], data[0]));
        Serial.print("AvgTemp1=");
        Serial.print(AvgTemp1Value);
        Serial.println("\n");
        break;
      case FlowRate:
//        Flt_Data = FLTSet (data[3], data[2], data[1], data[0]);
        FlowRateValue = String(FLTSet (data[3], data[2], data[1], data[0]));
        Serial.print("FlowRate=");
        Serial.print(FlowRateValue);
        Serial.println("\n");      
        break;
      case ProductNo:
//        Int_Data = (256*data[0]) + data[1];
        ProductNoValue = String((256*data[0]) + data[1]);
        Serial.print("ProductNo=");
        Serial.print(ProductNoValue);
        Serial.println("\n");      
        break;
      case AdditivType:
//        Int_Data = (256*data[0]) + data[1];
        AdditivTypeValue = String((256*data[0]) + data[1]);
        Serial.print("AdditivType=");
        Serial.print(AdditivTypeValue);
        Serial.println("\n");      
        break;
      case AddVT:
//        Flt_Data = FLTSet (data[3], data[2], data[1], data[0]);
        AddVTValue = String(FLTSet (data[3], data[2], data[1], data[0]));
        Serial.print("AddVT=");
        Serial.print(AddVTValue);
        Serial.println("\n");      
        break;
      case SumAddVT:
//        Flt_Data = FLTSet (data[3], data[2], data[1], data[0]);
        SumAddVTValue = String(FLTSet (data[3], data[2], data[1], data[0]));
        Serial.print("SumAddVT=");
        Serial.print(SumAddVTValue);
        Serial.println("\n");      
        break;
      case TargetAddRate:
//        Flt_Data = FLTSet (data[3], data[2], data[1], data[0]);
        TargetAddRateValue = String(FLTSet (data[3], data[2], data[1], data[0]));
        Serial.print("TargetAddRate=");
        Serial.print(TargetAddRateValue);
        Serial.println("\n");       
        break;
      case DateTime:
        break;                                                                                                                
    }
  });

  modbus.onError([](esp32Modbus::Error error) {
    Serial.printf("error: 0x%02x\n\n", static_cast<uint8_t>(error));
   
  });  
  
  modbus.begin();
  
}
void loop(){
  // put your main code here, to run repeatedly: 
  static uint32_t lastMillis = 0; 
  for (Register = State; Register <= DateTime; Register++ ){ 
    lastMillis = millis(); 
    while(millis() - lastMillis < 500) { }
    switch (Register){
      case State:
        modbus.readHoldingRegisters(SlaveAddress,StateRegister,StateRegNum);
        break;
      case VT:
        modbus.readHoldingRegisters(SlaveAddress,VTRegister,VTRegNum);
        break;
      case V_15:
        modbus.readHoldingRegisters(SlaveAddress,V_15Register,V_15RegNum);
        break;
      case Weight:
        modbus.readHoldingRegisters(SlaveAddress,WeightRegister,WeightRegNum);
        break;
      case SumVT:
        modbus.readHoldingRegisters(SlaveAddress,SumVTRegister,SumVTRegNum);
        break;
      case SumV15:
        modbus.readHoldingRegisters(SlaveAddress,SumV15Register,SumV15RegNum);
        break;
      case SumWght:
        modbus.readHoldingRegisters(SlaveAddress,SumWghtRegister,SumWghtRegNum);
        break;
      case BatchQuantity:
        modbus.readHoldingRegisters(SlaveAddress,BatchQuantityRegister,BatchQuantityRegNum);
        break;
      case AvgDensityX:
        modbus.readHoldingRegisters(SlaveAddress,AvgDensityXRegister,AvgDensityXRegNum);
        break;
      case AvgTemp1:
        modbus.readHoldingRegisters(SlaveAddress,AvgTemp1Register,AvgTemp1RegNum);
        break;
      case FlowRate:
        modbus.readHoldingRegisters(SlaveAddress,FlowRateRegister,FlowRateRegNum);
        break;
      case ProductNo:
        modbus.readHoldingRegisters(SlaveAddress,ProductNoRegister,ProductNoRegNum);
        break;
      case AdditivType:
        modbus.readHoldingRegisters(SlaveAddress,AdditivTypeRegister,AdditivTypeRegNum);
        break;
      case AddVT:
        modbus.readHoldingRegisters(SlaveAddress,AddVTRegister,AddVTRegNum);
        break;
      case SumAddVT:
        modbus.readHoldingRegisters(SlaveAddress,SumAddVTRegister,SumAddVTRegNum);
        break;
      case TargetAddRate:
        modbus.readHoldingRegisters(SlaveAddress,TargetAddRateRegister,TargetAddRateRegNum);
        break;
      case DateTime:
        modbus.readHoldingRegisters(SlaveAddress,DateTimeRegister,DateTimeRegNum);
        break;                                                                                                                
    }
    Start_Publish();   
  }
}

void Start_Publish(){
  
  WiFiClient client = server.available();   // Listen for incoming clients
     if (client) {                             // If a new client connects,
      currentTime = millis();
      previousTime = currentTime;
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
        currentTime = millis();
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          Serial.write(c);                    // print it out the serial monitor
          header += c;
          if (c == '\n') {                    // if the byte is a newline character

            if (currentLine.length() == 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
                        
             // Display the HTML web page
              client.println("<!DOCTYPE html><html>");
              client.println("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Document</title><link rel=\"stylesheet\" href=\"style.css\"></head>");
                        
              client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");           
              client.println(".container{display: flex; justify-content: center; align-items: center; min-height: 100vh;}");
              client.println(".wrapper{ border: 1px solid rgb(31,22,22); border-radius: 8px;  padding: 10px; display: flex; flex-wrap:wrap; justify-content:center; box-shadow:10px 10px 5px rgba(144, 150, 147, 0.329);}");
              client.println(".valueContainer{display: flex; border: 1px solid black; border-radius: 5px; margin:15px; width:160px; flex-direction:column;}");
              client.println(".key{color:rgb; background-color:rgba(237,197,148,0.5); border-bottom: 1px solid black; padding: 10px; font-size:18px;}");
 
            
              client.println(".value {padding: 10px; font-size:16px}");
              client.println(".even{color:rgb(14,114,67)}</style></head>");
           
            // Web Page Heading
              client.println("<body><h1>I-GRAPHX</h1>");
          
           // String value = "orhan";
              client.println("<div class=\"container\">");
              client.println("<div class=\"wrapper\">");

            //Indicator for State
              client.println("<div class=\"valueContainer\">");
              client.println("<div class=\"key\">State</div>");
              client.println("<div class=\"value\">"+StateValue+"</div>");
              client.println("</div>");  //valueContainer

            //Indicator for VT
              client.println("<div class=\"valueContainer\">");
              client.println("<div class=\"key\">VT</div>");
              client.println("<div class=\"value\">"+VTValue+"</div>");
              client.println("</div>");  //valueContainer

            //Indicator for V15
              client.println("<div class=\"valueContainer\">");
              client.println("<div class=\"key\">V15</div>");
              client.println("<div class=\"value\">"+V15Value+"</div>");
              client.println("</div>");  //valueContainer

             //Indicator for Weight
              client.println("<div class=\"valueContainer\">");
              client.println("<div class=\"key\">Weight</div>");
              client.println("<div class=\"value\">"+WeightValue+"</div>");
              client.println("</div>");  //valueContainer

            //Indicator for SumVT
              client.println("<div class=\"valueContainer\">");
              client.println("<div class=\"key\">SumVT</div>");
              client.println("<div class=\"value\">"+SumVtValue+"</div>");
              client.println("</div>");  //valueContainer

            //Indicator for SumV15
              client.println("<div class=\"valueContainer\">");
              client.println("<div class=\"key\">SumV15</div>");
              client.println("<div class=\"value\">"+SumV15Value+"</div>");
              client.println("</div>");  //valueContainer
            
            //Indicator for SumWght
              client.println("<div class=\"valueContainer\">");
              client.println("<div class=\"key\">SumWght</div>");
              client.println("<div class=\"value\">"+SumWghtValue+"</div>");
              client.println("</div>");  //valueContainer

            //Indicator for BatchQuantity
              client.println("<div class=\"valueContainer\">");
              client.println("<div class=\"key\">BatchQuantity</div>");
              client.println("<div class=\"value\">"+BatchQuantityValue+"</div>");
              client.println("</div>");  //valueContainer

            //Indicator for AvgDensityx
              client.println("<div class=\"valueContainer\">");
              client.println("<div class=\"key\">AvgDensityX</div>");
              client.println("<div class=\"value\">"+AvgDensityXValue+"</div>");
              client.println("</div>");  //valueContainer

             //Indicator for AvgTemp1
              client.println("<div class=\"valueContainer\">");
              client.println("<div class=\"key\">AvgTemp1</div>");
              client.println("<div class=\"value\">"+AvgTemp1Value+"</div>");
              client.println("</div>");  //valueContainer

            //Indicator for FlowRate
              client.println("<div class=\"valueContainer\">");
              client.println("<div class=\"key\">FlowRate</div>");
              client.println("<div class=\"value\">"+FlowRateValue+"</div>");
              client.println("</div>");  //valueContainer

            //Indicator for ProductNo
              client.println("<div class=\"valueContainer\">");
              client.println("<div class=\"key\">ProductNo</div>");
              client.println("<div class=\"value\">"+ProductNoValue+"</div>");
              client.println("</div>");  //valueContainer

            //Indicator for AdditivType
              client.println("<div class=\"valueContainer\">");
              client.println("<div class=\"key\">AdditivType</div>");
              client.println("<div class=\"value\">"+AdditivTypeValue+"</div>");
              client.println("</div>");  //valueContainer

            //Indicator for AddVT
              client.println("<div class=\"valueContainer\">");
              client.println("<div class=\"key\">AddVT</div>");
              client.println("<div class=\"value\">"+AddVTValue+"</div>");
              client.println("</div>");  //valueContainer

            //Indicator for SumAddVt
              client.println("<div class=\"valueContainer\">");
              client.println("<div class=\"key\">SumAddVT</div>");
              client.println("<div class=\"value\">"+SumAddVTValue+"</div>");
              client.println("</div>");  //valueContainer

            //Indicator for TargetAddRate
              client.println("<div class=\"valueContainer\">");
              client.println("<div class=\"key\">TargetAddRate</div>");
              client.println("<div class=\"value\">"+TargetAddRateValue+"</div>");
              client.println("</div>");  //valueContainer

             //Indicator for DateTime
              client.println("<div class=\"valueContainer\">");
              client.println("<div class=\"key\">DateTime</div>");
              client.println("<div class=\"value\">"+DateTimeValue+"</div>");
              client.println("</div>");  //valueContainer
            
              client.println("</div>");  //wrapper
              client.println("</div>");  //container

              client.println("</body></html>");
            
            // The HTTP response ends with another blank line
              client.println();
              // Break out of the while loop
              break;
            } else { // if you got a newline, then clear currentLine
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }
        }
      }
      // Clear the header variable
      header = "";
      // Close the connection
      client.stop();
      Serial.println("Client disconnected.");
      Serial.println("");
    } 
}
