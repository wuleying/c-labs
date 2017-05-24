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

struct luo_event_loop_s;

typedef void luo_file_proc_t(struct luo_event_loop_s *event_loop, int fd, void *client_data, int mask);

typedef int luo_time_proc_t(struct luo_event_loop_s *event_loop, long long id, void *client_data);

typedef void luo_event_finalizer_proc_t(struct luo_event_loop_s *event_loop, void *client_data);

typedef struct luo_file_event_s {
    int                        fd;
    int                        mask;
    luo_file_proc_t            *file_proc;
    luo_event_finalizer_proc_t *finalizer_proc;
    void                       *client_data;
    struct luo_file_event_s    *next;

} luo_file_event_t;

typedef struct luo_time_event_s {
    long long                  id;
    long                       when_sec;
    long                       when_ms;
    luo_time_proc_t            *time_proc;
    luo_event_finalizer_proc_t *finalizer_proc;
    void                       *client_data;
    struct luo_time_event_s    *next;

} luo_time_event_t;

typedef struct luo_event_loop_s {
    long long        time_event_next_id;
    luo_file_event_t *file_event_head;
    luo_time_event_t *time_event_head;
    int              stop;

} luo_event_loop_t;

/* 常量 */
#define LUO_EVENT_OK            0
#define LUO_EVENT_ERR           1

#define LUO_EVENT_READABLE      1
#define LUO_EVENT_WRITABLE      2
#define LUO_EVENT_EXCEPTION     4

#define LUO_EVENT_FILE_EVENTS   1
#define LUO_EVENT_TIME_EVENTS   2
#define LUO_EVENT_ALL_EVENTS    (LUO_EVENT_FILE_EVENTS | LUO_EVENT_TIME_EVENTS)
#define LUO_EVENT_DONE_WAIT     4

#define LUO_EVENT_NOMORE        -1

#endif //LUODB_EVENT_H
