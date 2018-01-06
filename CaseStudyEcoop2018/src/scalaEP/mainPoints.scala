trait tPointState {//5
  type p <: tPointState
  def x: Int
  def y: Int
  def of(x:Int,y:Int):p
}

trait tPointSum extends tPointState {//3*4
  def sum(that:p)=
    this.of(this.x+that.x,this.y+that.y)  
}

trait tPointSub extends tPointState {
  def sub(that:p)=
    this.of(this.x-that.x,this.y-that.y)  
}

trait tPointMul extends tPointState {
  def mul(that:p)=
    this.of(this.x*that.x,this.y*that.y)  
}

trait tPointDiv extends tPointState {
  def div(that:p)=
    this.of(this.x/that.x,this.y/that.y)  
}
//glue code from now on
class Point0(val x:Int, val y:Int) extends tPointState {//4*16
 override type p = Point0
 override def of(x:Int,y:Int)=
   new Point0(x,y)//3 duplication of "this name"
}

class PointSum(val x:Int, val y:Int) extends tPointSum {
 override type p = PointSum
 override def of(x:Int,y:Int)=
   new PointSum(x,y)
}

class PointSub(val x:Int, val y:Int) extends tPointSub {
 override type p = PointSub
 override def of(x:Int,y:Int)=
   new PointSub(x,y)
}

class PointMul(val x:Int, val y:Int) extends tPointMul {
 override type p = PointMul
 override def of(x:Int,y:Int)=
   new PointMul(x,y)
}

class PointDiv(val x:Int, val y:Int) extends tPointDiv {
 override type p = PointDiv
 override def of(x:Int,y:Int)=
   new PointDiv(x,y)
}

class PointSumSub(val x:Int, val y:Int) extends tPointSum with tPointSub {
 override type p = PointSumSub
 override def of(x:Int,y:Int)=
   new PointSumSub(x,y)
}

class PointSumMul(val x:Int, val y:Int) extends tPointSum with tPointMul {
 override type p = PointSumMul
 override def of(x:Int,y:Int)=
   new PointSumMul(x,y)
}

class PointSumDiv(val x:Int, val y:Int) extends tPointSum with tPointDiv {
 override type p = PointSumDiv
 override def of(x:Int,y:Int)=
   new PointSumDiv(x,y)
}

class PointSubMul(val x:Int, val y:Int) extends tPointSub with tPointMul {
 override type p = PointSubMul
 override def of(x:Int,y:Int)=
   new PointSubMul(x,y)
}

class PointSubDiv(val x:Int, val y:Int) extends tPointSub with tPointDiv {
 override type p = PointSubDiv
 override def of(x:Int,y:Int)=
   new PointSubDiv(x,y)
}

class PointMulDiv(val x:Int, val y:Int) extends tPointMul with tPointDiv {
 override type p = PointMulDiv
 override def of(x:Int,y:Int)=
   new PointMulDiv(x,y)
}

class PointSumSubDiv(val x:Int, val y:Int) extends tPointSum with tPointSub with tPointDiv {
 override type p = PointSumSubDiv
 override def of(x:Int,y:Int)=
   new PointSumSubDiv(x,y)
}

class PointSumSubMul(val x:Int, val y:Int) extends tPointSum with tPointSub with tPointMul {
 override type p = PointSumSubMul
 override def of(x:Int,y:Int)=
   new PointSumSubMul(x,y)
}

class PointSumMulDiv(val x:Int, val y:Int) extends tPointSum with tPointMul with tPointDiv {
 override type p = PointSumMulDiv
 override def of(x:Int,y:Int)=
   new PointSumMulDiv(x,y)
}

class PointSubMulDiv(val x:Int, val y:Int) extends tPointSub with tPointMul with tPointDiv {
 override type p = PointSubMulDiv
 override def of(x:Int,y:Int)=
   new PointSubMulDiv(x,y)
}

class PointSumSubMulDiv(val x:Int, val y:Int) extends tPointSum with tPointSub with tPointMul with tPointDiv {
 override type p = PointSumSubMulDiv
 override def of(x:Int,y:Int)=
   new PointSumSubMulDiv(x,y)
}
//main, not counted for the line numbers
object Test5 extends App {
    val pp=new Point0(2,4)
    val pp2=new PointSum(2,4)
    val pp3=pp2.sum(pp2)
    val ppAll=new PointSumSubMulDiv(3,4)
    val ppAll2=ppAll.sum(ppAll.mul(ppAll))
    println("Hi")
    println(ppAll2.x+ppAll2.y)
}