#ifdef CANNON_HAVE_GUROBI
  #include <gurobi_c++.h>
#endif


#include <cannon/log/registry.hpp>

using namespace cannon::log;

int main() {

#ifdef CANNON_HAVE_GUROBI
  try {
    GRBEnv env = GRBEnv(true);
    env.set("LogFile", "mip1.log");
    env.start();

    // Create empty model
    GRBModel model = GRBModel(env);

    // Create variables
    GRBVar x = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x");
    GRBVar y = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y");
    GRBVar z = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "z");

    // Set objective
    model.setObjective(x + y + 2*z, GRB_MAXIMIZE);

    // Add constraints
    model.addConstr(x + 2*y + 3*z <= 4, "c0");
    model.addConstr(x + y >= 1, "c1");

    model.optimize();

    log_info(x.get(GRB_StringAttr_VarName), x.get(GRB_DoubleAttr_X));
    log_info(y.get(GRB_StringAttr_VarName), y.get(GRB_DoubleAttr_X));
    log_info(z.get(GRB_StringAttr_VarName), z.get(GRB_DoubleAttr_X));

    log_info("Obj:", model.get(GRB_DoubleAttr_ObjVal));
  } catch (GRBException e) {
    log_error("Error code =", e.getErrorCode());
    log_error(e.getMessage());
  } catch (...) {
    log_error("Exception during optimization.");
  }
#endif

}

