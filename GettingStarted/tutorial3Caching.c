WBigTitle(Caching)
WTitle((1/5) Normalization)
One of the big advantages of deeply immutable objects is that two structurally identical objects are referentially transparent, that is, you can not distinguish if they are represented in memory by a single object or two.
This means that it is possible to reuse the same objects to save memory. While in other languages the programmer would have to implement some ad hoc support to reuse objects, in L42 this is supported directly by the language, in a process called Wemph(normalization).
An immutable object can be turned into its normalized version using Wcode(.norm())
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
The logic needed for normalization is the same needed to check if two arbitrary objects are structurally equal, to print an object to a redable string and to clone objects.
Thus data allows for all of those operations indireclty relying on normalization.
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
    This fibo1=This(n-1Num)//a 'computational object'
    return fibo1()+This(n-2Num)()
    }
  }
CCode
As you can see, instead of a method with parameters we can declare a class with fields and an unnamed method doing the actual computation.
Wcode(Cache.Lazy) is an annotation recognized by Wcode(Data) that works only on Wcode(imm) or Wcode(class) methods with no arguments and with an Wcode(imm) result.
The method is computed only one time and then cached on the normalized version of the receiver. In this way, 
all the redundant fibonacci calls are avoided.

WP
As you can see, the caching is is completly handled by the language and is not connected with the specific algorithm. This pattern is general enought to support any method from immuttable data to an immutable result.

WTitle((3/5) Automatic parallelism)
Wcode(Cache.Lazy) allows to cache the result of methods after they have been called the first time.
However, why wait for the method to be called?
once the receiver object is created, the method could be computed Wemph(eagerly) in a separed worker, so that when we call the method, we get the result without any wait at all.
That is, if we use Wcode(Cache.Eager) we can get automatic parallelism: the language will handle a set of parallel workers to execute such method bodies.

WP
An important consideration here is that both Wcode(Cache.Lazy) and Wcode(Cache.Eager) are unobservable; that is, the observed result of the code is not impacted by lazy or eager cache.

Consider the following code:

OBCode
Task = Data:{
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
But we can bring caching even earlier and compute some behaviour Wemph(at the same time) of the object creation.
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
In 42 the code above would simply define a class with tree unrelated fields, and while we are offering a factory that conveniently takes x and y and initialize the third field with 
the computed value, the user could easly create invalid instances by calling the factory method with three arguments.
As we will see later, in 42 we can prevent this from happening by making such method private.
For the class Wcode(Point) as presented, this would be a 
satisfactory solution, since all Wcode(Point) objects are deeply immutable.
However, what if the fields could be updated?
OBCode
Point = Data:{
  var Double x
  var Double y
  var Double distanceFromOrigin
  //should always be (x^2+y^2)^0.5
  //should be recomputed when x or y are updated
  }
CCode

In other programming languages this can be encoded by
making the fields private and customizing the implementations of the setters to recompute the distance when needed. This pattern can grow very complex very fast.
In 42 we can avoid manually handling those checks and recomputations by using Wcode(Cache.Now)
OBCode
Point = Data:{
  var Double x
  var Double y
  @Cache.Now class method Double distanceFromOrigin(Double x, Double y) = 
    ((x*x)+(y*y)).pow(exp=\"0.5")
  }
CCode
The Wcode(Point) class defined abouve has a single factory method taking just Wcode(x) and Wcode(y). In this way there is not need to have multiple ways to build the object and then hide the dangerous ones after the fact.

The method Wcode(distanceFromOrigin(x,y)) is computed when a Wcode(Point) object is created, and it is recomputed when Wcode(x) or Wcode(y) are updated.
Morever, Wcode(Data) adds a method Wcode(distanceFromOrigin()), allowing to read the computed/cached value as if it were a field.

If the method Wcode(distanceFromOrigin(x,y)) where to leak an error, such error would be propagated out as it would happen if the method were manually called.

This means that any time you receive a Wcode(Point), it has a valid distance.
We can leverege on this behaviour to encode class invariants:
Wcode(Cache.Now) methods with Wcode(Void) return type
designed to simply throw error if the state is incorrect.
For example, consider this updated version of Wcode(Point):

OBCode
Point = Data:{
  var Double x
  var Double y
  @Cache.Now class method Double distanceFromOrigin(Double x, Double y) = 
    ((x*x)+(y*y)).pow(exp=\"0.5")
  @Cache.Now class method Void invariant(Double x, Double y) = 
    if x<0Double || y<0Duble X"""
      | Invalid state:
      | x = %x
      | y = %y
      """
  }
CCode

Now, every time user code receives a Wcode(Point), they can rely on the fact that Wcode(x) and Wcode(y) are positive.

WTitle((5/5) Summary)

WBigTitle(Caching on mutable objects)
then on mutable


also on mutable objects

also lazy

Should discuss exceptions before??

<ul><li>
We gave a look at the most basic features of 42.
There is rich support to define your own specialized datastructures instead of having to rely on the ones provided by default.
</li><li>
Use Wcode(Units) 
and Wcode(S.Alphanumeric) to give meaning to your constants.
In this way, the type system will help you to use values with the semantics you decided.
Be sure to define all the right base classes to establish a convenient vocabulary
to talk about your problem domain.
</li></ul>