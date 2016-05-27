/**
 * @file cert.c
 * @brief certification
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-05-24
 */

#include "cert.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <string.h>

static size_t _cert_read_file(const gchar *certFile, const gchar *pubPem, gchar *buf, size_t s)
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

static CertMode _cert_get_mode(xmlNodePtr node)
{
    xmlChar *tmpStr = xmlGetProp(node, "mode");
    CertMode mode = CERT_MODE_INVALID;

    if (NULL == tmpStr) {
        return mode;
    }

    if (!xmlStrcmp(tmpStr, "NONE")) {
        mode = CERT_MODE_NONE;
    } else if (!xmlStrcmp(tmpStr, "RUN_COUNT")) {
        mode = CERT_MODE_RUNCOUNT;
    } else if (!xmlStrcmp(tmpStr, "RUN_TIME")) {
        mode = CERT_MODE_RUNTIME;
    } else if (!xmlStrcmp(tmpStr, "DATE")) {
        mode = CERT_MODE_DATE;
    }

    xmlFree(tmpStr);
    return mode;
}

static time_t _cert_get_time(xmlNodePtr node)
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

Cert *_cert_load(const xmlChar *buf, gint len)
{
    Cert *cert = NULL;
    xmlDocPtr doc = NULL;
    xmlNodePtr curNode = NULL;

    xmlKeepBlanksDefault(0);

    doc = xmlParseMemory(buf, len);
    if (NULL == doc) {
        g_warning("Cert Parse failed");
        goto _cert_end;
    }

    curNode = xmlDocGetRootElement(doc);
    if (NULL == curNode) {
        g_warning("Cert get root element failed");
        goto _cert_end;
    }

    cert = g_malloc0(sizeof(Cert));
    curNode = curNode->children;
    for (; curNode; curNode = curNode->next) {
        if (!xmlStrcmp(curNode->name, BAD_CAST"ID")) {
            cert->id = xmlNodeGetContent(curNode);
        } else if (!xmlStrcmp(curNode->name, BAD_CAST"AUTH")) {
            cert->mode = _cert_get_mode(curNode);
            if (cert->mode == CERT_MODE_DATE) {
                cert->data = _cert_get_time(curNode);
            } else {
                xmlChar *tmpStr = xmlNodeGetContent(curNode);
                if (tmpStr) {
                    cert->data = atol(tmpStr);
                } else {
                    cert->data = 0;
                }
                xmlFree(tmpStr);
            }

        }
    }

_cert_end:
    xmlFreeDoc(doc);
//    xmlCleanupParser();

    return cert;
}

Cert *cert_load(const gchar *certFile, const gchar *pubPem)
{
    g_return_val_if_fail( NULL != certFile
            || NULL != pubPem, NULL);
    xmlChar buf[1024*4] = {0};
    size_t len = 0;

    len = _cert_read_file(certFile, pubPem, buf, sizeof(buf));
    if (len <= 0) {
        g_warning("Read cert file failed");
        return NULL;
    }

    return _cert_load(buf, len);

}

void cert_free(Cert *c)
{
    g_return_if_fail( NULL != c );
    xmlFree(c->id);
    g_free(c);
}
