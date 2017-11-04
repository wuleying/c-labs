//
// Created by luoliang1 on 2017/8/25.
//

#include <luodb/object/object.h>
#include <luodb/config/config.h>

static void
luoObjectFreeString(luo_object *object) {
    luoStrFree(object->ptr);
}

static void
luoObjectFreeList(luo_object *object) {
    luoDListRelease((luo_dlist *) object->ptr);
}

static void
luoObjectFreeSet(luo_object *object) {
    luoDictRelease((luo_dict *) object->ptr);
}

static void
luoObjectFreeHash(luo_object *object) {
    luoDictRelease((luo_dict *) object->ptr);
}


void
luoObjectDecrRefcount(void *object, luo_dlist *object_free_list) {
    luo_object *obj = object;

    if (--(obj->refcount) == 0) {
        switch (obj->type) {
            case LUO_OBJECT_TYPE_STRING :
                luoObjectFreeString(obj);
                break;
            case LUO_OBJECT_TYPE_LIST:
                luoObjectFreeList(obj);
                break;
            case LUO_OBJECT_TYPE_SET:
                luoObjectFreeSet(obj);
                break;
            case LUO_OBJECT_TYPE_HASH:
                luoObjectFreeHash(obj);
                break;
            default:
                assert(false);
                break;
        }

        if (LUO_DLIST_LENGTH(object_free_list) > LUO_OBJECT_FREE_LIST_MAX
            || luoDListAddNodeHead(object_free_list, obj)) {
            luoFree(obj);
        }
    }
}