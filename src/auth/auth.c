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

static gchar *auth_get_mac()
{
    struct ifreq ifreq;
    int sfd;

    sfd=socket(AF_INET,SOCK_STREAM,0);
    if ( sfd<0 ) {
        g_message("%s[%d]", __func__, __LINE__);
        return NULL;
    }

    strcpy(ifreq.ifr_name, "usb0");
    if (ioctl(sfd,SIOCGIFHWADDR, &ifreq)<0){
        g_message("%s[%d]", __func__, __LINE__);
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

int auth()
{
    xmlChar buf[1024*4] = {0};
    xmlDocPtr doc = NULL;
    xmlNodePtr curNode = NULL;
    size_t len = 0;
    xmlChar *tmpStr = NULL;

    int err = 0;

    FILE *fp = popen("rsautl -verify -inkey pub.pem -pubin -in Phascan.cert", "r");

    if (NULL == fp) {
        err = -1;
        goto auth_err;
    }

    len = fread(buf, 1, sizeof(buf), fp);

    fclose(fp);

    if (len <= 0) {
        err = -2;
        goto auth_err;
    }

    doc = xmlParseMemory(buf, len);
    if (NULL == doc) {
        err = -3;
        goto auth_err;
    }

    curNode = xmlDocGetRootElement(doc);
    if (NULL == curNode) {
        err = -4;
        xmlFreeDoc(doc);
        goto auth_err;
    }

    xmlAttrPtr attrPtr = curNode->properties;
    for (; attrPtr; attrPtr = attrPtr->next) {
        if (!xmlStrcmp(attrPtr->name, BAD_CAST"ID")) {
            tmpStr = xmlGetProp(curNode, "ID");
            g_message("ID:%s", tmpStr);
            xmlFree(tmpStr);
        }
    }

    gchar *mac = auth_get_mac();
    g_message("mac:%s", mac);
    g_free(mac);

    curNode = curNode->children;
    for (; curNode; curNode = curNode->next) {
        if (!xmlStrcmp(curNode->name, BAD_CAST"AuthMode")) {
            tmpStr = xmlNodeGetContent(curNode);
            g_message("AuthMode:%s", tmpStr);
            xmlFree(tmpStr);
        } else if (!xmlStrcmp(curNode->name, BAD_CAST"AuthData")) {
            tmpStr = xmlNodeGetContent(curNode);
            g_message("AuthData:%s", tmpStr);
            xmlFree(tmpStr);
        }
    }


    xmlFreeDoc(doc);

    return err;

auth_err:
    g_warning("Verify failed[%d]\n", err);
    return err;
}
