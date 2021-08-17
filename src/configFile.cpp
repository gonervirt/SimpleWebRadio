#include "configFile.h"
/*
#include <ArduinoJson.h>
#include <LittleFS.h>


StaticJsonDocument<200> config;

void addConfig(const char* key, char *value) {
 config[key] = value;
}

char *getConfig(const char* key) {
 return config[key] = value;
}

bool loadConfig()
{
    File configFile = LittleFS.open("/config.json", "r");
    if (!configFile)
    {
        Serial.println("Failed to open config file");
        return false;
    }

    size_t size = configFile.size();
    if (size > 1024)
    {
        Serial.println("Config file size is too large");
        return false;
    }

    // Allocate a buffer to store contents of the file.
    std::unique_ptr<char[]> buf(new char[size]);

    // We don't use String here because ArduinoJson library requires the input
    // buffer to be mutable. If you don't use ArduinoJson, you may as well
    // use configFile.readString instead.
    configFile.readBytes(buf.get(), size);

    StaticJsonDocument<200> doc;
    auto error = deserializeJson(doc, buf.get());
    if (error)
    {
        Serial.println("Failed to parse config file");
        return false;
    }

    const char *serverName = doc["serverName"];
    const char *accessToken = doc["accessToken"];

    // Real world application would store these values in some variables for
    // later use.

    Serial.print("Loaded serverName: ");
    Serial.println(serverName);
    Serial.print("Loaded accessToken: ");
    Serial.println(accessToken);
    return true;
}

bool saveConfig() {
 
  doc["serverName"] = "api.example.com";
  doc["accessToken"] = "128du9as8du12eoue8da98h123ueh9h98";

  File configFile = LittleFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  serializeJson(doc, configFile);
  return true;
}
*/
