m4_include(`header.h')m4_dnl

WComm WTitle(`First, read in any order those introductions')
WComm <Button class= 'button' type= 'button' onclick= 'selectDiv("a");'>
WComm Why 42
WComm </Button>

WComm <Button class= 'button' type= 'button' onclick= 'selectDiv("c");'>
WComm Pure OO
WComm </Button>

<Button class= 'button' type= 'button' onclick=
 "location.href = 'tutorial.xhtml';">
Download
</Button>


WComm <Button class= 'button' type= 'button' onclick= 'selectDiv("b");'>
WComm Learn through examples
WComm </Button>


<Button class= 'button' type= 'button' onclick=
 "location.href = 'tutorial.xhtml';">
See the full Tutorial
</Button>

<Button class= 'button' type= 'button' onclick=
 "location.href = 'https://github.com/ElvisResearchGroup/L42';">
Git Repository
</Button>

<Div><p>
WTitle(Why 42)

42 is a new programming language, whose name is inspired by The Hitchhiker's Guide to the Galaxy.
Why should you give 42 a try?
WP
42 programs are secure, easy to optimize and customize.
WBR
More in details:
WP
<ul>
<li>
<h5>Security by Modularization</h5>
A secure program never does the wrong action. For example, it never commits to the database ill formed data.
In 42 you can clearly specify what those actions are and what are the corresponding correctnes criteria. The rest of the code will be unable to break those constraints.
In this way the programmer can focus the majority of their time on writing correct software, while the security is guaranteed by the system.
</li><li>
<h5>Performance by Abstraction</h5>
Most of the performance issues are not about smart ways to generate ultra fast assembly. Most often they are about code repeating the same operations over and over again. It may be a poorly designed algorithm, abstraction mismatch, or maintenance spoiling your perfomance by slowly twisting assumptions.
In 42 various forms of caching and automatic parallelism are proven semantically unobservable. This means that those optimizations are easy to insert and are unable to impact the behaviour of the system
</li><li>
<h5>Customization by Composition</h5>
Not all projects have the same requirements, and not all the deveolopers have the same preferences. Most languages are stuck with a single standard library, that ends up being obsolete after a few years. At best, developers can use a linter to enforce a specific programming style.
In 42, towels cover the same role of the standard library, but towels can be personalized, to fit better a given company, project or programmer.
Methods and classes can be added, removed or simply renamed. This can be done while preserving compatibility with all of the third party libraries ecosystem.
</li>
</ul>
WP
All those statements looks unbelivable given current programming widsom.

<ul><li>
In the Wlink(tutorial,guide), we explain the language, and we explain those statements and how they work.
</li><li>
You can also 
Wlink(tutorial,dowload 42) and try it yourself.
</li><li>
You can also engage with the open source GitHub <a href="https://github.com/ElvisResearchGroup/L42">repository</a>.
</li>
</ul>



<!--
42 is a new programming language,
whose name is inspired by The Hitchhiker's Guide to the Galaxy.
The language's goal is to allow the transparent cooperation and composition of millions of libraries at the same time, while providing a much higher security that any other widly used language.
WP
In 42 you will be able to import libraries just by naming them and to
compose them in your code without worrying about their dependencies.
Normally libraries are big monolithic things, coming from outside and manually imported/installed. The user has little control over
the library code, and the library has no influence over the importing context.
WP
In 42 libraries are usually much smaller and are first class entities, that can be manipulated as values, loaded, stored, modified, adapted, saved or simply used. You will write code that manipulate libraries, but more importantly you will use libraries that manipulate libraries in non-trivial ways.
Indeed in 42 manipulating libraries is so natural that is convenient to think of your own code as a library that you offer to yourself only.
WP
42 enforces a high level of abstraction, thus there is no concept of null or default initialization.
While is possible to write in a fully functional style in 42, usually 42 programs mix
WEmph(`mutable datastructures with aliasing')
and
WEmph(`deeply immutable data').
By using reference and object capabilities,
42 supports mutability and aliasing control, helping in mixing the two styles in an easy and natural way.

WP
Reference and object capabilities
enforce the correctness of caching, representation invariants and parallelism.
-->
</p></Div>


<Div id= "c"><p>
WTitle(Pure and fair Object Oriented language)
42 is a pure object oriented language,
where every value is an object. This means that: 
<ul>
<li>
Numbers are objects, 
so thay can have methods like Wcode(.sqrt()) or Wcode(.abs()). 
</li><li>
All operators are just method calls, thus Wcode(a + b) can be sugar for Wcode(`a.#plus0(b)'), 
and so on. Note how Wcode(`#plus0') is just an ordinary method name.
Any class that offers the method supports the operator.

</li><li>
Classes are objects, so when you write Wcode(Foo.bar()) you refer to the method Wcode(.bar()) offered by the object denoted by Wcode(Foo).
class objects are just objects, and you can also store them in local bindings if you wish, as in Wcode(x = Foo)
</li><li>
The code is an object, but only at the granularity of 
Libraries; that is, balanced pairs of curly brackets representing classes (or interfaces) with methods and nested libraries.
This is useful for meta-programming, which we will see later.

</li><li>
Differently from other pure object oriented languages, in 42
all objects have the same treatment, both syntactically and semantically.
For example Wcode(x =  S"Hello " ++ S"World")
This is a declaration for a local binding Wcode(x), using the string class Wcode(S) and the method operator Wcode(++),
used as string (or in general sequence/collection) concatenation.

Coming from another programming language, you may be surprised that we have to write Wcode(S) before Wcode("Hello ") and Wcode("World").
This extra verbosity is needed to provide fair treatment to all classes. (Wlink(`http://l42.is/tutorial.xhtml#BasicClasses',see more in Basic classes))
In 42 we do not give preferential treatment
to special objects/classes. In this sense, we consider
most other languages to be discriminatory.
They give priority to their "preferred" version of numbers and strings, and this puts into a position of unfair disadvantage library code trying to define its own kinds of numbers/strings.
Instead in 42 you may encounter strings like Wcode(Url"www.google.com") or
 Wcode(Email"Arthur.Dent@gmail.com").
The same for numeric classes: to talk about a street, 20 meters long, you would write
Wcode(streetLength= 20Meter).
Note that we write the class name after the number,
while usually the class name is before.
42 is a little incoherent in this point, both for better readability and because Wcode(Meter20) would be considered an identifier by the parser.
You may encounter Wcode(I), Wcode(Num), Wcode(Double), Wcode(Meter), Wcode(Kg), Wcode(Second), Wcode(Year), Wcode(PhoneNumber) and many other numeric classes.
</li></ul>

WTitle(A simple language)
We believe 42 is a simple language,
where all classes are final and interfaces are the only instrument to obtain subtyping.
Moreover, 42 does not offer many controversial features usually found in other programming
languages, including: 
(method) overloading; threading; numeric conversions and coercions; var-args; primitive types and automatic (un-)boxing;
inner classes; arrays; generics; scope-hiding of local variables/bindings, methods and fields;
closures/lambdas and null.

</p></Div>

m4_include(`syntaxCompare.h')m4_dnl

WComm WTitle(`Now, dig into the tutorial!')

WComm Wlink(tutorial,Let's board the Vogon ship)


m4_include(`footer.h')m4_dnl
