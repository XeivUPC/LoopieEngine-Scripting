using System;
using System.Runtime.CompilerServices;

namespace Loopie
{
    public class Entity
    {
        internal Entity(string id)
        {
            ID = id;
            transform = new Transform();
            transform.entity = this;
        }
        public readonly string ID;
        public Transform transform { get; }

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent (ID, componentType);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if (HasComponent<T>())
            {
                T component = new T();
                component.entity = this;
                return component;
            }

            string typeName = typeof(T).FullName;
            object scriptInstance = InternalCalls.Entity_GetScriptInstance(ID, typeName);

            return scriptInstance as T;
        }

        public Entity FindEntityByName(string name)
        {
            string entityID = InternalCalls.Entity_FindEntityByName(name);
            if (entityID == "")
                return null;

            return new Entity(entityID);
        }

        public static Entity Instantiate(string name)
        {
            string instanceId = InternalCalls.Entity_Create(name,null);
            if (instanceId == "")
                return null;

            return new Entity(instanceId);
        }

        public T AddComponent<T>() where T : Component, new()
        {
            string componentType = typeof(T).FullName;
            if(InternalCalls.Entity_AddComponent(ID, componentType))
            {
                T component = new T();
                component.entity = this;
                return component;
            }
            return null;
        }
    }
}