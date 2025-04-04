//---------------------------------------------------------------------------
//
// SCSI2Pi, SCSI device emulator and SCSI tools for the Raspberry Pi
//
// Copyright (C) 2001-2006 ＰＩ．(ytanaka@ipc-tokai.or.jp)
// Copyright (C) 2014-2020 GIMONS
// Copyright (C) 2022-2025 Uwe Seimet
//
//---------------------------------------------------------------------------

#include "optical_memory.h"
#include "shared/s2p_exceptions.h"

using namespace memory_util;

OpticalMemory::OpticalMemory(int lun) : Disk(SCMO, lun, true, true, { 512, 1024, 2048, 4096 })
{
    // 128 MB, 512 bytes per sector, 248826 sectors
    geometries[512 * 248826] = { 512, 248826 };
    // 230 MB, 512 bytes per block, 446325 sectors
    geometries[512 * 446325] = { 512, 446325 };
    // 540 MB, 512 bytes per sector, 1041500 sectors
    geometries[512 * 1041500] = { 512, 1041500 };
    // 640 MB, 20248 bytes per sector, 310352 sectors
    geometries[2048 * 310352] = { 2048, 310352 };

    Disk::SetProductData( { "", "SCSI MO", "" }, true);
    SetScsiLevel(ScsiLevel::SCSI_2);
    SetProtectable(true);
    SetRemovable(true);
}

void OpticalMemory::Open()
{
    assert(!IsReady());

    // For some capacities there are hard-coded, well-defined sector sizes and block counts
    if (const off_t size = GetFileSize(); !SetGeometryForCapacity(size)) {
        // Sector size (default 512 bytes) and number of sectors
        if (!SetBlockSize(GetConfiguredBlockSize() ? GetConfiguredBlockSize() : 512)) {
            throw IoException("Invalid sector size");
        }
        SetBlockCount(size / GetBlockSize());
    }

    ValidateFile();

    if (IsReady()) {
        SetAttn(true);
    }
}

vector<uint8_t> OpticalMemory::InquiryInternal() const
{
    return HandleInquiry(DeviceType::OPTICAL_MEMORY, true);
}

void OpticalMemory::SetUpModePages(map<int, vector<byte>> &pages, int page, bool changeable) const
{
    Disk::SetUpModePages(pages, page, changeable);

    // Page code 6 (option page)
    if (page == 0x06 || page == 0x3f) {
        AddOptionPage(pages);
    }

    if (page == 0x20 || page == 0x3f) {
        AddVendorPage(pages, changeable);
    }
}

void OpticalMemory::AddOptionPage(map<int, vector<byte>> &pages) const
{
    vector<byte> buf(4);
    pages[6] = buf;

    // Do not report update blocks
}

//
// Mode page code 20h - Vendor Unique Format Page
// Format mode XXh type 0
// Information: http://h20628.www2.hp.com/km-ext/kmcsdirect/emr_na-lpg28560-1.pdf

// Offset  description
// 02h   format mode
// 03h   type of format (0)
// 04~07h  size of user band (total sectors?)
// 08~09h  size of spare band (spare sectors?)
// 0A~0Bh  number of bands
//
// Actual value of each 3.5 inches optical medium (grabbed by Fujitsu M2513EL)
//
//                      128M     230M    540M    640M
// ---------------------------------------------------
// Size of user band   3CBFAh   6CF75h  FE45Ch  4BC50h
// Size of spare band   0400h    0401h   08CAh   08C4h
// Number of bands      0001h    000Ah   0012h   000Bh
//
// Further information: https://r2089.blog36.fc2.com/blog-entry-177.html
//
void OpticalMemory::AddVendorPage(map<int, vector<byte>> &pages, bool changeable) const
{
    vector<byte> buf(12);

    if (!changeable && IsReady()) {
        unsigned spare = 0;
        unsigned bands = 0;
        const uint64_t block_count = GetBlockCount();

        if (GetBlockSize() == 512) {
            switch (block_count) {
            // 128MB
            case 248826:
                spare = 1024;
                bands = 1;
                break;

                // 230MB
            case 446325:
                spare = 1025;
                bands = 10;
                break;

                // 540MB
            case 1041500:
                spare = 2250;
                bands = 18;
                break;

            default:
                break;
            }
        }

        if (GetBlockSize() == 2048) {
            switch (block_count) {
            // 640MB
            case 310352:
                spare = 2244;
                bands = 11;
                break;

                // 1.3GB (lpproj: not tested with real device)
            case 605846:
                spare = 4437;
                bands = 18;
                break;

            default:
                break;
            }
        }

        // Format mode
        buf[2] = byte { 0 };
        // Format type
        buf[3] = byte { 0 };
        SetInt32(buf, 4, static_cast<uint32_t>(block_count));
        SetInt16(buf, 8, spare);
        SetInt16(buf, 10, bands);
    }

    pages[32] = buf;

    return;
}

bool OpticalMemory::SetGeometryForCapacity(uint64_t capacity)
{
    if (const auto &geometry = geometries.find(capacity); geometry != geometries.end()) {
        SetBlockSize(geometry->second.first);
        SetBlockCount(geometry->second.second);

        return true;
    }

    return false;
}
