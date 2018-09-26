void initializeSD() {
  Sd2Card card;
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example

  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  // First, detect the card
  boolean  status = card.init(SPI_HALF_SPEED, BUILTIN_SDCARD); // Audio shield has SD card SD on pin 10
  if (status) {
    Serial.println("SD card is connected :-)");
  } else {
    Serial.println("SD card is not connected or unusable :-(");
    return;
  }

  int type = card.type();
  if (type == SD_CARD_TYPE_SD1 || type == SD_CARD_TYPE_SD2) {
    Serial.println("Card type is SD");
  } else if (type == SD_CARD_TYPE_SDHC) {
    Serial.println("Card type is SDHC");
  } else {
    Serial.println("Card is an unknown type (maybe SDXC?)");
  }
  SdVolume volume;
  // Then look at the file system and print its capacity
  status = volume.init(card);
  if (!status) {
    Serial.println("Unable to access the filesystem on this card. :-(");
    return;
  }

  float  size = volume.blocksPerCluster() * volume.clusterCount();
  size = size * (512.0 / 1e6); // convert blocks to millions of bytes
  Serial.print("File system space is ");
  Serial.print(size);
  Serial.println(" Mbytes.");

  // Now open the SD card normally
  status = SD.begin(BUILTIN_SDCARD); // Audio shield has SD card CS on pin 10
  if (status) {
    Serial.println("SD library is able to access the filesystem");
  } else {
    Serial.println("SD library can not access the filesystem!");
    Serial.println("Please report this problem, with the make & model of your SD card.");
    Serial.println("  http://forum.pjrc.com/forums/4-Suggestions-amp-Bug-Reports");
  }
}
