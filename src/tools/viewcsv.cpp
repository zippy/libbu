#include <bu/sio.h>
#include <bu/optparser.h>
#include <bu/csvreader.h>
#include <bu/file.h>
#include <bu/util.h>
#include <ncurses.h>

using namespace Bu;

class Options : public Bu::OptParser
{
public:
	Options( int argc, char *argv[] ) :
		bHeader( true )
	{
		addOption( bHeader, "no-header",
			"Don't use the first line as a header row.  This behaviour can "
			"also be toggled while running with 'h'." );
		setNonOption( slot( this, &Options::onNonOption ) );
		addHelpOption();

		setOverride( "no-header", "true" );
		parse( argc, argv );
	}

	virtual ~Options()
	{
	}

	int onNonOption( StrArray aParams )
	{
		//sio << aParams << sio.nl;
		sFileIn = aParams[0];

		return 0;
	}

	Bu::FString sFileIn;
	bool bHeader;
};

typedef Bu::Array<StrArray> StrGrid;
typedef Bu::Array<int> IntArray;
class CsvDoc
{
public:
	CsvDoc() :
		iMaxCols( 0 )
	{
	}

	virtual ~CsvDoc()
	{
	}

	void addRow( StrArray aStr )
	{
		sgData.append( aStr );
		if( iMaxCols < aStr.getSize() )
			iMaxCols = aStr.getSize();
		while( aWidths.getSize() < iMaxCols )
		{
			aWidths.append( 0 );
		}
		for( int j = 0; j < aStr.getSize(); j++ )
		{
			if( aWidths[j] < aStr[j].getSize() )
				aWidths[j] = aStr[j].getSize();
		}
	}

	int iMaxCols;
	StrGrid sgData;
	IntArray aWidths;
};

class CsvView
{
public:
	CsvView( CsvDoc &doc ) :
		doc( doc ),
		iXOff( 0 ),
		iYOff( 0 ),
		bHeaderRow( false )
	{
	}

	virtual ~CsvView()
	{
	}

	void render()
	{
		erase();
		int maxx, maxy;
		getmaxyx( stdscr, maxy, maxx );

		int iRows = min( (int)doc.sgData.getSize(), maxy-((bHeaderRow)?(4):(3)) );
		int iCols = min( doc.iMaxCols, (int)maxx-1 );

		int iHdrHeight = 1;
		if( bHeaderRow )
			iHdrHeight++;

		// Draw the headers
		for( int iRow = 0; iRow < iRows; iRow++ )
		{
			if( iRow+iYOff >= doc.sgData.getSize() )
				break;
			char buf[6];
			snprintf( buf, 6, "%5d", iRow+iYOff );
			mvaddnstr( iRow+iHdrHeight+1, 0, buf, 5 );
			mvaddch( iRow+iHdrHeight+1, 6, ACS_VLINE );
		}
		int iXPos = 6;
		try
		{
			for( int iCol = 0; iCol < iCols; iCol++ )
			{
				if( iXPos >= maxx )
					break;
				int iWidth = min( doc.aWidths[iCol+iXOff], maxx-iXPos-1 );
				char buf[6];
				snprintf( buf, 6, "%d", iCol+iXOff );
				mvaddch( 0, iXPos, ACS_VLINE );
				mvaddch( iHdrHeight, iXPos, ACS_PLUS );
				mvaddnstr( 0, iXPos+1, buf, 5 );
				if( bHeaderRow )
				{
					mvaddnstr(
						1, iXPos+1, doc.sgData[0][iCol+iXOff].getStr(), iWidth
						);
					mvaddch( 1, iXPos, ACS_VLINE );
				}
				for( int j = 0; j < iWidth; j++ )
				{
					mvaddch( iHdrHeight, iXPos+j+1, ACS_HLINE );
				}
				iXPos += iWidth+1;
			}
		}
		catch(...) { }
		for( int j = 0; j < 6; j++ )
		{
			mvaddch( iHdrHeight, j, ACS_HLINE );
		}

		// Draw some data
		for( int iRow = 0; iRow < iRows; iRow++ )
		{
			try
			{
			int iXPos = 6;
			for( int iCol = 0; iCol < iCols; iCol++ )
			{
				if( iXPos >= maxx )
					break;
				int iWidth = min( doc.aWidths[iCol+iXOff], maxx-iXPos-1 );
				mvaddch( iRow+iHdrHeight+1, iXPos, ACS_VLINE );
				mvaddnstr( iRow+iHdrHeight+1, iXPos+1,
						doc.sgData[iRow+iYOff][iCol+iXOff].getStr(), iWidth );
				iXPos += iWidth+1;
			}
			} catch(...) { }
		}

		attron( A_REVERSE );
		for( int j = 0; j < maxx; j++ )
		{
			mvaddch( maxy-1, j, ' ' );
		}
		mvaddstr( maxy-1, 1, "q) quit  h) toggle header row" );
		char buf[30];
		int iWidth = sprintf( buf, "[%dx%ld]",
			doc.iMaxCols, doc.sgData.getSize()
			);
		mvaddstr( maxy-1, maxx-iWidth-1, buf );
		attroff( A_REVERSE );
	}

