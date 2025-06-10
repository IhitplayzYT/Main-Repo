/* Server */
#include "Server.h"
char *html;
char *read_file(char *filename)
{
    FILE *file;
    char *buffer;
    long file_size;

    file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer = (char *)malloc(file_size + 1);
    if (buffer == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';
    fclose(file);
    return buffer;
}

void http_response(int client_soc, char *file, char *response)
{
    char buff[512];
    memset(buff, 0, 512);
    int l = strlen(response);
    int len = snprintf(buff, 511,
                       "HTTP/1.1 200 OK\r\n"
                       "Content-Type: %s\r\n"
                       "Content-Length: %d\r\n"
                       "Connection: close\r\n"
                       "\r\n"
                       "%s",
                       file, l, response);

    write(client_soc, buff, len);
}

void http_header(int client_soc, char *message, int code)
{
    char buff[512];
    char *status_text = "Error";
    memset(buff, 0, 512);
    int l = strlen(message);
    int len = snprintf(buff, 511,
                       "HTTP/1.1 %d %s\r\n"
                       "Content-Type: text/plain\r\n"
                       "Content-Length: %d\r\n"
                       "Connection: close\r\n"
                       "\r\n"
                       "%s",
                       code, status_text, l, message);

    write(client_soc, buff, len);
    return;
}

char *client_read(int client_soc)
{
    static char buff[512];
    memset(buff, 0, 512);
    int z = read(client_soc, buff, 511);
    if (z < 0)
    {

        fprintf(stderr, "Error in reading http %d : %s\n", errno, strerror(errno));
        return buff;
    }
    printf("%s", buff);
    return buff;
}

void client_connection(int serv_soc, int client_soc)
{
    char *p;
    Req *req;
    char *response;

    p = client_read(client_soc);
    if (!p)
    {
        fprintf(stderr, "Error in connecting to client %d : %s\n", errno, strerror(errno));
        close(client_soc);
        return;
    }
    req = parsehttp(p);
    if (!req)
    {
        fprintf(stderr, "Failed to parse Http request %d : %s\n", errno, strerror(errno));
        close(client_soc);
        return;
    }
    printf("METHOD : %s\nURL : %s\n", req->method, req->url);
    if (!strcmp(req->method, "GET") && !strcmp(req->url, "/app/webpage"))
    {
        response = html;
        // response = "<html>hello world</html>";
        http_response(client_soc, "text/html", response);
    }
    else
    {
        response = "404 - File not found";
        http_header(client_soc, response, 404);
    }
    close(client_soc);
    free(req);
    return;
}

Req *parsehttp(char *str)
{
    char *c;
    Req *req = (Req *)malloc(sizeof(Req));
    memset(req, 0, sizeof(Req));

    for (c = str; *c != ' ' && *c; c++)
        ;

    if (*c == ' ')
    {
        *c = '\0';
        c++;
    }
    strncpy(req->method, str, 7);
    str = c;
    for (c; *c != ' ' && *c; c++)
        ;
    if (*c == ' ')
    {
        *c = '\0';
    }
    strncpy(req->url, str, 127);

    return req;
}

int client_accept(int s)
{
    int c;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    memset(&addr, 0, sizeof(addr));
    c = accept(s, (struct sockaddr *)&addr, &addr_len);
    if (c < 0)
    {
        return -1;
    }

    return c;
}

int serv_init(int port)
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
    {
        fprintf(stderr, "Error %d : %s\n", errno, strerror(errno));
        return -1;
    }
    struct sockaddr_in serv_addr;
    serv_addr.sin_addr.s_addr = inet_addr(IP);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if ((bind(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
    {
        fprintf(stderr, "Error %d : %s\n", errno, strerror(errno));
        close(s);
        return -1;
    }

    if (listen(s, 10) < 0)
    {
        fprintf(stderr, "Error %d : %s\n", errno, strerror(errno));
        close(s);
        return -1;
    }
    printf("Listening on %s on port %d\n", IP, port);
    return s;
}

int main(int argc, char *argv[])
{
    int client_soc;
    if (argc != 3)
    {
        fprintf(stderr, "Usage %s <LISTENING_PORT> <HTML_SUPPLY>\n", argv[0]);
        return -1;
    }
    char *path = argv[2];
    html = read_file(path);
    // printf("%s", html);

    int port = atoi(argv[1]);
    int serv_soc = serv_init(port);
    if (serv_soc < 0)
    {
        fprintf(stderr, "Error occured in Server Init\n");
        return -1;
    }

    while (true)
    {
        client_soc = client_accept(serv_soc);
        if (client_soc < 0)
        {
            fprintf(stderr, "Error occured in Accept\n%d:%s", errno, strerror(errno));
            continue;
        }
        printf("Incoming connection\n");
        int pid = fork();
        if (pid == 0)
        {
            client_connection(serv_soc, client_soc);
            exit(0);
        }
    }
    close(serv_soc);
    close(client_soc);
    return 0;
}