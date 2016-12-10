#ifndef KE_HTTPTHREAD_HPP
#define KE_HTTPTHREAD_HPP

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Http.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>

namespace ke
{

class HttpThread
{
    public:
        HttpThread(const std::string& url, const std::string& body);
        ~HttpThread();

        void run();
	
		int getStatus() const;
		bool isFinished() const;
		std::string getBody() const;

        static void splitUrl(const std::string& longurl, std::string& url, std::string& uri);
		static bool sendRequest(const std::string& url, const std::string& body, std::string* response = nullptr);

    private:
        sf::Thread mThread;
		sf::Mutex mMutex;
		int mStatus;
		sf::Http mHttp;
		sf::Http::Request mRequest;
		sf::Http::Response mResponse;
};

} // namespace ke

#endif // KE_HTTPTHREAD_HPP
