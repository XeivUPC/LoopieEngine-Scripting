using System;
using System.Runtime.CompilerServices;

namespace Loopie
{
    public class Entity
    {
        protected Entity() { ID = ""; }
        internal Entity(string id)
        {
            ID = id;
        }
        public readonly string ID;

        //public bool HasComponent<T>() where T: Component, new()
        //{
        //    Type componentType = typeof(T);
        //    return Internal.Entity_HasComponent(ID, componentType);
        //}

        //public T GetComponent<T>() where T : Component, new()
        //{
        //    if (!HasComponent<T>())
        //        return null;
        //    Type componentType = typeof(T);
        //    return Internal.Entity_GetComponent(ID, componentType);
        //}

        //public Entity FindEntityByName(string name)
        //{
        //    ulong entityID = Internal.Entity_FindEntityByName(name);
        //    if (entityID == 0)
        //        return null;

        //    return entityID;
        //}

        //public Entity Instantiate(string )
        //{
        //    ulong entityID = Internal.Entity_FindEntityByName(name);
        //    if (entityID == 0)
        //        return null;

        //    return new Entity(entityID);
        //}
    }
}