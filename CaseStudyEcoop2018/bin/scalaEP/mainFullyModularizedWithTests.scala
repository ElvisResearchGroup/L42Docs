trait Base {
  type exp <: Exp

  trait Exp {  }
}

trait BaseNum extends Base {

  class Num(val value: Int) extends Exp {  }

  type BaseNum = Num
}

trait BasePlus extends Base {

  class Plus(val left: exp, val right: exp) extends Exp {  }

  type BasePlus = Plus
}

trait BaseNeg extends Base {

  class Neg(val term: exp) extends Exp {  }

  type BaseNeg = Neg
}

trait BasePlusNeg extends BasePlus with BaseNeg




//----------------------EVAL
trait Eval extends Base {
  type exp <: Exp

  trait Exp extends super.Exp {
    def eval: Int
  }
}

//----------------------EVALNUM
trait EvalNum extends BaseNum with Eval {

  trait NumBehavior {
    self: BaseNum =>
    def eval: Int = value
  }

   class Num(v: Int) extends BaseNum(v) with NumBehavior with Exp

}

//----------------------EVALPLUS
trait EvalPlus extends BasePlus with Eval {

  trait PlusBehavior {
    self: BasePlus =>
    def eval = left.eval + right.eval;
  }

   class Plus(l: exp, r: exp) extends BasePlus(l, r) with PlusBehavior with Exp

}

//----------------------EVALNEG
trait EvalNeg extends BaseNeg with Eval {

  trait NegBehavior {
    self: BaseNeg =>
    def eval = - term.eval;
  }

  class Neg(t: exp) extends BaseNeg(t) with NegBehavior with Exp

}

//----------------------EVALNUMPLUSNEG
trait EvalNumPlusNeg extends EvalNum with EvalPlus with EvalNeg


//----------------------SHOW
trait Show extends Base {
  type exp <: Exp

  trait Exp extends super.Exp {
    def show: String
  }
}

//----------------------SHOWNUM
trait ShowNum extends BaseNum with Show {

  trait NumBehavior {
    self: BaseNum =>
    def show: String = value.toString
  }

   class Num(v: Int) extends BaseNum(v) with NumBehavior with Exp

}

//----------------------SHOWPLUS
trait ShowPlus extends BasePlus with Show {

  trait PlusBehavior {
    self: BasePlus =>
    def show = left.show + "+" + right.show;
  }

   class Plus(l: exp, r: exp) extends BasePlus(l, r) with PlusBehavior with Exp

}

//----------------------SHOWNEG
trait ShowNeg extends BaseNeg with Show {

  trait NegBehavior {
    self: BaseNeg =>
    def show = "-(" + term.show + ")";
  }

  class Neg(t: exp) extends BaseNeg(t) with NegBehavior with Exp

}

//----------------------SHOWNUMPLUSNEG
trait ShowNumPlusNeg extends ShowNum with ShowPlus with ShowNeg




//----------------------TEST
object ShowNumPlusNegInstance extends ShowNumPlusNeg {
 override type exp = Exp
}

object Test extends App {
    import ShowNumPlusNegInstance._
    println("ShowNumPlusNegInstance:")
    println(new Num(3).show)
    println(new Plus(new Num(3), new Num(4)).show)
    println(new Neg(new Num(13)).show)

}

//----------------------EVAL+SHOW (Instantiation)

trait EvalShow extends EvalNumPlusNeg with ShowNumPlusNeg {
  type exp <: Exp

  trait Exp extends super[EvalNumPlusNeg].Exp with super[ShowNumPlusNeg].Exp

  trait NumBehavior extends super[EvalNumPlusNeg].NumBehavior with super[ShowNumPlusNeg].NumBehavior {
    self: BaseNum =>
  }

   class Num(v: Int) extends BaseNum(v) with NumBehavior with Exp

  trait PlusBehavior extends super[EvalNumPlusNeg].PlusBehavior with super[ShowNumPlusNeg].PlusBehavior {
    self: BasePlus =>
  }

   class Plus(l: exp, r: exp) extends BasePlus(l, r) with PlusBehavior with Exp

