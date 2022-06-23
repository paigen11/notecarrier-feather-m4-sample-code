
#include <Notecard.h>

#include <NotecardPseudoSensor.h>
using namespace blues;

#define usbSerial Serial
// #define txRxPinsSerial Serial1 // only needed when using breadboard to connect to MCU

Notecard notecard;
NotecardPseudoSensor sensor(notecard);

#define NOTE_PRODUCT_UID "com.blues.pniedringhaus:m4_notecarrier_sample"

void setup()
{
  usbSerial.begin(115200);
  while (!usbSerial)
  {
    ;
  }
  usbSerial.println("Starting ...");

  // sample code for communicating with Notehub over serial
  // txRxPinsSerial.begin(9600);
  // notecard.begin(txRxPinsSerial);
  // txRxPinsSerial.print("\n");

  // txRxPinsSerial.println("{\"req\":\"hub.set\",\"product\":\"" NOTE_PRODUCT_UID "\",\"mode\":\"continuous\"}");

  // delay(250);
  // while (txRxPinsSerial.available() > 0)
  // {
  //   char incomingByte = txRxPinsSerial.read();
  //   if (incomingByte != '\r' && incomingByte != '\n')
  //   {
  //     usbSerial.print(incomingByte);
  //   }
  // }
  // usbSerial.println();

  // notecard.begin(txRxPinsSerial, 9600);  // only needed when using breadboard to connect to MCU
  notecard.begin();
  notecard.setDebugOutputStream(usbSerial);

  J *req = notecard.newRequest("hub.set");
  if (req != NULL)
  {
    JAddStringToObject(req, "product", NOTE_PRODUCT_UID);
    JAddStringToObject(req, "mode", "continuous");
    notecard.sendRequest(req);
  }
}

void loop()
{
  float temperature = sensor.temp();
  float humidity = sensor.humidity();

  usbSerial.print("Temperature = ");
  usbSerial.print(temperature);
  usbSerial.println(" *C");
  usbSerial.print("Humidity = ");
  usbSerial.print(humidity);
  usbSerial.println(" %");

  delay(15000);

  // sample code for communicating with Notehub over serial
  // char json_request[150];
  // snprintf(json_request, sizeof(json_request),
  //          "{"
  //          "\"req\":\"note.add\","
  //          "\"file\":\"sensors.qo\","
  //          "\"sync\":true,"
  //          "\"body\":{"
  //          "\"temp\":%d.%02d,"
  //          "\"humidity\":%d.%02d"
  //          "}"
  //          "}",
  //          (int)temperature, abs(((int)(temperature * 100.0) % 100)),
  //          (int)humidity, (int)(humidity * 100.0) % 100);

  // txRxPinsSerial.println(json_request);
  // delay(250); // wait for the Notecard to respond
  // while (txRxPinsSerial.available() > 0)
  // {
  //   char incomingByte = txRxPinsSerial.read();
  //   if (incomingByte != '\r' && incomingByte != '\n')
  //   {
  //     usbSerial.print(incomingByte);
  //   }
  // }
  // usbSerial.println();

  J *req = notecard.newRequest("note.add");
  if (req != NULL)
  {
    JAddStringToObject(req, "file", "sensors.qo");
    JAddBoolToObject(req, "sync", true);
    J *body = JAddObjectToObject(req, "body");
    if (body)
    {
      JAddNumberToObject(body, "temp", temperature);
      JAddNumberToObject(body, "humidity", humidity);
    }
    notecard.sendRequest(req);
  }
}
