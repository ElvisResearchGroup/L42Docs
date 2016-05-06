WBigTitle(`Sequences: Strings and Collections')

WTitle((1/5)Vectors as Sequences, listing the basic operations)

As we have seen before, vectors can be declared using Wcode(Collections.vector(of)), as in the example below.

OBCode
Nums:Collections.vector(of:Num)//declaration for vectors of nums
/*..*/
xs=Nums[10Num;20Num;30Num]//immutable vector

ys=Nums.empty()//mutable vector
ys.add(right:10Num)
CCode

Immutable sequences are created with square brakets
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
Nums[a;b;b;c]-b==Nums[a;c]
Nums[a;b;b;c]--Nums[b;c]==Nums[a]
Nums[a;b;c]>>Nums[a;c] //holds
Nums[a;b;c]>>=Nums[a;c] //holds
Nums[a;b]>>=Nums[a;c] //holds
Nums[a;b]>>Nums[a;c] //not holds
Nums[a;b]>b //holds
b<Nums[a;b] //holds
Nums[a;b]& Nums[b;c]//Nums[b] set intersection
CCode

Mutable sequences are created with Wcode(.empty()) and
mutated by methods, as in the example below.
OBCode
//assuming foo.equals(Nums[a;b;c;d])
//consider each following line independently:

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

Both immutable and mutable collections can be 
accessed with the following methods:

OBCode
//access
foo.left()//a
foo.right()//d
foo.val(2Size)//c
foo.size()//4Size
foo.isEmpty()//not holds
CCode


<!--
When we will be able to mention those?
mut method T #left()
mut method T #right()
mut method T #val(N that)
-->

In addition of operators, immutable collections can
be manipulated by the following 

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

WTitle((2/5) `Wcode(with): a Swiss army knife to encode complex behaviour')


There are two basic usage for the Wcode(with) statement: as for-each and as a typecase.

OBCode
vec= Strings[S"foo"; S"bar"; S"beer"]
var S result=S""
with myElem in vec.vals() (result:=result++myElem)//like for(myElem:vec){..}
//result==S"foobarbeer"
with myData=foo.bar() (//like a typecase/switch/chain of instanceof
  on S  ( Gui.alert(myData) )  //if is a string
  on N case 2N.divides(myData) ( Gui.alert(...) ) //if is an even natural
  )
CCode

if Wcode(myData) is already declared one can simply write
OBCode
with myData (
  on S ( Gui.alert(myData) )  
  default (error S"myData was not a string") 
  )
CCode

Those two modes can be combined
OBCode
vec= AnyVector[S"foo"; 12N; S"beer";]
var S result=S""
with myElem in vec.vals() (on S ( result:=result++myElem ) ) 
//result==S"foobeer", composed by all strings inside vec
CCode

Wcode(with) can be used as list comprehension
OBCode
vec=AnyVector[S"foo"; 12N; S"beer";]
v=SVector[with myElem in vec.vals() (on S myElem )]//filter non strings
//v==SVector[S"foo"; S"beer";]
CCode

for multiple dispatch:

OBCode
method N m(Shape x, Person y, Vehicle z) //example of method using with
 with x y z (
 on Square Student Car (...return ...)//x here is a Square
 on Circle Person Airplane (...)//x here is a Circle
 default (...)//default case, here x is just a Shape
  )}
CCode

Or to iterate over multiple collections at once
OBCode
rs=Vector[1N;2N;3N;]
as= Vector[10N;20N;30N;]
bs= Vector[100N;200N;300N;]
//here a, b and r iterate over my data
with a in as.vals(), b in bs.vals(), var r in rs.vals() (r:=r+a+b)
//now rs==Vector[111N;222N;333N;]
CCode

While iterating on multiple collections, a dynamic error is usually raised if 
Wcode(rs),
 Wcode(as) and
 Wcode(bs) have different length.
This behaviour can be tuned in many way, for example using Wcode(cut()) and Wcode(fill(...))
No, I have to update with the new version

OBCode
rs=Vector[1N;2N;3N;]
as= Vector[10N;20N;30N;40N;50N;]
bs= Vector[100N;200N;]
with a in as.valsCut(), b in bs.vals(fill:300N), var r in rs.vals() (
  r:=r+a+b)
//rs==Vector[111N;222N;333N;]
CCode


This could go later
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








A very large class of practically useful programs can be obtained by
just declaring simple minded classes using Data and Collections.

WTitle(Libraries)


WBigTitle(not ready to read yet Libraries)

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
