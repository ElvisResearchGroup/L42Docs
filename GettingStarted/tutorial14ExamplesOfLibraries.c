WBigTitle(Example a 42 program)

In this chapter, we will see how to develop a GUI connected with a Data Base.
For simplicity we consider to have a database with a single tabel Wcode(Person) that contains a fiew fields.
We want to make a GUI that displays the data and allows to edit it.

OBCode
reuse [AdamTowel]
Unit = Load:{reuse[Unit]}
LoadJ = Load:{reuse[JavaServer]}
LoadGui = Load:{reuse[GuiBuilder]}
Query = Load:{reuse[Query]}
CCode
First we load the libraries we need:
Unit, JavaServer, GuiBuilder and Query.
WP
We then declare some useful units. Persons have ages expressed in years,
heights expressed in meters and weights expressed in Kgs.
OBCode
Year = Unit(I)
Meter = Unit(Num)
Kg = Unit(Num)
CCode
WP
We then ask Wcode(Query.sql) to generate a class to support SQL queries using a java slave and a connection string.
As an example, we are using a derby DB.
For now, we consider the DB to be altready populated. In the end we discuss how to initialize the DB itself.
OBCode
//SQL part
DBJ = LoadJ(slaveName=S"dbServer{}") //slave for the DB connection
DB = Query.sql(//instantiate Query for SQL queries
  connectionString=S"jdbc:derby:PersonsGui;create=true",
  javaServer=DBJ)
CCode


The class Wcode(DB) can now reify the DB tables; here it is just Wcode(Table.Person).
OBCode
Table = DB.#$of().tables() //
CCode


Finally, we can make a query box with all the queries that are relevant for this application.
A query box is a capability class, whose methods are able to do queries on a give database.
It is obtained with the decorartor Wcode(DB.QueryBox),
that can recognize nested classes created with the
Wcode(DB.query) method.
Since Wcode(DB) was created by providing the connection string, queries are aware of their database.

OBCode
Queries = DB.QueryBox:{
  All = DB.query[Table.Person.List]"SELECT * FROM Person"
  
  Insert = DB.query[Void;S;Year;Meter;Kg]"""
    |INSERT INTO Person (name,age,height,weight)
    |Values (@name,@age,@height,@weight)
    """
  DeleteId = DB.query[Void;I]"DELETE FROM Person WHERE id=@id"
  DeleteName = DB.query[Void;S]"DELETE FROM Person WHERE name=@name"
  }
CCode
The symbol Wcode(@) identifies parameter in the queries, while the types
in Wcode([..]) are the query result type followed by any parameters.
Queries return lists of objects. Those objects are constructed by calling a (factory) method whose arguments have the same name as the query fields.
WP

Right now the class Wcode(Query) supports both Wcode(SQL) and Wcode(IQL).
We expect to add more query languages in the future.
Wcode(IQL) is a query language to query the user by asking them to complete a form.
Using Wcode(IQL) in 42 is very similar to using Wcode(SQL).
In particular, the result is always in the form of lists of a type that can be instantiate using a unique #immK(..) method.
While this is a consistent and flexible way to process tabular data,
it means  that for queries returning a single column we must have a 
type with such a single field.
In 42, declaring those types and their corresponding list type takes just a single line.
Note how for Person we can use our specialized units Wcode(Year), Wcode(Meter) and Wcode(Kg).
OBCode
Key = Data:Data.AddList:{I id}
PName = Data:Data.AddList:{S name}
Person = Data:Data.AddList:{S name, Year age, Meter height, Kg weight}
CCode
In the same way, if a query returns a signle row, we will have it served as the only element of a lenght 1 list.
WP
We can now make the set of all user queries with another Wcode(QueryBox):

OBCode
GuiJ = LoadJ(slaveName=S"miniGuiSlave{}") //slave for IQL and Gui
IQL = Query.iql(javaServer=GuiJ)//instantiate Query for IQL queries
Dialogs = IQL.QueryBox:{//A box with all the queries we want to support
  AddPersons = IQL.query[Person.List]"""
    | 'Add persons' Pages('Add data for more persons')
    | name 'name' String
    | age  'age'  Integer
    | height 'height' Decimal{regex='[0-9]*\\.[0-9][0-9]'}
    | weight 'weight' Decimal{regex='[0-9]*\\.[0-9]'}
    """
  RemoveById=IQL.query[Key.List]"""
    | 'Deleting an entry' Single('Entry to delete?')
    | id 'index' String
    """
  RemoveByName = IQL.query[PName.List]"""
    | 'Deleting entries' Tabular('Entries to delete?')
    | name 'name' String
    """
  }
CCode

WP

