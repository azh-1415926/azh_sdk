#include <iostream>
#include <thread>
#include <fstream>

#include <azh/sdk/utils/logger.hpp>

void thread01()
{
    azh::sdk::utils::logger::getInstance()->warning("This is a warnning!");
    azh::sdk::utils::logger::getInstance()->info("This is a info!");
    azh::sdk::utils::logger::getInstance()->error("This is a error!");
    azh::sdk::utils::logger::getInstance()->fatal("This is a fatal!");
}

int main()
{
    azh::sdk::utils::logger::getInstance()->fatal("Main Thread Start!");
    // logger level default : warnning
    azh::sdk::utils::logger::getInstance()->info("Hello ","World!");
    azh::sdk::utils::logger::getInstance()->warning("This is a warnning!");
    azh::sdk::utils::logger::getInstance()->info("This is a info!");
    azh::sdk::utils::logger::getInstance()->error("This is a error!");
    azh::sdk::utils::logger::getInstance()->fatal("This is a fatal!");

    // set level to error, only show error、fatal
    azh::sdk::utils::logger::getInstance()->warning("This is a warnning!");
    azh::sdk::utils::logger::getInstance()->info("This is a info!");
    azh::sdk::utils::logger::getInstance()->error("This is a error!");
    azh::sdk::utils::logger::getInstance()->fatal("This is a fatal!");

    constexpr int n=10;
    std::thread* childThreads[n];
    
    for(int i=0;i<n;i++)
    {
        childThreads[i]=new std::thread(thread01);
    }

    for(int i=0;i<n;i++)
    {
        childThreads[i]->join();
        delete childThreads[i];
    }

    azh::sdk::utils::logger::getInstance()->fatal("Main Thread End!");

    return 0;
}