namespace _01._BrowserHistory
{
    using System;
    using System.Collections.Generic;
    using System.Linq.Expressions;
    using System.Security.Cryptography;
    using System.Text;
    using _01._BrowserHistory.Interfaces;

    public class BrowserHistory : IHistory
    {
        List<ILink> history;
        public BrowserHistory()
        {
            this.history = new List<ILink>();
        }
        public int Size => this.history.Count;

        public void Clear()
        {
            this.history.Clear();
        }

        public bool Contains(ILink link)
        {
            return this.history.Contains(link);
        }

        public ILink DeleteFirst()
        {
            this.EnsureIsEmpty();
            var current = this.history[0];
            this.history.RemoveAt(0);
            return current; 
        }

        public ILink DeleteLast()
        {
            this.EnsureIsEmpty();
            var current = this.history[this.history.Count - 1];
            this.history.RemoveAt(this.history.Count - 1);
            return current;
        }

        // TODO : optimize
        public ILink GetByUrl(string url)
        {
            foreach (var link in history)
            {
                if (link.Url == url)
                {
                    return link;
                }
            }
            return null;
        }

        public ILink LastVisited()
        {
            this.EnsureIsEmpty();
            return this.history[this.history.Count - 1];
        }

        public void Open(ILink link)
        {
            this.history.Add(link);
        }
        // TODO: optimize
        public int RemoveLinks(string url)
        {
            // return  this.history.RemoveAll(e => e.Url == url);
            var helper = new List<ILink>(this.history);
            int number = 0;
            foreach (var item in helper)
            {
                if (item.Url.Contains(url))
                {
                    number++;
                    this.history.Remove(item);
                }
            }
            if (number == 0)
            {
                throw new InvalidOperationException();
            }
            return number;
        }
        public ILink[] ToArray()
        {
            this.history.Reverse();
            return this.history.ToArray();
        }
        // TODO : Optimize
        public List<ILink> ToList()
        {
            this.history.Reverse();
            return this.history;
        }

        public string ViewHistory()
        {
            if (this.Size==0)
            {
                return "Browser history is empty!";
            }
            StringBuilder builder = new StringBuilder();
            for (int i = this.Size - 1; i >= 0; i--)
            {
                string asd = this.history[i].ToString();
                builder.AppendLine(asd);
            }

            return builder.ToString();
        }
        private void EnsureIsEmpty()
        {
            if (this.Size == 0)
            {
                throw new InvalidOperationException();
            }
        }
    }
}
