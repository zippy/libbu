/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <bu/sio.h>
#include <bu/lexer.h>
#include <bu/parser.h>
#include <bu/file.h>
#include <bu/queuebuf.h>

using namespace Bu;

enum TokenType
{
    tokIdentifier,
    tokColon,
    tokOr,
    tokSemiColon,
    tokTokens,
    tokEquals,
    tokOpenCurly,
    tokCloseCurly,
    tokOpenSquare,
    tokCloseSquare,

    tokEos=-1
};

class BnfLexer : public Lexer
{
public:
    BnfLexer( Stream &rSrc ) :
        rSrc( rSrc )
    {
    }

    virtual ~BnfLexer()
    {
    }

    virtual Token *nextToken()
    {
        char cBuf;

        for(;;)
        {
            if( qbIn.getSize() == 0 )
            {
                char buf[4096];
                qbIn.write( buf, rSrc.read( buf, 4096 ) );

                if( rSrc.isEos() && qbIn.getSize() == 0 )
                    return new Token( tokEos );
            }
            qbIn.peek( &cBuf, 1 );
            if( (cBuf >= 'a' && cBuf <= 'z') ||
                (cBuf >= 'A' && cBuf <= 'Z') ||
                (cBuf >= '0' && cBuf <= '9') ||
                cBuf == '_' )
            {
                sBuf.append( cBuf );
                qbIn.seek( 1 );
            }
            else if( sBuf.isSet() )
            {
                if( sBuf == "tokens" )
                {
                    sBuf.clear();
                    return new Token( tokTokens );
                }
                else
                {
                    Token *pRet = new Token( tokIdentifier, sBuf );
                    sBuf.clear();
                    return pRet;
                }
            }
            else
            {
                switch( cBuf )
                {
                    case ' ':
                    case '\t':
                    case '\n':
                    case '\r':
                        qbIn.seek( 1 );
                        continue;

                    case ':':
                        qbIn.seek( 1 );
                        return new Token( tokColon );

                    case ';':
                        qbIn.seek( 1 );
                        return new Token( tokSemiColon );

                    case '|':
                        qbIn.seek( 1 );
                        return new Token( tokOr );

                    case '=':
                        qbIn.seek( 1 );
                        return new Token( tokEquals );

                    case '[':
                        qbIn.seek( 1 );
                        return new Token( tokOpenSquare );

                    case ']':
                        qbIn.seek( 1 );
                        return new Token( tokCloseSquare );

                    case '{':
                        qbIn.seek( 1 );
                        return new Token( tokOpenCurly );

                    case '}':
                        qbIn.seek( 1 );
                        return new Token( tokCloseCurly );

                    default:
                        throw ExceptionBase("Unexpected character '%c'.",
                            cBuf );
                        break;
                }
            }
        }
    }

    virtual String tokenToString( const Token &t )
    {
        switch( (TokenType)t.iToken )
        {
            case tokIdentifier:     return "tokIdentifier";
            case tokColon:          return "tokColon";
            case tokOr:             return "tokOr";
            case tokSemiColon:      return "tokSemiColon";
            case tokTokens:         return "tokTokens";
            case tokEquals:         return "tokEquals";
            case tokOpenCurly:      return "tokOpenCurly";
            case tokCloseCurly:     return "tokCloseCurly";
            case tokOpenSquare:     return "tokOpenSquare";
            case tokCloseSquare:    return "tokCloseSquare";
            case tokEos:            return "tokEos";
        }

        return "???";
    }

private:
    Stream &rSrc;
    QueueBuf qbIn;
    String sBuf;
};

class BnfParser
{
public:
    BnfParser( BnfLexer &l ) :
        l( l ),
        pCur( NULL ),
        iLastToken( 0 )
    {
    }

    virtual ~BnfParser()
    {
        delete pCur;
        pCur = NULL;
    }

    void parse()
    {
        for(;;)
        {
            next();
            switch( pCur->iToken )
            {
                case tokTokens:
                    tokens();
                    break;

                case tokIdentifier:
                    nonTerminal();
                    break;

                case tokEos:
                    return;
                    break;

                default:
                    tokenError("tokTokens, tokIdentifier, or tokEos");
            }
        }
    }

private:
    void tokens()
    {
        next();
        if( pCur->iToken != tokEquals )
            tokenError("tokEquals");
        for(;;)
        {
            next();
            if( pCur->iToken == tokIdentifier )
            {
                hTokens.insert( pCur->vExtra.get<Bu::String>(), ++iLastToken );
                sio << "Added token[" << iLastToken << "]: "
                    << pCur->vExtra.get<Bu::String>() << sio.nl;
            }
            else if( pCur->iToken == tokSemiColon )
                break;
            else
                tokenError("tokIdentifier or tokSemiColon");
        }
    }

