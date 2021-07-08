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

/*

    C A O 
    Z H I
    C A O
+     P I
__________
= T E S T

*/

class PA_1 : public Script {
  protected:
    IntVarArray iva;

  public: //home space

    // constructor
    PA_1(const SizeOptions& opt) : Script(opt), iva(*this, 10, 0, 9) {
        IntVar C(iva[0]), A(iva[1]), O(iva[2]), Z(iva[3]), H(iva[4]), I(iva[5]), T(iva[7]), E(iva[8]), S(iva[9]);
        
        // all distinct digits
        distinct(*this, iva);

        // C, Z, T cannot be leading zero
        rel(*this, C, IRT_NQ, 0);
        rel(*this, Z, IRT_NQ, 0);
        rel(*this, T, IRT_NQ, 0);

        // equation
        rel(*this, 
            100*C + 10*A + O 
        +   100*Z + 10*H + I
        +   100*C + 10*A + O  
        ==   1000*T + 100*E + 10*S + T
        );

        branch(*this, iva, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    }

    //copy constructor
    PA_1(PA_1& s) : Script(s) {
      iva.update(*this, s.iva);
    }

    //virtual copy 
    virtual Space* copy(void){
      return new PA_1(*this);
    }

     /// Print solution
  virtual void
  print(std::ostream& os) const {
    os << "solution = " << iva << endl;
  } 
};

int main(int argc, char* argv[]) {
  SizeOptions opt("PA_1");
  opt.ipl(IPL_VAL);
  opt.solutions(0);
  Script::run<PA_1,DFS,SizeOptions>(opt);
  return 0;
}