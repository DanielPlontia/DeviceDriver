#include <iostream>

#include "Application.h"

using namespace std;

Application::Application(DeviceDriver* driver)
	: m_driver (driver) 
{}

void Application::ReadAndPrint(long startAddr, long endAddr)
{
	for (long i = startAddr; i <= endAddr; i++)
	{
		cout << m_driver->read(i) << endl;
	}
}

void Application::WriteAll(int data)
{
	for (int i = 0x0; i <= 0x4; i++)
	{
		m_driver->write(i, data);
	}
}

