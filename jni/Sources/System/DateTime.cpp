#include "DateTime.hpp"

namespace ke
{

DateTime::DateTime()
{
	std::time_t t = std::time(nullptr);
	#ifdef KEENGINE_DESKTOP
		localtime_s(&mTime, &t);
	#else
		mTime = *localtime(&t);
	#endif
	update();
}

DateTime::DateTime(const std::string& str, const std::string& format)
{
	fromString(str, format);
}

int DateTime::getYear() const
{
	return mTime.tm_year + 1900;
}

void DateTime::setYear(int year)
{
	mTime.tm_year = year - 1900;
	update();
}

int DateTime::getMonth() const
{
	return mTime.tm_mon + 1;
}

void DateTime::setMonth(int month)
{
	mTime.tm_mon = month - 1;
	update();
}

int DateTime::getDay() const
{
	return mTime.tm_mday;
}

void DateTime::setDay(int day)
{
	mTime.tm_mday = day;
	update();
}

int DateTime::getHours() const
{
	return mTime.tm_hour;
}

void DateTime::setHours(int hours)
{
	mTime.tm_hour = hours;
	update();
}

int DateTime::getMinutes() const
{
	return mTime.tm_min;
}

void DateTime::setMinutes(int minutes)
{
	mTime.tm_min = minutes;
	update();
}

int DateTime::getSeconds() const
{
	return mTime.tm_sec;
}

void DateTime::setSeconds(int seconds)
{
	mTime.tm_sec = seconds;
	update();
}

std::string DateTime::toString(const std::string& format)
{
	std::ostringstream oss;
	oss << std::put_time(&mTime, format.c_str());
	return oss.str();
}

void DateTime::fromString(const std::string& str, const std::string& format)
{
	std::istringstream iss(str);
	iss >> std::get_time(&mTime, format.c_str());
	update();
}

bool DateTime::passed()
{
	DateTime c;
	// Year
	if (getYear() < c.getYear())
		return true;
	else if (getYear() > c.getYear())
		return false;
	// Month
	if (getMonth() < c.getMonth())
		return true;
	else if (getMonth() > c.getMonth())
		return false;
	// Day
	if (getDay() < c.getDay())
		return true;
	else if (getDay() > c.getDay())
		return false;
	// Hours
	if (getHours() < c.getHours())
		return true;
	else if (getHours() > c.getHours())
		return false;
	// Minutes
	if (getMinutes() < c.getMinutes())
		return true;
	else if (getMinutes() > c.getMinutes())
		return false;
	// Seconds
	if (getSeconds() < c.getSeconds())
		return true;
	else if (getSeconds() > c.getSeconds())
		return false;
	return false;
}

void DateTime::update()
{
	mktime(&mTime);
}

} // namespace ke