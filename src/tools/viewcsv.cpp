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
	Options( int argc, char *argv[] )
	{
		setNonOption( slot( this, &Options::onNonOption ) );
		addHelpOption();
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

		int iRows = min( (int)doc.sgData.getSize(), maxy-1 );
		int iCols = min( doc.iMaxCols, (int)maxx-1 );

		int iHdrHeight = 1;
		if( bHeaderRow )
			iHdrHeight++;

		// Draw the headers
		for( int iRow = 0; iRow < iRows; iRow++ )
		{
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
				if( iXPos + doc.aWidths[iCol+iXOff]+2 >= maxx )
					break;
				char buf[6];
				snprintf( buf, 6, "%d", iCol+iXOff );
				mvaddch( 0, iXPos, ACS_VLINE );
				mvaddch( iHdrHeight, iXPos, ACS_PLUS );
				mvaddnstr( 0, iXPos+1, buf, 5 );
				if( bHeaderRow )
				{
					mvaddstr( 1, iXPos+1, doc.sgData[0][iCol+iXOff].getStr() );
					mvaddch( 1, iXPos, ACS_VLINE );
				}
				for( int j = 0; j <= doc.aWidths[iCol+iXOff]; j++ )
				{
					mvaddch( iHdrHeight, iXPos+j+1, ACS_HLINE );
				}
				iXPos += doc.aWidths[iCol+iXOff]+2;
			}
		}
		catch(...) { }

		// Draw some data
		for( int iRow = 0; iRow < iRows; iRow++ )
		{
			try
			{
			int iXPos = 6;
			for( int iCol = 0; iCol < iCols; iCol++ )
			{
				if( iXPos + doc.aWidths[iCol+iXOff]+2 >= maxx )
					break;
				mvaddch( iRow+iHdrHeight+1, iXPos, ACS_VLINE );
				mvaddstr( iRow+iHdrHeight+1, iXPos+1,
						doc.sgData[iRow+iYOff][iCol+iXOff].getStr() );
				iXPos += doc.aWidths[iCol+iXOff]+2;
			}
			} catch(...) { }
		}
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
		bHeaderRow = bOn;
		move( 0, 0 );
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
	view.toggleHeaderRow();

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

