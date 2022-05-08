#include "interface.h"

int Hierarchy::getHeight(const node* root) const
{
    if (!root) return 0;
    return std::max(1 + getHeight(root->child), getHeight(root->brother));
}

bool Hierarchy::hire(const string& who, const string& boss)
{
    if (who == "Uspeshnia")
        return false;

    node* foundedParent = find_and_return(boss, root); // Time: O(n), Memory O(n) 
    if (foundedParent == nullptr)
        return false;

    node* toBeAdded = find_and_return(who, root);
    if (toBeAdded == nullptr){   
        foundedParent->child = new node(who, nullptr, foundedParent->child);
        size++;
        return true;
    }
    else {
        node* parent = get_parent(who, root);
        if (parent->child->data == who) {
            node* temp = parent->child;
            parent->child = toBeAdded->brother;
            temp->brother = foundedParent->child;
            foundedParent->child = temp;
            return true;
        }
        else {
            node* util = parent->child;
            while (util->brother->data != who)
            {
                util = util->brother;
            }
            node* temp = util->brother;
            util->brother = util->brother->brother;
            temp->brother = foundedParent->child;
            foundedParent->child = temp;
            return true;
        }
    }
    return false;
}

void Hierarchy::incorporate()
{
    if (root == nullptr)
        return;

    std::stack<node*> toBeIncorporate = who_to_incorporate();
    while (!toBeIncorporate.empty())
    {
        node* curr = toBeIncorporate.top();
        toBeIncorporate.pop();

        if (curr->child) {
            node* parent = get_parent(curr->data, root);
            if (parent->child->data == curr->data) {
                node* temp = curr->child;
                while (temp->brother)
                {
                    temp = temp->brother;
                }
                temp->brother = curr->brother;
                curr->brother = nullptr;
            }
            else {
                node* temp = parent->child;
                while (temp->brother->data != curr->data)
                {
                    temp = temp->brother;
                }
                temp->brother = curr->brother;

                if (temp->brother) {
                    while (temp->brother)
                    {
                        temp = temp->brother;
                    }
                    temp->brother = curr->child;
                    curr->brother = nullptr;
                }
                else {
                    temp->brother = curr->child;
                }
                node* util = parent->child;
                parent->child = curr;
                curr->child = util;
            }
        }
        else {
            node* parent = get_parent(curr->data, root);
            curr->child = curr->brother;
            curr->brother = nullptr;
        }
    }
}

void Hierarchy::modernize()
{
	if (root == nullptr) 
        return;

    std::stack<node*> toBeModernize = who_to_modernize();
    while (!toBeModernize.empty())
    {
        fire(toBeModernize.top());
        toBeModernize.pop();
    }
}

