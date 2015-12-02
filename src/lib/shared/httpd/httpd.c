#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "ThreadLib.h"
#include "httpd.h"

#define SERVER_STRING "Server: httpd/0.1.0\r\n"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum httpd_error {
    HTTPD_ERROR_BAD_REQUEST = 400,
    HTTPD_ERROR_NOT_FOUND = 404,
    HTTPD_ERROR_INTERNAL_SERVER_ERROR = 500,
    HTTPD_ERROR_METHOD_NOT_IMPLEMENTED = 501,
};

int start_net(int port);
void worker_thread(void* param);
void handle_request_file(int client, const char *url);
void send_file(int client, FILE *resource);
void send_headers_ok(int client);
void send_headers_err(int client, int error);

void parse_httpd_message(char *buf, size_t len, struct httpd_request *message);
void parse_httpd_headers(char **buf, struct httpd_request *message);
char *skip(char **buf, const char *delimiters);
int is_valid_httpd_method(const char *s);
void remove_double_dots_and_double_slashes(char *s);
int url_decode(const char *src, size_t src_len, char *dst, size_t dst_len, int is_form_url_encoded);

#ifdef __cplusplus
}
#endif // __cplusplus

/**********************************************************************/
/* A request has caused a call to accept() on the server port to
 * return.  Process the request on the worker's thread.
 * Parameters: the struct httpd_request instance of the client */
/**********************************************************************/
void worker_thread(void* param) {
    char buf[1024] = {0};
    struct httpd_request* message = (struct httpd_request*)param;
    int numchars = 0;

    numchars = recv(message->remote_sock, buf, sizeof(buf), 0);

    printf("httpd recv connect: %s\n", message->remote_ip);
    printf("httpd recv message: %s\n", buf);

    if (numchars > 0) {
        // parse http message
        parse_httpd_message(buf, numchars, message);

        if (!strcasecmp(message->method, "POST")) {
            message->handler(message, HTTP_REQUEST);
        } else if (!strcasecmp(message->method, "GET")) {
            handle_request_file(message->remote_sock, message->uri);
        }
    }

    close(message->remote_sock);
    delete message;
}

/**********************************************************************/
/* Parse http message to struct httpd_request.
 * Parameters: http message buffer and length
 *             struct httpd_request for save parsing result */
/**********************************************************************/
void parse_httpd_message(char *buf, size_t len, struct httpd_request *message) {
    int is_request, n;

    if (len < 1) return;

    // RFC says that all initial whitespaces should be ignored
    while (*buf != '\0' && isspace(*(unsigned char *)buf)) {
        buf++;
    }

    message->method = skip(&buf, " ");
    message->uri = skip(&buf, " ");
    message->version = skip(&buf, "\r\n");

    // HTTP message could be either HTTP request or HTTP response, e.g.
    // "GET / HTTP/1.0 ...." or  "HTTP/1.0 200 OK ..."
    is_request = is_valid_httpd_method(message->method);
    if ((is_request && memcmp(message->version, "HTTP/", 5) != 0) ||
        (!is_request && memcmp(message->method, "HTTP/", 5) != 0)) {
        len = ~0;
    } else {
        if (is_request) {
            message->version += 5;
        } else {
            message->status_code = atoi(message->uri);
        }
        parse_httpd_headers(&buf, message);

        if ((message->query_string = strchr(message->uri, '?')) != NULL) {
            *(char *)message->query_string++ = '\0';
        }
        n = (int)strlen(message->uri);
        url_decode(message->uri, n, (char *)message->uri, n + 1, 0);
        if (*message->uri == '/' || *message->uri == '.') {
            remove_double_dots_and_double_slashes((char *)message->uri);
        }

        if (*buf != '\0') {
            message->body = buf;
            message->body_len = strlen(buf);
        }
    }
}

/**********************************************************************/
/* Parse HTTP headers from the given buffer, advance buffer to the point
 * where parsing stopped. */
/**********************************************************************/
void parse_httpd_headers(char **buf, struct httpd_request *message) {
    size_t i;

	for (i = 0; i < sizeof(message->headers) / sizeof(message->headers[0]); i++) {
        message->headers[i].name = skip(buf, ": ");
        message->headers[i].value = skip(buf, "\r\n");
        if (message->headers[i].name[0] == '\0')
            break;
        message->num_headers = i + 1;
    }
}

