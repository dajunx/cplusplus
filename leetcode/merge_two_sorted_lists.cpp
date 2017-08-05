//leetcode:Merge Two Sorted Lists
/*
Merge two sorted linked lists and return it as a new list. 
The new list should be made by splicing together the nodes of the first two lists.
*/
#include <iostream> 

//Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};


ListNode* mergeTwoLists(ListNode* l1, ListNode* l2)
{
  //both l1 and l2 were null list
  if(NULL == l1 && NULL == l2) {
    return NULL;
  }

  ListNode* head_tmp = NULL;
  ListNode* last_node = NULL;
  int node_val = 0;
  while(true)
  {
    if(NULL == l1 && NULL != l2) {
      node_val = l2->val;
      l2 = l2->next;
    } else if (NULL == l2 && NULL != l1) {
      node_val = l1->val;
      l1 = l1->next;
    } else if (NULL != l1 && NULL != l2) {
      if (l1->val <= l2->val) {
        node_val = l1->val;
        l1 = l1->next;
      } else {
        node_val = l2->val;
        l2 = l2->next;
      }
    } else if(NULL == l1 && NULL == l2) {
      break;
    }

    ListNode* pNode = new ListNode(0);
    pNode->val = node_val;
    pNode->next = NULL;

    if(last_node) {
      last_node->next = pNode;
    }

    last_node = pNode;

    if(head_tmp == NULL) {
      head_tmp = pNode;
    }
  }

  return head_tmp;
}

void init_list_data(ListNode** head, int start_pos, int length)
{
  ListNode* head_tmp = NULL;
  ListNode* last_node = NULL;
  for (int i=start_pos; i < start_pos + length; ++i)
  {
    ListNode* pNode = new ListNode(0);
    pNode->val = i;
    pNode->next = NULL;

    if(last_node) {
      last_node->next = pNode;
    }

    last_node = pNode;

    if(head_tmp == NULL) {
      head_tmp = pNode;
    }
  }

  *head = head_tmp;
}

int main(int argc, char** argv)
{
  ListNode* head1 = NULL;
  ListNode* head2 = NULL;

  //≥ı ºªØ¡¥±Ì1
  init_list_data(&head1, 0, 5);
  init_list_data(&head2, 5, 5);

  mergeTwoLists(head1, head2);

  int i = 0;
  return 0;  
}