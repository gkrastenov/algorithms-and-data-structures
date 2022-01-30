  struct ListNode {
      int val;
    ListNode *next;
     ListNode() : val(0), next(nullptr) {}
      ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
 };

class Solution {
public:
    ListNode* insertionSortList(ListNode* head) {
        ListNode* sorted = nullptr;
        auto temp = head;
        while (temp != nullptr)
        {
            auto nxt = temp->next;

            if (sorted == nullptr || sorted->val >= temp->val) {
                temp->next = sorted;
                sorted = temp;
            }
            else {
                // 2 5 | 3
                auto curr = sorted;
                while (curr->next != nullptr && curr->next->val < temp->val)
                {
                    curr = curr->next;
                }
                temp->next = curr->next;
                curr->next = temp;
            }
            temp = nxt;
        }

        return sorted;
    }
};