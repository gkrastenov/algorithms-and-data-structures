#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "interface.h"
#include "compilator.h"
TEST_CASE("Run built-in function")
{
	Compilator compilator;
	SECTION("Run built-in function in: int")
	{
		std::stringstream myInt("int(5.2)");
    	compilator.compileCode(myInt);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == std::multiset<double>{5});

		std::stringstream myErrorInt("int(5.2, 5)");
		REQUIRE_THROWS(compilator.compileCode(myErrorInt));
		REQUIRE(compilator.getErrorType() == ErrorType::ARGUMENT_EXEPTION);

		std::stringstream myErrorListInt("int([5 2 5])");
		REQUIRE_THROWS(compilator.compileCode(myErrorListInt));
		REQUIRE(compilator.getErrorType() == ErrorType::ARGUMENT_EXEPTION);
	}

	SECTION("Run built-in function in: eq")
	{
		std::multiset<double> isTrue{1};
		std::multiset<double> isFalse{0};

		std::stringstream myErrEq1("eq(5)");
		REQUIRE_THROWS(compilator.compileCode(myErrEq1));
		REQUIRE(compilator.getErrorType() == ErrorType::ARGUMENT_EXEPTION);

		std::stringstream myErrEq2("eq([5])");
		REQUIRE_THROWS(compilator.compileCode(myErrEq2));
		REQUIRE(compilator.getErrorType() == ErrorType::ARGUMENT_EXEPTION);

		std::stringstream myEq1("eq(5, 5)");
		compilator.compileCode(myEq1);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == isTrue);

		std::stringstream myEq2("eq(5, 5.1)");
		compilator.compileCode(myEq2);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == isFalse);

		std::stringstream myEq3("eq(5.1, 5)");
		compilator.compileCode(myEq3);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == isFalse);

		std::stringstream myEq4("eq([1 2], [1 2])");
		compilator.compileCode(myEq4);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == isTrue);

		std::stringstream myEq5("eq([2 1], [1 2])");
		compilator.compileCode(myEq5);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == isFalse);

		std::stringstream myEq6("eq([], [])");
		compilator.compileCode(myEq6);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == isTrue);

		std::stringstream myEq7("eq(2, [2 1])");
		compilator.compileCode(myEq7);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == isFalse);

		std::stringstream myEq8("eq(2, [2])");
		compilator.compileCode(myEq8);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == isTrue);

		std::stringstream myEq9("eq(5.1, 5.1)");
		compilator.compileCode(myEq9);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == isTrue);
	}

	SECTION("Run built-in function in: mod")
	{
		std::stringstream myMod("mod(5 2)");
		compilator.compileCode(myMod);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == std::multiset<double>{1});

		std::stringstream myErrorMod1("mod(5)");
		REQUIRE_THROWS(compilator.compileCode(myErrorMod1));
		REQUIRE(compilator.getErrorType() == ErrorType::ARGUMENT_EXEPTION);

		std::stringstream myErrorMod2("mod([5 4 6 2])");
		REQUIRE_THROWS(compilator.compileCode(myErrorMod2));
		REQUIRE(compilator.getErrorType() == ErrorType::ARGUMENT_EXEPTION);

		std::stringstream myErrorMod3("mod(5.2 1)");
		REQUIRE_THROWS(compilator.compileCode(myErrorMod3));
		REQUIRE(compilator.getErrorType() == ErrorType::MOD_WORK_ONLY_WITH_INTEGERS);

		std::stringstream myErrorMod4("mod(5 1.1)");
		REQUIRE_THROWS(compilator.compileCode(myErrorMod4));
		REQUIRE(compilator.getErrorType() == ErrorType::MOD_WORK_ONLY_WITH_INTEGERS);

		std::stringstream myErrorMod5("mod(5.1 1.1)");
		REQUIRE_THROWS(compilator.compileCode(myErrorMod5));
		REQUIRE(compilator.getErrorType() == ErrorType::MOD_WORK_ONLY_WITH_INTEGERS);
	}
	SECTION("Run built-in function in: mod") 
	{
		std::multiset<double> isTrue  {1};
		std::multiset<double> isFalse {0};

		std::stringstream myNand1("nand(5)");
		REQUIRE_THROWS(compilator.compileCode(myNand1));
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.getErrorType() == ErrorType::ARGUMENT_EXEPTION);

		std::stringstream myNand2("nand([5 3 2])");
		REQUIRE_THROWS(compilator.compileCode(myNand2));
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.getErrorType() == ErrorType::ARGUMENT_EXEPTION);

		std::stringstream myNand3("nand(5, 5)");
		compilator.compileCode(myNand3);
		REQUIRE(compilator.output() == isTrue);

		std::stringstream myNand4("nand(5, 0)");
		compilator.compileCode(myNand4);
		REQUIRE(compilator.output() == isTrue);

		std::stringstream myNand5("nand(0, 0)");
		compilator.compileCode(myNand5);
		REQUIRE(compilator.output() == isFalse);

		std::stringstream myNand6("nand([], [])");
		compilator.compileCode(myNand6);
		REQUIRE(compilator.output() == isFalse);

		std::stringstream myNand7("nand([1], [])");
		compilator.compileCode(myNand7);
		REQUIRE(compilator.output() == isTrue);

		std::stringstream myNand8("nand([1 2], [3])");
		compilator.compileCode(myNand8);
		REQUIRE(compilator.output() == isTrue);

		std::stringstream myNand9("nand(5, [5 3 2])");
		compilator.compileCode(myNand9);
		REQUIRE(compilator.output() == isTrue);

		REQUIRE(compilator.getContainerSize() == 0);
	}
}

