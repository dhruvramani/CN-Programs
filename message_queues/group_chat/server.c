#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <string.h>

struct mesg_buffer
{
    long mtype;
    char mtext[20];
} message;

struct group
{
    int num;
    char pids[10][20];
} grps[5];

void get_message(char message[], char pid[], char text[], char group[])
{
    char *token = strtok(message, "|");
    strcpy(pid, token);
    token = strtok(NULL, "|");
    strcpy(group, token);
    token = strtok(NULL, "|");
    strcpy(text, token);
}

int main()
{
    key_t k = ftok("key1", 65);
    int msqid = msgget(k, 0666 | IPC_CREAT);

    while(msgrcv(msqid, &message, sizeof(message), 1, 0))
    {
        char buff[20], pid[20], text[20];
        strcpy(buff, message.mtext);
        get_message(message.mtext, pid, text, group);
        int g_id = atoi(group);
        if(strcmp(text, "add") == 0)
            strcpy(grps[g_id].pids[grps[g_id].num++], pid);
        else {
            for(int i=0; i<grps[g_id].num; i++)
                if(strcmp(grps[g_id].pids[i], pid) != 0)
                {
                    strcpy(message.mtext, text)
                    message.mtype = atoi(grps[g_id].pids[i]);
                    msgsnd(msqid, &message, sizeof(message), 0);
                    printf("Server sent message %s\n", message.mtext);
                }
            
        }
    }