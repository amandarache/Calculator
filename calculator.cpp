#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
#include <queue>
#include <cmath>
using namespace std;

//check if variable is being assigned to a value
bool isAssignment(string line) {
	string assign = "let ";
	if(line.substr(0,4) == assign)
		return true;
	else return false;
}

//check if variable exists in hash map
bool varExists(unordered_map<string, double> variables, string var) {
	return !(variables.find(var) == variables.end());
}

//check if char at line[i] is operator
bool isOperator(char op) {
	if(op == '+' || op == '-' || op == '/' || op == '*' || op == '^')
		return true;
	else return false;
}

//get operator's associativity (left=0, right=1)
//power ^ is the only operator with right associativity
int opAssoc(char op) {
	int assoc;
	if(op == '+' || op == '-' || op == '*' || op == '/')
		assoc = 0;
	else assoc = 1;
	return assoc;
}

//get precedence of operator
int opPrecedence(char op) {
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
bool leftVsRight(char op1, char op2) {
	if((opAssoc(op1) == 0 && opPrecedence(op1) <= opPrecedence(op2)) ||
	 (opAssoc(op1) == 1 && opPrecedence(op1) < opPrecedence(op2))) {
		return true;
	} else return false;
}

//print result of expressiom
void print(stack<double> result) {
	cout << result.top();
}

//compute postfix expression
void compute(queue<string> output, unordered_map<string, double> variables, bool newVar) {
	stack<double> result;
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
				cout << "newVar";
				var = output.front();
				newVar = false;
			}
			else if(varExists(variables,check) && !newVar) {
				result.push(variables[check]);
			}
			else if(!varExists(variables,check) && !newVar) {
				cout << "Undeclared-Variable";
				exception = true;
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
					cout << "Division-By-Zero";
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
	//set new variable and put in hash map
	if(assignment && !exception) {
		variables[var] = result.top();
	}
	//cout << variables[var];
	//print only if there was no exception (divide by zero or unassigned variable)
	//and if there was no variable assignment
	if(!exception && !assignment) print(result);

}

//parse expression and convert to postfix
void parsePostfix(string line, unordered_map<string, double> variables) {
	string num, var;
	stack<char> op;
	queue<string> output;
	//Make global variable so compute and postfic can access it?
	// unordered_map<string, double> variables;
	bool newVar;

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
				cout << "assignment" << endl;
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
	//cout << var;

	//print postfix expression// while(!output.empty()) {// 	string a = output.front();
	// 	cout << a;// 	output.pop();// }

	/* UNCOMMENT */
	//pop remaining operators from stack to queue
	while(!op.empty()) {
		string top(1,op.top());
		output.push(top);
		op.pop();
	}

	/* UNCOMMENT */
	compute(output, variables, newVar);
}

int main() {
	string line;
	unordered_map<string, double> variables;
	getline(cin, line);
	if(line.find("let ") == -1)
		line.erase(remove(line.begin(),line.end(),' '),line.end());

	while(line != "quit") {
		parsePostfix(line, variables);
		cout << endl;
		getline(cin, line);
		if(line.find("let ") == -1)
			line.erase(remove(line.begin(),line.end(),' '),line.end());
	}

	// unordered_map<string, double> variables;
	// variables["amanda"] = 21;
	// variables["ale"] = 22;
	// if(variables.find("amanda") == variables.end()) {
	// 	cout << "nope";
	// }
	// else cout << "yas";
	// cout << variables["ale"];

	return 0;
}