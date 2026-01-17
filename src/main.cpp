#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// --- DEBUG MODUS ---
// 1 = Serielle Ausgabe an (Entwicklung)
// 0 = Serielle Ausgabe aus (Normalbetrieb / Stromsparen)
#define DEBUG 0

// Makro für die serielle Ausgabe
#if DEBUG == 1
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
  #define DEBUG_PRINTF(...) Serial.printf(__VA_ARGS__)
  #define DEBUG_BEGIN(x) Serial.begin(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(...)
  #define DEBUG_BEGIN(x)
#endif

// --- Konfiguration ---
const int ledPins[] = {12, 13, 14, 25, 26, 27}; 
const int numLeds = 6;
const int buttonPin = 15; 

const unsigned long SLEEP_TIMEOUT = 300000; 
unsigned long lastActivity = 0;

enum BuzzerState { STATE_WAITING, STATE_WINNER_LOCKED };
BuzzerState currentState = STATE_WAITING;

typedef struct struct_message {
    int playerID; 
} struct_message;

struct_message incomingData;

void allLedsOff() {
    for (int i = 0; i < numLeds; i++) digitalWrite(ledPins[i], LOW);
}

void enterDeepSleep() {
    DEBUG_PRINTLN("Inaktivität erkannt. Gehe in Deep Sleep...");
    allLedsOff();
    esp_sleep_enable_ext0_wakeup((gpio_num_t)buttonPin, 0); 
    esp_deep_sleep_start();
}

// Hier trägst du später die MAC-Adressen deiner eigenen Buzzer ein.
// Wie du diese Adressen ausliest, siehe prilchen.de
const uint8_t playerMacs[][6] = {
    {0xAA, 0xBB, 0xCC, 0x01, 0x01, 0x01}, // Beispiel Spieler 1
    {0xAA, 0xBB, 0xCC, 0x02, 0x02, 0x02}, // Beispiel Spieler 2
    {0xAA, 0xBB, 0xCC, 0x03, 0x03, 0x03}, // Beispiel Spieler 3
    {0xAA, 0xBB, 0xCC, 0x04, 0x04, 0x04}, // Beispiel Spieler 4
    {0xAA, 0xBB, 0xCC, 0x05, 0x05, 0x05}, // Beispiel Spieler 5
    {0xAA, 0xBB, 0xCC, 0x06, 0x06, 0x06}  // Beispiel Spieler 6
};
const int numPlayers = sizeof(playerMacs) / 6;

int getPlayerIndex(const uint8_t * mac_addr) {
    for (int i = 0; i < numPlayers && i < numLeds; i++) {
        if (memcmp(mac_addr, playerMacs[i], 6) == 0) return i;
    }
    return -1;
}

void OnDataRecv(const esp_now_recv_info_t *esp_now_info, const uint8_t *incomingBytes, int len) {
    lastActivity = millis(); 
    
    if (currentState == STATE_WAITING) {
        const uint8_t *mac = esp_now_info->src_addr;
        int winnerIdx = getPlayerIndex(mac);
        
        if (winnerIdx >= 0 && winnerIdx < numLeds) {
            allLedsOff();
            digitalWrite(ledPins[winnerIdx], HIGH);
            currentState = STATE_WINNER_LOCKED;
            DEBUG_PRINT("Gewinner Spieler: "); DEBUG_PRINTLN(winnerIdx + 1);
        }
    }
}

void setup() {
    DEBUG_BEGIN(115200);
    
    // LEDs & Button initialisieren
    for (int i = 0; i < numLeds; i++) {
        pinMode(ledPins[i], OUTPUT);
    }
    pinMode(buttonPin, INPUT_PULLUP);
    allLedsOff();

    // --- LED-Selbsttest beim Start / Aufwachen ---
    // Jede LED leuchtet kurz nacheinander auf (Lauflicht)
    DEBUG_PRINTLN("Starte LED-Selbsttest...");
    for (int i = 0; i < numLeds; i++) {
        digitalWrite(ledPins[i], HIGH);
        delay(500);
        digitalWrite(ledPins[i], LOW);
    }
    DEBUG_PRINTLN("Test beendet.");

    // WiFi & ESP-NOW Initialisierung
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        DEBUG_PRINTLN("Fehler bei ESP-NOW Init");
        return;
    }

    esp_now_register_recv_cb(OnDataRecv);
    DEBUG_PRINTLN("Zentrale bereit. Warte auf Buzzer...");
    
    lastActivity = millis(); 
}

void loop() {
    if (digitalRead(buttonPin) == LOW) {
        lastActivity = millis(); 
        
        if (currentState == STATE_WINNER_LOCKED) {
            delay(50);
            allLedsOff();
            currentState = STATE_WAITING;
            DEBUG_PRINTLN("System Reset - Nächste Runde!");
            while(digitalRead(buttonPin) == LOW); 
        }
    }

    if (millis() - lastActivity > SLEEP_TIMEOUT) {
        enterDeepSleep();
    }
}