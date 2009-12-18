#include "bu/optparser.h"
#include "bu/sio.h"
using namespace Bu;

#include <stdlib.h>

Bu::OptParser::OptParser()
{
}

Bu::OptParser::~OptParser()
{
}

void Bu::OptParser::parse( int argc, char **argv )
{
	for( int j = 1; j < argc; j++ )
	{
		if( argv[j][0] == '-' )
		{
			// Now we're on to something, which kind is it?
			if( argv[j][1] == '-' )
			{
				// Long param, cool, that's easy, first search for =
				int iEPos;
				for( iEPos = 2; argv[j][iEPos] != '\0' &&
					 argv[j][iEPos] != '='; iEPos++ ) { }

				Bu::FString sOpt;
				int iCount = argc-j;
				Bu::FString sExtraParam;
				if( argv[j][iEPos] == '=' )
				{
					sOpt.set( argv[j]+2, iEPos-2 );
					iCount++;
					sExtraParam.set( argv[j]+iEPos+1 );
				}
				else
				{
					sOpt.set( argv[j]+2 );
				}
				Option *pOpt = hlOption.get( sOpt );
				if( pOpt->sUsed )
				{
					Bu::StrArray aParams( iCount );
					aParams.append( sOpt );
					if( sExtraParam )
					{
						aParams.append( argv[j]+iEPos+1 );
					}
					for( int k = j+1; k < argc; k++ )
					{
						aParams.append( argv[k] );
					}
					j += pOpt->sUsed( aParams );
				}
				else if( pOpt->pProxy )
				{
					if( pOpt->sOverride )
					{
						pOpt->pProxy->setValue( pOpt->sOverride );
					}
					else if( sExtraParam )
					{
						pOpt->pProxy->setValue( sExtraParam );
					}
					else if( argv[j+1] != '\0' )
					{
						pOpt->pProxy->setValue( argv[j+1] );
						j++;
					}
				}
			}
			else
			{
				int iCPos;
				for( iCPos = 1; argv[j][iCPos] != '\0'; iCPos++ )
				{
					Option *pOpt = hsOption.get( argv[j][iCPos] );
					char buf[2] = {argv[j][iCPos], '\0'};
					if( pOpt->bShortHasParams )
					{
						if( pOpt->sUsed )
						{
							Bu::StrArray aParams( argc-j+1 );
							aParams.append( buf );
							if( argv[j][iCPos+1] != '\0' )
								aParams.append( argv[j]+iCPos+1 );
							for( int k = j+1; k < argc; k++ )
							{
								aParams.append( argv[k] );
							}
							j += pOpt->sUsed( aParams );
							break;
						}
						else if( pOpt->pProxy )
						{
							if( pOpt->sOverride )
							{
								pOpt->pProxy->setValue( pOpt->sOverride );
							}
							else if( argv[j][iCPos+1] != '\0' )
							{
								pOpt->pProxy->setValue(
									argv[j]+iCPos+1
									);
								break;
							}
						}
					}
					else
					{
						if( pOpt->sUsed )
						{
							Bu::StrArray aParam( 1 );
							aParam.append( buf );
							pOpt->sUsed( aParam );
						}
					}
				}
			}
		}
		else
		{
		}
	}
}

void Bu::OptParser::addOption( const Option &opt )
{
	lOption.append( opt );
	if( opt.cOpt != '\0' )
		hsOption.insert( opt.cOpt, &lOption.last() );
	if( opt.sOpt )
		hlOption.insert( opt.sOpt, &lOption.last() );
}

void Bu::OptParser::addHelpOption( char c, const Bu::FString &s, const Bu::FString &sHelp )
{
	Option o;
	o.sUsed = slot( this, &OptParser::optHelp );
	o.cOpt = c;
	o.sOpt = s;
	o.sHelp = sHelp;
	o.bShortHasParams = false;
	addOption( o );
}

