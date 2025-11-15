/* Server */
#include "Server.h"
char html[2048];
char css[2048];
char js[2048];
char fav[2048];
long file_size = 0;
void read_file(char *filename, char *buffer, int mode) {
  FILE *file;
  if (mode == 0) {
    file = fopen(filename, "r");
  } else {
    file = fopen(filename, "rb");
  }
  if (file == NULL) {
    fprintf(stderr, "Error: Cannot open file %s\n", filename);
    return;
  }

  fseek(file, 0, SEEK_END);
  file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  fread(buffer, 1, file_size, file);
  if (mode == 0) {
    buffer[file_size] = '\0';
  }
  fclose(file);
}

void http_response(int client_soc, char *file, char *response) {

  if (strcmp(file, "image/x-icon") == 0) {
    char header[2048];
    int header_len = snprintf(header, sizeof(header),
                              "HTTP/1.1 200 OK\r\n"
                              "Content-Type: %s\r\n"
                              "Content-Length: %ld\r\n"
                              "Connection: close\r\n"
                              "\r\n",
                              file, file_size);

    // Write header
    write(client_soc, header, header_len);
    // Write binary favicon data
    write(client_soc, response, file_size);

    return;
  }

  char buff[2056];
  memset(buff, 0, 2056);
  int l = strlen(response);
  int len = snprintf(buff, 2055,
                     "HTTP/1.1 200 OK\r\n"
                     "Content-Type: %s\r\n"
                     "Content-Length: %d\r\n"
                     "Connection: close\r\n"
                     "\r\n"
                     "%s",
                     file, l, response);

  write(client_soc, buff, len);
}

void http_header(int client_soc, char *message, int code) {
  char buff[2056];
  char *status_text = "Error";
  memset(buff, 0, 2056);
  int l = strlen(message);
  int len = snprintf(buff, 2055,
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

char *client_read(int client_soc) {
  static char buff[2056];
  memset(buff, 0, 2056);
  int z = read(client_soc, buff, 2055);
  if (z < 0) {

    fprintf(stderr, "Error in reading http %d : %s\n", errno, strerror(errno));
    return buff;
  }
  printf("%s", buff);
  return buff;
}

void client_connection(int serv_soc, int client_soc) {
  char *p;
  Req *req = (Req *)malloc(sizeof(Req));
  memset(req, 0, sizeof(Req));
  char *response;

  p = client_read(client_soc);
  if (!p) {
    fprintf(stderr, "Error in connecting to client %d : %s\n", errno,
            strerror(errno));
    close(client_soc);
    return;
  }
  parsehttp(p, req);
  if (!req) {
    fprintf(stderr, "Failed to parse Http request %d : %s\n", errno,
            strerror(errno));
    close(client_soc);
    return;
  }
  printf("METHOD : %s\nURL : %s\n\n", req->method, req->url);
  if (!strcmp(req->method, "GET")) {
    if (!strcmp(req->url, "/app/webpage")) {
      response = html;
      http_response(client_soc, "text/html", response);
    } else if (!strcmp(req->url, "/app/styles.css")) {
      response = css;
      http_response(client_soc, "text/css", response);
    } else if (!strcmp(req->url, "/app/script.js")) {
      response = js;
      http_response(client_soc, "application/javascript", response);
    } else if (!strcmp(req->url, "/favicon.ico")) {
      response = fav;
      http_response(client_soc, "image/x-icon", response);
    }

  } else {
    response = "404 - File not found";
    http_header(client_soc, response, 404);
  }
  close(client_soc);
  free(req);
  return;
}

void parsehttp(char *str, Req *req) {
  char *p = strchr(str, ' ');
  memcpy(req->method, str, p - str);
  memcpy(req->url, p + 1, (strchr(p + 1, ' ') - p - 1));
}

int client_accept(int s) {
  int c;
  struct sockaddr_in addr;
  socklen_t addr_len = sizeof(addr);
  memset(&addr, 0, sizeof(addr));
  c = accept(s, (struct sockaddr *)&addr, &addr_len);
  if (c < 0) {
    return -1;
  }

  return c;
}

int serv_init(int port) {
  int s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0) {
    fprintf(stderr, "Error %d : %s\n", errno, strerror(errno));
    return -1;
  }
  struct sockaddr_in serv_addr;
  serv_addr.sin_addr.s_addr = inet_addr(IP);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  if ((bind(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0) {
    fprintf(stderr, "Error %d : %s\n", errno, strerror(errno));
    close(s);
    return -1;
  }

  if (listen(s, 10) < 0) {
    fprintf(stderr, "Error %d : %s\n", errno, strerror(errno));
    close(s);
    return -1;
  }
  printf("Listening on %s on port %d\n", IP, port);
  return s;
}

int main(int argc, char *argv[]) {
  int client_soc;
  if (argc != 3) {
    fprintf(stderr, "Usage %s <LISTENING_PORT> <HTML_SUPPLY>\n", argv[0]);
    return -1;
  }

  read_file("/home/IhitplayzYT/webdev_learn/Counter/index.html", html, 0);
  read_file("/home/IhitplayzYT/webdev_learn/Counter/styles.css", css, 0);
  read_file("/home/IhitplayzYT/webdev_learn/Counter/script.js", js, 0);
  read_file("/home/IhitplayzYT/webdev_learn/Counter/favicon.ico", fav, 1);
  printf("%s", js);
  int port = atoi(argv[1]);
  int serv_soc = serv_init(port);
  if (serv_soc < 0) {
    fprintf(stderr, "Error occured in Server Init\n");
    return -1;
  }

  while (true) {
    client_soc = client_accept(serv_soc);
    if (client_soc < 0) {
      fprintf(stderr, "Error occured in Accept\n%d:%s", errno, strerror(errno));
      continue;
    }
    printf("Incoming connection\n");
    int pid = fork();
    if (pid == 0) {
      client_connection(serv_soc, client_soc);
      exit(0);
    }
  }
  close(serv_soc);
  close(client_soc);
  return 0;
}
