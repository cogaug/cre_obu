
#include <cre_obu.h>
#include "version.h"

/* client socket global variable  */
int g_client_sock = -1;

/* client thread run global variable */
int client_run = 0;

/**
 * @brief       버전 출력
 * @retval      none
 * @author      albert
 * @date        2024-01-09
 */
static void print_version()
{
    printf("%s %s.%s.%s\n",
        PROJECT_NAME,
        PROJECT_VER_MAJOR,
        PROJECT_VER_MINOR,
        PROJECT_VER_PATCH);
} /* print_version */

/**
 * @brief       send data to v2x using uds
 * @param[in]   buffer : 전달할 데이터
 * @param[in]   len : 전달할 데이터의 크기
 * @retval      none
 * @author      albert
 * @date        2024-01-09
 */
void acu_send_to_v2x(unsigned char *buffer, int len)
{
    int sock = -1;
    struct sockaddr_un addr_un;

    sock = socket(AF_UNIX, SOCK_DGRAM, 0);

    /* socket create error */
    if (sock < 0) {
        return;
    }

    memset(&addr_un, 0x00, sizeof(struct sockaddr_un));
    addr_un.sun_family = AF_UNIX;
    strcpy(addr_un.sun_path, UDS_FILE_ACU_TX);

    sendto(sock, buffer, len, 0, (struct sockaddr *)&addr_un, sizeof(struct sockaddr_un));

    close(sock);
} /* acu_send_to_v2x */

/**
 * @brief       TCP로 수신한 데이터 처리
 * @param[in]   sock : TCP 클리아언트 소켓
 * @retval      음수 : 데이터 없음, 0 : 연결끊김, 양수 : 수신데이터의 크기
 * @author      albert
 * @date        2024-01-09
 */
int recv_client_packet(int sock)
{
    unsigned char buffer[MAX_BUFFER_SIZE];
    o2a_header_t *pheader = (o2a_header_t *) buffer;
    int retval = 0;
    int length = 0;
    int left = 0;
    int retry = 0;

    /* STX 크기 읽음 */
    retval = recv(sock, buffer, sizeof(O2A_HEADER_STX), 0);

    if (retval < 0) { /* no data */
        return -1;
    } else if (retval == 0) { /* disconnect */
        return 0;
    } else if (retval == sizeof(O2A_HEADER_STX)) {
        if (htonl(pheader->header) != O2A_HEADER_STX) {
            printf("header not match %08X\n", pheader->header);
            return -1;
        }
    } else {
        printf("STX read size error\n");
        return -1;
    }

    /*  헤더 나머지 읽음 */
    retval = recv(sock, &buffer[4], O2A_HEADER_SIZE - sizeof(O2A_HEADER_STX), 0);

    if (retval < 0) { /* no data */
        return -1;
    } else if (retval == 0) { /* disconnect */
        return 0;
    } else if (retval == (O2A_HEADER_SIZE - sizeof(O2A_HEADER_STX))) {

        length = htonl(pheader->length);

        if (length >= 1400) {
            printf("invalid length %d\n", length);
            return -1;
        }

        left = length;

        printf("length = %d\n", left);

        /* 데이터(payload) 부분 읽어오기 */
        while (left > 0) {
            retval = recv(sock, &buffer[O2A_HEADER_SIZE + (length - left)], left, 0);

            if (retval > 0) {
                left -= retval;
            } else if (retval == 0) { /* 연결 끊어짐 */
                return 0;
            } else { /* 못읽는 경우 */
                if (retry++ >= 10000) {
                    printf("retry over\n");
                    return -1;
                }
            }
        }

        if (left == 0) {
            acu_send_to_v2x(buffer, O2A_HEADER_SIZE + length);
        } else {
            return -1;
        }

        return (O2A_HEADER_SIZE + length);
    }

    return 0;
}/* recv_client_packet */

/**
 * @brief       tcp client thread
 * @param[in]   arg : gps context
 * @retval      none
 * @author      albert
 * @date        2024-01-08
 */
