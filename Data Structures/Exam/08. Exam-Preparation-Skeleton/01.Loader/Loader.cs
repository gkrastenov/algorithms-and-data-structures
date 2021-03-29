namespace _01.Loader
{
    using _01.Loader.Interfaces;
    using _01.Loader.Models;
    using System;
    using System.Collections;
    using System.Collections.Generic;

    public class Loader : IBuffer
    {
        public int EntitiesCount => this.items.Count;

        private List<IEntity> items;

        public Loader()
        {
            this.items = new List<IEntity>();
        }

        public void Add(IEntity entity)
        {
            this.items.Add(entity);
        }

        public void Clear()
        {
            this.items.Clear();
        }

        public bool Contains(IEntity entity)
        {
            return this.GetById(entity.Id) != null;
        }


        public IEntity Extract(int id)
        {
            var toReturn = this.GetById(id);

            if (toReturn != null)
            {
                this.items.Remove(toReturn);

            }
            return toReturn;

        }

        private IEntity GetById(int id)
        {
            // var current = this.items.Find(x => x.Id == id);
            foreach (var item in items)
            {
                if (item.Id == id)
                {
                    return item;
                }
            }
            return null;
        }

        public IEntity Find(IEntity entity)
        {
            return this.GetById(entity.Id);
        }

        public List<IEntity> GetAll()
        {
            return items;
        }

        public IEnumerator<IEntity> GetEnumerator()
        {
            for (int i = 0; i < this.EntitiesCount; i++)
            {
                yield return this.items[i];
            }

        }

        public void RemoveSold()
        {
            this.items.RemoveAll(e => e.Status == BaseEntityStatus.Sold);
        }

        public void Replace(IEntity oldEntity, IEntity newEntity)
        {
            int indexOfEnity = this.items.IndexOf(oldEntity);
            this.ValidateEntity(indexOfEnity);

            this.items[indexOfEnity] = newEntity;
        }

        public List<IEntity> RetainAllFromTo(BaseEntityStatus lowerBound, BaseEntityStatus upperBound)
        {
            int lowerBoundIndex = (int)lowerBound;
            int upperBoundIndex = (int)upperBound;

            var result = new List<IEntity>(this.EntitiesCount);
            foreach (var item in items)
            {
                int statusNumber = (int)item.Status;
                if (lowerBoundIndex <= statusNumber
                    && statusNumber<= upperBoundIndex)
                {
                    result.Add(item);
                }
            }
            return result;

        }

        public void Swap(IEntity first, IEntity second)
        {
            int indexOfFirst = this.items.IndexOf(first);
            ValidateEntity(indexOfFirst);
            int indexOfSecond = this.items.IndexOf(second);
            ValidateEntity(indexOfSecond);


            var temp = this.items[indexOfFirst];
            this.items[indexOfFirst] = this.items[indexOfSecond];
            this.items[indexOfSecond] = temp;

        }

        public IEntity[] ToArray()
        {
            return items.ToArray();
        }

        public void UpdateAll(BaseEntityStatus oldStatus, BaseEntityStatus newStatus)
        {
            foreach (var item in items)
            {
                if (item.Status == oldStatus)
                {
                    item.Status = newStatus;
                }
            }
        }
        IEnumerator IEnumerable.GetEnumerator()
        {
            return this.GetEnumerator();

        }
        private void ValidateEntity(int index)
        {
            ;
            if (index == -1)
            {
                throw new InvalidOperationException("Entity not found");
            }
        }
    }
}
