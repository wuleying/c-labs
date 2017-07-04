//
// _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/4.
//

#include <luodb/event/event.h>

luo_event_loop *
luoEventLoopCreate(void) {
    luo_event_loop *event_loop;

    event_loop = luoMalloc(sizeof(*event_loop));

    if (event_loop == NULL) {
        return NULL;
    }

    event_loop->file_event_head    = NULL;
    event_loop->time_event_head    = NULL;
    event_loop->time_event_next_id = 0;
    event_loop->stop               = 0;

    return event_loop;
}

void
luoEventLoopDelete(luo_event_loop *event_loop) {
    luoFree(event_loop);
}

void
luoEventStop(luo_event_loop *event_loop) {
    event_loop->stop = 1;
}

int
luoEventFileCreate(luo_event_loop *event_loop, int fd, int mask, luo_file_proc *file_proc, void *client_data,
                   luo_event_finalizer_proc *finalizer_proc) {
    luo_file_event *file_event;

    file_event = luoMalloc(sizeof(*file_event));

    if (file_event == NULL) {
        return LUO_EVENT_ERR;
    }

    file_event->fd              = fd;
    file_event->mask            = mask;
    file_event->file_proc       = file_proc;
    file_event->finalizer_proc  = finalizer_proc;
    file_event->client_data     = client_data;
    file_event->next            = event_loop->file_event_head;
    event_loop->file_event_head = file_event;

    return LUO_EVENT_OK;
}

void
luoEventFileDelete(luo_event_loop *event_loop, int fd, int mask) {
    luo_file_event *file_event = event_loop->file_event_head;
    luo_file_event *prev       = NULL;

    while (file_event) {
        if (file_event->fd == fd && file_event->mask == mask) {
            if (prev == NULL) {
                event_loop->file_event_head = file_event->next;
            } else {
                prev->next = file_event->next;
            }

            if (file_event->finalizer_proc) {
                file_event->finalizer_proc(event_loop, file_event->client_data);
            }

            luoFree(file_event);
            return;
        }

        prev = file_event;

        file_event = file_event->next;
    }
}

static void
luoEventGetTime(long *seconds, long *milli_seconds) {
    struct timeval tv;

    gettimeofday(&tv, NULL);
    *seconds       = tv.tv_sec;
    *milli_seconds = tv.tv_usec / 1000;
}

static void
luoEventAddMilliSecondsToNow(long long milli_seconds, long *sec, long *ms) {
    long cur_sec, cur_ms, when_sec, when_ms;

    luoEventGetTime(&cur_sec, &cur_ms);

    when_sec = cur_sec + milli_seconds / 1000;
    when_ms  = cur_ms + milli_seconds % 1000;

    while (when_ms >= 1000) {
        when_sec++;
        when_ms -= 1000;
    }

    *sec = when_sec;
    *ms  = when_ms;
}

long long
luoEventTimeCreate(luo_event_loop *event_loop, long long milli_seconds, luo_time_proc *time_proc,
                   void *client_data, luo_event_finalizer_proc *finalizer_proc) {
    luo_time_event *time_event;
    long long      id = event_loop->time_event_next_id++;

    time_event = luoMalloc(sizeof(*time_event));

    if (time_event == NULL) {
        return LUO_EVENT_ERR;
    }

    time_event->id = id;
    luoEventAddMilliSecondsToNow(milli_seconds, &time_event->when_sec, &time_event->when_ms);
    time_event->time_proc      = time_proc;
    time_event->finalizer_proc = finalizer_proc;
    time_event->client_data    = client_data;
    time_event->next           = event_loop->time_event_head;

    event_loop->time_event_head = time_event;

    return id;
}

int
luoEventTimeDelete(luo_event_loop *event_loop, long long id) {
    luo_time_event *time_event;
    luo_time_event *prev = NULL;

    time_event = event_loop->time_event_head;

    while (time_event) {
        if (time_event->id == id) {
            if (prev == NULL) {
                event_loop->time_event_head = time_event->next;
            } else {
                prev->next = time_event->next;
            }

            if (time_event->finalizer_proc) {
                time_event->finalizer_proc(event_loop, time_event->client_data);
            }

            luoFree(time_event);

            return LUO_EVENT_OK;
        }

        prev       = time_event;
        time_event = time_event->next;
    }

    return LUO_EVENT_ERR;
}

static luo_time_event *
luoEventSearchNearestTimer(luo_event_loop *event_loop) {
    luo_time_event *time_event = event_loop->time_event_head;
    luo_time_event *nearest    = NULL;

    while (time_event) {
        if (!nearest || time_event->when_sec < nearest->when_sec ||
            (time_event->when_sec == nearest->when_sec && time_event->when_ms < nearest->when_ms)) {
            nearest = time_event;
        }

        time_event = time_event->next;
    }

    return nearest;
}

