#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <string>
#include <chrono>
#include <ctime>
#include <thread>
#include <iostream>
#include <fstream>

using namespace std;

#define DAEMON_NAME "backup_daemon"
  

void perform_backup(string &source_dir, string &backup_dir) {
    string command = "cp -r ";
    command.append(source_dir);
    command.append(" ");
    command.append(backup_dir);
    system(command.c_str());
}

int main(int argc, char *argv[]) {

    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);

    syslog(LOG_INFO, "Entering Daemon");

    string source_dir = "", backup_dir;
    int frequency;

    string line;
    ifstream settings("/home/parallels/Desktop/deamon_backup/settings");
    while (getline (settings, line)) {
        string key, value;
        bool key_read = true ;
        for (auto sym : line) {
            if (sym != '=' && sym != '\n') {
                if (key_read)
                    key.append(string(1, sym));
                else
                    value.append(string(1, sym));
            } else {
                key_read = false;
            }
        }

        if (key == "source") source_dir = value;
        else if (key == "to") backup_dir = value;
        else frequency = stoi(value);
    }
    settings.close();

    syslog(LOG_INFO, source_dir.c_str());
    syslog(LOG_INFO, backup_dir.c_str());
    syslog(LOG_INFO, to_string(frequency).c_str());

    while (true) {
        syslog(LOG_INFO, "Start backuping");
        perform_backup(source_dir, backup_dir);
        syslog(LOG_INFO, "Close backuping");

        this_thread::sleep_for(chrono::minutes(frequency));
    }
}
