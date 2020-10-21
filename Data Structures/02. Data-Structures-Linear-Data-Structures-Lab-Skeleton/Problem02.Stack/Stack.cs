namespace Problem02.Stack
{
    using System;
    using System.Collections;
    using System.Collections.Generic;

    public class Stack<T> : IAbstractStack<T>
    {
        private Node<T> top;

        public int Count { get; private set; }

        public bool Contains(T item)
        {

            var curr = this.top;
            while (curr != null)
            {
                if (curr.Item.Equals(item))
                {
                    return true;
                }
                curr = curr.Next;
            }
            return false;
        }

        public T Peek()
        {
            if (IsEmptyStack())
            {
                throw new InvalidOperationException();
            }

            return this.top.Item;
        }

        public T Pop()
        {
            if (IsEmptyStack())
            {
               throw new InvalidOperationException();
            }

            var topNodeItem = this.top.Item;

            var nextNode = this.top.Next;
            this.top = nextNode;
            this.Count--;
            return topNodeItem;
        }
        // 1 20 15 6 44
        public void Push(T item)
        {
            var newNode = new Node<T>(item);
            newNode.Next = top;
            this.top = newNode;
            this.Count++;
        }

        public IEnumerator<T> GetEnumerator()
        {
            var currentNode = this.top;
            while (currentNode != null)
            {
                yield return currentNode.Item;
                currentNode = currentNode.Next;
                 
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
            => this.GetEnumerator();

        private bool IsEmptyStack()
        {
            return this.top == null;
        }
    }
}