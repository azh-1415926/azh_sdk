#pragma once

#include <fstream>

namespace azh::sdk::utils
{
    class file_reader
    {
    public:
        static std::string readAll(const std::string &filepath)
        {
            std::ifstream in(filepath,std::ios::in);
            std::string str;

            char buffer[256];
            if (!in.is_open())
            {
                return "";
            }
            while (!in.eof())
            {
                in.getline(buffer, 100);
                str+=buffer;
                str+="\n";
            }

            in.close();

            return str;
        }

        static bool write(const std::string &filepath,const std::string& content)
        {
            std::ofstream out(filepath,std::ios::out);
            std::string str;

            if (!out.is_open())
            {
                return false;
            }
            
            out<<content;
            out.close();

            return true;
        }
    };
};