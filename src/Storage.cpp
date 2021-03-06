#include <sstream>
#include <iostream>
#include <functional>
#include <fstream>
#include <list>
#include <memory>
#include <cstring>
#include <string.h>
#include "Storage.hpp"
#include "Path.hpp"
using namespace std;

shared_ptr<Storage> Storage::m_instance = nullptr;
Storage::Storage() {
	m_dirty = false;
	readFromFile();
}
Storage::~Storage() {
	sync();
}
bool Storage::readFromFile(void) {
	string str;
	ifstream infile(Path::userPath);
	if (!infile.is_open()) return false;
	while (getline(infile, str)) {
		if (str.length() == 0) break;
		string t_name = "", t_password = "", t_email = "", t_phone = "";
		int len = str.length();
		int flag = 1;
		for (int i = 1; i < len; ++i) {
			if (flag == 1) t_name += str[i];
			if (flag == 2) t_password += str[i];
			if (flag == 3) t_email += str[i];
			if (flag == 4) t_phone += str[i];
			if (str[i + 1] == '\"') {
				i += 3;
				flag++;
			}
		}
		if (t_name != "" && t_password != "" && t_email != "" && t_phone != "")
		m_userList.push_back(User(t_name, t_password, t_email, t_phone));
	}
	infile.close();
	ifstream ifile(Path::meetingPath);
	if (!ifile.is_open()) return false;
	while (getline(ifile, str)) {
		if (str.length() == 0) break;
		int len = str.length();
		int flag = 1;
		string t_sponsor = "", t_title = "", t_start = "", t_end = "", person = "";
		vector<string> t_participator;
		for (int i = 1; i < len; ++i) {
			if (flag == 1) t_sponsor += str[i];
			if (flag == 2) {
				if (str[i] != '\"') {
					person += str[i];
					if (str[i + 1] == '&' || str[i + 1] == '\"') {
						string s_person(person);
						t_participator.push_back(s_person);
						person = "";
						i++;
					}
				}
				if (str[i] == '\"') i--;
			}
			if (flag == 3) t_start += str[i];
			if (flag == 4) t_end += str[i];
			if (flag == 5) t_title += str[i];
			if (str[i + 1] == '\"') {
				flag++;
				i += 3;
			}
		}
		if (t_sponsor != "" && (t_participator.size() != 0) && t_end != "" && t_start != "" && t_title != "")
		m_meetingList.push_back(Meeting(t_sponsor, t_participator,
			Date::stringToDate(t_start), Date::stringToDate(t_end), t_title));
	}
	ifile.close();
	return true;
}
bool Storage::writeToFile(void) {
	if (!m_dirty) return false;
	ofstream outfile(Path::userPath);
	if (!outfile.is_open()) return false;
	if (!m_userList.empty()) {
	list<User>::iterator u_iter = m_userList.begin();
	while (u_iter != m_userList.end()) {
		stringstream ss;
		ss << '\"' << (*u_iter).getName() << "\",\"" << (*u_iter).getPassword() << "\",\""
		<< (*u_iter).getEmail() << "\",\"" << (*u_iter).getPhone() << "\"";
		outfile << ss.str() << endl;
		++u_iter;
	}
	}
	outfile.close();
	ofstream ofile(Path::meetingPath);
	if (!ofile.is_open()) return false;
	list<Meeting>::iterator m_iter = m_meetingList.begin();
	while (m_iter != m_meetingList.end()) {
		stringstream mss;
		mss << "\"" << (*m_iter).getSponsor() << "\",\"";
		vector<string> temp = (*m_iter).getParticipator();
		if (!temp.empty()) {
		vector<string>::iterator pt = temp.begin();
		mss << *pt;
		pt++;
		while (pt != temp.end()) {
			mss << "&" << *pt;
			pt++;
		}
		}
		Date start = (*m_iter).getStartDate();
		Date end = (*m_iter).getEndDate();
		mss << "\",\"" << Date::dateToString(start) << "\",\"" << Date::dateToString(end)
		<< "\",\"" << (*m_iter).getTitle() << "\"";
		ofile << mss.str() << endl;
		++m_iter;
	}
	ofile.close();
	m_dirty = false;
	return true;
}
shared_ptr<Storage> Storage::getInstance(void) {
	if (m_instance == NULL) m_instance = (std::shared_ptr<Storage>)(new Storage());
	return m_instance;
}
void Storage::createUser(const User & t_user) {
	m_userList.push_back(t_user);
	m_dirty = true;
}
list<User> Storage::queryUser(function<bool(const User &)> filter) const {
	list<User> t_userList;
	auto iter = m_userList.begin();
	while (iter != m_userList.end()) {
		if (filter(*iter))
			t_userList.push_back(*iter);
		++iter;
	}
	return t_userList;
}
int Storage::updateUser(function<bool(const User &)> filter, function<void(User &)> switcher) {
	int t_userCount = 0;
	auto iter = m_userList.begin();
	while (iter != m_userList.end()) {
		if (filter(*iter)) {
			++t_userCount;
			switcher(*iter);
		}
		++iter;
	}
	if (t_userCount) m_dirty = true;
	return t_userCount;
}
int Storage::deleteUser(function<bool(const User &)> filter) {
	int t_userCount = 0;
	auto iter = m_userList.begin();
	while (iter != m_userList.end()) {
		if (filter(*iter)) {
			iter = m_userList.erase(iter);
			++t_userCount;
		} else {
			++iter;
		}
	}
	if (t_userCount != 0) m_dirty = true;
	return t_userCount;
}
void Storage::createMeeting(const Meeting & t_meeting) {
	m_meetingList.push_back(t_meeting);
	m_dirty = true;
}
list<Meeting> Storage::queryMeeting(function<bool(const Meeting &)> filter) const {
	list<Meeting> t_meetingList;
	auto iter = m_meetingList.begin();
	while (iter != m_meetingList.end()) {
		if (filter(*iter))
			t_meetingList.push_back(*iter);
		++iter;
	}
	return t_meetingList;
}
int Storage::updateMeeting(function<bool(const Meeting &)> filter, function<void(Meeting &)> switcher) {
	int t_meetingCount = 0;
	auto iter = m_meetingList.begin();
	while (iter != m_meetingList.end()) {
		if (filter(*iter)) {
			++t_meetingCount;
			switcher(*iter);
		}
		++iter;
	}
	if (t_meetingCount != 0) m_dirty = true;
	return t_meetingCount;
}
int Storage::deleteMeeting(function<bool(const Meeting &)> filter) {
	int t_meetingCount = 0;
	list<Meeting>::iterator iter = m_meetingList.begin();
	while (iter != m_meetingList.end()) {
		if (filter(*iter)) {
			++t_meetingCount;
			iter = m_meetingList.erase(iter);
		} else {
			iter++;
		}
	}
	if (t_meetingCount != 0) m_dirty = true;
	return t_meetingCount;
}
bool Storage::sync(void) {
	return writeToFile();
}