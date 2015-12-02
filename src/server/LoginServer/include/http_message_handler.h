#include "httpd.h"
#include <string>
#include "types.h"

int httpserver_ev_handler(struct httpd_request *conn, enum httpd_event ev);

// guest login, for my db
int64 auth_verify_from_mydb_by_client(const char* guest, char* accesstoken, int accesslen);
int auth_verify_from_mydb_by_gate(int64 userid, const char* accesstoken);
