namespace Problem03.Queue
{
    using System;
    using System.Collections;
    using System.Collections.Generic;

    public class Queue<T> : IAbstractQueue<T>
    {
        private Node<T> head;

        public int Count { get; private set; }

        public bool Contains(T item)
        {
            var curr = this.head;
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

        public T Dequeue()
        {
            if(this.head == null)
            {
                throw new InvalidOperationException();
            }

            var toReturn = this.head;
            this.head = toReturn.Next;
            this.Count--;
            return toReturn.Item;
        }
        // 5 10 21 55
        public void Enqueue(T item)
        {
            Node<T> newNode = new Node<T>(item);
            if (this.head == null)
                this.head = newNode;
            else

            {
                Node<T> current = this.head;
                while (current.Next != null)
                    current = current.Next;
                current.Next = newNode;
            }
            this.Count++;
        }

        public T Peek()
        {
            if (this.head == null)
            {
                throw new InvalidOperationException();
            }

            return this.head.Item;
        }

        public IEnumerator<T> GetEnumerator()
        {
            var currentNode = this.head;
            while (currentNode != null)
            {
                yield return currentNode.Item;
                currentNode = currentNode.Next;

            }
        }

        IEnumerator IEnumerable.GetEnumerator()
            => this.GetEnumerator();
    }
}