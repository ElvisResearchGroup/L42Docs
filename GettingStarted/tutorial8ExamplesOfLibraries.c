WBigTitle(Example of Libraries)

42 stands for the primate of libraries, so let see some libraries in action.
We have already see how to chose a towel, and many classes that are likely to be present in such towel, like
Wcode(S) and Wcode(Data).
Let see now how to load a library from its url: 
OBCode
{reuse L42.is/AdamsTowel
Gui: Load <>< {reuse L42.is/Gui}
Main: {
  Gui.alert(S"hi!")
  return ExitCode.success()
  }
}
CCode
Wcode(Load) is another decorator, here it modifies the library found in Wcode(L42.is/Gui)
 so that it can be used easily from AdamsTowel.


WTitle((1/5)Gui)

Gui allows to create graphical user interfaces using html.

OBCode
{reuse L42.is/AdamsTowel
Gui: Load <>< {reuse L42.is/Gui}
MyGui: Gui(
  title: S"My Gui"
  basePath: S"base/path/for/my/files" // use / on all operating systems
  x: 600Gui.Pixel
  y: 400Gui.Pixel
  //loads index.html from your basePath
  ) <>< {
  mut method
  Void event_quit(mut Gui gui, S msg)
    gui.close()

  mut method
  Void event_sayHi(mut Gui gui, S msg)
    Gui.alert(S"hi")

  mut method
  Void event_say(mut Gui gui, S msg) (
    //msg is going to be a multiline string coming from the
    //html message/event.
    //First line == "say", other lines give more info
    Strings ss= msg.split(S.nl())
    Gui.alert(S"hi dear "+ss.val(1\))
    )
  }

Main: MyGui()
}
CCode
And the file Wcode(index.html)
would look like: 
OHCode
<html>
  <head> </head>
  <body>
    Hi, buttons here
    <Button onclick= 'event42("quit");'> Quit</Button>
    <Button onclick= 'event42("sayHi");'> Say Hi</Button>
    <Button onclick= 'event42("say\ncaterpillar");'> Say Hi Caterpillar</Button>
  </body>
</html>
CCode
Notice the two layers of quotes: you need to quote both the call to Wcode(event42)
and the string codifying the event itself.

To make something happen in the gui, you can use 
Wcode(gui.executeJs(cmd)).
WBR
For example, an event could call the following method: 
OBCode
mut method
Void useJSToWriteOnTextArea(mut Gui gui) (
  js= Gui.JavaScript"document.getElementById('myTextAreaId').value =  'Hi! event happened';"
  gui.executeJS(js)
  )
CCode

