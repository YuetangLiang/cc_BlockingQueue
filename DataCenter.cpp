#include "DataCenter.h"
#include <iostream>

DataCenter* DataCenter::_instance{nullptr};
std::mutex DataCenter::_mutex;

DataCenter* DataCenter::getInstance(std::vector<std::string> topics)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if(_instance == nullptr) {
        _instance = new DataCenter(topics);
    }
    return _instance;
}

DataCenter::~DataCenter(){}

void DataCenter::push(std::string& topic,std::string& data)
{
    //std::cout << "[DataCenter] push data" << std::endl;
    _queues[topic]->Push(data);
}

bool DataCenter::empty(std::string& topic)
{
    return _queues[topic]->Empty();
}

int DataCenter::pop(std::string& topic, std::string& data)
{
    return _queues[topic]->Pop(data);
}

int DataCenter::pop(std::string& topic, std::string& data, int timeout_ms)
{
    return _queues[topic]->Pop(data, timeout_ms);
}


