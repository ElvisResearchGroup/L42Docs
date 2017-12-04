package classLessJavaStyle;

public class Main {

}

// 0
@Obj interface Point {
  int x();
  int y();

  static Point of(int x, int y) { return new Point() { public int x() { return x; } public int y() { return y; } }; }
}

// 1
@Obj interface PointSum extends Point {
  static PointSum of(int x, int y) { return new PointSum() { public int x() { return x; } public int y() { return y; } }; }

  default PointSum sum(Point that) {
    return of(this.x() + that.x(), this.y() + that.y());
  }
}

// 2
@Obj interface PointSub extends Point {
  static PointSub of(int x, int y) { return new PointSub() { public int x() { return x; } public int y() { return y; } }; }

  default PointSub sub(Point that) {
    return of(this.x() - that.x(), this.y() - that.y());
  }
}

// 3
@Obj interface PointMul extends Point {
  static PointMul of(int x, int y) { return new PointMul() { public int x() { return x; } public int y() { return y; } }; }

  default PointMul mul(Point that) {
    return of(this.x() * that.x(), this.y() * that.y());
  }
}

// 4
@Obj interface PointDiv extends Point {
  static PointDiv of(int x, int y) { return new PointDiv() { public int x() { return x; } public int y() { return y; } }; }

  default PointDiv div(Point that) {
    return of(this.x() / that.x(), this.y() / that.y());
  }
}

// 5
@Obj interface PointSumMul extends PointSum, PointMul {
  static PointSumMul of(int x, int y) { return new PointSumMul() { public int x() { return x; } public int y() { return y; } }; }

  default PointSumMul sum(Point that) {// we have to rewrite the method calling the new "of"
    return of(this.x() + that.x(), this.y() + that.y());// in order to produce an instance of PointSumMul
  }

  default PointSumMul mul(Point that) {
    return of(this.x() * that.x(), this.y() * that.y());
  }
}

// 6
@Obj interface PointSumSub extends PointSum, PointSub {
  static PointSumSub of(int x, int y) { return new PointSumSub() { public int x() { return x; } public int y() { return y; } }; }

  default PointSumSub sum(Point that) {// we have to rewrite the method calling the new "of"
    return of(this.x() + that.x(), this.y() + that.y());// in order to produce an instance of PointSumMul
  }

  default PointSumSub sub(Point that) {
    return of(this.x() - that.x(), this.y() - that.y());
  }
}

// 7
@Obj interface PointSumDiv extends PointSum, PointDiv {
  static PointSumDiv of(int x, int y) { return new PointSumDiv() { public int x() { return x; } public int y() { return y; } }; }

  default PointSumDiv sum(Point that) {// we have to rewrite the method calling the new "of"
    return of(this.x() + that.x(), this.y() + that.y());// in order to produce an instance of PointSumMul
  }

  default PointSumDiv div(Point that) {
    return of(this.x() / that.x(), this.y() / that.y());
  }
}

// 8
@Obj interface PointSubMul extends PointMul, PointSub {
  static PointSubMul of(int x, int y) { return new PointSubMul() { public int x() { return x; } public int y() { return y; } }; }

  default PointSubMul mul(Point that) {// we have to rewrite the method calling the new "of"
    return of(this.x() * that.x(), this.y() * that.y());// in order to produce an instance of PointSumMul
  }

  default PointSubMul sub(Point that) {
    return of(this.x() - that.x(), this.y() - that.y());
  }
}

// 9
@Obj interface PointSubDiv extends PointSub, PointDiv {
  static PointSubDiv of(int x, int y) { return new PointSubDiv() { public int x() { return x; } public int y() { return y; } }; }

  default PointSubDiv sub(Point that) {// we have to rewrite the method calling the new "of"
    return of(this.x() - that.x(), this.y() - that.y());// in order to produce an instance of PointSumMul
  }

  default PointSubDiv div(Point that) {
    return of(this.x() / that.x(), this.y() / that.y());
  }
}

