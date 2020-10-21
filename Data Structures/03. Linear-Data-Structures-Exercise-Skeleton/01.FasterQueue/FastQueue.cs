namespace Problem01.FasterQueue
{
    using System;
    using System.Collections;
    using System.Collections.Generic;

    public class FastQueue<T> : IAbstractQueue<T>
    {
        private Node<T> head;
        private Node<T> tailNode;

        public int Count { get; private set; }

        public bool Contains(T item)
        {
            var current = this.head;

            while (current != null)
            {
                if (current.Item.Equals(item))
                {
                    return true;
                }

                current = current.Next;
            }

            return false;
        }

        public T Dequeue()
        {
            this.EnsureNotEmpty();

            var headItem = this.head.Item;
            if (this.Count == 1)
            {
                this.head = this.tailNode = null;
            }
            else
            {
                this.head = this.head.Next;
            }
            this.Count--;

            return headItem;
        }

        public void Enqueue(T item)
        {
             var newNode = new Node<T>
            {
                Item = item,
                Next = null
            };

            if (this.head is null || this.Count == 0)
            {
                this.head = this.tailNode = newNode;
            }
            else
            {
                tailNode.Next = newNode;
                tailNode = newNode;
            }

            this.Count++;
            return;
        }
        public T Peek()
        {
            this.EnsureNotEmpty();
            return this.head.Item;
        }

        public IEnumerator<T> GetEnumerator()
        {
            var current = this.head;
            while (current != null)
            {
                yield return current.Item;
                current = current.Next;
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
            => this.GetEnumerator();

        private void EnsureNotEmpty()
        {
            if (this.Count == 0)
                throw new InvalidOperationException();
        }
    }
}