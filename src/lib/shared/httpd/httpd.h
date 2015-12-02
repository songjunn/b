#ifndef _HTTPD_H_
#define _HTTPD_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdlib.h>

enum httpd_event {
    HTTP_REQUEST = 100,
};

struct httpd_server;
struct httpd_request;

typedef int(*httpd_handler_t)(struct httpd_request *, enum httpd_event);

// This structure contains information about HTTP request.
struct httpd_request {
    const char *method;         // "GET", "POST", etc
    const char *uri;            // URL-decoded URI
    const char *version;        // E.g. "1.0", "1.1"
    const char *query_string;   // URL part after '?', not including '?', or NULL

    int remote_sock;            // Client's socket
    char remote_ip[48];         // Max IPv6 string length is 45 characters
    unsigned short remote_port; // Client's port

    struct httpd_header {
        const char *name;         // HTTP header name
        const char *value;        // HTTP header value
    } headers[30];
    unsigned short num_headers; // Number of HTTP headers

    char *body;                 // POST data, or NULL
    size_t body_len;            // Data length

    int status_code;            // HTTP status code for HTTP error handler
    httpd_handler_t handler;    // HTTP request handler
};

struct httpd_server {
    int sock;
    unsigned short port;
    httpd_handler_t handler;
};

//httd functions
void httpd_start(void* param); //param: struct httpd_server*
struct httpd_server* httpd_create_server(unsigned short port, httpd_handler_t handler);
int httpd_get_post_var(struct httpd_request *message, const char* name, char* buf, int buf_len);
size_t httpd_send_data(struct httpd_request *message, const void *data, int data_len);

//util functions
int httpd_get_param(const char* content, const char* name, char* buf, int buf_len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_HTTPD_H_
