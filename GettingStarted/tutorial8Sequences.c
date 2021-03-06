WBigTitle(`Collection: list, map, set, optional and their operations')

WTitle((1/5)lists manipulation)

As we have seen before, lists can be defined using Wcode(Collection.list(that)), as in the example below.

OBCode
Nums: Collection.list(Num) //declaration for a list of nums
/*..*/
xs0 = Nums[10\;20\;30\] //xs0 <: mut Nums
Nums xs1 = Nums[10\;20\;30\] //xs1 <: imm Nums == //xs1 <: Nums
imm xs2 = Nums[10\;20\;30\] //xs2 <: imm Nums == //xs2 <: Nums
xs3 = Num.List[10\;20\;30\] //xs3 is another kind of list of nums
CCode
As you can see above, many of the most common Wcode(AdamsTowel) classes have a nested class
called Wcode(.List) as a convenience feature, to avoid having to define your own in most programs.

WBR

Lists can be created with square brackets; they are born mutable but can become immutable 
if they are directly assigned to an immutable local binding or parameter, or by other forms of promotion; for example,
a method without Wcode(mut) parameters returning a 
Wcode(mut) reference
can be used to initialize an immutable binding.
You need to specify the type of the local binding to force the promotion.
WBR
For example: 

OBCode
Nums myNums= DoIt.getMutableNums() //ok promotions happens, myNums is immutable

myNums= DoIt.getMutableNums() //myNums type is inferred to be mut Nums
CCode

Immutable lists can be combined with operators.
The general idea is that operators 
Wcode(`+,-') works 
on one sequences and one element,
while the corresponding doubled-up operators
Wcode(`++,--')
works on two sequences.
You can see the details of this below.
OBCode
X[
  //element addition
  Nums[a;b;c]+d == Nums[a;b;c;d];
  //element addition works both ways
  k+Nums[a;b;c]+d == Nums[k;a;b;c;d];
  //sequence concatenation
  Nums[a;b]++Nums[c;d] == Nums[a;b;c;d];
  //element removal
  Nums[a;b;b;c]-b == Nums[a;c]; //how to tell two elements apart?
  //sequence subtraction (like removeAll / set minus)
  Nums[a;b;b;c]--Nums[b;c] == Nums[a];
  ]
CCode
In addition of operators, immutable lists also supports a pletora of methods:
OBCode
X[
  //replacement
  Nums[a;b;c;d].with(2I,val=e) == Nums[a;b;e;d];
  Nums[a;b;c;d].withLeft(e) == Nums[e;b;c;d];//==Nums[..].with(0I val=e)
  Nums[a;b;c;d].withRight(e) == Nums[a;b;c;e];//==Nums[..].with(\size-1I val=e)
  //insertion
  Nums[a;b;c;d].withAlso(2I,val=e) == Nums[a;b;e;c;d];
  Nums[a;b;c;d].withAlsoLeft(e) == Nums[e;a;b;c;d];//==Nums[..].withAlso(0I val=e)
  Nums[a;b;c;d].withAlsoRight(e) == Nums[a;b;c;d;e];//==Nums[..].withAlso(0I val=e)
  //skipping/filtering
  Nums[a;b;c;d].without(index=2I) == Nums[a;b;d];
  Nums[a;b;c;d].withoutLeft() == Nums[b;c;d];//==Nums[..].without(0I)
  Nums[a;b;c;d].withoutRight() == Nums[a;b;c];//==Nums[..].without(\size-1I)
  Nums[a;b;c;b;d].withoutAll(val=b) == Nums[a;c;d];
  Nums[a;b;c;b;d].withoutLeft(val: b) == Nums[a;c;b;d]; //filter out the leftmost b
  Nums[a;b;c;b;d].withoutRight(val: b) == Nums[a;b;c;d]; //filter out the rightmost b
  ]
CCode
As you notice, there are different kind of actions: 
replace an element (Wcode(with)),
insert an element (Wcode(withAlso))
and skipping/filtering elements out (Wcode(without)).
Then, elements can be specified by index, by being
the leftmost or the rightmost.
To filter elements
out, you can also just provide the element.

WP


Immutable collections (and also mutable ones, as we will see later)
can be accessed with the following methods: 

OBCode
X[
  //access
  Nums[a;b;c;d].left() == a;
  Nums[a;b;c;d].right() == d;
  Nums[a;b;c;d].val(2I) == c;
  Nums[a;b;c;d].size() == 4I;
  !Nums[a;b;c;d].isEmpty();
  ]
CCode

WTitle(`Mutate sequences')

Mutable sequences can contains mutable objects.
While this can be useful in special circumstances, it can create aliasing issues similar to the
ones of the animals example of before.
To warn against such issues, methods Wcode(left()), Wcode(right()) and Wcode(val(that)) return 
Wcode(read) references to mutable objects. In order to obtain 
a Wcode(mut) reference, the user need to use the methods
Wcode(`#left()'),
 Wcode(`#right()')
 and Wcode(`#val(that)').

WP

Mutable sequences can be more efficient that 
immutable ones, and are more general, since they 
can store mutable objects.
WBR


Now we show some methods over a mutable list Wcode(`foo = Nums[a;b;c;d]'); consider each following line independently: 
OBCode
//setting a value in a position
foo.set(2I val=e) //foo == Nums[a;b;e;d]
//setting at left or right
foo.left(e) //foo == Nums[e;b;c;d]
foo.right(e) //foo == Nums[a;b;c;e]

//add a value in a position
foo.add(2I val=e) //foo == Nums[a;b;e;c;d]

//add at left or right
foo.add(left=e) //foo == Nums[e;a;b;c;d]
foo.add(right=e) //foo == Nums[a;b;c;d;e]

//removal
foo.remove(2I) //foo == Nums[a;b;d]
foo.removeLeft() //foo == Nums[b;c;d]
foo.removeRight() //foo == Nums[a;b;c]

//removal
foo.removeAll(val=b) //foo == Nums[a;c;d]
foo.removeLeft(val=b) //remove the leftmost b
foo.removeRight(val=b) //remove the rightmost b
CCode


WTitle((2/5) `Iterations on lists')

We now show various pattens to iterate on lists.
First some usual foreach:
OBCode
vec = S.List[S"foo"; S"bar"; S"beer"]
var S result = S""
for myElem in vec ( result++=myElem ) 
//result==S"foobarbeer"
CCode

In 42 foreach allows to iterate on multiple collections at onece, and also to update the collections:
OBCode
rs = Nums[1\;2\;3\;]
as =  Nums[10\;20\;30\;]
bs =  Nums[100\;200\;300\;]
for a in as, b in bs, var r in rs ( r:= r+a+b )
//now rs==Nums[111\;222\;333\;]
CCode

In the example above, a dynamic error would be raised if 
Wcode(rs),
 Wcode(as) and
 Wcode(bs) have different length.
This behaviour can be tuned in many way: 
iterators can be parametrized with
Wcode(cut()) , Wcode(vals(that,to)) and Wcode(cutVals(that,to)):
Wcode(cut()) allows to skip extra elements, while Wcode(vals(that,to))
allows iterate on a subsequence only.

WBR
Let see some examples: 
OBCode
for x in xs.cut(), y in ys ( .. )
//will iterate as long as xs, even if ys is longer.
//will stop after xs.size() cycles, and fail if xs.size()>ys.size()

for x in xs.vals(5I to 10I), y in ys ( .. )
//will iterate across xs elements from position 5 to 10, pairing them up with elements 0-5 of ys.
//will stop after 5 cycles, and go in error is ys.size()!=5 or xs.size()<10

for x in xs.cut(), y in ys.cut() ( .. )
//will iterate for as long as both xs and ys have elements.
//similar to a functional zip

rs = Nums[1\;2\;3\;]
imm as =  Nums[10\;20\;30\;40\;50\;]
imm bs =  Nums[100\;200\]
for a in as.cut(), b in bs+300Num, var r in rs ( r:= r+a+b )
//rs==Nums[111\;222\;333\;]
CCode

WTitle(`The power of the \')

There are varius methods taking advantage of the Wcode(\) syntactic sugar.
They provides an expressive power similar to what list comprehensions provides in python and streams in Java, but by just using simple control flow like for/if:

OBCode
as =  Nums[1\;2\;3\;4\;]
//mapping
bs0 = Nums[](for a in as \add(a*10Num))
//bs0==Nums[10\;20\;30\;40\]
//filtering
bs1 = Nums[](for a in as if a>2Num \add(a))
//bs1==Nums[3\;4\]
//flatmapping
bs2 = Nums[](for a in as for b in bs0 \add(a+b))
//bs0==Nums[11\;21\;31\;41\;12\;22\;32\;42\;13\;23\;33\;43\;14\;24\;34\;44\;]
//reduce
tot0 = as.reduce()(for v in \vals \add(\acc+v)) //would go in error if as.isEmpty()
tot1 = as.reduce(0\)(for v in \vals \add(\acc+v)) //ok also if as.isEmpty()
//tot0 == tot1 == 10Num

//checks a property; great in an if or an X[]
ok0 = Match.Some()(for a in as \add(a>3Num))
//ok0==true
ok1 = Match.All()(for a in as \add(a>3Num))
//ok1==false
ok2 = Match.None()(for a in as \add(a>3Num))
//ok2==false
ok3 = Match.Count()(for a in as \add(a>3Num))
//ok3==2I
asContainsAllBs = Match.All()(for b in bs \add(b in as))
asIntersectBs = Match.Some()(for b in bs \add(b in as))
asDisjointBs = Match.None()(for b in bs \add(b in as))
//Note: b in as == as.contains(b)

CCode
The language 42 is expression based. Expressions that look like statements are just expressions with the
Wcode(Void) return type.
Those methods that take advantage of the Wcode(\) are simply methods with a single parameter 
Wcode(Void that).
The Wcode(\add(that)) method in the 
Wcode(Match.**) examples short circuit when appropriate, so that the for can terminate as soon as the result is known.


WTitle((3/5) `Lists with mutable and immutable elements')

Up to now we focused on lists of Wcode(Num), but 
all instances of Wcode(Num) are immutable; we now discuss what happens where
mutable lists contains a mixture of mutable and immutable elements.
Consider the following code:
Finally, we conclude discussing iteration on mutable elements:
Mutable lists can contains
OBCode
Point = Data:{var Num x, var Num y}
Points = Collection.list(Point)
..
imm p0=Point(x=0\,y=0\) //an imm point
p1=Point(x=1\,y=1\) //a mut point
ps=Points[p0;mutVal=p1] //a mut list with both points
X[
  p0==ps.val(0\);
  p0==ps.left();
  p1.readEquality(ps.#val(1\)); //== only works on imms,
  p1.readEquality(ps.#right()); //readEquality checks for structural equality
  p0.readEquality(ps.readVal(0\)); // .readVal generalizes over imm/mut values
  p1.readEquality(ps.readVal(1\));
  ]
for read p in ps ( Debug(p) )
CCode

As you can see, to insert a mutable point we need to use Wcode(mutVal) and to
take the point out we have to add the Wcode(`#') to the method.
When iterating on a list, if we expect a mixture of mut and imm values we must add Wcode(read)
to avoid a runtime error.
If we expect all values to be Wcode(mut), we can write Wcode(mut) instead.
When a mut collection is promoted to Wcode(imm), it still remembers what values were originally inserted as
 Wcode(mut).
To make it so that all values can be read as Wcode(imm), we can use the method
Wcode(.immNorm()). In addtion of normalizing the collection, it also marks all values
accessible as Wcode(imm), as shown in the code below:

OBCode
Points immPs=(
  imm p0=Point(x=0\,y=0\) //an imm point
  p1=Point(x=1\,y=1\) //a mut point
  Points[p0;mutVal=p1] //a mut list with both points
  ).immNorm()
for p in ps ( Debug(p) ) //works, it would fail without 'immNorm()'
CCode

WTitle((4/5) Map, set, opt..)

Wcode(Collection) also support maps, sets, optional and enumerations.
We will add more kinds of collections in the future.

WTitle(`Optional')
In 42 there is no concept of null, and all the values are always intentially initialized before
they can be read.
There are two main reasons programmers relies on nulls: optional values and circular/delayed initialization.
Circuar initialization can be solved with a Wcode(fwd) types, an advanced typing feature that we do not discuss here.
Optional values are a staple of functional programming and are logically equivalent to a collection of zero or one element, or, if you prefer, a box that may or may not contain an element of a certain type.
Optionals values can be obtained with Wcode(Collection.optional(that)) as shown below.
Optionals are also optimized so that they do not require the creation of any new objects at run time.

OBCode
Point = Data:{var Num x, var Num y}
OPoint = Collection.optional(Point)
Main = (
  imm p00=Point(x=0\ y=0\)//an imm Point in 00
  mut p01=Point(x=0\ y=1\)//a mut Point originally in 01
  var imm p00Box = OPoint(p00) //immutable Optional Point
  var mut p01Box = OPoint(p01) //mutable Optional Point
  X[
    p00 in p00Box; //the in syntax checks if an object is in the box
    p00Box.val()==p00; //Data defines == only for imm references
    p01Box.val().readEquality(p01);//here .val() gives us a read reference
    ]
  if p00Box ( Debug(S"printing %p00Box") )//printing [Point(x=0, y=0)]
  //we can just check if a box is not empty as if it was a boolean
  p01Box.#val().x(50\)//updates the x value of the point
  Debug(S"printing %p01")//printing Point(x=50, y=1)
  p00Box:= OPoint()//updates the local variables with empty boxes
  p01Box:= OPoint()
  if !p00Box ( Debug(S"printing %p00Box") )//printing []
    X[
    !(p00 in p00Box);
    !p00Box;//using isPresent() or not is just a matter of style
    !p00Box.isPresent();
    ]
  )
CCode
At this point in the tutorial, some readers will be confused that we can update the local variable binding 
Wcode(p00Box:= OPoint()) even if it is immutable.
Other readers instead will remember that immutability is a property of the reference and not of the binding/field: a local binding and fields declared Wcode(var) can be updated.
The updated value need to respect the modifier of the fild/binding type: if it is Wcode(mut/imm) it needs to be updated with another Wcode(mut/imm); if it is Wcode(read) than it can be updated with either
Wcode(mut), Wcode(imm) or Wcode(read).
Oh, yes, another reader will realize ... and a Wcode(capsule) reference can be assigned to any of Wcode(mut), Wcode(imm), Wcode(lent) or Wcode(read).

Note how both local bindings are updated using the same exact expression :
Wcode(p00Box:= OPoint()    p01Box:= OPoint())
In 42 Wcode(OPoint()) can be either Wcode(mut) or Wcode(imm) (or Wcode(capsule) indeed)
On the other side, consider 
Wcode(OPoint(p00)) and Wcode(OPoint(p01)): the first one is immutable since Wcode(p00) is Wcode(imm),
while the second one is mutable since Wcode(p01) is Wcode(mut).


WTitle(`Map')
Thanks to normalization 42 can have very fast and most reliable hash sets and hash maps.
The values of sets and the keys of maps must be immutable, and are normalized just before being intserted in the collection.
Then, the value of the normalized pointer is used to check for equality and hashcode.
This have various positive effects:

<ul><li>
The user does not need to write equality and hashing behaviour
</li><li>
There is no risk of mistake in the equality and hashing behaviour
</li><li>
The intrinsic invariants of the hashmap/hashset are never violated/corrupted.
</li><li>
The equality is a perfect structural equality, but is as fast as a pointer equality; for maps with large keys this can make a massive performance difference.
</li></ul>

Maps and sets have less methods that lists, but they can still be iterated upon, as shown in the following code:

OBCode
Point = Data:{var Num x, var Num y}
Points = Collection.list(Point)
PointToString = Collection.map(key=Point, val=S)
Roads = Collection.map(key=Point, val=Point)
Main = (
  map = PointToString[
    key=\(x=3\ y=4\), val=S"MyBase";
    key=\(x=0\ y=0\), val=S"Source";
    key=\(x=5\ y=8\), val=S"EnemyBase";
    ]
  for (key,val) in map ( Debug(S"%key->%val") )
  //we can use (..) to extract the key/val fields from PointToString.Entry
  //this iteration is straightforward since all values are imm
  roads = Roads[
    key=\(x=3\ y=4\), val=\(x=0\ y=0\); //immutable to immutable
    key=\(x=0\ y=0\), mutVal=\(x=0\ y=0\);//immutable to mutable
    key=\(x=5\ y=8\), mutVal=\(x=0\ y=0\);//immutable to mutable
    ]
  for read (key, val) in roads ( Debug(S"%key->%val") )
  //we add 'read' in front to be able to read mixed imm/mut values
  //if all the values were mutable, we could just add 'mut' in front
  )
  mut Roads.Opt?? optPoint = roads.#val(key=\(x=0\ y=0\)))
  optPoint.#val().x(50\)//update the field of the objectv inside the map
  CCode

As you can see, when objects are retrived from the map, we obtain an optional value; this is because statically we can not know if a key is mapped to a value or not.
WBR
In addition to conventional Wcode(size()) and Wcode(isEmpty()),
maps offers the following methods:
<ul><li>
To extract a value using the key:
Wcode(val(key)), Wcode(#val(key)) and Wcode(readVal(key)); to extract an optional
Wcode(imm), Wcode(mut) or a Wcode(read) reference, respectivelly.
As for lists, it is always safe to extract a Wcode(read) reference. An empty optional will be produced when attempting to extract as Wcode(imm/mut) a value that was inserted as Wcode(mut/imm) instead, so to relayably ask if a key is contained in the map we should write Wcode(map.readVal(key=myKey).isPresent()).

</li><li>
Mutable maps can be modified by
inserting immutable values with Wcode(put(key,val)) and mutable values with Wcode(#put(key,val)).
Finally, an association can be removed using Wcode(remove(key)).
</li><li>
Wcode(Collection.map(that)) creates a class remembering the insertion order.
This is needed to make the iteration deterministic.
The keys can be retreived with their order using
Wcode(key(that)) passing the desired Wcode(I index), from zero to Wcode(\size-1I)
The corresponding value can be retreived by methods Wcode(val(that)), Wcode(#val(that)) and Wcode(readVal(that)) 
to extract a
Wcode(imm), Wcode(mut) or a Wcode(read) (not optional) reference to the value, respectivelly.
</li></ul>
     

WTitle(`Set')
Sets behaves a lot like maps where the values are irrelevant, and have differently named methods.
In particular, in addition to conventional Wcode(size()) and Wcode(isEmpty()),
sets offers method Wcode(add(that)) and Wcode(remove(that)) to add and remove an element,
and elements can be extracted in the insertion order by using method Wcode(val(that))

We are considering adding operators Wcode(+,-,++,--) as for lists, and
support for the Wcode(\) like for lists.
An operator Wcode(&&) returning the intersection of the two sets.
On the other side, boolean methods like Wcode(intersect(that)) Wcode(disjoint(that)) and Wcode(containsAll(that)) can already be easily emulated with Wcode(Match) as we shown for lists.
WTitle(`(5/5) Collection summary')

<ul><li>
There are a tons of methods and operators to know, but since most code works 
around collections, it is worth the effort to memorize them.
</li><li>
Immutable collections are easy to play with, using operators and Wcode(with**) methods.
</li><li>
Mutable collections can be more efficient and flexible, but they come with additional
difficulties.
</li><li>
Most methods have a general version that works with an index, and specialized Wcode(left) and
Wcode(right) variants.
</li><li>
Wcode(\) can help remove a lot of boilerplate, but is a concept unique to 42, and require some effort to get used to.
</li><li>
Wcode(for) is very useful and flexible. It is common to find methods composed from just a large
Wcode(for) statement plus a little pre and post processing around it.
</li></ul>