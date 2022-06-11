#ifndef HARDWAREDRIVER_H
#define HARDWAREDRIVER_H

//My Previous Work of writing driver for ventilator plz see
//https://www.github.com/nabilphysics/ventilator
 
class HardwareDriver {
public:
    
    HardwareDriver(int tempSensor, int rtc, int imuSensor, int otherSensors);
    
    //INPUT SENSORS
    float getTemperatureSensorData();
    float getRtcData();
    float getBatteryVolt();
    float getImuData();
    float getGpsData();
    float getOtherSensorsInput();
    
    //OUTPUT DEVICE
    void RgbledOutput();
    void buzzerOutput();
    void otherOutputDevice();
private:

    int tempSensorAddress;
    int rtcAddress;
    int imuSensorAddress;
    int otherSensorsAddress;
    
};


#endif
