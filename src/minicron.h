#ifndef BU_MINICRON_H
#define BU_MINICRON_H

#include "bu/signals.h"
#include "bu/heap.h"
#include "bu/fstring.h"

#include <time.h>

namespace Bu
{
	/**
	 * A simple cron like system designed to be embedded in any program.  This
	 * class creates a simple cron system that can run any number of jobs at
	 * customizable intervals or schedules.  It does not support some of the
	 * more complex scheduling that some cron systems can do such as load
	 * balancing directly, but this could be done on the job side.
	 *
	 * This system is synchronous, it does not use any threads on it's own, but
	 * it is threadsafe, so a cron thread could be created if desired.
	 *
	 * The operation is fairly simple, jobs can be added at any time, and use
	 * any timer they would like, even custom timers.  When it is time for a
	 * job to be run it signals the slot provided when the job was added.  Every
	 * job slot recieves a handle to the job object so that it may control it's
	 * own lifetime and get information about itself.  In addition, every job
	 * is assigned a unique ID that can be used to control it's operation
	 * at any time.
	 *
	 * By default a job will continually reschedule itself after being run
	 * unless it calls stop() on it's job object, it is removed using
	 * removeJob() on the cron object, or it is added with addJobOnce.
	 */
	class MiniCron
	{
	public:
		class Job;
		class Timer;
		typedef Bu::Signal1<void, Bu::MiniCron::Job &> CronSignal;
		typedef int JobId;

		MiniCron();
		virtual ~MiniCron();

		/**
		 * Tells you if there are jobs registered in the MiniCron.
		 *@returns true if there are jobs, false otherwise.
		 */
		virtual bool hasJobs();

		/**
		 * If there are jobs, tells you the time the next one will execute.
		 *@returns The timestamp that the next job will execute at.
		 */
		virtual time_t getNextRun();

		/**
		 * Call this regularly to execute all jobs that should be executed.
		 * This will loop until all jobs who's run time match the current time
		 * or are below the current time (we've missed them).
		 * If there is nothing to run, the runtime of this funcion is constant,
		 * it is very fast.  Otherwise it executes at log(N) per job run,
		 * O(N*log(N)).
		 */
		virtual void poll();

		/**
		 * Add a job for repeated scheduling.  Pass in a slot to signal, and a
		 * Timer object to use to do the scheduling.  This function returns a
		 * JobId which can be used at a later time to control the execution of
		 * the job.
		 */
		virtual JobId addJob( CronSignal sigJob, const Timer &t );

		/**
		 * Add a job for one time scheduling.  Pass in a slot to signal, and a
		 * Timer object to use to schodule the one run of this job.  This
		 * function returns a JobId which can be used at a later time to control
		 * the execution of the job.
		 */
		virtual JobId addJobOnce( CronSignal sigJob, const Timer &t );

		/**
		 * Remove a job, preventing all future runs of the job.  If there is no
		 * job matching the given JobId then nothing will happen.  However, this
		 * function is relatively expensive compared to the others in this class
		 * and has a worse case runtime of 2*N*log(N), still not that bad, and
		 * a O(N*log(N)).
		 */
		virtual void removeJob( JobId jid );

		class Timer
		{
		public:
			Timer();
			virtual ~Timer();

			virtual time_t nextTime()=0;
			virtual Timer *clone() const = 0;
		};

		class TimerInterval : public Timer
		{
		public:
			TimerInterval( time_t tFirst, time_t tInterval );
			virtual ~TimerInterval();

			virtual time_t nextTime();
			virtual Timer *clone() const
				{ return new TimerInterval( *this ); }
		private:
			time_t tNext;
			time_t tInterval;
		};

		class TimerBasic : public Timer
		{
		public:
			TimerBasic( const Bu::FString &s );
			virtual ~TimerBasic();

			virtual time_t nextTime();
			virtual Timer *clone() const
				{ return new TimerBasic( *this ); }

		private:
			enum Token
			{
				tokDaily,
				tokHourly,
				tokWeekly,
				tokMonthly,
				tokYearly,
				valInt,
				tokErr,
				tokEos
			};
			Token lex( Bu::FString::const_iterator &i );
			int lexInt( Bu::FString::const_iterator &i );
			int iVal; //< A temp variable for parsing.
			time_t tLast;
			Bu::FString sSpec;
		};
		
		class Job
		{
			friend class Bu::MiniCron;
		public:
			Job( JobId jid, bool bRepeat=true );
			virtual ~Job();

			/**
			 * Execute this job once, increment the runcount and schedule the
			 * next occurance of it.
			 */
			void run();

			/**
			 * Get the time this job will next run.
			 */
			time_t getNextRun();

			/**
			 * Compute the time this job will next run.
			 */
			void calcNextRun();

			/**
			 * Replace the current job timer with a new one, this will trigger
			 * a re-schedule.
			 */
			void setTimer( const Timer &t );

			/**
			 * Stop execution of this job, never execute this job again.
			 */
			void stop();

			/**
			 * Undo a previous stop.  This will cause a job that has been
			 * stopped or even added with addJobOnce to be set for repeated
			 * scheduling.
			 */
			void resume();

			/**
			 * Get the unique ID of this job.
			 */
			JobId getId();

			/**
			 * Get the timestamp this job was created.
			 */
			time_t getTimeCreated();

			/**
			 * Get the current run count of this job, how many times it has been
			 * executed.  This is incremented before the slot is signaled.
			 */
			int getRunCount();

		private:
			CronSignal sigJob;
			time_t tNextRun;
			Timer *pTimer;
			bool bContinue;
			JobId jid;
			time_t tAdded;
			int iRunCount;
		};

	private:
		struct JobPtrCmp
		{
			bool operator()( const Job *pLeft, const Job *pRight )
			{
				return pLeft->tNextRun < pRight->tNextRun;
			}
		};
		typedef Bu::Heap<Job *, JobPtrCmp> JobHeap;
		JobHeap hJobs;
		JobId jidNext;
	};
};

#endif
