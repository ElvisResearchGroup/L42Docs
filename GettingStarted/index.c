#include "header.h"
WTitle(Why 42)
42 is a new programming language,
the name is inspired by The Hitchhiker's Guide to the Galaxy.
The language goal is to allow the transparent cooperation and composition of millions of libraries at the same time.
WP
In 42 you will be able to import libraries just by naming them and to
compose them in your code without worrying of their dependencies.
Normally libraries are big monolitic things, coming from outside and manually imported/installed. The user have little control on
the library code, and the library have no influence on the importing context.
WP
In 42 Libraries are usually much smaller and are first class entities, that can be manipulated as values,loaded, stored, modified, adapted, saved or simply used. You will write code that manipulate libraries, but more importantly you will use libraries that manipulate libraries in non trivial ways.
Indeed in 42 manipulating libraries is so natural that is convenient to think of your own code as a library that you offer to yourself only.
WP
42 enforce a high level of abstraction, thus there is no access to primitive
structures like arrays (just use collections), and there is no concept of null or default initialization.
While is possible to write in a fully functional style in 42, usually 42 programs mix WEmph(mutable datastructure with aliasing)  and WEmph(deeply immutable data). Indeed 42 offers powerful support for aliasing control and immutability, helping in mixing the two stiles in an easy and natural way.

WP
42 offers (well, will offer) automatic parallelism: thanks to immutability and aliasing control, many patterns of normal sequential code will be safely be executed in parallel.

WTitle(Object orientation)
42 is a pure object oriented language,
where every value is an object. This means that:
<ul>
<li>
Numbers are objects, 
so thay can have methods like Wcode(.sqrt()) or Wcode(.abs()). 
</li><li>
All operators are just method calls, thus Wcode(a + b) is sugar for Wcode(a.operator+(b)), and so on. Note how Wcode(operator+) is a valid method name.
Any class that offers that method supports the operator.

</li><li>
Classes are objects, when you write Wcode(Foo.bar()) you refer to the method Wcode(.bar()) offered by the object denoted by Wcode(Foo).
class objects are just objects, and you can also store them in local bindings if you wish, as in Wcode(x=Foo)
</li><li>
The code is an object, but only at the granularity of 
Libraries, that is, balanced pairs of curly brackets representing classes (or interfaces) with methods and nesteds.
This is usefull for meta-programming, as we will see later this is the most important feature of 42.
<!--OCode Library myCode={ method Int foo()} CCode
A local binding of type Library initialized with a class with a single Wcode(foo()) method. Since there is no body, it is an abstract method.
 -->

</li><li>
Differently from other pure object oriented languages, in 42
all objects have the same treatment, both syntactically and semantically.
For example Wcode(x = S"Hello " ++ S"World")
This is a declaration for a local binding Wcode(x), using the string class Wcode(S) and the operator Wcode(++),
used as string (or in general sequence/collection) concatenation.

Coming from another programming language, you many be surprised that we have to write Wcode(S) before Wcode("Hello ") and Wcode("World").
This extra verbosity is need to provide fair treatment to all classes. (Wlink(note1,see more))
In 42 we do not give preferential treatment
to special object/classes. In this sense, we consider
most other languages to be racist.
They give priority to their "preferred" version of numbers and strings, and this puts into a position of unfair disadvantage library code trying to define its own numbers kinds.
Instead in 42 you may encounter strings like Wcode(URL"www.google.com"),
 Wcode(EMAIL"Artur.Dent@gmail.com") or multiline strings like
 OCode Javascript"
'x=function(){
'  alert("hello world";)
'  }
' x()
"
CCode

The same for numeric classes: to talk about a street, 20 meters long, you would write
Wcode(streetLenght=20Meter).
Note that we write the class name after the number, both for better readability and because Wcode(Meter20) would be consider an identifier by the parser.
You may encounter Wcode(Int), Wcode(UInt), Wcode(Float), Wcode(Meter), Wcode(Kg), Wcode(Second), Wcode(Year), Wcode(PhoneNumber) and many other numeric classes.
</li></ul>

<!--WTitle(Simpler complete program)

Let now starts showing the simplest 42 program: an empty library.

OCode {} CCode

If we save this valid program in a file Wcode(Test.L42) and we run Wcode(L42 Test), we get an error.
WP
As you see 42 is very intuitive, as you would expect from your former life experiences, most simple things just does not work.
Note how valid programs can produce errors.
We will soon learn how to produce errors in controlled and elegant ways.
-->

