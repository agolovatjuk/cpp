/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mysysv.cpp
 * Author: golo
 *
 * Created on 5 февраля 2016 г., 1:13
 */

/*

Создайте очередь сообщения SysV с ключем:

key_t key = ftok("/tmp/msg.temp", 1);

Тестовая среда пошлет в нее сообщение.

struct message {
    long mtype;
    char mtext[80];
}

Получите его и сохраните в /home/box/message.txt

 */


#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    int rc;
    int fd;

    char fname[] = "message.txt";
    
//    int fd = open("/tmp/mesg.tmp", O_RDWR|O_CREAT, 0666);
//    close(fd);
    
    struct message {
        long mtype;
        char mtext[80];
    };

// делаю в одной проге, есть 2 структуры приём/передача
// msg --> queue --> rmsg --> file
    
    struct message msg;    
    msg.mtype = 1;   

    struct message rmsg;   
    rmsg.mtype = 1;
    
    key_t key = ftok("/tmp/msg.temp", 1);
    int msgid = msgget(key, IPC_CREAT|0666);
//    if (msgid < 0){
//        std::cout << "Error create queue" << std::endl;
//        return 1;
//    }
    
    snprintf(msg.mtext, sizeof(msg.mtext), "%s", "Hello, SYSV");
    
// TO DO size !!!    see write file 
    rc = msgsnd(msgid, &msg, sizeof(msg.mtext) , 0);
//    if (rc < 0){
//        std::cout << "Error send" << std::endl;
//        return 1;
//    }
    
    ssize_t bytes = msgrcv(msgid, &rmsg, sizeof(rmsg.mtext), 0, 0);
//    if (rc < 0){
//        std::cout << "Error recv" << std::endl;
//        return 1;
//    }
    rc = msgctl(msgid, IPC_RMID, NULL);

    printf(" Recieved:%s:", rmsg.mtext);
    fd = open(fname, O_CREAT|O_RDWR|O_TRUNC, 0666);
    rc = write(fd, rmsg.mtext, bytes);
//    rc = write(fd, rmsg.mtext, strlen(rmsg.mtext));
    close(fd);

//    FILE *f=fopen(fname, "w");
//    fprintf(f, "%s", rmsg.mtext);
//    fclose(f);
    
    std::cout << "Test SYSV queue:" << bytes << std::endl;
//    std::cout << "Test SYSV queue:" << msgid << std::endl;
//    std::cout << "Close queue RC:" << rc << std::endl;

    return 0;
}

