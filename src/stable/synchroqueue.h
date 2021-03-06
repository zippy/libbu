/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_SYNCHRO_QUEUE_H
#define BU_SYNCHRO_QUEUE_H

#include <pthread.h>

#include "bu/mutex.h"
#include "bu/condition.h"

namespace Bu
{
    /**
     * A thread-safe queue class.  This class is a very simple queue with some
     * cool extra functionality for use with the Synchro system.  The main extra
     * that it provides is the option to either dequeue without blocking, with
     * infinite blocking, or with timed blocking, which will return a value if
     * something is enqueued within the specified time limit, or NULL if the
     * time limit is exceded.
     *@ingroup Threading Containers
     */
    template <class T>
    class SynchroQueue
    {
    private:
        /**
         * Helper struct.  Keeps track of linked-list items for the queue data.
         */
        typedef struct Item
        {
            T pData;
            Item *pNext;
        } Item;

    public:
        /**
         * Construct an empty queue.
         */
        SynchroQueue() :
            pStart( NULL ),
            pEnd( NULL ),
            nSize( 0 )
        {
        }
        
        /**
         * Destroy the queue.  This function will simply free all contained
         * structures.  If you stored pointers in the queue, this will lose the
         * pointers without cleaning up the memory they pointed to.  Make sure
         * you're queue is empty before allowing it to be destroyed!
         */
        ~SynchroQueue()
        {
            cBlock.lock();
            Item *pCur = pStart;
            while( pCur )
            {
                Item *pTmp = pCur->pNext;
                delete pCur;
                pCur = pTmp;
            }
            cBlock.unlock();
        }

        /**
         * Enqueue a pieces of data.  The new data will go at the end of the
         * queue, and unless another piece of data is enqueued, will be the
         * last piece of data to be dequeued.
         *@param pData The data to enqueue.  If this is not a primitive data
         * type it's probably best to use a pointer type.
         */
        void enqueue( T pData )
        {
            cBlock.lock();

            if( pStart == NULL )
            {
                pStart = pEnd = new Item;
                pStart->pData = pData;
                pStart->pNext = NULL;
                nSize++;
            }
            else
            {
                pEnd->pNext = new Item;
                pEnd = pEnd->pNext;
                pEnd->pData = pData;
                pEnd->pNext = NULL;
                nSize++;
            }

            cBlock.signal();

            cBlock.unlock();
        }

        /**
         * Dequeue the first item from the queue.  This function can operate in
         * two different modes, blocking and non-blocking.  In non-blocking
         * mode it will return immediately weather there was data in the queue
         * or not.  If there was data it will remove it from the queue and
         * return it to the caller.
         *
         * In blocking mode it will block forever wating for data to be
         * enqueued.  When data finally is enqueued this function will return
         * immediately with the new data.  The only way this function should
         * ever return a null in blocking mode is if the calling thread was
         * cancelled.  It's probably a good idea to check for NULL return
         * values even if you use blocking, just to be on the safe side.
         *@param bBlock Set to true to enable blocking, leave as false to work
         * in non-blocking mode.
         *@returns The next piece of data in the queue, or NULL if no data was
         * in the queue.
         */
        T dequeue( bool bBlock=false )
        {
            cBlock.lock();
            if( pStart == NULL )
            {
                if( bBlock )
                {
                    cBlock.wait();

                    if( pStart == NULL )
                    {
                        cBlock.unlock();
                        return NULL;
                    }
                    T pTmp = pStart->pData;
                    Item *pDel = pStart;
                    pStart = pStart->pNext;
                    delete pDel;
                    nSize--;
                    
                    cBlock.unlock();
                    return pTmp;

                }

                cBlock.unlock();
                return NULL;
            }
            else
            {
                T pTmp = pStart->pData;
                Item *pDel = pStart;
                pStart = pStart->pNext;
                delete pDel;
                nSize--;
            
                cBlock.unlock();
                return pTmp;
            }
        }

        /**
         * Operates just like the other dequeue function in blocking mode with
         * one twist.  This function will block for at most nSec seconds and
         * nUSec micro-seconds.  If the timer is up and no data is available,
         * this will just return NULL.  If data is enqueued before the timeout
         * expires, it will dequeue and exit immediately.
         *@param nSec The number of seconds to wait, max.
         *@param nUSec The number of micro-seconds to wait, max.
         *@returns The next piece of data in the queue, or NULL if the timeout
         * was exceeded.
         */
        T dequeue( int nSec, int nUSec )
        {
            cBlock.lock();
            if( pStart == NULL )
            {
                cBlock.wait( nSec, nUSec );

                if( pStart == NULL )
                {
                    cBlock.unlock();
                    return NULL;
                }
            
                T pTmp = pStart->pData;
                Item *pDel = pStart;
                pStart = pStart->pNext;
                delete pDel;
                nSize--;
                    
                cBlock.unlock();
                return pTmp;
            }
            else
            {
                T pTmp = pStart->pData;
                Item *pDel = pStart;
                pStart = pStart->pNext;
                delete pDel;
                nSize--;
            
                cBlock.unlock();
                return pTmp;
            }
        }

        /**
         * Checks to see if the queue has data in it or not.  Note that there
         * is no function to determine the length of the queue.  This data
         * isn't kept track of.  If you really need to know, fix this.
         *@returns True if the queue is empty, false if it has data in it.
         */
        bool isEmpty()
        {
            cBlock.lock();
            bool bEmpty = (pStart == NULL );
            cBlock.unlock();

            return bEmpty;
        }

        long getSize()
        {
            cBlock.lock();
            long nRet = nSize;
            cBlock.unlock();

            return nRet;
        }

        void unblockAll()
        {
            cBlock.lock();
            cBlock.broadcast();
            cBlock.unlock();
        }

    private:
        Item *pStart;   /**< The start of the queue, the next element to dequeue. */
        Item *pEnd;     /**< The end of the queue, the last element to dequeue. */
        long nSize;     /**< The number of items in the queue. */

        Condition cBlock;   /**< The condition for blocking dequeues. */
    };
}

#endif
