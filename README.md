# BlockingQueue
## subr
```cpp
while (ok()) {
  int timeout = 1000;
  int ret = DataCenter::getInstance()->pop(topic, data, timeout);
  if (ret) {
    printf("%s timeout\n", topic.c_str());
    continue;
  }

  // TODO: data
}
```

## pubr
```cpp
DataCenter::getInstance()->push(topic, data);
```

