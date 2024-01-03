//---------------------------------------------------------------------------
//
// SCSI target emulator and SCSI tools for the Raspberry Pi
//
// Copyright (C) 2022-2023 Uwe Seimet
//
//---------------------------------------------------------------------------

#include <gtest/gtest.h>
#include "devices/ctapdriver.h"

TEST(CTapDriverTest, Crc32)
{
    array<uint8_t, ETH_FRAME_LEN> buf;

    buf.fill(0x00);
    EXPECT_EQ(0xe3d887bbU, CTapDriver::Crc32(span(buf.data(), ETH_FRAME_LEN)));

    buf.fill(0xff);
    EXPECT_EQ(0x814765f4U, CTapDriver::Crc32(span(buf.data(), ETH_FRAME_LEN)));

    buf.fill(0x10);
    EXPECT_EQ(0xb7288Cd3U, CTapDriver::Crc32(span(buf.data(), ETH_FRAME_LEN)));

    buf.fill(0x7f);
    EXPECT_EQ(0x4b543477U, CTapDriver::Crc32(span(buf.data(), ETH_FRAME_LEN)));

    buf.fill(0x80);
    EXPECT_EQ(0x29cbd638U, CTapDriver::Crc32(span(buf.data(), ETH_FRAME_LEN)));

    for (size_t i = 0; i < buf.size(); i++) {
        buf[i] = (uint8_t)i;
    }
    EXPECT_EQ(0xe7870705U, CTapDriver::Crc32(span(buf.data(), ETH_FRAME_LEN)));

    for (size_t i = buf.size() - 1; i > 0; i--) {
        buf[i] = (uint8_t)i;
    }
    EXPECT_EQ(0xe7870705U, CTapDriver::Crc32(span(buf.data(), ETH_FRAME_LEN)));
}
