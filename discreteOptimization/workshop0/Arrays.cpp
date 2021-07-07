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

class Arrays : public Script {
  protected:
    int n;
    IntVarArray x;
    IntVar product;
    IntVarArray temp;
  public: //home space

    // constructor
    Arrays(const SizeOptions& opt) : Script(opt), n(param), x(*this, n, 0, 9), product(*this, 0, 9*9*9), temp(*this, n-1, 0, 9*9*9) {

      // intvararray product
      mult(*this, x[0], x[1], temp[0]);
      for(int i = 1; i<n; i++){
        if(i == n-1){
          mult(*this, temp[i-1], x[i], product);
        } else {
          mult(*this, temp[i-1], x[i+1], temp[i]);
        }
      }
      
      //decreasing order
      rel(*this, x, IRT_GQ);

      //sum == product
      linear(*this, x, IRT_EQ, product);

      //branch (?)
      branch(*this, x, INT_VAL_MIN());
    }

    //copy constructor
    Arrays(Arrays& s) : Script(s) {
      x.update(*this, s.x);
      product.update(*this, s.product);
      temp.update(*this, s.temp);
    }

    //virtual copy 
    virtual Space* copy(void){
      return new Arrays(*this);
    }

     /// Print solution
  virtual void
  print(std::ostream& os) const {
    os << "x = " << x << endl;
  } 
};

int main(int argc, char* argv[]) {
  SizeOptions opt("Arrays");
  opt.ipl(IPL_VAL);
  opt.solutions(0);
  param = atoi(argv[1]);
  Script::run<Arrays,DFS,SizeOptions>(opt);
  return 0;
}