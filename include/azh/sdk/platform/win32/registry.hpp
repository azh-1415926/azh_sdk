#pragma once

#include <string>

#ifdef _WIN32
#include <Windows.h>

namespace azh::sdk::platform::win32
{
	class registry
	{
	public:
		enum class type
		{
			HKey_Local_Machine,
			HKey_Classes_Root,
			HKey_Current_User,
			HKey_Users,
			HKey_Current_Config
		};

		static std::string valueOfHKLM(const std::string& registry_path,const std::string& key) { return value(type::HKey_Local_Machine,registry_path,key); }
		static std::string valueOfHKCR(const std::string& registry_path,const std::string& key) { return value(type::HKey_Classes_Root,registry_path,key); }
		static std::string valueOfHKCU(const std::string& registry_path,const std::string& key) { return value(type::HKey_Current_User,registry_path,key); }
		static std::string valueOfHKU(const std::string& registry_path,const std::string& key) { return value(type::HKey_Users,registry_path,key); }
		static std::string valueOfHKCC(const std::string& registry_path,const std::string& key) { return value(type::HKey_Current_Config,registry_path,key); }

		static std::string value(const type& hkey,const std::string& registry_path,const std::string& key)
		{
			HKEY hkey_ret = NULL;

			/* registry_path is invaild */

			switch (hkey)
			{
			case type::HKey_Local_Machine:
				if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE,registry_path.c_str(),0,KEY_READ,&hkey_ret))
				{
					return false;
				}
				break;

			case type::HKey_Classes_Root:
				if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_CLASSES_ROOT,registry_path.c_str(),0,KEY_READ,&hkey_ret))
				{
					return false;
				}
				break;

			case type::HKey_Current_User:
				if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_CURRENT_USER,registry_path.c_str(),0,KEY_READ,&hkey_ret))
				{
					return false;
				}
				break;

			case type::HKey_Users:
				if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_USERS,registry_path.c_str(),0,KEY_READ,&hkey_ret))
				{
					return false;
				}
				break;

			case type::HKey_Current_Config:
				if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_CURRENT_CONFIG,registry_path.c_str(),0,KEY_READ,&hkey_ret))
				{
					return false;
				}
				break;
			
			default:
				return false;
				break;
			}

			char value[256];
			DWORD type;
			DWORD size;
			if (ERROR_SUCCESS != RegQueryValueEx(
									 hkey_ret,
									 key.c_str(), // key
									 0,
									 &type,
									 (LPBYTE)&value,
									 &size
									 ))
			{
				return false;
			}

			return value;
		}
	};
}

#endif