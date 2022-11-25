#include <iostream>
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
		treeParceResult result = tree.loadTree(argv[1]);
		cout << "load tree result: " << result.errorCode << endl;
		cout << "count of not loaded elements: " << result.countOfNotLoadedElements << endl;
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
			cast->set(1 + i);
		}
		
		for (int i = 0; i < 4; i++)
		{
			tmp = tree.root->childrens.at(0)->addChild(FLOAT, "element");
			floatElement *cast = static_cast <floatElement*> (tmp);
			cast->set(3.14 + i);

			for (int j = 0; j < 4; j++)
			{
			tmp->addChild(CHAR, "element");
			charArrayElement *cast2 = static_cast <charArrayElement*> (tmp->childrens.at(j));
			cast2->set("this is charArrayElement" + (i + j));
			}
		}

		tree.printTreeStructure();
		tree.saveTree();
		
	}
	
	return 0;
}
