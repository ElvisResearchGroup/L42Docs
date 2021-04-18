WBigTitle(Deploy 42)
In the context of 42 and AdamsTowel, there are three things that can be deployed:
 Executable programs,  Towels and Modules.


I'm assuming the following extra features still to implement:
-writing to url
-removing unused code
-symplifying dependency for Load that does not use stuff


WTitle((1/5)Deploy Towels)

A towel is about the most massively useful thing a programmer can have.
A towel has immense psychological value, and you should always know where your towel is.
All the classes that we have used up to now without defining them, are defined in AdamTowel.
They are all normal classes/libraries.
WBR
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
However, we do not expect all 42 programs to reuse the same exact towel.
For hygienic reasons, in real life everyone tends to use their own towel.
For similar reasons, any sizeable 42 program will use its own towel.
WP

We expect different programs to use massively different libraries for
what in other languages is the standard library.
That is, there is no such thing as 'the 42 standard library'.


WTitle(Using multiple Towels)

Towels shines when multiple towels are used at the same time.

OBCode
reuse [L42.is/AdamsTowel]
//here you can access to lots of utility classes defined inside the towel
//including numbers, strings and so on.
C = {reuse [L42.is/FordTowel]
  //here you can access a different set of classes.
  //For example, Num would refer to the number in FordTowel
  //and to see the number defined in AdamsTowel you have to write This1.N
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
reuse [L42.is/AdamsTowel]
ToDeploy = Trait:{
  reuse [L42.is/AdamsTowel]
  Unit = Load:{reuse [L42.is/Unit]}
  Kg = Units.of(Num)
  Meter = Units.of(Num)
  }
Task = Deploy.towel(
  url=\"https://github.com/MyProjectName/RichTowel.L42"
  permissions=\".." //???
  code=Class:ToDeploy
}
CCode

The former code will create your towel and update it
on your github repository every time you
run it.
WP
A WEmph(Stained Towel) is a towel that looks like another but it is enriched by adding more things, either at the bottom.
In our example, Wcode(RichTowel)
 is just a stained variation of Wcode(AdamsTowel).


WTitle((2/5)Module deployment)

If you start writing in 42, you will soon feel the need
to factorize your project into libraries that can
be independently tested, deployed and loaded.
We call those library WTerm(Modules).
Very successful modules are used by multiple 
independent projects and developers; they are what is often called a third party library.
However, most modules exists just as development tools in 
order to keep the complexity of big projects under control.
WP
In 42 is easy to code with multiple modules, and modules can be much smaller than usual third party libraries and framewors in other languages.
WP
In 42 it is possible to employ a programming model where every developer (or every pair of developers in a pair programming style) is the
only one responsible of one (or more) modules and their maintenance process, while the group leader give specifications and tests to be met to the various module developers and will glue all the code together.

WP

Modules can be deployed in a way similar to towel deployment;
Wcode(Load) is used to load libraries,
but it also contains all the knowledge to deploy
them.
WBR
The following example code deploys a Module
using Wcode(AdamsTowel):
OBCode
reuse [L42.is/AdamsTowel]
//could be L42.is/RichTowel and nothing would change

Module = Trait:{
  reuse [L42.is/RichTowel]
  //need to be RichTowel; for example AirplaneUnits is using Unit
  AirplaneUnitsUtilities = {...}
  AirplaneUnits = {...}
  }
Task = Deploy.module(
  url=\"https://github.com/MyProjectName/AirplaneUnits.L42"
  permissions=\".." //???
  code=Class:Module
  name='AirplaneUnits
  )
CCode

This code deployes Wcode(Module.AirplaneUnits) to an URL as a module.
WBR
If there was any nested library unreachable from public classes
Wcode(AirplaneUnitsUtilities) it will be pruned away.
Same for any nested library stained on top of Wcode(AdamsTowel) and for 
any private unreachable one in Wcode(AirplaneUnits).
WP
The deployed library can be imported as usual.
For example using
Wcode(Imported = Load:{reuse ..})
we will see the content of Wcode(AirplaneUnits) inside of Wcode(Imported).

WP

All the deployed code is closed code.
Towels are closed because they contain all the code to implement strings, numbers and so on.
Modules are also closed. They have abstract classes/methods
for each of the original towel concepts (before staining), and they can be rebound
to a multitude of towels.
In particular all stained versions of the same towel are compatible.
Every needed nested library
that was not present in the original towel, will be made private.
On the other side, all the classes in the original towel will 
be made abstract by Wcode(Deploy.module(..))
and will be rebound to the current towel by Wcode(Load).

WP

Thus, in our example, 
Wcode(Unit),
Wcode(Kg),
Wcode(Meter) and Wcode(AirplaneUnitsUtilities)
would become a private implementation detail of the exposed library.


WTitle((3/5)Deploy programs)

We can run our  applications inside 42, but we can also deploy them as  Jars, so that they can be run as a Java application.
WBR
In 42 libraries can be directly manipulated, and
one possible manipulation is to convert them in 
another format, like an executable jar or a native program
and then save the result somewhere, such as on the website where the users can download it.
WBR
For example, we could rework the code of the former chapter as follows:

OBCode
reuse [L42.is/AdamTowel]
ToJar = Trait:{reuse [L42.is/AdamTowel]
  Unit = Load:{reuse[L42.is/Unit]}
  LoadJ = Load:{reuse[L42.is/JavaServer]}
  LoadGui = Load:{reuse[L42.is/GuiBuilder]}
  Query = Load:{reuse[L42.is/Query]}
  Year = Unit(I)
  Meter = Unit(Num)
  Kg = Unit(Num)
  DBJ = LoadJ(slaveName=S"dbServer{}")
  DB = Query.sql(connectionString=S"jdbc:derby:PersonsGui;create=true", javaServer=DBJ)
  Table = DB.#$of().tables()
  Queries = DB.QueryBox:{...}
  GuiJ = LoadJ(slaveName=S"miniGuiSlave{}")
  IQL = Query.iql(javaServer=GuiJ)
  Dialogs = IQL.QueryBox:{...}
  Gui = LoadGui(javaServer=GuiJ)  
  Model = Data:GuiJ.Handler:{...}
  OpenGui = {...}
  class method Void #$main() = ( //method #$main instead of 'Main'
    j=GuiJ.#$of()
    sql=Queries(DB.#$of())
    iql=Dialogs(IQL(j))
    model=Model(j=j,sql=sql,iql=iql)
    OpenGui(j=j)
    for e in j(\['Example]) ( e>>model )
    )
  }
Main = Deploy.jar(
  url=\"https://github.com/MyProjectName/MyApplication.jar"
  permissions=\".." //???
  code=Class:ToJar
  )
CCode
As you can see,
we are wrapping the application code into a trait, including
a second reuse of Wcode(AdamTowel).
In this way the code of Wcode(ToJar) is fully self contained, and 
Wcode(Main) in the outer scope can still use all of the towel features by taking them from the outer Wcode(reuse[AdamTowel]).
We then use Wcode(`Deploy.jar(..)') to deploy our application onto a jar in a specific location.
WP
When 42 is used to deploy an application as Jar, you can see the whole 42 execution as a comprehensive compilation framework,
encompassing all the tools and phases that could possibly be needed into a single cohesive abstraction.

WP
In this example we reuse AdamsTowel both outside Wcode(ToJar)
and inside of it.
The two towels do not need to be the same.
The outermost just has to support the deployment process
Wcode(Deploy), while the inner one is needed to make
Wcode(ToJar) a closed library: only libraries that do not refer to external classes can be deployed.

WTitle(A 42 project testing and deploying)

A common way to use 42 is to have a folder with the name of your project, containing
a folder Wcode(Main) with all the actual code,
and then various files providing testing and deploying functionalities, as in the following example:

OBCode
reuse [L42.is/AdamsTowel]
Main = Trait:{
  ...
  Tests = {
    TestSuite_1 = {...}
    /*..*/
    TestSuite_n = {...}
    }
  }
Task = Deploy.jar(url=\"MyApplication" code=Class:Main)
CCode

In general, for medium size projects is a good idea to keep executing the tests before the deployment; for example
we can have a test suite after the Wcode(...).
Do not panic, If the test are not reachable from Wcode(Main.#$main()), they are not going to be included in the executable jar.
WP


WTitle((4/5)Towel embroidery: Define and deploy our own towel)


Towel embroidery it is like adding your initials to your towel.
WP
While we can simply add to the end by staining, embroidery is much more powerful.
WP
The most common embroidery tool 
is Wcode(Organize).
Together with late casts, we can add methods to any existing class as shown below:
OBCode
Code = Trait:Organize:{reuse [L42.is/AdamTowel]
  S$ = {/*..more methods for string here..*/
    method S reverse() = (/*..*/ this<:@This1 S /*..*/)
    }
  Num$ = {/*..more methods for numbers here..*/}
  }
CCode



The advantage with respect to composing two separated
libraries is that the scope is the same,
that is the implementation of Wcode(reverse()) will be able to use Wcode(Bool), Wcode(Num) and so on.


Towel staining is a very minimal personalization, and stained towels
are fully compatible with the original one.
By embroidery you can personalize a lot more the content of your towel,
but when module deployment 
relies on an embroidered towel, compatibility with the original towel is lost.
For example, an embroidered version of 
Wcode(AdamsTowel) 
can Wcode(Load) a library developed on the original 
Wcode(AdamsTowel), but a library developed on the embroidered version 
needs to be loaded into a similarly embroidered towel.

One typical reason to embroider a towel is to
extend the set of classes that are shared between libraries.
For example, one may want to develop a Towel for scientific use
where the existence of some units of measure can be shared between all the libraries.
We could do the following:

import units
import SI as $
organize
super Load.baseDeps() and add all the SI
annotate all of the SI with corresponding wikipedia names by sum

Can have a decorator lib * Name * string ->
add nested Name to Load.baseDeps() and annotates @AbstractTowel{string}
on Name.
is it worth while? it may hide useful understanding

Load.baseDeps()
and annotate with @AbstractTowel{name}
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
this would be interpreted as a private nested class
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
and modules is 
introduced not by 42, but by Wcode(AdamTowel); radically different towels may provide different meaning for the concepts of deploying and
loading libraries/towels.
</li><li>
Application developers can freely stain and embroider towels;
in this way they can adapt Wcode(AdamTowel) to serve them better.
However, Library developers need to carefully consider the effect of embroidery.
</li></ul>
 <!--


 <!--WTitle(Simpler complete program)

Let now starts showing the simplest 42 program: an empty library.

OBCode
{}
CCode

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


TODO:
In 42 we have single and multiline strings, and string interpolation works on both.
String interpolation 

%x.foo()
%varName/ClassName .foo()[].foo()[]()
or
%(...)
Yes comments when reasonable
string literals only in () and in multiline strings
Bug: (un)balanced parenthesis in string literals, multi line str lit and comments are not ignored
