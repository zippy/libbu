/**\file cgi.h
 * Describes extra params needed to use the Cgi class as well as the class
 * itself.
 *@author Mike Buland
 */

#include "linkedlist.h"
#include "hashtable.h"
#include "hashfunctionstring.h"

#define VAR_STDINPUT		0x01 /**< Variable came from stdinput, web form */
#define VAR_COOKIE			0x02 /**< Variable came from a cookie */
#define VAR_CMDLINE			0x04 /**< Variable came from commandline / uri */
#define VAR_ANY				0xFF /**< Mask including all other types */

/**
 * Cgi header processor originally designed for apache cgi programs.  When used
 * from apache with what I beleive are some sort of standard set of command
 * line parameters and environment variables.  This always worked for all of my
 * purposes.  This class will automatically extract all data from the system
 * that you need and places it into tables and things for easy access.
 * There are three types of input that data can come from, StandardInput,
 * CommandLine, and Cookies.  StandardInput is when you get formdata in
 * multi-part forms, Cookies should usually be cookies that you set, and
 * command line is everything after the question mark in the URL.
 * This also contains some simple helpers for putting templated data into the
 * HTTP data feed.
 *@author Mike Buland
 */
class Cgi
{
public:
	/**
	 * Create a complete CGI object, this object will automatically read data
	 * from all available sources and be ready for use on the very next line!
	 * If strSource is filled in it will also automatically read in a content
	 * file, which is a simple file format containing named blocks of reusable
	 * templates.
	 *@param strSource Set to a filename in order to load up a content file.
	 */
	Cgi( const char *strSource = NULL );

	/**
	 * Destroy the cgi object.
	 */
	virtual ~Cgi(  );

	/**
	 * Get's the value for a variable as a character string.  The name is the
	 * name that was given on the URL or in the form or cookie.  Skip can be
	 * set to any value above zero to retreive subsequent variables with the
	 * same name.  The most obvious use of this is when dealing with file
	 * uploads, each file upload sends you three variables with the same name
	 * and different content.  Finally the variable type determines where you
	 * will accept this variable from.  This is generally a bit of a security
	 * thing, if you store login info in a cookie and don't want people getting
	 * in by faking the appropriate URL.
	 *@param name The name of the variable you wish to retreive.
	 *@param skip THe number of variables with the given name to skip before
	 * returning something meaningful.  The only way to determine how many
	 * variables with the same name there are is to skip until you get a NULL
	 * value returned.
	 *@param type Can be set to any combination of VAR_STDINPUT, VAR_COOKIE,
	 * VAR_CMDLINE, or just VAR_ANY.  This takes bitflags, so you can or the
	 * values together.  If a variable is found but came from the wrong source
	 * it won't match any other criteria and will be treated as though it
	 * doesn't exist.
	 *@returns A null-terminated string representing the value of the requested
	 * variable, or NULL if the variable did not exist.  If a variable does
	 * exist but has no value the string returned will start with a NULL char,
	 * but be a valid string.
	 */
	char *getVarValue( const char *name, int skip=0, unsigned char type=VAR_ANY );

	/**
	 * This functions identically in every way to getVarValue, except that
	 * instead of returning a pointer to the variable's value, it returns the
	 * length of the variable's value string.  The params are the same and so
	 * a call to both functions with the same params should yeild a value and
	 * a corresponding length.
	 *@param name The name of the variable you wish to retreive.
	 *@param skip THe number of variables with the given name to skip before
	 * returning something meaningful.  The only way to determine how many
	 * variables with the same name there are is to skip until you get a NULL
	 * value returned.
	 *@param type Can be set to any combination of VAR_STDINPUT, VAR_COOKIE,
	 * VAR_CMDLINE, or just VAR_ANY.  This takes bitflags, so you can or the
	 * values together.  If a variable is found but came from the wrong source
	 * it won't match any other criteria and will be treated as though it
	 * doesn't exist.
	 *@returns The length of the value-string of the requested variable.  If
	 * the requested variable is not found, -1 is returned.
	 */
	int getVarLength( const char *name, int skip=0, unsigned char type=VAR_ANY );

	/**
	 * A handy little function that writes a load of debug info related to
	 * parsing CGI params to the standard output in html.  This is generally
	 * best used at the end of a page.
	 */
	void writeDebugInfo();

	/**
	 * Write a content header to the standard output.  This should also be the
	 * first thing that you do (except for writing cookies) after initializing
	 * the Cgi class.  You can select a type of header or content from the
	 * header enum, and a properly formatted header will show up on the
	 * standard output.
	 *@param type Any value from the header enum in this class.  The default is
	 * to write an html header, probably the most common as well.
	 */
	void writeContentHeader( int type=headerHTML );

	/**
	 * Write content to the stnadard output.  The content variable should have
	 * been loaded during construction of the Cgi object or with the
	 * loadContent function.  The content variable should be formatted just like
	 * a printf string, so that anything you want to put into it will have a %
	 * symbol replacement code, like %s, %d, etc.  Since this actually uses a
	 * type of printf function everything from those docs work here.
	 *@param name The name of the content variable to format and write to
	 * stnadard output.
	 *@param ... As many params as you want to include, ala printf.
	 */
	void writeContent( const char *name, ...);

	/**
	 * Load a content file.  I don't want to describe the format here, you can
	 * just read the code or find an example for now.  Sorry.
	 *@param strSource The name of the file to open and read in to get the
	 * content loaded.
	 */
	void loadContent( const char *strSource = NULL );

	/**
	 * Write a cookie-set header to the output stream.  This should be done
	 * before any other content-headers are written.  The specifics of this
	 * function are very simple, since I rely on the user's understanding of
	 * how standard HTTP/1.1 or HTTP/1.0 cookie syntax works.  If you don't
	 * care then just use the name and value and the defaults should keep you
	 * in good stead for a long time.
	 *@param name The name of the cookie variable to set.
	 *@param value The value to set to that variable.
	 *@param expires The formatted string value for the date and time this
	 * cookie should expire.  A NULL here will put a "until the browser closes"
	 * tag in.
	 *@param path The path (URL) that this cookie belongs to.  If you run a lot
	 * of hosted servers or sub-sites that may have some shared URL bits then
	 * you may want to set this.  The cookie should only be sent to URL's that
	 * match this as their first part.
	 *@param domain The domain that is allowed to read this, if not set, it's
	 * the domain the web browser contacted when they got the cookie.
	 *@param secure I'm not sure, I think it's something to tell if the cookie
	 * is safe to keep because any potentially valuable data is encypted or
	 * otherwise unusable.  I could be wrong.
	 */
	void writeCookie( char const *name, char const *value, char const *expires=NULL, char const *path=NULL, char const *domain=NULL, bool secure=false );
	
	/**
	 * A simple helper class to contain variable data.
	 */
	class Item
	{
	public:
		/**
		 * Build an empty Item.
		 */
		Item(  )
		{
			name = NULL;
			value = NULL;
			len = 0;
			type = 0;
		}
		/** The name of the item.  */
		char *name;
		/** The value of the item. */
		char *value;
		/** The length of the item's value. */
		unsigned long len;
		/** The type of the item (where it came from). */
		unsigned char type;
	};
    
	/** Header values */
	enum
	{
		headerHTML
	};

private:
	/** Keeps track of all contained variables. */
    LinkedList aVars;
	/** Keeps track of all content variables. */
	HashTable aContent;
};
