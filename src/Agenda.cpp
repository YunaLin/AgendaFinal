#include "AgendaUI.hpp"
#include <csignal>
using namespace std;
AgendaUI aui;

void signalHandle(int sigNum) {
	switch(sigNum) {
		case 1:
			cout << "Get a signal -- SIGHUP" << endl;
			aui.quitAgenda();
			break;
		case 2:
			cout << "Get a signal -- SIGINT" << endl;
			aui.quitAgenda();
			break;
		case 3:
			cout << "Get a signal -- SIGQUIT" << endl;
			aui.quitAgenda();
			break;
		case 9:
			cout << "Get a signal -- SIGKILL" << endl;
			aui.quitAgenda();
			break;
		case 13:
			cout << "Get a signal -- SIGPIPE" << endl;
			aui.quitAgenda();
			break;
		case 19:
			cout << "Get a signal -- SIGSTOP" << endl;
			aui.quitAgenda();
			break;
		case 20:
			cout << "Get a signal -- SIGTSTP" << endl;
			aui.quitAgenda();
			break;
		case 21:
			cout << "Get a signal -- SIGTTIN" << endl;
			aui.quitAgenda();
			break;
		case 22:
			cout << "Get a signal -- SIGTTOU" << endl;
	}
	return;
}

int main() {
	signal(SIGHUP, signalHandle);
	signal(SIGINT, signalHandle);
	signal(SIGQUIT, signalHandle);
	signal(SIGKILL, signalHandle);
	signal(SIGPIPE, signalHandle);
	signal(SIGSTOP, signalHandle);
	signal(SIGTSTP, signalHandle);
	signal(SIGTTIN, signalHandle);
	signal(SIGTTOU, signalHandle);
    aui.OperationLoop();
    return 0;
}
