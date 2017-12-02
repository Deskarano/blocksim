cmake_minimum_required(VERSION 3.8)
project(blocksim)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall")

set(SOURCE_FILES main.cpp

        src/bc/Blockchain.cpp
        src/bc/Blockchain.h
        src/Block.cpp
        src/Block.h
        src/Transaction.cpp
        src/Transaction.h

        src/crypto/SHA256.cpp
        src/crypto/SHA256.h

        src/bc/InternalWallet.h)
add_executable(blocksim ${SOURCE_FILES})