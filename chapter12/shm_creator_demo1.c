/*****************************************************************************
  Title          : shm_creator_demo1.c
  Author         : Stewart Weiss
  Created on     : June 7, 2024
  Description    : A program that creates and writes into shared memory
  Purpose        : To introduce basics of shared memory
  Usage          : shm_creator_demo1  <shared-memory-name>
  Build with     : gcc -Wall -g -I../include -L../lib  -o shm_creator_demo1 \
                    shm_creator_demo1.c -lrt

******************************************************************************
* Copyright (C) 2023 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/

#include "shm_demo1.h"  /* For definitions of constants */


int main(int argc, char *argv[])
{
    int fd;       /* File descriptor referring to shared memory object */
    char *shmp;   /* Pointer to start of shared memory region          */
    char  usage[256];

    if (argc != 2) {
        sprintf(usage, "Usage: %s /shm-path\n", argv[0]);
        usage_error(usage);
    }

    /* Create the named shared memory object for reading and writing.  */
    if ( (fd = shm_open(argv[1], O_CREAT | O_EXCL | O_RDWR,
                       S_IRUSR | S_IWUSR) ) == -1 )
        fatal_error(errno, "shm_open");

    if (ftruncate(fd, BUF_SIZE) == -1)  /* Make it a fixed size.       */
        fatal_error(errno,"ftruncate");

     /* Map the object into the process address space. */
    if ( ( shmp = mmap(NULL, BUF_SIZE, PROT_READ|PROT_WRITE,
                      MAP_SHARED, fd, 0) ) == MAP_FAILED )
        fatal_error(errno,"mmap");

    if ( -1 == close(fd))  /* Close unneeded file descriptor.          */
        fatal_error(errno, "close");

    printf("Memory is mapped in process starting at address %p\n", shmp);

    /* Write two strings at fixed locations into this memory. */
    strcpy(shmp+offset1, "Hello");
    strcpy(shmp+offset2, "World");
    exit(EXIT_SUCCESS);
 }


