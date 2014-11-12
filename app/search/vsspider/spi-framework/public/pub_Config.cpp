#include <stdio.h>
#include <stdlib.h>

#include "ul_conf.h"
#include "ul_log.h"
#include "ul_thr.h"

#include "pub_Config.h"
#include "misc.h"

ul_logstat_t g_logstat;

c_descripts_callback_t *c_callback;
void *c_callback_ctx;

int add_c_descripts_callbacks(c_descripts_callback_t *callback, void *ctx)
{
    if (c_callback)
	return -1;

    c_callback = callback;
    c_callback_ctx = ctx;

    return 0;
}

int
load_all_c_descripts(const char *configfile, c_descript* cdp1, int openlogfile, char*defaultlogname) {
    Ul_confdata *buckets;

    // init buckets
    buckets = ul_initconf(0);
    if (!buckets) {
        WARNING("ul_initconf failed, \"%s\"", configfile);
        return -1;
    }

    // load ...
    if (access (configfile, F_OK | R_OK) != 0) {
        WARNING("configfile \"%s\" access failed, %m", configfile);
        return -1;
    }

    char tmpdir[PATH_MAX];
    char tmpfilename[PATH_MAX];

    char *lastslash = strrchr(configfile, '/');
    if (!lastslash)
        strlcpy(tmpdir, "./", sizeof(tmpdir));
    else {
        int pathLen = lastslash - configfile + 1;
        if (pathLen >= (int) sizeof(tmpdir)) {
            WARNING("configfile \"%s\" PATH Length(%d) > %d, failed.", configfile, pathLen, (int) sizeof(tmpdir));
            return -1;
        }

        strlcpy(tmpdir, configfile, pathLen + 1);
    }

    if (!lastslash)
        strlcpy(tmpfilename, configfile, sizeof(tmpfilename));
    else {
        lastslash ++;
        int nameLen = strlen(lastslash);

        if (nameLen <= 0 || nameLen >= (int) sizeof(tmpfilename)) {
            WARNING("configfile \"%s\" FILE Length(%d) > %d, failed.", configfile, nameLen, (int) sizeof(tmpfilename));
            return -1;
        }

        strlcpy(tmpfilename, lastslash, nameLen + 1);
    }

    if (ul_readconf(tmpdir, tmpfilename, buckets) < 0) {
        WARNING("load configfile \"%s\" {\"%s\", \"%s\"} failed, %m", configfile, tmpdir, tmpfilename);
        return -1;
    }

    c_descript *cdp = cdp1;
    int tmpInt;
    char tmpStr[LINE_MAX];

    while (cdp && cdp->name) {
        switch (cdp->type) {
        case cdp_eInt:
            tmpInt = 0;
            if (!ul_getconfint(buckets, cdp->name, &tmpInt)) {
                if (cdp->options & cdp_Required) {
                    WARNING("cdp {name \"%s\", type %d} Required, but not found!", cdp->name, cdp->type);
                    return -1;
                }
                // else keep it as it is ...
            } else *(int*)cdp->addr = tmpInt;
            break;
        case cdp_eStr:
            *tmpStr = '\0';
            if (!ul_getconfstr(buckets, cdp->name, tmpStr)) {
                if (cdp->options & cdp_Required) {
                    WARNING("cdp {name \"%s\", type %d} Required, but not found!", cdp->name, cdp->type);
                    return -1;
                }

                if (cdp->default_str)
                    strcpy ((char *)cdp->addr, cdp->default_str);

                // keep it as it is ...
            } else strcpy ((char *)cdp->addr, tmpStr);
            break;
        default:
            WARNING("Unknown cdp->type, %d, check it!!!", cdp->type);
            return -1;
            break;
        }

        cdp ++;
    }

    // callback
    if (c_callback) {
	if ((*c_callback)(buckets, c_callback_ctx) == -1) {
	    WARNING("custom callback failed, {c_callback %p, c_callback_ctx %p}.",
		    c_callback, c_callback_ctx);
	    return -1;
	}
    }

    // logfile
    int loglevel = UL_LOG_NOTICE,
	logsize = 2048;

    if (openlogfile) {
        if (!ul_getconfstr(buckets, "LOG_PATH", tmpdir))
            strlcpy(tmpdir, "./log/", sizeof(tmpdir));

        if (!ul_getconfstr(buckets, "LOG_NAME", tmpfilename)) {
            if (!defaultlogname || *defaultlogname == '\0') {
                WARNING("call load_all_c_descripts with defaultlogname empty when openlogfile is true!!!");
                return -1;
            }

            strlcpy(tmpfilename, defaultlogname, sizeof(tmpfilename));
        }

        if (!ul_getconfint(buckets, "LOG_LEVEL", &loglevel))
            loglevel = UL_LOG_NOTICE;

        if (!ul_getconfint(buckets, "LOG_SIZE", &logsize))
            logsize = 2048;
    }

    ul_freeconf(buckets);

    if (openlogfile) {
        g_logstat.events = loglevel;
        g_logstat.spec = 0;
        g_logstat.to_syslog = 0;

        if (ul_openlog(tmpdir, tmpfilename, &g_logstat, logsize)) {
            WARNING("ul_openlog failed {\"%s\", \"%s\", %d, %d}, %m", tmpdir, tmpfilename, loglevel, logsize);
            return -1;
        }

        ul_threadlog_sup();
    }

    return 0;
}

ulNewThread::ulNewThread(const char *name, int detach) {
    ul_openlog_r(name, &g_logstat);

    if (detach) {
        ul_pthread_detach(pthread_self());
    }

    ok = 1;
}

ulNewThread::ulNewThread()
{
    ok = 0;
}

int ulNewThread::Run(const char *name, int detach)
{
    if (ok) return 0;
    ul_openlog_r(name, &g_logstat);

    if (detach) {
        ul_pthread_detach(pthread_self());
    }

    return ok = 1;
}

ulNewThread::~ulNewThread() {
    if (ok) {
        ul_closelog_r(0);
    }
}
