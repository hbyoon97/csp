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

char* input;

class Nqueen : public Script {
  protected:
    const int n = 4;
    IntVarArray x; //sudoku cell values??
  public: //home space

    // constructor
    Nqueen(const SizeOptions& opt) : Script(opt), x(*this, n*n, 0, 1) {
        Matrix<IntVarArray> m(x, n, n);
        // constraints for rows and columns
        for(int i = 0; i<n; i++){
          IntVarArgs iva1, iva2, iva3, iva4;
          linear(*this, m.row(i), IRT_EQ, 1);
          linear(*this, m.col(i), IRT_EQ, 1);

          // //upper left
          int k = i;
          for(int j = 0; j<i+1 && k>=0; j++, k--){
            iva1 << m(k , j);
          } 

          //upper right
          k = i;
          for(int j = 0; j<i+1 && k>=0; j++, k--){
            iva2 << m(j, 3-k);
          } 
 
          //bottom right
          k = i;
          for(int a = 3; a>=3-i && k<3; a--, k--){
            iva3 << m(a, 3-k);
          }

          //bottom left
          k = 0;
          for(int a = 3-i; a<=3 && k<=i; a++, k++){
            iva4 << m(a, k);
          }
          linear(*this, iva1, IRT_LQ, 1);
          linear(*this, iva2, IRT_LQ, 1);
          linear(*this, iva3, IRT_LQ, 1);
          linear(*this, iva4, IRT_LQ, 1);
        }
    //branch (?)
    branch(*this, x, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    }

    //copy constructor
    Nqueen(Nqueen& s) : Script(s), n(s.n) {
      x.update(*this, s.x);
    }

    //virtual copy 
    virtual Space* copy(void){
      return new Nqueen(*this);
    }

     /// Print solution
  virtual void
  print(std::ostream& os) const {
    os << "  ";
    for (int i = 0; i<16; i++) {
      if (x[i].assigned()) {
        if (x[i].val())
          os << x[i] << " ";
        else
          os << (char)(x[i].val()) << " ";
      }
      else
        os << ". ";
      if((i+1)%(4) == 0)
        os << std::endl << "  ";
    }
    os << std::endl;
  } 
};

int main(int argc, char* argv[]) {
  SizeOptions opt("Nqueen");
  opt.ipl(IPL_VAL);
  opt.solutions(1);
  opt.parse(argc,argv);
  Script::run<Nqueen,DFS,SizeOptions>(opt);
  return 0;
}