/*
  Copyright (C) 2013 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#ifndef __CSOUND_TOKER__
#define __CSOUND_TOKER__

#include <JuceHeader.h>

class CsoundTokeniser : public CodeTokeniser
{
public:
    CsoundTokeniser() {}
    ~CsoundTokeniser() {}

    //==============================================================================
    enum TokenType
    {
        tokenType_error = 0,
        tokenType_comment,
        tokenType_builtInKeyword,
        tokenType_identifier,
        tokenType_integerLiteral,
        tokenType_floatLiteral,
        tokenType_stringLiteral,
        tokenType_operator,
        tokenType_bracket,
        tokenType_punctuation,
        tokenType_preprocessor,
        tokenType_csdTag
    };

    CodeEditorComponent::ColourScheme getDefaultColourScheme()
    {
        struct Type
        {
            const char* name;
            uint32 colour;
        };

        const Type types[] =
        {
            { "Error",              Colours::red.getARGB() },
            { "Comment",            Colours::red.getARGB() },
            { "Keyword",            Colours::red.getARGB() },
            { "Identifier",         Colours::red.getARGB() },
            { "Integer",            Colours::red.getARGB() },
            { "Float",              Colours::red.getARGB() },
            { "String",             Colours::red.getARGB() },
            { "Operator",           Colours::red.getARGB() },
            { "Bracket",            Colours::red.getARGB() },
            { "Punctuation",        Colours::red.getARGB() },
            { "Preprocessor Text",  Colours::red.getARGB() },
            { "Csd Tag",            Colours::red.getARGB() }
        };

        CodeEditorComponent::ColourScheme cs;

        for ( std::size_t i = 0; i < int (sizeof (types) / sizeof (types[0])); ++i) // (NB: numElementsInArray doesn't work here in GCC4.2)
            cs.set (types[i].name, Colour (types[i].colour));

        return cs;
    }


private:
    //==============================================================================
    StringArray getTokenTypes()
    {
        StringArray s;
        s.add ("Error");
        s.add ("Comment");
        s.add ("C++ keyword");
        s.add ("Identifier");
        s.add ("Integer literal");
        s.add ("Float literal");
        s.add ("String literal");
        s.add ("Operator");
        s.add ("Bracket");
        s.add ("Punctuation");
        s.add ("Preprocessor line");
        s.add ("CSD Tag");
        return s;
    }

    //==============================================================================
    void skipQuotedString (CodeDocument::Iterator& source)
    {
        const juce_wchar quote = source.nextChar();

        for (;;)
        {
            const juce_wchar c = source.nextChar();

            if (c == quote || c == 0)
                break;

            if (c == '\\')
                source.skip();
        }
    }


    //==============================================================================
    void skipCSDTag (CodeDocument::Iterator& source) noexcept
    {
        for (;;)
        {
            const juce_wchar c = source.nextChar();

            if (c == 0 || (c == '>'))
                break;
        }
    }

    //==============================================================================
    bool isIdentifierStart (const char c)
    {
        return CharacterFunctions::isLetter (c)
               || c == '_' || c == '@';
    }

    //==============================================================================
    bool isIdentifierBody (const char c)
    {
        return CharacterFunctions::isLetter (c)
               || CharacterFunctions::isDigit (c)
               || c == '_' || c == '@';
    }

    //==============================================================================
    bool isReservedKeyword (String::CharPointerType token, const int tokenLength) noexcept
    {
        //populate char array with Csound keywords
        //this list of keywords is not completely up to date!
        const char* const* k;

        if (tokenLength < 2 || tokenLength > 16)
            return false;

        else
            k = CsoundKeywords;


        int i = 0;

        while (k[i] != 0)
        {
            if (token.compare (CharPointer_ASCII (k[i])) == 0)
                return true;

            ++i;
        }

        return false;
    }

    //==============================================================================
    int parseIdentifier (CodeDocument::Iterator& source) noexcept
    {
        int tokenLength = 0;
        String::CharPointerType::CharType possibleIdentifier [20];
        String::CharPointerType possible (possibleIdentifier);

        while (isIdentifierBody (source.peekNextChar()))
        {
            const juce_wchar c = source.nextChar();

            if (tokenLength < 20)
                possible.write (c);

            ++tokenLength;
        }

        if (tokenLength > 1 && tokenLength <= 16)
        {
            possible.writeNull();

            if (isReservedKeyword (String::CharPointerType (possibleIdentifier), tokenLength))
                return CsoundTokeniser::tokenType_builtInKeyword;
        }

        return CsoundTokeniser::tokenType_identifier;
    }

    //==============================================================================
    int readNextToken (CodeDocument::Iterator& source)
    {
        int result = tokenType_error;
        source.skipWhitespace();
        char firstChar = source.peekNextChar();

        switch (firstChar)
        {
            case 0:
                source.skip();
                break;

            case ';':
                source.skipToEndOfLine();
                result = tokenType_comment;
                break;

            case '"':
                //   case T('\''):
                skipQuotedString (source);
                result = tokenType_stringLiteral;
                break;


            case '/':
                //result = CPlusPlusCodeTokeniser::tokenType_operator;
                source.skip();

                if (source.peekNextChar() == '=')
                {
                    source.skip();
                }
                else if (source.peekNextChar() == '/')
                {
                    result = tokenType_comment;
                    source.skipToEndOfLine();
                }
                else if (source.peekNextChar() == '*')
                {
                    source.skip();
                    result = tokenType_comment;
                    bool lastWasStar = false;

                    for (;;)
                    {
                        const juce_wchar c = source.nextChar();

                        if (c == 0 || (c == '/' && lastWasStar))
                            break;

                        lastWasStar = (c == '*');
                    }

                }

                break;


            case '<':
                source.skip();

                if ((source.peekNextChar() == 'C') ||
                    (source.peekNextChar() == '/'))
                {
                    skipCSDTag (source);
                    result = tokenType_csdTag;
                }

                break;

            default:
                if (isIdentifierStart (firstChar))
                {
                    result = parseIdentifier (source);
                }
                else
                    source.skip();

                break;
        }

        //jassert (result != tokenType_unknown);
        return result;
    }
};

#endif
