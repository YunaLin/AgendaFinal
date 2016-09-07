#include <iostream>
#include <string>
#include <iomanip>
#include <stdio.h>
#include <cstdlib>
#include "AgendaUI.hpp"
using namespace std;

AgendaUI::AgendaUI() {
	m_userName = "";
	m_userPassword = "";
	startAgenda();
}
void AgendaUI::OperationLoop(void) {
	cout << endl;
	cout << "-------------------------------------";
	printf("\033[40;32mAgenda\033[0m");
	cout << "--------------------------------------" << endl;
	cout << "Action :" << endl;
	cout << "l   - log in Agenda by user name and password" << endl;
	cout << "r   - register an Agenda account" << endl;
	cout << "q   - quit Agenda" << endl;
	cout << "---------------------------------------------------------------------------------" << endl;
	cout << endl;
	cout << "Agenda : ~$ ";
	char ch;
	while (cin >> ch && ch != 'q') {
		if (ch == 'l') {
			userLogIn();
		} else if (ch == 'r') {
			userRegister();
		} else {
			printf("\033[40;31m[ERROR]\033[0m ");
			cout << "Invalid input, try again please" << endl;
		}
	}
	if (ch == 'q') quitAgenda();
}
void AgendaUI::startAgenda(void) {
	m_agendaService.startAgenda();
}

