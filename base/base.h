//
// Created by frank on 17-2-12.
//

#ifndef FANCY_BASE_H
#define FANCY_BASE_H

#define _GNU_SOURCE
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/epoll.h>

#include <signal.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "error.h"

#define FCY_OK      0
#define FCY_ERROR   -1
#define FCY_AGAIN   EAGAIN

#define link_data(node, type, member) \
    (type*)((u_char*)node - offsetof(type, member))

#define RETURN_ON(exp, err)   \
do {    \
    if (exp == err) {    \
        err_sys("%s error at line %d", __FUNCTION__, __LINE__); \
        return FCY_ERROR;   \
    }   \
} while(0)

#define ABORT_ON(exp, err)    \
do {    \
    if (exp == err) {    \
        err_sys("%s error at line %d", __FUNCTION__, __LINE__); \
        abort();    \
    }   \
} while(0)

#endif //FANCY_BASE_H