Hierarchy Hierarchy::join(const Hierarchy& right) const
{
    if (this == &right)
        return Hierarchy(right);

    if (right.getSize() == 1)
        return Hierarchy(*this);

    Hierarchy joinedHierarchy(*this);
    std::queue<node*> queue;
    queue.push(right.root->child);
    while (!queue.empty())
    {
        node* curr = queue.front();
        queue.pop();

        if (curr->brother != nullptr)
            queue.push(curr->brother);

        if (curr->child != nullptr)
            queue.push(curr->child);

        if (joinedHierarchy.find(curr->data)) {
            node* firstHierarchyParent = get_parent(curr->data, joinedHierarchy.root);
            node* secondHierarchyParent = right.get_parent(curr->data, right.root);

            if (firstHierarchyParent->data != secondHierarchyParent->data) {
                // A join is not possible if there is a situation in which employee X is subordinate to Y in one
                // hierarchy (directly or not) and vice versa in the other. In this case, you do not have to create a 
                // joined hierarchy.
                if (right.find_and_return(firstHierarchyParent->data, curr) != nullptr)
                    return Hierarchy();

                node* parent = joinedHierarchy.get_parent(curr->data, joinedHierarchy.root);
                if (parent->child->data == curr->data) {
                    parent->child = parent->child->brother;
                }
                else {
                    node* helper = parent->child;
                    while (helper->brother->data != curr->data)
                    {
                        helper = helper->brother;
                    }
                    helper->brother = helper->brother->brother;

                }             
                joinedHierarchy.size--;

                // if parents data are not qeual then compare by level
                int firstParentLevel = joinedHierarchy.get_node_level(firstHierarchyParent->data, joinedHierarchy.root);
                int secondParentLevel = right.get_node_level(secondHierarchyParent->data, right.root);

                // if levels of parents are equal then compare by data
                if (firstParentLevel == secondParentLevel) {
                    //  1.Zero is returned when the strings being compared are lexicographically equal
                    //  2.Greater than 0 (> 0) : this is returned when the first string is lexicographically greater than the second string
                    //  3.Less than 0 (< 0) : this is returned when the first string is lexicographically smaller than the second string
                    int value = firstHierarchyParent->data.compare(secondHierarchyParent->data);

                    // first has bigger data than second
                    if (value > 0) {
                        node* joinedParent = joinedHierarchy.find_and_return(secondHierarchyParent->data, joinedHierarchy.root);
                        node* joinedChild = find_and_return(curr->data, root);
                        if (joinedParent->child == nullptr){
                            joinedParent->child = new node(joinedChild->data, joinedHierarchy.copy(joinedChild->child), nullptr);
                        }
                        else {
                            node* util = joinedParent->child;
                            while (util->brother)
                            {
                                util = util->brother;
                            }
                            util->brother = new node(joinedChild->data, joinedHierarchy.copy(joinedChild->child), nullptr);
                        }
                        joinedHierarchy.size++;
                    }
                    if (value < 0) {
                        node* joinedParent = joinedHierarchy.find_and_return(firstHierarchyParent->data, joinedHierarchy.root);
                        node* joinedChild = right.find_and_return(curr->data, right.root);
                        if (joinedParent->child == nullptr) {
                            joinedParent->child = new node(joinedChild->data, joinedHierarchy.copy(joinedChild->child), nullptr);
                        }
                        else {
                            node* util = joinedParent->child;
                            while (util->brother)
                            {
                                util = util->brother;
                            }
                            util->brother = new node(joinedChild->data, joinedHierarchy.copy(joinedChild->child), nullptr);
                        }
                        joinedHierarchy.size++;
                    }
                }

                if (firstParentLevel < secondParentLevel) {
                    node* joinedParent = joinedHierarchy.find_and_return(firstHierarchyParent->data, joinedHierarchy.root);
                    node* joinedChild = right.find_and_return(curr->data, right.root);
                    if (joinedParent->child == nullptr) {
                        joinedParent->child = new node(joinedChild->data, joinedHierarchy.copy(joinedChild->child), nullptr);
                    }
                    else {
                        node* util = joinedParent->child;
                        while (util->brother)
                        {
                            util = util->brother;
                        }
                        util->brother = new node(joinedChild->data, joinedHierarchy.copy(joinedChild->child), nullptr);
                    }
                    joinedHierarchy.size++;
                }          

                if (firstParentLevel > secondParentLevel) {
                    node* joinedParent = joinedHierarchy.find_and_return(secondHierarchyParent->data, joinedHierarchy.root);
                    node* joinedChild = this->find_and_return(curr->data, root);
                    if (joinedParent->child == nullptr) {
                        joinedParent->child = new node(joinedChild->data, joinedHierarchy.copy(joinedChild->child), nullptr);
                    }
                    else {
                        node* util = joinedParent->child;
                        while (util->brother)
                        {
                            util = util->brother;
                        }
                        util->brother = new node(joinedChild->data, joinedHierarchy.copy(joinedChild->child), nullptr);
                    }
                    joinedHierarchy.size++;
                }
            }
        }
        else {
            // If an employee meets in only one hierarchy provided in the association, just below the instance of his manager 
            // (as his direct subordinate), together with all his subordinates from the respective;
            node* parent = right.get_parent(curr->data, right.root);
            joinedHierarchy.insert(parent->data, curr->data);
        }
    }
    return joinedHierarchy;
}

Hierarchy::Hierarchy(const string& data)
{
	if (data == "") 
        return;

	create_hierarchy(data);
}

string Hierarchy::print() const
{
    if (root == nullptr)
        return "";
  
    string result = "";
    std::vector<string> lines = print_by_leveles(root);
    for (size_t i = 0; i < lines.size(); i++)
    {
        result += lines[i];
    }
    
    return result;
}

