cmake_minimum_required(VERSION 3.24.1)

if (ALGORITHM)
    add_subdirectory(algorithm)
endif(ALGORITHM)

if (BINDING)
    add_subdirectory(binding)
endif(BINDING)

if (CONTAINER)
    add_subdirectory(container)
endif(CONTAINER)

if (CRYPTO)
    add_subdirectory(crypto)
endif(CRYPTO)

if (DB)
    add_subdirectory(db)
endif(DB)

if (ENCODING)
    add_subdirectory(encoding)
endif(ENCODING)

if (ENV)
    add_subdirectory(env)
endif(ENV)

if (FILE)
    add_subdirectory(file)
endif(FILE)

if (GRAPHICAL)
    add_subdirectory(graphical)
endif(GRAPHICAL)

if (IO)
    add_subdirectory(io)
endif(IO)

if (LOG)
    add_subdirectory(log)
endif()

if (MATH)
    add_subdirectory(math)
endif(MATH)

if (MEMORY)
    add_subdirectory(memory)
endif(MEMORY)

if (META)
    add_subdirectory(meta)
endif(META)

if (MISC)
    add_subdirectory(misc)
endif(MISC)

if (NET)
    add_subdirectory(net)
endif(NET)

if (OS)
    add_subdirectory(os)
endif(OS)

if (PLUGIN)
    add_subdirectory(plugin)
endif(PLUGIN)

if (STRINGS)
    add_subdirectory(strings)
endif(STRINGS)

if (SYNC)
    add_subdirectory(sync)
endif(SYNC)

if (TEST)
    add_subdirectory(test)
endif(TEST)

if (TIME)
    add_subdirectory(time)
endif(TIME)

if (TYPES)
    add_subdirectory(types)
endif(TYPES)

if (UTIL)
    add_subdirectory(util)
endif(UTIL)