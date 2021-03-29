namespace _02.DOM
{
    using System;
    using System.Collections.Generic;
    using System.Linq.Expressions;
    using System.Text;
    using System.Threading;
    using _02.DOM.Interfaces;
    using _02.DOM.Models;
    using Wintellect.PowerCollections;

    public class DocumentObjectModel : IDocument
    {
        IHtmlElement firstChild;
        IHtmlElement secondChild;
        IHtmlElement element;

        public DocumentObjectModel(IHtmlElement root)
        {
            this.Root = root;
        }


        public DocumentObjectModel()
        {
            Create();
        }
        private void Create()
        {
            this.Root.Type = ElementType.Document;
            element.Type = ElementType.Html;
            firstChild.Parent = element;
            secondChild.Parent = element;

            firstChild.Type = ElementType.Head;
            secondChild.Type = ElementType.Body;
            element.Children.Add(firstChild);
            element.Children.Add(secondChild);
        }
        IHtmlElement current;
        public IHtmlElement Root { get; private set; }

        public IHtmlElement GetElementByType(ElementType type)
        {
            var queue = new Queue<IHtmlElement>();

            queue.Enqueue(this.Root);
            while (queue.Count > 0)
            {
                var subTree = queue.Dequeue();
                if (subTree.Type.Equals(type))
                {
                    return subTree;
                }
                foreach (var child in subTree.Children)
                {
                    queue.Enqueue(child);
                }
            }
            return null;
        }

        public List<IHtmlElement> GetElementsByType(ElementType type)
        {
            var result = new Stack<IHtmlElement>();
            var toTraverse = new Stack<IHtmlElement>();

            toTraverse.Push(this.Root);
            while (toTraverse.Count > 0)
            {
                var subtree = toTraverse.Pop();

                foreach (var child in subtree.Children)
                {
                    toTraverse.Push(child);
                }
                if (subtree.Type.Equals(type))
                {
                    result.Push(subtree);
                }


            }

            if (result.Count == 0)
            {
                return new List<IHtmlElement>();
            }
            return new List<IHtmlElement>(result);
        }

        public bool Contains(IHtmlElement htmlElement)
        {
            var queue = new Queue<IHtmlElement>();

            queue.Enqueue(this.Root);
            while (queue.Count > 0)
            {
                var subTree = queue.Dequeue();
                if (subTree.Equals(htmlElement))
                {
                    return true;
                }
                foreach (var child in subTree.Children)
                {
                    queue.Enqueue(child);
                }
            }
            return false;
        }

        public void InsertFirst(IHtmlElement parent, IHtmlElement child)
        {
            current = FindBfs(parent);
            if (current == null)
            {
                throw new InvalidOperationException();
            }
            child.Parent = current;
            current.Children.Insert(0, child);
        }

        public void InsertLast(IHtmlElement parent, IHtmlElement child)
        {
            current = FindBfs(parent);
            if (current == null)
            {
                throw new InvalidOperationException();
            }
            child.Parent = current;
            current.Children.Add(child);
        }

        public void Remove(IHtmlElement htmlElement)
        {
            var current = FindBfs(htmlElement);
            if (current == null)
            {
                throw new InvalidOperationException();
            }

            current.Children.Clear();

            var parentNode = htmlElement.Parent;

            if (parentNode !=null)
            {
                parentNode.Children.Remove(current);
                current.Parent = null;
            }


            //var parent = current.Parent;
            //parent.Children.Remove(current);
            // current.Parent = null;
            //current.Children.Clear();

        }

        public void RemoveAll(ElementType elementType)
        {
            throw new NotImplementedException();
        }

        public bool AddAttribute(string attrKey, string attrValue, IHtmlElement htmlElement)
        {
            var result = this.FindBfs(htmlElement);
            if (result == null)
            {
                throw new InvalidOperationException();
            }
            if (result.Attributes.ContainsKey(attrKey))
            {
                return false;
            }
            result.Attributes.Add(attrKey, attrValue);
            return true;
        }

        public bool RemoveAttribute(string attrKey, IHtmlElement htmlElement)
        {
            var result  = this.FindBfs(htmlElement);
            if (result == null)
            {
                throw new InvalidOperationException();
            }

            bool toReturn = result.Attributes.ContainsKey(attrKey);
            result.Attributes.Remove(attrKey);
            return toReturn;
        }

        public IHtmlElement GetElementById(string idValue)
        {
            var result = new List<IHtmlElement>();
            var queue = new Queue<IHtmlElement>();

            queue.Enqueue(this.Root);
            while (queue.Count > 0)
            {

                var subTree = queue.Dequeue();
                result.Add(subTree);
                if (subTree.Attributes.ContainsValue(idValue))
                {
                    return subTree;
                }
                foreach (var child in subTree.Children)
                {
                    queue.Enqueue(child);
                }
            }
            return null;
        }
        private IHtmlElement FindBfs(IHtmlElement node)
        {
            var result = new List<IHtmlElement>();
            var queue = new Queue<IHtmlElement>();

            queue.Enqueue(this.Root);
            while (queue.Count > 0)
            {
                var subTree = queue.Dequeue();
                result.Add(subTree);

                if (subTree.Equals(node))
                {
                    return subTree;
                }
                foreach (var child in subTree.Children)
                {
                    queue.Enqueue(child);
                }
            }
            return null;
        }

        public override string ToString()
        {
            StringBuilder result = new StringBuilder();
            this.DFSRecurstionPrint(this.Root, 0, result);
            return result.ToString().Trim();
        }
        private void DFSRecurstionPrint(IHtmlElement subTree,
           int spaces,
           StringBuilder result)
        {

            result.Append(new string(' ', spaces))
                .Append(subTree.Type.ToString())
                .Append(Environment.NewLine);

            foreach (var child in subTree.Children)
            {
                this.DFSRecurstionPrint(child, spaces + 2, result);
            }

        }
    }
}
