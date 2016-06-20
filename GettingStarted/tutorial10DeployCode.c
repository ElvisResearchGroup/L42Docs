WBigTitle(Deploy 42)
In the context of 42 and AdamsTowel, there are three things that can be deployed:
 Executable programs,  Towels and Libraries.


WTitle((1/5)Deploy programs)
In 42 libraries can be directly manipulated, and
one possible manipulation is to convert them in 
another format, like an executable jar or a native program
and then save the result somewhere, such as on the website where you users can download it.

OBCode
{reuse L42.is/AdamsTowel
ToDeploy: Resource <>< {reuse L42.is/AdamsTowel
  //yes, we repeat the reuse
  /*..lots of code here..*/
  class method
  Void main() (
    /*..*/
    )
  }
Task: Deploy.asExecutableJar(
  main: Selector"main()"
  location: URL".."
  ) <>< ToDeploy()
}
CCode

Note that we reuse AdamsTowel both outside Wcode(MyProgram)
and inside of it.
The two towels do not need to be the same.
The outermost just has to support the deployment process
Wcode(Deploy), while the inner one is needed to make
Wcode(MyProgram) a closed library: only libraries that do not refer to external classes can be deployed.

WTitle(42 projects)
In order to write any sizeable program, it would be great
to be able to organize our code in multiple files spanning a hierarchy of folders.
WP
A 42 project can be either a file
with upper case name and extension Wcode(.L42)
or a folder containing a file called Wcode(This.L42).
Folders can contain other files Wcode(.L42) or folders containing other Wcode(This.L42) and other files.

In 42 we use  ellipsis Wcode(...) to include content of other files.
The meaning of Wcode(...) depend 
on both the location of the ellipsis in the code and 
of the position of the current file in the file system. To evaluate an ellipsis Wcode(...)
we locate the nearest enclosing nested
library declaration, and we record its name, Wcode(Main) in the following example.
This identifies either a file Wcode(Main.L42) or a folder folder Wcode(Main), that will contain a file This.L42.


If both or neither of these exist, there is an error.
 Otherwise we include the contents of the found file.

Note that the found *.L42 file can contain more ellipses, which will be
resolved relative to the file before importing it into the current scope.

WP
The following code show how to deploy some code as an executable jar.
OBCode
{reuse L42.is/AdamsTowel
ToDeploy: Resource <>< {
  reuse L42.is/AdamsTowel
  Main: ...
  }
Task: Deploy.asExecutableJar(
  mainPath:Path"Main"
  main: Selector"main()"
  location: URL"..") <>< ToDeploy()
}
CCode

A common way to use 42 is to have a folder with the name of your project, containing
a folder Wcode(Main) with all the actual code,
and then various files providing testing and deploying functionalities, as in the following example:

OBCode
{reuse L42.is/AdamsTowel
ToDeploy: Resource <>< {
  reuse L42.is/AdamsTowel
  Main: ...
  TestsRunner: ...
  }
Task: Deploy.asExecutableJar(
  mainPath:Path"Main"
  main: Selector"main()"
  location: URL"..") <>< ToDeploy()
}
CCode

In general, for medium size projects is a good idea to keep executing the tests before the deployment; for example
directly under Wcode(Main: ...) we could add
Wcode(TestsRunner: ...)
Do not panic, If the test are not reachable from Wcode(Main.main()), they are not going to be included in the
executable jar.
WP
42 could support various kinds of testing libraries,
but there is no support at this stage in AdamsTowel.

WTitle((2/5)Deploy Towels)
A towel is about the most massively useful thing a programmer can have.
A towel has immense psychological value, and you should always know where your towel is.
All the classes that we have used up to now without defining them, are defined in AdamsTowel.
They are all normal classes/libraries.

You can code without a towel, but this means starting from first principles,
which could be quite unpleasant; especially since the only
primitive things that 42 offers are Library literals
(code as first class entities), the constant Wcode(void),
and the types Wcode(Library), Wcode(Void) and Wcode(Any).
WP


Towels are libraries providing standard
functionalities and types, such as number, boolean,
string and various kinds of decorators and system errors.

WP
However, we do not expect all 42 programs to reuse the same towel.
For hygienic reasons, in real life everyone tends to use their own towel.
For similar reasons, any sizeable 42 program will use its own towel.

WP

We expect different programs to use massively different libraries for
what in other languages is the standard library.
That is, there is no such thing as 'the 42 standard library'.


WTitle(Using multiple Towels)

Towels shines when multiple towels are used at the same time.

OBCode
{reuse L42.is/AdamsTowel
//here you can access to lots of utility classes declared inside the towel
//including numbers, strings and so on.
C: {reuse L42.is/FordTowel
//here you can access a different set of classes.
//For example, Num would refer to the number in FordTowel
//and to see the number declared in AdamsTowel you have to write This1.N
  }
}
CCode

