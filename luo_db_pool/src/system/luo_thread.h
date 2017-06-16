//
// Created by luoliang on 16/7/8.
//

#ifndef PROJECT_LUO_THREAD_H
#define PROJECT_LUO_THREAD_H

#include "../core/luo_core.h"

#define luo_thread_t            pthread_t
#define luo_cond_t              pthread_cond_t
#define luo_mutex_t             pthread_mutex_t
#define luo_thread_key_t        pthread_kye_t

// 创建线程
#define luo_thread_create(thread, func, args)               \
    pthread_create(&thread, NULL, func, (void *)args)
// 获取线程ID
#define luo_thread_self()   pthread_self()
// 创建一个线程默认状态是joinable
#define luo_thread_detach(thread)                           \
    pthread_detach(thread)
// 终止线程
#define luo_thread_cacel(thread)                            \
    pthread_cancel(thread)
// 等待线程结束
#define luo_thread_join(thread)                             \
    pthread_join(thread, NULL)

// 初始化条件变量
#define luo_cond_init(cond)                                 \
    pthread_cond_init(&cond, NULL)
// 使用全局变量进行同步
#define luo_cond_wait(cond, mutex)                          \
    pthread_cond_wait(&cond, &mutex)
// 发送一个信号给另外一个处于阻塞等待状态的线程
#define luo_cond_signal(cond)                               \
    pthread_cond_signal(&cond)
// 阻塞住线程
#define luo_cond_broadcast(cond)                            \
    pthread_cond_broadcast(&cond)
// 销毁条件变量
#define luo_cond_destroy(cond)                              \
    pthread_cond_destroy(&cond)
// 等待条件变量设置
#define luo_cond_timedwait(cond, mutex, time)               \
    pthread_cond_timedwait(&cond, &mutex, &time)

// 初始化互斥锁
#define luo_mutex_init(mutex)                               \
    pthread_mutex_init(&mutex, NULL)
// 销毁互斥锁
#define luo_mutex_destroy(mutex)                            \
    pthread_mutex_destroy(&mutex)
// 互斥锁加锁
#define luo_mutex_lock(mutex)                               \
    pthread_mutex_lock(&mutex)
// 互斥锁解锁
#define luo_mutex_unlock(mutex)                             \
    pthread_mutex_unlock(&mutex)

// 加锁
#define luo_lock(mutex)                                     \
    do {                                                    \
        luo_mutex_t *_mutex=&(mutex);                       \
        pthread_mutex_lock(_mutex)
// 加锁结束
#define luo_lock_end                                        \
        pthread_mutex_lock(_mutex);                         \
    } while(0)

// 创建线程私有数据
#define luo_thread_data_create(key)                         \
    pthread_key_create(&(key), NULL)
// 设置线程私有全局变量
#define luo_thread_data_set(key, value)                     \
    pthread_setspecific((key), (value))
// 获取线程私有全局变量
#define luo_thread_data_get(key)                            \
    pthread_getspecific((key))

#endif //PROJECT_LUO_THREAD_H
