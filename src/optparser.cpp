/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

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
				int iEPos;
				for( iEPos = 2; argv[j][iEPos] != '\0' &&
					 argv[j][iEPos] != '='; iEPos++ ) { }

				Bu::String sOpt;
				int iCount = argc-j;
				Bu::String sExtraParam;
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
				if( !hlOption.has( sOpt ) )
				{
					optionError( "--" + sOpt );
				}
				else
				{
					// Long param, cool, that's easy, first search for =
					Option *pOpt = hlOption.get( sOpt );
					if( pOpt->sUsed )
					{
						Bu::StrArray aParams( iCount );
						aParams.append( sOpt );
						if( sExtraParam.isSet() )
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
						if( pOpt->sOverride.isSet() )
						{
							pOpt->pProxy->setValue( pOpt->sOverride );
						}
						else if( sExtraParam.isSet() )
						{
							pOpt->pProxy->setValueFromStr( sExtraParam );
						}
						else if( argv[j+1] != '\0' )
						{
							pOpt->pProxy->setValueFromStr( argv[j+1] );
							j++;
						}
					}
				}
			}
			else
			{
				int iCPos;
				for( iCPos = 1; argv[j][iCPos] != '\0'; iCPos++ )
				{
					if( !hsOption.has( argv[j][iCPos] ) )
					{
						Bu::String sOpt("-");
						sOpt += argv[j][iCPos];
						optionError( sOpt );
					}
					else
					{
						Option *pOpt = hsOption.get( argv[j][iCPos] );
						char buf[2] = {argv[j][iCPos], '\0'};
						if( pOpt->sUsed )
						{
							Bu::StrArray aParams( argc-j+1 );
							aParams.append( buf );
							int iMod = 0;
							if( argv[j][iCPos+1] != '\0' )
							{
								aParams.append( argv[j]+iCPos+1 );
								iMod = -1;
							}
							for( int k = j+1; k < argc; k++ )
							{
								aParams.append( argv[k] );
							}
							int iUsed = pOpt->sUsed( aParams );
							if( iUsed > 0 )
							{
								j += iUsed + iMod;
								break;
							}
						}
						else if( pOpt->pProxy )
						{
							if( pOpt->sOverride.isSet() )
							{
								pOpt->pProxy->setValue( pOpt->sOverride );
							}
							else if( argv[j][iCPos+1] != '\0' )
							{
								pOpt->pProxy->setValueFromStr(
									argv[j]+iCPos+1
									);
								break;
							}
							else if( argv[j+1] )
							{
								pOpt->pProxy->setValueFromStr(
									argv[j+1]
									);
								j++;
								break;
							}
						}
					}
				}
			}
		}
		else
		{
			if( !sNonOption )
			{
				optionError( argv[j] );
			}
			else
			{
				int iCount = argc-j;
				Bu::StrArray aParams( iCount );
				for( int k = j; k < argc; k++ )
				{
					aParams.append( argv[k] );
				}
				j += sNonOption( aParams );
			}
		}
	}
}

void Bu::OptParser::parse( const Bu::String &sLine )
{
	Bu::String sCmd = sLine.clone();
	int iParams = 0;
	bool bInGap = true;
	bool bInQuote = false;
	for( Bu::String::iterator i = sCmd.begin(); i; i++ )
	{
		if( bInQuote == false && (*i == ' ' || *i == '\t') )
		{
			if( bInGap == false )
			{
				bInGap = true;
			}
		}
		else if( *i == '"' )
		{
			bInQuote = !bInQuote;
		}
		else
		{
			if( bInGap )
			{
				iParams++;
				bInGap = false;
			}
		}
	}

	bInQuote = false;
	bInGap = true;
	char **asParam = new char*[iParams];
	iParams = 0;
	for( char *i = sCmd.getStr(); *i; i++ )
	{
		if( bInQuote == false && (*i == ' ' || *i == '\t') )
		{
			if( bInGap == false )
			{
				bInGap = true;
				*i = '\0';
			}
		}
		else if( *i == '"' )
		{
			bInQuote = !bInQuote;
		}
		else
		{
			if( bInGap )
			{
				asParam[iParams++] = i;
				bInGap = false;
			}
		}
	}

	parse( iParams, asParam );

	delete[] asParam;
}

