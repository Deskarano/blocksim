cmake_minimum_required(VERSION 3.8)
project(blocksim)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp src/Blockchain.cpp src/Blockchain.h src/Block.cpp src/Block.h src/Wallet.cpp src/Wallet.h src/Transaction.cpp src/Transaction.h src/SHA256.cpp src/SHA256.h)
add_executable(blocksim ${SOURCE_FILES})