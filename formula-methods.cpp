#include "formula.h"

//---------------------------------------------------------------------------------------------------------------------
/*
void FTree::display_ftree()
{
	display_ft(FormulaTree);
	return;
}
//-----------------------------------------------------------------------------------------------------------------------
void FTree::display_ftree_pre()
{
	display_ft_pre(FormulaTree);
	return;
}
*/
//---------------------------------------------------------------------------------------------------------------------
void FTree::display_ft(FTNode* FT)
{
	
	if(FT==NULL){
		std::cout <<"\nError in the formula tree.\n" << std::endl;
		return;
	}
	switch(FT->get_type()){
		case 2: ////std::cout <<"\nThe node type is " << FT->get_type() << std::endl;
			std::cout << "(";
			display_ft(FT->get_left());
			std::cout << FT->get_val();
			display_ft(FT->get_right());
			std::cout << ")";
			break;

		case 1: ////std::cout <<"\nThe node type is " << FT->get_type() << std::endl;
			std::cout << "(";
			std::cout << FT->get_val();
			display_ft(FT->get_next());
			std::cout << ")";
			break;

		case 0: ////std::cout <<"\nThe node type is " << FT->get_type() << std::endl;
			std::cout << "(";
			std::cout << FT->get_val();
			std::cout << ")";
			break;

		default:
			std::cout  << "\nError! Type can either be 0,1 or 2.\n" << std::endl;
			break;
	}

}
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
void FTree::display_ft_pre(FTNode* FT)
{
	
	if(FT==NULL){
		std::cout <<"\nError in the formula tree.\n" << std::endl;
		return;
	}
	switch(FT->get_type()){

		case 2: ////std::cout <<"\nThe node type is " << FT->get_type() << std::endl;
			std::cout << FT->get_val();
			display_ft_pre(FT->get_left());
			display_ft_pre(FT->get_right());
			break;

		case 1: ////std::cout <<"\nThe node type is " << FT->get_type() << std::endl;
			std::cout << FT->get_val();
			display_ft_pre(FT->get_next());
			break;

		case 0: ////std::cout <<"\nThe node type is " << FT->get_type() << std::endl;
			std::cout << "(";
			std::cout << FT->get_val();
			std::cout << ")";
			break;

		default:
			std::cout  << "\nError! Type can either be 0,1 or 2.\n" << std::endl;
			break;
	}

}
//--------------------------------------------------------------------------------------------------------
bool Formula::construct_nnf()
{
	int flag=-1;
	FTNode* NNFTNode=const_nnf(FormulaTree->get_root(),flag);
	this->NNFormula = new FTree(NNFTNode);//what is this ?
    //when an object is being created using new then it has to be deleted eventually.
    //there is no garabage collection in C++

//After the NNF of the formula is constructed, we print and check it.
	//std::cout <<"\nNNF of Formula Tree Constructed...............\n" << std::endl;
	fflush(stdout);
//read the formula tree and print the leaves
	//std::cout <<"\nReading the Formula Tree...............\n" << std::endl;
	//std::cout <<"\nIn infix form:";
	//FTree::display_ft((this->NNFormula)->root);
    FTree::display_ft(NNFTNode);
	//std::cout <<"\n" << std::endl;
	//std::cout <<"\nIn prefix form:";
	//FTree::display_ft((this->NNFormula)->root);
    FTree::display_ft(NNFTNode);
	//std::cout <<"\n" << std::endl;
}
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
FTNode* Formula::const_nnf(FTNode* TFormula, int flag)
{
	FTNode* NTFormula=NULL;
	
	switch(flag)
	{
		case -1:// this is the beginning of the nnf construction. look at the root. if it is labelled ~ make changes in the child node and call nnf recursively with flag =1. else if it is not labelled ~ keep the child as it is and call nnf recursively with flag=0.
			//std::cout <<"\nWe are looking at the root of the formula tree.\n" << std::endl;
			if(TFormula->get_val()[0]=='~')
			{
				//std::cout <<"\nThere is a ~(NOT) at the root.\n" << std::endl;
				return(const_nnf(TFormula->get_next(),1));
			}
			else{
				//std::cout <<"\nThere is NO ~(NOT) at the root.\n" << std::endl;
				return(const_nnf(TFormula,0));
			}
		case 0:
			if(!isoperator(TFormula->get_val()[0]))
			{
				//std::cout <<"\nWe have hit a leaf with flag " << flag << "\n" << std::endl;
				//std::cout <<"\nWe copy the node and return it.\n" << std::endl;
				NTFormula=FTNode::copy(TFormula);
				return(NTFormula);
			}//end of if
			else
			{
				if(TFormula->get_val()[0]=='~')
				{
					//std::cout <<"\nWe hit a ~(NOT) with " << flag << " at an internal node.\n" << std::endl;
					//std::cout <<"\nRecursively Call the function with flag " << 1-flag << "\n" << std::endl;
					return(const_nnf(TFormula->get_next(),1-flag));
				}//end of if	
				else{
					switch(TFormula->get_val()[0])
					{
						case '@':
								//std::cout <<"\nWe hit a @ (AT) with flag " << flag << "\n" << std::endl;

								NTFormula=new FTNode();//is it correct ? constructor ?
								NTFormula->set_val(TFormula->get_val());
								NTFormula->set_type(1);
								NTFormula->set_next(const_nnf(TFormula->get_next(),flag));
//we have pushed ~(NOT) inside the local formula. that is correct
								NTFormula->set_parent(NULL);
								return(NTFormula);

						case '|'://generate a new formula tree node labelled &, attach the nnf of its left and right child respectively and return it.
								//std::cout <<"\nWe hit a | (OR) with flag " << flag << "\n" << std::endl;

								NTFormula=new FTNode();//is it correct ? constructor ?
								NTFormula->set_val("|");
								NTFormula->set_type(2);
								NTFormula->set_left(const_nnf(TFormula->get_left(),0));
								NTFormula->set_right(const_nnf(TFormula->get_right(),0));
								NTFormula->set_parent(NULL);
								return(NTFormula);


						case '&'://generate a new formula tree node labelled |, attach the nnf of its left and right child respectively and return it.
								//std::cout <<"\nWe hit a & (AND) with flag " << flag << "\n" << std::endl;

								NTFormula=new FTNode();//is it correct ? constructor ?
								NTFormula->set_val("&");
								NTFormula->set_type(2);
								NTFormula->set_left(const_nnf(TFormula->get_left(),0));
								NTFormula->set_right(const_nnf(TFormula->get_right(),0));
								NTFormula->set_parent(NULL);
								return(NTFormula);


						case 'X':
								//std::cout <<"\nWe hit a X (NEXT) with flag " << flag << "\n" << std::endl;

								NTFormula=new FTNode();//is it correct ? constructor ?
								NTFormula->set_val(TFormula->get_val());
								NTFormula->set_type(1);
								NTFormula->set_next(const_nnf(TFormula->get_next(),0));
								NTFormula->set_parent(NULL);
								return(NTFormula);

						case 'Y':
								//std::cout <<"\nWe hit a Y (PREVIOUS) with flag " << flag << "\n" << std::endl;

								NTFormula=new FTNode();//is it correct ? constructor ?
								NTFormula->set_val(TFormula->get_val());
								NTFormula->set_type(1);
								NTFormula->set_next(const_nnf(TFormula->get_next(),0));
								NTFormula->set_parent(NULL);
								return(NTFormula);

						case 'F':
								//std::cout <<"\nWe hit a F (DIAMOND) with flag " << flag << "\n" << std::endl;

								NTFormula=new FTNode();//is it correct ? constructor ?
								NTFormula->set_val(TFormula->get_val());
								NTFormula->set_type(1);
								NTFormula->set_next(const_nnf(TFormula->get_next(),0));
								NTFormula->set_parent(NULL);
								return(NTFormula);

						case 'G':
								//std::cout <<"\nWe hit a G (BOX) with flag " << flag << "\n" << std::endl;

								NTFormula=new FTNode();//is it correct ? constructor ?
								NTFormula->set_val(TFormula->get_val());
								NTFormula->set_type(1);
								NTFormula->set_next(const_nnf(TFormula->get_next(),0));
								NTFormula->set_parent(NULL);
								return(NTFormula);

						case 'P':
								//std::cout <<"\nWe hit a P (PAST) with flag " << flag << "\n" << std::endl;

								NTFormula=new FTNode();//is it correct ? constructor ?
								NTFormula->set_val(TFormula->get_val());
								NTFormula->set_type(1);
								NTFormula->set_next(const_nnf(TFormula->get_next(),0));
								NTFormula->set_parent(NULL);
								return(NTFormula);

						case 'Q':
								//std::cout <<"\nWe hit a Q (IFPAST) with flag " << flag << "\n" << std::endl;

								NTFormula=new FTNode();//is it correct ? constructor ?
								NTFormula->set_val(TFormula->get_val());
								NTFormula->set_type(1);
								NTFormula->set_next(const_nnf(TFormula->get_next(),0));
								NTFormula->set_parent(NULL);
								return(NTFormula);

						default:	//std::cout <<"\nError in the formula\n" << std::endl;
								exit(0);
					
					}//end of switch
				}//end of else
			}//end of else and case
		case 1:
			if(!isoperator(TFormula->get_val()[0]))
			{
				//it is an atomic proposition
				//add a NOT at the top and return it.
				//std::cout <<"\nWe have hit a leaf with flag " << flag << "\n" << std::endl;
				//std::cout <<"\nWe copy the node, add a ~(NOT) on top and return it.\n" << std::endl;
				NTFormula=FTNode::copy(TFormula);
				NTFormula=FTNode::negate(NTFormula);
				return(NTFormula);

			}//end of if
			else{
				if(TFormula->get_val()[0]=='~')
				{
					//std::cout <<"\nWe hit a ~(NOT) with " << flag << " at an internal node.\n" << std::endl;
					//std::cout <<"\nRecursively Call the function with flag " << 1-flag << "\n" << std::endl;
					return(const_nnf(TFormula->get_next(),1-flag));
				}	
				else{
					switch(TFormula->get_val()[0])
					{
						case '@':
								//std::cout <<"\nWe hit a @ (AT) with flag " << flag << "\n" << std::endl;

								NTFormula=new FTNode();//is it correct ? constructor ?
								NTFormula->set_val(TFormula->get_val());
								NTFormula->set_type(1);
								NTFormula->set_next(const_nnf(TFormula->get_next(),flag));
//we push ~(NOT) inside the local formula. that is quite correct. 
								NTFormula->set_parent(NULL);
								return(NTFormula);

						case '|'://generate a new formula tree node labelled &, attach the nnf of its left and right child respectively and return it.
								//std::cout <<"\nWe hit a |(OR) with flag " << flag << "\n" << std::endl;

								NTFormula=new FTNode();//is it correct ? constructor ?
								NTFormula->set_val("&");
								NTFormula->set_type(2);
								NTFormula->set_left(const_nnf(TFormula->get_left(),1));
								NTFormula->set_right(const_nnf(TFormula->get_right(),1));
								NTFormula->set_parent(NULL);
								return(NTFormula);


						case '&'://generate a new formula tree node labelled |, attach the nnf of its left and right child respectively and return it.
								//std::cout <<"\nWe hit a & (AND) with flag " << flag << "\n" << std::endl;

								NTFormula=new FTNode();//is it correct ? constructor ?
								NTFormula->set_val("|");
								NTFormula->set_type(2);
								NTFormula->set_left(const_nnf(TFormula->get_left(),1));
								NTFormula->set_right(const_nnf(TFormula->get_right(),1));
								NTFormula->set_parent(NULL);
								return(NTFormula);


						case 'X':
								//std::cout <<"\nWe hit a X (NEXT) with flag " << flag << "\n" << std::endl;

								NTFormula=new FTNode();//is it correct ? constructor ?
								//NTFormula->set_val(TFormula->get_val());
								//NTFormula->get_val()[0]='Y';
                                NTFormula->set_val("Y");
								NTFormula->set_type(1);
								NTFormula->set_next(const_nnf(TFormula->get_next(),1));
								NTFormula->set_parent(NULL);
								return(NTFormula);

						case 'Y':
								//std::cout <<"\nWe hit a Y (PREVIOUS) with flag " << flag << "\n" << std::endl;

								NTFormula=new FTNode();//is it correct ? constructor ?
								//NTFormula->set_val(TFormula->get_val());
								//NTFormula->get_val()[0]='X';
                                NTFormula->set_val("X");
								NTFormula->set_type(1);
								NTFormula->set_next(const_nnf(TFormula->get_next(),1));
								NTFormula->set_parent(NULL);
								return(NTFormula);

						case 'F':
								//std::cout <<"\nWe hit a F (DIAMOND) with flag " << flag << "\n" << std::endl;

								NTFormula=new FTNode();//is it correct ? constructor ?
								//NTFormula->set_val(TFormula->get_val());
								//NTFormula->get_val()[0]='G';
                                NTFormula->set_val("G");
								NTFormula->set_type(1);
								NTFormula->set_next(const_nnf(TFormula->get_next(),1));
								NTFormula->set_parent(NULL);
								return(NTFormula);

						case 'G':
								//std::cout <<"\nWe hit a G (BOX) with flag " << flag << "\n" << std::endl;

								NTFormula=new FTNode();//is it correct ? constructor ?
								//NTFormula->set_val(TFormula->get_val());
								//NTFormula->get_val()[0]='F';
                                NTFormula->set_val("F");
								NTFormula->set_type(1);
								NTFormula->set_next(const_nnf(TFormula->get_next(),1));
								NTFormula->set_parent(NULL);
								return(NTFormula);

						case 'P':
								//std::cout <<"\nWe hit a P (PAST) with flag " << flag << "\n" << std::endl;

								NTFormula=new FTNode();//is it correct ? constructor ?
								//NTFormula->set_val(TFormula->get_val());
								//NTFormula->get_val()[0]='Q';
                                NTFormula->set_val("Q");
								NTFormula->set_type(1);
								NTFormula->set_next(const_nnf(TFormula->get_next(),1));
								NTFormula->set_parent(NULL);
								return(NTFormula);

						case 'Q':
								//std::cout <<"\nWe hit a Q (IFPAST) with flag " << flag << "\n" << std::endl;

								NTFormula=new FTNode();//is it correct ? constructor ?
								//NTFormula->set_val(TFormula->get_val());
								//NTFormula->get_val()[0]='P';
                                NTFormula->set_val("P");
								NTFormula->set_type(1);
								NTFormula->set_next(const_nnf(TFormula->get_next(),1));
								NTFormula->set_parent(NULL);
								return(NTFormula);


						default:	
                                //std::cout <<"\nError in the formula\n" << std::endl;
								exit(0);
					
					}//end of switch
				}//end of else
			}//end of else
		default:
			//std::cout <<"\Error in the flag\n"<<std::endl;
			exit(0);

	}//end of switch

}//end of const_nnf
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
FTree* FTree::negate(FTree* TFormula)
{
    FTNode* neg_root = FTNode::negate(TFormula->root);
    FTree* neg_formula = new FTree(neg_root);
    return neg_formula;
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
FTNode* FTNode::negate(FTNode* TFormula)
{
			FTNode* new_node=NULL;
			FTNode* new_subf=NULL;

			new_node=new FTNode();//is it correct ? constructor ?
			if(new_node==NULL){
				//printf("\nError in memory allocation\nExiting\n");
				exit(0);
			}
			//printf("\nnegating the formula (in prefix form):");
			//display_FTree_pre(TFormula);
			new_subf=copy(TFormula);
			//printf("\nMade a copy of the formula. The copy (in prefix form) is:");
			//display_FTree_pre(new_subf);
			fflush(stdout);

			new_node->set_type(1);
			//new_node.global=0;
			new_node->set_val("~");
			new_node->set_left(NULL);
			new_node->set_right(NULL);
			new_node->set_next(new_subf);
			return(new_node);

}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
FTree* FTree::copy(FTree* TFormula)
{
    FTNode* new_root = FTNode::copy(TFormula->root);
    FTree* new_formula = new FTree(new_root);
    return new_formula;
}
//-------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
FTNode* FTNode::copy(FTNode* TFormula)
{

	 FTNode* new_leaf=NULL;
	 FTNode* new_node=NULL;
	 FTNode* new_subtree=NULL;
	 FTNode* new_l_subtree=NULL;
	 FTNode* new_r_subtree=NULL;

	//printf("\nIn the copy routine");
	if(TFormula==NULL){
		//printf("\nError in the formula tree.\n");
		return(NULL);
	}
			//printf("\nThe formula to copy (in prefix form)");
			//display_FTree_pre(TFormula);	
			fflush(stdout);

	switch(TFormula->get_type()){
		case 2: //std::cout << "\nThe node type is:   " << TFormula->get_type() << std::endl;
			new_l_subtree=copy(TFormula->get_left());
			new_r_subtree=copy(TFormula->get_right());

			new_node=new FTNode();//is it correct ? constructor ?
			if(new_node==NULL){
				//std::cout << "\nError in memory allocation\nExiting\n" << std::endl;
				return 0;
			}
			new_node->set_type(2);
			 
			new_node->set_val(TFormula->get_val());
			new_node->set_left(new_l_subtree);
			new_node->set_right(new_r_subtree);
			new_node->set_next(NULL);
			return(new_node);

		case 1: //std::cout << "\nThe node type is:   " << TFormula->get_type() << std::endl;
			fflush(stdout);
			new_subtree=copy(TFormula->get_next());
			new_node=new FTNode();//is it correct ? constructor ?
			if(new_node==NULL){
				//std::cout << "\nError in memory allocation\nExiting\n" << std::endl;
				return 0;
			}
			new_node->set_type(1);
			 
			new_node->set_val(TFormula->get_val());
			new_node->set_left(NULL);
			new_node->set_right(NULL);
			new_node->set_next(new_subtree);
			return(new_node);

		case 0: //std::cout << "\nThe node type is:   " << TFormula->get_type() << std::endl;
			fflush(stdout);
			new_leaf=new FTNode();//is it correct ? constructor ?
			if(new_leaf==NULL){
				//std::cout << "\nError in memory allocation\nExiting\n" << std::endl;
				return 0;
			}
			new_leaf->set_type(0);
			 
			new_leaf->set_val(TFormula->get_val());
			new_leaf->set_left(NULL);
			new_leaf->set_right(NULL);
			new_leaf->set_next(NULL);

			return(new_leaf);		
			

		default:
			//std::cout << "\nError! Type can either be 0,1 or 2.\n" << std::endl;
			break;

	}
}
//---------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
bool Formula::ftree_convert()
{
	int len=0;
	int i,j;

	std::stack<FTNode*> for_stack;	
	std::stack<FTNode*> op_stack;


	std::string token;

	FTNode* for_node_n=NULL;
	FTNode* for_node_l=NULL;
	FTNode* for_node_r=NULL;
	FTNode* for_node=NULL;

	FTNode* op_node=NULL;
	FTNode* new_op_node=NULL;
	//std::cout <<"\nThe formula is:" << FormulaText << "\n\n\n" << std::endl;
	len=FormulaText.size();
	//std::cout <<"\nLength of the formula is:" << len << "\n\n\n" << std::endl;
//we replace for by while and increment i whenever necessary
//	for(i=0;i<len;i++)
	i=0;
	while(i<len)
	{
		//std::cout <<"\nLooking at the " << i <<" th symbol of the formula:" << FormulaText[i]<< "\n\n\n" << std::endl;
		//std::getchar();
		//looking at FormulaText[i]
		if(isoperator(FormulaText[i])){//we expect operators to be single letter symbols.
//---------------------------------------------------------------------------------------------------------------------------
//the function isoperator takes a symbol as input and 
//returns 1 if it is ~,|,&,= (logical operators) or XY,,F,G,U,V,P,Q, (temporal operators) or '(' and ')'
//else returns 0.
//Essentially, using isoperator function we want to differentiate between variables/propositions/predicates and others.
//---------------------------------------------------------------------------------------------------------------------------
			//std::cout << FormulaText[i] << "is an operator\n" << std::endl;
			switch(op_prcd(FormulaText[i])){
//---------------------------------------------------------------------------------------------------------------------------
//the function op_prcd takes a symbol as input and 
//returns 2 if it is ~,|,&,= (logical operators) or XY,,F,G,U,V,P,Q, (temporal operators) 
//else returns 0 if it is '('.
//else returns 1 if it is ')'
//Essentially, using op_prcd function we want to differentiate between proper operators, '(' and ')'.
//Later, once we have got rid of full parenthesization we can have more granularity in the returned values to handle the different operators.
//-------------------------------------------------------------------------------------------------------------------

				case 0://**(** do nothing

					//std::cout <<"\nWe are looking at the symbol: " << FormulaText[i] << std::endl;
					//std::cout <<"\nIgnore,move to the next symbol.\n" << std::endl;
					break;


				case 1://**)**

					//std::cout <<"\nWe are looking at the symbol: " << FormulaText[i] << std::endl;
					//look at the operator on the top of the operation stack and accordingly construct a tree by taking one or more operands from the operand stack

                    op_node=op_stack.top();op_stack.pop();
					//std::cout <<"\nThe operator is "<< op_node->get_val()[0];
					//std::cout <<"\npop an operator from the op stack\ncheck whether it is unary or binary\n" ;
					switch(op_type(op_node->get_val()[0])){
//---------------------------------------------------------------------------------------------------------------------------
//the function op_type takes an operator as input and returns it type, unary or binary in the present case.
//---------------------------------------------------------------------------------------------------------------------------
						case 1://unary operator
								//std::cout <<"\nif unary,pop a subformula from the formula stack\n" << std::endl;

								for_node_n=for_stack.top();for_stack.pop();

								op_node->set_next(for_node_n);
								op_node->set_type(1);//for unary operator
								//std::cout <<"\npush the new op node onto the stack.......\n" << std::endl;
								
								//std::cout <<"\nConstructed a larger subformula\n" << std::endl;
								
								//std::cout <<"\npush the new subformula node onto the formula stack............\n" << std::endl;

                                for_stack.push(op_node);


								break;


						case 2://binary operator
								//std::cout <<"\nif binary pop two subformulas from the formula stack\n" << std::endl;

								for_node_r=for_stack.top();for_stack.pop();

								for_node_l=for_stack.top();for_stack.pop();

								op_node->set_left(for_node_l);
								op_node->set_right(for_node_r);
								op_node->set_type(2);//it is a binary operator
								
								//std::cout <<"\npush the new subformula node onto the formula stack.............\n" << std::endl;

								for_stack.push(op_node);
								
								break;
						default:
								//std::cout<< "\nOperator can either be unary or binary.\n" << std::endl;
								return(0);


					}//end of switch(op_type(op_node->get_val()[0]))
					break;
					
					

				case 2:	//if FormulaText[i] is not left or right bracket but an operator (~,&,|,F,G,X,U etc.) then construct an internal node and store in the stack.
					//std::cout <<"\nLooking at a real operator:" << FormulaText[i] << "\n\n\n" << std::endl;
					new_op_node=new FTNode();//is it correct ? constructor ?
					if(new_op_node==NULL){
						//std::cout <<"\nMemory Allocation Error\n" << std::endl;
						return(0);
					}
					//std::cout <<"\nConstructed a tree node to store " << FormulaText[i] << std::endl;
					//std::cout.flush();

					token.push_back(FormulaText[i++]);
					
//If we assume operator to be a single symbol then the following is not needed. We can simply have new_op_node->set_val(FormulaText[i]			
					while(FormulaText[i]!=')' && FormulaText[i]!='('){

						token.push_back(FormulaText[i++]);
						//std::cout <<"\nThe next character read is  " << FormulaText[i] << std::endl;
						//getchar();
						//std::cout.flush();
					}
					

					new_op_node->set_val(token);//convert c string token to c++ string and assign to new_op_node->get_val()
					token.clear();
					//std::cout <<"\nThe operator we have seen is " << new_op_node->get_val() << std::endl;
					//std::getchar();

					new_op_node->set_type(op_type(new_op_node->get_val()[0]));

					//std::cout <<"\nLabel for internal node is " << new_op_node->get_val() << std::endl;
					//std::getchar();

					//std::cout.flush();
					new_op_node->set_parent(NULL);
					new_op_node->set_right(NULL);
					new_op_node->set_left(NULL);
					new_op_node->set_next(NULL);
					

					op_stack.push(new_op_node);
					//std::cout <<"\npush the new op node onto the stack...............\n" << std::endl;

					continue;
				
				default:
					//std::cout<< "I see an unfamiliar symbol.\nThere is a problem.\nPlease Check.\n" << std::endl;
					return(0);


			}//end of outer switch
		}//end of outer if----operator part.
		else{//FormulaText[i] is not an operator
			//std::cout << "\n\n\n" << FormulaText[i] << "is not an operator" << "\n\n\n" << std::endl;

			token.push_back(FormulaText[i++]);
//We admit only alphanumeric characters in the token.
			while(std::isalpha(FormulaText[i]) || std::isdigit(FormulaText[i]))
				token.push_back(FormulaText[i++]);
//Sanity check before we move ahead.
			switch(FormulaText[i]){
				case ')': 
						//std::cout << "\nOkay! We expect only propositions or variables. \nContinue" << std::endl;
						break;
				case '(':
						//std::cout << "The atomic formula seems to be contain a predicate. \nNot allowed in LTL." << std::endl;
						return 0;
				default:
						//std::cout << "\nEncountered an unwanted symbol in the input formula. \nExiting......" << std::endl;
						return 0;
			}

			//std::cout <<"\nThe token for atomic formula is: " << token << std::endl;
			//std::getchar();
			for_node=new FTNode();//is it correct ? constructor ?
			if(for_node==NULL){
				//std::cout <<"\nMemory Allocation Error\n" << std::endl;
				return 0;
			}
					
			for_node->set_type(0);
 
			for_node->set_val(token);
			token.clear();
			//std::cout <<"\nThe label for leaf node is:" << for_node->get_val() << std::endl;
			//getchar();
			for_node->set_parent(NULL);
			for_node->set_right(NULL);
			for_node->set_left(NULL);
			for_node->set_next(NULL);
					
			for_stack.push(for_node);
			//std::cout <<"\npush the new leaf node onto the stack...............\n" << std::endl;
			 
			i++;//consume one ')' attached to the atomic formula and move to the next symbol
			continue;
		}//end of else-----end of non-operator part.
	i++;//move to the next symbol and go to the beginning of the loop
	}//end of while

    if(!for_stack.empty()){
	    FormulaTree= new FTree(for_stack.top());for_stack.pop();
        return(1);
    }
	return(0);
}//end of FTree_convert
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

int Formula::op_type(char symbol)
{
 switch (symbol)
 {
//-------------------------UNARY------------------------------------- 
 case '~'://NOT
 case 'X'://NEXT
 case 'Y'://ONE-STEP PAST
 case 'F'://DIAMOND
 case 'G'://BOX
 case 'P'://DIAMOND MINUS
 case 'Q'://BOX MINUS
	return 1;
//-------------------------BINARY-------------------------------------
 case '|'://OR
 case '&'://AND
 case '='://EQUALS
 case '%'://IMPLIES
 case '^'://XOR
 case '$'://NAND
 case '#'://NOR
 case 'U'://UNTIL
 case 'V'://RELEASE
	return 2;
 }
}
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
int Formula::op_prcd(char symbol)
{
 switch (symbol)
 {
//-------------------------UNARY-------------------------------------
 case '~'://NOT
 case 'X'://NEXT
 case 'Y'://ONE-STEP PAST
 case 'F'://DIAMOND
 case 'G'://BOX
 case 'P'://DIAMOND MINUS
 case 'Q'://BOX MINUS
//-------------------------BINARY-------------------------------------
 case '|'://OR
 case '&'://AND
 case '='://EQUALS
 case '%'://IMPLIES
 case '^'://XOR
 case '$'://NAND
 case '#'://NOR
 case 'U'://UNTIL
 case 'V'://RELEASE
	return 2;
//---------------------------------------------------------------------
 case '(':
	return 0;
//---------------------------------------------------------------------
 case ')':
 	return 1;
//---------------------------------------------------------------------
 }
 
}
//--------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
int Formula::isoperator(char symbol)
{
 	switch (symbol)
 	{

 		case '~'://NOT
 		case '|'://OR
 		case '&'://AND
 		case '='://EQUALS
 		case '%'://IMPLIES
 		case '^'://XOR
 		case '$'://NAND
 		case '#'://NOR
 		case 'X':
 		case 'Y':
 		case 'U':
 		case 'V':
 		case 'F':
 		case 'G':
 		case 'P':
	 	case 'Q':


 		case '(':
 		case ')':
 			return 1;

 		default:
 			return 0;
 	}
 
}
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
