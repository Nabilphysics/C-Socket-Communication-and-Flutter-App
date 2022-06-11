#include "hardwaredriver.h"

HardwareDriver::HardwareDriver(int tempSensor, int rtc, int imuSensor, int otherSensors)
{
	tempSensorAddress = tempSensor;
	rtcAddress = rtc;
	imuSensorAddress = imuSensor;
	otherSensorsAddress = otherSensors;
}

float HardwareDriver::getTemperatureSensorData()
{
	

}
float HardwareDriver::getRtcData() {
	//RTC CODE
}
float HardwareDriver::getBatteryVolt() {
	//Battery CODE
}
float HardwareDriver::getImuData() {
	//IMU CODE
}
float HardwareDriver::getGpsData() {
	//GPS CODE
}
float HardwareDriver::getOtherSensorsInput() {
	
}

//OUTPUT DEVICE
void HardwareDriver::RgbledOutput() {

}
void HardwareDriver::buzzerOutput() {

}
void HardwareDriver::otherOutputDevice() {

}