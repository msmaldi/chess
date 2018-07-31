***** PGN-SPEC *****

Standard: Portable Game Notation Specification and Implementation Guide

Revised: 1994.03.12

Authors: Interested readers of the Internet newsgroup rec.games.chess

Coordinator: Steven J. Edwards (send comments to sje@world.std.com)


0: Preface

From the Tower of Babel story:

"If now, while they are one people, all speaking the same language, they have
started to do this, nothing will later stop them from doing whatever they
propose to do."

Genesis XI, v.6, _New American Bible_


1: Introduction

PGN is "Portable Game Notation", a standard designed for the representation of
chess game data using ASCII text files.  PGN is structured for easy reading and
writing by human users and for easy parsing and generation by computer
programs.  The intent of the definition and propagation of PGN is to facilitate
the sharing of public domain chess game data among chessplayers (both organic
and otherwise), publishers, and computer chess researchers throughout the
world.

PGN is not intended to be a general purpose standard that is suitable for every
possible use; no such standard could fill all conceivable requirements.
Instead, PGN is proposed as a universal portable representation for data
interchange.  The idea is to allow the construction of a family of chess
applications that can quickly and easily process chess game data using PGN for
import and export among themselves.


2: Chess data representation

Computer usage among chessplayers has become quite common in recent years and a
variety of different programs, both commercial and public domain, are used to
generate, access, and propagate chess game data.  Some of these programs are
rather impressive; most are now well behaved in that they correctly follow the
Laws of Chess and handle users' data with reasonable care.  Unfortunately, many
programs have had serious problems with several aspects of the external
representation of chess game data.  Sometimes these problems become more
visible when a user attempts to move significant quantities of data from one
program to another; if there has been no real effort to ensure portability of
data, then the chances for a successful transfer are small at best.


2.1: Data interchange incompatibility

The reasons for format incompatibility are easy to understand.  In fact, most
of them are correlated with the same problems that have already been seen with
commercial software offerings for other domains such as word processing,
spreadsheets, fonts, and graphics.  Sometimes a manufacturer deliberately
designs a data format using encryption or some other secret, proprietary
technique to "lock in" a customer.  Sometimes a designer may produce a format
that can be deciphered without too much difficulty, but at the same time
publicly discourage third party software by claiming trade secret protection.
Another software producer may develop a non-proprietary system, but it may work
well only within the scope of a single program or application because it is not
easily expandable.  Finally, some other software may work very well for many
purposes, but it uses symbols and language not easily understood by people or
computers available to those outside the country of its development.


2.2: Specification goals

A specification for a portable game notation must observe the lessons of
history and be able to handle probable needs of the future.  The design
criteria for PGN were selected to meet these needs.  These criteria include:

1) The details of the system must be publicly available and free of unnecessary
complexity.  Ideally, if the documentation is not available for some reason,
typical chess software developers and users should be able to understand most
of the data without the need for third party assistance.

2) The details of the system must be non-proprietary so that users and software
developers are unrestricted by concerns about infringing on intellectual
property rights.  The idea is to let chess programmers compete in a free market
where customers may choose software based on their real needs and not based on
artificial requirements created by a secret data format.

3) The system must work for a variety of programs.  The format should be such
that it can be used by chess database programs, chess publishing programs,
chess server programs, and chessplaying programs without being unnecessarily
specific to any particular application class.

4) The system must be easily expandable and scalable.  The expansion ability
must include handling data items that may not exist currently but could be
expected to emerge in the future.  (Examples: new opening classifications and
new country names.)  The system should be scalable in that it must not have any
arbitrary restrictions concerning the quantity of stored data.  Also, planned
modes of expansion should either preserve earlier databases or at least allow
for their automatic conversion.

5) The system must be international.  Chess software users are found in many
countries and the system should be free of difficulties caused by conventions
local to a given region.

6) Finally, the system should handle the same kinds and amounts of data that
are already handled by existing chess software and by print media.


2.3: A sample PGN game

Although its description may seem rather lengthy, PGN is actually fairly
simple.  A sample PGN game follows; it has most of the important features
described in later sections of this document.

[Event "F/S Return Match"]
[Site "Belgrade, Serbia JUG"]
[Date "1992.11.04"]
[Round "29"]
[White "Fischer, Robert J."]
[Black "Spassky, Boris V."]
[Result "1/2-1/2"]