void *client_thread(void *arg)
{
    int uds_sock = -1;
    struct sockaddr_un msg_addr_un;
    struct sockaddr_un msg_recv_un;
    int namelen = sizeof(struct sockaddr_un);
    unsigned char rx_buffer[2302];
    int fl = 0;
    g_client_sock = *((int *) arg);
    int retval = 0;

    if (g_client_sock == -1) {
        sleep(1);
        goto client_exit;
    }

    printf("ACU connected\n");

    client_run = 1;

    /* set tcp socket  nonblocking */
    if ((fl = fcntl(g_client_sock, F_GETFL, NULL)) == -1) {
        sleep(1);
        goto client_tcp_exit;
    }

    fl |= O_NONBLOCK;

    if (fcntl(g_client_sock, F_SETFL, fl) == -1) {
        sleep(1);
        goto client_tcp_exit;
    }

    /* uds server open */
    if (access(UDS_FILE_ACU_RX, F_OK) == 0) {
        unlink(UDS_FILE_ACU_RX);
    }

    uds_sock = socket(AF_UNIX, SOCK_DGRAM, 0);

    /* uds socket create error */
    if (uds_sock < 0) {
        printf("uds sokcet fail\n");
        sleep(1);
        goto client_tcp_exit;
    }

    memset(&msg_addr_un, 0x00, sizeof(struct sockaddr_un));
    msg_addr_un.sun_family = AF_UNIX;
    strcpy(msg_addr_un.sun_path, UDS_FILE_ACU_RX);

    if (bind(uds_sock, (struct sockaddr *) &msg_addr_un, sizeof(struct sockaddr_un)) < 0) {
        printf("uds bind fail\n");
        sleep(1);
        goto client_uds_exit;
    }


    /* set uds socket nonblocking */
    if ((fl = fcntl(uds_sock, F_GETFL, NULL)) == -1) {
        sleep(1);
        goto client_uds_exit;
    }

    fl |= O_NONBLOCK;

    if (fcntl(uds_sock, F_SETFL, fl) == -1) {
        sleep(1);
        goto client_uds_exit;
    }

    while (client_run == 1) {
        /* ACU로부터 송신요청 수신시 처리 */
        retval = recv_client_packet(g_client_sock);

        if (retval == 0) { /* 연결 끊김 */
            client_run = 0;
            break;
        }

        /* recv from v2x */
        retval = recvfrom(uds_sock, rx_buffer, sizeof(rx_buffer), 0, (struct sockaddr *)&msg_recv_un, (socklen_t*)&namelen);

        /* There is something to send forward to ACU */
        if (retval > 0) {
            /* */
            // printf("o2a v2x rx msgID %d len %d\n", htonl(pheader->msg_id), htonl(pheader->length));
            /* send to ACU */
            if (write(g_client_sock, rx_buffer, retval)) ;
        }

        /* 100us sleep */
        usleep(100);
    }

client_uds_exit:
    /* uds close */
    if (uds_sock > 0) {
        close(uds_sock);
    }

client_tcp_exit:
    /* tcp close */
    if (g_client_sock > 0) {
        close(g_client_sock);
        g_client_sock = -1;
    }

client_exit:
    printf("ACU disconnected\n");

    pthread_exit(NULL);
} /* client_thread */

/**
 * @brief       메인함수
 * @param[in]   argc : 입력 arguement 개수
 * @param[in]   argv : 입력 arguement 값
 * @retval      always 0
 * @author      albert
 * @date        2024-01-08
 */
int main(int argc, char **argv)
{
    int server_sock = -1;
    struct sockaddr_in tcp_sa;
    int ov = 1;
    int fl = 0;
    int addrlen = sizeof(struct sockaddr_in);
    int client_sock = -1;
    struct sockaddr_in addr;
    /* threads tid */
    pthread_t tid;
    pthread_t client_tid;
    int retval = 0;
    int i = 0;

    /* 버전 옵션 검색 */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            print_version();
            exit(0);
        }
    }

    /* create v2x_thread */
    retval = pthread_create(&tid, NULL, v2x_thread, NULL);

    if (retval != 0) {
        printf("v2x_thread create fail\n");
        goto app_exit;
    }

    /* init server socket */

    if ((server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) == -1) {
        printf("server socket create fail\n");
        sleep(1);
        goto app_exit;
    }

    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &ov, sizeof(int)) == -1) {
        printf("setsockopt fail\n");
        sleep(1);
        goto tcp_server_exit;
    }

    memset((char*)&tcp_sa, 0, sizeof(tcp_sa));
    tcp_sa.sin_family = AF_INET;
    tcp_sa.sin_addr.s_addr = htonl(INADDR_ANY);
    tcp_sa.sin_port = htons(SERVER_PORT);

    /* bind */
    if (bind(server_sock, (struct sockaddr *)&tcp_sa, sizeof(tcp_sa)) == -1) {
        sleep(1);
        printf("bind fail\n");
        goto tcp_server_exit;
    }

    /* listen */
    if (listen(server_sock, SOMAXCONN) == -1) {
        sleep(1);
        printf("listen fail\n");
        goto tcp_server_exit;
    }

    /* set tcp socket nonblocking */
    if ((fl = fcntl(server_sock, F_GETFL, NULL)) == -1) {
        sleep(1);
        goto tcp_server_exit;
    }

    fl |= O_NONBLOCK;

    if (fcntl(server_sock, F_SETFL, fl) == -1) {
        sleep(1);
        goto tcp_server_exit;
    }

    printf("TCP socket listen port %d\n", SERVER_PORT);

    while (1) {
        client_sock = accept(server_sock, (struct sockaddr *) &addr, (socklen_t *)&addrlen);

        /* new connection */
        if (client_sock > 0) {
            /* 기존 연결이 살아있음 - 기존 쓰레드 종료 */
            if (g_client_sock > 0) {
                client_run = 0;
                pthread_join(client_tid, NULL);
            }

            /* 새로운 클라이언트 쓰레드 생성 */
            pthread_create(&client_tid, NULL, client_thread, (void *)&client_sock);
        }
        /* 50 msec */
        usleep(1000 * 50);
    }

    /* 기존 연결이 살아있음 - 기존 쓰레드 종료 */
    if (g_client_sock > 0) {
        client_run = 0;
        pthread_join(client_tid, NULL);
    }

tcp_server_exit:
    if (server_sock > 0) {
        close(server_sock);
    }

app_exit:
    printf("app exit\n");

    return 0;
} /* main */

/* end of cre_obu.c */
