#pragma once
#include <vector>

#include "../interface.h"

class ASTNode;

using ChildrenNodes = std::vector<ASTNode*>;

class ASTNode{
private:
    Function function{};
    ChildrenNodes children{}; // List of children nodes linked to this AST node.

public:
    ASTNode(const Function& function)
        : function(function)
    {};

    explicit ASTNode(){} 
    ~ASTNode()
    {
        for (int i = 0; i < children.size(); i++) {
            delete children[i];
            children[i] = nullptr;
        }
    }

    ASTNode(const ASTNode&) = default;
    ASTNode(ASTNode&&) = default;
    ASTNode& operator=(const ASTNode&) = default;
    ASTNode& operator=(ASTNode&&) = default;
public:
    Function& getFunction() { return function; }
    ChildrenNodes& getChildrenNodes() { return children; }
    const size_t getChindrenSize() const { return children.size(); }
    ASTNode* getChildrenNodeByIndex(size_t index)
    {
        if (index >= children.size())
            throw " Out of range exeption";
        return children[index];
    }
    void add_child_nodes(ASTNode* node)
    {
        children.emplace_back(node);
    }
};