WBigTitle(Basic classes)

An immutable class is WEmph(base/basic)
if is logically not composed by other elements,
and can be instantiated by a single operations that takes no parameters.
For examples numbers and strings are basics, while
a collections is not: you need to provide the elements and the collection is logically
composed by its elements.
In the examples of before, Wcode(Point) and 
Wcode(Animal) are not basic, since they are logically composed by their fields.


WTitle((1/5) Num and Size)
Wcode(Num) is a general number type,
implemented as an arbitrary precision rational.
When in doubt of what numeric type to use, Wcode(Num)
is a good first guess.
Some examples of usage: 
OBCode
little= 123Num
stillLittle= 4567890Num
big= 100000000000000000Num
bigger= 100000000000000000.0001Num
fraction1= Num"1234567/890"
fraction2 == little/stillLittle //holds
Debug(fraction1) //will print '1234567/890'
Debug(Num"12/4") //will print '3'
CCode

Another useful numeric type is Wcode(Size).
It corresponds to sizes and indexes in sequences.
Wcode(Size)s are returned by Wcode(size()) methods
and are expected as parameter by indexing methods.
Wcode(Size) represent 32 bit numbers with the usual 
but triky modulo arithmetic.

WTitle(Loading other numeric types)

You can import other numeric types by loading libraries.
For example

OBCode
Int: Load <>< {reuse L42.is/Numbers/Int} 
//infinite precision positive and negative integer numbers
Double: Load <>< {reuse L42.is/Numbers/Double} 
//double precision positive and negative floating points numbers
Float: Load <>< {reuse L42.is/Numbers/Float} 
//single precision positive and negative floating points numbers
Int64: Load <>< {reuse L42.is/Numbers/Int64} 
//64 bit modulo arithmetic
UInt64: Load <>< {reuse L42.is/Numbers/UInt64} 
//64 bit modulo arithmetic, unsigned
CCode

The class decorator Wcode(Load) allows to load libraries and embed them in the 
current context, while the
reuse keyword imports the code from the web.

WTitle(Conversions)
Conversions between various numeric classes must be performed explicitly.

AdamsTowel offers a simple way to convert between numeric classes, and more in general
between base classes.
All numeric  classes implements
the Wcode(Base) interface and offers the Wcode(.from(base)) method.
So, for example 
OBCode
Double: Load <>< {reuse L42.is/Numbers/Double}
size= S"hello".size()
myDouble= Double.from(base: size)
CCode
converts from Wcode(Size) to Wcode(Double).
This avoid precision loss as much as possible.




WTitle((2/5) Units)


The class 
Wcode(Units)
offers methods to create units out of numeric supports, like Wcode(Num)
and Wcode(Size).

For example
OBCode
Meter: Units.of(Num)
Second: Units.of(Num)
res= (6Meter +4Meter)*2Num //20Meter
//wrong1= 6Meter+2Second
//wrong2= 6Meter/2Second
CCode
As you can see, we can sum meters together, and we can use the support for multiplication, but we can not mix different units of measure.


