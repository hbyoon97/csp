#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <iostream>

#include "nqueen_propagator.h"

using namespace Gecode;
using namespace std;

class Nqueen_propagator : public Propagator {
 protected:
        ViewArray<Int::IntView> view_m;
        int n;

 public:
        Nqueen_propagator(Home home, ViewArray<Int::IntView>& m, int n)
            :Propagator(home), view_m(m), n(n) {
                view_m.subscribe(home, *this, Int::PC_INT_DOM);
        }

        static ExecStatus post(Space& home,
            ViewArray<Int::IntView> view_m, int n) { 
            (void) new (home) Nqueen_propagator(home, view_m, n);
            return ES_OK;  
        }

        virtual ExecStatus propagate(Space& home, const ModEventDelta&) {
            for(int i = 0; i<n*n; i++){ 
                //checking row ->
                int counter = 0;
                for(int j = 0; j<n-1-(i%n); j++){
                    if(view_m[j+i+1].size() == 1 && view_m[j+i+1].val() == 1){
                        counter++;
                        GECODE_ME_CHECK(view_m[i].eq(home, 0));
                    }
                    if(view_m[i].size() == 1 && view_m[i].val() == 1){
                        counter++;
                        GECODE_ME_CHECK(view_m[j+i+1].eq(home, 0));
                    }
                }
                
                //checking row <-
                for(int j = 0; j<i%n; j++){
                    if(view_m[i-j-1].size() == 1 && view_m[i-j-1].val() == 1){
                        GECODE_ME_CHECK(view_m[i].eq(home, 0));
                    }
                    if(view_m[i].size() == 1 && view_m[i].val() == 1){
                        GECODE_ME_CHECK(view_m[i-j-1].eq(home, 0));
                    }
                }

                //checking column ↑
                int k = 0;
                for(int j = 0; j<i/n; j++, k+=n){
                    if(view_m[i%n+k].size() == 1 && view_m[i%n+k].val() == 1)
                        GECODE_ME_CHECK(view_m[i].eq(home, 0));
                    if(view_m[i].size() == 1 && view_m[i].val() == 1)
                        GECODE_ME_CHECK(view_m[i%n+k].eq(home, 0));
                }
                
                //checking column ↓
                for(int j = 0; j<n-1-i/n; j++){
                    if(view_m[n+i+n*j].size() == 1 && view_m[n+i+n*j].val() == 1)
                        GECODE_ME_CHECK(view_m[i].eq(home, 0));
                    if(view_m[i].size() == 1 && view_m[i].val() == 1)
                        GECODE_ME_CHECK(view_m[n+i+n*j].eq(home, 0));
                }

                // checking diagonal ↘
                for(int j = i; j<n*n; j+=n+1){
                    if(j != 0 && (j+1)%n == 0) break;
                    if(j+n+1 >= n*n) break;
                    if(view_m[j+n+1].size() == 1 && view_m[j+n+1].val() == 1)
                        GECODE_ME_CHECK(view_m[i].eq(home, 0));
                    if(view_m[i].size() == 1 && view_m[i].val() == 1)
                        GECODE_ME_CHECK(view_m[j+n+1].eq(home, 0));
                }

                //checking diagonal ↙
                for(int j = i; j<n*n; j+=n-1){
                    if(i >= n*n-n || i%n == 0 || j%n == 0) break;
                    if(j+n-1 >= n*n) break;
                    if(view_m[j+n-1].size() == 1 && view_m[j+n-1].val() == 1)
                        GECODE_ME_CHECK(view_m[i].eq(home, 0));
                    if(view_m[i].size() == 1 && view_m[i].val() == 1)
                        GECODE_ME_CHECK(view_m[j+n-1].eq(home, 0));
                }

                //checking sum(IntVarArray) == n
                bool allAssigned = true;
                int sum = 0;
                for(int i = 0; i<n*n; i++){
                    if(view_m[i].size() != 1){
                        allAssigned = false;
                        break;
                    }
                    sum+= view_m[i].val();
                }

                if (allAssigned && (sum != n)) return ES_FAILED; 
            }

            int count = 0;
            for(int i = 0 ; i<n*n; i++){
                if(view_m.size() == 1) {
                    count ++;
                } 
            }
            if(count == n*n)
                return home.ES_SUBSUMED(*this);
            return ES_NOFIX;
        }

        Nqueen_propagator(Space& home, Nqueen_propagator& p, int n)
            :Propagator(home, p), n(n) {
            view_m.update(home, p.view_m);
        }

        virtual Propagator* copy(Space& home) { 
            return new (home) Nqueen_propagator(home, *this, this->n);
        }

        virtual PropCost cost(const Space&, const ModEventDelta&) const { 
            return PropCost::binary(PropCost::LO);
        }

        virtual void reschedule(Space& home) {
            view_m.reschedule(home, *this, Int::PC_INT_DOM);
                
        }

        virtual size_t dispose(Space& home) {  
            view_m.cancel(home, *this, Int::PC_INT_DOM);
            (void) Propagator::dispose(home);
            return sizeof(*this);
        }

};

void nqueen_propagator(Space& home, IntVarArgs m, int n) {
    GECODE_POST;
    ViewArray<Int::IntView> view_m(home, m);
    GECODE_ES_FAIL(Nqueen_propagator::post(home, view_m, n));
}