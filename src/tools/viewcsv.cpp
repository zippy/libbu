#include <bu/sio.h>
#include <bu/optparser.h>
#include <bu/csvreader.h>
#include <bu/file.h>

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

	sio << "Csv grid:  " << doc.iMaxCols << " x " << doc.sgData.getSize()
		<< sio.nl;

	return 0;
}

