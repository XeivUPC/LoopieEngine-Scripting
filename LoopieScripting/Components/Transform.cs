using System;

namespace Loopie
{
	enum Space
	{
		localSpace,
		worldSpace
	}
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

		public Vector3 scale
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

		public void Translate(Vector3 translation, Space space)
		{
			InternalCalls.Transform_Translate(entity.ID, translation, space);
		}
		public void Rotate(Vector3 eulerAngles, Space space)
		{
			InternalCalls.Transform_Rotate(entity.ID, eulerAngles, space);
		}
		public void LookAt(Vector3 target, Vector3 up_vector)
		{
			InternalCalls.Transform_LookAt(entity.ID, target, up_vector);
		}

		public Vector3 forward
		{ get {return Forward()}; }

		public Vector3 back
		{ get {return Back()}; }

		public Vector3 up
		{ get {return Up()}; }

		public Vector3 down
		{ get {return Down()}; }

		public Vector3 left
		{ get {return Left()}; }

		public Vector3 right
		{ get {return Right()}; }

		private Vector3 Forward()
		{
			Vector3 vector = Vector3.Forward;
			InternalCalls.Transform_Forward(entity.ID, vector);
			return vector;
		}
		private Vector3 Back()
		{
            Vector3 vector = -Vector3.Forward;
            InternalCalls.Transform_Back(entity.ID, vector);
            return vector;
        }
		private Vector3 Up()
		{
            Vector3 vector = Vector3.Up;
            InternalCalls.Transform_Up(entity.ID, vector);
            return vector;
        }
		private Vector3 Down()
		{
            Vector3 vector = -Vector3.Up;
            InternalCalls.Transform_Down(entity.ID, vector);
            return vector;
        }
		private Vector3 Left()
		{
            Vector3 vector = -Vector3.Right;
            InternalCalls.Transform_Left(entity.ID, vector);
            return vector;
        }
		private Vector3 Right()
		{
            Vector3 vector = Vector3.Right;
            InternalCalls.Transform_Right(entity.ID, vector);
            return vector;
        }
	}
}
