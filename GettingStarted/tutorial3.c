WBigTitle(Units of Measure)

WTitle((1/5) Num and Size)
Wcode(Num) is a general number type,
implemented as an arbitrary precision rational.
When in doubt of what numeric type to use, Wcode(Num)
is a good first guess.
Some examples of usage:
OCode
little=123Num
stillLittle=4567890Num
big=100000000000000000Num
bigger=100000000000000000.0001Num
fraction=Num"123/4567890"
hold= fraction == little/stillLittle
Debug(fraction)
Debug(Num"12/4") //will print "3"
CCode

Another useful numeric type is Wcode(Size).
It represents sizes and indexes in sequences.
Wcode(Size)s are returned by Wcode(size()) methods
and are expected as parameter by indexing methods.
Wcode(Size) represent 32 bit numbers with the usual 
but triky modulo arithmetic.

WTitle(Loading other numeric types)

You can import other numerc types by loading libraries.
For example

OCode
Int:Load<<{reuse L42.is/Numbers/Int}//not supported yet
//infinite precision positive and negative integer numbers
Double:Load<<{reuse L42.is/Numbers/Double}//not supported yet
//double precision positive and negative floating points numbers
Float:Load<<{reuse L42.is/Numbers/Float}//not supported yet
//single precision positive and negative floating points numbers
Int64:Load<<{reuse L42.is/Numbers/Int64}//not supported yet
//64 bit modulo arithmetic
UInt64:Load<<{reuse L42.is/Numbers/UInt64}//not supported yet
//64 bit modulo arithmetic, unsigned
CCode

The class decorator Wcode(Load) allows to load libraries and embed them in the 
current context, while the
reuse keyword imports the code from the web.

WTitle(Conversions)
Conversions between varius numeric classes must be performed explicitly.
All numeric classes implements 
the Wcode(Numeric) interface and offer the Wcode(.from(numeric)) method.
So, for example 
OCode
Double:Load<<{reuse L42.is/Numbers/Double}
size=S"hello".size()
myDouble=Double.from(numeric:size)
CCode
converts from Wcode(Size) to Wcode(Double).
This avoid precision loss as much as possible.

WTitle((2/5) Iteration)

It is possible to iterate over a range of numbers:

OCode
with i in 4Size.vals() (
  Debug(i)//prints 4,5,6,7 and so on forever
  )
CCode

OCode
with i in 4Size.upTo(42Size) (
  Debug(i)//prints 4,5,6,7 and so on up to 42 excluded
  )
CCode



WTitle((3/5) Units)


The class 
Wcode(Units)
offers methods to create units out of numeric supports, like Wcode(Num)
and Wcode(Size).

For example
OCode
Meter:Units.of(Num)
Second:Units.of(Num)
res=(6Meter +4Meter)*2Num //20Meter
//wrong1=6Meter+2Second
//wrong2=6Meter/2Second
CCode
As you can see, we can sum meters together, and we can use the support for multiplicaiton, but we can not mix different units of measure.
Wcode(Units) supports composite units:
OCode
Speed:Units.of(Meter per:Second)
fast1=Speed(42Meter per:0.1Second)
fast2=Speed"42/0.1"
distance1=fast1.per(60Second)

Acc:Units.of(Speed per:Second)
g=Acc"9.8"
speedAfter=g.per(10Second)//98 m/s
distance2=speedAfter.per(10Second)/2Num //490 m after 10s free fall

Kg:Units.of(Num)
Newton:Units.of(Kg and:Acc)//Kg*m/s2
myRoket=900Newton
gForceOnMe=Newton(78Kg and:g)//little less than 780
myLift=myRoket-gForceOnMe
if myLift>0Newton (Debug(S"I can fly"))
myAcc=myLift.div1(78Kg)//get second component
reachedHeight=myAcc.per(10Second).per(10Second)/2N //after 10 sec
CCode
Note how we can use Wcode(.per()),
 Wcode(.div1()) 
 and Wcode(.div2()) to extract the first or the second component
 in a composed unit.
WP
We can also declare aliasing units:

OCode
Cm:Units.alias(0.01Meter)//not supported yet
Meter height=178Cm
CCode

Note how height is of type Wcode(Meter).
Alias units are just shortcut to declare values of
the original unit.


WTitle((3/5) Alphanumeric)
In the same way Wcode(Units) allows easy creation of
arithmetic classes,
Wcode(Alphanumeric) allows easy creation of alphanumeric classes:
classes that can be istantiated from a string literal that follow certain 
properties.


