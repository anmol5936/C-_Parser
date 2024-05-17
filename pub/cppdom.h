
// Header Guard
// The header guard prevents multiple inclusions of this header file, which can cause errors. 

#ifndef __CPPPARSER_CPPDOM_H__ 

//#ifndef prepocessor directive that stands for if not defined
//if the file is not defined, then define it
//if the file is defined, then do not define it
//this is a way to prevent multiple inclusions of the same file
// It checks whether the symbol following it has been previously defined. If it has not, the preprocessor includes 
// the code that follows untilit encounters an #endif directive.

//__CPPPARSER_CPPDOM_H__ is the symbol which is checked

//#define __CPPPARSER_CPPDOM_H__: If the symbol __CPPPARSER_CPPDOM_H__ has not been previously defined, this line defines it. This tells the preprocessor that the header file 
//has been included, and it should not include it again.

#define __CPPPARSER_CPPDOM_H__

#include "const.h"


#include <string>
#include <vector>
#include <list>
#include <xutility> //provides access to utility function and classes

struct CppCompound;

struct CppObj{

enum Type {
		kUnknown			= 0x0000,
		kBlankLine,			// Blank line containing nothing other than may be whitespace.
		kVarType,			// Just the type of variable.
		kVar,				// A variable declaration.
		kVarList,			// List of variables declared as comma separated identifiers.
		kHashIf,			// #if, #ifdef, #ifndef, #else, #elif.
		kHashInclude,		// #include
		kHashDefine,		// #define
		kHashUndef,			// #undef
		kHashPragma,		// #pragma
		kUnRecogPrePro,		// Any unrecognized pre-processor.
		kDocComment,
		kTypedef,
		kEnum,
		kCompound,			// file, namespace, class, struct, union, block.
		kFwdClsDecl,		// Forward declaration of compound type.
		kFunction,
		kConstructor,
		kDestructor,
		kFunctionPtr,		// Function proc declaration using typedef. e.g. typedef void (*fp) (void);
		kExpression,		// A C++ expression
		kFuncCall,			// A function call expression
		kBlob,				// Some unparsed/unrecognized part of C++ source code.
	};

    Type			objType_;
	CppCompound*	owner_;
	CppObjProtLevel	prot_;	///< All objects do not need this but for simplicity we will have this in all objects.

// objType_: This member variable stores the type of the object.
// owner_: This member variable is a pointer to a CppCompound object, representing the owner of this object.
// prot_: This member variable stores the protection level (CppObjProtLevel) of the object.


	CppObj(Type type, CppObjProtLevel prot)
	: objType_	(type)
	, owner_	(NULL)
	, prot_		(prot)
	{
	}

// type: Specifies the type of the object.
// prot: Specifies the protection level of the object.
// Inside the constructor:

	virtual ~CppObj() = 0 {}

};

/**
 * A blank line in a C/C++ program.
 */
struct CppBlankLine : public CppObj
{
	int numLines_;

	CppBlankLine()
		: CppObj(kBlankLine, kUnknownProt)
		, numLines_(1)
	{
	}
};

// struct CppBlankLine : public CppObj: This line declares a structure named CppBlankLine that inherits from the CppObj structure. Inheritance allows CppBlankLine to inherit properties and behaviors from CppObj.
// int numLines_;: This line declares an integer variable numLines_, which represents the number of consecutive blank lines.
// CppBlankLine(): This is the constructor of the CppBlankLine structure. Constructors are special member functions that initialize objects of a class. This constructor initializes a CppBlankLine object.
// : CppObj(kBlankLine, kUnknownProt): This is the constructor initializer list. It initializes the base class CppObj with the parameters kBlankLine and kUnknownProt. This indicates that the object being created is of type kBlankLine, representing a blank line, and its protection level is unknown (kUnknownProt).
// , numLines_(1): This initializes the member variable numLines_ to 1, indicating that by default, there is one consecutive blank line.

typedef std::list<CppObj*> CppObjArray; 

// typedef: This keyword is used in C++ to create an alias for an existing data type. 
// It allows programmers to define a new name for a data type that is already defined.

struct CppDefine : public CppObj
{
	enum DefType {
		kRename,
		kConstNumDef,
		kConstStrDef,
		kConstCharDef,
		kComplexMacro,
	};
	DefType		defType_;
	std::string	name_;
	std::string	defn_; ///< This will contain everything after name.

	CppDefine(std::string name, std::string defn = std::string())
		: CppObj	(kHashDefine, kUnknownProt)
		, name_		(std::move(name))
		, defn_		(std::move(defn))
	{
	}
};

