/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <bu/file.h>
#include <bu/optparser.h>
#include <bu/buffer.h>
#include <bu/list.h>
#include <bu/sio.h>
#include <bu/variant.h>

using namespace Bu;

class Test
{
public:
	Test() :
		bExpectPass( true )
	{
	}

	Bu::String sName;
	bool bExpectPass;
};
typedef Bu::List<Test> TestList;

class Suite
{
public:
	Bu::String sName;
	TestList lTest;
};
//typedef Bu::List<Suite> SuiteList;

enum TokType
{
	tokFluff,
	tokSuite,
	tokTest,
	tokChar,
	tokBlock,
	tokEof
};

Bu::Formatter &operator<<( Bu::Formatter &f, TokType t )
{
	switch( t )
	{
		case tokFluff:		return f << "tokFluff";
		case tokSuite:		return f << "tokSuite";
		case tokTest:		return f << "tokTest";
		case tokChar:		return f << "tokChar";
		case tokBlock:		return f << "tokBlock";
		case tokEof:		return f << "tokEof";
	}

	return f;
}

Bu::Formatter &operator<<( Bu::Formatter &f, const Test &t )
{
	return f << "{" << t.sName << ", bExpectPass=" << t.bExpectPass << "}";
}

Bu::Formatter &operator<<( Bu::Formatter &f, const Suite &s )
{
	return f << "Suite[" << s.sName << "] = " << s.lTest << f.nl;
}

class Parser
{
public:
	Parser( const Bu::String &sFile ) :
		sIn( sFile ),
		fIn( sFile, File::Read ),
		bIn( fIn ),
		cBuf( 0 ),
		bAvail( false ),
		eMode( mRoot ),
		iLine( 1 ),
		iChar( 0 ),
		iDepth( 0 )
	{
	}

	char nextChar()
	{
		if( bAvail )
			return cBuf;

		if( bIn.read( &cBuf, 1 ) < 1 )
			throw Bu::ExceptionBase("End of stream");
		bAvail = true;

		if( cBuf == '\n' )
		{
			iLine++;
			iChar = 0;
		}
		else
			iChar++;

		return cBuf;
	}
	
	TokType nextToken( Variant &v, Bu::String &sWsOut, int &iLineStart,
			int &iCharStart )
	{
		Bu::String sTok, sWs;
	
		char buf;
		try
		{
			buf = nextChar();
		}
		catch(...)
		{
			return tokEof;
		}

		for(;;)
		{
			if( buf == ' ' || buf == '\t' || buf == '\n' || buf == '\r' )
			{
				sWs += buf;
				bAvail = false;
			}
			else
				break;

			try
			{
				buf = nextChar();
			}
			catch(...)
			{
				sWsOut = sWs;
				return tokEof;
			}
		}

		sWsOut = sWs;

		iLineStart = iLine;
		iCharStart = iChar;
		bool bInStr = false;
		bool bDblStr;

		for(;;)
		{
			switch( eMode )
			{
				case mRoot:
					if( buf == ' ' || buf == '\t' || buf == '\n'
						|| buf == '\r' )
					{
						if( sTok == "suite" )
							return tokSuite;
						else
						{
							v = sTok;
							return tokFluff;
						}
					}
					else if( buf == '(' || buf == ')' || buf == '{'
						|| buf == '}' || buf == ';' )
					{
						if( sTok.getSize() == 0 )
						{
							bAvail = false;
							v = buf;
							return tokChar;
						}
						else
						{
							v = sTok;
							return tokFluff;
						}
					}
					else
					{
						sTok += buf;
						bAvail = false;
					}
					break;

				case mSuite:
					if( buf == ' ' || buf == '\t' || buf == '\n'
						|| buf == '\r' )
					{
						if( sTok == "test" )
							return tokTest;
						else
						{
							v = sTok;
							return tokFluff;
						}
					}
					else if( buf == '(' || buf == ')'
						|| buf == '}' || buf == ';' )
					{
						if( sTok.getSize() == 0 )
						{
							bAvail = false;
							v = buf;
							return tokChar;
						}
						else
						{
							v = sTok;
							return tokFluff;
						}
					}
					else if( buf == '{' )
					{
						if( sTok.getSize() > 0 )
						{
							v = sTok;
							return tokFluff;
						}
						else
						{
							sTok += buf;
							bAvail = false;
							eMode = mBlock;
							iDepth = 1;
						}
					}
					else
					{
						sTok += buf;
						bAvail = false;
					}
					break;

				case mBlock:
					if( bInStr )
					{
						if( buf == '\\' )
						{
							sTok += buf;
							bAvail = false;
							sTok += nextChar();
							bAvail = false;
						}
						else if( bDblStr == true && buf == '\"' )
						{
							sTok += buf;
							bAvail = false;
							bInStr = false;
						}
						else if( bDblStr == false && buf == '\'' )
						{
							sTok += buf;
							bAvail = false;
							bInStr = false;
						}
						else
						{
							sTok += buf;
							bAvail = false;
						}
					}
					else
					{
						if( buf == '\"' )
						{
							bInStr = true;
							bDblStr = true;
							sTok += buf;
							bAvail = false;
						}
						else if( buf == '\'' )
						{
							bInStr = true;
							bDblStr = false;
							sTok += buf;
							bAvail = false;
						}
						else if( buf == '}' )
						{
							sTok += buf;
							bAvail = false;
							iDepth--;
							if( iDepth == 0 )
							{
								v = sTok;
								eMode = mSuite;
								return tokBlock;
							}
						}
						else if( buf == '{' )
						{
							sTok += buf;
							bAvail = false;
							iDepth++;
						}
						else
						{
							sTok += buf;
							bAvail = false;
						}
					}
					break;
			}
		
			buf = nextChar();
		}
	}

