WBigTitle(Exercises)

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
  UndefinedOnEmpty: Message:{[Message.Guard]}
  //MaxOfList is undefined on empty lists.
  //Since there was no mention of preconditions, we should explicitly handle all the error cases (as guards)
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
Defining your own error may still produce more readable errors, so feel free to mix and match the two approaches as show in the next exercise


WTitle((2/5) Merge two lists of strings)
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
      "keys= %keys.size(), values= %values.size()" 
    //the former formatting allows us to keep a whole line for the error message
    res = S.List()(for k in keys, v in vals \add(S"%k->%v"))
    if res.isEmpty() return S"[]"
    return S"["++res.reduce()(for s in \vals \add(S", %s"))++S"]"
    }
  }
CCode

WTitle((3/5) Filtering)
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
CCode
Again we see yet another way to handle errors; preconditions are appropriate when it is an observed  bug if the user calls it with wrong parameters.

WTitle((4/5) Random mole)
For a longer example, represent a piece of land as a 80*80 bi-dimensional vector,
where every cell can be full of dirt (90%) or rock (10%).
Then a mole start from the left top corner and attempts to
digs through dirt randomly.
After 100 steps the mole stops.
define the opportune classes and write a Wcode(randomDig)
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
    this.x()>= 0Size & this.x()<80Size & this.y()>= 0Size & this.y()<80Size

  This go(Direction that) {
    if that.isUp() return this.with(x: \-1Size)
    if that.isDown() return this.with(x: \+1Size)
    if that.isLeft() return this.with(y: \-1Size)
    X[that.isRight()] return this.with(y: \+1Size)
    catch error Concept.Invariant.Failure err  (return this)
    }
  }

Land: Data <>< { //we may want to put the field and the predefined factory private;
  //you can search in the documentation of Data how to do it.
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
    var Point current= Point(x: 0Size,y: 0Size)
    with i in Range(stop: 100Size) (
      this.set(current,val: Cell.empty())
      d= Direction.from(index: Random(4Size))
      newPoint= current.go(d)
      if !this.get(d).isRock() ( //no digging in rock
        current:= newPoint
        )
      )
    this.set(current,val: Cell.mole()) //finally, the mole is where we ends up
    )
  
  toS() S""[with x in Range(stop: 80Size) (
      use[S.nl()] //newline
      with y in Range(stop: 80Size) {
        p= this.get(Point(x: x,y: y))
        if p.isRock() return use[S"#"]//common pattern: with {return use[..]}
        if p.isDirt() return use[S"%"]//use[..] return void, so is ok
        if p.isEmpty() return use[S" "]//as final result of a with block
        X[p.isMole()] return use[S"M"]         
      })]""++S.nl()
  //since we define 'toS()' explicitly, Data will leave it alone :)
  }
CCode


WTitle((5/5) Examples summary)
<ul><li>
Always think about what can go wrong upfront
</li><li>
Many methods can be completed by first checking for 
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

We should not confound adaptability (without touching the original source, make it takle a new problem), with maintenability
 (it is easy to change the original source to keep it up to date
with the ever-changing set of requirements).