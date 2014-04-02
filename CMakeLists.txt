if(NOT DEFINED UTILS_TARGET)
    set(UTILS_TARGET Utils)
endif()
string(TOLOWER ${UTILS_TARGET} UTILS_TARGET_TOLOWER)

project(${UTILS_TARGET})

if(NOT DEFINED ${UTILS_VERSION_MAJOR})
    set(UTILS_VERSION_MAJOR 1)
endif()
if(NOT DEFINED ${UTILS_VERSION_MINOR})
    set(UTILS_VERSION_MINOR 0)
endif()
if(NOT DEFINED ${UTILS_URI})
    set(UTILS_URI ${UTILS_TARGET})
endif()

option(QMLDUMP "Dump qml info so it's loadable by the QtCreator editor" ON)

set(out ${CMAKE_CURRENT_BINARY_DIR}/out )
file(MAKE_DIRECTORY ${out})

set(LIB_DIR "${out}/lib/${BUILD_ARCH}")
file(MAKE_DIRECTORY ${LIB_DIR})

set(UTILS_PLUGIN_DIR "${LIB_DIR}/${UTILS_TARGET}")
file(MAKE_DIRECTORY ${UTILS_PLUGIN_DIR})
file(COPY "qmldir" DESTINATION "${UTILS_PLUGIN_DIR}/")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/backend.h.in ${CMAKE_CURRENT_BINARY_DIR}/backend.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/backend.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/backend.cpp)

set(LIBS "")

set(utils_HDRS
    ${CMAKE_CURRENT_BINARY_DIR}/backend.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qmlenv.h
    )

set(utils_SRCS
    ${CMAKE_CURRENT_BINARY_DIR}/backend.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qmlenv.cpp
    )

add_library(${UTILS_TARGET} SHARED
    ${utils_MOC_SRCS}
    ${utils_SRCS}
)

qt5_use_modules(${UTILS_TARGET} Gui Core Qml Quick)

target_link_libraries(${UTILS_TARGET}
    ${LIBS}
)

set(CMAKE_BUILD_TYPE Release)

GET_TARGET_PROPERTY(UTILS_LIB_NAME ${UTILS_TARGET} LOCATION)
set(CP_LIB_FROM ${UTILS_LIB_NAME})
set(CP_LIB_TO "${UTILS_PLUGIN_DIR}")
add_custom_command(TARGET ${UTILS_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "POST BUILD"
    COMMAND ${CMAKE_COMMAND} -E echo "Copy lib to run directory"
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CP_LIB_FROM}
        ${CP_LIB_TO})

if(QMLDUMP)
add_custom_command(TARGET ${UTILS_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Run qmlplugindump"
    COMMAND qmlplugindump -notrelocatable ${UTILS_TARGET} 1.0 ${LIB_DIR} > ${CP_LIB_TO}/${UTILS_TARGET_TOLOWER}.qmltypes)
endif(QMLDUMP)

install(TARGETS ${UTILS_TARGET}
    DESTINATION lib/${BUILD_ARCH}/${UTILS_TARGET} )
install(FILES "qmldir"
    DESTINATION lib/${BUILD_ARCH}/${UTILS_TARGET} )