Different code parts reason about different set of classes;
including those predefined in other languages.
That is, by introducing multiple towels in nested scopes,
the names of the other scopes are "masked".

Useful for code that reasons on code; that is a very common task
in 42. 

WTitle(Staining Towels)
If you are writing a sizeable program, 
or many similar programs, it make sense to
enrich a towel with some pre loaded libraries
and basic classes.

OBCode
{reuse L42.is/AdamsTowel
ToDeploy: Resource <>< {
  reuse L42.is/AdamsTowel
  Gui: Load <>< {reuse L42.is/Gui}
  Kg: Units.of(Num)
  Meter: Units.of(Num)
  }
Task: Deploy.asTowel(
  url: Url"https: //github.com/MyProjectName/RichTowel.L42"
  permissions: S".."
  ) <>< ToDeploy()
}
CCode

The former code will create your towel and update it
on your github repository every time you
run it.
WP
A WEmph(Stained Towel) is a towel that looks like another but is enriched by adding more things at the bottom.
In our example, Wcode(RichTowel)
 is just a stained variation of Wcode(AdamsTowel).


WTitle((3/5)Library deployment)

If you start writing in 42, you will soon feel the need
to factorize your project into libraries that can
be independently tested, deployed and loaded.
While successful libraries are used by multiple 
independent projects and developers,
most libraries exists just as development tools in 
order to keep the complexity of big projects under control.
WP
In 42 is easy to code with multiple libraries, and libraries can be much smaller.
WP
In 42 is possible to employ a programming model where every developer (or every pair of developers in a pair programming style) is the
only one responsible of one (or more) library and their maintenance process, while the group leader give specifications and tests to be met to the various library developers and will glue all the code together.

WP

Libraries can be deployed in a way similar to towel deployment;
Wcode(Load) is used to load libraries,
but it also contains all the knowledge to deploy
them.
WBR
The following example code deploys a library
using Wcode(AdamsTowel): 
OBCode
{reuse L42.is/AdamsTowel
//could be L42.is/RichTowel and nothing would change

ToDeploy: Resource <>< {
  reuse L42.is/RichTowel
  //need to be RichTowel; for example MyLib is using Gui
  MoreStuff:...
  MyLib: ...
  }
}
Task: Load.DeployLibrary(
  path: Path"MyLib",
  url: Url".."
  ) <>< ToDeploy()
}
CCode

This code deploy Wcode(MyLib) to an URL as a library.
WBR
If there was any nested library unreachable from public classes
in Wcode(MyLib) or in Wcode(MoreStuff) it will be pruned away.
Same for any nested library stained on top of Wcode(AdamsTowel).
WP
The deployed library can be imported as usual.
For example using
Wcode(Imported:Load <>< {reuse ..})
we will see the content of Wcode(MyLib) inside of Wcode(Imported).

WP
Most 42 libraries are not towels, but all 42 libraries are closed code.

They will have have abstract classes/methods
for each of the original towel concepts (before staining), and they can be rebound
to a multitude of towels.
In particular all stained versions of the same towel are compatible.
Every needed nested library
that was not present in the original towel, will be made private.
On the other side, all the classes in the original towel will 
be made abstract by Wcode(Load.DeployLibrary)
and will be rebound to the current towel by Wcode(Load).

WP

Thus, in our example, 
Wcode(Gui),
Wcode(Kg),
Wcode(Meter) and Wcode(MoreStuff)
would become a private implementation detail of the exposed library.


WTitle((4/5)Towel embroidery: Define and deploy our own towel)


Towel embroidery it is like adding your initials to your towel.
WP
While we can simply add to the end by staining, embroidery is much more powerful.
WP
The most common embroidery tool 
is Wcode(Extend.patch(that)).
The idea is that we extend a towel using a part of itself
as a patch.
WBR
As an artificial example: 
OBCode
Code: Extend.patch(Path"Fix") <>< {
  class method
  S sayHi()
    S"Hi"
  Fix: {
    class method @override
    S sayHi()
      S"Say "++this.#1sayHi()
    }
  }
CCode

Wcode(Code.sayHi()) will return Wcode(S"Say Hi").
This also works for nested classes. If for example
you wish to add a reverse method into Wcode(S) in your towel, you could do the following: 

OBCode
ToDeploy: Resource <>< Extend.patch(Path"Fix") <>< {
  reuse L42.is/AdamsTowel
  Fix: {S: {
    method S reverse() {/*..*/ 0Num /*..*/}
    }}
  }
CCode

The advantage with respect to composing two separated
libraries is that the scope is the same,
that is the implementation of Wcode(reverse()) will be able to use Wcode(Bool), Wcode(Num) and so on.
That is, if we was to write our code as 

OBCode
ToDeploy: Resource <>< Extend[{reuse L42.is/AdamsTowel}] <>< {
  S: {
    method S reverse() {/*..*/ 0Num /*..*/}
    }
  }