TEST_CASE("Compile complex funcitons")
{
	Compilator compilator;
	std::multiset<double> isCreated{ 0 };
	SECTION("Compile complex functions")
	{
		std::stringstream myEvenList("myEvenList ->[10 5 3 7]");
		compilator.compileCode(myEvenList);
		REQUIRE(compilator.getIsCreated());
		REQUIRE(compilator.getContainerSize() == 1);
		REQUIRE(compilator.output() == isCreated);


		std::stringstream tailMyEvenList("tail(myEvenList())");
		compilator.compileCode(tailMyEvenList);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.getContainerSize() == 1);
		REQUIRE(compilator.output() == std::multiset<double>{5,3,7});

		std::stringstream isOdd("isOdd -> eq(mod(int(#0), 2), 1)");
		compilator.compileCode(isOdd);
		REQUIRE(compilator.getIsCreated());
		REQUIRE(compilator.getContainerSize() == 2);
		REQUIRE(compilator.output() == isCreated);
		
		std::stringstream isOddCall("isOdd(5.2)");
		compilator.compileCode(isOddCall);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.getContainerSize() == 2);
		REQUIRE(compilator.output() == std::multiset<double>{1});

		std::stringstream myNumber("myNumber -> 5");
		compilator.compileCode(myNumber);
		REQUIRE(compilator.getIsCreated());
		REQUIRE(compilator.getContainerSize() == 3);
		REQUIRE(compilator.output() == isCreated);

		std::stringstream isOddCallNumber("isOdd(myNumber())");
		compilator.compileCode(isOddCallNumber);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.getContainerSize() == 3);
		REQUIRE(compilator.output() == std::multiset<double>{1});

		std::stringstream myList("myList1 -> [3 4 5 7 9 10]");
		compilator.compileCode(myList);
		REQUIRE(compilator.getIsCreated());
		REQUIRE(compilator.getContainerSize() == 4);
		std::multiset<double> expected1{ 0 };
		REQUIRE(compilator.output() == expected1);

		std::stringstream head("head(myList1())");
		compilator.compileCode(head);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.getContainerSize() == 4);
		std::multiset<double> expected2{ 3 };
		REQUIRE(compilator.output() == expected2);

		std::stringstream isEven("isEven -> nand(isOdd(#0), 1)");
		compilator.compileCode(isEven);
		REQUIRE(compilator.getIsCreated());
		REQUIRE(compilator.getContainerSize() == 5);
		REQUIRE(compilator.output() == isCreated);

		std::stringstream isEvenExampleWithNumber("isEven(myNumber())");
		compilator.compileCode(isEvenExampleWithNumber);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.getContainerSize() == 5);
		REQUIRE(compilator.output() == std::multiset<double> {1});
	}
}

