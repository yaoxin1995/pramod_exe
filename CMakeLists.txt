cmake_minimum_required(VERSION 2.8)
project(hashTable)

include_directories(src)

add_executable(server
    src/server.c
    src/queue.c
    src/hash_table.c
)
target_link_libraries(server pthread)
