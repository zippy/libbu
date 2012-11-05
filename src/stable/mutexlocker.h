#ifndef BU_MUTEX_LOCKER_H
#define BU_MUTEX_LOCKER_H

namespace Bu
{
    class Mutex;
    class MutexLocker
    {
    public:
        MutexLocker( Mutex &mu );
        virtual ~MutexLocker();

        void unlock();
        void relock();

    private:
        Mutex &mu;
    };
};

#endif