    void nonTerminal()
    {
        Bu::String sNtName = pCur->vExtra.get<Bu::String>();
        Parser::NonTerminal nt;
        p.addNonTerminal( sNtName );
        sio.incIndent();
        sio << "Created non-terminal: " << sNtName << sio.nl;

        next();
        if( pCur->iToken != tokColon )
            tokenError("tokColon");
        production( nt );
        for(;;)
        {
            switch( pCur->iToken )
            {
                case tokOr:
                    production( nt );
                    break;

                case tokSemiColon:
                    p.setNonTerminal( sNtName, nt );
                    sio.decIndent();
                    sio << "Closing non-terminal." << sio.nl;
                    return;

                default:
                    tokenError("tkOr or tokSemiColon");
                    break;
            }
        }
    }

    void production( Parser::NonTerminal &nt )
    {
        sio.incIndent();
        sio << "Adding new production:" << sio.nl;
        Parser::Production pr;
        bool bAnything = false;
        for(;;)
        {
            next();
            switch( pCur->iToken )
            {
                case tokIdentifier:
                    {
                        const Bu::String &sName =
                            pCur->vExtra.get<Bu::String>();
                        if( hTokens.has( sName ) )
                        {
                            pr.append(
                                Parser::State(
                                    Parser::State::typeTerminal,
                                    hTokens.get( sName )
                                    )
                                );
                            sio << "Added terminal " << sName << sio.nl;
                        }
                        else
                        {
                            if( !p.hasNonTerminal( sName ) )
                            {
                                p.addNonTerminal( sName );
                            }
                            pr.append(
                                Parser::State(
                                    Parser::State::typeNonTerminal,
                                    p.getNonTerminalId( sName )
                                    )
                                );
                            sio << "Added non-terminal " << sName << sio.nl;
                        }
                    }
                    break;

                case tokOpenSquare:
                    {
                        next();
                        if( pCur->iToken != tokIdentifier )
                            tokenError("tokIdentifier");
                        Bu::String sName =
                            pCur->vExtra.get<Bu::String>();
                        next();
                        if( pCur->iToken != tokCloseSquare )
                            tokenError("tokCloseSquare");

                        if( !hTokens.has( sName ) )
                            throw ExceptionBase("Only token names may be "
                                "enclosed in square brackets.");

                        pr.append(
                            Parser::State(
                                Parser::State::typeTerminalPush,
                                hTokens.get( sName )
                                )
                            );
                        sio << "Added terminal-push " << sName << sio.nl;
                    }
                    break;

                case tokOpenCurly:
                    {
                        next();
                        if( pCur->iToken != tokIdentifier )
                            tokenError("tokIdentifier");
                        Bu::String sName =
                            pCur->vExtra.get<Bu::String>();
                        next();
                        if( pCur->iToken != tokCloseCurly )
                            tokenError("tokCloseCurly");

                        if( !p.hasReduction( sName ) )
                            p.addReduction( sName );

                        pr.append(
                            Parser::State(
                                Parser::State::typeReduction,
                                p.getReductionId( sName )
                                )
                            );
                        sio << "Added reduction " << sName << sio.nl;
                    }
                    break;

                case tokOr:
                case tokSemiColon:
                    if( bAnything )
                    {
                        nt.addProduction( pr );
                        sio.decIndent();
                        sio << "Closing production." << sio.nl;
                    }
                    else
                    {
                        nt.setCanSkip();
                        sio.decIndent();
                        sio << "Closing empty production." << sio.nl;
                    }
                    return;

                default:
                    tokenError("tokIdentifier, tokOpenSquare, tokOr, "
                            "tokOpenCurly, or tokSemiColon");
            }
        }
    }

private:
    void next()
    {
        delete pCur;
        pCur = l.nextToken();
    }

    void tokenError( const String &s )
    {
        throw ExceptionBase( ("Expected " + s + " but found "
                + l.tokenToString( *pCur ) + ".").getStr() );
    }

private:
    typedef Bu::Hash<Bu::String, int> TokenHash;
    TokenHash hTokens;
    BnfLexer &l;
    BnfLexer::Token *pCur;
    int iLastToken;
    Parser p;
};

int main( int argc, char *argv[] )
{
    if( argc < 2 )
    {
        println("Provide an input filename as the first parameter.");
        return 0;
    }
    File fIn( argv[1], File::Read );

    BnfLexer bl( fIn );
    BnfParser parser( bl );

    parser.parse();

/*
    for(;;)
    {
        Lexer::Token *pTok = bl.nextToken();
        sio << bl.tokenToString(*pTok);
        if( pTok->vExtra.isSet() )
        {
            sio << " - " << pTok->vExtra;
        }
        sio << sio.nl;
        if( pTok->iToken == tokEos )
            break;
    }
*/

    return 0;
}