void Bu::OptParser::addOption( const Option &opt )
{
	lOption.append( opt );
	if( opt.cOpt != '\0' )
		hsOption.insert( opt.cOpt, &lOption.last() );
	if( opt.sOpt.isSet() )
		hlOption.insert( opt.sOpt, &lOption.last() );
}

void Bu::OptParser::setOverride( char cOpt, const Bu::Variant &sOverride )
{
	hsOption.get( cOpt )->sOverride = sOverride;
}

void Bu::OptParser::setOverride( const Bu::String &sOpt, const Bu::Variant &sOverride )
{
	hlOption.get( sOpt )->sOverride = sOverride;
}

void Bu::OptParser::setHelpDefault( const Bu::String &sOpt, const Bu::String &sTxt )
{
	hlOption.get( sOpt )->sHelpDefault = sTxt;
}

void Bu::OptParser::addHelpOption( char c, const Bu::String &s, const Bu::String &sHelp )
{
	Option o;
	o.sUsed = slot( this, &OptParser::optHelp );
	o.cOpt = c;
	o.sOpt = s;
	o.sHelp = sHelp;
	addOption( o );
}

void Bu::OptParser::addHelpBanner( const Bu::String &sText, bool bFormatted )
{
	Banner b;
	b.sText = sText;
	b.bFormatted = bFormatted;
	if( lOption.getSize() > 0 )
	{
		for( b.iAfter = lOption.begin(); b.iAfter+1; b.iAfter++ ) { }
	}
	lBanner.append( b );
}

int Bu::OptParser::optHelp( StrArray /*aParams*/ )
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
		int lOptSize = (*i).sOpt.getSize() + (*i).sHelpDefault.getSize();
		if( (*i).sOpt.isSet() && iMaxWidth < lOptSize )
			iMaxWidth = lOptSize;
	}
	int iIndent = 4;
	if( bHasShort )
		iIndent += 4;
	if( iMaxWidth > 0 )
		iIndent += 4 + iMaxWidth;

	BannerList::iterator iBanner;
	for( iBanner = lBanner.begin(); iBanner; iBanner++ )
	{
		if( (*iBanner).iAfter )
			break;

		if( (*iBanner).bFormatted )
			sio << format( (*iBanner).sText, iScrWidth-1, 0 );
		else
			sio << (*iBanner).sText;
		sio << sio.nl;
	}
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
			if( (*i).sOpt.isSet() )
			{
				sio << "--" << Fmt(iMaxWidth, Fmt::Left)
					<< (*i).sOpt + (*i).sHelpDefault;
			}
			else
			{
				sio << "  " << Fmt(iMaxWidth) << "";
			}
			sio << "  ";
		}
		sio << format( (*i).sHelp, iScrWidth-iIndent-1, iIndent );
		sio << sio.nl;
	
		for( ; iBanner; iBanner++ )
		{
			if( (*iBanner).iAfter != i )
				break;
			
			if( (*iBanner).bFormatted )
				sio << format( (*iBanner).sText, iScrWidth-1, 0 );
			else
				sio << (*iBanner).sText;
			sio << sio.nl;
		}
	}
	exit( 0 );
	return 0;
}

void Bu::OptParser::optionError( const Bu::String &sOption )
{
	sio << "Unregcognized option discovered: " << sOption << sio.nl << sio.nl;
	exit( 1 );
}

void Bu::OptParser::setNonOption( OptionSignal sSignal )
{
	sNonOption = sSignal;
}

Bu::String Bu::OptParser::format( const Bu::String &sIn, int iWidth,
		int iIndent )
{
	Bu::String sOut;
	Bu::String sIndent;
	for( int j = 0; j < iIndent; j++ )
		sIndent.append(" ", 1);
	bool bFirst = true;
	int iSpaceCount = 0;
	bool bSpace = false;
	int iPrevLineLen;
	int iLineLen = 0;
	char c;
	Bu::String::const_iterator iLastSpace, iStart;
	for( Bu::String::const_iterator i = iLastSpace = iStart = sIn.begin(); i; i++ )
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
			for( Bu::String::const_iterator k = iStart; k != iLastSpace; k++ )
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
	pProxy( NULL )
{
}

Bu::OptParser::Option::Option( const Option &rSrc ) :
	cOpt( rSrc.cOpt ),
	sOpt( rSrc.sOpt ),
	sHelp( rSrc.sHelp ),
	sUsed( rSrc.sUsed ),
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

