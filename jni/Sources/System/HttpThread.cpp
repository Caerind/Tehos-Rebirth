#include "HttpThread.hpp"

namespace ke
{

HttpThread::HttpThread(const std::string& url, const std::string& request)
: mThread(&HttpThread::run, this)
, mMutex()
, mStatus(0)
, mHttp()
, mRequest()
, mResponse()
{
    std::string tempUrl, tempUri;
    splitUrl(url, tempUrl, tempUri);
    mHttp.setHost(tempUrl);
    mRequest.setMethod(sf::Http::Request::Post);
    mRequest.setBody(request);
    mRequest.setUri(tempUri);
    mThread.launch();
}

HttpThread::~HttpThread()
{
    mThread.wait();
}

void HttpThread::run()
{
    mMutex.lock();
    mResponse = mHttp.sendRequest(mRequest);
    if (mResponse.getStatus() == sf::Http::Response::Ok)
    {
        mStatus = 1;
    }
    else
    {
        mStatus = -1;
    }
    mMutex.unlock();
}

int HttpThread::getStatus() const
{
    return mStatus;
}

bool HttpThread::isFinished() const
{
   return mStatus != 0;
}
	
std::string HttpThread::getBody() const
{
    if (isFinished())
    {
        return mResponse.getBody();    
    }
    else
    {
	return "";    
    }
}	

void HttpThread::splitUrl(const std::string& longurl, std::string& url, std::string& uri)
{
	std::string temp = longurl;
	if (longurl.find("http://") != std::string::npos)
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
			url = longurl;
			uri = "";
		}
	}
	else
	{
		url = longurl;
		uri = "";
	}
}

bool HttpThread::sendRequest(const std::string& url, const std::string& body, std::string* response)
{
	std::string tempUrl, tempUri;
	splitUrl(url, tempUrl, tempUri);
	sf::Http::Response rep = sf::Http(tempUrl).sendRequest(sf::Http::Request(tempUri, sf::Http::Request::Post, body));
	bool ret = rep.getStatus() == sf::Http::Response::Ok;
	if (response != nullptr && ret)
	{
		*response = rep.getBody();
	}
	return ret;
}

} // namespace ke
