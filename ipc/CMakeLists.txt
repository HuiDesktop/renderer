cmake_minimum_required(VERSION 3.2)

project(ipc C)

add_library(ipc "ipc.c" "ipc.h" "windowsSharedMemory.c" "windowsSharedMemory.h" "randomName.c" "randomName.h" "message.c" "message.h")
target_include_directories(ipc PUBLIC .)

add_executable(ipcSenderTest "senderTest.c")
target_link_libraries(ipcSenderTest ipc)

add_executable(ipcReceiverTest "receiverTest.c")
target_link_libraries(ipcReceiverTest ipc)