Gui provides help to display datastructures of various kind, 
for example vectors of Data classes can be shown as tables in the following way: 
OBCode
Person: Data <>< { Size id, Name name, Name surname, Year age }
Persons: Collections.vector(of: Person)
ShowPersons: Gui.widget(table: Persons) //each person as a row in the table
InputPerson: Gui.input(dialogForm: Person) //relies on 'JQuery UI'
MyGui: Gui(/*..*/) <>< { mut Persons persons //field
  /*..*/
  mut method
  Void eventLoad(mut Gui gui) ( //no underscore for system events
    gui.add(ShowPersons(this.#persons(), into: Gui.Id"divLeft"))
    //the element with that id will contain the table
    )
  mut method
  Void event_addPerson(mut Gui gui,S msg) (
    Person p= InputPerson(gui,title: S"New Person details")
    catch exception InputPerson.Cancelled exc ( void) //do nothing
    this.persons().add(left: p)
    gui.refresh()
    )
  }
Main: MyGui(persons: Persons[])
CCode

WTitle((2/5) Files)
In 42 you can import Wcode(FileSystem) to read and write files.


OBCode
{reuse L42.is/AdamsTowel
FileSystem: Load <>< {reuse L42.is/FileSystem}

Main: {
  files= FileSystem()
  files.write(S"foo.txt",S"foo foo foo!") //the file 'foo.txt' in the current directory
  S foos= files.read(S"foo.txt") //most likely, it contains 'foo foo foo!'
  return ExitCode.normal()
  }
}
CCode

Each instance of the FileSystem class owns its own stream of
operations, which are not coordinated with the other instances.  If
you create multiple instances, then you may find that events attached
to other instances happen much earlier than you expect.
This happens since 42 can execute every closed expression early,
even at compile time, if it could give a performance boost.
To avoid unexpected early effects
you should
pass a files object to functions that do I/O: 

OBCode

method S readFoo()
 FileSystem().read(S"foo.txt") //may read foo once and for all at compile time,
//and then return the same value every time. It can be useful
//for loading resources, like image files in a simple game.

method S readFoo(mut FileSystem that)
 that.read(S"foo.txt") //need the parameter to act, thus
//will wait until a parameter is provided
CCode

In general, all the system interaction that happens over the same system object are
chronologically sorted with respect to each other, but there is no guarantee 
of ordering between different system objects.

WTitle((3/5) Db)
In AdamsTowel, databases can be accessed in two ways: 
Raw access (similar to what is supported by
Wcode(DBC) or Wcode(JDBC) libraries) 
and structured access.
OBCode
{reuse L42.is/AdamsTowel
Db: Load <>< {reuse L42.is/Db} //Db can do Raw access
UnivDb: Db.importStructure(Db.ConnectionS"...")
QueryCountry: UnivDb.query(\"select * from student where country= @country")
Main: {
  connection= UnivDb.connect()
  UnivDb.Student.Table ss= QueryCountry(connection, country: S"Italy")
  /*..*/
  }
}
CCode
Here
we use Wcode(Db) to 
import the structure of the database. This means that 
Wcode(UnivDb) will contain a class for each table of the database, and
every class will have a nested type representing 
multiple rows (that is, a table).
Wcode(UnivDb.query(that)) allows prepared queries.
In the case in the example, we can use
Wcode(`QueryCountry(that,country)')
to get the Italian students.
Note how Wcode(country) is a parameter of the method: in this way the  query users are informed of the queries requirements.
Our query used Wcode(*) (all columns), so the type Wcode(UnivDb.Student.Table)
could be reused.
In other cases a new type would be generated, eg. Wcode(QueryCountry.Table).

WTitle((4/5) Example Gui and Db together)

In the following code we show an example where a Gui display the Italian students.

We could directly display elements of 
type Wcode(UnivDb.Student), but we chose 
to write more flexible and maintainable code
(code maintenance requires localized changes),
where we define our basic classes (Wcode(Name) and
Wcode(Year))
and we define an injection from
Wcode(UnivDb.Student)
to
Wcode(Person).
Then we use the 
Wcode(eventLoad) method
to load the information from the DB and
to display it.

OBCode
{reuse L42.is/AdamsTowel

Name: Alphanumeric: <>< {This parse(S that) (
  if that.contains(S.nl()) (error Alphanumeric.ParseError
    "new lines not allowed in method names")
  This(that) 
  )} //check for more restrictions

Year: Unit.of(Num)

Db: Load <>< {reuse L42.is/Db}

UnivDb: Db.importStructure(Db.ConnectionS"...") //will not use Name and Year

QueryCountry: UnivDb.query(\"select * from students where country= @country")

Person: Data <>< { Name name, Name surname, Year age 
  class method
  This from(UnivDb.Student db)
    Person(
      name: Name.from(base: db.name())
      surname: Name.from(base: db.surname())
      age: Year.from(base: db.age())
      )
  }

Persons: Collections.vector(of: Person)

ShowPersons: Gui.widget(table: Persons)

MyGui: Gui(/*..*/) <>< {
  mut method
  Void eventLoad(mut Gui gui, S msg) (
    connection= UnivDb.connect()
    UnivDb.Student.Table ss= QueryCountry(connection, country: S"Italy")
    ps= Persons[with s in ss.vals() (use[Person.from(db: s)])]
    gui.add(ShowPersons(ps, into: Gui.Id"divLeft"))
    )
  }

Main: MyGui()
}
CCode

It is interesting to notice that if we wish to change the
content of the students displayed representation, we just need to change the class Wcode(Person) and add/remove/swap fields.
WP

It is also interesting to consider what happens if the database schema changes.

If there are no more students, or the students do not have countries any more,
then we will get an error while generating the class Wcode(QueryCountry).

If the students will not have names, surnames or ages
any more, then we will get an error while generating the
Wcode(Person) class.
In some sense we are turning into understandable compile time errors events that would have caused a runtime exception in most other languages.

WTitle((5/5) Libraries, summary)

42 is designed to support libraries and cooperation of multiple libraries at the same time.

Since 42 is still in its infancy, there are not many libraries around yet.
Stay tuned for more!