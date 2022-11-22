/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef _NIMBLE_NPL_OS_H_
#define _NIMBLE_NPL_OS_H_

#include <stdint.h>
#include <stdbool.h>


// TOCK INCLUDES
#include "alarm.h"
#include "internal/alarm.h"
#include "timer.h"


// #include "event/callback.h"
// #include "mutex.h"
// #include "sema.h"
// #include "ztimer.h"

// #if defined(CPU_FAM_NRF51) || defined(CPU_FAM_NRF52)
// #include "nrf_clock.h"
// #endif


// It seems we end up with a lot of redundant function declarations, maybe
// because this is a .h file? Suppress those warnings.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wredundant-decls"


#ifdef __cplusplus
extern "C" {
#endif







#define BLE_NPL_OS_ALIGNMENT    4

#define BLE_NPL_TIME_FOREVER    UINT32_MAX

typedef uint32_t ble_npl_time_t;
typedef int32_t ble_npl_stime_t;

struct ble_npl_event {
    ble_npl_event_fn* fn;
    void* arg;
};

struct ble_npl_eventq {
    struct ble_npl_event* events[100];
    uint8_t head;
    uint8_t tail;
};

struct ble_npl_callout {
    // ztimer_t timer;
    ble_npl_time_t ticks;
    struct ble_npl_event e;
    // event_queue_t *q;
    alarm_t* alarm;
};

struct ble_npl_mutex {
    // mutex_t mu;
};

struct ble_npl_sem {
    bool cond;
};

static inline bool
ble_npl_os_started(void)
{
    return true;
}

static inline void *
ble_npl_get_current_task_id(void)
{
    // return (void *)(uint32_t)thread_getpid();

    return (void *)(uint32_t)1;
}

static inline void
ble_npl_eventq_init(struct ble_npl_eventq *evq)
{
    // event_queue_init_detached(&evq->q);

    evq->head = 0;
    evq->tail = 99;
}

static inline void
ble_npl_eventq_put(struct ble_npl_eventq *evq, struct ble_npl_event *ev)
{
    // event_post(&evq->q, &ev->e.super);

    evq->events[evq->head] = ev;
    evq->head = (evq->head+1) % 100;
}

static inline struct ble_npl_event *
ble_npl_eventq_get(struct ble_npl_eventq *evq, ble_npl_time_t tmo)
{
    // if (evq->q.waiter == NULL) {
    //     event_queue_claim(&evq->q);
    // }

    // if (tmo == 0) {
    //     return (struct ble_npl_event *)event_get(&evq->q);
    // } else if (tmo == BLE_NPL_TIME_FOREVER) {
    //     return (struct ble_npl_event *)event_wait(&evq->q);
    // } else {
    //     return (struct ble_npl_event *)event_wait_timeout_ztimer(&evq->q,
    //                                                              ZTIMER_MSEC,
    //                                                              (uint32_t)tmo);
    // }

    uint8_t next_up = (evq->tail+1) % 100;

    if (next_up != evq->head) {
        struct ble_npl_event* ret = evq->events[next_up];
        evq->tail = next_up;
        return ret;
    } else if (tmo == BLE_NPL_TIME_FOREVER) {
        // UPDATE
        return NULL;
    } else {
        // UPDATE
        return NULL;
    }
}

static inline void
ble_npl_eventq_remove(__attribute__ ((unused)) struct ble_npl_eventq *evq, __attribute__ ((unused)) struct ble_npl_event *ev)
{
    // event_cancel(&evq->q, &ev->e.super);
}

static inline void
ble_npl_event_run(struct ble_npl_event *ev)
{
    // ev->e.super.handler(&ev->e.super);

    ev->fn(ev->arg);
}

static inline void
ble_npl_event_init(struct ble_npl_event *ev, ble_npl_event_fn *fn, void *arg)
{
    // /*
    //  * Need to clear list_node manually since init function below does not do
    //  * this.
    //  */
    // ev->e.super.list_node.next = NULL;
    // event_callback_init(&ev->e, (void(*)(void *))fn, ev);
    // ev->arg = arg;

    ev->fn = fn;
    ev->arg = arg;
}

static inline bool
ble_npl_event_is_queued(__attribute__ ((unused)) struct ble_npl_event *ev)
{
    // return (ev->e.super.list_node.next != NULL);

    return true;
}

static inline void *
ble_npl_event_get_arg(struct ble_npl_event *ev)
{
    return ev->arg;
}

static inline void
ble_npl_event_set_arg(struct ble_npl_event *ev, void *arg)
{
    ev->arg = arg;
}



////////////////////////////////////////////////////////////////////////////////
/// MUTEX
////////////////////////////////////////////////////////////////////////////////

static inline ble_npl_error_t
ble_npl_mutex_init(__attribute__ ((unused)) struct ble_npl_mutex *mu)
{
    // mutex_init(&mu->mu);
    return BLE_NPL_OK;
}

static inline ble_npl_error_t
ble_npl_mutex_pend(__attribute__ ((unused)) struct ble_npl_mutex *mu, __attribute__ ((unused)) ble_npl_time_t timeout)
{
    // assert(timeout == BLE_NPL_TIME_FOREVER);
    // (void)timeout;

    // mutex_lock(&mu->mu);
    return BLE_NPL_OK;
}

static inline ble_npl_error_t
ble_npl_mutex_release(__attribute__ ((unused)) struct ble_npl_mutex *mu)
{
    // mutex_unlock(&mu->mu);
    return BLE_NPL_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// Semaphore
///
/// Only used for receiving HCI acks.
////////////////////////////////////////////////////////////////////////////////

#define DRIVER_NUM_BLE_HCI 0x876
#define TOCK_SUBSCRIBE_BLE_HCI_ACK 1

static void tock_hci_upcall_ack(__attribute__ ((unused)) int   callback_type,
                       __attribute__ ((unused)) int   arg1,
                       __attribute__ ((unused)) int   arg2,
                       void* callback_args) {
    // All we have to do in this call to recognize an acknowledgement from the
    // controller is set our condition variable to true.
    struct ble_npl_sem* sem = (struct ble_npl_sem*) callback_args;
    sem->cond = true;
}

static inline ble_npl_error_t
ble_npl_sem_init(struct ble_npl_sem *sem, __attribute__ ((unused)) uint16_t tokens)
{
    // Set our condition variable to false to start with.
    sem->cond = false;

    // Setup a callback for the controller to notify us of ACKs.
    subscribe_return_t subval = subscribe(DRIVER_NUM_BLE_HCI, TOCK_SUBSCRIBE_BLE_HCI_ACK, tock_hci_upcall_ack, (void*) sem);
    if (!subval.success) {
        return -1;
    }

    return BLE_NPL_OK;
}

static inline ble_npl_error_t
ble_npl_sem_release(struct ble_npl_sem *sem)
{
    // Mark the condition variable set as a response to an upcall.
    sem->cond = true;

    return BLE_NPL_OK;
}

static inline uint16_t
ble_npl_sem_get_count(struct ble_npl_sem *sem)
{
    // We only do 0/1 for our semaphore.
    if (sem->cond) {
        return 1;
    } else {
        return 0;
    }
}

static inline void
ble_npl_callout_stop(struct ble_npl_callout *co)
{
    // ztimer_remove(ZTIMER_MSEC, &co->timer);

    // int alarm_at(uint32_t reference, uint32_t dt, subscribe_upcall, void*, alarm_t *alarm);

    // * \brief Cancels an existing alarm.
    //  *
    //  * The caller is responsible for freeing the `alarm_t`.
    //  *
    //  * \param alarm

    // void alarm_cancel(alarm_t*);

    // /** \brief Get the current counter value of the timer.
    //  * \return The current value of the underlying clock.
    //  */
    // uint32_t alarm_read(void);


    alarm_cancel(co->alarm);
}

static inline bool
ble_npl_callout_is_active(__attribute__ ((unused)) struct ble_npl_callout *c)
{
    // return ztimer_is_set(ZTIMER_MSEC, &c->timer);

    return true;
}

static inline ble_npl_time_t
ble_npl_callout_get_ticks(struct ble_npl_callout *co)
{
    return co->ticks;
}

static inline void
ble_npl_callout_set_arg(struct ble_npl_callout *co, void *arg)
{
    co->e.arg = arg;
}

static inline ble_npl_time_t
ble_npl_time_get(void)
{
    // return (ble_npl_time_t)ztimer_now(ZTIMER_MSEC);

    uint32_t now;
    alarm_internal_read(&now);

    return (ble_npl_time_t) now;
}

static inline ble_npl_error_t
ble_npl_time_ms_to_ticks(uint32_t ms, ble_npl_time_t *out_ticks)
{
    // *out_ticks = ms;

    uint32_t frequency;
    alarm_internal_frequency(&frequency);
    *out_ticks = (ms / 1000) * frequency + (ms % 1000) * (frequency / 1000);

    return BLE_NPL_OK;
}

static inline ble_npl_error_t
ble_npl_time_ticks_to_ms(ble_npl_time_t ticks, uint32_t *out_ms)
{
    // *out_ms = ticks;

    uint32_t frequency;
    alarm_internal_frequency(&frequency);
    *out_ms = (ticks * 1000) / frequency;

    return BLE_NPL_OK;
}

static inline ble_npl_time_t
ble_npl_time_ms_to_ticks32(uint32_t ms)
{
    uint32_t frequency;
    alarm_internal_frequency(&frequency);
    return (ms / 1000) * frequency + (ms % 1000) * (frequency / 1000);
}

static inline uint32_t
ble_npl_time_ticks_to_ms32(ble_npl_time_t ticks)
{
    uint32_t frequency;
    alarm_internal_frequency(&frequency);
    return (ticks * 1000) / frequency;
}

static inline void
ble_npl_time_delay(ble_npl_time_t ticks)
{
    uint32_t ms = ble_npl_time_ticks_to_ms32(ticks);
    delay_ms(ms);
}

static inline uint32_t
ble_npl_hw_enter_critical(void)
{
    // return (uint32_t)irq_disable();

    return 0;
}

static inline void
ble_npl_hw_exit_critical(__attribute__ ((unused)) uint32_t ctx)
{
    // irq_restore((unsigned)ctx);
}

static inline bool
ble_npl_hw_is_in_critical(void)
{
    // return (bool)!irq_is_enabled();

    return false;
}

// /* XXX: these functions are required to build hal_timer.c, however with the
// *       default configuration they are never used... */
// #if defined(CPU_FAM_NRF51) || defined(CPU_FAM_NRF52)
// static inline void
// nrf52_clock_hfxo_request(void)
// {
//     clock_hfxo_request();
// }

// static inline void
// nrf52_clock_hfxo_release(void)
// {
//     clock_hfxo_release();
// }
// #endif







#ifdef __cplusplus
}
#endif



#pragma GCC diagnostic pop

#endif  /* _NPL_H_ */
