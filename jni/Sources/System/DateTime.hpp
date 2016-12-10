#ifndef KE_DATETIME_HPP
#define KE_DATETIME_HPP

#include <ctime>
#include <string>
#include <iomanip>
#include <sstream>

#include <SFML/System/Time.hpp>

#include "../Config.hpp"

namespace ke
{

class DateTime
{
	public:
		DateTime();

		DateTime(const std::string& str, const std::string& format = "%d-%m-%Y %H-%M-%S");

		int getYear() const;
		void setYear(int year);

		int getMonth() const;
		void setMonth(int month);

		int getDay() const;
		void setDay(int day);

		int getHours() const;
		void setHours(int hours);

		int getMinutes() const;
		void setMinutes(int minutes);

		int getSeconds() const;
		void setSeconds(int seconds);

		std::string toString(const std::string& format = "%d-%m-%Y %H-%M-%S");

		void fromString(const std::string& str, const std::string& format = "%d-%m-%Y %H-%M-%S");

		bool passed();

		void update();

	private:
		std::tm mTime;
};


inline bool operator==(const DateTime& left, const DateTime& right)
{
	return (left.getYear() == right.getYear()
		&& left.getMonth() == right.getMonth()
		&& left.getDay() == right.getDay()
		&& left.getHours() == right.getHours()
		&& left.getMinutes() == right.getMinutes()
		&& left.getSeconds() == right.getSeconds());
}

inline bool operator!=(const DateTime& left, const DateTime& right)
{
	return !(left == right);
}

inline bool operator<(const DateTime& left, const DateTime& right)
{
	// Year
	if (left.getYear() < right.getYear())
		return true;
	else if (left.getYear() > right.getYear())
		return false;

	// Month
	if (left.getMonth() < right.getMonth())
		return true;
	else if (left.getMonth() > right.getMonth())
		return false;

	// Day
	if (left.getDay() < right.getDay())
		return true;
	else if (left.getDay() > right.getDay())
		return false;

	// Hours
	if (left.getHours() < right.getHours())
		return true;
	else if (left.getHours() > right.getHours())
		return false;

	// Minutes
	if (left.getMinutes() < right.getMinutes())
		return true;
	else if (left.getMinutes() > right.getMinutes())
		return false;

	// Seconds
	if (left.getSeconds() < right.getSeconds())
		return true;
	else if (left.getSeconds() > right.getSeconds())
		return false;

	return false;
}

inline bool operator>(const DateTime& left, const DateTime& right)
{
	return (left != right && !(left < right));
}

inline bool operator>=(const DateTime& left, const DateTime& right)
{
	return !(left < right);
}

inline bool operator<=(const DateTime& left, const DateTime& right)
{
	return !(left > right);
}

} // namespace ke

#endif // KE_DATETIME_HPP
