cmake_minimum_required( VERSION 3.00)
include(CMakeDependentOption)

# set the project name
project( webserver1 VERSION 0.1 )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)



#default 
set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE 
        STRING "Choose the type of build." FORCE)
    set(CMAKE_DEBUG_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
    set(CMAKE_DEBUG_FLAGS_RELEASE "ENV{CXXFLAGS} -O3 -Wall")

    # # set gui-property
    # set_property(CACHE CAMKE_BUILD_TYPE PROPERTY STRINGS
    # "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_THREAD_PREFER_PTHREAD ON)
find_package(Threads REQUIRED)

if(CMAKE_COMPILER_IS_GNUCC_OR_CLANG)
  if(CMAKE_COMPILER_IS_CLANG)
    if(UNIX AND NOT APPLE)
      set(CMAKE_CXX_FLAGS "-pthread ${CMAKE_CXX_FLAGS}")  #force pthread to avoid bugs in some cmake setups
      set(CMAKE_C_FLAGS "-pthread ${CMAKE_C_FLAGS}")
    endif()
  endif()
  string(REGEX REPLACE "-O0" "-Og" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
  string(REGEX REPLACE "-O3" "-Ofast" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
  string(REGEX REPLACE "-O0" "-Og" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
  string(REGEX REPLACE "-O3" "-Ofast" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
  if(ENABLE_SSE_AND_AVX_FLAGS)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffp-contract=fast -mavx -mavx2 -msse3 -msse4.1 -msse4.2 -msse4a")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffp-contract=fast -mavx -mavx2 -msse3 -msse4.1 -msse4.2 -msse4a")
  endif()
endif()

set(CXX_FLAGS
    -g
    -Wall
    -std=c++17
    -D_PTHREADS
    -Wno-unused-parameter
    -o out.o
)

# 
aux_source_directory( . SRC_LIST)

# add the executable ${XXX} 不能有空格
add_executable( webserver1 ${SRC_LIST} ) 

# .a .so link directory
set(LINK_DIR /usr/lib/x86_64-linux-gnu)
link_directories(${LINK_DIR})

target_link_libraries( webserver1 libpthread.so)

#尋找 opencv 庫 
set(OpenCV_DIR /ur/local/include/opencv4)
find_package(OpenCV 4 REQUIRED)  # 要求最小版本
#添加頭文件
include_directories(${OpenCV_INCLUDE_DIRS})
# 最後還要鏈接！把庫目錄和項目鏈接
target_link_libraries( webserver1 ${OpenCV_LIBS} )

