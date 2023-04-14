#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include "mongoose.h"

LASTINPUTINFO lii;

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    if (ev == MG_EV_HTTP_MSG)
    {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;

        if (mg_http_match_uri(hm, "/"))
        {
            const char html[] =
                "<html>"
                "<head><title>user_exporter</title></head>"
                "<body>"
                "<h1>user_exporter</h1>"
                "<p><a href=\"/metrics\">Metrics</a></p>"
                "<p><i>(version=" USER_IDLE_EXPORTER_VERSION ")</i></p>"
                "</body>"
                "</html>";

            mg_http_reply(c, 200, "Content-Type: text/html\r\n", html);
        }
        else if (mg_http_match_uri(hm, "/metrics"))
        {
            int ret = GetLastInputInfo(&lii);
            int seconds = (GetTickCount() - lii.dwTime) / 1000;

            const char text[] =
                "# HELP user_exporter_build_info User exporter build info\n"
                "# TYPE user_exporter_build_info gauge\n"
                "user_idle_exporter_build_info{version=\"" USER_IDLE_EXPORTER_VERSION "\"} 1\n"
                "# HELP user_idle_time User idle time\n"
                "# TYPE user_idle_time gauge\n"
                "user_idle_seconds %d\n"
                "# HELP user_idle_error User idle time retrieval error \n"
                "# TYPE user_idle_error gauge\n"
                "user_idle_error %d\n";

            mg_http_reply(c, 200, "Content-Type: text/plain\r\n", text, seconds, ret == 0);
        }
        else
        {
            mg_http_reply(c, 404, NULL, "404\n");
        }
    }
}

int main()
{
    lii.cbSize = sizeof(LASTINPUTINFO);

    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, USER_IDLE_EXPORTER_LISTEN_ADDR, fn, &mgr);
    printf("Listening " USER_IDLE_EXPORTER_LISTEN_ADDR "\n");

    for (;;)
        mg_mgr_poll(&mgr, 1000);

    return 0;
}