/**********************************************************************/
/* Skip the characters until one of the delimiters characters found.
 * 0-terminate resulting word. Skip the rest of the delimiters if any.
 * Advance pointer to buffer to the next word. Return found 0-terminated word. */
/**********************************************************************/
char *skip(char **buf, const char *delimiters) {
    char *p, *begin_word, *end_word, *end_delimiters;

    begin_word = *buf;
    end_word = begin_word + strcspn(begin_word, delimiters);
    end_delimiters = end_word + strspn(end_word, delimiters);

    if (end_word == end_delimiters) {
        return end_word;
    }

    for (p = end_word; p < end_delimiters; p++) {
        *p = '\0';
    }

    *buf = end_delimiters;

    return begin_word;
}

/**********************************************************************/
/* Check if the method is a valid htttpd method. */
/**********************************************************************/
int is_valid_httpd_method(const char *s) {
	return !strcmp(s, "GET") || !strcmp(s, "POST") || !strcmp(s, "HEAD") ||
		!strcmp(s, "CONNECT") || !strcmp(s, "PUT") || !strcmp(s, "DELETE") ||
		!strcmp(s, "OPTIONS") || !strcmp(s, "PROPFIND") || !strcmp(s, "MKCOL") ||
		!strcmp(s, "PATCH");
}

/**********************************************************************/
/* Url decode. */
/**********************************************************************/
int url_decode(const char *src, size_t src_len, char *dst,
	size_t dst_len, int is_form_url_encoded) {
	size_t i, j = 0;
	int a, b;
#define HEXTOI(x) (isdigit(x) ? (x) - '0' : (x) - 'W')

	for (i = j = 0; i < src_len && j < dst_len - 1; i++, j++) {
		if (src[i] == '%' && i + 2 < src_len &&
			isxdigit(*(const unsigned char *)(src + i + 1)) &&
			isxdigit(*(const unsigned char *)(src + i + 2))) {
			a = tolower(*(const unsigned char *)(src + i + 1));
			b = tolower(*(const unsigned char *)(src + i + 2));
			dst[j] = (char)((HEXTOI(a) << 4) | HEXTOI(b));
			i += 2;
		}
		else if (is_form_url_encoded && src[i] == '+') {
			dst[j] = ' ';
		}
		else {
			dst[j] = src[i];
		}
	}

	dst[j] = '\0'; // Null-terminate the destination

	return i >= src_len ? j : -1;
}

/**********************************************************************/
/* Protect against directory disclosure attack by removing '..',
 * excessive '/' and '\' characters */
/**********************************************************************/
void remove_double_dots_and_double_slashes(char *s) {
	char *p = s;

	while (*s != '\0') {
		*p++ = *s++;
		if (s[-1] == '/' || s[-1] == '\\') {
			// Skip all following slashes, backslashes and double-dots
			while (s[0] != '\0') {
				if (s[0] == '/' || s[0] == '\\') { s++; }
				else if (s[0] == '.' && (s[1] == '/' || s[1] == '\\')) { s += 2; }
				else if (s[0] == '.' && s[1] == '.' && s[2] == '\0') { s += 2; }
				else if (s[0] == '.' && s[1] == '.' && (s[2] == '/' || s[2] == '\\')) { s += 3; }
				else { break; }
			}
		}
	}
	*p = '\0';
}

/**********************************************************************/
/* Parse parameter from format string.
 * like "action=1&platform=0&guest=test" */
/**********************************************************************/
int httpd_get_param(const char* content, const char* name, char* buf, int buf_len) {
    const char *begin_word;
    int value_len;

    begin_word = strstr(content, name) + strlen(name) + 1; // exclude '='
    value_len = strcspn(begin_word, "&");
    if (buf_len > value_len) {
        strncpy(buf, begin_word, value_len);
        buf[value_len] = '\0';
    }
    return value_len;
}

/**********************************************************************/
/* Get post param for httpd request. */
/**********************************************************************/
int httpd_get_post_var(struct httpd_request *message, const char* name, char* buf, int buf_len) {
    return httpd_get_param(message->body, name, buf, buf_len);
}

/**********************************************************************/
/* Send response to the request.
 * Parameters: httpd request
 *             send data and length */
/**********************************************************************/
size_t httpd_send_data(struct httpd_request *message, const void *data, int data_len) {
	return send(message->remote_sock, data, data_len, 0);
}

