#ifndef BU_UUID_H
#define BU_UUID_H

namespace Bu
{
	class Uuid
	{
	public:
		Uuid();
		virtual ~Uuid();

		static Uuid genV1();
		static Uuid genV2();
		static Uuid genV3();
		static Uuid genV4();
		static Uuid genV5();

		void clear();

	private:
		unsigned char data[8];
	};
};

#endif