// 10
@Obj interface PointMulDiv extends PointMul, PointDiv {
  static PointMulDiv of(int x, int y) { return new PointMulDiv() { public int x() { return x; } public int y() { return y; } }; }

  default PointMulDiv mul(Point that) {// we have to rewrite the method calling the new "of"
    return of(this.x() * that.x(), this.y() * that.y());// in order to produce an instance of PointSumMul
  }

  default PointMulDiv div(Point that) {
    return of(this.x() / that.x(), this.y() / that.y());
  }
}

// 11
@Obj interface PointSumSubDiv extends PointSumSub, PointSumDiv, PointSubDiv {
  static PointSumSubDiv of(int x, int y) { return new PointSumSubDiv() { public int x() { return x; } public int y() { return y; } }; }

  default PointSumSubDiv sum(Point that) {
    return of(this.x() + that.x(), this.y() + that.y());
  }

  default PointSumSubDiv sub(Point that) {
    return of(this.x() - that.x(), this.y() - that.y());
  }

  default PointSumSubDiv div(Point that) {
    return of(this.x() / that.x(), this.y() / that.y());
  }
}

// 12
@Obj interface PointSumSubMul extends PointSumSub, PointSumMul, PointSubMul {
  static PointSumSubMul of(int x, int y) { return new PointSumSubMul() { public int x() { return x; } public int y() { return y; } }; }

  default PointSumSubMul sum(Point that) {
    return of(this.x() + that.x(), this.y() + that.y());
  }

  default PointSumSubMul sub(Point that) {
    return of(this.x() - that.x(), this.y() - that.y());
  }

  default PointSumSubMul mul(Point that) {
    return of(this.x() * that.x(), this.y() * that.y());
  }
}

// 13
@Obj interface PointSumMulDiv extends PointSumMul, PointMulDiv, PointSumDiv {
  static PointSumMulDiv of(int x, int y) { return new PointSumMulDiv() { public int x() { return x; } public int y() { return y; } }; }

  default PointSumMulDiv sum(Point that) {
    return of(this.x() + that.x(), this.y() + that.y());
  }

  default PointSumMulDiv mul(Point that) {// we have to rewrite the method calling the new "of"
    return of(this.x() * that.x(), this.y() * that.y());// in order to produce an instance of PointSumMul
  }

  default PointSumMulDiv div(Point that) {
    return of(this.x() / that.x(), this.y() / that.y());
  }
}

// 14
@Obj interface PointSubMulDiv extends PointSubMul, PointMulDiv, PointSubDiv {
  static PointSubMulDiv of(int x, int y) { return new PointSubMulDiv() { public int x() { return x; } public int y() { return y; } }; }

  default PointSubMulDiv sub(Point that) {
    return of(this.x() - that.x(), this.y() - that.y());
  }

  default PointSubMulDiv mul(Point that) {// we have to rewrite the method calling the new "of"
    return of(this.x() * that.x(), this.y() * that.y());// in order to produce an instance of PointSumMul
  }

  default PointSubMulDiv div(Point that) {
    return of(this.x() / that.x(), this.y() / that.y());
  }
}

// 15
@Obj interface PointSumSubMulDiv extends PointSumSubMul, PointSumMulDiv, PointSumSubDiv, PointSubMulDiv {
  static PointSumSubMulDiv of(int x, int y) { return new PointSumSubMulDiv() { public int x() { return x; } public int y() { return y; } }; }

  default PointSumSubMulDiv sum(Point that) {
    return of(this.x() + that.x(), this.y() + that.y());
  }

  default PointSumSubMulDiv sub(Point that) {
    return of(this.x() - that.x(), this.y() - that.y());
  }

  default PointSumSubMulDiv mul(Point that) {// we have to rewrite the method calling the new "of"
    return of(this.x() * that.x(), this.y() * that.y());// in order to produce an instance of PointSumMul
  }

  default PointSumSubMulDiv div(Point that) {
    return of(this.x() / that.x(), this.y() / that.y());
  }
}