/*
   Copyright (C) gnbdev

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include "gnb_arg_list.h"


int gnb_get_pid(){
    int pid = getpid();
    return pid;
}


void gnb_set_env(const char *name, const char *value){

    setenv(name, value, 1);

}


pid_t gnb_exec(char *app_filename, char *current_path, gnb_arg_list_t *arg_list, int flag){

    pid_t pid;

    int fd;

    int ret;

    char *argv[arg_list->argc];

    pid = fork();

    if ( 0 != pid ){
        return pid;
    }

    int i;

    for(i=0; i<arg_list->argc; i++){
        argv[i] = arg_list->argv[i];
    }

    argv[i] = NULL;

    ret = chdir(current_path);

    if( 0 != ret ){
    	goto finish;
    }

    fd = open("/dev/null", O_RDWR);

    if ( 0 != fd ){
        ret = dup2(fd, STDIN_FILENO);
        ret = dup2(fd, STDOUT_FILENO);
        ret = dup2(fd, STDERR_FILENO);
    }

    ret = execve(app_filename, argv, NULL);

    if( -1==ret ){
    	goto finish;
    }

finish:

    exit(0);

    return 0;

}


void gnb_kill(pid_t pid){

    kill(pid, SIGKILL);

}
