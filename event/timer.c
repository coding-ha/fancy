//
// Created by frank on 17-2-12.
//

#include "base.h"
#include "timer.h"

#define TIMER_INFINITE (timer_msec)-1;

static rbtree      timer;
static rbtree_node sentinel;

static timer_msec timer_recent();
static timer_msec current_msec();

void timer_init()
{
    rbtree_init(&timer, &sentinel);
}

void timer_add(event *ev, timer_msec timeout)
{
    assert(!ev->timer_set);

    timer_msec      key;

    key = timeout + current_msec();

    ev->rb_node.key = key;
    rbtree_insert(&timer, &ev->rb_node);

    assert(!ev->timeout);
    ev->timer_set = 1;
}

void timer_del(event *ev)
{
    assert(ev->timer_set);

    rbtree_delete(&timer, &ev->rb_node);

    ev->timer_set = 0;
    ev->timeout = 0;
}

void timer_expired_process()
{
    timer_msec      current;
    event           *ev;
    rbtree_node     *node;

    current = current_msec();

    while (!rbtree_empty(&timer)) {
        node = rbtree_min(&timer);
        if (current < node->key) {
            break;
        }

        rbtree_delete(&timer, node);

        ev = link_data(node, event, rb_node);
        ev->timer_set = 0;
        ev->timeout = 1;

        ev->handler(ev);
    }
}

void event_and_timer_process()
{
    int         n_ev;

    n_ev = event_process(timer_recent());
    if (n_ev == FCY_ERROR) {
        return;
    }

    timer_expired_process();
}

static timer_msec timer_recent()
{
    rbtree_node *recent;
    timer_msec  current;

    /* no event is waiting */
    if (rbtree_empty(&timer)) {
        return TIMER_INFINITE;
    }

    recent = rbtree_min(&timer);
    current = current_msec();
    if (current < recent->key) {
        /* event will be timeout in the future */
        return recent->key - current;
    }

    /* event already timeout */
    return 0;
}

static timer_msec current_msec()
{
    struct timeval now;

    CHECK(gettimeofday(&now, NULL));

    return (timer_msec)now.tv_sec * 1000 + now.tv_usec / 1000;
}