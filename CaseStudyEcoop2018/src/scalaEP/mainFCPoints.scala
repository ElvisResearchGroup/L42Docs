trait tPointState {//8
  type p <: tPointState
  def x: Int
  def y: Int
  def withX(that:Int):p
  def withY(that:Int):p
  def of(x:Int,y:Int):p
  def merge(that:p):p
}

trait tPointSum extends tPointState {//3
  def sum(that:p)=
    this.merge(that).withX(this.x+that.x).withY(this.y+that.y)  
}

trait tColored {//6*2
  type p <: tColored
  def color:Int
  def withColor(that:Int):p
  def merge(that:p)=
    this.withColor(this.color+that.color)
  }

trait tFlavored{
  type p <: tFlavored
  def flavor:Int
  def withFlavor(that:Int):p
  def merge(that:p)=
    this.withFlavor(that.flavor)
  }
//glue code from now on //12 lines
class CPoint(val x:Int, val y:Int, val color:Int) extends tPointSum with tColored {
  override type p = CPoint
  def of(x:Int,y:Int)=
    this.of(x,y,0)
  def of(x:Int,y:Int,color:Int)=
    new CPoint(x,y,color)
  def withX(that:Int)=
    of(that,y,color)
  def withY(that:Int)=
    of(x,that,color)
  def withColor(that:Int)=
    of(x,y,that)
  }
//18 lines
class FCPoint(val x:Int, val y:Int, val color:Int, val flavor:Int) extends tPointSum with tColored with tFlavored{
  override type p = FCPoint
  def of(x:Int,y:Int)=
    this.of(x,y,0,0)
  def of(x:Int,y:Int,color:Int,flavor:Int)=
    new FCPoint(x,y,color,flavor)
  def withX(that:Int)=
    of(that,y,color,flavor)
  def withY(that:Int)=
    of(x,that,color,flavor)
  def withColor(that:Int)=
    of(x,y,that,flavor)
  def withFlavor(that:Int)=
    of(x,y,color,that)
  def superMergeFlavoured(that:FCPoint)=
    super[tFlavored].merge(that)
  override def merge(that:FCPoint)=
      super[tColored].merge(that).superMergeFlavoured(that)
  }
  
object Test7 extends App {
    val pp=new FCPoint(1,1,7,1)
    val pp2=pp.sum(pp)
    println("Hi")
    println(pp2.x+pp2.y+pp2.color+pp2.flavor)
}