#pragma once
#include "DeviceDriver.h"

class Application {
public:
	Application(DeviceDriver* driver);
	void ReadAndPrint(long startAddr, long endAddr);
	void WriteAll(int data);

protected:
    DeviceDriver* m_driver;
};
