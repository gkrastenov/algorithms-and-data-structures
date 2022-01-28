#include <string>
#include <iostream>
using namespace std;

struct Node {
    Node* next{};
    int data{};
    Node() {};
    Node(Node* next, const int data) : next(next), data(data) {}
    Node(const int data) : next(nullptr), data(data) {}
};

class LList
{
private:
	Node* head;
	Node* tail;
	size_t sz;
public:
	LList() : head(nullptr), tail(nullptr), sz(0) {};
    LList(const LList& other)
    {
        copy(other);
    }
    LList& operator=(const LList& rhs)
    {
        if (this != &rhs) {
            free();
            copy(rhs);
        }

        return *this;
    }
    ~LList()
    {
        free();
    }

    Node* front() const { return head; } // TODO: check for null
    Node* back() const { return tail; }  // TODO: check for null

    bool is_empty() const { return sz == 0; }
    size_t size() const { return sz; }

    void push_end(const int val)
    {
        if (head == nullptr) {
            head = new Node(val);
            sz++;
            return;
        }
        else if (head != nullptr && tail == nullptr)
        {
            tail = new Node(val);
            head->next = tail;
            sz++;
            return;
        }    
        tail->next = new Node(val);
        tail = tail->next;
        ++sz;
    }
    string print()
    {
        string result = "";
        auto temp = head;
        while (temp != nullptr)
        {
            result += to_string(temp->data);
            result.push_back(' ');
            temp = temp->next;
        }
        return result;
    }
private:
	void free() {
        Node* iter = head;
        Node* detach = head;

        for (size_t i = 0; i < sz; ++i) {
            detach = iter;
            iter = iter->next;
            delete detach;
        }
        tail = head = nullptr;
        sz = 0;
	}
    void copy(const LList& other)
    {
        Node dummy;
        Node* iter = &dummy;
        Node* curr = other.head;
        while (curr) {
            iter->next = new Node(curr->data);
            iter = iter->next;
            curr = curr->next;
        }
        head = dummy.next;
        tail = head ? iter : nullptr;
        sz = other.sz;
    }
};

LList findMaxSubList(const LList& list) {
    LList result;

    auto startNode = list.front();
    int sum = startNode->data;
    int size = 1;

    auto temp = list.front();
    while (temp != nullptr)
    {
        // in descending order
        auto helper = temp;     
        int currSize = 1;
        int currSum = helper->data;
        while (helper != nullptr)
        {
            if (helper->next != nullptr)
            {
                if (helper->data > helper->next->data == false)
                    break;
                ++currSize;
                currSum += helper->next->data;
            }
            helper = helper->next;         
        }
        if (currSize > size) {
            startNode = temp;
            size = currSize;
            sum = currSum;
        }else if (currSize == size){
            if (currSum > sum)
            {
                startNode = temp;
                sum = currSum;
            }
        }

        // in ascending order
        helper = temp;
        currSize = 1;
        currSum = helper->data;
        while (helper != nullptr)
        {
            if (helper->next != nullptr)
            {
                if (helper->data < helper->next->data == false)
                    break;
                ++currSize;
                currSum += helper->next->data;
            }
            helper = helper->next;
        }
        if (currSize > size) {
            startNode = temp;
            size = currSize;
            sum = currSum;
        }
        else if (currSize == size) {
            if (currSum > sum)
            {
                startNode = temp;
                sum = currSum;
            }
        }
        temp = temp->next;
    }

    while (size != 0  && startNode != nullptr)
    {
        result.push_end(startNode->data);
        startNode = startNode->next;
        --size;
    }
    return result;
}
int main()
{
	// TODO: optimize solution
    // 2 -> 3 -> 2 -> 1 -> 0 -> 3 -> 2
    LList list1;
    list1.push_end(2);
    list1.push_end(3);
    list1.push_end(2);
    list1.push_end(1);
    list1.push_end(0);
    list1.push_end(3);
    list1.push_end(2);
	// 3 -> 2 -> 1 -> 0
    auto result1 = findMaxSubList(list1);
    string output1 = result1.print();

    LList list2;
    list2.push_end(2);
    list2.push_end(4);
    list2.push_end(5);
    list2.push_end(1);
    list2.push_end(0);
    list2.push_end(3);
    list2.push_end(2);
    auto result2 = findMaxSubList(list2);
    string output2 = result2.print();
}