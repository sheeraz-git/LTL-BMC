
#include "bmc-ltl.h"
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
//----------------loopFree_at_k() is not needed for LTL-BMC.----------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
expr ltl_bmc::loopFree_at_k(int k)
{

                expr_vector	z(c);
				for(unsigned i=0; i<k*N; ++i){
					std::stringstream xName;
					xName << "x" << i;
					z.push_back(c.bool_const(xName.str().c_str()));
				}

//                std::cout << "\n\ncontents of z are" << z << "\n" << std::endl;
//                //getchar();

                expr_vector	z1(c);
				for(unsigned i=0; i<N; ++i){
					std::stringstream xName;
					xName << "x" << i+N*k;
					z1.push_back(c.bool_const(xName.str().c_str()));
				}

//                std::cout << "\n\ncontents of z1 are" << z1 << "\n" << std::endl;
//                //getchar();

//Now, compute loopFree expression
                std::cout << "\n\nNow we compute loopFree expression for k=" << k << "\n" << std::endl;

                expr temp0(c);
                expr temp1(c);

                temp0=(z1[0]!=z[0]);
                for(unsigned i=1; i<N; ++i){
                    temp0=temp0 || (z1[i]!=z[i]);
                }
                temp1=temp0;
                for(unsigned j=1; j<k; ++j){
                    temp0=(z1[0]!=z[N*j]);
                    for(unsigned i=1; i<N; ++i){
                        temp0=temp0 || (z1[i]!=z[N*j+i]);
                    }
                    temp1=temp1 && temp0;
                }
                std::cout << "\n\nLoopFree formula is-->" << temp1 << "\n\n" << std::endl;
                return temp1;

}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void ltl_bmc::translate(int k)
{
//--------------------------------------------------------------------------------------------------
//The translate method takes parameter k as input and constructs the Boolean expressions needed to execute BMC
//for any k: alpha[k] contains the Boolean expression describing all paths of length k in input system represented by (IF,TF).
//for any k: beta[k] contains the Boolean approximation of the LTL property TLF for length k trace.
//for any k: lambda[k] contains the Boolean expression describing (k,l)-loop for any k<=l in the path of length k.
//--------------------------------------------------------------------------------------------------
	std::cout << "\nWe are in the translate routine with k= " << k << std::endl;

	switch(k){
	
		case 0:	

			alpha.push_back(IF);//for trace of length 0
			std::cout << "\nComputed alpha[0]\n" << std::endl;
        	std::getchar();
			
			beta.push_back(encode_LTL_property_into_PL(0));//for trace of length 0
			std::cout << "\nComputed beta[0]\n" << std::endl;
        	std::getchar();

			lambda.push_back(compute_loop_constraints_at_k(0));//loop constraint for trace of length k
			std::cout << "\nComputed lambda[0]\n" << std::endl;
        	std::getchar();

			break;

		default:

			alpha.push_back(alpha[k-1] && instantiate_T_at_k(k));//for trace of length k
			std::cout << "\nComputed alpha[" << k << "]\n" << std::endl;
        	std::getchar();
			
			beta.push_back(encode_LTL_property_into_PL(k));//for trace of length k
			std::cout << "\nComputed beta[" << k << "]\n" << std::endl;
        	std::getchar();

			lambda.push_back(compute_loop_constraints_at_k(k));//loop constraint for trace of length k
			std::cout << "\nComputed lambda[" << k << "]\n" << std::endl;
        	std::getchar();

			break;
	}
	
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------IMPORTANT---------------------------------------------------
//---The lambda being computed in the translate() method is not being used in LTL encoding.---------
//---Consequently, the method compute_loop_constraints_at_k() is not being used anywhere.-----------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
expr ltl_bmc::encode_LTL_property_into_PL(int k)
{

	expr temp0(c),temp1(c),temp2(c),temp3(c);

	std::cout << "\nWe are in encode_LTL_property_into_PL routine with k= " << k << "\n" << std::endl;
	std::getchar();

	temp0=instantiate_T_for_loop_constraints(k,0);//T(s_k,s_0)
	std::cout << "\nComputed the loop constraint T(s_"<< k << ",s_0) and assigned to temp0\n" << std::endl;
	std::getchar();

	temp2=temp0 && translate_LTL_with_backloop_from_k_to_l(LTLF->get_root(),k,0,0);
	std::cout << "\nComputed the encoded formula T(s_"<< k << ",s_0) land 0_[f]_" << k << "^0 and assigned to temp2\n" 
                                << std::endl;
	std::getchar();
    
	temp3=temp0;
	for (int l=1;l<=k;l++){

		temp0=instantiate_T_for_loop_constraints(k,l);//T(s_k,s_l)

    	std::cout << "\nComputed the loop constraint T(s_" << k << ",s_" << l << ") and assigned to temp0\n" 
                                << std::endl;
	    std::getchar();

		temp1=temp0 && translate_LTL_with_backloop_from_k_to_l(LTLF->get_root(),k,l,0);
	    std::cout << "\nComputed the encoded formula T(s_" << k << ",s_" << l << ") land " << l << "_[f]_" << k
                                << "^0 and assigned to temp1\n" << std::endl;
	    std::getchar();

		temp2=temp2 || temp1;//\bigwedge_{j=0}^{l}(L_k^j \land [f,0]_k^j)
        std::cout << "\ntemp2 is updated by ORing it with temp1\n" << std::endl;
	    std::getchar();
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
		temp3=temp3||temp0;//L_k^l=T(s_k,s_0)\lor T(s_k,s_1)\lor \cdots \lor T(s_k,s_l)		
        std::cout << "\ntemp3 is updated by ORing it with temp0\n" <<std::endl;
	    std::getchar();

		
	}
//--------------------------------------------------------------------------------------------------
//--------------temp2 contains the encoded formula guarded with loop constraints.-------------------
//-----------------------temp3 contains all the loop constraints.-----------------------------------
//--------------------------------------------------------------------------------------------------
	return ((!temp3 && translate_LTL_for_no_loop(LTLF->get_root(),k,0)) || temp2);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
expr ltl_bmc::translate_LTL_with_backloop_from_k_to_l(FTNode* TLFormula,int k,int l,int i)
{
	expr temp0(c),temp1(c),temp2(c),temp3(c),temp4(c),temp5(c),temp6(c);


	std::cout << "\nWe are in the method translate_LTL_with_backloop_from_k_to_l with k= " << k 
                << " and i=" << i << " and l= " << l << "\n" << std::endl;
    
	if(TLFormula==NULL){
		std::cout << "\nError in the formula tree.\n" << std::endl;
		std::exit(1);
	}

    std::cout << "\nConsidering the LTL subformula:" <<std::endl;FTree::display_ft_pre(TLFormula);

	switch(TLFormula->get_type()){
		case 2: 
			switch(TLFormula->get_val()[0]){
				case '='://EQUALITY
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
					temp1=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_left(),k,l,i);
					temp2=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,i);
					temp3=(temp1==temp2);
					return temp3;
				case '&'://AND
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
					temp1=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_left(),k,l,i);
					temp2=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,i);
					temp3=(temp1 && temp2);
					return temp3;

				case '|'://OR
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
					temp1=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_left(),k,l,i);
					temp2=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,i);
					temp3=(temp1 || temp2);
					return temp3;


				case '^'://XOR
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
					temp1=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_left(),k,l,i);
					temp2=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,i);
					temp3=(temp1 || temp2) && ! (temp1 && temp2);
					return temp3;

				case '$'://NAND
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
					temp1=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_left(),k,l,i);
					temp2=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,i);
					temp3=! (temp1 && temp2);
					return temp3;



				case '#'://NOR
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
					temp1=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_left(),k,l,i);
					temp2=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,i);
					temp3=!(temp1 || temp2);
					return temp3;




				case '%'://IMPLIES
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
					temp1=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_left(),k,l,i);
					temp2=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,i);
					temp3=implies(temp1,temp2);
					return temp3;



				case 'U'://UNTIL
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
					temp1=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,i);//[g]^i
					for(int j=i+1;j<=k;j++){
						temp2=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,j); //[g]^j
						temp3=temp2 && translate_LTL_with_backloop_from_k_to_l(TLFormula->get_left(),k,l,i); //[g]^j and [f]^i
						for(int n=i+1;n<=j-1;n++){
						  temp3=temp3 && translate_LTL_with_backloop_from_k_to_l(TLFormula->get_left(),k,l,n); //[g]^j and [f]^i and [f]^i+1 and ....[f]^n
						}
						temp1=temp1 || temp3;//[g]^i or ([g]^j and [f]^i and [f]^i+1 and ....[f]^j)
					}



					temp0=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,l);//[g]^i
					temp5=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_left(),k,l,i);
					for(int n=i+1;n<=k;n++){
						temp5=temp5 && translate_LTL_with_backloop_from_k_to_l(TLFormula->get_left(),k,l,n);
					}
					temp0=temp0 && temp5;

					for(int j=l+1;j<=k;j++){
						temp2=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_left(),k,l,j); //[g]^j

				     	temp4=temp2 && translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,i);
						for(int n=i+1;n<=k;n++){
							temp4=temp4 && translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,n);
						}

						temp3=temp4 && translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,l); //[g]^j and [f]^i
						for(int n=l+1;n<=j-1;n++){
						  temp3=temp3 && translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,n); //[g]^j and [f]^i and [f]^i+1 and ....[f]^n
						}
						temp0=temp0 || temp3;//[g]^i or ([g]^j and [f]^i and [f]^i+1 and ....[f]^j)
					}
					std::cout << "\nReturning a proposition for " << TLFormula->get_val() << " formula\n" << std::endl;
					std::cout << "\n----------------------------------------------------------------\n" << std::endl;
					//std::getchar();

					return temp1 || temp0;


				case 'R'://RELEASE
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;


					temp1=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_left(),k,l,i);//[g]^i
					for(int j=i+1;j<=k;j++){
						temp2=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_left(),k,l,j); //[g]^j
						temp3=temp2 && translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,i); //[g]^j and [f]^i
						for(int n=i+1;n<=j;n++){
						  temp3=temp3 && translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,n); //[g]^j and [f]^i and [f]^i+1 and ....[f]^n
						}
						temp1=(temp1 || temp3);//[g]^i or ([g]^j and [f]^i and [f]^i+1 and ....[f]^j)
					}



					temp0=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_left(),k,l,l);//[g]^i
					temp5=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,i);
					for(int n=i+1;n<=k;n++){
						temp5=temp5 && translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,n);
					}
					temp0=temp0 && temp5;

					for(int j=l+1;j<=k;j++){
						temp2=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,j); //[g]^j

    					     	temp4=temp2 && translate_LTL_with_backloop_from_k_to_l(TLFormula->get_left(),k,l,i);
						for(int n=i+1;n<=k;n++){
							temp4=temp4 && translate_LTL_with_backloop_from_k_to_l(TLFormula->get_left(),k,l,n);
						}

						temp3=temp4 && translate_LTL_with_backloop_from_k_to_l(TLFormula->get_left(),k,l,l); //[g]^j and [f]^i
						for(int n=l+1;n<=j;n++){
						  temp3=temp3 && translate_LTL_with_backloop_from_k_to_l(TLFormula->get_left(),k,l,n); //[g]^j and [f]^i and [f]^i+1 and ....[f]^n
						}
						temp0=temp0 || temp3;//[g]^i or ([g]^j and [f]^i and [f]^i+1 and ....[f]^j)
					}
					std::cout << "\nReturning a proposition for " << TLFormula->get_val() << " formula\n" << std::endl;
					std::cout << "\n----------------------------------------------------------------\n" << std::endl;
					//std::getchar();


					if(i<l){
						std::cout<< "\nThe Case when i < l: i=  " << i << "  l=  " << l << "\n" << std::endl;
						temp6=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,i);
						for(int j=i+1;j<=k;j++){
						 temp6=temp6 && translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,j);
						}
						std::cout << "\nReturning a non-false proposition for G formula\n" << std::endl;
						std::cout << "\n----------------------------------------------------------------\n" << std::endl;
						//std::getchar();

					}
					else{
						std::cout<< "\nThe Case when i >= l: i=  " << i << "  l=  " << l << "\n" << std::endl;
						temp6=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,l);
						for(int j=l+1;j<=k;j++){
						 temp6=temp6 && translate_LTL_with_backloop_from_k_to_l(TLFormula->get_right(),k,l,j);
						}
						std::cout << "\nReturning a non-false proposition for G formula\n" << std::endl;
						std::cout << "\n----------------------------------------------------------------\n" << std::endl;
						//std::getchar();

					}

					return (temp6 || (temp0 || temp1));


				default:
					std::cout << "\nError\n" << std::endl;
					std::exit(1);

			}

		case 1: 
			switch(TLFormula->get_val()[0]){
				case '~'://NOT
					std::cout << "\nLooking at a " << TLFormula->get_val()[0] << " node\n" << std::endl;
					temp1=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_next(),k,l,i);					
					return !temp1;

				case 'X'://NEXT
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
					if(i<k)
						return(translate_LTL_with_backloop_from_k_to_l(TLFormula->get_next(),k,l,i+1));
					else
						return(translate_LTL_with_backloop_from_k_to_l(TLFormula->get_next(),k,l,l));
					

				case 'F'://DIAMOND -- EVENTUALLY
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
					if(i<l){
						temp1=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_next(),k,l,i);
						for(int j=i+1;j<=k;j++){
						 temp1=temp1 || translate_LTL_with_backloop_from_k_to_l(TLFormula->get_next(),k,l,j);
						}
						std::cout << "\nReturning a non-false proposition for F formula\n" << std::endl;
						std::cout << "\n----------------------------------------------------------------\n" << std::endl;
						return temp1;
					}
					else{
						temp1=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_next(),k,l,l);
						for(int j=l+1;j<=k;j++){
						 temp1=temp1 || translate_LTL_with_backloop_from_k_to_l(TLFormula->get_next(),k,l,j);
						}
						std::cout << "\nReturning a non-false proposition for F formula\n" << std::endl;
						std::cout << "\n----------------------------------------------------------------\n" << std::endl;
						return temp1;
					}


				case 'G'://BOX -- ALWAYS
					std::cout << "\nLooking at a " << TLFormula->get_val()[0] << " node\n" << std::endl;
					if(i<l){
						std::cout<< "\nThe Case when i < l: i=  " << i << "  l=  " << l << "\n" << std::endl;
						temp1=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_next(),k,l,i);
						for(int j=i+1;j<=k;j++){
						 temp1=temp1 && translate_LTL_with_backloop_from_k_to_l(TLFormula->get_next(),k,l,j);
						}
						std::cout << "\nReturning a non-false proposition for G formula:" << temp1 << std::endl;
						std::cout << "\n----------------------------------------------------------------\n" << std::endl;
						std::getchar();
						return temp1;
					}
					else{
						std::cout<< "\nThe Case when i >= l: i=  " << i << "  l=  " << l << "\n" << std::endl;
						temp1=translate_LTL_with_backloop_from_k_to_l(TLFormula->get_next(),k,l,l);
						for(int j=l+1;j<=k;j++){
						 temp1=temp1 && translate_LTL_with_backloop_from_k_to_l(TLFormula->get_next(),k,l,j);
						}
						std::cout << "\nReturning a non-false proposition for G formula:" << temp1 << std::endl;
						std::cout << "\n----------------------------------------------------------------\n" << std::endl;
						std::getchar();
						return temp1;
					}


					
				default:
					std::cout << "\nError\n" << std::endl;
					std::exit(1);

			}
		case 0: 

			if(TLFormula->get_val()=="t"){
				std::cout << "\nReplacing t by true\n" << std::endl;
				return (t && true);
			}
			else if(TLFormula->get_val()=="f"){
				std::cout << "\nReplacing f by false\n" << std::endl;
				return (f && false);			
            }
			else{
				char* strng;
				int M=0;
				int ia;
				int len=TLFormula->get_val().size();

				if(TLFormula->get_val()[0]!='x'){
					std::cout << "Error in the input\nThe variables in I/P can only have x\n" << std::endl;
					std::exit(1);
				
				}
				
				TLFormula->get_val().copy(strng,len-1,1);
//Copies a substring of the string object TLFormula->get_val() into the array pointed by strng. 
//This substring contains the len-1 characters that start at position position 1.

				//strng contains a number as string -- this string is converted to integer M
				for(int t=0;t<len-1;t++){
					ia=strng[t]-'0';
					M=10*M+ia;
				}
				std::cout << "\nReplacing " << TLFormula->get_val() << " by x[" << M << "][" << i << "]\n" << std::endl;
		        //std::stringstream xName;
		        //xName << "x" << M + i*N;
		        //return c.bool_const(xName.str().c_str());
//is this expr of Boolean type or int type ? please check ?			
				return x[M + i*N];
				
				
			}			
			std::cout << "\nError\n" << std::endl;
			std::exit(1);

		default:
					std::cout << "\nError\n" << std::endl;
					std::exit(1);
			
			break;

	}

}
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
expr ltl_bmc::translate_LTL_for_no_loop(FTNode* TLFormula,int k,int i)
{

	expr temp1(c),temp2(c),temp3(c);

//  expr f=c.bool_const("f");
//	expr t=c.bool_const("t");

	std::cout << "\nWe are in the method translate_LTL_for_no_loop routine with k= " << k << " and i= " << i << "\n" << std::endl;
	if(TLFormula==NULL){
		std::cout << "\nError in the formula tree.\n" << std::endl;
		std::exit(1);
	}

	switch(TLFormula->get_type()){
		case 2: 
			switch(TLFormula->get_val()[0]){
				case '=':
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << "  node\n" << std::endl;
					temp1=translate_LTL_for_no_loop(TLFormula->get_left(),k,i);
					temp2=translate_LTL_for_no_loop(TLFormula->get_right(),k,i);
					temp3=temp1==temp2;
					return temp3;
				case '&':
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
					temp1=translate_LTL_for_no_loop(TLFormula->get_left(),k,i);
					temp2=translate_LTL_for_no_loop(TLFormula->get_right(),k,i);
					temp3=(temp1 && temp2);
					return temp3;

				case '|':
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
					temp1=translate_LTL_for_no_loop(TLFormula->get_left(),k,i);
					temp2=translate_LTL_for_no_loop(TLFormula->get_right(),k,i);
					temp3=(temp1 || temp2);
					return temp3;




				case '^':
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
					temp1=translate_LTL_for_no_loop(TLFormula->get_left(),k,i);
					temp2=translate_LTL_for_no_loop(TLFormula->get_right(),k,i);
					temp3=(temp1 || temp2) && !(temp1 && temp2);
					return temp3;

				case '$':
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
					temp1=translate_LTL_for_no_loop(TLFormula->get_left(),k,i);
					temp2=translate_LTL_for_no_loop(TLFormula->get_right(),k,i);
					temp3=!(temp1 && temp2);
					return temp3;



				case '#':
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
					temp1=translate_LTL_for_no_loop(TLFormula->get_left(),k,i);
					temp2=translate_LTL_for_no_loop(TLFormula->get_right(),k,i);
					temp3=!(temp1 || temp2);
					return temp3;




				case '%':
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
					temp1=translate_LTL_for_no_loop(TLFormula->get_left(),k,i);
					temp2=translate_LTL_for_no_loop(TLFormula->get_right(),k,i);
					temp3=implies(temp1,temp2);
					return temp3;





				case 'U':
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
//loop from j=i to j=k
					temp1=translate_LTL_for_no_loop(TLFormula->get_right(),k,i);//[g]^i
					for(int j=i+1;j<=k;j++){
						temp2=translate_LTL_for_no_loop(TLFormula->get_right(),k,j); //[g]^j
						temp3=(temp2 && translate_LTL_for_no_loop(TLFormula->get_left(),k,i)); //[g]^j and [f]^i
						for(int n=i+1;n<=j-1;n++){
						  temp3=(temp3 && translate_LTL_for_no_loop(TLFormula->get_left(),k,n)); //[g]^j and [f]^i and [f]^i+1 and ....[f]^n
						}
						temp1=(temp1 || temp3);//[g]^i or ([g]^j and [f]^i and [f]^i+1 and ....[f]^j)
					}
					std::cout << "\nReturning a proposition for " << TLFormula->get_val() << " formula\n" << std::endl;
					std::cout << "\n----------------------------------------------------------------\n" << std::endl;
					//std::getchar();

					return temp1;

				case 'R':
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
					temp1=translate_LTL_for_no_loop(TLFormula->get_left(),k,i);//[f]^i
					for(int j=i+1;j<=k;j++){
						temp2=translate_LTL_for_no_loop(TLFormula->get_left(),k,j); //[f]^j
						temp3=(temp2 && translate_LTL_for_no_loop(TLFormula->get_right(),k,i)); //[f]^j and [g]^i
						for(int n=i+1;n<=j;n++){
						  temp3=(temp3 && translate_LTL_for_no_loop(TLFormula->get_right(),k,n)); //[f]^j and [g]^i and [g]^i+1 and ....[g]^n
						}
						temp1=(temp1 || temp3);//[f]^i or ([f]^j and [g]^i and [g]^i+1 and ....[g]^j)
					}
					std::cout << "\nReturning a proposition for " << TLFormula->get_val() << " formula\n" << std::endl;
					std::cout << "\n----------------------------------------------------------------\n" << std::endl;
					//std::getchar();

					return temp1;


				default:
					std::cout << "\nError\n" << std::endl;
					std::exit(1);

			}

		case 1: 
			switch(TLFormula->get_val()[0]){
				case '~':
					std::cout << "\nLooking at a " << TLFormula->get_val()[0] << " node\n" << std::endl;
					temp1=translate_LTL_for_no_loop(TLFormula->get_next(),k,i);
					//temp2=S.lnot(temp1);
					temp2=!temp1;
					return temp2;

				case 'X':
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
					if(i<k)
						return(translate_LTL_for_no_loop(TLFormula->get_next(),k,i+1));
					else{
                        expr f=c.bool_const("f");
				        return (f && false);				
                    }
				case 'F':
					std::cout << "\nLooking at an " << TLFormula->get_val()[0] << " node\n" << std::endl;
					temp1=translate_LTL_for_no_loop(TLFormula->get_next(),k,i);
					for(int j=i+1;j<=k;j++){
						temp1=(temp1 || translate_LTL_for_no_loop(TLFormula->get_next(),k,j));
					}
					std::cout << "\nReturning a proposition for " << TLFormula->get_val() << " formula\n" << std::endl;
					std::cout << "\n----------------------------------------------------------------\n" << std::endl;
					//std::getchar();

					return temp1;


				case 'G':
					std::cout << "\nLooking at a " << TLFormula->get_val()[0] << " node\n\nReturning false proposition";
					//std::getchar();
				    return (f && false);

					
				default:
					std::cout << "\nError\n" << std::endl;
					std::exit(1);

			}
		case 0: 

			if(TLFormula->get_val()=="t"){
				std::cout << "\nReplacing t by true\n" << std::endl;
				return (t || true);
			}
			else if(TLFormula->get_val()=="f"){
				std::cout << "\nReplacing f by false\n" << std::endl;
				return (f && false);
			}
			else{
				char* strng;
				int M=0;
				int ia;
				int len=TLFormula->get_val().size();

				if(TLFormula->get_val()[0]!='x'){
					std::cout << "Error in the input\nThe variables in LTLF can only have x\n" << std::endl;
					exit(0);
				
				}

				
				TLFormula->get_val().copy(strng,len-1,1); 
//Copies a substring of the string object TLFormula->get_val() into the array pointed by strng. 
//This substring contains the len-1 characters that start at position position 1.

				//strng contains a number as string -- this is converted to integer M
				for(int t=0;t<len-1;t++){
					ia=strng[t]-'0';
						M=10*M+ia;
				}
				//std::cout << "\nReplacing " << TLFormula->get_val() << " by x[" << M << "][" << i << "]\n" << std::endl;
		        //std::stringstream xName;
		        //xName << "x" << M + i*N;
		        //return c.bool_const(xName.str().c_str());	
                return x[M + i*N];								
			}


			
			std::cout << "\nError\n" << std::endl;
			std::exit(1);
			
		default:
					std::cout << "\nError\n" << std::endl;
					std::exit(1);
			
			break;

	}

}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
expr ltl_bmc::instantiate_T_at_k(int k)
{
//--------------------------------------------------------------------------------------------------
//-------------In the case of instantiation of T, we do not need any tree traversal.----------------
//--------------We just have to call the substitute method provided by expr class.------------------
//--------------------------------------------------------------------------------------------------
//--------------------This method computes T(s_{k-1},s_{k}), given k.-------------------------------
//---------------So, in the formula T, (x_0,x_1,...x_{N-1}) is replaced by -------------------------
//---------------------(x_{N*(k-1)+1},x_{N*(k-1)+2},...x_{N*(k-1)+N-1})-----------------------------
//----------------and (y_0,y_1,...y_{N-1}) is replaced by (x_{N*k+1},x_{N*k+2},...x_{N*k+N-1})------
//--------------------------------------------------------------------------------------------------

	std::cout << "\nWe are in the instantiate_T_at_k routine with k= " << k << "\n" << std::endl;
/*
                expr_vector	z(c);
				for(unsigned i=0; i<N; ++i){
					z.push_back(x[i]);
				}
                expr_vector	z0(c);
				for(unsigned i=0; i<N; ++i){
					z0.push_back(x[i+N*(k-1)]);
				}
                std::cout << "\nz0 that replaces x in T is:\t" << z0 << std::endl;
                expr_vector	z1(c);
				for(unsigned i=0; i<N; ++i){
					z1.push_back(x[i+N*k]);
				}
                std::cout << "\nz1 that replaces y in T is:\t" << z1 << std::endl;
*/
    expr_vector	z(c);
    expr_vector	z0(c);
    expr_vector	z1(c);
    for(unsigned i=0; i<N; ++i){
        z.push_back(x[i]);
        z0.push_back(x[i+N*(k-1)]);
        z1.push_back(x[i+N*k]);
    }
    std::cout << "\nz0 that replaces x in T is:\t" << z0 << std::endl;
    std::cout << "\nz1 that replaces y in T is:\t" << z1 << std::endl;
//--------------------------------------------------------------------------------------------------------
//-------------subT is T(x,y) with x=(x0,x1,..x_N-1) replaced by current state z0 and --------------------
//---------------------y=(y0,y1,...y_N-1) replaced by next state z1.--------------------------------------
//-----------------We should make a copy of TF before substitution.---------------------------------------
//--------------------------------------------------------------------------------------------------------
    expr subT(c);
    expr T=TF;
    subT=T.substitute(y,z1);
    subT=subT.substitute(z,z0);
    std::cout << "T substituted : " << subT << "\n\n" << std::endl; 
    std::getchar();
    return subT;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
expr ltl_bmc::compute_loop_constraints_at_k(int k)
{
//This method computes temp0=T(s_k,s_0) \lor T(s_k,s_1) \lor T(s_k,s_2) \lor \cdots \lor T(s_k,s_k)
	expr temp0(c);
	std::cout << "\nIn the compute_loop_constraints_at_k routine with k=" << k << "\n" << std::endl;

	temp0=instantiate_T_for_loop_constraints(k,0);//Initially, temp0=T(s_k,s_0)
	for (unsigned j=1;j<=k;j++){
		temp0=temp0 || instantiate_T_for_loop_constraints(k,j);//temp0=temp0 \lor T(s_k,s_j) --- where j=1 to k 
		
	}
	return temp0;
//temp0 is satisfiable only if there is a back loop from k to some l: 0 \le l \le k
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
expr ltl_bmc::instantiate_T_for_loop_constraints(int k,int j)
{
//--------------------------------------------------------------------------------------------------
//In the case of instantiation of T, we do not need any tree traversal. 
//We just have to call the substitute method provided by expr class.
//--------------------------------------------------------------------------------------------------
//This method computes T(s_k,s_j), given k and j.
//So, in the formula T, (x_0,x_1,...x_{N-1}) is replaced by (x_{N*k+1},x_{N*k+2},...x_{N*k+N-1})
//and (y_0,y_1,...y_{N-1}) is replaced by (x_{N*j+1},x_{N*j+2},...x_{N*j+N-1})
//--------------------------------------------------------------------------------------------------

	std::cout << "\nWe are in the instantiate_T_for_loop_constraints routine with k=" << k << " and j=" <<j << "\n" << std::endl;
    std::cout << "\nComputing the formula T(s_" << k<< ",s_" <<j<< ")\n" << std::endl;
               
/*                
                expr_vector	z(c);
				for(unsigned i=0; i<N; ++i){
					z.push_back(x[i]);
				}
                expr_vector	z0(c);
				for(unsigned i=0; i<N; ++i){
					z0.push_back(x[i+N*k]);
				}
                std::cout << "\nz0 that replaces y in T is:\t" << z0 << std::endl;
                expr_vector	z1(c);
				for(unsigned i=0; i<N; ++i){
					z1.push_back(x[i+N*j]);
				}
                std::cout << "\nz1 that replaces x in T is:\t" << z1 << std::endl;
*/
//-----------------------------------------------------------------------------------------------------
//We have replaced the above three loops by one loop.
//-----------------------------------------------------------------------------------------------------
    expr_vector	z(c);
    expr_vector	z0(c);
    expr_vector	z1(c);
    for(unsigned i=0; i<N; ++i){
        z.push_back(x[i]);
		z0.push_back(x[i+N*k]);
    	z1.push_back(x[i+N*j]);
	}

    std::cout << "\nz0 that replaces y in T is:\t" << z0 << std::endl;
    std::cout << "\nz1 that replaces x in T is:\t" << z1 << std::endl;

    std::cout << "\ns_" << k<< " is:" << z0 << "\ns_" <<j<< " is:" << z1 << "\n" << std::endl;
    std::cout << "\nT is: " << TF << std::endl;

//subT is T(x,y) with x=(x0,x1,x2) replaced current state z0 and y=(y0,y1,y2) replaced by next state z1.
    expr subT(c);
    expr T=TF;
    subT=T.substitute(y,z1);
    subT=subT.substitute(z,z0);
    std::cout << "T substituted : " << subT << "\n\n" << std::endl; 
    std::getchar();
    return subT;

}
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
