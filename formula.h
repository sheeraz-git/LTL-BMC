#include <iostream>
#include <cctype>
#include <string>
#include <stack>
#include <vector>
#include <z3++.h>

//-------------------------------------------------------------------------------------------
//------The class FTNode defines the Formula Tree node, and in effect the Formula Tree.------
//-------------------------------------------------------------------------------------------
class FTNode{
  private:
//  public:
	unsigned int type:4;//type can either be 0 or 1 or 2
	std::string val;//if the tree node is a leaf (==1) val field contains atomic formula of the kind [a,b,..z][0,1,..9]* 
    //else it contains an operator from {~,|,&,X,Y,U,R,P,Q,F,G}
	FTNode* parent;
	FTNode* left;
	FTNode* right;
	FTNode* next;
  public:
    FTNode(){this->parent=NULL;this->left=NULL;this->right=NULL;this->next=NULL;};//constructor
    ~FTNode(){};//destructor
//copy and negate methods are static as it suffices to have only one copy of each.
//these methods work on the FTNode which is passed on to them as parameter.
//they do not need to use anything from object instance, so they are made part of the class.
 	static FTNode* copy(FTNode*);//Makes a copy of the formula tree with this node as root.
	static FTNode* negate(FTNode*);//Negates the formula tree with this node as root.

    unsigned int get_type()  {return this->type;};//a constant function does not change the state of object
    std::string get_val()  {return this->val;};//a constant function does not change the state of object

    void set_type(unsigned int t){this->type = t;};
    void set_val(std::string v){this->val = v;};

    void set_parent(FTNode* p){this->parent = p;};
    void set_left(FTNode* l){this->left = l;};
    void set_right(FTNode* r){this->right = r;};
    void set_next(FTNode* n){this->next = n;};

    FTNode*  get_parent()  {return this->parent;};//a constant function does not change the state of object
    FTNode*  get_left()  {return this->left;};//a constant function does not change the state of object
    FTNode*  get_right()  {return this->right;};//a constant function does not change the state of object
    FTNode*  get_next()  {return this->next;};//a constant function does not change the state of object
};
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
class FTree{
  private:
//  public:
    FTNode* root;//the root of the formula tree corresponding to the formula
  public:
    FTree(FTNode* node){this->root=node;};//Constructor
    ~FTree(){};//destructor  
//copy and negate methods are static as it suffices to have only one copy of each.
//these methods work on the FTree which is passed on to them as parameter.
//they do not need to use anything from object instance, so they are made part of the class.
 	static FTree* copy(FTree*);//copies the formula
	static FTree* negate(FTree*);//negates the formula

	void display_ftree()  {display_ft(root);};
	void display_ftree_pre()  {display_ft_pre(root);};

    void set_root(FTNode* r){this->root = r;};
    FTNode* get_root()  {return this->root;};//a constant function does not change the state of object

//  protected:
//following display methods are static as it suffices to have only one copy of each.
//these methods work on the FTNode which is passed on to them as parameter.
//they do not need to use anything from object instance, so they are made part of the class.
	static void display_ft(FTNode*);
	static void display_ft_pre(FTNode*);    

};
//--------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
class Formula{
 private:
	std::string FormulaText;
	FTree* FormulaTree;
    FTree* NNFormula;
 public:
	Formula(){FormulaTree=NULL;NNFormula=NULL;};//Constructor
	Formula(std::string InExp){FormulaText=InExp;FormulaTree=NULL;NNFormula=NULL;};//Parameterized Constructor
	~Formula(){};//Destructor
		
	bool ftree_convert();
    bool construct_nnf();

	void display_formula()  {FormulaTree->display_ftree();};
	void display_formula_pre()  {FormulaTree->display_ftree_pre();};

    std::string get_ftext()  {return FormulaText;};//a constant function does not change the state of object
    FTree* get_ftree()  {return FormulaTree;};//a constant function does not change the state of object
    FTree* get_nnftree()  {return NNFormula;};//a constant function does not change the state of object

    FTNode* const_nnf(FTNode*, int);
 protected:

	int isoperator(char);
	int op_prcd(char);
	int op_type(char);

};
