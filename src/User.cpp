#include <iostream>
#include <string>
#include "User.hpp"
using namespace std;

User::User(string t_userName, string t_userPassword, 
	string t_userEmail, string t_userPhone):
	m_name(t_userName),m_password(t_userPassword),
	m_email(t_userEmail),m_phone(t_userPhone) {}

User::User(const User& t_user) {
	m_name = t_user.getName();
	m_phone = t_user.getPhone();
	m_password = t_user.getPassword();
	m_email = t_user.getEmail();
}
string User::getName() const { return m_name;}

void User::setName(string t_userName) { m_name = t_userName;}

string User::getPassword() const { return m_password;}

void User::setPassword(string t_userPassword) { m_password = t_userPassword;}

string User::getEmail() const { return m_email;}

void User::setEmail(string t_userEmail) { m_email = t_userEmail;}

string User::getPhone() const { return m_phone;}

void User::setPhone(string t_userPhone) { m_phone = t_userPhone;}
