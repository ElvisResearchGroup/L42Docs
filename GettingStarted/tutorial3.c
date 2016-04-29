WBigTitle(Units of Measure)

WTitle((1/5) Bases and Units)
AdamTowel define
many common bases:
<ul>
<li>Wcode(Int):
infinite precision positive and negative integer numbers
</li>
<li>Wcode(Double):
double precision positive and negative floating points numbers
</li>
<li>
Wcode(Q): infinite precision positive and negative fractions
</li>
<li>
Wcode(Int64): 64 bits numbers
</li>
</ul>
Other types of bases, that could be called Wcode(Int32),
Wcode(Int16),
Wcode(Int8),
Wcode(Float)
are available as separate libraries that can be imported.

The class 
Wcode(Units)
offers methods to create units out of those bases.
For example
OCode
Meter:Units.of(Q)
Kg:Units.of(Q)
res=Meter[6Meter;1Kg;div:2Kg]// 6m*1kg/2kg
//res == 3Meter
CCode
Where the square brackets allows to 
compose different kinds of units.
We can also declare composite units and
aliasing units:

OCode
Meter2:Units[Meter;Meter]
Cm:Units.alias(0.01Meter)
Meter height=178Cm
res=Meter2[height;3Meter]
CCode

Note how height is of type Wcode(Meter).
Alias units are just shortcut to declare values of
the original unit.

WBigTitle(Collections and Data classes)

A very large class of practically useful programs can be obtained by
just declaring simple minded classes using Data and Collections.

WTitle(Libraries)


WBigTitle(not ready to read yet Libraries)

WTitle(Libraries)
42 stands for the primate of libraries, so let see some libraries in action.
We have already see how to chose a towel, and many classes that are likely to be present in such towel, like
Wcode(S) and Wcode(Data).
Let see now how to load a library from its url:
OCode
{reuse L42.is/AdamTowel
Gui: Load<<{reuse L42.is/Gui}
Main:{
  Gui.alert(S"hi!")
  return ExitCode.success()
  }
}
CCode
Load is another decorator, here it modifies the library found in L42.is/Gui so that it can be used easly from AdamTowel.
 <!--
    //Meter dx=this.x()-that.x();
    //Meter dy=this.y()-that.y();
    //m2=Meter2[dx;dx]+Meter2[dy;dy]
    //return Meter[root:2Unit of:m2]
    //dx=dx.unit()
    //Meter.fromUnit(((dx*dx)+(dy*dy)).sqrt())
 -->	
OCode
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
OCode
Points:Collections.mutList(Point)

...

zero=Point(x=0Int, y=0Int)
one=Point(x=1Int, y=1Int)
ps0=Points[]//the empty list
ps1=Points[zero;one]//contains zero, one
ps2=Points[zero;one]//contains zero, one

CCode



<!--OCode Library myCode={ method Int foo()} CCode
A local binding of type Library initialized with a class with a single Wcode(foo()) method. Since there is no body, it is an abstract method.
 -->	 

 <!--WTitle(Simpler complete program)

Let now starts showing the simplest 42 program: an empty library.

OCode {} CCode

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
Wcode(MyCode.hello(..)) use directly the Wcode(MyCode) class instance as receiver. We can also give it a name 
e se vuoi puoi anche salvarlo su un binding locale, tipo x=MyCode  x.hello(...)
-->
