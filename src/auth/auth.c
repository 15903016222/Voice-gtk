/**
 * @file auth.c
 * @brief authorization
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-05-13
 */

#include "auth.h"

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <net/if.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <string.h>

typedef struct _Auth Auth;

struct _Auth {
    AuthMode mode;
    time_t data;
};

static Auth *auth = NULL;

static gchar *auth_get_mac()
{
    struct ifreq ifreq;
    int sfd;

    sfd=socket(AF_INET,SOCK_STREAM,0);
    if ( sfd<0 ) {
        return NULL;
    }

    strcpy(ifreq.ifr_name, "usb0");
    if (ioctl(sfd,SIOCGIFHWADDR, &ifreq)<0){
        close(sfd);
        return NULL;
    }
    return g_strdup_printf("%02x%02x%02x%02x%02x%02x",
                    (guchar)ifreq.ifr_hwaddr.sa_data[0],
                    (guchar)ifreq.ifr_hwaddr.sa_data[1],
                    (guchar)ifreq.ifr_hwaddr.sa_data[2],
                    (guchar)ifreq.ifr_hwaddr.sa_data[3],
                    (guchar)ifreq.ifr_hwaddr.sa_data[4],
                    (guchar)ifreq.ifr_hwaddr.sa_data[5]);
}

static AuthMode _get_mode(xmlNodePtr node)
{
    xmlChar *tmpStr = xmlGetProp(node, "mode");
    AuthMode mode = AUTH_MODE_INVALID;

    if (NULL == tmpStr) {
        return mode;
    }

    if (!xmlStrcmp(tmpStr, "NONE")) {
        mode = AUTH_MODE_NONE;
    } else if (!xmlStrcmp(tmpStr, "COUNT")) {
        mode = AUTH_MODE_CNT;
    } else if (!xmlStrcmp(tmpStr, "RUN")) {
        mode = AUTH_MODE_RUN;
    } else if (!xmlStrcmp(tmpStr, "DATE")) {
        mode = AUTH_MODE_DATE;
    }

    xmlFree(tmpStr);
    return mode;
}

static time_t _get_time(xmlNodePtr node)
{
    struct tm t;
    xmlChar *dateStr = xmlNodeGetContent(node);

    memset(&t, 0, sizeof(t));

    if (NULL == dateStr) {
        return 0;
    }

    sscanf(dateStr, "%d/%d/%d %d:%d",
           &t.tm_year, &t.tm_mon, &t.tm_mday,
           &t.tm_hour, &t.tm_min);

    xmlFree(dateStr);

    t.tm_year -= 1900;
    t.tm_mon -= 1;
    t.tm_isdst = 1;
//    t.tm_zone = "BST";

    return mktime(&t);
}

static size_t _read_cert(gchar *buf, size_t s)
{
    size_t len = 0;
    FILE *fp = popen("rsautl -verify -inkey pub.pem -pubin -in Phascan.cert", "r");
    if (NULL == fp) {
        return 0;
    }
    len = fread(buf, 1, s, fp);
    fclose(fp);
    return len;
}

void auth_init()
{
    xmlChar buf[1024*4] = {0};
    xmlDocPtr doc = NULL;
    xmlNodePtr curNode = NULL;
    size_t len = 0;

    if (auth) {
        g_warning("Auth already init");
        return;
    }

    len = _read_cert(buf, sizeof(buf));
    if (len <= 0) {
        g_warning("Read cert file failed");
        return;
    }

    xmlKeepBlanksDefault(0);

    doc = xmlParseMemory(buf, len);
    if (NULL == doc) {
        g_warning("Auth Parse failed");
        return;
    }

    curNode = xmlDocGetRootElement(doc);
    if (NULL == curNode) {
        g_warning("Auth get element failed");
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return;
    }

    auth = g_malloc0(sizeof(Auth));

    curNode = curNode->children;
    for (; curNode; curNode = curNode->next) {
        if (!xmlStrcmp(curNode->name, BAD_CAST"MAC")) {
            xmlChar *tmpStr = xmlNodeGetContent(curNode);
            xmlChar *mac = auth_get_mac();

            if ( xmlStrcasecmp(mac, tmpStr) ) {
                g_free(auth);
                auth = NULL;
                goto auth_end;
            }

            xmlFree(tmpStr);
            xmlFree(mac);
        } else if (!xmlStrcmp(curNode->name, BAD_CAST"AUTH")) {
            auth->mode = _get_mode(curNode);
            auth->data = _get_time(curNode);
        }
    }

auth_end:
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return;
}

void auth_uninit()
{
    g_free(auth);
    auth = NULL;
}

AuthMode auth_get_mode()
{
    if (auth) {
        return auth->mode;
    }
    return AUTH_MODE_INVALID;
}

time_t auth_get_data()
{
    if (auth) {
        return auth->data;
    }
    return 0;
}

gboolean auth_is_valid()
{
    if (NULL == auth || AUTH_MODE_INVALID == auth->mode) {
        return FALSE;
    } else if (AUTH_MODE_NONE == auth->mode) {
        return TRUE;
    } else if (AUTH_MODE_RUN == auth->mode) {
    } else if (AUTH_MODE_DATE == auth->mode) {
    } else if (AUTH_MODE_CNT == auth->mode) {
    }
    return FALSE;
}
