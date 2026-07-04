#include <azh/sdk/type/type.hpp>
#include <azh/sdk/utils/logger.hpp>
#include <azh/sdk/utils/unique_time_stamp.hpp>
#include <azh/sdk/utils/file_reader.hpp>
#include <azh/sdk/core/ptr.hpp>
#include <mutex>
#include <thread>

int main(int argc,char** argv)
{
    azh::sdk::type::vector<std::string> v={"1","2","3"};
    azh::sdk::utils::logger::getInstance()->fatal("Main Thread Start!");

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

    int* test=new int;;
    *test=1;
    azh::sdk::core::ptr<int> p;
    azh::sdk::core::ptr<int> p1;
    p.reset(test);
    p1.reset(test);
    aDebug()<<*p;
    *test=2;
    aDebug()<<*p1;

    return 0;
}