int Hierarchy::longest_chain() const
{
	return getHeight(root);
}

bool Hierarchy::find(const string& name) const
{
	return find_and_return(name, root) != nullptr;
}

int Hierarchy::num_employees() const
{
    return getSize();
}

int Hierarchy::num_overloaded(int level) const
{   
    return num_overloaded(root, level);
}

string Hierarchy::manager(const string& name) const
{
    node* parent = get_parent(name, root);
    if (parent)
        return parent->data;
    return "";
}

int Hierarchy::num_subordinates(const string& name) const
{
    node* foundedNode = find_and_return(name, root);
    if (foundedNode != nullptr)
    {
        int value = 0;
        node* curr = foundedNode->child;
        while (curr != nullptr)
        {
            curr = curr->brother;
            value++;
        }
        return value;
    }
    return -1;
}

unsigned long Hierarchy::getSalary(const string& who) const
{
    node* foundedNode = find_and_return(who, root);
    if (foundedNode == nullptr)
        return -1;

    if (foundedNode->child == nullptr)
        return 0;

    std::stack<node*> stack;
    stack.push(foundedNode->child);

    bool helper = true;
    int directChildren = 0;
    int undirectChildren = 0;
    while (stack.empty() == false)
    {
        node* curr = stack.top();
        stack.pop();

        if (curr->child != nullptr){
            undirectChildren++;
            stack.push(curr->child);
        }

        if (curr->brother != nullptr){
            if (helper)
            {
                directChildren++;
                stack.push(curr->brother);
            }
            else {
                stack.push(curr->brother);
                undirectChildren++;
            }
        }
        else {
            if (helper) 
                directChildren++;

            helper = false;
        }
    }

    return 500 * directChildren + 50 * undirectChildren;
}

bool Hierarchy::fire(const string& who)
{
    if (root == nullptr || who == "Uspeshnia")
        return false;

    node* parent = get_parent(who, root);
    node* toBeRemoved = find_and_return(who, root);

    if (parent == nullptr || toBeRemoved == nullptr) return false;

    if (count_all_subordinates(parent->data) == 1) {
        parent->child = toBeRemoved->child;
        delete toBeRemoved;
        size--;
        return true;
    }
    else {
        if (parent->child->data == who){
            parent->child = toBeRemoved->brother;
            node* curr = toBeRemoved->brother;
            while (curr->brother)
            {
                curr = curr->brother;
            }
            curr->brother = toBeRemoved->child;
            delete toBeRemoved;
            size--;
            return true;
        }
        else {
            node* curr = parent->child;
            while (curr->brother->data != who)
            {
                curr = curr->brother;
            }
            curr->brother = curr->brother->brother;

            while (curr->brother)
            {
                curr = curr->brother;
            }
            curr->brother = toBeRemoved->child;
            delete toBeRemoved;
            size--;
            return true;
        }
    }
    return false;
}

void Hierarchy::create_hierarchy(const string& data)
{
    if (data == "")
        return;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = data.find(DELIMETAR, prev)) != string::npos)
    {
        string row = data.substr(prev, pos - prev);
        prev = pos + 1;

        ManagerEmployeePair pair(row);
        insert_pair(row);
    }

    // To get the last substring (or only, if delimiter is not found)
    string row = data.substr(prev);
    if (row != ""){
        ManagerEmployeePair pair(row);
        insert_pair(row);
    }
}

bool Hierarchy::insert_pair(const ManagerEmployeePair& pair)
{
	if (pair.getManager().empty() || pair.getEmployee().empty())
		throw "Invalid input, can not create pair in this row. Manager or Employee name is empty";

	if (root == nullptr && pair.getManager() != "Uspeshnia")
		throw "Invalid input";

	if (size == 0 && pair.getManager() == "Uspeshnia") insert("Uspeshnia");

	return insert(pair.getManager(), pair.getEmployee());
}