int Bu::OptParser::optHelp( StrArray aParams )
{
	bool bHasShort = false;
	int iMaxWidth = 0;
	int iScrWidth = 80;
	char *env = getenv("COLUMNS");
	if( env )
		iScrWidth = strtol( env, NULL, 10 );
	for( OptionList::iterator i = lOption.begin(); i; i++ )
	{
		if( (*i).cOpt != '\0' )
			bHasShort = true;
		if( (*i).sOpt && iMaxWidth < (*i).sOpt.getSize() )
			iMaxWidth = (*i).sOpt.getSize();
	}
	int iIndent = 4;
	if( bHasShort )
		iIndent += 4;
	if( iMaxWidth > 0 )
		iIndent += 4 + iMaxWidth;
	for( OptionList::iterator i = lOption.begin(); i; i++ )
	{
		sio << "    ";
		if( bHasShort )
		{
			if( (*i).cOpt == '\0' )
				sio << "  ";
			else
				sio << "-" << (*i).cOpt;
			sio << "  ";
		}
		if( iMaxWidth > 0 )
		{
			if( (*i).sOpt )
			{
				sio << "--" << Fmt(iMaxWidth, Fmt::Left) << (*i).sOpt;
			}
			else
			{
				sio << "  " << Fmt(iMaxWidth) << "";
			}
			sio << "  ";
		}
		sio << format( (*i).sHelp, iScrWidth-iIndent-1, iIndent );
		sio << sio.nl;
	}
	exit( 0 );
	return 0;
}

Bu::FString Bu::OptParser::format( const Bu::FString &sIn, int iWidth,
		int iIndent )
{
	Bu::FString sOut;
	Bu::FString sIndent;
	for( int j = 0; j < iIndent; j++ )
		sIndent.append(" ", 1);
	bool bFirst = true;
	int iSpaceCount = 0;
	bool bSpace = false;
	int iPrevLineLen;
	int iLineLen = 0;
	char c;
	Bu::FString::const_iterator iLastSpace, iStart;
	for( Bu::FString::const_iterator i = iLastSpace = iStart = sIn.begin(); i; i++ )
	{
		c = *i;
		if( *i == ' ' )
		{
			if( bSpace == false )
			{
				iLastSpace = i;
				iSpaceCount++;
				bSpace = true;
				iPrevLineLen = iLineLen;
			}
		}
		else
		{
			bSpace = false;
		}
		iLineLen++;

		if( iLineLen >= iWidth )
		{
			iSpaceCount--;
			if( bFirst == true )
				bFirst = false;
			else
				sOut += sIndent;
			int iExtraSpaces = iWidth-iPrevLineLen;
			bSpace = false;
			float fFill = 0.0;
			int iSubSpaceCount = 0;
			float fAdd = ((float)iExtraSpaces/(float)iSpaceCount);
			for( Bu::FString::const_iterator k = iStart; k != iLastSpace; k++ )
			{
				sOut += *k;
				if( *k == ' ' )
				{
					if( bSpace == false && iExtraSpaces > 0 )
					{
						bSpace = true;
						fFill += fAdd;
						iSubSpaceCount++;
						for( int sp = 0; sp < (int)(fFill); sp++ )
						{
							sOut += ' ';
							iExtraSpaces--;
						}
						fFill -= (int)fFill;
						if( iSubSpaceCount == iSpaceCount && iExtraSpaces > 0 )
						{
							for(; iExtraSpaces > 0; iExtraSpaces-- )
							{
								sOut += ' ';
							}
						}
					}
				}
				else
					bSpace = false;
			}
			//sOut.append( iStart, iLastSpace );
			sOut.append("\n");
			for(; iLastSpace && *iLastSpace == ' '; iLastSpace++ ) { }
			iStart = i = iLastSpace;
			bSpace = false;
			iLineLen = 1;
			iSpaceCount = 0;
		}
	}
	if( !bFirst )
		sOut += sIndent;
	sOut.append( iStart );
	return sOut;
}


//
// Code for Bu::OptParser::_ValueProxy
//

Bu::OptParser::_ValueProxy::_ValueProxy()
{
}

Bu::OptParser::_ValueProxy::~_ValueProxy()
{
}

//
// Code for Bu::OptParser::Option
//

Bu::OptParser::Option::Option() :
	cOpt( '\0' ),
	bShortHasParams( false ),
	pProxy( NULL )
{
}

Bu::OptParser::Option::Option( const Option &rSrc ) :
	cOpt( rSrc.cOpt ),
	sOpt( rSrc.sOpt ),
	sHelp( rSrc.sHelp ),
	sUsed( rSrc.sUsed ),
	bShortHasParams( rSrc.bShortHasParams ),
	pProxy( NULL ),
	sOverride( rSrc.sOverride )
{
	if( rSrc.pProxy )
		pProxy = rSrc.pProxy->clone();
}

Bu::OptParser::Option::~Option()
{
	delete pProxy;
	pProxy = NULL;
}

