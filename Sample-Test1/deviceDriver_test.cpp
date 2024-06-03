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

TEST(DeviceDriverFixture, Read5times) {
	DDMock mock;
	DeviceDriver hw(&mock);
	EXPECT_CALL(mock, read)
		.Times(5)
		.WillRepeatedly(Return('A'));

	int ret = hw.read(0);
	cout << ret;
}

TEST(DeviceDriverFixture, NotSameReadResult) {
	DDMock mock;
	DeviceDriver hw(&mock);

	EXPECT_CALL(mock, read(0))
		.Times(5)
		.WillOnce(Return('A'))
		.WillOnce(Return('A'))
		.WillOnce(Return('A'))
		.WillOnce(Return('A'))
		.WillOnce(Return('F'));

	EXPECT_THROW(hw.read(0), ReadFailException);
}

TEST(DeviceDriverFixture, NormalWrite) {
	DDMock mock;
	DeviceDriver hw(&mock);

	EXPECT_CALL(mock, read(0))
		.Times(5)
		.WillRepeatedly(Return(0xFF));
	EXPECT_CALL(mock, write(0, 'B'))
		.Times(1);

	hw.write(0, 'B');
}

TEST(DeviceDriverFixture, AlreadyWrite) {
	DDMock mock;
	DeviceDriver hw(&mock);

	EXPECT_CALL(mock, read(0))
		.Times(5)
		.WillRepeatedly(Return(0x49));

	EXPECT_THROW(hw.write(0, 'B'), WriteFailException);
}