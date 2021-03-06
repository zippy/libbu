/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_THREAD_H
#define BU_THREAD_H

#include "bu/config.h"
#include <pthread.h>

#include "bu/exceptionbase.h"

namespace Bu
{
    subExceptionDecl( ThreadException );
    class ThreadId
    {
    friend class Thread;
    private:
        ThreadId( pthread_t tId );

    public:
        ThreadId();

        bool operator==( const ThreadId &rhs );
        bool operator!=( const ThreadId &rhs );

    private:
        pthread_t tId;
    };

    /**
     * Simple thread class.  This wraps the basic pthread (posix threads)
     * system in an object oriented sort of way.  It allows you to create a
     * class with standard member variables and callable functions that can be
     * run in it's own thread, one per class instance.
     *@ingroup Threading
     */
    class Thread
    {
    public:
        /**
         * Construct an Thread thread.
         */
        Thread();

        /**
         * Destroy an Thread thread.
         */
        virtual ~Thread();

        static ThreadId currentThread();
        ThreadId getId() { return ThreadId( ptHandle ); }

        /**
         * Begin thread execution.  This will call the overridden run function,
         * which will simply execute in it's own thread until the function
         * exits, the thread is killed, or the thread is cancelled (optionally).
         * The thread started in this manner has access to all of it's class
         * variables, but be sure to protect possible multiple-access with
         * ThreadMutex objects.
         * @returns True if starting the thread was successful.  False if
         * something went wrong and the thread has not started.
         */
        bool start();

        /**
         * Forcibly kill a thread.  This is not generally considered a good
         * thing to do, but in those rare cases you need it, it's invaluable.
         * The problem with stopping (or killing) a thread is that it stops it
         * the moment you call stop, no matter what it's doing.  The object
         * oriented approach to this will help clean up any class variables
         * that were used, but anything not managed as a member variable will
         * probably create a memory leak type of situation.  Instead of stop,
         * consider using cancel, which can be handled by the running thread in
         * a graceful manner.
         *@returns True if the thread was stopped, false otherwise.  When this
         * function returns the thread may not have stopped, to ensure that the
         * thread has really stopped, call join.
         */
        bool stop();

        /**
         * Join the thread in action.  This function performs what is commonly
         * called a thread join.  That is that it effectively makes the calling
         * thread an the Thread thread contained in the called object one in the
         * same, and pauses the calling thread until the called thread exits.
         * That is, when called from, say, your main(), mythread.join() will
         * not return until the thread mythread has exited.  This is very handy
         * at the end of programs to ensure all of your data was cleaned up.
         *@returns True if the thread was joined, false if the thread couldn't
         * be joined, usually because it isn't running to begin with.
         */
        bool join();

    private:
        pthread_t ptHandle; /**< Internal handle to the posix thread. */

    protected:
        /**
         * The workhorse of the Thread class.  This is the function that will
         * run in the thread, when this function exits the thread dies and is
         * cleaned up by the system.  Make sure to read up on Bu::Mutex,
         * Bu::Condition, and the Bu::Synchro* classes to see how to control
         * and protect everything you do in a safe way within this function.
         *@returns I'm not sure right now, but this is the posix standard form.
         */
        virtual void run()=0;

        /**
         * Gives up some cpu-time in the currently running thread.  If a thread
         * is working hard, but you want to give other threads on the system
         * some time to do some work, call yield.
         */
        void yield();

    private:
        /**
         * This is the hidden-heart of the thread system.  While run is what the
         * user gets to override, and everything said about it is true, this is
         * the function that actually makes up the thread, it simply calls the
         * run member function in an OO-friendly way.  This is what allows us to
         * use member variables from within the thread itself.
         *@param pThread Should always be this.
         *@returns This is specified by posix, I'm not sure yet.
         */
        static void *threadRunner( void *pThread );
    };
}

#endif
