#include <bu/sio.h>
#include <bu/optparser.h>
#include <bu/file.h>
#include <bu/deflate.h>
#include <bu/bzip2.h>
#include <bu/lzma.h>
#include <bu/base64.h>
#include <bu/hex.h>
#include <bu/streamstack.h>

#include <bu/strfilter.h>

using namespace Bu;

class Options : public OptParser
{
public:
	Options( int argc, char *argv[] ) :
		sClass("Datafiles")
	{
		addOption( sClass, 'c', "Class name [default=\"Datafiles\"]");
		addOption( sOutBase, 'o', "Output base filename [defaults to classname]");
		addOption( sOutDir, 'd', "Output directory [defaults to current dir]");
		addOption( slot(this, &Options::addFilter), 'f', "Add filter: deflate, bzip2, lzma, base64, hex");
		setNonOption( slot(this, &Options::addInput) );
		addHelpOption();

		parse( argc, argv );

		if( !sOutBase.isSet() )
			sOutBase = sClass.toLower();
		if( sOutDir.isSet() )
			sOutDir += "/";
	}

	virtual ~Options()
	{
	}

	int addFilter( Bu::StrArray aArgs )
	{
		slFilter.append( aArgs[1] );
		return 1;
	}

	int addInput( Bu::StrArray aArgs )
	{
		slInput.append( aArgs[0] );
		return 0;
	}

	Bu::String sClass;
	Bu::String sOutBase;
	Bu::String sOutDir;
	Bu::StringList slInput;
	Bu::StringList slFilter;
};

