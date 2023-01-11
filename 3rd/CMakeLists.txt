cmake_minimum_required(VERSION 3.24.1)

include(FetchContent)

# boost
FetchContent_Declare(
    boost_https
    URL https://boostorg.jfrog.io/artifactory/main/release/1.80.0/source/boost_1_80_0.zip
    URL_HASH SHA256=e34756f63abe8ac34b35352743f17d061fcc825969a2dd8458264edb38781782
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/boost
)

# tensorflow
FetchContent_Declare(
    tensorflow_https
    URL https://github.com/tensorflow/tensorflow/archive/refs/tags/v2.11.0.zip
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tensorflow
)
FetchContent_Declare(
    pugixml_git
    GIT_REPOSITORY https://github.com/tensorflow/tensorflow.git
    GIT_TAG v2.11.0
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tensorflow
)

# behavior_tree_cpp
FetchContent_Declare(
    behavior_tree_cpp_https
    URL https://github.com/BehaviorTree/BehaviorTree.CPP/archive/refs/tags/3.7.0.zip
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/behavior_tree_cpp
)
FetchContent_Declare(
    pugixml_git
    GIT_REPOSITORY https://github.com/BehaviorTree/BehaviorTree.CPP.git
    GIT_TAG 3.7.0
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/behavior_tree_cpp
)

# openssl
FetchContent_Declare(
    openssl_https
    URL https://www.openssl.org/source/old/1.1.1/openssl-1.1.1q.tar.gz
    URL_HASH SHA256=d7939ce614029cdff0b6c20f0e2e5703158a489a72b2507b8bd51bf8c8fd10ca
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/openssl
)
FetchContent_Declare(
    openssl_git
    GIT_REPOSITORY https://github.com/openssl/openssl.git
    GIT_TAG OpenSSL_1_1_1q
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/openssl
)

# hiredis
FetchContent_Declare(
    hiredis_https
    URL https://github.com/redis/hiredis/archive/refs/tags/v1.1.0.zip
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/hiredis
)
FetchContent_Declare(
    hiredis_git
    GIT_REPOSITORY https://github.com/redis/hiredis.git
    GIT_TAG v1.1.0
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/hiredis
)

# nlohmann_json
FetchContent_Declare(
    nlohmann_json_https
    URL https://github.com/nlohmann/json/archive/refs/tags/v3.11.2.zip
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/nlohmann_json
)
FetchContent_Declare(
    nlohmann_json_git
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/nlohmann_json
)

# rapidjson
FetchContent_Declare(
    rapidjson_https
    URL https://github.com/Tencent/rapidjson/archive/refs/tags/v1.1.0.zip
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson
)
FetchContent_Declare(
    rapidjson_git
    GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
    GIT_TAG v1.1.0
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson
)

# pugixml
FetchContent_Declare(
    pugixml_https
    URL https://github.com/zeux/pugixml/archive/refs/tags/v1.13.zip
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pugixml
)
FetchContent_Declare(
    pugixml_git
    GIT_REPOSITORY https://github.com/zeux/pugixml.git
    GIT_TAG v1.13
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pugixml
)

# opencv
FetchContent_Declare(
    opencv_https
    URL https://github.com/opencv/opencv/archive/refs/tags/4.7.0.zip
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/opencv
)
FetchContent_Declare(
    opencv_git
    GIT_REPOSITORY https://github.com/opencv/opencv.git
    GIT_TAG 4.7.0
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/opencv
)

# libzmq
FetchContent_Declare(
    libzmq_https
    URL https://github.com/zeromq/libzmq/releases/download/v4.3.4/zeromq-4.3.4.zip
    URL_HASH MD5=4d1cf4d0a1c5b5c821ba3479220a24ae
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libzmq
)
FetchContent_Declare(
    libzmq_git
    GIT_REPOSITORY https://github.com/zeromq/libzmq.git
    GIT_TAG master
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libzmq
)

# zmqpp
FetchContent_Declare(
    zmqpp_https
    URL https://github.com/zeromq/zmqpp/archive/refs/tags/4.2.0.zip
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zmqpp
)
FetchContent_Declare(
    zmqpp_git
    GIT_REPOSITORY https://github.com/zeromq/zmqpp.git
    GIT_TAG master
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zmqpp
)

# log4cplus
FetchContent_Declare(
    log4cplus_https
    URL https://github.com/log4cplus/log4cplus/archive/refs/tags/REL_2_0_7.zip
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/log4cplus
)
FetchContent_Declare(
    log4cplus_git
    GIT_REPOSITORY https://github.com/log4cplus/log4cplus.git
    GIT_TAG v2.0.7
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/log4cplus
)

# libharu (a pdf project)
FetchContent_Declare(
    libharu_https
    URL https://github.com/libharu/libharu/archive/refs/tags/v2.4.3.zip
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libharu
)
FetchContent_Declare(
    libharu_git
    GIT_REPOSITORY https://github.com/libharu/libharu.git
    GIT_TAG v.2.4.3
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libharu
)

