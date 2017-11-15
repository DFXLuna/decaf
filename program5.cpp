// Matt Grant
// Program 5
// Nov 9 2017

#include"tables.h"
#include"node.h"
#include<FlexLexer.h>
#include"program5.tab.h"
#include<vector>
using std::vector;


yyFlexLexer scanner;
vector<Node*> tree;

int main(int argc, char* argv[]){
    yyparse();

    TableManager t;
    bool typeCheck = true;

    for( unsigned int i = 0; i < tree.size(); i++ ){
        if( tree[i]->registerType( &t ) == false ){
            typeCheck = false;
        }
    }

    if(typeCheck){
        for( unsigned int i = 0; i < tree.size(); i++ ){
            tree[i]->populateTables( &t );
        }
    }

    t.dump();

    return 0;
}