int main( int argc, char *argv[] )
{
	Options opt( argc, argv );

	File fHdrOut( opt.sOutDir + opt.sOutBase + ".h", File::WriteNew );
	File fSrcOut( opt.sOutDir + opt.sOutBase + ".cpp", File::WriteNew );

	Bu::Hash<char, bool> hFilters;

	Formatter fHdr( fHdrOut );
	Formatter fSrc( fSrcOut );
	fHdr << "#ifndef BIN2CPP_" << opt.sClass.toUpper() << "_H" << fHdr.nl
		<< "#define BIN2CPP_" << opt.sClass.toUpper() << "_H" << fHdr.nl << fHdr.nl
		<< "#include <bu/string.h>" << fHdr.nl
		<< "#include <bu/streamstack.h>" << fHdr.nl
		<< fHdr.nl
		<< "class " << opt.sClass << fHdr.nl
		<< "{" << fHdr.nl
		<< "public:" << fHdr.nl
		<< "\tclass File { public: int iSize; const char *data; const char *flt; };" << fHdr.nl << fHdr.nl
		<< "\tstatic const File &getFile( const Bu::String &sName );" << fHdr.nl
		<< "\tstatic Bu::StreamStack *open( const Bu::String &sName );" << fHdr.nl
		<< "\tstatic Bu::StreamStack *openRaw( const Bu::String &sName );" << fHdr.nl
		<< "\tstatic Bu::String getString( const Bu::String &sName );" << fHdr.nl
		<< "\tstatic Bu::String getStringRaw( const Bu::String &sName );" << fHdr.nl
		<< fHdr.nl;
	fHdr << "public:" << fHdr.nl
		<< "\tstatic const File aFile[];" << fHdr.nl
		<< "};" << fHdr.nl << fHdr.nl;
	fHdr << "#endif";

	fSrc << "#include \"" << opt.sOutBase << ".h\"" << fSrc.nl 
		<< "#include <bu/deflate.h>" << fSrc.nl
		<< "#include <bu/bzip2.h>" << fSrc.nl
		<< "#include <bu/base64.h>" << fSrc.nl
		<< "#include <bu/lzma.h>" << fSrc.nl
		<< "#include <bu/hex.h>" << fSrc.nl
		<< "#include <bu/strfilter.h>" << fSrc.nl
		<< "#include <bu/staticmembuf.h>" << fSrc.nl << fSrc.nl
		<< "const " << opt.sClass << "::File " << opt.sClass << "::aFile[] = {" << fSrc.nl;

	for( Bu::StringList::iterator i = opt.slInput.begin(); i; i++ )
	{
		File fIn( *i, File::Read );
		Bu::String sDat;
		char buf[1024];
		while( !fIn.isEos() )
		{
			sDat.append( buf, fIn.read( buf, 1024 ) );
		}

		Bu::String sFltDesc;
		for( Bu::StringList::iterator f = opt.slFilter.begin(); f; f++ )
		{
			if( *f == "deflate" )
			{
				sDat = encodeStr<Deflate>( sDat );
				sFltDesc.prepend("d");
				hFilters.insert('d', true );
			}
			else if( *f == "bzip2" )
			{
				sDat = encodeStr<BZip2>( sDat );
				sFltDesc.prepend("b");
				hFilters.insert('b', true );
			}
			else if( *f == "lzma" )
			{
				sDat = encodeStr<Lzma>( sDat );
				sFltDesc.prepend("l");
				hFilters.insert('l', true );
			}
			else if( *f == "base64" )
			{
				sDat = encodeStr<Base64>( sDat );
				sFltDesc.prepend("6");
				hFilters.insert('6', true );
			}
			else if( *f == "hex" )
			{
				sDat = encodeStr<Hex>( sDat );
				sFltDesc.prepend("h");
				hFilters.insert('h', true );
			}
			else
			{
				sio << "No known filter named " << *f << sio.nl;
				return 1;
			}
		}
		
		fSrc << "  {" << sDat.getSize() << ", \"";

		for( Bu::String::iterator j = sDat.begin(); j; j++ )
		{
			fSrc << "\\x" << Fmt::hex() << (unsigned char)*j;
		}
		fSrc << "\", \"" << sFltDesc << "\"}," << fSrc.nl;
	}
	fSrc << "};" << fSrc.nl << fSrc.nl;

	fSrc << "const " << opt.sClass << "::File &" << opt.sClass << "::getFile( const Bu::String &sName )"
		<< fSrc.nl
		<< "{" << fSrc.nl
		<< "\tswitch( Bu::__calcHashCode( sName ) )" << fSrc.nl
		<< "\t{" << fSrc.nl;

	int idx = 0;
	for( Bu::StringList::iterator i = opt.slInput.begin(); i; i++ )
	{
		fSrc << "\t\tcase " << Bu::__calcHashCode( *i ) << "UL:" << fSrc.nl
			<< "\t\t\treturn aFile[" << idx << "];" << fSrc.nl;
		idx++;
	}
	fSrc << "\t}" << fSrc.nl
		<< "\tthrow Bu::ExceptionBase(\"No file matching \\\"%s\\\" found.\", sName.getStr() );" << fSrc.nl
		<< "}" << fSrc.nl << fSrc.nl;
	
	fSrc << "Bu::StreamStack *" << opt.sClass << "::open( const Bu::String &sName )" << fSrc.nl
		<< "{" << fSrc.nl
		<< "\tconst File &f = getFile( sName );" << fSrc.nl
		<< "\tBu::StreamStack *s = new Bu::StreamStack( new Bu::StaticMemBuf( f.data, f.iSize ) );" << fSrc.nl;

	if( !hFilters.isEmpty() )
	{
		fSrc << "\tfor( const char *t = f.flt; *t; t++ )" << fSrc.nl
			<< "\t{" << fSrc.nl
			<< "\t\tswitch( *t )" << fSrc.nl
			<< "\t\t{" << fSrc.nl;
		if( hFilters.has('d') )
			fSrc << "\t\t\tcase 'd': s->pushFilter<Bu::Deflate>(); break;" << fSrc.nl;
		if( hFilters.has('b') )
			fSrc << "\t\t\tcase 'b': s->pushFilter<Bu::BZip2>(); break;" << fSrc.nl;
		if( hFilters.has('l') )
			fSrc << "\t\t\tcase 'l': s->pushFilter<Bu::Lzma>(); break;" << fSrc.nl;
		if( hFilters.has('6') )
			fSrc << "\t\t\tcase '6': s->pushFilter<Bu::Base64>(); break;" << fSrc.nl;
		if( hFilters.has('h') )
			fSrc << "\t\t\tcase 'h': s->pushFilter<Bu::Hex>(); break;" << fSrc.nl;
		fSrc << "\t\t}" << fSrc.nl
			<< "\t}" << fSrc.nl;
	}
	fSrc << "\treturn s;" << fSrc.nl
		<< "}" << fSrc.nl << fSrc.nl;

	fSrc << "Bu::StreamStack *" << opt.sClass << "::openRaw( const Bu::String &sName )" << fSrc.nl
		<< "{" << fSrc.nl
		<< "\tconst File &f = getFile( sName );" << fSrc.nl
		<< "\treturn new Bu::StreamStack( new Bu::StaticMemBuf( f.data, f.iSize ) );" << fSrc.nl
		<< "}" << fSrc.nl << fSrc.nl;

	fSrc << "Bu::String " << opt.sClass << "::getString( const Bu::String &sName )" << fSrc.nl
		<< "{" << fSrc.nl
		<< "\tBu::StreamStack *ss = open( sName );" << fSrc.nl
		<< "\tBu::String s;" << fSrc.nl
		<< "\tchar buf[1024];" << fSrc.nl
		<< "\twhile( !ss->isEos() )" << fSrc.nl
		<< "\t{" << fSrc.nl
		<< "\t\ts.append( buf, ss->read( buf, 1024 ) );" << fSrc.nl
		<< "\t}" << fSrc.nl
		<< "\tdelete ss;" << fSrc.nl
		<< "\treturn s;" << fSrc.nl
		<< "}" << fSrc.nl << fSrc.nl;
	
	fSrc << "Bu::String " << opt.sClass << "::getStringRaw( const Bu::String &sName )" << fSrc.nl
		<< "{" << fSrc.nl
		<< "\tconst File &f = getFile( sName );" << fSrc.nl
		<< "\treturn Bu::String( f.data, f.iSize );" << fSrc.nl
		<< "}" << fSrc.nl << fSrc.nl;

	return 0;
}

