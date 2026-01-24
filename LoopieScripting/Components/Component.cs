using System;

namespace Loopie
{
    public abstract class Component
    {
        protected Entity entity {  get; internal set; }
    }
}