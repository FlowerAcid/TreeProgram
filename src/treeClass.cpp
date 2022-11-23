#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "string.h"
#include "stdio.h"
#include "treeClass.hpp"

using namespace std;
using namespace pugi;

// base element
baseElement::baseElement() { parent = nullptr; }

baseElement::~baseElement() 
{ 
	for (unsigned int i = 0; i < childrens.size(); i++)
		delete childrens.at(i);
}

char *baseElement::getName() { return name; }
int baseElement::getType() { return type; }

void baseElement::printElementStructure(int count = 1)
{
	int size = 50;
	char *tmp = new char[size];
	this->getDataStr(tmp, size);
	cout << "data: " << tmp << endl;
	
	cout << "|";
	
	for (unsigned int i = 0; i < this->childrens.size(); i++)
	{
		cout << setw(5 * (count - 1)); if (count > 1) cout << "|";
		for (int i = 0; i <  4; i++) cout << "-";
		cout << "child num: " << i + 1 << setw(10);
		this->childrens.at(i)->printElementStructure(++count);
		count--;
	}

	delete[] tmp;
}

///////////////////////////// childrens add-functions
	
baseElement* baseElement::addChild(elementTypes type, const char *title)
{
	char tmp[TREE_ELEMENT_NAME_MAX_LENGTH];
	strncpy(tmp, title, TREE_ELEMENT_NAME_MAX_LENGTH);
	
	baseElement *x;
	switch(type)
	{
		case elementTypes::CHAR:
		{
			x = new charArrayElement(tmp);
			break;
		}
		case elementTypes::INT:
		{
			x = new intElement(tmp);
			break;
		}
		case elementTypes::FLOAT:
		{
			x = new floatElement(tmp);
			break;
		}
		default: cout << "not correct type" << endl;
	}
	
	x->parent = this;
	childrens.push_back(x);

	return x;
}

// other elements

//////////////////////////////////////////////////////  CHAR

charArrayElement::charArrayElement(char *title)
{
	strncpy(name, title, TREE_ELEMENT_NAME_MAX_LENGTH);
	type = CHAR;
}

int charArrayElement::getDataStr(char *arr, int size)
{
	strncpy(arr, data, size);
	if (size < strlen(data))
		return OVERLOADING_CHAR_ARRAY;
	else 
		return NO_ERRORS;
		
}

void charArrayElement::set(const char *val) { strncpy(data, val, TREE_CHAR_ARRAY_MAX_LENGTH); }

//////////////////////////////////////////////////////  INT

intElement::intElement(char *title)
{
	strncpy(name, title, TREE_ELEMENT_NAME_MAX_LENGTH);
	type = INT;
}

int intElement::getDataStr(char *arr, int size)
{
	if (size < snprintf(arr, size,"%d", data))
		return OVERLOADING_CHAR_ARRAY;
	else 
		return NO_ERRORS;
}

void intElement::set(int val) { data = val; }

//////////////////////////////////////////////////////  FLOAT

floatElement::floatElement(char *title)
{
	strncpy(name, title, TREE_ELEMENT_NAME_MAX_LENGTH);
	type = FLOAT;
}

int floatElement::getDataStr(char *arr, int size)
{
	if (size < snprintf(arr, size,"%.2f", data))
		return OVERLOADING_CHAR_ARRAY;
	else
		return NO_ERRORS;
}

void floatElement::set(float val) { data = val; }

///////////////// tree

treeClass::treeClass() {root = nullptr;}
treeClass::~treeClass() { delete root; }

void treeClass::makeTree(elementTypes type, const char *title)
{
	char tmp[TREE_ELEMENT_NAME_MAX_LENGTH];
	strncpy(tmp, title, TREE_ELEMENT_NAME_MAX_LENGTH);
	
	if (!root)
	{
		switch(type)
		{
			case elementTypes::CHAR:
			{
				root = new charArrayElement(tmp);
				break;
			}
			case elementTypes::INT:
			{
				root = new intElement(tmp);
				break;
			}
			case elementTypes::FLOAT:
			{
				root = new floatElement(tmp);
				break;
			}
			default:
				cout << "not correct type" << endl;
		}
	}
	else cout << "You already have a tree!" << endl;
}

