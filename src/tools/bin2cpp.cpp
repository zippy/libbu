#include <bu/sio.h>
#include <bu/optparser.h>
#include <bu/file.h>
#include <bu/deflate.h>
#include <bu/bzip2.h>
#include <bu/base64.h>
#include <bu/streamstack.h>

#include <bu/strfilter.h>

using namespace Bu;

class Options : public OptParser
{
public:
	Options( int argc, char *argv[] ) :
		sOutBase("bin")
	{
		addOption( sOutBase, 'o', "Output base filename [default=\"bin\"]");
		addOption( slot(this, &Options::addFilter), 'f', "Add filter: deflate, bzip2, base64");
		setNonOption( slot(this, &Options::addInput) );
		addHelpOption();

		parse( argc, argv );
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
		sio << "Input: " << aArgs[0] << sio.nl;
		slInput.append( aArgs[0] );
		return 0;
	}

	Bu::String sOutBase;
	Bu::StringList slInput;
	Bu::StringList slFilter;
};

int main( int argc, char *argv[] )
{
	Options opt( argc, argv );

	File fHdrOut( opt.sOutBase + ".h", File::WriteNew );
	File fSrcOut( opt.sOutBase + ".cpp", File::WriteNew );

	Formatter fHdr( fHdrOut );
	Formatter fSrc( fSrcOut );
	fHdr << "#ifndef BIN2CPP_DATAFILE_H" << fHdr.nl
		<< "#define BIN2CPP_DATAFILE_H" << fHdr.nl << fHdr.nl
		<< "#include <bu/string.h>" << fHdr.nl
		<< "#include <bu/streamstack.h>" << fHdr.nl
		<< fHdr.nl
		<< "class Datafiles" << fHdr.nl
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
		<< "#include <bu/strfilter.h>" << fSrc.nl
		<< "#include <bu/membuf.h>" << fSrc.nl << fSrc.nl
		<< "const Datafiles::File Datafiles::aFile[] = {" << fSrc.nl;

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
			}
			else if( *f == "bzip2" )
			{
				sDat = encodeStr<BZip2>( sDat );
				sFltDesc.prepend("b");
			}
			else if( *f == "base64" )
			{
				sDat = encodeStr<Base64>( sDat );
				sFltDesc.prepend("6");
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

	fSrc << "const Datafiles::File &Datafiles::getFile( const Bu::String &sName )"
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
	
	fSrc << "Bu::StreamStack *Datafiles::open( const Bu::String &sName )" << fSrc.nl
		<< "{" << fSrc.nl
		<< "\tconst File &f = getFile( sName );" << fSrc.nl
		<< "\tBu::StreamStack *s = new Bu::StreamStack( new Bu::MemBuf( Bu::String( f.data, f.iSize ) ) );" << fSrc.nl
		<< "\tfor( const char *t = f.flt; *t; t++ )" << fSrc.nl
		<< "\t{" << fSrc.nl
		<< "\t\tswitch( *t )" << fSrc.nl
		<< "\t\t{" << fSrc.nl
		<< "\t\t\tcase 'd': s->pushFilter<Bu::Deflate>(); break;" << fSrc.nl
		<< "\t\t\tcase 'b': s->pushFilter<Bu::BZip2>(); break;" << fSrc.nl
		<< "\t\t\tcase '6': s->pushFilter<Bu::Base64>(); break;" << fSrc.nl
		<< "\t\t}" << fSrc.nl
		<< "\t}" << fSrc.nl
		<< "\treturn s;" << fSrc.nl
		<< "}" << fSrc.nl << fSrc.nl;

	fSrc << "Bu::StreamStack *Datafiles::openRaw( const Bu::String &sName )" << fSrc.nl
		<< "{" << fSrc.nl
		<< "\tconst File &f = getFile( sName );" << fSrc.nl
		<< "\treturn new Bu::StreamStack( new Bu::MemBuf( Bu::String( f.data, f.iSize ) ) );" << fSrc.nl
		<< "}" << fSrc.nl << fSrc.nl;

	fSrc << "Bu::String Datafiles::getString( const Bu::String &sName )" << fSrc.nl
		<< "{" << fSrc.nl
		<< "\tconst File &f = getFile( sName );" << fSrc.nl
		<< "\tBu::String s( f.data, f.iSize );" << fSrc.nl
		<< "\tfor( const char *t = f.flt; *t; t++ )" << fSrc.nl
		<< "\t{" << fSrc.nl
		<< "\t\tswitch( *t )" << fSrc.nl
		<< "\t\t{" << fSrc.nl
		<< "\t\t\tcase 'd': s = Bu::decodeStr<Bu::Deflate>( s ); break;" << fSrc.nl
		<< "\t\t\tcase 'b': s = Bu::decodeStr<Bu::BZip2>( s ); break;" << fSrc.nl
		<< "\t\t\tcase '6': s = Bu::decodeStr<Bu::Base64>( s ); break;" << fSrc.nl
		<< "\t\t}" << fSrc.nl
		<< "\t}" << fSrc.nl
		<< "\treturn s;" << fSrc.nl
		<< "}" << fSrc.nl << fSrc.nl;
	
	fSrc << "Bu::String Datafiles::getStringRaw( const Bu::String &sName )" << fSrc.nl
		<< "{" << fSrc.nl
		<< "\tconst File &f = getFile( sName );" << fSrc.nl
		<< "\treturn Bu::String( f.data, f.iSize );" << fSrc.nl
		<< "}" << fSrc.nl << fSrc.nl;

	return 0;
}

