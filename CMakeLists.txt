cmake_minimum_required(VERSION 3.23)
project(mivnewet2)

set(CMAKE_CXX_STANDARD 14)

add_executable(mivnewet2 mainWet2.cpp recordsCompany.cpp Customer.cpp Record.cpp hashTable.cpp unionFind.cpp
        AVLTree.h utilesWet2.h)
