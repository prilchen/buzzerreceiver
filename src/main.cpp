#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// --- Konfiguration ---
// WICHTIG: GPIOs 7-12 sind beim ESP32 oft für Flash belegt. 
// Nutze beim Lolin Lite lieber diese (Beispiel):
const int ledPins[] = {12, 13, 14, 25, 26, 27}; 
const int numLeds = 6;
const int buttonPin = 15; // Reset-Taster

enum BuzzerState { STATE_WAITING, STATE_WINNER_LOCKED };
BuzzerState currentState = STATE_WAITING;

// Struktur für die Daten (muss beim Sender identisch sein)
typedef struct struct_message {
    int playerID; // Alternativ nutzen wir die MAC-Adresse
} struct_message;

struct_message incomingData;

// --- Hilfsfunktionen ---
void allLedsOff() {
    for (int i = 0; i < numLeds; i++) digitalWrite(ledPins[i], LOW);
}

// Zuordnung: MAC-Adresse zu Spieler-Index (0-5)
// Hier trägst du die MACs deiner C3-Buzzer ein
// Beispiel: Erste MAC-Adresse (D8:3B:DA:E4:7C:04) ist Spieler 1
const uint8_t playerMacs[][6] = {
    {0xFC, 0xE8, 0xC0, 0xE1, 0xF5, 0x0C}, // Spieler 1
    {0xFC, 0xE8, 0xC0, 0xDE, 0xB1, 0xE4}, // Spieler 2
    {0xF8, 0xB3, 0xB7, 0x74, 0xDE, 0xD4}, // Spieler 3
    {0x38, 0x18, 0x2B, 0xA7, 0x7F, 0x58}, // Spieler 4
    {0x38, 0x18, 0x2B, 0xA7, 0x7F, 0x20}, // Spieler 5
    {0xFC, 0xE8, 0xC0, 0xDF, 0x97, 0xA8}  // Spieler 6
    // Weitere MACs für Spieler 5..6 hier eintragen
};
const int numPlayers = sizeof(playerMacs) / 6;

int getPlayerIndex(const uint8_t * mac_addr) {
    // MAC-Abgleich gegen die bekannte Liste
    for (int i = 0; i < numPlayers && i < numLeds; i++) {
        if (memcmp(mac_addr, playerMacs[i], 6) == 0) {
            return i; // Index 0..n-1 => Spieler i+1
        }
    }

    // Fallback: falls Sender eine playerID im Paket sendet, nutzen
    int idx = incomingData.playerID - 1;
    if (idx >= 0 && idx < numLeds) return idx;

    // Unbekannte MAC-Adresse (debug output)
    Serial.print("Unbekannte MAC: ");
    for (int j = 0; j < 6; j++) {
        if (mac_addr[j] < 0x10) Serial.print("0");
        Serial.print(mac_addr[j], HEX);
        if (j < 5) Serial.print(":");
    }
    Serial.println();

    return -1; // nicht gefunden
}

// Callback wenn Daten empfangen werden
void OnDataRecv(const esp_now_recv_info_t *esp_now_info, const uint8_t *incomingBytes, int len) {
    if (currentState == STATE_WAITING) {
        memcpy(&incomingData, incomingBytes, sizeof(incomingData));
        
        // MAC-Adresse aus esp_now_info auslesen
        const uint8_t *mac = esp_now_info->src_addr;
        int winnerIdx = getPlayerIndex(mac);
        
        if (winnerIdx >= 0 && winnerIdx < numLeds) {
            allLedsOff();
            digitalWrite(ledPins[winnerIdx], HIGH);
            currentState = STATE_WINNER_LOCKED;
            Serial.print("Gewinner Spieler: "); Serial.println(winnerIdx + 1);
        }
    }
}

void setup() {
    Serial.begin(115200);
    
    // LEDs & Button
    for (int i = 0; i < numLeds; i++) pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);
    allLedsOff();

    // WiFi & ESP-NOW Initialisierung
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("Fehler bei ESP-NOW Init");
        return;
    }

    esp_now_register_recv_cb(OnDataRecv);
    Serial.println("Zentrale bereit. Warte auf Buzzer...");
}

void loop() {
    // Reset-Logik
    if (digitalRead(buttonPin) == LOW && currentState == STATE_WINNER_LOCKED) {
        delay(50); // Entprellen
        allLedsOff();
        currentState = STATE_WAITING;
        Serial.println("System Reset - Nächste Runde!");
        while(digitalRead(buttonPin) == LOW); // Warten bis losgelassen
    }
}