bool Hierarchy::insert(const string& parent, const string& data)
{
    if (data.empty() && this->root == nullptr)
    {
        this->root = new node(parent, nullptr, nullptr);
        size++;
        return true;
    }

    node* foundedRoot = find_and_return(parent, root);
    if (foundedRoot == nullptr)
        throw "Parent was not founded";

    if (foundedRoot->child == nullptr) {
        foundedRoot->child = new node(data, nullptr, nullptr);
        size++;
        return true;
    }
    else {
        node* curr = foundedRoot->child;
        while (curr->brother != nullptr) {
            curr = curr->brother;
        }
        curr->brother = new node(data, nullptr, nullptr);
        size++;
        return true;
    }
    return false;
}

void Hierarchy::clear(node* root)
{
    if (root) {
        clear(root->child);
        clear(root->brother);
        delete root;
    }
}

Hierarchy::node* Hierarchy::copy(node* root)
{
    return root ?
        new node(root->data, copy(root->child), copy(root->brother)) :
        root;
}

Hierarchy::node* Hierarchy::get_parent(const string& data, node* root) const {
    if (root == nullptr) {
        // ancestor must exist on n's path to root to be able to find parent
        return nullptr;
    }
    if (root->data == data) {
        // if n is the ancestor we assume the ancestor is root
        // parent of root is nullptr
        return nullptr;
    }

    node* curr = root->child;
    node* p = nullptr;
    while (curr != nullptr) {
        if (curr->data == data) {
            return root;
        }
        p = get_parent(data, curr);
        if (p != nullptr)
            return p;
        else curr = curr->brother;
    }
    return nullptr;
}

int Hierarchy::get_node_level(const string& data, node* tree) const
{
    std::queue<Hierarchy::node*> queue;
    int level = 1;

    queue.push(root);
    queue.push(nullptr);
    while (!queue.empty()) {
        node* temp = queue.front();
        queue.pop();
        if (temp == nullptr) {
            if (queue.empty()) break;

            if (queue.front() != nullptr) {
                queue.push(nullptr);
            }
            level += 1;
        }
        else {
            if (temp->data == data)
                return level;

            if (temp->child) 
                queue.push(temp->child);
                       
            if (temp->brother) {
                node* helper = temp->brother;
                while (helper)
                {
                    if (helper->data == data)
                        return level;

                    if (helper->child != nullptr) 
                        queue.push(helper->child);
                                          
                    helper = helper->brother;
                }
            }
        }
    }
    return level;
}

const std::stack<Hierarchy::node*> Hierarchy::who_to_modernize() const
{
    std::stack<Hierarchy::node*> modernizeNodes;
    std::queue<Hierarchy::node*> queue;
    int level = 1;

    queue.push(root);

    // extra nullptr is pushed to keep track
    // of all the nodes to be pushed before
    // level is incremented by 1
    queue.push(nullptr);
    while (!queue.empty()) {
        node* temp = queue.front();
        queue.pop();
        if (temp == nullptr) {
            if (queue.empty()) break;

            if (queue.front() != nullptr) {
                queue.push(nullptr);
            }
            level += 1;
        }
        else {
            if (level % 2 == 0 && temp->child != nullptr) 
                modernizeNodes.push(temp);
            
            if (temp->child) 
                queue.push(temp->child);
            
            if (temp->brother) {
                node* helper = temp->brother;
                while (helper)
                {
                    if (level % 2 == 0 && helper->child != nullptr) 
                        modernizeNodes.push(helper);
                    
                    if (helper->child != nullptr)
                        queue.push(helper->child);
                    
                    helper = helper->brother;
                }
            }
        }
    }
    return modernizeNodes;
}

const std::stack<Hierarchy::node*> Hierarchy::who_to_incorporate() const
{
    std::stack<node*> incorporateNodes;
    std::queue<node*> queue;

    queue.push(root->child);
    while (!queue.empty()) {
        node* temp = queue.front();
        queue.pop();

        if (temp->brother) {
            node* biggestNode = biggest_node(temp);
            incorporateNodes.push(biggestNode);
        }

        if (temp->child) 
            if (temp->child->brother) 
                queue.push(temp->child);
                
        if (temp->brother) {
            node* helper = temp->brother;
            while (helper)
            {
                if (helper->child) 
                    if (helper->child->brother) 
                        queue.push(helper->child);
                                   
                helper = helper->brother;
            }
        }
    }
    return incorporateNodes;
}

