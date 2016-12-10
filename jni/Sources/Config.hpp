#ifndef KE_CONFIG_HPP
#define KE_CONFIG_HPP

#include <sstream>

#include <SFML/Config.hpp>

#ifndef SFML_STATIC
	#ifdef SFML_SYSTEM_WINDOWS
		#ifdef KEENGINE_EXPORTS // Windows compilers need specific (and different) keywords for export and import
			#define KEENGINE_API __declspec(dllexport)
		#else
			#define KEENGINE_API __declspec(dllimport)
		#endif
	#else // Linux, FreeBSD, Mac OS X
		#if __GNUC__ >= 4 // GCC 4 has special keywords for showing/hidding symbols
			#define KEENGINE_API __attribute__((__visibility__("default")))
		#else
			#define KEENGINE_API
		#endif
	#endif
#else
	#define KEENGINE_API
#endif

#ifdef SFML_SYSTEM_ANDROID
	#define KEENGINE_MOBILE
	#define KEENGINE_ANDROID
#elif defined SFML_SYSTEM_IOS
	#define KEENGINE_MOBILE
	#define KEENGINE_MACOS
#else
	#define KEENGINE_DESKTOP
	#ifdef SFML_SYSTEM_WINDOWS
		#define KEENGINE_WINDOWS
	#elif defined SFML_SYSTEM_MACOS
		#define KEENGINE_MACOS
	#elif defined SFML_SYSTEM_LINUX
		#define KEENGINE_LINUX
	#elif defined SFML_SYSTEM_FREEBSD
		#define KEENGINE_FREEBSD
	#endif
#endif

#define KEENGINE
#define KEENGINE_VERSION_MAJOR 0
#define KEENGINE_VERSION_MINOR 1

namespace ke
{
	inline bool isMobile()
	{
		#ifdef KEENGINE_MOBILE
		return true;
		#else
		return false;
		#endif
	}

	inline bool isAndroid()
	{
		#ifdef KEENGINE_ANDROID
		return true;
		#else
		return false;
		#endif		
	}

	inline bool isIOS()
	{
		#ifdef KEENGINE_IOS
		return true;
		#else
		return false;
		#endif		
	}

	inline bool isDesktop()
	{
		#ifdef KEENGINE_DESKTOP
		return true;
		#else
		return false;
		#endif
	}

	inline bool isWindows()
	{
		#ifdef KEENGINE_WINDOWS
		return true;
		#else
		return false;
		#endif	
	}

	inline bool isMacOS()
	{
		#ifdef KEENGINE_MACOS
		return true;
		#else
		return false;
		#endif	
	}

	inline bool isLinux()
	{
		#ifdef KEENGINE_LINUX
		return true;
		#else
		return false;
		#endif	
	}

	inline bool isFreeBSD()
	{
		#ifdef KEENGINE_FREEBSD
		return true;
		#else
		return false;
		#endif	
	}

	inline std::string getPlatformName()
	{
		if (isAndroid())
		{
			return "Android";
		}
		else if (isIOS())
		{
			return "IOS";
		}
		else if (isWindows())
		{
			return "Windows";
		}
		else if (isMacOS())
		{
			return "MacOS";
		}
		else if (isLinux())
		{
			return "Linux";
		}
		else if (isFreeBSD())
		{
			return "FreeBSD";
		}
		return "Unknown";
	}

	inline std::string getVersionName()
	{
		std::ostringstream oss;
		oss << KEENGINE_VERSION_MAJOR << "." << KEENGINE_VERSION_MINOR;
		return oss.str();
	}
}

#endif // KE_CONFIG_HPP
