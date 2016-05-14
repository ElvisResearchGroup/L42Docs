WBigTitle(Errors and Exceptions: Messages in AdamTowel)

WTitle((1/5)`Errors, Messages, Asserts, Guards, .. so much terminology') 
In 42 when something takes an unexpected turn,
you can throw an Wcode(error)
This is similar to Java unchecked exceptions.
Every immutable object can be thrown as an error.
While it is possible to thrown informative strings, they do no offer enough
structure to fully take advantage of the error mechanism.
AdamTowel defines the interface Wcode(Message):
a structured way to provide a certain kind of message to the user.
Wcode(Message)s has Wcode(.text()) ,
Wcode(.isResponse()) and Wcode(.responseOf()).
The text is the informative string, while if there is
a response (Wcode(msg.isResponse()==Bool.true()) 
then Wcode(msg.responseOf()) will be the 
former message in the chain.

There are two main kinds of Wcode(Message)s:
Wcode(Guard) and Wcode(Assert).
While Assertions are useful to observe bugs, the application
logic should not depend on them, since they may change
in unpredictable ways during library evolutions, and can be
enabled or disabled.
A guard is guaranteed to be consistent across library evolution
thus program logic can depend on them being thrown.

Assertions are very convenient to check for pre/post conditions.
The following code show usages of Wcode(Assert.Pre) and Wcode(Assert.Bug)

OBCode
Assert.Pre[ //preconditions
  myVal>0Nat;//simplest form
  myVal>0Nat msg:S"here with personalized message";
  myVal expected:42Nat //call equals and do a better error reporting
  ]//in a bunch of assertions, they are all going to be checked together.
Assert.Bug[ //postconditions/checks in the middle
  res expected: 42Nat msg:S" message"
  ]
if notGoodParameter (Assert.Pre"error message")
if observedBug  (Assert.Bug"error message")
CCode

WTitle(`(2/5) create, throw and capture')

You can create new kinds of messages using the 
service class of the message interface:

OBCode
AnswerNotUnderstood:Message.$<<{implements Guard}
//this is a new kind of message, implementing Guard.
//you can also add methods to your kind of message.
//you can add fields, we will see this more in detail later.
/*..*/
//throwing an error
if this.ohNoNoNOOO() (error AnswerNotUnderstood"Well, too bad")

if this.iWasDistracted() (
  //throwing an error in response of another
  Guard other=NotListening""//empty message
  error AnswerNotUnderstood"Try again"(other)
  )

CCode

As you can see, since Wcode(Message) is an interface, it can not help us directly
to create messages, however it has a service nested class called Wcode($), that
is a class decorator helping us to create valid messages.
As you can see we can create messages with text, and
then we can optionally add a response.
Now, let see how to capture errors:

OBCode
(myWait=7.5MillionYear
 answer=DeepThought.answer()
 understanding=myBrain.process(answer)
 catch error AnswerNotUnderstood msg (/*handle it*/)
 catch error Guard msg (/*something else gone wrong*/)
 //in the catch bodies, 'understanding' and 'answer' are not defined
 myBrain.accept(understanding)//normal execution if no issue
 )
CCode

As you see, in 42 there is no explicit Wcode(try) statement,
but any block of code can contain Wcode(catch).
The catch capture errors in the block before the last (group of) catches.
Catches can not see local variables declared in the scope of a possible error.
The following example explain the control flow more in detail:

OBCode
res=(
 b1=CanGoWrong()
 b2=CanGoWrong()//see b1
 catch error Wrong msg1  1Nat//not see b1,b2
 catch error Guard msg2  2Nat//not see b1,b2
 b3=CanGoWrong()//can see b1, b2
 catch error Wrong msg3  3Nat//see b1,b2, not see b3
 4Nat//see b1,b2,b3
 )
CCode
The following code can assign to Wcode(res) either 1,2,3 or 4.
Note how every catch exits from the whole block.
Errors guarantee a property called strong error safety
(strong exception safety in the Java/C++ terminology)
This means that the body of a catch will observe the same 
state present at the start of the block.
This is enforced by disallowing catching errors if the (portion of the) block before can mutate visible objects.

That is, the following code do not compile
OBCode
p=Person(name:S"Bill" age:23Year)
res=(
 p.age(p.age()+1Year)
 p.age(p.age()+1Year)
 catch error Guard msg2  (/*could see p with 23 or 24 years*/)
 p
 )
CCode

While the following is accepted.

OBCode
res=(
 p=Person(name:S"Bill" age:23Year)
 p.age(p.age()+1Year)
 p.age(p.age()+1Year)
 catch error Guard msg2  (/*can not see p*/)
 p
 )
CCode

WTitle(`(3/5) exceptions and errors')

Exceptions are like checked exceptions in java.
As for errors, every immutable object can be thrown as an exception.
just write "exception" instead of "error" while throwing or capturing.
Exceptions represent expected, documented and reliable behaviour,
they are just another way to express control flow.
They are useful to characterize multiple outcomes for an operation,
where is important to prevent the programmer from forgot about
the many possible outcome and focus only on its preferred one.
Exceptions are checked, so methods leaking exceptions have to
declare so in their header, as in the following.
OBCode
/*somewhere in a GUI library*/
method
S promptUser(S text)
exception CancelPressed {
  /*implementation to open a text dialog*/
  }
CCode
Where the programmer using Wcode(promptUser) will have to handle 
the possibility that the cancel button was pressed.

Exceptions does not enforce strong exception safety as error do,
so they can be used more flexibly, and since they are documented in
the types, we can take their existence in account while writing imperative programs.
Wcode(Assert)ions should not be thrown as exceptions, but only as errors.

Often, the programmer wants to just turn exceptions in errors or other exceptions.
This is possible with the following code:


OBCode
//long version
DoStuff()
catch exception FileNotFound fnf (
  error WTF"I just created it!"(fnf)
  )

//short version
DoStuff()
error on FileNotFound
  WTF"I just created it!"
CCode
The two snippets of code behave identically: the first
show a very common patter; 42 supports syntactic sugar to
ease following that pattern, as you can see in the second snippet.
Wcode(WTF) stands for what a terrible failure
(as in android), and can be used to mark branches of code
that the programmer believe would never be executed.
Wcode(WTF) implements Wcode(Assert), thus code capturing
Wcode(WTF) is unreliable.





WTitle(`(4/5) return')

Return, as we have seen, can be used to exit from the inner
most level of curly brackets.
Also curly brackets can have catches, let see some examples
OBCode
{
x=DoStuff()
catch exception Stuff e1
  void//just swallow the exception
catch exception Guard e2
  obj.doSideEffect()//this method return void
catch exception Message e3
  return e3//a result of the curly brackets
y=DoStuff(x)
return y
error on Guard 
  WTF""
}  
CCode

Moreover, curly brackets/return can be used
to provide a different result if some computation fails:

OBCode
res={return PlanA()
  catch error Guard x
    return PlanB()
  }
CCode

WTitle(`return looks similar to error/exception')
Return is actually another thing that can be thrown and captured.
While only immutable values can be thrown as errors/exceptions,
return can throw any kind of value, but returns can not flow
outside of the scope of a method.
Hold your head before it explodes, but curly brackets are just a syntactic sugar
 to capture returns; those two snippets of code are equivalent

OBCode
N res={if bla return e1
  return e2}
//---
N res=(
  Void unused=(
    if bla return e1
    return e2
    )
  catch return N x
    x
  error void//this line is never executed
  )
CCode
Depending on how your brain works,
knowing the desugaring of Wcode({..return..})
can help you to use return better and understand why you can omit 
Wcode({..return..}) for simple method bodies, and why you can
write multiple groups of curly brackets and have local returns.
Or can just be very confusing. If you are in the second group, just
never ever write Wcode(catch return) explicitly and continue
your 42 experience ignoring the issue.


WTitle(`(5/5) Errors, exceptions and return, summary')
<ul><li>
Whenever possible, try to detect if your code misbehave, and 
terminate it with an Wcode(Assert)
</li><li>
Whenever something out of your
control happen, Give it a name and throw it as an error, as in
OBCode
NameOfIssue:Message.$<<{implements Guard}
/*...*/
if /*..*/ error NameOfIssue"more info"
CCode
It just take 2 lines, and will make debugging your code so much 
easier.
</li><li>
Use errors as much as makes sense, 
but use exceptions sparsely: they are needed only in few 
cases, mostly when designing public libraries.
</li><li>
To convert exception into errors or other exceptions, use the convenient short
syntax Wcode(`error on T1,..,Tn  OtherMessage""') or 
Wcode(`exception on T1,..,Tn  OtherMessage""')
</li><li>
Sometimes it is possible to write elegant and correct code
that is not covered in layers upon layers of error/exception checking,
but often is not possible or is not convenient.
Good 42 code can be composed for half of
its content of just error/exception handling/lifting and management.
Do not be scared of turning your code in it's own policemen.
</li></ul>