1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 4. Ba4 Nf6 5. O-O Be7 6. Re1 b5 7. Bb3 d6 8. c3
O-O 9. h3 Nb8 10. d4 Nbd7 11. c4 c6 12. cxb5 axb5 13. Nc3 Bb7 14. Bg5 b4 15.
Nb1 h6 16. Bh4 c5 17. dxe5 Nxe4 18. Bxe7 Qxe7 19. exd6 Qf6 20. Nbd2 Nxd6 21.
Nc4 Nxc4 22. Bxc4 Nb6 23. Ne5 Rae8 24. Bxf7+ Rxf7 25. Nxf7 Rxe1+ 26. Qxe1 Kxf7
27. Qe3 Qg5 28. Qxg5 hxg5 29. b3 Ke6 30. a3 Kd6 31. axb4 cxb4 32. Ra5 Nd5 33.
f3 Bc8 34. Kf2 Bf5 35. Ra7 g6 36. Ra6+ Kc5 37. Ke1 Nf4 38. g3 Nxh3 39. Kd2 Kb5
40. Rd6 Kc5 41. Ra6 Nf2 42. g4 Bd3 43. Re6 1/2-1/2


3: Formats: import and export

There are two formats in the PGN specification.  These are the "import" format
and the "export" format.  These are the two different ways of formatting the
same PGN data according to its source.  The details of the two formats are
described throughout the following sections of this document.

Other than formats, there is the additional topic of PGN presentation.  While
both PGN import and export formats are designed to be readable by humans, there
is no recommendation that either of these be an ultimate mode of chess data
presentation.  Rather, software developers are urged to consider all of the
various techniques at their disposal to enhance the display of chess data at
the presentation level (i.e., highest level) of their programs.  This means
that the use of different fonts, character sizes, color, and other tools of
computer aided interaction and publishing should be explored to provide a high
quality presentation appropriate to the function of the particular program.


3.1: Import format allows for manually prepared data

The import format is rather flexible and is used to describe data that may have
been prepared by hand, much like a source file for a high level programming
language.  A program that can read PGN data should be able to handle the
somewhat lax import format.


3.2: Export format used for program generated output

The export format is rather strict and is used to describe data that is usually
prepared under program control, something like a pretty printed source program
reformatted by a compiler.


3.2.1: Byte equivalence

For a given PGN data file, export format representations generated by different
PGN programs on the same computing system should be exactly equivalent, byte
for byte.


3.2.2: Archival storage and the newline character

Export format should also be used for archival storage.  Here, "archival"
storage is defined as storage that may be accessed by a variety of computing
systems.  The only extra requirement for archival storage is that the newline
character have a specific representation that is independent of its value for a
particular computing system's text file usage.  The archival representation of
a newline is the ASCII control character LF (line feed, decimal value 10,
hexadecimal value 0x0a).

Sadly, there are some accidents of history that survive to this day that have
baroque representations for a newline: multicharacter sequences, end-of-line
record markers, start-of-line byte counts, fixed length records, and so forth.
It is well beyond the scope of the PGN project to reconcile all of these to the
unified world of ANSI C and the those enjoying the bliss of a single '\n'
convention.  Some systems may just not be able to handle an archival PGN text
file with native text editors.  In these cases, an indulgence of sorts is
granted to use the local newline convention in non-archival PGN files for those
text editors.


3.2.3: Speed of processing

Several parts of the export format deal with exact descriptions of line and
field justification that are absent from the import format details.  The main
reason for these restrictions on the export format are to allow the
construction of simple data translation programs that can easily scan PGN data
without having to have a full chess engine or other complex parsing routines.
The idea is to encourage chess software authors to always allow for at least a
limited PGN reading capability.  Even when a full chess engine parsing
capability is available, it is likely to be at least two orders of magnitude
slower than a simple text scanner.


3.2.4: Reduced export format

A PGN game represented using export format is said to be in "reduced export
format" if all of the following hold: 1) it has no commentary, 2) it has only
the standard seven tag roster identification information ("STR", see below), 3)
it has no recursive annotation variations ("RAV", see below), and 4) it has no
numeric annotation glyphs ("NAG", see below).  Reduced export format is used
for bulk storage of unannotated games.  It represents a minimum level of
standard conformance for a PGN exporting application.