Mathematically you can obtain the support out of the unit by
division: Wcode(42Meter/2Meter =  21Meter).
This do not work directly in 42, since multiplication and division
takes the support and not a unit.
Units provide method Wcode(divide(that)) for this aim.
Units also provide method  Wcode(`#'inner()),
this is just extracting the value of the support from the unit.
This can be convenient during programming but 
does not make a lot of sense mathematically.
Methods like that are required to be used with care, so they start with
Wcode(`#') to underline that.

OBCode
Num n1= 42Meter.divide(2Meter) //= 21Num
Num n2= 42Meter.#inner() //= 42Num
CCode



WTitle(Composite Units)

Wcode(Units) supports composite units: 
OBCode
Speed: Units.of(Meter per: Second)
fast1= Speed(42Meter per: 0.1Second)
fast2= Speed"420" //equivalent ways to initialize it
fast3= Speed"840/2"
distance1= fast1.right(left: 60Second)

Acc: Units.of(Speed per: Second)
g= Acc"9.8"
speedAfter= g.right(left: 10Second) //98 m/s
distance2= speedAfter.right(left: 10Second)/2Num //490 m after 10s free fall

Kg: Units.of(Num)
Newton: Units.of(Kg and: Acc) //Kg*m/s2
myRoket= 900Newton
gForceOnMe= Newton(78Kg and: g) //little less than 780
myLift= myRoket-gForceOnMe
if myLift>0Newton (Debug(S"I can fly"))
myAcc= myLift.right(left: 78Kg) //get second component
reachedHeight= myAcc.right(left: 10Second).right(left: 10Second)/2Num //after 10 sec
CCode
Note how in a cmposite unit we can use Wcode(right(left)) and
 Wcode(left(right)) 
 to extract the right component providing a value 
for the left one, or we can extract the left component providing a value for the right one.
 
WP
We can also declare aliasing units: 

OBCode
Cm: Units.alias(0.01Meter) 
Meter height= 178Cm
CCode

Note how height is of type Wcode(Meter).
Alias units are just shortcut to declare values of
the original unit.




WTitle((3/5) Alphanumeric)
In the same way Wcode(Units) allows easy creation of
arithmetic classes,
Wcode(Alphanumeric) allows easy creation of alphanumeric classes: 
classes that can be instantiated from a string literal that follow certain 
properties.


OBCode
Email: Alphanumeric <>< { 
  S local //fields
  S domain

  class method
  This parse(S that) {
    index= that.indexOf(S"@") //works only for simple emails
    if index.isPresent() (error Alphanumeric.ParseError"@ not found")
    local= that(end: index.get()) //string slicing
    domain= that(start: index.get()+1Size ) //string slicing
    if domain.contains(S"@") (error this.parseError(S"multiple @ found"))
    return This(that,local: local,domain: domain)
    } //call the factory with fields plus the original string
}
/*..*/
myEmail= Email"arthur.dent@gmail.com"
myEmail.local() ==S"arthur.dent" //holds
myEmail.domain() ==S"gmail.com" //holds
myEmail.toS() =="arthur.dent@gmail.com" //holds
CCode

Note how we can raise an error if the string does not have the shape we expected.
We will see errors/exception in more detail soon.
We can declare fields, and compute their values by parsing the string.
While it is suggested to propagate the original string in the factory,
it is not mandatory, for example you could apply some form of normalization, as shown under: 

OBCode
Email: Alphanumeric <>< {/*..*/
  This parse(S that) { //google ignore dots anyway
    /*..*/
    local= that(end: index).replaceAll(S"." with: S"")
    /*..*/
    return This(local++S"@"++domain,local: local,domain: domain)
    } 
  }
/*..*/
myEmail= Email"arthur.dent@gmail.com"
myEmail.toS() ==S"arthurdent@gmail.com" //holds
CCode


WTitle((4/5) Enumerations)

Enumerations can be obtained with the Wcode(Enumeration) class, as in the following code.

OBCode
Direction: Enumeration"north, east, south, west"
/*..*/
n= Direction.north()
s= Direction.south()
Direction.names()==Strings[S"north,S"east",S"south",S"west"]

if n.isNorth() (/*..*/)
else if n.isEast() (/*..*/)

Debug(n) //prints north

e= Direction.from(base: S"east")

with d in Direction.all().vals() (
  Debug(d) //prints all the directions in order.
  )

CCode
Enumerations also come with their customized set (nested) class,
we will see more about that when we discuss collections.

WTitle((5/5) Summary)

<ul><li>
Base classes are the minimal building block for your program;
be sure to declare all the right base classes to establish a convenient vocabulary
to talk about your problem domain.
</li><li>
Use Wcode(Num) as your first guess for numeric types,
if you have special needs, consider loading a numeric library.
</li><li>
Use Wcode(Size) for indexing linear datastructures like vectors and strings.
Beware of the tricky modulo arithmetic.
</li><li>
Use Wcode(Units) 
and Wcode(Alphanumeric) to give meaning to your constants.
In this way, the type system will help you to use values with the semantics you decided.
</li><li>
Both alphanumerics and enumerations
use Wcode(_".."), the WEmph(string literal postfix operator) to
provide a compact syntax.
</li></ul>
