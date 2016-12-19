#include <iostream>
#include <string>

using namespace std;

struct Node{
	string value;														//Sets the value of that Node
	struct Node *next;													//Has a pointer to point to the next Node
};

class HashTable{
private:
	int n;																//This is the size of your hashtable
	string item;														//Item that is stored in the hashtable
	Node** A;															//This is the HashTable structure
	
public:
	HashTable();														//Initializes constructor class
	int hash_function(int n, string item);								//Initializes hash function
	void addItem(string item);											//Store string values into the nodes
	void searchItem(string item);										//Search for the string value in each node
};

HashTable::HashTable(){
	cin >> n; 
	this->item = item;													//This is the string that is passed through
	A = new Node*[n];													//Sets the values of the array equal to a node


	for(int i=0; i < n-1; i++){											//Initializes the array		
		A[i] = new Node;												//Each index is an array index
		A[i]->next = NULL;												//Each pointer for the array index is set to NULL
		A[i]->value = "These Values in the array will be empty";		//The value of each array index is empty
	}
}

int HashTable::hash_function(int n, string item){
	//This function is used to convert each string item into a key value
	//The key value is denoted as hash and this int value is therefore indexed
	//on the array. The string value that is passed in will therefore take up this value.
	
		int counter1 = 0;												
		for (int i=0; i< (int) item.length(); i++){
			counter1 += item[i];
		}
		int hash = counter1 % n;
		return hash;
}

void HashTable::addItem(string item){									
	int currentValue = hash_function(n, item);							//Hash_function is called to find that key value of the string
	Node *movingPoint;													//Pointer is used to point to the nodes along the "Linkedlist" index
	Node *lastPoint = new Node();										//Creation of a new node
	lastPoint->value = item;											//Sets the particular value of the node to the string value
	lastPoint->next = NULL;												//Last node will therefore point to nothing or "NULL"

	
	if(A[currentValue]->value == "Empty"){								//Head is empty?
		A[currentValue]->value = item;									//Insert this node into the top of the array index
	}
	else{																//Head is filled?
		movingPoint = A[currentValue];									//The pointer is used to point to the initial value
		
		while(movingPoint->next != NULL){								//While the last pointer is not Null or empty
			movingPoint = movingPoint->next;							//Move the pointer to the next node
		}																//Exits once the nodes are traversed
		
		movingPoint->next = lastPoint;									//Sets the new node to the last node of the linkedlist
	}
}


void HashTable::searchItem(string item){								//Function used to search for the string value
	int currentValue = hash_function(n, item);							//Hash_function is called to find that key value of the string
	Node *movingPoint;													//This moving pointer is used to traverse through the linkedlist
	bool counter = false;
	
	if(A[currentValue]->value == item){									//Head is equal to the item value?
		cout << "Yes" << endl;											//Found the item!
	}
		
	else{																//If not?
		movingPoint = A[currentValue];									//Set a pointer equal to the hash index
		while(movingPoint != NULL){										//While the pointer is null
			if(movingPoint->value == item){								//if the value is there at that point
				cout << "Yes" << endl;									//Found the item!
				counter = true;											//Counter used to stop an extra "No"
			}
			
			movingPoint = movingPoint->next;							//Pointer is moved to the next node
		}
		
		if(counter == false){											//At this point the program has not found a yes
			cout<< "No" << endl;										//So, print No
		}
			
	}
}

int main() {
	HashTable hashtable = HashTable();									//Initializes the Hashtable
	bool counter = true;												//Sets a counter to true
	while(counter){														//Runs the function until opcode = 0
		int opcode;														
		string item;													
		cin >> opcode;													//Value that is passed in by user
		cin >> item;													//Value that is passed in by user
		if(opcode == 0){												//This will exit the program
			counter = false;											//Counter set to false will leave program
		}
		else if(opcode == 1){											//Opcode 1 will insert a string item
			hashtable.addItem(item);									//Adds a node or a string item value
		}
		
		else if(opcode == 2){
			hashtable.searchItem(item);									//Searches for the string value passed in
		}
	}
	return 0;															//Terminates the Program
}

