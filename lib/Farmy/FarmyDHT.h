#ifndef FARMY_DHT_H
#define FARMY_DHT_H

#define ERROR_TEMPERATURE -99;

class FarmyDHT {
 public:
    FarmyDHT(int port_number);
    float fetch_temperature();

 private:
    int port_number;
};

#endif
