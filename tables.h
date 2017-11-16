#ifndef TABLES_H
#define TABLES_H

#include<string>
using std::string;
#include<map>
using std::map;
#include<vector>
using std::vector;
#include<iostream>
using std::cout;
using std::endl;

class Table;
class GlobalTypeTable;
class TypeInst;
class TypeDecl;
class MethDecl;

class TableManager{
public:
    TableManager();
    ~TableManager();
    void enterScope( string name );
    void exitScope();

    // These could probably have the same name
    bool addTypeInst( string type, string name );
    bool addMethDecl( string name, vector<string> argTypes, 
                      string returnType );

    // If successful, result points to an entry in the global type table and
    // returns true
    bool tryLookup( string name, TypeDecl*& result );
    
    // If successful, result points to an entry in the type instance tree  and
    // returns true
    bool tryLookup( string name, TypeInst*& result );
    
    // If successful, result points to an entry in the method declaration tree
    // and returns true
    bool tryLookup( string name, MethDecl*& result );

    // These control the global type table
    // Declare a type without a width
    bool forwardEntryGlobalTypeTable( string name );
    // Set width of forward declared entry
    bool resolveForwardGlobalTypeTable( string name, int width );
    void dump();

    // verifies array types in method param lists before adding them to table
    bool verifyTypes( vector<string> types );
    void addTypes( vector<string> types );

    string getCurrentScope();

private:
    bool createGlobalTypeTable();
    GlobalTypeTable* globalTypeTable;
    Table* currTable;
    // A little bit of a hack to allow methods to have void return types
    // without allowing variables to have it
    TypeDecl* voidType;
};

class Table{
public:
    Table( Table* parent, string name );
    ~Table();
    // If successful, result points to an entry in the type instance tree  and
    // returns true
    bool tryLookup( string name, TypeInst*& result );
    // If successful, result points to an entry in the tree of method
    // declarations and returns true
    bool tryLookup( string name, MethDecl*& result );
    // Add variable to typetable
    bool tryAddEntry( string varName, TypeInst type );
    // Add method declaration to type table
    bool tryAddEntry( string methName, vector<TypeDecl*> argTypes,
                      TypeDecl* returnType, bool forwardDecl = true );
    
    Table* getParent();
    string getName();
    void registerChild( Table* child );

    void print( int indent );
private:
    Table* parent;
    map<string, TypeInst> typeTable;
    map<string, MethDecl> methTable;
    // Needed for dumping table and freeing memory
    vector<Table*> children;
    string name;
};

class GlobalTypeTable {
public:
    bool tryAddEntry( string typeName, TypeDecl t );
    bool tryLookup( string typeName, TypeDecl*& result );
    void print();
private:
    map<string, TypeDecl> types;
};

// This covers instances of a type
class TypeInst {
public:
    // For the map, which insists on inserting an empty element
    // then populating it.
    TypeInst();
    TypeInst( string name, TypeDecl* type );
    string getName();
    void print();
private:
    // Var name
    string name;
    // Pointer to entry in typetable
    TypeDecl* type;
};

// This covers the declaration of a type
// Allows implicit forward declaration so that all types will be in type table
// before they are actually processed.
class TypeDecl {
public:
    // for map
    TypeDecl();
    TypeDecl( string name, int width = 0, bool forwardDecl = true );

    string getName();
    int getWidth();
    void resolveForward();
    void setWidth( int );
    bool isForward();
    void print();
private:
    string name;
    int width;
    bool forwardDecl;
};

// This covers the declaration of a method,
// methods get their own separate table, these also allow forward declarations
// for type checking 
class MethDecl {
public:
    // For map
    MethDecl();
    MethDecl( string name, vector<TypeDecl*> argTypes,
              TypeDecl* retType, bool forwardDecl = true );
    bool isForward();
    void resolveForward();
    vector<TypeDecl*> getArgTypes();
    void print();
    bool operator==(const MethDecl& rhs) const;
private:
    string name;
    vector<TypeDecl*> argTypes;
    TypeDecl* retType;
    bool forwardDecl;
};

#endif