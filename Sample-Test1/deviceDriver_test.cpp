#include <stdexcept>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../DeviceDriver/DeviceDriver.cpp"
#include "../DeviceDriver/Application.cpp"
using namespace testing;

class DDMock : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

class DeviceDriverFixture : public testing::Test {
public:
	NiceMock<DDMock> mock;
	DeviceDriver* dd = new DeviceDriver(&mock);
	Application* app = new Application(dd);
};

TEST_F(DeviceDriverFixture, Read5times) {
	EXPECT_CALL(mock, read)
		.Times(5)
		.WillRepeatedly(Return('A'));

	int ret = dd->read(0);
	cout << ret << endl;
}

TEST_F(DeviceDriverFixture, NotSameReadResult) {
	EXPECT_CALL(mock, read(0))
		.Times(5)
		.WillOnce(Return('A'))
		.WillOnce(Return('A'))
		.WillOnce(Return('A'))
		.WillOnce(Return('A'))
		.WillOnce(Return('F'));

	EXPECT_THROW(dd->read(0), ReadFailException);
}

TEST_F(DeviceDriverFixture, NormalWrite) {
	EXPECT_CALL(mock, read(0))
		.Times(5)
		.WillRepeatedly(Return(0xFF));
	EXPECT_CALL(mock, write(0, 'B'))
		.Times(1);
	dd->write(0, 'B');
}

TEST_F(DeviceDriverFixture, AlreadyWrite) {
	EXPECT_CALL(mock, read(0))
		.Times(5)
		.WillRepeatedly(Return(0x49));
	EXPECT_CALL(mock, write(0, 'B'))
		.Times(0);
	EXPECT_THROW(dd->write(0, 'B'), WriteFailException);
}

TEST_F(DeviceDriverFixture, ApplicationRead) {
	EXPECT_CALL(mock, read(0x1))
		.Times(5)
		.WillRepeatedly(Return(0x49));
	EXPECT_CALL(mock, read(0x2))
		.Times(5)
		.WillRepeatedly(Return(0x50));
	EXPECT_CALL(mock, read(0x3))
		.Times(5)
		.WillRepeatedly(Return(0x51));
	EXPECT_CALL(mock, read(0x4))
		.Times(5)
		.WillRepeatedly(Return(0x52));
	EXPECT_CALL(mock, read(0x5))
		.Times(5)
		.WillRepeatedly(Return(0x53));

	app->ReadAndPrint(0x1, 0x5);
}

TEST_F(DeviceDriverFixture, ApplicationWrite) {
	EXPECT_CALL(mock, read(0x0))
		.Times(5)
		.WillRepeatedly(Return(0xFF));
	EXPECT_CALL(mock, read(0x1))
		.Times(5)
		.WillRepeatedly(Return(0xFF));
	EXPECT_CALL(mock, read(0x2))
		.Times(5)
		.WillRepeatedly(Return(0xFF));
	EXPECT_CALL(mock, read(0x3))
		.Times(5)
		.WillRepeatedly(Return(0xFF));
	EXPECT_CALL(mock, read(0x4))
		.Times(5)
		.WillRepeatedly(Return(0xFF));

	EXPECT_CALL(mock, write(0x0, 0x60));
	EXPECT_CALL(mock, write(0x1, 0x60));
	EXPECT_CALL(mock, write(0x2, 0x60));
	EXPECT_CALL(mock, write(0x3, 0x60));
	EXPECT_CALL(mock, write(0x4, 0x60));
	app->WriteAll(0x60);
}