WTitle(Simple hello world program)
Let show now a simple hello world program:
OCode 
{reuse L42.is/AdamTowel
Main:{
  Debug(S"Hello world")
  return ExitCode.success()
  }
}
CCode
At the right of Wcode(Main:) we write the expression/statements that
we wish to execute. Wcode(Main) is not a method, and Wcode(Main) is not special name either. In 42 there is no concept of main method as in
Java or C. For now you can think of Wcode(Main:) as a top level command, we will understand later how this fits with the general language design.
<!--A 42 program execution WEmph(is) the generation of all its nested classes/interface.
, code is simply executed from top to bottom as in 
Python, Javascript or Php. However, the
top level expression is a Library, and code can go in libraries 
as an initializer for you need to put the code into an expression
-->
When we write Wcode(reuse L42.is/AdamTowel) we are asking 42 to
reuse the code offered by the library offered in the internet address 
Wcode(L42.is/AdamTowel).
AdamTowel is our Towel, that is the set of classes and interfaces that we wish to start from (Wlink(towel,see more on towels)).
Wcode(L42.is) is the main website of 42, where most commonly used libraries are hosted. To reuse code you need internet connection; but this also means that you will never have to manually import any code. Required code will be transitivelly downloaded and cached on your machine, you need not to be aware of the existence of this mechanism.
WP
We are not supposed to always start from AdamTowel, there are many interesting towels out there, and you may also become skilled in the 
advance technique of towel embroidery.
WP
Lets now declare a method and call it.
OCode
{reuse L42.is/AdamTowel
MyCode:{
  class method
  S hello(S name){
    return S"hello "++name
    } 
  }
Main:{
  Debug(MyCode.hello(name=S"world"))
  return ExitCode.success()
  }
}
CCode
Here we declare a class to host our Wcode(hello(name)) method.
We write Wcode(class method) to declare a method that can be called on the class object, as in Wcode(MyCode.hello(name=S"world")). This is equivalent to a static method in languages like Java or C++.
WP
Per Andre: MyCode.hello usa la classe, MyCode e' il receiver, MyCode e' un oggetto,
e se vuoi puoi anche salvarlo su un binding locale, tipo x=MyCode  x.hello(...)
WP
Note how the method is called using the parameter name explicitly.
We believe this increase readability.
WP
You can also notice how there are two different usages for curly brackets: if there is at least a return keyword it is a set of statements,
otherwise is a library literal, and can contains methods and nesteds.
A nested (class or interface) is denoted by an upper-case name, and can be obtained by a library literal or an expression producing a library literal.
A library can be a class (default case) or an interface (starts with the Wcode(interface) keyword).
A nested in 42 is similar to a static inner class in Java, or a nested class in C++. It is just a convenient way to divide in a tree shape the various components of our program.
WP
The class Wcode(MyCode) of before offers a single class method, have no fields and you can not create instances of Wcode(MyCode), since no factory is present; you can think about it as a module.
In 42 we do not have constructors. Objects are created by factory methods. We believe this is a much simpler and consistent approach to object initialization.

Let create now a class with state and factory:
OCode
Point:Data<<{
  Int x
  Int y
  method
  Point add(Int x)
    Point(x=x+this.x(), y=this.y())
  Point add(Int y)
    this.with(y=y+this.y())
  }
CCode
Here you can see we declare a Wcode(Point) class with Wcode(x) and Wcode(y) Wcode(Int) coordinates.
Wcode(Data) is a decorator. Decorators are classes/objects that offers an operator Wcode(<<), called the babel fish operator,
whose goal is to translate a library into a "better" library.
In this case, Wcode(Data) is translating the class Wcode({Int x COMMA Int y}) into a much longer class, with
a factory method taking in input the fields and initializing them, but also containing
 boring but useful definitions for
equality, inequality, conversions from and into human readable strings, xml and binary representations for (de)serialization.
WP
Finally, we declare methods to add to a single coordinate.
For very short methods we can omit the curly brackets and return.
Indeed method bodies are just expressions, and the curly brackets turn statements into expressions. 

In the method Wcode(add(x)) we show a constructor call and getters.
In the method Wcode(add(y)) we show an improved version, using the with method, another gift of Data, that allows us to easily create a clone with a
single field updated.
We can declare two methods, Wcode(add(x)) and Wcode(add(y)) with the same name, if parameter names are different.

WTitle(Decorators)
Decorators are one of the main concepts used by 42 programmers. We will encounter many decorators in this tutorial.
For now, just get used to the pattern of writing
Wcode(<<) to go from a minimal chunk of code, with method declarations for the important bits, to a fully fledged usable class.
Wlink(Decorators, More on decorators)

WTitle(Kinds of classes)
Wcode(Point) is a WEmph(Immutable class): none of its field can be updated or mutated.
Immutable classes are very easy to use but may be inadequate when representing real objects, whose state can mutate across time.
A WEmph(Mutable class) is a class where the state of its instances may be mutated.

Lets now declare a Wcode(Animal) mutable class, whose location can be updated.
OCode
Animal:Data<<{
  var Point location
  mut method
  Void run() 
    this.location(this.location().add(x=20Int))
  }
CCode
As you see, we are using the add method from before.
You can also notice that we are using a setter here, and that we are providing the first parameter without the argument name.
This is accepted when writing down such name would not improve the readability, and in those cases the first parameter is conventionally called Wcode(that).
More important, there are two new keyword:
<ul><li>
the location field is declared var.
  This is called a variable field, and can be updated by calling a setter.
  Non variable fields can not be updated
