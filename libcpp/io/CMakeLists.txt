cmake_minimum_required(VERSION 3.24.1)

message("libcpp/io enabled")

if (USE_BOOST_IO)
    set(FETCH_BOOST "ON" CACHE STRING INTERNAL FORCE)
endif(USE_BOOST_IO)
