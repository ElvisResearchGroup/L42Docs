WBigTitle(`A taste of Metaprogramming')

WTitle((1/5)Refactor and Introspection)

WTitle(Refactor)
Wcode(Refactor) is a class supporting modification of
library literals.
For example, you may want to rename the method Wcode(importStructure(that)) into just Wcode(import(that)).
You can do the following:
OBCode
{reuse L42.is/AdamTowel
Db:Refactor.RenameSelector(
  Selector"importStructure(that)" to:Selector"import(that)")<<Load<<{reuse L42.is/Db}
UnivDb:Db.import(Db.ConnectionS"...")
/*..*/
}
CCode
The type Wcode(Selector) represent method selectors;
in the same way the type Wcode(Path) represent 
paths inside library literals, as in Wcode(Path"MyNested.MyNestedNested") or
Wcode(Path"This").

There are a lot of refactoring operations nested under Wcode(Refactor):
<ul><li>
Wcode(RenameSelector)
and 
Wcode(RenamePath)
rename methods either at top level (as we just did) or
in an arbitrary nested library;
or rename paths into other paths
</li><li>

Wcode(Redirect)
removes a nested library and redirects all its references to
an external one. This emulates generics, as we will see later.
</li><li>
Wcode(UpdateDocumentationSelector)
and Wcode(UpdateDocumentationPath)
add to/alter/delete the documentation of methods/paths.
</li><li>
Wcode(MakeAbstractSelector)
and Wcode(MakeAbstractPath)
remove all the implementation out of a method or path,
leaving only the public skeleton
</li><li>
Wcode(HideSelector)
and Wcode(HidePath)
mark methods or paths as private.
We have not seen details on private members, the main idea is that
they are renamed into invisible names that you can never guess, and automatically renamed to avoid collisions 
by refactoring operations.
</li></ul>
WP
In addition to all those nested classes,
Wcode(Refactor) offers Wcode(`Refactor.compose(left,right)')
allowing a simmetric sum of two library literals.
The main idea is that members with the same name are recursively composed

WTitle(Introspection)
Wcode(Introspection) is
a class for exploring libraries, to discover what methods they have and so on.

The main classes inside of Introspection are
Wcode(Introspection.NestedLibrary),
 Wcode(Introspection.Method) and
Wcode(Introspection.Type).
You can obtain a nested library by calling the factory methods 
Wcode(Introspection(lib)) and Wcode(Introspection(classObj)),
respectively for library literals or class objects.
We will see some example later of use of Wcode(Introspection).


WTitle((2/5)Traits and Metaprogramming)

Metaprogramming is the most important feature of 42.
All the decorators that you have seen up to now are implemented with metaprogramming, 
which shows that 42 offers a good balance of freedom and
safety.
WP
The main idea of 42 metaprogramming is that only library literals can
be manipulated.
Metaprogramming is evaluated top down nested/inner-most first.
Once a library literal has a name, it can not be independently metaprogrammed; but only influenced
by metaprogramming over the library that contains it.

WP
We call Wemph(traits) methods that return reusable (unnamed) code.
For example
OBCode
Transaction:{
  class method
  Library traitEnsureTransaction() 
    {//begin library literal
    class method //without name: can be called as a functor
    Void (mut Db.Connection connection)
    exception Db.Query.Failure //no body: abstract method

    class method //without name: can be called as a functor
    Void (mut Db.Connection that)
    exception Db.Query.Failure (
      that.openTransaction()
      This(connection:that)//here we call us as a functor
      catch exception Db.Query.Failure x (
        that.rollback()
        exception x //no 'finally' needed thanks to strong error safety, more on this later
        )
      that.commitTransaction()
      )
    }
  }//end library literal

MyAction:Refactor.compose(
  left:Transaction.traitEnsureTransaction()
  right:{ //and now the missing implementation
    class method
    Void (mut Db.Connection connection)
    exception Db.Query.Failure {
      /*..my operation..*/
      }
    })

Main:{
  /*..*/
  MyAction(conn)//do the operation in a transaction
  /*..*/
  }
CCode

Note that Wcode(traitEnsureTransaction()) is just a normal 
class method that directly returns a library literal.
Traits in 42 are nothing fancier than that.

Now Wcode(MyAction) will execute the operation inside of a transaction.

However, as you can see declaring Wcode(MyAction) using 
Wcode(Refactor.compose) is verbose,
and we need to know the code of Wcode(traitEnsureTransaction())
to use it;
we now show how to improve.
WP

Manually declaring a class just to declare a single trait method
returning a library literal is verbose.
In AdamTowel we can use the class Wcode(Resource)
which automate this process.
WBR
For example:
OBCode
TraitEnsureTransaction:Resource<<{
  class method
  Void (mut Db.Connection connection)//method selector here is '(connection)'
  exception Db.Query.Failure 

  class method
  Void (mut Db.Connection that)//method selector here is '(that)'
  exception Db.Query.Failure (/*..as before..*/)
  }

MyAction:Refactor.compose(
  left:TraitEnsureTransaction()
  right:{ /*..as before..*/})
CCode

This let us save just a couple of lines. 
We can improve further and make a Wcode(Transaction)
class decorator:

OBCode
Transaction:{
  InvalidAction:Message.$<<{implements MetaGuard}
  //meta guard is the root of all the metaprogramming guards
  class method //using << to define the babelfish operator
  Library << (Library that) 
  exception InvalidAction {
    i=Introspection(lib:that)
    if !i.hasMethod(\"(connection)") (exception InvalidAction
      "Action method '(connection)' missing")
    composed=Refactor.compose(  left: TraitEnsureTransaction(), right: that  )
    exception on MetaGuard ( InvalidAction
      "Action invalid:type of '(connection)' does not fit or already defined '(that)'")
    return Refactor.HideSelector(\"(connection)")<<composed
    error on Metaguard
      WTF"'(connection)' is there, ready to be hidden"
    }  
  }
//So, MyAction becomes shorter and better checked:
MyAction:Transaction<<{
  class method
  Void(mut Db.Connection connection)
  exception Db.Query.Failure {
    /*..my operation..*/
    }
  }
CCode

Note how we check some well formedness of the parameter
in an Wcode(if), then we catch and wrap the exceptions of Wcode(compose(left,right)),
and finally we state our assumption that Wcode(HideSelector) can not fail in
that context.

Now we can use Transaction as a decorator.


WTitle((3/5)Extend)

Wcode(Extend)
 is a decorator implemented using 
Wcode(Refactor) and
Wcode(Introspection)
which provides a flexible model of multiple inheritance with super calls in AdamTowel.
WBR
As an example, in a game we can have a chest which contains objects in certain positions,
a boat which host humanoids, and
a cargo boat, which host humanoids and contains objects like a chest.
We want to reuse the code of chest and boat to obtain the cargo boat.
WBR 
For example:
OBCode
ChestTrait:Resource<<{
  mut Objects objects
  /*.. methods to validate access to objects..*/
  read method
  Kg weight() {
    var Kg res=0Kg
    with o in this.objects().vals() (res+=o.weight() )
    return res
    }
  }

BoatTrait:Resource<<{
  mut Humanoids crew
  Kg maxCapacity
  /*.. methods to validate access to crew..*/
  read method
  Kg weight() {/*..with-loop on the crew..*/}
  
  read method
  Kg capacityLeft()
    this.maxCapacity()-this.weight()    
  }

Chest:Data<<ChestTrait()
Boat:Data<<BoatTrait()
CargoBoat:Data<<Extend[ChestTrait();BoatTrait()]<<{
  read method @override //explained below
  Kg weight() this.#1weight()+this.#2weight()
  }
CCode

As you see, we annotate with Wcode(@override) to 
override the Wcode(weight()) method, and we use 
Wcode(`#'1weight()) and 
Wcode(`#'2weight()) to refer to the super implementations.
As an alternative to Wcode(@override),
we could use Wcode(@hide) to just hide the old methods and put our new
version on top. There are two main difference between Wcode(@override) and 
Wcode(@hide).
With override internal references will refer to the new implementation,
while with hide they will refer to the old one.
With override the method type must be identical,
while with hide they can be completely different.


WTitle((4/5)An intolerant type system)

As an exercise, lets try to use what we learned to add a Wcode(sum()) method to
a vector.

OBCode
Nums:Extends[Collections.vector(of:Num)]<<{
  read method
  Num sum(){
    var Num res=0Num
    with n in this.vals() (res+=n )
    return res
    }
  }
CCode

Easy.
However, note that we are calling Wcode(this.vals()) to
do the iteration, and we are not declaring a Wcode(vals())
method.
The idea is that while computing Wcode(Nums), the type system is temporary allowing for incomplete/untypable code at the right of the 'Wcode(:)'.
The typesystem will check that all is ok when the declaration of Wcode(Nums) is complete.
WP
However, we have done an extension only on our specific Wcode(Nums) vector, we would have to repeat
such code for each vector.
Can we produce vectors that will have a Wcode(sum()) method?
Well, this can only work for vectors of elements with a Wcode(+) operator, and a zero concept. Luckily, all 
numeric classes offer a Wcode(zero()) 
and Wcode(one()) method.
WBR
Building on that, we could attempt the following, invalid solution:
OBCode
MyCollection:{
  class method
  Library traitSum()
    {//my sum feature
    T:{
      class method T zero()
      method T +(T that)
      }
    read method
    Num sum(){
      var T res=T.zero()
      with n in this.vals() (res+=n ) //error here, vals() undefined
      return res
      }
    }
  class method
  Library vector(class Any of) {
    oldPart=Collection.vector(of:of)
    newPart=Refactor.Redirect(Path"T" to:of)<<this.traitSum()
    return Refactor.compose(left:oldPart, right:newPart)
    }
CCode

Conceptually, we declare a new trait for the sum method,
and we make it general introducing Wcode(T) and our
needed requirements.
Sadly, this is not going to compile, since 
in the method Wcode(sum()) we call Wcode(this.vals()),
and there is no definition for such method.
Similar code worked in the former example, but here
the definition of Wcode(MyCollection) gets completed,
and the code in the method Wcode(traitSum()) is still 
incomplete.
We could just repeat there the definition of Wcode(vals()),
but that would be duplicating code; moreover, Wcode(vals()) returns an iterator, which has methods too...

Wcode(Collection) offers a solution: a trait containing
the minimal code skeleton to make Wcode(vals()) over path 
'Wcode(T)'.
WBR
The idea is that
the composition of Wcode(traitSum()) and
Wcode(Collection.traitValsT()) is complete code.
However, even declaring Wcode(traitSum()) as
OBCode
class method
Library traitSum() 
  Extend[Collections.traitValsT()]<<{/*my sum feature as before*/}
CCode

whould not work: the Wcode(<<) method would
be called when Wcode(traitSum()) runs, leaving incomplete code in the resulting library literal.
We need to force the computation to happen before
Wcode(MyColleciton) is completed.
A solution is to use Wcode(Resource).

OBCode
TraitSum:Resource<<Extend[Collections.traitValsT()]<<{/*my sum feature as before*/}
MyCollection:{
  class method
  Library vector(class Any of) (
    oldPart=Collection.vector(of:of)//surely works
    {newPart=Refactor.Redirect(Path"T" to:of)<<TraitSum()
    return Extend[oldPart]<<newPart
    catch exception MetaGuard g return oldPart
    })
CCode

By the way, earlier we also forgot to handle exceptions!
If our parameter does not support zero and plus,
we will just return a normal collection. We need to insert additional brackets otherwise the 
binding Wcode(oldPart) would not be visible in the catch body.

As you may notice there is some incoherence in our programming style:
should traits be methods in a class or Resources?
should we use 
the more primitive
Wcode(Refactor.compose(left,right))
or the more flexible Wcode(Extend[]<<)?
In the current state of the art we do not have an answer for what is the best in 42.
WBR
Indeed, we still do not understand the question.


WTitle((5/5)Metaprogramming summary)
<ul><li>
Metaprogramming is hard; 42 tries to make it simpler, but not trivial.
</li><li>
Error handling is important while writing decorators.
More then half of decorators code should be dedicated
to handling errors and lifting them into a more understandable
form, for the sake of the final user.
</li><li>
We are just scratching the surface of what we
can do with metaprogramming.
If you are interested in becoming a Magrathean, then
refer to the painful metaprogramming guide (link);
otherwise just use existing metaprogramming libraries 
and use Wcode(Refactor) only when all the other options feel more painful.
</li></ul>


