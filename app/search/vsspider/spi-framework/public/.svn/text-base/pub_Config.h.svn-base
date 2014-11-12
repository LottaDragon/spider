/**
 * $Id: pub_Config.h,v 1.5 2009/04/27 09:53:28 pengdw Exp $
 */

#ifndef	_pub_CONFIG_H__
#define	_pub_CONFIG_H__

#include "ul_conf.h"

enum cdpType {
    cdp_eNull,
    cdp_eInt,
    cdp_eStr,
};

#define cdp_Required	0x01
#define cdp_XXX		0xFF

typedef struct {
    char *name;
    enum cdpType type;
    void *addr;
    int options;
    char *default_str;
} c_descript;

/**
 * load all c_descript by open file "configfile".
 *
 * 0, ok;
 * else failed.
 */

int load_all_c_descripts(const char *configfile, c_descript* cdp1,
                         int openlogfile = 0, char *defaultlogname = NULL);

/**
 * add a callback funtion when "configfile" is open & parse successfully.
 */

typedef
int (c_descripts_callback_t)(Ul_confdata *allcnf, void *ctx);

int add_c_descripts_callbacks(c_descripts_callback_t *callback, void *ctx);

class ulNewThread {
public:
    ulNewThread(const char *name, int detach);
    ~ulNewThread();

    ulNewThread();
    int Run(const char *name, int detach);

private:
    int ok;
};

#endif	/* ~ _pub_CONFIG_H__ */