4: Lexicographical issues

PGN data is composed of characters; non-overlapping contiguous sequences of
characters form lexical tokens.


4.1: Character codes

PGN data is represented using a subset of the eight bit ISO 8859/1 (Latin 1)
character set.  ("ISO" is an acronym for the International Standards
Organization.)  This set is also known as ECMA-94 and is similar to other ISO
Latin character sets.  ISO 8859/1 includes the standard seven bit ASCII
character set for the 32 control character code values from zero to 31.  The 95
printing character code values from 32 to 126 are also equivalent to seven bit
ASCII usage.  (Code value 127, the ASCII DEL control character, is a graphic
character in ISO 8859/1; it is not used for PGN data representation.)

The 32 ISO 8859/1 code values from 128 to 159 are non-printing control
characters.  They are not used for PGN data representation.  The 32 code values
from 160 to 191 are mostly non-alphabetic printing characters and their use for
PGN data is discouraged as their graphic representation varies considerably
among other ISO Latin sets.  Finally, the 64 code values from 192 to 255 are
mostly alphabetic printing characters with various diacritical marks; their use
is encouraged for those languages that require such characters.  The graphic
representations of this last set of 64 characters is fairly constant for the
ISO Latin family.

Printing character codes outside of the seven bit ASCII range may only appear
in string data and in commentary.  They are not permitted for use in symbol
construction.

Because some PGN users' environments may not support presentation of non-ASCII
characters, PGN game authors should refrain from using such characters in
critical commentary or string values in game data that may be referenced in
such environments.  PGN software authors should have their programs handle such
environments by displaying a question mark ("?") for non-ASCII character codes.
This is an important point because there are many computing systems that can
display eight bit character data, but the display graphics may differ among
machines and operating systems from different manufacturers.

Only four of the ASCII control characters are permitted in PGN import format;
these are the horizontal and vertical tabs along with the linefeed and carriage
return codes.

The external representation of the newline character may differ among
platforms; this is an acceptable variation as long as the details of the
implementation are hidden from software implementors and users.  When a choice
is practical, the Unix "newline is linefeed" convention is preferred.


4.2: Tab characters

Tab characters, both horizontal and vertical, are not permitted in the export
format.  This is because the treatment of tab characters is highly dependent
upon the particular software in use on the host computing system.  Also, tab
characters may not appear inside of string data.


4.3: Line lengths

PGN data are organized as simple text lines without any special bytes or
markers for secondary record structure imposed by specific operating systems.
Import format PGN text lines are limited to having a maximum of 255 characters
per line including the newline character.  Lines with 80 or more printing
characters are strongly discouraged because of the difficulties experienced by
common text editors with long lines.

In some cases, very long tag values will require 80 or more columns, but these
are relatively rare.  An example of this is the "FEN" tag pair; it may have a
long tag value, but this particular tag pair is only used to represent a game
that doesn't start from the usual initial position.


5: Commentary

Comment text may appear in PGN data.  There are two kinds of comments.  The
first kind is the "rest of line" comment; this comment type starts with a
semicolon character and continues to the end of the line.  The second kind
starts with a left brace character and continues to the next right brace
character.  Comments cannot appear inside any token.

Brace comments do not nest; a left brace character appearing in a brace comment
loses its special meaning and is ignored.  A semicolon appearing inside of a
brace comment loses its special meaning and is ignored.  Braces appearing
inside of a semicolon comments lose their special meaning and are ignored.

*** Export format representation of comments needs definition work.


6: Escape mechanism

There is a special escape mechanism for PGN data.  This mechanism is triggered
by a percent sign character ("%") appearing in the first column of a line; the
data on the rest of the line is ignored by publicly available PGN scanning
software.  This escape convention is intended for the private use of software
developers and researchers to embed non-PGN commands and data in PGN streams.

A percent sign appearing in any other place other than the first position in a
line does not trigger the escape mechanism.


7: Tokens

PGN character data is organized as tokens.  A token is a contiguous sequence of
characters that represents a basic semantic unit.  Tokens may be separated from
adjacent tokens by white space characters.  (White space characters include
space, newline, and tab characters.)  Some tokens are self delimiting and do
not require white space characters.

