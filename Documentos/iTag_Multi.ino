#include <BLEDevice.h>

//Id dos clientes. É incrementado toda vez que se conecta um novo cliente
static uint16_t appId = 3;
//UUID do serviço que queremos do iTag
static const BLEUUID serviceUUID("0000ffe0-0000-1000-8000-00805f9b34fb");
//UUID da característica do serviço (botão do iTag)
static const BLEUUID characteristicUUID("0000ffe1-0000-1000-8000-00805f9b34fb");


//Classe responsável por controlar cada iTag
class ITag: public BLENotifier 
{
    public:
        int pinNumber; //número do pino vinculado a este iTag
        int pinStatus = HIGH; //status autal do pino (HIGH ou LOW)
        BLEClient* client; //cliente da conexão com o iTag
        std::string address; //endereço do iTag ao qual irá se conectar

        ITag(std::string addrs, int pNum)
        {
            //Vinculamos os valores do endereço, número do pino, colocamos como saída
            address = addrs;
            pinNumber = pNum;
            pinMode(pinNumber, OUTPUT);
            digitalWrite(pinNumber, pinStatus);
        }

        void connect()
        {
            //Verificamos se já havia um cliente antes e desconectamos caso afirmativo
            if(client != NULL)
            {
                client->disconnect();
                delete client;
            }

            //Criamos o cliente com uma nova id e conectamos ao iTag
            client = BLEDevice::createClient(appId++);
            BLEAddress bleAddress(address);
            boolean connected = client->connect(bleAddress);
            
            //Se a conexão foi bem sucedida
            if(connected)
            {
                //Obtemos o serviço e característica do botão do iTag e vinculamos a função onData
                //para responder o pressionar de botão 
                BLERemoteService* remoteService = client->getService(serviceUUID);   
                BLERemoteCharacteristic* remoteCharacteristic = remoteService->getCharacteristic(characteristicUUID);
                remoteCharacteristic->registerForNotify(this);
            }   
        }

        //Função chamada toda vez que o botão do iTag é pressionado
        void onData(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify)
        {
            //Invertemos o estado atual do pino (de HIGH para LOW ou de LOW para HIGH)
            //e mandamos o novo estado para saída do pino
            pinStatus = !pinStatus;
            digitalWrite(pinNumber, pinStatus);
        }
};


//Intervalo entre cada scan 
#define SCAN_INTERVAL 3000
//Quantidade de iTag que temos (altere de acordo com a quantidade de iTags que tiver)
#define ITAG_COUNT 3

//Definição dos iTags que iremos utilizar
//Para cada iTag vinculamos o endereço dele (obtido através de um scan)
//e o pino cujo o estado será alterado quando o botão do iTag for pressionado
ITag iTags[ITAG_COUNT] = {
    ITag("fc:58:fa:44:06:01", 25),
    ITag("fc:58:fa:43:f0:3e", 26), 
    ITag("fc:58:fa:44:0c:2e", 27)
};
//primeiro preto
//segundo azul
//terceiro branco

//Variável que irá guardar o scan
BLEScan* pBLEScan;

//Quando ocorreu o último scan
uint32_t lastScanTime = 0; 

void setup()
{
    Serial.begin(115200);

    //Iniciamos o BLE
    BLEDevice::init("");
    
    //Guardamos o objeto responsável pelo scan
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setActiveScan(true);
}

void loop()
{
    //Tempo em milissegundos desde o boot
    uint32_t now = millis();

    //Se está no tempo de fazer scan
    if(now - lastScanTime > SCAN_INTERVAL)
    { 
        //Marca quando ocorreu o último scan e começa o scan
        lastScanTime = now;
        scan();
    }
}

void scan()
{
    //Realiza o scan por 2 segundos
    BLEScanResults results = pBLEScan->start(2);
    pBLEScan->stop();

    //Para cada dispositivo encontrado pelo scan
    for(int i=0; i<results.getCount(); i++)
    {
        //Guardamos a referência para o dispositivo e mostramos no monitor serial
        BLEAdvertisedDevice advertisedDevice = results.getDevice(i);
        Serial.println("advertisedDevice: " + String(advertisedDevice.toString().c_str()));

        //Para cada iTag que temos
        for(int j=0; j<ITAG_COUNT; j++)
        {
            //Se o dispositivo scaneado for um dos nossos iTags
            if(advertisedDevice.getAddress().toString() == iTags[j].address)
            {
                //Mandamos conectar
                iTags[j].connect();
            }
        }
    }
}
