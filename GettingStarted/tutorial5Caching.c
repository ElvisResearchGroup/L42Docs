WBigTitle(Caching)
WTitle((1/5) Normalization)
One of the big advantages of deeply immutable objects is that two structurally identical objects are referentially transparent, that is, you can not distinguish if they are represented in memory by a single object or two.
This means that it is possible to reuse the same objects to save memory. While in other languages the programmer would have to implement some ad. hoc. support to reuse objects, in L42 this is supported directly by the language, in a process called WEmph(normalization).
An immutable object can be turned into its normalized version using Wcode(.norm()).
OBCode
Person = Data:{S name}
..
var bob1 = Person(S"Bob")//one bob in memory
var bob2 = Person(S"Bob")//two bobs in memory
bob1:=bob1.norm()
bob2:=bob2.norm()//most likely now bob2 referes to
//the same object of bob1.
//The object originally pointed by bob2 can now be
//garbage collected.
bob2:=bob2.norm()//repeating normalization has no effect.
//The second calls to norm is as fast as a field access.
CCode
As you can see, objects are born not normalized and can be normalized manually by the programmer.
Normalization is cheap but it is not free, so it is not convenient to apply it on short lived objects.
The programmer can express that all objects of a class are normalized during creation by passing parameters to Wcode(Data):
OBCode
Person = Data('This,autoNorm=\.true()):{S name}
..
var bob1 = Person(S"Bob")//one bob in memory
var bob2 = Person(S"Bob")//two bobs in memory
bob2:=bob2.norm()//no-op, bob2 was already normalized
CCode
Normalizing requires to check if another structurally equivalent object has ever been normalized. This also support circular objects and normalizes all the sub objects.
Consider the following richer example:
OBCode
Person = Data:{S name}
Dog = Data:{S name, Person owner}
..
bob1 = Person(S"Bob")//one bob in memory
bob2 = Person(S"Bob")//two bobs in memory
dog1 = Dog(S"Grunthos", bob1)
dog2 = Dog(S"Agrajag", bob2)
//dog1.owner() is a different object of dog2.owner()
//abait there are structurally identical and we can
//not tell them apart using L42.
dog1.norm()//note: we do not reassign dog1
dog2.norm()
//dog1 and dog 2 are different objects, but
//dog1.owner() is now the same object of dog2.owner()
//bob1 and bob2 are still two different objects.
CCode
The logic needed for normalization is the same needed to check if two arbitrary objects are structurally equal, to print an object to a readable string and to clone objects.
Thus data allows for all of those operations indirectly relying on normalization.
Those are all operations requiring to scan the whole ROG of the object, so the cost of normalization is acceptable in context.

WTitle((2/5) Lazy Caching)

Some methods may take a long time to compute, but the are deterministic, and thus we could cache the result and reuse it many times.
A typical example is fibonacci:
OBCode
class method Num slowFibo(Num n) = {
  if n==0Num || n==1Num (return n)
  return This.slowFibo(n=n-1\)+This.slowFibo(n=n-2\)
  }
CCode
This fibonacci implementation would take a very long time to run, since it would require recomputing the same
results an exponential amount of times.

This tweaked implementation relying on caching is much faster.
OBCode
ComputeFibo=Data:{
  Num that
  @Cache.Lazy method Num () = {
    n=this.that()
    if n==0Num ||n==1Num ( return n )
    This fibo1=This(n-1Num)//a 'computation object'
    return fibo1()+This(n-2Num)()
    }
  }
//...
//usage example
ComputeFibo(42\)() == 267914296Num
CCode
As you can see, instead of a method with parameters we can declare a class with fields and an unnamed method doing the actual computation.
Wcode(Cache.Lazy) is an annotation recognized by Wcode(Data) that works only on Wcode(imm) or Wcode(class) methods with no arguments and with an Wcode(imm) result.
Wcode(ComputeFibo fibo1) is a WEmph(computation object): an imm object whose only goal is to support one (or more) computationally intense methods.
Thanks to normalization, the cache of computation objects is centrally stored, and thus recursive calls computing fibonacci will be able to reuse the cache from other objects.
That is, the method result is cached on the normalized version of the receiver. In this way, 
all the redundant fibonacci calls are avoided.

WP
As you can see, the caching is is completely handled by the language and is not connected with the specific algorithm. This pattern is general enough to support any method from immutable data to an immutable result.

WTitle((3/5) Automatic parallelism)
Wcode(Cache.Lazy) allows to cache the result of methods after they have been called the first time.
However, why wait for the method to be called?
once the receiver object is created, the method could be computed WEmph(eagerly) in a separed worker, so that when we call the method, we get the result without any wait at all.
That is, if we use Wcode(Cache.Eager) we can get automatic parallelism: the language will handle a set of parallel workers to execute such method bodies.

WP
An important consideration here is that both Wcode(Cache.Lazy) and Wcode(Cache.Eager) are unobservable; that is, the observed result of the code is not impacted by lazy or eager cache.

Consider the following code:

OBCode
Task = Data:{ //Tasks are also computation objects
  S text
  @Cache.Eager method Bool isPolite() = ( .. )
  @Cache.Eager method Bool isGrammatical() = ( .. )
  }
Tasks = Collection.list(Task)

Main=(
  tasks=Tasks[
    \(text=S"..");
    ..
    \(text=S"..");
    ]
  for t in tasks (
    Debug( t.isPolite() )
    )
  )
CCode
Here we declare a Task object with a string field and two eager methods: one will check if the text in the string is polite and another will check if the string is grammatically correct.
This can take quite a while. By using eager methods, it is sufficient to just create the objects to start those computations in parallel.
When we need the results, we can just iterate on those task objects and call the methods.
Objects with eager methods are automatically normed during creation, as if we used 
Wcode(Data('This,autoNorm=\.true())) instead of Wcode(Data).

As you can see, parallelism and caching are just two sides of the same coin.

WTitle((4/5) Invariants and derived fields)
We have seen that cached behaviour can be computed lazily or eagerly on immutable objects.
But we can bring caching even earlier and compute some behaviour WEmph(at the same time) of the object creation.
This allows to encode derived fields: fields whose value must depend from the other object fields.
Consider the following example:
OBCode
Point = Data:{//not ok, the tree arg factory still exists
  Double x
  Double y
  Double distanceFromOrigin
  //should always be (x^2+y^2)^0.5
  class method This (Double x, Double y) = \(
    x=x
    y=y
    distanceFromOrigin=((x*x)+(y*y)).pow(exp=\"0.5")
    )
  }
CCode
Where the class Wcode(Point) has 3 fields, but the value of the third one should depend from the other two.
In 42 the code above would simply define a class with three unrelated fields, and while we are offering a factory that conveniently takes x and y and initialize the third field with 
the computed value, the user could easy create invalid instances by calling the factory method with three arguments.
As we will see later, in 42 we can prevent this from happening by making such method private.
However, we would still be able to create an invalid Wcode(Point) inside of other Wcode(Point) methods.
Ideally, we would like to truly have only two fields, and have the third one as a precomputed derived data.

In 42, we can encode such concept using Wcode(Cache.Now):
OBCode
Point = Data:{
  Double x
  Double y
  @Cache.Now class method Double distanceFromOrigin(Double x, Double y) = 
    ((x*x)+(y*y)).pow(exp=\"0.5")
  }
CCode
The Wcode(Point) class defined above has a single factory method taking just Wcode(x) and Wcode(y). In this way there is no need to have multiple ways to build the object and then hide the dangerous ones after the fact.

The method Wcode(distanceFromOrigin(x,y)) is computed when a Wcode(Point) object is created.
Moreover, Wcode(Data) adds a method Wcode(read method Double distanceFromOrigin()), allowing to read the computed/cached value as if it were a field. Note how the 
Wcode(class)
method is turned into a Wcode(read) method.

If the method Wcode(distanceFromOrigin(x,y)) where to leak an error, such error would be propagated out as it would happen if the method were manually called during object construction.

This means that any time you receive a Wcode(Point), it has a valid distance.
We can leverage on this behaviour to encode class invariants:
Wcode(Cache.Now) methods with Wcode(Void) return type
designed to simply throw error if the state is incorrect.
For example, consider this updated version of Wcode(Point):

OBCode
Point = Data:{
  Double x
  Double y
  @Cache.Now class method Double distanceFromOrigin(Double x, Double y) = 
    ((x*x)+(y*y)).pow(exp=\"0.5")
  @Cache.Now class method Void invariant(Double x, Double y) = 
    if x<0Double || y<0Double error X"""%
      | Invalid state:
      | x = %x
      | y = %y
      """
  }
CCode

Now, every time user code receives a Wcode(Point), they can rely on the fact that Wcode(x) and Wcode(y) are positive.

WTitle((5/5) Summary)
In 42 immutable objects can be normalized in order to save memory space.
This works also on circular object graphs and relies on a variation of DFA normalization.
As a special case, objects without fields (immutable or not) are always represented in memory as a single object.
Cached informations are attached to the normalized version of objects, thus making it possible to recover the cache simply by rebuilding a structurally identical object.

There are tree kinds of caching, depending on the time the caching behaviour activates:

<ul><li>
Wcode(Cache.Lazy) computes the cached value when the annotated method is first called.
It works on Wcode(imm) and Wcode(class) no-args methods.
An obvious workaround for the no-args limitation is to define computation objects; this also works well with normalization: computation objects will remember the cache of any structurally equivalent object.
</li><li>
Wcode(Cache.Eager) computes the cached value in a separate parallel worker, starting when the object is created. It only works on Wcode(imm) no-args methods of classes whose objects are all deeply immutable.
Those classes will automatically normalize their instances upon creation.
</li><li>
Wcode(Cache.Now) computes the cached value during object construction.
Since the object does not exists yet, the annotation is not placed on an instance method, but on a Wcode(class) method whose parameters represent the needed object fields.
This annotation does influence the observable behaviour: if an error is raised computing the cache, the error is leaked during object construction and not while trying to observe the cached value.
This, in turn, allows to encode class invariants and to provide a static guarantee that users of a class can rely upon.
</li></ul>

WBigTitle(Caching on Mutable objects)
WTitle((1/5) Cache invalidation)
The main advantage of caching method of immutable objects is that the cache is always valid.
L42 allows to cache also methods of mutable objects, and discovers on its own when the cache needs to be invalidated.
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
When a setter for Wcode(x) or Wcode(y) is invoked, then two Wcode(Cache.Now) methods are recomputed.

In other programming languages, this behaviour can be encoded by
making the fields private and customizing the implementations of the setters to recompute the distance when needed. This pattern can grow very complex very fast.
L42 guarantees that a cached value is always structurally equivalent to the value that would be returned by calling the method again.
Moreover, for Wcode(Cache.Now) L42 also guarantees that such computation would terminate without errors.
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
Wcode(Cache.LazyRead) allows to encode conventional caching on mutable datastructures and automatic cache invalidation:
The operations are computed when they are first asked, and the cache is automatically invalidated when a Wcode(Cache.Clear) method terminates.
Again, we think that at all times Wcode(Cache.LazyRead) has the same semantic as recomputing the value, but with a different performance.
This annotation is called Wcode(Cache.LazyRead) because it produces a 
Wcode(read) method, while 
Wcode(Cache.Lazy) works on immutable ones.

Cache invalidation is considered one of the great challenges in writing correct programs; L42 can handle it correctly and automatically.
However, there is a cost: you have to encode the algorithm so that the type system accepts your code and that the caching annotations can be applied.

WTitle((3/5) Box patten)

As we have seen, in order to write well encapsulated mutable objects we need to designed them well, using Wcode(capsule) to initialize the mutable data, using Wcode(Cache.Clear) to mutate such state and Wcode(Cache.Now) for the invariant.
However, we can also program a naive deeply mutable object and boxit up as a second step.
This can require a little more code, but it is more intuitive, and works very well for arbitrarly complex cases.
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

We can easly create a Wcode(BikeBox) class containing and encapsulating, such a Wcode(Bike):
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
There are various ways to prevent that

Wcode(System.mutReferenceEquality(a,and=b))

WTitle((5/5) Summary)
