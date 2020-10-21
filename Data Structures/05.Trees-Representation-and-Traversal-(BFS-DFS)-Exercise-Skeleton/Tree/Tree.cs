namespace Tree
{
    using System;
    using System.Collections.Generic;
    using System.Collections.Specialized;
    using System.Linq;
    using System.Text;

    public class Tree<T> : IAbstractTree<T>
    {
        private readonly List<Tree<T>> children;

        public Tree(T key, params Tree<T>[] children)
        {
            this.Key = key;
            this.children = new List<Tree<T>>();
          
            foreach (var child in children)
            {
                this.AddChild(child);
                child.Parent = this;
            }
        }

        public T Key { get; private set; }

        public Tree<T> Parent { get; private set; }


        public IReadOnlyCollection<Tree<T>> Children
            => this.children.AsReadOnly();

        public void AddChild(Tree<T> child)
        {
            this.children.Add(child);
        }

        public void AddParent(Tree<T> parent)
        {
            this.Parent = parent;
        }

        public string GetAsString()
        {
            StringBuilder result = new StringBuilder();
            this.DFSRecurstionPrint(this, 0, result);
            return result.ToString().Trim();
        }
        private void DFSRecurstionPrint(Tree<T> subTree, 
            int spaces, 
            StringBuilder result)
        {

            result.Append(new string(' ', spaces))
                .Append(subTree.Key)
                .Append(Environment.NewLine);

            foreach (var child in subTree.Children)
            {
                this.DFSRecurstionPrint(child, spaces + 2, result);
            }

        }
        public Tree<T> GetDeepestLeftomostNode()
        {
            var result = new SortedDictionary<int, Tree<T>>();
            var queue = new Queue<Tree<T>>();
            int index = 1;

            queue.Enqueue(this);
            while (queue.Count > 0)
            {
                var subTree = queue.Dequeue();
                result.Add(index ,subTree);

                foreach (var child in subTree.children)
                {
                    queue.Enqueue(child);
                }

                index++;
            }
            
            return result.First().Value;
        }

        public List<T> GetLeafKeys()
        {
            var stack = new Stack<T>();
            var toTraverse = new Stack<Tree<T>>();
            var result = new List<T>();
            toTraverse.Push(this);
            while (toTraverse.Count > 0)
            {
                var subtree = toTraverse.Pop();

                foreach (var child in subtree.Children)
                {
                    if (child.Children.Count == 0)
                    {
                        result.Add(child.Key);
                    }
                    toTraverse.Push(child);
                }
                stack.Push(subtree.Key);


            }

            result.Sort();
            return result;
        }

        public List<T> GetMiddleKeys()
        {
            var stack = new Stack<T>();
            var toTraverse = new Stack<Tree<T>>();
            var result = new List<T>();
            toTraverse.Push(this);
            while (toTraverse.Count > 0)
            {
                var subtree = toTraverse.Pop();

                foreach (var child in subtree.Children)
                {
                    if (child.Children.Count > 0 && child.Parent != null)
                    {
                        result.Add(child.Key);
                    }
                    toTraverse.Push(child);
                }
                stack.Push(subtree.Key);


            }

            result.Sort();
            return result;
        }

        public List<T> GetLongestPath()
        {
            var deepNode = this.GetDeepestLeftomostNode();
            var result = new List<T>();
            var current = deepNode;
            while (deepNode != null)
            {
                result.Add(deepNode.Key);
                current = current.Parent;
            }
            result.Reverse();
            return result;
        }

        public List<List<T>> PathsWithGivenSum(int sum)
        {
            var result = new List<List<T>>();
            var currenPath = new List<T>();
            currenPath.Add(this.Key);
            int currentSum = Convert.ToInt32(this.Key);

            this.GetPathWithDfs(this, result, currenPath, ref currentSum, sum);
             return result;
        }

        private void GetPathWithDfs(Tree<T> current,
            List<List<T>> wantedPaths,
            List<T> currenPath,
            ref int currentSum,
            int sum)
        {
            foreach (var child in current.Children)
            {
                currenPath.Add(child.Key);
                currentSum += Convert.ToInt32(child.Key);
                this.GetPathWithDfs(child, wantedPaths,currenPath, ref currentSum, sum);
            }
            if (currentSum == sum)
            {
                wantedPaths.Add(new List<T>(currenPath));
            }

            currentSum -= Convert.ToInt32(current.Key);
            currenPath.RemoveAt(currenPath.Count - 1);
        }

        public List<Tree<T>> SubTreesWithGivenSum(int sum)
        {
            throw new NotImplementedException();
        }
    }
}
