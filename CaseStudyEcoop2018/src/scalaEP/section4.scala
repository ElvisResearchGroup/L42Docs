trait Base {
  trait Exp {
    def accept(v: visitor): unit;
  }
  class Num(value: Int) extends Exp {
    def accept(v: visitor): unit = v.visitNum(value);
  }
  type visitor <: Visitor;
  trait Visitor {
    def visitNum(value: Int): unit;
  }
  class Eval: visitor extends Visitor {
    var result: Int = _;
    def apply(t: Exp): Int = { t.accept(this); result }
    def visitNum(value: Int): unit = {
      result = value;
    }
  }
}

trait BasePlus extends Base {
  type visitor <: Visitor;
  trait Visitor extends super.Visitor {
    def visitPlus(left: Exp, right: Exp): unit; 
  }
  class Plus(left: Exp, right: Exp) extends Exp {
    def accept(v: visitor): unit = v.visitPlus(left, right);
  }
  class Eval: visitor extends super.Eval with Visitor {
    def visitPlus(l: Exp, r: Exp): unit = {
      result = apply(l) + apply(r);
    }
  }
}

trait BaseNeg extends Base {
  type visitor <: Visitor;
  trait Visitor extends super.Visitor {
    def visitNeg(term: Exp): unit;
  }
  class Neg(term: Exp) extends Exp {
    def accept(visitor: v): unit = visitor.visitNeg(term);
  }
  class Eval: visitor extends super.Eval with Visitor {
    def visitNeg(term: Exp): unit = {
      result = -apply(term);
    }
  }
}

trait BasePlusNeg extends BasePlus with BaseNeg {
  type visitor <: Visitor;
  trait Visitor extends super.Visitor
                   with super[BaseNeg].Visitor;
  class Eval: visitor extends super.Eval
                         with super[BaseNeg].Eval
                         with Visitor;
}

object BasePlusNegTest extends BasePlusNeg {
  type visitor = Visitor;
  val op: visitor = new Eval;
  Console.println(op.apply( new Plus(new Num(1), new Neg(new Num(2))) ));
}

trait DblePlusNeg extends BasePlusNeg {
  class Dble: visitor extends Visitor {
    var result: Exp = _;
    def apply(t: Exp): Exp = {
      t.accept(this); result
    }
    def visitNum(value: Int): unit = {
      result = new Num(2 * value)
    }
    def visitPlus(l: Exp, r: Exp): unit = {
      result = new Plus(apply(l), apply(r))
    }
    def visitNeg(term: Exp): unit = {
      result = new Neg(apply(term))
    }
  }
}

trait ShowPlusNeg extends BasePlusNeg {
  class Show: visitor extends Visitor {
    var result: String = _;
    def apply(t: Exp): String = {
      t.accept(this); result
    }
    def visitNum(value: int): unit = {
      result = value.toString()
    }
    def visitPlus(l: Exp, r: Exp): unit = {
      result = apply(left) + "+" + apply(right)
    }
    def visitNeg(term: Exp): unit = {
      result = "-(" + apply(term) + ")"
    }
  }
}

trait ShowDblePlusNeg extends DblePlusNeg
                         with ShowPlusNeg;


