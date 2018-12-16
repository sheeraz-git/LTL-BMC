#include "formula.h"

using namespace z3;
//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
class ltl_bmc{
  public:
// private:
    context c;//context class manages all Z3 objects, global configuration options, etc.
    solver  S;//solver class provides methods for sat checking etc.

//Z3 expression is used to represent formulas and terms. a formula is any expression of sort Boolean. every expression (formula or term) has to have a sort. sort (aka type) is another Z3 class.
  
    expr    IF;//formula IF represents the initial states of the state-transition system (Kripke structure).
    expr    TF;//formula TF represents the transition function of the state-transition system (Kripke structure).

    FTree*  LTLF;//formula tree LTLF represents the temporal property to be checked for the Kripke structure (IF,TF)

    int     N;//N is the number of state variables --- used in IF, TF and LTLF.

// public:
    bool    init();

// protected:
    expr    instantiate_P_at_k(int k);

    expr    instantiate_T_at_k(int k);

    expr    loopFree_at_k(int k);

    void    translate(int k);

    void    print_trace(int k);

    expr    translate_LTL_with_backloop_from_k_to_l(FTNode* TLFormula,int k,int l,int i);

    expr    translate_LTL_for_no_loop(FTNode* TLFormula,int k,int i);

    expr    encode_LTL_property_into_PL(int k);

    expr    compute_loop_constraints_at_k(int k);

    expr    instantiate_T_for_loop_constraints(int k,int j);

//exp_vector is vector of expressions defined as typedef ast_vector_tpl<expr>.
//class template ast_vector_tpl<T> defines various methods to manipulate generic vectors.
    expr_vector     x;

    expr_vector     y;

    expr_vector     alpha;

    expr_vector     beta;

    expr_vector     lambda;

    expr    f;//Boolean constant FALSE
    
    expr    t;//Boolean constant TRUE

    ltl_bmc():
        S(c),
        IF(c),
        TF(c),
        x(c),
        y(c),
        alpha(c),
        beta(c),
        lambda(c),f(c),t(c){};//constructor
    ~ltl_bmc(){};//destructor


};
//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
void dummy_display_ft_pre(FTNode* FT);
void dummy_display_ft(FTNode* FT);
