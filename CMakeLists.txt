project(Struggle_nsq)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(PUB_SOURCE_FILES
        pub_test.c pub_client.c
        pub_client.h)

add_executable(pub_client.o ${PUB_SOURCE_FILES})

set(SUB_SOURCE_FILES sub_test.c sub_client.c sub_client.h)
add_executable(test.o ${SUB_SOURCE_FILES})
