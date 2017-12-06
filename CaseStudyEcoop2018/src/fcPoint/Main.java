package fcPoint;

enum Flavor {
  NONE, SOUR, SWEET, SALTY, SPLICY;
}

class Color {
  final int r;
  final int g;
  final int b;

  public Color(int r, int g, int b) {
    this.r = r;
    this.g = g;
    this.b = b;
  }

  public Color mix(Color that) {
    return new Color((this.r + that.r) / 2, (this.g + that.g) / 2, (this.b + that.b) / 2);
  }
}

class Point {
  final int x;
  final int y;

  public Point(int x, int y) {
    this.x = x;
    this.y = y;
  }

  public Point withX(int that) {
    return new Point(that, this.y);
  }

  public Point withY(int that) {
    return new Point(this.x, that);
  }
}

class PointSum extends Point {
  public PointSum(int x, int y) {
    super(x, y);
  }

  public PointSum withX(int that) {
    return new PointSum(that, this.y);
  }

  public PointSum withY(int that) {
    return new PointSum(this.x, that);
  }

  public PointSum sum(Point that) {
    return this.withX(this.x + that.x).withY(this.y + that.y);
  }
}

class CPoint extends PointSum {
  final Color color;

  public CPoint(int x, int y, Color color) {
    super(x, y);
    this.color = color;
  }

  public CPoint withX(int that) {
    return new CPoint(that, this.y, this.color);
  }

  public CPoint withY(int that) {
    return new CPoint(this.x, that, this.color);
  }

  public CPoint withColor(Color that) {
    return new CPoint(this.x, this.y, that);
  }

  public CPoint merge(CPoint that) {
    return this.withColor(this.color.mix(that.color));
  }
}

class FCPoint extends CPoint {
  final Flavor flavor;

  public FCPoint(int x, int y, Color color, Flavor flavor) {
    super(x, y, color);
    this.flavor = flavor;
  }

  public FCPoint withX(int that) {
    return new FCPoint(that, this.y, this.color, this.flavor);
  }

  public FCPoint withY(int that) {
    return new FCPoint(this.x, that, this.color, this.flavor);
  }

  public FCPoint withColor(Color that) {
    return new FCPoint(this.x, this.y, that, this.flavor);
  }

  public FCPoint withFlavor(Flavor that) {
    return new FCPoint(this.x, this.y, this.color, that);
  }

  public FCPoint merge(FCPoint that) {
    return this.withColor(that.color).withFlavor(that.flavor);
  }
}