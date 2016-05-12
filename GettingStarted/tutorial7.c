WBigTitle(Exercises)

A very large class of practically useful programs can be obtained by
just declaring
elemental classes, collections
and simple Data classes.

Insert exercises here


WBigTitle(Example of Libraries)

WTitle((1/5)Gui)
42 stands for the primate of libraries, so let see some libraries in action.
We have already see how to chose a towel, and many classes that are likely to be present in such towel, like
Wcode(S) and Wcode(Data).
Let see now how to load a library from its url:
OBCode
{reuse L42.is/AdamTowel
Gui: Load<<{reuse L42.is/Gui}
Main:{
  Gui.alert(S"hi!")
  return ExitCode.success()
  }
}
CCode
Load is another decorator, here it modifies the library found in L42.is/Gui
 so that it can be used easily from AdamTowel.

Gui allows to create graphical user interfaces using html.

OBCode
{reuse L42.is/AdamTowel
Gui: Load<<{reuse L42.is/Gui}
MyGui:Gui(
  title:S"My Gui"
  basePath:S"base/path/for/my/files"// use / on all op systems
  x:600Gui.Pixel//seek for index.html in your basePath
  y:400Gui.Pixel
  )<<{
  mut method
  Void event_quit(mut Gui gui, S msg)
    gui.close()

  mut method
  Void event_sayHi(mut Gui gui, S msg)
    Gui.alert(S"hi")

  mut method
  Void event_say(mut Gui gui, S msg) (
    //msg is going to be a multiline string,
    //first line equal to "say", other lines give more info
    Strings ss=msg.split(S.nl())
    Gui.alert(S"hi dear "+ss.val(1\))
    )
  }

Main:MyGui()
}
CCode
And the file Wcode(index.html)
would look like:
OBCode
//add html, css and javascript formatting
<html>
<head>
</head>
<body>
Hi, buttons here
<Button onclick='event42("quit");'> Quit</Button>
<Button onclick='event42("sayHi");'> Say Hi</Button>
<Button onclick='event42("say\ncaterpillar");'> Say Hi</Button>
</body>
</html>
CCode
Do not forget the two layers of quotes: you need to quote both the call to Wcode(event42)
and the string codifying the event itself.

To make something happen in the gui, you can use 
Wcode(gui.executeJs(cmd)).
For example
OBCode
mut method
Void event_writeOnTextArea(mut Gui gui, S msg) (
  Strings ss=msg.split(S.nl())
  js1=Gui.JavaScript"document.getElementById('myTextAreaId').value = 'Hi! event happened';"
  //or, to use information in the message
  js2=Gui.JavaScript"document.getElementById('"[ss.val(1\)]"').value = '"[ss.val(2\)]"';"
  gui.executeJS(js2)
  )
CCode

Gui provide help to visualize datastructures of various kind, 
for example vectors of Data classes can be shown as tables in the following way:
OBCode
Person:Data<<{Size id, Name name, Name surname, Year age}
Persons:Collections.vector(of:Person)
ShowPersons:Gui.widget(table:Persons)
InputPerson:Gui.input(dialogForm:Person)//rely on JQuery UI
MyGui:Gui(/*..*/)<<{ mut Persons persons //field
  /*..*/
  mut method
  Void eventLoad(mut Gui gui, S msg) ( //no underscore for system events
    gui.add(ShowPersons(this.#persons(), into:Gui.Id"divLeft"))
    //the element with that id will contain the table
    )
  mut method
  Void event_addPerson(mut Gui gui,S msg) (
    Person p=InputPerson(gui,title:S"New Person details")
    catch exception InputPerson.Cancelled void
    this.persons().add(left:p)
    gui.refresh()
    )
  }
Main:MyGui(persons:Persons[])
CCode

WTitle((2/5) Files)
In 42 you can import Wcode(FSystem) to read and write files.


OBCode
{reuse L42.is/AdamTowel
FSystem: Load<<{reuse L42.is/FSystem}

Main:{
  files=FSystem()
  files.write(S"foo.txt",S"foo foo foo!")
  S foos=files.read(S"foo.txt")//most likely, it contains 'foo foo foo!'
  return ExitCode.normal()
  }
}
CCode

42 can execute every closed expression early, even at compile time, if it could give 
a performance boost, thus you may want to pass a files object around to functions that do
I/O. Otherwise you may end up with unexpected early effects:

OBCode

method S readFoo()
 FSystem().read(S"foo.txt") //may read foo once and for all
//at compile time, and then return the same value every time
//it can be useful for loading resources, like image files
//in a simple game.

method S readFoo(mut FSystem that)
 that.read(S"foo.txt") //need the parameter to act, thus
 //will wait until a parameter is provided
CCode

In general, all the system interaction that happens over the same system object are
chronologically sorted with respect to each other, but there is no guarantee 
of ordering between different system objects.

WTitle((3/5) Db)
In AdamTowel, databases can be access in two ways:
Raw access (similar to what is supported by
Wcode(DBC) or Wcode(JDBC) libraries) 
and Structured access.
OBCode
{reuse L42.is/AdamTowel
Db: Load<<{reuse L42.is/Db}//Db can do Raw access
UnivDb:Db.importStructure(Db.ConnectionS"...")
QueryCountry:UnivDb.Query"select * from students where country=@country"
Main:{
  connection=UnivDb.connect()
  UnivDb.Student.Table ss=QueryCountry(connection, country:S"Italy")
  /*..*/
  }
}
CCode
Here
we use Wcode(Db) to 
import the structure of the database. This means that 
Wcode(UnivDb) will contain a class for each table-row of the database,
every class will have a nested type representing 
multiple rows (that is, a table).
Wcode(UnivDb.Query) allows prepared queryes.
In the case in the example, we can use
Wcode(`QueryCountry(that,country)')
to get the italian students.
Note how Wcode(country) is a parameter of the method: in this way the  query users are informed of the query expectations.
Our query used Wcode(*), so the type Wcode(UnivDb.Student.Table)
could be reused.
In other cases a new type would be generated.

WTitle((4/5) example Gui and Db together)



WTitle((5/5) Libraries, summary)

WBigTitle(Refactor and Extend)

WTitle((1/5)Refactor)
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
leaving only the public scheleton
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
The main idea is that members with the same name are recoursivelly summed

WTitle((2/5)Traits and Metaprogramming)

Metaprogramming is the most important feature of 42,
all the decorators that you have seen up to now are implemented with metaprogramming, that in 42 offers a good balance of freedom and
safety.
The main idea of 42 metaprogramming is that only library literals can
be manipulated, and that once a library literal has a name, it can not be directly metaprogrammed any more.
Thus, we call traits methods that return reusable (unnamed) code.
For example
OBCode
ReusableCode:{
  Library traitEnsureTransaction() {
    class method //without name: can be called as a funtor
    Void(mut Db.Connection connection)
    exception Db.Query.Failure //no body: abstract method

    class method
    Void doAction(mut Db.Connection that)
    exception Db.Query.Failure (
      that.openTransaction()
      This(connection:that)//here we call us as a funtor
      catch exception Db.Query.Failure x (
        that.rollback()
        exception x
        )
      that.commitTransaction()
      )
    }
  }

MyAction:Refactor.compose(
  left:ReusableCode.traitEnsureTransaction()
  right:{ //and now the missing implementation
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



WTitle((3/5)Extend)


WTitle((4/5)Exercise: extending vector)

WTitle((5/5)Metaprogramming summary)
 <!--
 
 
 Use Extends Refactor
 


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
