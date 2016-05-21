WBigTitle(Guarantees and Philosophy)

WTitle((1/5)Language invariants)
42 guarantee a set of language invariants: properties that are always true

WTitle(Immutability)
In 42, once an object become immutable,
it will never change again.
It's whole reachable object graph is frozen forever.
This is what in some languages is called value semantic.

WTitle(Encapsulation)
A capsule binding (not a capsule field)
is encapsulated, that is:
such capsule binding is the only way to reach
that object and its whole (mutable) object graph.
It is irrelevant if immutable leaves are shared or not.


WTitle(Hygienic aliasing)
If some references points to disjoint object graphs (modulo immutable leaves)
then after any operation that takes in input one or zero mutable references,
those original references will still point to disjoint object graphs (modulo immutable leaves).
That is:
<ul><li>
capsule references can be used only once, so they will not be available any more,
so it is irrelevant what they referred to;
</li><li>
class and immutable references  are irrelevant since it can not be observed if two references are the same object or
an identical clone;
</li><li>
read and lent references can not be stored inside of other object graphs.
Lent view point adaptation is designed to carefully preserve 
Hygienic aliasing while allowing lent reference to do mutation by promoting them to mutable in a controlled scope.
</li></ul>

WTitle(Strong error safety)
If an error is captured, the catch body will observe the same state it was present
at the start of its guarded block.
That is, operations guarded by a catch error can not modify externally visible state.
WTitle(Checked exceptions)
It is always statically known what exceptions every piece of code can raise.
As for Java, this is obtained by declaring exceptions on the method signature.
However, 42 offers convenient syntactic sugar to turn exceptions into errors/other exceptions.
WTitle(Subtyping control)
In 42 all class types are exact, so if a method takes a Wcode(Point), and Wcode(Point) is not
an interface, that method is always going to receive exactly a point. 
If Wcode(Point) is an interface, you know is going to be some class implementing that interface.
Subtyping control also implies conventional
avoidance of method not understood.
WTitle(MetaSafety)
In 42, nested library declarations that do not contains library literals,
as for example
Wcode(`Nums:Collections.vector(of:Num)') are
guaranteed to produce well typed code. This does not happens in most other meta-programming approaches.
nested library declarations containing library literal,
as for example
Wcode(`Point:Data<<{Num x, Num y, method This add(S x) This.with(x:/+x)}') can produce
non well typed code, if such type error was present in such contained library literals.
In the case of our example, we mistakenly used the Wcode(S) type instead of 
Wcode(Num); this will cause a type error in the result, that can be traced back to a type error
in the initial library literal.


WTitle(Object always fully initialized)
In 42 all object are born with all the fields containing objects of the right types, and this will hold for the whole lifespan of the object.
There is no Wcode(null) and no uninitialized values.
To allow for the initialization of circular object graph 
you can use a feature called Wcode(fwd) references, that we have not explored in this tutorial.
See (link)


WTitle((2/5)Philosopy of L42 and AdamsTowel)
Other languages have a weak division between language features
and their standard library.
42 have a very strong separation between language and libraries.
You can see that from the mentality of the language and
the (different) mentality of the popular Wcode(AdamTowel).

WTitle(Philosopy of L42)
<ul><li>
42 is just an instrument; use it as best as you can,
to do what you prefer.
</li><li>
No idea is too crazy for 42, 
no matter what you do, you can not break the language
invariants; so play hard with it, It can take it!
</li><li>
Do not let other people or libraries 
tell you what to prefer/avoid.
</li><li>
There is no intrinsic meaning in 42, no language feature
is designed to be used only in a certain way.
</li><li>
42 is based on a minimal core composed by little more than
method calls and exception handling.
Then there is a thick layer of syntactic sugar allowing 
for more convenient syntax.

Please,experiment with the fixed but flexible syntactic sugar of
42 and find new idiomatic ways to mesh 42 into expressing what you want in the
way you want it.
</li></ul>


WTitle(Philosopy of AdamTowel)

<ul><li>
AdamTowel offers a large set of simple concepts, that you can use
to encode the domain of your problem.
<BR/>
Basic classes should represent atoms of knowledge,
<BR/>
Collections should represent homogeneous groups of objects,
where every object serve the same role in your domain.
<BR/>
Data classes are agglomerates of other classes with an invariant.
<BR/>
Modules just have class methods and are a simple way to organize your code.
<BR/>
Resources serve to indicate constants.
<BR/>
Algorithmic classes will implement a certain algorithm interface
and will perform their tasks polymorphically.
<BR/>
Messages will report errors and exceptions.
<BR/>
Decorators will complete your code from,
 adding the needed boilerplate.
</li><li>
AdamTowel can be embroidered to create many variations,
and those variations can all play together with little effort/discipline.
</li><li>
You can use modifiers in a disciplined way to express meaning:
<ul><li>
Immutable references are abstract/mathematical concepts, there are used to model the world 
of your program but are not materialized in your world.
For example, a Wcode(Car) has a Wcode(Kg) weight, but Wcode(25Kg) is not an actual
thing in the world of cars.
</li><li>
Class objects models kinds of things, and you can use
class methods as convenience way to refer to general concepts not specially connected to any object
in the domain of your program.
A special role is covered by
class methods in interfaces:
In this way you can express behaviour that is parametric on the kind of objects, without
resorting to metaprogramming, in a pattern similar to dependency injection, see (link).
This will require using references to class objects instead of just naming them by path.
<!--For example if the Wcode(WebServer) interface 
has a Wcode(class method This connect(S connectionString))
then you can write code taking in input
-->
Another point where we need to use class references is metaprogramming; for example in Wcode(Refactor.Redirect).

</li><li>
Mutable reference will point to mutable objects;
shallow mutable objects can be the main protagonist in your world.
While everything sort of turn around them, must functions will just work on their
properties, often immutable objects.
Deep mutable objects need to be handled with more care.
</li><li>
Lent reference/parameters indicate the desire of a method to be hygienic with
a chunk of data.
If a method have only one mutable parameter including the method modifier,
we suggest to  not making it lent, nothing would change and it would only look more involved.
If a method have more than one mutable parameter, then if possible keep the 
method modifier mutable and make the other parameters lent.
The only case where it is reasonable to have a lent method is when there are at least two 
other mutable parameters, and the method is using the information inside of the receiver to decide how to mix
their reachable graphs.
</li><li>
Readable references/parameters indicate the desire of a method to just read 
the content of a chunk of data, without storing it or mutating it.
If possible, make methods and methods parameters read if they can not be immutable.
</li></ul>
</li></ul>


WTitle(`(3/5) Reconciling opposite views')
How can the philosophy of AdamTowel be so different with respect of
the philosophy of 42?
Actually, AdamTowel is following all the suggestions of 42:

It is using 42 as an instrument, as best as it can.
By checking for specific usage pattens it can enforce 
class invariants by building on the language invariants.
Certain refactoring operations performs crazy complicated 
operations, but they can be abstracted as a simple high level concept.

AdamTowel goes its long way to support units of measure and alphanumerics, even if 
most other standard libraries would avoid going that way.

Strings in AdamTowel use the square bracket Wcode([]) syntax to do 
string interpolation, instead of just sequence building,
Wcode(Enumeration) use the string literal postfix operator to generate enumeration 
classes.
Who know what new and creative ways to use the 42 syntax could be used by another, more mature towel.



WTitle(`(4/5) Embrace failure')
We love programs that fails.
We love more the ones that fails early and with good error messages.
We love static type system, it allows us to fail very fast.
We love to have code analysis, it makes our code fail quite fast and reliably. 
We love to check for additional constraints at run time.
The important think is that a code that behave differently on how we expected should
never be allowed to produce a result.

This mindset is different with respect to the one found in many other language communities.
WBR
For example languages supporting flexible silent automatic conversions between different 
datatypes (as string, bools and numbers) are clearly searching for a possible
way to interpret a possible confusing programmer request and give it a meaning.
For example Wcode("2"+2) may mean Wcode("22") while Wcode(2+"2") may mean Wcode(4).
Reasonable interpretations, possibly motivated by certain examples, but they can not possibly scale to
the general case.
WBR
On the other side, programmers coming from languages supporting very strong type systems
and encouraging type safety as a way of mind, 
would prefer to either encode a precondition at the type level, 
or to encode a generalize behaviour where such precondition is not needed, to dodge the problem.
For example under their mindset, Wcode(max) should not return an element, but an optional
element, and the option of no element will be produced in case of an empty list.
This approach do not scale:
certain conditions can not be expressed in the type system,
some other conditions could be expressed but it would be too cumbersome.
WBR
In our vision, the purpose of the type system is to help making the program adhere to
its intended behaviour or break fast.
We do not want to bend our intended behaviour so that code never breaks by construction;
such code can still behave unexpectedly (with respect to the original intended behaviour).





WTitle(`(5/5) Going forward')

You are now ready to do simple programs in 42.
While coding, you should refer to (link) where you can find
detailed documentation for all the classes of AdamTowel and many useful libraries.

WP

If you want to go forward and have a better understanding
of 42, you can now read, in any order,

<ul><li>
42 Core language design, for programming language experts
</li><li>
42 syntactic sugar, detailed explanations
</li><li>
42 type system, exact rules for promotions and usage of fwd types
</li><li>
42 metaprogramming guide and how there is no state
</li><li>
42 testing, mocking and configuration
</li><li>
exporting 42 on different platforms
</li><li>
42 and native code, how to import libraries from other languages
</li><li>
42 optimizers 
</li></ul>