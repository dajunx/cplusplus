#include "define.h"
#include <string>
#include <unordered_map>
#include <map>
// Definition for singly-linked list.
struct ListNode
{
	int val;
	ListNode *next;
	ListNode(int x) : val(x), next(NULL) {}
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
};
// Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
// Output: 7 -> 0 -> 8
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2)
{
	ListNode* res_node = NULL;
	for (;l1 != NULL && l2 != NULL;)
	{
		int iSum = l1->val + l2->val;
		static int carry = 0; // 进位
		iSum += carry;
		carry = 0;
		if (iSum >= 10)
		{
			if(res_node == NULL) 
				res_node = new ListNode(iSum - 10);
			else
				res_node->add_item(res_node,iSum - 10);
			carry = 1; // 有进位
		} 
		else if(iSum < 10 && iSum >= 0)
		{
			if(res_node == NULL)
				res_node = new ListNode(iSum);
			else
				res_node->add_item(res_node,iSum);
		}
		l1 = l1->next;
		l2 = l2->next;
	}
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
	l1->add_item(l1,8);
	//l1->add_item(l1,3);

	//l2->add_item(l2,6);
	//l2->add_item(l2,4);

	addTwoNumbers(l1,l2);
	return 0;
};