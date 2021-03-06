#ifndef NODE_H
#define NODE_H

#include<string>
using std::string;
#include<iostream>
using std::cout;
using std::endl;
#include<vector>
using std::vector;
#include"tables.h"

class Node {
public:
    Node( Node* l = 0, Node* r = 0 );
    virtual ~Node();
    void setLeft( Node* );
    void setRight( Node* );
    void virtual print();
    // This is about to get very messy
    bool virtual registerType( TableManager* tm );
    void virtual populateTables( TableManager* tm );

    // If node is an idNode, populates result and returns true
    bool virtual getID( string& result );
    //if node is a TypeId node, populates type, id and returns true
    bool virtual getTypeID( string& type, string& ID );
    // gather parameter types from a plist node
    bool virtual gatherParams( vector<string>& types, vector<string>& ids );

    // count the number of brackets in a multibracket set
    int virtual gatherBrackets();

    // Puts all of a function calls arguments into result
    // returns false on any error or miscall
    bool virtual gatherArgs( TableManager* tm, vector<TypeDecl*>& result );

    // Verifies constructors have the same name as containing class
    bool virtual checkConstructorNames();

    bool virtual passConstructorNames( string name );

    bool virtual typeCheck( TableManager* tm );

    // Get types from name nodes
    bool virtual tryGetType( TableManager* tm, TypeDecl*& result );
    bool virtual tryGetType( TableManager* tm, vector<TypeDecl*> args, MethDecl*& result );

    // Allows while nodes to check and see if a node is of a given type
    bool virtual isBlock();
    bool virtual isThis();

    // Allows a node to check if its children contain a return statement as
    // well as verifying the type
    bool virtual verifyReturn( TableManager* tm, string returnType );
protected:
    Node* left;
    Node* right;
};

class IdNode : public Node {
public:
    IdNode( string v );
    bool getID( string& result );
private:
    string val;
};

class TypeIdNode : public Node {
public:
    TypeIdNode( Node* type = 0, Node* id = 0 );
    bool getTypeID( string& type, string& ID );
    void print();
};

/////////////////////////////////////////
// Class Dec Node
class ClassDecNode : public Node {
public:
    ClassDecNode( Node* classbody = 0, Node* id = 0 );
    void print();
    bool registerType( TableManager* tm );
    bool checkConstructorNames();
    void populateTables( TableManager* tm );
    bool typeCheck( TableManager* tm );
};

/////////////////////////////////////////
// Classbody node
class ClassBodyNode : public Node {
public:
    ClassBodyNode( Node* vardecls = 0, Node* condecls = 0,
    Node* methdecls = 0 );
    ~ClassBodyNode();
    bool passConstructorNames( string name );
    void populateTables( TableManager* tm );
    bool typeCheck( TableManager* tm );
    void print();
private:
    Node* methdecls;
};


/////////////////////////////////////////
// grouping nodes. Vardecls, condecls, methdecls
// This could probably be one node
class VarDeclsNode : public Node {
public:
    VarDeclsNode( Node* vardecl = 0 , Node* next = 0 );
    void populateTables( TableManager* tm );
    void print();
};

class ConDeclsNode : public Node {
public:
    ConDeclsNode( Node* condecl = 0 , Node* next = 0 );
    bool passConstructorNames( string name );
    void populateTables( TableManager* tm );
    bool typeCheck( TableManager* tm );
    void print();
};

class MethDeclsNode : public Node {
public:
    MethDeclsNode( Node* methdecl = 0 , Node* next = 0 );
    void populateTables( TableManager* tm );
    bool typeCheck( TableManager* tm );
    void print();
};


/////////////////////////////////////////
// Var Decl Nodes

class VarTypeIdNode : public Node {
public:
    VarTypeIdNode( Node* tid = 0 );
    void populateTables( TableManager* tm ); 
    void print();
};

class IdIdNode : public Node {
public:
    IdIdNode( Node* l = 0, Node* r = 0);
    void populateTables( TableManager* tm );
    void print();
};

class IdMultiIdNode : public Node {
public:
    IdMultiIdNode( Node* type = 0, Node* b = 0, Node* id = 0 );
    ~IdMultiIdNode();
    void populateTables( TableManager* tm );
    void print();
private:
    // One extra pointer needed for id multibrackets id case
    Node* brackets;
};

/////////////////////////////////////////
// Type Nodes

class TypeNode : public Node {
public: 
    TypeNode( Node* t = 0, Node* brackets = 0);
    bool getID( string& result );
    void print();
};

class SimpleTypeNode : public Node {
public:
    SimpleTypeNode();
    bool getID( string& result );
    void print();
};

/////////////////////////////////////////
// Constructor Declaration Node
class ConstructorDecNode : public Node {
public:
    ConstructorDecNode( Node* id = 0, Node* plist = 0, Node* block = 0 );
    ~ConstructorDecNode();
    bool passConstructorNames( string name );
    void populateTables( TableManager* tm );
    bool typeCheck( TableManager* tm );
    void print();
private:
    Node* block;
};

