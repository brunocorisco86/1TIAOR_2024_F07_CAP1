#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include <DHT.h>

// Configurações de Wi-Fi
const char* ssid = "quarto";
const char* password = "veracruz";

// Configurações do MQTT (Mosquitto no Raspberry Pi)
const char* mqtt_server = "192.168.1.7";  // IP do Raspberry Pi
const int mqtt_port = 1883;
const char* mqtt_user = "mosquitto";
const char* mqtt_password = "blurbang";
const char* mqtt_topic = "sensor/data";

// Configurações do sensor DHT
#define DHTPIN D4         // Pino onde o DHT22 está conectado
#define DHTTYPE DHT22     // Tipo do sensor (DHT22)
DHT dht(DHTPIN, DHTTYPE);

// Cliente Wi-Fi e MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Variáveis para controle de tempo
unsigned long lastPublishTime = 0;
unsigned long lastSensorReadTime = 0;
const unsigned long publishInterval = 300000; // 5 minutos em milissegundos
const unsigned long sensorReadInterval = 15000; // 15 segundos em milissegundos
float lastTemperature = 0.0; // Armazena a última temperatura válida
float lastHumidity = 0.0;   // Armazena a última umidade válida
bool firstRun = true;       // Flag para indicar a primeira execução

// Função para sincronizar o relógio com NTP do Brasil
void syncTime() {
    configTime(-4 * 60 * 60, 0, "200.160.0.8", "200.189.40.8", "200.192.232.8"); // Sem ajuste de fuso horário (UTC), múltiplos servidores NTP.br
    Serial.println("[INFO] Sincronizando relógio com NTP.br...");
    struct tm timeinfo;
    int attempts = 0;
    while (!getLocalTime(&timeinfo) && attempts < 10) {
        delay(1000);
        attempts++;
        Serial.print(".");
    }
    if (attempts < 10) {
        char buffer[30];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
        Serial.println("\n[OK] Relógio sincronizado! Hora atual: " + String(buffer));
    } else {
        Serial.println("\n[ERRO] Falha na sincronização do relógio após 10 tentativas!");
    }
}

// Função para obter o timestamp atual
String getTimestamp() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("[ERRO] Falha ao obter timestamp!");
        return "N/A";
    }
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return String(buffer);
}

// Conectar ao Wi-Fi
void connectWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("[INFO] Conectando ao Wi-Fi ");
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n[OK] Conectado ao Wi-Fi! IP: " + WiFi.localIP().toString());
    } else {
        Serial.println("\n[ERRO] Falha na conexão Wi-Fi após 20 tentativas!");
    }
}

// Reconectar ao MQTT e enviar dados na primeira conexão
void reconnectMQTT() {
    while (!client.connected()) {
        Serial.print("[INFO] Conectando ao Mosquitto...");
        String clientId = "ESP8266-" + String(random(0xffff), HEX);
        if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
            Serial.println("\n[OK] Conectado ao Mosquitto!");
            // Após conectar, lê e exibe os dados do sensor imediatamente
            readSensorData();
            // Se for a primeira execução, envia os dados ao Mosquitto
            if (firstRun && lastTemperature != 0.0 && lastHumidity != 0.0) {
                sendSensorData();
                firstRun = false; // Desativa a flag após o primeiro envio
            }
        } else {
            Serial.print("\n[ERRO] Falha na conexão ao Mosquitto, rc=");
            Serial.print(client.state());
            Serial.println(" Tentando novamente em 5s...");
            delay(5000);
        }
    }
}

// Ler dados do sensor DHT
void readSensorData() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("[ERRO] Falha ao ler o sensor DHT22!");
    } else {
        lastTemperature = temperature; // Armazena a última leitura válida
        lastHumidity = humidity;
        Serial.print("[SENSOR] Temperatura: ");
        Serial.print(temperature, 1);
        Serial.print("°C | Umidade: ");
        Serial.print(humidity, 1);
        Serial.println("%");
    }
}

// Enviar dados via MQTT
void sendSensorData() {
    String timestamp = getTimestamp();
    String payload = "{\"temperature\":";
    payload += String(lastTemperature, 1);
    payload += ",\"humidity\":";
    payload += String(lastHumidity, 1);
    payload += ",\"timestamp\":\"";
    payload += timestamp;
    payload += "\"}";

    Serial.print("[MQTT] Tentando enviar: ");
    Serial.println(payload);
    
    if (client.publish(mqtt_topic, payload.c_str())) {
        Serial.println("[OK] Dados enviados ao Mosquitto com sucesso!");
    } else {
        Serial.println("[ERRO] Falha ao enviar dados ao Mosquitto! Verifique a conexão.");
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("\n[INFO] Iniciando ESP8266...");
    dht.begin();
    delay(2000); // Aguarda o sensor estabilizar
    connectWiFi();
    client.setServer(mqtt_server, mqtt_port);
    syncTime();
}

void loop() {
    if (!client.connected()) {
        reconnectMQTT();
    }
    client.loop();

    unsigned long currentTime = millis();

    // Lê o sensor a cada 15 segundos
    if (currentTime - lastSensorReadTime >= sensorReadInterval) {
        readSensorData();
        lastSensorReadTime = currentTime;
    }

    // Publica no MQTT a cada 5 minutos
    if (currentTime - lastPublishTime >= publishInterval) {
        if (lastTemperature != 0.0 && lastHumidity != 0.0) { // Garante que há dados válidos
            sendSensorData();
        } else {
            Serial.println("[AVISO] Nenhum dado válido para enviar. Aguardando leitura do sensor...");
        }
        lastPublishTime = currentTime;
    }
}