//
// _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/4.
//

#ifndef LUODB_EVENT_H
#define LUODB_EVENT_H

#include <luodb/common/macro.h>
#include <luodb/memory/mem.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

struct luo_event_loop_s;

typedef void luo_file_proc(struct luo_event_loop_s *event_loop, int fd, void *client_data, int mask);

typedef int luo_time_proc(struct luo_event_loop_s *event_loop, long long id, void *client_data);

typedef void luo_event_finalizer_proc(struct luo_event_loop_s *event_loop, void *client_data);

typedef struct luo_file_event_s {
    int                      fd;
    int                      mask;
    luo_file_proc            *file_proc;
    luo_event_finalizer_proc *finalizer_proc;
    void                     *client_data;
    struct luo_file_event_s  *next;

} luo_file_event;

typedef struct luo_time_event_s {
    long long                id;
    long                     when_sec;
    long                     when_ms;
    luo_time_proc            *time_proc;
    luo_event_finalizer_proc *finalizer_proc;
    void                     *client_data;
    struct luo_time_event_s  *next;

} luo_time_event;

typedef struct luo_event_loop_s {
    long long      time_event_next_id;
    luo_file_event *file_event_head;
    luo_time_event *time_event_head;
    int            stop;

} luo_event_loop;

/* 常量 */
#define LUO_EVENT_OK            0
#define LUO_EVENT_ERR           -1

#define LUO_EVENT_READABLE      1
#define LUO_EVENT_WRITABLE      2
#define LUO_EVENT_EXCEPTION     4

#define LUO_EVENT_FILE_EVENTS   1
#define LUO_EVENT_TIME_EVENTS   2
#define LUO_EVENT_ALL_EVENTS    (LUO_EVENT_FILE_EVENTS | LUO_EVENT_TIME_EVENTS)
#define LUO_EVENT_DONE_WAIT     4

#define LUO_EVENT_NOMORE        -1

/* 接口 */
luo_event_loop *luoEventLoopCreate(void);

void luoEventLoopDelete(luo_event_loop *event_loop);

void luoEventStop(luo_event_loop *event_loop);

int luoEventFileCreate(luo_event_loop *event_loop, int fd, int mask, luo_file_proc *file_proc, void *client_data,
                       luo_event_finalizer_proc *finalizer_proc);

void luoEventFileDelete(luo_event_loop *event_loop, int fd, int mask);

long long luoEventTimeCreate(luo_event_loop *event_loop, long long milli_seconds, luo_time_proc *time_proc,
                             void *client_data, luo_event_finalizer_proc *finalizer_proc);

int luoEventTimeDelete(luo_event_loop *event_loop, long long id);

int luoEventProcessEvents(luo_event_loop *event_loop, int flags);

int luoEventWait(int fd, int mask, long long milli_seconds);

void luoEventMain(luo_event_loop *event_loop);

#endif //LUODB_EVENT_H
