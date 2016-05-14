WBigTitle(`Interfaces, Concepts and Data')

WTitle((1/5)Interfaces, Basis)
In 42 interfaces are quite similar to interfaces in other oo languages.
There are however a couple of important differences:

WP
while implementing an interface method, you have not to repeat the
typing.
For example, in the following code, to implement Wcode(Shape.draw(that)) inside
of Wcode(Square), we do not repeat the types Wcode(Void) and Wcode(mut Canvas)
OBCode
Shape:{interface
  method Void draw(mut Canvas that)
  }
Square:{implements Shape
  method draw(that){..}
  }
CCode

In 42, we say that the method Wcode(draw(that)) 
implemented in Wcode(Square)
WEmph(is defined by)
Wcode(Shape).
Each method is defined in a single point, either the class itself
or a (transitively) implemented interface.
This means that a class can not implement multiple interfaces defining methods
with the same name.
For example, this code is ill-typed:
OBCode
Card:{interface
  method Num draw()//the value of the drawn card
  }
Gun:{interface
  method Num draw()//the time it takes to drawn the gun
  }
Wrong:{implements Card,Gun//not allowed
  }
CCode


Note that that would be bad 42 code anyway, you should define an
enumeration (or an alphanumeric)
 for your cards and use a Wcode(Second) unit of measure
for the time.

WTitle((2/5)Interfaces, Details)
However, interface diamond is allowed, that is, the following code is correct:
OBCode
Shape:{interface
  method Void draw(mut Canvas that)
  }
Animal:{interface implements Shape
  method Meter run(){..}
  }
Noisy:{interface implements Shape
  method Void play(mut Audio that){..}
  }
LoudCat:{implements Animal, Noisy
  method draw(that){..}
  method run(){..}
  method play(that){..}
  }
CCode

You can further specify the type of an interface method by using the keyword 
Wcode(refine):
OBCode
Monster:{interface
  method Monster spawnMinion()
  }
BigMonster:{implements Monster
  refine method BigMonster spawnMinion(){..}
  }
CCode


WTitle((3/5)`Concepts: ToS, Equals, Classable, ...')
Wcode(Concepts) is a class defined in AdamTowel,
containing interfaces commonly used by many classes.
WP
The most well known one is 
Wcode(Concepts.ToS), implemented by all objects that can
be converted in human readable strings.

In AdamTowel, all basic classes (as alphanumeric, numeric and units) just print
the string or the number as it is.

WP
Other objects prints their fields content in square brackets.
Wcode(Data) generates a reasonable
Wcode(Concepts.ToS) implementation, following this patter. For fields of interface type,
the class name of the stored instance is also produced.
Strings and numbers are enclosed with double quotes.

WP
Another very well known interface is Wcode(Concepts.Equals)
implemented by objects that can be compared with
Wcode(read method Bool equals(Concepts.Equals that)).
Such objects usually also define methods Wcode(==) and Wcode(!=)
based on Wcode(equals), but is not required by the interface.

Wcode(Data) will automatically generate a valid Wcode(Concepts.Equals)
implementation, and derived methods Wcode(==) and Wcode(!=).
The default implementation of 
Wcode(Data) do not takes in account circular object graphs, and will end up in loop in those cases. We will discuss later how to personalize the behaviour of Wcode(Data).

WP
Finally, an interface that near every class or interface implements is Wcode(Concepts.Classable).
It allows to get the class object out of an ordinary instance.
This is very useful when wanting to create an instance based on another.
For example, to create another shape of the same kind of a given shape,
we could do the following:
OBCode
Shape:{implements Concepts.Classable
  class method This newShape(Color that)
  }
Square:{implements Shape
  Color color
  method newShape(that)//implemented from Shape
    This(color:that)
  method class()//implemented from Concept.Classable
    This//this implementation can be omitted if "Data<<" is used
  }
/*..*/
Shape s=..
s.class().newShape(Color.red())
CCode
Where the interface Wcode(Shape) defines an abstract factory method (that is,
an interface class method returning Wcode(This))
and class Wcode(Square) implements
 Wcode(Shape) (and thus also Wcode(Concepts.Classable)).
 
 
WTitle((4/5)`Concepts.Invariant')
 
Wcode(Concepts.Invariant) is another interface implemented very often and
 that can be handled by Wcode(Data), but in a different way with respect to the former ones:
 An instance of an object is more than just a record storing other objects of various types;
 those values are bond together by an invariant: a property that
 holds for all values of that type.
 AdamTowel and Data use Wcode(Concepts.Invariant) to represent this idea:
 A class that does not implement Wcode(Concepts.Invariant) just have the
 empty invariant enforced by type type of its fields, but the programmer can specify a
 more stringent invariant by implementing Wcode(Concepts.Invariant), as 
 in the following code:
 
 OBCode
Margin:Data<<{implements Concepts.Invariant
  var Meter minX
  var Meter minY
  var Meter maxX
  var Meter maxY
  method invariant()
    this.minX()<=this.maxX() & this.minY()<=this.maxY()
  }
CCode

Here we define a class Wcode(Margin) where the invariant
states that the min coordinates have to be smaller that the max ones.

Wcode(Data) will generate code so that the invariant is checked while the object is created
and in any field update.
Wcode(Data) guarantees that no Wcode(Margin) object that does not 
satisfy the invariant will ever be visible outside of the scope of the Wcode(invariant()) method itself.

See more about restrictions of this mechanism in (Wlink(dataAndInvariants,
`data and invariants'))

The current example shows a class with all immutable fields.
It is possible to enforce the invariant also on classes with immutable
and capsule fields.
Then Wcode(Data) will additionally check that the
lent exposers are used in a safe pattern:
they are used only on the Wcode(this) receiver,
and every method using the lent exposer do not return
a lent result.
It is unobvious why such requirements suffice: the idea is that the only way a mutable/lent reference
to the content of a capsule field can be
accessed is by the exposer. The reference produced by the lent exposer can only be returned as lent,
or wrapped inside of freshly created objects referred to by lent references.
(WEmph(note for marco:) this means no more lent/read fields)


WTitle((5/5)`Interfaces and Data, summary')

Interfaces in 42 serves the same role they serve in other languages,
with a little bit of a twist in the details.

The big news is that Decorators (Wcode(Data) in our examples) can
help and provide implementations for free.
This is much more powerful with respect of traits, multiple inheritance or
Java 8 default methods, since the implementation can be generated ad hoc
by examining the class.
 