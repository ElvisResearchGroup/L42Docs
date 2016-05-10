WBigTitle(`Sequences high level manipulation')

WTitle((1/5)Vectors as Sequences, listing the basic operations)

As we have seen before, vectors can be declared using Wcode(Collections.vector(of)), as in the example below.

OBCode
Nums:Collections.vector(of:Num)//declaration for vectors of nums
/*..*/
Nums xs=Nums[10Num;20Num;30Num]//immutable vector
//note how we declare the type explicitly,
//the default vector would be mutable.
CCode

Immutable sequences are created with square brackets
and can be combined with operators.
The general idea is that operators 
Wcode(+,-,<,>) works 
on one sequences and one element,
while the corresponding doubled-up operators
Wcode(++,--,<<,>>, >>=,<<=,==)
works on two sequences.
In the details, as you can see in the code below:
OBCode
Nums[a;b;c]+d==Nums[a;b;c;d]
Nums[a;b]++Nums[c;d]==Nums[a;b;c;d]
Nums[a;b;b;c]-b==Nums[a;c] //only if elements implements Concepts.Equals
Nums[a;b;b;c]--Nums[b;c]==Nums[a] //same for all the operators under
Nums[a;b;c]>>Nums[a;c] //holds
Nums[a;b;c]>>=Nums[a;c] //holds
Nums[a;b]>>=Nums[a;c] //holds
Nums[a;b]>>Nums[a;c] //not holds
Nums[a;b]>b //holds
b<Nums[a;b] //holds
Nums[a;b]& Nums[b;c]//Nums[b] set intersection
CCode

In addition of operators, immutable collections can
be manipulated by the following methods:

OBCode
//filtering
Nums[a;b;c;d].without(index:2Size)//Nums[a;b;d]
Nums[a;b;c;d].withoutLeft()//Nums[b;c;d]
Nums[a;b;c;d].withoutRight()//Nums[a;b;c]
//filtering, if elements implements Concepts.Equals
Nums[a;b;c;d].withoutAll(elem:b)//Nums[a;c;d]
Nums[a;b;c;d].withoutLeft(elem:b)//filter out the leftmost b
Nums[a;b;c;d].withoutRight(elem:b)//filter out the rightmost b

//with replacement
Nums[a;b;c;d].withLeft(b)//Nums[b;b;c;d]
Nums[a;b;c;d].withRight()//Nums[a;b;c;b]
Nums[a;b;c;d].with(2Size,d)//Nums[a;d;c;d]

//appending
Nums[a;b;c;d].withAlsoLeft(b)//Nums[b;a;b;c;d]
Nums[a;b;c;d].withAlsoRight()//Nums[a;b;c;d;b]
Nums[a;b;c;d].withAlso(2Size,b)//Nums[a;b;b;c;d]
CCode

Immutable collections (and also mutable ones, as we will see later)
can be accessed with the following methods:

OBCode
//access
foo.left()//a
foo.right()//d
foo.val(2Size)//c
foo.size()//4Size
foo.isEmpty()//not holds
CCode





WTitle((2/5) `Suggested parameter values using "\"')

In 42 is possible to use Wcode(\) while calling a method or using the square brackets,
to ask the receiver for a suggestion about the parameter values.
The library designer have full freedom to implement those suggestion in the more opportune way, however we
recognize three important common patterns:
WP
When setting/updating a value, the old value is suggested.
WP
When adding a new value, the factory is suggested.
WP
When the parameter is a number from zero to a max, the maximum is suggested.
WP
For example:
OBCode
Nums[a;b;c;d].withAlso(left:42\) //the \ is Num
Nums[a;b;c;d].without(index:\-1Size)//remove the last (the right-most)
Nums[a;b;c;d].with(left:\ * 2Num)//the leftmost is now a*2

Points[\(x:12\ y: 0\)]==Points[Point(x:12Num, y:0Num)]
CCode
Sometime, using Wcode(\) makes a huge difference,
for example, for the animal example of before:
OBCode
horse.location(\.with(x:\+20\)
//is equivalent to the much longer
horse.location(horse.location().with(
  x:horse.location().x()+20Num)
CCode

Is also possible to use Wcode(\) followed by an identifier, that will denote the method
with the same name on the receiver.
For example, if we want to reflect a point, and invert x and y coordinate, we can write

OBCode
p=point.with(x:\y, y:\x)
//is equivalent to
p=point.with(x:point.y(), y:point.x())
CCode

The Wcode(\) is very convenient also while initializing list/set of enumerated values.
For example

OBCode
Direction=Enumeration"north, east, south, west"
Direction.Set[\north;\east]//the bitflag corresponding
//to the set of north and east.
//is equivalent to the much longer
Direction.Set[Direction.north();Direction.east()]

p=point.with(x:point.y(), y:point.x())
CCode








WTitle((3/5) `Mutable sequences')


Mutable sequences can be more efficient that 
immutable ones, and are more general, since they 
can handled mutable objects.
The square brackets creates mutable sequences/collections,
so 

OBCode
foo=Nums[a;b;c;d]
//equivalent to 
mut Nums foo=Nums[a;b;c;d]
CCode


Now we show some methods over mutable collections, consider each following line independently:
OBCode
//setting a value in a position
foo(2Size,val:b)//foo.equals(Nums[a;b;b;d])
//setting at left or right
foo.left(b)//foo.equals(Nums[b;b;c;d])
foo.right(b)//foo.equals(Nums[a;b;c;b])

//add a value in a position
foo.add(2Size,val:b)//foo.equals(Nums[a;b;b;c;d])

//add at left or right
foo.add(left:b)//foo.equals(Nums[b;a;b;c;d])
foo.add(right:b)//foo.equals(Nums[a;b;c;d;b])

//removal
foo.remove(index:2Size)//foo.equals(Nums[a;b;d])
foo.removeLeft()//foo.equals(Nums[b;c;d])
foo.removeRight()//foo.equals(Nums[a;b;c])

//removal, if elements implements Concepts.Equals
foo.removeAll(elem:b)//foo.equals(Nums[a;c;d])
foo.removeLeft(elem:b)//remove the leftmost b
foo.removeRight(elem:b)//remove the rightmost b
CCode



Mutable collections can become immutable by promotion; for example,
a method without mutable parameters returning a mutable collection 
can be used to initialize a mutable one.
You may need to explicit the type of the local binding to force the promotion.
For example

OBCode
Nums myNums=DoIt.getMutableNums() //ok promotions happens, myNums is immutable

myNums=DoIt.getMutableNums() //myNums type is inferred to be mut Nums
CCode

Mutable collections can contains mutable objects.
While this can be useful in special circumstances, it can create aliasing issues similar to the
ones of the animals example of before.
To warn against such issues, methods Wcode(left()), Wcode(right()) and Wcode(val(that)) return 
readable references of the mutable objects. In order to obtain 
the actual mutable reference, the user need to use the methods
Wcode(`#left()'), Wcode(`#right()') and Wcode(`#val(that)').



WTitle((4/5) `Wcode(with): a Swiss army knife to encode complex behaviour')


There are two basic usage for the Wcode(with) statement: as for-each and as a typecase.

OBCode
//as a for each
vec= Strings[S"foo"; S"bar"; S"beer"]
var S result=S""
with myElem in vec.vals() (result:=result++myElem)//like for(myElem:vec){..}
//result==S"foobarbeer"

//as a typecase
with myData=foo.bar() (//like a typecase/switch/chain of instanceof
  on S  Debug(S"A string "++myData)   //print strings
  on Num  void //do nothing if is a number
  on Concepts.ToS Debug(myData) //print stringables that are not numbers. 
  )
CCode

if Wcode(myData) is already declared one can simply write
OBCode
with myData (
  on S Debug(myData)
  default error WTF"" 
  )
CCode

Those two modes can be combined
OBCode
vec= Anys[S"foo"; 12Num; S"beer";]
var S result=S""
with myElem in vec.vals() (on S  result:=result++myElem  ) 
//result==S"foobeer", composed by all strings inside vec
CCode

Wcode(with) can be used as list comprehension
OBCode
vec=Anys[S"foo"; 12Num; S"beer";]
v=Strings[with myElem in vec.vals() (on S use[myElem] )]//filter non strings
//v==Strings[S"foo"; S"beer";]
CCode

for multiple dispatch:

OBCode
method Num m(Shape x, Person y, Vehicle z) //example of method using with
 with x y z (
 on Square Student Car (..return ..)//x here is a Square
 on Circle Person Airplane (..)//x here is a Circle
 default (..)//default case, here x is just a Shape
  )}
CCode

Or to iterate over multiple collections at once
OBCode
rs=Nums[1\;2\;3\;]
as= Nums[10\;20\;30\;]
bs= Nums[100\;200\;300\;]
//here a, b and r iterate over my data
with a in as.vals(), b in bs.vals(), var r in rs.vals() (r:=r+a+b)
//now rs==Nums[111\;222\;333\;]
CCode

While iterating on multiple collections, a dynamic error is usually raised if 
Wcode(rs),
 Wcode(as) and
 Wcode(bs) have different length.
This behaviour can be tuned in many way:
iterators can be parametrized with
Wcode(from:) , Wcode(to:), Wcode(fill:) and 
Wcode(stopOk:) (I need a better name for this last one).

Wcode(from:) and Wcode(to:) are on default zero and the sequence length,
and can be specified to start from another value, like 1 or 2, and
to end before the very end.

Wcode(fill:) can be used in combination with 
Wcode(from:) and Wcode(to:)
to iterate outside of the sequence range.
If Wcode(fill:) is used without specifying Wcode(to:), the
sequence is considered infinite and iteration may go on forever.

Wcode(stopOk:) is useful when multiple collections are iterated at once,
and specify the minimal allowed iteration cycles (suggested value is 0).
Let see some examples:
OBCode
with x in xs.vals(), y in ys.vals(fill:10N)
//will iterate for as long as xs, even if ys is shorter
//will stop after xs.size() cycles, and fail if xs.size()<ys.size()

with x in xs.vals(), y in ys.vals(fill:10N stopOk:\)
//will iterate for as long as xs, even if ys is shorter
//will stop after xs.size() cycles

with x in xs.vals(stopOk:\), y in ys.vals(stopOk:\)
//will iterate for as long as both xs and ys have elements

with x in xs.vals(fill:10N), y in ys.vals(fill:10N)
//will go on forever

with x in xs.vals(stopOk:5N), y in ys.vals(from:1N to: \-1N stopOk:3N )
//will extract at least 5 elements from xs, will skip the first 
//and the last element of ys and extract at least 2 elements from ys.
CCode


And, in a case similar of before:
OBCode
rs=Nums[1\;2\;3\;]
as= Nums[10\;20\;30\;40\;50\;]
bs= Nums[100\;200\;]
with a in as.vals(stopOk:\), b in bs.vals(fill:300Num), var r in rs.vals() (
  r:=r+a+b)
//rs==Nums[111\;222\;333\;]
CCode


WTitle(`Strings interpolation, even better with Wcode(with)')

Alphanumeric classes and strings can be seen as immutable
sequences of Strings of length 1.

All the operators working on immutable sequences works on strings and alphanumerics.

However, they can not be constructed with square brackets, that is Wcode(S[a;b;c]) does not compile.
Square brackets can be used to interpolate strings and alphanumerics; as in:
OBCode
S"hello "[name]", have a good day, and do not panic!"
CCode
However, also Wcode(with) can be used; this can make code very compact,
for example assume we want to collect some names and numbers in a string:
OBCode
res=S"your info: "[with name in names.vals(), num in nums.vals()  (
  use[ S"name:"[name]", num:"[num]""]
  )]""//we always need the ending string, even if empty.
CCode
In order to put a semicolon between elements in our string, we can use Wcode(sep:)
OBCode
res=S"your info: "[with name in names.vals(), num in nums.vals()  (
  use[ S"name:"[name]", num:"[num]"", sep:S"; "]
  )]""//we always need the ending string, even if empty.
CCode


WTitle(`(5/5) Collections recall')

<ul><li>
There are a tons of methods and operators to know, but since most code works 
around collections, it is worth the effort to memorize them.
</li><li>
Immutable collections are easy to play with, using operators and with methods.
</li><li>
Mutable collections can be more efficient and flexible, but they come with additional
difficulties.
</li><li>
Most methods have a general version that works with an index, and specialized Wcode(left) and
Wcode(right) variants.
</li><li>
Wcode(/) can help remove a lot of boilerplate, but is not trivial to get used to it.
</li><li>
Wcode(with) is very useful and flexible. It is common to find methods composed be just a large
Wcode(with) statement plus a little pre and post processing around it.
</li></ul>

WBigTitle(not ready to read yet)

A very large class of practically useful programs can be obtained by
just declaring simple minded classes using Data and Collections.



WTitle(Libraries)
42 stands for the primate of libraries, so let see some libraries in action.
We have already see how to chose a towel, and many classes that are likely to be present in such towel, like
Wcode(S) and Wcode(Data).
Let see now how to load a library from its url:
OBCode
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
    
    
    
    WTitle((2/5) Iteration)

It is possible to iterate over a range of numbers:

OBCode
with i in 4Size.vals() (
  Debug(i)//prints 4,5,6,7 and so on forever
  )
CCode

OBCode
with i in 4Size.upTo(42Size) (
  Debug(i)//prints 4,5,6,7 and so on up to 42 excluded
  )
CCode


Resources used within an iteration can be released after the iteration 
since collections are notified when the iteration ends.

OBCode
//a contains "foo1 \n foo2 \n foo3"
//b contains "bar1 \n bar2"
with 
  input in LineStream.readFile(S"a"), 
  var output in LineStream.readWriteFile(S"b",fill:S"None") (
    output:= output +" : "+input)//line by line, add input in the file
//b contains "bar1 : foo1 \n bar2 : foo2 \n None : foo3"
CCode


    
 -->	
OBCode
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
OBCode
Points:Collections.mutList(Point)

...

zero=Point(x=0Int, y=0Int)
one=Point(x=1Int, y=1Int)
ps0=Points[]//the empty list
ps1=Points[zero;one]//contains zero, one
ps2=Points[zero;one]//contains zero, one

CCode



<!--OBCode Library myCode={ method Int foo()} CCode
A local binding of type Library initialized with a class with a single Wcode(foo()) method. Since there is no body, it is an abstract method.
 -->	 

 <!--WTitle(Simpler complete program)

Let now starts showing the simplest 42 program: an empty library.

OBCode {} CCode

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
