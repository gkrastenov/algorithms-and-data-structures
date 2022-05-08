#pragma once
#include <iostream>
#include <queue>
#include <string>
#include <stack>

#include "hierarchyPair.h"

using std::string;

class Hierarchy
{
private:
    struct node
    {
        string data{};
        node* child{}, * brother{};

        node()
        {};
        node(const string& data, node* child = nullptr, node* brother = nullptr)
            : data(data)
            , child(child)
            , brother(brother)
        {}
    } *root{};

    size_t size{};

    /// <summary>
    /// Parameter who is used to save name of hierarchy.
    /// </summary>
    string name{};

    /// <summary>
    /// Const value who is used to split given data for creating the hierarchy.
    /// </summary>
    const string DELIMETAR = "\n";
public:
    Hierarchy() : root(nullptr), size(0) , name(""){};
    Hierarchy(Hierarchy&& hierarchy) noexcept : Hierarchy()
    {
        std::swap(root, hierarchy.root);
        std::swap(size, hierarchy.size);
        std::swap(name, hierarchy.name);
    };
    Hierarchy(const Hierarchy& hierarchy)
        : root(copy(hierarchy.root))
        , size(hierarchy.size) 
        , name(hierarchy.name)       
    {};
    Hierarchy(const string& data);
    ~Hierarchy() noexcept { clear(root); }
    void operator=(const Hierarchy&) = delete;

    string print() const;

    int longest_chain() const;
    bool find(const string& name) const;
    int num_employees() const;
    int num_overloaded(int level = 20) const;

    string manager(const string& name) const;
    int num_subordinates(const string& name) const;

    unsigned long getSalary(const string& who) const;
    int getSize() const { return size; }
    int getHeight(const node* root) const;
    string getName() const { return name;}
    void setName(const string& name) { this->name = name;}

    bool fire(const string& who);
    bool hire(const string& who, const string& boss);

    void incorporate();
    void modernize();

    Hierarchy join(const Hierarchy& right) const;

private:
    /// <summary>
    /// Search for node and return him.
    node* find_and_return(const string& data, node* root) const;

    /// <summary>
    /// Attaches new node to another node, make element child of root
    /// </summary>
    bool insert(const string& parent, const string& data = "");
    void clear(node* root);
    node* copy(node* root);
    node* get_parent(const string& data, node* ancestor) const;

    /// <summary>
    /// Get the level of node in the given tree.
    /// </summary>
    int get_node_level(const string& data, node* tree) const;
    const std::stack<node*> who_to_modernize() const;
    const std::stack<node*> who_to_incorporate() const;
    node* biggest_node(node* start) const;
    void fire(const node* toBeFired);
    int num_employees(node* root) const;
    int num_overloaded(node* root, const int level) const;
    void create_hierarchy(const string& data);
    bool insert_pair(const ManagerEmployeePair& pair);
    int count_all_subordinates(const string& data) const;
    std::vector<string> print_by_leveles(node* root) const;
};