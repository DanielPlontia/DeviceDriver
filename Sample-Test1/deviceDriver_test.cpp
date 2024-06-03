#include <stdexcept>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../DeviceDriver/DeviceDriver.cpp"
using namespace testing;

class DDMock : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

class DeviceDriverFixture : public Test {
public:
	DDMock mock;
	DeviceDriver* hw = new DeviceDriver(&mock);
};

TEST_F(DeviceDriverFixture, Read5times) {
	EXPECT_CALL(mock, read)
		.Times(5)
		.WillRepeatedly(Return('A'));

	int ret = hw->read(0);
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

	EXPECT_THROW(hw->read(0), ReadFailException);
}

TEST_F(DeviceDriverFixture, NormalWrite) {
	EXPECT_CALL(mock, read(0))
		.Times(5)
		.WillRepeatedly(Return(0xFF));
	EXPECT_CALL(mock, write(0, 'B'))
		.Times(1);

	hw->write(0, 'B');
}

TEST_F(DeviceDriverFixture, AlreadyWrite) {
	EXPECT_CALL(mock, read(0))
		.Times(5)
		.WillRepeatedly(Return(0x49));

	EXPECT_THROW(hw->write(0, 'B'), WriteFailException);
}