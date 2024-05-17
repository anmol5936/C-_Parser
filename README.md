CppParser is a comprehensive library designed to parse C/C++ source code into a Document Object Model (DOM) structure. Unlike many other parsers, CppParser does not perform pre-processing and strives to retain comments within the source code, providing a more complete representation of the original file.

Features
DOM Representation: Parses C/C++ source code into a DOM structure, enabling hierarchical and detailed inspection of source files.
Lenient Parsing: Designed to be extremely lenient, CppParser aims to process only valid C/C++ files, ensuring robust parsing without strict error enforcement.
Comment Preservation: Maintains comments within the source code, ensuring that no information is lost during the parsing process.