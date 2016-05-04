WBigTitle(`Interfaces, Concepts and Data')

WTitle((1/5)Interfaces, Basis)
In 42 interfaces are quite similar to interfaces in other oo languages.
There are however a couple of important differences:

WP
while implementing an interface method, you have not to repeat the
typing.
For example, in the following code, to implement Wcode(Shape.draw(that)) inside
of Wcode(Square), we do not repeat the types Wcode(Void) and Wcode(Canvas)
OCode
Shape:{interface
  method Void draw(Canvas that)
  }
Square:{implements Shape
  method draw(that){..}
  }
CCode

In 42, we say that the method Wcode(draw(that)) 
implemented in Wcode(Square)
WEmph(is defined by)
Wcode(Shape).
Each method is defined in a single point, either the class itself
or a (transitivelly) implemented interface.
This means that a class can not implement multiple interfaces defining methods
with the same name.
For example, this code is ill-typed:
OCode
Card:{interface
  method N draw()//the value of the drawn card
  }
Gun:{interface
  method N draw()//the time it takes to drawn the gun
  }
Wrong:{implements Card,Gun//not allowed
  }
CCode


Note that that would be bad 42 code anyway, you should define an
enumeration (see later) for your cards and use a Wcode(Second) unit of measure
for the time.

WTitle((2/5)Interfaces, Details)
However, interface diamond is allowed, that is, the following code is correct:
OCode
Shape:{interface
  method Void draw(Canvas that)
  }
Animal:{interface implements Shape
  method Meter run(){..}
  }
Noisy:{interface implements Shape
  method Void play(Audio that){..}
  }
LoudCat:{implements Animal, Noisy
  method draw(that){..}
  method run(){..}
  method play(that){..}
  }
CCode

You can further specify the type of an interface method by using the keyword 
Wcode(refine):
OCode
Monster:{interface
  method Monster spawnMinion()
  }
BigMonster:{implements Monster
  refine method BigMonster spawnMinion(){..}
  }
CCode


WTitle((3/5)`Concepts: ToS, Equals, Classable, ...')
Wcode(Concepts) is a class defined in AdamTowel,
containing interfaces commonly used by many classes.
WP
The most well known one is 
Wcode(Concepts.ToS), implemented by all objects that can
be converted in human readable strings.

In AdamTowel,
Alphanumeric, Numeric and Units just print
the string or the number as it is.
Other objects prints their fields content in square brackets.
Wcode(Data) will automatically generate a valid
Wcode(Concepts.ToS) implementation. For fields of interface type,
the class name of the stored instance is also produced.
Strings and numbers are enclosed with double quotes.

WP
Another very well known interface is Wcode(Concepts.Equals)
implemented by objects that can be compared with
Wcode(method Bool equals(Concepts.Equals that)).
Such objects usully also define methods Wcode(==) and Wcode(!=)
based on Wcode(equals), but is not required by the interface.

WCode(Data) will automatically generate a valid Wcode(Concepts.Equals)
implementation, and derived mehtods Wcode(==) and Wcode(!=).
The default implementaiton of Wcode(Data) do not takes in account circular object graphs, and will end up in loop in those cases. We will discuss later how to personalize the behaviour of Wcode(Data).

WP
Finally, an interface that near every class or interface implements is Wcode(Concepts.Classable).
It allows to get the class object out of an ordinary instance.
This is very usefull when wanting to create an instance based on another.
For example, to create another shape of the same kind of a given shape,
we could do the following:
OCode
Shape:{implements Concepts.Classable
  class method This newShape(Color that)
  }
Square:{implements Shape
  Color color
  method newShape(that)//implemented from Shape
    This(color:that)
  method clazz()//implemented from Concept.Classable
    This//this implementation can be omitted if "Data<<" is used
  }
/*..*/
Shape s=..
s.clazz().newShape(Color.red())
CCode
Where the interface Wcode(Shape) defines an abstract factry method (that is,
an interface class method returning Wcode(This))
and class Wcode(Square) implements
 Wcode(Shape) (and thus also Wcode(Concepts.Classable)).
 
 
WTitle((4/5)`Concepts.Invariant')
 
Wcode(Concepts.Invariant) is another interface implemented very often and
 that can be handled by Wcode(Data), but in a different way with respect to the former ones:
 An instance of an object is more than just a record storing other objects of various types;
 those values are binded together by an invariant: a property that
 holds for all values of that type.
 AdamTowel and Data use Wcode(Concepts.Invariant) to represent this idea:
 A class that does not implement Wcode(Concepts.Invariant) just have the
 empty invariant enforced by type type of its fields, but the programmer can specify a more stringent invariant by implementing Wcode(Concepts.Invariant), as 
 in the following code:
 
 OCode
Margin:Data<<{implements Concepts.Invariant
  var Meter minX
  var Meter minY
  var Meter maxX
  var Meter maxY
  method invariant()
    this.minX()<=this.maxX() & this.minY()<=this.maxY()
  }
CCode

Here we define a class Wcode(Margin) where the invariant
states that the min coordinates have to be smaller that the max ones.

Wcode(Data) will generate code so that the invariant is checked while the object is created
and in any field update.
Wcode(Data) guarantees that no Wcode(Margin) object that does not 
satify the invariant will ever be visible outside of the scope of the Wcode(invariant()) method itself.

See more about restrictions of this mechanism in (Wlink(dataAndInvariants,
`data and invariants'))
(This will go in that link, but need more context to be understood: more explanation of Capsule field vs Capsule bindings, exposers and lent, also, is not implemented in 42 Yet)
The current example shows a class with all immutable fields.
It is possible to enforce the invariant also on classes with immutable
and capsule fields.
Then Wcode(Data) will additionally check that the
lent exposers are all private and not used in internal nested classes,
and that every method using the lent exposer do not return a lent result.
It is unobvious why such requirents sufficie: the idea is that the only way a capsule value ...


WTitle((5/5)`Interfaces and Data, recall')

Interfaces in 42 serves the same role they serve in other languages,
with a little bit of a twist in the details.

The big news is that Decorators (Wcode(Data) in our examples) can
help and provide implementations for free.
This is much more powerfull with respect of traits, multiple inheritance or
Java 8 default methods, since the implementation can be generated ad hoc
by examining the class.
 

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
