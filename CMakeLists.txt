CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT("simple.nn" C CXX)

######## VERSION #######
SET(SIMPLE_FLOW_VERSION "3.0.0")

######## COMPILE #######
SET(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
SET(CMAKE_BUILD_TYPE "DEBUG")
SET(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread")

######## CONFIG OPTION ######
OPTION(BUILD_USE_AVX           "Use AVX"                    OFF)
OPTION(BUILD_TEST              "Build GooleTest"            OFF)
OPTION(BUILD_LOG               "Build spdlog"               OFF)

IF(BUILD_LOG)
    # SET(CONFIG_SIMPLE_BASE_ENABLE_SPDLOG 1)
    ADD_DEFINITIONS(-DCONFIG_SIMPLE_BASE_ENABLE_SPDLOG)
ENDIF(BUILD_LOG)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
LIST(APPEND CMAKE_FIND_ROOT_PATH "${CMAKE_SOURCE_DIR}/third_party/")
####### EXTERNAL DEPS #######
SET(EXTERNAL_DEPS)
####### EXTERNAL LIBS #######
SET(EXPORT_LIBS) 

INCLUDE(Utils)
INCLUDE(simple-base)
INCLUDE(simple-lib)

IF(BUILD_QNN)
    CMAKE_POLICY(PUSH)
    IF(NOT DEFINED QNN_DIR)
        SET(QNN_DIR third_party/simple.lib)
    ENDIF()

    # ADD_SUBDIRECTORY(${QNN_DIR})
    # INCLUDE(simple-lib)
    INCLUDE_DIRECTORIES(${HPC_INC_PATH})
    # LIST(APPEND QNN_LIBRARIES gtest gtest_main)
    CMAKE_POLICY(POP)
ENDIF(BUILD_QNN)

CONFIGURE_FILE(
    "${PROJECT_SOURCE_DIR}/src/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h")
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

####### INCLUDE #######
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/include
)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

####### SOURCES #######
FILE(GLOB_RECURSE nn_src ${PROJECT_SOURCE_DIR}/src/*.cc)

####### THIDPARTY LIBS #######
SET(THIRD_PARTY_LIBS
    ${EXPORT_LIBS}
    #${EIGEN3_LIBS}
)


####### BUILD STATIC LIB #######
SET(SIMPLE_NN_LIBS nn)
ADD_LIBRARY(${SIMPLE_NN_LIBS} STATIC ${nn_src})
ADD_DEPENDENCIES(${SIMPLE_NN_LIBS} ${EXTERNAL_DEPS})
####### BUILD SHARED LIB #######
# SET(SIMPLE_NN_LIBS nn)
# ADD_LIBRARY(${SIMPLE_NN_LIBS} SHARED ${nn_src})

####### BUILD SAMPLE #######
ADD_SUBDIRECTORY(samples)

####### INSTALL LIBS #######
IF(NOT SKIP_INSTALL_HEADERS AND NOT SKIP_INSTALL_ALL )
    INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/include DESTINATION "${CMAKE_INSTALL_PREFIX}/")
ENDIF()
IF(NOT SKIP_INSTALL_LIBRARIES AND NOT SKIP_INSTALL_ALL )
    INSTALL(TARGETS ${SIMPLE_NN_LIBS} ${SIMPLE_NN_LIBS}
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" )
ENDIF()

####### UINT TEST   ##########
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

######### Note! Please Use GCC 5.0+ #######
IF(BUILD_TEST)
    pipe_download_dependency(
        "https://github.com/google/googletest.git"
        release-1.12.1
        gtest
        ${PROJECT_SOURCE_DIR}/third_party)

    CMAKE_POLICY(PUSH)
    OPTION(BUILD_GMOCK "Builds the googlemock subproject" OFF)
    OPTION(
        gtest_force_shared_crt
        "Use shared (DLL) run-time lib even when Google Test is built as static lib."
        ON)

    IF(NOT DEFINED GTEST_DIR)
        SET(GTEST_DIR third_party/gtest)
    ENDIF()

    ADD_SUBDIRECTORY(${GTEST_DIR})
    SET(GTEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/third_party/gtest/googletest/include)
    LIST(APPEND GTEST_LIBRARIES gtest gtest_main)
    CMAKE_POLICY(POP)
ENDIF(BUILD_TEST)

IF(BUILD_TEST)
    ADD_SUBDIRECTORY(tests)
ENDIF()