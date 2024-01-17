//Program 1


#include <iostream>
#include <string>
using namespace std;


struct nodeType{
	char data;
	struct nodeType *next;
	struct nodeType *prev;
} *head,*newNode,*list,*current,*top;

void Push(char new_data){
	newNode=new nodeType;
	newNode->data=new_data;
	newNode->prev=top;
	newNode->next=nullptr;
	top->next=newNode;
	top=newNode;
};

char Pop(){
	char new_data=top->data;
	top=top->prev;
	return new_data;
};

char Top(){
	char new_data=top->data;
	return new_data;
};


bool prompt(string choice){
    bool YesNo = false;
    string text;

    do{
       cout << choice << endl; 
       cout << "(y = yes, n = no); " << endl;
       cin >> text;

    for(int i=0; i<text.length(); i++) text[i] = tolower(text[i]);
        if(text == "yes") text = "y";
        if(text == "no") text = "n";
        if(text == "y") YesNo = true;
        if(text == "n") YesNo = false;
    }
    while(text != "y" && text != "n");
        return YesNo;
}


int main(){
	list=new nodeType;
	head=list;
	top=head;
	
	char palindrome[1000];
    while (true){
	cout<<"Enter a word: ";
    cin >> palindrome;

    int i = 0, size = 0;
	char temp[1000];
	while(palindrome[i]!='\0'){
			temp[size]=tolower(palindrome[i]);
			size++;
		i++;
	}
				
	for(i=0;i<=size/2-1;i++)
		Push(temp[i]);  

	if(size%2==1)
		i++;

	bool palindrometf=true;
	while(i<size){
		if(Pop()!=temp[i]){
			palindrometf=false;
			break;
		}
		i++;
	}

	if(palindrometf)
		cout<<"The word entered is a Palindrome" << endl;
	else {
		cout<<"The word entered is not a Palindrome"<< endl; 

    }

bool answer = prompt("Enter more words? ");
    if (answer) {
        cout << "" << endl;
        continue;
    }
    else{
        break;
    }

    } 
    cout << "Goodbye." << endl;

}