TEST_CASE("Run already created function")
{
	Compilator compilator;
	std::multiset<double> isCreated{0};

	SECTION("Run and print already created basic function")
	{
		std::stringstream myList("myList -> [3 4 5 7 9 10]");
		compilator.compileCode(myList);
		REQUIRE(compilator.getContainerSize() == 1);
		REQUIRE(compilator.output() == isCreated);

		std::stringstream myListPrint1("myList()");
		compilator.compileCode(myListPrint1);
		REQUIRE(!compilator.getIsCreated());
		std::multiset<double> expected1{ 3,4,5,7,9,10 };
		REQUIRE(compilator.output() == expected1);

		std::stringstream myList1("myList1 -> -10");
		compilator.compileCode(myList1);
		REQUIRE(compilator.output() == isCreated);

		std::stringstream myListPrint2("myList1()");
		compilator.compileCode(myListPrint2);
		REQUIRE(!compilator.getIsCreated());
		std::multiset<double> expected2{ -10 };
		REQUIRE(compilator.output() == expected2);


		std::stringstream myList2("myList2 -> 10");
		compilator.compileCode(myList2);
		REQUIRE(compilator.output() == isCreated);

		REQUIRE(compilator.getContainerSize() == 3);

		std::stringstream myListPrint3("myList2()");
		compilator.compileCode(myListPrint3);
		REQUIRE(!compilator.getIsCreated());
		std::multiset<double> expected3{ 10 };
		REQUIRE(compilator.output() == expected3);
		REQUIRE(compilator.getContainerSize() == 3);
	}
}

