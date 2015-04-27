SET(SOURCES
    frame.h
    main.cpp
    message.cpp
    message.h
    optional.cpp
    optional.h
    result.cpp
    result.h
    udplink.cpp
    udplink.h
)

add_executable(app ${SOURCES})
