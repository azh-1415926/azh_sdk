#include "azh/platform/win32/registry.hpp"

#include "azh/utils/command.hpp"
#include "azh/utils/logger.hpp"
#include "azh/utils/string.hpp"

using astring=azh::sdk::utils::string;

int main()
{
    #ifdef _WIN32
    std::string vs_registry_path="SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\devenv.exe";
    std::string value=azh::sdk::platform::win32::registry::valueOfHKLM(vs_registry_path,"");
    azh::sdk::utils::logger::getInstance()->debug("devenv.exe : ",value);

    std::string vs_root=astring::subtract(value,"\\common7\\ide\\devenv.exe\"");

    azh::sdk::utils::logger::getInstance()->debug("vs_root : ",vs_root);
    std::string vcvarsall_bat_path=vs_root+"\\VC\\Auxiliary\\Build\\vcvarsall.bat\"";

    azh::sdk::utils::logger::getInstance()->debug("vcvarsall_bat_path : ",vcvarsall_bat_path);
    azh::sdk::utils::command vs_env(vcvarsall_bat_path,true);
    vs_env<<"x64";
    vs_env.run();
    #endif

    return 0;
}