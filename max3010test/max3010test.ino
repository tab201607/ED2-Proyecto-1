#include "config.h"

#include <Wire.h>
#include "MAX30105.h"
#include "driver/uart.h"
#include <HardwareSerial.h>
 
#include "heartRate.h"
 
MAX30105 particleSensor;

char alcohol_UART[10], bpm_UART[10];

//incializa adafruit para bpm
AdafruitIO_Feed *bpm_send = io.feed("bpm");
AdafruitIO_Feed *alc_send = io.feed("alc");
 
const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
 
float beatsPerMinute;
int beatAvg;
unsigned int alcohol, alcohol_temp;

unsigned long sendthething = 0;; // este sirve para regular que tan frecuentemente se envian datos a adafruit

HardwareSerial SerialPort(2); // use UART2

void setup()
{
  
Serial.begin(115200);

//control de uart en 2
SerialPort.begin(9600, SERIAL_8N1, 16, 17); 

// incializa sensor
if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
{
Serial.println("MAX30105 was not found. Please check wiring/power. ");
while (1);
}
Serial.println("Place your index finger on the sensor with steady pressure.");
 
particleSensor.setup(); //Configure sensor with default settings
particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED

//incializa adafruit

Serial.println("Initializing...");

Serial.print("Connecting to Adafruit IO");

//conectar a adafruit

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  
 


delay(10000);
}

//-----------------------------------------------
//INICIO DE LOOP
//-----------------------------------------------
 
void loop() {
  
long irValue = particleSensor.getIR();
 
if (checkForBeat(irValue) == true)
{
//We sensed a beat!
long delta = millis() - lastBeat;
lastBeat = millis();
 
beatsPerMinute = 60 / (delta / 1000.0);
 
if (beatsPerMinute < 255 && beatsPerMinute > 20)
{
rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
rateSpot %= RATE_SIZE; //Wrap variable
 
//Take average of readings
beatAvg = 0;
for (byte x = 0 ; x < RATE_SIZE ; x++)
beatAvg += rates[x];
beatAvg /= RATE_SIZE;
}
}
 
Serial.print("IR=");
Serial.print(irValue);
Serial.print(", BPM=");
Serial.print(beatsPerMinute);
Serial.print(", Avg BPM=");
Serial.print(beatAvg);
Serial.print(" ");
Serial.print(sendthething);
Serial.print("Alc: ");
Serial.print(alcohol);
 
if (irValue < 50000)
Serial.print(" No finger?");
 
Serial.println();

sprintf(bpm_UART, "%u", beatAvg);


//Envia señal a uart

alcohol_temp = int(SerialPort.read());
 if (alcohol_temp != 4294967295)  alcohol = alcohol_temp;
SerialPort.write(beatAvg);



if (sendthething == 300) {
  Serial.print("sending ");
  Serial.print(sendthething);
  Serial.println();
 io.run();
 bpm_send->save(beatAvg);
 delay(10);
 if (alcohol != 4294967295)  alc_send->save(alcohol);

 sendthething = 0;

// SerialPort.print(bpm_UART);
//SerialPort.print("\0");
 
} 

 sendthething++;

 

//delay(1000);
}
