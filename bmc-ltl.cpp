#include "bmc-ltl.h"
//--------------------------------------------------------------------------------------------------
void dummy_display_ft(FTNode* FT)
{
	
	if(FT==NULL){
		std::cout <<"\nError in the formula tree.\n" << std::endl;
		return;
	}
	switch(FT->get_type()){

		case 2: //std::cout <<"\nThe node type is " << FT->get_type() << std::endl;
            std::cout << "(";
			dummy_display_ft(FT->get_left());
			std::cout << FT->get_val();
			dummy_display_ft(FT->get_right());
            std::cout << ")";
			break;

		case 1: //std::cout <<"\nThe node type is " << FT->get_type() << std::endl;
            std::cout << "(";
			std::cout << FT->get_val();
			dummy_display_ft(FT->get_next());
            std::cout << ")";
			break;

		case 0:// std::cout <<"\nThe node type is " << FT->get_type()<<std::endl;
            std::cout << "(";
			std::cout << FT->get_val();
            std::cout << ")";
			break;

		default:
			std::cout <<"\nError! Type can either be 0,1 or 2.\n" << std::endl;
			break;
	}

}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void dummy_display_ft_pre(FTNode* FT)
{
	
	if(FT==NULL){
		std::cout <<"\nError in the formula tree.\n" << std::endl;
		return;
	}
	switch(FT->get_type()){
		case 2: 
            //std::cout <<"\nThe node type is " << FT->get_type()<<std::endl;
			std::cout << FT->get_val();
			dummy_display_ft_pre(FT->get_left());
			dummy_display_ft_pre(FT->get_right());
			break;

		case 1: 
            //std::cout <<"\nThe node type is " << FT->get_type()<<std::endl;
			std::cout << FT->get_val();
			dummy_display_ft_pre(FT->get_next());
			break;

		case 0: 
            //std::cout <<"\nThe node type is " << FT->get_type()<<std::endl;
            std::cout << "(";
			std::cout << FT->get_val();
            std::cout << ")";	
			break;

		default:
			std::cout <<"\nError! Type can either be 0,1 or 2.\n" << std::endl;
			break;
	}

}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
int main()
{
//--------------------------------------------------------------------------------------------------
//------------------------------- CONVENTIONS-------------------------------------------------------
//-------| means lor-------& means land------^ means lxor----% means limplies---~ means lnot--------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//-------------------------------Three bit Ring Counter---------------------------------------------
//	std::string IFText = "((((x0)=(f))&((x1)=(f))&((x2)=(t))))";

//	std::string TFText = "((((y0)=(x1))&((y1)=(x2))&((y2)=(x0))))";

//	std::string PFText = "((((x0)^(x1))^(x2))&(~(((x0)&(x1))&(x2))))";
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Create an object of the type LTLBMC to start the LTL BMC algorithm.
    ltl_bmc LTLBMC;
//--------------------------------------------------------------------------------------------------
//Call the root method that does the rest.
    bool result=LTLBMC.init();
//-------------------------------------------------------------------------------------------------- 	
	return 0;
}
//--------------------------------------------------------------------------------------------------
void ltl_bmc::print_trace(int k)
{
                    model m = S.get_model();
 //                   std::cout << m << "\n\n" <<std::endl;
    // traversing the model
                    for (unsigned i = 0; i < m.size(); i++) {
                 //       std::cout << m[i] << "\n\n" <<std::endl;
                        func_decl v = m[i];
                //        std::cout << v << "\n\n" <<std::endl;
                    // this problem contains only constants
                 //       assert(v.arity() == 0); 
                        std::cout << v.name() << " = " << m.get_const_interp(v) << " ";
                    }
                    std::cout << "\n\n" <<std::endl;


}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool ltl_bmc::init()
{

//--------------------------------------------------------------------------------------------------
//	std::string IF = "(((x0)=(f))&((x1)=(f)))";//Boolean formula for Input state set	

// 	std::string IF = "(((x0)=(f))&((x2)=(f)))";//Boolean formula for Input state set	

// 	std::string IF = "(((x1)=(f))&((x2)=(f)))";//Boolean formula for Input state set 

// 	std::string IF = "(((x0)=(t))&((x2)=(f)))";//Boolean formula for Input state set

//	std::string IF = "((((x0)=(f))&((x1)=(f)))&((x2)=(f)))";//Boolean formula for Input state set	

//  std::string TF = "(((y0)=(x1))&(((y1)=(x2))&((y2)=(t))))";//Boolean formula for Transition Relation

//  std::string TLF = "(G((((x0)=(t))&((x1)=(t)))&((x2)=(t))))";//LTL Formula with G modality

// 	std::string TLF = "(F((((x0)=(t))&((x1)=(t)))&((x2)=(t))))";//LTL Formula with F modality

//	std::string TLF = "(((x0)=(f))U((x0)=(t)))";//LTL Formula with U modality

//  std::string TLF = "(X(((x1)=(f))U((x1)=(t))))";//LTL Formula with X and U modality
//--------------------------------------------------------------------------------------------------
//------------------------------- CONVENTIONS-------------------------------------------------------
//-------| means lor-------& means land------^ means lxor----% means limplies---~ means lnot--------
//--------------------------------------------------------------------------------------------------
//-------------------------------Two bit Counter----------------------------------------------------
//	std::string IF = "(((x0)=(f))&((x1)=(f)))";

//	std::string TF = "((~((y0)^(~(x0))))&(~((y1)^((x0)^(x1)))))";
//--------------------------------------------------------------------------------------------------
//-------------------------------LTL Properties-----------------------------------------------------
//--------------------------------------------------------------------------------------------------
//	std::string TLF = "(G((((x0)=(f))%(X((x0)=(t))))&((((x0)=(t))%(X((x0)=(f))))))";

//	std::string TLF = "(G((((x0)=(t))&(X((x0)=(f))))%((((x1)=(t))&(X((x1)=(f)))|(((x1)=(f))&(X((x1)=(t))))))))";

//	std::string TLF = "(G(((x0)=(f))|((x1)=(f))))";
//--------------------------------------------------------------------------------------------------
//---------------------------------------Safety Property--------------------------------------------
//--------------------------------------------------------------------------------------------------
	std::string TLF = "(G(((~(x0))|(~(x1)))|(~(x2))))";//at least one of the state variable is 0
//--------------------------------------------------------------------------------------------------
//--------------------------------------Liveness Property-------------------------------------------
//--------------------------------------------------------------------------------------------------
//    std::string TLF = "(F(((x0)&(x1))&(x2)))";//all the state variables are eventually 1
//--------------------------------------------------------------------------------------------------
//	std::cout << "The initial state set in symbolic form:" << IF << "\n\n" <<std::endl;

//	std::cout << "The Transition relation in symbolic form:" << TF << "\n\n" <<std::endl;

//	std::cout << "The LTL Formula in symbolic form:" << TLF << "\n\n" <<std::endl;
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
    std::cout << "\n\nEnter the no. of state variables:->";
    std::cin >> this->N;
    std::cout << "\n\n" <<std::endl;            
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
	// loop for creating the current state variables [x0,x1,x2,etc.]
	for(unsigned i=0; i<this->N; ++i){
		std::stringstream xName;
		xName << "x" << i;
		this->x.push_back(c.bool_const(xName.str().c_str()));
	}
	std::cout << this->x << "\n\n" <<std::endl;fflush(stdout);
//--------------------------------------------------------------------------------------------------
	// loop for creating the next state variables [y0,y1,y2,etc.]
//--------------------------------------------------------------------------------------------------
	for(unsigned i=0; i<this->N; ++i){
		std::stringstream yName;
		yName << "y" << i;
		this->y.push_back(c.bool_const(yName.str().c_str()));
	}
	std::cout << this->y << "\n\n" <<std::endl;fflush(stdout);
//--------------------------------------------------------------------------------------------------
//-----------The following state transition system is symbolically represented as (IF,TF)-----------
//------------------------>000->100->110->111->111--------------------------------------------------
//--------------------------------------------------------------------------------------------------
	this->IF=(!x[0] && !x[1] && !x[2]); // initial state formula --- initial state is (0,0,0)
    std::cout << "I is :" << this->IF << "\n\n" <<std::endl;fflush(stdout);

    this->TF= (y[0]== x[1]) && (y[1]==x[2]) && y[2];// transition relation formula --- right shift with MSB 1
    std::cout << "T is :" << this->TF << "\n\n" <<std::endl;fflush(stdout);
    
    std::getchar();
//--------------------------------------------------------------------------------------------------
//--Convert the LTL Formula TLF in text (string type) form to tree (FTNode*) form	
//--Then assign this to LTLF, so that it can later be used by other methods.
//--------------------------------------------------------------------------------------------------
    Formula LTLFormula(TLF);//Creating an object of the type Formula with parameter TLF
	LTLFormula.ftree_convert();//Converting TLF to tree form.
	std::cout <<  "\nFormula Tree for TLF Constructed...............\n" <<std::endl;
//	return(0);

//read the formula tree for TLF and print the leaves
	std::cout <<  "\nReading the Formula Tree...............\n" <<std::endl;
	std::cout <<  "\nIn infix form:";
	LTLFormula.display_formula();
	std::cout << "\n\n\n" <<std::endl;
	std::cout << "\nIn prefix form:";
	LTLFormula.display_formula_pre();
	std::cout << "\n\n\n" <<std::endl;

	std::getchar();
//	return(0);
//--------------------------------------------------------------------------------------------------
//--------------Negate the LTLFormula and construct the NNF-----------------------------------------
//--------------------------------------------------------------------------------------------------
    FTree* TFormula = LTLFormula.get_ftree();
    FTree* NegTFormula = FTree::negate(TFormula);
    FTNode* NegTFormulaRoot = NegTFormula->get_root();
    FTNode* TLFNode = LTLFormula.const_nnf(NegTFormulaRoot,-1);   
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------Assign the LTL formula, in tree form, to the appropriate data member----------------
//---------------------- in the ltl_bmc object we are working with.---------------------------------
//--------------------------------------------------------------------------------------------------    
    this->LTLF = new FTree(TLFNode);
//--------------------------------------------------------------------------------------------------
//---------------------definition of logical constants f and t.-------------------------------------
//--------------------------------------------------------------------------------------------------
    this->f=c.bool_const("f");
	this->t=c.bool_const("t");
//--------------------------------------------------------------------------------------------------
//---------For k=0, we don't need to initialize x[0],x[1],.......,x[N-1]. Already done above.
//--------------------------------------------------------------------------------------------------
//------Compute the necessary formulas for k=0 trace length, by calling the translate method.
//--------------------------------------------------------------------------------------------------
	this->translate(0);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Now compute the actual formula to be sent to SAT solver, for k=0 trace length.
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------		
    S.add(alpha[0] && beta[0]);
	bool result=S.check();

	if(result==sat){
//Print the counterexample and exit	
		std::cout <<"\nThe counterexample is of length 0";
		std::cout << "\n(";
        print_trace(0);
		return 0;
	}
	else{
		std::cout <<"\nNo counterexample of length 0";
		std::cout <<"\n----------------------------------------------------------------\n" <<std::endl;
		std::getchar();	
				
	}
    S.reset();
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Run a loop to inductively check property for trace lengths of k>0.
//--------------------------------------------------------------------------------------------------
	int k=1;
	while(1){
	    std::cout <<"\nLooking at traces of length " << k;
//--------------------------------------------------------------------------------------------------
//--------------Before we compute the various formulas in the translate routine,--------------------
//------------------ we should initialize the sequence of x expressions.----------------------------
//--------------------------------------------------------------------------------------------------
//------------loop for creating the current state variables [xk*N+0,xk*N+1,xk*N+2,etc.]-------------
//--------------------------------------------------------------------------------------------------
	    for(unsigned i=0; i<N; ++i){
		    std::stringstream xName;
		    xName << "x" << k*N+i;
		    x.push_back(c.bool_const(xName.str().c_str()));
	    }
        std::cout << "list of x variables:" << x << "\n\n" <<std::endl;
        std::getchar();
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Translate the LTLFormula to a PL formula for all traces of length k in the transition system (IF,TF).
//LTLformula is being asserted in an initial state at the time instant 0.
//The LTLFormula is encoded into the trace of length k. 
//This is done through an inductive process starting from point 0.
//That is why we have an i in the encoding.
//--------------------------------------------------------------------------------------------------
		this->translate(k);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Now compute the actual formula to be sent to SAT solver
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------			
        S.add(alpha[k] && beta[k]);
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//Reset the previous switches
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
		result=S.check();

		if(result==sat){
//Print the counterexample and exit	
			std::cout <<"\nThe counterexample is of length "<< k << "\n" <<std::endl;fflush(stdout);
			std::getchar();
            print_trace(k);
			return 0;
		}
		else{
//Continue the loop
			std::cout <<"\nNo counterexample of length "<< k << "\n" <<std::endl;
			std::cout <<"\n----------------------------------------------------------------\n" <<std::endl;
//--------------------------------------------------------------------------------------------------
			k++;//move to the next trace length
            S.reset();//reset the SAT-solver state
            std::cout<<"Move to the next trace length k =  " << k << "\n";
			std::getchar();
			continue;
		}
			

	}
//-------------------------------------------------------------------------------------------------- 	
	return 0;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

