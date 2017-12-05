package exampleGenericThisType;

public class Main {

}

abstract class A<Self extends A<Self>>{
  int x;int y; A(int x,int y){this.x=x;this.y=y;}
  abstract Self self();
  abstract Self self(int x,int y);
  Self withX(int x) {return self(x,y);}
  Self withY(int y) {return self(x,y);}
}
abstract class APointSum<Self extends APointSum<Self>> extends A<Self>{
  APointSum(int x, int y) {super(x, y);}}

class PointSum extends APointSum<PointSum>{PointSum(int x, int y) {super(x, y);}
PointSum self() {return this;}
PointSum self(int x,int y) {return new PointSum(x,y);}
}

class C extends A<C>{
  C(int x, int y) {super(x, y);}
  C self() {return this;}
  C self(int x,int y) {return new C(x,y);}
  }