// Overall, this structure is designed to represent various types of definitions encountered in C++ code, 
//such as renaming, constant number, string, or character definitions, as well as complex macro definitions.

// kRename: Used for renaming.
// kConstNumDef: Represents a constant number definition.
// kConstStrDef: Represents a constant string definition.
// kConstCharDef: Represents a constant character definition.
// kComplexMacro: Indicates a complex macro definition.
// Following the enumeration, there are member variables:

// defType_: This variable holds the type of definition.
// name_: A string that stores the name of the definition.
// defn_: Another string that contains the definition itself, which includes everything after the name.

struct CppUndef : public CppObj
{
	std::string	name_;

	CppUndef(std::string name)
		: CppObj	(kHashUndef, kUnknownProt)
		, name_		(std::move(name))
	{
	}
};

struct CppInclude : public CppObj
{
	std::string	name_;

	CppInclude(std::string name)
		: CppObj	(kHashInclude, kUnknownProt)
		, name_		(std::move(name)) //The std::move function is used here to efficiently transfer the ownership of the name string to the name_ member variable.
	{
	}
};

//So, name_ essentially steals the guts of name, leaving name in a valid but unspecified state 
//(usually empty), which is okay because the intention here is to transfer ownership. This way, we avoid unnecessary copying of potentially large strings, making our code more efficient.

//The CppUndef structure represents an 'undefine' directive in C++, which is typically used to remove a previously defined macro or identifier.

struct CppHashIf : public CppObj
{
	enum CondType {
		kIf, kElse, kIfDef, kIfNDef, kElIf, kEndIf
	};

	CondType	condType_;
	std::string	cond_;

	CppHashIf(CondType condType, std::string cond)
		: CppObj	(kHashIf, kUnknownProt)
		, condType_	(condType)
		, cond_		(std::move(cond))
	{
	}

	CppHashIf(CondType condType)
		: CppObj	(kHashIf, kUnknownProt)
		, condType_	(condType)
	{
	}
};

//Overall, this struct represents different types of preprocessor conditional directives and their associated conditions.

struct CppPragma : public CppObj
{
	std::string defn_;

	CppPragma(std::string defn)
		: CppObj(CppObj::kHashPragma, kUnknownProt)
		, defn_(std::move(defn))
	{
	}
};

// Sure, think of CppPragma as a blueprint for handling #pragma directives in C++. 
// defn_: Imagine this as a container where we store whatever comes after #pragma in the code.
// CppPragma(std::string defn): This is like a special function that gets called when we want
//  to create a CppPragma object. It's like an instruction manual for building a CppPragma.

struct CppUnRecogPrePro : public CppObj
{
	std::string name_;
	std::string	defn_;

	CppUnRecogPrePro(std::string name, std::string defn)
		: CppObj(CppObj::kUnRecogPrePro, kUnknownProt)
		, name_	(std::move(name))
		, defn_	(std::move(defn))
	{
	}
};

// struct CppUnRecogPrePro : public CppObj: This line defines a new struct CppUnRecogPrePro that inherits 
// from CppObj. This means CppUnRecogPrePro will have all the properties and methods of CppObj, plus any 
// additional ones defined within CppUnRecogPrePro.

// Member Variables:
// std::string name_;: This is a string variable to store the name of the unrecognized preprocessor directive.
// std::string defn_;: This is another string variable to store the definition or the rest of the unrecognized preprocessor directive after its name.

// defn_ (std::move(defn)): Similarly, this initializes the defn_ member variable using std::move(defn) to efficiently transfer the value of defn.
struct CppExpr;
struct CppVarType : public CppObj
{
	std::string		baseType_; // This is the basic data type of var e.g. for 'const int*& pi' base-type is int.
	unsigned int	typeAttr_; // Attribute associated with type, e.g. const int* x.
	unsigned short  ptrLevel_; // Pointer level. e.g. int** ppi has pointer level of 2.
	CppRefType		refType_;
	CppExpr*		arraySize_; // Should be NULL for non-array vars

// Base Type (baseType_): Stores the fundamental data type of the variable, like int, float, etc.
// Type Attributes (typeAttr_): Stores additional attributes of the type, like const, static, etc.
// Pointer Level (ptrLevel_): Indicates how many levels of pointers the variable has (e.g., int** has a pointer level of 2).
// Reference Type (refType_): Indicates if the variable is a reference, an r-value reference, or not a reference at all.
// Array Size (arraySize_): Points to the size of the array if the variable is an array; otherwise, it is NULL.