	void firstPass()
	{
		Variant v;
		Bu::String sWs;
		int iL, iC;
		for(;;)
		{
			TokType t = nextToken( v, sWs, iL, iC );
			if( t == tokEof )
				return;
			switch( eMode )
			{
				case mRoot:
					if( t == tokSuite )
					{
						if( nextToken( v, sWs, iL, iC ) != tokFluff )
							throw Bu::ExceptionBase("%d:%d: Expected string "
								"following suite.", iL, iC );
						s.sName = v.get<Bu::String>();
						if( nextToken( v, sWs, iL, iC ) != tokChar ||
							v.get<char>() != '{' )
							throw Bu::ExceptionBase("%d:%d: Expected {, got "
								"'%s'", iL, iC, v.toString().getStr() );
						eMode = mSuite;
					}
					break;

				case mSuite:
					switch( t )
					{
						case tokFluff:
							break;

						case tokBlock:
							break;

						case tokTest:
							{
								if( nextToken( v, sWs, iL, iC ) != tokFluff )
									throw Bu::ExceptionBase("%d:%d: Expected "
										"string following test.", iL, iC );
								Test t;
								t.sName = v.get<Bu::String>();
								if( nextToken( v, sWs, iL, iC ) != tokBlock )
									throw Bu::ExceptionBase("%d:%d: Expected "
										"{...} block.",
										iL, iC );
								s.lTest.append( t );
							}
							break;

						case tokChar:
							if( v.get<char>() == '}' )
							{
								eMode = mRoot;
							}
							else
							{
							}
							break;

						default:
							sio << iL << ":" << iC << ": Unexpected "
								<< t << " found." << sio.nl;
							return;
							break;
					}
					break;

				default:
					sio << "???" << sio.nl;
					break;
			}
		}
	}

