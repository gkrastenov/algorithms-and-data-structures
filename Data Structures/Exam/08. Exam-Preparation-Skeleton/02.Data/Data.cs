namespace _02.Data
{
    using _02.Data.Interfaces;
    using _02.Data.Models;
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Threading;
    using Wintellect.PowerCollections;

    public class Data : IRepository
    {
        private OrderedBag<IEntity> items;
        public Data()
        {
            this.items = new OrderedBag<IEntity>();
        }
        public int Size => this.items.Count;

        public void Add(IEntity entity)
        {
            this.items.Add(entity);

            var parentNode = this.GetById((int)entity.ParentId);

            if (parentNode !=null)
            {
                parentNode.Children.Add(entity);
            }
        }

        public Data (Data copy)
        {
            this.items = copy.items;
        }
        public IRepository Copy()
        {
            Data copy = (Data)this.MemberwiseClone();

            return new Data(copy);
        }

        public IEntity DequeueMostRecent()
        {
            this.EnsureNotEmpty();

            return this.items.RemoveFirst();
        }

        public List<IEntity> GetAll()
        {
            return new List<IEntity>(this.items);
        }

        public List<IEntity> GetAllByType(string type)
        {
            if (type != typeof(Invoice).Name
                && type != typeof(StoreClient).Name
                  && type != typeof(User).Name)
            {
                throw new InvalidOperationException($"Ivalid type: {type}");
            }
            var result = new List<IEntity>();
            foreach (var item in items)
            {
                if (item.GetType().Name == type)
                {
                    result.Add(item);
                }
            }
            return result;
        }

        public IEntity GetById(int id)
        {
            if (id < 0 || id> this.Size)
            {
                return null;
            }

            return this.items[this.Size - 1 - id];
        }

        public List<IEntity> GetByParentId(int parentId)
        {
            var parentNode = this.GetById(parentId);

            if (parentNode == null)
            {
                return new List<IEntity>();
            }

            return parentNode.Children;
        }

        public IEntity PeekMostRecent()
        {
            this.EnsureNotEmpty();

            return this.items.GetFirst();
        }

        private void EnsureNotEmpty()
        {
            if (this.Size == 0)
            {
                throw new InvalidOperationException
                    ("Operation on empty Data");
            }
        }
    }
}