/**********************************************************************/
/* Send a regular file to the client. 
 * Use headers, and report errors to client if they occur.
 * Parameters: client's socket
 *             request url */
/**********************************************************************/
void handle_request_file(int client, const char *url) {
	FILE *resource = NULL;
	struct stat st;
	char path[1024];

	sprintf(path, "htdocs%s", url);
	if (path[strlen(path) - 1] == '/') {
		strcat(path, "index.html");
	}
	if (stat(path, &st) == -1) {
		send_headers_err(client, HTTPD_ERROR_NOT_FOUND);
	} else {
		if ((st.st_mode & S_IFMT) == S_IFDIR)
			strcat(path, "/index.html");

		/*if ((st.st_mode & S_IXUSR) ||
			(st.st_mode & S_IXGRP) ||
			(st.st_mode & S_IXOTH))*/

		resource = fopen(path, "r");
		if (resource == NULL) {
			send_headers_err(client, HTTPD_ERROR_NOT_FOUND);
		} else {
			send_headers_ok(client);
			send_file(client, resource);
			fclose(resource);
		}
	}
}

/**********************************************************************/
/* Put the entire contents of a file out on a socket.  This function
 * is named after the UNIX "cat" command, because it might have been
 * easier just to do something like pipe, fork, and exec("cat").
 * Parameters: the client socket descriptor
 *             FILE pointer for the file to cat */
/**********************************************************************/
void send_file(int client, FILE *resource) {
	char buf[1024];

	fgets(buf, sizeof(buf), resource);
	while (!feof(resource)) {
		send(client, buf, strlen(buf), 0);
		fgets(buf, sizeof(buf), resource);
	}
}

/**********************************************************************/
/* Return the informational HTTP headers.
 * Parameters: the socket to print the headers on */
/**********************************************************************/
void send_headers_ok(int client) {
    char buf[1024];

    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    send(client, buf, strlen(buf), 0);
	sprintf(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
/* Return the error HTTP headers.
 * Parameters: the socket to print the headers on 
 *             the error number */
/**********************************************************************/
void send_headers_err(int client, int error) {
    char buf[1024];

    sprintf(buf, "HTTP/1.0 %d\r\n", error);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
/* This function starts the process of listening for web connections
 * on a specified port.
 * Parameters: pointer to variable containing the port to connect on
 * Returns: the socket */
/**********************************************************************/
int start_net(int port) {
    int sock = 0;
    struct sockaddr_in name;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		printf("httpd create socket error\n");
		return -1;
	}

    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    
	if (bind(sock, (struct sockaddr *)&name, sizeof(name)) < 0) {
		printf("httpd bind socket error\n");
		return -1;
	}

	if (listen(sock, SOMAXCONN) < 0) {
		printf("httpd listen socket error\n");
		return -1;
	}

	return sock;
}

/**********************************************************************/
/* Create struct httpd_server instance.
 * Parameters: the port to listen on
 *             http message handler function */
/**********************************************************************/
struct httpd_server* httpd_create_server(unsigned short port, httpd_handler_t handler) {
	struct httpd_server* httpd = new struct httpd_server;
	httpd->port = port;
	httpd->handler = handler;
	return httpd;
}

/**********************************************************************/
/* Httpd main thread.
 * Parameters: struct httpd_server instance */
/**********************************************************************/
void httpd_start(void* param) {
    int client_sock = -1;
	struct sockaddr_in client_addr;
	struct httpd_server* server = (struct httpd_server *)param;
	socklen_t client_addr_len = sizeof(client_addr);

	server->sock = start_net(server->port);
	if (server->sock == -1) {
		printf("httpd start net failed\n");
		return;
	}
	printf("httpd running on port %d\n", server->port);

	while (1) {
		client_sock = accept(server->sock,
			(struct sockaddr *)&client_addr,
			&client_addr_len);

		if (client_sock == -1) {
			printf("httpd accept socket error\n");
			continue;
		}

        struct httpd_request* client = new struct httpd_request;
        client->remote_sock = client_sock;
        client->handler = server->handler;

		sockaddr_in * pRemote = (sockaddr_in*)&client_addr;
		sprintf(client->remote_ip, inet_ntoa(pRemote->sin_addr));

        if (ThreadLib::Create(worker_thread, (void*)client) == 0) {
			printf("httpd create thread error\n");
			continue;
		}
	}

	close(server->sock);
	delete server;
}