OCode
Email:Alphanumeric<<{//not supported yet
  S local //fields
  S domain
  class method
  This parse(S that) {
    index=that.indexOf(S"@")//works only for simple emails
    if index==-1 (error this.parseError(S"@ not found"))
    local=that(end:index)//string slicing
    domain=that(start:index+1/ )//string slicing
    if domain.contains(S"@") (error this.parseError(S"multiple @ found"))
    return This(that,local:local,domain:domain)
    }//call the factory with fields plus the original string
}
myEmail=Email"arthur.dent@gmail.com"
Assert.$[// assertions check the truth of some statementes
  myEmail.local() expected:S"arthur.dent";
  myEmail.domain() expected:S"gmail.com";
  myEmail.toS() expected:S"arthur.dent@gmail.com";
  ]
CCode

Note how we can raise an error if the string does not have the shape we expected.
We will see errors/exception in more detail soon.
We can declare fields, and compute their values by parsing the string.
While it is suggested to propagate the original string in the factory,
it is not mandatory, for example you could apply some form of normalization, as shown under:

OCode
Email:Alphanumeric<<{/*..*/
  This parse(S that) {/*..google ignore dots anyway..*/
    local=that(end:index).replaceAll(S"." with:S"")
    /*..*/
    return This(local++S"@"+domain,local:local,domain:domain)
    } 
  }
myEmail=Email"arthur.dent@gmail.com"
Assert.$[myEmail.toS() expected:S"arthurdent@gmail.com"]
CCode



WBigTitle(Collections and Data classes)

A very large class of practically useful programs can be obtained by
just declaring simple minded classes using Data and Collections.

WTitle(Libraries)


WBigTitle(not ready to read yet Libraries)

WTitle(Libraries)
42 stands for the primate of libraries, so let see some libraries in action.
We have already see how to chose a towel, and many classes that are likely to be present in such towel, like
Wcode(S) and Wcode(Data).
Let see now how to load a library from its url:
OCode
{reuse L42.is/AdamTowel
Gui: Load<<{reuse L42.is/Gui}
Main:{
  Gui.alert(S"hi!")
  return ExitCode.success()
  }
}
CCode
Load is another decorator, here it modifies the library found in L42.is/Gui so that it can be used easly from AdamTowel.
 <!--
 
 Data invariants
 interface
  
 Sequences
 Strings
 Collections
 iterations 
 
 Exceptions
 Assertions
 
 Units and alphanumerics
 
 
 

 
 Use Extends Refactor
 
    //Meter dx=this.x()-that.x();
    //Meter dy=this.y()-that.y();
    //m2=Meter2[dx;dx]+Meter2[dy;dy]
    //return Meter[root:2Unit of:m2]
    //dx=dx.unit()
    //Meter.fromUnit(((dx*dx)+(dy*dy)).sqrt())
 -->	
OCode
{reuse L42.is/AdamTowel
Collections: Load<<{reuse L42.is/Collections}
Point:Data<<{Meter x, Meter y
  method 
  Meter distance(Point from){/*...*/}
  }
Points:Collections.vector(of:Point)
Main:{
  zero=Point(0Meter,0Meter)
  ps=Points[ Point(x=12Meter, y=20Meter);Point(x=1Meter, y=2Meter);]
  var Meter tot=0Meter
  with p in ps.vals() (
    tot+=p.distance(from:zero)
    )
  return ExitCode.success()
  }
}
CCode




WTitle(Collections)
Declaring and using collections is simple and easy in 42.
For example, we can declare and use a list of  Wcode(Point) by writing
OCode
Points:Collections.mutList(Point)

...

zero=Point(x=0Int, y=0Int)
one=Point(x=1Int, y=1Int)
ps0=Points[]//the empty list
ps1=Points[zero;one]//contains zero, one
ps2=Points[zero;one]//contains zero, one

CCode



<!--OCode Library myCode={ method Int foo()} CCode
A local binding of type Library initialized with a class with a single Wcode(foo()) method. Since there is no body, it is an abstract method.
 -->	 

 <!--WTitle(Simpler complete program)

Let now starts showing the simplest 42 program: an empty library.

OCode {} CCode

If we save this valid program in a file Wcode(Test.L42) and we run Wcode(L42 Test), we get an error.
WP
As you see 42 is very intuitive, as you would expect from your former life experiences, most simple things just does not work.
Note how valid programs can produce errors.
We will soon learn how to produce errors in controlled and elegant ways.
-->
<!--A 42 program execution WEmph(is) the generation of all its nested classes/interface.
, code is simply executed from top to bottom as in 
Python, Javascript or Php. However, the
top level expression is a Library, and code can go in libraries 
as an initializer for you need to put the code into an expression
-->

<!-- LATER?
Wcode(MyCode.hello(..)) use directly the Wcode(MyCode) class instance as receiver. We can also give it a name 
e se vuoi puoi anche salvarlo su un binding locale, tipo x=MyCode  x.hello(...)
-->
