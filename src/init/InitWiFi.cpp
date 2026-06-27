#include "device.h"
#include "DeviceConstants.h"
#include "init/InitWiFi.h"
#include "handlers/LCDHandler.h"

void scanNetworks(Device* device);
void printNetworkInfo(Device* device);
void connectWiFi(Device* device);

void initWiFi(Device* device) {
	device->getSerial()->println("Initializing WiFi...");
	if(device->getDeviceMode() != SETTINGS_MODE) {
        scanNetworks(device);
        connectWiFi(device);
    }
	printLCD("WiFi OK", LCD_FIRST_CHAR, LCD_SECOND_ROW);
    device->getSerial()->println("Init WiFi done. OK.\n");
	device->delayFunc(DELAY_BETWEEN_INIT_STAGES_MS);
}

void scanNetworks(Device* device) {
    const char* wifiAuthModes[] = {"open", "WEP", "WPA", "WPA2", "WPA+WPA2", "WPA2-EAP", "WPA3", "WPA2+WPA3", "WAPI"};
    
	device->getSerial()->println("Scan networks started");
    const int totalNetworks = device->getWIFI()->scanNetworks();
    if (totalNetworks == 0) {
        device->getSerial()->println("ERROR: No networks found");
    } else {
    	device->getSerial()->printf("%i networks found\n", totalNetworks);
    	device->getSerial()->println("Nr | SSID                             | RSSI | CH | Encryption");
    	for (int i = 0; i < totalNetworks; ++i) {
        	device->getSerial()->printf("%2d | %-32.32s | %4d | %2d | ", 
            	i + 1, device->getWIFI()->SSID(i).c_str(), device->getWIFI()->RSSI(i), device->getWIFI()->channel(i));
       		const int authMode = device->getWIFI()->encryptionType(i);
			if (authMode >= 0 && authMode <= 8) {
				device->getSerial()->printf("%s\n", wifiAuthModes[authMode]);
			} else {
				device->getSerial()->println("unknown");
			}
			device->delayFunc(SCAN_WIFI_DELAY_MS);
     	}
   	}
	device->getSerial()->println("Scan networks done. OK.\n");
}

void connectWiFi(Device* device) {
	const char* wifiStatuses[] = {"WL_IDLE_STATUS", "WL_SCAN_COMPLETED", "WL_NO_SSID_AVAIL", "WL_CONNECT_FAILED", "WL_CONNECTION_LOST", "WL_CONNECTED", "WL_DISCONNECTED"};

	device->getSerial()->println("Connecting to network started");
	device->getWIFI()->mode(WIFI_STA);
    device->getWIFI()->begin(device->getWIFISSID(), device->getWIFIPassword());
    int status = device->getWIFI()->status();
    int attempts = 0;
    while (status != WL_CONNECTED) {
		device->getSerial()->println(wifiStatuses[status]);
		if(attempts == MAXIMUM_WIFI_ATTEMPTS) {
			device->getSerial()->println("\nRestart due to WiFi retries");
			device->delayFunc(RESTART_DELAY_MS);
			device->getESP()->restart();
		}
		status = device->getWIFI()->status();
		device->delayFunc(WIFI_RECONNECT_DELAY_MS);
		attempts++;
		printLCD("WiFi tries: " + String(attempts), LCD_FIRST_CHAR, LCD_SECOND_ROW);
    }
    device->getSerial()->println("WiFi connected");
    printNetworkInfo(device);
	device->getSerial()->println("Connecting to network done. OK.\n");
}


void printNetworkInfo(Device* device) {
	device->getSerial()->printf("[*] Network information for %s\n", device->getWIFISSID());
	device->getSerial()->printf("[+] BSSID: %s\n", device->getWIFI()->BSSIDstr().c_str());
	device->getSerial()->printf("[+] Gateway IP: %s\n", device->getWIFI()->gatewayIP().toString().c_str());
	device->getSerial()->printf("[+] Subnet Mask: %s\n", device->getWIFI()->subnetMask().toString().c_str());
	device->getSerial()->printf("[+] RSSI: %i %s\n", device->getWIFI()->RSSI(), " dB");
	device->getSerial()->printf("[+] ESP32 IP: %s\n", device->getWIFI()->localIP().toString().c_str());
}