# concurrentqueue
FetchContent_Declare(
    concurrentqueue_https
    URL https://github.com/cameron314/concurrentqueue/archive/refs/tags/v1.0.3.zip
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/concurrentqueue
)
FetchContent_Declare(
    concurrentqueue_git
    GIT_REPOSITORY https://github.com/cameron314/concurrentqueue.git
    GIT_TAG v1.0.3
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/concurrentqueue
)

# mosquitto
FetchContent_Declare(
    mosquitto_https
    URL https://github.com/eclipse/mosquitto/archive/refs/tags/v2.0.15.zip
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mosquitto
)
FetchContent_Declare(
    mosquitto_git
    GIT_REPOSITORY https://github.com/eclipse/mosquitto.git
    GIT_TAG v2.0.15
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mosquitto
)

# zlib
FetchContent_Declare(
    zlib_https
    URL https://github.com/madler/zlib/archive/refs/tags/v1.2.13.zip
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zlib
)
FetchContent_Declare(
    zlib_git
    GIT_REPOSITORY https://github.com/madler/zlib.git
    GIT_TAG v1.2.13
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zlib
)

# unreal
FetchContent_Declare(
    unreal_https
    URL https://github.com/EpicGames/UnrealEngine/archive/refs/tags/5.1.0-release.zip
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/unreal
)
FetchContent_Declare(
    unreal_git
    GIT_REPOSITORY https://github.com/EpicGames/UnrealEngine.git
    GIT_TAG 5.1.0-release
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/unreal
)

# eigen
FetchContent_Declare(
    eigen_https
    URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.zip
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/eigen
)

# spdlog
FetchContent_Declare(
    spdlog_https
    URL https://github.com/gabime/spdlog/archive/refs/tags/v1.11.0.zip
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/spdlog
)
FetchContent_Declare(
    spdlog_git
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.11.0
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/spdlog
)



################################################################################

if (USE_BOOSTLOG)
    set(FETCH_BOOST "ON" CACHE STRING INTERNAL FORCE)
elseif (USE_BOOST_TYPES)
    set(FETCH_BOOST "ON" CACHE STRING INTERNAL FORCE)
elseif (USE_BOOST_IO)
    set(FETCH_BOOST "ON" CACHE STRING INTERNAL FORCE)
endif()
if(FETCH_BOOST)
	FetchContent_MakeAvailable(boost_https)

    if (UNIX)
        execute_process(COMMAND ./bootstrap.sh 
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/boost)
        execute_process(COMMAND ./b2
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/boost)
    endif()
    if (WIN32)
        execute_process(COMMAND bootstrap.bat 
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/boost)
        execute_process(COMMAND b2.exe 
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/boost)
    endif()
    if (APPLE)

    endif()
    if (MINGW)

    endif()
endif()

if(FETCH_TENSORFLOW)
	FetchContent_MakeAvailable(tensorflow_https)
endif()

if(FETCH_BEHAVIOR_TREE_CPP)
	FetchContent_MakeAvailable(behavior_tree_cpp_https)
endif()

if(FETCH_CONCURRENTQUEUE)
	FetchContent_MakeAvailable(concurrentqueue_https)
endif()

if(FETCH_OPENSSL)
	FetchContent_MakeAvailable(openssl_https)
endif()

if(FETCH_HIREDIS)
	FetchContent_MakeAvailable(hiredis_https)
endif()

if(FETCH_NLOHMANN_JSON)
	FetchContent_MakeAvailable(nlohmann_json_https)
endif()

if(FETCH_RAPIDJSON)
	FetchContent_MakeAvailable(rapidjson_https)
endif()

if(FETCH_PUGIXML)
	FetchContent_MakeAvailable(pugixml_https)
endif()

if(FETCH_OPENCV)
	FetchContent_MakeAvailable(opencv_https)
endif()

if(FETCH_LIBZMQ)
	FetchContent_MakeAvailable(libzmq_https)
endif()

if(FETCH_ZMQPP)
	FetchContent_MakeAvailable(zmqpp_https)
endif()

if(FETCH_LOG4CPLUS)
	FetchContent_MakeAvailable(log4cplus_https)

    # if (WIN32)
    #     execute_process(COMMAND devenv.exe ./msvc14/log4cplus.sln /build "Release" /Project "log4cplus" /ProjectConfig "Release|x64" 
    #                     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/log4cplus)
    # endif()
endif()

if(FETCH_LIBHARU)
	FetchContent_MakeAvailable(libharu_https)
endif()

if(FETCH_MOSQUITTO)
	FetchContent_MakeAvailable(mosquitto_https)
endif()

if(FETCH_ZLIB)
	FetchContent_MakeAvailable(zlib_https)
endif()

if(FETCH_UNREAL)
	FetchContent_MakeAvailable(unreal_https)
endif()

if(FETCH_EIGEN)
	FetchContent_MakeAvailable(eigen_https)
endif()

if(FETCH_SPDLOG)
	FetchContent_MakeAvailable(spdlog_https)
endif()