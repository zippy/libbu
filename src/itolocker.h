#ifndef BU_ITO_LOCKER_H
#define BU_ITO_LOCKER_H

namespace Bu
{
	class ItoMutex;
	class ItoLocker
	{
	public:
		ItoLocker( ItoMutex &mu );
		virtual ~ItoLocker();

	private:
		ItoMutex & mu;
	};
};

#endif
