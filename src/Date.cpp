#include "Date.hpp"
#include <iostream>
#include <sstream>
#include <cstring>
#include <cmath>
using namespace std;

Date::Date():m_year(0),m_month(0),m_day(0),m_hour(0),m_minute(0) {}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute):
m_year(t_year), m_month(t_month), m_day(t_day), m_hour(t_hour), m_minute(t_minute) {}

Date::Date(string dateString) {
	Date date;
	date = date.stringToDate(dateString);
	m_year = date.getYear();
	m_month = date.getMonth();
	m_day = date.getDay();
	m_hour = date.getHour();
	m_minute = date.getMinute();
}
int Date::getYear(void) const { return m_year;}

void Date::setYear(const int t_year) { m_year = t_year;}

int Date::getMonth(void) const { return m_month;}

void Date::setMonth(const int t_month) { m_month = t_month;}

int Date::getDay(void) const { return m_day;}

void Date::setDay(const int t_day) { m_day = t_day;}

int Date::getHour(void) const { return m_hour;}

void Date::setHour(const int t_hour) { m_hour = t_hour;}

int Date::getMinute(void) const { return m_minute;}

void Date::setMinute(const int t_minute) { m_minute = t_minute;}

int daysOfMonth(int month, int year) {
	if (month % 2 == 1 && month < 8) return 31;
	if (month % 2 == 0 && month >= 8) return 31;
	if (month == 4 || month == 6 || month == 9 || month == 11)
		return 30;
	if (month == 2) {
		if (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0))
			return 29;
		return 28;
	}
}

bool Date::isValid(const Date t_date) {
	if (t_date.getYear() < 1000 || t_date.getYear() > 9999) return false;
	if (t_date.getMonth() <= 0 || t_date.getMonth() > 12) return false;
	if (t_date.getDay() <= 0 || t_date.getDay() > 31) return false;
	if (t_date.getHour() < 0 || t_date.getHour() >= 24) return false;
	if (t_date.getMinute() < 0 || t_date.getMinute() >= 60) return false;
	int day = daysOfMonth(t_date.getMonth(), t_date.getYear());
	int month = t_date.getMonth();
	if (month == 4 || month == 6 || month == 9 || month == 11) {
		if (t_date.getDay() > 30) return false;
	}
	if (month == 2) {
		if (t_date.getYear() % 400 == 0 || (t_date.getYear() % 100 != 0 && t_date.getYear() % 4 == 0)) {
			if (t_date.getDay() > 29)
				return false;
		} else {
			if (t_date.getDay() > 28)
				return false;
		}
	}
	return true;
}

bool validCh(char ch) {
	if (ch <= '9' && ch >= '0') return true;
	else return false;
}
bool isValidString(string str) {
	if (!validCh(str[0])||!validCh(str[1])||!validCh(str[2])||!validCh(str[3])) return false;
	if (str[4] != '-') return false;
	if (!validCh(str[5])||!validCh(str[6])||!validCh(str[8])||!validCh(str[9])) return false;
	if (str[7] != '-') return false;
	if (!validCh(str[11])||!validCh(str[12])||!validCh(str[14])||!validCh(str[15])) return false;
	if (str[10] != '/') return false;
	if (str[13] != ':') return false;
	return true;
}

Date Date::stringToDate(const string t_dateString) {
	if (t_dateString.length() != 16) return Date();
	if (!isValidString(t_dateString)) return Date();
	int t_year = 0, t_month = 0, t_day = 0, t_hour = 0, t_minute = 0;
	char ch_year[4], ch_month[2], ch_day[2], ch_hour[2], ch_minute[2];
	int i_year[4], i_month[2], i_day[2], i_hour[2], i_minute[2];
	for (int i = 0; i < 4; ++i) {
		ch_year[i] = t_dateString[i];
		i_year[i] = ch_year[i] - '0';
	}
	for (int i = 5; i < 7; ++i) {
		ch_month[i - 5] = t_dateString[i];
		i_month[i - 5] = ch_month[i - 5] - '0';
	}
	for (int i = 8; i < 10; ++i) {
		ch_day[i - 8] = t_dateString[i];
		i_day[i - 8] = ch_day[i - 8] - '0';
	}
	for (int i = 11; i < 13; ++i) {
		ch_hour[i - 11] = t_dateString[i];
		i_hour[i - 11] = ch_hour[i - 11] - '0';
	}
	for (int i = 14; i < 16; ++i) {
		ch_minute[i - 14] = t_dateString[i];
		i_minute[i - 14] = ch_minute[i - 14] - '0';
	}
	for (int i = 0; i < 4; ++i) t_year += i_year[i]*pow(10, 3 - i);
	for (int i = 0; i < 2; ++i) t_month += i_month[i]*pow(10, 1 - i);
	for (int i = 0; i < 2; ++i) t_day += i_day[i]*pow(10, 1 - i);
	for (int i = 0; i < 2; ++i) t_hour += i_hour[i]*pow(10, 1 - i);
	for (int i = 0; i < 2; ++i) t_minute += i_minute[i]*pow(10, 1 - i);
	Date temp = Date(t_year, t_month, t_day, t_hour, t_minute);
    return temp;
}
string Date::dateToString(Date t_date) {
	if (!isValid(t_date)) return "0000-00-00/00:00";
	stringstream ss_year, ss_month, ss_day, ss_hour, ss_minute;
	string result = "";
	ss_year << t_date.getYear();
	result = result + ss_year.str() + "-";
	ss_month << t_date.getMonth();
	if (t_date.getMonth() < 10) result = result + "0" + ss_month.str() + "-";
	else result = result + ss_month.str() + "-";
	ss_day << t_date.getDay();
	if (t_date.getDay() < 10) result = result + "0" + ss_day.str() + "/";
	else result = result + ss_day.str() + "/";
	ss_hour << t_date.getHour();
	if (t_date.getHour() < 10) result = result + "0" + ss_hour.str() + ":";
	else result = result + ss_hour.str() + ":";
	ss_minute << t_date.getMinute();
	if (t_date.getMinute() < 10) result = result + "0" + ss_minute.str();
	else result = result + ss_minute.str();
	return result; 
}
Date& Date::operator=(const Date& t_date) {
	m_year = t_date.getYear();
	m_month = t_date.getMonth();
	m_day = t_date.getDay();
	m_hour = t_date.getHour();
	m_minute = t_date.getMinute();
	return *this;
}
bool Date::operator==(const Date& t_date) const {
	if (m_year != t_date.getYear()) return false;
	if (m_month != t_date.getMonth()) return false;
	if (m_day != t_date.getDay()) return false;
	if (m_hour != t_date.getHour()) return false;
	if (m_minute != t_date.getMinute()) return false;
	return true;
}
bool Date::operator>(const Date& t_date) const {
	if (m_year > t_date.getYear()) return true;
	if (m_year < t_date.getYear()) return false;
	if (m_month > t_date.getMonth()) return true;
	if (m_month < t_date.getMonth()) return false;
	if (m_day > t_date.getDay()) return true;
	if (m_day < t_date.getDay()) return false;
	if (m_hour > t_date.getHour()) return true;
	if (m_hour < t_date.getHour()) return false;
	if (m_minute > t_date.getMinute()) {
		return true;
	} else {
	return false;
	}
}
bool Date::operator<(const Date& t_date) const {
	return !(*this >= t_date);
}
bool Date::operator>=(const Date& t_date) const {
	return *this > t_date || *this == t_date;
}
bool Date::operator<=(const Date& t_date) const {
	return *this < t_date || *this == t_date;
}