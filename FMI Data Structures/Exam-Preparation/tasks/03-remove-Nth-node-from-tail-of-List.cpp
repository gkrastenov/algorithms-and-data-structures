  struct ListNode {
      int val;
      ListNode *next;
     ListNode() : val(0), next(nullptr) {}
      ListNode(int x) : val(x), next(nullptr) {}
      ListNode(int x, ListNode *next) : val(x), next(next) {}
  };


    ListNode* removeNthFromEnd(ListNode* head, int n) {
        int count = 0;
        auto temp = head;
        while (temp != nullptr)
        {
            ++count;
            temp = temp->next;
        }
        if (count == n)
        {
            auto helper = head;
            head = head->next;
            delete helper;
            return head;
        }

        int pos = count - n;
        count = 0;
        temp = head;
        while (temp != nullptr)
        {
            ++count;
            if (count == pos)
            {
                if (temp->next != nullptr)
                {
                    auto helper = temp->next->next;
                    delete temp->next;
                    temp->next = helper;
                    return head;
                }
               
                    
            }
            temp = temp->next;
        }
        return head;
    }