	void secondPass( const Bu::String &sOut )
	{
		File fOut( sOut, File::WriteNew );
		Formatter f( fOut );
		fIn.setPos( 0 );
		bIn.stop();
		bIn.start();
		bAvail = false;
		eMode = mRoot;
		iLine = 1;
		iChar = 0;
		bool bHasIncluded = false;

		Bu::String sWs;
		Variant v;
		int iL, iC;
		for(;;)
		{
			TokType t = nextToken( v, sWs, iL, iC );
			switch( eMode )
			{
				case mRoot:
					if( t == tokSuite )
					{
						fOut.write( sWs );
						if( nextToken( v, sWs, iL, iC ) != tokFluff )
							throw Bu::ExceptionBase("%d:%d: Expected string "
								"following suite.", iL, iC );
						s.sName = v.get<Bu::String>();
						if( nextToken( v, sWs, iL, iC ) != tokChar ||
							v.get<char>() != '{' )
							throw Bu::ExceptionBase("%d:%d: Expected {",
								iL, iC );
						eMode = mSuite;

						if( bHasIncluded == false )
						{
							fOut.write("#include <bu/unitsuite.h>\n");
							bHasIncluded = true;
						}

						Bu::String sClass = "_UnitSuite_" + s.sName;
						f << "class " << sClass
							<< " : public Bu::UnitSuite" << f.nl
							<< "{" << f.nl << "public:" << f.nl
							<< "\t" << sClass << "()" << f.nl
							<< "\t{" << f.nl
							<< "\t\tsetName(\"" << s.sName << "\");" << f.nl;
						for( TestList::iterator i = s.lTest.begin(); i; i++ )
						{
							f << "\t\tadd( static_cast<Bu::UnitSuite::Test>("
								"&" << sClass << "::" << (*i).sName << "), \""
								<< (*i).sName << "\", Bu::UnitSuite::"
								"expectPass );" << f.nl;
						}
						f << "\t}" << f.nl << f.nl
							<< "\tvirtual ~" << sClass << "() { }" << f.nl
							<< f.nl;
					}
					else if( t == tokEof )
					{
						Bu::String sClass = "_UnitSuite_" + s.sName;
						f << sWs << f.nl << "int main( int argc, char *argv[] )"
							<< f.nl << "{" << f.nl << "\treturn " << sClass
							<< "().run( argc, argv );" << f.nl << "}" << f.nl;
					}
					else
					{
						fOut.write( sWs );
						f << v;
					}
					break;

				case mSuite:
					switch( t )
					{
						case tokFluff:
							fOut.write( sWs );
							fOut.write( v.get<Bu::String>() );
							break;

						case tokTest:
							{
								fOut.write( sWs );
								if( nextToken( v, sWs, iL, iC ) != tokFluff )
									throw Bu::ExceptionBase("%d:%d: Expected "
										"string following test.", iL, iC );
								Test t;
								t.sName = v.get<Bu::String>();
								if( nextToken( v, sWs, iL, iC ) != tokBlock )
									throw Bu::ExceptionBase("%d:%d: Expected "
										"{...} block.",
										iL, iC );
								
								f << "\tvoid " << t.sName << "()"
									<< f.nl << "#line " << iL
									<< " \"" << sIn << "\"" << f.nl
									<< v << f.nl;
							}
							break;

						case tokChar:
							if( v.get<char>() == '}' )
							{
								f << "};" << f.nl << f.nl;
								eMode = mRoot;
							}
							else
							{
								char buf = v.get<char>();
								fOut.write( sWs );
								fOut.write( &buf, 1 );
							}
							break;
						
						case tokBlock:
							fOut.write( sWs );
							f << f.nl << "#line " << iL << " \"" << sIn
								<< "\"" << f.nl;
							fOut.write( v.get<Bu::String>() );

							break;

						default:
							sio << iL << ":" << iC << ": Unexpected "
								<< t << " found." << sio.nl;
							return;
							break;
					}
					break;

				default:
					sio << "???" << sio.nl;
					break;
			}
			if( t == tokEof )
				return;
		}
	}

private:
	Bu::String sIn;
	File fIn;
	Buffer bIn;
	char cBuf;
	bool bAvail;
	enum Mode
	{
		mRoot,
		mSuite,
		mBlock
	};
	Mode eMode;
	int iLine, iChar;
	int iDepth;
	Suite s;
};

int main( int argc, char *argv[] )
{
	if( argc < 3 )
	{
		sio << "Too few parameters." << sio.nl;
		return 0;
	}
	Parser p( argv[1] );

	p.firstPass();

	p.secondPass( argv[2] );
}

