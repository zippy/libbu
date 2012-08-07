#ifndef BU_FILE_SYSTEM_H
#define BU_FILE_SYSTEM_H

namespace Bu
{
	/**
	 * Represents an abstract baseclass for any file organization structure.
	 * If you're looking for acessing a local filesystem, try Bu::Dir.  This
	 * is used to make switching between different types of filesystems
	 * seamless.  FileSystems could be anything that organizes and grants
	 * access to file data, local filesystems, FTP servers, zip or myriadfs
	 * files, etc.
	 */
	class FileSystem
	{
	public:
		FileSystem();
		virtual ~FileSystem();

	
	};
};

#endif
