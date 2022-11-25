#include "pugixml.hpp"
#include <vector>
#define TREE_CHAR_ARRAY_MAX_LENGTH 50
#define TREE_ELEMENT_NAME_MAX_LENGTH 20

// error codes for baseElement
#define NO_ERRORS 0
#define OVERLOADING_CHAR_ARRAY 1

// error codes for treeClass
#define NO_ERRORS 0
#define NOT_CORRECT_TYPE_OF_ROOT 1
#define DID_NOT_FOUND_TREES_ROOT 2

struct treeParceResult;

//////////////////////////////////////////// base element

enum elementTypes { CHAR, INT, FLOAT };

class baseElement
{
protected:
	char name[TREE_ELEMENT_NAME_MAX_LENGTH];
	elementTypes type;
public:
	baseElement *parent;
	std::vector <baseElement*> childrens;
	
	baseElement();
	virtual ~baseElement();
	 
	char *getName();
	int getType();
	void printElementStructure(int count);
	
	baseElement *addChild(elementTypes type, const char *title);
	
	virtual int getDataStr(char *arr, int size) = 0;
};

//////////////////////////////////////////// tree

class treeClass
{
private:
	void saveNode(pugi::xml_node node, baseElement *element);
	int loadNode(pugi::xml_node node, baseElement *element);
public:
	baseElement* root;
	
	treeClass();
	~treeClass();
	
	void makeTree(elementTypes type, const char *name);
	void printTreeStructure();
	void saveTree();
	treeParceResult loadTree(char *path);
};

struct treeParceResult
{
	int errorCode = 0;
	int countOfNotLoadedElements;
};

//////////////////////////////////////////// other elements

class charArrayElement: public baseElement
{
	char data[TREE_CHAR_ARRAY_MAX_LENGTH];
public:
	charArrayElement(char *title);
	int getDataStr(char *arr, int size) override;
	void set(const char *val);
};

class intElement: public baseElement
{
	int data;
public:
	intElement(char *title);
	int getDataStr(char *arr, int size) override;
	void set(int val);
};

class floatElement: public baseElement
{
	float data;
public:
	floatElement(char *title);
	int getDataStr(char *arr, int size) override;
	void set(float val);
};
