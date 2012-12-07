#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#include "uv.h"

/* Libuv multiple loops + thread communication example. Written by Kristian
 * Evensen <kristian.evensen@gmail.com>  */

//rpath is needed because it is an argument to the linker (not compiler) about
//where to look
//gcc -Iinclude -g libuv-test.c -o libuv_test -L "./" -l uv -Xlinker -rpath -Xlinker "./" -lrt

void timer_callback(uv_timer_t *handle, int status){
    uv_async_t *other_thread_notifier = (uv_async_t *) handle->data;

    fprintf(stderr, "Timer expired, notifying other thread\n");

    //Notify the other thread
    uv_async_send(other_thread_notifier);
}

void *child_thread(void *data){
    uv_loop_t *thread_loop = (uv_loop_t *) data;
    fprintf(stderr, "Consumer thread will start event loop\n");

    //Start this loop
    uv_run(thread_loop);
    pthread_exit(NULL);
}

void consumer_notify(uv_async_t *handle, int status){
    fprintf(stderr, "Hello from the other thread\n", handle->loop->backend_fd);
}

int main(int argc, char *argv[]){
    pthread_t thread;
    uv_async_t async;

    /* Create and set up the consumer thread */
    uv_loop_t *thread_loop = uv_loop_new();
    uv_async_init(thread_loop, &async, consumer_notify);
    pthread_create(&thread, NULL, child_thread, thread_loop);

    /* Main thread will run default loop */
    uv_loop_t *main_loop = uv_default_loop();
    uv_timer_t timer_req;
    uv_timer_init(main_loop, &timer_req);

    /* Timer callback needs async so it knows where to send messages */
    timer_req.data = &async;
    uv_timer_start(&timer_req, timer_callback, 0, 2000);

    fprintf(stderr, "Starting main loop\n");
    uv_run(main_loop);

    return 0;
}
