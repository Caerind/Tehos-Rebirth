#ifndef KE_LOG_HPP
#define KE_LOG_HPP

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <SFML/Network/Http.hpp>
#include <SFML/Network/IpAddress.hpp>

namespace ke
{
	
class Log
{
    public:
        enum Type
        {
            Info,
            Warning,
            Error
        };

		static Log& instance()
		{
			static Log* log = new Log();
			return *log;
		}
		
		static void useConsole(bool use)
		{ 
			instance().mUseConsole = use; 
		}
		
		static void useFile(bool use, const std::string& filename = "output.log")
		{
			instance().mUseFile = false;
			instance().mFile.close();
			if (use && filename != "")
			{
				instance().mFile.open(filename);
				if (instance().mFile.is_open())
				{
					instance().mUseFile = true;
				}
				else
				{
					log("Can't open log file ! (" + filename + ")", Type::Warning);
				}
			}
		}
		
		static void useOnline(bool use, const std::string& url = "")
		{
			if (use && url != "")
			{
				instance().mUseOnline = true;
				instance().mUrl = url;
			}
			else
			{
				instance().mUseOnline = false;
				instance().mUrl = "";
			}
		}

		static void setOnlineData(const std::string& id, const std::string& data)
		{
			instance().mOnlineData[id] = data;
		}
		
        static void log(const std::string& message, Type type = Type::Info)
        {
			// Get date
			std::time_t t = std::time(nullptr);
			std::tm time;
			#ifdef _MSC_VER
				localtime_s(&time, &t);
			#else
				time = *localtime(&t);
			#endif
			mktime(&time);
			std::ostringstream dateStream;
			dateStream << std::put_time(&time, "%d-%m-%Y %H-%M-%S");
			
			// Get type
			std::string typeString;
            switch (type)
            {
				case Type::Error:
					typeString = "ERROR";
					break;
				case Type::Warning:
					typeString = "WARNING";
					break;
				case Type::Info:
				default:
					typeString = "INFO";
					break;
            }
			
			// Console
			if (instance().mUseConsole)
			{
				std::string consoleString = "[" + typeString + "]: " + message; 
				if (type == Type::Error)
				{
					std::cerr << consoleString << std::endl; 
				}
				else
				{
					std::cout << consoleString << std::endl;
				}
			}
			
			// File
			if (instance().mUseFile && instance().mFile.is_open())
			{
				instance().mFile << "[" + dateStream.str() + "][" + typeString + "]: " + message << std::endl;
			}
			
			// Online
			if (instance().mUseOnline && instance().mUrl != "")
			{
				std::ostringstream onlineStream;
				onlineStream << "action=insert";
				onlineStream << "&type=" << typeString;
				onlineStream << "&address=" << sf::IpAddress::getPublicAddress().toString();
				for (auto itr = instance().mOnlineData.begin(); itr != instance().mOnlineData.end(); itr++)
				{
					onlineStream << "&" << itr->first << "=" << itr->second;
				}
				onlineStream << "&content=" << message;
				
				std::string url, uri;
				std::string temp = instance().mUrl;
				if (instance().mUrl.find("http://") != std::string::npos)
				{
					temp = temp.substr(7);
					std::size_t found = temp.find_first_of("/");
					if (found != std::string::npos)
					{
						url = "http://" + temp.substr(0, found + 1);
						uri = temp.substr(found + 1);
					}
					else
					{
						url = instance().mUrl;
						uri = "";
					}
				}
				else
				{
					url = instance().mUrl;
					uri = "";
				}

				sf::Http(url).sendRequest(sf::Http::Request(uri, sf::Http::Request::Post, onlineStream.str()));
			}
			
			instance().mType = Type::Info;
		}
		
		friend Log& operator<<(Log& log, const std::string& message)
		{
			Log::log(message, instance().mType);
			return log;
		}

		friend Log& operator<<(Log& log, Type type)
		{
			instance().mType = type;
			return log;
		}
		
		static void info(const std::string& message)
		{
			log(message, Type::Info);
		}

		static void warning(const std::string& message)
		{
			log(message, Type::Warning);
		}

		static void error(const std::string& message)
		{
			log(message, Type::Error);
		}
		
	private:
		Log() : mType(Type::Info), mUseConsole(true), mUseFile(false), mUseOnline(false), mFile(), mUrl("")
		{
		}
			
			
	private:
		Type mType;
		bool mUseConsole;
		bool mUseFile;
		bool mUseOnline;
		std::ofstream mFile;
		std::string mUrl;
		std::unordered_map<std::string, std::string> mOnlineData;
			
};

} // namespace ke

#endif // KE_LOG_HPP