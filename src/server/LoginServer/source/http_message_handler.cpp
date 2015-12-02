#include "http_message_handler.h"
#include <string.h>
#include "sqlite3.h"
#include "gtime.h"
#include "logger.h"

int httpserver_ev_handler(struct httpd_request *conn, enum httpd_event ev) 
{
    if (ev == HTTP_REQUEST) {
        LOGGER_DEBUG("[HttpServer]Recv Request:%s content:%s", conn->remote_ip, conn->body);

        char action[8] = {0}, platform[8] = {0};
        httpd_get_post_var(conn, "action", action, sizeof(action));
        httpd_get_post_var(conn, "platform", platform, sizeof(platform));

        if (!strcmp(action, "1") && !strcmp(platform, "0")) {
            char guest[128], accesstoken[64];
            httpd_get_post_var(conn, "guest", guest, sizeof(guest));
            int64 uid = auth_verify_from_mydb_by_client(guest, accesstoken, sizeof(accesstoken));
            
            char response[1024];
            snprintf(response, sizeof(response), "uid=%lld&accesstoken=%s", uid, accesstoken);
            httpd_send_data(conn, response, strlen(response));
            LOGGER_DEBUG("[HttpServer]Send Response:%s %s", conn->remote_ip, response);
        } else if (!strcmp(action, "2") && !strcmp(platform, "0")) {
            char uid[64], accesstoken[64];
            httpd_get_post_var(conn, "userid", uid, sizeof(uid));
            httpd_get_post_var(conn, "accesstoken", accesstoken, sizeof(accesstoken));
            int ret = auth_verify_from_mydb_by_gate(atoll(uid), accesstoken);

            char response[32];
            snprintf(response, sizeof(response), "result=%d", ret);
            httpd_send_data(conn, response, strlen(response));
            LOGGER_DEBUG("[HttpServer]Send Response:%s %s", conn->remote_ip, response);
        }

        /*if (true) {
            httpd_send_data(conn, "0", 1);
        } else {
            httpd_send_data(conn, "1", 1);
        }*/
        return 0;
    } else {
        return 1;
    }
}

int64 auth_verify_from_mydb_by_client(const char* guest, char* accesstoken, int accesslen)
{
    int ret, row, col;
    sqlite3 *db = NULL;
    char *errmsg = NULL, **result, sql[1024];
    uint64 time;
    int64 uid;

    // open database
    ret = sqlite3_open("./auth.db", &db);
    if (ret != SQLITE_OK) {
        LOGGER_ERROR("auth_verify_from_mydb_by_client open error: %s", sqlite3_errmsg(db));
        goto QUIT;
    }

    // check table exist
    snprintf(sql, sizeof(sql), "select count(*) from sqlite_master where type='table' and name='guest'");
    ret = sqlite3_get_table(db, sql, &result, &row, &col, &errmsg);
    if (ret != SQLITE_OK) {
        LOGGER_ERROR("auth_verify_from_mydb_by_client check table exist error: %d, %s", sqlite3_errcode(db), sqlite3_errmsg(db));
        goto QUIT;
    }

    // if not exist then create
    if (atoi(result[1]) <= 0) {
        snprintf(sql, sizeof(sql), "create table guest(uid integer primary key autoincrement, name varchar(128), token varchar(128), time datatime)");
        ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
        if (ret != SQLITE_OK) {
            LOGGER_ERROR("auth_verify_from_mydb_by_client create table error: %d, %s", sqlite3_errcode(db), sqlite3_errmsg(db));
            goto QUIT;
        }
    }

    // check guest if exist
    snprintf(sql, sizeof(sql), "select * from guest where name='%s'", guest);
    ret = sqlite3_get_table(db, sql, &result, &row, &col, &errmsg);
    if (ret != SQLITE_OK) {
        LOGGER_ERROR("auth_verify_from_mydb_by_client get table error: %d, %s", sqlite3_errcode(db), sqlite3_errmsg(db));
        goto QUIT;
    }

    time = GetTimeSec();
    snprintf(accesstoken, accesslen, "%lld", time);

    // if not exist then insert
    if (row <= 0) {
        snprintf(sql, sizeof(sql), "insert into guest (name, token, time) values('%s', '%s', %lld)", guest, accesstoken, time);
        ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
        if (ret != SQLITE_OK) {
            LOGGER_ERROR("auth_verify_from_mydb_by_client insert guest error: %s, %s", guest, sqlite3_errmsg(db));
            goto QUIT;
        }

        snprintf(sql, sizeof(sql), "select * from guest where name='%s'", guest);
        ret = sqlite3_get_table(db, sql, &result, &row, &col, &errmsg);
        if (ret != SQLITE_OK) {
            LOGGER_ERROR("auth_verify_from_mydb_by_client get table error: %d, %s", sqlite3_errcode(db), sqlite3_errmsg(db));
            goto QUIT;
        }
    } else {
        snprintf(sql, sizeof(sql), "update guest set token='%s', time=%lld where name='%s'", accesstoken, time, guest);
        ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
        if (ret != SQLITE_OK) {
            LOGGER_ERROR("auth_verify_from_mydb_by_client update guest error: %s, %s", guest, sqlite3_errmsg(db));
            goto QUIT;
        }
    }

    uid = atoll(result[col]);

QUIT:
    if (db) sqlite3_close(db);
    if (errmsg) sqlite3_free(errmsg);

    return uid;
}

int auth_verify_from_mydb_by_gate(int64 userid, const char* accesstoken)
{
    int ret, row, col, response = 0;
    sqlite3 *db = NULL;
    char *errmsg = NULL, **result, sql[1024];
    int64 nowtime, tokentime;

    // open database
    ret = sqlite3_open("./auth.db", &db);
    if (ret != SQLITE_OK) {
        LOGGER_ERROR("auth_verify_from_mydb_by_gate open error: %s", sqlite3_errmsg(db));
        goto QUIT;
    }
    
    // verify guest's accesstoken
    snprintf(sql, sizeof(sql), "select token,time from guest where uid=%lld", userid);
    ret = sqlite3_get_table(db, sql, &result, &row, &col, &errmsg);
    if (ret != SQLITE_OK) {
        LOGGER_ERROR("auth_verify_from_mydb_by_gate get table error: %d, %s", sqlite3_errcode(db), sqlite3_errmsg(db));
        goto QUIT;
    }

    nowtime = GetTimeSec();
    tokentime = atoll(result[col+1]);

    if (row > 0 && result[col] != NULL && result[col+1] != NULL) {
        if (!strcmp(result[col], accesstoken) && nowtime - tokentime < 3600) {
            response = 1;
        }
    }

QUIT:
    if (db) sqlite3_close(db);
    if (errmsg) sqlite3_free(errmsg);

    return response;
}

