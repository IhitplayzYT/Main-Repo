/* Toraliser */
#include "Toraliser.h"

Sent *Request(char *d_ip, int d_port)
{
    Sent *request = (Sent *)malloc(sizeof(Sent));
    request->vn = 4;
    request->cd = 1;
    request->dst_port = htons(d_port);
    request->dst_ip = inet_addr(d_ip);
    strcpy(request->username, Username);
    return request;
}

int main(int argc, char *argv[])
{
    int serv_port;
    char *serv_ip;

    if (argc == 1)
    {
        fprintf(stderr, "Usage format is : %s <server_ip[NECESSARY]> <server_port[OPTIONAL]>\n", argv[0]);
        perror("Reason ");
        return -1;
    }
    else if (argc == 2)
    {
        if (strlen(argv[1]) < 2)
        {
            printf("Please input server ip instead of server port\n");
            return -1;
        }
        else
        {
            printf("Default port will be used: HTTP(80)\n");
            serv_port = default_port;
            serv_ip = argv[1];
        }
    }
    else if (argc == 3)
    {
        serv_ip = argv[1];
        serv_port = atoi(argv[2]);
    }
    else
    {
        fprintf(stderr, "Usage format is : %s <server_ip[NECESSARY]> <server_port[OPTIONAL]>\n", argv[0]);
        perror("Reason ");
        return -1;
    }
    int soc = socket(AF_INET, SOCK_STREAM, 0);
    if (soc < 0)
    {
        fprintf(stderr, "Socket wasnt able to be created\n");
        perror("Reason ");
        close(soc);
        return -2;
    }

    struct sockaddr_in socadd;
    socadd.sin_addr.s_addr = inet_addr(tor_proxy_addr);
    socadd.sin_port = htons(tor_proxy_port);
    socadd.sin_family = AF_INET;

    if ((connect(soc, (const struct sockaddr *)&socadd, sizeof(socadd))) == 0)
    {
        printf("Successful connection to TOR PROXY SERVER!\n");
    }
    else
    {
        fprintf(stderr, "Failed to connect the socket to the tor proxy server\n");
        perror("Reason ");
        return -3;
    }

    char buff_reciev[64];
    Sent *send_packet = Request(serv_ip, serv_port);
    write(soc, send_packet, sizeof(Sent));
    memset(buff_reciev, 0, 64);
    read(soc, buff_reciev, sizeof(buff_reciev));
    Recieved *reciev = (Recieved *)buff_reciev;
    if (reciev->cd != 90)
    {
        fprintf(stderr, "Handshake failed!\n");
        free(send_packet);
        close(soc);
        return -4;
    }
    printf("Successfull handshake\n");
    char buff[4096];
    memset(buff, 0, 4096);

    //
    snprintf(buff, 4095,
             "HEAD / HTTP/1.0\r\n"
             "Host: www.archlinux.org\r\n"
             "\r\n");
    write(soc, buff, strlen(buff));
    memset(buff, 0, 4096);
    read(soc, buff, 4095);
    printf("%s\n", buff);

    free(send_packet);
    close(soc);
    //

    return 0;
}