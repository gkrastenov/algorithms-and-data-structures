#pragma once
#include <vector>

#include "../interface.h"

class ASTNode;

using ChildrenNodes = std::vector<ASTNode*>;

class ASTNode{
private:
    Function function{};
    ChildrenNodes children;    // List of children nodes linked to this AST node.

public:
    /* template<class T>
    inline LList<T>::Node::Node(const T& val, Node* next)
        :data(val), next(next)
    {} */
    ASTNode(const Function& function)
        : function(function)
    {};

    explicit ASTNode(){ } // TODO: what to create
    ~ASTNode() = default; // TODO: what to delete

    ASTNode(const ASTNode&) = default;
    ASTNode(ASTNode&&) = default;
    ASTNode& operator=(const ASTNode&) = default;
    ASTNode& operator=(ASTNode&&) = default;
public:
    Function& getFunction() { return function; }
    ChildrenNodes& getChildrenNodes() { return children; }
    const size_t getChindrenSize() const { return children.size(); }
    void add_child_nodes(ASTNode* node)
    {
        children.emplace_back(node);
    }
};