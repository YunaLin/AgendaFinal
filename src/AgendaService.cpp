// modifying
#include <list>
#include <string>
#include <functional>
#include <iostream>
#include <exception>
#include "AgendaService.hpp"
using namespace std;

void AgendaService::startAgenda(void) {
	m_storage = NULL;
	m_storage = Storage::getInstance();
}
void AgendaService::quitAgenda(void) {
	m_storage->sync();
}
AgendaService::AgendaService() {
	startAgenda();
}
AgendaService::~AgendaService() {
	quitAgenda();
} 
bool AgendaService::userLogIn(const string userName, const string password) {
	auto filter = [userName, password](const User& t_user) {
		if (userName == t_user.getName() && password == t_user.getPassword())
			return true;
		else 
			return false;
	};
	auto temp = m_storage->queryUser(filter);
	if (temp.size() != 0) return true;
	else return false;
}
bool AgendaService::userRegister(const string userName, const string password,
	const string email, const string phone) {
	auto filter = [userName, password, email, phone](const User& t_user) {
		if (userName == t_user.getName())
			return true;
		else
			return false;
	};
	auto temp = m_storage->queryUser(filter);
	if (temp.size() != 0) {
		return false;
	} else {
		User user = User(userName, password, email, phone);
		m_storage->createUser(user);
		return true;
	}
}
bool AgendaService::deleteUser(const string userName, const string password) {
	auto filter = [userName, password](const User& t_user) {
		if (userName == t_user.getName() && password == t_user.getPassword())
			return true;
		else
			return false;
	};
	auto filter1 = [userName](const Meeting& t_meeting) {
		if (userName == t_meeting.getSponsor() || t_meeting.isParticipator(userName))
			return true;
		else return false;
	};
	auto temp = m_storage->queryUser(filter);
	if (temp.size() != 0) {
		m_storage->deleteUser(filter);
		m_storage->deleteMeeting(filter1);  // delete all the related meetings
		return true;
	} else {
		return false;
	}
}
list<User> AgendaService::listAllUsers() const {
	auto filter = [](const User& t_user) {
		return true;
	};
	return m_storage->queryUser(filter);
}
bool isNoTimeOverlap(Date start, Date end, Date t_start, Date t_end) {
	if (start >= end) return false;
	if (start >= t_end) return true;
	if (t_start >= end) return true;
	return false;
}
bool AgendaService::createMeeting(const string userName, const string title,
	const string startDate, const string endDate, const vector<string> participator) {
	Date start = Date::stringToDate(startDate);
	Date end = Date::stringToDate(endDate);
	// sponsor can not be the participator
	auto test = participator.begin();
	while (test != participator.end()) {
		if (userName == (*test))
			return false;
		test++;
	}
	//title can not be the same
	auto filter = [title](const Meeting& t_meeting) {
		if (title == t_meeting.getTitle()) return true;
		else return false;
	};
	auto temp1 = m_storage->queryMeeting(filter);
	if (temp1.size() > 0)
		return false;
	// participator can not be the same
	auto i_it = participator.begin();
	auto j_it = participator.begin();
	for (i_it; i_it != participator.end(); ++i_it) {
		for (j_it = i_it + 1; j_it != participator.end(); ++j_it)
			if (*i_it == *j_it)
				return false;
	}
	// sponsor exists
	auto sp_filter = [userName](const User& user) {
		if (userName == user.getName()) return true;
		else return false;
	};
	auto t_user = m_storage->queryUser(sp_filter);
	if (t_user.size() == 0) return false;
	// participators exist
	if (participator.size() == 0) return false;
	auto par_it = participator.begin();
	while (par_it != participator.end()) {
		string t_ptor = *par_it;
		auto par_filter = [t_ptor](const User& user) {
			if (t_ptor == user.getName())
				return true;
			return false;
		};
		auto t_participatorList = m_storage->queryUser(par_filter);
		if (t_participatorList.size() == 0) return false;
		++par_it;
	}
	// date should be valid
	if (!Date::isValid(start)) return false;
	if (!Date::isValid(end)) return false;
	if (start >= end) 
		return false;
	// date overlap
	auto filter1 = [userName, start, end, participator](const Meeting& t_meeting) {
		if (userName == t_meeting.getSponsor() || t_meeting.isParticipator(userName)) {
			if (!isNoTimeOverlap(start, end, t_meeting.getStartDate(), t_meeting.getEndDate())) {
				return true;
			}
		}
		auto pit = participator.begin();
		while (pit != participator.end()) {
			if (*pit == t_meeting.getSponsor() || t_meeting.isParticipator(*pit)) {
				if (!isNoTimeOverlap(start, end, t_meeting.getStartDate(), t_meeting.getEndDate())) {
					return true;
				}
			}
			++pit;
		}
		return false;
    };
    auto temp = m_storage->queryMeeting(filter1);
    if (temp.size() > 0)
    	return false;
    Meeting meeting = Meeting(userName, participator, startDate, endDate, title);
    m_storage->createMeeting(meeting);
	return true;
}
list<Meeting> AgendaService::meetingQuery(const string userName, const string title) const {
	auto filter = [userName, title](const Meeting& t_meeting) {
		if (title != t_meeting.getTitle()) return false;
		if (userName == t_meeting.getSponsor() || t_meeting.isParticipator(userName))
			return true;
		else 
			return false;
	};
	return m_storage->queryMeeting(filter);
}
list<Meeting> AgendaService::meetingQuery(const string userName, const string startDate, const string endDate) const {
	auto filter = [userName, startDate, endDate](const Meeting& t_meeting) {
		if (userName != t_meeting.getSponsor() && !t_meeting.isParticipator(userName)) return false;
		Date t_start = Date::stringToDate(startDate);
		Date t_end = Date::stringToDate(endDate);
		Date start = t_meeting.getStartDate();
		Date end = t_meeting.getEndDate();
		if (!(start > t_end || t_start > end)) return true;
		else return false;
	};
	return m_storage->queryMeeting(filter);
}
list<Meeting> AgendaService::listAllMeetings(const string userName) const {
	auto filter = [userName](const Meeting& t_meeting) {
		if (userName == t_meeting.getSponsor() || t_meeting.isParticipator(userName))
		    return true;
		return false;
	};
	return m_storage->queryMeeting(filter);
}
list<Meeting> AgendaService::listAllSponsorMeetings(const string userName) const {
	auto filter = [userName](const Meeting& t_meeting) {
		if (userName == t_meeting.getSponsor())
			return true;
		return false;
	};
	return m_storage->queryMeeting(filter);
} 
list<Meeting> AgendaService::listAllParticipateMeetings(const string userName) const {
	auto filter = [userName](const Meeting& t_meeting) {
		if (t_meeting.isParticipator(userName))
			return true;
		return false;
	};
	return m_storage->queryMeeting(filter);
}
bool AgendaService::deleteMeeting(const string userName, const string title) {
	auto filter = [userName, title](const Meeting& t_meeting) {
		if (t_meeting.getSponsor() == userName) {
			if (t_meeting.getTitle() == title)
				return true;
			else 
				return false;
		}
		return false;
	};
	if (m_storage->deleteMeeting(filter) != 0)
		return true;
	else return false;
}
bool AgendaService::deleteAllMeetings(string userName) {
	auto filter = [userName](const Meeting& t_meeting) {
		if (userName == t_meeting.getSponsor())
			return true;
		else
			return false;
	};
	if (m_storage->deleteMeeting(filter) != 0)
		return true;
	else 
		return false;
}
void AgendaService::updateEmail(string userName, string password, string newEmail) {
	auto switcher = [newEmail](User& t_user) {
		t_user.setEmail(newEmail);
	};
	auto filter = [userName, password](const User& t_user) {
		if (userName == t_user.getName() && password == t_user.getPassword())
			return true;
		else
			return false;
	};
	m_storage->updateUser(filter, switcher);
}
void AgendaService::updatePassword(string userName, string password, string newPassword) {
	auto switcher = [newPassword](User& t_user) {
		t_user.setPassword(newPassword);
	};
	auto filter = [userName, password](const User& t_user) {
		if (userName == t_user.getName() && password == t_user.getPassword())
			return true;
		else
			return false;
	};
	m_storage->updateUser(filter, switcher);
}
void AgendaService::updatePhone(string userName, string password, string newPhone) {
	auto switcher = [newPhone](User& t_user) {
		t_user.setPhone(newPhone);
	};
	auto filter = [userName, password](const User& t_user) {
		if (userName == t_user.getName() && password == t_user.getPassword())
			return true;
		else
			return false;
	};
	m_storage->updateUser(filter, switcher);
}
void AgendaService::updateMeetingByTitle(string sponsor, string title, string startDate,
	string endDate, vector<string> participator) {
	auto filter = [title](const Meeting& t_meeting) {
		if (title == t_meeting.getTitle())
			return true;
		else
			return false;
	};
	auto switcher = [sponsor, title, startDate, endDate, participator](Meeting& t_meeting) {
		t_meeting.setSponsor(sponsor);
		t_meeting.setTitle(title);
		t_meeting.setStartDate(startDate);
		t_meeting.setEndDate(endDate);
		t_meeting.setParticipator(participator);
	};
	m_storage->updateMeeting(filter, switcher);
}