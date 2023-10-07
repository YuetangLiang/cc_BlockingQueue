#ifndef DATACENTER_H
#define DATACENTER_H

#include "BlockingQueue.h"
#include <map>

class DataCenter
{
  public:
    static DataCenter* getInstance(std::vector<std::string> topics = {});
    ~DataCenter();

    void push(std::string&, std::string& data);
    bool empty(std::string&);
    int pop(std::string&, std::string& data);
    int pop(std::string&, std::string& data, int);

    DataCenter(std::vector<std::string> topics = {}) {
        /* std::vector<std::string> topics = { */
        /*     "/ap/egopose", "/msd/worldmodel", */
        /* }; */

        for(auto &topic : topics) {
            printf("topic:%s queue created\n", topic.c_str());
            _queues[topic].reset(new dataQueue_t(1));
        }

        //_queues["/mla/egopose"].reset(new dataQueue_t(1));
    };
    DataCenter(DataCenter& other) = delete;
    void operator=(const DataCenter &) = delete;

    static DataCenter* _instance;
    static std::mutex  _mutex;
    mutable std::mutex _m;

    typedef BlockingQueue<std::string> dataQueue_t;
    std::map<std::string, std::shared_ptr<dataQueue_t>> _queues;

};


#endif  /* DATACENTER_H */