A string token is a sequence of zero or more printing characters delimited by a
pair of quote characters (ASCII decimal value 34, hexadecimal value 0x22).  An
empty string is represented by two adjacent quotes.  (Note: an apostrophe is
not a quote.)  A quote inside a string is represented by the backslash
immediately followed by a quote.  A backslash inside a string is represented by
two adjacent backslashes.  Strings are commonly used as tag pair values (see
below).  Non-printing characters like newline and tab are not permitted inside
of strings.  A string token is terminated by its closing quote.  Currently, a
string is limited to a maximum of 255 characters of data.

An integer token is a sequence of one or more decimal digit characters.  It is
a special case of the more general "symbol" token class described below.
Integer tokens are used to help represent move number indications (see below).
An integer token is terminated just prior to the first non-symbol character
following the integer digit sequence.

A period character (".") is a token by itself.  It is used for move number
indications (see below).  It is self terminating.

An asterisk character ("*") is a token by itself.  It is used as one of the
possible game termination markers (see below); it indicates an incomplete game
or a game with an unknown or otherwise unavailable result.  It is self
terminating.

The left and right bracket characters ("[" and "]") are tokens.  They are used
to delimit tag pairs (see below).  Both are self terminating.

The left and right parenthesis characters ("(" and ")") are tokens.  They are
used to delimit Recursive Annotation Variations (see below).  Both are self
terminating.

The left and right angle bracket characters ("<" and ">") are tokens.  They are
reserved for future expansion.  Both are self terminating.

A Numeric Annotation Glyph ("NAG", see below) is a token; it is composed of a
dollar sign character ("$") immediately followed by one or more digit
characters.  It is terminated just prior to the first non-digit character
following the digit sequence.

A symbol token starts with a letter or digit character and is immediately
followed by a sequence of zero or more symbol continuation characters.  These
continuation characters are letter characters ("A-Za-z"), digit characters
("0-9"), the underscore ("_"), the plus sign ("+"), the octothorpe sign ("#"),
the equal sign ("="), the colon (":"),  and the hyphen ("-").  Symbols are used
for a variety of purposes.  All characters in a symbol are significant.  A
symbol token is terminated just prior to the first non-symbol character
following the symbol character sequence.  Currently, a symbol is limited to a
maximum of 255 characters in length.


8: Parsing games

A PGN database file is a sequential collection of zero or more PGN games.  An
empty file is a valid, although somewhat uninformative, PGN database.

A PGN game is composed of two sections.  The first is the tag pair section and
the second is the movetext section.  The tag pair section provides information
that identifies the game by defining the values associated with a set of
standard parameters.  The movetext section gives the usually enumerated and
possibly annotated moves of the game along with the concluding game termination
marker.  The chess moves themselves are represented using SAN (Standard
Algebraic Notation), also described later in this document.


8.1: Tag pair section

The tag pair section is composed of a series of zero or more tag pairs.

A tag pair is composed of four consecutive tokens: a left bracket token, a
symbol token, a string token, and a right bracket token.  The symbol token is
the tag name and the string token is the tag value associated with the tag
name.  (There is a standard set of tag names and semantics described below.)
The same tag name should not appear more than once in a tag pair section.

A further restriction on tag names is that they are composed exclusively of
letters, digits, and the underscore character.  This is done to facilitate
mapping of tag names into key and attribute names for use with general purpose
database programs.

For PGN import format, there may be zero or more white space characters between
any adjacent pair of tokens in a tag pair.

For PGN export format, there are no white space characters between the left
bracket and the tag name, there are no white space characters between the tag
value and the right bracket, and there is a single space character between the
tag name and the tag value.

Tag names, like all symbols, are case sensitive.  All tag names used for
archival storage begin with an upper case letter.

PGN import format may have multiple tag pairs on the same line and may even
have a tag pair spanning more than a single line.  Export format requires each
tag pair to appear left justified on a line by itself; a single empty line
follows the last tag pair.

Some tag values may be composed of a sequence of items.  For example, a
consultation game may have more than one player for a given side.  When this
occurs, the single character ":" (colon) appears between adjacent items.
Because of this use as an internal separator in strings, the colon should not
otherwise appear in a string.

