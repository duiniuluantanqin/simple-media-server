﻿#ifndef JT1078Client_h
#define JT1078Client_h

#include "Common/MediaClient.h"
#include "JT1078MediaSource.h"
#include "Net/TcpClient.h"

#include <string>
#include <memory>
#include <functional>

using namespace std;

class JT1078Client : public MediaClient, public TcpClient
{
public:
    JT1078Client(MediaClientType type, const string& appName, const string& streamName);
    ~JT1078Client();

public:
    // override MediaClient
    void start(const string& localIp, int localPort, const string& url, int timeout) override;
    void stop() override;
    void pause() override;
    void setOnClose(const function<void()>& cb) override;

public:
    // override TcpClient
    void onRead(const StreamBuffer::Ptr& buffer, struct sockaddr* addr, int len) override;
    void onError(const string& err) override;
    void close() override;
    void onConnect() override;

public:
    void setSimCode(const string& simCode) {_simCode = simCode;}
    void setChannel(int channel) {_channel = channel;}
    void startSendTalkData(const JT1078MediaSource::Ptr &jtSrc);
    void sendRtpPacket(const JT1078MediaSource::RingDataType &pkt);

private:
    MediaClientType _type;
    int _channel;
    string _simCode;
    UrlParser _localUrlParser;
    UrlParser _peerUrlParser;
    JT1078MediaSource::Wptr _source;
    JT1078MediaSource::RingType::DataQueReaderT::Ptr _playReader;
    
    EventLoop::Ptr _loop;
    Socket::Ptr _socket;
    function<void()> _onClose;
};

#endif //JT1078Client_h