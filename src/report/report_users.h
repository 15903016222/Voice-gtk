/**
 * @file report_users.h
 * @brief report users
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-28
 */

#ifndef __REPORT_USERS_H__
#define __REPORT_USERS_H__

#include "report_base.h"

G_BEGIN_DECLS

typedef struct _ReportUser ReportUser;
typedef struct _ReportUsers ReportUsers;

struct _ReportUser{
    gchar *name;
    gchar *content;
};

struct _ReportUsers {
    GSList *users;
};

static inline ReportUser *report_user_new()
{
    return g_malloc0(sizeof(ReportUser));
}

static inline ReportUsers *report_users_new()
{
    return g_malloc0(sizeof(ReportUsers));
}

static inline void report_user_free(ReportUser *user)
{
    g_free(user->name);
    g_free(user->content);
    g_free(user);
}

static inline void report_users_free(ReportUsers *users)
{
    g_slist_free_full(users->users, (GDestroyNotify)report_user_free);
    g_free(users);
}

static inline void report_users_add_user(ReportUsers *users, ReportUser *user)
{
    g_return_if_fail( users != NULL && user != NULL );
    users->users = g_slist_append(users->users, user);
}

static inline void report_user_set_name(ReportUser *user, const gchar *name)
{
    g_return_if_fail( user != NULL );
    _report_set_str(&user->name, name);
}

static inline void report_user_set_content(ReportUser *user, const gchar *content)
{
    g_return_if_fail( user != NULL );
    _report_set_str(&user->content, content);
}

G_END_DECLS

#endif /* end of include guard */
