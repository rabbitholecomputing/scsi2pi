//---------------------------------------------------------------------------
//
// SCSI2Pi, SCSI device emulator and SCSI tools for the Raspberry Pi
//
// Copyright (C) 2022-2025 Uwe Seimet
//
//---------------------------------------------------------------------------

#include <spdlog/spdlog.h>
#include "page_handler.h"
#include "base/primary_device.h"
#include "base/property_handler.h"
#include "controllers/abstract_controller.h"
#include "shared/s2p_exceptions.h"

using namespace spdlog;
using namespace memory_util;
using namespace s2p_util;

PageHandler::PageHandler(PrimaryDevice &d, bool m, bool p) : device(d), supports_mode_select(m), supports_save_parameters(
    p)
{
    device.AddCommand(ScsiCommand::MODE_SENSE_6, [this]
        {
            device.DataInPhase(
                device.ModeSense6(device.GetController()->GetCdb(), device.GetController()->GetBuffer()));
        });
    device.AddCommand(ScsiCommand::MODE_SENSE_10, [this]
        {
            device.DataInPhase(
                device.ModeSense10(device.GetController()->GetCdb(), device.GetController()->GetBuffer()));
        });

    // Devices that support MODE SENSE must (at least formally) also support MODE SELECT
    device.AddCommand(ScsiCommand::MODE_SELECT_6, [this]
        {
            ModeSelect(device.GetCdbByte(4));
        });
    device.AddCommand(ScsiCommand::MODE_SELECT_10, [this]
        {
            ModeSelect(device.GetCdbInt24(7));
        });
}

int PageHandler::AddModePages(cdb_t cdb, data_in_t buf, int offset, int length, int max_size) const
{
    const int max_length = length - offset;
    if (max_length < 0) {
        return length;
    }

    const bool changeable = (cdb[2] & 0xc0) == 0x40;

    const auto page_code = cdb[2] & 0x3f;

    // Mode page data mapped to the respective page codes, C++ maps are ordered by key
    map<int, vector<byte>> pages;
    device.SetUpModePages(pages, page_code, changeable);
    const auto& [vendor, product, _] = device.GetProductData();
    for (const auto& [p, data] : GetCustomModePages(vendor, product)) {
        if (data.empty()) {
            pages.erase(p);
        }
        else {
            pages[p] = data;
        }
    }

    if (pages.empty()) {
        throw ScsiException(SenseKey::ILLEGAL_REQUEST, Asc::INVALID_FIELD_IN_CDB);
    }

    // Holds all mode page data
    vector<byte> result;

    for (const auto& [index, page_data] : pages) {
        // The specification mandates that page 0 must be returned last
        if (index) {
            const auto off = result.size();

            // Page data
            result.insert(result.end(), page_data.cbegin(), page_data.cend());
            // Page code, PS bit may already have been set
            result[off] |= static_cast<byte>(index);
            // Page payload size, does not count itself and the page code field
            result[off + 1] = static_cast<byte>(page_data.size() - 2);
        }
    }

    if (pages.contains(0)) {
        // Page data only (there is no standardized size field for page 0)
        result.insert(result.end(), pages[0].cbegin(), pages[0].cend());
    }

    if (static_cast<int>(result.size()) > max_size) {
        throw ScsiException(SenseKey::ILLEGAL_REQUEST, Asc::INVALID_FIELD_IN_CDB);
    }

    const int size = min(max_length, static_cast<int>(result.size()));
    memcpy(&buf.data()[offset], result.data(), size);

    // Do not return more than the requested number of bytes
    return size + offset < length ? size + offset : length;
}

map<int, vector<byte>> PageHandler::GetCustomModePages(const string &vendor, const string &product) const
{
    map<int, vector<byte>> pages;

    const string identifier = vendor + COMPONENT_SEPARATOR + product;

    for (const auto& [key, value] : PropertyHandler::GetInstance().GetProperties()) {
        const auto &key_components = Split(key, '.', 3);

        if (key_components[0] != PropertyHandler::MODE_PAGE) {
            continue;
        }

        const int page_code = ParseAsUnsignedInt(key_components[1]);
        if (page_code == -1 || page_code > 0x3e) {
            warn("Ignored invalid page code in mode page property '{}'", key);
            continue;
        }

        if (!identifier.starts_with(key_components[2])) {
            continue;
        }

        vector<byte> page_data;
        try {
            page_data = HexToBytes(value);
        }
        catch (const out_of_range&) {
            warn("Ignored invalid mode page definition for page {0}: {1}", page_code, value);
            continue;
        }

        if (page_data.empty()) {
            trace("Removing default mode page {}", page_code);
        }
        else {
            // Validate the page code and (except for page 0, which has no well-defined format) the page size
            if (page_code && (page_code != to_integer<int>(page_data[0] & byte { 0x3f }))) {
                warn("Ignored mode page definition with inconsistent page code {0}: {1}", page_code, page_data[0]);
                continue;
            }

            if (page_code && static_cast<byte>(page_data.size() - 2) != page_data[1]) {
                warn("Ignored mode page definition with wrong page size {0}: {1}", page_code, page_data[1]);
                continue;
            }

            trace("Adding/replacing mode page {0}: {1}", page_code, value);
        }

        pages[page_code] = page_data;
    }

    return pages;
}

void PageHandler::ModeSelect(int length) const
{
    if (!supports_mode_select || (!supports_save_parameters && (device.GetCdbByte(1) & 0x01))) {
        throw ScsiException(SenseKey::ILLEGAL_REQUEST, Asc::INVALID_FIELD_IN_CDB);
    }

    device.DataOutPhase(length);
}
