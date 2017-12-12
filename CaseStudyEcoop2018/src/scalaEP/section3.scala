trait Base {
  type exp <: Exp;
  trait Exp {
    def eval: Int
  }
  class Num(v: Int) extends Exp {
    val value = v;
    def eval = value
  }
}

trait BasePlus extends Base {
  class Plus(l: exp, r: exp) extends Exp {
    val left = l; val right = r;
    def eval = left.eval + right.eval
  }
}

trait BaseNeg extends Base {
  class Neg(t: exp) extends Exp {
    val term = t;
    def eval = - term.eval;
  }
}

trait BasePlusNeg extends BasePlus with BaseNeg;

trait Show extends Base {
  type exp <: Exp;
  trait Exp extends super.Exp {
    def show: String;
  }
  class Num(v: Int) extends super.Num(v) with Exp {
    def show = value.toString();
  }
}

trait ShowPlusNeg extends BasePlusNeg with Show {
  class Plus(l: exp, r: exp) extends super.Plus(l, r) with Exp {
    def show = left.show + "+" + right.show;
  }
  class Neg(t: exp) extends super.Neg(t) with Exp {
    def show = "-(" + term.show + ")";
  }
}

trait DblePlusNeg extends BasePlusNeg {
  type exp <: Exp;
  trait Exp extends super.Exp {
    def dble: exp;
  }
  def Num(v: Int): exp;
  def Plus(l: exp, r: exp): exp;
  def Neg(t: exp): exp;
  class Num(v: Int) extends super.Num(v) with Exp {
    def dble = Num(v * 2);
  }
  class Plus(l: exp, r: exp) extends super.Plus(l, r) with Exp {
    def dble = Plus(left.dble, right.dble);
  }
  class Neg(t: exp) extends super.Neg(t) with Exp {
    def dble = Neg(t.dble);
  }
}

trait ShowDblePlusNeg extends ShowPlusNeg with DblePlusNeg {
  type exp <: Exp;
  trait Exp extends super[ShowPlusNeg].Exp
               with super[DblePlusNeg].Exp;
  class Num(v: Int)
    extends super[ShowPlusNeg].Num(v)
       with super[DblePlusNeg].Num(v)
       with Exp;
  class Plus(l: exp, r: exp)
    extends super[ShowPlusNeg].Plus(l, r)
       with super[DblePlusNeg].Plus(l, r)
       with Exp;
  class Neg(t: exp)
    extends super[ShowPlusNeg].Neg(t)
       with super[DblePlusNeg].Neg(t)
       with Exp;
}

trait Equals extends Base {
  type exp <: Exp;
  trait Exp extends super.Exp {
    def eql(other: exp): Boolean;
    def isNum(v: Int) = false;
  }
  class Num(v: Int) extends super.Num(v) with Exp {
    def eql(other: exp): Boolean = other.isNum(v);
    override def isNum(v: Int) = v == value;
  }
}

trait EqualsPlusNeg extends BasePlusNeg with Equals {
  type exp <: Exp;
  trait Exp extends super[BasePlusNeg].Exp
               with super[Equals].Exp {
    def isPlus(l: exp, r: exp): Boolean = false;
    def isNeg(t: exp): Boolean = false;
  }
  class Num(v: Int) extends super[Equals].Num(v)
                       with Exp;
  class Plus(l: exp, r: exp) extends Exp
                                with super.Plus(l, r) {
    def eql(other: exp): Boolean = other.isPlus(l, r);
    override def isPlus(l: exp, r: exp) = (left eql l) && (right eql r)
  }
  class Neg(t: exp) extends Exp
                       with super.Neg(t) {
    def eql(other: exp): Boolean = other.isNeg(t);
    override def isNeg(t: exp) = term eql t
  }
}

trait EqualsShowPlusNeg extends EqualsPlusNeg
                           with ShowPlusNeg {
  type exp <: Exp;
  trait Exp extends super[EqualsPlusNeg].Exp
               with super[ShowPlusNeg].Exp;
  class Num(v: Int)
    extends super[EqualsPlusNeg].Num(v)
       with super[ShowPlusNeg].Num(v)
       with Exp;
  class Plus(l: exp, r: exp)
    extends super[EqualsPlusNeg].Plus(l, r)
       with super[ShowPlusNeg].Plus(l, r)
       with Exp;
  class Neg(term: exp)
    extends super[EqualsPlusNeg].Neg(term)
       with super[ShowPlusNeg].Neg(term)
       with Exp;
}

//tests below

object BaseTest extends Base with App {
  type exp = Exp;
  val e: exp = new Num(7);
  Console.println(e.eval);
}

object ShowPlusNegTest extends ShowPlusNeg with App {
  type exp = Exp;
  val e: exp = new Neg(
  new Plus(new Num(7), new Num(6)))
  Console.println(e.show + " = " + e.eval);
}

object DblePlusNegTest extends DblePlusNeg with App {
  type exp = Exp;
  def Num(v: Int): exp = new Num(v);
  def Plus(l: exp, r: exp): exp = new Plus(l, r);
  def Neg(t: exp): exp = new Neg(t);
  val e: exp = Plus(Neg(Plus(Num(1), Num(2))), Num(3));
  Console.println(e.dble.eval);
}

object EqualsShowPlusNegTest extends EqualsPlusNeg
                                with App {
  type exp = Exp;
  val term1 = new Plus(new Num(1), new Num(2));
  val term2 = new Plus(new Num(1), new Num(2));
  val term3 = new Neg(new Num(2));
  Console.print(term1.show + "=" + term2.show + "? ");
  Console.println(term1 eql term2);
  Console.print(term1.show + "=" + term3.show + "? ");
  Console.println(term1 eql term3);
}