  trait NegBehavior extends super[EvalNumPlusNeg].NegBehavior with super[ShowNumPlusNeg].NegBehavior {
    self: BaseNeg =>
  }
   class Neg(t: exp) extends BaseNeg(t) with NegBehavior with Exp
}

//----------------------TEST
object EvalShowInstance extends EvalShow {
  override type exp = Exp
}

object Test2 extends App {
    import EvalShowInstance._
    println("EvalShowInstance:")
    println(new Num(3).show)
    println(new Plus(new Num(3), new Num(4)).show)
    println(new Neg(new Num(13)).show)

    println(new Num(3).eval)
    println(new Plus(new Num(3), new Num(4)).eval)
    println(new Neg(new Num(13)).eval)

}



//----------------------DBLE
trait Dble extends Base {
  type exp <: Exp

  trait Exp extends super.Exp {
    def dble: exp
  }
}

//----------------------DBLENUM
trait DbleNum extends BaseNum with Dble {
  type exp <: Exp
  trait Exp extends super[BaseNum].Exp with super[Dble].Exp 
  
  trait NumBehavior {
    self: BaseNum =>
    def dble = Num(value * 2)
  }
  def Num(v: Int): exp

  class Num(v: Int) extends super.Num(v) with NumBehavior

}

//----------------------DBLEPLUS
trait DblePlus extends BasePlus with Dble {
  type exp <: Exp
  trait Exp extends super[BasePlus].Exp with super[Dble].Exp
  
  trait PlusBehavior {
    self: BasePlus =>
    def dble = Plus(left.dble, right.dble)
  }

  def Plus(l: exp, r: exp): exp
  
  class Plus(l: exp, r: exp) extends super.Plus(l, r) with PlusBehavior with Exp

}

//----------------------DBLENEG
trait DbleNeg extends BaseNeg with Dble {
  type exp <: Exp
  trait Exp extends super[BaseNeg].Exp with super[Dble].Exp

  trait NegBehavior {
    self: BaseNeg =>
    def dble = Neg(term.dble)
  }

  def Neg(t: exp): exp

  class Neg(t: exp) extends super.Neg(t) with NegBehavior with Exp

}

//----------------------DBLENUMPLUSNEG
trait DbleNumPlusNeg extends DbleNum with DblePlus with DbleNeg {
  type exp <: Exp;
  trait Exp extends super[DbleNum].Exp with super[DblePlus].Exp with super[DbleNeg].Exp
   class Num(v: Int) extends super[DbleNum].Num(v) with Exp

   class Plus(l: exp, r: exp) extends super[DblePlus].Plus(l, r) with Exp

   class Neg(t: exp) extends super[DbleNeg].Neg(t) with Exp
}
object DbleNumPlusNegInstance extends DbleNumPlusNeg {
  override type exp = Exp
  def Num(v: Int) = new Num(v)

  def Plus(l: exp, r: exp) = new Plus(l,r)

  def Neg(t: exp) = new Neg(t)
}

trait EvalShowDble extends EvalShow with DbleNumPlusNeg {
  type exp <: Exp;
  trait Exp extends super[EvalShow].Exp with super[DbleNumPlusNeg].Exp

  trait NumBehavior extends super[EvalShow].NumBehavior with super[DbleNumPlusNeg].NumBehavior {
    self: BaseNum =>
  }

   class Num(v: Int) extends BaseNum(v) with NumBehavior with Exp

  trait PlusBehavior extends super[EvalShow].PlusBehavior with super[DbleNumPlusNeg].PlusBehavior {
    self: BasePlus =>
  }

   class Plus(l: exp, r: exp) extends BasePlus(l, r) with PlusBehavior with Exp

  trait NegBehavior extends super[EvalShow].NegBehavior with super[DbleNumPlusNeg].NegBehavior {
    self: BaseNeg =>
  }
   class Neg(t: exp) extends BaseNeg(t) with NegBehavior with Exp
}

object EvalShowDbleInstance extends EvalShowDble {
  override type exp = Exp
  def Num(v: Int) = new Num(v)

