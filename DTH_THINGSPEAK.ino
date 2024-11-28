//Trabajo Final
// Grupo Los Inadaptados
// Integrantes: 
// Mao Ying Gómez Uribe
// Yang li Gómez Uribe
// Juan David Tezna Londoño 
// Guiller Sebastián Lasso Rendón
// Fabian Andrés Rosero Aldana
// Yeison Andrés villegas Pabón

#include <ThingSpeak.h>
#include <WiFi.h>
#include "DHT.h"

// Configuración del DHT22
#define DHT_PIN 17               // Nuevo pin GPIO para el sensor DHT22
#define DHT_TYPE DHT22          // Tipo de sensor DHT

// Credenciales Wi-Fi
char ssid[] = "Celupro1";       // Nombre de la red Wi-Fi //Celupro TODO PARA TU CELULAR!
char pass[] = "2024celupro";    // Contraseña

// Configuración de ThingSpeak
unsigned long channelID = 2766217;          // ID del canal
const char* WriteAPIKey = "W4LNFD0MRCJBMXX7"; // API Key de escritura

// Inicializar el sensor DHT y cliente Wi-Fi
DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient cliente;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Inicializando DHT22 y conexión Wi-Fi..."));

  // Iniciar conexión Wi-Fi
  conectarWiFi();

  // Inicializar ThingSpeak
  ThingSpeak.begin(cliente);

  // Iniciar DHT22
  dht.begin();
  Serial.println("Setup completo.");
}

void loop() {
  // Leer y enviar datos del DHT22
  enviarDatosDHT22();

  // Espera para cumplir con el límite de ThingSpeak
  Serial.println("Esperando 15 segundos...");
  delay(15000);
}

// Conexión Wi-Fi con reintentos
void conectarWiFi() {
  Serial.print("Conectando a Wi-Fi");
  WiFi.begin(ssid, pass);

  int intentos = 0;
  while (WiFi.status() != WL_CONNECTED && intentos < 20) {
    delay(500);
    Serial.print(".");
    intentos++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n¡Wi-Fi conectado!");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nNo se pudo conectar a Wi-Fi. Reiniciando...");
    ESP.restart();
  }
}

// Leer y enviar datos del DHT22 a ThingSpeak
void enviarDatosDHT22() {
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();

  if (!isnan(temperatura) && !isnan(humedad)) {
    Serial.println("Enviando datos del DHT22...");
    Serial.printf("Temperatura: %.2f ºC\n", temperatura);
    Serial.printf("Humedad: %.2f %%\n", humedad);

    // Configurar campos de ThingSpeak
    ThingSpeak.setField(1, temperatura);
    ThingSpeak.setField(2, humedad);

    // Enviar datos
    if (ThingSpeak.writeFields(channelID, WriteAPIKey)) {
      Serial.println("Datos enviados correctamente a ThingSpeak.");
    } else {
      Serial.println("Error al enviar datos a ThingSpeak.");
    }
  } else {
    Serial.println("Error en la lectura del DHT22.");
  }
}

