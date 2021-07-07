#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <gecode/set.hh>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace Gecode;
using namespace std;

class SimpleDecision : public Script {
  protected:
    IntVar x;
  public: //home space

    // constructor
    SimpleDecision(const SizeOptions& opt) : Script(opt), x(*this, 0, 10) {
      //branch (?)
      branch(*this, x, INT_VAL_MIN());
    }

    //copy constructor
    SimpleDecision(SimpleDecision& s) : Script(s) {
      x.update(*this, s.x);
    }

    //virtual copy 
    virtual Space* copy(void){
      return new SimpleDecision(*this);
    }

     /// Print solution
  virtual void
  print(std::ostream& os) const {
    os << "  ";
    if (x.assigned()) {
      if (x.val())
        os << x << " ";
    }
  } 
};

int main(int argc, char* argv[]) {
  SizeOptions opt("SimpleDecision");
  opt.ipl(IPL_VAL);
  opt.solutions(1);
  opt.parse(argc,argv);
  Script::run<SimpleDecision,DFS,SizeOptions>(opt);
  return 0;
}