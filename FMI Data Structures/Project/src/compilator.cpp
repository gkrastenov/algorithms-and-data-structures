#include "compilator.h"

string Compilator::buildCompileCode(const string& code, const std::vector<string>& arguments)
{
	size_t index = 0;
	string buildCode = code;
	string curr = "";
	int stopIndex = 0;
	for (size_t i = 0; i < buildCode.size(); i++)
	{
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
				i = stopIndex;
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
		}
		else if (funCode[i] == '[') {
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
	{
		return nullptr;
	}
	if (astNode->getChindrenSize() == 0)
	{
		return astNode;
	}
	for (size_t i = 0; i < astNode->getChindrenSize(); i++)
	{
		auto curr = runTreeBody(astNode->getChildrenNodeByIndex(i));
		auto childFunction = curr->getFunction();
		if (astNode->getFunction().getType() == FunctionType::CONCAT) { // concat
			if (astNode->getChindrenSize() != 2)
				setErrorLogger(ErrorType::CONCAT_MISSING_ARGUMENTS);
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

		}else if (astNode->getFunction().getType() == FunctionType::NAND) { // nand
			if (astNode->getChindrenSize() != 2)
				setErrorLogger(ErrorType::NAND_MISSING_ARGUMENTS);
			if (i == 1) 
				nand(astNode);

		}
		else if (astNode->getFunction().getType() == FunctionType::EQ) { // eq
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

		}else if (astNode->getFunction().getType() == FunctionType::INT) { // int
			if (astNode->getChindrenSize() != 1)
				setErrorLogger(ErrorType::INT_MISSING_ARGUMENTS);
			integer(astNode, childFunction);

		} else if (astNode->getFunction().getType() == FunctionType::TAIL) { // tail
			if (astNode->getChindrenSize() != 1)
				errorLogger.setErrorType(ErrorType::ARGUMENT_EXEPTION);
			tail(astNode, childFunction);

		} else if (astNode->getFunction().getType() == FunctionType::HEAD) {// head
			if (astNode->getChindrenSize() != 1)
				errorLogger.setErrorType(ErrorType::HEAD_ZERO_ARGUMENTS);
			// add good exepetion, throw "HEAD_ZERO_ARGUMENTS"; HEAD_NOT_MATCHING_CORRECT_ARGUMENT
			head(astNode, childFunction);

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

void Compilator::head(ASTNode* root, Function& listFunciton)
{
	if (isTypeList(listFunciton.getType()))
	{
		if (listFunciton.getList().size() >= 1)
			root->getFunction().replaceList(listFunciton.getList().front());
		// what to throw if i have head([])
	}
	// TODO: trow exeption for not correct type
}

void Compilator::tail(ASTNode* root, Function& listFunciton)
{
	 if (isTypeList(listFunciton.getType()))
	{
		if (listFunciton.getList().size() >= 1)
			root->getFunction().replaceList(1, listFunciton.getList());
		// what to throw if i have tail([])
	}
	// TODO: trow exeption for not correct type
}

void Compilator::integer(ASTNode* root, Function& numberFunction)
{
	if (numberFunction.getType() != FunctionType::NUMBER)
		throw "Not correct argument for function int()"; // TODO: Better name

	auto integer = std::floor(numberFunction.getList().front());
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

void Compilator::nand(ASTNode* root)
{
	auto children = root->getChildrenNodes(); // vector

	bool firstIsNand;
	bool secondIsNand;

	// first argument
	if (isTypeList(children[0]->getFunction().getType()))
	{
		firstIsNand = children[0]->getFunction().getList().nand();
	}else {
		if (children[0]->getFunction().getList().size() == 0)
			firstIsNand = false;
	   else firstIsNand = children[0]->getFunction().getList().front() != 0;
	}

	// second argument
	if (isTypeList(children[1]->getFunction().getType())) 
	{
		secondIsNand = children[1]->getFunction().getList().nand();
	}else {
		if (children[1]->getFunction().getList().size() == 0)
			secondIsNand = false;
       else secondIsNand = children[1]->getFunction().getList().front() != 0;
	}

	bool result = firstIsNand || secondIsNand;
	root->getFunction().replaceList(result);
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


	children[0]->getFunction().getList();

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
			arguments.push_back(curr);
			curr = "";
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
			arguments.push_back(curr);
			curr = "";
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

std::multiset<double> Compilator::output() const
{
	if (canBePrinted == false){
		if(isCreated)
			return std::multiset<double> {0};
		return std::multiset<double> {1};
	}

	std::multiset<double> multiset;
	compiledFunction.getList().print(multiset);
	return multiset;
}