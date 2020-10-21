namespace Tree
{
    using System;
    using System.Collections.Generic;

    public class Tree<T> : IAbstractTree<T>
    {
        private readonly List<Tree<T>> children;

        public Tree(T value)
        {
            this.Value = value;
            this.Parent = null;
            this.children = new List<Tree<T>>();
        }

        public Tree(T value, params Tree<T>[] children)
            : this(value)
        {
            foreach (var child in children)
            {
                child.Parent = this;
                this.children.Add(child);
            }
        }

        public T Value { get; private set; }
        public Tree<T> Parent { get; private set; }
        public IReadOnlyCollection<Tree<T>> Children => this.children.AsReadOnly();
        private bool IsRootDeleted { get; set; }

        private Tree<T> FindBfs(T value)
        {
            var result = new List<T>();
            var queue = new Queue<Tree<T>>();

            queue.Enqueue(this);
            while (queue.Count > 0)
            {
                var subTree = queue.Dequeue();
                result.Add(subTree.Value);

                if (subTree.Value.Equals(value))
                {
                    return subTree;
                }
                foreach (var child in subTree.children)
                {
                    queue.Enqueue(child);
                }
            }
            return null;
        }
        public ICollection<T> OrderBfs()
        {
            var result = new List<T>();
            var queue = new Queue<Tree<T>>();
            if (this.IsRootDeleted)
            {
                return result;
            }
            queue.Enqueue(this);
            while (queue.Count>0)
            {
                var subTree = queue.Dequeue();
                result.Add(subTree.Value);

                foreach (var child in subTree.children)
                {
                    queue.Enqueue(child);
                }
            }
            return result;
        }

        public ICollection<T> OrderDfs()
        {
            var result = new List<T>();

            if (this.IsRootDeleted)
            {
                return result;
            }

            this.DFSRecursion(this, result);
            return result;

        }
        private void DFSRecursion(Tree<T> subTree, List<T> result)
        {
            foreach (var child in subTree.Children)
            {
                this.DFSRecursion(child, result);
            }
            result.Add(subTree.Value);
        }
        private ICollection<T> OrderDfsWithStack()
        {
            var result = new Stack<T>();
            var toTraverse = new Stack<Tree<T>>();

            toTraverse.Push(this);
            while (toTraverse.Count>0)
            {
                var subtree = toTraverse.Pop();

                foreach (var child in subtree.Children)
                {
                    toTraverse.Push(child);
                }
                result.Push(subtree.Value);

            }
            return new List<T>(result);

        }
        public void AddChild(T parentKey, Tree<T> child)
        {
            var searchedElement = this.FindBfs(parentKey);
            CheckEmptyNode(searchedElement);

            searchedElement.children.Add(child);
        }

        private void CheckEmptyNode(Tree<T> parentKey)
        {
            if (parentKey is null)
            {
                throw new ArgumentNullException();
            }
        }

        public void RemoveNode(T nodeKey)
        {
            var currentNode = this.FindBfs(nodeKey);
            CheckEmptyNode(currentNode);

            foreach (var child in currentNode.children)
            {
                child.Parent = null;
            }

            currentNode.children.Clear();

            var parentNode = currentNode.Parent;

            if (parentNode is null)
            {
                this.IsRootDeleted = true;
            }
            else
            {
                parentNode.children.Remove(currentNode);
                currentNode.Parent = null;
            }

            currentNode.Value = default(T);
        }

        public void Swap(T firstKey, T secondKey)
        {
            throw new NotImplementedException();
        }
    }
}
