WBigTitle(Exercises)

A very large class of practically useful programs can be obtained by
just declaring
elemental classes, collections
and simple Data classes.

Let's see some exercses and solutions 
to understand better how 42 code looks like

WTitle(max method)
write a class method max returning the max from a list of numbers
WP
Solution:
OBCode

CCode

WTitle(write a class method map producing a string from to lists of stirngs of the same lenght.
– For example
map(keys:Strings[S"a";S"b";S"c"],vals:Strings[S"z";S"y";S"z"])
should produce S"a->z, b->y, c->z"
– [c] write a class method Strings upTo(Strings that, N size) that filters out from a list of strings the ones longer
than size.
– For example upTo(Strings[S"a";S"ab";S"abc"],size:2N)
==Strings[S"a";S"ab"]
– [d] declare a class person with String name and N age, declare a list of persons Persons.
write a class method from(names,ages) producing a list of persons from a list of strings and a list of
ages.
Completion (B):
As for before, for each of the following task, create a class method with the required behavior and a main testing such
class method. Here you are going to be required to use the class decorators.
– [a] This is a variation of Core.d We ask you to to two changes:
Using Use or Extends declare Persons so that the class method from(names,ages) is a member of
Persons.
Using Invariant declare Person so that valid persons only have positive ages and not empty names. Add more
test cases to check the invariant is holding.
– [b] Define a new kind of list generator, producing lists that also offers
operators >>, <<. >>= and <<= for strictly superset/subset and superset/subset
Challenge(A+)
– [a] try to edit Collecitons.vector(of:) so that it behaves like Collecitons.vector(ofEq:) when possible.
– [b] In the current state TestGui does not work. The reason is that there is a *very* long string literal, that forces
us to generate code that is larger that what javac can handle.
 Add in PlgAlu a plugin method S fileRead(S filename) that can read a file, import this functionality in
AdamTowel01/ AdamTowel02. (look at MfileDebug£xfileName£xcontent)
After that, modify the Gui libr


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