namespace Problem03.ReversedList
{
    using System;
    using System.Collections;
    using System.Collections.Generic;

    public class ReversedList<T> : IAbstractList<T>
    {
        private const int DefaultCapacity = 4;

        private T[] items;

        public ReversedList()
            : this(DefaultCapacity) { }

        public ReversedList(int capacity)
        {
            if (capacity < 0)
                throw new ArgumentOutOfRangeException(nameof(capacity));

            this.items = new T[capacity];
        }

        public T this[int index]
        {
            get
            {
                this.ValidateIndex(index);
                return this.items[this.Count - 1 - index];
            }
            set
            {
                this.ValidateIndex(index);
                this.items[index] = value;
            }
        }

        public int Count { get; private set; }

        public void Add(T item)
        {
            GrowIfNeccessary();
            this.items[this.Count++] = item;
        }

        public bool Contains(T item)
        {
            return this.IndexOf(item) != -1;
        }

        public int IndexOf(T item)
        {
            var index = 0;
            for (int i = this.Count - 1; i>=0; i--)
            {
                if (this.items[i].Equals(item))
                {
                    return index;
                }
                index++;
            }

            return -1;
        }

        public void Insert(int index, T item)
        {
            GrowIfNeccessary();
            this.ValidateIndex(index);

            int indexOfElement = this.Count - index;
            for (int i = this.Count; i > indexOfElement; i--)
            {
                this.items[i] = items[index - 1];
            }
            this.items[indexOfElement] = item;
            this.Count++;

        }

        public bool Remove(T item)
        {
            int elementIndex = this.IndexOf(item);
            if (elementIndex == -1)
            {
                return false;
            }

            this.RemoveAt(elementIndex);
            return true;
        }

        public void RemoveAt(int index)
        {
            this.ValidateIndex(index);
            int indexOfElement = this.Count - 1 - index;

            for (int i = indexOfElement; i <this.Count-1; i++)
            {
                items[i] = items[i + 1];
            }
            items[this.Count - 1] = default;
            this.Count--;
           
        }

        public IEnumerator<T> GetEnumerator()
        {
            for (int i = this.Count-1; i>= 0 ; i--)
            {
                yield return this.items[i];
            }
        }
        IEnumerator IEnumerable.GetEnumerator()
        {
            return this.GetEnumerator();
        }
        private void GrowIfNeccessary()
        {
            if (this.Count == this.items.Length)
            {
                Grow();
            }
           
        }
        private void Grow()
        {
            T[] newItems = new T[this.items.Length * 2];
            Array.Copy(this.items, newItems, this.items.Length);
            this.items = newItems;
        }
        private void ValidateIndex(int index)
        {
            if (index < 0 || index >= this.Count)
            {
                throw new IndexOutOfRangeException();
            }
        }
    }
}