The tag pair format is designed for expansion; initially only strings are
allowed as tag pair values.  Tag value formats associated with the STR (Seven
Tag Roster, see below) will not change; they will always be string values.
However, there are long term plans to allow general list structures as tag
values for non-STR tag pairs.  Use of these expanded tag values will likely be
restricted to special research programs.  In all events, the top level
structure of a tag pair remains the same: left bracket, tag name, tag value,
and right bracket.


8.1.1: Seven Tag Roster

There is a set of tags defined for mandatory use for archival storage of PGN
data.  This is the STR (Seven Tag Roster).  The interpretation of these tags is
fixed as is the order in which they appear.  Although the definition and use of
additional tag names and semantics is permitted and encouraged when needed, the
STR is the common ground that all programs should follow for public data
interchange.

For import format, the order of tag pairs is not important.  For export format,
the STR tag pairs appear before any other tag pairs.  (The STR tag pairs must
also appear in order; this order is described below).  Also for export format,
any additional tag pairs appear in ASCII order by tag name.

The seven tag names of the STR are (in order):

1) Event (the name of the tournament or match event)

2) Site (the location of the event)

3) Date (the starting date of the game)

4) Round (the playing round ordinal of the game)

5) White (the player of the white pieces)

6) Black (the player of the black pieces)

7) Result (the result of the game)

A set of supplemental tag names is given later in this document.

For PGN export format, a single blank line appears after the last of the tag
pairs to conclude the tag pair section.  This helps simple scanning programs to
quickly determine the end of the tag pair section and the beginning of the
movetext section.


8.1.1.1: The Event tag

The Event tag value should be reasonably descriptive.  Abbreviations are to be
avoided unless absolutely necessary.  A consistent event naming should be used
to help facilitate database scanning.  If the name of the event is unknown, a
single question mark should appear as the tag value.

Examples:

[Event "FIDE World Championship"]

[Event "Moscow City Championship"]

[Event "ACM North American Computer Championship"]

[Event "Casual Game"]


8.1.1.2: The Site tag

The Site tag value should include city and region names along with a standard
name for the country.  The use of the IOC (International Olympic Committee)
three letter names is suggested for those countries where such codes are
available.  If the site of the event is unknown, a single question mark should
appear as the tag value.  A comma may be used to separate a city from a region.
No comma is needed to separate a city or region from the IOC country code.  A
later section of this document gives a list of three letter nation codes along
with a few additions for "locations" not covered by the IOC.

Examples:

[Site "New York City, NY USA"]

[Site "St. Petersburg RUS"]

[Site "Riga LAT"]


8.1.1.3: The Date tag

The Date tag value gives the starting date for the game.  (Note: this is not
necessarily the same as the starting date for the event.)  The date is given
with respect to the local time of the site given in the Event tag.  The Date
tag value field always uses a standard ten character format: "YYYY.MM.DD".  The
first four characters are digits that give the year, the next character is a
period, the next two characters are digits that give the month, the next
character is a period, and the final two characters are digits that give the
day of the month.  If the any of the digit fields are not known, then question
marks are used in place of the digits.

Examples:

[Date "1992.08.31"]

[Date "1993.??.??"]

[Date "2001.01.01"]


8.1.1.4: The Round tag

The Round tag value gives the playing round for the game.  In a match
competition, this value is the number of the game played.  If the use of a
round number is inappropriate, then the field should be a single hyphen
character.  If the round is unknown, a single question mark should appear as
the tag value.

Some organizers employ unusual round designations and have multipart playing
rounds and sometimes even have conditional rounds.  In these cases, a multipart
round identifier can be made from a sequence of integer round numbers separated
by periods.  The leftmost integer represents the most significant round and
succeeding integers represent round numbers in descending hierarchical order.

Examples:

[Round "1"]

[Round "3.1"]

[Round "4.1.2"]


8.1.1.5: The White tag

The White tag value is the name of the player or players of the white pieces.
The names are given as they would appear in a telephone directory.  The family
or last name appears first.  If a first name or first initial is available, it
is separated from the family name by a comma and a space.  Finally, one or more
middle initials may appear.  (Wherever a comma appears, the very next character
should be a space.  Wherever an initial appears, the very next character should
be a period.)  If the name is unknown, a single question mark should appear as
the tag value.

