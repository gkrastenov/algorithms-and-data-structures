namespace _02.MaxHeap
{
    using System;
    using System.Collections.Generic;

    public class MaxHeap<T> : IAbstractHeap<T>
        where T : IComparable<T>
    {
        private List<T> elements;
        public MaxHeap()
        {
            this.elements = new List<T>();
        }
        public int Size => elements.Count;

        public void Add(T element)
        {
            this.elements.Add(element);

            this.HeapifyUp();
        }

        private void HeapifyUp()
        {
            int currentIndex = this.Size - 1;
            int parentIndex = this.GetParentIndex(currentIndex);
            while (this.IndexIsValid(currentIndex)
                && this.isGreater(currentIndex,parentIndex))
            {
                // Swap
                var temp = this.elements[currentIndex];
                this.elements[currentIndex] = this.elements[parentIndex];
                this.elements[parentIndex] = temp;

                currentIndex = parentIndex;
                parentIndex = this.GetParentIndex(currentIndex);
            }
        }
        private bool isGreater(int childIndex, int parentIndex)
        {
            return this.elements[childIndex]
                .CompareTo(this.elements[parentIndex]) > 0;
        }
        private int GetParentIndex(int childIndex)
        {
            return (childIndex - 1) / 2;
        }
        private bool IndexIsValid(int index)
        {
            return index >0;
        }
        public T Peek()
        {
            EnsureNotEmpty();
            return this.elements[0];
        }

        private void EnsureNotEmpty()
        {
            if (this.elements.Count == 0)
            {
                throw new InvalidOperationException();
            }

        }
    }
}
