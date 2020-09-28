double fin_diff(double f(double), double x, double h) {
  return (f(x+h) - f(x)) / h;
}