/////////////////////////////////////////
// Method Declaration Node
class MethodDecNode : public Node {
public:
    MethodDecNode( Node* tid = 0, Node* plist = 0, Node* block = 0 );
    ~MethodDecNode();
    void populateTables( TableManager* tm );
    bool typeCheck( TableManager* tm );
    void print();
private:
    Node* block;
};

////

class VoidMethodDecNode : public Node {
public:
    VoidMethodDecNode( Node* id = 0, Node* plist = 0, Node* block = 0 );
    ~VoidMethodDecNode();
    void populateTables( TableManager* tm );
    bool typeCheck( TableManager* tm );
    void print();

private:
    Node* block;
};

////
class IDMethodDecNode : public Node {
public:
    IDMethodDecNode( Node* result = 0, Node* id = 0, 
    Node* plist = 0, Node* block = 0 );
    ~IDMethodDecNode();
    void populateTables( TableManager* tm );
    bool typeCheck( TableManager* tm );
    void print();
private:
    Node* result;
    Node* block;
};

/////////////////////////////////////////
// Parameter List Node
class ParameterListNode : public Node {
public:
    ParameterListNode( Node* param = 0 );
    bool gatherParams( vector<string>& types, vector<string>& ids );
    void print();
};

/////////////////////////////////////////
// Parameter Node
class ParameterNode : public Node {
public:
    ParameterNode( Node* type = 0, Node* id = 0, Node* next = 0 );
    ~ParameterNode();
    bool gatherParams( vector<string>& types, vector<string>& ids );
    void print();
private:
    Node* next;
};

class ParameterIDNode : public Node {
public:
    ParameterIDNode( Node* type = 0, Node* mb = 0, Node* id = 0,
    Node* next = 0 );
    ~ParameterIDNode();
    bool gatherParams( vector<string>& types, vector<string>& ids );
    void print();
private:
    Node* mb;
    Node* next;
};


/////////////////////////////////////////
// Block Nodes
class BlockNode : public Node {
public:
    BlockNode( Node* localvars = 0, Node* stmts = 0 );
    void populateTables( TableManager* tm );
    bool typeCheck( TableManager* tm );
    bool isBlock();
    bool verifyReturn( TableManager* tm , string returnType ); 
    void print();
};

////

class LocalVarsNode : public Node {
public:
    LocalVarsNode( Node* localvar = 0, Node* next = 0 );
    void populateTables( TableManager* tm );
    void print();
};

////

class StatementsNode : public Node {
public:
    StatementsNode( Node* stmt = 0, Node* next = 0);
    bool typeCheck( TableManager* tm );
    bool verifyReturn( TableManager* tm, string returnType );
    void print();
};

/////////////////////////////////////////
// Localvar nodes
class LocalVarDecNode : public Node {
public:
    LocalVarDecNode( Node* type = 0, Node* id = 0 );
    void populateTables( TableManager* tm );
    void print();
};

class LocalVarDecIDNode : public Node {
public:
    LocalVarDecIDNode( Node* type = 0, Node* id = 0, Node* mb = 0 );
    ~LocalVarDecIDNode();
    void populateTables( TableManager* tm );
    void print();
private:
    Node* mb;
};

/////////////////////////////////////////
// Statement nodes
class EmptyStatementNode : public Node {
public:
    EmptyStatementNode();
    bool typeCheck( TableManager* tm );
    bool verifyReturn( TableManager* tm, string returnType );
    void print();
};

class EQStatementNode : public Node {
public:
    EQStatementNode( Node* name = 0, Node* expr = 0 );
    bool typeCheck( TableManager* tm );
    bool verifyReturn( TableManager* tm, string returnType );
    void print();
};

class FuncStatementNode : public Node {
public:
    FuncStatementNode( Node* name = 0, Node* arglist = 0 );
    bool typeCheck( TableManager* tm );
    bool verifyReturn( TableManager* tm, string returnType );
    void print();
};

class PrintStatementNode : public Node {
public:
    PrintStatementNode( Node* arglist = 0 );
    bool typeCheck( TableManager* tm );
    bool verifyReturn( TableManager* tm, string returnType );
    void print();
};

class WhileStatementNode : public Node {
public:
    WhileStatementNode( Node* expr = 0, Node* stmt = 0 );
    bool typeCheck( TableManager* tm );
    bool verifyReturn( TableManager* tm, string returnType );
    void print();
};

class ReturnStatementNode : public Node {
public:
    ReturnStatementNode( Node* optexpr = 0 );
    bool typeCheck( TableManager* tm );
    bool verifyReturn( TableManager* tm, string returnType );
    void print();
};

/////////////////////////////////////////
// Name nodes

