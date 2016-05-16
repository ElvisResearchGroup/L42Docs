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