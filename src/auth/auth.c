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
#include <pthread.h>

typedef struct _Auth Auth;

struct _Auth {
    AuthMode mode;
    time_t data;
};

static Auth *auth = NULL;
pthread_rwlock_t  rwlock = PTHREAD_RWLOCK_INITIALIZER;

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

static size_t _read_cert(const gchar *certFile, const gchar *pubPem, gchar *buf, size_t s)
{
    size_t len = 0;
    gchar *cmd = g_strdup_printf("rsautl -verify -inkey %s -pubin -in %s", pubPem, certFile);
    FILE *fp = popen(cmd, "r");
    g_free(cmd);
    if (NULL == fp) {
        return 0;
    }
    len = fread(buf, 1, s, fp);
    fclose(fp);
    return len;
}

void auth_init(const gchar *certFile, const gchar *pubPem, const gchar *serialNo)
{
    g_return_if_fail( NULL != serialNo );

    xmlChar buf[1024*4] = {0};
    xmlDocPtr doc = NULL;
    xmlNodePtr curNode = NULL;
    size_t len = 0;
    gboolean flag = FALSE;

    pthread_rwlock_rdlock(&rwlock);
    if (auth) {
        pthread_rwlock_unlock(&rwlock);
        g_warning("Auth already init");
        return;
    }
    pthread_rwlock_unlock(&rwlock);

    len = _read_cert(certFile, pubPem, buf, sizeof(buf));
    if (len <= 0) {
        g_warning("Read cert file failed");
        goto auth_end1;
    }

    xmlKeepBlanksDefault(0);

    doc = xmlParseMemory(buf, len);
    if (NULL == doc) {
        g_warning("Auth Parse failed");
        goto auth_end1;
    }

    curNode = xmlDocGetRootElement(doc);
    if (NULL == curNode) {
        g_warning("Auth get element failed");
        goto auth_end1;
    }

    pthread_rwlock_wrlock(&rwlock);

    auth = g_malloc0(sizeof(Auth));

    curNode = curNode->children;
    for (; curNode; curNode = curNode->next) {
        if (!xmlStrcmp(curNode->name, BAD_CAST"ID")) {
            xmlChar *tmpStr = xmlNodeGetContent(curNode);

            if ( xmlStrcasecmp(serialNo, tmpStr) ) {
                goto auth_end;
            }
            flag = TRUE;

            xmlFree(tmpStr);
        } else if (!xmlStrcmp(curNode->name, BAD_CAST"AUTH")) {
            auth->mode = _get_mode(curNode);
            auth->data = _get_time(curNode);
        }
    }

auth_end:
    if (! flag) {
        g_free(auth);
        auth = NULL;
    }
    pthread_rwlock_unlock(&rwlock);

auth_end1:
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return;
}

void auth_uninit()
{
    pthread_rwlock_wrlock(&rwlock);
    g_free(auth);
    auth = NULL;
    pthread_rwlock_unlock(&rwlock);
}


AuthMode auth_get_mode()
{
    AuthMode mode = AUTH_MODE_INVALID;
    pthread_rwlock_rdlock(&rwlock);
    if (auth) {
        mode = auth->mode;
    }
    pthread_rwlock_unlock(&rwlock);
    return mode;
}

time_t auth_get_data()
{
    time_t t = 0;
    pthread_rwlock_rdlock(&rwlock);
    if (auth) {
        t = auth->data;
    }
    pthread_rwlock_unlock(&rwlock);
    return t;
}

gboolean auth_is_valid()
{
    gboolean flag = FALSE;
    pthread_rwlock_rdlock(&rwlock);
    if (NULL == auth || AUTH_MODE_INVALID == auth->mode) {
    } else if (AUTH_MODE_NONE == auth->mode) {
        flag = TRUE;
    } else if (AUTH_MODE_RUN == auth->mode
               || AUTH_MODE_CNT == auth->mode) {
        flag = ( auth->data > 0 );
    } else if (AUTH_MODE_DATE == auth->mode) {
        flag = ( auth->data > time(NULL) );
    }
    pthread_rwlock_unlock(&rwlock);
    return flag;
}
