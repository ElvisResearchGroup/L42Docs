WBigTitle(`Interfaces, subtypes and matching')

WTitle((1/5)`Interfaces, Basis and Details')
WTitle( Interfaces Basis)
Interfaces in 42 are quite similar to interfaces in other OO languages.
There are however a couple of important differences.

WP

While implementing an interface method, you can avoid the
type signature.
For example, in the following code, to implement Wcode(Shape.draw(that)) inside
of Wcode(Square), the types Wcode(Void) and Wcode(mut Canvas) are not repeated.
OBCode
Shape = {interface
  method Void draw(mut Canvas that)
  }
Square = {[Shape]
  method draw(that) = /*..*/
  }
CCode

In 42, we say that the method Wcode(draw(that)) 
implemented in Wcode(Square)
WEmph(is declared by)
Wcode(Shape).
Each method is declared at a single point. 
WComm HERE 7/9/21
Methods can be 
defined (that is, declared and implemented)
in the class itself;
or declared in a (transitively) implemented interface and
then just implemented.
This means that a class can not 
satisfy multiple interfaces declaring methods
with the same name.
For example, this code is ill-typed: 
OBCode
Card = {interface
  method Num draw() //the value of the drawn card
  }
Gun = {interface
  method Num draw() //the time it takes to drawn the gun
  }
Wrong = {[Card,Gun] //not allowed
  }
CCode


Note that would be bad 42 code anyway, you should define an enumeration (or an alphanumeric)
for your cards and use a Wcode(Second) unit of measure
for the time.

WP

However, interface diamond is allowed, that is, the following code is correct: 
OBCode
Shape = {interface
  method Void draw(mut Canvas that)
  }
Animal = {interface [Shape]
  method Meter run()
  }
Noisy = {interface [Shape]
  method Void play(mut Audio that)
  }
LoudCat = {[Animal, Noisy]
  method draw(that) = /*..*/
  method run() = /*..*/
  method play(that) = /*..*/
  }
CCode

WP

You can refine the return type of an interface method, by repeating the full type signature with the desired return type.
OBCode
Monster = {interface
  method Monster spawnMinion()
  }
BigMonster = {[Monster]
  method BigMonster spawnMinion() = /*..*/
  }
CCode

However, the parameter types can not be refined.

WTitle((2/5) Interfaces and class methods)

Interface methods in 42 are all abstract, that is, without bodies.
A version of the body will be provided by all classes implementing the interface.
WEmph(This also include class methods.)
WBR
For example, consider the following code: 
OBCode
Shape = {interface
  class method 
  Num numberOfSides()
  class method
  This newShape(Color that)
  method
  Void draw(Canvas that)
  }

Square = {[Shape]
  Color color
  method numberOfSides() = //class method implemented
    4Num
  method newShape(that) //class method implemented
    This(color=that)
  method draw(that) = //immutable method implemented
    /*..*/
  }
/*..*/
class Shape kindOfShape= Square
kindOfShape.numberOfSides()==4Num //holds
Shape s= kindOfShape.newShape(Color.red())
CCode

The pattern in the code above allows to encode the abstract factory 
pattern in a much simpler way: 
the binding Wcode(kindOfShape) serve the role of
an instance of an abstract factory, and can create instances of
a specific kind of shape.
WP

In 42 interfaces can not have 
implemented class methods.
Sometimes there is the need of semantically associate some behaviour with an interface.
For example we could check intersections between shapes using
the draw method.
What we would need, is a traditional (non dynamically dispatched) static method.
In 42, static methods are just nested classes with a single class method with the empty name. In 42 adding new classes is very common, so do not be scared of adding a new class just to host a method.

WBR
For example
OBCode
Shape = {interface
  /*..*/
  Intersect = {class method
    Bool (Shape left, Shape right) = 
      /*..*/
    }
  }
CCode

WTitle((3/5)`Subtyping')
Dynamic dispatch is the most important feature of Object oriented languages.
Subtiping is the main feature supporting dynamic dispach; for example 
we can iterate over a list of Wcode(Shape)
and Wcode(draw) them without the need to consider case by case all the possible kinds of 
Wcode(Shape).
 
Note how modifiers (Wcode(mut),
Wcode(read),
Wcode(capsule),..)) offers subtyping but not dynamic dispatch.

There is a fundamental compromise between interfaces and classes:
<ul><li>
Interfaces allows client code to be implicitly parametric on the behaviour of individual objects.
The client code can take no assumption on the specific implementation of the interface.
</li><li>
Classes allows client code to rely on their invariant.
The user code is forced to pass only a specific kind of implementation.
</li></ul>


WTitle((4/5)`Matching')

It is possible to inspect the runtime type of object by using matching.
We will now provide varius examples of matching and explain the behaviour.

OBCode
method Square example1(Shape s1, Shape s2) = {
  if s1 <: Square ( return x )//s1 is of type Square in the if body
  if Square y = s1.bar() ( return y ) //if bar returns a Square, we call it y
  if x<: HasToS, z<:Triangle ( return z.with(x.toS()) )
  error X""
  }
CCode 
We can use an Wcode(if) to check for the type of a local binding or an expression.
We can also check for more then one local binding at a time.
This Wcode(if-return) pattern allows to write complex dispatch in a compact way.
The syntax Wcode(<:) can also be used in expressions when the type system is unable to figure out
the type, usually because some information will be generated by a decorator on a later stage, thus sometimes we may have to write code like the following
Wcode(a.bla()<:mut Foo.baz()): the method 
Wcode(baz()) is called on the result of 
Wcode(a.bla()), that we expect to return a
Wcode(mut Foo).
While the Wcode(if _ <: _) will run the check at run time,
the cast is checked at compile time, as if we wrote 
Wcode(`(mut Foo tmp = a.bla(), tmp.baz())')

WBR
The code below
OBCode
method Point example2(Point left,Point right) = {
  (x1,y1) = left
  (x2,y2) = right
  return Point(x=x1+x2,y=y1+y2)  
  }
CCode
is equivalent to
OBCode
method Point example2(Point left,Point right) = {
  x1=left.x()
  y1=left.y()
  x2=right.x()
  y2=right.y()
  return Point(x=x1+x2,y=y1+y2)  
  }
CCode
That is: we can match out the result of any no-arg method into a variable with the same name, plus an optional numeric index.
This syntax can also be used inside of the Wcode(if).
This is often done for fields. Of course it works also to extract a sigle field:

OBCode
method Square example3(Shapes ss) = {
  (size) = ss //extracts the 'size' from ss
  (I size1) = ss//same but with explicit type
  if (Square left) = ss ( return left )//matches if the leftmost element of the list is a Square
  if (Triangle left, Square right) = ss ( return right )//matches if the leftmost and the
    //rightmost elements are a Triangle and a Square.
  if Circle(Point center) = ss.left() ( return center.squareAround() ) //matches if the leftmost
    //is a Circle and the method center() returns a Point. Both checks are dynamic.
  error X""
  }
CCode

As we have seen, matches are a form of syntactic sugar allowing to be more compact when extracting subcomponents and taking decisions about their dynamic types.

WTitle((5/5)`Interfaces, subtypes and matching, summary')

Interfaces in 42 serves the same role that they serve in other languages,
with a little bit of a twist in the details.

The big news is that Decorators (Wcode(Data) in our examples) can
provide the boilerplate implementations for free.
This is much more powerful than traits, multiple inheritance or
Java 8 default methods, since the implementation can be generated by examining the class.
 
WBR
In the right conditions, matching is a useful tool to reduce code size and complexity.
However, dynamic dispach is still the preferred option in the general case: we can only match on a finite
amount of pre-defined cases, thus matching is fragile with respect to code evolution.