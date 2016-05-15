WBigTitle(Philosopy)

Other languages have a weak division between language features
and their standard library.
42 have a very strong separatione between language and libraries.
You can see that from the mentality of the language and
the mentality of the popular Wcode(AdamTowel).

WTitle((1/5)Philosopy of L42)
<ul><li>
42 is just an instrument; use it as best as you can,
to do what you prefer.
</li><li>
The most outstanding language invariants are 
immutablity (of immutable objects),
encapsulation (of capsule references)
strong error safety,  MetaSafety (to introduce before),
Full guaranteed object initialization.
Other caracteristcs includes
absence of null pointers and unsafe casts,
safe circular initialization,
support for automatic parallelism
Of course this is all copuled with the conventional
avoidance of method not understood.
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
</li></ul>

WTitle((2/5)Philosopy of AdamTowel)

<ul><li>
AdamTowel offers a large set of simple concepts, that you can use
to encode the domain of your problem.
<BR/>
Basic classes should represent atoms of knowledge,
<BR/>
Collections should represent homogeneus groups of objects,
where every object serve the same role in your domain.
<BR/>
Data classes are agglomerates of other classes with an invariant.
<BR/>
Modules just have class methods and are a simple way to organize your code.
<BR/>
Resources serve to indicate constants.
<BR/>
Algorithmic classes will implement a certain algorithm interface
and will perform their tasks polimorphically.
<BR/>
Messages will report errors and exceptions.
<BR/>
Decorators will complete your code from,
 adding the needed boilerplate.
</li><li>
AdamTowel can be embrowdered to create many variations,
and those variations can all play together with little effort/discipline.
</li><li>

</li><li>
There is no intrinsic meaning in 42, no language feature
is designed to be used only in a certain way.
</li><li>
42 is based on a minimal core composed by little more than
method calls and exception handling.
Then there is a thick layer of syntactic sugar allowing 
for more convenient syntax.
</li></ul>




In 42 libraries can be directly manipulated, and
a possible manipulation is to convert it in 
another format, like an executable jar or a native program
and then save the result somewhere, like on the website where you users can download it.

OBCode
{reuse L42.is/AdamTowel
ToDeploy:Resource<<{
  reuse L42.is/AdamTowel
  //yes, we repeat the reuse
  /*..lots of code here..*/
  }
Task:DeployOut.asExecutableJar(
  main:Selector"aClassMethod"
  location:URL"..."
  )<<ToDeploy()
}
CCode

Note that we reuse adamTowel both outside Wcode(MyProgram)
and inside of it;
The two towels do not need to be the same.
The outermost just have to support the deployment process
Wcode(DeployOut), while the inner one is needed to make
Wcode(MyProgram) a close library: only libraries that do not refer to external classes can be deployed.

WTitle(42 projects)
In order to write any sizable program, it would be great
to be able to organize our code in multiple files spanning a hierarcky of folders.
In 42, we can obtain this by using 'Wcode(...)'.
A 42 project can be either a file
with upper case name and extension Wcode(.L42)
or a folder containing a file called Wcode(This.L42).
Folders can contain other files Wcode(.L42) or folders containing other Wcode(This.L42) and other files.

WP
In the 42 code, when using 'Wcode(...)' we refer
to a folder/file with the name of the innermost nested library and we import its content.

For example in the following, we refer to the file/folder Wcode(MyProgram).
OBCode
{reuse L42.is/AdamTowel
ToDeploy:Resource<<{
  reuse L42.is/AdamTowel
  Main:...
  class method
  Void main()
    Main.runStuff()
  }
Task:DeployOut.asExecutableJar(
  location:URL"...")<<ToDeploy()
}
CCode

A common way to use 42 is to have a folder Wcode(ProjectName) containing
a folder Wcode(Main) with all the actual code,
and then various files providing testing and deploying functionalities.

In general, for medium size projects is a good idea to keep executing the tests before the deployment; for example
directly under Wcode(Main:...) we could add
Wcode(TestsRunner: ...)
Do not panick, If the test are not reachable from Wcode(myMainMethod), they are not going to be included in the
executable jar.
WP
42 can support varius kinds of unit testing and mocking,
but there is no support at this stage in AdamTowel.

WTitle((2/5)Towels embroidery)
A towel is about the most massively useful thing a programmer can have.
A towel has immense psychological value, and you should always know where your towel is.
All the classes we have used up to now without defining them, are defined in AdamTowel.
They are all normal classes/libraries.

You can code without a towel, but this means starting from first principles,
witch could be quite unpleasant; especially since the only
primitive things that 42 offers are Library literals
(code as first class entities), the constant Wcode(void),
and the types Wcode(Library), Wcode(Void) and Wcode(Any).
WP

The distinction between towels and other libraries is just psicological;
we generally call towels libraries that are expected to provide standard
functionalities and types, such as numbers, booleans Bool,
strings S and various kinds of decorators and system errors.

WP
However, we do not expect all L42 programs to reuse the same towel.
For hygienic reasons, in real life everyone tends to use their own towel.
For similar reasons, any sizable 42 program will use its own towel.

WP

We expect different programs to use massively different libraries for
what in other languages is the standard library.
That is, there is no such thing as 'the L42 standard library'


WTitle(Using multiple Towels)

