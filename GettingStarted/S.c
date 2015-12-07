#include "header.h"

WTitle(Strings and S)

Strings can be created by string literals as in S&quot;_&quot;
or by multiline string literals as in
OCode
S"
'foo
'bar
"
CCode
WP
Some special caracter can be obtained by factory methods, like 
Wcode(S.doubleQuote()) or Wcode(S.newLine())
There is no escaping in strings, so WcodeQuote(S&quot;\&quot;) is a string with the single character Wcode(\)
WP
Many objects can be turned into strings by calling the toS() method, for example
WcodeQuote(Point(x=0Int,y=0Int).toS()==S&quot;Point(x=0,y=0)&quot;)
Objects implementing the toS() method can be subtype of the Concept.ToS interface.
WP
Strings can be compared by Wcode(==), Wcode(>),Wcode(<), Wcode(>=), Wcode(<=) and Wcode(!=).
WP
Note that you can manually make your own class supporting Wcode(==) by writing a method with the special name
Wcode(operator==(that)), or automatically simply using Data.
WP
Strings are seen as senquences, so you can ask the 
Wcode(.size()), Wcode(.isEmpty()) and you can
extract the single characters using 
WP
WcodeQuote(S&quot;abcd&quot;(2UInt)==S&quot;c&quot;), 
WP or ask for substrings
WP
WcodeQuote(S&quot;abcd&quot;(from: 0UInt to: 2UInt)==S&quot;abc&quot;)
WP
WcodeQuote(S&quot;abcd&quot;(to: 2UInt)==S&quot;abc&quot;)
WP
WcodeQuote(S&quot;abcd&quot;(from: 2UInt)==S&quot;d&quot;)
WP
or (functionally) update a substring
WcodeQuote(S&quot;abcd&quot;.with(1UInt,val=S&quot;WW&quot;)==S&quot;aWWcd&quot;)
WP
WcodeQuote(S&quot;abcd&quot;.with(from:1UInt to:2UInt,val=S&quot;WW&quot;)==S&quot;aWWd&quot;)
WP
You can also concatenate strings with Wcode(++).
WP
However to concatenate strings and other datatypes you would have to convert them in strings manually, as in
WcodeQuote(S&quot;Hello &quot;++42Int.toS()++&quot;!!&quot;)
This can become cumbersome if you need to concatenate more then a couple of things.
To simplify this, there is the string formatting syntax:
WP
WcodeQuote(S&quot;Hello &quot;[42Int]&quot;!!&quot;)
WP
This allows multiple values, and need to be always terminated by a quotes:
WP
WcodeQuote(S&quot;Hello &quot;[42Int]&quot; and &quot;[8Int]&quot;&quot;)
WP
It allows formatting
WP
WcodeQuote(S&quot;Hello &quot;[d_2=42Int]&quot;!!&quot;==S&quot;Hello 42.00&quot;)
WP
Lets now understand how this syntax works in 42:
S defines a class method 
WcodeQuote(S operator&quot;&quot;(that))
S defines a method 
Wcode(S.Formatter operator[(that))
Wcode(//is that a good name? what about operator#begin ?
)

Wcode(S.Formatter)
 defines a
Wcode(method S.Formatter operator;)
 adding ToS object to the string under construction
Wcode(// what about operator#add
)
Wcode(S.Formatter)
defines a
Wcode(method operator](that) //what about operator#end
)
Wcode(S.Formatter)
 defines a

WcodeQuote(metod S operator&quot;&quot;(that)), allowing to add more string characters after the closed ]

This syntax also allows variations, like
WcodeQuote(S&quot;Hello &quot;[42Int; 12Int]&quot;&quot;)
or
WcodeQuote(myString[]&quot;more text here&quot;)
This is also the suggested way to write a single line string on multiple lines of code. Instead of concatenating many strings with Wcode(++), just do
WcodeQuote(S&quot;foo&quot;
[]&quot;bar&quot;
[]&quot;beer&quot;
)
There are more methods in strings, like Wcode(.contains(that)) and 
Wcode(.firstIndexOf(that))



#include "footer.h"