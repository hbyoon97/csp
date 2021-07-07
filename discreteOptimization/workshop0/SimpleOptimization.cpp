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

class SimpleOptimization : public IntMinimizeScript {
  protected:
    IntVar x;
    IntVar y;
  public: //home space

    // constructor
    SimpleOptimization(const SizeOptions& opt) : IntMinimizeScript(opt), x(*this, 0, 10), y(*this, 0, 50) {
      //x is divisible by 4
      rel(*this, x % 4 == 0);

      //minimum of (x-7)^2
      rel(*this, (x-7)*(x-7) == y);

      //branch (?)
      branch(*this, x, INT_VAL_MIN());
    }

    //copy constructor
    SimpleOptimization(SimpleOptimization& s) : IntMinimizeScript(s) {
      x.update(*this, s.x);
      y.update(*this, s.y);
    }

    //virtual copy 
    virtual Space* copy(void){
      return new SimpleOptimization(*this);
    }

    virtual IntVar cost(void) const {
      return y;
    }

     /// Print solution
  virtual void
  print(std::ostream& os) const {
    os << "x = " << x << endl;
    os << "y = " << y << endl;
  } 
};

int main(int argc, char* argv[]) {
  SizeOptions opt("SimpleOptimization");
  opt.ipl(IPL_VAL);
  opt.solutions(0);
  opt.print_last(true);
  opt.parse(argc,argv);
  Script::run<SimpleOptimization,DFS,SizeOptions>(opt);
  return 0;
}