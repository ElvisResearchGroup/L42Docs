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
 

WBigTitle(Errors and Exceptions: Messages in AdamTowel)

WTitle((1/5)`Errors, Messages, Asserts, Guards, .. so much terminology') 
In 42 when something takes an unexpected turn,
you can throw an Wcode(error)
This is similar to Java unchecked exceptions.
Every immutable object can be thrown as an error.
While it is possible to thrown informative strings, they do no offer enough
structure to fully take advantage of the error mechanism.
AdamTowel define the interface Wcode(Message):
a structured way to provide a certain kind of message to the user.
Wcode(Message)s has Wcode(.text()) ,
Wcode(.isResponse()) and Wcode(.responseOf()).
The text is the informative string, while if there is
a response (Wcode(msg.isResponse()==Bool.true()) 
then Wcode(msg.responseOf()) will be the 
former message in the chain.

There are two main kinds of Wcode(Message)s:
Wcode(Guard) and Wcode(Assert).
While Assertions are useful to observ bugs, the application
logic should not depend on them, since they may change
in unpredictable ways during library evolutions, and can be
enabled or disabled.
A guard is guaranteed to be consistent across library evolution
thus program logic can depend on them being thrown.

Assertions are very convenients to check for pre/post conditions
the following code show usages of Wcode(Assert.Pre) and Wcode(Assert.Bug)

OCode
Assert.Pre[ //preconditions
  myVal>0Nat;//simplest form
  myVal>0Nat msg:S"here with personalized message";
  myVal expected:42Nat //call equals and do a better error reporting
  ]//in a bunch of assertions, they are all going to be checked together.
Assert::Bug[ //postconditions/checks in the middle
  res expected: 42Nat msg:S" message"
  ]
if notGoodParameter (Assert::Pre"error message")
if observedBug  (Assert::Bug"error message")
CCode

WTitle(`(2/5) create, throw and capture')

You can create new kinds of messages using the 
service class of the message interface:

OCode
AnswerNotUnderstood:Message.$<<{implements Guard}
//this is a new kind of message, implementing Guard.
//you can also add methods to your kind of message.
//you can add fields, we will see this more in detail later.
/*..*/
//throwing an error
if this.ohNoNoNOOO() (error AnswerNotUnderstood"Well, too bad")

if this.iWasDistracted() (
  //throwing an error in response of another
  Guard other=NotListening""//empty message
  error AnswerNotUnderstood"Try again"(other)
  )

CCode

As you can see, since Wcode(Message) is an interface, it can not help us directly
to create messages, however it has a service nested class called Wcode($), that
is a class decorator helping us to create valid messages.
As you can see we can create messages with text, and
then we can optoinally add a reponse.
Now, let see how to capture errors:

OCode
(myWait=7.5MillionYear
 answer=DeepThought.answer()
 understanding=myBrain.process(answer)
 catch error AnswerNotUnderstood msg (/*handle it*/)
 catch error Guard msg (/*something else gone wrong*/)
 //in the catch bodies, undestanding and answer are not defined
 myBrain.accept(understanding)//normal execution if no issue
 )
CCode

As you see, in 42 there is no explicit Wcode(try) statement,
but any block of code can contain Wcode(catch).
The catch capture errors in the block before the last (group of) catches.
Catches can not see local variables declared in the scope of a possible error.
The following example explain the control flow more in detail:

OCode
res=(
 b1=CanGoWrong()
 b2=CanGoWrong()//see b1
 catch error Wrong msg1  1Nat//not see b1,b2
 catch error Guard msg2  2Nat//not see b1,b2
 b3=CanGoWrong()//can see b1, b2
 catch error Wrong msg3  3Nat//see b1,b2, not see b3
 4Nat//see b1,b2,b3
 )
CCode
The following code can assign to Wcode(res) either 1,2,3 or 4.
Note how every catch exits from the whole block.
Errors guarantee a property called strong error safety
(storng exception safety in the Java/C++ terminology)
This means that the body of a catch will observe the same 
state present at the start of the block.
This is enforced by disallowing catching errors if the (portion of the) block before can mutate visible objects.

That is, the following code do not compile
OCode
p=Person(name:S"Bill" age:23Year)
res=(
 p.age(p.age()+1Year)
 p.age(p.age()+1Year)
 catch error Guard msg2  (/*could see p with 23 or 24 years*/)
 p
 )
CCode

While the following is accepted.

OCode
res=(
 p=Person(name:S"Bill" age:23Year)
 p.age(p.age()+1Year)
 p.age(p.age()+1Year)
 catch error Guard msg2  (/*can not see p*/)
 p
 )
CCode

WTitle(`(3/5) exceptions and errors')

Exceptions are like checked exceptions in java.
As for errors, every immutable object can be thrown as an exception.
just write "exception" instead of "error" while trowing or capturing.
Exceptions represent expected, documented and relayable behaviour,
they are just another way to express control flow.
They are useful to carachterize multiple outcomes for an operation,
where is important to prevent the programmer from forgot about
the many possible outcome and focus only on its preferred one.
Exceptions are checked, so methods leaking exceptions have to
declare so in their header, as in the following.
OCode
/*somewhere in a GUI library*/
method
S promptUser(S text)
exception CancelPressed {
  /*implementation to open a text dialog*/
  }
CCode
Where the programmer using Wcode(promptUser) will have to handle 
the possibilty that the cancel button was pressed.

Exceptions does not enforce strong exception safety as error do,
so they can be used more flexibly, and since they are documented in
the types, we can take their existence in account while writing imperative programs.
Wcode(Assert)ions should not be thrown as exceptions, but only as errors.

Often, the programmer wants to just turn exceptions in errors or other exceptions.
This is possible with the following code:


OCode
//long version
DoStuff()
catch exception FileNotFound fnf (
  error WTF"I just created it!"(fnf)
  )

//short version
DoStuff()
error on FileNotFound
  WTF"I just created it!"
CCode
The two sipplets of code behave identically: the first
show a very common patter; 42 supports sintactic sugar to
ease following that pattern, as you can see in the second snipplet.
Wcode(WTF) stands for what a terrible failure
(as in android), and can be used to mark brances of code
that the programmer believe would never be executed.
Wcode(WTF) implements Wcode(Assert), thus code capturing
Wcode(WTF) is unrelayable.





WTitle(`(4/5) return')

Return, as we have seen, can be used to exit from the inner
most level of curly brakets.
Also curly brakets can have catches, let see some examples
OCode
{
x=DoStuff()
catch exception Stuff e1
  void//just swallow the exception
catch exception Guard e2
  obj.doSideEffect()//this method return void
catch exception Message e3
  return e3//a result of the curly brackets
y=DoStuff(x)
return y
error on Guard 
  WTF""
}  
CCode

Moreover, curly brakets/return can be used
to privide a different result if some computation fails:

OCode
res={return PlanA()
  catch error Guard x
    return PlanB()
  }
CCode

WTitle(`return looks similar to error/exception')
Return is actually anoher thing that can be thrown and captured.
While only immutable values can be thrown as errors/exceptions,
return can throw any kind of value, but returns can not flow
outside of the scope of a method.
Hold your head before it explodes, but curly brakets are just a syntactic sugar to capture returns; those two snipplets of code are equivalent

OCode
N res={if bla return e1
return e2}
//---
N res=(
  Void unused=(
    if bla return e1
    return e2
    )
  catch return N x
    x
  error void//this line is never executed
  )
CCode
Depending on how your brain works,
knowing the desugaring of Wcode({..return..})
can help you to use return better and understand why you can omit 
Wcode({..return..}) for simple method bodies, and why you can
write multiple groups of curly brakets and have local returns.
Or can just be very confusing. If you are in the second group, just
never ever write Wcode(catch return) explicitly and continue
your 42 experince ignoring the issue.


WTitle(`(5/5) Errors, exceptions and return, recall')
<ul><li>
Whenever possible, try to detect if your code misbehave, and 
terminate it with an Wcode(Assertion)
</li><li>
Whenever something out of your
control happen, Give it a name and throw it as an error, as in
OCode
NameOfIssue:Message.$<<{implements Guard}
/*...*/
if /*..*/ error NameOfIssue"more info"
CCode
It just take 2 lines, and will make debugging your code so much 
easier.
</li><li>
Use errors as much as makes sense, 
but use exceptions sparsely: they are needed only in few 
cases, mostly when designing public libraries.
</li><li>
To convert exception into errors or other exceptions, use the convienient short
syntax Wcode(error on T1,..,Tn  OtherMessage"") or 
Wcode(exception on T1,..,Tn  OtherMessage"")
</li><li>
Sometimes it is possible to write elegant and correct code
that is not covered in layers upon layers of error/exception checking,
but often is not possible or is not convenient.
Good 42 code can be composed for half of
its content of just error/exception handling/lifting and management.
Do not be scared of turning your code in it's own policemen.
</li></ul>
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
