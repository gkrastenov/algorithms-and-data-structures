#include "compilator.h"

string Compilator::build_compile_code(const string& code, const std::vector<string>& arguments)
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
				if (containerContains(curr) == false) {
					logger.setErrorType(ErrorType::NOT_FOUNDED_FUNCTION);
					throw "NOT_FOUNDED_FUNCTION";
				}
				stopIndex = i - curr.size();
				string newCode = container[curr].getCode();
				curr.push_back(buildCode[i]);
				while (buildCode[i] != ')')
				{
					++i;
					if (i == code.size()) {

						logger.setErrorType(ErrorType::SYNTAX_INVALID);
						throw "SYNTAX_INVALID";
					}
					curr.push_back(code[i]);
				}
				if (replace(buildCode, curr, newCode) == false)
				{
					// TODO: Change error type with: cant be replaced string or something else.
					logger.setErrorType(ErrorType::NOT_FOUNDED_FUNCTION);
					throw "NOT_FOUNDED_FUNCTION";
				}
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

			curr = "";
			--i;
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

		if (getFunctionType(functionName) != FunctionType::UNKNOWN) {
			logger.setErrorType(ErrorType::WRONG_USE_OPERATION_MODIFIED_BUILT_IN_FUNCTION);
			throw "WRONG_USE_OPERATION_MODIFIED_BUILT_IN_FUNCTION";
		}

		if (containerContains(functionName))
			isCreated = false;
		else {
			isCreated = true;
		}
		unsigned int countOfArguments = count_arguments(functionCode);
		container[functionName] = ContainerValue(countOfArguments, functionCode);
	}
	else {
		isCreated = false;
		canBePrinted = true;

		std::vector<string> countOfArguments = getArguments(code);
		string compileCode = build_compile_code(code, countOfArguments);
		createTreeBody(compileCode);
		compiledFunction = runTreeBody(tree)->getFunction();
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
		if (funCode[i] == ' ' && curr != ""){		
			if (isalnum(curr[0]) || curr[0] == '-')
				createNumberNode(stack, curr);
				
			curr = "";
			continue;
		}
		else if (funCode[i] == '[') {
			if (openListBracket){
				logger.setErrorType(ErrorType::SYNTAX_MISSING_LIST_RBRACE);
				throw "SYNTAX_MISSING_LIST_RBRACE";
			}

			if (minus) {
				logger.setErrorType(ErrorType::SYNTAX_MINUS_BEFORE_LIST_LBRACE);
				throw "SYNTAX_MINUS_BEFORE_LIST_LBRACE";
			}

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
			if (openListBracket == false){
				logger.setErrorType(ErrorType::SYNTAX_MISSING_LIST_LBRACE);
				throw "SYNTAX_MISSING_LIST_RBRACE";
			}

			if (closedListBracket){
				logger.setErrorType(ErrorType::SYNTAX_MISSING_LIST_RBRACE);
				throw "SYNTAX_MISSING_LIST_RBRACE";
			}
			if (minus) {
				logger.setErrorType(ErrorType::SYNTAX_MINUS_BEFORE_LIST_RBRACE);
				throw "SYNTAX_MINUS_BEFORE_LIST_RBRACE";
			}

			closedListBracket = true;
			if (curr == "")
				continue;

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
	for (ASTNode* child : astNode->getChildrenNodes())
	{
		auto curr = runTreeBody(child);
		auto childFunction = curr->getFunction();

		if (astNode->getFunction().getType() == FunctionType::MOD) {
			if (astNode->getChindrenSize() != 2) {
				// TODO: good exeption
			}
			else {
				mod(astNode, childFunction);
				continue;
			}
		}
		else if (astNode->getFunction().getType() == FunctionType::INT) { // int
			if (astNode->getChindrenSize() != 1) {}
				// TODO: good exeption		

			intiger(astNode, childFunction);
			continue;
		}
		else if (astNode->getFunction().getType() == FunctionType::TAIL) { // tail
			if (astNode->getChindrenSize() != 1) 
				logger.setErrorType(ErrorType::TAIL_ZERO_ARGUMENTS);
				// add good exepetion, to be removed "TAIL_ZERO_ARGUMENTS" "TAIL_TOO_MUCH_ARGUMENTS";;
			
			tail(astNode, childFunction);
			continue;
		}
		else if (astNode->getFunction().getType() == FunctionType::HEAD) {// head
			if (astNode->getChindrenSize() != 1)
				logger.setErrorType(ErrorType::HEAD_ZERO_ARGUMENTS);
			// add good exepetion, throw "HEAD_ZERO_ARGUMENTS"; HEAD_NOT_MATCHING_CORRECT_ARGUMENT
			head(astNode, childFunction);
			continue;
		}

		if (astNode->getFunction().getType() == FunctionType::NOT_CREATED_LIST){
			if (childFunction.getType() == FunctionType::NUMBER) {
				astNode->getFunction().add_list(childFunction.getList());
			}
			else {
				logger.setErrorType(ErrorType::ARGUMENT_ONLY_NUMBERS);
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
	if (listFunciton.getType() == FunctionType::CREATED_LIST || listFunciton.getType() == FunctionType::NOT_CREATED_LIST)
	{
		if (listFunciton.getList().size() >= 1)
			listFunciton.replace_list(listFunciton.getList().front());
		// what to throw if i have head([])
	}
	// TODO: trow exeption for not correct type
}

void Compilator::tail(ASTNode* root, Function& listFunciton)
{
	 if (listFunciton.getType() == FunctionType::CREATED_LIST || listFunciton.getType() == FunctionType::NOT_CREATED_LIST)
	{
		if (listFunciton.getList().size() >= 1)
			root->getFunction().replace_list(1, listFunciton.getList());
		// what to throw if i have tail([])
	}
	// TODO: trow exeption for not correct type
}

void Compilator::intiger(ASTNode* root, Function& numberFunction)
{
	if (numberFunction.getType() != FunctionType::NUMBER)
		throw "Not correct argument for function int()"; // TODO: Better name


	root->getFunction().add_number(std::floor(numberFunction.getList().front() * 100.) / 100);
}

void Compilator::mod(ASTNode* root, Function& numberFunction)
{

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


unsigned int Compilator::count_arguments(std::string& code) const
{
	unsigned int occurrences = 0;
	std::string::size_type pos = 0;
	std::string target = "#";
	while ((pos = code.find(target, pos)) != std::string::npos) {
		++occurrences;
		pos += target.length();
	}
	return occurrences;
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
	return FunctionType::UNKNOWN;
}

void Compilator::createNumberNode(std::stack<ASTNode*>& stack, const string& code)
{
	double number = stringToNumber(code);
	if (logger.getErrorType() == ErrorType::WRONG_CAN_NOT_CONVERT_STRING_TO_NUMBER)
		throw "WRONG_CAN_NOT_CONVERT_STRING_TO_NUMBER";

	Function func(ARGUMENT_NUMBER, FunctionType::NUMBER);
	func.add_number(number);
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
	 logger.setErrorType(ErrorType::WRONG_CAN_NOT_CONVERT_STRING_TO_NUMBER);
	}
	return value;
}

bool Compilator::valid_syntax_basic_funciton(const string& basicFuncCode) const
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