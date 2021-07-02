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

class KillerSudoku : public Script {
  protected:
    const int n = 9;
    IntVarArray x; //sudoku cell values??
  public: //home space

    // constructor
    KillerSudoku(const SizeOptions& opt) : Script(opt), x(*this, 81, 1, 9) {
        Matrix<IntVarArray> m(x, 9, 9);
        ifstream infile(input);
        if (infile.is_open() && infile.good()) {
            string line;
            while(getline(infile, line)){
              IntVarArgs asdf; 
              istringstream ss(line);
              vector<int> tokens;
              int word, cageSum, numCageCells;
              // Tokenizing w.r.t. space ' '
              while(ss >> word) tokens.push_back(word);
              cageSum = tokens[0];
              numCageCells = tokens[1];
              for(int i = 2; i<tokens.size(); i+=2)
                asdf << m(tokens[i]-1, tokens[i+1]-1);
              linear(*this, asdf, IRT_EQ, cageSum);
              distinct(*this, asdf, opt.ipl());
            } 
        } 

        // constraints for rows and columns
        for(int i = 0; i<9; i++){
          distinct(*this, m.row(i), opt.ipl());
          distinct(*this, m.col(i), opt.ipl());
        }

    // Constraints for squares
    for (int i=0; i<9; i+=3) {
      for (int j=0; j<9; j+=3) {
        distinct(*this, m.slice(i, i+3, j, j+3), opt.ipl());
      }
    }

    //branch (?)
    branch(*this, x, INT_VAR_AFC_SIZE_MAX(opt.decay()), INT_VAL_MIN()); 
    }

    //copy constructor
    KillerSudoku(KillerSudoku& s) : Script(s), n(s.n) {
      x.update(*this, s.x);
    }

    //virtual copy 
    virtual Space* copy(void){
      return new KillerSudoku(*this);
    }

     /// Print solution
  virtual void
  print(std::ostream& os) const {
    os << "  ";
    for (int i = 0; i<81; i++) {
      if (x[i].assigned()) {
        if (x[i].val()<10)
          os << x[i] << " ";
        else
          os << (char)(x[i].val()+'A'-10) << " ";
      }
      else
        os << ". ";
      if((i+1)%(9) == 0)
        os << std::endl << "  ";
    }
    os << std::endl;
  }   
};

int main(int argc, char* argv[]) {
  SizeOptions opt("KillerSudoku");
  opt.ipl(IPL_DOM);
  opt.solutions(1);
  opt.parse(argc,argv);
  input = argv[1];
  Script::run<KillerSudoku,DFS,SizeOptions>(opt);
  return 0;
}