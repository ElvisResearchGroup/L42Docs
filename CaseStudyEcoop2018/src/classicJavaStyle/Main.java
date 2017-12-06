package classicJavaStyle;

//0
class Point {
  final int x;
  final int y;

  public Point(int x, int y) {
    this.x = x;
    this.y = y;
  }
}

// 1
class PointSum extends Point {
  public PointSum(int x, int y) {
    super(x, y);
  }

  public PointSum sum(Point that) {
    return new PointSum(this.x + that.x, this.y + that.y);
  }
}

// 2
class PointSub extends Point {
  public PointSub(int x, int y) {
    super(x, y);
  }

  public PointSub sub(Point that) {
    return new PointSub(this.x - that.x, this.y - that.y);
  }
}

// 3
class PointMul extends Point {
  public PointMul(int x, int y) {
    super(x, y);
  }

  public PointMul mul(Point that) {
    return new PointMul(this.x * that.x, this.y * that.y);
  }
}

// 4
class PointDiv extends Point {
  public PointDiv(int x, int y) {
    super(x, y);
  }

  public PointDiv div(Point that) {
    return new PointDiv(this.x / that.x, this.y / that.y);
  }
}

// 5
class PointSumSub extends PointSum {
  public PointSumSub(int x, int y) {
    super(x, y);
  }

  public PointSumSub sum(Point that) {
    return new PointSumSub(this.x + that.x, this.y + that.y);
  }

  public PointSumSub sub(Point that) {
    return new PointSumSub(this.x - that.x, this.y - that.y);
  }
}

// 6
class PointSumMul extends PointSum {
  public PointSumMul(int x, int y) {
    super(x, y);
  }

  public PointSumMul sum(Point that) {
    return new PointSumMul(this.x + that.x, this.y + that.y);
  }

  public PointSumMul mul(Point that) {
    return new PointSumMul(this.x * that.x, this.y * that.y);
  }
}

// 7
class PointSumDiv extends PointSum {
  public PointSumDiv(int x, int y) {
    super(x, y);
  }

  public PointSumDiv sum(Point that) {
    return new PointSumDiv(this.x + that.x, this.y + that.y);
  }

  public PointSumDiv div(Point that) {
    return new PointSumDiv(this.x / that.x, this.y / that.y);
  }
}

// 8
class PointSubMul extends PointSub {
  public PointSubMul(int x, int y) {
    super(x, y);
  }

  public PointSubMul sub(Point that) {
    return new PointSubMul(this.x - that.x, this.y - that.y);
  }

  public PointSubMul mul(Point that) {
    return new PointSubMul(this.x * that.x, this.y * that.y);
  }
}

// 9
class PointSubDiv extends PointSub {
  public PointSubDiv(int x, int y) {
    super(x, y);
  }

  public PointSubDiv sub(Point that) {
    return new PointSubDiv(this.x - that.x, this.y - that.y);
  }

  public PointSubDiv div(Point that) {
    return new PointSubDiv(this.x / that.x, this.y / that.y);
  }
}

// 10
class PointMulDiv extends PointMul {
  public PointMulDiv(int x, int y) {
    super(x, y);
  }

  public PointMulDiv mul(Point that) {
    return new PointMulDiv(this.x * that.x, this.y * that.y);
  }

  public PointMulDiv div(Point that) {
    return new PointMulDiv(this.x / that.x, this.y / that.y);
  }
}

// 11
class PointSumSubDiv extends PointSumSub {
  public PointSumSubDiv(int x, int y) {
    super(x, y);
  }

  public PointSumSubDiv sum(Point that) {
    return new PointSumSubDiv(this.x + that.x, this.y + that.y);
  }

  public PointSumSubDiv sub(Point that) {
    return new PointSumSubDiv(this.x - that.x, this.y - that.y);
  }

  public PointSumSubDiv div(Point that) {
    return new PointSumSubDiv(this.x / that.x, this.y / that.y);
  }
}

// 12
class PointSumSubMul extends PointSumSub {
  public PointSumSubMul(int x, int y) {
    super(x, y);
  }

  public PointSumSubMul sum(Point that) {
    return new PointSumSubMul(this.x + that.x, this.y + that.y);
  }

  public PointSumSubMul sub(Point that) {
    return new PointSumSubMul(this.x - that.x, this.y - that.y);
  }

  public PointSumSubMul mul(Point that) {
    return new PointSumSubMul(this.x * that.x, this.y * that.y);
  }
}

// 13
class PointSumMulDiv extends PointMulDiv {
  public PointSumMulDiv(int x, int y) {
    super(x, y);
  }

  public PointSumMulDiv sum(Point that) {
    return new PointSumMulDiv(this.x + that.x, this.y + that.y);
  }

  public PointSumMulDiv mul(Point that) {
    return new PointSumMulDiv(this.x * that.x, this.y * that.y);
  }

  public PointSumMulDiv div(Point that) {
    return new PointSumMulDiv(this.x / that.x, this.y / that.y);
  }
}

// 14
class PointSubMulDiv extends PointMulDiv {
  public PointSubMulDiv(int x, int y) {
    super(x, y);
  }

  public PointSubMulDiv sub(Point that) {
    return new PointSubMulDiv(this.x - that.x, this.y - that.y);
  }

  public PointSubMulDiv mul(Point that) {
    return new PointSubMulDiv(this.x * that.x, this.y * that.y);
  }

  public PointSubMulDiv div(Point that) {
    return new PointSubMulDiv(this.x / that.x, this.y / that.y);
  }
}

// 15
class PointSumSubMulDiv extends PointSumSubMul {
  public PointSumSubMulDiv(int x, int y) {
    super(x, y);
  }

  public PointSumSubMulDiv sum(Point that) {
    return new PointSumSubMulDiv(this.x + that.x, this.y + that.y);
  }

  public PointSumSubMulDiv sub(Point that) {
    return new PointSumSubMulDiv(this.x - that.x, this.y - that.y);
  }

  public PointSumSubMulDiv mul(Point that) {
    return new PointSumSubMulDiv(this.x * that.x, this.y * that.y);
  }

  public PointSumSubMulDiv div(Point that) {
    return new PointSumSubMulDiv(this.x / that.x, this.y / that.y);
  }
}