A model for this simple application will have the two boxes and the java slave to control the Gui.
OBCode
//GUI part
Gui = LoadGui(javaServer=GuiJ)  
Model = Data:GuiJ.Handler:{ //the model answering to Java events
  mut GuiJ j
  mut Queries sql
  mut Dialogs iql
  mut method Void submitEvent(S that, S msg)[_] =
    this.#j().submitEvent(key='Example.Display, id=that, msg=msg)
  @GuiJ.Handler mut method Void printAll(S msg)=(
    this.submitEvent(S"tableClear", msg=S"")
    for (id,name,age,height,weight) in this.#sql().all()() (
      this.submitEvent(S"tableAdd", msg=S"%id,%name,%age,%Double(height),%Double(weight),")
      )
    whoops DB.Fail, GuiJ.Fail
    )
  @GuiJ.Handler mut method Void addPerson(S msg)=(
    for (name,age,height,weight) in this.#iql().addPersons()() (
      this.#sql().insert()(name=name, age=age, height=height, weight=weight)
      )
    this.printAll(msg=msg)
    whoops DB.Fail, IQL.Fail
    )
  @GuiJ.Handler mut method Void removeById(S msg)=(
    for (id) in this.#iql().removeById()() (//zero or one time
      this.#sql().deleteId()(id=id)
      )
    this.printAll(msg=msg)
    whoops DB.Fail, IQL.Fail
    )
  @GuiJ.Handler mut method Void removeByName(S msg)=(
    for (name) in this.#iql().removeByName()() (//zero or many times
      this.#sql().deleteName()(name=name)
      )
    this.printAll(msg=msg)
    whoops DB.Fail, IQL.Fail
    )
  }
OpenGui = {class method Void (mut GuiJ j)[_] = (
  gui=Gui(j=j,package=S"miniGui",imports=S"""%
    | %Gui.defaultImports()
    | import javax.swing.table.DefaultTableModel;
    """,
    name='Example,x=800\,y=600\
    )
  gui"""%
    |JPanel screen1=new JPanel();
    |{add(screen1);}
    |JPanel buttons=new JPanel();
    |{addNorth(screen1,buttons);}
    |%gui.button(id=S"addPerson",msg='PressedAdd,text=S"add")
    |{addFlow(buttons,addPerson);}
    |%gui.button(id=S"removeById",msg='PressedRemove,text=S"remove by id")
    |{addFlow(buttons,removeById);}
    |%gui.button(id=S"removeByName",msg='PressedRemove,text=S"remove by name")
    |{addFlow(buttons,removeByName);}
    |%gui.button(id=S"printAll" msg='PressedPrint text=S"printAll")
    |{addFlow(buttons,printAll);}
    |Object[] tLabels={"id","name","age","height","weight"};
    |DefaultTableModel tModel=new DefaultTableModel(new Object[][]{},tLabels);
    |JTable table = new JTable(tModel);
    |{addCenter(screen1,new JScrollPane(table));}
    |{event.registerEvent("Example.Display","tableAdd",
    |  (k,id,msg)->SwingUtilities.invokeLater(()->tModel.addRow(msg.split(","))));}
    |{event.registerEvent("Example.Display","tableClear",
    |  (k,id,msg)->SwingUtilities.invokeLater(()->tModel.setRowCount(0)));}
    """
  )}
//Finally, the Main puts all together
Main=(
  j=GuiJ.#$of()//the Java GUI slave
  sql=Queries(DB.#$of())//sql queries and the Java DB slave
  iql=Dialogs(IQL(j))//iql queries supported by the GUI slave
  model=Model(j=j,sql=sql,iql=iql)
  OpenGui(j=j)
  for e in j(\['Example]) ( e>>model )//event loop
  )
//Drop=DB.query[Void]"DROP TABLE Person"//optionally clean up
//AfterMain= ( Drop(DB.#$of())(),  db.kill() )



Create = DB.query[Void]"""
  |CREATE TABLE Person (
  |  id int NOT NULL PRIMARY KEY GENERATED ALWAYS AS IDENTITY
  |    (Start with 1, Increment by 1),
  |  name varchar(255),
  |  age int,
  |  height decimal(5,2),
  |  weight decimal(5,1)
  |  )
  """
PopulateIfEmpty = {//set up the DB tables in case this is the first run
  return Create(DB.#$of())() 
  catch Message _ return void
  }

CCode
Eventually, also show Deploy as Jar.
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


42 stands for the primate of libraries, so let see some libraries in action.
We have already see how to chose a towel, and many classes that are likely to be present in such towel, like
Wcode(S) and Wcode(Data).
WBR
Let's see how to load a library from its url: 
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
    //msg is going to be a multiline string coming from the html message/event.
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
    <button onclick= 'event42("quit");'> Quit</button>
    <button onclick= 'event42("sayHi");'> Say Hi</button>
    <button onclick= 'event42("say\ncaterpillar");'> Say Hi Caterpillar</button>
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
pass a Wcode(FileSystem) object to functions that do I/O: 

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
each of those classes will have a nested class representing 
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
  This from(UnivDb.Student db) //injection/conversion method: UnivDb.Student -> Person
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