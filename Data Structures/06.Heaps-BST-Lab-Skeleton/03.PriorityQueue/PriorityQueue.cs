namespace _03.PriorityQueue
{
    using System;
    using System.Collections.Generic;

    public class PriorityQueue<T> : IAbstractHeap<T>
        where T : IComparable<T>
    {
        private List<T> elements;
        public PriorityQueue()
        {
            this.elements = new List<T>();
        }
        public int Size => this.elements.Count;

        public T Dequeue()
        {
            var firstElement = this.Peek();
            this.Swap(0, this.Size - 1);
            this.elements.RemoveAt(this.Size - 1);
            this.HeapifyDown();
            return firstElement;

        }

        private void HeapifyDown()
        {
            throw new NotImplementedException();
        }

        public void Add(T element)
        {
            this.elements.Add(element);

            this.HeapifyUp();
        }
        private int GetParentIndex(int childIndex)
        {
            return (childIndex - 1) / 2;
        }
        private bool IndexIsValid(int index)
        {
            return index > 0;
        }
        public T Peek()
        {
            EnsureNotEmpty();
            return this.elements[0];
        }
        private bool isGreater(int childIndex, int parentIndex)
        {
            return this.elements[childIndex]
                .CompareTo(this.elements[parentIndex]) > 0;
        }
        private void EnsureNotEmpty()
        {
            if (this.elements.Count == 0)
            {
                throw new InvalidOperationException();
            }

        }

        private void Swap(int currentIndex, int parentIndex)
        {
            var temp = this.elements[currentIndex];
            this.elements[currentIndex] = this.elements[parentIndex];
            this.elements[parentIndex] = temp;
        }
        private void HeapifyUp()
        {
            int currentIndex = this.Size - 1;
            int parentIndex = this.GetParentIndex(currentIndex);
            while (this.IndexIsValid(currentIndex)
                && this.isGreater(currentIndex, parentIndex))
            {
                // Swap
                this.Swap(currentIndex, parentIndex);

                currentIndex = parentIndex;
                parentIndex = this.GetParentIndex(currentIndex);
            }
        }
    }
}
