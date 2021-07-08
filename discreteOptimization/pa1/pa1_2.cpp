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

    S E V E N 
+   P A C E S
_______________
=   V E R S E

*/

class PA_1_2 : public IntMaximizeScript {
  protected:
    IntVarArray iva;
    IntVar verse;

  public: //home space

    // constructor
    PA_1_2(const SizeOptions& opt) : IntMaximizeScript(opt), iva(*this, 8, 0, 9), verse(*this, 0, 99999) {
        IntVar S(iva[0]), E(iva[1]), V(iva[2]), N(iva[3]), P(iva[4]), A(iva[5]), C(iva[6]), R(iva[7]);
        
        // all distinct digits
        distinct(*this, iva);

        // S, P, V cannot be leading zero
        rel(*this, S, IRT_NQ, 0);
        rel(*this, P, IRT_NQ, 0);
        rel(*this, V, IRT_NQ, 0);

        // verse
        rel(*this, verse == 10000*V + 1000*E + 100*R + 10*S + E);

        // equation
        rel(*this, 
            10000*S + 1000*E + 100*V + 10*E + N 
        +   10000*P + 1000*A + 100*C + 10*E + S
        ==  10000*V + 1000*E + 100*R + 10*S + E
        );

        branch(*this, iva, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    }

    //copy constructor
    PA_1_2(PA_1_2& s) : IntMaximizeScript(s) {
      verse.update(*this, s.verse);
      iva.update(*this, s.iva);
    }

     virtual IntVar cost(void) const {
        return verse; 
    }

    //virtual copy 
    virtual Space* copy(void){
      return new PA_1_2(*this);
    }

     /// Print solution
    virtual void
    print(std::ostream& os) const {
        os << "verse = " << verse << endl;
    } 
};

int main(int argc, char* argv[]) {
  SizeOptions opt("PA_1_2");
  opt.ipl(IPL_VAL);
  opt.solutions(0);
  opt.print_last(true);
  Script::run<PA_1_2,BAB,SizeOptions>(opt);
  return 0;
}