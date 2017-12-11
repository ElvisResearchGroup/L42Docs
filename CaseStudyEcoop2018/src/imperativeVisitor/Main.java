package imperativeVisitor;

import java.util.HashSet;
import java.util.Set;

public class Main {

}

// e::= x | \x.e | e1 e1 | l | e1-e2
interface Exp {
  <T> T accept(Visitor<T> v);
}

interface Visitor<T> {
  T visit(X e);

  T visit(Lambda e);

  T visit(Application e);

  T visit(Literal e);

  T visit(Subtraction e);
}

class X implements Exp {
  String x;

  public X(String x) {
    this.x = x;
  }

  @Override
  public <T> T accept(Visitor<T> v) {
    return v.visit(this);
  }
}

class Lambda implements Exp {
  String x;
  Exp e;

  public Lambda(String x, Exp e) {
    this.x = x;
    this.e = e;
  }

  @Override
  public <T> T accept(Visitor<T> v) {
    return v.visit(this);
  }
}

class Application implements Exp {
  Exp e1;
  Exp e2;

  Application(Exp e1, Exp e2) {
    this.e1 = e1;
    this.e2 = e2;
  }

  @Override
  public <T> T accept(Visitor<T> v) {
    return v.visit(this);
  }
}

class Literal implements Exp {
  int num;

  Literal(int num) {
    this.num = num;
  }

  @Override
  public <T> T accept(Visitor<T> v) {
    return v.visit(this);
  }
}

class Subtraction implements Exp {
  Exp e1;
  Exp e2;

  Subtraction(Exp e1, Exp e2) {
    this.e1 = e1;
    this.e2 = e2;
  }

  @Override
  public <T> T accept(Visitor<T> v) {
    return v.visit(this);
  }
}

class CloneVisitor implements Visitor<Exp> {
  @Override
  public Exp visit(X e) {
    return new X(e.x);
  }

  @Override
  public Exp visit(Lambda e) {
    return new Lambda(e.x, e.e.accept(this));
  }

  @Override
  public Exp visit(Application e) {
    return new Application(e.e1.accept(this), e.e2.accept(this));
  }

  @Override
  public Exp visit(Literal e) {
    return new Literal(e.num);
  }

  @Override
  public Exp visit(Subtraction e) {
    return new Subtraction(e.e1.accept(this), e.e2.accept(this));
  }
}

class QueryVisitor implements Visitor<Void> {
  @Override
  public Void visit(X e) {
    return null;
  }

  @Override
  public Void visit(Lambda e) {
    e.e.accept(this);
    return null;
  }

  @Override
  public Void visit(Application e) {
    e.e1.accept(this);
    e.e2.accept(this);
    return null;
  }

  @Override
  public Void visit(Literal e) {
    return null;
  }

  @Override
  public Void visit(Subtraction e) {
    e.e1.accept(this);
    e.e2.accept(this);
    return null;
  }
}

class AllVar extends QueryVisitor {
  Set<String> vars = new HashSet<>();

  @Override
  public Void visit(X e) {
    vars.add(e.x);
    return null;
  }

  @Override
  public Void visit(Lambda e) {
    vars.add(e.x);
    return super.visit(e);
  }
}

class Dimension extends QueryVisitor {
  int size = 0;

  @Override
  public Void visit(X e) {
    size++;
    return null;
  }

  @Override
  public Void visit(Lambda e) {
    size++;
    return super.visit(e);
  }

  @Override
  public Void visit(Application e) {
    size++;
    return super.visit(e);
  }

  @Override
  public Void visit(Literal e) {
    size++;
    return null;
  }

  @Override
  public Void visit(Subtraction e) {
    size++;
    return super.visit(e);
  }
}

class FV extends AllVar {
  @Override
  public Void visit(Lambda e) {
    FV fv = new FV();
    e.e.accept(fv);
    fv.vars.remove(e.x);
    vars.addAll(fv.vars);
    return null;
  }
}

class SubstErr extends CloneVisitor {
  String x;
  Exp e;
  Set<String> fve;

  SubstErr(String x, Exp e) {
    this.x = x;
    this.e = e;
    FV fv = new FV();
    e.accept(fv);
    this.fve = fv.vars;
  }

  @Override
  public Exp visit(X e) {
    if (!e.x.equals(this.x)) {
      return e;
    }
    return this.e;
  }

  @Override
  public Exp visit(Lambda e) {
    if (e.x.equals(this.x)) {
      return e;
    }
    if (this.fve.contains(e.x)) {
      throw new RuntimeException();
    }
    return super.visit(e);
  }
}

// alpha x into yFresh:
// x-->y; \ x.e -->\ x.e \ y.e -->\ z.e[x-->y] with x!=z, z!=y
class AlphaXFresh extends CloneVisitor {
  String x;
  String yFresh;

  AlphaXFresh(String x, Set<String> used) {
    this.x = x;
    for (int num = 0;; num++) {
      String current = x + num;
      if (used.contains(current)) {
        continue;
      }
      used.add(current);
      this.yFresh = current;
      return;
    }
  }

  @Override
  public Exp visit(X e) {
    if (!e.x.equals(this.x)) {
      return e;
    }
    return new X(yFresh);
  }

  @Override
  public Exp visit(Lambda e) {
    if (!e.x.equals(this.x)) {
      return super.visit(e);
    }
    return new Lambda(this.yFresh, e.e.accept(this));
  }
}

class Subst extends SubstErr {
  Set<String> used;

  Subst(String x, Exp e, Set<String> used) {
    super(x, e);
    this.used = used;
  }

  @Override
  public Exp visit(Lambda e) {
    if (e.x.equals(this.x)) {
      return e;
    }
    if (this.fve.contains(e.x)) {
      AlphaXFresh alpha = new AlphaXFresh(e.x, used);
      Exp body = e.e.accept(alpha).accept(this);
      return new Lambda(alpha.yFresh, body);
    }
    return super.visit(e);
  }
}

class FullBeta extends CloneVisitor {
  Set<String> used;

  FullBeta(Set<String> used) {
    this.used = used;
  }

  @Override
  public Exp visit(Application e) {
    if (!(e.e1 instanceof Lambda)) {
      return super.visit(e);
    }
    Lambda l = (Lambda) e.e1;
    Exp arg = e.e2.accept(this);
    return l.e.accept(new Subst(l.x, arg, used));
  }

  @Override
  public Exp visit(Subtraction e) {
    if (!(e.e1 instanceof Literal && e.e2 instanceof Literal)) {
      return super.visit(e);
    }
    Literal l1 = (Literal) e.e1;
    Literal l2 = (Literal) e.e2;
    return new Literal(l1.num - l2.num);
  }
}

class Minimize {
  static Exp minimizeMany(Exp input) {
    AllVar vars = new AllVar();
    input.accept(vars);
    FullBeta b = new FullBeta(vars.vars);
    Dimension next = new Dimension();
    input.accept(next);
    Exp e = input;
    Dimension current = next;
    do {
      current = next;
      e = e.accept(b);
      next = new Dimension();
      e.accept(next);
    } while (current.size > next.size);
    return e;
  }
}

class ToS implements Visitor<String> {
  public String visit(X e) {
    return e.x;
  }

  public String visit(Lambda e) {
    return "(\\" + e.x + "." + e.e.accept(this) + ")";
  };

  public String visit(Application e) {
    return e.e1.accept(this) + " " + e.e2.accept(this);
  }

  public String visit(Literal e) {
    return "" + e.num;
  }

  public String visit(Subtraction e) {
    return e.e1.accept(this) + " - " + e.e2.accept(this);
  }
}