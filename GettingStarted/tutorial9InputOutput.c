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
FS = Load:{reuse[L42.is/FileSystem]}
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
  read method Void :1(Library that) = that:Test"ReadWriteOk"(
    actual=this.log() expected=S"readwrite")
  }
Test1= (
  m=Mock()
  ReadWrite(f=m)
  {}:m
  )
CCode
TODO: the above does not work that good if ReadWrite throws an error. But is the same for all tests?


{}:Test""(expected=ReadSelf(FileSystem.Real.#$of())
    actual=S"reuse [AdamTowel]%S.nl()MaxOfList = [###]")

FileSystem = Load:{reuse[FileSystem]}
  class method
  S (mut FileSystem that) = (
    S s=that.read(fileName=S"This.L42")
    Debug(S"[[%s]]")
    s
    //_=f.write(fileName=S"Example1.txt",content=S"SomeContent")
    )



LATER
Capability objects are a useful abstraction and can be designed and implemented by normal 42 programs; they are not just a way for the language implementation to expose native code.
New object capabilities can easy be defined by simple wrapping over existing capability objects


Intrinsic and pervasive non deterministic errors

A very large class of practically useful programs can be obtained
just by declaring
basic classes, collections
and simple Data classes.

Let's see some exercises and solutions 
to understand better how 42 code looks like

WTitle((1/5) Max method)
Write a static method Wcode(MaxOfList) returning the max from a list of numbers
WP
Basic Solution: 
OBCode
MaxOfList = {//static method pattern
  UndefinedOnEmpty = Message:{[Message.Guard]}
  //Max is undefined on empty lists.
  //Since there was no mention of preconditions, we should explicitly handle all the error cases as Guards
  class method
  Num (Num.List that) = {
    if that.isEmpty() ( error UndefinedOnEmpty"Max is undefined on empty lists" )
    //now that we know we can proceed: 
    var Num maxCandidate= that.left()
    //there is no such thing like a minimum Num, we need to select one element from the list.
    for n in that (
      //we could write 'that.withoutLeft()' to avoid checking on the first again
      if maxCandidate<n (maxCandidate:= n)
      //update the variable to keep track of the max so far.
      )
    return maxCandidate
    }
  }
CCode

Solution using Wcode(.reduce()):
OBCode
MaxOfList = {
  class method
  Num (Num.List that) =
    that.reduce()(
      for e in \vals() (
        if \acc < e \add(e) 
        )
      )
  }
CCode
Where the method Wcode(reduce())  will already throw a meaningful error in case of an empty list: Wcode(Collection.OperationUndefinedOnEmpty).
Defining your own error may still produce more readable errors, so feel free to mix and match the two approaches as show in the next exercise:


WTitle((2/5) Merging and filtering)
Write a static method Wcode(MapText) producing a string from to lists of strings of the same length.
 For example
Wcode(`MapText(keys=S.List[S"a";S"b";S"c"] vals=S.List[S"z";S"y";S"z"])')
should produce Wcode(`S"[a->z, b->y, c->z]"')
WP
Solution: 
OBCode
MapText = {
  UnequalSize = Message:{[Message.Guard]}
  class method
  S (S.List keys, S.List vals) = {
    if keys.size() !=  vals.size() error UnequalSize
      "keys= %keys.size(), values= %vals.size()" 
    //the former formatting allows us to keep a whole line for the error message
    res = S.List()(for k in keys, v in vals \add(S"%k->%v"))
    if res.isEmpty() return S"[]"
    text = res.reduce()(for s in \vals \add(\acc++S", %s"))
    return S"[%text]"
    }
  }
CCode
Note how we write Wcode(`\add(\acc++S", %s")')
instead of Wcode(`\add(S"%\acc, %s")') since 
 string interpolation does not currently support the Wcode(`\').

WBR
Write a static method Wcode(`FilterUpTo{ class method S.List(S.List that, I size)}') filtering out from a list of strings the ones longer
than \Q@size@.
For example 
Wcode(`FilterUpTo(S.List[S"a";S"ab";S"abc"] size=2I)==S.List[S"a";S"ab"]')
WP
Precondition: Wcode(size) is not negative
WP
Solution: 
OBCode
FilterUpTo = {
  class method
  S.List (S.List that, I size) = (
    X.Pre[size >= 0I]
    S.List()(for s in that if s.size()<= size \add(s))
    )
  }
CCode
Again we see yet another way to handle errors; preconditions are appropriate when it is an observed  bug if the user calls it with wrong parameters.

WTitle((3/5) Read/Write files)

Write a static method Wcode(ReadSelf) returning the content of the file where the current source code is located.
WBR
To read and write files we need to load a Wcode(FileSystem) library
{}:Test""(expected=ReadSelf(FileSystem.Real.#$of())
    actual=S"reuse [AdamTowel]%S.nl()MaxOfList = [###]")
OBCode
FileSystem = Load:{reuse[FileSystem]}

ReadSelf = {
  class method
  S (mut FileSystem that) = (
    S s=that.read(fileName=S"This.L42")
    Debug(S"[[%s]]")
    s
    //_=f.write(fileName=S"Example1.txt",content=S"SomeContent")
    )
  }
CCode
WTitle((5/5) Examples summary)
<ul><li>
Always think about what can go wrong upfront
</li><li>
Many methods can be completed by first checking for 
errors/issues and then using a Wcode(for), possibly inside a Wcode(reduce()) or a Wcode(Match).
</li><li>
Before heading into a problem,
spend some time to define your problem domain.
We dodged a lot of headaches by defining
points with invariants.
</li></ul>