The intent is to allow meaningful ASCII sorting of the tag value that is
independent of regional name formation customs.  If more than one person is
playing the white pieces, the names are listed in alphabetical order and are
separated by the colon character between adjacent entries.  A player who is
also a computer program should have appropriate version information listed
after the name of the program.

The format used in the FIDE Rating Lists is appropriate for use for player name
tags.

Examples:

[White "Tal, Mikhail N."]

[White "van der Wiel, Johan"]

[White "Acme Pawngrabber v.3.2"]

[White "Fine, R."]


8.1.1.6: The Black tag

The Black tag value is the name of the player or players of the black pieces.
The names are given here as they are for the White tag value.

Examples:

[Black "Lasker, Emmanuel"]

[Black "Smyslov, Vasily V."]

[Black "Smith, John Q.:Woodpusher 2000"]

[Black "Morphy"]


8.1.1.7: The Result tag

The Result field value is the result of the game.  It is always exactly the
same as the game termination marker that concludes the associated movetext.  It
is always one of four possible values: "1-0" (White wins), "0-1" (Black wins),
"1/2-1/2" (drawn game), and "*" (game still in progress, game abandoned, or
result otherwise unknown).  Note that the digit zero is used in both of the
first two cases; not the letter "O".

All possible examples:

[Result "0-1"]

[Result "1-0"]

[Result "1/2-1/2"]

[Result "*"]


8.2: Movetext section

The movetext section is composed of chess moves, move number indications,
optional annotations, and a single concluding game termination marker.

Because illegal moves are not real chess moves, they are not permitted in PGN
movetext.  They may appear in commentary, however.  One would hope that illegal
moves are relatively rare in games worthy of recording.


8.2.1: Movetext line justification

In PGN import format, tokens in the movetext do not require any specific line
justification.

In PGN export format, tokens in the movetext are placed left justified on
successive text lines each of which has less than 80 printing characters.  As
many tokens as possible are placed on a line with the remainder appearing on
successive lines.  A single space character appears between any two adjacent
symbol tokens on the same line in the movetext.  As with the tag pair section,
a single empty line follows the last line of data to conclude the movetext
section.

Neither the first or the last character on an export format PGN line is a
space.  (This may change in the case of commentary; this area is currently
under development.)


8.2.2: Movetext move number indications

A move number indication is composed of one or more adjacent digits (an integer
token) followed by zero or more periods.  The integer portion of the indication
gives the move number of the immediately following white move (if present) and
also the immediately following black move (if present).


8.2.2.1: Import format move number indications

PGN import format does not require move number indications.  It does not
prohibit superfluous move number indications anywhere in the movetext as long
as the move numbers are correct.

PGN import format move number indications may have zero or more period
characters following the digit sequence that gives the move number; one or more
white space characters may appear between the digit sequence and the period(s).


8.2.2.2: Export format move number indications

There are two export format move number indication formats, one for use
appearing immediately before a white move element and one for use appearing
immediately before a black move element.  A white move number indication is
formed from the integer giving the fullmove number with a single period
character appended.  A black move number indication is formed from the integer
giving the fullmove number with three period characters appended.

All white move elements have a preceding move number indication.  A black move
element has a preceding move number indication only in two cases: first, if
there is intervening annotation or commentary between the black move and the
previous white move; and second, if there is no previous white move in the
special case where a game starts from a position where Black is the active
player.

There are no other cases where move number indications appear in PGN export
format.


8.2.3: Movetext SAN (Standard Algebraic Notation)

SAN (Standard Algebraic Notation) is a representation standard for chess moves
using the ASCII Latin alphabet.

Examples of SAN recorded games are found throughout most modern chess
publications.  SAN as presented in this document uses English language single
character abbreviations for chess pieces, although this is easily changed in
the source.  English is chosen over other languages because it appears to be
the most widely recognized.

An alternative to SAN is FAN (Figurine Algebraic Notation).  FAN uses miniature
piece icons instead of single letter piece abbreviations.  The two notations
are otherwise identical.


8.2.3.1: Square identification