	CppVarType(CppObjProtLevel prot, std::string baseType, unsigned int typeAttr, unsigned short ptrLevel, CppRefType refType)
		: CppObj(CppObj::kVarType, prot)
		, baseType_(std::move(baseType))
		, typeAttr_(typeAttr)
		, ptrLevel_(ptrLevel)
		, refType_(refType)
		, arraySize_(NULL)
	{
	}

	CppVarType(std::string baseType, unsigned int typeAttr = 0, unsigned short ptrLevel = 0, CppRefType refType = kNoRef)
		: CppObj(CppObj::kVarType, kUnknownProt)
		, baseType_(std::move(baseType))
		, typeAttr_(typeAttr)
		, ptrLevel_(ptrLevel)
		, refType_(refType)
		, arraySize_(NULL)
	{
	}

	bool isVoid() const
	{
		if(typeAttr_ != 0 || ptrLevel_ != 0 || refType_ != kNoRef)
			return false;
		return (baseType_.compare("void") == 0);
	}

    // isVoid(): Checks if the variable type is void. It returns true if baseType_ is "void" and there are no type attributes, pointer levels, or reference types associated with the variable.

protected:
	CppVarType(CppObj::Type type, CppObjProtLevel prot, std::string baseType, unsigned int typeAttr, unsigned short ptrLevel, CppRefType refType)
		: CppObj(type, prot)
		, baseType_(std::move(baseType))
		, typeAttr_(typeAttr)
		, ptrLevel_(ptrLevel)
		, refType_(refType)
		, arraySize_(NULL)
	{
	}
};

struct CppExpr;
/**
 * Class to represent C++ variables.
 * A variable can be global, local or member of a struct, class, namespace, or union.
 * It can also be a function parameter.
 */
struct CppVar : public CppVarType
{
	std::string		name_;
	unsigned int	varAttr_;  // Attribute associated with var, e.g. int* const p.
	CppExpr*		assign_; // Value assigned at declaration.
	std::string		apidocer_; // It holds things like WINAPI, __declspec(dllexport), etc.

	CppVar(CppObjProtLevel prot, std::string baseType, unsigned int typeAttr, unsigned int varAttr, unsigned short ptrLevel, CppRefType refType, std::string name)
	: CppVarType(CppObj::kVar, prot, std::move(baseType), typeAttr, ptrLevel, refType)
	, varAttr_	(varAttr)
	, name_		(std::move(name))
	, assign_	(NULL)
	{
	}
};
// name_: The variable's name.
// varAttr_: Attributes specific to the variable, such as const or static.
// assign_: The expression assigned to the variable at the time of declaration.
// apidocer_: Additional specifiers that might be associated with the variable, like WINAPI or __declspec(dllexport).

typedef std::list<CppVar*> CppVarObjList;

/**
 * \brief List of variables declared in a line without repeating its type, e.g. int i, j; is a var-list.
 */
struct CppVarList : public CppObj
{
	CppVarObjList varlist_;

	CppVarList(CppObjProtLevel prot = kUnknownProt)
		: CppObj(CppObj::kVarList, prot)
	{
	}

	~CppVarList() {
		for(CppVarObjList::iterator itr = varlist_.begin(); itr != varlist_.end(); ++itr)
			delete *itr;
	}

	void addVar(CppVar* var) {
		varlist_.push_back(var);
	}
};

struct CppTypedef : public CppVarType
{
	std::list<std::string>	names_;

	CppTypedef(CppObjProtLevel prot, std::string baseType, unsigned int typeAttr, unsigned short ptrLevel, CppRefType refType)
		: CppVarType(CppObj::kTypedef, prot, std::move(baseType), typeAttr, ptrLevel, refType)
	{
	}
};

struct CppFwdClsDecl : public CppObj
{
	CppCompoundType cmpType_;
	std::string		name_;

	CppFwdClsDecl(CppObjProtLevel prot, std::string name, CppCompoundType cmpType)
		: CppObj	(CppObj::kFwdClsDecl, prot)
		, cmpType_	(cmpType)
		, name_		(std::move(name))
	{
	}
};

struct CppInheritInfo {
	std::string baseName;
	CppObjProtLevel inhType;

	CppInheritInfo(std::string _baseName, CppObjProtLevel _inhType)
		: baseName(std::move(_baseName))
		, inhType(_inhType)
	{
	}
};

typedef std::list<CppInheritInfo> CppInheritanceList;

// These structures help in organizing and managing the different aspects of C++ programs, providing a clear and structured way to represent complex programming elements.

#endif