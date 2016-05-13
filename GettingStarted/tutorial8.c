WBigTitle(Refactor and Extend)

WTitle((1/5)Refactor and Introspection)

WTitle(Refactor)
Wcode(Refactor) is a class supporting modification of
library literals.
For example, you may want to rename the method Wcode(importStructure(that)) in just Wcode(import(that)).
You can do the following:
OBCode
{reuse L42.is/AdamTowel
Db:Refactor.RenameMethod(Selector"importStructure(that)" 
  to:Selector"import(that)")<<Load<<{reuse L42.is/Db}
UnivDb:Db.import(Db.ConnectionS"...")
/*..*/
}
CCode
The type Wcode(Selector) represent method selectors;
in the same way the type Wcode(Path) represent 
paths, as in Wcode(Path"Refactor.RenameMethod") or
Wcode(Path"This").

There are a lot of refactoring operations nested under Wcode(Refactor):
<ul><li>
Wcode(RenameSelector)
and 
Wcode(RenamePath)
rename methods either at top level (as we just did) or
in an arbitrary nested library;
or rename paths in other paths
</li><li>

Wcode(Redirect)
allowing to remove a nested library and redirect all its references to
an external one. This allows to emulate generics, as we will see later.
</li><li>
Wcode(UpdateDocumentationSelector)
and Wcode(UpdateDocumentationPath)
to add to/alter/delete the documentation of methods/paths.
</li><li>
Wcode(MakeAbstractSelector)
and Wcode(MakeAbstractPath)
to remove all the implementation out of a method or path,
leaving only the public skeleton
</li><li>
Wcode(HideSelector)
andWcode(HidePath)
To mark methods or paths as private.
We have not seen details on private members, the main idea is that
they are renamed in invisible names that you can never guess, and automatically renamed on need by refactoring operations.
</li></ul>
WP
In addition to all those nested classes,
Wcode(Refactor) offers Wcode(`Refactor.compose(left,right)')
allowing a simmetric sum of two library literals.
The main idea is that members with the same name are recursively summed

WTitle(Introspection)
Wcode(Introspection) is
a class allowing to explore libraries, to discover what methods they have and so on.

The main classes inside of Introspection are
Wcode(Introspection.NestedLibrary),
 Wcode(Introspection.Method) and
Wcode(Introspection.Type).
You can obtain a nested library by calling the factory methods 
Wcode(Introspection(lib)) and Wcode(Introspection(classObj)),
respectively for library literals or class objects.
We will see later some example of use of Wcode(Introspection).


WTitle((2/5)Traits and Metaprogramming)

Metaprogramming is the most important feature of 42,
all the decorators that you have seen up to now are implemented with metaprogramming, that in 42 offers a good balance of freedom and
safety.
The main idea of 42 metaprogramming is that only library literals can
be manipulated, and that once a library literal has a name, it can not be directly metaprogrammed any more.
Thus, we call traits methods that return reusable (unnamed) code.
For example
OBCode
Transaction:{
  class method
  Library traitEnsureTransaction() {

    class method //without name: can be called as a functor
    Void(mut Db.Connection connection)
    exception Db.Query.Failure //no body: abstract method

    class method //without name: can be called as a functor
    Void (mut Db.Connection that)
    exception Db.Query.Failure (
      that.openTransaction()
      This(connection:that)//here we call us as a functor
      catch exception Db.Query.Failure x (
        that.rollback()
        exception x
        )
      that.commitTransaction()
      )
    }
  }

MyAction:Refactor.compose(
  left:Transaction.traitEnsureTransaction()
  right:{ //and now the missing implementation
    class method
    Void(mut Db.Connection connection)
    exception Db.Query.Failure {
      /*..my operation..*/
      }
    })

Main:{
  /*..*/
  MyAction(conn)//do the operation in a transaction
  /*..*/
  }
CCode

Note how Wcode(traitEnsureTransaction()) is just a normal 
class method that directly return a library literal.
Traits in 42 are nothing fancier than that.

Now Wcode(MyAction) will execute the operation inside of a transaction.
However, as you can see declaring Wcode(MyAction) using 
Wcode(Refactor.compose) is verbose.
Let see how we can improve our code and make Wcode(Transaction)
into a real class decorator.
First we want to check that the provided code has an 

OBCode
Transaction:{
  class method
  Library traitEnsureTransaction() {/*.. as before ..*/}
  
  InvalidAction:Message.$<<{implements MetaGuard}
  //meta guard is the root of all the metaprogramming guards
  class method //using << to define the babelfish operator
  Library << (Library that) 
  exception InvalidAction {
    i=Introspection(lib:that)
    if !i.hasMethod(Selector"(connection)") (exception InvalidAction"Action method missing")
    composed=Refactor.compose(left: This.traitEnsureTransaction(), right: that)
    exception on MetaGuard
      InvalidAction"
        'Action invalid:type of '(connection)' 
        'do not fit or already defined '(that)'
        "
    return Refactor.HideSelector(Selector"(connection)")<<composed
    error on Metaguard
      WTF"(connection) is there, ready to be hidden"
    }  
  }
MyAction:Transaction<<{
  class method
  Void(mut Db.Connection connection)
  exception Db.Query.Failure {
    /*..my operation..*/
    }
  }

CCode

Note how we check some well formedness of the parameter
in an Wcode(if), then we catch and wrap the exceptions of Wcode(compose(left,right)),
and finally we state our assumption that Wcode(HideSelector) can not fail in
that context.

Now we can use Transaction as a decorator.

WTitle((3/5)Extend)

Wcode(Extend)
 is a decorator implemented using 
Wcode(Refactor) and
Wcode(Introspection)
and provide in AdamTowel a flexible model of multiple inheritance with super calls.
As an example, in a game we can have a chest, that contains objects in a certain position,
a boat that can host humanoids and
a cargo boat, that can host humanoids and can contains objects like a chest.
We want to reuse the code of chest and boat to obtain the cargo boat.
For example
OBCode
Traits:{
  class method
  Library chest(){
    mut Objects objects
    /*.. methods to validate access to objects..*/
    read method
    Kg weight() {
      var Kg res=0Kg
      with o in this.objects().vals() (res+=o.weight() )
      return res
      }
    }

  class method
  Library boat() {
    mut Humanoids crew
    Kg maxCapacity
    /*.. methods to validate access to crew..*/
    read method
    Kg weight() {/*..with-loop on the crew..*/}

    read method
    Kg capacityLeft()
      this.maxCapacity()-this.weight()    
    }
  }

Chest:Data<<Traits.chest()
Boat:Data<<Traits.boat()
CargoBoat:Data<<Extend[Traits.chest();Traits.boat() ]<<{
  read method @override
  Kg weight() this.#1weight()+this.#2weight()
  }
CCode

As you see, we annotate with Wcode(@override) to 
override the Wcode(weight()) method, and we use 
Wcode(`#'1weight()) and 
Wcode(`#'2weight()) to refer to the super implementations.
As an alternative to Wcode(@override),
we could use Wcode(@hide) to just hide the old methods and put our new
version on top. There are two main difference between Wcode(@override) and 
Wcode(@hide).
With override internal references will refer to the new implementation,
while with hide they will refer to the old one.
With override the method type must be identical,
while with hide they can be completely different.


WTitle((4/5)Exercise: extending vector)

As an exercise, lets try to use what we learned to add a Wcode(sum()) method to
a vector.

OBCode
Nums:Extends[Collections.vector(of:Num)]<<{
  read method
  Num sum(){
    var Num res=0Num
    with n in this.vals() (res+=n )
    return res
    }
  }
CCode

Easy.
WP
However, we have done an extension only on our specific Nums vector, we would have to repeat
such code for each vector.
Can we produce vectors that will have a Wcode(sum()) method?
Well, this can only work for vectors of elements with a Wcode(+) operator, and a zero concept.

This is not trivial and require us to learn many concepts.
Our first attempt could be to use ...no....
OBCode
Summable:{
  class method
  Library vector(class Any of)
    Refactor.Redirect(Path"T" Extends[Collections.vector(of:of)]<<{
      read method
      of sum(){
      var of res=0of
      with n in this.vals() (res+=n )
      return res
      }
    }
  }
CCode


WTitle((5/5)Metaprogramming summary)

 <!--

OBCode
Observer:{interface
  mut method update()
  }
Observers:Collection.vector(ofMut:Observer)

Traits:{

  class method
  Library subject(){
    mut Observers observers

    class method
    mut Observers #default_observers()
      Observers[]

    mut method
    Void register(Observer that)
      this.#observers().add(right:that)

    mut method
    Void notify() 
      with Observer o in this.#observers().vals() (
        o.update()
        )
  }
  class method
  Library widget(){ /*..*/}

}

MyObservableWidget:Extends[Traits.subject(); Traits.widget()] <<{
  mut method @override
  Void repaint() (this.#2repaint() this.notify())
  }
CCode

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
Wcode(MyCode.hello(..)) 
use directly the Wcode(MyCode) class instance as
receiver. We can also give it a name 
e se vuoi puoi anche salvarlo su un binding locale, 
tipo x=MyCode  x.hello(...)
-->
