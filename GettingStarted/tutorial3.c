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

You can import other numeric types by loading libraries.
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
Conversions between various numeric classes must be performed explicitly.
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




WTitle((2/5) Units)


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
As you can see, we can sum meters together, and we can use the support for multiplication, but we can not mix different units of measure.


Mathematically you can obtain the support out of the unit by
division: Wcode(42Meter/2Meter = 21Meter).
This do not work directly in 42, since multiplication and division
takes the support and not a unit.
Units provide method Wcode(div(that)) for this aim.
Units also provide method  Wcode(`#'inner()),
this is just extracting the value of the support from the unit.
This can be convenient during programming bud 
does not makes a lot of sense mathematically.
Methods like that, require to be used with care, they start with
Wcode(`#') to underline that they should be used with care.

OCode
Num n1=42Meter.div(2Meter)//=21Num
Num n2=42Meter.#inner()//=42Num
CCode



WTitle((3/5) Composite Units)

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
reachedHeight=myAcc.per(10Second).per(10Second)/2Num //after 10 sec
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




WTitle((4/5) Alphanumeric)
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

WTitle((5/5) Recall)

<ul><li>
Use Wcode(Num) as your first guess for numeric types,
if you have special needs, can consider loading a numeric library.
</li><li>
Use Wcode(Size) for indexing linear datastructures like vectors and strings.
Beware of the tricky modulo arithmetic.
</li><li>
Use Wcode(Units) 
and Wcode(Alphanumeric) to give meaning to your constants.
In this way the type system will help you to use values with the semantic you decided.
</li></ul>
