cmake_minimum_required(VERSION 3.8)
project(blocksim)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -lpthread")

set(SOURCE_FILES main.cpp

        src/blockchain/Blockchain.cpp
        src/blockchain/Blockchain.h
        src/block/Block.cpp
        src/block/Block.h
        src/block/Transaction.cpp
        src/block/Transaction.h

        src/crypto/SHA256.cpp
        src/crypto/SHA256.h

        src/blockchain/InternalWallet.h)
add_executable(blocksim ${SOURCE_FILES})
