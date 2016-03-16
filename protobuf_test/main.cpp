#include <iostream>
#include <string>
#include <fstream>
#include "lm.helloworld.pb.h"
using namespace std;

void ListMsg(const lm::helloworld & msg)
{
	cout << msg.id() << endl; 
	cout << msg.str() << endl; 
}

int main()
{
	lm::helloworld msg1;
	msg1.set_id(101);
	msg1.set_str("hello");

	// Write the new address book back to disk. 
	fstream output("./log", ios::out | ios::trunc | ios::binary);

	if (!msg1.SerializeToOstream(&output))
	{
		cerr << "Failed to write msg." << endl;
		return -1;
	}

	// write
	lm::helloworld msg2;
	{
		fstream input("./log", ios::in | ios::binary);
		if (!msg2.ParseFromIstream(&input))
		{
			cerr << "Failed to parse address book." << endl;
			return -1;
		}
	}
	ListMsg(msg2);

	return 0;
}