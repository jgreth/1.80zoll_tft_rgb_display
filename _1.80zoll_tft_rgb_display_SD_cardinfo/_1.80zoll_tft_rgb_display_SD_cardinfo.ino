/***
 * Test für den SD Slot des 1,80 Zoll SPI TFT-Display
 * v1.0 - 2018-01-07
 * Jan Greth <github@greth.me>
 * 
 * Basierend auf dem SD-Card-Info Arduino Beispiel-Script
 * created  28 Mar 2011 by Limor Fried 
 * modified 9 Apr 2012 by Tom Igoe
 ***/

/*
 SD card test - Pinout:
 SD-Card - Arduino Nano  
 
 SD_CS   - D10
 SD_MOSI - D11
 SD_MISO - D12
 SD_CLK  - D13

 VCC - 5V
 GND - GND
 
 */
// SD library laden:
#include <SPI.h>
#include <SD.h>

// Variablen initialisieren
Sd2Card card;
SdVolume volume;
SdFile root;

// Auf SD_CS setzen
const int chipSelect = 10;

void setup() {
  // Serielle Schnittstelle ans laufen bringen 
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  Serial.print("\nSD card initialisieren...");

  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("Initialisierung fehlgeschlagen. Zu prüfen:");
    Serial.println("* ist eine Karte eingelegt?");
    Serial.println("* Verkabelung korrekt??");
    Serial.println("* den Wert von chipSelect so angepasst das er zur Hardware passt?");
    return;
  } else {
    Serial.println("Verkabelung korrekt und Karte eingelegt.");
  }

  // Kartentyp? 
  Serial.print("\nKartentyp: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unbekannt");
  }

  // Versuche 'volume'/'partition' zu öffnen - sollte FAT16 oder FAT32 sein
  if (!volume.init(card)) {
    Serial.println("Konnte keine FAT16/FAT32 partition finden.\nIst die Karte (richtig) formatiert?");
    return;
  }

  // Typ und Größe der ersten FAT Partition ausgeben
  uint32_t volumesize;
  Serial.print("\nVolume typ ist FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  Serial.print("Volume groesse (bytes): ");
  Serial.println(volumesize);
  Serial.print("Volume groesse (Kbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume groesse (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);


  Serial.println("\nDateien wurden auf der Karte gefunden (Dateiname, Datum and Groesse in bytes): ");
  root.openRoot(volume);

  // Alle Dateien der Karte auflisten
  root.ls(LS_R | LS_DATE | LS_SIZE);
}


void loop(void) {

}
