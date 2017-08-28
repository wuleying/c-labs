//
// Created by luoliang1 on 2017/8/25.
//

#include <luodb/object/object.h>

void
luoObjectDecrRefcount(void *object) {
    luo_object *obj = object;

    if(--(obj->refcount) == 0) {
        // todo free luodb object
    }
}