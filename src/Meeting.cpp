#include <iostream>
#include <vector>
#include "Meeting.hpp"
using namespace std;

Meeting::Meeting(string t_sponsor, vector<string> t_participator,
	Date t_startDate, Date t_endDate, string t_title) {
	m_sponsor = t_sponsor;
	m_startDate = t_startDate;
	m_endDate = t_endDate;
	m_title = t_title;
	m_participators.clear();
	vector<string>::iterator it = t_participator.begin();
	for (; it != t_participator.end(); ++it)
		m_participators.push_back(*it);
}
Meeting::Meeting(const Meeting& t_meeting) {
	m_sponsor = t_meeting.getSponsor();
	m_startDate = t_meeting.getStartDate();
	m_endDate = t_meeting.getEndDate();
	m_title = t_meeting.getTitle();
	while (!m_participators.empty()) {
		m_participators.pop_back();
	}
	vector<string> temp = t_meeting.getParticipator();
	vector<string>::iterator it = temp.begin();
	while (it != temp.end()) {
		m_participators.push_back(*it);
		++it;
	}
}  //  does it need to clear the initial data in m_participator?
//  not very sure
string Meeting::getSponsor(void) const {
	return m_sponsor;
}
void Meeting::setSponsor(const string t_sponsor) {
	m_sponsor = t_sponsor;
}
vector<string> Meeting::getParticipator(void) const {
	return m_participators;
}
void Meeting::setParticipator(vector<string> t_participator) {
	while (!m_participators.empty()) {
		m_participators.pop_back();
	}
	vector<string>::iterator it = t_participator.begin();
	while (it != t_participator.end()) {
		m_participators.push_back(*it);
		++it;
	}
}
Date Meeting::getStartDate(void) const {
	return m_startDate;
}
void Meeting::setStartDate(const Date t_startDate) {
	m_startDate = t_startDate;
}
Date Meeting::getEndDate(void) const {
	return m_endDate;
}
void Meeting::setEndDate(const Date t_endDate) {
	m_endDate = t_endDate;
}
string Meeting::getTitle(void) const {
	return m_title;
}
void Meeting::setTitle(string t_title) {
	m_title = t_title;
}
bool Meeting::isParticipator(const string t_username) const {
	vector<string>::const_iterator it = m_participators.begin();
	while (it != m_participators.end()) {
		if (*it == t_username)
			return true;
		++it;
	}
	return false;
}