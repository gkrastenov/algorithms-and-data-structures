namespace _02.DOM.Models
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using _02.DOM.Interfaces;

    public class HtmlElement : IHtmlElement
    {
        // TODO : problems
        public HtmlElement(ElementType type, params IHtmlElement[] children)
        {
            this.Type = type;
            this.Parent = null;
            this.Children = children.ToList();
            this.Attributes = new Dictionary<string, string>();
        }

        public ElementType Type { get; set; }

        public IHtmlElement Parent { get; set; }

        public List<IHtmlElement> Children { get; }

        public Dictionary<string, string> Attributes { get; }
    }
}
