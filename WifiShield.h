#ifndef WIFISHIELD_H
#define WIFISHIELD_H

#include "Config.h"


Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIVIDER); // you can change this clock speed


void initWifiShield()
{
  Serial.begin(115200);

#ifdef DEBUG
  Serial.println(F("Hello, CC3000!\n")); 
  Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);
  Serial.println(F("\nInitializing..."));
#endif

  // Initialise the module
  if (!cc3000.begin())
  {
#ifdef DEBUG
    Serial.println(F("Couldn't begin()! Check your wiring?"));
#endif
    while(1);
  }

#ifdef DEBUG
    Serial.print(F("\nAttempting to connect to ")); Serial.println(WLAN_SSID);
#endif

  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
#ifdef DEBUG    
    Serial.println(F("Failed!"));
#endif
    while(1);
  }

#ifdef DEBUG    
  Serial.println(F("Connected!"));  
  Serial.println(F("Request DHCP"));
#endif

  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  

  // Display the IP address DNS, Gateway, etc.
#ifdef DEBUG    
  while (! displayConnectionDetails()) {
    delay(1000);
  }

  // ******************************************************
  // You can safely remove this to save some flash memory!
  // ******************************************************
  Serial.println(F("\r\nNOTE: This sketch may cause problems with other sketches"));
  Serial.println(F("since the .disconnect() function is never called, so the"));
  Serial.println(F("AP may refuse connection requests from the CC3000 until a"));
  Serial.println(F("timeout period passes.  This is normal behaviour since"));
  Serial.println(F("there isn't an obvious moment to disconnect with a server.\r\n"));
#endif


};



// Tries to read the IP address and other connection details
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

#endif