Towels shines when multiple towels are used at the same time.

OBCode
{reuse L42.is/AdamsTowel
//here you can access to lots of
//utility classes declared inside the towel
//including numbers, strings and so on.
C:{reuse L42.is/FordTowel
//here you can access a different set of classes.
//For example, Num would refer to the number in FordTowel
//and to see the number declared in AdamsTowel
//you have to write This1.N
  }
}
CCode

Different code parts reason about different set of classes;
including those predefined in other languages.

Useful for code that reasons on code; that is a very common task
in L42. 

WTitle(Define and deploy our own towel)
If you are writing a sizable program, 
or many similar programs, it make sense to
deploy you own towel, when you can 
pre load import some libraries and define your basic classes.

OBCode
{reuse L42.is/AdamTowel
ToDeploy:Resource<<{
  reuse L42.is/AdamTowel
  Gui:Load<<{reuse L42.is/Gui}
  Kg:Units.of(Num)
  Meter:Units.of(Num)
  /*..*/  
  }
Task:DeployTowel(
  url:Url"https://github.com/MyProjectName/MyTowel.L42"
  permissions:S".."
  )<<ToDeploy()
}
CCode

The former code would create your towel and update it
on your github repository every time you
run it.
WP
The idea of modifying a towel to create a variation is caled
WEmph(Towel Embroidery).
Wcode(MyTowel)
 is just a variation of AdamTowel, with more stuff added at the bottom.
We can do much more than that.

One useful tool to modify the content of a towel
is Wcode(Extend.patch(that)).
The idea is that we extend a library using a part of itself
as a path. As an example
OBCode
Code:Extend.patch(Path"Fix")<<{
  class method
  S sayHi()
    S"Hi"
  Fix:{
    class method @override
    S sayHi()
      S"Say "++this.#1sayHi()
    }
  }
CCode

Wcode(Code.sayHi()) will return Wcode(S"Say Hi").
This also works for nested classes. If for example
you wish to add a reverse method to Wcode(S) in your towel, you could do the following:

OBCode
ToDeploy:Resource<<Extend.patch(Path"Fix")<<{
  reuse L42.is/AdamTowel
  Fix:{S:{
    method S reverse() {/*..*/}
    }}
  }
CCode

The advantage with respect to compose two separated
libraries is that the scope is the same,
that is the implementation of Wcode(reverse()) will be able to use Wcode(Bool), Wcode(Num) and so on.




WTitle((3/5)Library deployment)

If you start writing in 42, you will soon feel the need
of factorizing your project into libraries, that could
be independently tested, deployed and loaded.
While successfull libraries are used by multiple 
independent projects and developers,
most libraries exists just as a developement tool in 
order to keep under control the complexity of big projects.
In 42 is easy to code with multiple libraries.
In 42 is possible to employ a programming model where every developer (or every pair of developers in a pair programming style) is the
only responsable of one (or more) library and its mainentence process, while the group leader give specifications and tests to be met to the varius library developers and will glue all the code together.

WP

Most L42 libraries are not towels.
All 42 libraries are closed code.
Thus, most non-towel 42 library are generic
(have abstract classes/methods) that can be rebinded
to a moltitute of towels.

Libraries can be deployed in a way similar to towel deployment.
Wcode(Load) is used to load libraries,
but it also contains all the knowledge to deploy
them.
OBCode
{reuse L42.is/AdamTowel
ToDeploy:Resource<<{
  reuse L42.is/MyTowel
    MyLib:...
  }
}
Task:Load.DeployLibrary(
  path:Path"MyLib",
  url:Url".."
  )<<ToDeploy()
}
CCode

If there was any nested library unreachable from public classes
in Wcode(MyLib), it will be pruned away.

Note how in this example we use towels Wcode(AdamTowel)
 and Wcode(MyTowel).
Wcode(Load) used in
Wcode(Task) comes from 
Wcode(AdamTowel), thus has no idea of the 
embrowdery process that created.
This means that all classes already present 
in Wcode(AdamTowel) will be attempted to be abstracted away, while all the newly introduced classes will become private.
Thus, in our example, 
Wcode(Gui),
Wcode(Kg) and
Wcode(Meter)
would become an implementation detail of the exposed library.

if Wcode(MyTowel) had some non trivial embrowdery, like
the string Wcode(reverse()) shown before,
then it needs to be loaded on similarly embrowdered towels.

WP
If we wish to expand the Loading/deployment process, we
need to patch Wcode(ConceptMap)
as in the following example:
OBCode
ToDeploy:Resource<<Extend.patch(Path"Fix")<<{
  reuse L42.is/AdamTowel
  Kg:Units.of(Num)
  Gui:Load<<{reuse L42.is/Gui}
  Fix:{
    ConceptMap:{interface 
      method Kg _Kg()
      method Gui _Gui()
      }
    }
  }
CCode

In this simple case, we can just use the class name to encode the
WEmph(conceptual names) for the newly added classes.
For more flexible library loadings, you can use semantic URI as ontological nodes.
WP
Our fixed towel can be used now to deploy and load libraries wrote in
this new towel, and libraries deployed and loaded in this way will 
share a unique definition for Wcode(Kg) and Wcode(Gui).






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


    
OBCode Library myCode={ method Int foo()} CCode
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
