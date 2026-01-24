using System;

namespace Loopie
{
	public class Transform : Component
	{
		public Vector3 position
		{ 
			get { return GetPosition(); }
			set { SetPosition(value); }
		}
		public Vector3 local_position
		{ 
			get { return GetLocalPosition(); }
			set { SetLocalPosition(value); }
		}
		public Vector3 rotation
		{ 
			get { return GetRotation(); }
			set { SetRotation(value); }
		}
		public Vector3 local_rotation
		{ 
			get { return GetLocalRotation(); }
			set { SetLocalRotation(value); }
		}
		public Vector3 local_scale
		{ 
			get { return GetLocalScale(); }
			set { SetLocalScale(value); }
		}

		private Vector3 GetPosition()
		{
			Vector3 position = Vector3.Zero;
			InternalCalls.Transform_GetPosition(entity.ID, out position);
			return position;
		}
		private void SetPosition(Vector3 position)
		{
			InternalCalls.Transform_SetPosition(entity.ID, position);
		}
		private Vector3 GetLocalPosition()
		{
			Vector3 position = Vector3.Zero;
			InternalCalls.Transform_GetLocalPosition(entity.ID, out position);
			return position;
		}
		private void SetLocalPosition(Vector3 position)
		{
			InternalCalls.Transform_SetLocalPosition(entity.ID, position);
		}

		private Vector3 GetRotation()
		{
			Vector3 rotation = Vector3.Zero;
			InternalCalls.Transform_GetRotation(entity.ID, out rotation);
			return rotation;
		}
		private void SetRotation(Vector3 rotation)
		{
			InternalCalls.Transform_SetRotation(entity.ID, rotation);
		}
		private Vector3 GetLocalRotation()
		{
			Vector3 rotation = Vector3.Zero;
			InternalCalls.Transform_GetLocalRotation(entity.ID, out rotation);
			return rotation;
		}
		private void SetLocalRotation(Vector3 rotation)
		{
			InternalCalls.Transform_SetLocalRotation(entity.ID, rotation);
		}

		private Vector3 GetLocalScale()
		{
			Vector3 scale = Vector3.Zero;
			InternalCalls.Transform_GetLocalScale(entity.ID, out scale);
			return scale;
		}
		private void SetLocalScale(Vector3 scale)
		{
			InternalCalls.Transform_SetLocalScale(entity.ID, scale);
		}
	}
}
