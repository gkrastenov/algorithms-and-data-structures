#include <iostream>
using namespace std;

struct Node {
public:
    int data{};
    Node* next{};
    Node(const int data, Node* next = nullptr)
        : data(data),
         next(next) {};     
    ~Node()
    {

    }
};
class List {
public:
    size_t sz{};
    Node* head{};
    Node* tail{};

    List() = default;
    ~List()
    {
        free();
    }
    void free()
    {
        auto temp = head;
        while (temp != nullptr)
        {
            auto dummy = temp;
            temp = temp->next;
            delete dummy;
        }
        head = tail = nullptr;
        sz = 0;
    }

    void push_back(const int data)
    {
        if (head == nullptr)
        {
            head = new Node(data);
            ++sz;
            return;
        }

        if (sz == 1 && tail == nullptr)
        {
            tail = new Node(data);
            head->next = tail;
            ++sz;
            return;
        }
        
        tail->next = new Node(data);
        tail = tail->next;
        ++sz;     
        return;
    }
    Node* front() { return head; }
};


void ascendingInsertionSor_util(Node* nod, Node* sorted)
{
    if (sorted == nullptr || sorted->data >= nod->data)
    {
        nod->next = sorted;
        sorted = nod;
    }
    else {
        auto curr = sorted->next;
        while (curr->next != nullptr && curr->next->data < nod->data)
        {
            curr = curr->next;
        }
        nod->next = curr->next;
        curr->next = nod;
    }
}

void ascendingInsertionSort(List& list)
{
    // 3->7->1->17
    Node* sorted = nullptr;

    auto temp = list.head;
    while (temp != nullptr)
    {
        auto nxt = temp->next;
        
        if (sorted == nullptr || sorted->data >= temp->data)
        {
            temp->next = sorted;
            sorted = temp;
        }
        else {
            auto curr = sorted;
            while (curr->next != nullptr && curr->next->data < temp->data)
            {
                curr = curr->next;
            }
            temp->next = curr->next;
            curr->next = temp;
        }
        temp = nxt;
    }
    list.head = sorted;
}

void descendingInsertionSort(List& list)
{
    Node* sorted = nullptr;

    auto temp = list.head;
    while (temp != nullptr)
    {
        auto nxt = temp->next;

        if (sorted == nullptr || sorted->data <= temp->data)
        {
            temp->next = sorted;
            sorted = temp;
        }
        else {
            auto curr = sorted;
            while (curr->next != nullptr && curr->next->data > temp->data)
            {
                curr = curr->next;
            }
            temp->next = curr->next;
            curr->next = temp;
        }
        temp = nxt;
    }
    list.head = sorted;   
}

Node* removeDuplicates(Node* head)
{
    auto temp = head;
    while (head != nullptr && head->next != nullptr)
    {
        if (head->data == head->next->data)
        {
            head->next = head->next->next;
        }
        else
        {
            head = head->next;
        }
    }
    return temp;
}

// 3 -> 30 -> 2 -> 8 -> 8 -> 8 ->7 ->1 ->2 ->17
void makeSort(List& list)
{
    List oddList;
    List evenList;

    auto temp = list.front();
    while (temp != nullptr)
    {
        if (temp->data % 2 == 0)
            evenList.push_back(temp->data);
        else oddList.push_back(temp->data);

        temp = temp->next;
    }

    // 30->2->8->8->8->2
    ascendingInsertionSort(evenList); // 2->2->8->8->8->30
    // 3->7->1->17
    descendingInsertionSort(oddList); // 17->7->3->1

    auto headEven = evenList.front();
    headEven = removeDuplicates(headEven); // 2->8->30

    auto headOdd = oddList.front();
    headOdd = removeDuplicates(headOdd); // 17->7->3->1

    list.free();
    while (headEven != nullptr)
    {
        list.push_back(headEven->data);
        headEven = headEven->next;
    }

    while (headOdd != nullptr)
    {
        list.push_back(headOdd->data);
        headOdd = headOdd->next;
    }
}
int main()
{
    List list;
    list.push_back(3);
    list.push_back(30);
    list.push_back(2);
    list.push_back(8);
    list.push_back(8);
    list.push_back(8);
    list.push_back(7);
    list.push_back(1);
    list.push_back(2);
    list.push_back(17);
    makeSort(list);
}