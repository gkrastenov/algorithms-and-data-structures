namespace Problem04.SinglyLinkedList
{
    using System;
    using System.Collections;
    using System.Collections.Generic;

    public class SinglyLinkedList<T> : IAbstractLinkedList<T>
    {
        private Node<T> head;

        public int Count { get; private set; }

        public void AddFirst(T item)
        {
            Node<T> node = new Node<T>(item);
            if (head == null)
            {
                this.head = node;
            }
            else
            {
                node.Next = this.head;
                this.head = node;
            }
            this.Count++;
        }

        public void AddLast(T item)
        {
            Node<T> node = new Node<T>(item);
            if (this.head == null)
            {
                this.head = node;
            }
            else
            {
                var current = this.head;
                while (current.Next != null)
                {
                    current = current.Next;
                }
                current.Next = node;
            }

            this.Count++;
        }

        public T GetFirst()
        {
            IsEmpty();

            return this.head.Item;
        }

        public T GetLast()
        {
            IsEmpty();

            var current = this.head;
            while (current.Next != null)
            {
                current = current.Next;
            }
            return current.Item;
        }

        public T RemoveFirst()
        {
            IsEmpty();
            var toReturnItem = this.head.Item;
            this.head = this.head.Next;
            this.Count--;

            return toReturnItem;
        }

        public T RemoveLast()
        {
            IsEmpty();

            var current = this.head;
            while (current.Next != null)
            {
                current = current.Next;
            }
            var toReturn = current.Item;
            current = null;
            return toReturn;

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

        private void IsEmpty()
        {
            if (this.Count == 0 || this.head == null)
            {
                throw new InvalidOperationException();
            }
        }
    }
}