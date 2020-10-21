namespace Problem02.DoublyLinkedList
{
    using System;
    using System.Collections;
    using System.Collections.Generic;

    public class DoublyLinkedList<T> : IAbstractLinkedList<T>
    {
        private Node<T> head;
        private Node<T> tail;

        public DoublyLinkedList(Node<T> head)
        {
            this.head = this.tail = head;
            this.Count++;
        }

        public DoublyLinkedList()
        {
            this.head = this.tail = null;
            this.Count = 0;
        }

        public int Count { get; private set; }

        public void AddFirst(T item)
        {
            Node<T> node = new Node<T>()
            {
                Item = item
            };

            if (this.Count ==0)
            {
                this.head = this.tail = node;
            }
            else
            {
                head.Previous = node;
                node.Next = head;
                head = node;
            }

            this.Count++;
        }

        public void AddLast(T item)
        {
            Node<T> node = new Node<T>()
            {
                Item = item
            };

            if (this.Count == 0)
            {
                this.head = this.tail = node;
            }
            else
            {          
                node.Previous = tail;
                tail.Next = node;
                tail = node;
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
            return this.tail.Item;
        }

        public T RemoveFirst()
        {
            IsEmpty();

            var toIntern = this.head.Item;
            this.head = this.head.Next;

            this.Count--;
            return toIntern;
        }

        public T RemoveLast()
        {
            IsEmpty();

            var toIntern = this.tail.Item;
            this.tail = this.tail.Previous;

            this.Count--;
            return toIntern;
        }

        public IEnumerator<T> GetEnumerator()
        {
            var current = head;
            while (current != null)
            {
                yield return current.Item;
                current = current.Next;
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return this.GetEnumerator();
        }
        private void IsEmpty()
        {
            if (this.head == null || this.Count == 0)
            {
                throw new InvalidOperationException();
            }
        }
    }
}