# libcpp

cpp library framework implementationed by c++11.

---

## Component

| component |
| --------- |
| algorithm |
| binding   |
| container |
| crypto    |
| db        |
| encoding  |
| env       |
| file      |
| graphical |
| io        |
| log       |
| math      |
| memory    |
| misc      |
| net       |
| os        |
| plugin    |
| reflect   |
| string    |
| sync      |
| test      |
| time      |
| types     |
| util      |

---

## Build

### Dependencies

- GCC
- Clang
- MSVC

build all with cmake cmd:

```sh
cmake .. -DLOG=ON -DUSE_LOG4CPLUS=ON -DUSE_BOOSTLOG=ON -DUSE_SPDLOG=ON -DMATH=ON -DUSE_OPENCV_MAT=ON
```

| option           | default | description |
| ---------------- | ------- | ----------- |
| -DALGORITHM      | ON      |             |
| -DBINDING        | ON      |             |
| -DCONTAINER      | ON      |             |
| -DCRYPTO         | ON      |             |
| -DDB             | ON      |             |
| -DENCODING       | ON      |             |
| -DENV            | ON      |             |
| -DFILE           | ON      |             |
| -DGRAPHICAL      | ON      |             |
| -DIO             | ON      |             |
| -DUSE_BOOST_IO   | ON      |             |
| -DLOG            | ON      |             |
| -DUSE_LOG4CPLUS  | ON      |             |
| -DUSE_BOOSTLOG   | OFF     |             |
| -DUSE_SPDLOG     | OFF     |             |
| -DMATH           | ON      |             |
| -DUSE_OPENCV_MAT | OFF     |             |
| -DMEMORY         | ON      |             |
| -DMETA           | ON      |             |
| -DMISC           | ON      |             |
| -DNET            | ON      |             |
| -DOS             | ON      |             |
| -DPLUGIN         | ON      |             |
| -DSTRINGS        | ON      |             |
| -DSYNC           | ON      |             |
| -DTEST           | ON      |             |
| -DTIME           | ON      |             |
| -DTYPES          | ON      |             |
| -DUTIL           | ON      |             |

---

## Examples

```sh
cmake .. -DBUILD_TEST=ON
```

| option       | default | description |
| ------------ | ------- | ----------- |
| -DBUILD_TEST | ON      |             |

---

## License

---

## 3rd Party

- Boost: https://www.boost.org
- tensorflow: https://github.com/tensorflow/tensorflow
- BehaviorTree.CPP: https://github.com/BehaviorTree/BehaviorTree.CPP
- OpenSSL: https://www.openssl.org
- hiredis: https://github.com/redis/hiredis
- nlohmann/json: https://github.com/nlohmann/json
- rapidjson: https://github.com/Tencent/rapidjson
- pugixml: https://github.com/zeux/pugixml
- opencv: https://github.com/opencv/opencv
- libzmq: https://github.com/zeromq/libzmq
- zmqpp: https://github.com/zeromq/zmqpp
- log4cplus: https://github.com/log4cplus/log4cplus
- libharu: https://github.com/libharu/libharu
- concurrentqueue: https://github.com/cameron314/concurrentqueue
- mosquitto: https://github.com/eclipse/mosquitto
- zlib: https://github.com/madler/zlib
- spdlog: https://github.com/gabime/spdlog