SAN identifies each of the sixty four squares on the chessboard with a unique
two character name.  The first character of a square identifier is the file of
the square; a file is a column of eight squares designated by a single lower
case letter from "a" (leftmost or queenside) up to and including "h" (rightmost
or kingside).  The second character of a square identifier is the rank of the
square; a rank is a row of eight squares designated by a single digit from "1"
(bottom side [White's first rank]) up to and including "8" (top side [Black's
first rank]).  The initial squares of some pieces are: white queen rook at a1,
white king at e1, black queen knight pawn at b7, and black king rook at h8.


8.2.3.2: Piece identification

SAN identifies each piece by a single upper case letter.  The standard English
values: pawn = "P", knight = "N", bishop = "B", rook = "R", queen = "Q", and
king = "K".

The letter code for a pawn is not used for SAN moves in PGN export format
movetext.  However, some PGN import software disambiguation code may allow for
the appearance of pawn letter codes.  Also, pawn and other piece letter codes
are needed for use in some tag pair and annotation constructs.

It is admittedly a bit chauvinistic to select English piece letters over those
from other languages.  There is a slight justification in that English is a de
facto universal second language among most chessplayers and program users.  It
is probably the best that can be done for now.  A later section of this
document gives alternative piece letters, but these should be used only for
local presentation software and not for archival storage or for dynamic
interchange among programs.


8.2.3.3: Basic SAN move construction

A basic SAN move is given by listing the moving piece letter (omitted for
pawns) followed by the destination square.  Capture moves are denoted by the
lower case letter "x" immediately prior to the destination square; pawn
captures include the file letter of the originating square of the capturing
pawn immediately prior to the "x" character.

SAN kingside castling is indicated by the sequence "O-O"; queenside castling is
indicated by the sequence "O-O-O".  Note that the upper case letter "O" is
used, not the digit zero.  The use of a zero character is not only incompatible
with traditional text practices, but it can also confuse parsing algorithms
which also have to understand about move numbers and game termination markers.
Also note that the use of the letter "O" is consistent with the practice of
having all chess move symbols start with a letter; also, it follows the
convention that all non-pwn move symbols start with an upper case letter.

En passant captures do not have any special notation; they are formed as if the
captured pawn were on the capturing pawn's destination square.  Pawn promotions
are denoted by the equal sign "=" immediately following the destination square
with a promoted piece letter (indicating one of knight, bishop, rook, or queen)
immediately following the equal sign.  As above, the piece letter is in upper
case.


8.2.3.4: Disambiguation

In the case of ambiguities (multiple pieces of the same type moving to the same
square), the first appropriate disambiguating step of the three following steps
is taken:

First, if the moving pieces can be distinguished by their originating files,
the originating file letter of the moving piece is inserted immediately after
the moving piece letter.

Second (when the first step fails), if the moving pieces can be distinguished
by their originating ranks, the originating rank digit of the moving piece is
inserted immediately after the moving piece letter.

Third (when both the first and the second steps fail), the two character square
coordinate of the originating square of the moving piece is inserted
immediately after the moving piece letter.

Note that the above disambiguation is needed only to distinguish among moves of
the same piece type to the same square; it is not used to distinguish among
attacks of the same piece type to the same square.  An example of this would be
a position with two white knights, one on square c3 and one on square g1 and a
vacant square e2 with White to move.  Both knights attack square e2, and if
both could legally move there, then a file disambiguation is needed; the
(nonchecking) knight moves would be "Nce2" and "Nge2".  However, if the white
king were at square e1 and a black bishop were at square b4 with a vacant
square d2 (thus an absolute pin of the white knight at square c3), then only
one white knight (the one at square g1) could move to square e2: "Ne2".


8.2.3.5: Check and checkmate indication characters

If the move is a checking move, the plus sign "+" is appended as a suffix to
the basic SAN move notation; if the move is a checkmating move, the octothorpe
sign "#" is appended instead.

Neither the appearance nor the absence of either a check or checkmating
indicator is used for disambiguation purposes.  This means that if two (or
more) pieces of the same type can move to the same square the differences in
checking status of the moves does not allieviate the need for the standard rank
and file disabiguation described above.  (Note that a difference in checking
status for the above may occur only in the case of a discovered check.)

Neither the checking or checkmating indicators are considered annotation as
they do not communicate subjective information.  Therefore, they are
qualitatively different from move suffix annotations like "!" and "?".
Subjective move annotations are handled using Numeric Annota