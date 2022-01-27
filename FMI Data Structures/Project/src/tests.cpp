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
		REQUIRE(compilator.getErrorType() == ErrorType::INT_MISSING_ARGUMENTS);

		std::stringstream myErrorListInt("int([5 2 5])");
		REQUIRE_THROWS(compilator.compileCode(myErrorListInt));
		REQUIRE(compilator.getErrorType() == ErrorType::INT_MISSING_ARGUMENTS);
	}

	SECTION("Run built-in function in: eq")
	{
		std::multiset<double> isTrue{1};
		std::multiset<double> isFalse{0};

		std::stringstream myErrEq1("eq(5)");
		REQUIRE_THROWS(compilator.compileCode(myErrEq1));
		REQUIRE(compilator.getErrorType() == ErrorType::EQ_MISSING_ARGUMENTS);

		std::stringstream myErrEq2("eq([5])");
		REQUIRE_THROWS(compilator.compileCode(myErrEq2));
		REQUIRE(compilator.getErrorType() == ErrorType::EQ_MISSING_ARGUMENTS);

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
		REQUIRE(compilator.getErrorType() == ErrorType::MOD_MISSING_ARGUMENTS);

		std::stringstream myErrorMod2("mod([5 4 6 2])");
		REQUIRE_THROWS(compilator.compileCode(myErrorMod2));
		REQUIRE(compilator.getErrorType() == ErrorType::MOD_MISSING_ARGUMENTS);

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
	SECTION("Run built-in function in: nand") 
	{
		std::multiset<double> isTrue  {1};
		std::multiset<double> isFalse {0};

		std::stringstream myNand1("nand(5)");
		REQUIRE_THROWS(compilator.compileCode(myNand1));
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.getErrorType() == ErrorType::NAND_MISSING_ARGUMENTS);

		std::stringstream myNand2("nand([5 3 2])");
		REQUIRE_THROWS(compilator.compileCode(myNand2));
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.getErrorType() == ErrorType::NAND_MISSING_ARGUMENTS);

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
	SECTION("Run built-in function in: sqrt")
	{
		std::stringstream mySqrt1("sqrt(25)");
		compilator.compileCode(mySqrt1);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == std::multiset<double>{5});

		std::stringstream mySqrt2("sqrt(25, 30)");
		REQUIRE_THROWS(compilator.compileCode(mySqrt2));
		REQUIRE(compilator.getErrorType() == ErrorType::SQRT_MISSING_ARGUMENT);

		std::stringstream mySqrt3("sqrt([1 2])");
		REQUIRE_THROWS(compilator.compileCode(mySqrt3));
		REQUIRE(compilator.getErrorType() == ErrorType::SQRT_INCORRECT_ARGUMENTS);

		std::stringstream mySqrt4("sqrt([])");
		REQUIRE_THROWS(compilator.compileCode(mySqrt4));
		REQUIRE(compilator.getErrorType() == ErrorType::SQRT_INCORRECT_ARGUMENTS);

		std::stringstream mySqrt5("sqrt(9)");
		compilator.compileCode(mySqrt5);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == std::multiset<double>{3});

		REQUIRE(compilator.getContainerSize() == 0);
	}
	SECTION("Run built-in function in: add")
	{
		std::stringstream myAdd1("add(25, 25)");
		compilator.compileCode(myAdd1);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == std::multiset<double>{50});

		std::stringstream myAdd2("add(0, 0)");
		compilator.compileCode(myAdd2);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == std::multiset<double>{0});

		std::stringstream myAdd3("add([])");
		REQUIRE_THROWS(compilator.compileCode(myAdd3));
		REQUIRE(compilator.getErrorType() == ErrorType::ADD_MISSING_ARGUMENTS);

		std::stringstream myAdd4("add([1])");
		REQUIRE_THROWS(compilator.compileCode(myAdd4));
		REQUIRE(compilator.getErrorType() == ErrorType::ADD_MISSING_ARGUMENTS);

		std::stringstream myAdd5("add(5, [1])");
		REQUIRE_THROWS(compilator.compileCode(myAdd5));
		REQUIRE(compilator.getErrorType() == ErrorType::ADD_INCORRECT_ARGUMENTS);

		std::stringstream myAdd6("add(25, -25)");
		compilator.compileCode(myAdd6);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == std::multiset<double>{0});

		std::stringstream myAdd7("add([3], [1])");
		REQUIRE_THROWS(compilator.compileCode(myAdd7));
		REQUIRE(compilator.getErrorType() == ErrorType::ADD_INCORRECT_ARGUMENTS);

		REQUIRE(compilator.getContainerSize() == 0);
	}
	SECTION("Run built-in function in: sub")
	{
		std::stringstream mySub1("sub(25, 25)");
		compilator.compileCode(mySub1);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == std::multiset<double>{0});

		std::stringstream mySub2("sub(25, -25)");
		compilator.compileCode(mySub2);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == std::multiset<double>{50});

		std::stringstream mySub3("sub(1, 25)");
		compilator.compileCode(mySub3);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == std::multiset<double>{-24});

		std::stringstream mySub5("sub(5)");
		REQUIRE_THROWS(compilator.compileCode(mySub5));
		REQUIRE(compilator.getErrorType() == ErrorType::SUB_MISSING_ARGUMENTS);

		std::stringstream mySub6("sub([], [1])");
		REQUIRE_THROWS(compilator.compileCode(mySub6));
		REQUIRE(compilator.getErrorType() == ErrorType::SUB_INCORRECT_ARGUMENTS);

		std::stringstream mySub7("sub([1 2])");
		REQUIRE_THROWS(compilator.compileCode(mySub7));
		REQUIRE(compilator.getErrorType() == ErrorType::SUB_MISSING_ARGUMENTS);

		std::stringstream mySub8("sub(5, [1 2])");
		REQUIRE_THROWS(compilator.compileCode(mySub8));
		REQUIRE(compilator.getErrorType() == ErrorType::SUB_INCORRECT_ARGUMENTS);

		REQUIRE(compilator.getContainerSize() == 0);
	}
	/*
	SECTION("Run built-in function in: concat")
	{
		std::stringstream myConcat1("concat([], [])");
		compilator.compileCode(myConcat1);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == std::multiset<double>());

		std::stringstream myConcat2("concat([1], [1])");
		compilator.compileCode(myConcat2);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == std::multiset<double>{1, 1});

		std::stringstream myConcat3("concat([1], [])");
		compilator.compileCode(myConcat3);
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == std::multiset<double>{1});

		std::stringstream myConcat4("concat([1 2], [3 4])");
		REQUIRE(!compilator.getIsCreated());
		REQUIRE(compilator.output() == std::multiset<double>{1, 2, 3, 4});

		std::stringstream myConcat6("concat(5, 6)");
		REQUIRE_THROWS(compilator.compileCode(myConcat6));
		REQUIRE(compilator.getErrorType() == ErrorType::CONCAT_INCORRECT_ARGUMENTS);

		std::stringstream myConcat7("concat(7, [1])");
		REQUIRE_THROWS(compilator.compileCode(myConcat7));
		REQUIRE(compilator.getErrorType() == ErrorType::CONCAT_INCORRECT_ARGUMENTS);

		std::stringstream myConcat8("concat(5)");
		REQUIRE_THROWS(compilator.compileCode(myConcat8));
		REQUIRE(compilator.getErrorType() == ErrorType::CONCAT_MISSING_ARGUMENTS);

		std::stringstream myConcat9("concat([1 2 3 4)");
		REQUIRE_THROWS(compilator.compileCode(myConcat9));
		REQUIRE(compilator.getErrorType() == ErrorType::CONCAT_MISSING_ARGUMENTS);

		REQUIRE(compilator.getContainerSize() == 0);
	}	
	*/
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
}
/*
TEST_CASE("Run built-in function")
{
	Compilator compilator;
	REQUIRE(compilator.compile_code("list(1, 1, 10)") == "[1 2 3 4 5 6 7 8 9 10]");

}
*/

