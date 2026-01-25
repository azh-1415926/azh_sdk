#include <azh/type/type.hpp>
#include <azh/utils/logger.hpp>
// #include <azh/utils/arguments.hpp>
#include <azh/utils/file_reader.hpp>
#include <azh/core/demo.hpp>
#include <mutex>
#include <thread>

void test01()
{
    azh::sdk::core::_base *b = new azh::sdk::core::demo;
    azh::sdk::utils::logger::getInstance()->debug(b->className());

    delete b;

    azh::sdk::utils::logger::getInstance()->debug("_base init");
}

int main(int argc,char** argv)
{
    // azh::sdk::utils::arguments args(argc,argv);
    // args.addArg("h","help");
    // args.addArg("s","source");
    // aDebug()<<args.get("h").as<std::string>();
    // aDebug()<<args;
    azh::sdk::type::vector<std::string> v={"1","2","3"};
    // aDebug()<<args.keys();

    // aDebug()<<args.usage();
    aDebug() << "test for core.";
    azh::sdk::utils::logger::getInstance()->fatal("Main Thread Start!");

    constexpr int n = 3;
    std::thread *childThreads[n];

    for (int i = 0; i < n; i++)
    {
        childThreads[i] = new std::thread(test01);
    }

    for (int i = 0; i < n; i++)
    {
        childThreads[i]->join();
        delete childThreads[i];
    }

    aDebug()<<"----------------";

    azh::sdk::type::b_tree<azh::sdk::utils::unique_time_stamp,std::string,5> b_tree;

    b_tree.insert(azh::sdk::utils::unique_time_stamp(),"1");
    b_tree.insert(azh::sdk::utils::unique_time_stamp(),"2");
    b_tree.insert(azh::sdk::utils::unique_time_stamp(),"3");
    b_tree.insert(azh::sdk::utils::unique_time_stamp(),"4");
    b_tree.insert(azh::sdk::utils::unique_time_stamp(),"5");
    b_tree.insert(azh::sdk::utils::unique_time_stamp(),"6");
    b_tree.insert(azh::sdk::utils::unique_time_stamp(),"7");
    b_tree.insert(azh::sdk::utils::unique_time_stamp(),"8");
    b_tree.insert(azh::sdk::utils::unique_time_stamp(),"9");
    b_tree.insert(azh::sdk::utils::unique_time_stamp(),"10");
    b_tree.insert(azh::sdk::utils::unique_time_stamp(),"11");
    aDebug()<<b_tree;

    azh::sdk::utils::logger::getInstance()->fatal("Main Thread End!");

    aDebug()<<azh::sdk::utils::file_reader::readAll("test-20260104_211453.log");
    aDebug()<<azh::sdk::utils::file_reader::write("hello.txt","nihao\nhi~");
    aDebug()<<azh::sdk::utils::file_reader::readAll("hello.txt");

    return 0;
}