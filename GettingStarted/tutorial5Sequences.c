WBigTitle(`High level sequences manipulation')

WTitle((1/5)Vectors as Sequences, listing the basic operations)

As we have seen before, vectors can be declared using Wcode(Collections.vector(of)), as in the example below.

OBCode
Nums:Collections.vector(of:Num) //declaration for vectors of nums
/*..*/
Nums xs=Nums[10Num;20Num;30Num] //immutable vector
//note that we declare the type of 'xs' explicitly,
//otherwise the vector would be mutable.
CCode

Sequences can be created with square brackets,
and can be combined with operators.
The general idea is that operators 
Wcode(+,-,<,>) works 
on one sequences and one element,
while the corresponding doubled-up operators
Wcode(++,--,<<,>>, >>=,<<=,==)
works on two sequences.
You can see the details of this below.
OBCode
//element addition
Nums[a;b;c]+d==Nums[a;b;c;d]
//sequence concatenation
Nums[a;b]++Nums[c;d]==Nums[a;b;c;d]
//element removal
Nums[a;b;b;c] - b==Nums[a;c] //only if elements implements Concepts.Equals
//set subtraction
Nums[a;b;b;c] -- Nums[b;c]==Nums[a] //same for all the operators under
//set intersection
Nums[a;b]& Nums[b;c]==Nums[b]
//superset
Nums[a;b;c]>>Nums[a;c] //holds
//superseteq
Nums[a;b]>>=Nums[a;c] //holds but [a;b]>>Nums[a;c] does not
//contains element
Nums[a;b]>b //holds
//is element contained
b<Nums[a;b] //holds
CCode
In addition of operators, many sequences can
be manipulated by the following methods:

OBCode
//replacement
Nums[a;b;c;d].withLeft(e) == Nums[e;b;c;d]
Nums[a;b;c;d].withRight(e) == Nums[a;b;c;e]
Nums[a;b;c;d].with(2Size,val:e) == Nums[a;b;e;d]

//insertion
Nums[a;b;c;d].withAlsoLeft(e) == Nums[e;a;b;c;d]
Nums[a;b;c;d].withAlsoRight(e) == Nums[a;b;c;d;e]
Nums[a;b;c;d].withAlso(2Size,val:e) == Nums[a;b;e;c;d]

//skipping/filtering
Nums[a;b;c;d].without(index:2Size) == Nums[a;b;d]
Nums[a;b;c;d].withoutLeft() == Nums[b;c;d]
Nums[a;b;c;d].withoutRight() == Nums[a;b;c]

//filtering, if elements implements Concepts.Equals
Nums[a;b;c;b;d].withoutAll(val:b) == Nums[a;c;d]
Nums[a;b;c;b;d].withoutLeft(val:b) == Nums[a;c;b;d]//filter out the leftmost b
Nums[a;b;c;b;d].withoutRight(val:b) == Nums[a;b;c;d]//filter out the rightmost b
CCode
As you notice, there are different kind of actions:
replace an element (Wcode(with)),
insert an element (Wcode(withAlso))
and skipping/filtering elements out (Wcode(without)).
Then, elements can be specified by (Wcode(index)), by being
the leftmost or the rightmost. To filter elements
out, you can also just provide the element.

WP


Immutable collections (and also mutable ones, as we will see later)
can be accessed with the following methods:

OBCode
//access
Nums[a;b;c;d].left() //a
Nums[a;b;c;d].right() //d
Nums[a;b;c;d].val(2Size) //c
Nums[a;b;c;d].size() //4Size
Nums[a;b;c;d].isEmpty() //does not hold
CCode





WTitle((2/5) `Suggested parameter values using "\"')

In 42 is possible to use 'Wcode(\)' while calling a method or using the square brackets,
to ask the receiver for a suggestion about the parameter values.
The library designer has full freedom to implement those suggestion in the most opportune way, however we
recognize three important common patterns:
WP
When setting/updating a value, the old value is suggested.
WP
When adding a new value, the factory is suggested.
WP
When the parameter is a number from zero to a max, the maximum is suggested.
WP
For example:
OBCode
Nums[a;b;c;d].withAlso(left:42\) //the \ is Num
Nums[a;b;c;d].without(index:\ - 1Size) //remove the last (the right-most)
Nums[a;b;c;d].with(left:\ * 2Num) //the leftmost is now a*2

Points[\(x:12\ y: 0\)]==Points[Point(x:12Num, y:0Num)]
CCode
Sometime, using Wcode(\) makes a huge difference,
for example, for the animal example of before:
OBCode
horse.location(\.with(x:\+20\)
//is equivalent to the much longer
horse.location(horse.location().with(
  x:horse.location().x()+20Num)
CCode

Is also possible to use Wcode(\) followed by an identifier, that will denote the method
with the same name on the receiver.
For example, if we want to reflect a point, and invert x and y coordinate, we can write

OBCode
p=point.with(x:\y, y:\x)
//is equivalent to
p=point.with(x:point.y(), y:point.x())
CCode

The Wcode(\) is very convenient also while initializing list/set of enumerated values.
For example

OBCode
Direction=Enumeration"north, east, south, west"
Direction.Set[\north;\east] //the bitflag corresponding
//to the set of north and east.
//is equivalent to the much longer
Direction.Set[Direction.north();Direction.east()]

p=point.with(x:point.y(), y:point.x())
CCode








WTitle((3/5) `Mutable sequences')


Mutable sequences can be more efficient that 
immutable ones, and are more general, since they 
can handled mutable objects.
The square brackets creates mutable sequences/collections,
so 

OBCode
foo=Nums[a;b;c;d]
//equivalent to 
mut Nums foo=Nums[a;b;c;d]
CCode


Now we show some methods over mutable collections, consider each following line independently:
OBCode
//setting a value in a position
foo(2Size,val:b) //foo.equals(Nums[a;b;b;d])
//setting at left or right
foo.left(b) //foo.equals(Nums[b;b;c;d])
foo.right(b) //foo.equals(Nums[a;b;c;b])

//add a value in a position
foo.add(2Size,val:b) //foo.equals(Nums[a;b;b;c;d])

//add at left or right
foo.add(left:b) //foo.equals(Nums[b;a;b;c;d])
foo.add(right:b) //foo.equals(Nums[a;b;c;d;b])

//removal
foo.remove(index:2Size) //foo.equals(Nums[a;b;d])
foo.removeLeft() //foo.equals(Nums[b;c;d])
foo.removeRight() //foo.equals(Nums[a;b;c])

//removal, if elements implements Concepts.Equals
foo.removeAll(elem:b) //foo.equals(Nums[a;c;d])
foo.removeLeft(elem:b) //remove the leftmost b
foo.removeRight(elem:b) //remove the rightmost b
CCode



Mutable collections can become immutable by promotion; for example,
a method without mutable parameters returning a mutable collection 
can be used to initialize a mutable one.
You may need to explicit the type of the local binding to force the promotion.
For example

OBCode
Nums myNums=DoIt.getMutableNums() //ok promotions happens, myNums is immutable

myNums=DoIt.getMutableNums() //myNums type is inferred to be mut Nums
CCode

Mutable collections can contains mutable objects.
While this can be useful in special circumstances, it can create aliasing issues similar to the
ones of the animals example of before.
To warn against such issues, methods Wcode(left()), Wcode(right()) and Wcode(val(that)) return 
readable references of the mutable objects. In order to obtain 
the actual mutable reference, the user need to use the methods
Wcode(`#left()'), Wcode(`#right()') and Wcode(`#val(that)').



WTitle((4/5) `Wcode(with): a Swiss army knife to encode complex behaviour')


There are two basic usage for the Wcode(with) statement: as for-each and as a typecase.

OBCode
//as a for each
vec= Strings[S"foo"; S"bar"; S"beer"]
var S result=S""
with myElem in vec.vals() (result:=result++myElem) //like for(myElem:vec){..}
//result==S"foobarbeer"

//as a typecase
with myData=foo.bar() ( //like a typecase/switch/chain of instanceof
  on S  Debug(S"A string "++myData) //print strings
  on Num  void //do nothing if is a number
  on Concepts.ToS Debug(myData) //print stringables that are not numbers. 
  )
CCode

if Wcode(myData) is already declared one can simply write
OBCode
with myData (
  on S Debug(myData)
  default Debug(S"Your data is not a string")
  )
CCode

Those two modes can be combined
OBCode
vec= Anys[S"foo"; 12Num; S"beer";]
var S result=S""
with myElem in vec.vals() (on S  result:=result++myElem  ) 
//result==S"foobeer", composed by all strings inside vec
CCode

Wcode(with) can be used as list comprehension
OBCode
vec=Anys[S"foo"; 12Num; S"beer";]
v=Strings[with myElem in vec.vals() (on S use[myElem] )] //filter non strings
//v==Strings[S"foo"; S"beer";]
CCode

for multiple dispatch:

OBCode
method Num m(Shape x, Person y, Vehicle z) //example of method using with
 with x y z (
 on Square Student Car (..return ..) //x here is a Square
 on Circle Person Airplane (..) //x here is a Circle
 default (..) //default case, here x is just a Shape
  )}
CCode

Or to iterate over multiple collections at once
OBCode
rs=Nums[1\;2\;3\;]
as= Nums[10\;20\;30\;]
bs= Nums[100\;200\;300\;]
//here a, b and r iterate over my data
with a in as.vals(), b in bs.vals(), var r in rs.vals() (r:=r+a+b)
//now rs==Nums[111\;222\;333\;]
CCode

While iterating on multiple collections, a dynamic error is usually raised if 
Wcode(rs),
 Wcode(as) and
 Wcode(bs) have different length.
This behaviour can be tuned in many way:
iterators can be parametrized with
Wcode(from:) , Wcode(to:), Wcode(fill:) and 
Wcode(stopOk:) (I need a better name for this last one).

Wcode(from:) and Wcode(to:) are on default zero and the sequence length,
and can be specified to start from another value, like 1 or 2, and
to end before the very end.

Wcode(fill:) can be used in combination with 
Wcode(from:) and Wcode(to:)
to iterate outside of the sequence range.
If Wcode(fill:) is used without specifying Wcode(to:), the
sequence is considered infinite and iteration may go on forever.

Wcode(stopOk:) is useful when multiple collections are iterated at once,
and specify the minimal allowed iteration cycles (suggested value is 0).
Let see some examples:
OBCode
with x in xs.vals(), y in ys.vals(fill:10N)
//will iterate for as long as xs, even if ys is shorter
//will stop after xs.size() cycles, and fail if xs.size()<ys.size()

with x in xs.vals(), y in ys.vals(fill:10N stopOk:\)
//will iterate for as long as xs, even if ys is shorter
//will stop after xs.size() cycles

with x in xs.vals(stopOk:\), y in ys.vals(stopOk:\)
//will iterate for as long as both xs and ys have elements

with x in xs.vals(fill:10N), y in ys.vals(fill:10N)
//will go on forever

with x in xs.vals(stopOk:5N), y in ys.vals(from:1N to: \ - 1N stopOk:3N )
//will extract at least 5 elements from xs, will skip the first 
//and the last element of ys and extract at least 2 elements from ys.
CCode


And, in a case similar of before:
OBCode
rs=Nums[1\;2\;3\;]
as= Nums[10\;20\;30\;40\;50\;]
bs= Nums[100\;200\;]
with a in as.vals(stopOk:\), b in bs.vals(fill:300Num), var r in rs.vals() (
  r:=r+a+b)
//rs==Nums[111\;222\;333\;]
CCode


WTitle(`Strings interpolation, even better with Wcode(with)')

Alphanumeric classes and strings can be seen as immutable
sequences of Strings of length 1.

All the operators working on immutable sequences works on strings and alphanumerics.

However, they can not be constructed with square brackets, that is Wcode(S[a;b;c]) does not compile.
Square brackets can be used to interpolate strings and alphanumerics; as in:
OBCode
S"hello "[name]", have a good day, and do not panic!"
CCode
However, also Wcode(with) can be used; this can make code very compact,
for example assume we want to collect some names and numbers in a string:
OBCode
res=S"your info: "[with name in names.vals(), num in nums.vals()  (
  use[ S"name:"[name]", num:"[num]""]
  )]"" //we always need the ending string, even if empty.
CCode
In order to put a semicolon between elements in our string, we can use Wcode(sep:)
OBCode
res=S"your info: "[with name in names.vals(), num in nums.vals()  (
  use[ S"name:"[name]", num:"[num]"", sep:S"; "]
  )]"" //we always need the ending string, even if empty.
CCode


WTitle(`(5/5) Collections summary')

<ul><li>
There are a tons of methods and operators to know, but since most code works 
around collections, it is worth the effort to memorize them.
</li><li>
Immutable collections are easy to play with, using operators and with methods.
</li><li>
Mutable collections can be more efficient and flexible, but they come with additional
difficulties.
</li><li>
Most methods have a general version that works with an index, and specialized Wcode(left) and
Wcode(right) variants.
</li><li>
Wcode(/) can help remove a lot of boilerplate, but is not trivial to get used to it.
</li><li>
Wcode(with) is very useful and flexible. It is common to find methods composed be just a large
Wcode(with) statement plus a little pre and post processing around it.
</li></ul>