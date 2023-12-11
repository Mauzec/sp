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

int main(int argc, char *argv[]) {
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);

    syslog(LOG_INFO, "Entering configuration");

    if (argc < 4) {
        syslog(LOG_INFO, "Wrong arguments passed. Use: <folder1 to copy> <folder2 where folder1 will be copied> <frequency in minutes>");
        cout << "Wrong arguments passed. Use: <folder1 to copy> <folder2 where folder1 will be copied> <frequency in minutes>\n";
        return -1;
    }

    string source_dir = string(argv[1]), backup_dir = string(argv[2]);
    int frequency = stoi(argv[3]);

    string line;

    system("rm -rf /home/parallels/Desktop/deamon_backup/settings");
    ofstream settings("/home/parallels/Desktop/deamon_backup/settings");
    settings << "source=" << source_dir << endl;
    settings << "to=" << backup_dir << endl;
    settings << "freq=" << frequency << endl;
    settings.close();

    syslog(LOG_INFO, "Configuration file saved");
    cout << "Configuration file saved\n";

    syslog(LOG_INFO, "Recompile /home/parallels/Desktop/deamon_backup/deamon.cpp");
    cout << "Recompile /home/parallels/Desktop/deamon_backup/deamon.cpp\n";
    system("g++ /home/parallels/Desktop/deamon_backup/deamon.cpp -o /home/parallels/Desktop/deamon_backup/g");
    
    syslog(LOG_INFO, "Kill existed daemon");
    cout << "Kill existed daemon\n";
    system("systemctl stop backup_daemon.service");
    
    syslog(LOG_INFO, "Delete existed daemon");
    cout << "Delete existed daemon\n";
    system("rm -rf /etc/systemd/system/backup_daemon.service");
    
    syslog(LOG_INFO, "Write backup_daemon.service");
    cout << "Write backup_daemon.service\n";
    system("touch backup_daemon.service");
    
    ofstream service("backup_daemon.service");
    service << "﻿[Unit]\nDescription=Backup Daemon\n\n";
    service << "[Service]\nExecStart=/home/parallels/Desktop/deamon_backup/g\nRestart=always\n\n";
    service << "[Install]\nWantedBy=multi-user.target\n";
    service.close();
    
    syslog(LOG_INFO, "Copy backup_daemon.service to systemd");
    cout << "Copy backup_daemon.service to systemd\n";
    system("cp backup_daemon.service /etc/systemd/system/");
    system("rm -rf backup_daemon.service");
    
    syslog(LOG_INFO, "Start daemon");
    cout << "Start daemon\n";
    system("systemctl start backup_daemon.service");
    
    syslog(LOG_INFO, "Reload units");
    cout << "Reload units\n";
    system("systemctl daemon-reload");    
}
