#include "header.h"

<p>
In 42, to write strings and numbers literals
you can use the string or number construction operators,
as shown in the example before.
In most cases such operator is offered as a type method, so you just need to specify
the class name. That is, you can write Wcode(12N), Wcode(12Meters), 
Wcode(S"hello")
or Wcode(Url"L42.is") 
if N, Meters, S and Url support the number and string constructor operator.

We say that a class is a "numeric" class if it supports the number construction and some of the conventional Wlink(missing,algebric operators).

We say that a class is a "string" class if it support the string constructor and Wlink(missing,sequence operators)

In most languages you are stuck with a finite set of number types and (usually) a single
string type, predefined in the language or in the standard library.
For example "Foo" is an instance of the predefined string type and
12d is an instance of the predefined double number kind,
and 12 is either an instance of int or any predefined number kind based
on some obscure disambiguation rule.
One of the design principle of 42 is to be clear and flexible
even at the cost of a little verbosity.
Thus, 12 or "foo" on their own are not part of the syntax of 42.

</p>
	 
	 <H3> Strings and numbers</H3>
	1 support all the java primitive types plus big int and big rational
  
	2	 Unit of measures: Design decorators to generate unit of
     measure out of representation
	   set of rules multiset of component->component
	   and component -inverse-> component
	   interpreted to generate Wcode(KgMeter[......dividedBy:... ])
	3 there should be a decorator Wcode(String[S;escape:S"something" as:S"somethingElse"]<<{ method invariant(){....}})
	  

Desugaring for numbers and strings is as following
Wcode(S"ab")=
Wcode(S.parseString(S.parser().parse_a().parse_b()))
Wcode(12Int)=
WCode(Int.parseNumber(Int.parser().parse_1().parse_2()))
WCode(Int.parser()) and
WCode(S.parser()) may return an instance of a Wcode(Parser) class,
potentially offering up to 100 methods.
  
    
      You can launch your application from command line typing 
Wcode(L42 fileName.L42)
or
Wcode(L42 folderName)
where 
Wcode(folderName)
must contain a file called
Wcode(Outer0.L42).



#include "footer.h"	 