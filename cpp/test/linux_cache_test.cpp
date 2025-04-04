//---------------------------------------------------------------------------
//
// SCSI2Pi, SCSI device emulator and SCSI tools for the Raspberry Pi
//
// Copyright (C) 2024-2025 Uwe Seimet
//
//---------------------------------------------------------------------------

#include "mocks.h"
#include "devices/linux_cache.h"
#include "test_shared.h"

using namespace testing;

TEST(LinuxCache, Init)
{
    LinuxCache cache1("", 0, 0, false);
    EXPECT_FALSE(cache1.Init());

    LinuxCache cache2("", 512, 0, false);
    EXPECT_FALSE(cache2.Init());

    LinuxCache cache3("", 0, 1, false);
    EXPECT_FALSE(cache3.Init());

    LinuxCache cache4("", 512, 1, false);
    EXPECT_FALSE(cache4.Init());

    LinuxCache cache5("test", 512, 1, false);
    EXPECT_FALSE(cache5.Init());

    LinuxCache cache6(CreateTempFile(1), 512, 1, false);
    EXPECT_TRUE(cache6.Init());
}

TEST(LinuxCache, ReadWriteSectors)
{
    vector<uint8_t> buf(512);
    LinuxCache cache(CreateTempFile(buf.size()), static_cast<int>(buf.size()), 1, false);
    EXPECT_TRUE(cache.Init());

    EXPECT_EQ(0, cache.ReadSectors(buf, 1, 1));
    EXPECT_EQ(0, cache.WriteSectors(buf, 1, 1));

    buf[1] = 123;
    EXPECT_EQ(512, cache.WriteSectors(buf, 0, 1));
    buf[1] = 0;

    EXPECT_EQ(512, cache.ReadSectors(buf, 0, 1));
    EXPECT_EQ(123, buf[1]);
}

TEST(LinuxCache, ReadWriteLong)
{
    vector<uint8_t> buf(512);
    LinuxCache cache(CreateTempFile(buf.size()), static_cast<int>(buf.size()), 1, false);
    EXPECT_TRUE(cache.Init());

    EXPECT_EQ(0, cache.ReadLong(buf, 1, 1));
    EXPECT_EQ(0, cache.WriteLong(buf, 1, 1));

    buf[1] = 123;
    EXPECT_EQ(2, cache.WriteLong(buf, 0, 2));
    buf[1] = 0;

    EXPECT_EQ(2, cache.ReadLong(buf, 0, 2));
    EXPECT_EQ(123, buf[1]);
}

TEST(LinuxCache, Flush)
{
    LinuxCache cache(CreateTempFile(1), 512, 1, false);
    EXPECT_TRUE(cache.Flush());
}

TEST(LinuxCache, GetStatistics)
{
    NiceMock<MockDevice> device(0);
    LinuxCache cache("", 0, 0, false);

    EXPECT_EQ(2U, cache.GetStatistics(device).size());
    device.SetReadOnly(true);
    EXPECT_EQ(1U, cache.GetStatistics(device).size());
}