TEST_CASE("Built-In functions must not be modified")
{
	Compilator compilator;
	SECTION("Built-In functions must not be modified by user")
	{
		std::stringstream throw1("sqrt -> sqrt(2)");
		REQUIRE_THROWS(compilator.compileCode(throw1));
		REQUIRE(compilator.getErrorType() == ErrorType::WRONG_USE_OPERATION_MODIFIED_BUILT_IN_FUNCTION);

		std::stringstream throw2("eq -> 5");
		std::stringstream throw3("eq -> true");
		std::stringstream throw4("read -> 5");
		REQUIRE_THROWS(compilator.compileCode(throw2));
		REQUIRE_THROWS(compilator.compileCode(throw3));
		REQUIRE_THROWS(compilator.compileCode(throw4));
		REQUIRE(compilator.getErrorType() == ErrorType::WRONG_USE_OPERATION_MODIFIED_BUILT_IN_FUNCTION);

		std::stringstream throw5("head -> list(read(), read(), read())");
		REQUIRE_THROWS(compilator.compileCode(throw5));
		REQUIRE(compilator.getErrorType() == ErrorType::WRONG_USE_OPERATION_MODIFIED_BUILT_IN_FUNCTION);

		std::stringstream throw6("add -> (5 6)");
		std::stringstream throw7("mul -> 3 2");
		REQUIRE_THROWS(compilator.compileCode(throw6));
		REQUIRE_THROWS(compilator.compileCode(throw7));
		REQUIRE(compilator.getErrorType() == ErrorType::WRONG_USE_OPERATION_MODIFIED_BUILT_IN_FUNCTION);

		std::stringstream throw8("le -> 3 2");
		std::stringstream throw9("nand -> 5 2");
		std::stringstream throw10("tail -> 100");
		std::stringstream throw11("length -> [1 2 3 4 5 6]");
		REQUIRE_THROWS(compilator.compileCode(throw8));
		REQUIRE_THROWS(compilator.compileCode(throw9));
		REQUIRE_THROWS(compilator.compileCode(throw10));
		REQUIRE_THROWS(compilator.compileCode(throw11));
		REQUIRE(compilator.getErrorType() == ErrorType::WRONG_USE_OPERATION_MODIFIED_BUILT_IN_FUNCTION);

		std::stringstream throw12("concat -> ([1 2 3 4 5 6], [1 2 3])");
		REQUIRE_THROWS(compilator.compileCode(throw12));
		REQUIRE(compilator.getErrorType() == ErrorType::WRONG_USE_OPERATION_MODIFIED_BUILT_IN_FUNCTION);

		std::stringstream throw13("if -> 3 2");
		REQUIRE_THROWS(compilator.compileCode(throw13));
		REQUIRE(compilator.getErrorType() == ErrorType::WRONG_USE_OPERATION_MODIFIED_BUILT_IN_FUNCTION);

		std::stringstream throw14("read -> [1 2 3]");
		std::stringstream throw15("write -> 5");
		std::stringstream throw16("mod -> 5 5");
		REQUIRE_THROWS(compilator.compileCode(throw14));
		REQUIRE_THROWS(compilator.compileCode(throw15));
		REQUIRE_THROWS(compilator.compileCode(throw16));
		REQUIRE(compilator.getErrorType() == ErrorType::WRONG_USE_OPERATION_MODIFIED_BUILT_IN_FUNCTION);
	}

	SECTION("Built-In functions can not be run without argument/arguments")
	{
		std::stringstream throw1("sqrt()");
		REQUIRE_THROWS(compilator.compileCode(throw1));
		REQUIRE(compilator.getErrorType() == ErrorType::SQRT_ARGUMENT);

		std::stringstream throw2("eq()");
		REQUIRE_THROWS(compilator.compileCode(throw2));
		REQUIRE(compilator.getErrorType() == ErrorType::EQ_MISSING_ARGUMENTS);

		std::stringstream throw3("le()");
		REQUIRE_THROWS(compilator.compileCode(throw3));
		REQUIRE(compilator.getErrorType() == ErrorType::LE_MISSING_ARGUMENTS);

		std::stringstream throw4("nand()");
		REQUIRE_THROWS(compilator.compileCode(throw4));
		REQUIRE(compilator.getErrorType() == ErrorType::NAND_MISSING_ARGUMENTS);

		std::stringstream throw5("length()");
		REQUIRE_THROWS(compilator.compileCode(throw5));
		REQUIRE(compilator.getErrorType() == ErrorType::LENGTH_MISSING_ARGUMENTS);

		std::stringstream throw6("head()");
		REQUIRE_THROWS(compilator.compileCode(throw6));
		REQUIRE(compilator.getErrorType() == ErrorType::HEAD_NOT_MATCHING_CORRECT_ARGUMENT);

		std::stringstream throw7("tail()");
		REQUIRE_THROWS(compilator.compileCode(throw7));
		REQUIRE(compilator.getErrorType() == ErrorType::TAIL_NOT_MATCHING_CORRECT_ARGUMENT);

		std::stringstream throw8("list()");
		REQUIRE_THROWS(compilator.compileCode(throw8));
		REQUIRE(compilator.getErrorType() == ErrorType::LIST_MISSING_ARGUMENTS);

		std::stringstream throw9("concat()");
		REQUIRE_THROWS(compilator.compileCode(throw9));
		REQUIRE(compilator.getErrorType() == ErrorType::CONCAT_MISSING_ARGUMENTS);

		std::stringstream throw10("if()");
		REQUIRE_THROWS(compilator.compileCode(throw10));
		REQUIRE(compilator.getErrorType() == ErrorType::IF_MISSING_ARGUMENTS);

		std::stringstream throw11("write()");
		REQUIRE_THROWS(compilator.compileCode(throw11));
		REQUIRE(compilator.getErrorType() == ErrorType::WRITE_MISSING_ARGUMENTS);

		std::stringstream throw12("int()");
		REQUIRE_THROWS(compilator.compileCode(throw2));
		REQUIRE(compilator.getErrorType() == ErrorType::INT_MISSING_ARGUMENTS);

		std::stringstream throw13("eq(5 eq())");
		REQUIRE_THROWS(compilator.compileCode(throw13));
		REQUIRE(compilator.getErrorType() == ErrorType::EQ_MISSING_ARGUMENTS);
	}
	/*
	SECTION("Bult-In function \"read\" can be run without argument/arguments")
	{
		compilator.compile_code("read()");
	}*/
}
/*
TEST_CASE("Run built-in function")
{
	Compilator compilator;
	REQUIRE(compilator.compile_code("list(1, 1, 10)") == "[1 2 3 4 5 6 7 8 9 10]");

}
*/
/*
TEST_CASE("Create and save function")
{
	Compilator compilator;
	SECTION("Create basic function and save it")
	{
		REQUIRE(compilator.compile_code("myList1 -> [3 4 5 7 9 10]") == "0");
		REQUIRE(compilator.getContainerSize() == 1);
		REQUIRE(compilator.compile_code("myList1 -> [3 4 5 7 9]") == "1");
		REQUIRE(compilator.getContainerSize() == 1);
		REQUIRE(compilator.compile_code("myList2 -> [1]") == "0");
		REQUIRE(compilator.getContainerSize() == 2);

		REQUIRE(compilator.compile_code("myNumber1 -> 5") == "0");
		REQUIRE(compilator.compile_code("myNumber2 -> 5 ") == "0");
		REQUIRE(compilator.compile_code("myNumber3 -> 50") == "0");
		REQUIRE(compilator.compile_code("myNumber4 -> 6.5") == "0");
		REQUIRE(compilator.getContainerSize() == 6);
		REQUIRE(compilator.compile_code("myNumber1 -> -5") == "1");
		REQUIRE(compilator.getContainerSize() == 6);
		REQUIRE(compilator.compile_code("myNumber1 -> 50") == "1");

		REQUIRE(compilator.compile_code("myNumber3 -> 6") == "1");
		REQUIRE(compilator.compile_code("myNumber4 -> 7") == "1");
		REQUIRE(compilator.getContainerSize() == 6);
	}

	SECTION("Validation syntax for basic function")
	{
		// Invalid syntax
		REQUIRE(!compilator.valid_syntax_basic_funciton("asd"));
		REQUIRE(!compilator.valid_syntax_basic_funciton("[a 1 2 3 4]"));
		REQUIRE(!compilator.valid_syntax_basic_funciton("[a --1 2 3 4]"));
		REQUIRE(!compilator.valid_syntax_basic_funciton("-[a 1 2 3 4]"));
		REQUIRE(!compilator.valid_syntax_basic_funciton("-[a 1 2 3 4-]"));
		REQUIRE(!compilator.valid_syntax_basic_funciton("-[a 1 2 3 -4-]"));
		REQUIRE(!compilator.valid_syntax_basic_funciton("-[a 1 2 3 4]-"));
		REQUIRE(!compilator.valid_syntax_basic_funciton("[[1 2 3 4 5]]"));
		REQUIRE(!compilator.valid_syntax_basic_funciton("[1 2 3] 5"));
		REQUIRE(!compilator.valid_syntax_basic_funciton("[a]"));
		REQUIRE(!compilator.valid_syntax_basic_funciton("[a b]"));

		// Valid syntax
		REQUIRE(compilator.valid_syntax_basic_funciton("56"));
		REQUIRE(compilator.valid_syntax_basic_funciton("[1 2 3 4]"));
		REQUIRE(compilator.valid_syntax_basic_funciton("[]"));
		REQUIRE(compilator.valid_syntax_basic_funciton("[1]"));
	}

	SECTION("Create and do not save basic function")
	{
		REQUIRE(compilator.compile_code("56") == "56");
		REQUIRE(compilator.compile_code("[1 2 3 4]") == "[1 2 3 4]");
		REQUIRE(compilator.getContainerSize() == 0);
		REQUIRE(compilator.compile_code("[]") == "[]");
		REQUIRE_THROWS(compilator.compile_code("[a]"));
		REQUIRE(compilator.getErrorType() == ErrorType::SYNTAX_INVALID);

		REQUIRE_THROWS(compilator.compile_code("[a --1 2 3 4]"));
		REQUIRE(compilator.getErrorType() == ErrorType::SYNTAX_INVALID);
		REQUIRE(compilator.getContainerSize() == 0);
	}
}
*/