
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

	CppObj(Type type, CppObjProtLevel prot)
	: objType_	(type)
	, owner_	(NULL)
	, prot_		(prot)
	{
	}

	virtual ~CppObj() = 0 {}

};



#endif