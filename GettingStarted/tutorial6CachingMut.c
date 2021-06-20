WBigTitle(Caching on Mutable objects)
WTitle((1/5) Cache invalidation)
The main advantage of caching methods of immutable objects is that the cache stays valid.
L42 can also cache methods of mutable objects, and discovers on its own when the cache needs to be invalidated.
Consider this trivial variation of the Wcode(Point) example above where the fields can be updated:

OBCode
Point = Data:{
  var Double x
  var Double y
  @Cache.Now class method Double distanceFromOrigin(Double x, Double y) = 
    ((x*x)+(y*y)).pow(exp=\"0.5")
  @Cache.Now class method Void invariant(Double x, Double y) = 
    if x<0Double || y<0Double error X".."
  }
CCode
When a setter for Wcode(x) or Wcode(y) is invoked, then the two Wcode(Cache.Now) methods are recomputed.

In other programming languages, this behaviour can be encoded by
making the fields private and customizing the implementations of the setters to recompute the distance when needed. This pattern can grow very complex very fast.
L42 guarantees that a cached value is always structurally equivalent to the value that would be returned by calling the method again.
Moreover, for Wcode(Cache.Now) L42 also guarantees that, if the computation was re-run, it would terminate without errors.
Thus, when Wcode(Cache.Now) is used to emulate invariants, those invariants are guaranteed to hold for all observable objects, that is, all objects where the annotated method could possibly be called.

This is possible thanks to the strong L42 type system, and we believe this property can not be broken.
That is, we believe this property to hold even in the presence of exceptions, errors, aliasing, input output and non deterministic behaviour.
It is possible to make L42 work together with Java or even (possibly broken) native code, and we believe our property will hold also in those cases.

WTitle((2/5) Deeply mutable objects)
As discussed above, a deeply mutable object is a mutable object with some mutable fields.
Also deeply mutable objects can support Wcode(Cache.Now), but such mutable state needs to be WEmph(encapsulated), as we have seen before for the class
Wcode(Animal).

OBCode
Animal = Data:{
  var Point location
  capsule Points path

  mut method
  Void move() = (
    this.location(\path.left())
    this.removeLeftPath()
    )

  @Cache.Clear class method
  Void removeLeftPath(mut Points path) =
    path.removeLeft()
  }
CCode