int
luoEventProcessEvents(luo_event_loop *event_loop, int flags) {
    int       max_fd    = 0;
    int       num_fd    = 0;
    int       processed = 0;
    long long max_id;
    fd_set    rfds, wfds, efds;

    luo_file_event *file_event = event_loop->file_event_head;
    luo_time_event *time_event;

    LUO_NOT_USED(flags);

    if (!(flags & LUO_EVENT_TIME_EVENTS) && !(flags & LUO_EVENT_FILE_EVENTS)) {
        return 0;
    }

    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&efds);

    // 检查文件事件
    if (flags & LUO_EVENT_FILE_EVENTS) {
        while (file_event) {
            if (file_event->mask & LUO_EVENT_READABLE) {
                FD_SET(file_event->fd, &rfds);
            }

            if (file_event->mask & LUO_EVENT_WRITABLE) {
                FD_SET(file_event->fd, &wfds);
            }

            if (file_event->mask & LUO_EVENT_EXCEPTION) {
                FD_SET(file_event->fd, &efds);
            }

            if (max_fd < file_event->fd) {
                max_fd = file_event->fd;
            }

            num_fd++;
            file_event = file_event->next;
        }
    }

    if (num_fd || ((flags & LUO_EVENT_TIME_EVENTS) && !(flags & LUO_EVENT_DONE_WAIT))) {
        int ret_val;

        luo_time_event *event_test = NULL;

        struct timeval tv, *tv_ptr;

        if ((flags & LUO_EVENT_TIME_EVENTS) && !(flags & LUO_EVENT_DONE_WAIT)) {
            event_test = luoEventSearchNearestTimer(event_loop);
        }

        if (event_test) {
            long now_sec;
            long now_ms;

            luoEventGetTime(&now_sec, &now_ms);

            tv_ptr = &tv;
            tv_ptr->tv_sec = event_test->when_sec - now_sec;

            if (event_test->when_ms < now_ms) {
                tv_ptr->tv_usec = (int) ((event_test->when_ms + 1000) - now_ms) * 1000;
                tv_ptr->tv_sec--;
            } else {
                tv_ptr->tv_usec = (int) (event_test->when_ms - now_ms) * 1000;
            }

        } else {
            if (flags & LUO_EVENT_DONE_WAIT) {
                tv.tv_sec = tv.tv_usec = 0;
                tv_ptr = &tv;
            } else {
                tv_ptr = NULL;
            }
        }

        ret_val = select(max_fd + 1, &rfds, &wfds, &efds, tv_ptr);

        if (ret_val > 0) {
            file_event = event_loop->file_event_head;

            while (file_event) {
                int fd = file_event->fd;

                if ((file_event->mask & LUO_EVENT_READABLE && FD_ISSET(fd, &rfds)) ||
                    (file_event->mask & LUO_EVENT_WRITABLE && FD_ISSET(fd, &wfds)) ||
                    (file_event->mask & LUO_EVENT_EXCEPTION && FD_ISSET(fd, &efds))) {
                    int mask = 0;

                    if (file_event->mask & LUO_EVENT_READABLE && FD_ISSET(fd, &rfds)) {
                        mask |= LUO_EVENT_READABLE;
                    }

                    if (file_event->mask & LUO_EVENT_WRITABLE && FD_ISSET(fd, &wfds)) {
                        mask |= LUO_EVENT_WRITABLE;
                    }

                    if (file_event->mask & LUO_EVENT_EXCEPTION && FD_ISSET(fd, &efds)) {
                        mask |= LUO_EVENT_EXCEPTION;
                    }

                    file_event->file_proc(event_loop, file_event->fd, file_event->client_data, mask);
                    processed++;

                    file_event = event_loop->file_event_head;

                    FD_CLR(fd, &rfds);
                    FD_CLR(fd, &wfds);
                    FD_CLR(fd, &efds);
                } else {
                    file_event = file_event->next;
                }
            }
        }
    }

    // 检查时间事件
    if (flags & LUO_EVENT_TIME_EVENTS) {
        time_event = event_loop->time_event_head;
        max_id     = event_loop->time_event_next_id - 1;

        while (time_event) {
            long      now_sec, now_ms;
            long long id;

            if (time_event->id > max_id) {
                time_event = time_event->next;
                continue;
            }

            luoEventGetTime(&now_sec, &now_ms);

            if (now_sec > time_event->when_sec || (now_sec == time_event->when_sec && now_ms > time_event->when_ms)) {
                int ret_val;

                id = time_event->id;

                ret_val = time_event->time_proc(event_loop, id, time_event->client_data);

                if (ret_val != LUO_EVENT_NOMORE) {
                    luoEventAddMilliSecondsToNow(ret_val, &time_event->when_sec, &time_event->when_ms);
                } else {
                    luoEventTimeDelete(event_loop, id);
                }

                time_event = event_loop->time_event_head;
            } else {
                time_event = time_event->next;
            }
        }
    }

    return processed;
}

int
luoEventWait(int fd, int mask, long long milli_seconds) {
    struct timeval tv;

    fd_set rfds, wfds, efds;
    int    ret_mask = 0;
    int    ret_val;

    tv.tv_sec  = milli_seconds / 1000;
    tv.tv_usec = (int) ((milli_seconds % 1000) * 1000);

    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&efds);

    if (mask & LUO_EVENT_READABLE) {
        FD_SET(fd, &rfds);
    }

    if (mask & LUO_EVENT_WRITABLE) {
        FD_SET(fd, &wfds);
    }

    if (mask & LUO_EVENT_EXCEPTION) {
        FD_SET(fd, &efds);
    }

    if ((ret_val = select(fd + 1, &rfds, &wfds, &efds, &tv)) > 0) {
        if (FD_ISSET(fd, &rfds)) {
            ret_mask |= LUO_EVENT_READABLE;
        }

        if (FD_ISSET(fd, &wfds)) {
            ret_mask |= LUO_EVENT_WRITABLE;
        }

        if (FD_ISSET(fd, &efds)) {
            ret_mask |= LUO_EVENT_EXCEPTION;
        }

        return ret_mask;
    } else {
        return ret_val;
    }
}

void
luoEventMain(luo_event_loop *event_loop) {
    event_loop->stop = 0;

    while (!event_loop->stop) {
        luoEventProcessEvents(event_loop, LUO_EVENT_ALL_EVENTS);
    }
}