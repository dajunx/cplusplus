#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
using namespace std;
// Definition for singly-linked list.
struct ListNode
{
	int val;
	ListNode *next;
	ListNode(int x) : val(x), next(NULL) {}


};
ListNode* get_last_node(ListNode* head = NULL)
{
	ListNode* end_node = NULL;
	while(head)
	{
		end_node = head;
		head = head->next;
	}
	return end_node;
}
void add_item(ListNode* head, int new_item)
{
	ListNode* last_node = NULL;
	last_node = get_last_node(head);
	ListNode* temp = NULL;
	temp = new ListNode(new_item);
	last_node->next = temp;
}
// Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
// Output: 7 -> 0 -> 8
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2)
{
	ListNode* res_node = NULL;
	int carry = 0; // 进位
	for (;l1 != NULL || l2 != NULL;)
	{
		int iSum = 0;
		if(l1) iSum += l1->val;
		if(l2) iSum += l2->val;
		iSum += carry;
		carry = 0;
		if (iSum >= 10)
		{
			if(res_node == NULL) 
				res_node = new ListNode(iSum - 10);
			else
				add_item(res_node,iSum - 10);
			carry = 1; // 有进位
		} 
		else if(iSum < 10 && iSum >= 0)
		{
			if(res_node == NULL)
				res_node = new ListNode(iSum);
			else
				add_item(res_node,iSum);
		}
		if(l1) l1 = l1->next;
		if(l2) l2 = l2->next;
	}
	if( 0 != carry) // 特殊处理尾端有进位值
		add_item(res_node,1);
	return res_node;
}

int main(int argCount,char ** args)
{
	// 	unordered_map<int,int> unorder_test;
	// 	unorder_test[1] = 2;
	// 	unorder_test.find(1);
	//产生数据
	ListNode * l1 = new ListNode(1);
	ListNode * l2 = new ListNode(0);
	add_item(l1,8);
	addTwoNumbers(l1,l2);
	return 0;
};