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

int param;

class Arrays : public IntMaximizeScript {
  protected:
    int n;
    IntVarArray x;
    IntVar sum;
  public: //home space

    // constructor
    Arrays(const SizeOptions& opt) : IntMaximizeScript(opt), n(param), x(*this, n, 0, 3), sum(*this, 0, 789789) {
      //first number is 0
      rel(*this, x[0], IRT_EQ, 0);

      //last number is 3
      rel(*this, x[n-1], IRT_EQ, 3);

      //sum of any two adjacent between 1~n-2 is at most 3
      for(int i = 0; i<n-1; i++){
        //positions divisible by 3 to be greater than or equal to 2
        if((i+1)%3 == 0){
            rel(*this, x[i] >= 2);
        } 
        rel(*this, x[i] + x[i+1] <= 3);
      }

      linear(*this, x, IRT_EQ, sum);
      //branch (?)
      branch(*this, x, INT_VAL_MIN());
    }

    //copy constructor
    Arrays(Arrays& s) : IntMaximizeScript(s) {
      x.update(*this, s.x);
      sum.update(*this, s.sum);
    }

    virtual IntVar cost(void) const{
        return sum;
    }

    //virtual copy 
    virtual Space* copy(void){
      return new Arrays(*this);
    }

    // Print solution
    virtual void
    print(std::ostream& os) const {
        os << sum << " = " << x << endl;
    } 
};

int main(int argc, char* argv[]) {
  SizeOptions opt("Sequence");
  opt.ipl(IPL_VAL);
  opt.solutions(0);
//   opt.print_last(true);
  param = atoi(argv[1]);
  opt.parse(argc, argv);
  // BAB will give optimal
  Script::run<Arrays,BAB,SizeOptions>(opt);
  return 0;
}