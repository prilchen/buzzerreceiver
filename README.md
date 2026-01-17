# ESP32 Quiz-Würfel Zentrale (Lolin 32 Lite)

Dieses Projekt ist das Herzstück eines kabellosen Quiz-Buzzer-Systems. Die Zentrale empfängt Signale von mehreren Sendern (Buzzern) via **ESP-NOW** und zeigt den schnellsten Spieler per LED an.

Das vollständige Tutorial mit Gehäusedaten und Bauanleitung findest du auf [prilchen.de](https://prilchen.de).

## 🚀 Highlights
- **Funkprotokoll:** Nutzt ESP-NOW (kein WLAN-Router erforderlich, extrem schnell).
- **Energiemanagement:** Wechselt nach 5 Minuten Inaktivität in den **Deep Sleep** (Tiefschlaf).
- **Wake-Up:** Aufwachen durch den Reset-Taster (EXT0-Wakeup).
- **Selbsttest:** Jede LED leuchtet beim Start kurz auf, um Funktion und Akku zu prüfen.
- **Debug-Modus:** Serielle Ausgaben können über `#define DEBUG 0/1` für maximale Stromeinsparung deaktiviert werden.

## 🛠 Hardware-Anforderungen
- **Mikrocontroller:** ESP32 Lolin 32 Lite.
- **LEDs:** 6 Stück (angeschlossen an GPIO 12, 13, 14, 25, 26, 27).
- **Taster:** 1x Reset-Taster (angeschlossen an GPIO 15 gegen GND).
- **Stromversorgung:** LiPo-Akku (der Lolin 32 Lite hat eine integrierte Ladeelektronik).

## 💻 Einrichtung mit Visual Studio Code (VSC)
1. Installiere die Erweiterung **PlatformIO** in VSC.
2. Klone dieses Repository oder kopiere die Dateien `main.cpp` und `platformio.ini`.
3. **WICHTIG:** Bevor du den Code hochlädst, musst du die **MAC-Adressen** deiner Buzzer in der `main.cpp` im Bereich `playerMacs` eintragen.
4. Nutze den Upload-Button in PlatformIO, um den ESP32 zu bespielen.

### platformio.ini
Die Konfiguration ist für den Lolin 32 Lite optimiert:
```ini
[env:lolin32_lite]
platform = [https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip](https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip)
board = lolin32_lite
framework = arduino
monitor_speed = 115200
