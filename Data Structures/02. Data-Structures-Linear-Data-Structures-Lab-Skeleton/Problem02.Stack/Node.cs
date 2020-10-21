namespace Problem02.Stack
{
    public class Node<T>
    {
        // TODO: Implement

        public T Item { get; set; }
        public Node<T> Next { get; set; }

        public Node(T item, Node<T> next = null)
        {
            this.Item = item;
            this.Next = next;
        }
    }
}