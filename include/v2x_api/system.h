/*
 * system.h
 *
 *  Created on: 2017. 2. 16.
 *      Author: ITT
 *
 *  WPM(및 관련 어플리케이션)에서 필요한 리눅스 시스템 헤더파일들을 정의한 헤더파일.
 *  어플리케이션 개발자는 API 함수를 사용하기 위해 본 헤더를 include해야 한다.
 */

#ifndef SYSTEM_H
#define SYSTEM_H


/****************************************************************************************
	시스템 헤더

****************************************************************************************/
/* 커널 모듈 빌드 시에 필요한 헤더 파일 */
#ifdef _KERNEL_SPACE_

	#include <linux/init.h>
	#include <linux/kernel.h>
	#include <linux/module.h>
	#include <linux/irq.h>
	#include <linux/types.h>
	#include <linux/workqueue.h>
	#include <linux/netdevice.h>
	#include <linux/if.h>
	#include <linux/if_ether.h>
	#include <linux/etherdevice.h>
	#include <linux/wait.h>
	#include <linux/time.h>
	#include <linux/kthread.h>
	#include <linux/types.h>
	#include <linux/netlink.h>
	#include <linux/jiffies.h>
	#include <net/sock.h>

/* 사용자 레벨 모듈 빌드 시에 필요한 헤더 파일 */
#else
	#include <stdint.h>
	#include <stdio.h>
	#include <stdbool.h>
	#include <string.h>
	#include <stdlib.h>
	#include <stdarg.h>
	#include <signal.h>
	#include <fcntl.h>
	#include <termios.h>
	#include <errno.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <pthread.h>
	#include <mqueue.h>
	#include <netdb.h>
	#include <sysexits.h>
	#include <getopt.h>
	#include <dirent.h>
	#include <linux/if_ether.h>
	#include <linux/netlink.h>
	#include <linux/types.h>
	#include <linux/ipv6_route.h>
	#include <sys/stat.h>
	#include <sys/mman.h>
	#include <sys/shm.h>
	#include <sys/wait.h>
	#include <sys/socket.h>
	#include <sys/ioctl.h>
	#include <sys/time.h>
	#include <sys/types.h>
	#include <sys/ipc.h>
	#include <sys/msg.h>
	#include <sys/syscall.h>
	#include <arpa/inet.h>
	#include <netinet/in.h>
	#include <net/if.h>
	#include <poll.h>
#endif


/****************************************************************************************
	프로젝트 헤더

****************************************************************************************/


/****************************************************************************************
	상수

****************************************************************************************/


/****************************************************************************************
	매크로

****************************************************************************************/


/****************************************************************************************
	유형 정의

****************************************************************************************/


/****************************************************************************************
	열거형

****************************************************************************************/


/****************************************************************************************
	구조체

****************************************************************************************/


/****************************************************************************************
	전역변수

****************************************************************************************/


/****************************************************************************************
	함수원형(지역/전역)

****************************************************************************************/


#endif /* !SYSTEM_H */