  def Plus(l: exp, r: exp) = new Plus(l,r)

  def Neg(t: exp) = new Neg(t)
}

//----------------------TEST
object Test3 extends App {
    import EvalShowDbleInstance._
    println("EvalShowDbleInstance:")
    println(new Num(3).dble.show)
    println(new Neg(new Num(13)).dble.show)
    println(new Plus(new Num(3), new Num(4)).dble.show)
}

//----------------------EQUALS
trait Equals extends Base {
  type exp <: Exp;

  trait Exp extends super.Exp {
    def eql(other: exp): Boolean;
  }
}

//----------------------EQUALSNUM
trait EqualsNum extends BaseNum with Equals {
  type exp <: Exp;

  trait Exp extends super.Exp {
    def isNum(v: Int): Boolean = false;
  }

  trait NumBehavior extends Exp {
    self: BaseNum =>
    def eql(other: exp): Boolean = other.isNum(value);

    override def isNum(v: Int) = v == value;
  }

   class Num(v: Int) extends BaseNum(v) with NumBehavior with Exp

}

//----------------------EQUALSPLUS
trait EqualsPlus extends BasePlus with Equals {
  type exp <: Exp;

  trait Exp extends super[BasePlus].Exp with super[Equals].Exp {
    def isPlus(l: exp, r: exp): Boolean = false;
  }

  trait PlusBehavior extends Exp {
    self: BasePlus =>
    def eql(other: exp): Boolean = other.isPlus(left, right);

    override def isPlus(l: exp, r: exp) = (left eql l) && (right eql r)
  }

   class Plus(l: exp, r: exp) extends BasePlus(l, r) with PlusBehavior with Exp

}

//----------------------EQUALSNEG
trait EqualsNeg extends BaseNeg with Equals {
  type exp <: Exp;

  trait Exp extends super[BaseNeg].Exp with super[Equals].Exp {
    
    def isNeg(t: exp): Boolean = false;
  }

  trait NegBehavior extends Exp {
    self: BaseNeg =>
    def eql(other: exp): Boolean = other.isNeg(term);

    override def isNeg(t: exp) = term eql t
  }

   class Neg(t: exp) extends BaseNeg(t) with NegBehavior with Exp

}


//----------------------EQUALSNUMPLUSNEG
trait EqualsNumPlusNeg extends EqualsNum with EqualsPlus with EqualsNeg {
  type exp <: Exp

  trait Exp extends super[EqualsNum].Exp with super[EqualsPlus].Exp with super[EqualsNeg].Exp
}

//----------------------EQUALS+SHOW+EVAL (Instantiation)

trait EvalShowEquals extends EvalShow with EqualsNumPlusNeg {//parametric on the super traits but cut-pasted otherwise
  type exp <: Exp

  trait Exp extends super[EvalShow].Exp with super[EqualsNumPlusNeg].Exp

  trait NumBehavior extends super[EvalShow].NumBehavior with super[EqualsNumPlusNeg].NumBehavior {
    self: BaseNum =>
  }

   class Num(v: Int) extends BaseNum(v) with NumBehavior with Exp

  trait PlusBehavior extends super[EvalShow].PlusBehavior with super[EqualsNumPlusNeg].PlusBehavior {
    self: BasePlus =>
  }

   class Plus(l: exp, r: exp) extends BasePlus(l, r) with PlusBehavior with Exp

  trait NegBehavior extends super[EvalShow].NegBehavior with super[EqualsNumPlusNeg].NegBehavior {
    self: BaseNeg =>
  }

   class Neg(t: exp) extends BaseNeg(t) with NegBehavior with Exp

}

//----------------------TEST
object EvalShowEqualsInstance extends EvalShowEquals {
 override type exp = Exp
}

object Test4 extends App {
    import EvalShowEqualsInstance._
    println("EvalShowInstance:")
    println(new Num(3).show)
    println(new Plus(new Num(3), new Num(4)).show)
    println(new Neg(new Num(13)).show)

    println(new Num(3).eval)
    println(new Plus(new Num(3), new Num(4)).eval)
    println(new Neg(new Num(13)).eval)