class ThisNode : public Node {
public:
    ThisNode();
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool isThis();
    void print();
};

class NameIdNode : public Node {
public:
    NameIdNode( Node* id );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool tryGetType( TableManager* tm, vector<TypeDecl*> args, MethDecl*& result );
    void print();
};

class NameDotIdNode : public Node {
public:
    NameDotIdNode( Node* name = 0, Node* Id = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool tryGetType( TableManager* tm, vector<TypeDecl*> args, MethDecl*& result );
    void print();
};

class NameExprNode : public Node {
public:
    NameExprNode( Node* name = 0, Node* expr = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    void print();
};

/////////////////////////////////////////
// Arglist nodes

class ArgListNode : public Node {
public:
    ArgListNode( Node* args = 0 );
    bool gatherArgs( TableManager* tm, vector<TypeDecl*>& result );
    void print();
};

class ArgNode : public Node {
public:
    ArgNode( Node* expr = 0, Node* next = 0 );
    bool gatherArgs( TableManager* tm, vector<TypeDecl*>& result );
    void print();
};

/////////////////////////////////////////
// Conditional Statement Node

class CondStatementNode : public Node {
public:
    CondStatementNode( Node* expr = 0, Node* stmt = 0, Node* estmt = 0 );
    bool typeCheck( TableManager* tm );
    ~CondStatementNode();
    void print();
private:
    Node* estmt;
};


/////////////////////////////////////////
// Optional Expression Node

class OptExprNode : public Node {
public:
    OptExprNode( Node* expr = 0 );
    bool typeCheck( TableManager* tm );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    void print();
};

/////////////////////////////////////////
// expression nodes

class ExprNameNode : public Node {
public:
    ExprNameNode( Node* name );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

class NumNode : public Node {
public:
    NumNode( int );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
private:
    int val;
};

class NullNode : public Node {
public:
    NullNode();
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

class ReadNode : public Node {
public:
    ReadNode();
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

class NewExprNode : public Node {
public:
    NewExprNode( Node* newexpr = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

class MethodCallNode : public Node {
public:
    MethodCallNode( Node* name = 0, Node* arglist = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

// Sum/Unary Operators
class MinusNode : public Node {
public:
    MinusNode( Node* l = 0, Node* r = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

class SumNode : public Node {
public:
    SumNode( Node* l = 0, Node* r = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

class ORNode : public Node {
public:
    ORNode( Node* l = 0, Node* r = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

/////////////////////////////////////////
// Relation Nodes

class EqNode : public Node {
public:
    EqNode( Node* l = 0, Node* r = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

class NeqNode : public Node {
public:
    NeqNode( Node* l = 0, Node* r = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

class LeqNode : public Node {
public:
    LeqNode( Node* l = 0, Node* r = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

class GeqNode : public Node {
public:
    GeqNode( Node* l = 0, Node* r = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

class LessNode : public Node {
public:
    LessNode( Node* l = 0, Node* r = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

class GreaterNode : public Node {
public:
    GreaterNode( Node* l = 0, Node* r = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};
/////////////////////////////////////////

// Product Nodes
class TimesNode : public Node {
public:
    TimesNode( Node* l = 0, Node* r = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

class DivNode : public Node {
public:
    DivNode( Node* l = 0, Node* r = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

class ModNode : public Node {
public:
    ModNode( Node* l = 0, Node* r = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

class ANDNode : public Node {
public:
    ANDNode( Node* l = 0, Node* r = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};
/////////////////////////////////////////

// Unary Nodes

class UMinusNode : public Node {
public:
    UMinusNode( Node* r = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

class UPlusNode : public Node {
public:
    UPlusNode( Node* r = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

class UExclNode : public Node {
public:
    UExclNode( Node* r = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

/////////////////////////////////////////
// 0 arg nodes?

class ParenNode : public Node {
public:
    ParenNode( Node* l = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

/////////////////////////////////////////
// New Expression nodes

class NewIdArgsNode : public Node {
public:
    NewIdArgsNode( Node* type = 0, Node* arglist = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

class NewIdNode : public Node {
public:
    NewIdNode( Node* type = 0, Node* bracket = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();
};

class NewSimpleNode : public Node {
public:
    NewSimpleNode ( Node* type = 0, Node* bracket = 0 );
    bool tryGetType( TableManager* tm, TypeDecl*& result );
    bool typeCheck( TableManager* tm );
    void print();

};

/////////////////////////////////////////
// Brackets nodes

class BracketSetNode : public Node {
public:
    BracketSetNode( Node* exprb = 0, Node* b = 0 );
    int gatherBrackets();
    void print();
};

class BracketNode : public Node {
public:
    BracketNode( Node* next = 0 );
    int gatherBrackets();
    void print();
};

class BracketExprNode : public Node {
public:
    BracketExprNode( Node* next = 0, Node* expr = 0);
    int gatherBrackets();
    void print();
};

#endif