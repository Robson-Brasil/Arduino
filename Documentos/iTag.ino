#include <BLEDevice.h>

#define ADDRESS "ff:ff:c2:07:ab:16" //Endereço do iTag, conseguido pelo próprio scan
#define RELAY_PIN 2 //Pino do Relê
#define SCAN_INTERVAL 1000 //intervalo entre cada scan
#define TARGET_RSSI -100 //RSSI limite para ligar o relê.
#define MAX_MISSING_TIME 5000 //Tempo para desligar o relê desde o momento que o iTag não for encontrado

BLEScan* pBLEScan; //Variável que irá guardar o scan
uint32_t lastScanTime = 0; //Quando ocorreu o último scan
boolean found = false; //Se encontrou o iTag no último scan
uint32_t lastFoundTime = 0; //Tempo em que encontrou o iTag pela última vez
int rssi = 0;

//Callback das chamadas ao scan
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        //Sempre que um dispositivo for encontrado ele é mostrado aqui
        Serial.print("Device found: ");      
        Serial.println(advertisedDevice.toString().c_str());

        //Se for o dispositivo que esperamos
        if(advertisedDevice.getAddress().toString() == ADDRESS)
        {
            //Marcamos como encontrado, paramos o scan e guardamos o rssi
            found = true;
            advertisedDevice.getScan()->stop();
            rssi = advertisedDevice.getRSSI();
            Serial.println("RSSI: " + rssi);
        }
    }
};

void setup()
{
    Serial.begin(115200);
    //Configura o pino do relê como saída e coloca com low
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);

    //Guardamos a referência e configuramos o objeto scan
    BLEDevice::init(""); 
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
}

void loop()
{   
    uint32_t now = millis(); //Tempo em milissegundos desde o boot

    if(found){ //Se econtrou o iTag no último scan
        lastFoundTime = millis(); //Tempo em milissegundos de quando encontrou
        found = false;
        
        if(rssi > TARGET_RSSI){ //Se está dentro do limite, ligamos o relê
            digitalWrite(RELAY_PIN, HIGH);
        }
        else{ //senão desligamos
            digitalWrite(RELAY_PIN, LOW);
        }
    }
    //Se não encontrou e o tempo desde a última vez que econtrou for maior que o tempo máximo de desaparecido
    else if(now - lastFoundTime > MAX_MISSING_TIME){
        digitalWrite(RELAY_PIN, LOW);  //Desligamos o relê
    }
    
    if(now - lastScanTime > SCAN_INTERVAL){ //Se está no tempo de fazer scan
        //Marca quando ocorreu o último scan e começa o scan
        lastScanTime = now;
        pBLEScan->start(1);
    }
}
