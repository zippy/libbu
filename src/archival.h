#ifndef BU_ARCHIVAL_H
#define BU_ARCHIVAL_H

namespace Bu
{
	/**
	 * The base class for any class you want to archive.  Simply include this as
	 * a base class, implement the purely virtual archive function and you've
	 * got an easily archiveable class.
	 *
	 * Archival: "of or pertaining to archives or valuable records; contained
	 * in or comprising such archives or records."
	 */
	class Archival
	{
	public:
		/**
		 * Does nothing, here for completeness.
		 */
		Archival();

		/**
		 * Here to ensure the deconstructor is virtual.
		 */
		virtual ~Archival();

		/**
		 * This is the main workhorse of the archive system, just override and
		 * you've got a archiveable class.  A reference to the Archive
		 * used is passed in as your only parameter, query it to discover if
		 * you are loading or saving.
		 * @param ar A reference to the Archive object to use.
		 */
		virtual void archive( class Archive &ar )=0;
	};
}

#endif
