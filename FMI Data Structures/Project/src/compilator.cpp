#include "compilator.h"

string Compilator::buildCompileCode(const string& code, const std::vector<string>& arguments)
{
	size_t index = 0;
	string buildCode = code;
	string curr = "";
	int stopIndex = 0;
	for (size_t i = 0; i < buildCode.size(); i++)
	{
		if (buildCode[i] == ',' || buildCode[i] == ' ')
		{
			curr = "";
			continue;
		}

		if (buildCode[i] == '(')
		{
			if (getFunctionType(curr) == FunctionType::UNKNOWN) {
				if (containerContains(curr) == false)
					setErrorLogger(ErrorType::NOT_FOUNDED_FUNCTION);

				stopIndex = i - curr.size();
				string newCode = container[curr].getCode();
				curr.push_back(buildCode[i]);
				while (buildCode[i] != ')')
				{
					++i;
					if (i == code.size()) 
						setErrorLogger(ErrorType::SYNTAX_INVALID);
				
					curr.push_back(buildCode[i]);
				}
				if (replace(buildCode, curr, newCode) == false)
					setErrorLogger(ErrorType::NOT_FOUNDED_FUNCTION); // TODO: Change error type with: cant be replaced string or something else.

				if (isdigit(newCode[0]) || newCode[0] == '-')
					i = stopIndex + newCode.size();
				else i = stopIndex - 1;
			}
			curr = "";
			continue;
		}
		else if (curr == "#" + std::to_string(index))
		{
			if (arguments.size() <= index)
				throw "Too many arguments";

			if (replace(buildCode, curr, arguments[index]) == false)
				throw "Can not be parsed arguments";

			i -= curr.size() + 1;
			curr = "";
			++index;
			continue;
		}	
		curr.push_back(buildCode[i]);
	}
	return buildCode;
}

