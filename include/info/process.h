#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>

typedef struct _ProcessInfo {
    pid_t child, parent;
    int fd[2];
} ProcessInfo;

ProcessInfo* createProcessInfo();

void setProcChild(ProcessInfo* info, pid_t pid);

void setProcParent(ProcessInfo* info, pid_t pid);

pid_t getProcChild(ProcessInfo* info);

pid_t getProcParent(ProcessInfo* info);

int readFileDescriptor(ProcessInfo* info);

int writeFileDescriptor(ProcessInfo* info);

#endif