	void move( int iX, int iY )
	{
		iXOff += iX;
		iYOff += iY;
		if( iXOff < 0 )
			iXOff = 0;

		if( bHeaderRow )
		{
			if( iYOff < 1 )
				iYOff = 1;
		}
		else
		{
			if( iYOff < 0 )
				iYOff = 0;
		}

		if( iYOff >= doc.sgData.getSize() )
			iYOff = doc.sgData.getSize()-1;

		if( iXOff >= doc.iMaxCols )
			iXOff = doc.iMaxCols-1;
	}

	void pageDown()
	{
		int maxx, maxy;
		getmaxyx( stdscr, maxy, maxx );
		move( 0, maxy-2 );
	}

	void pageUp()
	{
		int maxx, maxy;
		getmaxyx( stdscr, maxy, maxx );
		move( 0, -(maxy-2) );
	}

	void setHeaderRow( bool bOn )
	{
		if( bHeaderRow == bOn )
			return;

		bHeaderRow = bOn;
		move( 0, ((bOn)?(1):(-1)) );
	}

	void toggleHeaderRow()
	{
		setHeaderRow( !bHeaderRow );
	}

	CsvDoc &doc;
	int iXOff;
	int iYOff;
	bool bHeaderRow;
};

int main( int argc, char *argv[] )
{
	Options opt( argc, argv );

	if( !opt.sFileIn.isSet() )
	{
		sio << "No file specified." << sio.nl;
		return 1;
	}

	CsvDoc doc;
	File fIn( opt.sFileIn, File::Read );
	CsvReader cr( fIn );

	while( !fIn.isEos() )
	{
		StrArray sa = cr.readLine();
		if( fIn.isEos() )
			break;
		doc.addRow( sa );
	}

	initscr();
	cbreak();
	noecho();
	nonl();
	intrflush( stdscr, FALSE );
	keypad( stdscr, TRUE ); 
	curs_set( 0 );

	CsvView view( doc );
	view.setHeaderRow( opt.bHeader );

	bool bRun = true;
	do
	{
		view.render();

		int ch = getch();
		switch( ch )
		{
			case 'q':
				bRun = false;
				break;

			case KEY_DOWN:
				view.move( 0, 1 );
				break;

			case KEY_UP:
				view.move( 0, -1 );
				break;

			case KEY_LEFT:
				view.move( -1, 0 );
				break;

			case KEY_RIGHT:
				view.move( 1, 0 );
				break;

			case KEY_NPAGE:
				view.pageDown();
				break;

			case KEY_PPAGE:
				view.pageUp();
				break;

			case 'h':
				view.toggleHeaderRow();
				break;
		}
	} while( bRun );

	endwin();

	return 0;
}