TEST_CASE("Create and save function")
{
	Compilator compilator;
	SECTION("Create basic function and save it")
	{
		std::multiset<double> isCreated{ 0 };
		std::multiset<double> isEdited{ 1 };

		std::stringstream list1Created("myList1 -> [3 4 5 7 9 10]");
		compilator.compileCode(list1Created);
		REQUIRE(compilator.output() == isCreated);
		REQUIRE(compilator.getContainerSize() == 1);

		std::stringstream list1Edited("myList1 -> [3 4 5 7 9]");
		compilator.compileCode(list1Edited);
		REQUIRE(compilator.output() == isEdited);
		REQUIRE(compilator.getContainerSize() == 1);

		std::stringstream list2("myList2 -> [1]");
		compilator.compileCode(list2);
		REQUIRE(compilator.output() == isCreated);
		REQUIRE(compilator.getContainerSize() == 2);

		std::stringstream number1("myNumber1 -> 5");
		compilator.compileCode(number1);
		REQUIRE(compilator.output() == isCreated);

		std::stringstream number2("myNumber2 -> 5");
		compilator.compileCode(number2);
		REQUIRE(compilator.output() == isCreated);

		std::stringstream number3("myNumber3 -> 50");
		compilator.compileCode(number3);
		REQUIRE(compilator.output() == isCreated);

		std::stringstream number4("myNumber4 -> 6.5");
		compilator.compileCode(number4);
		REQUIRE(compilator.output() == isCreated);
		REQUIRE(compilator.getContainerSize() == 6);

		std::stringstream number1Edited("myNumber1 -> -5");
		compilator.compileCode(number1Edited);
		REQUIRE(compilator.output() == isEdited);
		REQUIRE(compilator.getContainerSize() == 6);

		std::stringstream number3Edited("myNumber3 -> 6");
		compilator.compileCode(number3Edited);
		REQUIRE(compilator.output() == isEdited);
		REQUIRE(compilator.getContainerSize() == 6);

		std::stringstream number4Edited("myNumber4 -> 7");
		compilator.compileCode(number4Edited);
		REQUIRE(compilator.output() == isEdited);
		REQUIRE(compilator.getContainerSize() == 6);
		REQUIRE(compilator.getContainerSize() == 6);
	}
	
	SECTION("Create and do not save basic function")
	{
		// Invalid syntax
		std::stringstream invalidCode1("asd");
		REQUIRE_THROWS(compilator.compileCode(invalidCode1));
		REQUIRE(compilator.getErrorType() == ErrorType::SYNTAX_INVALID);

		std::stringstream invalidCode2("[a 1 2 3 4]");
		REQUIRE_THROWS(compilator.compileCode(invalidCode2));
		REQUIRE(compilator.getErrorType() == ErrorType::SYNTAX_INVALID);

		std::stringstream invalidCode3("[a --1 2 3 4]");
		REQUIRE_THROWS(compilator.compileCode(invalidCode3));
		REQUIRE(compilator.getErrorType() == ErrorType::SYNTAX_INVALID);

		std::stringstream invalidCode4("-[a 1 2 3 4]");
		REQUIRE_THROWS(compilator.compileCode(invalidCode4));
		REQUIRE(compilator.getErrorType() == ErrorType::SYNTAX_INVALID);

		std::stringstream invalidCode5("[a 1 2 3 4-]");
		REQUIRE_THROWS(compilator.compileCode(invalidCode5));
		REQUIRE(compilator.getErrorType() == ErrorType::SYNTAX_INVALID);

		std::stringstream invalidCode6("[[1 2 3 4 5]]");
		REQUIRE_THROWS(compilator.compileCode(invalidCode6));
		REQUIRE(compilator.getErrorType() == ErrorType::SYNTAX_INVALID);

		std::stringstream invalidCode7("[1 2 3] 5");
		REQUIRE_THROWS(compilator.compileCode(invalidCode7));
		REQUIRE(compilator.getErrorType() == ErrorType::SYNTAX_INVALID);

		std::stringstream invalidCode8("[a]");
		REQUIRE_THROWS(compilator.compileCode(invalidCode8));
		REQUIRE(compilator.getErrorType() == ErrorType::SYNTAX_INVALID);

		std::stringstream invalidCode9("[5 b]");
		REQUIRE_THROWS(compilator.compileCode(invalidCode9));
		REQUIRE(compilator.getErrorType() == ErrorType::SYNTAX_INVALID);

		std::stringstream validCode1("56");
		compilator.compileCode(validCode1);
		REQUIRE(compilator.output() == std::multiset<double>{56});

		std::stringstream validCode2("[1 2 3 4]");
		compilator.compileCode(validCode2);
		REQUIRE(compilator.output() == std::multiset<double>{1,2,3,4});

		std::stringstream validCode3("[]");
		compilator.compileCode(validCode3);
		REQUIRE(compilator.output() == std::multiset<double>{});

		std::stringstream validCode4("[1]");
		compilator.compileCode(validCode4);
		REQUIRE(compilator.output() == std::multiset<double>{1});
		REQUIRE(compilator.getContainerSize() == 0);
	}
}