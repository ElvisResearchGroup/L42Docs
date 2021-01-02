<Div id= "b"><p>
WTitle(Learn through examples)
If you like to learn by examples, 
here you can find snippets of code in 42 and their equivalent version in Java.
We format the code so that, line by line, you can see the differences.


<!-- WTitle()
<div class= "compare">
OCode
CCode

OJCode
CJCode
</div>-->

<!-- ############################################################################-->
<!-- ############################################################################-->
<!-- ############################################################################-->
<!-- ############################################################################-->
WTitle(Imports and comments)
<div class= "compare">
OCode
//42 code
//comment

/*multiline comment*/

reuse [L42.is/AdamsTowel]  //importing code
CCode

OJCode
//(sort of) equivalent Java code 
//comment

/*multiline comment*/

import java.utils.ArrayList;  //importing code
...  //more and more importing code
import java.utils.List;  //eventually, end importing
CJCode
</div>


<!-- ############################################################################-->
<!-- ############################################################################-->
<!-- ############################################################################-->
<!-- ############################################################################-->

WTitle(String and numbers)
<div class= "compare">
OCode
//42 code
S"hello world"  //String

Url"www.google.com" //Specially formatted string


Javascript"""
  |x= function(){
  |  alert("hello world";)
  |  }
  | x()
  """ //multiline string

14I  //number suitable to be a collection index
1Num// arbitrary precision rational
Num"-13" //negative number
Num"13/234" //number as fraction
CCode

OJCode
//(sort of) equivalent Java code 
"hello world"  //String

URI.create("www.google.com") //explicit parsing

Javascript.create("""
 x= function(){
   alert("hello world");
   }
 x()
 """)//using the new multiline strings

14  //general purpose int
new BigInteger("1");// big integer, some libraries
//may have 'BigRational' too.
25d  //to specify 'non int' you add the 'type' later.
//25d+100L is a double, while in 42 
//it would require an explicit conversion.
CJCode
</div>





<!-- ############################################################################-->
<!-- ############################################################################-->
<!-- ############################################################################-->
<!-- ############################################################################-->

WTitle(Hello world)
<div class= "compare">
OCode
reuse [L42.is/AdamsTowel]
Main =  //Note: Main is not a method, just a 'task'
  Debug(S"Hello world")
CCode

OJCode
//implicitly imports java.lang
class MyClassName{
  public static void main(String[] args){
    System.out.println("Hello world");
  }
}
CJCode
</div>
<!-- ############################################################################-->
<!-- ############################################################################-->
<!-- ############################################################################-->
<!-- ############################################################################-->




WTitle(Methods and Classes)
<div class= "compare">
OCode
//42 code
class method //class method definition
S answer() = //return type and method name
  S"42" //simple bodies do not need {..return..}

class method //class method definition
S compose(S left, S right) //parameters
  left++right //sequence concatenation

Person = Data : { //define a class Person
  S name  //fields are just getters/setters
  var Year age //only var fields have setters
//no need to manually define getters/setters
//Also, no need to write down a constructor: 
//Data defines a 'name, age' factory for us.

//Data generates implementations for methods like
//equality, hashCode and conversion to string.

  method //instance method definition
  S sayHi(S to) = {  //here we use {..return..}
    if to==this.name() ( //no parenthesis on if/while
      return S"Oh, you are called %this.name() too?"
      )  //string interpolation
    return S"""%%
      |Hi, my name is %%this.name()
      |and  I'm %%this.age() years old
      """//Note: specified interpolation sequence
    }
  class method//This == Person here, 
  This bob() = //simple method instantiating a Person
    This(name:S"Bob", age: 23Year)
  }
CCode

OJCode
//(sort of) equivalent Java code 
static String answer(){ //static method definition
  return "42";
  }

static String compose(S left, S right){
  return left+right; //magically specially overridden +
  }

class Person{ //define a class Person
  String name;  //fields
  int age;
  String getName(){return this.name;}
  int getAge(){return this.age;}
  public Person(String name, int age){
    this.name= name;this.age= age; //even longer
  } //if standard formatting is used
  public boolean equals(Object other){/*...*/}
  public int hashCode(){/*was int or long?*/}
  public String toString(){/*...*/}
  
  public String sayHi(String to){
    if (to.equals(this.getName()) {
      return "Oh, you are called "+name+" too?";
    } //is 'name' better then 'this.getName()'?
    return "Hi, my name is "+this.getName()
      +" and  I'm "+this.getAge()+" years old";
  }
  public static Person bob(){
    return new Person("Bob",23);
  }
}

CJCode
</div>
<!-- ############################################################################-->
<!-- ############################################################################-->
<!-- ############################################################################-->
<!-- ############################################################################-->



WTitle(Control structures)
<div class= "compare">
OCode
if a>b (return /*...*/)
var acc = S""  //explicit 'var': can be updated
while Bool.true() ( //booleans are not constants
  if acc.size()>50I ( //Size: type of indexes 
    Break()
    )
  acc ++= S"step"
  )
CCode

OJCode
if (a>b) {return /*...*/;}
S acc= ""; //updatable is default, but can be final
while (true) {
  if (acc.length()>50) {
    break;
    }
  acc += "step";
}
CJCode
</div>


<!-- ############################################################################-->
<!-- ############################################################################-->
<!-- ############################################################################-->
<!-- ############################################################################-->



WTitle(Vectors)
<div class= "compare">
OCode
Years = Collections.list(Year) //we define it
...
names = S.List[S"Fred";S"Mary";S"Mark"]
ages = Years[20Year;23Year;22Year]

for n in names ( Debug(n) )

for n in names, a in ages (
//parallel iteration of names and ages
  Debug(Person(name= n, age= a)) //print all the persons
  ) //fails with error if names.size()!= ages.size()

Persons = Collections.list(Person)

friends = Persons[
  Person(name=S"Fred", age=20Year);
  \(name=S"Mary", age=23Year);//here \ == Person
  \(name=S"Mark", age=22Year);// == the expected type
  ]

moreFriends = Persons()(
  for n in names, a in ages 
    \add(\(name=n, age=a))
  )
//moreFriends contains the same elements as friends

var stringFriends= S"my friends ages are "
for n in names.vals(), a in ages.vals() (
  stringFriends += S"%a: %n"++S.nl()//there is no \n
  )
CCode

OJCode
List<String> names= List.of("Fred","Mary","Mark");


List<Integer> ages= List.of(20,23,22);

for(String n : names){
  System.out.println(n);
}

for(int i= 0; i<names.size(); i++){
//iterate 0 to size, and manualy use .get(i)
  System.out.println(
    new Person(names.get(i), ages.get(i)));
  ); //silently buggy if names.size()<ages.size()
}

List<Person> friends= new ArrayList<>(Arrays.asList(
  new Person("Fred",20),
  new Person("Mary"23),
  new Person("Mark"22)
  ));

List<Person> moreFriends= new ArrayList<>();
for(int i= 0; i<names.size(); i++){
  moreFriends.add(
    new Person(names.get(i), ages.get(i)));
} //here and below: silent bug if names.size()<ages.size()

String stringFriends= "my friends ages are";
for(int i= 0; i<names.size(); i++){
  stringFriends+= ages.get(i)+": "+names.get(i)+"\n";
}
CJCode
</div>

</p></Div>
 
