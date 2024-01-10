//---------------------------------------------------------------------------
//
// SCSI target emulator and SCSI tools for the Raspberry Pi
//
// Copyright (C) 2023 Uwe Seimet
//
//---------------------------------------------------------------------------

#pragma once

#include <cstdint>
#include <string>
#include <set>
#include <vector>

using namespace std;

struct sockaddr_in;

namespace network_util
{
bool IsInterfaceUp(const string&);
vector<uint8_t> GetMacAddress(const string&);
set<string, less<>> GetNetworkInterfaces();
bool ResolveHostName(const string&, sockaddr_in*);
}
