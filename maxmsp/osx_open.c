#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <unistd.h>

#include "ext.h"
#include "ext_obex.h"

#define CMD_PREFIX     "/usr/bin/open "
#define CMD_PREFIX_LEN 13
#define NULL_TERM      1
#define PREFIX_LEN     7
#define QUOTES         2

typedef struct _osx_open
{
    t_object ob;
} t_osx_open;

static t_class *s_osx_open_class;

void ext_main(void *);
void osx_open(t_osx_open *, t_symbol *);
void *osx_open_new();

void *
osx_open_new()
{
        t_osx_open *x = (t_osx_open *)object_alloc(s_osx_open_class);
        
        return (void *)x;
}

void
ext_main(void *r)
{
        t_class *c;
        
        c = class_new("osx_open", (method)osx_open_new, (method)NULL,
            sizeof(t_osx_open), 0L, 0);
        class_addmethod(c, (method)osx_open, "open", A_SYM, 0);
        
        class_register(CLASS_BOX, c);
        
        s_osx_open_class = c;
}


void
osx_open(t_osx_open *x, t_symbol *als_path)
{
        char cmd[CMD_PREFIX_LEN + MAXPATHLEN + NULL_TERM + QUOTES];
        char *path;
        
        if (strlen(als_path->s_name) > MAXPATHLEN + PREFIX_LEN) {
                error("Path too long: must be < %d", MAXPATHLEN);
                return;
        }
        
        strtok(als_path->s_name, ":");
        path = strtok(NULL, ":");
        
        if (!(path)) {
                error("Not a valid path!");
                return;
        }

        if ((access(path, F_OK) == -1) || (access(path, R_OK) == -1)) {
                error("Bad project path");
                return;
        }
        
        post("Opening file %s...", path);

        snprintf(cmd, CMD_PREFIX_LEN + MAXPATHLEN + QUOTES + NULL_TERM,
            "%s\"%s\"", CMD_PREFIX, path);
        system(cmd);
}
