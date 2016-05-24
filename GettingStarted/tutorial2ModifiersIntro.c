WBigTitle(`Keep control:  Modifiers, kinds of classes/references/objects')

WTitle(`(1/5)Kinds of classes')
Wcode(Point) is an WEmph(Immutable class: ) 
 none of its fields can be WEmph(updated) 
or WEmph(mutated.)
Immutable classes are very easy to use but may be inadequate when representing real objects, whose state can changes across time.
A WEmph(Mutable class) is a class where the state of (some of) its instances may be mutated.

Let's now declare a Wcode(Animal) mutable class, whose location can be updated.
OBCode
Animal: Data <>< {
  var Point location

  mut method
  Void run() 
    this.location(this.location().add(x: 20Num))
  }
CCode
There are two new keywords used here: 
<ul><li>
the Wcode(location) field is declared Wcode(var).
  This is called a variable field, and can be WEmph(updated) by calling a setter.
  Non variable fields can not be updated
</li><li>
  the modifier Wcode(mut) in front of the method. 
  We have seen Wcode(class) already, an we have seen methods with the default modifier ( 
Wcode(add(x)) and
Wcode(add(y)) ).
  Wcode(mut) methods can mutate the "this" object. If you have experience with C++
  you can see the relationship with Wcode(const) methods.
  immutable (default) methods works only on immutable "this" objects. We will see later much more about modifiers
</li></ul>

WP
As you see, we are using the Wcode(add) method from before.
Also notice that we are using a setter here, where we are providing the first parameter without the argument name.
While this is usual in other languages, in 42 parameters are selected by name.
However, for some methods with a single parameter, writing down the parameter name would not improve the readability and just add noise.
In those cases, the first parameter is conventionally called Wcode(that), 
and writing Wcode(a.b(that: c))
is equivalent to writing Wcode(a.b(c)).
This works also for methods with multiple parameters, if the first one is called Wcode(that).
We can use an animal by writing, for example: 

OBCode
  mut Animal dog1=Animal(location: Point(x: 0Num, y: 0Num))
  dog2=Animal(location: Point(x: 0Num, y: 0Num)) //type 'mut Animal' inferred
  dog1.run()
CCode

WTitle(`(2/5)Interaction between mutable and immutable')

We now explore some interaction between mutable and immutable objects.
OBCode
Animal: Data <>< {
  var Point location
  mut Points path
  mut method
  Void move() 
    this.location(path.left()))
    this.#path().removeLeft()
  }
CCode


Here we use Wcode(mut Points path) to denote a mutable list of points. Note the absence of Wcode(var); this is conceptually similar to a Wcode(Points * const path;) in C++  or Wcode(final Points path;) in Java.
To contrast, the declaration Wcode(var Point location) is similar to
Wcode(Point const * location;) in C++  or Wcode(ImmPoint location;) in Java (for an opportune Wcode(ImmPoint) class).
That is, 
mutable objects can be referred using mutable references,
Immutable objects can be referred using immutable references.
WP
The method Wcode(move)
first use the Wcode(location(that)) setter method to update the Wcode(location) field,
then uses the Wcode(`#path()')
 WEmph(exposer) method and 
the Wcode(removeLeft()) method to mutate the list of points.
Both exposers and getters provide access to the value of a field;
Exposer are used to access the value of mutable fields.
Exposers should be used with care: taking access to parts of a mutable 
state of an object could cause 
spooky action at a distance effects by aliasing.

In general, methods starting with `#' should be used with care.
WP
This code models an animal following a path. It can be used like this.
OBCode
zero=Point(x: 0Num, y: 0Num)
ps1=Points[ Point(x: 12Num, y: 20Num);Point(x: 1Num, y: 2Num)]
ps2=Points[ zero;Point(x: 1Num, y: 2Num)]
dog1=Animal(location: zero, path: ps1)
dog2=Animal(location: zero, path: ps2)
dog1.move()
dog2.move()
CCode

In this code the first dog goes to 12: 20.
The second dog goes to 0: 0. 

This code involves a mutable animal with a mutable field. This is often
a terrible idea, since its behaviour may depend on aliasing:  what happens if two dogs follow the same path?
OBCode
zero=Point(x: 0Num, y: 0Num)
ps=Points[ Point(x: 12Num, y: 20Num);Point(x: 1Num, y: 2Num)]
dog1=Animal(location: zero, path: ps)
dog2=Animal(location: zero, path: ps)
dog1.move()
dog2.move()
CCode
The first dog moves and consumes the path for the second one as well.
That is, the first goes to 12: 20 and the second goes to 1: 2.

This is because Wcode(Animal) is now a WEmph(Deeply mutable class: )  a mutable class with mutable fields. 
An amazing amount of bugs are caused by the usage of deeply mutable classes.

Note how we are using the exposer Wcode(`#path()')
in a safe pattern: only called over the Wcode(this) receiver, and the reference does not leak out of the method.
The problem here arise since the object was shared to begin with. 

WTitle(`(3/5)Capsules:  Keep aliasing graphs untangled')

This tricky behaviour is correct for a 
deeply mutable class. 
In 42 we can change Wcode(Animal) to prevent this aliasing issue.
OBCode
Animal: Data <>< {
  var Point location
  capsule Points path
  mut method
  Void move() 
    this.location(path.left()))
    this.#path().removeLeft()
  }
CCode
Now we use the modifier Wcode(capsule), this requires the field to be encapsulated with respect to aliasing.
Immutable objects do not influence aliasing, so they are free from aliasing limitations.

WP
The Wcode(capsule)
 modifier WEmph(forces) the users to provide well encapsulated values,
 and WEmph(ensure) 
 the Wcode(Animal) data is well encapsulated.
WP

Now the code from before would not compile. However we can still write the following variant
OBCode
zero=Point(x: 0Num, y: 0Num)
capsule Points ps=Points[ Point(x: 12Num, y: 20Num);Point(x: 1Num, y: 2Num)]
dog1=Animal(location: zero, path: ps)
dog2=Animal(location: zero, path: Points[ Point(x: 12Num, y: 20Num);Point(x: 1Num, y: 2Num)])
dog1.move()
dog2.move()
CCode
Where the Wcode(ps) local binding is declared capsule; 
it can satisfy the Animal.path requirement, but it can be used only once.
Wcode(dog2)
has to use another capsule. It is okay to just write the object creation in place as is done.
Alternatively, most classes offer a Wcode(clone()) method,
so in this case we could write

Wcode(dog2=Animal(location: zero, path: dog1.path().clone()))


WTitle(`(4/5)Handle mutability')

WTitle(`Immutable objects of Mutable classes')

How can we get an immutable Wcode(Animal)?
When an Wcode(Animal) is created using Wcode(Animal(location: __,path: __)) we create a Wcode(mut Animal).

In most cases you can promote such reference to immutable/capsule; just make the type of the local binding explicit.
 The type system will take care of the rest.
If a reference can not be safely promoted to immutable/capsule, you may have to use the Wcode(.clone()) method or to refactor your code.
OBCode
mut Animal dog1=Animal(__) //no promotion here
Animal dog2=Animal(__) //promotion mutable->immutable
dog1.move()
 //dog2.move()  //ill-typed, requires a mut Animal
CCode

We will explain later the exact rules for promotion,
the main idea is that if the initialization expression uses local bindings in a controlled/safe way, then promotion can be applied.
For example, a mutable expression using only capsule or immutable references can be promoted to capsule or immutable, as we prefer.

WTitle(`Exposers and getters: mutable, lent and read')

As we had seen before, exposers are needed to 
access a mutable reference for a mutable field.
In case of capsule fields, the exposer will provide a Wcode(lent) reference; that is: 
a hygienic mutable reference allowing mutation but not long term storage.

Lent references are a supertype of mutable references and can not be stored in mutable/capsule/immutable fields.
A single lent reference in a subexpression can be temporary promoted to a mutable references.
for example, when doing 
Wcode(`this.#path().removeLeft()')
the reference produced by Wcode(`this.#path()') is promoted to mutable in order to call
the mutable method Wcode(removeLeft()).

When access to a mutable reference is not needed, one can use
a normal getter also for mutable fields.
In this case, a Wcode(read) reference is provided.

In general, we can 
use Wcode(read) when we not care about the mutability of an object.
For example, we could add to Wcode(Animal)

OBCode
read method
Bool hasArrived()
  this.path().isEmpty()
CCode
This method can be called to mutable and immutable animals: 

OBCode
Debug(dog1.hasArrived())
Debug(dog2.hasArrived())
CCode

WTitle(`(5/5) Summary')
  
WTitle(`Kinds of classes, summary')
  
<ul>
<li>
immutable classes:  have only immutable fields.
It is useful to model mathematical concepts.
It is easy to reason about code using immutable classes,
but some properties of real objects can be better modelled with state mutation.
</li><li>
shallow mutable classes:  have only (variable) fields of immutable or capsule type (or class, as we will see later). 
Reasoning with shallow mutable classes is near as easy as reasoning with immutable ones, and often more natural.
</li><li>
deep mutable classes:  have mutable fields.
Reasoning with deep mutable classes can be very hard.
</li></ul>
  
WTitle(`Modifiers: summary')

<ul>
<li>
immutable:  the default. When you omit the modifier,
 you mean immutable. 
An immutable reference points to an object that is never changing. Its whole reachable object graph never changes and is immutable as well.

</li><li>
mutable:  A mutable reference behaves like a normal reference in Java, C#, C++ , Python and many other languages.
Mutable references require mutable objects and allow mutating the referred object.
</li><li>
capsule:  capsule references are used only once and they guarantee that the whole reachable object graph is reachable only thought that
capsule reference. 
Capsule references provide a structured way to reason over deep mutable objects.

Fields can be annotated capsule, the meaning is that they need to be initialized/updated with capsule variables.
We will discuss more about capsule fields and how they differs from capsule references later.
 
</li><li>
read:  A readable reference can not be used to mutate the referred object; but other mutable references pointing to the same object can mutate it.
Read references can point to both mutable and immutable objects.
It is easy to be confused between read and immutable references.
As a rule of thumb, if you are in doubt about whether to use an immutable or a readable reference,
you probably want an immutable reference.

</li><li>
lent:  a hygienic mutable reference allowing mutation but not storage.
Lent and read are useful to handle in controlled way the state of deep mutable classes;
moreover using lent and read on method parameters 
allows to make explicit what are the method intentions and requirements.
</li><li>

class:  class references denote the class object,
  on methods the meaning is the same of static methods in many languages, but it can consistently be used on parameters/local variables/fields
to encode behaviours similar to dependency injection.


</li></ul>



WTitle(`Kinds of objects, summary')

<ul>
<li>
immutable: immutable objects 
 can be instances of immutable classes, or promoted instances of mutable classes.
 They
can be referred to only by immutable and read references.
</li><li>
mutable:  mutable objects are instances of mutable classes.
They can be referred to by capsule, mutable, lent and read references.
</li><li>
class:  class objects can be accessed from anywhere by using the corresponding class name;
It is also possible to 
store them into local binding, but they can be referred to only by class references,
either of their class or any of the 
transitively implemented interfaces.
Some programmers found the fact that class objects are instances of themselves deeply concerning
or disturbing, while for others it is just a good story to tell to break the ice at parties.
</li></ul>
