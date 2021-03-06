WBigTitle(Input Output with Object Capabilities)
WTitle((1/5) Controlling non determinism)
Traditionally, in imperative languages I/O and side effects can happen everywhere, while
in pure functional languages like Haskell they are kept in check by monads.

In 42 we have type modifiers to keep mutation and aliasing under control.
With only the features shown up to now, 42 is a deterministic language, thus
every expression that takes in input only immutable objects
can be called multiple times with the same arguments and will produce the same result.
WBR
The whole caching system relies on this property to work.
WBR
Thus input output, random numbers and any other kind of observable non determinism must preserve this property.
Introducing object capabilities:
WBR
An WTerm(object capability) is a mutable object whose methods can do some non deterministic, or otherwise privileged operation.
If a method is given a Wcode(mut) reference to an object capability, then it can be non deterministic,
otherwise it will be deterministic.
Creation of new object capabilities is only possible by either relying on an existent object capability
or by using a WTerm(capability method): a method whose name starts with Wcode(`#$'); however those specially named methods can only be called from a main, from another capability method or from a Wcode(mut) method of a capability object.

WTitle((2/5) Example: File System)

To read and write files we need to load a Wcode(FileSystem) library as shown below:
OBCode
FS = Load:{reuse [L42.is/FileSystem]}
CCode
Wcode(FS) is the local name for the library located at Wcode(L42.is/FileSystem).
The Wcode(Load) decorator embeds the librady in the current environment.
We can now use our file system:
OBCode
Main1 = (
  mut FS f = FS.Real.#$of()
  S s=f.read(fileName=S"data.txt") //non determistic operation
  //the result depends on the current file content
  f.write(fileName=S"data.txt",content=S"SomeContent") //non determistic operation
  //the operation could go in error if there is not enough space to write the content on disk.
  Debug(s)
  )
  }
CCode
The crucial point in the former code example is the call to 
Wcode(FS.Real.#$of()).
This instantiates a capability object using the capability method Wcode(#$of()).

We could write the code inside a method in the following way:
OBCode
ReadWrite = {class method
  Void (mut FS f) = (
    S s=f.read(fileName=S"data.txt")
    f.write(fileName=S"data.txt",content=S"SomeContent")
    Debug(s)
    )
  }
Main1 = ReadWrite(f=FS.Real.#$of())
CCode
Note how we pass the capability object explicity to the method.
This is the most common style, and have great testing advantages:
Indeed, Wcode(FS) corresponds to the following interface:
OBCode
interface
mut method Void makeDirs(S fileName)
mut method Void delete(S fileName)
mut method Void write(S fileName,S content)
mut method S read(S fileName)
CCode
and Wcode(FS.Real) is simply an implementation of such interface connected with the real file system.
Thus, we can write a simple mock to check that the function behaves as expected:
OBCode
Mock = Data:{[Fs]
  var S log=S""
  method makeDirs(fileName) = error X""
  method delete(fileName) = error X""
  method write(fileName,content) = ( 
    X[actual=fileName expected=S"data.txt";
      actual=content  expected=S"SomeContent";]
    this.log(\log++S"write")
    )
  method read(fileName) = (
    X[actual=fileName expected=S"data.txt"]
    this.log(\log++S"read")
    S"oldContent"
    )
  }
Test1= (
  m=Mock()
  ReadWrite(f=m)
  {}:Test"ReadWriteOk"(actual=m.log() expected=S"readwrite")
  )
CCode

WTitle((3/5) Object capabilities programming patterns)

The advantage of the division of the file system in an interface and a Wcode(Real) implementation are not limited to testing.
For example, the user could embed some security and on some restrictions in an alternative implementation of a file system.
Consider the following code:

OBCode
OnlyTxt = Data:{[Fs]
  capsule Fs inner
  class method
  Void checkTxt(S that)= X.Checked[
    that.endsWith(S".txt")
    ]
  method makeDirs(fileName) = error X""
  @Cache.Clear class method Void delete(mut Fs inner, S fileName) = (
    this.checkTxt(fileName)
    inner.delete(fileName)
    )
  @Cache.Clear class method Void write(mut Fs inner, S fileName, S content) = ( 
    this.checkTxt(fileName)
    inner.write(fileName=fileName, content=content)
    )
  @Cache.Clear class method Void read(mut Fs inner, S fileName) = ( 
    this.checkTxt(fileName)
    inner.read(fileName=fileName)
    )
  class method          //example user declared #$ method, that can
  mut This #$of() =     //use $# methods inside its body
    This(inner=Fs.Real.#$of())
  }
SaferMain = (
  fs = OnlyTxt.#$of()
  ReadWrite(f=fs)
  )
CCode
Any code that would take in input a Wcode(mut OnlyTxt) would have a limited access to the file system; only able to read and write onWcode(`*.txt') files.
Note that the field Wcode(inner) is not private but it is well encapsulated, thus a user of 
a Wcode(mut OnlyTxt) could only extract the Wcode(read) version of Wcode(inner).
We will see later how to set some members private, but in this case just encapsulation is sufficient.
WBR
Instances of Wcode(mut OnlyTxt) are capability objects; note how Wcode(OnlyTxt) can even declare a Wcode(#$of) method. In this way for the user there is no syntactical difference between using Wcode(Fs.Real) or using Wcode(OnlyTxt).
Capability objects are a useful abstraction and can be designed and implemented by normal 42 programs; they are not just a way for the language implementation to expose native code.
We have just shown that new object capabilities can easy be defined by simple wrapping over existing capability objects.

WBR
Since inner is of type Wcode(Fs), this programming patterns allows to layer many levels of security / restrictions on top of a capability object, as shown below:
OBCode
fs = OnlyTxt(inner=OnlySmallFiles(inner=Fs.Real.#$of()))
CCode


WTitle((4/5) Connection with other languages)
In general, all the  non determinism in 42 is obtained by comunicating with other languages.
42 allows to connect with Java, and Java allows to connect with C/assembly.
The best way to connect with java is to use the library Wcode(JavaServer) as shown below:
OBCode
reuse [L42.is/AdamsTowel]
J0 = Load:{reuse [L42.is/JavaServer]}
J = J0(slaveName=S"mySlave{}")
CCode
The code above loads the library Wcode(JavaServer). It is a generic library: before being used we need to provide a name for the Java slave.
A 42 program is not a single process but a cluster of intercommunicating Java processes.
There is one master process where the 42 computation is actually running and many other slave processes allowing safe input output and safe interaction with arbitrary code.
Such slave processes have their own name: in this case Wcode(mySlave).
Slaves also have a set of options, that can be specified between the Wcode({}).
We do not describe the details of those options here.
The class Wcode(J) can now be used to comunicate with the Java slave as shown below:
OBCode
MainAsk = (
  j = J.#$of()
  j.loadCode(fullName=S"foo.Bar1",code=S"""
    |package foo;
    |import is.L42.platformSpecific.javaEvents.Event;
    |public record Bar1(Event event){//class Bar1 will be instanstiated by 42
    |  public Bar1{                  //and the Event parameter is provided
    |    event.registerAskEvent("BarAsk",(k,id,msg)->
    |      "any string computed in Java using "+id+" and "+msg);
    |    }
    |  }
    """)
  S.Opt text = j.askEvent(key=S"BarAsk", id=S"anId",msg=S"aMsg")
  {}:Test"OptOk"(actual=text, expected=S"""
    |["any string computed in Java using anId and aMsg"]
    """.trim())
  )
CCode
This code asks the event Wcode("anId", "aMsg") on the channel Wcode("BarAsk").
The Java code registers the capacity of answering to the channel Wcode("BarAsk") and
computes an answer parametrized over Wcode(id) and Wcode(msg).
The method Wcode(askEvent) is syncronus: it will wait for java to provide an answer as an optional string; optional since Java can return Wcode(null) as a result.

As you can see, you can embed arbitrary Java code in 42 and communicate back and forth serializing data and instructions as strings.

Syncronus comunication is sometimes undesirable. 
For example, to use Java to open a GUI it would be better to have asyncronus comunication and a queue of events.
You can do this with Wcode(J.Handler), as shown below:
OBCode
Model = Data:J.Handler:{
  var I count, mut J j
  @J.Handler mut method Void fromJavaToL42(S msg)=(
    this.count(\count+1\)
    Debug(S"pong received %msg with count=%this.count()")
    if this.count()<40I (
      this.#j().submitEvent(key=S"BarIn", id=S"ping", msg=S"the message %this.count()")
      whoops J.Fail
      )
    else this.#j().kill()
    )
  }
MainPingPong = (
  j=J.#$of()
  j.loadCode(fullName=S"foo.Bar2",code=S"""
    |package foo;
    |import is.L42.platformSpecific.javaEvents.Event;
    |public record Bar2(Event event){
    | public Bar2{
    |    event.registerEvent("BarIn","ping"(k,id,msg)->{
    |      System.out.println("Ping Event received "+id+" "+msg);
    |      event.submitEvent("BarOut","fromJavaToL42","pong");
    |      });
    |    event.registerEvent("Kill",(k,id,msg)->{
    |      System.out.println("Doing cleanup before slave JVM is killed");
    |      System.exit(0);
    |      });
    |    }
    |  }
    """)
  model=Model(count=0I, j=j)
  model.fromJavaToL42(msg=S"Initial message")
  keys=S.List[S"BarOut"]
  models=J.Handler.Map[key=S"BarOut" mutVal=model]
  for e in j(keys) ( e>>models )
  Debug(S"Completed")
  )
CCode
The class Wcode(Model) handles the events inside of 42:
if Java send an event with id Wcode("fromJavaToL42") then the method
Wcode(Model.fromJavaToL42(msg)) will be called.
In turn, such method
sends to java the message 
Wcode("ping", "the message %this.count()") on channel Wcode("BarIn") using
Wcode(mut method J.submitEvent(key,id,msg))
up to 40 times, and kills the slave JVM after that.

In Wcode(MainPingPong) we initialize the slave JVM to respond to two channels:
Wcode("BarIn") and Wcode("Kill").
In our example Java will submit an asyncronus event to 42 as a response to
the Wcode("BarIn":"ping") event and will terminate the slave on any Wcode("Kill") event.
The slave should always terminate its JVM when receiving a kill, but can do any kind of clean-up before that.
After a JVM is terminated, it can be restarted by simply calling Wcode(J.loadCode(fullName,code)) again.

Finally, we set up the event loop:
An event loop will collect events from a list of Wcode(keys) and dispatch them
to a map of Wcode(models), mapping every key to a specific Wcode(model).
Note that both Wcode(keys) and Wcode(models) are mutable objects. In this way we can dynamically
register and unregistered keys/models by mutating Wcode(keys) and Wcode(models).
If the JVM is killed or the list of keys becomes empty, the event loop Wcode(for e in j(keys)) will terminate.
The operation Wcode(e>>models) dispatches the event to the model.
WBR
We need to use two different channels (Wcode("BarIn") and Wcode("BarOut")) to distinguish if an event is should be handled by 42 or by Java.


WTitle((5/5) Object capabilities summuary)
<ul><li>
While most languages run in a single process, 42 runs in a cluster of processes; this is needed so that the master process is procted from any slave process going into undefined behaviour.
This is the final piece of the puzzle allowing the correctness properties of 42 to be ensured in any circumstance.
</li><li>
To enable non deterministic behaviour we need to call those specially named Wcode(#$) methods.
Since they can only be called in a few controlled places, we can control what parts of the code can perform non determinism by explicitly passing capability objects.
</li><li>
Capability objects are a very convenient centralized point of control to inject security or other kinds of restrictions.
</li></ul>