CCode
Now Wcode(Num) would be bound to the outer towel instead of the inner one.



Towel staining is a very minimal personalization, and stained towels
are fully compatible with the original one.
By embroidery you can personalize a lot more the content of your towel,
but when library deployment 
rely on an embroidered towel, compatibility with the original towel is lost.
For example, an embroidered version of 
Wcode(AdamsTowel) 
can Wcode(Load) a library developed on the original 
Wcode(AdamsTowel), but a library developed on the embroidered version 
needs to be loaded into a similarly embroidered towel.

One typical reason to embroider a towel is to
extend the set of classes that are shared between libraries.
For example, one may want to develop a Towel for scientific use
where the existence of some units of measure can be shared between all the libraries.

WP
To extend/expand the Loading/deployment process in this way, we
need to patch Wcode(ConceptMap)
as in the following example: 
OBCode
{reuse L42.is/AdamsTowel
ToDeploy: Resource <>< Extend.patch(Path"Fix") <>< {
  reuse L42.is/AdamsTowel
  Kg: Units.of(Num)
  Metre: Units.of(Num)
  Second: Units.of(Num)
  Fix: {
    ConceptMap: {interface 
      method Kg en_wikipedia_org$wiki$Kilogram()
      method Metre en_wikipedia_org$wiki$Metre()
      method Second en_wikipedia_org$wiki$Second()
      }
    S: {
      method
      S reverse() {/*..*/}
      }
    //just to show we can add to the concept map and to string
    //at the same time, no problem.
    }
  }

Task: Deploy.asTowel(
  url: Url"https://github.com/SI/SITowel.L42"
  permissions: S".."
  ) <>< ToDeploy()
}
CCode
Now Wcode(SITowel) can be used  as a towel,
and can be used to deploy libraries that can be loaded by 
Wcode(SITowel).

By using (standard transmogrification of) semantic URIs as
ontological nodes, we
can create a basis for other libraries when trying to infer the meaning of our added types.
WP
Our fixed towel can be used now to deploy and load libraries wrote in
this new towel, and libraries deployed and loaded in this way will 
share a unique definition for certain units of measure.
Note that libraries originally developed for
Wcode(AdamsTowel) can still be loaded normally.
If they was to internally define a concept of eg. Wcode(Meter),
this would be interpreted as a normal (possibly private) nested class
inside of the loaded library, and will not be merged with the unified 
concept of Wcode(Meter) defined in Wcode(SITowel).


WTitle(`(5/5)Deployment: programs, libraries and towels; summary')
<ul><li>
42 is a metaprogramming tool.
It is natural to use 42 either a language (to run a program)
or as a compiler (to deploy programs, libraries and towels).
</li><li>
Indeed we expect all sizeable 42 projects to use 42 as a compiler,
to produce some reusable artefacts.
</li><li>
The distinction between towels (that do not need to be Wcode(Load)ed)
and other (Wcode(Load)able) libraries is 
introduced not by 42, but by Wcode(AdamsTowel); radically different towels may provide different meaning for the concepts of deploying and
loading libraries/towels.
</li><li>
Application developers can freely stain and embroider towels;
in this way they can adapt Wcode(AdamsTowel) to serve them better.
However, Library developers need to carefully consider the effect of embroidery.
</li></ul>
 <!--

OBCode
Observer: {interface
  mut method update()
  }
Observers: Collection.vector(ofMut: Observer)

Traits: {

  class method
  Library subject(){
    mut Observers observers

    class method
    mut Observers #default_observers()
      Observers[]

    mut method
    Void register(Observer that)
      this.#observers().add(right: that)

    mut method
    Void notify() 
      with Observer o in this.#observers().vals() (
        o.update()
        )
  }
  class method
  Library widget(){ /*..*/}

}

MyObservableWidget: Extends[Traits.subject(); Traits.widget()] <>< {
  mut method @override
  Void repaint() (this.#2repaint() this.notify())
  }
CCode

It is possible to iterate over a range of numbers: 

OBCode
with i in 4Size.vals() (
  Debug(i) //prints 4,5,6,7 and so on forever
  )
CCode

OBCode
with i in 4Size.upTo(42Size) (
  Debug(i) //prints 4,5,6,7 and so on up to 42 excluded
  )
CCode


Resources used within an iteration can be released after the iteration 
since collections are notified when the iteration ends.

OBCode
//a contains "foo1 \n foo2 \n foo3"
//b contains "bar1 \n bar2"
with 
  input in LineStream.readFile(S"a"), 
  var output in LineStream.readWriteFile(S"b",fill: S"None") (
    output:=  output +" : "+input) //line by line, add input in the file
//b contains "bar1 : foo1 \n bar2 : foo2 \n None : foo3"
CCode


    
OBCode Library myCode= { method Int foo()} CCode
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
tipo x= MyCode  x.hello(...)
-->
