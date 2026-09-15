#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
const int rele = 16; 
const int led = 2;



void setup() {
  Serial.begin(9600);
  SerialBT.begin("ExRugbier");// Nombre del dispositivo Bluetooth
  pinMode(rele, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(rele, LOW);
  digitalWrite(led, LOW);

}

void loop() {
   while(SerialBT.available()){
    char comando = SerialBT.read();
    Serial.print(comando);
   
    //Control del relé
    if(comando=='1') {
       digitalWrite(rele, HIGH);
       digitalWrite(led, HIGH);
    }
    else if (comando=='0')
      {  digitalWrite(rele, LOW);
         digitalWrite(led, LOW);
      } 
        }
  delay(20);
}