void treeClass::printTreeStructure()
{
	if(root)
	{
		root->printElementStructure();
	}
	else cout << "you did not make a tree!" << endl;
}

/////////////////////////////////// Save tree functions

void treeClass::saveNode(xml_node node, baseElement *element)
{
	char tmp[TREE_CHAR_ARRAY_MAX_LENGTH];
	
	element->getDataStr(tmp, TREE_CHAR_ARRAY_MAX_LENGTH);
	
	switch (element->getType())
	{
		case elementTypes::CHAR: {node.append_attribute("type") = "CHAR"; break; }
		case elementTypes::INT: {node.append_attribute("type") = "INT"; break; }
		case elementTypes::FLOAT: {node.append_attribute("type") = "FLOAT"; break; }
		default: cout << "not correct type" << endl;
	}
	node.append_attribute("data") = tmp;

	xml_node nodeTmp;
	for (unsigned int i = 0; i < element->childrens.size(); i++)
	{
		nodeTmp = node.append_child(element->childrens.at(i)->getName());
		saveNode(nodeTmp, element->childrens.at(i));
	}
}

void treeClass::saveTree()
{
	if (root)
	{
		xml_document doc;
		xml_node dRoot = doc.append_child(root->getName());
		saveNode(dRoot, root);
		doc.save_file("tree.xml");
		cout << "your tree was saved!" << endl;
	}
	else cout << "you did not have a tree!" << endl;
}

/////////////////////////////////// Load tree functions

int treeClass::loadNode(xml_node node, baseElement *element)
{
	int typeErrorCount = 0;
	
	switch (element->getType())
	{
		case elementTypes::CHAR: 
		{
			charArrayElement *charElelementPtr = static_cast <charArrayElement*> (element);
			char tmp[TREE_CHAR_ARRAY_MAX_LENGTH];
			strncpy(tmp, node.first_attribute().next_attribute().value(), TREE_CHAR_ARRAY_MAX_LENGTH);
			charElelementPtr->set(tmp);
			break; 
		}
		case elementTypes::INT: 
		{
			intElement *intElelementPtr = static_cast <intElement*> (element);
			int i;
			sscanf(node.first_attribute().next_attribute().value(), "%d", &i);
			intElelementPtr->set(i);
			break;
		}
		case elementTypes::FLOAT: 
		{
			floatElement *floatElementPtr = static_cast <floatElement*> (element);
			float i;
			sscanf(node.first_attribute().next_attribute().value(), "%f", &i);
			floatElementPtr->set(i);
			break;
		}
	}
	
	baseElement* elementTmp;
	xml_node nodeTmp = node.first_child(); 
	while (nodeTmp)
	{
		elementTypes type;
		if (!strcmp(nodeTmp.first_attribute().value(), "CHAR")) type = CHAR;
		else if (!strcmp(nodeTmp.first_attribute().value(), "INT")) type = INT;
		else if (!strcmp(nodeTmp.first_attribute().value(), "FLOAT")) type = FLOAT;
		else 
		{ 
			typeErrorCount++;
			nodeTmp = nodeTmp.next_sibling();
			continue; 
		}
		
		elementTmp = element->addChild(type, nodeTmp.name());
		typeErrorCount += loadNode(nodeTmp, elementTmp);
		nodeTmp = nodeTmp.next_sibling();
	}
	
	return typeErrorCount;
}

int treeClass::loadTree(char* path)
{
	xml_document doc;
	xml_parse_result result = doc.load_file(path);
		
	cout << "load xml result: " << result.description() << endl;
	if (!result.status && doc.child("root")) 
	{
		elementTypes type;
		if (!strcmp(doc.child("root").first_attribute().value(), "CHAR")) type = CHAR;
		else if (!strcmp(doc.child("root").first_attribute().value(), "INT")) type = INT;
		else if (!strcmp(doc.child("root").first_attribute().value(), "FLOAT")) type = FLOAT;
		else 
		{
			return NOT_CORRECT_TYPE_OF_ROOT;
		}
		makeTree(type, doc.child("root").name());
		cout << "not correct elementTypes of nodes " << loadNode(doc.last_child(), root) << "x" << endl;
	}
	else 
	{ 
		cout << result.status << endl;
		return DID_NOT_FOUND_TREES_ROOT;
	}
	return NO_ERRORS;
}
