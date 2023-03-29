#include <igris/osinter/ctrobj.h>

void ctrobj_set_future(struct ctrobj *obj, intptr_t future)
{
    obj->future = future;
}

void ctrobj_init(struct ctrobj *obj, uint8_t type)
{
    obj->type = type;
    obj->future = 0;
    dlist_init(&obj->lnk);
}

void ctrobj_deinit(struct ctrobj *obj)
{
    dlist_del_init(&obj->lnk);
}

intptr_t ctrobj_get_future(struct ctrobj *obj)
{
    return obj->future;
}