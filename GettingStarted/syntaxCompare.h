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

reuse L42.is/AdamsTowel  //importing code
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


Javascript"
'x= function(){
'  alert("hello world";)
'  }
' x()
" //multiline string

14Size  //number suitable to be an index
100Meter  //100 meter
25Kg  //25 kg, 
//25Kg+100Meter do not compile
CCode

OJCode
//(sort of) equivalent Java code 
"hello world"  //String

URI.create("www.google.com") //explicit parsing
//(and Java URL class is discouraged)

Javascript.create( //poor man solution
"x= function(){",
"  alert(\"hello world\";)",
"  }",
" x()"
) //painful. Note how we need to escape "

14  //general purpose int
100L  // may be we want long distances
25d  //may be we want high precision weight
//25d+100L is a double :)
CJCode
</div>





<!-- ############################################################################-->
<!-- ############################################################################-->
<!-- ############################################################################-->
<!-- ############################################################################-->

WTitle(Hello world)
<div class= "compare">
OCode
{reuse L42.is/AdamsTowel
Main: { //Note: Main is not a method, just a 'task'
  Debug(S"Hello world")
  return ExitCode.success()
  }
}
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
S answer() //return type and method name
  S"42" //simple bodies do not need {..return..}

class method //class method definition
S compose(S left, S right) //paramters
  left++right //sequence concatenation

Person: Data <>< { //declare a class Person
  S name  //fields
  Year age
//in 42 fields are seen as getters and setters,
//thus there is no need to declare those.
//Also, no need to write down a constructor: 
//Data declares a 'name, age' factory for us.

//Data generates implementations for methods like
//equality, hashCode and conversion to string.

  method //instance method definition
  S sayHi(S to){  //here we use {..return..}
    if to==this.name() ( //no parenthesis on if/while
      return S"Oh, you are called "[this.name()]" too?"
      )  //string interpolation
    return S"Hi, my name is "[this.name()]
      " and  I'm "[this.age()]" years old"
    }
  }
CCode

OJCode
//(sort of) equivalent Java code 
static String answer(){ //static method definition
  return "42";
  }

static String compose(S left, S right){
  return left+right; //magically specially overriden +
  }

class Person{ //declare a class Person
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
var S acc= S""  //explicit 'var': can be updated
while Bool.true() ( //booleans are not constants
  if acc.size()>50Size ( //Size: type of indexes 
    exception void //like java break, rarely used in 42
    )
  acc++= S"step"
  )
CCode

OJCode
if (a>b) {return /*...*/;}
S acc= ""; //updatable is default, but can be final
while (true) {
  if (acc.length()>50) {
    break;
    }
  acc+= "step";
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
names= Strings[S"Fred";S"Mary";S"Mark"]

Years: Collection.vector(of: Year) //we declare it

ages= Years[20Year;23Year;22Year]


with n in names.vals() ( //like a for each
  Debug(n)
  )

with n in names.vals(), a in ages.vals() (
//parallel iteration of names and ages
  Debug(Person(name: n, age: a)) //print all the persons
  ) //fails with error if names.size()!= ages.size()

Persons: Collection.vector(of: Person)

friends= Persons[
  Person(name: S"Fred", age: 20Year);
  Person(name: S"Mary", age: 23Year);
  Person(name: S"Mark", age: 22Year);
  ]

moreFriends= Persons[
  with n in names.vals(), a in ages.vals() (
    use[ Person(name: n, age: a) ]
  )]
//moreFriends contains the same as friends

//with works also in string interpolation: 
stringFriends= S"my friends ages are"[
  with n in names.vals(), a in ages.vals() (
    use[ a++S": "++n, sep: S", ") ]
  )]"" //== S"20: Fred, 23: Mary, 22: Mark"
CCode

OJCode
List<String> names= new ArrayList<>(Arrays.asList(
  "Fred","Mary","Mark"));


List<Integer> ages= new ArrayList<>(Arrays.asList(
  20,23,22));

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
} //silently buggy if names.size()<ages.size()

//again, silently buggy if names.size()<ages.size()
String stringFriends= "my friends ages are";
for(int i= 0; i<names.size(); i++){
  if(i>0){stringFriends+= ", ";}
  stringFriends+= ages.get(i)+": "+names.get(i);
}
CJCode
</div>




</p></Div>
 
