#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

class Network {
private:
  Network(){}
public:
  static Network* getInstance() {
    static Network obj;
    return &obj;
  };
  void network_init();
  void network_loop();
};

#endif  // _NETWORK_H_