    println(new Num(3).eql(new Num(3)))
    println(new Plus(new Num(3), new Num(4)).eql(new Num(3)))
    println(new Neg(new Num(13)).eql(new Neg(new Num(3))))

}

object All extends 
    Eval with Show with Dble with Equals 
    with EvalNum with EvalPlus with EvalNeg 
    with ShowNum with ShowPlus with ShowNeg 
    with DbleNum with DblePlus with DbleNeg 
    with EqualsNum with EqualsPlus with EqualsNeg 
    {
  override type exp = Exp
  //type exp <: Exp
  
  trait Exp extends 
      super[Eval].Exp with super[Show].Exp with super[Dble].Exp with super[Equals].Exp
      with super[DbleNum].Exp with super[DblePlus].Exp with super[DbleNeg].Exp
      with super[EqualsNum].Exp with super[EqualsPlus].Exp with super[EqualsNeg].Exp

  trait NumBehavior extends
      super[EvalNum].NumBehavior with super[ShowNum].NumBehavior
      with super[DbleNum].NumBehavior with super[EqualsNum].NumBehavior {
    self: BaseNum =>
  }

  class Num(v: Int) extends BaseNum(v) with NumBehavior with Exp

  trait PlusBehavior extends
      super[EvalPlus].PlusBehavior with super[ShowPlus].PlusBehavior
      with super[DblePlus].PlusBehavior with super[EqualsPlus].PlusBehavior {
    self: BasePlus =>
  }

   class Plus(l: exp, r: exp) extends BasePlus(l, r) with PlusBehavior with Exp

  trait NegBehavior extends
      super[EvalNeg].NegBehavior with super[ShowNeg].NegBehavior
      with super[DbleNeg].NegBehavior with super[EqualsNeg].NegBehavior {
    self: BaseNeg =>
  }

   class Neg(t: exp) extends BaseNeg(t) with NegBehavior with Exp
  
  def Num(v: Int) = new Num(v)
  
  def Plus(l: exp, r: exp) =new Plus(l,r)

  def Neg(t: exp) = new Neg(t)
}


/*
//----------------------EQUALS+SHOW+EVAL+DBLE (Instantiation)

trait EvalShowEqualsDble extends EvalShowEquals with DbleNumPlusNeg {//parametric on the super traits but cut-pasted otherwise
  type exp <: Exp

  trait Exp extends super[EvalShowEquals].Exp with super[DbleNumPlusNeg].Exp

  trait NumBehavior extends super[EvalShowEquals].NumBehavior with super[DbleNumPlusNeg].NumBehavior {
    self: BaseNum =>
  }

   class Num(v: Int) extends BaseNum(v) with NumBehavior with Exp

  trait PlusBehavior extends super[EvalShowEquals].PlusBehavior with super[DbleNumPlusNeg].PlusBehavior {
    self: BasePlus =>
  }

   class Plus(l: exp, r: exp) extends BasePlus(l, r) with PlusBehavior with Exp

  trait NegBehavior extends super[EvalShowEquals].NegBehavior with super[DbleNumPlusNeg].NegBehavior {
    self: BaseNeg =>
  }

   class Neg(t: exp) extends BaseNeg(t) with NegBehavior with Exp

}

//----------------------TEST
object EvalShowEqualsDbleInstance extends EvalShowEqualsDble {
 override type exp = Exp
}
*/
object Test5 extends App {
    import All._
    println("All:")
    println(new Num(3).show)
    println(new Plus(new Num(3), new Num(4)).show)
    println(new Neg(new Num(13)).show)

    println(new Num(3).eval)
    println(new Plus(new Num(3), new Num(4)).eval)
    println(new Neg(new Num(13)).eval)

    println(new Num(3).eql(new Num(3)))
    println(new Plus(new Num(3), new Num(4)).eql(new Num(3)))
    println(new Neg(new Num(13)).eql(new Neg(new Num(3))))

    println(new Num(3).dble.show)
    println(new Plus(new Num(3), new Num(4)).dble.show)
    println(new Neg(new Num(13)).dble.show)


}