The field Wcode(`capsule Points path') is an encapsulated mutable field.
It can be accessed as Wcode(read) by doing Wcode(`this.path()'), but can not be directly accessed as Wcode(mut).
However, we can write WEmph(capsule mutator) methods by using 
Wcode(Cache.Clear).
Similarly to Wcode(Cache.Now), a class method can be annotated with Wcode(Cache.Clear) and can
take parameters representing the object fields.
In addition, more parameters can be present encoding extra arguments.
To clarify, consider this richer example, where our Wcode(Animal) has an invariant and another capsule mutator method:

OBCode
Point = Data:{ Double x, Double y
  method Double distance(Point that) = 3Double
  }

Points = Collection.list(Point)

Animal = Data:{
  var Point location
  capsule Points path

  mut method
  Void move() = (
    this.location(\path.left())
    this.removeLeftPath()
    )

  mut method
  Void trim() = 
    this.removeFarthest(location=\location, distance=3Double)

  @Cache.Clear class method
  Void removeLeftPath(mut Points path) =
    path.removeLeft()
  
  @Cache.Clear class method
  Void removeFarthest(mut Points path, Point location, Double distance) = (
    var maxD=distance
    var maxI=I"-1"
    for p in path, i in Range(path.size()) (
      currentD = location.distance(p)
      if currentD>maxD (
        maxI:=i
        maxD:=currentD
        )
      )
    if maxI!=I"-1" path.remove(maxI)
    )

  @Cache.Now class method
  Void invariant(read Points path, Point location) = 
    if path.contains(location) error X"..."
  }
CCode
We added a method to remove the farthest away point if it is over a certain distance.
As you can see, the parameters Wcode(path) and Wcode(location) corresponds to fields, while
the parameter Wcode(distance) is extra needed information.
When we call Wcode(`this.removeFarthest(distance=3\)') we pass only Wcode(distance); the other parameters are passed automatically.
As an invariant, here we require that the current location is not in the Wcode(path).
This code, in the current form, has a bug; can you spot it?
Look carefully to the method Wcode(move()):

OBCode
  mut method
  Void move() = (
    this.location(\path.left())
    this.removeLeftPath()
    )
CCode
Here we first set up the location, then we remove it from the path.
The method Wcode(move()) recomputes the invariant twice: one after the field setter 
and one after the call to the Wcode(Cache.Clear) method.
This first check is going to fail, since the leftmost element of the path has not being removed yet.
In this case we can solve the problem by swapping the lines:

OBCode
  mut method
  Void move() = (
    left=this.path().left()
    this.removeLeftPath()
    this.location(left)
    )
CCode

However, this kind of solution does not scales in the general case; next we will see a programming pattern allowing to delay in a controlled way the invariant checks, or more in general, the recomputation of Wcode(Cache.Now).

Every method annotated as Wcode(Cache.Now) can instead be annotated as 
Wcode(Cache.LazyRead).
Wcode(Cache.LazyRead) allows to encode conventional caching on mutable data-structures and automatic cache invalidation:
The operations are computed when they are first asked, and the cache is automatically invalidated when a Wcode(Cache.Clear) method terminates.
Again, we think that at all times Wcode(Cache.LazyRead) has the same semantic as recomputing the value, but with a different performance.
This annotation is called Wcode(Cache.LazyRead) because it produces a 
Wcode(read) method, while 
Wcode(Cache.Lazy) works on immutable ones.

Cache invalidation is considered one of the great challenges in writing correct programs; L42 can handle it correctly and automatically.
However, there is a cost: you have to encode the algorithm so that the type system accepts your code and that the caching annotations can be applied.

WTitle((3/5) Box patten)

As we have seen, in order to write well encapsulated mutable objects we need to designed them well, using Wcode(capsule) to initialize the mutable data, using Wcode(Cache.Clear) to mutate such state and Wcode(Cache.Now) for the invariant.
However, we can also program a naive deeply mutable object and box it up as a second step.
This can require a little more code, but it is more intuitive, and works very well for arbitrary complex cases.
Consider the following code:

OBCode
Bike = Data:{
  var mut Wheel front
  var mut Wheel back
  var mut Seat seat
  var mut Chain chain
  mut method Void nail() = this.#front().addHole()
  mut method Void rain(Second time) = this.#chain().addRust(time)
  read method Void invariant() = X[
    this.front().size()==this.back().size();
    this.seat().isConfortable();
    ]    
  }
//components can mutate (get damaged)
//and be updated (replaced with new ones)
CCode
As you can see, the Wcode(Bike) is a deeply mutable class, design with no attention to
correctness: if the programmer is not carefully, the same Wcode(Weel) 
may end up used for multiple bikes at the same time.
Also, the method called Wcode(invariant) only represents a programmer intention, but it is not enforced in any way and thus it could be silently broken.

We can easy create a Wcode(BikeBox) class containing and encapsulating, such a Wcode(Bike):
OBCode
BikeBox = Data:{
  capsule Bike box
  
  @Cache.Now class method
  Void invariant(read Bike box) = box.invariant()
  
  @Cache.Clear class method
  Void nail(mut Bike box) = box.nail()
  
  @Cache.Clear class method
  Void nail(mut Bike box, Second time) = box.nail(time=time)

  @Cache.Clear class method
  Void front(mut Bike box, capsule Wheel that) = box.front(that)
  }
..
//user code
b = BikeBox(box=\(front=\(..) back=\(..) seat=\(..) chain=\(..)))
b.nail()
//b.box().nail()/ ill-typed
Debug(b.box().front())//will display the unfortunate wheel
b.front(\(..)) //looks just like a normal setter, but acts on the internal object
CCode

As you can see, no matter how complex some class code is, we can simply wrap it into a box and apply Wcode(Cache.Now)
 and Wcode(Cache.Clear) on top of it.
In this way we end up with two types:
Wcode(Bike), that does not offers any guarantee,
 and Wcode(BikeBox), ensuring the invariant and that the state is well encapsulated.
The methods Wcode(`BikeBox.nail()'),
Wcode(`BikeBox.rail(time)')
and
Wcode(`BikeBox.front(that)')
 will check for the invariant exactly one time, at the end of their execution.
Following this pattern, the programmer can perform an arbitrary long computation before the checks are triggered.

When writing other classes we can chose to use Wcode(Bike)
or Wcode(BikeBox) depending on the specific details of our code.
If we chose to use Wcode(Bike) as a field of another class, we can still check 
the Wcode(Bike) invariant inside the invariant of the composite class:
OBCode
FamilyGarage = Data:{
  mut Bike daddyBike
  mut Bike mummyBike
  mut Trike timmyBike  
  ..
  read method Void invariant() = X[
    this.daddyBike().invariant();
    this.mummyBike().invariant();
    this.timmyBike().invariant();
    ]
  }
FamilyGarageBox = Data:{
  capsule FamiliyGarage box

  @Cache.Now class method
  Void invariant(read FamilyGarage box) = box.invariant()  
  }
CCode

WTitle((4/5) Controlling the ROG shape)

An attentive reader may have notice that we would allow for fields Wcode(left)
 and Wcode(right) to point to the
 same Wcode(Wheel) object.
A Java programmer may be tempted to just add 
Wcode(this.front()!=this.back();) in the invariant,
but this would just use the user defined Wcode(!=) operator, that
on classes created using Wcode(Data) is likely to check for structural equality instead of pointer equality.
AdamsTowel offers Wcode(`System.mutReferenceEquality(a and=b)') to check for reference equality, but 
this method only works for Wcode(mut) objects.
The wheels are mut objects indeed, but the invariant method takes a Wcode(read) receiver, thus we can only see the wheels as Wcode(read).
In this case, the inability of using pointer equality is actually a good thing, since it does not correspond to what we really wanted to express: What if the two wheels are different objects but they share the same Wcode(mut Tire) object?
What we want is to check that the mutable objects are not aliased in physically unreasonable ways.
More in general, we want to ensure a tree shape of the mutable part of the object graph.

In 42 we can create classes where all the instances are guaranteed to follow this property, by making all fields either Wcode(imm) or
Wcode(capsule) of a class recursively respecting this property.
However, according to what we have seen up to now, Wcode(capsule) fields can only be mutated by defining Wcode(Cache.Clear) methods, and those methods will be unable to mutate any other 
Wcode(capsule) field.
Consider the following code:

OBCode
Tire = Data:{var Num pressure}
Wheel = Data:{var mut Tire tire}
Seat = Data:{var S description}
Chain = Data:{ var Num damage 
  mut method Void onWheel(lent Wheel that) = (
    lent Tire t=that.#tire()
    t.pressure(\pressure-1\)
    this.damage(\damage+1\)
    )
  }
CCode

Here the Wcode(Chain) can rub onto the wheel damaging it.
The parameter of method Wcode(onWheel) is
Wcode(lent). This guarantees that the object graphs of the chain and the wheel will 
not be mangled together by the method Wcode(onWheel).
Can we assemble a Wcode(Bike) while being able to both use 
Wcode(onWheel) and guaranteeing the tree structure?
The trick is to declare Wcode(lent) exposers manually:
OBCode
Bike = Data:{
  var capsule Wheel front
  var capsule Wheel back
  lent method lent Wheel #back() //lent exposer
  var capsule Seat seat
  var capsule Chain chain
  lent method lent Chain #chain()//lent exposer
  mut method Void chainOnWheel() = 
    this.#chain().onWheel(this.#back())
    
  }
BikeBox = Data:{..}//as before
CCode
That is, by declaring lent exposers manually we gain the possibility of writing methods mutating the capsule fields without using the Wcode(Cache.Clear) pattern.
In exchange for this extra flexibility, the class can not use 
Wcode(Cache.Clear), Wcode(Cache.Now) or Wcode(Cache.LazyRead) directly.
However, we can still use through the box pattern, as show before.

WTitle((5/5) Summary)

Caching and representation invariants can be applied also to mutable objects.
Proving properties on mutable objects requires to know and apply various patterns.
Historically, in software verification, representation invariants where small 
units of code, mostly focusing on the direct content of the fields and mostly relying on either pointer equality or the binary value of primitive datatypes, where the invariants could be purposely broken while still being able to observe the broken object.
Nothing of this is possible in 42; in particular, 42 guarantees that broken object can not be observed.
The box patterns allows to divide the value in two types: the one with an enforced invariant and the raw object state, recovering the flexibility of temporarily open invariants without any of the drawbacks.

Note how avoiding pointer equality tests is required when normalization is took into account.
Consider the example of a list whose invariant requires all object to  pointer unique.
A valid list of such type may contain two structurally equal but not pointer equal objects.
If such a list becomes immutable and was normalized, those two objects would become point equals, and the invariant would be broken by normalization.