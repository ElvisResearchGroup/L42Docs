package imperativeVisitor;


import org.junit.Assert;
import org.junit.jupiter.api.Test;


class TestSimpl {
  static Exp x(String x) {return new X(x);}
  static Exp la(String x,Exp b) {return new Lambda(x,b);}
  static Exp a(Exp e1,Exp e2) {return new Application(e1,e2);}
  static Exp li(int n) {return new Literal(n);}
  static Exp s(Exp e1,Exp e2) {return new Subtraction(e1,e2);}
  static Exp id() {return la("x",x("x"));}
  static Exp omega() {return la("x",a(x("x"),x("x")));}
  static Exp freeY() {return la("x",x("y"));}
  static Exp freeX() {return la("y",x("x"));}
  static Exp replaceY() {return a(la("y",x("y")),freeY());}
  static Exp replaceY2() {return a(la("x",freeX()),freeY());}
  @Test
  void testToS() {
    Exp e=la("x",a(x("x"),x("x")));
    Assert.assertEquals(e.accept(new ToS()),"(\\x.x x)");
    Exp e1=la("x",s(li(1),x("x")));
    Assert.assertEquals(e1.accept(new ToS()),"(\\x.1 - x)");
  }
  @Test
  void testSimplIdId() {
    Exp e=a(id(),id());
    Exp res=Minimize.minimizeMany(e);
    Assert.assertEquals(res.accept(new ToS()),"(\\x.x)");
  }
  @Test
  void testSimplIdIdId() {
    Exp e=a(id(),a(id(),id()));
    Exp res=Minimize.minimizeMany(e);
    Assert.assertEquals(res.accept(new ToS()),"(\\x.x)");
  }
  @Test
  void testFreVar() {
    Assert.assertEquals(replaceY().accept(new ToS()),"(\\y.y) (\\x.y)");
    Exp res=Minimize.minimizeMany(replaceY());
    Assert.assertEquals(res.accept(new ToS()),"(\\x.y)");
  }
  @Test
  void testFreVarRenamed() {
    Assert.assertEquals(replaceY2().accept(new ToS()),"(\\x.(\\y.x)) (\\x.y)");
    Exp res=Minimize.minimizeMany(replaceY2());
    Assert.assertEquals(res.accept(new ToS()),"(\\y0.(\\x.y))");
  }

}
