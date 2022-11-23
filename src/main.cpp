#include <iostream>
#include <memory>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "treeClass.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		treeClass tree = treeClass();
		tree.loadTree(argv[1]);
		tree.printTreeStructure();
	}
	else
	{
	
		treeClass tree = treeClass();
		tree.makeTree(CHAR, "root");
		charArrayElement *cast = static_cast <charArrayElement*> (tree.root);
		cast->set("this is the root of tree");
		
		baseElement *tmp;
		for (int i = 0; i < 4; i++)
		{
			tmp = tree.root->addChild(INT, "element");
			intElement *cast = static_cast <intElement*> (tmp);
			cast->set(123);
		}
		
		for (int i = 0; i < 4; i++)
		{
			tmp = tree.root->childrens.at(0)->addChild(FLOAT, "element");
			floatElement *cast = static_cast <floatElement*> (tmp);
			cast->set(3.14);
		}
		
		tree.printTreeStructure();
		tree.saveTree();
		
	}
	
	return 0;
}
