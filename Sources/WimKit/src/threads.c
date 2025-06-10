/*
 * threads.c - Thread, mutex, and condition variable support.  Wraps around
 *             pthreads or Windows native threads.
 */

/*
 * Copyright 2016-2023 Eric Biggers
 *
 * This file is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option) any
 * later version.
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this file; if not, see https://www.gnu.org/licenses/.
 */

#include "config.h"

#ifdef _WIN32
#  include "wimlib/win32_common.h"
#else
#  include <errno.h>
#  include <pthread.h>
#endif

#include "wimlib/libwim_assert.h"
#include "wimlib/error.h"
#include "wimlib/threads.h"
#include "wimlib/util.h"

bool libwim_thread_create(struct thread *t, void *(*thrproc)(void *), void *arg)
{
	int err = pthread_create(&t->pthread, NULL, thrproc, arg);

	if (err) {
		errno = err;
		ERROR_WITH_ERRNO("Failed to create thread");
		return false;
	}
	return true;
}

void thread_join(struct thread *t)
{
	int err = pthread_join(t->pthread, NULL);

	wimlib_assert(err == 0);
}

bool mutex_init(struct mutex *m)
{
	int err = pthread_mutex_init(&m->pthread_mutex, NULL);

	if (err) {
		errno = err;
		ERROR_WITH_ERRNO("Failed to initialize mutex");
		return false;
	}
	return true;
}

void mutex_destroy(struct mutex *m)
{
	int err = pthread_mutex_destroy(&m->pthread_mutex);

	wimlib_assert(err == 0);
}

void mutex_lock(struct mutex *m)
{
	int err = pthread_mutex_lock(&m->pthread_mutex);

	wimlib_assert(err == 0);
}

void mutex_unlock(struct mutex *m)
{
	int err = pthread_mutex_unlock(&m->pthread_mutex);

	wimlib_assert(err == 0);
}

bool condvar_init(struct condvar *c)
{
	int err = pthread_cond_init(&c->pthread_cond, NULL);

	if (err) {
		errno = err;
		ERROR_WITH_ERRNO("Failed to initialize condition variable");
		return false;
	}
	return true;
}

void condvar_destroy(struct condvar *c)
{
	int err = pthread_cond_destroy(&c->pthread_cond);

	wimlib_assert(err == 0);
}

void condvar_wait(struct condvar *c, struct mutex *m)
{
	int err = pthread_cond_wait(&c->pthread_cond, &m->pthread_mutex);

	wimlib_assert(err == 0);
}

void condvar_signal(struct condvar *c)
{
	int err = pthread_cond_signal(&c->pthread_cond);

	wimlib_assert(err == 0);
}

void condvar_broadcast(struct condvar *c)
{
	int err = pthread_cond_broadcast(&c->pthread_cond);

	wimlib_assert(err == 0);
}
