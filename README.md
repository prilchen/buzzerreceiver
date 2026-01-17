ESP32 Quiz-Würfel (Zentrale)
Dieses Repository enthält die Software für die Zentrale (Empfänger) eines drahtlosen Quiz-Buzzer-Systems. Das Projekt basiert auf dem ESP32 Lolin Lite und nutzt das ESP-NOW Protokoll für extrem schnelle Reaktionszeiten.

Das passende Tutorial und die Anleitung für die Buzzer (Sender) findest du auf prilchen.de und auf meinem YouTube-Kanal.

🚀 Funktionen
ESP-NOW Funk: Empfängt Signale von bis zu 6 Buzzern ohne WLAN-Router.

Deep Sleep: Automatisches Einschlafen nach 5 Minuten Inaktivität zur Akkuschonung.

EXT0-Wakeup: Aufwachen aus dem Tiefschlaf per Reset-Taster.

Power-On Self-Test: Beim Starten leuchten alle LEDs nacheinander auf (Funktionskontrolle).

Debug-Modus: Per Software-Schalter (#define DEBUG) können serielle Ausgaben für die Fehlersuche aktiviert oder deaktiviert werden.

🛠 Hardware
Board: ESP32 Lolin Lite (mit LiPo-Laderegler).

LEDs: 6 Standard-LEDs (an GPIOs 12, 13, 14, 25, 26, 27).

Taster: 1 Reset-Taster an GPIO 15 (gegen GND geschaltet).

Gehäuse: 3D-Druck Gehäuse (Vorlagen auf Maker World verlinkt).

💻 Installation (Visual Studio Code & PlatformIO)
Klone dieses Repository oder lade die Dateien herunter.

Öffne den Ordner in Visual Studio Code (VSC).

Stelle sicher, dass die Erweiterung PlatformIO installiert ist.

Passe in der main.cpp die MAC-Adressen deiner Buzzer im Array playerMacs an.

Klicke auf den Upload-Pfeil in der blauen Statusleiste unten, um das Programm auf den ESP32 zu flashen.

platformio.ini
Die Konfiguration nutzt eine stabile Plattform-Version für den Lolin Lite:

Ini, TOML

[env:lolin32_lite]
platform = https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip
board = lolin32_lite
framework = arduino
monitor_speed = 115200
📝 Lizenz & Kontakt
Erstellt von Dirk Thiel (prilchen).

Webseite: prilchen.de

GitHub: github.com/prilchen

YouTube: @prilchen

Viel Spaß beim Nachbauen und Buzzern!
