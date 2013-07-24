#ifndef __SHARED_DEFINES_H__
# define __SHARED_DEFINES_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "xfunc.h"
#include "init.h"

# define TCP_PORT 3000
# define UDP_PORT 3001
# define MAX_CLIENT 42

#endif /* !__SHARED_DEFINES_H__ */
