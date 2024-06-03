#include "DeviceDriver.h"
#include <vector>
#include <stdexcept>
using namespace std;

class ReadFailException : public exception {
public:
    ReadFailException(char const* _Message)
        : exception(_Message)
    {
    }
};

class WriteFailException : public exception {
public:
    WriteFailException(char const* _Message)
        : exception(_Message)
    {
    }
};

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    int result = (int)(m_hardware->read(address));
    for (int i = 0; i < 4; i++) {
        if (result != (int)(m_hardware->read(address))) throw ReadFailException("Not Same Result");
    }
    return result;
}

void DeviceDriver::write(long address, int data)
{
    if (read(address) != 0xFF) throw WriteFailException("already have value");
	m_hardware->write(address, (unsigned char)data);
}