WBigTitle(Exercises)

A very large class of practically useful programs can be obtained by
just declaring
elemental classes, collections
and simple Data classes.

Let's see some exercses and solutions 
to understand better how 42 code looks like

WTitle((1/5) Max method)
write a class method max returning the max from a list of numbers
WP
Solution:
OBCode
UndefinedOnEmpty:Message.$<<{implements Guard}
//Max is undefined on empty lists.
//Since there was no mention of preconditions,
//we should explicitly handle all the error cases!
class method
Num max(Nums that) {
  if that.isEmpty() (error UndefinedOnEmpty"Max is undefined on empty lists")
  //now that we know we can proceed:
  var Num maxCandidate=that.left()
  //there is no such thing like a minimum number,
  //we need to select one element from the list.
  with n in that.vals() (
    //we could write that.withoutLeft().vals()
    //to avoid cheching on the first again
    if maxCandidate<n (maxCandidate:=n)
    //update the variable to keep track of the max so far.
    )
  return maxCandidate
  }
CCode

WTitle((2/5) Merge two lists of strings)
Write a class method map producing a string from to lists of stirngs of the same lenght.
 For example
Wcode(`map(keys:Strings[S"a";S"b";S"c"],vals:Strings[S"z";S"y";S"z"])')
should produce Wcode(`S"[a->z, b->y, c->z]"')

Solution:
OBCode
UnequalSize:Message.$<<{implements Guard}
class method
S map(Strings keys, Strings values) (
  if keys.size() != values.size() (error UnequalSize"keys="[keys.size()]", values="[values.size()]"" )
  S"["[with k in keys.vals(), v in values.vals() (
    use[k++S"->"++v, sep:S", "]
    )]"]"
  //no need to use return, the result is the last element in round brakets
  )
CCode

WTitle((3/5) Filtering)
Write a Wcode(`class method Strings upTo(Strings that, Size size)') that filters out from a list of strings the ones longer
than size.
For example 
Wcode(`upTo(Strings[S"a";S"ab";S"abc"],size:2Size)==Strings[S"a";S"ab"]')
Precondition: size is not negative
Solution:
OBCode
class method
Strings upTo(Strings that, Size size) (
  Assert.Pre[size>=0Size]
  Strings[with s in that.vals() (
    if s.size()<=size (use[s])
    )]
  )
CCode

WTitle((4/5) Random mole)
For a longer example, represent a piece of land as a 80*80 bidimensional vector,
where every cell can be full of dirt (90%) or rock (10%).
Then a mole start from the left top corner and attempts to
digs throught dirt randomly.
After 100 steps the mole stops.
declare the opportune classes and write a randomDig
method. You can use the library Wcode(L42.is/Random)
for randomness. You can use Wcode(Range(stop)) to iterate over
all (Wcode(Size)) nubers from 0 to Wcode(stop)-1 included.

A possible solution:
OBCode
Random:Load<<{reuse L42.is/Random}

Cell:Enumeration"dirt, rock, empty, mole"

Direction:Enumeration"up, down, left, right"

Point:Data<<{implements Concept.Invariant
  Size x,Size y
  method invariant()
    x>=0Size & x<80Size & y>=0Size & y<80Size
  This go(Direction that) {
    if that.isUp() (return this.with(x:\-1Size))
    if that.isDown() (return this.with(x:\+1Size))
    if that.isLeft() (return this.with(y:\-1Size))
    Assert.Bug[that.isRight()]
    return this.with(y:\+1Size)
    catch error Concept.Invariant.Failure err
      return this
    }
  }

Land:Data<<{(mut Cell.List cells)
  
  class method
  mut This ()
    This(cells:Cell.List[
      with i in Range(stop:80Size*80Size) (
        if Random(10Size)==0Size (use[Cell.rock()])
        else (use[Cell.dirt()])
        )
      ])   

  mut method
  Void randomDig() (
    dirs=Directions.List.all()
    var Point current=Point(x:0Size,y:0Size)
    with i in Range(stop:100Size) (
      this.set(current,val:Cell.empty())
      d=dirs.val(Random(4Size))
      newPoint=current.go(d)
      if !this.get(d).isRock() (//no digging in rock
        current:=newPoint
        )
      )
    this.set(current,val:Cell.mole())
    )
  
  //implementation of the matrix as an example,
  //should be imported from a library
  mut method 
  Void set(Point that, Cell val)
    this.#cells()(that.y()*80Size+that.x(), val:val)
    
  read method 
  Cell get(Point that)
    this.#cells().val(that.y()*80Size+that.x())    
  
  toS()(//since we define it explicitly, Data will leave it alone :)
    S""[with x in Range(stop:80Size)
      use[S.nl()]//newline
      with y in Range(stop:80Size) (
        p=Point(x:x,y:y)
        if this.get(p).isRock() (use[S"#"])
        else if this.get(p).isDirt() (use[S"%"])
        else if this.get(p).isEmpty() (use[S" "])
        else (Assert.Bug[this.get(p).isMole()] use[S"M"])         
      )]"")
  }
CCode


WTitle((5/5) Examples summary)
<ul><li>
Always think about what can go wrong upfront
</li><li>
Most methods can be completed by first checking for 
errors/issues and then using a Wcode(with)
</li><li>
Before heading into a problem,
spend some time to define your problem domain.
We dogded a lot of headeches by defining
points with invariants.
</li></ul>

Some may argue that in a real object oriented implementation,
directions and cells should be interfaces with subtypes;
so that we can use single dispatch to
avoid the cascades of ifs.
We have mixed feelings about this:
shorter code is better and more maintainable then longer code, and the version with subtyping would have been much longer.

The crucial point is that that code is not designed to be used by other programmers as a library.
Libraries should have well modularize code,
and provide covenient hooks for adaptation.
Metaprogramming and interfaces are the right tool for this task.

We should not counfond adaptability (without touching the original source, make it so that a new problem can be tackled), with maintenability (change the original source to keep it up to date
with the everchaning set of requirements).





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