#ifndef _PLUGIN_HTTPSERVEHANDLER_H_
#define _PLUGIN_HTTPSERVEHANDLER_H_

#include "pluginHandler.h"
#include "plugin_httpServe.h"
#include "PaymentVerifyModule.h"

class CHttpServeHandler : public CPluginHandler
{
public:
	static int httpserve_ev_handler(struct mg_connection *conn, enum mg_event ev) {
		if (ev == MG_REQUEST) {
			//mg_send_header(conn, "Content-Type", "text/plain");
			//mg_printf_data(conn, "This is a reply from server instance # %s", (char *)conn->server_param);
			if (PaymentVerifyModule.OnChargeFromSWServer(conn)) {
				mg_send_data(conn, "0", 1);
			}
			else {
				mg_send_data(conn, "1", 1);
			}

			return MG_TRUE;
		}
		else if (ev == MG_AUTH) {
			return MG_TRUE;
		}
		else {
			return MG_FALSE;
		}
	}
};

#endif //_PLUGIN_HTTPSERVEHANDLER_H_
