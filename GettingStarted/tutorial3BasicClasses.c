WBigTitle(Basic classes)

WTitle((1/5) Num, I, Double and Math.Long)
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

Another useful numeric type is Wcode(I), for index and index offsets.
It corresponds to sizes and indexes in sequences.
Wcode(I)s are returned by Wcode(size()) methods
and are expected as parameter by indexing methods.
Wcode(I) represent 32 bit integers with the usual 
but triky modulo arithmetic.

WTitle(Other numeric types)

AdamTowel offers two other numeric types:
Wcode(Double) (64 bits floating point) and Wcode(Math.Long) (64 bits integers, rarelly used).


WTitle(Conversions)
Conversions between various numeric classes must be performed explicitly.

AdamsTowel offers a simple way to convert between numeric classes, and more in general
between base classes.
All numeric  classes implements Wcode(Math.Numeric)
so that they can be converted in to each other using the the unnamed method. For example we can convert indexes into doubles by writing Wcode(Double(12I)).
This will avoid precision loss as much as possible.

WTitle((2/5) Units: An example library)

We now see how to load and use an interesting 42 Library: Units
Consider the following code, where the class decorator Wcode(Load) allows to load libraries and embed them in the 
current context, while the
reuse keyword imports the code from the web. 
OBCode
reuse [L42.is/AdamTowel]
Unit = Load:{reuse[L42.is/Unit]}
Year = Unit(I)
Person = Data:{S name, Year age}
CBCode

The library
Wcode(Unit)
offers methods to create units out of numeric supports, like Wcode(Num) and Wcode(I).
The code above shows how to create a Wcode(Year) unit and use it to represent a person age.

Units can be sum with themselves and multiplied by constants; for example
Wcode(3Year+2Year == 5Year) and Wcode(3Year *2I == 6Year)would hold, but Wcode(3Year * 2Year) would not compile.

Unit could be used to manually define all the units of SI; and a pre-build unit of such reusable code is already provided in the library; we simply need to specify the desired support:

OBCode
SI = Class:Unit.TraitSI['Support=>Num]
..
res= (6SI.Meter +4SI.Meter)*2Num //20Meter
//wrong1= 6Meter+2Second
//wrong2= 6Meter/2Second
CCode
As you can see, we can sum meters together, and we can use the support for multiplication, but we can not mix different units of measure.

Mathematically you can obtain the support out of the unit by
division; that is, 42 meters divided by 2 meters is  21.
Units also provide method  Wcode(`#'inner()),
this is just extracting the value of the support from the unit.
This can be convenient during programming but 
does not make a lot of sense mathematically.
Methods like that are required to be used with care, so they start with
Wcode(`#') to underline that.

OBCode
Num n1= 42SI.Meter / 2SI.Meter //= 21Num
Num n2= 42SI.Meter.#inner() //= 42Num
CCode

Below you can find some code using units in interesting ways

OBCode
SI.Meter res1 = (6SI.Meter+4SI.Meter)*2Num //20M

Num res2 = 42SI.Meter/2SI.Meter

Num res3 = (42SI.Meter).#inner()

SI.Velocity fast1 =  42SI.Meter/0.1SI.Second

fast2 = SI.Velocity"420" //equivalent ways to initialize it
fast3 = SI.Velocity"840/2"

distance1 = 60SI.Second * fast1

g = 9.8SI.Acceleration

speedAfter = 10SI.Second * g //98 m/s

t = 10SI.Second

//free fall distance d=(gt^2)/2
distance2 = (g*t*t)/2Num//490 m after 10s free fall

//Newton=Kg*m/s2 = Kg*Acceleration
myRoket = 900SI.Newton
gForceOnMe = 80SI.Kg*g //little less than 800
myLift = myRoket-gForceOnMe
if myLift>0SI.Newton (Debug(S"I can fly"))
myAcc = myLift / 80SI.Kg
reachedHeight = (myAcc*t*t) / 2Num //after 10 sec
CCode


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
    if !index.isPresent() (error Alphanumeric.ParseFail"@ not found")
    local= that(end: index.get()) //string slicing
    domain= that(start: index.get()+1Size ) //string slicing
    if domain.contains(S"@") (error Alphanumeric.ParseFail"multiple @ found")
    return This(that,local: local,domain: domain)
    } //call the factory with fields plus the original string
}
/*..*/
myEmail= Email"arthur.dent@gmail.com"
myEmail.local() ==S"arthur.dent" //holds
myEmail.domain() ==S"gmail.com" //holds
myEmail.toS() ==S"arthur.dent@gmail.com" //holds
CCode

Note how we can raise an error if the string does not have the shape we expected.
We will see errors/exception in more detail soon.
We can define fields, and compute their values by parsing the string.
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
be sure to define all the right base classes to establish a convenient vocabulary
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
offer  the string literal postfix operator to
provide a compact initialization syntax.
</li></ul>