Hierarchy::node* Hierarchy::biggest_node(node* start) const
{
    node* result = start;
    unsigned long maxSalary = getSalary(start->data);

    node* temp = start->brother;
    while (temp)
    {
        unsigned long tempSalary = getSalary(temp->data);
        if (tempSalary == maxSalary){
            //  1.Greater than 0 (> 0) : this is returned when the first string is lexicographically greater than the second string
            //  2.Less than 0 (< 0) : this is returned when the first string is lexicographically smaller than the second string
            int value = result->data.compare(temp->data);
            if (value > 0)
                result = temp;
        }
        if (tempSalary > maxSalary){
            maxSalary = tempSalary;
            result = temp;
        }
        temp = temp->brother;
    }
    return result;
}

void Hierarchy::fire(const node* toBeFired)
{
    if (root == nullptr) 
        return;

    node* parent = get_parent(toBeFired->data, root);
    if (parent == nullptr || toBeFired == nullptr) 
        return;

    if (count_all_subordinates(parent->data) == 1) {
        parent->child = toBeFired->child;
        delete toBeFired;
        size--;
        return;
    }
    else {
        if (parent->child->data == toBeFired->data)
        {
            parent->child = toBeFired->brother;
            node* curr = toBeFired->brother;
            while (curr->brother)
            {
                curr = curr->brother;
            }
            curr->brother = toBeFired->child;
            delete toBeFired;
            size--;
            return;
        }
        else {
            node* curr = parent->child;
            while (curr->brother->data != toBeFired->data)
            {
                curr = curr->brother;
            }
            curr->brother = curr->brother->brother;

            while (curr->brother)
            {
                curr = curr->brother;
            }
            curr->brother = toBeFired->child;
            delete toBeFired;
            size--;
            return;
        }
    }
    return;
}

int Hierarchy::num_employees(node* root) const
{
    if (root == nullptr)
        return 0;
    return 1 + num_employees(root->brother) + num_employees(root->child);
}

int Hierarchy::num_overloaded(node* root, const int level) const
{
    if (root == nullptr)
        return 0;

    if (root->child == nullptr)
        return 0;

    if (num_employees(root->child) > level) {
        return 1 + num_overloaded(root->brother, level) + num_overloaded(root->child, level);
    }
    else {
        return num_overloaded(root->brother, level) + num_overloaded(root->child, level);
    }
}

Hierarchy::node* Hierarchy::find_and_return(const string& data, node* root) const
{
    if (root == nullptr)
        return nullptr;

    if (root->data == data)
        return this->root;

    std::queue<node*> queue;
    queue.push(root);
    while (queue.empty() == false) {
        node* curr = queue.front();
        if (curr->data == data)
            return curr;

        queue.pop();
        if (curr->child != nullptr)
            queue.push(curr->child);

        if (curr->brother != nullptr)
            queue.push(curr->brother);
    }
    return nullptr; // not founded
}

int Hierarchy::count_all_subordinates(const string& data) const
{
    node* foundedNode = find_and_return(data, root);
    if (foundedNode != nullptr){
        int value = 0;
        node* curr = foundedNode->child;
        while (curr != nullptr)
        {
            curr = curr->brother;
            value++;
        }
        return value;
    }
    return -1;
}

std::vector<string> Hierarchy::print_by_leveles(node* root) const
{
    std::vector<string> result;
    std::vector<string> level;

    if (root == nullptr)
        return result;

    std::queue<const node*> front;
    front.push(root);
    front.push(nullptr);
    // code from lecture
    for (;;){
        const node* current = front.front();
        front.pop();
        if (current == nullptr){
            for (size_t i = 0; i < level.size(); i++){
                for (size_t j = i + 1; j < level.size(); j++) {
                    if (level[j] < level[i]){
                        string temp = level[j];
                        level[j] = level[i];
                        level[i] = temp;
                    }
                }
            }
            for (size_t i = 0; i < level.size(); i++){
                result.push_back(level[i]);
            }
            level.erase(level.begin(), level.end());
            if (front.empty()) 
                return result;

            front.push(nullptr);
        }
        else {
            string row = "";
            for (const node* it = current->child; it; it = it->brother){
                row += current->data;
                row.push_back('-');
                row += it->data;
                row.push_back('\n');
                level.push_back(row);
                row = "";
                front.push(it);
            }
        }
    }
    return result;
}