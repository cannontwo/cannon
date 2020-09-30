#include <sstream>
#include <cassert>
#include <vector>
#include <string>

#include <cannon/logic/form.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::logic;
using namespace cannon::log;

int main() {
  // Basic printing
  std::stringstream ss1;
  ss1 << Atomic(0);
  log_info(ss1.str());
  assert(ss1.str().compare("p0") == 0);
  ss1.str(std::string());

  ss1 << make_negation(Atomic(0));
  log_info(ss1.str());
  assert(ss1.str().compare("(!p0)") == 0);
  ss1.str(std::string());

  ss1 << make_and(Atomic(0), Atomic(1));
  log_info(ss1.str());
  assert(ss1.str().compare("(p0^p1)") == 0);
  ss1.str(std::string());

  ss1 << make_or(Atomic(0), Atomic(1));
  log_info(ss1.str());
  assert(ss1.str().compare("(p0vp1)") == 0);
  ss1.str(std::string());
  

  ss1 << make_implies(Atomic(0), Atomic(1));
  log_info(ss1.str());
  assert(ss1.str().compare("(p0>p1)") == 0);
  ss1.str(std::string());

  ss1 << make_iff(Atomic(0), Atomic(1));
  log_info(ss1.str());
  assert(ss1.str().compare("(p0-p1)") == 0);
  ss1.str(std::string());
  
  // Basic eval
  auto f1 = Atomic(0);
  std::vector<bool> t = {true};
  assert(f1.eval(t));
  t[0] = false;
  assert(!f1.eval(t));

  auto f2 = make_negation(Atomic(0));
  t[0] = false;
  assert(f2.eval(t));
  t[0] = true;
  assert(!f2.eval(t));

  auto f3 = make_and(Atomic(0), Atomic(1));
  t[0] = true;
  t.push_back(true);
  assert(f3.eval(t));
  t[0] = false;
  assert(!f3.eval(t));
  t[1] = false;
  assert(!f3.eval(t));
  t[0] = true;
  assert(!f3.eval(t));

  auto f4 = make_or(Atomic(0), Atomic(1));
  t[0] = true;
  t[1] = true;
  assert(f4.eval(t));
  t[0] = false;
  assert(f4.eval(t));
  t[1] = false;
  assert(!f4.eval(t));
  t[0] = true;
  assert(f4.eval(t));

  auto f5 = make_implies(Atomic(0), Atomic(1));
  t[0] = true;
  t[1] = true;
  assert(f5.eval(t));
  t[0] = false;
  assert(f5.eval(t));
  t[1] = false;
  assert(f5.eval(t));
  t[0] = true;
  assert(!f5.eval(t));

  auto f6 = make_iff(Atomic(0), Atomic(1));
  t[0] = true;
  t[1] = true;
  assert(f6.eval(t));
  t[0] = false;
  assert(!f6.eval(t));
  t[1] = false;
  assert(f6.eval(t));
  t[0] = true;
  assert(!f6.eval(t));

  // Nested  
  std::stringstream ss2;
  auto alpha = make_and(make_implies(Atomic(0), make_and(Atomic(1), Atomic(2))), 
      make_implies(make_negation(Atomic(0)), make_and(Atomic(2), Atomic(3))));
  log_info(alpha);
  ss2 << alpha;
  assert(ss2.str().compare("((p0>(p1^p2))^((!p0)>(p2^p3)))") == 0);

  log_info(t.size());

  // Throwing if not enough values in truth assignment
  try {
    alpha.eval(t);
    assert(false);
  } catch (...) {}

}
