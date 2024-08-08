﻿#ifndef RtpSort_H
#define RtpSort_H

#include <map>
#include <string>
#include <memory>
#include <iostream>
#include <vector>

#include "Net/Buffer.h"
#include "RtpPacket.h"
#include "Log/Logger.h"

using namespace std;

struct myCompare {
    bool operator()(const uint16_t& l, const uint16_t& r)const
    {
        static constexpr uint16_t SEQ_MAX = (std::numeric_limits<uint16_t>::max)();
        static constexpr uint16_t kBreakpoint = SEQ_MAX >> 1 + 1;

        if (l - r == kBreakpoint) {
            return l > r;
        }

        return l != r && static_cast<uint16_t>(l - r) > kBreakpoint;
    }
};

class RtpSort
{
public:
    using Ptr = shared_ptr<RtpSort>;
    RtpSort(int maxQueSize = 25);

    void inputRtp(const RtpPacket::Ptr& rtp);
    void onRtpPacket(const RtpPacket::Ptr& rtp);
    void setOnRtpPacket(const function<void(const RtpPacket::Ptr& rtp)>& cb);
    
private:
    bool _firstRtp = true;
    uint16_t _lastRtpSeq = -1;
    int _maxQueSize;
    map<uint16_t, RtpPacket::Ptr, myCompare> _mapRtp;
    function<void(const RtpPacket::Ptr& rtp)> _onRtpPacket;
};


#endif //RtpSort_H
