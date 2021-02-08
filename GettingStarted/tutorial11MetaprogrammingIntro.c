WBigTitle(`Finally, Metaprogramming')

WTitle((1/5)Traits and Metaprogramming)

Metaprogramming is the most important feature of 42.
All the decorators that you have seen up to now are implemented with metaprogramming, 
which shows that 42 offers a good balance of freedom and safety.
WP
The main idea of 42 metaprogramming is that only library literals can
be manipulated.
Metaprogramming is evaluated top down nested/inner-most first.
Once a library literal has a name, it can not be independently metaprogrammed; but only influenced
by metaprogramming over the library that contains it.

WP

Wcode(Trait) is a decorator allowing to store code in a reusable format.
Wcode(Class) is a decorator that extracts the code from a trait. For example
OBCode
TraitMsg = Trait:{
  class method S msg() = S"Hello world" 
  }
TraitPrint = Trait:{
  class method Void printMsg() = Debug(this.msg())
  class method S msg()
  }
TraitPrintMsg = TraitPrint:TraitMsg //TraitPrintMsg is another trait
ClassPrintMsg = Class:TraitPrintMsg
ClassPrintMsg = Class:TraitPrint:TraitMsg //equivalent
ClassPrintMsg = {//also equivalent
  class method Void printMsg() = Debug(this.msg())
  class method S msg() = S"Hello world" 
  }
Main0 = ClassPrintMsg.printMsg() //prints Hello world
CCode

In this code we show that Wcode(ClassPrintMsg) contains all the code of both
Wcode(TraitPrint) and Wcode(TraitMsg).
Note how the abstract Wcode(class method S msg()) in Wcode(TraitPrint) is merged with the corresponding implemented method in Wcode(TraitMsg).

Traits allows to merge code from different sources, as it happens with multiple inheritance.
However, traits are flattened: The code is actually copied in the result.

Traits are boxes containing the code and offering methods to manipulate such code.
A trait can be created by doing either Wcode(Trait(theCode))
or Wcode(Trait:theCode).
The trait object has a method Wcode(.code()) returining the contained code.
Trait content can be composed using the operator Wcode(:) (as shown above) or Wcode(+).
For traits there is no diference in behaviour between Wcode(:) and Wcode(+), but the operator precedence and associativity is different.

Simply composing traits allows to emulate a large chunk of the expressive power of conventional inheritance.
For example, in Java we may have an abstract class offering some implemented and some abstract methods.
Then multiple heir classes can extend such abstract class implementing those abstract methods.
The same scenario can be replicated with traits: a trait can offer some implemented and some abstract methods. Then multiple classes can be obtained composing that trait with some other code implementing those abstract methods.

OBcode
A = Trait:{/*implemented and abstract methods here*/}
B = Class:A:{ /*more code here*/}//reuse the code of A
C = Data:Class:A:{ /*more code here*/}//reuse the code of A, also with Data
CCode

WTitle((2/5)Trait composition: methods, nested classes and state.)
Trait composition merges members with the same name. As shown above, this allows method composition.
Also nested classes can be merged in the same way: nested classes with the same name are recursivelly composed, as shown below:
OBCode
T1 = 
  Trait({
    Foo = {
      method S hello()
      method S helloWorld() = this.hello()++S" World"
      }
    })
  +
  Trait({
    Foo = {
      method S hello()=S"Hi" 
      }
    })
//it is equivalent to
T1 = 
  Trait({
    Foo = {
      method S hello()=S"Hi" 
      method S helloWorld() = this.hello()++S" World"
      }
    })
CCode

But what about fields? how are fields and constructors composed by traits?
The answer to this question is quite interesting:
In 42 there are no true fields or constructors; they are just abstract methods serving a specific role.

That is, the following code declares a usable Wcode(Point) class:
OBCode
TraitGeometryPoint = Trait:{
  Point = {
    read method Num x()
    mut method Void x(Num that)
    read method Num y()
    mut method Void y(Num that)
    class method mut This (Num x,Num y)
    method This double() = \(x=this.x()*2\,y=this.y()*2\)
    }
  }
Geometry1=Class:TraitGeometryPoint //declaring class Geometry1
..
  imm p=Geometry1.Point(x=3\,y=4\)
  p2 = p.double()//example usage
CCode
That is, any Wcode(read), Wcode(imm) or Wcode(mut) no-args abstract method can play the role of a getter for a corrispondly named field, and any abstract Wcode(class) method can play the role of a factory, where the parameters are used to initialize the fields.
Finally, Wcode(mut) methods with one argument called Wcode(that) can play the role of a setter.
Candidate getters and setters are connected with the parameters of candidate factories by name.
To allow for more then one getter/setter for each parameter, getters/setters names can also start with any number of Wcode(#).
We call those abstract methods WEmph(Abstract State Operations).
In Java and many other languages a class is abstract if it has any abstract methods.
In 42, a class is coherent if its set of abstract state operations ensure 
that all the callable methods have a defined behaviour; this includes the initialization of all the usable getters. More in the detail, a class is coherent if:
<ul><li>
All candidate factories provide a value for all candidate getters, and the all types of those values
agrees with the return type of the corresponding getters.
The parameter type of all candidate setters agrees with the return type of the corresponding getters.
</li><li>
Additionally, any non-class method can be abstract if none of the candidate factory return a value whose modifier allows to call such a method.
</li></ul>
In particular, this implies that if a class has no candidate factories, 
any non class method may be abstract, as shown below:
OBCode
Foo = {
  class method Void bar()=Debug(S"coherent!") 
  method I answer()//abstract no problem!
  }
Main = Foo.bar()
CCode

A main can call class methods only on coherent classes that only depend from other coherent classes, thus for example
OBCode
Foo0 ={class method Void bar0()}
Foo = {class method Void bar()=Foo0.bar0() }
Main = Foo.bar()
CCode
The decorators Wcode(Class) and Wcode(Data) also checks for coherence: the following application of Wcode(Class)
OBCode
T1 = Trait:{class method Void bar()}
C1 = Class:T1
CCode
would fail with the following message: Wcode("The class is not coherent. Method bar() is not part of the abstract state").
We can use Wcode(Class.Relax) and Wcode(Data.Relax) to suppress this check when needed.

Earlier in this tutorial we have shown code like
OBCode
Person = Data:{S name, var I age, mut S.List friends}
CCode
In that code Wcode(S name) looks like a conventional field declaration, but it is simply syntactic sugar for the following set of methods:

OBCode
Person = Data:{
  read method S name()
  read method I age()
  mut method Void age(I that)
  read method read S.List friends()
  mut method mut S.List #friends()
  }
CCode
Then, Wcode(Data) will discover that Wcode(name), Wcode(age) and Wcode(friends) are good candidate fields and will add factories
OBCode
class method mut This(S name, I age, mut S.List friends)
class method This #immK(S name, I age, S.List friends)
CCode
and a lot of other utility methods.


WTitle((3/5)Nested Trait composition: a great expressive power.)

Composing traits with nested classes allows to merge arbitrarly complex units of code, and solve in a natural way problems that in other languages requires complex patterns like dependency injection, as shown below:

OBCode
TraitGeometryPoint = Trait:{/*Same as before*/}

TraitGeometryRectangle = Trait:{
  Point = {method Point double();} // Declare only the necessary methods
  Rectangle = {
    method Point upLeft()
    method Point downRight()
    method This This(Point upLeft, Point downRight)
    method This double() = \(
      this.upLeft().double()
      this.downRight().double()
      )
    }
  }
...
Geometry2=Class:TraitGeometryPoint:TraitGeometryRectangle
CCode
As you can see, we can define more code using Wcode(Point) while only repeating the needed dependencies.
We will use this idea in the following, more elaborated scenario:
Bob and Alice are doing a videogame. In particular, Alice is doing the code related to loading the game map from a file.

OBCode
Game = { //example game code, NOT MODULARISED
  Item = {interface 
    Point point
    Item hit
    }
  Rock = {[Item]
    Num weight
    class method This(Point point, Num weight)
    method Item hit() = \(point=this.point(),weight=this.weight()/2\)
    }
  Wall = {[Item]
    Num height
    class method This(Point point, Num height)
    method Item hit() = Rock(point=this.point(),weight=..)
    }
  Map = {..//map implementation by Bob
    class method mut This empty() = ..
    read method Item val(Point that) = ..
    mut method Void set(Item that) = ..
    }
  class method Void #$run() = ..this.load(..).. //implemented by Bob
//------------------------------------//Anything under this line is implemented by Alice
  class method Map load(mut FS files, S fileName) = (//Alice writes load(_)
    map = Map.empty()
    ..//read from file and divide in lines,
    for line in lines ( this.load(map=map, line=line) )
    )
  class method Void load(mut Map map, S line) = (
    ..//example line: S"Rock 23 in 12, 7"
    if line.startsWith(S"Rock") (
      map.set(Rock(point=\(x=.., y=..), weight=..))
    if line.startsWith(S"Wall")) ..
    ..
    )
  }
Main = (.. Game.#$run() ..)
CCode

As you can see from the non modularized code above, Alice code is tightly connected with Bob code:
She have to instanstiate Wcode(Map) and all the kinds of Wcode(Item)s. In a language like Java, Alice would need to write her code after Bob have finished writing his, or they would have to agree to use dependency injection and all the related indirections.

Instead, in 42 they could simply factorize their code into two independent traits:

OBCode
TraitBob = Trait:{ //all code is as before, but load is abstract
  Item = {interface 
    Point point
    Item hit
    }
  Rock = {[Item]
    Num weight
    class method This(Point point, Num weight)
    method Item hit() = \(point=this.point(),weight=this.weight()/2\)
    }
  Wall = {[Item]
    Num height
    class method This(Point point, Num height)
    method Item hit() = Rock(point=this.point(),weight=..)
    }
  Map = {..
    class method mut This empty() = ..
    read method Item val(Point that) = ..
    mut method Void set(Item that) = ..
    }
  class method Void #$run() = ..this.load(..)..
  class method Map load(mut FS files, S fileName)
  }

TraitAlice = Trait:{//here we just repeat the used signatures
  Item = {interface }
  Rock = {[Item] class method This(Point point, Num weight)}
  Wall = {[Item] class method This(Point point, Num height)}
  Map  = { class method This empty(),  mut method Void set(Item that)}
  
  class method Map load(mut FS files, S fileName) = (//the actual Alice code
    map = Map.empty()   //is untouched; it can stay completely identical
    ..
    for line in lines ( this.load(map=map, line=line) )
    )
  class method Void load(mut Map map, S line) = (
    ..//example line: S"Rock 23 in 12, 7"
    if line.startsWith(S"Rock") (
      map.set(Rock(point=\(x=.., y=..), weight=..))
    if line.startsWith(S"Wall")) ..
    ..
    )
  }
Game=Class:TraitBob:TraitAlice
Main = (.. Game.#$run() ..)
CCode

Now that the code of alice and bob are separated, they can test their code in isolation:

OBCode
MockAlice = Class:TraitAlice:{
  Item = {interface, S info}
  Rock = {[Item]
    class method This(S info)
    class method This(Point point, Num weight) = 
      \(info=S"Rock: %point -> %weight")
    }
  Wall = ..
  Map = {
    var S info
    class method mut This (S info)
    class method mut This empty() = \(S"")
    mut method Void set(Item i) = this.info(\info++i.info()++S.nl())
    }
  }
TestAlice = (
  files=FS.#$()
  {}:Test"justARock"(
    actual=MockAlice.load(files=files,fileName=S"justARock.txt")
    expected=S"""
      |Rock: Point(5,6) -> 35
      """)
  {}:Test"rockAndWall"(
    actual=MockAlice.load(files=files,fileName=S"rockAndWall.txt")
        expected=S"""
      |Rock: Point(5,6) -> 35
      |Wall: Point(1,2) -> 10
      """)
  ..//more tests here
  )
CCore
//TODO: check the toString would make sense

WTitle((4/5)Typing considerations)

Talk about the impact of non declaring used abstract methods


WTitle((4/5)Traits and rename)

Operator Wcode(:) merges multiple units of code together; but sometime code needs some kind of adaptation before it can be merged

Note that Wcode(traitEnsureTransaction()) is just a normal 
class method that directly returns a library literal.
Traits in 42 are nothing fancier than that.

Now Wcode(MyAction) will execute the operation inside of a transaction.

However, as you can see declaring Wcode(MyAction) using 
Wcode(Refactor.compose) is verbose,
and we need to know the code of Wcode(traitEnsureTransaction())
to use it;
we now show how to improve.
WP

Manually declaring a class just to define a single trait method
returning a library literal is verbose.
In AdamsTowel we can use the class Wcode(Resource)
which automate this process.
WBR
For example: 
OBCode
TraitEnsureTransaction: Resource <>< {
  class method
  Void (mut Db.Connection connection) //method selector here is '(connection)'
  exception Db.Query.Failure 

  class method
  Void (mut Db.Connection that) //method selector here is '(that)'
  exception Db.Query.Failure (/*..as before..*/)
  }

MyAction: Refactor.compose(
  left: TraitEnsureTransaction()
  right: { /*..as before..*/})
CCode

This let us save just a couple of lines. 
We can improve further and make a Wcode(Transaction)
class decorator: 

OBCode
Transaction: {
  InvalidAction: Message.$ <>< {implements MetaGuard}
  //meta guard is the root of all the metaprogramming guards
  class method //using <>< to define the babelfish operator
  Library <>< (Library that) 
  exception InvalidAction {
    i= Introspection(lib: that)
    if !i.hasMethod(\"(connection)") (exception InvalidAction
      "Action method '(connection)' missing")
    composed= Refactor.compose(  left: TraitEnsureTransaction(), right: that  )
    exception on MetaGuard ( InvalidAction
      "Action invalid: type of '(connection)' does not fit or already defined '(that)'")
    return Refactor.HideSelector(\"(connection)") <>< composed
    error on Metaguard
      X"'(connection)' is there, ready to be hidden"
    }  
  }
//So, MyAction becomes shorter and better checked: 
MyAction: Transaction <>< {
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

Now we can use Wcode(Transaction) as a decorator.


WTitle((3/5)Extend)

Wcode(Extend)
 is a decorator implemented using 
Wcode(Refactor) and
Wcode(Introspection)
which provides a flexible model of multiple inheritance with super calls in AdamsTowel.
WBR
As an example, in a game we can have a chest which contains objects in certain positions,
a boat which host humanoids, and
a cargo boat, which host humanoids and contains objects like a chest.
We want to reuse the code of chest and boat to obtain the cargo boat.
WBR 
For example: 
OBCode
ChestTrait: Resource <>< {
  mut Objects objects
  /*.. methods to validate access to objects..*/
  read method
  Kg weight() {
    var Kg res= 0Kg
    with o in this.objects().vals() (res+= o.weight() )
    return res
    }
  }

BoatTrait: Resource <>< {
  mut Humanoids crew
  Kg maxCapacity
  /*.. methods to validate access to crew..*/
  read method
  Kg weight() {/*..with-loop on the crew..*/}
  
  read method
  Kg capacityLeft()
    this.maxCapacity()-this.weight()    
  }

Chest: Data <>< ChestTrait()
Boat: Data <>< BoatTrait()
CargoBoat: Data <>< Extend[ChestTrait();BoatTrait()] <>< {
  read method @override //explained below
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


WTitle((4/5)An intolerant type system)

As an exercise, lets try to use what we learned to add a Wcode(sum()) method to
a vector.

OBCode
Nums: Extends[Collections.vector(of: Num)] <>< {
  read method
  Num sum(){
    var Num res= 0Num
    with n in this.vals() (res+= n )
    return res
    }
  }
CCode

Easy.
However, note that we are calling Wcode(this.vals()) to
do the iteration, and we are not declaring a Wcode(vals())
method.
The idea is that while computing Wcode(Nums), the type system is temporary allowing for incomplete/untypable code at the right of the Wcode(:).
The typesystem will check that all is ok when the declaration of Wcode(Nums) is complete.
WP
However, we have done an extension only on our specific Wcode(Nums) vector, we would have to repeat
such code for each vector.
Can we directly produce vectors that will have a Wcode(sum()) method?
Well, this can only work for vectors of elements with a Wcode(+) operator, and a zero concept. Luckily, all 
numeric classes offer a Wcode(zero()) 
and Wcode(one()) method.
WBR
Building on that, we could attempt the following, invalid solution: 
OBCode
MyCollection: {
  class method
  Library traitSum()
    { //my sum feature
    T: {
      class method T zero()
      method T +(T that)
      }
    read method
    Num sum(){
      var T res= T.zero()
      with n in this.vals() (res+= n ) //error here, vals() undefined
      return res
      }
    }
  class method
  Library vector(class Any of) {
    oldPart= Collections.vector(of: of)
    newPart= Refactor.Redirect(Path"T" to: of) <>< this.traitSum()
    return Refactor.compose(left: oldPart, right: newPart)
    }
CCode

Conceptually, we define a new trait for the sum method,
and we make it general introducing Wcode(T) and our
needed requirements.
Sadly, this is not going to compile, since 
in the method Wcode(sum()) we call Wcode(this.vals()),
and there is no definition for such method.
Similar code worked in the former example, but here
the definition of Wcode(MyCollection) gets completed,
and the code in the method Wcode(traitSum()) is still 
incomplete.
We could just repeat there the definition of Wcode(vals()),
but that would be duplicating code; moreover, Wcode(vals()) returns an iterator, which has methods too...
WP

Wcode(Collection) offers a solution: a trait containing
the minimal code skeleton to make Wcode(vals()) over path 
Wcode(T).
WBR
The idea is that
the composition of Wcode(traitSum()) and
Wcode(Collections.traitValsT()) is complete code.
However, even declaring Wcode(traitSum()) as
OBCode
class method
Library traitSum() 
  Extend[Collections.traitValsT()] <>< {/*my sum feature as before*/}
CCode

whould not work: the Wcode(<><) method would
be called when Wcode(traitSum()) runs, leaving incomplete code in the resulting library literal.
We need to force the computation to happen before
Wcode(MyColleciton) is completed.
A solution is to use Wcode(Resource).

OBCode
TraitSum: Resource <>< Extend[Collections.traitValsT()] <>< {/*my sum feature as before*/}
MyCollection: {
  class method
  Library vector(class Any of) (
    oldPart= Collections.vector(of: of) //surely works
    {newPart= Refactor.Redirect(Path"T" to: of) <>< TraitSum()
    return Extend[oldPart] <>< newPart
    catch exception MetaGuard g return oldPart
    })
CCode

By the way, earlier we also forgot to handle exceptions!
If our parameter does not support zero and plus,
we will just return a normal collection. We need to insert additional brackets otherwise the 
binding Wcode(oldPart) would not be visible in the catch body.

As you may notice there is some incoherence in our programming style: 
should traits be methods in a class or Resources?
should we use 
the more primitive
Wcode(Refactor.compose(left,right))
or the more flexible Wcode(Extend[] <><)?
In the current state of the art we do not have an answer for what is the best in 42.
WBR
Indeed, we still do not understand the question.







WTitle((1/5)Refactor and Introspection)

WTitle(Refactor)
Wcode(Refactor) is a class supporting modification of
library literals.
For example, you may want to rename the method Wcode(importStructure(that)) into just Wcode(import(that)).
You can do the following: 
OBCode
{reuse L42.is/AdamsTowel
Db: Refactor.RenameSelector(
  Selector"importStructure(that)" to: Selector"import(that)"
  ) <>< Load <>< {reuse L42.is/Db}
UnivDb: Db.import(Db.ConnectionS"...")
/*..*/
}
CCode
The type Wcode(Selector) represent method selectors;
in the same way the type Wcode(Path) represent 
paths inside library literals, as in Wcode(Path"MyNested.MyNestedNested") or
Wcode(Path"This").

There are a lot of refactoring operations nested under Wcode(Refactor): 
<ul><li>
Wcode(RenameSelector)
and 
Wcode(RenamePath)
rename methods either at top level (as we just did) or
in an arbitrary nested library;
or rename paths into other paths
</li><li>

Wcode(Redirect)
removes a nested library and redirects all its references to
an external one. This emulates generics, as we will see later.
</li><li>
Wcode(UpdateDocumentationSelector)
and Wcode(UpdateDocumentationPath)
add to, alter or delete the documentation of methods/paths.
</li><li>
Wcode(MakeAbstractSelector)
and Wcode(MakeAbstractPath)
remove all the implementation out of a method or path,
leaving only the public skeleton
</li><li>
Wcode(HideSelector)
and Wcode(HidePath)
mark methods or paths as private.
We have not seen details on private members, the main idea is that
they are renamed into invisible names that you can never guess, and automatically renamed to avoid collisions 
by refactoring operations.
</li></ul>
WP
In addition to all those nested classes,
Wcode(Refactor) offers Wcode(`Refactor.compose(left,right)')
allowing a simmetric sum of two library literals.
The main idea is that members with the same name are recursively composed

WTitle(Introspection)
Wcode(Introspection) is
a class for exploring libraries, to discover what methods they have and so on.

The main classes inside of Introspection are
Wcode(Introspection.NestedLibrary),
 Wcode(Introspection.Method) and
Wcode(Introspection.Type).
You can obtain a nested library by calling the factory methods 
Wcode(Introspection(lib)) and Wcode(Introspection(classObj)),
respectively for library literals or class objects.
We will see some example later of use of Wcode(Introspection).



WTitle((5/5)Metaprogramming summary)
<ul><li>
Metaprogramming is hard; 42 tries to make it simpler, but not trivial.
</li><li>
Error handling is important while writing decorators.
More then half of decorators code should be dedicated
to handling errors and lifting them into a more understandable
form, for the sake of the final user.
</li><li>
We are just scratching the surface of what we
can do with metaprogramming.
If you are interested in becoming a Magrathean, then
refer to the painful metaprogramming guide (link);
otherwise just use existing metaprogramming libraries 
and use Wcode(Refactor) only when all the other options feel more painful.
</li></ul>


