#pragma once
class NetworkEvent
{
public:
	virtual void OnConnect() = 0;
	virtual void OnDisconnect() = 0;
	virtual void OnPacketReceive(std::string packet) = 0;
};