string AgendaUI::getOperation() {
	string operation;
	cout << endl;
	cout << "-------------------------------------";
	printf("\033[40;32mAgenda\033[0m");
	cout << "-------------------------------------" << endl;
	cout << "Action:" << endl;
	cout << "o   - log out Agenda" << endl;
	cout << "dc  - delete Agenda accout" << endl;
	cout << "lu  - list all Agenda user" << endl;
	cout << "cm  - create a meeting" << endl;
	cout << "la  - list all meetings" << endl;
	cout << "las - list all sponsor meetings" << endl;
	cout << "lap - list all participate meetings" << endl;
	cout << "qm  - query meeting by title" << endl;
	cout << "qt  - query meeting by time interval" << endl;
	cout << "dm  - delete meeting by title" << endl;
	cout << "da  - delete all meetings" << endl;
	cout << "Modify:" << endl;
	cout << "me  - modify the initial email" << endl;
	cout << "mp  - modify the initial phone" << endl;
	cout << "mw  - modify the initial password" << endl;
//	cout << "umt - update meeting by title" << endl;
	cout << "-------------------------------------------------------------------------------" << endl;
	cout << endl;
	cout << "Agenda @ " << m_userName << " : # ";
	cin >> operation;
	return operation;
}
bool AgendaUI::executeOperation(string t_operation) {
	if (t_operation == "o") {
		userLogOut();
		return true;
	}
	if (t_operation == "dc") {
		deleteUser();
		return true;
	}
	if (t_operation == "lu") {
		listAllUsers();
		return true;
	}
	if (t_operation == "cm") {
		createMeeting();
		return true;
	}
	if (t_operation == "la") {
		listAllMeetings();
		return true;
	}
	if (t_operation == "las") {
		listAllSponsorMeetings();
		return true;
	}
	if (t_operation == "lap") {
		listAllParticipateMeetings();
		return true;
	}
	if (t_operation == "qm") {
		queryMeetingByTitle();
		return true;
	}
	if (t_operation == "qt") {
		queryMeetingByTimeInterval();
		return true;
	}
	if (t_operation == "dm") {
		deleteMeetingByTitle();
		return true;
	}
	if (t_operation == "da") {
		deleteAllMeetings();
		return true;
	}
	if (t_operation == "me") {
		string newEmail;
		cout << "new email : ";
		cin >> newEmail;
		m_agendaService.updateEmail(m_userName, m_userPassword, newEmail);
		cout << "success!" << endl;
		return true;
	}
	if (t_operation == "mw") {
		string newPassword;
		cout << "new password : ";
		cin >> newPassword;
		string again;
		cout << "input again please" << endl;
		cin >> again;
		while (newPassword != again) {
			cout << "two different input" << endl;
			cout << "try again please" << endl;
			cin >> again;
		}
		if (newPassword == again) {
			m_agendaService.updatePassword(m_userName, m_userPassword, again);
			cout << "success" << endl;
		}
		return true;
	}
	if (t_operation == "mp") {
		string newPhone;
		cout << "new phone : ";
		cin >> newPhone;
		m_agendaService.updatePhone(m_userName, m_userPassword, newPhone);
		cout << "success" << endl;
		return true;
	} 
	if (t_operation == "umt") {
		string title;
		cout << "[title] ";
		cin >> title;
		list<Meeting> temp = m_agendaService.meetingQuery(m_userName, title);
		if (temp.size() != 0) {
			string nsponsor, ntitle, nstartDate, nendDate, nptor;
			vector<string> nparticipator;
			int count = 0;
			cout << "input the update information please" << endl;
			cout << "[sponsor] [title] [startDate(yyyy-mm-dd/hh:mm)] [endDate(yyyy-mm-dd/hh:mm)]" << endl;
			cin >> nsponsor >> ntitle >> nstartDate >> nendDate;
			cout << "the number of participators is ";
			cin >> count; 
			for (int i = 0; i < count; ++i) {
				cin >> nptor;
				nparticipator.push_back(nptor);
			}
			m_agendaService.updateMeetingByTitle(nsponsor, ntitle, nstartDate, nendDate, nparticipator);
			cout << "success" << endl;
		} else {
			cout << title << " does not exist" << endl;
		}
		return true;
	} else {
		printf(" \033[40;31m[ERROR]\033[0m ");
		cout << "Invalid operation, try again please" << endl;
		return true;
	}
}
void AgendaUI::userLogIn(void) {
	cout << "[log in] [user name] [password]" << endl;
	cout << "[log in] ";
	cin >> m_userName >> m_userPassword;
	bool flag = m_agendaService.userLogIn(m_userName, m_userPassword);
	if (flag) {
		cout << "succeed!" << endl;
		while (executeOperation(getOperation()));
	} else {
		cout << "log in fail!" << endl;
		OperationLoop();
	}
}
void AgendaUI::userRegister(void) {
	cout << "[register] [userName] [password] [email] [phone]" << endl;
	cout << "[register] ";
	string t_email, t_phone;
	cin >> m_userName >> m_userPassword >> t_email >> t_phone;
	bool flag = m_agendaService.userRegister(m_userName, m_userPassword, t_email, t_phone);
	if (flag) {
		cout << "succeed!" << endl;
		while (executeOperation(getOperation()));
	} else {
		cout << "register fail!" << endl; 
		OperationLoop();
	}
}
void AgendaUI::quitAgenda(void) {
	char ch;
	cout << "Are you sure to quit ? (Y/N) " << endl;
	cin >> ch;
	if (ch == 'y' || ch == 'Y') {
		m_agendaService.quitAgenda();
		exit(1);
	} else {
		return;
	}
}
void AgendaUI::userLogOut(void) {
	OperationLoop();
}
void AgendaUI::deleteUser(void) {
	int count = m_agendaService.deleteUser(m_userName, m_userPassword);
	cout << endl;
	if (count != 0) {
		cout << "[delete Agenda account] succeed!" << endl;
		OperationLoop();
	}
}
void AgendaUI::listAllUsers(void) {
	cout << "[list all users]" << endl;
	cout << setiosflags(ios::left) << setw(15) << "name" << 
	setiosflags(ios::left) << setw(15)  << "email" <<
	setiosflags(ios::left) << setw(15)  << "phone" << endl;
	list<User> temp = m_agendaService.listAllUsers();
	list<User>::iterator iter = temp.begin();
	while (iter != temp.end()) {
		cout << setiosflags(ios::left) << setw(15) << (*iter).getName() <<
		setiosflags(ios::left) << setw(15) << (*iter).getEmail() << 
		setiosflags(ios::left) << setw(15) << (*iter).getPhone() << endl;
		iter++;
	}
}
void AgendaUI::createMeeting(void) {
	int count;
	cout << "[create meeting] [the number of participators]" << endl;
	cout << "[create meeting] ";
	cin >> count;
	vector<string> part;
	string str;
	for (int i = 0; i < count; ++i) {
		cout << "[create meeting] [please enter the participator " << (i + 1) << " ]" << endl;
		cout << "[create meeting] ";
		cin >> str;
		list<User> temp = m_agendaService.listAllUsers();
		list<User>::iterator it = temp.begin();
		bool validUser = false;
		if (temp.size() == 0) validUser = true;
		while (it != temp.end()) {
			if ((*it).getName() == str) {
				validUser = true;
				break;
			}
		 	it++;
		}
		if (validUser) {
			part.push_back(str);
		} else {
			i--;
			cout << "user " << str << " does not exist in this system, try again please" << endl; 
			continue;
		}
	}
	string t_title, t_start, t_end;
	cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
	cout << "[create meeting] ";
	cin >> t_title >> t_start >> t_end;
	bool flag = m_agendaService.createMeeting(m_userName, t_title, t_start, t_end, part);
	if (flag) {
		cout << "succeed!" << endl;
	} else {
		cout << "fail!" << endl;

	}
}
void AgendaUI::listAllMeetings(void) {
	cout << "[list all meetings]" << endl;
	list<Meeting> temp = m_agendaService.listAllMeetings(m_userName);
	printMeetings(temp);
}
void AgendaUI::listAllSponsorMeetings(void) {
	cout << "[list all sponsor meetings]" << endl;
	list<Meeting> temp = m_agendaService.listAllSponsorMeetings(m_userName);
	printMeetings(temp);
}
void AgendaUI::listAllParticipateMeetings(void) {
	cout << "[list all participator meetings]" << endl;
	list<Meeting> temp = m_agendaService.listAllParticipateMeetings(m_userName);
	printMeetings(temp);
}
void AgendaUI::queryMeetingByTitle(void) {
	string t_title;
	cout << "[query meeting] [title]:" << endl;
	cout << "[query meeting] ";
	cin >> t_title;
	list<Meeting> temp = m_agendaService.meetingQuery(m_userName, t_title);
	printMeetings(temp);
}
void AgendaUI::queryMeetingByTimeInterval(void) {
	cout << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
	string start, end;
	cout << "[query meetings] ";
	cin >> start >> end;
	list<Meeting> temp = m_agendaService.meetingQuery(m_userName, start, end);
	printMeetings(temp);
}
void AgendaUI::deleteMeetingByTitle(void) {
	cout << "[delete meeting] [title]" << endl;
	cout << "[delete meeting] ";
	string t_title;
	cin >> t_title;
	cout << endl;
	bool flag =m_agendaService.deleteMeeting(m_userName, t_title);
	if (flag) cout << "[delete meeting by title] succeed!" << endl;
	else cout << "[error] delete meeting fail!" << endl;
}
void AgendaUI::deleteAllMeetings(void) {
	list<Meeting> temp = m_agendaService.listAllMeetings(m_userName);
	list<Meeting>::iterator iter = temp.begin();
	while (iter != temp.end()) {
		m_agendaService.deleteAllMeetings(m_userName);
		++iter;
	}
	cout << "[delete all meetings] succeed!" << endl;
}
void AgendaUI::printMeetings(list<Meeting> t_meetings) {
	list<Meeting>::iterator iter = t_meetings.begin();
	cout << "total : " << t_meetings.size() << endl;
	if (t_meetings.size() != 0) {
		cout << setiosflags(ios::left) << setw(15) << "title" <<
		setiosflags(ios::left) << setw(15) << "sponsor";
		cout << setiosflags(ios::left) << setw(18) << "start time" << 
		setiosflags(ios::left) << setw(18) << "end time";
		cout << setiosflags(ios::left) << setw(15) << "participators" << endl;
		while (iter != t_meetings.end()) {
		    cout << setiosflags(ios::left) << setw(15) << (*iter).getTitle();
			cout << setiosflags(ios::left) << setw(15) << (*iter).getSponsor();
		    cout << setiosflags(ios::left) << setw(18) << Date::dateToString((*iter).getStartDate());
		    cout << setiosflags(ios::left) << setw(18) << Date::dateToString((*iter).getEndDate());
		    vector<string> part = (*iter).getParticipator();
		    if (part.size() != 0) {
		    	vector<string>::iterator it = part.begin();
		    	cout << (*it);
		    	it++;
		    	while (it != part.end()) {
			    	cout << "&" << (*it);
			    	it++;
		    	}
		    }	 
		    cout << endl;
		    iter++;
		}	
	} 
}