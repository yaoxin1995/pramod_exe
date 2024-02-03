cmake_minimum_required(VERSION 2.8)
project(hashTable)
add_compile_options("-Wno-unused-variable")

include_directories(src)

add_executable(server
    src/server.c
    src/queue.c
    src/hash_table.c
    src/ipc_api.c
)
target_link_libraries(server pthread)


add_executable(client
    src/client.c
    src/queue.c
    src/ipc_api.c
    src/hash_table.c
)
target_link_libraries(client pthread)