</li><li>
  the modifier "mut" in front of the method. 
  We have seen "class" already, an we have seen methods with the default modifier ( add(x) and add(y) ).
  mut methods can mutate the "this" object. If you have experience with C++ you can see the similarity with const methods.
  immutable (default) methods works only on immutable "this" objects. We will see later much more about modifiers
</li></ul>

WTitle(Interaction between mutable and immutable)

We now explore some interaction between mutable and immutable objects.
OCode
Animal:Data<<{
  var Point location
  mut Points path
  mut method
  Void move() 
    this.location(path.first()))
    path.removeFirst()
  }
CCode
This code models an animal following a path. It can be used like this.
This code involve a mutable animal with a mutable field. This is often
a terrible idea, since its behaviour may depend on aliasing: what happens with 2 dogs?
OCode
  zero=Point(x=0Int, y=0Int)
  ps=Points[ Point(x=12Int, y=20Int);Point(x=1Int, y=2Int)]
  dog1=Animal(location=zero, path=ps)
  dog2=Animal(location=zero, path=ps)
  dog1.move()
  dog2.move()
CCode
The first dog moves and consumes the path also for the second one.
This is because Wcode(Animal) is now a WEmph(Deeply mutable class): a mutable class with mutable fields. 
An amazing amount of bugs is caused by the usage of deeply mutable classes.
 If this behaviour is not what you expected, we have to change Wcode(Animal) to prevent this aliasing issue.
OCode
Animal:Data<<{
  var Point location
  capsule Points path
  mut method
  Void move() 
    this.location(path.first()))
    path.removeFirst()
  }
CCode
Now we use the modifier "capsule", this requires the field to be encapsulated with respect to aliasing.
Immutable objects do not influence aliasing, so they are free from aliasing limitations.
Now the code of before would not compile. However we can still write the following variant
OCode
  zero=Point(x=0Int, y=0Int)
  capsule Points ps=Points[ Point(x=12Int, y=20Int);Point(x=1Int, y=2Int)]
  dog1=Animal(location=zero, path=ps)
  dog2=Animal(location=zero, path=Points[ Point(x=12Int, y=20Int);Point(x=1Int, y=2Int)])
  dog1.move()
  dog2.move()
CCode
Where the Wcode(ps) local binding is declared capsule, thus it can satisfy the Animal.path requirement, but it can be used only onece.
dog2 have to use another capsule. It is ok to just write the object creation in place as is done.

WTitle(Modifiers COMMA recall)
We have now seen many different modifiers, lets recall them:
immutable: the default, when you omit the modifier, you mean immutable. 
An immutable reference points to an object that is never changing. Its whole reachable object graph never change and is immutable as well.
mutable: A mutable reference...
capsule: capsule references are used only once and they guaranteed that the whole reachable object graph is reachable only thought that
capsule reference.
Fields can be annotate capsule, the meaning is that they need to be initialized/updated with capsule variables. 
class: class references denote the class object,
  on methods the meaning is the same of static methods in many languages, but it can consistently be used on parameters/local variables/fields
to encode behaviours similar to dependency injection.
We still need to see lent and read modifiers:
lent: a hygienic mutable reference (more on lent later).
read: common supertype between immutable and mutable (and lent).
Use read when you do not care what is the mutability of an object.
For example, we could add to Animal
OCode
read method
Bool isArrived()
  this.path().isEmpty()
CCode
This method can be called to mutable and immutable animals.
Wait.. how can we get an immutable Animal? just make the type of the local binding explicit. The type system will take care of the rest.
If a reference can not be safely promoted to immutable/capsule, you may have to use the .clone() method.
OCode
  mut Animal dog1=Animal(__)
  Animal dog2=Animal(__)
  dog1.move()
  Debug(dog1.isArrived())
  //dog2.move()
  Debug(dog2.isArrived())
CCode

WTitle(Object creation COMMA recall)

We have also seen many different syntactic forms to create objects: lets recall them:
12Int: from a numeric representation
S"foo": from a string representation
Point(x=_,y=_): from the parameter values
Points[_;_;_]: from a variable length sequence of values.

Note that in 42 those are all just expressions, and represents methods in the named class.
Sometime is convenient to reuse this kind of syntax to get better syntactic support for certain operations:

Wlink(S, To understand it better: a guide to the S class)

WTitle(Libraries)
42 stands for the primate of libraries, so let see some libraries in action.
We have already see how to chose a towel, and many classes that are likely to be present in such towel, like
Int and Data.
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
OCode
{reuse L42.is/AdamTowel
Collections: Load<<{reuse L42.is/Collections}
Point:Data<<{Int x, Int y}
Points:Collections.vector(of:Point)
Main:{
  ps=Points[ Point(x=12Int, y=20Int);Point(x=1Int, y=2Int);]
  var tot=0
  with p in ps (tot+=p.x()+p.y())
  return ExitCode.success()
  }
}
CCode
A very large class of practically useful programs can be obtained by just declaring smart structs using Data and Collections.

	 

#include "footer.h"	 