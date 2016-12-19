#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
#include <queue>
#include <cmath>
using namespace std;

class Calculator {
private:
	//queue<string> output;
	//stack<char> op;
	stack<double> result;
	unordered_map<string, double> variables;

public:
	Calculator();
	~Calculator();
	bool isAssignment(string line);
	bool varExists(string var);
	bool isOperator(char op);
	int opAssoc(char op);
	int opPrecedence(char op);
	bool leftVsRight(char op1, char op2);
	void print();
	void compute(queue<string> output, bool newVar);
	void parsePostfix(string line);

};

//constructor
Calculator :: Calculator() {

}

//check if variable is being assigned to a value
bool Calculator :: isAssignment(string line) {
	string assign = "let ";
	if(line.substr(0,4) == assign)
		return true;
	else return false;
}

//check if variable exists in hash map
bool Calculator :: varExists(string var) {
	return !(variables.find(var) == variables.end());
}

//check if char at line[i] is operator
bool Calculator :: isOperator(char op) {
	if(op == '+' || op == '-' || op == '/' || op == '*' || op == '^')
		return true;
	else return false;
}

//get operator's associativity (left=0, right=1)
//power ^ is the only operator with right associativity
int Calculator :: opAssoc(char op) {
	int assoc;
	if(op == '+' || op == '-' || op == '*' || op == '/')
		assoc = 0;
	else assoc = 1;
	return assoc;
}

//get precedence of operator
int Calculator :: opPrecedence(char op) {
	int prec;
	switch(op) {
		case '+': prec = 1; break;
		case '-': prec = 1; break;
		case '*': prec = 2; break;
		case '/': prec = 2; break;
		case '^': prec = 3; break;
	}
	return prec;
}

//check precedence of line[i] operator vs op.top() operator and it's associativity
//condition for while loop in parsePostfix
bool Calculator :: leftVsRight(char op1, char op2) {
	if((opAssoc(op1) == 0 && opPrecedence(op1) <= opPrecedence(op2)) ||
	 (opAssoc(op1) == 1 && opPrecedence(op1) < opPrecedence(op2))) {
		return true;
	} else return false;
}

//print result of expressiom
void Calculator :: print() {
	cout << result.top() << endl;
}

//compute postfix expression
void Calculator :: compute(queue<string> output, bool newVar) {
	string var;
	bool assignment = newVar;
	bool exception = false;

	//go through everything in postfix expression
	while(!output.empty()) {
		string check = output.front();
		char i = check[0];

		//is an operand
		if(isdigit(i)) {
			double num = stod(output.front());
			result.push(num);
		}
		//is a variable
		else if(isalpha(i)) {
			if(newVar) {
				//cout << "newVar";
				var = output.front();
				newVar = false;
			}
			else if(varExists(check) && !newVar) {		
				result.push(variables[check]);
			}
			else if(!varExists(check) && !newVar) {
				cout << "Undeclared-Variable" << endl;
				exception = true;
				// while(!output.empty()) output.pop();
				break;
			}
		}
		//is an operator
		else if(isOperator(i)) {
			double b = result.top();
			result.pop();
			double a = result.top();
			result.pop();

			if(output.front() == "+") {
				result.push(a+b);
			}
			else if(output.front() == "-") {
				result.push(a-b);
			}
			else if(output.front() == "*") {
				result.push(a*b);
			}
			else if(output.front() == "/") {
				if(b == 0) {
					cout << "Division-By-Zero" << endl;
					exception = true;
					break;
				} else result.push(a/b);
			}
			else if(output.front() == "^") {
				result.push(pow(a,b));
			}
		}
		output.pop();
	}
	//set new variable and put in hash map if there was no exception
	if(assignment && !exception) {
		variables[var] = result.top();
	}
	//print only if there was no exception (divide by zero or unassigned variable)
	//and if there was no variable assignment
	if(!exception && !assignment) print();

}

//parse expression and convert to postfix
void Calculator :: parsePostfix(string line) {
	string num, var;
	queue<string> output;
	stack<char> op;
	bool newVar = false;

	for(int i = 0; i < line.size(); i++) {
		if(line[i] == ' ') continue;

		//is a number
		if(isdigit(line[i]) || line[i] == '.') {
			int start = i;
			//get full number if more than one digit
			while(isdigit(line[i]) || line[i] == '.') {
				i++;
			}
			num = line.substr(start, i - start);
			output.push(num);
			i--;
		}
		//is an operator
		else if(isOperator(line[i])) {
			while(!op.empty() && isOperator(op.top()) && leftVsRight(line[i],op.top())) {
			 	string top(1,op.top());
				output.push(top);
				op.pop();
			}
			op.push(line[i]);
		}
		//is an open parenthesis
		else if(line[i] == '(') {
			op.push(line[i]);
		}
		//is a closing parenthesis
		else if(line[i] == ')') {
			while(!op.empty() && op.top() != '(') {
				string top(1,op.top());
				output.push(top);
				op.pop();
			}
			op.pop();
		}
		//is alphabetic character
		else if(isalpha(line[i])) {
			if(line.substr(i,4) == "let ") {
				newVar = true;
				i += 2;
			}
			else {
				var = "";
				//get full variable name
				while(isalpha(line[i])) {
					var = var + line[i];
					i++;
				}
				output.push(var);
				i--;
			}
		}
	}

	//pop remaining operators from stack to queue
	while(!op.empty()) {
		string top(1,op.top());
		output.push(top);
		op.pop();
	}

	//if only one operand, check if needs to be printed
	if(output.size() == 1) {
		string s = output.front();
		if(isdigit(s[0])) {
			cout << s << endl;
		}
		else if(isalpha(s[0])) {
			if(varExists(s)) cout << variables[s] << endl;
			else cout << "Undeclared-Variable" << endl;
		}
		output.pop();
	} else compute(output, newVar);
}

int main() {
	string line;
	Calculator* myCalc = new Calculator();
	getline(cin, line);
	if(line.find("let ") == -1)
		line.erase(remove(line.begin(),line.end(),' '),line.end());

	while(line != "quit") {
		myCalc->parsePostfix(line);
		getline(cin, line);
		if(line.find("let ") == -1)
			line.erase(remove(line.begin(),line.end(),' '),line.end());
	}
	return 0;
}