bool Compilator::replace(string& str, const string& from, const string& to)
{
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

void Compilator::compileCode(std::istream& stream)
{
	string code;
	std::getline(stream, code);
	string::size_type pos = 0;
	string::size_type prev = 0;
	pos = code.find(DELIMETAR, prev);
	if (pos != string::npos) {
		canBePrinted = false;
		string functionName = code.substr(prev, pos - prev);
		functionName.erase(std::remove_if(functionName.begin(), functionName.end(), isspace), functionName.end()); // remove all spaces

		prev = pos + DELIMETAR.size();
		while (code[prev] == ' ' && prev < code.size()) // remove spaces
			prev++;

		string functionCode = code.substr(prev);

		if (getFunctionType(functionName) != FunctionType::UNKNOWN)
			setErrorLogger(ErrorType::WRONG_USE_OPERATION_MODIFIED_BUILT_IN_FUNCTION);

		if (containerContains(functionName))
			isCreated = false;
	   else isCreated = true;

		unsigned int countOfArguments = countArguments(functionCode);
		container[functionName] = ContainerValue(countOfArguments, functionCode);
	}
	else {
		isCreated = false;
		canBePrinted = true;

		if (isBasicFunction(code))
		{
			if(isValidBasicFunction(code) == false)
				setErrorLogger(ErrorType::SYNTAX_INVALID);

			createTreeBody(code);
			compiledFunction = runTreeBody(tree)->getFunction();
		}
		else {
			std::vector<string> countOfArguments = getArguments(code);
			string compileCode = buildCompileCode(code, countOfArguments);
			createTreeBody(compileCode);
			compiledFunction = runTreeBody(tree)->getFunction();
		}
	}
}

int Compilator::createTreeBody(const string& funCode)
{
	if (funCode.empty())
		throw "Empty Compile code";

	size_t index = 0;
	while (funCode[index] == ' ') // skip all spaces
		++index;
	
	bool openListBracket = false; // -> [
	bool closedListBracket = false; // -> ]

	bool openFunctionBracket = false; // -> (
	bool closedFunctionBracket = false; // -> )

	bool minus = false;

	std::stack<ASTNode*> stack;
	string curr = "";

	for (size_t i = index; i < funCode.size(); i++)
	{
		if (curr == " ")
			curr = "";

		if (funCode[i] == ' ' && curr != ""){		
			if (isalnum(curr[0]) || curr[0] == '-')
				createNumberNode(stack, curr);			
			curr = "";
			continue;

		}else if (funCode[i] == ',' && curr != "") {
			if (isalnum(curr[0]) || curr[0] == '-')
				createNumberNode(stack, curr);
			curr = "";
			continue;

		} else if (funCode[i] == '[') {
			if (openListBracket)
				setErrorLogger(ErrorType::SYNTAX_MISSING_LIST_RBRACE);

			if (minus) 
				setErrorLogger(ErrorType::SYNTAX_MINUS_BEFORE_LIST_LBRACE);

			openListBracket = true;
			Function func(ARGUMENT_LIST, FunctionType::NOT_CREATED_LIST);
			if (!stack.empty()){
				auto newNode = new ASTNode(func);
				auto current = stack.top();
				current->add_child_nodes(newNode);
				stack.push(newNode);
			} else {
				tree = new ASTNode(func);
				stack.push(tree);
			}
			curr = "";
			continue;
		}
		else if (funCode[i] == ']') {
			if (openListBracket == false)
				setErrorLogger(ErrorType::SYNTAX_MISSING_LIST_LBRACE);
			if (closedListBracket)
				setErrorLogger(ErrorType::SYNTAX_MISSING_LIST_RBRACE);
			if (minus) 
				setErrorLogger(ErrorType::SYNTAX_MINUS_BEFORE_LIST_RBRACE);

			openListBracket = false;
			closedListBracket = false;
			/*
			if (curr == "")
				continue;
			*/
			if (isalnum(curr[0]) || curr[0] == '-')
				createNumberNode(stack, curr);
			if (!stack.empty())
				 stack.pop();
			curr = "";
			continue;
		}
		else if (funCode[i] == '(') {
			if (!curr.empty()) {
				auto type = getFunctionType(curr);
				Function func(curr, type);

				if (!stack.empty()) {
					auto current = stack.top();
					auto node = new ASTNode(func);
					current->add_child_nodes(node);
					stack.push(node);
				}
				else {
					tree = new ASTNode(func);
					stack.push(tree);
				}
				curr = "";
				continue;
			}
		}
		else if (funCode[i] == ')') {
			if (isalnum(curr[0]) || curr[0] == '-')
			{
				createNumberNode(stack, curr);	
			}
			if (!stack.empty())
				stack.pop();
			curr = "";
			continue;
		}
		else if (i + 1 == funCode.size()) {
				curr.push_back(funCode[i]);

			if (isalnum(curr[0]) || curr[0] == '-')
				createNumberNode(stack, curr);

			curr = "";
			continue;
		}	
		curr.push_back(funCode[i]);
	}
}

ASTNode* Compilator::runTreeBody(ASTNode* astNode)
{
	if (astNode == nullptr)
		return nullptr;

	if (astNode->getFunction().getType() == FunctionType::IF) // if
	{
		if(astNode->getChindrenSize() != 3)
			setErrorLogger(ErrorType::IF_MISSING_ARGUMENTS);

		auto children = astNode->getChildrenNodes(); // vector

		ASTNode* condition = runTreeBody(children[0]);
		if (condition->getFunction().getList().front() == 1) // true
		{
			ASTNode* ifTrue = runTreeBody(children[1]);
			if (isTypeList(ifTrue->getFunction().getType()))		
				astNode->getFunction().replaceList(0, ifTrue->getFunction().getList());
			else astNode->getFunction().replaceList(ifTrue->getFunction().getList().front());
			return astNode;
		}
		else {
			ASTNode* ifFalse = runTreeBody(children[2]);
			if (isTypeList(ifFalse->getFunction().getType()))
				astNode->getFunction().replaceList(0, ifFalse->getFunction().getList());
			else astNode->getFunction().replaceList(ifFalse->getFunction().getList().front());
			return astNode;
		}
	}
	if (astNode->getFunction().getType() == FunctionType::NAND) // nand
	{
		if (astNode->getChindrenSize() != 2)
			setErrorLogger(ErrorType::NAND_MISSING_ARGUMENTS);

		auto children = astNode->getChildrenNodes(); // vector

		// first argument
		ASTNode* first = runTreeBody(children[0]);
		bool firstIsNand;
		if (isTypeList(first->getFunction().getType()))
		{
			firstIsNand = first->getFunction().getList().nand();
		}
		else {
			if (first->getFunction().getList().size() == 0)
				firstIsNand = false;
			else firstIsNand = first->getFunction().getList().front() != 0;
		}

		if (!firstIsNand) 
		{
			astNode->getFunction().replaceList(true);
			return astNode;
		}

		// second argument
		ASTNode* second = runTreeBody(children[1]);	
		bool secondIsNand;
		if (isTypeList(second->getFunction().getType()))
		{
			secondIsNand = second->getFunction().getList().nand();
		}
		else {
			if (second->getFunction().getList().size() == 0)
				secondIsNand = false;
			else secondIsNand = children[1]->getFunction().getList().front() != 0;
		}

		if (firstIsNand && secondIsNand)
			astNode->getFunction().replaceList(false);
	   else astNode->getFunction().replaceList(true);

		return astNode;
	}

	if (astNode->getFunction().getType() == FunctionType::READ) { // read
		if (astNode->getChindrenSize() != 0)
			setErrorLogger(ErrorType::READ_MISSING_ARGUMENTS);
		read(astNode);
	}

	if (astNode->getChindrenSize() == 0)	
		return astNode;
	
	for (size_t i = 0; i < astNode->getChindrenSize(); i++)
	{
		auto curr = runTreeBody(astNode->getChildrenNodeByIndex(i));
		auto childFunction = curr->getFunction();

		if (astNode->getFunction().getType() == FunctionType::LIST)
		{
			if(astNode->getChindrenSize() > 3 || astNode->getChindrenSize() < 0)
				setErrorLogger(ErrorType::LIST_MISSING_ARGUMENTS);

			if (i == astNode->getChindrenSize() - 1)
				list(astNode, astNode->getChindrenSize());	

		}else if (astNode->getFunction().getType() == FunctionType::CONCAT) { // concat
			if (astNode->getChindrenSize() != 2)
				setErrorLogger(ErrorType::CONCAT_MISSING_ARGUMENTS);
			if (i == 1)
				concat(astNode);
		}
		else if (astNode->getFunction().getType() == FunctionType::SQRT) { // sqrt
			if (astNode->getChindrenSize() != 1)
				setErrorLogger(ErrorType::SQRT_MISSING_ARGUMENT);
			sqrt(astNode, childFunction);

		}else if (astNode->getFunction().getType() == FunctionType::ADD) { // add
			if (astNode->getChindrenSize() != 2)
				setErrorLogger(ErrorType::ADD_MISSING_ARGUMENTS);
			if (i == 1) 
				add(astNode);

		}else if (astNode->getFunction().getType() == FunctionType::SUB) { // sub
			if (astNode->getChindrenSize() != 2)
				setErrorLogger(ErrorType::SUB_MISSING_ARGUMENTS);
			if (i == 1) 
				sub(astNode);

		} else if (astNode->getFunction().getType() == FunctionType::EQ) { // eq
			if (astNode->getChindrenSize() != 2)
				setErrorLogger(ErrorType::EQ_MISSING_ARGUMENTS);
			if (i == 1)
				eq(astNode);

		} else if (astNode->getFunction().getType() == FunctionType::LE) { // le
				if (astNode->getChindrenSize() != 2)
					setErrorLogger(ErrorType::LE_MISSING_ARGUMENTS);
				if (i == 1)
					le(astNode);

		}else if (astNode->getFunction().getType() == FunctionType::MOD) { // mod
			if (astNode->getChindrenSize() != 2)
				setErrorLogger(ErrorType::MOD_MISSING_ARGUMENTS);
			if (i == 1) 
				mod(astNode);

		}
		else if (astNode->getFunction().getType() == FunctionType::DIV) { // div
			if (astNode->getChindrenSize() != 2)
				setErrorLogger(ErrorType::DIV_MISSING_ARGUMENTS);
			if (i == 1)
				div(astNode);

		}else if (astNode->getFunction().getType() == FunctionType::LENGTH) { // length
			if (astNode->getChindrenSize() != 1)
				setErrorLogger(ErrorType::LENGTH_MISSING_ARGUMENTS);

			length(astNode);

		}else if (astNode->getFunction().getType() == FunctionType::INT) { // int
			if (astNode->getChindrenSize() != 1)
				setErrorLogger(ErrorType::INT_MISSING_ARGUMENTS);
			integer(astNode);

		
		}else if (astNode->getFunction().getType() == FunctionType::TAIL) { // tail
			if (astNode->getChindrenSize() != 1)
				setErrorLogger(ErrorType::TAIL_MISSING_ARGUMENTS);
			tail(astNode);

		} else if (astNode->getFunction().getType() == FunctionType::HEAD) {// head
			if (astNode->getChindrenSize() != 1)
				setErrorLogger(ErrorType::HEAD_MISSING_ARGUMENTS);
			head(astNode);

		}else if (astNode->getFunction().getType() == FunctionType::NOT_CREATED_LIST) {
			if (childFunction.getType() == FunctionType::NUMBER 
				|| childFunction.getType() == FunctionType::NOT_CREATED_LIST) {
				astNode->getFunction().addList(childFunction.getList());
			}
			else {
				errorLogger.setErrorType(ErrorType::ARGUMENT_ONLY_NUMBERS);
				throw "ARGUMENT_ONLY_NUMBERS";
			}
		}
	}

	if (astNode->getFunction().getType() == FunctionType::NOT_CREATED_LIST) {
		astNode->getFunction().setType(FunctionType::CREATED_LIST); // TODO: maybe have to create new type: CREATED_LIST
	}

	return astNode;
}

void Compilator::head(ASTNode* root)
{
	auto children = root->getChildrenNodes(); // vector

	if (isTypeList(children[0]->getFunction().getType()) == false)
		setErrorLogger(ErrorType::HEAD_NOT_MATCHING_CORRECT_ARGUMENT);

	if(children[0]->getFunction().getList().size() == 0
	&& children[0]->getFunction().getList().getIsLoop() == false)
		setErrorLogger(ErrorType::HEAD_EMPTY_LIST_ARGUMENT);

	root->getFunction().replaceList(children[0]->getFunction().getList().front());
}

void Compilator::tail(ASTNode* root)
{
	auto children = root->getChildrenNodes(); // vector

	 if (isTypeList(children[0]->getFunction().getType()) == false)
		 setErrorLogger(ErrorType::TAIL_NOT_MATCHING_CORRECT_ARGUMENT);
	 
	 root->getFunction().replaceList(1, children[0]->getFunction().getList());
}

void Compilator::integer(ASTNode* root)
{
	auto children = root->getChildrenNodes(); // vector

	if (isTypeList(children[0]->getFunction().getType()))
		setErrorLogger(ErrorType::INT_INCORRECT_ARGUMENTS);

	auto integer = std::floor(children[0]->getFunction().getList().front());
	root->getFunction().addNumber(integer);
}

void Compilator::mod(ASTNode* root)
{
	auto children = root->getChildrenNodes(); // vector

	if (isTypeList(children[0]->getFunction().getType())
	 || isTypeList(children[1]->getFunction().getType()))
		setErrorLogger(ErrorType::MOD_INCORRECT_ARGUMENTS);

	int divindend = children[0]->getFunction().getList().front();
	int divisor = children[1]->getFunction().getList().front();

	if(divindend != children[0]->getFunction().getList().front()
	|| divisor != children[1]->getFunction().getList().front())
		setErrorLogger(ErrorType::MOD_WORK_ONLY_WITH_INTEGERS);

	root->getFunction().replaceList(divindend % divisor);		
}

void Compilator::div(ASTNode* root)
{
	auto children = root->getChildrenNodes(); // vector

	if (isTypeList(children[0]->getFunction().getType())
	 || isTypeList(children[1]->getFunction().getType()))
			setErrorLogger(ErrorType::DIV_INCORRECT_ARGUMENTS);

	double divindend = children[0]->getFunction().getList().front();
	double divisor = children[1]->getFunction().getList().front();

	if(divisor == 0)
		setErrorLogger(ErrorType::DIVIDE_BY_ZERO);

	root->getFunction().replaceList(divindend / divisor);
}

void Compilator::eq(ASTNode* root)
{
	auto children = root->getChildrenNodes(); // vector
	int result = children[0]->getFunction().getList().eqCompare(children[1]->getFunction().getList());
	root->getFunction().replaceList(result);
}

void Compilator::le(ASTNode* root)
{
	auto children = root->getChildrenNodes(); // vector
	int result = children[0]->getFunction().getList().leCompare(children[1]->getFunction().getList());
	root->getFunction().replaceList(result);
}

void Compilator::length(ASTNode* root)
{
	auto children = root->getChildrenNodes(); //vector;

	if (isTypeList(children[0]->getFunction().getType()) == false)
	{
		root->getFunction().replaceList(-1);
		return;
	}
	else {
		if (children[0]->getFunction().getList().getIsLoop())
		{
			auto count = children[0]->getFunction().getList().getCount();
			if(count == 0)
				setErrorLogger(ErrorType::LENGTH_LOOP_LIST_ARGUMENT);
			root->getFunction().replaceList(count);
		}
		else {
			auto size = children[0]->getFunction().getList().size();
			root->getFunction().replaceList(size);
		}
	}
}

void Compilator::add(ASTNode* root)
{
	auto children = root->getChildrenNodes(); //vector;

	if (isTypeList(children[0]->getFunction().getType())
	 || isTypeList(children[1]->getFunction().getType()))
		setErrorLogger(ErrorType::ADD_INCORRECT_ARGUMENTS);

	if (children[0]->getFunction().getList().size() != 1 
	 || children[1]->getFunction().getList().size() != 1)
		setErrorLogger(ErrorType::ADD_INCORRECT_ARGUMENTS);

	double result = children[0]->getFunction().getList().front()
		          + children[1]->getFunction().getList().front();

	root->getFunction().replaceList(result);
}

void Compilator::sqrt(ASTNode* root, Function& numberFunction)
{
	if(isTypeList(numberFunction.getType()))
		setErrorLogger(ErrorType::SQRT_INCORRECT_ARGUMENTS);

	if(numberFunction.getList().size() != 1)
		setErrorLogger(ErrorType::SQRT_INCORRECT_ARGUMENTS);

	double result = std::sqrt(numberFunction.getList().front());
	root->getFunction().replaceList(result);
}

void Compilator::sub(ASTNode* root)
{
	auto children = root->getChildrenNodes(); //vector;

	if (isTypeList(children[0]->getFunction().getType())
		|| isTypeList(children[1]->getFunction().getType()))
		setErrorLogger(ErrorType::SUB_INCORRECT_ARGUMENTS);

	if (children[0]->getFunction().getList().size() != 1
		|| children[1]->getFunction().getList().size() != 1)
		setErrorLogger(ErrorType::SUB_INCORRECT_ARGUMENTS);

	double result = children[0]->getFunction().getList().front()
		- children[1]->getFunction().getList().front();

	root->getFunction().replaceList(result);
}

void Compilator::concat(ASTNode* root)
{
	auto children = root->getChildrenNodes(); //vector;

	if (isTypeList(children[0]->getFunction().getType()) == false
		|| isTypeList(children[1]->getFunction().getType()) == false)
		setErrorLogger(ErrorType::CONCAT_INCORRECT_ARGUMENTS);

	if(children[0]->getFunction().getList().getIsLoop()
    && children[0]->getFunction().getList().getCount() == 0)
		setErrorLogger(ErrorType::CONCAT_LOOP_LIST);

	children[0]->getFunction().getList().concat(children[1]->getFunction().getList());
	root->getFunction().replaceList(0, children[0]->getFunction().getList());
}

void Compilator::read(ASTNode* root)
{
	string input;
	std::cout << "> read(): ";
    std::getline(std::cin, input);
	if (input.size() == 0)
		setErrorLogger(ErrorType::READ_INCORRECT_ARGUMENTS);

	bool minus = false;
	for (size_t i = 0; i < input.size(); i++)
	{
		if (input[i] == '-')
		{
			if(minus)
				setErrorLogger(ErrorType::READ_INCORRECT_ARGUMENTS);
			minus = true;
		}
		if(isdigit(input[i]) == false)
			setErrorLogger(ErrorType::READ_INCORRECT_ARGUMENTS);
	}
	root->getFunction().replaceList(stringToNumber(input));
}

void Compilator::list(ASTNode* root, const int countOfArguments = 0)
{
	auto children = root->getChildrenNodes(); // vector

	if (countOfArguments == 1)
	{
		if (isTypeList(children[0]->getFunction().getType()))
			setErrorLogger(ErrorType::LIST_INCORRECT_ARGUMENTS);

		double start = children[0]->getFunction().getList().front();

		root->getFunction().replaceList(start, 1, 0);
	}
	else if (countOfArguments == 2)
	{
		if (isTypeList(children[0]->getFunction().getType())
			|| isTypeList(children[1]->getFunction().getType()))
			setErrorLogger(ErrorType::LIST_INCORRECT_ARGUMENTS);

		double start = children[0]->getFunction().getList().front();
		double step = children[1]->getFunction().getList().front();

		root->getFunction().replaceList(start, step, 0);
	}
	else if (countOfArguments == 3)
	{
		if (isTypeList(children[0]->getFunction().getType())
			|| isTypeList(children[1]->getFunction().getType())
			|| isTypeList(children[2]->getFunction().getType()))
			setErrorLogger(ErrorType::LIST_INCORRECT_ARGUMENTS);

		double start = children[0]->getFunction().getList().front();
		double step = children[1]->getFunction().getList().front();
		double count = children[2]->getFunction().getList().front();

		root->getFunction().replaceList(start, step, count);
	}
}

void Compilator::write(ASTNode* root)
{
	/*
	if (isValidBasicFunction(code) == false)
		setErrorLogger(ErrorType::SYNTAX_INVALID);*/
}

std::vector<string> Compilator::getArguments(std::string& code) const
{
	bool foundFirstOpenBracket = false; // before first open bracket is name of funciton
	std::vector<string> arguments;

	size_t index = 0;
	while (code[index] == ' ') // skip spaces
		++index;
	
	string curr = "";
	for (size_t i = 0; i < code.size(); i++)
	{
		if (code[i] == ',') {
			if (curr != "")
				arguments.push_back(curr);
			else curr = "";
			continue;
		}else if (code[i] == '('){
			if (foundFirstOpenBracket == false) {
				foundFirstOpenBracket = true;
				curr = "";
				continue;
			}
		}else if (code[i] == ')' && i + 1 == code.size()){
			if(curr != "")
				arguments.push_back(curr);
			continue;
		}else if (code[i] == ')')
		{
			curr.push_back(code[i]);
			if(curr != "")
				arguments.push_back(curr);
			curr = "";
			continue;
		}							
		
		curr.push_back(code[i]);
	}
	return arguments;
}

unsigned int Compilator::countArguments(string& code) const
{
	unsigned int occurrences = 0;
	string::size_type pos = 0;
	string target = "#";
	while ((pos = code.find(target, pos)) != string::npos) {
		++occurrences;
		pos += target.length();
	}
	return occurrences;
}

bool Compilator::isTypeList(const FunctionType type) const
{
	return type == FunctionType::LIST || type == FunctionType::CREATED_LIST
		|| type == FunctionType::NOT_CREATED_LIST || type == FunctionType::TAIL;
}

FunctionType Compilator::getFunctionType(const string& funcName) const
{
	if (funcName == "eq")
		return FunctionType::EQ;
	if (funcName == "mod")
		return FunctionType::MOD;
	if (funcName == "head")
		return FunctionType::HEAD;
	if (funcName == "tail")
		return FunctionType::TAIL;
	if (funcName == "read")
		return FunctionType::READ;
	if (funcName == "sqrt")
		return FunctionType::SQRT;
	if (funcName == "mul")
		return FunctionType::MUL;
	if (funcName == "length")
		return FunctionType::LENGTH;
	if (funcName == "le")
		return FunctionType::LE;
	if (funcName == "concat")
		return FunctionType::CONCAT;
	if (funcName == "if")
		return FunctionType::IF;
	if (funcName == "write")
		return FunctionType::WRITE;
	if (funcName == "nand")
		return FunctionType::NAND;
	if (funcName == "add")
		return FunctionType::ADD;
	if (funcName == "read")
		return FunctionType::READ;
	if (funcName == "int")
		return FunctionType::INT;
	if (funcName == "sub")
		return FunctionType::SUB;
	if (funcName == "div")
		return FunctionType::DIV;
	if (funcName == "list")
		return FunctionType::LIST;

	return FunctionType::UNKNOWN;
}

void Compilator::setErrorLogger(const ErrorType errorType)
{
	errorLogger.setErrorType(errorType);
	throw errorLogger.getErrorMessages();
}

void Compilator::createNumberNode(std::stack<ASTNode*>& stack, const string& code)
{
	double number = stringToNumber(code);

	Function func(ARGUMENT_NUMBER, FunctionType::NUMBER);
	func.addNumber(number);
	if (!stack.empty())
	{
		auto current = stack.top();
		current->add_child_nodes(new ASTNode(func));
	}
	else {
		tree = new ASTNode(func);
		stack.push(tree);
	}
}

double Compilator::stringToNumber(const string& str)
{
	double value = 0.0;
	try
	{
		value = std::stod(str);
	}
	catch (const std::exception& exp)
	{
		setErrorLogger(ErrorType::WRONG_CAN_NOT_CONVERT_STRING_TO_NUMBER);
	}
	return value;
}

bool Compilator::isValidBasicFunction(const string& basicFuncCode) const
{
	bool openBracket = false;
	bool closedBracket = false;
	bool minus = false;
	for (size_t i = 0; i < basicFuncCode.size(); ++i)
	{
		if (openBracket && closedBracket)
			return false;
		if (basicFuncCode[i] == '[')
		{
			if (openBracket)
				return false;
			if (minus)
				return false;
			if (i != 0)
				return false;
			openBracket = true;
			continue;
		}
		if (basicFuncCode[i] == ']')
		{			
			if (openBracket == false)
				return false;
			if (closedBracket)
				return false;
			if (minus)
				return false;
			closedBracket = true;
			continue;
		}
		if (basicFuncCode[i] == ' ')
			continue;
		if ((basicFuncCode[i] >= 48 && basicFuncCode[i] <= 57) == false) {
			return false;
		}
		else {
			if (minus)
				minus = false;
			continue;
		}	
		if (basicFuncCode[i] == '-')
		{
			if (minus)
				return false;
			if (i + 1 == basicFuncCode.size())
				return false;

			minus = true;
			continue;
		}
	}
	if (openBracket != closedBracket)
		return false;

	return true;
}

bool Compilator::isBasicFunction(const string& code) const
{
	string callOpen = "(";
	string callClosed = ")";

	size_t foundCallOpen = code.find(callOpen);
	size_t foundCallClosed = code.find(callClosed);

	return foundCallOpen == string::npos && foundCallClosed == string::npos;
}

std::vector<double> Compilator::output()
{
	if (canBePrinted == false){
		if(isCreated)
			return std::vector<double> {0};
		return std::vector<double> {1};
	}

	std::vector<double> vec;
	compiledFunction.getList().print(vec);
	return vec;
}