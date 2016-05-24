WBigTitle(Exercises)

A very large class of practically useful programs can be obtained
just by declaring
basic classes, collections
and simple Data classes.

Let's see some exercises and solutions 
to understand better how 42 code looks like

WTitle((1/5) Max method)
Write a class method Wcode(max) returning the max from a list of numbers
WP
Solution: 
OBCode
UndefinedOnEmpty: Message.$ <>< {implements Guard}
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
    //we could write 'that.withoutLeft().vals()'
    //to avoid cheching on the first again
    if maxCandidate<n (maxCandidate: =n)
    //update the variable to keep track of the max so far.
    )
  return maxCandidate
  }
CCode

WTitle((2/5) Merge two lists of strings)
Write a class method map producing a string from to lists of strings of the same length.
 For example
Wcode(`map(keys: Strings[S"a";S"b";S"c"],vals: Strings[S"z";S"y";S"z"])')
should produce Wcode(`S"[a->z, b->y, c->z]"')
WP
Solution: 
OBCode
UnequalSize: Message.$ <>< {implements Guard}
class method
S map(Strings keys, Strings values) {
  if keys.size() != values.size() (error UnequalSize
    "keys="[keys.size()]", values="[values.size()]"" )
  //the former formatting allows us to keep a whole line for the error message
  return S"["[with k in keys.vals(), v in values.vals() (
    use[k++S"->"++v, sep: S", "]
    )]"]"
  }
CCode

WTitle((3/5) Filtering)
Write a Wcode(`class method Strings upTo(Strings that, Size size)') that filters out from a list of strings the ones longer
than size.
For example 
Wcode(`upTo(Strings[S"a";S"ab";S"abc"],size: 2Size)==Strings[S"a";S"ab"]')
WP
Precondition: size is not negative
WP
Solution: 
OBCode
class method
Strings upTo(Strings that, Size size) ( //no need of '{..return..}' for simple methods
  Assert.Pre[size>=0Size]
  Strings[with s in that.vals() (  if s.size()<=size (use[s]) )]
  )
CCode

WTitle((4/5) Random mole)
For a longer example, represent a piece of land as a 80*80 bi-dimensional vector,
where every cell can be full of dirt (90%) or rock (10%).
Then a mole start from the left top corner and attempts to
digs through dirt randomly.
After 100 steps the mole stops.
declare the opportune classes and write a randomDig
method. 
WP
You can use the library Wcode(L42.is/Random)
for pseudo randomness. You can use Wcode(Range(stop)) to iterate over
all (Wcode(Size)) numbers from 0 to Wcode(stop)-1 included.
WP
A possible solution: 
OBCode
Random: Load <>< {reuse L42.is/Random}

Cell: Enumeration"dirt, rock, empty, mole"

Direction: Enumeration"up, down, left, right"

Cells: Collections.vector(of: Cell)

Point: Data <>< {implements Concept.Invariant
  Size x, Size y

  method invariant()
    x>=0Size & x<80Size & y>=0Size & y<80Size

  This go(Direction that) {
    if that.isUp() (return this.with(x: \-1Size))
    if that.isDown() (return this.with(x: \+1Size))
    if that.isLeft() (return this.with(y: \-1Size))
    Assert.Holds[that.isRight()]
    return this.with(y: \+1Size)
    catch error Concept.Invariant.Failure err  (return this)
    }
  }

Land: Data <>< { //we may want to put the field and the predefined factory private;
  //we will learn how to do that later.
  mut Cells cells

  class method
  mut This ()
    This(cells: Cells[
      with i in Range(stop: 80Size*80Size) (
        if Random(10Size)==0Size (use[Cell.rock()])
        else (use[Cell.dirt()])
        )
      ])   

  //implementation of the matrix as an example,
  //in good 42 code should be imported from a library
  mut method 
  Void set(Point that, Cell val)
    this.#cells()(that.y()*80Size+that.x(), val: val)
    
  read method 
  Cell get(Point that)
    this.#cells().val(that.y()*80Size+that.x())    
  
  
  mut method
  Void randomDig() (
    var Point current=Point(x: 0Size,y: 0Size)
    with i in Range(stop: 100Size) (
      this.set(current,val: Cell.empty())
      d=Direction.from(index: Random(4Size))
      newPoint=current.go(d)
      if !this.get(d).isRock() ( //no digging in rock
        current: =newPoint
        )
      )
    this.set(current,val: Cell.mole()) //finally, the mole is where we ends up
    )
  
  toS() S""[with x in Range(stop: 80Size) (
      use[S.nl()] //newline
      with y in Range(stop: 80Size) (
        p=Point(x: x,y: y)
        if this.get(p).isRock() (use[S"#"])
        else if this.get(p).isDirt() (use[S"%"])
        else if this.get(p).isEmpty() (use[S" "])
        else (Assert.Bug[this.get(p).isMole()] use[S"M"])         
      ))]""++S.nl()
  //since we define it explicitly, Data will leave it alone : )
  }
CCode


WTitle((5/5) Examples summary)
<ul><li>
Always think about what can go wrong upfront
</li><li>
May methods can be completed by first checking for 
errors/issues and then using a Wcode(with)
</li><li>
Before heading into a problem,
spend some time to define your problem domain.
We dodged a lot of headaches by defining
points with invariants.
</li></ul>

Some may argue that in a real object oriented implementation,
directions and cells should be interfaces with subtypes;
so that we can use single dispatch to
avoid the cascades of ifs.
We have mixed feelings about this: 
shorter code is better and more maintainable then longer code, and the version with subtyping would have been much longer.

The crucial point is that the 'random mole' code is not designed to be used by other programmers as a library.
Libraries should have well modularize code,
and provide convenient hooks for adaptation.
Metaprogramming and interfaces are the right tool for this task.

We should not confound adaptability (without touching the original source, make it takle a new problem), with maintenability (change the original source to keep it up to date
with the ever-changing set of requirements).