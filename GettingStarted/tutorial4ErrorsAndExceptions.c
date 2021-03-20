WBigTitle(Errors and Exceptions: Messages in AdamsTowel)

WTitle((1/5)`Errors, Messages, Asserts, Guards, .. so much terminology') 
In 42 when something takes an unexpected turn,
you can throw an Wcode(error)
This is similar to Java unchecked exceptions.
Every immutable object can be thrown as an error.
While it is possible to thrown informative strings, they do no offer enough
structure to fully take advantage of the error mechanism.
AdamsTowel defines the interface Wcode(Message): 
a structured way to provide a certain kind of message to the user.
There are two main kinds of Wcode(Message)s: 
Wcode(Guard) and Wcode(Assert).
While Assertions are useful to observe bugs, the application
logic should not depend on them, since they may change
in unpredictable ways during library evolutions, and can be enabled or disabled.
Since program logic can depend on guards being thrown,
guards need to be consistent across library evolution.

Assertions are the right tool to prevent the code from proceding
out of our designed space. The assertion class called Wcode(X) 
looks like a road sign
and represent this 
"NO/PROHIBITED/FORBIDDEN" 
feeling.

Assertions are also very convenient to check for pre/post conditions.
The following code show usages of Wcode(X.Pre) (for preconditions and, in general, blaming the client of a function)
 and Wcode(X) (for postconditions checks in the middle and, in general, blaming the function implementation).

OBCode
method Nat confirmAnswer(Nat answer) (
  X.Pre[ //preconditions
    answer>0Num; //simplest form
    answer<10000Num msg=S"here with personalized message answer= %answer";
    actual=answer, expected=42Num //do a better error reporting
    ] //in a bunch of assertions, they are all going to be checked/reported together.
  recomputedAnswer=6Num*7Num
  X[//postconditions/checks in the middle
    actual=recomputedAnswer
    expected=42Num
    ]
  X[answer==recomputedAnswer]
  if answer>50Num (//how to just throw error X
    error X""
    )
CCode

Wcode(X) is often used as last case in a sequence of if-return:

OBCode
Direction = Collection.Enum:{
  North={} East={} South={} Weast={}
  }
/*..*/
Direction opposite={
  if d==Direction.North() return Direction.South()
  if d==Direction.South() return Direction.North()
  if d==Direction.East() return Direction.West()
  X[d==Direction.West()] return Direction.East()
  }
CCode
As you can see, since there are only 4 directions, we believe by exclusion that the last case must hold. However, we prefer to
make our assumptions clear and have them checked.


WTitle(`(2/5) Create, throw and capture')

WTitle(Create and throw)

You can create new kinds of messages using Wcode(Message)
as a decorator:

OBCode
AnswerNotUnderstood = Message:{[Message.Guard]}
//this is a new kind of message, implementing Guard.
//you can also add methods to your kind of message.
//you can add fields, we will see this more in detail later.
/*..*/
//throwing an error
if this.ohNoNoNOOO() (error AnswerNotUnderstood"Well, too bad")
CCode

You may be surprised that we can use Wcode(Message) as a decorator, since it is an interface.
When operators are called on class names directly, they are desugared as a method on one of the class nested classes. For example
Wcode(`Message:{..}') becomes
Wcode(`Message.ClassOperators.#colon0({..})').
It is very common for interfaces to be usable as decorator creating new code with a meaningful default implelemtation for such an interface.

WTitle(Capturing errors and exceptions)

In 42 there is no explicit Wcode(try) statement,
but any block of code delimited by round or curly brackets can contain Wcode(catch).

OBCode
res = (
 b1 = CanGoWrong()
 b2 = CanGoWrong() //see b1
 catch error Wrong msg1  S"hi 1" //not see b1, b2
 catch error Message.Guard msg2  S"hi 2" //not see b1, b2
 b3 = CanGoWrong() //can see b1, b2
 b4 = CanGoWrong() //can see b1, b2, b3
 S"hi 3" //can see b1, b2, b3, b4
 )
CCode
The catches above do not see local variables Wcode(b1) and Wcode(b2) because they may be capturing an error raised by the execution of the initialization of such variable.
L42 never exposes uninitialized data.
If a catch is successful, then the result of its catch expression
will be the result of the whole code block.
In this way, blocks with catches behave like conditionals.
That is, The code above can assign to Wcode(res) either 
Wcode(S"hi 1"),
Wcode(S"hi 2") or
Wcode(S"hi 3").

WTitle(Strong error safety)

Errors guarantee a property called strong error safety
(strong exception safety in the Java/C++ terminology)
This means that the body of a catch will observe the same 
state present at the start of the paragraph before.
This is enforced by disallowing catching errors if the paragraph can mutate objects visible in the catch expression.
WBR
That is, the following code do not compile
OBCode
p = Person(name=S"Bill" age=23Year)
res = (
 p.age(p.age()+1Year)
 p.age(p.age()+1Year)
 catch error Message.Guard msg2 (
   /*could see p with 23 or 24 years*/)
 p
 )
CCode

While the following is accepted.

OBCode
res = (
 p = Person(name=S"Bill" age=23Year)
 p.age(p.age()+1Year)
 p.age(p.age()+1Year)
 catch error Message.Guard msg2 (/*can not see p*/)
 p
 )
CCode

WTitle(`(3/5) Exceptions and errors')

Exceptions are like checked exceptions in java.
As with errors, every immutable object can be thrown as an exception.
You can just write Wcode(exception) instead of Wcode(error) while throwing or capturing. When capturing, Wcode(exception) is the default, so you can write Wcode(catch Foo x)
instead of Wcode(catch exception Foo x)
Exceptions represent expected, documented and reliable behaviour,
they are just another way to express control flow.
They are useful to characterize multiple outcomes of an operation,
where is important to prevent the programmer from forgetting about
the many possible outcome and focusing only on their preferred one.
Exceptions are checked, so methods leaking exceptions have to
mention it in their header, as in the following.
OBCode
/*somewhere in a GUI library*/
method
S promptUser(S text)[CancelPressed] = {
  /*implementation to open a text dialog*/
  }
CCode
The programmer using Wcode(promptUser) has to handle 
the possibility that the cancel button was pressed.
However, L42 supports exception inference; to simply propagate all the exceptions leaked out by the other methods called in a method body, you can write Wcode(_), as shown below:
OBCode
/*somewhere in a GUI library*/
method
S promptUser(S text)[_] = {
  /*implementation to open a text dialog*/
  }
CCode
Exceptions does not enforce strong exception safety as errors do,
so they can be used more flexibly, and since they are documented in
the types, we can take their existence in account while writing imperative programs.
WP
Often, the programmer wants to just turn exceptions into errors.
While this can be done manually, L42 offers a convenient syntax: Wcode(whoops).

OBCode
//long version
Res foo = {
  return DoStuff()
  catch FileNotFound fnf 
    error fnf
  catch FileCorrupted fc
    error fc
  }

//short version
Res foo = {
  return DoStuff()
  whoops FileNotFound, FileCorrupted
  }
CCode

The two snippets of code behave near identically: 
in the second the exceptions are also notified of the 
position in the code where they are Wcode(whoopsed).
This is conceptually similar to the very common Java patten to wrap checked exceptions into uncheced ones.
WBR

As we shown before,  we can use Wcode(X) to mark branches of code
that the programmer believes would never be executed.
Wcode(X) implements Wcode(Assert), thus code capturing
Wcode(X) is unreliable: as explained before, AdamTowel programmers are free
to change when and how assertion violations are detected.
In particular, the programmer may recognize that
such branch could be actually executed, and thus replace the error with correct behaviour.
WP
Wcode(Assert)ions should not be thrown as exceptions, but only as errors.


WTitle(`(4/5) Return')

Return, as we have seen, can be used to exit from the inner
most level of curly brackets.
Also curly brackets can have catches.
In this case, all catch bodies must ends with
Wcode(return),
Wcode(error) or
Wcode(exception).
WBR Let's see some examples: 
OBCode
{
  x = DoStuff()
  catch Stuff e1
    return void //just swallow the exception (this block return 'Void')
  catch Message.Guard e2 (
    obj.doSideEffect()
    return void //do something and return
    )
  catch error Message e3
    error X"not supposed to happen"
  (//example of a nested block
    y = DoStuff(x)
    return y
    whoops Message.Guard
    )
  }  
CCode

Moreover, curly brackets/return can be used
to provide a different result if some computation fails: 

OBCode
res = {
  return PlanA()
  catch error Message.Guard x
    return PlanB()
  }
CCode

WTitle(`Return looks similar to error/exception')
Return is actually another thing that can be thrown and captured.
While only immutable values can be thrown as errors/exceptions,
return can throw any kind of value, but returns can not flow
outside of the scope of a method.
Hold your head before it explodes, but curly brackets are just a syntactic sugar
 to capture returns; these two snippets of code are equivalent: 
<div class= "compare">
OBCode
N res = {
  
  if bla (return e1)
  return e2
  


  
  }
CCode
OBCode
N res = (
  Void unused= (
    if bla ( return e1 )
    return e2
    )
  catch return N x ( x )
  error void //this line is never executed
  )
CCode
</div>
WP
Depending on how your brain works,
knowing the desugaring of Wcode({..return..})
can help you to use return better and understand why you can omit 
Wcode({..return..}) for simple method bodies, and why you can
write multiple groups of curly brackets and have local returns.
Or it may just be very confusing. If you are in the second group, just
never ever write Wcode(catch return) explicitly and continue
your 42 experience ignoring the issue.


WTitle(`(5/5) Errors, exceptions and return, summary')
<ul><li>
Always detect if your code misbehaves, and 
terminate it with an Wcode(Assert)
</li><li>
Whenever something out of your
control happen, Give it a name and throw it as an error, as in
OBCode
NameOfIssue = Message:{[Message.Guard]}
/*...*/
if /*..*/ ( error NameOfIssue"more info" )
CCode
It just take 2 lines, and will make debugging your code so much 
easier.
</li><li>
Use errors intensivelly, 
but use exceptions sparsely: they are needed only in few 
cases, mostly when designing public libraries.
</li><li>
To convert exception into errors or other exceptions, use the convenient short
syntax Wcode(`whoops T1,..,Tn""').
</li><li>
Instead of manually writing long lists of leaked exceptions, just use Wcode([_]).
</li><li>
It is sometimes possible to write elegant and correct code
that is not covered in layers upon layers of error/exception checking,
but often is not possible or not convenient.
Up to half of good 42 code will be composed of
just error/exception handling/lifting and management.
Do not be scared of turning your code in it's own policemen.
</li></ul>