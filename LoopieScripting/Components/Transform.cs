using System;

namespace Loopie
{
    public sealed class Transform
	{
        internal Entity entity;

        public Vector3 position
		{ 
			get { return GetPosition(); }
			set { SetPosition(value); }
		}

		private Vector3 GetPosition()
		{
			Vector3 entityPosition = Vector3.Zero;
			InternalCalls.Transform_GetPosition(entity.ID, out entityPosition);
			return entityPosition;
		}
		private void SetPosition(Vector3 position)
		{
			InternalCalls.Transform_SetPosition(entity.ID, position);
		}

		public Vector3 local_position
		{ 
			get { return GetLocalPosition(); }
			set { SetLocalPosition(value); }
		}

		private Vector3 GetLocalPosition()
		{
			Vector3 entityPosition = Vector3.Zero;
			InternalCalls.Transform_GetLocalPosition(entity.ID, out entityPosition);
			return entityPosition;
		}
		private void SetLocalPosition(Vector3 position)
		{
			InternalCalls.Transform_SetLocalPosition(entity.ID, position);
		}

		public Vector3 rotation
		{ 
			get { return GetRotation(); }
			set { SetRotation(value); }
		}

		private Vector3 GetRotation()
		{
			Vector3 entityPosition = Vector3.Zero;
			InternalCalls.Transform_GetRotation(entity.ID, out entityPosition);
			return entityPosition;
		}
		private void SetRotation(Vector3 position)
		{
			InternalCalls.Transform_SetRotation(entity.ID, position);
		}

		public Vector3 local_rotation
		{ 
			get { return GetLocalRotation(); }
			set { SetLocalRotation(value); }
		}

		private Vector3 GetLocalRotation()
		{
			Vector3 entityPosition = Vector3.Zero;
			InternalCalls.Transform_GetLocalRotation(entity.ID, out entityPosition);
			return entityPosition;
		}
		private void SetLocalRotation(Vector3 position)
		{
			InternalCalls.Transform_SetLocalRotation(entity.ID, position);
		}

		public Vector3 local_scale
		{ 
			get { return GetLocalScale(); }
			set { SetLocalScale(value); }
		}

		private Vector3 GetLocalScale()
		{
			Vector3 entityPosition = Vector3.Zero;
			InternalCalls.Transform_GetLocalScale(entity.ID, out entityPosition);
			return entityPosition;
		}
		private void SetLocalScale(Vector3 position)
		{
			InternalCalls.Transform_SetLocalScale(entity.ID, position);
		}
	}
}
